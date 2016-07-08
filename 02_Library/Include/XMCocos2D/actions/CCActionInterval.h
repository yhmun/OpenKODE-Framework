/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionInterval.h 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCActionInterval_h__
#define __CCActionInterval_h__

#include "CCAction.h"
#include "../sprite_nodes/CCAnimation.h"

#include <vector>

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

/** 
 *	@brief	An interval action is an action that takes place within a certain period of time.
 *			It has an start time, and a finish time. The finish time is the parameter
 *			duration plus the start time.
 *
 *	These CCActionInterval actions have some interesting properties, like:
 *		- They can run normally (default)
 *		- They can run reversed with the reverse method
 *		- They can run with the time altered with the Accelerate, AccelDeccel and Speed actions.
 *	
 *	For example, you can simulate a Ping Pong effect running the action normally and
 *	then running it again in Reverse mode.
 *
 *	Example:
 *
 *	CCAction *pingPongAction = CCSequence::actions(action, action->reverse(), KD_NULL);
 */
class CCActionInterval : public CCFiniteTimeAction
{
	public:

		/** creates the action */
		static  CCActionInterval*	create ( KDfloat fDuration );

	public:

		/** initializes the action */
		virtual KDbool				initWithDuration ( KDfloat fDuration );

		/** how many seconds had elapsed since the actions started to run. */
		inline  KDfloat				getElapsed ( KDvoid ) { return m_fElapsed; }		
		
		virtual CCObject*			copyWithZone ( CCZone* pZone );

		/** returns true if the action has finished */
		virtual KDbool				isDone ( KDvoid );

		virtual KDvoid				step ( KDfloat fDelta );
	
        virtual KDvoid				startWithTarget ( CCNode* pTarget );

		/** returns a reversed action */
		virtual CCActionInterval*	reverse ( KDvoid );	

		// extension in CCGridAction 
		KDvoid						setAmplitudeRate ( KDfloat fAmplitudeRate );
		KDfloat						getAmplitudeRate ( KDvoid );

	protected :

		KDfloat						m_fElapsed;
		KDbool						m_bFirstTick;
};

/**
 *	@brief	Runs actions sequentially, one after another
 */
class CCSequence : public CCActionInterval
{
	public :

		virtual ~CCSequence ( KDvoid );

		/** helper constructor to create an array of sequenceable actions */
		static   CCSequence*		create ( CCFiniteTimeAction* pAction1, ... );

		/** helper constructor to create an array of sequenceable actions given an array */
		static   CCSequence*		create ( CCArray* pArrayOfActions );

		/** helper constructor to create an array of sequence-able actions */
		static   CCSequence*		createWithVariableList ( CCFiniteTimeAction* pAction1, KDVaListKHR pArgs );

		/** creates the action */
		static   CCSequence*		createWithTwoActions ( CCFiniteTimeAction* pAction1, CCFiniteTimeAction* pAction2 );

	public :

		/** initializes the action */
		virtual KDbool				initWithTwoActions ( CCFiniteTimeAction* pAction1, CCFiniteTimeAction* pAction2 );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				stop ( KDvoid );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );		

	protected :

		CCFiniteTimeAction*			m_pActions[2];
		KDfloat						m_fSplit;
		KDint						m_nLast;
};

/**
 *	@brief	Repeats an action a number of times.
 *			To repeat an action forever use the CCRepeatForever action.
 */
class CCRepeat : public CCActionInterval
{
	public :

		virtual ~CCRepeat ( KDvoid );

		/** creates a CCRepeat action. Times is an unsigned integer between 1 and pow(2,30) */
		static  CCRepeat*			create ( CCFiniteTimeAction* pAction, KDuint uTimes );

	public :

		/** initializes a CCRepeat action. Times is an unsigned integer between 1 and pow(2,30) */
		virtual KDbool				initWithAction ( CCFiniteTimeAction* pAction, KDuint uTimes );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDbool				isDone ( KDvoid );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				stop ( KDvoid );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

		inline  KDvoid				setInnerAction ( CCFiniteTimeAction* pAction )
		{
			if ( m_pInnerAction != pAction )
			{
				CC_SAFE_RETAIN ( pAction );
				CC_SAFE_RELEASE ( m_pInnerAction );
				m_pInnerAction = pAction;				
			}
		}
		
		inline CCFiniteTimeAction*	getInnerAction ( KDvoid )
		{
			return m_pInnerAction;
		}

