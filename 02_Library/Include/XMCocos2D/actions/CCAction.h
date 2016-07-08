/* -----------------------------------------------------------------------------------
 *
 *      File            CCAction.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCAction_h__
#define __CCAction_h__

#include "../cocoa/CCObject.h"
#include "../cocoa/CCGeometry.h"

NS_CC_BEGIN
    
enum 
{
	//!	Default tag
	kCCActionTagInvalid = -1,
};

/**
 * @addtogroup actions
 * @{
 */

/**
 * @brief	Base class for CCAction objects.			
 */
class CCAction : public CCObject 
{
	public :

		         CCAction ( KDvoid );
	    virtual ~CCAction ( KDvoid );

		/** Create an action */
		static CCAction*		create ( KDvoid );

	public :

		const KDchar*			description ( KDvoid );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		//!	return true if the action has finished
		virtual KDbool			isDone ( KDvoid );

		virtual KDvoid			startWithTarget ( CCNode* pTarget );

		/** 
		 *	called after the action has finished. It will set the 'target' to nil.
		 *	IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
		 */
		virtual KDvoid			stop ( KDvoid );

		//!	called every frame with it's delta time. DON'T override unless you know what you are doing.
		virtual KDvoid			step ( KDfloat fDelta );
	
		/** 
		 *	called once per frame. time a value between 0 and 1
		 *
		 *	For example : 
		 *		- 0 means that the action just started
		 *		- 0.5 means that the action is in the middle
		 *		- 1 means that the action is over
		 */
		virtual KDvoid			update ( KDfloat fTime );
		
		inline  CCNode*			getTarget ( KDvoid ) { return m_pTarget; }

		/**
		 *	The action will modify the target properties. 
		 */
		inline  KDvoid			setTarget ( CCNode* pTarget ) { m_pTarget = pTarget; }
		
		inline  CCNode*			getOriginalTarget ( KDvoid ) { return m_pOriginalTarget; } 

		/** 
		 *	Set the original target, since target can be nil.
		 *	Is the target that were used to run the action. Unless you are doing something complex, like CCActionManager, you should NOT call this method.
		 *	The target is 'assigned', it is not 'retained'.
		 *	@since v0.8.2
		*/
		inline  KDvoid			setOriginalTarget ( CCNode* pOriginalTarget ) {	m_pOriginalTarget = pOriginalTarget; }

		inline  KDint			getTag ( KDvoid ) {	return m_nTag; }
		inline  KDvoid			setTag ( KDint nTag ) { m_nTag = nTag; }

	protected :

		CCNode*					m_pOriginalTarget;

		/**		
		 *	The "target".
		 *	The target will be set with the 'startWithTarget' method.
		 *	When the 'stop' method is called, target will be set to nil.
		 *	The target is 'assigned', it is not 'retained'.
		 */
		CCNode*					m_pTarget;

		/** The action tag. An identifier of the action */
		KDint					m_nTag;
};

/** 
 * @brief 
 *	Base class actions that do have a finite time duration.
 *	Possible actions:
 *		- An action with a duration of 0 seconds
 *		- An action with a duration of 35.5 seconds
 *
 *	Infinite time actions are valid
 */
class CCFiniteTimeAction : public CCAction
{
	public :

		CCFiniteTimeAction ( KDvoid );

	public :

		//!	get duration in seconds of the action
		inline  KDfloat			getDuration ( KDvoid ) { return m_fDuration; }

		//! set duration in seconds of the action
		inline  KDvoid			setDuration ( KDfloat fDuration ) { m_fDuration = fDuration; }

		/** returns a reversed action */
		virtual CCFiniteTimeAction*		reverse ( KDvoid );

	protected :

		//! duration in seconds
		KDfloat					m_fDuration;
};

class CCActionInterval;
class CCRepeatForever;

/** 
 * @brief	Changes the speed of an action, making it take longer (speed>1)
 *			or less (speed<1) time.
 *			Useful to simulate 'slow motion' or 'fast forward' effect.
 * @warning This action can't be Sequenceable because it is not an CCIntervalAction
 */
class CCSpeed : public CCAction
{
	public :

		         CCSpeed ( KDvoid );
		virtual ~CCSpeed ( KDvoid );

		/** create the action */
		static CCSpeed*			create ( CCActionInterval* pAction, KDfloat fSpeed );

	public :

		/** initializes the action */
		KDbool					initWithAction ( CCActionInterval* pAction, KDfloat fSpeed );

		inline  KDfloat			getSpeed ( KDvoid ) { return m_fSpeed; }

		/** alter the speed of the inner function in runtime */
		inline  KDvoid			setSpeed ( KDfloat fSpeed ) { m_fSpeed = fSpeed; }		

		virtual CCObject*		copyWithZone ( CCZone* pZone );
		
		virtual KDvoid			startWithTarget ( CCNode* pTarget );
    
		virtual KDvoid			stop ( KDvoid );
    
		virtual KDvoid			step ( KDfloat fDelta );
    
		virtual KDbool			isDone ( KDvoid );

		virtual CCActionInterval*	reverse ( KDvoid );
		
		KDvoid					setInnerAction ( CCActionInterval* pAction );
	
		inline CCActionInterval*	getInnerAction ( KDvoid ) { return m_pInnerAction; }

	protected :

		KDfloat					m_fSpeed;
		CCActionInterval*		m_pInnerAction;
};

/** 
 * @brief CCFollow is an action that "follows" a node.
 * 
 * Eg :
 *	layer->runAction(CCFollow::actionWithTarget(hero));
 *
 *	Instead of using CCCamera as a "follower", use this action instead.
 * @since v0.99.2
*/
class CCFollow : public CCAction
{
	public :

		         CCFollow ( KDvoid );
		virtual ~CCFollow ( KDvoid );
		
		/**
		 *	creates the action with a set boundary,
		 *	It will work with no boundary if @param rect is equal to CCRectZero.
		 */
		static CCFollow*		create ( CCNode* pFollowedNode, const CCRect& tRect = CCRectZero );

	public :

		/** initializes the action with a set boundary */
		KDbool					initWithTarget ( CCNode* pFollowedNode, const CCRect& tRect = CCRectZero );

		inline  KDbool			isBoundarySet ( KDvoid ) { return m_bBoundarySet; }

		/** alter behavior - turn on/off boundary */
		inline  KDvoid			setBoudarySet ( KDbool bValue ) { m_bBoundarySet = bValue; }

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			step ( KDfloat fDelta );
		
		virtual KDbool			isDone ( KDvoid );

		virtual KDvoid			stop ( KDvoid );

	protected :

		// node to follow
		CCNode*					m_pFollowedNode;

		// whether camera should be limited to certain area
		KDbool					m_bBoundarySet;

		// if screen size is bigger than the boundary - update not needed
		KDbool					m_bBoundaryFullyCovered;

		// fast access to the screen dimensions
		CCPoint					m_tHalfScreenSize;
		CCPoint					m_tFullScreenSize;

		// world boundaries
		KDfloat					m_fLeftBoundary;
		KDfloat					m_fRightBoundary;
		KDfloat					m_fTopBoundary;
		KDfloat					m_fBottomBoundary;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCAction_h__
