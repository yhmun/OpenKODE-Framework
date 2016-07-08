/* --------------------------------------------------------------------------
 *
 *      File            HttpClient.h
 *      Author          Young-Hwan Mun
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

#ifndef __HttpClient_h__
#define __HttpClient_h__

#include "HttpRequest.h"
#include "HttpResponse.h"

NS_CC_BEGIN

/**
 * @addtogroup Network
 * @{
 */


/**
 *	@brief Singleton that handles asynchrounous http requests
 *	Once the request completed, a callback will issued in main thread when it provided during make request
 */
class CCHttpClient : public CCObject
{
	private :
				 CCHttpClient ( KDvoid );
		virtual ~CCHttpClient ( KDvoid );

	public :

		/** Return the shared instance **/
		static CCHttpClient*	getInstance ( KDvoid );
	    
		/** Relase the shared instance **/
		static  KDvoid			destroyInstance ( KDvoid );	       

	public :

		/**
		 *	Add a get request to task queue
		 *	@param  request a CCHttpRequest object, which includes url, response callback etc.
		 *			please make sure request->_requestData is clear before calling "send" here.
		 *	@return NULL
		 */
		KDvoid				send ( CCHttpRequest* pRequest );
	  	    
		/**
		 *	Change the connect timeout
		 *	@param timeout 
		 *	@return NULL
		 */
		inline KDvoid		setTimeoutForConnect ( KDint nValue )
		{
			m_nTimeoutForConnect = nValue;
		};
	    
		/**
		 *	Get connect timeout
		 *	@return int
		 *
		 */
		inline KDint		getTimeoutForConnect ( KDvoid )
		{
			return m_nTimeoutForConnect;
		}
	    
	    
		/**
		 *	Change the download timeout
		 *	@param value
		 *	@return NULL
		 */
		inline KDvoid		setTimeoutForRead ( KDint nValue )
		{
			m_nTimeoutForRead = nValue;
		};
	    

		/**
		 *	Get download timeout
		 *	@return int
		 */
		inline KDint		getTimeoutForRead ( KDvoid )
		{
			return m_nTimeoutForRead;
		};
	        
	private :
	    
		/**
		 *	Init pthread mutex, semaphore, and create new thread for http requests
		 *	@return bool
		 */
		KDbool				lazyInitThreadSemphore ( KDvoid );

		/** Poll function called from main thread to dispatch callbacks when http requests finished **/
		KDvoid				dispatchResponseCallbacks ( KDfloat fDelta );
	    
	private :

		KDint				m_nTimeoutForConnect;
		KDint				m_nTimeoutForRead;
};

// end of Network group
/// @}

NS_CC_END

#endif	// __HttpClient_h__
