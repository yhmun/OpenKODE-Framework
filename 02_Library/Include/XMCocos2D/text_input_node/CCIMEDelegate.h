/* -----------------------------------------------------------------------------------
 *
 *      File            CCIMEDelegate.h
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

#ifndef __CCIMEDelegate_h__
#define __CCIMEDelegate_h__

#include "../cocoa/CCGeometry.h"

NS_CC_BEGIN
    
/**
 * @addtogroup input
 * @{
 */

typedef struct
{
    CCRect		begin;                // the soft keyboard rectangle when animation begins
    CCRect		end;                  // the soft keyboard rectangle when animation ends
    KDfloat		duration;             // the soft keyboard animation duration
} CCIMEKeyboardNotificationInfo;

/**
 *	@brief	Input method editor delegate.
 */
class CCIMEDelegate
{
	protected :
		         CCIMEDelegate ( KDvoid );
		virtual ~CCIMEDelegate ( KDvoid );

		friend class CCIMEDispatcher;

	public :
		
		virtual KDbool		attachWithIME ( KDvoid );
		virtual KDbool		detachWithIME ( KDvoid );

	protected :		
		
		/**
		 *	@brief	Decide if the delegate instance is ready to receive an IME message.
		 *
		 *	Called by CCIMEDispatcher.
		 */
		virtual KDbool		canAttachWithIME ( KDvoid ) { return KD_FALSE; }
		
		/**
		 *	@brief	When the delegate detaches from the IME, this method is called by CCIMEDispatcher.
		 */
		virtual KDvoid		didAttachWithIME ( KDvoid ) { }
		
		/**
		 *	@brief	Decide if the delegate instance can stop receiving IME messages.
		 */
		virtual KDbool		canDetachWithIME ( KDvoid ) { return KD_FALSE; }
		
		/**
		 *	@brief	When the delegate detaches from the IME, this method is called by CCIMEDispatcher.
		 */
		virtual KDvoid		didDetachWithIME ( KDvoid ) { }
		
		/**
		 *	@brief	Called by CCIMEDispatcher when text input received from the IME.
		 */
		virtual KDvoid		insertText ( const KDchar* szText, KDsize uLen )
		{
			CC_UNUSED_PARAM ( szText );
			CC_UNUSED_PARAM ( uLen ); 
		}
		
		/**
		 *	@brief	Called by CCIMEDispatcher after the user clicks the backward key.
		 */
		virtual KDvoid		deleteBackward ( KDvoid ) { }

		/**
		 *	@brief	Called by CCIMEDispatcher for text stored in delegate.
		 */
		virtual const KDchar*	getContentText ( KDvoid ) { return 0; }
		
		//////////////////////////////////////////////////////////////////////////
		// keyboard show/hide notification
		//////////////////////////////////////////////////////////////////////////
		virtual KDvoid		keyboardWillShow ( CCIMEKeyboardNotificationInfo& tInfo ) { CC_UNUSED_PARAM ( tInfo ); }
		virtual KDvoid		keyboardDidShow  ( CCIMEKeyboardNotificationInfo& tInfo ) { CC_UNUSED_PARAM ( tInfo ); }
		virtual KDvoid		keyboardWillHide ( CCIMEKeyboardNotificationInfo& tInfo ) { CC_UNUSED_PARAM ( tInfo ); }
		virtual KDvoid		keyboardDidHide  ( CCIMEKeyboardNotificationInfo& tInfo ) { CC_UNUSED_PARAM ( tInfo ); }
};

// end of input group
/// @}

NS_CC_END

#endif // __CCIMEDelegate_h__
