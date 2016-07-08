/* -----------------------------------------------------------------------------------
 *
 *      File            CCNotificationCenter.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      Erawppa
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCNotificationCenter_h__
#define __CCNotificationCenter_h__

#include "../cocoa/CCObject.h"
#include "../cocoa/CCArray.h"
#include <string>

NS_CC_BEGIN 

class CCNotificationCenter : public CCObject
{
	protected :

		/** CCNotificationCenter constructor */
		CCNotificationCenter ( KDvoid );

		/** CCNotificationCenter destructor */
		virtual ~CCNotificationCenter ( KDvoid );
	    
	public :

		/** Gets the single instance of CCNotificationCenter. */
		static   CCNotificationCenter*		sharedNotificationCenter ( KDvoid );

		/** Destroys the single instance of CCNotificationCenter. */
		static   KDvoid		purgeNotificationCenter ( KDvoid );

	public :

		/** 
		 *	@brief Adds an observer for the specified target.
		 *  @param target The target which wants to observe notification events.
		 *  @param selector The callback function which will be invoked when the specified notification event was posted.
		 *  @param name The name of this notification.
		 *  @param obj The extra parameter which will be passed to the callback function.
		 */
		KDvoid				addObserver ( CCObject* pTarget, SEL_CallFuncO pSelector, const KDchar* szName, CCObject* pObject );
	    
		/**
		 *	@brief Removes the observer by the specified target and name.
		 *  @param target The target of this notification.
		 *  @param name The name of this notification. 
		 */
		KDvoid				removeObserver ( CCObject* pTarget, const KDchar* szName );
	    
		/** 
		 *	@brief Removes all notifications registered by this target
		 *  @param target The target of this notification.
		 *  @returns the number of observers removed
		 */
		KDint				removeAllObservers ( CCObject* pTarget );

		/**
		 *	@brief Registers one hander for script binding.
		 *  @note Only supports Lua Binding now.
		 *  @param handler The lua handler.
		 */
		KDvoid				registerScriptObserver ( CCObject* pTarget, KDint nHandler, const KDchar* szName );

		/** Unregisters script observer */
		KDvoid				unregisterScriptObserver ( CCObject* pTarget, const KDchar* szName );

		/**
		 *	@brief Posts one notification event by name.
		 *  @param name The name of this notification.
		 */
		KDvoid				postNotification ( const KDchar* szName );

		/**
		 *	@brief Posts one notification event by name.
		 *  @param name The name of this notification.
		 *  @param object The extra parameter.
		 */
		KDvoid				postNotification ( const KDchar* szName, CCObject* pObject );
	    
		/**
		 *	@brief Gets script handler.
		 *  @note Only supports Lua Binding now.
		 *  @return The script handle.
		 */
		inline  KDint		getScriptHandler ( KDvoid ) { return m_nScriptHandler; };

		/** 
		 *	@brief Gets observer script handler.
		 *  @param name The name of this notification.
		 *  @return The observer script handle.
		 */
		KDint				getObserverHandlerByName ( const KDchar* szName );

	private :

		// Check whether the observer exists by the specified target and name.
		KDbool				observerExisted ( CCObject* pTarget, const KDchar* szName );
	    
	private :

		CCArray*			m_pObservers;
		KDint				m_nScriptHandler;
};

class CCNotificationObserver : public CCObject
{
	public :

		/**
		 *	@brief CCNotificationObserver constructor
		 *  @param target The target which wants to observer notification events.
		 *  @param selector The callback function which will be invoked when the specified notification event was posted.
		 *  @param name The name of this notification.
		 *  @param obj The extra parameter which will be passed to the callback function.
		 */
		CCNotificationObserver ( CCObject* pTarget, SEL_CallFuncO pSelector, const KDchar* szName, CCObject* pObject );

		/** CCNotificationObserver destructor function */
		virtual ~CCNotificationObserver ( KDvoid );      
	    
	public :

		/** Invokes the callback function of this observer */
		KDvoid				performSelector ( CCObject* pObject );

	public :

		CC_PROPERTY_READONLY ( CCObject*	, m_pTarget	 , Target	);
		CC_PROPERTY_READONLY ( SEL_CallFuncO, m_pSelector, Selector );
		CC_PROPERTY_READONLY ( KDchar*		, m_szName	 , Name		);
		CC_PROPERTY_READONLY ( CCObject*	, m_pObject	 , Object	);
		CC_PROPERTY			 ( KDint		, m_nHandler , Handler  );
};

NS_CC_END

#endif //__CCNotificationCenter_h__