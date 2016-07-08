/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionInterval.h 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2011 Ricardo Quesada
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

#ifndef __CCActionInterval_h__
#define __CCActionInterval_h__

#include "CCAction.h"
#include "../base_nodes/CCNode.h"
#include "../sprite_nodes/CCSpriteFrame.h"
#include "../sprite_nodes/CCAnimation.h"

NS_CC_BEGIN

/**
 *	@addtogroup actions
 *	@{
 */

/** 
 *	@brief An interval action is an action that takes place within a certain period of time.
 *	It has an start time, and a finish time. The finish time is the parameter
 *	duration plus the start time.
 *
 *	These ActionInterval actions have some interesting properties, like:
 *		- They can run normally (default)
 *		- They can run reversed with the reverse method
 *		- They can run with the time altered with the Accelerate, AccelDeccel and Speed actions.
 *
 *	For example, you can simulate a Ping Pong effect running the action normally and
 *	then running it again in Reverse mode.
 *
 *	Example:
 *
 *	Action *pingPongAction = Sequence::actions(action, action->reverse(), NULL);
 */
class CC_DLL ActionInterval : public FiniteTimeAction
{
	public :

		/** how many seconds had elapsed since the actions started to run. */
		inline KDfloat				getElapsed ( KDvoid ) { return m_fElapsed; }

		/** initializes the action */
		KDbool						initWithDuration ( KDfloat d );

		// extension in GridAction
		KDvoid						setAmplitudeRate ( KDfloat fAmp );
		KDfloat						getAmplitudeRate ( KDvoid );

		//
		// Overrides
		//
		virtual KDbool				isDone ( KDvoid ) const override;
		virtual KDvoid				step ( KDfloat dt ) override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual ActionInterval*		reverse ( KDvoid ) const override = 0;
		virtual ActionInterval*		clone ( KDvoid ) const override = 0;

	protected :

		KDfloat						m_fElapsed;
		KDbool						m_bFirstTick;
};

/** 
 *	@brief Runs actions sequentially, one after another
 */
class CC_DLL Sequence : public ActionInterval
{
	public :

		/** helper constructor to create an array of sequenceable actions */
		static Sequence*			create ( FiniteTimeAction* pAction1, ... ) CC_REQUIRES_NULL_TERMINATION;

		/**
		 *	helper constructor to create an array of sequenceable actions given an array
		 *	@code
		 *	When this funtion bound to the js or lua,the input params changed
		 *	in js  :var   create(var   object1,var   object2, ...)
		 *	in lua :local create(local object1,local object2, ...)
		 *	@endcode
		 */
		static Sequence*			create ( Array* pArrayOfActions );

		/** helper constructor to create an array of sequence-able actions */
		static Sequence*			createWithVariableList ( FiniteTimeAction* pAction1, KDVaListKHR pArgs );

		/** creates the action */
		static Sequence*			createWithTwoActions ( FiniteTimeAction* pActionOne, FiniteTimeAction* pActionTwo );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Sequence ( KDvoid );

		/** initializes the action */
		KDbool						initWithTwoActions ( FiniteTimeAction* pActionOne, FiniteTimeAction* pActionTwo );

		//
		// Overrides
		//
		virtual Sequence*			clone ( KDvoid ) const override;
		virtual Sequence*			reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
		virtual KDvoid				update ( KDfloat t ) override;

	protected :

		FiniteTimeAction*			m_aActions [ 2 ];
		KDfloat						m_fSplit;
		KDint						m_nLast;
};

/**
 *	@brief Repeats an action a number of times.
 *	To repeat an action forever use the RepeatForever action.
 */
class CC_DLL Repeat : public ActionInterval
{
	public :

		/** creates a Repeat action. Times is an unsigned integer between 1 and pow(2,30) */
		static Repeat*				create ( FiniteTimeAction* pAction, KDuint uTimes );
		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~Repeat ( KDvoid );

		/** initializes a Repeat action. Times is an unsigned integer between 1 and pow(2,30) */
		KDbool						initWithAction ( FiniteTimeAction* pAction, KDuint uTimes );

