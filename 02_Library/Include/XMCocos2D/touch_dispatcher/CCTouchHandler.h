/* -----------------------------------------------------------------------------------
 *
 *      File            CCTouchHandler.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Valentin Milea
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

#ifndef __CCTouchHandler_h__
#define __CCTouchHandler_h__

#include "CCTouchDispatcher.h"
#include "../cocoa/CCObject.h"
#include "../cocoa/CCSet.h"

NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */

/**
 *	CCTouchHandler
 *	Object than contains the delegate and priority of the event handler.
 */
class CCTouchHandler : public CCObject
{
	public :

		virtual ~CCTouchHandler ( KDvoid );

		/** allocates a TouchHandler with a delegate and a priority */
		static   CCTouchHandler*	create ( CCTouchDelegate* pDelegate, KDint nPriority );

	public :

		/** initializes a TouchHandler with a delegate and a priority */
		virtual KDbool				initWithDelegate ( CCTouchDelegate* pDelegate, KDint nPriority );

		/** delegate */
		CCTouchDelegate*			getDelegate ( KDvoid );
		KDvoid						setDelegate ( CCTouchDelegate* pDelegate );

		/** priority */
		KDint						getPriority ( KDvoid );
		KDvoid						setPriority ( KDint nPriority );

		/** enabled selectors */
		KDint						getEnabledSelectors ( KDvoid );
		KDvoid						setEnalbedSelectors ( KDint nValue );	

	protected :

		CCTouchDelegate*			m_pDelegate;
		KDint						m_nPriority;
		KDint						m_nEnabledSelectors;
};

/**
 *	CCStandardTouchHandler
 *	It forwards each event to the delegate.
 */
class CCStandardTouchHandler : public CCTouchHandler
{
	public :

		/** allocates a TouchHandler with a delegate and a priority */
		static CCStandardTouchHandler*		create ( CCTouchDelegate* pDelegate, KDint nPriority );

	public :

		/** initializes a TouchHandler with a delegate and a priority */
		virtual KDbool				initWithDelegate ( CCTouchDelegate* pDelegate, KDint nPriority );
};

/**
 *	CCTargetedTouchHandler
 *	Object than contains the claimed touches and if it swallows touches.
 *	Used internally by TouchDispatcher
 */
class CCTargetedTouchHandler : public CCTouchHandler
{
	public : 

		virtual ~CCTargetedTouchHandler ( KDvoid );

		/** allocates a TargetedTouchHandler with a delegate, a priority and whether or not it swallows touches or not */
		static   CCTargetedTouchHandler*	create ( CCTouchDelegate* pDelegate, KDint nPriority, KDbool bSwallow );

	public :

		/** initializes a TargetedTouchHandler with a delegate, a priority and whether or not it swallows touches or not */
		virtual KDbool				initWithDelegate ( CCTouchDelegate* pDelegate, KDint nPriority, KDbool bSwallow );

		/** whether or not the touches are swallowed */
		KDbool						 isSwallowsTouches ( KDvoid );
		KDvoid						setSwallowsTouches ( KDbool bSwallowsTouches );

		/** MutableSet that contains the claimed touches */
		CCSet*						getClaimedTouches ( KDvoid );		
	
	protected :

		KDbool						m_bSwallowsTouches;
		CCSet*						m_pClaimedTouches;
};

// end of input group
/// @}

NS_CC_END

#endif // __CCTouchHandler_h__
