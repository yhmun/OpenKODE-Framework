/* -----------------------------------------------------------------------------------
 *
 *      File            CCAction.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCAction_h__
#define __CCAction_h__

#include "../../base/CCObject.h"
#include "../../base/CCGeometry.h"

NS_CC_BEGIN

/**
 *	@addtogroup actions
 *	@{
 */

/** 
 *	@brief Base class for Action objects.
 */
class CC_DLL Action : public Object, public Clonable
{
	public :

		/// Default tag used for all the actions
		static const KDint	INVALID_TAG = -1;

		/**
		 *	@js ctor
		 */
		Action ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Action ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*				description ( KDvoid ) const;

		/** returns a clone of action */
		virtual Action*				clone ( KDvoid ) const = 0;

		/** returns a new action that performs the exactly the reverse action */
		virtual Action*				reverse ( KDvoid ) const = 0;

		//! return true if the action has finished
		virtual KDbool				isDone ( KDvoid ) const;

		//! called before the action start. It will also set the target.
		virtual KDvoid				startWithTarget ( Node* pTarget );

		/** 
		 *	called after the action has finished. It will set the 'target' to nil.
		 *	IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
		 */
		virtual KDvoid				stop ( KDvoid );

		//! called every frame with it's delta time. DON'T override unless you know what you are doing.
		virtual KDvoid				step ( KDfloat dt );

		/** 
		 *	called once per frame. time a value between 0 and 1
		 *
		 *	For example: 
		 *		- 0 means that the action just started
		 *		- 0.5 means that the action is in the middle
		 *		- 1 means that the action is over
		 */
		virtual KDvoid				update ( KDfloat fTime );
    
		inline Node*				getTarget ( KDvoid ) const { return m_pTarget; }

		/** The action will modify the target properties. */
		inline KDvoid				setTarget ( Node* pTarget ) { m_pTarget = pTarget; }
    
		inline Node*				getOriginalTarget ( KDvoid ) const { return m_pOriginalTarget; }
		/**
		 *	Set the original target, since target can be nil.
		 *	Is the target that were used to run the action. Unless you are doing something complex, like ActionManager, you should NOT call this method.
		 *	The target is 'assigned', it is not 'retained'.
		 *	@since v0.8.2
		*/
		inline KDvoid				setOriginalTarget ( Node* pOriginalTarget ) { m_pOriginalTarget = pOriginalTarget; }

		inline KDint				getTag ( KDvoid ) const { return m_nTag; }
		inline KDvoid				setTag ( KDint nTag ) { m_nTag = nTag; }

	protected :

		Node*						m_pOriginalTarget;

		/**
		 *	The "target".
		 *	The target will be set with the 'startWithTarget' method.
		 *	When the 'stop' method is called, target will be set to nil.
		 *	The target is 'assigned', it is not 'retained'.
		 */
		Node*						m_pTarget;
		/** The action tag. An identifier of the action */
		KDint						m_nTag;
};

/** 
 *	@brief 
 *	Base class actions that do have a finite time duration.
 *	Possible actions:
 *		- An action with a duration of 0 seconds
 *		- An action with a duration of 35.5 seconds
 *
 *	Infinite time actions are valid
 */
class CC_DLL FiniteTimeAction : public Action
{
	public :

		/**
		 *	@js ctor
		 */
		FiniteTimeAction ( KDvoid )
		{
			m_fDuration = 0;
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~FiniteTimeAction ( KDvoid ) { }

		//! get duration in seconds of the action
		inline KDfloat				getDuration ( KDvoid ) const { return m_fDuration; }

		//! set duration in seconds of the action
		inline KDvoid				setDuration ( KDfloat fDuration ) { m_fDuration = fDuration; }

		//
		// Overrides
		//
		virtual FiniteTimeAction*	reverse ( KDvoid ) const override = 0;
		virtual FiniteTimeAction*	clone ( KDvoid ) const override = 0;

	protected :

		//! duration in seconds
		KDfloat						m_fDuration;
};

class ActionInterval;
class RepeatForever;

/** 
 *	@brief Changes the speed of an action, making it take longer (speed>1)
 *	or less (speed<1) time.
 *	Useful to simulate 'slow motion' or 'fast forward' effect.
 *	@warning This action can't be Sequenceable because it is not an IntervalAction
 */
class CC_DLL Speed : public Action
{
	public :

