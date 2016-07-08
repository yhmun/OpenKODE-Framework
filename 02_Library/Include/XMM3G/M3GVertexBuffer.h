/* --------------------------------------------------------------------------
 *
 *      File            M3GVertexBuffer.h
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

#ifndef __M3GVertexBuffer_h__
#define __M3GVertexBuffer_h__

#include "M3GObject3D.h"

class M3GVertexArray;

/**
 * @~English	VertexBuffer holds references to VertexArrays that
 *				contain the positions, colors, normals, and
 *				texture coordinates for a set of vertices.
 */
class M3GVertexBuffer : public M3GObject3D
{
	public :

        /**
         * @~English	Creates an empty VertexBuffer with default values.
         */
		M3GVertexBuffer ( KDvoid );

        /**
         * @~English	Destructs this object.
         */
		virtual ~M3GVertexBuffer ( KDvoid );

	public :

        virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

        virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

        virtual KDint  getReferences ( M3GObject3D** references ) const;

        /**
         * @~English	Gets the current color array, or null if per-vertex colors are not set.
         * @return		
         */
		M3GVertexArray*  getColors ( KDvoid ) const;

        /**
         * @~English	Retrieves the default color of this VertexBuffer.
         * @return		
         */
		KDint	getDefaultColor ( KDvoid ) const;
 
        /**
         * @~English	Gets the current normal vector array, or null if normals are not set.
         * @return		
         */
		M3GVertexArray*  getNormals ( KDfloat* scaleBias = KD_NULL ) const; 

        /**
         * @~English	Returns the current vertex position array, or null if positions are not set.
         * @param[out]	scaleBias 
         * @return		
         */
		M3GVertexArray*  getPositions ( KDfloat* scaleBias ) const;

		/**
		 * @~English	Gets the current texture coordinate array for the specified texturing unit, or null
		 *				if texture coordinates for that unit are not set.
		 * @param[in]	index
		 * @param[out]	scaleBias
		 * @return		
		 */
		M3GVertexArray*  getTexCoords ( KDint index, KDfloat* scaleBias ) const;

		/**
		 * @~English	Retrieves the current number of vertices in this VertexBuffer.
		 * @return		the number of vertices currently in this VertexBuffer, or zero if no vertex arrays are set
		 */
		KDint	getVertexCount ( KDvoid ) const;

		/**
		 * @~English	Sets the per-vertex colors for this VertexBuffer.
		 * @param[in]	colors
		 * @return		
		 */ 
		KDvoid	setColors ( M3GVertexArray* colors );

		/**
		 * @~English	Sets the color to use in absence of per-vertex colors.
		 * @param[in]	ARGB	
		 */  
		KDvoid	setDefaultColor ( KDint ARGB );

		/**
		 * @~English	Sets the normal vectors for this VertexBuffer.
		 * @param[in]	normals	
		 */   
		KDvoid	setNormals ( M3GVertexArray* normals );

		/**
		 * @~English	Sets the vertex positions for this VertexBuffer.
		 * @param[in]	positions	
		 * @param[in]	scale	
		 * @param[in]	bias	
		 */    
		KDvoid	setPositions ( M3GVertexArray* positions, KDfloat scale, KDfloat* bias );

		/**
		 * @~English	Sets the texture coordinates for the specified texturing unit.
		 * @param[in]	index	
		 * @param[in]	texCoords	
		 * @param[in]	scale	
		 * @param[in]	bias	
		 */  
		KDvoid	setTexCoords ( KDint index, M3GVertexArray* texCoords, KDfloat scale, KDfloat* bias );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GVertexBuffer* pObj ) const;

	private :

        M3GVertexArray*		m_pPositions;
        M3GVertexArray*		m_pNormals;
        M3GVertexArray*		m_pColors;
        M3GVertexArray*		m_pTexCoords [ MAX_TEXTURE_UNITS ];

        KDfloat				m_fPositionsScale;
        KDfloat				m_fPositionsBias  [ 3 ];
        KDfloat				m_fTexCoordsScale [ MAX_TEXTURE_UNITS ];
        KDfloat				m_fTexCoordsBias  [ MAX_TEXTURE_UNITS ][ 3 ];

        KDint				m_nARGB;
        mutable KDfloat		m_fNodeAlpha;
};

#endif
