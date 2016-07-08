/* --------------------------------------------------------------------------
 *
 *      File            M3GAppearance.h
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

#ifndef __M3GAppearance_h__
#define __M3GAppearance_h__

#include <vector>

#include "M3GObject3D.h"

class M3GCompositingMode;
class M3GPolygonMode;
class M3GFog;
class M3GMaterial;
class M3GTexture2D;

/**
 * @~English	set of component objects that define the rendering attributes of a Mesh or Sprite3D.
 */
class M3GAppearance : public M3GObject3D
{
	public :

		/**
		 * @~English	Constructs an Appearance object with default values.
		 */
		M3GAppearance ( KDvoid );

		/**
		 * @~English	Destruct an Appearance object.
		 */
		virtual ~M3GAppearance ( KDvoid );

	public :

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Returns the current CompositingMode for this Appearance.
		 * @return		
		 */
		M3GCompositingMode*  getCompositingMode ( KDvoid ) const;

		/**
		 * @~English	Returns the current fogging attributes for this Appearance.
		 * @return		
		 */
		M3GFog*  getFog ( KDvoid ) const;

		/**
		 * @~English	Gets the current rendering layer for this Appearance.
		 * @return		
		 */
		KDint	getLayer ( KDvoid ) const;

		/**
		 * @~English	Returns the current Material for this Appearance.
		 * @return		
		 */
		M3GMaterial*  getMaterial ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current texture image and its attributes for the given texturing unit.
		 * @return		
		 */
		M3GPolygonMode*  getPolygonMode ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current texture image and its attributes for the given texturing unit.
         * @param[in]	index
		 * @return		
		 */
		M3GTexture2D*  getTexture ( KDint index ) const;

		/**
		 * @~English	Sets the CompositingMode to use for this Appearance.
         * @param[in]	compositingMode
		 */
		KDvoid	setCompositingMode ( M3GCompositingMode* compositingMode );

		/**
		 * @~English	Sets the fogging attributes to use for this Appearance.
         * @param[in]	fog
		 */
		KDvoid	setFog ( M3GFog* fog );

		/**
		 * @~English	Sets the rendering layer for this Appearance.
         * @param[in]	layer
		 */
		KDvoid	setLayer ( KDint layer );

		/**
		 * @~English	Sets the Material to use when lighting this Appearance.
         * @param[in]	material    
		 */
		KDvoid	setMaterial ( M3GMaterial* material );

		/**
		 * @~English	Sets the PolygonMode to use for this Appearance.
         * @param[in]	polygonMode    
		 */
		KDvoid	setPolygonMode ( M3GPolygonMode* polygonMode );

		/**
		 * @~English	Sets the texture image and its attributes for the given texturing unit.
         * @param[in]	index    
         * @param[in]	texture  
		 */
		KDvoid	setTexture ( KDint index, M3GTexture2D* texture );

	public :

		static  KDvoid  render ( KDvoid );

		virtual KDvoid  render ( M3GRenderState& state ) const; 

		KDint  getLayer2 ( KDvoid ) const;

	protected :

		KDvoid  _duplicate ( M3GAppearance* pObj ) const;

	private :

        KDint						m_nLayer;
        M3GPolygonMode*				m_pPolygonMode;
        M3GCompositingMode*			m_pCompositingMode;
        M3GMaterial*				m_pMaterial;
        M3GFog*						m_pFog;
        M3GTexture2D*				m_pTextures [ MAX_TEXTURE_UNITS ];
};

#endif
