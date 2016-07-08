/* -----------------------------------------------------------------------------------
 *
 *      File            HttpRequest.h
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

#ifndef __HttpRequest_h__
#define __HttpRequest_h__

#include "../base/CCObject.h"
#include <vector>

namespace network {

class HttpClient;
class HttpResponse;
typedef KDvoid ( cocos2d::Object::*SEL_HttpResponse ) ( HttpClient* pClient, HttpResponse* pResponse );
#define httpresponse_selector( _SELECTOR ) ( network::SEL_HttpResponse ) ( &_SELECTOR )

/** 
 *	@brief defines the object which users must packed for HttpClient::send(HttpRequest*) method.
 *	Please refer to samples/TestCpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 *	@since v2.0.2
 */
class HttpRequest : public cocos2d::Object
{
	public :

		/** Use this enum type as param in setReqeustType(param) */
		enum class Type
		{
			GET,
			POST,
			PUT,
			DELETE,
			UNKNOWN,
		};
    
		/**
		 *	Constructor
		 *	Because HttpRequest object will be used between UI thead and network thread,
		 *	requestObj->autorelease() is forbidden to avoid crashes in AutoreleasePool
		 *	new/retain/release still works, which means you need to release it manually
		 *	Please refer to HttpRequestTest.cpp to find its usage
		 */
		HttpRequest ( KDvoid )
		{
			m_eRequestType = Type::UNKNOWN;
			m_sUrl.clear();
			m_aRequestData.clear ( );
			m_sTag.clear();
			m_pTarget   = KD_NULL;
			m_pSelector = KD_NULL;
			m_pUserData = KD_NULL;
		}
    
		/** Destructor */
		virtual ~HttpRequest ( KDvoid )
		{
			if ( m_pTarget )
			{
				m_pTarget->release ( );
			}
		}
    
		/** Override autorelease method to avoid developers to call it */
		cocos2d::Object*				autorelease ( KDvoid )
		{
			CCASSERT ( false, "HttpResponse is used between network thread and ui thread therefore, autorelease is forbidden here" );
			return KD_NULL;
		}
            
		// setter/getters for properties
     
		/** 
		 *	Required field for HttpRequest object before being sent.
		 *	kHttpGet & kHttpPost is currently supported
		 */
		inline KDvoid					setRequestType ( Type eType )
		{
			m_eRequestType = eType;
		};

		/** Get back the kHttpGet/Post/... enum value */
		inline Type						getRequestType ( KDvoid )
		{
			return m_eRequestType;
		};
    
		/**
		 *	Required field for HttpRequest object before being sent.
		 */
		inline KDvoid					setUrl ( const KDchar* pUrl )
		{
			m_sUrl = pUrl;
		};

		/** Get back the setted url */
		inline const KDchar*			getUrl ( KDvoid )
		{
			return m_sUrl.c_str ( );
		};
    
		/**
		 *	Option field. You can set your post data here
		 */
		inline KDvoid					setRequestData ( const KDchar* pBuffer, KDuint uLen )
		{
			m_aRequestData.assign ( pBuffer, pBuffer + uLen );
		};

		/** Get the request data pointer back */
		inline KDchar*					getRequestData ( KDvoid )
		{
			if ( m_aRequestData.size ( ) != 0 )
			{
				return &( m_aRequestData.front ( ) );
			}

			return nullptr;
		}

		/** Get the size of request data back */
		inline KDint					getRequestDataSize ( KDvoid )
		{
			return m_aRequestData.size ( );
		}
    
		/** 
		 *	Option field. You can set a string tag to identify your request, this tag can be found in HttpResponse->getHttpRequest->getTag()
		 */
		inline KDvoid					setTag ( const KDchar* pTag )
		{
			m_sTag = pTag;
		};

		/** 
		 *	Get the string tag back to identify the request. 
		 *	The best practice is to use it in your MyClass::onMyHttpRequestCompleted(sender, HttpResponse*) callback
		 */
		inline const KDchar*			getTag ( KDvoid )
		{
			return m_sTag.c_str ( );
		};
    
		/** 
		 *	Option field. You can attach a customed data in each request, and get it back in response callback.
		 *	But you need to new/delete the data pointer manully
		 */
		inline KDvoid					setUserData ( KDvoid* pUserData )
		{
			m_pUserData = pUserData;
		};

		/**
		 *	Get the pre-setted custom data pointer back.
		 *	Don't forget to delete it. HttpClient/HttpResponse/HttpRequest will do nothing with this pointer
		 */
		inline KDvoid*					getUserData ( KDvoid )
		{
			return m_pUserData;
		};   

		inline KDvoid					setResponseCallback ( cocos2d::Object* pTarget, SEL_HttpResponse pSelector )
		{
			m_pTarget = pTarget;
			m_pSelector = pSelector;
        
			if ( m_pTarget )
			{
				m_pTarget->retain ( );
			}
		}    
		/** Get the target of callback selector funtion, mainly used by HttpClient */
		inline cocos2d::Object*			getTarget ( KDvoid )
		{
			return m_pTarget;
		}

		/* This sub class is just for migration SEL_CallFuncND to SEL_HttpResponse, 
		   someday this way will be removed */
		class _prxy
		{
			public :

				 _prxy ( SEL_HttpResponse pCb ) :m_pCb ( pCb ) { }
				~_prxy ( KDvoid ) { }

				operator SEL_HttpResponse ( KDvoid ) const { return m_pCb; }

			protected :

				SEL_HttpResponse		m_pCb;
		};
    
		/** Get the selector function pointer, mainly used by HttpClient */
		inline _prxy					getSelector ( KDvoid )
		{
			return _prxy ( m_pSelector );
		}
    
		/** Set any custom headers **/
		inline KDvoid					setHeaders ( std::vector<std::string> aHeaders )
   		{
   			m_aHeaders = aHeaders;
   		}
   
		/** Get custom headers **/
   		inline std::vector<std::string>	getHeaders ( KDvoid )
   		{
   			return m_aHeaders;
   		}
    
	protected :

		// properties
		Type							m_eRequestType;			/// kHttpRequestGet, kHttpRequestPost or other enums
		std::string						m_sUrl;					/// target url that this request is sent to
		std::vector<KDchar>				m_aRequestData;			/// used for POST
		std::string						m_sTag;					/// user defined tag, to identify different requests in response callback
		cocos2d::Object*				m_pTarget;				/// callback target of pSelector function
		SEL_HttpResponse				m_pSelector;			/// callback function, e.g. MyLayer::onHttpResponse(HttpClient *sender, HttpResponse * response)
		KDvoid*							m_pUserData;			/// You can add your customed data here 
		std::vector<std::string>		m_aHeaders;				/// custom http headers
};

}

#endif	// __HttpRequest_h__
