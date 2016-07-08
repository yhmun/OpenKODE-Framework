/* -----------------------------------------------------------------------------------
 *
 *      File            CCKeypadDispatcher.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCKeypadDispatcher_h__
#define __CCKeypadDispatcher_h__

#include "CCKeypadDelegate.h"
#include "../cocoa/CCArray.h"

NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */

/**
 *	@class	CCKeypadDispatcher
 *	@brief	Dispatch the keypad message from the phone
 */
class CCKeypadDispatcher : public CCObject
{
	public :
				 CCKeypadDispatcher ( KDvoid );
		virtual ~CCKeypadDispatcher ( KDvoid );

	public :

		/**
		 *	@brief	add delegate to concern keypad msg
		 */
		KDvoid		addDelegate ( CCKeypadDelegate* pDelegate );

		/**
		 *	@brief	remove the delegate from the delegates who concern keypad msg
		 */
		KDvoid		removeDelegate ( CCKeypadDelegate* pDelegate );

		/**
		 *	@brief	force add the delegate
		 */
		KDvoid		forceAddDelegate ( CCKeypadDelegate* pDelegate );
		
		/**
		 *	@brief	force remove the delegate
		 */
		KDvoid		forceRemoveDelegate ( CCKeypadDelegate* pDelegate );

		/**
		 *	@brief	dispatch the key pad msg
		 */
		KDbool		dispatchKeypadMSG ( KDbool bPressed, KDint nID );

	protected :

		CCArray*				m_pDelegates;
		KDbool					m_bLocked;
		KDbool					m_bToAdd;
		KDbool					m_bToRemove;

		struct _ccCArray*		m_pHandlersToAdd;
		struct _ccCArray*		m_pHandlersToRemove;
};

// end of input group
/// @}

NS_CC_END

#endif // __CCKeypadDispatcher_h__