	protected :

		KDuint						m_uTimes;
		KDuint						m_uTotal;

		KDfloat						m_fNextDt;
		KDbool						m_bActionInstant;

		/** Inner action */
		CCFiniteTimeAction*			m_pInnerAction;
};

/**
 *	@brief	 Repeats an action for ever.
 *			 To repeat the an action for a limited number of times use the Repeat action.
 *	@warning This action can't be Sequenceable because it is not an IntervalAction
 */
class CCRepeatForever : public CCActionInterval
{
	public :

		         CCRepeatForever ( KDvoid );
		virtual ~CCRepeatForever ( KDvoid );

		/** creates the action */
		static   CCRepeatForever*	create ( CCActionInterval* pAction );

	public :

		/** initializes the action */
		virtual KDbool				initWithAction ( CCActionInterval* pAction );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDbool				isDone ( KDvoid );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				step ( KDfloat fDelta );

		virtual CCActionInterval*	reverse ( KDvoid );

		inline  KDvoid				setInnerAction ( CCActionInterval* pAction )
		{
			if ( m_pInnerAction != pAction )
			{
				CC_SAFE_RETAIN ( pAction );
				CC_SAFE_RELEASE ( m_pInnerAction );
				m_pInnerAction = pAction;	
			}
		}
		
		inline  CCActionInterval*	getInnerAction ( KDvoid )
		{
			return m_pInnerAction;
		}

	protected :

		/** Inner action */
		CCActionInterval*			m_pInnerAction;
};

/**
 *	@brief	Spawn a new action immediately
 */
class CCSpawn : public CCActionInterval
{
	public :

		virtual ~CCSpawn ( KDvoid );

		/** helper constructor to create an array of spawned actions */
		static   CCSpawn*			create ( CCFiniteTimeAction* pAction1, ... );

		/** helper constructor to create an array of spawned actions given an array */
		static   CCSpawn*			create ( CCArray* pArrayOfActions );

		/** helper constructor to create an array of spawned actions */
		static   CCSpawn*			createWithVariableList ( CCFiniteTimeAction* pAction1, KDVaListKHR pArgs );

		/** creates the Spawn action */
		static   CCSpawn*			createWithTwoActions ( CCFiniteTimeAction* pAction1, CCFiniteTimeAction* pAction2 );

	public :

		/** initializes the Spawn action with the 2 actions to spawn */
		virtual KDbool				initWithTwoActions ( CCFiniteTimeAction* pAction1, CCFiniteTimeAction* pAction2 );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				stop ( KDvoid );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		CCFiniteTimeAction*			m_pAction1;
		CCFiniteTimeAction*			m_pAction2;
};

/**
 *	@brief	Rotates a CCNode object to a certain angle by modifying it's
 *			rotation attribute.
 *			The direction will be decided by the shortest angle.
*/ 
class CCRotateTo : public CCActionInterval
{
	public :

		/** creates the action */
		static  CCRotateTo*			create ( KDfloat fDuration, KDfloat fDeltaAngle );

		/** creates the action with separate rotation angles */
		static  CCRotateTo*			create ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY );    

	public :

		/** initializes the action */
		virtual KDbool				initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngle );
		virtual KDbool				initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

	protected :

		CCPoint						m_tStartAngle;
        CCPoint						m_tEndAngle;
		CCPoint						m_tDiffAngle;
};

/**
 *	@brief	Rotates a CCNode object clockwise a number of degrees by modifying it's rotation attribute.
 */
class CCRotateBy : public CCActionInterval
{
	public :

		/** creates the action */
		static  CCRotateBy*			create ( KDfloat fDuration, KDfloat fDeltaAngle );
		static  CCRotateBy*			create ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY );		

	public :

		/** initializes the action */
		virtual KDbool				initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngle );
		virtual KDbool				initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );  

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		CCPoint						m_tAngle;
		CCPoint						m_tStartAngle;
};

/**
 *	Moves a CCNode object x,y pixels by modifying it's position attribute.
 *	x and y are relative to the position of the object.
 *	Several CCMoveBy actions can be concurrently called, and the resulting
 *	movement will be the sum of individual movements.
 *	@since v2.1beta2-custom
 */ 
class CCMoveBy : public CCActionInterval
{
	public :
	
		/** creates the action */
		static  CCMoveBy*			create ( KDfloat fDuration, const CCPoint& tDeltaPosition );

