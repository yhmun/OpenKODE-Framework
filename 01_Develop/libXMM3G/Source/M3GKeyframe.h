/* --------------------------------------------------------------------------
 *
 *      File            M3GKeyframe.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 UEDA.Takashi
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __M3GKeyframe_h__
#define __M3GKeyframe_h__

/**
 * @internal
 * @~English	Represent 1 keyframe (time and data)
 */
class M3GKeyframe
{
	public :

		/**
		 * @internal
		 * @~English	Create a new empty Keyframe object
		 */
		M3GKeyframe ( KDvoid );

		/**
		 * @internal
		 * @~English	Create a new Keyframe object.
		 * @param[in]	time	time of this keyframe.
		 * @param[in]	value	value of this keyframe.
		 */
		M3GKeyframe ( KDint time, KDfloat* value );

		/**
		 * @internal
		 * @~English	Destruct this Keyframe object.
		 */
		~M3GKeyframe ( KDvoid );

	public :

		KDint				m_nTime;    
		KDfloat*			m_pValue;   
};

/**
 * @internal
 * @~English	Perform step interpolation.  
 * @param[in]	s				Blending weight of keyframe k0 and keyframe k1.
 * @param[in]	k0				Keyframe k0.
 * @param[in]	k1				Kefyrame k1.
 * @param[in]	componentCount	Component count of keyframe value.
 * @param[out]	value			Output memory for writing interpolated value.
 */
KDvoid  m3gStep ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, KDint componentCount, KDfloat* value );

/**
 * @internal
 * @~English	Perform linear interpolation.  
 * @param[in]	s				Blending weight of keyframe k0 and keyframe k1.
 * @param[in]	k0				Keyframe k0.
 * @param[in]	k1				Kefyrame k1.
 * @param[in]	componentCount	Component count of keyframe value.
 * @param[out]	value			Output memory for writing interpolated value.
 */
KDvoid  m3gLinear ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, KDint componentCount, KDfloat* value );

/**
 * @internal
 * @~English	Perform spherical linear interpolation.  
 * @param[in]	s				Blending weight of keyframe k0 and keyframe.
 * @param[in]	k0				Keyframe k0.
 * @param[in]	k1				Kefyrame k1.
 * @param[in]	componentCount	Component count of keyframe value.
 * @param[out]	value			Output memory for writing interpolated value.
 */
KDvoid  m3gSlerp ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, KDint componentCount, KDfloat* value );

/*
 * @internal
 * @~English	Perform spline interpolation.  
 * @param[in]	s				Blending weight of keyframe k1 and keyframe k2.
 * @param[in]	k0				Keyframe k0.
 * @param[in]	k1				Kefyrame k1.
 * @param[in]	k2				Keyframe k2.
 * @param[in]	k3				Kefyrame k3.
 * @param[in]	componentCount	Component count of keyframe value.
 * @param[out]	value			Output memory for writing interpolated value.
 */
KDvoid  m3gSpline ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, const M3GKeyframe& k2, const M3GKeyframe& k3, KDint componentCount, KDfloat* value );

/*
 * @internal
 * @~English	Perform spline interpolation of quaternions.  
 * @param[in]	s				Blending weight of keyframe k1 and keyframe k2.
 * @param[in]	k0				Keyframe k0.
 * @param[in]	k1				Kefyrame k1.
 * @param[in]	k2				Keyframe k2.
 * @param[in]	k3				Kefyrame k3.
 * @param[in]	componentCount	Component count of keyframe value.
 * @param[out]	value			Output memory for writing interpolated value.
 */
KDvoid  m3gSquad ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, const M3GKeyframe& k2, const M3GKeyframe& k3, KDint componentCount, KDfloat* value );


#endif