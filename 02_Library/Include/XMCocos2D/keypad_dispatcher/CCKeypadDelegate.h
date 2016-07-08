/* -----------------------------------------------------------------------------------
 *
 *      File            CCKeypadDelegate.h
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

#ifndef __CCKeypadDelegate_h__
#define __CCKeypadDelegate_h__

#include "../cocoa/CCObject.h"

NS_CC_BEGIN
    
/**
 * @addtogroup input
 * @{
 */

class CCKeypadDelegate
{
	public :

		virtual KDvoid		keyReleased ( KDint nID ) { CC_UNUSED_PARAM ( nID ); }
		virtual KDvoid		keyPressed  ( KDint nID ) { CC_UNUSED_PARAM ( nID ); }

		// The back key clicked
		virtual KDvoid		keyBackClicked ( KDvoid ) { }

		// The menu key clicked. only available on wophone & android
		virtual KDvoid		keyMenuClicked ( KDvoid ) { }
};

/**
 *	@brief
 *	CCKeypadHandler
 *	Object than contains the CCKeypadDelegate.
 */
class CCKeypadHandler : public CCObject
{
	public :

		virtual ~CCKeypadHandler ( KDvoid );

		/** allocates a CCKeypadHandler with a delegate */
		static   CCKeypadHandler*	create ( CCKeypadDelegate* pDelegate );

	public :

		/** initializes a CCKeypadHandler with a delegate */
		virtual KDbool			initWithDelegate ( CCKeypadDelegate* pDelegate );

		/** delegate */
		CCKeypadDelegate*		getDelegate ( KDvoid );
		KDvoid					setDelegate ( CCKeypadDelegate* pDelegate );		

	protected :

		CCKeypadDelegate*		m_pDelegate;
};

// end of input group
/// @} 

NS_CC_END

#endif // __CCKeypadDelegate_h__