	public :

		/** initializes the action */
		virtual KDbool				initWithDuration ( KDfloat fDuration, const CCPoint& tDeltaPosition );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual CCActionInterval*	reverse ( KDvoid );

		virtual KDvoid				update ( KDfloat fTime );

	protected :

		CCPoint						m_tPositionDelta;
		CCPoint						m_tStartPosition;
		CCPoint						m_tPreviousPosition;
};

/** 
 *	Moves a CCNode object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
 *	Several CCMoveTo actions can be concurrently called, and the resulting
 *	movement will be the sum of individual movements.
 *	@since v2.1beta2-custom
 */
class CCMoveTo : public CCMoveBy
{
	public:

		/** creates the action */
		static  CCMoveTo*			create ( KDfloat fDuration, const CCPoint& tPosition );

	public :

		/** initializes the action */
		virtual KDbool				initWithDuration ( KDfloat fDuration, const CCPoint& tPosition );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );   

	protected :

        CCPoint						m_tEndPosition;
};

/** 
 *	@brief	Skews a CCNode object to given angles by modifying it's skewX and skewY attributes
 *	@since	v1.0
 */
class CCSkewTo : public CCActionInterval
{
	public :

		CCSkewTo ( KDvoid );

		/** creates the action */
		static CCSkewTo*			create ( KDfloat fDuration, KDfloat fSkewX, KDfloat fSkewY );

	public :

		virtual KDbool				initWithDuration ( KDfloat fDuration, KDfloat fSkewX, KDfloat fSkewY );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

	protected :

		CCPoint						m_tSkew;
		CCPoint						m_tStartSkew;
		CCPoint						m_tEndSkew;
		CCPoint						m_tDelta;
};

/** 
 *	@brief	a CCNode object by skewX and skewY degrees
 *	@since	v1.0
 */
class CCSkewBy : public CCSkewTo
{
	public :
		
		/** creates the action */
		static CCSkewBy*			create ( KDfloat fDuration, KDfloat fDeltaSkewX, KDfloat fDeltaSkewY );

	public :
		
		virtual KDbool				initWithDuration ( KDfloat fDuration, KDfloat fDeltaSkewX, KDfloat fDeltaSkewY );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual CCActionInterval*	reverse ( KDvoid );
};

class CCJumpBy : public CCActionInterval
{
	public :

		/** creates the action */
		static  CCJumpBy*			create ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDuint uJumps );

	public :

		/** initializes the action */
		virtual KDbool				initWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDuint uJumps );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		CCPoint						m_tStartPosition;
		CCPoint						m_tDelta;
		KDfloat						m_fHeight;
		KDuint						m_uJumps;
		CCPoint						m_tPreviousPos;
};

/**
 *	@brief	Moves a CCNode object to a parabolic position simulating a jump movement by modifying it's position attribute.
 */ 
class CCJumpTo : public CCJumpBy
{
	public :

		/** creates the action */
		static  CCJumpTo*			create ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDint uJumps );

	public :

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );
};

/** 
 *	Moves a CCNode object simulating a parabolic jump movement by modifying its position attribute.
 */
class CCJumpY : public CCActionInterval
{
	public :

		/** creates the action */
		static  CCJumpY*			create ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDint uJumps, KDfloat* pGroundPlane );

	public :

		/** initializes the action */
		virtual KDbool				initWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDuint uJumps, KDfloat* pGroundPlane );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		CCPoint						m_tStartPosition;
		CCPoint						m_tDelta;
		KDfloat						m_fHeight;
		KDuint						m_uJumps;
		KDfloat*					m_pGroundPlane;
		CCNode*						m_pParent;
};


/**
 *	@typedef	bezier configuration structure
 */
typedef struct _ccBezierConfig 
{
	//! end position of the bezier
	CCPoint    tEndPosition;

	//! Bezier control point 1
	CCPoint    tControlPoint1;

	//! Bezier control point 2
	CCPoint    tControlPoint2;
} ccBezierConfig;

/**
 *	@brief	An action that moves the target with a cubic Bezier curve by a certain distance.
 */
class CCBezierBy : public CCActionInterval
{
	public :

		/** creates the action with a duration and a bezier configuration */
		static  CCBezierBy*			create ( KDfloat fDuration, const ccBezierConfig& tBezierConfig );

	public :

