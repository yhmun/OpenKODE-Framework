/* -----------------------------------------------------------------------------------
 *
 *      File            CCArmatureAnimation.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __CCArmatureAnimation_h__
#define __CCArmatureAnimation_h__

#include "CCProcessBase.h"
#include <queue>

namespace cocostudio {


enum MovementEventType
{
    START,
    COMPLETE,
    LOOP_COMPLETE
};


class Armature;
class Bone;

typedef KDvoid ( cocos2d::Object::*SEL_MovementEventCallFunc ) ( Armature*, MovementEventType, const KDchar* );
typedef KDvoid ( cocos2d::Object::*SEL_FrameEventCallFunc ) ( Bone*, const KDchar*, KDint, KDint );

#define movementEvent_selector(_SELECTOR)	( cocostudio::SEL_MovementEventCallFunc) ( &_SELECTOR )
#define frameEvent_selector(_SELECTOR)		( cocostudio::SEL_FrameEventCallFunc ) ( &_SELECTOR )

struct FrameEvent
{
    Bone*				bone;
    const KDchar*		frameEventName;
    KDint				originFrameIndex;
    KDint				currentFrameIndex;
};

class ArmatureAnimation : public ProcessBase
{
	public :

		/**
		 *	Create with a Armature
		 *	@param armature The Armature ArmatureAnimation will bind to
		 */
		static ArmatureAnimation*		create ( Armature* pArmature );

	public :

		/**
		 *	@js ctor
		 */
		ArmatureAnimation ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ArmatureAnimation ( KDvoid );

		/**
		 *	Init with a Armature
		 *	@param armature The Armature ArmatureAnimation will bind to
		 */
		virtual KDbool					init ( Armature* pArmature );

		/**
		 *	Scale animation play speed.
		 *	@param animationScale Scale value
		 */
		virtual KDvoid					setSpeedScale ( KDfloat fSpeedScale );
		virtual KDfloat					getSpeedScale ( KDvoid ) const;

		//! The animation update speed
		virtual KDvoid					setAnimationInternal ( KDfloat fAnimationInternal );

		using ProcessBase::play;

		/**
		 *	Play animation by animation name.
		 *
		 *	@param  animationName  The animation name you want to play
		 *	@param  durationTo The frames between two animation changing-over.
		 *	        It's meaning is changing to this animation need how many frames
		 *
		 *          -1 : use the value from MovementData get from flash design panel
		 *	@param  durationTween  The frame count you want to play in the game.
		 *          if  _durationTween is 80, then the animation will played 80 frames in a loop
		 *
		 *          -1 : use the value from MovementData get from flash design panel
		 *
		 *	@param  loop   Whether the animation is loop
		 *
		 *          loop < 0 : use the value from MovementData get from flash design panel
		 *          loop = 0 : this animation is not loop
		 *          loop > 0 : this animation is loop
		 *
		 *	@param  tweenEasing Tween easing is used for calculate easing effect
		 *
		 *          TWEEN_EASING_MAX : use the value from MovementData get from flash design panel
		 *          -1 : fade out
		 *          0  : line
		 *          1  : fade in
		 *          2  : fade in and out
		 *
		 */
		KDvoid							play ( const KDchar* pAnimationName, KDint nDurationTo = -1, KDint nDurationTween = -1, KDint nLoop = -1, KDint nTweenEasing = TWEEN_EASING_MAX );

		/**
		 *	Play animation by index, the other param is the same to play.
		 *	@param  animationIndex  the animation index you want to play
		 */
		KDvoid							playByIndex ( KDint nAnimationIndex, KDint nDurationTo = -1, KDint nDurationTween = -1, KDint nLoop = -1, KDint nTweenEasing = TWEEN_EASING_MAX );

		/**
		 *	Go to specified frame and play current movement.
		 *	You need first switch to the movement you want to play, then call this function.
		 * 
		 *	example : playByIndex(0);
		 *            gotoAndPlay(0);
		 *            playByIndex(1);
		 *            gotoAndPlay(0);
		 *            gotoAndPlay(15);
		 */
		virtual KDvoid					gotoAndPlay ( KDint nFrameIndex );

		/**
		 *	Go to specified frame and pause current movement.
		 */
		virtual KDvoid					gotoAndPause ( KDint nFrameIndex );

		/**
		 *	Pause the Process
		 */
		virtual KDvoid					pause ( KDvoid );

		/**
		 *	Resume the Process
		 */
		virtual KDvoid					resume ( KDvoid );

		/**
		 *	Stop the Process
		 */
		virtual KDvoid					stop ( KDvoid );

		/**
		 *	Get movement count
		 */
		KDint							getMovementCount ( KDvoid ) const;

		KDvoid							update ( KDfloat dt );

		/**
		 *	Get current movementID
		 *	@return The name of current movement
		 */
		std::string						getCurrentMovementID ( KDvoid ) const;

		/**
		 *	Set armature's movement event callback function
		 *	To disconnect this event, just setMovementEventCallFunc(nullptr, nullptr);
		 */
		KDvoid							setMovementEventCallFunc ( cocos2d::Object* pTarget, SEL_MovementEventCallFunc pCallFunc );

		/**
		 * Set armature's frame event callback function
		 * To disconnect this event, just setFrameEventCallFunc(nullptr, nullptr);
		 */
		KDvoid							setFrameEventCallFunc ( cocos2d::Object* pTarget, SEL_FrameEventCallFunc pCallFunc );

		virtual KDvoid					setAnimationData ( AnimationData* pData ) 
		{
			if ( m_pAnimationData != pData )
			{
				CC_SAFE_RETAIN ( pData );
				CC_SAFE_RELEASE ( m_pAnimationData );
				m_pAnimationData = pData; 
			}
		}
		virtual AnimationData*			getAnimationData ( KDvoid ) const { return m_pAnimationData; }


		/** 
		 *	Returns a user assigned Object
		 * 
		 *	Similar to userData, but instead of holding a void* it holds an object
		 *
		 *	@return A user assigned Object
		 *	@js NA
		 *	@lua NA
		 */
		virtual Object*					getUserObject ( KDvoid ) { return m_pUserObject; }

		/** 
		 *	@js NA
		 *	@lua NA
		 */
		virtual const Object*			getUserObject ( KDvoid ) const { return m_pUserObject; }

		/**
		 *	Returns a user assigned Object
		 *
		 *	Similar to UserData, but instead of holding a void* it holds an object.
		 *	The UserObject will be retained once in this method,
		 *	and the previous UserObject (if existed) will be relese.
		 *	The UserObject will be released in Node's destructure.
		 *
		 *	@param userObject    A user assigned Object
		 */
		virtual KDvoid					setUserObject ( Object* pUserObject );

	protected :

		/**
		 *	Update(float dt) will call this handler, you can handle your logic here
		 */
		KDvoid							updateHandler ( KDvoid );

		/**
		 *	Update current key frame, and process auto stop, pause
		 */
		KDvoid							updateFrameData ( KDfloat fCurrentPercent );

		/**
		 *	Emit a frame event
		 */
		KDvoid							frameEvent ( Bone* pBone, const KDchar* pFrameEventName, KDint nOriginFrameIndex, KDint nCurrentFrameIndex );

		KDbool							isIgnoreFrameEvent ( KDvoid ) const { return m_bIgnoreFrameEvent; }

		friend class Tween;

	protected :

		//! AnimationData save all MovementDatas this animation used.
		AnimationData*					m_pAnimationData;

		//! Scale the animation speed
		KDfloat							m_fSpeedScale;

		MovementData*					m_pMovementData;					//! MovementData save all MovementFrameDatas this animation used.

		Armature*						m_pArmature;						//! A weak reference of armature

		std::string						m_sMovementID;						//! Current movment's name

		KDint							m_nToIndex;							//! The frame index in MovementData->m_pMovFrameDataArr, it's different from m_iFrameIndex.

		cocos2d::Array*					m_pTweenList;

		KDbool							m_bIgnoreFrameEvent;
    
		std::queue<FrameEvent*>			m_aFrameEventQueue;

		cocos2d::Object*				m_pUserObject;

	protected :

		/**
		 *	MovementEvent CallFunc.
		 *	@param Armature* a Armature
		 *	@param MovementEventType, Event Type, like START, COMPLETE.
		 *	@param const char*, Movement ID, also called Movement Name
		 */
		SEL_MovementEventCallFunc		m_pMovementEventCallFunc;

		/**
		 *	FrameEvent CallFunc.
		 *	@param Bone*, a Bone
		 *	@param const char*, the name of this frame event
		 *	@param int, origin frame index
		 *	@param int, current frame index, animation may be delayed
		 */
		SEL_FrameEventCallFunc			m_pFrameEventCallFunc;


		cocos2d::Object*				m_pMovementEventTarget;
		cocos2d::Object*				m_pFrameEventTarget;
};

}

#endif	// __CCArmatureAnimation_h__
