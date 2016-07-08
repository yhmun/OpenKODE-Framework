/* --------------------------------------------------------------------------
 *
 *      File            M3GVertexArray.h 
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

#ifndef __M3GVertexArray_h__
#define __M3GVertexArray_h__

#include "M3GObject3D.h"

class M3GMatrix;
class M3GBoneWeight;

/**
 * @~English	An array of integer vectors representing vertex positions, normals, colors, or texture coordinates.
 */
class M3GVertexArray : public M3GObject3D
{
	protected :

		M3GVertexArray ( KDvoid );

	public :

		/**
		 * @~English	Constructs a new VertexArray with the given dimensions.
         * @param[in]	numVertices    
         * @param[in]	numComponents  
         * @param[in]	componentSize  
		 */
		M3GVertexArray ( KDint numVertices, KDint numComponents, KDint componentSize );

		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GVertexArray ( KDvoid );

	public :

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Returns a range of 8-bit vertex attributes.
         * @param[in]	firstVertex    
         * @param[in]	numVertices  
         * @param[out]	values  
		 */
		KDvoid	get ( KDint firstVertex, KDint numVertices, KDubyte* values ) const;

		/**
		 * @~English	Returns a range of 16-bit vertex attributes.
         * @param[in]	firstVertex    
         * @param[in]	numVertices  
         * @param[out]	values  
		 */
		KDvoid	get ( KDint firstVertex, KDint numVertices, KDshort* values ) const;

		/**
		 * @~English	Returns a range of 32-bit float vertex attributes.
         * @param[in]	firstVertex    
         * @param[in]	numVertices  
         * @param[out]	values  
		 */
		KDvoid	get ( KDint firstVertex, KDint numVertices, KDfloat* values ) const;

		/**
		 * @~English	Returns a scale-biased range of 32-bit float vertex attributes.
         * @param[in]	firstVertex    
         * @param[in]	numVertices  
		 * @param[in]	scale 
		 * @param[in]	bias 
         * @param[out]	values  
		 */
		KDvoid  get ( KDint firstVertex, KDint numVertices, KDfloat scale, KDfloat* bias, KDfloat* values ) const;

		/**
		 * @~English	Returns the number of components per vertex.
		 * @return
		 */
		KDint	getComponentCount ( KDvoid ) const;

		/**
		 * @~English	Returns the data type (size) of vertex components.
		 * @return
		 */ 
		KDint	getComponentType ( KDvoid ) const;

		/**
		 * @~English	Returns the number of vertices in this array.
		 * @return
		 */
		KDint	getVertexCount ( KDvoid ) const;

		/**
		 * @~English	Copies in an array of 8-bit vertex attributes.
         * @param[in]	firstVertex    
         * @param[in]	numVertices  
         * @param[in]	values  
		 */ 
		KDvoid	set ( KDint firstVertex, KDint numVertices, const KDubyte* values );

		/**
		 * @~English	Copies in an array of 16-bit vertex attributes.
         * @param[in]	firstVertex    
         * @param[in]	numVertices  
         * @param[in]	values  
		 */
		KDvoid	set ( KDint firstVertex, KDint numVertices, const KDshort* values );

		/**
		 * @~English	Copies in an array of 32-bit float vertex attributes.
         * @param[in]	firstVertex    
         * @param[in]	numVertices  
         * @param[in]	values  
		 */
		KDvoid	set ( KDint firstVertex, KDint numVertices, const KDfloat* values );

	protected :

		KDvoid  _duplicate ( M3GVertexArray* pObj ) const;

		KDvoid  get ( KDint firstVertex, KDint numVertices, KDint valueSize, KDvoid* values ) const;

		KDvoid	set ( KDint firstVertex, KDint numVertices, KDint valueSize, const KDvoid* values );

	public :

		KDvoid  updateOpenGLData ( const KDvoid* value ) const;

		KDuint  getOpenGLVBO ( KDvoid ) const;

		KDuint  getOpenGLFormat ( KDvoid ) const;

		KDvoid  convert ( KDint componentSize );

        KDvoid  setSkinning ( const M3GVertexArray*								basePositions,
							  const std::vector< std::vector<M3GBoneWeight> >&	boneIndices,
							  const std::vector<M3GMatrix>&						matrixPalette );

        KDvoid  setMorphing ( const M3GVertexArray*							basePositions,
							  const std::vector<const M3GVertexArray*>&		morphTargetPositions,
							  const std::vector<KDfloat>&					morphWeights );

	protected :

		KDvoid	init ( KDint numVertices, KDint numComponents, KDint componentSize );
	
	protected :

		KDuint			m_uID;
		KDint			m_nVertexCount;
		KDint			m_nComponentCount;
		KDint			m_nComponentSize;

		union 
		{
			KDubyte*	m_pValues8;
			KDshort*	m_pValues16;
			KDfloat*	m_pValues32;
		};	

		friend class M3GLoader;
};


#endif
