/* -----------------------------------------------------------------------------------
 *
 *      File            CCNotificationCenter.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCNotificationCenter_h__
#define __CCNotificationCenter_h__

#include "../base/CCObject.h"
#include "../base/CCArray.h"

NS_CC_BEGIN

class ScriptHandlerMgr;
class CC_DLL NotificationCenter : public Object
{
    friend class ScriptHandlerMgr;

	public :

		/**
		 *	NotificationCenter constructor
		 *	@js ctor
		 */
		NotificationCenter ( KDvoid );

		/**
		 *	NotificationCenter destructor
		 *	@js NA
		 *	@lua NA
		 */
		~NotificationCenter ( KDvoid );
    
		/** Gets the single instance of NotificationCenter. */
		static NotificationCenter*	getInstance ( KDvoid );

		/** Destroys the single instance of NotificationCenter. */
		static KDvoid				destroyInstance ( KDvoid );

		/**
		 *	@brief Adds an observer for the specified target.
		 *  @param target The target which wants to observe notification events.
		 *  @param selector The callback function which will be invoked when the specified notification event was posted.
		 *  @param name The name of this notification.
		 *  @param sender The object whose notifications the target wants to receive. Only notifications sent by this sender are delivered to the target. NULL means that the sender is not used to decide whether to deliver the notification to target.
		 */
		KDvoid						addObserver ( Object* pTarget, SEL_CallFuncO pSelector, const KDchar* pName, Object* pSender );

		/**
		 *	@brief Removes the observer by the specified target and name.
		 *  @param target The target of this notification.
		 *  @param name The name of this notification. 
		 */
		KDvoid						removeObserver ( Object* pTarget, const KDchar* pName );
    
		/** 
		 *	@brief Removes all notifications registered by this target
		 *  @param target The target of this notification.
		 *  @returns the number of observers removed
		 */
		KDint						removeAllObservers ( Object* pTarget );

		/** 
		 *	@brief Registers one hander for script binding.
		 *  @note Only supports Lua Binding now.
		 *  @param handler The lua handler.
		 */
		KDvoid						registerScriptObserver ( Object* pTarget, KDint nHandler, const KDchar* pName );

		/** Unregisters script observer */
		KDvoid						unregisterScriptObserver ( Object* pTarget, const KDchar* pName );
    
		/**
		 *	@brief Posts one notification event by name.
		 *  @param name The name of this notification.
		 */
		KDvoid						postNotification ( const KDchar* pName );

		/**
		 *	@brief Posts one notification event by name.
		 *  @param name The name of this notification.
		 *  @param sender The object posting the notification. Can be NULL
		 */
		KDvoid						postNotification ( const KDchar* pName, Object* pSender );
    
		/**
		 *	@brief Gets script handler.
		 *  @note Only supports Lua Binding now.
		 *  @return The script handle.
		 */
		inline KDint				getScriptHandler ( KDvoid ) const { return m_nScriptHandler; }
    
		/** 
		 *	@brief Gets observer script handler.
		 *  @param name The name of this notification.
		 *  @return The observer script handle.
		 */
		KDint						getObserverHandlerByName ( const KDchar* pName );

	private :

		// internal functions

		// Check whether the observer exists by the specified target and name.
		KDbool						observerExisted ( Object* pTarget, const KDchar* pName, Object* pSender );
    
		// variables
		//
		Array*						m_pObservers;
		KDint						m_nScriptHandler;
};

class CC_DLL NotificationObserver : public Object
{
	public :

		/** 
		 *	@brief NotificationObserver constructor
		 *  @param target The target which wants to observer notification events.
		 *  @param selector The callback function which will be invoked when the specified notification event was posted.
		 *  @param name The name of this notification.
		 *  @param sender The object whose notifications the target wants to receive. Only notifications sent by this sender are delivered to the target. NULL means that the sender is not used to decide whether to deliver the notification to target.
		 *	@js NA
		 *	@lua NA
		 */
		NotificationObserver ( Object* pTarget, SEL_CallFuncO pSelector, const KDchar* pName, Object* pSender );

		/**
		 *	NotificationObserver destructor function 
		 *	@js NA
		 *	@lua NA
		 */
		~NotificationObserver ( KDvoid );      
    
		/**
		 *	Invokes the callback function of this observer 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						performSelector ( Object* pSender );
    
		// Getters / Setters

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Object*						getTarget ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		SEL_CallFuncO				getSelector ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*				getName ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Object*						getSender ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDint						getHandler ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						setHandler ( KDint nHandler );

	private :

		Object*						m_pTarget;
		SEL_CallFuncO				m_pSelector;
		std::string					m_sName;
		Object*						m_pSender;
		KDint						m_nHandler;
};

NS_CC_END

#endif	// __CCNotificationCenter_h__
