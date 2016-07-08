/* --------------------------------------------------------------------------
 *
 *      File            M3GMesh.h
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

#ifndef __M3GMesh_h__
#define __M3GMesh_h__

#include "M3GNode.h"

class M3GVertexBuffer;
class M3GIndexBuffer;
class M3GAppearance;
class M3GVector;
class M3GRayIntersection;

/**
 * @~English	A scene graph node that represents a 3D object defined as a polygonal surface.
 */
class M3GMesh : public M3GNode
{
	protected :

		M3GMesh ( KDvoid );

	public :

		/**
		 * @~English	Constructs a new Mesh with the given VertexBuffer and submeshes.
		 * @param[in]	vertices
		 * @param[in]	numSubmesh
		 * @param[in]	submeshes
		 * @param[in]	numAppearances
		 * @param[in]	appearances
		 */
		M3GMesh ( M3GVertexBuffer* vertices, KDint numSubmesh, M3GIndexBuffer** submeshes, KDint numAppearances, M3GAppearance** appearances );

		/**
		 * @~English	Constructs a new Mesh consisting of only one submesh.
		 * @param[in]	vertices
		 * @param[in]	submesh
		 * @param[in]	appearance
		 */
		M3GMesh ( M3GVertexBuffer* vertices, M3GIndexBuffer* submesh, M3GAppearance* appearance );

		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GMesh ( KDvoid );

	public :

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Gets the current Appearance of the specified submesh.
		 * @param[in]	index
		 * @return
		 */
		M3GAppearance*  getAppearance ( KDint index ) const;

		/**
		 * @~English	Retrieves the submesh at the given index.
		 * @param[in]	index
		 * @return
		 */
		M3GIndexBuffer*  getIndexBuffer ( KDint index ) const;

		/**
		 * @~English	Gets the number of submeshes in this Mesh.
		 * @return
		 */
		KDint	getSubmeshCount ( KDvoid ) const;

		/**
		 * @~English	Gets the vertex buffer of this Mesh.
		 * @return
		 */
		M3GVertexBuffer*  getVertexBuffer ( KDvoid ) const;

		/**
		 * @~English	Sets the Appearance for the specified submesh.
		 * @param[in]	index
		 * @param[in]	appearance
		 */
		KDvoid	setAppearance ( KDint index, M3GAppearance* appearance );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

		virtual KDbool  intersect ( const M3GVector& org, const M3GVector& dir, M3GRayIntersection* ri ) const;

	protected :

		KDvoid  init ( M3GVertexBuffer* vertices, KDint numSubmesh, M3GIndexBuffer** submeshes, KDint numAppearances, M3GAppearance** appearances );
		KDvoid  init ( M3GVertexBuffer* vertices, M3GIndexBuffer* submesh, M3GAppearance* appearance );
		KDvoid  _duplicate ( M3GMesh* pObj ) const;

	protected :

		M3GVertexBuffer*				m_pVertices;
		std::vector<M3GIndexBuffer*>	m_vIndices;
		std::vector<M3GAppearance*>		m_vAppearances;

		friend class M3GLoader;
};

#endif
