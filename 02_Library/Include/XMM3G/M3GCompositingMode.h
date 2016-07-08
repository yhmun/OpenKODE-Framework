/* --------------------------------------------------------------------------
 *
 *      File            M3GCompositingMode.h
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

#ifndef __M3GCompositingMode_h__
#define __M3GCompositingMode_h__

#include "M3GObject3D.h"

/**
 * @~English	An Appearance component encapsulating per-pixel comositing attributes.
 */
class M3GCompositingMode : public M3GObject3D
{
	public :

		/**
		 * @~English	Selects the alpha blend mode.
		 */
		static const KDint	ALPHA        = 64;

		/**
		 * @~English	Selects the alpha blend mode.
		 */
		static const KDint	ALPHA_ADD    = 65;

		/**
		 * @~English	Selects the basic modulating blending mode; source pixels are multiplied with the destination pixels.
		 */
		static const KDint	MODULATE     = 66;

		/**
		 * @~English	Selects the brighter modulating blending mode.
		 */
		static const KDint	MODULATE_X2  = 67;

		/**
		 * @~English	Selects the replace mode.
		 */
		static const KDint	REPLACE      = 68;

	public :

		/**
		 * @~English	Constructs a CompositingMode object with default values.
		 */
		M3GCompositingMode ( KDvoid );

		/**
		 * @~English	Destuct a CompositingMode object.
		 */
		virtual ~M3GCompositingMode ( KDvoid );

	public :

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current alpha testing threshold.
		 * @return
		 */
		KDfloat  getAlphaThreshold ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current frame buffer blending mode.
		 * @return
		 * @return
		 */
		KDint  getBlending ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current depth offset slope factor.
		 * @return
		 */
		KDfloat  getDepthOffsetFactor ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current constant depth offset.
		 * @return
		 */
		KDfloat  getDepthOffsetUnits ( KDvoid ) const;

		/**
		 * @~English	Queries whether alpha writing is enabled.
		 * @return
		 */
		KDbool	isAlphaWriteEnabled ( KDvoid ) const;

		/**
		 * @~English	Queries whether color writing is enabled.
		 * @return
		 */
		KDbool	isColorWriteEnabled ( KDvoid ) const;

		/**
		 * @~English	Queries whether depth testing is enabled.
		 * @return
		 * @return
		 */
		KDbool	isDepthTestEnabled ( KDvoid ) const;

		/**
		 * @~English	Queries whether depth writing is enabled.
		 * @return
		 */
		KDbool	isDepthWriteEnabled ( KDvoid ) const;

		/**
		 * @~English	Sets the threshold value for alpha testing.
		 * @param[in]	threshold
		 */
		KDvoid	setAlphaThreshold ( KDfloat threshold );

		/**
		 * @~English	Enables or disables writing of fragment alpha values into the color buffer.
		 * @param[in]	enable
		 */
		KDvoid	setAlphaWriteEnable ( KDbool enable );

		/**
		 * @~English	Selects a method of combining the pixel to be rendered with the pixel already in the frame buffer.
		 * @param[in]	mode
		 */
		KDvoid	setBlending ( KDint mode );

		/**
		 * @~English	Enables or disables writing of fragment color values into the color buffer.
		 * @param[in]	enable
		 */
		KDvoid	setColorWriteEnable ( KDbool enable );

		/**
		 * @~English	Defines a value that is added to the screen space Z coordinate of a pixel immediately before depth test and depth write.
		 * @param[in]	factor
		 * @param[in]	units
		 */
		KDvoid	setDepthOffset ( KDfloat factor, KDfloat units );

		/**
		 * @~English	Enables or disables depth testing.
		 * @param[in]	enable
		 */
		KDvoid	setDepthTestEnable ( KDbool enable );

		/**
		 * @~English	Enables or disables writing of fragment depth values into the depth buffer.
		 * @param[in]	enable
		 */
		KDvoid	setDepthWriteEnable ( KDbool enable );

	public :

		static  KDvoid  render ( KDvoid );

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GCompositingMode* pObj ) const;

	private :

        KDint				m_nBlendingMode;
        KDfloat				m_fAlphaThreshold;
        KDfloat				m_fOffsetFactor;
		KDfloat				m_fOffsetUnits;
        KDbool				m_bDepthTest;
        KDbool				m_bDepthWrite;
        KDbool				m_bColorWrite;
        KDbool				m_bAlphaWrite;
};

#endif
