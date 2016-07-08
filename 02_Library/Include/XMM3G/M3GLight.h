/* --------------------------------------------------------------------------
 *
 *      File            M3GLight.h
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

#ifndef __M3GLight_h__
#define __M3GLight_h__

#include "M3GNode.h"

/**
 * @~English	A scene graph node that represents different kinds of light sources.
 */
class M3GLight : public M3GNode
{
	public :

		/**
		 * @~English	A parameter to setMode, specifying an ambient light source.
		 */
		static const KDint	AMBIENT     = 128;

		/**
		 * @~English	A parameter to setMode, specifying a directional light source.
		 */
		static const KDint	DIRECTIONAL = 129;

		/**
		 * @~English	A parameter to setMode, specifying an omnidirectional light source.
		 */
		static const KDint	OMNI        = 130;

		/**
		 * @~English	A parameter to setMode, specifying a spot light source.
		 */
		static const KDint	SPOT        = 131;

	public :

		/**
		 * @~English	Constructs a new Light with default values.
		 */
		M3GLight ( KDvoid );

		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GLight ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current color of this Light.
		 * @return
		 */
		KDint  getColor ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current constant attenuation coefficient for this Light.
		 * @return
		 */
		KDfloat  getConstantAttenuation ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current intensity of this Light.
		 * @return
		 */
		KDfloat  getIntensity ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current linear attenuation coefficient for this Light.
		 * @return
		 */
		KDfloat  getLinearAttenuation ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current type of this Light.
		 * @return
		 */
		KDint  getMode ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current quadratic attenuation coefficient for this Light.
		 * @return
		 */
		KDfloat  getQuadraticAttenuation ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current spot angle of this Light.
		 * @return
		 */
		KDfloat  getSpotAngle ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current spot exponent for this Light.
		 */
		KDfloat  getSpotExponent ( KDvoid ) const;

		/**
		 * @~English	Sets the attenuation coefficients for this Light.
		 * @param[in]	constant
		 * @param[in]	linear
		 * @param[in]	quadratic
		 */
		KDvoid	setAttenuation ( KDfloat constant, KDfloat linear, KDfloat quadratic );

		/**
		 * @~English	Sets the color of this Light.
		 * @param[in]	RGB
		 */
		KDvoid	setColor ( KDint RGB );

		/**
		 * @~English	Sets the intensity of this Light.
		 * @param[in]	intensity
		 */
		KDvoid	setIntensity ( KDfloat intensity );

		/**
		 * @~English	Sets the type of this Light.
		 * @param[in]	mode
		 */
		KDvoid	setMode ( KDint mode );

		/**
		 * @~English	Sets the spot cone angle for this Light.
		 * @param[in]	angle
		 */
		KDvoid	setSpotAngle ( KDfloat angle );

		/**
		 * @~English	Sets the spot exponent for this Light.
		 * @param[in]	exponent
		 */
		KDvoid	setSpotExponent ( KDfloat exponent );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GLight* pObj ) const;

	private:

        KDint					m_nMode;
        KDint					m_nColor;
        KDfloat					m_fIntensity;
        KDfloat					m_fConstant;
		KDfloat					m_fLinear;
		KDfloat					m_fQuadratic;
        KDfloat					m_fAngle;
		KDfloat					m_fExponent;
};

#endif
