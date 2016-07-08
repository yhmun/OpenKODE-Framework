/* --------------------------------------------------------------------------
 *
 *      File            M3GMaterial.h
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

#ifndef __M3GMaterial_h__
#define __M3GMaterial_h__

#include "M3GObject3D.h"

/**
 * @~English	An Appearance component encapsulating material attributes for lighting computations.
 */
class M3GMaterial : public M3GObject3D
{
	public :

        /**
         * @~English	A parameter to setColor and getColor, specifying that the ambient color component.
         */
		static const KDint	AMBIENT  = 1<<10;

        /**
         * @~English	A parameter to setColor and getColor, specifying that the diffuse color component.
         */
		static const KDint	DIFFUSE  = 1<<11;

        /**
         * @~English	A parameter to setColor and getColor, specifying that the emissive color component.
         */
		static const KDint	EMISSIVE = 1<<12;

        /**
         * @~English	A parameter to setColor and getColor, specifying that the specular color component.
         */
		static const KDint	SPECULAR = 1<<13;

	public :

        /**
         * @~English	Creates a Material object with default values.
         */
		M3GMaterial ( KDvoid );

        /**
         * @~English	Destruct this object.
         */
		virtual ~M3GMaterial ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

        /**
         * @~English	Gets the value of the specified color component of this Material.
         * @param[in]	target  
		 * @return		
         */
		KDint	getColor ( KDint target ) const;

        /**
         * @~English	Gets the current shininess of this Material.
		 * @return		
         */
		KDfloat getShininess ( KDvoid ) const;

        /**
         * @~English	Queries whether vertex color tracking is enabled.
		 * @return		
         */
		KDbool	isVertexColorTrackingEnabled ( KDvoid ) const;

        /**
         * @~English	Sets the given value to the specified color component(s) of this Material.
         * @param[in]	target    
         * @param[in]	ARGB    
         */
		KDvoid	setColor ( KDint target, KDint ARGB );

        /**
         * @~English	Sets the shininess of this Material.
         * @param[in]	shininess    
         */
		KDvoid	setShininess ( KDfloat shininess );

        /**
         * @~English	Enables or disables vertex color tracking.
         * @param[in]	enable    
         */
		KDvoid	setVertexColorTrackingEnable ( KDbool enable );

	public :

		static  KDvoid  render ( KDvoid );

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GMaterial* pObj ) const;

	private :

        KDbool			m_bVertexColorTracking;
        KDint			m_nAmbientColor;
        KDint			m_nDiffuseColor;
        KDint			m_nEmissiveColor;
        KDint			m_nSpecularColor;
        KDfloat			m_fShininess;
};


#endif
