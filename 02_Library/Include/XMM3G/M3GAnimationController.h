/* --------------------------------------------------------------------------
 *
 *      File            M3GAnimationController.h
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

#ifndef __M3GAnimationController_h__
#define __M3GAnimationController_h__

#include "M3GObject3D.h"

/**
 * @~English	Controls the position, speed and weight of an animation sequence.
 */
class M3GAnimationController : public M3GObject3D
{
	public :

		/**
		 * @~English	Creates a new AnimationController object.
		 */
		M3GAnimationController ( KDvoid );

		/**
		 * @~English	Destruct a AnimationController object.
		 */
		virtual ~M3GAnimationController ( KDvoid );

	public :

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Retrieves the ending time of the current active interval of this animation controller, in world time units.
		 * @return	
		 */
		KDint  getActiveIntervalEnd ( KDvoid ) const;

		/**
		 * @~English	Retrieves the starting time of the current active interval of this animation controller, in world time units.
		 * @return	
		 */
		KDint  getActiveIntervalStart ( KDvoid ) const;

		/**
		 * @~English	Retrieves the sequence time that corresponds to the given world time.
		 * @param[in]	worldTime
		 * @return	
		 */
		KDfloat  getPosition ( KDint worldTime ) const;

		/**
		 * @~English	Returns the current reference world time.
		 * @return	
		 */
		KDint  getRefWorldTime ( KDvoid ) const;

		/**
		 * @~English	Retrieves the currently set playback speed of this animation controller.
		 * @return	
		 */
		KDfloat  getSpeed ( KDvoid ) const;

		/**
		 * @~English	Retrieves the currently set blending weight for this animation controller.
		 * @return	
		 */
		KDfloat  getWeight ( KDvoid ) const;

		/**
		 * @~English	Sets the world time interval during which this animation controller is active.
		 * @param[in]	start
		 * @param[in]	end
		 */
		KDvoid  setActiveInterval ( KDint start, KDint end );

		/**
		 * @~English	Sets a new playback position, relative to world time, for this animation controller.
		 * @param[in]	sequenceTime
		 * @param[in]	worldTime
		 */
		KDvoid  setPosition ( KDfloat sequenceTime, KDint worldTime );

		/**
		 * @~English	Sets a new playback speed for this animation.
		 * @param[in]	speed
		 * @param[in]	worldTime
		 */
		KDvoid  setSpeed ( KDfloat speed, KDint worldTime );

		/**
		 * @~English	Sets the blending weight for this animation controller.
		 * @param[in]	weight
		 */
		KDvoid  setWeight ( KDfloat weight );

	public :

		KDbool isActive ( KDint worldTime ) const;

	protected :

		KDvoid  _duplicate ( M3GAnimationController* pObj ) const;

	private :

		KDfloat				m_fActiveIntervalStart;
        KDfloat				m_fActiveIntervalEnd;
        KDfloat				m_fBlendingWeight;
        KDfloat				m_fSpeed;
        KDfloat				m_fRefWorldTime;
        KDfloat				m_fRefSequenceTime;
};

#endif
