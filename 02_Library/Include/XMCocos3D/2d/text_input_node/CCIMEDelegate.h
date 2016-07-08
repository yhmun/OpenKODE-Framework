/* -----------------------------------------------------------------------------------
 *
 *      File            CCIMEDelegate.h
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

#ifndef __CCIMEDelegate_h__
#define __CCIMEDelegate_h__

#include "../../base/CCGeometry.h"

NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */

typedef struct
{
    Rect		begin;              // the soft keyboard rectangle when animation begins
    Rect		end;                // the soft keyboard rectangle when animation ends
    KDfloat		duration;           // the soft keyboard animation duration
} IMEKeyboardNotificationInfo;

/**
 *	@brief    Input method editor delegate.
 */
class CC_DLL IMEDelegate
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~IMEDelegate ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool			attachWithIME ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool			detachWithIME ( KDvoid );

	protected :

		friend class IMEDispatcher;

		/**
		 *	@brief    Decide if the delegate instance is ready to receive an IME message.
		 *
		 *	Called by IMEDispatcher.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool			canAttachWithIME ( KDvoid ) { return false; }
		
		/**
		 *	@brief    When the delegate detaches from the IME, this method is called by IMEDispatcher.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			didAttachWithIME ( KDvoid ) { }

		/**
		 *	@brief    Decide if the delegate instance can stop receiving IME messages.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool			canDetachWithIME ( KDvoid ) { return false; }

		/**
		 *	@brief    When the delegate detaches from the IME, this method is called by IMEDispatcher.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			didDetachWithIME ( KDvoid ) { }

		/**
		 *	@brief    Called by IMEDispatcher when text input received from the IME.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			insertText ( const KDchar* pText, KDint nLen ) { CC_UNUSED_PARAM ( pText ); CC_UNUSED_PARAM ( nLen ); }

		/**
		 *	@brief    Called by IMEDispatcher after the user clicks the backward key.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			deleteBackward ( KDvoid ) { }

		/**
		 *	@brief    Called by IMEDispatcher for text stored in delegate.
		 *  @js NA
		 *  @lua NA
		 */
		virtual const KDchar*	getContentText() { return 0; }

		//////////////////////////////////////////////////////////////////////////
		// keyboard show/hide notification
		//////////////////////////////////////////////////////////////////////////
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			keyboardWillShow ( IMEKeyboardNotificationInfo& tInfo )   { CC_UNUSED_PARAM ( tInfo ); }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			keyboardDidShow ( IMEKeyboardNotificationInfo& tInfo )    { CC_UNUSED_PARAM ( tInfo ); }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			keyboardWillHide ( IMEKeyboardNotificationInfo& tInfo )   { CC_UNUSED_PARAM ( tInfo ); }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			keyboardDidHide ( IMEKeyboardNotificationInfo& tInfo )    { CC_UNUSED_PARAM ( tInfo ); }

	protected :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		IMEDelegate ( KDvoid );
};

//	end of input group
/// @}

NS_CC_END

#endif	// __CCIMEDelegate_h__