		inline KDvoid				setInnerAction ( FiniteTimeAction* pAction )
		{
			if ( m_pInnerAction != pAction )
			{
				CC_SAFE_RETAIN ( pAction );
				CC_SAFE_RELEASE ( m_pInnerAction );
				m_pInnerAction = pAction;
			}
		}

		inline FiniteTimeAction*	getInnerAction ( KDvoid )
		{
			return m_pInnerAction;
		}

		//
		// Overrides
		//
		virtual Repeat*				clone ( KDvoid ) const override;
		virtual Repeat*				reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
		virtual KDvoid				update ( KDfloat dt ) override;
		virtual KDbool				isDone ( KDvoid ) const override;

	protected :

		KDuint						m_uTimes;
		KDuint						m_uTotal;
		KDfloat						m_fNextDt;
		KDbool						m_bActionInstant;
		/** Inner action */
		FiniteTimeAction*			m_pInnerAction;
};

/** 
 *	@brief Repeats an action for ever.
 *	To repeat the an action for a limited number of times use the Repeat action.
 *	@warning This action can't be Sequenceable because it is not an IntervalAction
 */
class CC_DLL RepeatForever : public ActionInterval
{
	public :

		/** creates the action */
		static RepeatForever*		create ( ActionInterval* pAction );

		/**
		 *	@js ctor
		 */
		RepeatForever ( KDvoid )
		{
			m_pInnerAction = KD_NULL;
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~RepeatForever ( KDvoid );

		/** initializes the action */
		KDbool						initWithAction ( ActionInterval* pAction );

		inline KDvoid				setInnerAction ( ActionInterval* pAction )
		{
			if ( m_pInnerAction != pAction )
			{
				CC_SAFE_RELEASE ( m_pInnerAction );
				m_pInnerAction = pAction;
				CC_SAFE_RETAIN ( m_pInnerAction );
			}
		}

		inline ActionInterval*		getInnerAction ( KDvoid )
		{
			return m_pInnerAction;
		}

		//
		// Overrides
		//
		virtual RepeatForever*		clone ( KDvoid ) const override;
		virtual RepeatForever*		reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				step ( KDfloat dt ) override;
		virtual KDbool				isDone ( KDvoid ) const override;

	protected :

		/** Inner action */
		ActionInterval*				m_pInnerAction;
};

/**
 *	@brief Spawn a new action immediately
 */
class CC_DLL Spawn : public ActionInterval
{
	public :

		/** helper constructor to create an array of spawned actions 
		 * @code
		 * When this funtion bound to the js or lua,the input params changed
		 * in js  :var   create(var   object1,var   object2, ...)
		 * in lua :local create(local object1,local object2, ...)
		 * @endcode
		 */
		static Spawn*				create ( FiniteTimeAction* pAction1, ... ) CC_REQUIRES_NULL_TERMINATION;

		/** helper constructor to create an array of spawned actions */
		static Spawn*				createWithVariableList ( FiniteTimeAction* pAction1, KDVaListKHR pArgs );

		/** helper constructor to create an array of spawned actions given an array */
		static Spawn*				create ( Array* pArrayOfActions );

		/** creates the Spawn action */
		static Spawn*				createWithTwoActions ( FiniteTimeAction* pAction1, FiniteTimeAction* pAction2 );

		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~Spawn ( KDvoid );

		/** initializes the Spawn action with the 2 actions to spawn */
		KDbool						initWithTwoActions ( FiniteTimeAction* pAction1, FiniteTimeAction* pAction2 );

		//
		// Overrides
		//
		virtual Spawn*				clone ( KDvoid ) const override;
		virtual Spawn*				reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		FiniteTimeAction*			m_pOne;
		FiniteTimeAction*			m_pTwo;
};

/** 
 *	@brief Rotates a Node object to a certain angle by modifying it's
 *	rotation attribute.
 *	The direction will be decided by the shortest angle.
 */ 
class CC_DLL RotateTo : public ActionInterval
{
	public :

		/** creates the action with separate rotation angles */
		static RotateTo*			create ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY );

		/** creates the action */
		static RotateTo*			create ( KDfloat fDuration, KDfloat fDeltaAngle );

