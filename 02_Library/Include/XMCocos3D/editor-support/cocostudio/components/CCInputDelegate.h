/* -----------------------------------------------------------------------------------
 *
 *      File            CCInputDelegate.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __CCInputDelegate_h__
#define __CCInputDelegate_h__

#include "../../../2d/event_dispatcher/CCTouch.h"
#include "../../../2d/event_dispatcher/CCEventAcceleration.h"
#include "../../../2d/event_dispatcher/CCEventKeyboard.h"
#include "../../../2d/event_dispatcher/CCEventListener.h"

#include <vector>

namespace cocostudio {

class InputDelegate
{
	protected :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		InputDelegate ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~InputDelegate ( KDvoid );
    
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool							isTouchEnabled ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid							setTouchEnabled ( KDbool bValue );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool							isAccelerometerEnabled ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid							setAccelerometerEnabled ( KDbool bValue );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool							isKeypadEnabled ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid							setKeypadEnabled ( KDbool bValue );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid							setTouchMode ( cocos2d::Touch::DispatchMode eMode );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual cocos2d::Touch::DispatchMode	getTouchMode ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid							setTouchPriority ( KDint nPriority );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDint							getTouchPriority ( KDvoid ) const;

		virtual KDvoid							onAcceleration ( cocos2d::Acceleration* pAcc, cocos2d::Event* pEvent ) { };

		virtual KDvoid							onKeyPressed  ( cocos2d::EventKeyboard::KeyCode eKeyCode, cocos2d::Event* pEvent ) { };
		virtual KDvoid							onKeyReleased ( cocos2d::EventKeyboard::KeyCode eKeyCode, cocos2d::Event* pEvent ) { };
    
		virtual KDbool							onTouchBegan ( cocos2d::Touch* pTouch, cocos2d::Event* pEvent );
		virtual KDvoid							onTouchMoved ( cocos2d::Touch* pTouch, cocos2d::Event* pEvent );
		virtual KDvoid							onTouchEnded ( cocos2d::Touch* pTouch, cocos2d::Event* pEvent );
		virtual KDvoid							onTouchCancelled ( cocos2d::Touch* pTouch, cocos2d::Event* pEvent );

		virtual KDvoid							onTouchesBegan ( const std::vector<cocos2d::Touch*>& aTouches, cocos2d::Event* pEvent );
		virtual KDvoid							onTouchesMoved ( const std::vector<cocos2d::Touch*>& aTouches, cocos2d::Event* pEvent );
		virtual KDvoid							onTouchesEnded ( const std::vector<cocos2d::Touch*>& aTouches, cocos2d::Event* pEvent );
		virtual KDvoid							onTouchesCancelled ( const std::vector<cocos2d::Touch*>& aTouches, cocos2d::Event* pEvent );

	protected :

		KDbool									m_bTouchEnabled;
		cocos2d::EventListener*					m_pTouchListener;
		KDbool									m_bAccelerometerEnabled;
		cocos2d::EventListener*					m_pAccelerometerListener;
		KDbool									m_bKeypadEnabled;
		cocos2d::EventListener*					m_pKeyboardListener;

	private :

		KDint									m_nTouchPriority;
		cocos2d::Touch::DispatchMode			m_eTouchMode;
};

}

#endif  // __CCInputDelegate_h__
