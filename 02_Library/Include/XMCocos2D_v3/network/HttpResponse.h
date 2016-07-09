/* -----------------------------------------------------------------------------------
 *
 *      File            HttpResponse.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2012 cocos2d-x.org
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

#ifndef __HttpResponse_h__
#define __HttpResponse_h__

#include "network/HttpRequest.h"

namespace network {

/** 
 *	@brief defines the object which users will receive at onHttpCompleted(sender, HttpResponse) callback
 *	Please refer to samples/TestCpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 *	@since v2.0.2
 */
class HttpResponse : public cocos2d::Object
{
	public :

		/**
		 *	Constructor, it's used by HttpClient internal, users don't need to create HttpResponse manually
		 *	@param request the corresponding HttpRequest which leads to this response 
		 */
		HttpResponse ( HttpRequest* pRequest )
		{
			m_pHttpRequest = pRequest;
			if ( m_pHttpRequest )
			{
				m_pHttpRequest->retain ( );
			}
        
			m_bSucceed = false;
			m_aResponseData.clear ( );
			m_aErrorBuffer.clear ( );
		}
    
		/** 
		 *	Destructor, it will be called in HttpClient internal,
		 *	users don't need to desturct HttpResponse object manully 
		 */
		virtual ~HttpResponse ( KDvoid )
		{
			if ( m_pHttpRequest )
			{
				m_pHttpRequest->release ( );
			}
		}
    
		/** Override autorelease method to prevent developers from calling it */
		cocos2d::Object*				autorelease ( KDvoid )
		{
			CCASSERT ( false, "HttpResponse is used between network thread and ui thread therefore, autorelease is forbidden here" );
			return KD_NULL;
		}
    
		// getters, will be called by users
    
		/** 
		 *	Get the corresponding HttpRequest object which leads to this response 
		 *	There's no paired setter for it, coz it's already setted in class constructor
		 */
		inline HttpRequest*				getHttpRequest ( KDvoid )
		{
			return m_pHttpRequest;
		}
        
		/**
		 *	To see if the http reqeust is returned successfully,
		 *	Althrough users can judge if (http return code = 200), we want an easier way
		 *	If this getter returns false, you can call getResponseCode and getErrorBuffer to find more details
		 */
		inline KDbool					isSucceed ( KDvoid )
		{
			return m_bSucceed;
		};
    
		/** Get the http response raw data */
		inline std::vector<KDchar>*		getResponseData ( KDvoid )
		{
			return &m_aResponseData;
		}
    
		/** get the Rawheader **/
		inline std::vector<KDchar>*		getResponseHeader ( KDvoid )
		{
			return &m_aResponseHeader;
		}

		/** Get the http response errorCode
		 *  I know that you want to see http 200 :)
		 */
		inline KDint					getResponseCode ( KDvoid )
		{
			return m_nResponseCode;
		}

		/** 
		 *	Get the rror buffer which will tell you more about the reason why http request failed
		 */
		inline const KDchar*			getErrorBuffer ( KDvoid )
		{
			return m_aErrorBuffer.c_str ( );
		}
    
		// setters, will be called by HttpClient
		// users should avoid invoking these methods    
    
		/**
		 *	Set if the http request is returned successfully,
		 *	Althrough users can judge if (http code == 200), we want a easier way
		 *	This setter is mainly used in HttpClient, users mustn't set it directly
		 */
		inline KDvoid					setSucceed ( KDbool bValue )
		{
			m_bSucceed = bValue;
		};    
    
		/**
		 *	Set the http response raw buffer, is used by HttpClient
		 */
		inline KDvoid					setResponseData ( std::vector<KDchar>* pData )
		{
			m_aResponseData = *pData;
		}
    
		/** 
		 *	Set the http response Header raw buffer, is used by HttpClient
		 */
		inline KDvoid					setResponseHeader ( std::vector<KDchar>* pData )
		{
			m_aResponseHeader = *pData;
		}
    
    
		/** 
		 *	Set the http response errorCode
		 */
		inline KDvoid					setResponseCode ( KDint nValue )
		{
			m_nResponseCode = nValue;
		}
    
    
		/** 
		 *	Set the error buffer which will tell you more the reason why http request failed
		 */
		inline KDvoid					setErrorBuffer ( const KDchar* pValue )
		{
			m_aErrorBuffer.clear ( );
			m_aErrorBuffer.assign ( pValue );
		};
    
	protected :

		KDbool							initWithRequest ( HttpRequest* pRequest );
    
		// properties
		HttpRequest*					m_pHttpRequest;			/// the corresponding HttpRequest pointer who leads to this response 
		KDbool							m_bSucceed;				/// to indecate if the http reqeust is successful simply
		std::vector<KDchar>				m_aResponseData;		/// the returned raw data. You can also dump it as a string
		std::vector<KDchar>				m_aResponseHeader;		/// the returned raw header data. You can also dump it as a string
		KDint							m_nResponseCode;		/// the status code returned from libcurl, e.g. 200, 404
		std::string						m_aErrorBuffer;			/// if _responseCode != 200, please read _errorBuffer to find the reason 
    
};

}

#endif	// __HttpResponse_h__