		/** initializes the action */
		KDbool						initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngle );
    
		KDbool						initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY );

		//
		// Overrides
		//
		virtual RotateTo*			clone ( KDvoid ) const override;
		virtual RotateTo*			reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;
    
	protected :

		KDfloat						m_fDstAngleX;
		KDfloat						m_fStartAngleX;
		KDfloat						m_fDiffAngleX;
    
		KDfloat						m_fDstAngleY;
		KDfloat						m_fStartAngleY;
		KDfloat						m_fDiffAngleY;
};

/**
 *	@brief Rotates a Node object clockwise a number of degrees by modifying it's rotation attribute.
 */
class CC_DLL RotateBy : public ActionInterval
{
	public :

		/** creates the action */
		static RotateBy*			create ( KDfloat fDuration, KDfloat fDeltaAngle );

		/** initializes the action */
		KDbool						initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngle );
    
		static RotateBy*			create ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY );
		KDbool						initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY );

		//
		// Override
		//
		virtual RotateBy*			clone ( KDvoid ) const override;
		virtual RotateBy*			reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;
    
	protected :

		KDfloat						m_fAngleX;
		KDfloat						m_fStartAngleX;
		KDfloat						m_fAngleY;
		KDfloat						m_fStartAngleY;
};

/** 
 *	Moves a Node object x,y pixels by modifying it's position attribute.
 *	x and y are relative to the position of the object.
 *	Several MoveBy actions can be concurrently called, and the resulting
 *	movement will be the sum of individual movements.
 *	@since v2.1beta2-custom
 */
class CC_DLL MoveBy : public ActionInterval
{
	public :

		/** creates the action */
		static MoveBy*				create ( KDfloat fDuration, const Point& tDeltaPosition );

		/** initializes the action */
		KDbool						initWithDuration ( KDfloat fDuration, const Point& tDeltaPosition );

		//
		// Overrides
		//
		virtual MoveBy*				clone ( KDvoid ) const override;
		virtual MoveBy*				reverse ( KDvoid ) const  override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		Point						m_tPositionDelta;
		Point						m_tStartPosition;
		Point						m_tPreviousPosition;
};

/** 
 *	Moves a Node object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
 *	Several MoveTo actions can be concurrently called, and the resulting
 *	movement will be the sum of individual movements.
 *	@since v2.1beta2-custom
 */
class CC_DLL MoveTo : public MoveBy
{
	public :
		/** creates the action */
		static MoveTo*				create ( KDfloat fDuration, const Point& tPosition );

		/** initializes the action */
		KDbool						initWithDuration ( KDfloat fDuration, const Point& tPosition );

		//
		// Overrides
		//
		virtual MoveTo*				clone ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;

	protected :

		Point						m_tEndPosition;
};

/** 
 *	Skews a Node object to given angles by modifying it's skewX and skewY attributes
 *	@since v1.0
 */
class CC_DLL SkewTo : public ActionInterval
{
	public :

		/** creates the action */
		static SkewTo*				create ( KDfloat t, KDfloat sx, KDfloat sy );

		SkewTo ( KDvoid );

		KDbool						initWithDuration ( KDfloat t, KDfloat sx, KDfloat sy );

		//
		// Overrides
		//
		virtual SkewTo*				clone ( KDvoid ) const override;
		virtual SkewTo*				reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDfloat						m_fSkewX;
		KDfloat						m_fSkewY;
		KDfloat						m_fStartSkewX;
		KDfloat						m_fStartSkewY;
		KDfloat						m_fEndSkewX;
		KDfloat						m_fEndSkewY;
		KDfloat						m_fDeltaX;
		KDfloat						m_fDeltaY;
};

/** 
 *	Skews a Node object by skewX and skewY degrees
 *	@since v1.0
 */
class CC_DLL SkewBy : public SkewTo
{
	public :

		/** creates the action */
		static SkewBy*				create ( KDfloat t, KDfloat fDeltaSkewX, KDfloat fDeltaSkewY );

		KDbool						initWithDuration ( KDfloat t, KDfloat sx, KDfloat sy );

		//
		// Overrides
		//
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual SkewBy*				clone ( KDvoid ) const override;
		virtual SkewBy*				reverse ( KDvoid ) const override;
};

/** 
 *	@brief Moves a Node object simulating a parabolic jump movement by modifying it's position attribute.
 */
class CC_DLL JumpBy : public ActionInterval
{
	public :

