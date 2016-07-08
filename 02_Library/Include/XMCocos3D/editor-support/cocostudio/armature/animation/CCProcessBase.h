/* -----------------------------------------------------------------------------------
 *
 *      File            CCProcessBase.h
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

#ifndef __CCProcessBase_h__
#define __CCProcessBase_h__

#include "../utils/CCArmatureDefine.h"
#include "../datas/CCDatas.h"

namespace cocostudio {

enum AnimationType
{
    SINGLE_FRAME = -4,          //! the animation just have one frame
    ANIMATION_NO_LOOP,          //! the animation isn't loop

    ANIMATION_TO_LOOP_FRONT,    //! the animation loop from front
    ANIMATION_TO_LOOP_BACK,     //! the animation loop from back

    ANIMATION_LOOP_FRONT,       //! the animation loop from front
    ANIMATION_LOOP_BACK,        //! the animation loop from back

    ANIMATION_MAX,

};


class  ProcessBase : public cocos2d::Object
{
	public :

		/**
		 *	@js ctor
		 */
		ProcessBase ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ProcessBase ( KDvoid );

		/**
		 *  Play animation by animation name.
		 *
		 *  @param  durationTo The frames between two animation changing-over.
		 *          It's meaning is changing to this animation need how many frames
		 *
		 *          -1 : use the value from MovementData get from flash design panel
		 *  @param  durationTween  The frame count you want to play in the game.
		 *          if  _durationTween is 80, then the animation will played 80 frames in a loop
		 *
		 *          -1 : use the value from MovementData get from flash design panel
		 *
		 *  @param  loop   Whether the animation is loop
		 *
		 *          loop < 0 : use the value from MovementData get from flash design panel
		 *          loop = 0 : this animation is not loop
		 *          loop > 0 : this animation is loop
		 *
		 *  @param  tweenEasing Tween easing is used for calculate easing effect
		 *
		 *          TWEEN_EASING_MAX : use the value from MovementData get from flash design panel
		 *          -1 : fade out
		 *          0  : line
		 *          1  : fade in
		 *          2  : fade in and out
		 *
		 */
		virtual KDvoid			play ( KDint nDurationTo, KDint nDurationTween, KDint nLoop, KDint nTweenEasing );

		/**
		 *	Pause the Process
		 */
		virtual KDvoid			pause ( KDvoid );

		/**
		 *	Resume the Process
		 */
		virtual KDvoid			resume ( KDvoid );

		/**
		 *	Stop the Process
		 */
		virtual KDvoid			stop ( KDvoid );


		/**
		 *	You should never call this function, unless you know what you do
		 *	Update the Process, include current process, current frame and son on
		 *
		 *	@param The duration since last update
		 */
		virtual KDvoid			update ( KDfloat dt );
		 
		virtual KDint			getCurrentFrameIndex ( KDvoid );

		virtual KDvoid			setProcessScale ( KDfloat fProcessScale ) { m_fProcessScale = fProcessScale; }
		virtual KDfloat			getProcessScale ( KDvoid ) const { return m_fProcessScale; }

		virtual KDvoid			setPause ( KDbool bPause ) { m_bIsPause = bPause; }
		virtual KDbool			 isPause ( KDvoid ) const { return m_bIsPause; }

		virtual KDvoid			setComplete ( KDbool bComplete ) { m_bIsComplete = bComplete; }
		virtual KDbool		 	 isComplete ( KDvoid ) const { return m_bIsComplete; }

		virtual KDvoid			setPlaying ( KDbool bPlaying ) { m_bIsPlaying = bPlaying; }
		virtual KDbool			 isPlaying ( KDvoid ) const { return m_bIsPlaying; }

		virtual KDfloat			getCurrentPercent ( KDvoid ) const { return m_fCurrentPercent; }
		virtual KDint			getRawDuration ( KDvoid ) const { return m_nRawDuration; }

		virtual KDvoid			setAnimationInternal ( KDfloat fAnimationInternal ) { m_fAnimationInternal = fAnimationInternal; }
		virtual KDfloat			getAnimationInternal ( KDvoid ) const { return m_fAnimationInternal; }

	protected :

		virtual KDvoid			gotoFrame ( KDint nFrameIndex );

		/**
		 *	Update(float dt) will call this handler, you can handle your logic here
		 */
		virtual KDvoid			updateHandler ( KDvoid ) { };

	protected :

		//! Scale the process speed
		KDfloat					m_fProcessScale;

		//! Set and get whether the aniamtion is pause
		KDbool					m_bIsPause;

		//! Set and get whether the aniamtion is complete
		KDbool					m_bIsComplete;

		//! Set and get whether the aniamtion is playing
		KDbool					m_bIsPlaying;

		//! Current percent this process arrived
		KDfloat					m_fCurrentPercent;

		//! The raw duration
		KDint					m_nRawDuration;

		//! The animation whether or not loop
		AnimationType			m_eLoopType;

		//! The tween easing effect
		TweenType				m_eTweenEasing;

		//! The animation update speed
		KDfloat					m_fAnimationInternal;

	protected :

		//! The durantion frame count will run
		KDint					m_nDurationTween;

		//! Current frame this process arrived, this frame is tween frame
		KDfloat					m_fCurrentFrame;
		//! Frame index it the time line
		KDint					m_nCurFrameIndex;

		//! Next frame this process need run to
		KDint					m_nNextFrameIndex;

		KDbool					m_bIsLoopBack;
};

}

#endif	// __CCProcessBase_h__
