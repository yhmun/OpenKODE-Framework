/* -----------------------------------------------------------------------------------
 *
 *      File            CCEventDispatcher.h
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

#ifndef __CCEventDispatcher_h__
#define __CCEventDispatcher_h__

#include "CCEventDelegate.h"
#include "../cocoa/CCArray.h"

NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */

class CCEventDispatcher : public CCObject
{
	public :
				 CCEventDispatcher ( KDvoid );
		virtual ~CCEventDispatcher ( KDvoid );

	public :

		KDvoid					addDelegate ( CCEventDelegate* pDelegate );

		KDvoid					removeDelegate ( CCEventDelegate* pDelegate );

		KDvoid					forceAddDelegate ( CCEventDelegate* pDelegate );
		
		KDvoid					forceRemoveDelegate ( CCEventDelegate* pDelegate );

		KDbool					dispatchEvent ( const KDEvent* pEvent );

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

#endif // __CCEventDispatcher_h__