		/** initializes the action with a duration and a bezier configuration */
		virtual KDbool				initWithDuration ( KDfloat fDuration, const ccBezierConfig& tBezierConfig );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		ccBezierConfig				m_tBezierConfig;
		CCPoint						m_tStartPosition;
		CCPoint						m_tPreviousPosition;
};

/**
 *	@brief	An action that moves the target with a cubic Bezier curve to a destination point.
 *	@since	v0.8.2
 */
class CCBezierTo : public CCBezierBy
{
	public :

		/** creates the action with a duration and a bezier configuration */
		static  CCBezierTo*			create ( KDfloat fDuration, const ccBezierConfig& tBezierConfig );

	public :

		virtual KDbool				initWithDuration ( KDfloat fDuration, const ccBezierConfig& tBezierConfig );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

	protected :

		ccBezierConfig				m_tBezierToConfig;
};

/**
 *	@brief	 Scales a CCNode object to a zoom factor by modifying it's scale attribute.
 *	@warning This action doesn't support "reverse"
 */
class CCScaleTo : public CCActionInterval
{
	public :

		/** creates the action with the same scale factor for X and Y */
		static  CCScaleTo*			create ( KDfloat fDuration, KDfloat fScale );

		/** creates the action with and X factor and a Y factor */
		static  CCScaleTo*			create ( KDfloat fDuration, KDfloat fScaleX, KDfloat fScaleY );

	public :

		/** initializes the action with the same scale factor for X and Y */
		virtual KDbool				initWithDuration ( KDfloat fDuration, KDfloat fScale );

		/** initializes the action with and X factor and a Y factor */
		virtual KDbool				initWithDuration ( KDfloat fDuration, KDfloat fScaleX, KDfloat fScaleY );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

	protected :

		CCPoint						m_tScale;
		CCPoint						m_tStartScale;
		CCPoint						m_tEndScale;
		CCPoint						m_tDelta;
};

/**
 *	@brief	Scales a CCNode object a zoom factor by modifying it's scale attribute.
 */
class CCScaleBy : public CCScaleTo
{
	public :

		/** creates the action with the same scale factor for X and Y */
		static  CCScaleBy*			create ( KDfloat fDuration, KDfloat fScale );

		/** creates the action with and X factor and a Y factor */
		static  CCScaleBy*			create ( KDfloat fDuration, KDfloat fScaleX, KDfloat fScaleY );

	public :

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual CCActionInterval*	reverse ( KDvoid );
};

/**
 *	@brief	Blinks a CCNode object by modifying it's visible attribute
 */
class CCBlink : public CCActionInterval
{
	public :

		/** creates the action */
		static  CCBlink*			create ( KDfloat fDuration, KDuint uBlinks );

	public :

		/** initializes the action */
		virtual KDbool				initWithDuration ( KDfloat fDuration, KDuint uBlinks );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				stop ( KDvoid );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		KDuint						m_uBlinks;
		KDbool						m_bOriginalState;
};

/**
 *	@brief	Fades In an object that implements the CCRGBAProtocol protocol. It modifies the opacity from 0 to 255.
 *			The "reverse" of this action is FadeOut
 */
class CCFadeIn : public CCActionInterval
{
	public :

		/** creates the action */
		static  CCFadeIn*			create ( KDfloat fDuration );

	public :

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );
};

/**
 *	@brief	Fades Out an object that implements the CCRGBAProtocol protocol. It modifies the opacity from 255 to 0.
 *			The "reverse" of this action is FadeIn
*/
class CCFadeOut : public CCActionInterval
{
	public :

		/** creates the action */
		static  CCFadeOut*			create ( KDfloat fDuration );

	public :

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );
};

/**
 *	@brief	 Fades an object that implements the CCRGBAProtocol protocol. It modifies the opacity from the current value to a custom one.
 *	@warning This action doesn't support "reverse"
 */
class CCFadeTo : public CCActionInterval
{
	public :

		/** creates an action with duration and opacity */
		static  CCFadeTo*			create ( KDfloat fDuration, GLubyte cOpacity );

	public :

		/** initializes the action with duration and opacity */
		virtual KDbool				initWithDuration ( KDfloat fDuration, GLubyte cOpacity );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

	protected :

		GLubyte						m_cToOpacity;
		GLubyte						m_cFromOpacity;
};

/**
 *	@brief	 Tints a CCNode that implements the CCNodeRGB protocol from current tint to a custom one.
 *	@warning This action doesn't support "reverse"
 *	@since	 v0.7.2
 */