		/** creates the action */
		static JumpBy*				create ( KDfloat fDuration, const Point& tPosition, KDfloat fHeight, KDint nJumps );

		/** initializes the action */
		KDbool						initWithDuration ( KDfloat fDuration, const Point& tPosition, KDfloat fHeight, KDint nJumps );

		//
		// Overrides
		//
		virtual JumpBy*				clone ( KDvoid ) const override;
		virtual JumpBy*				reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		Point						m_tStartPosition;
		Point						m_tDelta;
		KDfloat						m_fHeight;
		KDint						m_nJumps;
		Point						m_tPreviousPos;
};

/** 
 *	@brief Moves a Node object to a parabolic position simulating a jump movement by modifying it's position attribute.
 */ 
class CC_DLL JumpTo : public JumpBy
{
	public :

		/** creates the action */
		static JumpTo*				create ( KDfloat fDuration, const Point& tPosition, KDfloat fHeight, KDint nJumps );

		//
		// Override
		//
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual JumpTo*				clone ( KDvoid ) const override;
		virtual JumpTo*				reverse ( KDvoid ) const override;
};

/**
 *	Bezier configuration structure
 */
typedef struct _ccBezierConfig 
{
    //! end position of the bezier
    Point			tEndPosition;
    //! Bezier control point 1
    Point			tControlPoint1;
    //! Bezier control point 2
    Point			tControlPoint2;
} ccBezierConfig;

/** 
 *	@brief An action that moves the target with a cubic Bezier curve by a certain distance.
 */
class CC_DLL BezierBy : public ActionInterval
{
	public :

		/**
		 *	creates the action with a duration and a bezier configuration 
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var create(var t,var table)
		 *	in lua: lcaol create(local t, local table)
		 *	@endcode
		 */
		static BezierBy*			create ( KDfloat t, const ccBezierConfig& c );

		/** initializes the action with a duration and a bezier configuration */
		KDbool						initWithDuration ( KDfloat t, const ccBezierConfig& c );

		//
		// Overrides
		//
		virtual BezierBy*			clone ( KDvoid ) const override;
		virtual BezierBy*			reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		ccBezierConfig				m_tConfig;
		Point						m_tStartPosition;
		Point						m_tPreviousPosition;
};

/** 
 *	@brief An action that moves the target with a cubic Bezier curve to a destination point.
 *	@since v0.8.2
 */
class CC_DLL BezierTo : public BezierBy
{
	public :

		/**
		 *	creates the action with a duration and a bezier configuration 
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var create(var t,var table)
		 *	in lua: lcaol create(local t, local table)
		 *	@endcode
		 */
		static BezierTo*			create ( KDfloat t, const ccBezierConfig& c );
		KDbool						initWithDuration ( KDfloat t, const ccBezierConfig& c );

		//
		// Overrides
		//
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual BezierTo*			clone ( KDvoid ) const override;
		virtual BezierTo*			reverse ( KDvoid ) const override;

	protected :

		ccBezierConfig				m_tToConfig;
};

/** 
 *	@brief Scales a Node object to a zoom factor by modifying it's scale attribute.
 *	@warning This action doesn't support "reverse"
 */
class CC_DLL ScaleTo : public ActionInterval
{
	public :

		/** creates the action with the same scale factor for X and Y */
		static ScaleTo*				create ( KDfloat fDuration, KDfloat s );

		/** creates the action with and X factor and a Y factor */
		static ScaleTo*				create ( KDfloat fDuration, KDfloat sx, KDfloat sy );

		/** initializes the action with the same scale factor for X and Y */
		KDbool						initWithDuration ( KDfloat fDuration, KDfloat s );

		/** initializes the action with and X factor and a Y factor */
		KDbool						initWithDuration ( KDfloat fDuration, KDfloat sx, KDfloat sy );

		//
		// Overrides
		//
		virtual ScaleTo*			clone ( KDvoid ) const override;
		virtual ScaleTo*			reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDfloat						m_fScaleX;
		KDfloat						m_fScaleY;
		KDfloat						m_fStartScaleX;
		KDfloat						m_fStartScaleY;
		KDfloat						m_fEndScaleX;
		KDfloat						m_fEndScaleY;
		KDfloat						m_fDeltaX;
		KDfloat						m_fDeltaY;
};

