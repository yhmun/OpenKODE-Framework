/* --------------------------------------------------------------------------
 *
 *      File            HttpRequest.h
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

#ifndef __HttpRequest_h__
#define __HttpRequest_h__

#include "../../cocoa/CCObject.h"
#include "../../cocoa/CCString.h"

#include <vector>

NS_CC_BEGIN

/** 
 *	@brief defines the object which users must packed for CCHttpClient::send(HttpRequest*) method.
 *	Please refer to samples/TestCpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 *	@since v2.0.2
 */
class CCHttpRequest : public CCObject
{
	public :

		/** Use this enum type as param in setReqeustType(param) */
		typedef enum
		{
			kCCHttpGet		,
			kCCHttpPost		,
			kCCHttpUnkown	,
		} HttpRequestType;
	    
		/** 
		 *	Constructor 
		 *	Because HttpRequest object will be used between UI thead and network thread,
		 *	requestObj->autorelease() is forbidden to avoid crashes in CCAutoreleasePool
		 *	new/retain/release still works, which means you need to release it manually
		 *	Please refer to HttpRequestTest.cpp to find its usage
		 */

		CCHttpRequest ( KDvoid )
		{
			m_eRequestType = kCCHttpUnkown;

			m_sUrl.clear ( );
			m_sTag.clear ( );
			m_aRequestData.clear ( );
			
			m_pTarget   = KD_NULL;
			m_pSelector = KD_NULL;
			m_pUserData = KD_NULL;
		};
	    
		/** Destructor */
		virtual ~CCHttpRequest ( KDvoid )
		{
			CC_SAFE_RELEASE ( m_pTarget );
		};
	    
		/** Override autorelease method to avoid developers to call it */
		CCObject*				autorelease ( KDvoid )
		{
			CCAssert ( KD_FALSE, "HttpResponse is used between network thread and ui thread therefore, autorelease is forbidden here" );
			return KD_NULL;
		}
	            
		// setter/getters for properties
	     
		/** Required field for HttpRequest object before being sent.
			kHttpGet & kHttpPost is currently supported
		 */
		inline KDvoid			setRequestType ( HttpRequestType eType )
		{
			m_eRequestType = eType;
		};

		/** Get back the kHttpGet/Post/... enum value */
		inline HttpRequestType		getRequestType ( KDvoid )
		{
			return m_eRequestType;
		};
	    
		/** Required field for HttpRequest object before being sent. */
		inline KDvoid			setUrl ( const KDchar* szUrl )
		{
			m_sUrl = szUrl;
		};

		/** Get back the setted url */
		inline const KDchar*	getUrl ( KDvoid )
		{
			return m_sUrl.c_str ( );
		};
	    
		/** Option field. You can set your post data here
		 */
		inline KDvoid			setRequestData ( const KDchar* pBuffer, KDuint uLen )
		{
			m_aRequestData.assign ( pBuffer, pBuffer + uLen );
		};

		/** Get the request data pointer back */
		inline KDchar*			getRequestData ( KDvoid )
		{
			return &(m_aRequestData.front());
		}

		/** Get the size of request data back */
		inline KDint			getRequestDataSize ( KDvoid )
		{
			return m_aRequestData.size ( );
		}
	    
		/** Option field. You can set a string tag to identify your request, this tag can be found in HttpResponse->getHttpRequest->getTag()
		 */
		inline KDvoid			setTag ( const KDchar* szTag )
		{
			m_sTag = szTag;
		};

		/**
		 *	Get the string tag back to identify the request. 
		 *	The best practice is to use it in your MyClass::onMyHttpRequestCompleted(sender, HttpResponse*) callback
		 */
		inline const KDchar*	getTag ( KDvoid )
		{
			return m_sTag.c_str ( );
		};
	    
		/** 
		 *	Option field. You can attach a customed data in each request, and get it back in response callback.
		 *	But you need to new/delete the data pointer manully
		 */
		inline KDvoid			setUserData ( KDvoid* pUserData )
		{
			m_pUserData = pUserData;
		};

		/** 
		 *	Get the pre-setted custom data pointer back.
		 *	Don't forget to delete it. HttpClient/HttpResponse/HttpRequest will do nothing with this pointer
		 */
		inline KDvoid*			getUserData ( KDvoid )
		{
			return m_pUserData;
		};
	    
		/**
		 *	Required field. You should set the callback selector function at ack the http request completed
		 */
		inline KDvoid			setResponseCallback ( CCObject* pTarget, SEL_CallFuncND pSelector )
		{
			m_pTarget   = pTarget;
			m_pSelector = pSelector;
	        
			CC_SAFE_RETAIN ( m_pTarget );
		}    

		/** Get the target of callback selector funtion, mainly used by CCHttpClient */
		inline CCObject*		getTarget ( KDvoid )
		{
			return m_pTarget;
		}

		/** Get the selector function pointer, mainly used by CCHttpClient */
		inline SEL_CallFuncND	getSelector ( KDvoid )
		{
			return m_pSelector;
		}

		/** Set any custom headers **/
		inline KDvoid			setHeaders ( std::vector<std::string> aHeaders )
   		{
   			m_aHeaders = aHeaders;
   		}
	   
		/** Get custom headers **/
   		inline std::vector<std::string>		getHeaders ( KDvoid )
   		{
   			return m_aHeaders;
   		}

	protected :

		// properties
		HttpRequestType             m_eRequestType;		/// kHttpRequestGet, kHttpRequestPost or other enums
		std::string                 m_sUrl;				/// target url that this request is sent to
		std::vector<KDchar>         m_aRequestData;		/// used for POST
		std::string                 m_sTag;				/// user defined tag, to identify different requests in response callback
		CCObject*					m_pTarget;			/// callback target of pSelector function
		SEL_CallFuncND				m_pSelector;		/// callback function, e.g. MyLayer::onHttpResponse(CCObject *sender, void *data)
		KDvoid*                     m_pUserData;		/// You can add your customed data here 
		std::vector<std::string>    m_aHeaders;			/// custom http headers
};

NS_CC_END

#endif	// __HttpRequest_h__