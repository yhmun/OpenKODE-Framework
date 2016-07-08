/* --------------------------------------------------------------------------
 *
 *      File            HttpClient.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      greathqy
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCNetwork/HttpClient.h"
#include "cocoa/CCArray.h"
#include "support/CCDirector.h"
#include "support/CCScheduler.h"
#include "XMCurl/curl.h"

NS_CC_BEGIN

static KDThread*        l_pNetworkThread		= KD_NULL;
static KDThreadMutex*	l_pRequestQueueMutex	= KD_NULL;
static KDThreadMutex*	l_pResponseQueueMutex	= KD_NULL;

static KDThreadMutex*	l_pSleepMutex			= KD_NULL;
static KDThreadCond*	l_pSleepCondition		= KD_NULL;

static KDuint			l_uAsyncRequestCount	= 0;

static KDbool			l_bNeedQuit				= KD_FALSE;
static CCArray*			l_pRequestQueue			= KD_NULL;
static CCArray*			l_pResponseQueue		= KD_NULL;
static CCHttpClient*	l_pHttpClient			= KD_NULL; // pointer to singleton

static KDchar			l_szErrorBuffer [ CURL_ERROR_SIZE ];

typedef KDsize ( *write_callback ) ( KDvoid* pPtr, KDsize uSize, KDsize uCount, KDvoid* pStream );

// Callback function used by libcurl for collect response data
static KDsize  writeData ( KDvoid* pPtr, KDsize uSize, KDsize uCount, KDvoid* pStream )
{
    std::vector<KDchar>*  pRecvBuffer = ( std::vector<KDchar>* ) pStream;
    KDsize  uBytes = uSize * uCount;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    pRecvBuffer->insert ( pRecvBuffer->end ( ), (KDchar*) pPtr, (KDchar*) pPtr + uBytes );
    
    return uBytes;
}

// Prototypes
KDbool configureCURL   ( CURL* pHandle );
KDint  processGetTask  ( CCHttpRequest* pRequest, write_callback pCallback, KDvoid* pStream, KDint* pErrorCode );
KDint  processPostTask ( CCHttpRequest* pRequest, write_callback pCallback, KDvoid* pStream, KDint* pErrorCode );

// Worker thread
static KDvoid*  networkThread ( KDvoid* pData )
{    
    CCHttpRequest*  pRequest = KD_NULL;
    
    while ( 1 ) 
    {        
        if ( l_bNeedQuit )
        {
            break;
        }
        
        // step 1: send http request if the requestQueue isn't empty
        pRequest = KD_NULL;
        
		kdThreadMutexLock ( l_pRequestQueueMutex );	// Get request task from queue

        if ( 0 != l_pRequestQueue->count ( ) )
        {
            pRequest = dynamic_cast<CCHttpRequest*> ( l_pRequestQueue->objectAtIndex ( 0 ) );
            l_pRequestQueue->removeObjectAtIndex ( 0 );  
            // request's refcount = 1 here
        }
		kdThreadMutexUnlock ( l_pRequestQueueMutex );
        
        if ( KD_NULL == pRequest )
        {
			// Wait for http request tasks from main thread
        	kdThreadCondWait ( l_pSleepCondition, l_pSleepMutex );
            continue;
        }
   
        // step 2: libcurl sync access
        
        // Create a HttpResponse object, the default setting is http access failed
        CCHttpResponse*  pResponse = new CCHttpResponse ( pRequest );
        
        // request's refcount = 2 here, it's retained by HttpRespose constructor
        pRequest->release ( );
        // ok, refcount = 1 now, only HttpResponse hold it.
        
        KDint  nResponseCode = -1;
        KDint  nRetValue = 0;

        // Process the request -> get response packet
        switch ( pRequest->getRequestType ( ) )
        {
            case CCHttpRequest::kCCHttpGet :	// HTTP GET
                nRetValue = processGetTask  ( pRequest, writeData, pResponse->getResponseData ( ), &nResponseCode );
                break;
            
            case CCHttpRequest::kCCHttpPost :		// HTTP POST
                nRetValue = processPostTask ( pRequest, writeData, pResponse->getResponseData ( ), &nResponseCode );
                break;
            
            default :
                CCAssert ( KD_TRUE, "CCHttpClient: unkown request type, only GET and POSt are supported" );
                break;
        }
                  
        // write data to HttpResponse
        pResponse->setResponseCode ( nResponseCode );
        
        if ( nRetValue != 0 ) 
        {
            pResponse->setSucceed ( KD_FALSE );
            pResponse->setErrorBuffer ( l_szErrorBuffer );
        }
        else
        {
            pResponse->setSucceed ( KD_TRUE );
        }
        
        // add response packet into queue
        kdThreadMutexLock ( l_pResponseQueueMutex );
        l_pResponseQueue->addObject ( pResponse );
        kdThreadMutexUnlock ( l_pResponseQueueMutex );
        
        // resume dispatcher selector
        CCDirector::sharedDirector ( )->getScheduler ( )->resumeTarget ( CCHttpClient::getInstance ( ) );		
    }
    
    // cleanup: if worker thread received quit signal, clean up un-completed request queue
    kdThreadMutexLock ( l_pRequestQueueMutex );
    l_pRequestQueue->removeAllObjects ( );
    kdThreadMutexUnlock ( l_pRequestQueueMutex );
    l_uAsyncRequestCount -= l_pRequestQueue->count ( );
    
    if ( l_pRequestQueue != KD_NULL ) 
	{        
		kdThreadMutexFree ( l_pRequestQueueMutex );
		kdThreadMutexFree ( l_pResponseQueueMutex );
        
		kdThreadMutexFree ( l_pSleepMutex );
        kdThreadCondFree  ( l_pSleepCondition );

        l_pRequestQueue->release ( );
		l_pRequestQueue = KD_NULL;
        l_pResponseQueue->release ( );	
		l_pResponseQueue = KD_NULL;
    }

	kdThreadExit ( KD_NULL );
    
    return 0;
}

// Configure curl's timeout property
KDbool configureCURL ( CURL* pHandle )
{
    if ( !pHandle )
	{
        return KD_FALSE;
    }
    
    KDint  nCode;
    nCode = curl_easy_setopt ( pHandle, CURLOPT_ERRORBUFFER, l_szErrorBuffer );
    if ( nCode != CURLE_OK ) 
	{
        return KD_FALSE;
    }

    nCode = curl_easy_setopt ( pHandle, CURLOPT_TIMEOUT, CCHttpClient::getInstance ( )->getTimeoutForRead ( ) );
    if ( nCode != CURLE_OK ) 
	{
        return KD_FALSE;
    }

    nCode = curl_easy_setopt ( pHandle, CURLOPT_CONNECTTIMEOUT, CCHttpClient::getInstance ( )->getTimeoutForConnect ( ) );
    if ( nCode != CURLE_OK ) 
	{
        return KD_FALSE;
    }
    
    return KD_TRUE;
}

// Process Get Request
KDint processGetTask ( CCHttpRequest* pRequest, write_callback pCallback, KDvoid* pStream, KDint* pResponseCode )
{
    CURLcode  eCode = CURL_LAST;
    CURL*     pCurl = curl_easy_init ( );
    
    do 
	{
        if ( !configureCURL ( pCurl ) ) 
        {
            break;
        }
        
        // handle custom header data 
        // create curl linked list 
        struct curl_slist*  pHeaders = KD_NULL;

        // get custom header data (if set) 
       	std::vector<std::string>  aHeaders = pRequest->getHeaders ( );
  		if ( !aHeaders.empty ( ) )
  		{      			
    			for ( std::vector<std::string>::iterator it = aHeaders.begin ( ); it != aHeaders.end ( ); it++ )
    			{
					// append custom headers one by one 
      				pHeaders = curl_slist_append ( pHeaders, it->c_str ( ) );
    			}
				// set custom headers for curl 
    			eCode = curl_easy_setopt ( pCurl, CURLOPT_HTTPHEADER, pHeaders );
    			if ( eCode != CURLE_OK ) 
				{
      				break;
    			}
  		}
              
        eCode = curl_easy_setopt ( pCurl, CURLOPT_URL, pRequest->getUrl ( ) );
        if ( eCode != CURLE_OK ) 
        {
            break;
        }
        
        eCode = curl_easy_setopt ( pCurl, CURLOPT_FOLLOWLOCATION, pRequest->getUrl ( ) );
        if ( eCode != CURLE_OK ) 
        {
            break;
        }

        eCode = curl_easy_setopt ( pCurl, CURLOPT_WRITEFUNCTION, pCallback );
        if ( eCode != CURLE_OK ) 
        {
            break;
        }
        
        eCode = curl_easy_setopt ( pCurl, CURLOPT_WRITEDATA, pStream );
        if ( eCode != CURLE_OK ) 
        {
            break;
        }
        
        eCode = curl_easy_perform ( pCurl );
        if ( eCode != CURLE_OK ) 
        {
            break;
        }
        
        // free the linked list for header data 
        curl_slist_free_all ( pHeaders );

        eCode = curl_easy_getinfo ( pCurl, CURLINFO_RESPONSE_CODE, pResponseCode ); 
        if ( eCode != CURLE_OK || *pResponseCode != 200 ) 
        {
            eCode = CURLE_HTTP_RETURNED_ERROR;
        }

    } while ( 0 );
    
    if ( pCurl )
	{
        curl_easy_cleanup ( pCurl );
    }
    
    return ( eCode == CURLE_OK ? 0 : 1 );
}

// Process POST Request
KDint processPostTask ( CCHttpRequest* pRequest, write_callback pCallback, KDvoid* pStream, KDint* pResponseCode )
{
    CURLcode  eCode = CURL_LAST;
    CURL*     pCurl = curl_easy_init ( );
    
    do 
	{
        if ( !configureCURL ( pCurl ) )
		{
            break;
        }
        
        // handle custom header data 
        // create curl linked list 
        struct curl_slist*  pHeaders = KD_NULL;
        // get custom header data (if set) 
  		std::vector<std::string>  aHeaders = pRequest->getHeaders ( );
  		if ( !aHeaders.empty ( ) )
  		{      			
    		for ( std::vector<std::string>::iterator it = aHeaders.begin ( ); it != aHeaders.end ( ); it++ )
    		{
				// append custom headers one by one 
      			pHeaders = curl_slist_append ( pHeaders, it->c_str ( ) );
    		}
			
			// set custom headers for curl 
    		eCode = curl_easy_setopt ( pCurl, CURLOPT_HTTPHEADER, pHeaders );
    		if ( eCode != CURLE_OK )
			{
      			break;
    		}
  		}
              
        eCode = curl_easy_setopt ( pCurl, CURLOPT_URL, pRequest->getUrl ( ) );
        if ( eCode != CURLE_OK ) 
		{
            break;
        }

        eCode = curl_easy_setopt ( pCurl, CURLOPT_WRITEFUNCTION, pCallback );
        if ( eCode != CURLE_OK ) 
		{
            break;
        }

        eCode = curl_easy_setopt ( pCurl, CURLOPT_WRITEDATA, pStream );
        if ( eCode != CURLE_OK ) 
		{
            break;
        }

        eCode = curl_easy_setopt ( pCurl, CURLOPT_POST, 1 );
        if ( eCode != CURLE_OK ) 
		{
            break;
        }

        eCode = curl_easy_setopt ( pCurl, CURLOPT_POSTFIELDS, pRequest->getRequestData ( ) );
        if ( eCode != CURLE_OK ) 
		{
            break;
        }

        eCode = curl_easy_setopt ( pCurl, CURLOPT_POSTFIELDSIZE, pRequest->getRequestDataSize ( ) );
        if ( eCode != CURLE_OK ) 
		{
            break;
        }

        eCode = curl_easy_perform ( pCurl );
        if ( eCode != CURLE_OK ) 
		{
            break;
        }
        
        // free the linked list for header data 
        curl_slist_free_all ( pHeaders );

        eCode = curl_easy_getinfo ( pCurl, CURLINFO_RESPONSE_CODE, pResponseCode ); 
        if ( eCode != CURLE_OK || *pResponseCode != 200 ) 
		{
            eCode = CURLE_HTTP_RETURNED_ERROR;
        }

    } while ( 0 );

    if ( pCurl )
	{
        curl_easy_cleanup ( pCurl );
    }
    
    return ( eCode == CURLE_OK ? 0 : 1 );    
}

// HttpClient implementation
CCHttpClient* CCHttpClient::getInstance ( KDvoid )
{
    if ( l_pHttpClient == KD_NULL ) 
	{
        l_pHttpClient = new CCHttpClient ( );
    }
    
    return l_pHttpClient;
}

KDvoid CCHttpClient::destroyInstance ( KDvoid )
{
    CCAssert ( l_pHttpClient, "" );

    CCDirector::sharedDirector ( )->getScheduler ( )->unscheduleSelector ( schedule_selector ( CCHttpClient::dispatchResponseCallbacks ), l_pHttpClient );
    
	l_pHttpClient->release ( );
}

CCHttpClient::CCHttpClient ( KDvoid )
{
	m_nTimeoutForConnect = 30;
	m_nTimeoutForRead	 = 60;

	CCScheduler*  pScheduler = CCDirector::sharedDirector ( )->getScheduler ( );
    pScheduler->scheduleSelector ( schedule_selector ( CCHttpClient::dispatchResponseCallbacks ), this, 0, KD_FALSE );
    pScheduler->pauseTarget ( this );
}

CCHttpClient::~CCHttpClient ( KDvoid )
{
    l_bNeedQuit = KD_TRUE;
    
    if ( l_pRequestQueue != KD_NULL )
	{
		kdThreadCondSignal ( l_pSleepCondition );
    }
    
    l_pHttpClient = KD_NULL;
}

// Lazy create semaphore & mutex & thread
KDbool CCHttpClient::lazyInitThreadSemphore ( KDvoid )
{
    if ( l_pRequestQueue != KD_NULL )
	{
        return KD_TRUE;
    }
	else 
	{		
        l_pRequestQueue = new CCArray ( );
        l_pRequestQueue->init ( );
        
        l_pResponseQueue = new CCArray ( );
        l_pResponseQueue->init ( );
        
		l_pRequestQueueMutex  = kdThreadMutexCreate ( KD_NULL );
		l_pResponseQueueMutex = kdThreadMutexCreate ( KD_NULL );
        
		l_pSleepMutex = kdThreadMutexCreate ( KD_NULL );
        l_pSleepCondition = kdThreadCondCreate ( KD_NULL );

		l_pNetworkThread = kdThreadCreate ( KD_NULL, networkThread, KD_NULL );
		kdThreadDetach ( l_pNetworkThread );
        
        l_bNeedQuit = KD_FALSE;
    }
    
    return KD_TRUE;
}

// Add a get task to queue
KDvoid CCHttpClient::send ( CCHttpRequest* pRequest )
{    
    if ( KD_FALSE == lazyInitThreadSemphore ( ) ) 
    {
        return;
    }
    
    if ( !pRequest )
    {
        return;
    }
        
    ++l_uAsyncRequestCount;
    
    pRequest->retain ( );
        
    kdThreadMutexLock ( l_pRequestQueueMutex );
    l_pRequestQueue->addObject ( pRequest );
    kdThreadMutexUnlock ( l_pRequestQueueMutex );
    
    // Notify thread start to work
    kdThreadCondSignal ( l_pSleepCondition );
}

// Poll and notify main thread if responses exists in queue
KDvoid CCHttpClient::dispatchResponseCallbacks ( KDfloat fDelta )
{ 
    CCHttpResponse*  pResponse = KD_NULL;
    
	kdThreadMutexLock ( l_pResponseQueueMutex );
    if ( l_pResponseQueue->count ( ) )
    {
        pResponse = dynamic_cast<CCHttpResponse*> ( l_pResponseQueue->objectAtIndex ( 0 ) );
        l_pResponseQueue->removeObjectAtIndex ( 0 );
    }
    kdThreadMutexUnlock ( l_pResponseQueueMutex );
    
    if ( pResponse )
    {
        --l_uAsyncRequestCount;
        
        CCHttpRequest*  pRequest = pResponse->getHttpRequest ( );
        CCObject*        pTarget = pRequest->getTarget ( );
        SEL_CallFuncND pSelector = pRequest->getSelector ( );

        if ( pTarget && pSelector ) 
        {
            ( pTarget->*pSelector ) ( (CCNode*) this, pResponse );
        }
        
        pResponse->release ( );
    }
    
    if ( 0 == l_uAsyncRequestCount ) 
    {
        CCDirector::sharedDirector ( )->getScheduler ( )->pauseTarget ( this );
    }   
}

NS_CC_END