/** 
 *	@brief Scales a Node object a zoom factor by modifying it's scale attribute.
 */
class CC_DLL ScaleBy : public ScaleTo
{
	public :

		/** creates the action with the same scale factor for X and Y */
		static ScaleBy*				create ( KDfloat fDuration, KDfloat s );

		/** creates the action with and X factor and a Y factor */
		static ScaleBy*				create ( KDfloat fDuration, KDfloat sx, KDfloat sy );

		//
		// Overrides
		//
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual ScaleBy*			clone ( KDvoid ) const override;
		virtual ScaleBy*			reverse ( KDvoid ) const override;
};

/**
 *	@brief Blinks a Node object by modifying it's visible attribute
 */
class CC_DLL Blink : public ActionInterval
{
	public :

		/** creates the action */
		static Blink*				create ( KDfloat fDuration, KDint nBlinks );

		/** initializes the action */
		KDbool						initWithDuration ( KDfloat fDuration, KDint nBlinks );

		//
		// Overrides
		//
		virtual Blink*				clone ( KDvoid ) const override;
		virtual Blink*				reverse ( KDvoid ) const override;
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
    
	protected :

		KDint						m_nTimes;
		KDbool						m_bOriginalState;
};

/**
 *	@brief Fades In an object that implements the RGBAProtocol protocol. It modifies the opacity from 0 to 255.
 *	The "reverse" of this action is FadeOut
 */
class CC_DLL FadeIn : public ActionInterval
{
	public :

		/** creates the action */
		static FadeIn*				create ( KDfloat d );

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual FadeIn*				clone ( KDvoid ) const override;
		virtual ActionInterval*		reverse ( KDvoid ) const override;
};

/**
 *	@brief Fades Out an object that implements the RGBAProtocol protocol. It modifies the opacity from 255 to 0.
 *	The "reverse" of this action is FadeIn
 */
class CC_DLL FadeOut : public ActionInterval
{
	public :

		/** creates the action */
		static FadeOut*				create ( KDfloat d );

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual FadeOut*			clone ( KDvoid ) const  override;
		virtual ActionInterval*		reverse ( KDvoid ) const override;
};

/**
 *	@brief Fades an object that implements the RGBAProtocol protocol. It modifies the opacity from the current value to a custom one.
 *	@warning This action doesn't support "reverse"
 */
class CC_DLL FadeTo : public ActionInterval
{
	public :

		/** creates an action with duration and opacity */
		static FadeTo*				create ( KDfloat fDuration, GLubyte cOpacity );

		/** initializes the action with duration and opacity */
		KDbool						initWithDuration ( KDfloat fDuration, GLubyte cOpacity );

		//
		// Overrides
		//
		virtual FadeTo*				clone ( KDvoid ) const override;
		virtual FadeTo*				reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		GLubyte						m_cToOpacity;
		GLubyte						m_cFromOpacity;
};

/**
 *	@brief Tints a Node that implements the NodeRGB protocol from current tint to a custom one.
 *	@warning This action doesn't support "reverse"
 *	@since v0.7.2
 */
class CC_DLL TintTo : public ActionInterval
{
	public :

		/** creates an action with duration and color */
		static TintTo*				create ( KDfloat fDuration, GLubyte cRed, GLubyte cGreen, GLubyte cBlue );

		/** initializes the action with duration and color */
		KDbool						initWithDuration ( KDfloat fDuration, GLubyte cRed, GLubyte cGreen, GLubyte cBlue );

		//
		// Overrides
		//
		virtual TintTo*				clone ( KDvoid ) const override;
		virtual TintTo*				reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		Color3B						m_tTo;
		Color3B						m_tFrom;
};

/**
 *	@brief Tints a Node that implements the NodeRGB protocol from current tint to a custom one.
 *	@since v0.7.2
 */
class CC_DLL TintBy : public ActionInterval
{
	public :

		/** creates an action with duration and color */
		static TintBy*				create ( KDfloat fDuration, GLshort nDeltaRed, GLshort nDeltaGreen, GLshort nDeltaBlue );

		/** initializes the action with duration and color */
		KDbool						initWithDuration ( KDfloat fDuration, GLshort nDeltaRed, GLshort nDeltaGreen, GLshort nDeltaBlue );

