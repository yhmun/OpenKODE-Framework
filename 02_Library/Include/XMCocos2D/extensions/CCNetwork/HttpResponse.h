/* --------------------------------------------------------------------------
 *
 *      File            HttpResponse.h
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

#ifndef __HttpResponse_h__
#define __HttpResponse_h__

#include "HttpRequest.h"

NS_CC_BEGIN

/** 
 *	@brief defines the object which users will receive at onHttpCompleted(sender, HttpResponse) callback
 *	Please refer to samples/TestCpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 *	@since v2.0.2
 */
class CCHttpResponse : public CCObject
{
	public :

		/**
		 *	Constructor, it's used by CCHttpClient internal, users don't need to create HttpResponse manually
		 *	@param request the corresponding HttpRequest which leads to this response 
		 */
		CCHttpResponse ( CCHttpRequest* pRequest )
		{
			m_pHttpRequest = pRequest;

			CC_SAFE_RETAIN ( m_pHttpRequest );
	        
			m_bSucceed = KD_FALSE;

			m_aResponseData.clear ( );
			m_aErrorBuffer .clear ( );
		}
	    
		/** 
		 *	Destructor, it will be called in CCHttpClient internal,
		 *	users don't need to desturct HttpResponse object manully 
		 */
		virtual ~CCHttpResponse ( KDvoid )
		{
			CC_SAFE_RELEASE ( m_pHttpRequest );
		}

	public :

		/** Override autorelease method to prevent developers from calling it */
		CCObject*				autorelease ( KDvoid )
		{
			CCAssert ( KD_FALSE, "HttpResponse is used between network thread and ui thread therefore, autorelease is forbidden here" );
			return KD_NULL;
		}
	    
		// getters, will be called by users
	    
		/** 
		 *	Get the corresponding HttpRequest object which leads to this response 
		 *	There's no paired setter for it, coz it's already setted in class constructor
		 */
		inline CCHttpRequest*	getHttpRequest ( KDvoid )
		{
			return m_pHttpRequest;
		}
	        
		/**
		 *	To see if the http reqeust is returned successfully,
		 *	Althrough users can judge if (http return code = 200), we want an easier way
		 *	If this getter returns false, you can call getResponseCode and getErrorBuffer to find more details
		 */
		inline KDbool			isSucceed ( KDvoid )
		{
			return m_bSucceed;
		};
	    
		/** Get the http response raw data */
		inline std::vector<KDchar>*		getResponseData ( KDvoid )
		{
			return &m_aResponseData;
		}

		/**
		 *	Get the http response errorCode
		 *  I know that you want to see http 200 :)
		 */
		inline KDint			getResponseCode ( KDvoid )
		{
			return m_nResponseCode;
		}

		/** Get the rror buffer which will tell you more about the reason why http request failed */
		inline const KDchar*	getErrorBuffer ( KDvoid )
		{
			return m_aErrorBuffer.c_str ( );
		}
	    
		// setters, will be called by CCHttpClient
		// users should avoid invoking these methods
	    
	    
		/** 
		 *	Set if the http request is returned successfully,
		 *	Althrough users can judge if (http code == 200), we want a easier way
		 *	This setter is mainly used in CCHttpClient, users mustn't set it directly
		 */
		inline KDvoid			setSucceed ( KDbool bValue )
		{
			m_bSucceed = bValue;
		};
	    
	    
		/** Set the http response raw buffer, is used by CCHttpClient */
		inline KDvoid			setResponseData ( std::vector<KDchar>* pData )
		{
			m_aResponseData = *pData;
		}
	    
	    
		/** Set the http response errorCode */
		inline KDvoid			setResponseCode ( KDint nValue )
		{
			m_nResponseCode = nValue;
		}
	    
	    
		/** Set the error buffer which will tell you more the reason why http request failed
		 */
		inline KDvoid			setErrorBuffer ( const KDchar* pValue )
		{
			m_aErrorBuffer.clear ( );
			m_aErrorBuffer.assign ( pValue );
		};
	    
	protected :
	    
		// properties
		CCHttpRequest*			m_pHttpRequest;		/// the corresponding HttpRequest pointer who leads to this response 
		KDbool					m_bSucceed;			/// to indecate if the http reqeust is successful simply
		std::vector<KDchar>		m_aResponseData;	/// the returned raw data. You can also dump it as a string
		KDint					m_nResponseCode;    /// the status code returned from libcurl, e.g. 200, 404
		std::string				m_aErrorBuffer;		/// if _responseCode != 200, please read m_aErrorBuffer to find the reason     
};

NS_CC_END

#endif	// HttpResponse
