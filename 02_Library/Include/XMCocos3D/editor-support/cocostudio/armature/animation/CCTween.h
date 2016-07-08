/* -----------------------------------------------------------------------------------
 *
 *      File            CCTween.h
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

#ifndef __CCTween_h__
#define __CCTween_h__

#include "CCProcessBase.h"
#include "../utils/CCTweenFunction.h"

namespace cocostudio {

class Bone;
class ArmatureAnimation;

class  Tween : public ProcessBase
{
	public :

		/**
		 *	Create with a Bone
		 *	@param bone the Bone Tween will bind to
		 */
		static Tween*		create ( Bone* pBone );

	public :

		/**
		 * @js ctor
		 */
		Tween ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Tween ( KDvoid );

		/**
		 *	Init with a Bone
		 *	@param bone the Bone Tween will bind to
		 */
		virtual KDbool						init ( Bone* pBone );

		using ProcessBase::play;

		/**
		 *  Start the Process
		 *
		 *  @param  movementBoneData  the MovementBoneData include all FrameData
		 *  @param  durationTo the number of frames changing to this animation needs.
		 *  @param  durationTween  the number of frames this animation actual last.
		 *
		 *  @param  loop   whether the animation is loop
		 *
		 *         loop < 0 : use the value from MovementData get from Action Editor
		 *         loop = 0 : this animation is not loop
		 *         loop > 0 : this animation is loop
		 *
		 *  @param  tweenEasing    tween easing is used for calculate easing effect
		 *
		 *         TWEEN_EASING_MAX : use the value from MovementData get from Action Editor
		 *         -1 : fade out
		 *         0  : line
		 *         1  : fade in
		 *         2  : fade in and out
		 *
		 */
		virtual KDvoid						play ( MovementBoneData* pMovementBoneData, KDint nDurationTo, KDint nDurationTween, KDint nLoop, KDint nTweenEasing );

		inline KDvoid						setAnimation ( ArmatureAnimation* pAnimation ) { m_pAnimation = pAnimation; }
		inline ArmatureAnimation*			getAnimation ( KDvoid ) const { return m_pAnimation; }

		virtual KDvoid						gotoAndPlay ( KDint nFrameIndex );
		virtual KDvoid						gotoAndPause ( KDint nFrameIndex );

		virtual KDvoid						setMovementBoneData ( MovementBoneData* pData ) { m_pMovementBoneData = pData; }
		virtual const MovementBoneData*		getMovementBoneData ( KDvoid ) const { return m_pMovementBoneData; }

	protected :

		/**
		 *	Update(float dt) will call this handler, you can handle your logic here
		 */
		virtual KDvoid						updateHandler ( KDvoid );

		/**
		 *	Calculate which frame arrived, and if current frame have event, then call the event listener
		 */
		virtual KDfloat						updateFrameData ( KDfloat fCurrentPercent );

		/**
		 *	Calculate the between value of _from and _to, and give it to between frame data
		 */
		virtual KDvoid						setBetween ( FrameData* pFrom, FrameData* pTo, KDbool bLimit = true );

		/**
		 *	According to the percent to calculate current FrameData with tween effect
		 */
		virtual FrameData*					tweenNodeTo ( KDfloat fPercent, FrameData* pNode = nullptr );

		/**
		 *	According to the percent to calculate current color with tween effect
		 */
		virtual KDvoid						tweenColorTo ( KDfloat fPercent, FrameData* pNode );

		/**
		 *	Update display index and process the key frame event when arrived a key frame
		 */
		virtual KDvoid						arriveKeyFrame ( FrameData* pKeyFrameData );

	protected :

		//! A weak reference to the current MovementBoneData. The data is in the data pool
		MovementBoneData*					m_pMovementBoneData;

		FrameData*							m_pTweenData;					//! The computational tween frame data, //! A weak reference to the Bone's tweenData
		FrameData*							m_pFrom;						//! From frame data, used for calculate between value
		FrameData*							m_pTo;							//! To frame data, used for calculate between value
		FrameData*							m_pBetween;						//! Between frame data, used for calculate current FrameData(m_pNode) value

		Bone*								m_pBone;						//! A weak reference to the Bone

		TweenType							m_eFrameTweenEasing;			//! Dedermine which tween effect current frame use

		KDint								m_nBetweenDuration;				//! Current key frame will last _betweenDuration frames
		KDint								m_nTotalDuration;

		KDint								m_nFromIndex;				    //! The current frame index in FrameList of MovementBoneData, it's different from m_iFrameIndex
		KDint								m_nToIndex;						//! The next frame index in FrameList of MovementBoneData, it's different from m_iFrameIndex

		ArmatureAnimation*					m_pAnimation;

		KDbool								m_bPassLastFrame;				//! If current frame index is more than the last frame's index
};

}

#endif	// __CCTween_h__
