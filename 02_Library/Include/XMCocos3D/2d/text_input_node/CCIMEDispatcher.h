/* -----------------------------------------------------------------------------------
 *
 *      File            CCIMEDispatcher.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCIMEDispatcher_h__
#define __CCIMEDispatcher_h__

#include "CCIMEDelegate.h"

NS_CC_BEGIN

/**
 *	@addtogroup input
 *	@{
 */

/**
 *	@brief    Input Method Edit Message Dispatcher.
 */
class CC_DLL IMEDispatcher
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~IMEDispatcher ( KDvoid );

		/**
		 *	@brief Returns the shared IMEDispatcher object for the system.
		 *	@js NA
		 *	@lua NA
		 */
		static IMEDispatcher*	sharedDispatcher ( KDvoid );

//     /**
//      *	@brief Releases all IMEDelegates from the shared dispatcher.
//      */
//     static KDvoid			purgeSharedDispatcher ( KDvoid );

		/**
		 *	@brief Dispatches the input text from IME.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					dispatchInsertText ( const KDchar* pText, KDint nLen );

		/**
		 *	@brief Dispatches the delete-backward operation.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					dispatchDeleteBackward ( KDvoid );

		/**
		 *	@brief Get the content text from IMEDelegate, retrieved previously from IME.
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*			getContentText ( KDvoid );

		//////////////////////////////////////////////////////////////////////////
		// dispatch keyboard notification
		//////////////////////////////////////////////////////////////////////////
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					dispatchKeyboardWillShow ( IMEKeyboardNotificationInfo& tInfo );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					dispatchKeyboardDidShow ( IMEKeyboardNotificationInfo& tInfo );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					dispatchKeyboardWillHide ( IMEKeyboardNotificationInfo& tInfo );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					dispatchKeyboardDidHide ( IMEKeyboardNotificationInfo& tInfo );

	protected :

		friend class IMEDelegate;

		/**
		 *	@brief Add delegate to receive IME messages.
		 */
		KDvoid					addDelegate ( IMEDelegate* pDelegate );

		/**
		 *	@brief Attach the pDelegate to the IME.
		 *	@return If the old delegate can detach from the IME, and the new delegate 
		 *		can attach to the IME, return true, otherwise false.
		 */
		KDbool					attachDelegateWithIME ( IMEDelegate* pDelegate );
		KDbool					detachDelegateWithIME ( IMEDelegate* pDelegate );

		/**
		 *	@brief Remove the delegate from the delegates which receive IME messages.
		 */
		KDvoid					removeDelegate ( IMEDelegate* pDelegate );

	private :

		IMEDispatcher ( KDvoid );
    
		class Impl;
		Impl*					m_pImpl;
};

//  end of input group
/// @}

NS_CC_END

#endif	// __CCIMEDispatcher_h__
