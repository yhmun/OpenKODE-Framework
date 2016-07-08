/* -----------------------------------------------------------------------------------
 *
 *      File            CCTouchDispatcher.h
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

#ifndef __CCTouchDispatcher_h__
#define __CCTouchDispatcher_h__

#include "CCTouchDelegateProtocol.h"
#include "../cocoa/CCArray.h"

NS_CC_BEGIN
    
/**
 * @addtogroup input
 * @{
 */

typedef enum
{
	ccTouchSelectorBeganBit	= 1 << 0,
	ccTouchSelectorMovedBit	= 1 << 1,
	ccTouchSelectorEndedBit	= 1 << 2,
	ccTouchSelectorAllBits	= ( ccTouchSelectorBeganBit | ccTouchSelectorMovedBit | ccTouchSelectorEndedBit ),
} ccTouchSelectorFlag;

enum
{
	CCTOUCHBEGAN,
	CCTOUCHMOVED,
	CCTOUCHENDED,
	ccTouchMax,
};

class CCSet;
class CCEvent;

struct ccTouchHandlerHelperData 
{
	// we only use the type
	KDint	m_nType;
};

class  CCTouchHandler;
struct _ccCArray;

/**
 *	@brief	CCTouchDispatcher.
 *	Singleton that handles all the touch events.
 *	The dispatcher dispatches events to the registered TouchHandlers.
 *	There are 2 different type of touch handlers:
 *		- Standard Touch Handlers
 *		- Targeted Touch Handlers
 *
 *	The Standard Touch Handlers work like the CocoaTouch touch handler: a set of touches is passed to the delegate.
 *	On the other hand, the Targeted Touch Handlers only receive 1 touch at the time, and they can "swallow" touches (avoid the propagation of the event).
 *	
 *	Firstly, the dispatcher sends the received touches to the targeted touches.
 *	These touches can be swallowed by the Targeted Touch Handlers. If there are still remaining touches, then the remaining touches will be sent
 *	to the Standard Touch Handlers.
 *	
 *	@since	v0.8.0
 */
class CCTouchDispatcher : public CCObject
{
	public :
				 CCTouchDispatcher ( KDvoid );
		virtual ~CCTouchDispatcher ( KDvoid );		

	public :

		virtual KDbool		init ( KDvoid );
	
		/** Whether or not the events are going to be dispatched. Default: true */
		KDbool				 isDispatchEvents ( KDvoid );
		KDvoid				setDispatchEvents ( KDbool bDispatchEvents );

		/** 
		 *	Adds a standard touch delegate to the dispatcher's list.
		 *	See StandardTouchDelegate description.
		 *	IMPORTANT: The delegate will be retained.
		 */
		KDvoid				addStandardDelegate ( CCTouchDelegate* pDelegate, KDint nPriority );
		
		/** 
		 *	Adds a targeted touch delegate to the dispatcher's list.
		 *	See TargetedTouchDelegate description.
		 *	IMPORTANT: The delegate will be retained.
		 */
		KDvoid				addTargetedDelegate ( CCTouchDelegate* pDelegate, KDint nPriority, KDbool bSwallowsTouches );

		/** 
		 *	Removes a touch delegate.
		 *	The delegate will be released
		 */
		KDvoid				removeDelegate ( CCTouchDelegate* pDelegate );

		/** Removes all touch delegates, releasing all the delegates */
		KDvoid				removeAllDelegates ( KDvoid );

		KDvoid				setTargetedSwallow ( KDbool bSwallow, CCTouchDelegate* pDelegate );

		/** 
		 *	Changes the priority of a previously added delegate. The lower the number,
		 *	the higher the priority 
		 */
		KDvoid				setPriority ( KDint nPriority, CCTouchDelegate* pDelegate );

		KDvoid				touches ( CCSet* pTouches, CCEvent* pEvent, KDuint uIndex );

		virtual KDvoid		touchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		touchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		touchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		CCTouchHandler*		findHandler ( CCTouchDelegate* pDelegate );

		CCArray*			getTargetedHandlers ( KDvoid );
		CCArray*			getStandardHandlers ( KDvoid );

	protected :

		KDvoid				forceRemoveDelegate ( CCTouchDelegate* pDelegate );
		KDvoid				forceAddHandler ( CCTouchHandler* pHandler, CCArray* pArray );
		KDvoid				forceRemoveAllDelegates ( KDvoid );

		KDvoid				rearrangeHandlers ( CCArray* pArray );

		CCTouchHandler*		findHandler ( CCArray* pArray, CCTouchDelegate* pDelegate );

	protected :

		CCArray*							m_pTargetedHandlers;
		CCArray*							m_pStandardHandlers;

		KDbool								m_bLocked;
		KDbool								m_bToAdd;
		KDbool								m_bToRemove;
		CCArray*							m_pHandlersToAdd;
		struct _ccCArray*					m_pHandlersToRemove;
		KDbool								m_bToQuit;
		KDbool								m_bDispatchEvents;

		// 4, 1 for each type of event
		struct ccTouchHandlerHelperData		m_sHandlerHelperData [ ccTouchMax ];
};

// end of input group
/// @}

NS_CC_END

#endif // __CCTouchDispatcher_h__