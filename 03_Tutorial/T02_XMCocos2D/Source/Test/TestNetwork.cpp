/* --------------------------------------------------------------------------
 *
 *      File            TestNetwork.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "TestNetwork.h"

KDvoid TestNetwork::onEnter ( KDvoid )
{	
	TestBasic::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

    CCLabelTTF*  pLabel = CCLabelTTF::create ( "Http Request Test", "fonts/arial.ttf", 28 );
    pLabel->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy - 150 ) );
    this->addChild ( pLabel, 0 );  
    
    // Get 
    CCMenuItemLabel*  pItemGet = CCMenuItemLabel::create
	(
		CCLabelTTF::create ( "Test Get", "fonts/arial.ttf", 22 ), 
		this, menu_selector ( TestNetwork::onMenuGetTestClicked ) 
	);
    pItemGet->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy - 200 ) );
    
    // Post
    CCMenuItemLabel*  pItemPost = CCMenuItemLabel::create 
	(
		CCLabelTTF::create ( "Test Post", "fonts/arial.ttf", 22 ),
		this, menu_selector ( TestNetwork::onMenuPostTestClicked ) 
	);
    pItemPost->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy - 250 ) );
    
    // Post Binary
    CCMenuItemLabel*  pItemPostBinary = CCMenuItemLabel::create 
	(
		CCLabelTTF::create ( "Test Post Binary", "fonts/arial.ttf", 22 ),
		this, menu_selector ( TestNetwork::onMenuPostBinaryTestClicked )
	);
    pItemPostBinary->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy - 300 ) );
    
	CCMenu*  pMenuRequest = CCMenu::create ( pItemGet, pItemPost, pItemPostBinary, KD_NULL );
    this->addChild ( pMenuRequest );

    // Response Code Label
	m_pLabel = CCLabelTTF::create 
	(
		"HTTP Status Code", "fonts/Marker Felt.ttf", 32, CCSizeZero, kCCTextAlignmentLeft 
	);	
	m_pLabel->setPosition ( ccp ( tLyrSize.cx / 2,  tLyrSize.cy - 350 ) );
	this->addChild ( m_pLabel );
}

KDvoid TestNetwork::onExit ( KDvoid )
{
	CCHttpClient::getInstance ( )->destroyInstance ( );

	TestBasic::onExit ( );
}

KDvoid TestNetwork::onMenuGetTestClicked ( CCObject* pSender )
{
	CCHttpClient*	pHttpClient = CCHttpClient::getInstance ( );
	CCHttpRequest*  pRequest = KD_NULL;

    // test 1    
    pRequest = new CCHttpRequest ( );
    pRequest->setUrl ( "http://just-make-this-request-failed.com" );
    pRequest->setRequestType ( CCHttpRequest::kCCHttpGet );
    pRequest->setResponseCallback ( this, callfuncND_selector ( TestNetwork::onHttpRequestCompleted ) );
    pRequest->setTag ( "GET test1" );
    pHttpClient->send ( pRequest );
    pRequest->release ( );
        
    // test 2
    pRequest = new CCHttpRequest ( );
	// required fields
	pRequest->setUrl ( "http://www.httpbin.org/ip" );
	pRequest->setRequestType ( CCHttpRequest::kCCHttpGet );
	pRequest->setResponseCallback ( this, callfuncND_selector ( TestNetwork::onHttpRequestCompleted ) );
	// optional fields                            
	pRequest->setTag ( "GET test2" );

	pHttpClient->send ( pRequest );

	// don't forget to release it, pair to new
	pRequest->release ( );
    
    
    // test 3   
    pRequest = new CCHttpRequest ( );
	pRequest->setUrl ( "http://www.httpbin.org/get" );
	pRequest->setRequestType ( CCHttpRequest::kCCHttpGet );
	pRequest->setResponseCallback ( this, callfuncND_selector ( TestNetwork::onHttpRequestCompleted ) );
	pRequest->setTag ( "GET test3" );
	pHttpClient->send ( pRequest );
	pRequest->release ( );
            
    // waiting
    m_pLabel->setString ( "waiting..." );
}

KDvoid TestNetwork::onMenuPostTestClicked ( CCObject* pSender )
{
	CCHttpClient*   pHttpClient = CCHttpClient::getInstance ( );
	CCHttpRequest*  pRequest    = KD_NULL;
	const KDchar*   szPostData  = KD_NULL;

    // test 1    
	pRequest = new CCHttpRequest ( );
	pRequest->setUrl ( "http://www.httpbin.org/post" );
	pRequest->setRequestType ( CCHttpRequest::kCCHttpPost );
	pRequest->setResponseCallback ( this, callfuncND_selector ( TestNetwork::onHttpRequestCompleted ) );

	// write the post data
	szPostData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
	pRequest->setRequestData ( szPostData, kdStrlen ( szPostData ) );
	pRequest->setTag  ( "POST test1" );
	pHttpClient->send ( pRequest );
	pRequest->release ( );
    
    // test 2: set Content-Type
	pRequest = new CCHttpRequest ( );
	pRequest->setUrl ( "http://www.httpbin.org/post" );
	pRequest->setRequestType ( CCHttpRequest::kCCHttpPost );
	std::vector<std::string>  aHeaders;
	aHeaders.push_back ( "Content-Type: application/json; charset=utf-8" );
	pRequest->setHeaders ( aHeaders );
	pRequest->setResponseCallback ( this, callfuncND_selector ( TestNetwork::onHttpRequestCompleted ) );

	// write the post data
	szPostData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
	pRequest->setRequestData ( szPostData, kdStrlen ( szPostData ) ); 
	pRequest->setTag  ( "POST test2" );
	pHttpClient->send ( pRequest );
	pRequest->release ( );
        
    // waiting
    m_pLabel->setString ( "waiting..." );
}

KDvoid TestNetwork::onMenuPostBinaryTestClicked	( CCObject* pSender )
{
    CCHttpRequest*  pRequest = new CCHttpRequest ( );
    pRequest->setUrl ( "http://www.httpbin.org/post" );
    pRequest->setRequestType ( CCHttpRequest::kCCHttpPost );
    pRequest->setResponseCallback ( this, callfuncND_selector ( TestNetwork::onHttpRequestCompleted ) );
    
    // write the post data
    KDchar  szPostData [ 22 ] = "binary=hello\0\0cocos2d";  // including \0, the strings after \0 should not be cut in response
    pRequest->setRequestData ( szPostData, 22 ); 
    
    pRequest->setTag ( "POST Binary test" );
    CCHttpClient::getInstance ( )->send ( pRequest );
    pRequest->release ( );
    
    // waiting
    m_pLabel->setString ( "waiting..." );
}

KDvoid TestNetwork::onHttpRequestCompleted ( CCNode* pSender, KDvoid* pData )
{
    CCHttpResponse*  pResponse = (CCHttpResponse*) pData;

    if ( !pResponse )
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if ( 0 != kdStrlen ( pResponse->getHttpRequest ( )->getTag ( ) ) ) 
    {
        CCLOG ( "%s completed", pResponse->getHttpRequest ( )->getTag ( ) );
    }
    
	KDint  nStatusCode = pResponse->getResponseCode ( );
    m_pLabel->setString ( ccszf ( "HTTP Status Code: %d, tag = %s", nStatusCode, pResponse->getHttpRequest ( )->getTag ( ) ) );
    CCLOG ( "response code: %d", nStatusCode );
    
    if ( !pResponse->isSucceed ( ) ) 
    {
        CCLOG ( "response failed" );
        CCLOG ( "error buffer: %s", pResponse->getErrorBuffer ( ) );
        return;
    }
    
    // dump data
    std::vector<KDchar>*  pBuffer = pResponse->getResponseData ( );
	std::string  sOutput = "Http Test, dump data: ";
    for ( KDuint i = 0; i < pBuffer->size ( ); i++ )
    {
		sOutput.push_back ( (*pBuffer)[ i ] );
    }
	CCLOG ( sOutput.c_str ( ) );
}