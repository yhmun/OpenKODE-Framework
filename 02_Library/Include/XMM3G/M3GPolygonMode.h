/* --------------------------------------------------------------------------
 *
 *      File            M3GPolygonMode.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010 UEDA.Takashi
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com  
 *
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

#ifndef __M3GPolygonMode_h__
#define __M3GPolygonMode_h__

#include "M3GObject3D.h"

/**
 * @~English	An Appearance component encapsulating polygon-level attributes.
 */
class M3GPolygonMode : public M3GObject3D
{
	public:

		/**
		 * @~English	A parameter to setCulling, specifying that the back-facing side of a polygon is not to be drawn.
		 */
		static const KDint	CULL_BACK    = 160;

		/**
		 * @~English	A parameter to setCulling, specifying that the front-facing side of a polygon is not to be drawn.
		 */
		static const KDint	CULL_FRONT   = 161;

		/**
		 * @~English	A parameter to setCulling, specifying that both faces of a polygon are to be drawn.
		 */
		static const KDint	CULL_NONE    = 162;

		/**
		 * @~English	A parameter to setShading, specifying that flat shading is to be used.
		 */
		static const KDint	SHADE_FLAT   = 164;

		/**
		 * @~English	A parameter to setShading, specifying that smooth shading is to be used.
		 */
		static const KDint	SHADE_SMOOTH = 165;

		/**
		 * @~English	A parameter to setWinding, specifying that a polygon having its vertices in counter-clockwise order
		 *				in screen space is to be considered front-facing.
		 */
		static const KDint	WINDING_CCW  = 168;

		/**
		 * @~English	A parameter to setWinding, specifying that a polygon having its vertices in clockwise order 
		 *				in screen space is to be considered front-facing.
		 */
		static const KDint	WINDING_CW   = 169;

	public:

		/**
		 * @~English	Constructs a PolygonMode object with default values.
		 */
		M3GPolygonMode ( KDvoid );

		/**
		 * @~English	Destructs this object.
		 */
		virtual ~M3GPolygonMode ( KDvoid );

	public :

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current polygon culling mode.
		 * @return	
		 */
		KDint	getCulling ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current polygon shading mode.
		 * @return	
		 */
		KDint	getShading ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current polygon winding mode.
		 * @return	
		 */
		KDint	getWinding ( KDvoid ) const;

		/**
		 * @~English	Queries whether local camera lighting is enabled.
		 * @return	
		 */
		KDbool	isLocalCameraLightingEnabled ( KDvoid ) const;

		/**
		 * @~English	Queries whether perspective correction is enabled.
		 * @return	
		 */
		KDbool	isPerspectiveCorrectionEnabled ( KDvoid ) const;

		/**
		 * @~English	Queries whether two-sided lighting is enabled.
		 */
		KDbool	isTwoSidedLightingEnabled ( KDvoid ) const;

		/**
		 * @~English	Sets the polygon culling mode.
		 * @param[in]	mode
		 */
		KDvoid	setCulling ( KDint mode );

		/**
		 * @~English	Enables or disables local camera lighting.
		 * @param[in]	enable
		 */
		KDvoid	setLocalCameraLightingEnable ( KDbool enable );

		/**
		 * @~English	Enables or disables perspective correction.
		 * @param[in]	enable
		 */
		KDvoid	setPerspectiveCorrectionEnable ( KDbool enable );

		/**
		 * @~English	Sets the polygon shading mode.
		 * @param[in]	mode
		 */
		KDvoid	setShading ( KDint mode );

		/**
		 * @~English	Enables or disables two-sided lighting.
		 * @param[in]	enable
		 */
		KDvoid	setTwoSidedLightingEnable ( KDbool enable );

		/**
		 * @~English	Sets the polygon winding mode to clockwise or counter-clockwise.
		 * @param[in]	mode
		 */
		KDvoid	setWinding ( KDint mode );

	public :

		static  KDvoid  render ( KDvoid );

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GPolygonMode* pObj ) const;

	private :

        KDint					m_nCulling;
        KDint					m_nWinding;
        KDint					m_nShading;
		KDbool					m_bTwoSidedLighting;
		KDbool					m_bLocalCameraLighting;
        KDbool					m_bPerspectiveCorrection;
};

#endif