class CCTintTo : public CCActionInterval
{
	public :

		/** creates an action with duration and color */
		static  CCTintTo*			create ( KDfloat fDuration, GLubyte cRed, GLubyte cGreen, GLubyte cBlue );

	public :

		/** initializes the action with duration and color */
		virtual KDbool				initWithDuration ( KDfloat fDuration, GLubyte cRed, GLubyte cGreen, GLubyte cBlue );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget ); 

		virtual KDvoid				update ( KDfloat fTime );

	protected :

		ccColor3B					m_tTo;
		ccColor3B					m_tFrom;
};

/**
 *	@brief	Tints a CCNode that implements the CCNodeRGB protocol from current tint to a custom one.
 *	@since	v0.7.2
 */
class CCTintBy : public CCActionInterval
{
	public :

		/** creates an action with duration and color */
		static  CCTintBy*			create ( KDfloat fDuration, GLshort nDeltaRed, GLshort nDeltaGreen, GLshort nDeltaBlue );

	public :

		/** initializes the action with duration and color */
		virtual KDbool				initWithDuration ( KDfloat fDuration, GLshort nDeltaRed, GLshort nDeltaGreen, GLshort nDeltaBlue );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		GLshort						m_nDeltaRed;
		GLshort						m_nDeltaGreen;
		GLshort						m_nDeltaBlue;

		GLshort						m_nFromRed;
		GLshort						m_nFromGreen;
		GLshort						m_nFromBlue;
};

/**
 *	@brief	Delays the action a certain amount of seconds
 */
class CCDelayTime : public CCActionInterval
{
	public :

		/** creates the action */
		static  CCDelayTime*		create ( KDfloat fDuration );

	public :

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );
};

/**
 *	@brief	 Executes an action in reverse order, from time=duration to time=0
 *	@warning Use this action carefully. This action is not
 *			 sequenceable. Use it as the default "reversed" method
 *			 of your own actions, but using it outside the "reversed"
 *			 scope is not recommended.
 */
class CCReverseTime : public CCActionInterval
{
	public :

 		         CCReverseTime ( KDvoid );
		virtual ~CCReverseTime ( KDvoid );

		/** creates the action */
		static   CCReverseTime*		create ( CCFiniteTimeAction* pAction );

	public :

		/** initializes the action */
		virtual KDbool				initWithAction ( CCFiniteTimeAction* pAction );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget ); 

		virtual KDvoid				stop ( KDvoid );    

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		CCFiniteTimeAction*			m_pOther;
};

class CCTexture2D;

class CCAnimate : public CCActionInterval
{
	public :

		         CCAnimate ( KDvoid );
		virtual ~CCAnimate ( KDvoid );

		/** creates the action with an Animation and will restore the original frame when the animation is over */
		static  CCAnimate*			create ( CCAnimation* pAnimation );

	public :

		/** initializes the action with an Animation and will restore the original frame when the animation is over */
		virtual KDbool				initWithAnimation ( CCAnimation* pAnimation );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget(CCNode *pTarget);

		virtual KDvoid				stop ( KDvoid );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	public :

		CC_SYNTHESIZE_RETAIN ( CCAnimation*, m_pAnimation, Animation );

	protected :

		std::vector<KDfloat>*		m_pSplitTimes;
		KDint						m_nNextFrame;
		CCSpriteFrame*				m_pOrigFrame;
		KDuint						m_uExecutedLoops;
};

/** Overrides the target of an action so that it always runs on the target
 * specified at action creation rather than the one specified by runAction.
 */
class CCTargetedAction : public CCActionInterval
{
	public :

		         CCTargetedAction ( KDvoid );
		virtual ~CCTargetedAction ( KDvoid );

		/** Create an action with the specified action and forced target */
		static  CCTargetedAction*	create ( CCNode* pTarget, CCFiniteTimeAction* pAction );

	public :

		/** Init an action with the specified action and forced target */
		virtual KDbool				initWithTarget ( CCNode* pTarget, CCFiniteTimeAction* pAction );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				stop ( KDvoid );

		virtual KDvoid				update ( KDfloat fTime );

		/** This is the target that the action will be forced to run with */
		CC_SYNTHESIZE_RETAIN ( CCNode*, m_pForcedTarget, ForcedTarget );

	private :

		CCFiniteTimeAction*			m_pAction;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionInterval_h__
