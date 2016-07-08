/* -----------------------------------------------------------------------------------
 *
 *      File            CCTouchDelegateProtocol.h
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

#ifndef __CCTouchDelegateProtocol_h__
#define __CCTouchDelegateProtocol_h__

#include "../cocoa/CCObject.h"

NS_CC_BEGIN
    
class CCTouch;
class CCEvent;
class CCSet;
class CCTouchDispatcher;

/**
 * @addtogroup input
 * @{
 */

class CCTouchDelegate
{
	public :
				 CCTouchDelegate ( KDvoid ) { }
		virtual ~CCTouchDelegate ( KDvoid ) { }

	public :

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouch ); CC_UNUSED_PARAM ( pEvent ); return KD_FALSE; }
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouch ); CC_UNUSED_PARAM ( pEvent ); }
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouch ); CC_UNUSED_PARAM ( pEvent ); }

 		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouches ); CC_UNUSED_PARAM ( pEvent ); }
 		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouches ); CC_UNUSED_PARAM ( pEvent ); }
 		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouches ); CC_UNUSED_PARAM ( pEvent ); }
};

/**
 *	@brief
 *	Using this type of delegate results in two benefits:
 *		- 1. You don't need to deal with CCSets, the dispatcher does the job of splitting
 *			 them. You get exactly one UITouch per call.
 *		- 2. You can *claim* a UITouch by returning YES in ccTouchBegan. Updates of claimed
 *			 touches are sent only to the delegate(s) that claimed them. So if you get a move/
 *			 ended/canceled update you're sure it's your touch. This frees you from doing a
 *			 lot of checks when doing multi-touch. 
 *
 *	(The name TargetedTouchDelegate relates to updates "targeting" their specific
 *	handler, without bothering the other handlers.)
 *	@since	v0.8
 */
class CCTargetedTouchDelegate : public CCTouchDelegate
{
	public :

		/** 
		 *	Return YES to claim the touch.
 		 *	@since v0
		 */
		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouch ); CC_UNUSED_PARAM ( pEvent ); return KD_FALSE; }
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouch ); CC_UNUSED_PARAM ( pEvent ); }
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouch ); CC_UNUSED_PARAM ( pEvent ); }
};
 
/**
 *	@brief
 *	This type of delegate is the same one used by CocoaTouch. You will receive all the events (Began,Moved,Ended).
 *	@since	v0.8
 */
class CCStandardTouchDelegate : public CCTouchDelegate
{
	public :

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouches ); CC_UNUSED_PARAM ( pEvent ); }
		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouches ); CC_UNUSED_PARAM ( pEvent ); }
		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent ) { CC_UNUSED_PARAM ( pTouches ); CC_UNUSED_PARAM ( pEvent ); }
};

// end of input group
/// @}

NS_CC_END

#endif // __CCTouchDelegateProtocol_h__