		/** create the action */
		static Speed*				create ( ActionInterval* pAction, KDfloat fSpeed );

		/**
		 *	@js ctor
		 */
		Speed ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Speed ( KDvoid );

		inline KDfloat				getSpeed ( KDvoid ) const { return m_fSpeed; }

		/** alter the speed of the inner function in runtime */
		inline KDvoid				setSpeed ( KDfloat fSpeed ) { m_fSpeed = fSpeed; }

		/** initializes the action */
		KDbool						initWithAction ( ActionInterval* pAction, KDfloat fSpeed );

		KDvoid						setInnerAction ( ActionInterval* pAction );

		inline ActionInterval*		getInnerAction ( KDvoid ) const { return m_pInnerAction; }

		//
		// Override
		//
		virtual Speed*				clone ( KDvoid ) const override;
		virtual Speed*				reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
		virtual KDvoid				step ( KDfloat dt ) override;
		virtual KDbool				isDone ( KDvoid ) const override;

	protected :

		KDfloat						m_fSpeed;
		ActionInterval*				m_pInnerAction;
};

/** 
 *	@brief Follow is an action that "follows" a node.
 *
 *	Eg:
 *	@code
 *	layer->runAction(Follow::actionWithTarget(hero));
 *	@endcode
 *
 *	Instead of using Camera as a "follower", use this action instead.
 *	@since v0.99.2
 */
class CC_DLL Follow : public Action
{
	public:
		/**
		 *	Creates the action with a set boundary or with no boundary.
		 *
		 *	@param followedNode  The node to be followed.
		 *	@param rect  The boundary. If \p rect is equal to Rect::ZERO, it'll work
		 *	             with no boundary.
		 */
		static Follow*				create ( Node* pFollowedNode, const Rect& tRect = Rect::ZERO );

		/**
		 *	@js ctor
		 */
		Follow ( KDvoid )
		{
			m_pFollowedNode			= nullptr;
			m_bBoundarySet			= false;
			m_bBoundaryFullyCovered = false;
			m_fLeftBoundary			= 0.0;
			m_fRightBoundary		= 0.0;
			m_fTopBoundary			= 0.0;
			m_fBottomBoundary		= 0.0;
			m_tWorldRect			= Rect::ZERO;
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Follow ( KDvoid );
    
		inline KDbool				isBoundarySet ( KDvoid ) const { return m_bBoundarySet; }

		/** alter behavior - turn on/off boundary */
		inline KDvoid				setBoudarySet ( KDbool bValue ) { m_bBoundarySet = bValue; }

		/**
		 *	Initializes the action with a set boundary or with no boundary.
		 *
		 *	@param followedNode  The node to be followed.
		 *	@param rect  The boundary. If \p rect is equal to Rect::ZERO, it'll work
		 *	             with no boundary.
		 */
		KDbool						initWithTarget ( Node* pFollowedNode, const Rect& tRect = Rect::ZERO );

		//
		// Override
		//
		virtual Follow*				clone ( KDvoid ) const override;
		virtual Follow*				reverse ( KDvoid ) const override;
		virtual KDvoid				step ( KDfloat dt ) override;
		virtual KDbool				isDone ( KDvoid ) const override;
		virtual KDvoid				stop ( KDvoid ) override;

	protected :

		// node to follow
		Node*						m_pFollowedNode;

		// whether camera should be limited to certain area
		KDbool						m_bBoundarySet;

		// if screen size is bigger than the boundary - update not needed
		KDbool						m_bBoundaryFullyCovered;

		// fast access to the screen dimensions
		Point						m_tHalfScreenSize;
		Point						m_tFullScreenSize;

		// world boundaries
		KDfloat						m_fLeftBoundary;
		KDfloat						m_fRightBoundary;
		KDfloat						m_fTopBoundary;
		KDfloat						m_fBottomBoundary;
		Rect						m_tWorldRect;
};

//	end of actions group
/// @}

NS_CC_END

#endif	// __CCAction_h__
