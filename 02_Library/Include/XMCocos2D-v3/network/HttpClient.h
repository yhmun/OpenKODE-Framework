/* -----------------------------------------------------------------------------------
 *
 *      File            HttpClient.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2011      greathqy.
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __HttpClient_h__
#define __HttpClient_h__

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpClient.h"

namespace network {

/**
 *	@addtogroup Network
 *	@{
 */


/** 
 *	@brief Singleton that handles asynchrounous http requests
 *	Once the request completed, a callback will issued in main thread when it provided during make request
 */
class HttpClient : public cocos2d::Object
{
	public :

		/** Return the shared instance **/
		static HttpClient*		getInstance ( KDvoid );
    
		/** Relase the shared instance **/
		static KDvoid			destroyInstance ( KDvoid );

		/** Enable cookie support. **/
		KDvoid					enableCookies ( const KDchar* pCookieFile );
        
		/**
		 *	Add a get request to task queue
		 *	@param request a HttpRequest object, which includes url, response callback etc.
						   please make sure request->_requestData is clear before calling "send" here.
		 */
		KDvoid					send ( HttpRequest* pRequest );
  
    
		/**
		 *	Change the connect timeout
		 *	@param value The desired timeout.
		 */
		inline KDvoid			setTimeoutForConnect ( KDint nValue ) { m_nTimeoutForConnect = nValue; }
    
		/**
		 *	Get connect timeout
		 *	@return int
		 */
		inline KDint			getTimeoutForConnect ( KDvoid ) { return m_nTimeoutForConnect; }
    
    
		/**
		 *	Change the download timeout
		 *	@param value
		 */
		inline KDvoid			setTimeoutForRead ( KDint nValue ) { m_nTimeoutForRead = nValue; }
    

		/**
		 *	Get download timeout
		 *	@return int
		 */
		inline KDint			getTimeoutForRead ( KDvoid ) { return m_nTimeoutForRead; }
        
	private :

				 HttpClient ( KDvoid );
		virtual ~HttpClient ( KDvoid );

		KDbool					init ( KDvoid );
    
		/**
		 *	Init pthread mutex, semaphore, and create new thread for http requests
		 *	@return bool
		 */
		KDbool					lazyInitThreadSemphore ( KDvoid );

		/** Poll function called from main thread to dispatch callbacks when http requests finished **/
		KDvoid					dispatchResponseCallbacks ( KDfloat fDelta );
    
	private :

		KDint					m_nTimeoutForConnect;
		KDint					m_nTimeoutForRead;   
};

//	end of Network group
/// @}

}

#endif	// __HttpClient_h__