		//
		// Overrides
		//
		virtual TintBy*				clone ( KDvoid ) const override;
		virtual TintBy*				reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		GLshort						m_nDeltaR;
		GLshort						m_nDeltaG;
		GLshort						m_nDeltaB;

		GLshort						m_nFromR;
		GLshort						m_nFromG;
		GLshort						m_nFromB;
};

/**
 *	@brief Delays the action a certain amount of seconds
 */
class CC_DLL DelayTime : public ActionInterval
{
	public :

		/** creates the action */
		static DelayTime*			create ( KDfloat d );

		//
		// Overrides
		//
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual DelayTime*			reverse ( KDvoid ) const override;
		virtual DelayTime*			clone ( KDvoid ) const override;
};

/**
 *	@brief Executes an action in reverse order, from time=duration to time=0
 *
 *	@warning Use this action carefully. This action is not
 *	sequenceable. Use it as the default "reversed" method
 *	of your own actions, but using it outside the "reversed"
 *	scope is not recommended.
 */
class CC_DLL ReverseTime : public ActionInterval
{
	public :

		/** creates the action */
		static ReverseTime*			create ( FiniteTimeAction* pAction );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ReverseTime ( KDvoid );

		/**
		 *	@js ctor
		 */
		ReverseTime ( KDvoid );

		/** initializes the action */
		KDbool						initWithAction ( FiniteTimeAction* pAction );

		//
		// Overrides
		//
		virtual ReverseTime*		reverse ( KDvoid ) const override;
		virtual ReverseTime*		clone ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		FiniteTimeAction*			m_pOther;
};

class Texture2D;
/** @brief Animates a sprite given the name of an Animation */
class CC_DLL Animate : public ActionInterval
{
	public :

		/** creates the action with an Animation and will restore the original frame when the animation is over */
		static Animate*				create ( Animation* pAnimation );
		
		/**
		 *	@js ctor
		 */
		Animate ( KDvoid );
		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~Animate ( KDvoid );

		/** initializes the action with an Animation and will restore the original frame when the animation is over */
		KDbool						initWithAnimation ( Animation* pAnimation );

		/** sets the Animation object to be animated */
		KDvoid						setAnimation ( Animation* pAnimation );

		/** returns the Animation object that is being animated */
		Animation*					getAnimation ( KDvoid ) { return m_pAnimation; }
		const Animation*			getAnimation ( KDvoid ) const { return m_pAnimation; }

		//
		// Overrides
		//
		virtual Animate*			clone ( KDvoid ) const override;
		virtual Animate*			reverse ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
		virtual KDvoid				update ( KDfloat t) override;

	protected :

		std::vector<KDfloat>*		m_pSplitTimes;
		KDint						m_nNextFrame;
		SpriteFrame*				m_pOrigFrame;
		KDuint						m_uExecutedLoops;
		Animation*					m_pAnimation;
};

/** 
 *	Overrides the target of an action so that it always runs on the target
 *	specified at action creation rather than the one specified by runAction.
 */
class CC_DLL TargetedAction : public ActionInterval
{
	public :

		/**
		 *	@js ctor
		 */
		TargetedAction ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TargetedAction ( KDvoid );

		/** Create an action with the specified action and forced target */
		static TargetedAction*		create ( Node* pTarget, FiniteTimeAction* pAction );

		/** Init an action with the specified action and forced target */
		KDbool						initWithTarget ( Node* pTarget, FiniteTimeAction* pAction );

		/** Sets the target that the action will be forced to run with */
		KDvoid						setForcedTarget ( Node* pForcedTarget );

		/** returns the target that the action is forced to run with */
		Node*						getForcedTarget ( KDvoid ) { return m_pForcedTarget; }
		const Node*					getForcedTarget ( KDvoid ) const { return m_pForcedTarget; }

		//
		// Overrides
		//
		virtual TargetedAction*		clone ( KDvoid ) const override;
		virtual TargetedAction*		reverse ( KDvoid ) const  override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				stop ( KDvoid ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	private :

		FiniteTimeAction*			m_pAction;
		Node*						m_pForcedTarget;
};

// end of actions group
/// @}

NS_CC_END

#endif	// __CCActionInterval_h__
