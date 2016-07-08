/* --------------------------------------------------------------------------
 *
 *      File            M3GKeyframeSequence.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010 UEDA.Takashi
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com     
 *
 * --------------------------------------------------------------------------
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __M3GKeyframeSequence_h__
#define __M3GKeyframeSequence_h__

#include "M3GObject3D.h"

class M3GKeyframe;

/**
 * @~English	Encapsulates animatin data as a sequence of time-stamped, vector-valued keyframes.
 */
class M3GKeyframeSequence : public M3GObject3D
{
	public :

		/**
		 * @~English	A parameter to setRepeatMode, specifying that this sequence is to be played back just once and not repeated.
		 */
		static const KDint	CONSTANT = 192;

		/**
		 * @~English	A constructor parameter that specifies linear interpolation between keyframes.
		 */
		static const KDint	LINEAR   = 176;

		/**
		 * @~English	A parameter to setRepeatMode, specifying that this sequence is to be repeated indefinitely.
		 */
		static const KDint	LOOP     = 193;

		/**
		 * @~English	A constructor parameter that specifies spherical linear interpolation of quaternions.
		 */
		static const KDint	SLERP    = 177;

		/**
		 * @~English	A constructor parameter that specifies spline interpolation between keyframes.
		 */
		static const KDint	SPLINE   = 178;

		/**
		 * @~English	A constructor parameter that specifies spline interpolation of quaternions.
		 */
		static const KDint	SQUAD    = 179;

		/**
		 * @~English	A constructor parameter that specifies stepping from one keyframe value to the next.
		 */
		static const KDint	STEP     = 180;

	protected :

		M3GKeyframeSequence ( KDvoid );

	public :

		/**
		 * @~English	Constructs a new keyframe sequence with specified interpolation method,
		 *				number of components per keyframe, and number of keyframes.
		 */
		M3GKeyframeSequence ( KDint numKeyframes, KDint numComponents, KDint interpolation );

		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GKeyframeSequence ( KDvoid );

	public :

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Returns the number of components per keyframe in this sequence.
		 * @return	
		 */
		KDint  getComponentCount ( KDvoid ) const;

		/**
		 * @~English	Gets the duration of this sequence.
		 * @return	
		 */
		KDint  getDuration ( KDvoid ) const;

		/**
		 * @~English	Returns the type of interpolation for this sequence.
		 * @return	
		 */
		KDint  getInterpolationType ( KDvoid ) const;

		/**
		 * @~English	Retrieves the time stamp and value of a single keyframe.
		 * @param[in]	index
		 * @param[out]	value
		 * @return	
		 */
		KDint  getKeyframe ( KDint index, KDfloat* value ) const;

		/**
		 * @~English	Returns the total number of keyframes in this sequence.
		 * @return	
		 */
		KDint  getKeyframeCount ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current repeat mode of this KeyframeSequence.
		 * @return	
		 */
		KDint  getRepeatMode ( KDvoid ) const;

		/**
		 * @~English	Returns the first keyframe of the current valid range for this sequence.
		 * @return	
		 */
		KDint  getValidRangeFirst ( KDvoid ) const;

		/**
		 * @~English	Returns the last keyframe of the current valid range for this sequence.
		 * @return	
		 */
		KDint  getValidRangeLast ( KDvoid ) const;

		/**
		 * @~English	Sets the duration of this sequence in sequence time units.
		 * @param[in]	duration
		 */
		KDvoid	setDuration ( KDint duration );

		/**
		 * @~English	Sets the time position and value of the specified keyframe.
		 * @param[in]	index
		 * @param[in]	time
		 * @param[in]	value
		 */
		KDvoid	setKeyframe ( KDint index, KDint time, const KDfloat* value );

		/**
		 * @~English	Sets the repeat mode of this KeyframeSequence.
		 * @param[in]	mode
		 */
		KDvoid	setRepeatMode ( KDint mode );

		/**
		 * @~English	Selects the range of keyframes that are included in the animation.
		 * @param[in]	first
		 * @param[in]	last
		 */
		KDvoid	setValidRange ( KDint first, KDint last );

	public :

		KDvoid	sample ( KDint sequence, KDfloat* value ) const;

	protected :

		KDvoid  init ( KDint numKeyframes, KDint numComponents, KDint interpolation );

		KDvoid  _duplicate ( M3GKeyframeSequence* pObj ) const;

	private :

        KDint						m_nRepeatMode;
        KDint						m_nInterpType;
        KDint						m_nDuration;
        KDint						m_nRangeFirst;
		KDint						m_nRangeLast;
        KDint						m_nComponentCount;
        KDint						m_nKeyframeCount;
        std::vector<M3GKeyframe>	m_vKeyframes;

		friend class M3GLoader;
};

#endif
