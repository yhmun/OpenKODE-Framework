/* --------------------------------------------------------------------------
 *
 *      File            M3GFog.h
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

#ifndef __M3GFog_h__
#define __M3GFog_h__

#include "M3GObject3D.h"

/**
 * @~English	An Appearance component encapsulating attributes for fogging.
 */
class M3GFog : public M3GObject3D
{
	public :

		/**
		 * @~English	A parameter to setMode, specifying exponential fog.
		 */
		static const KDint	EXPONENTIAL = 80;

		/**
		 * @~English	A parameter to setMode, specifying linear fog.
		 */
		static const KDint	LINEAR      = 81;

	public :

		/**
		 * @~English	Constructs a new Fog object with default values.
		 */
		M3GFog ( KDvoid );

		/**
		 * @~English	Destruct a this Fog object.
		 */
		virtual ~M3GFog ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current color of this Fog.
		 * @return	
		 */
		KDint  getColor ( KDvoid ) const;

		/**
		 * @~English	Retrieves the fog density of exponential fog.
		 * @return	
		 */
		KDfloat  getDensity ( KDvoid ) const;

		/**
		 * @~English	Retrieves the linear fog far distance.
		 * @return	
		 */
		KDfloat  getFarDistance ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current fog mode.
		 * @return	
		 */
		KDint  getMode ( KDvoid ) const;

		/**
		 * @~English	Retrieves the linear fog near distance.
		 * @return	
		 */
		KDfloat  getNearDistance ( KDvoid ) const;

		/**
		 * @~English	Sets the color of this Fog.
		 * @param[in]	RGB
		 */
		KDvoid	setColor ( KDint RGB );

		/**
		 * @~English	Sets the fog density for exponential fog.
		 * @param[in]	density
		 */
		KDvoid	setDensity ( KDfloat density );

		/**
		 * @~English	Sets the near and far distances for linear fog.
		 * @param[in]	zNear
		 * @param[in]	zFar
		 */
		KDvoid	setLinear ( KDfloat zNear, KDfloat zFar );

		/**
		 * @~English	Sets the fog mode to either linear or exponential.
		 * @param[in]	mode
		 */
		KDvoid	setMode ( KDint mode );

	public :

		static  KDvoid  render ( KDvoid );

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GFog* pObj ) const;

	private :

        KDint				m_nMode;
        KDint				m_nColor;
        KDfloat				m_fDensity;
        KDfloat				m_fNear;
		KDfloat				m_fFar;
};


#endif
