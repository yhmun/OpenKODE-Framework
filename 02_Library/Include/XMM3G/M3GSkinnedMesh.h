/* --------------------------------------------------------------------------
 *
 *      File            M3GSkinnedMesh.h
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

#ifndef __M3GSkinnedMesh_h__
#define __M3GSkinnedMesh_h__

#include "M3GMesh.h"

class M3GGroup;
class M3GBoneWeight;
class M3GBindPose;

/**
 * @~English	A scene graph node that represents a skeletally animated polygon mesh.
 */
class M3GSkinnedMesh : public M3GMesh
{
	protected :
	
		M3GSkinnedMesh ( KDvoid );

	public :

		/**
		 * @~English	Constructs a new SkinnedMesh with the given vertices, submeshes and skeleton.
		 * @param[in]	vertices
		 * @param[in]	numSubmesh
		 * @param[in]	submeshes
		 * @param[in]	numAppearances
		 * @param[in]	appearances
		 * @param[in]	skeleton
		 */
		M3GSkinnedMesh ( M3GVertexBuffer* vertices, KDint numSubmesh, M3GIndexBuffer** submeshes, KDint numAppearance, M3GAppearance** appearances, M3GGroup* skeleton );

		/**
		 * @~English	Constructs a new SkinnedMesh with the given vertices, submesh and skeleton.
		 * @param[in]	vertices
		 * @param[in]	submesh
		 * @param[in]	appearance
		 * @param[in]	skeleton
		 */
		M3GSkinnedMesh ( M3GVertexBuffer* vertices, M3GIndexBuffer* submesh, M3GAppearance* appearance, M3GGroup* skeleton );

		/**
		 * @~English	Destructs this object.
		 */
		virtual ~M3GSkinnedMesh ( KDvoid );

	public :

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Associates a weighted transformation, or "bone", with a range of vertices in this SkinnedMesh.
		 * @param[in]	bone
		 * @param[in]	weight
		 * @param[in]	firstVertex
		 * @param[in]	numVertices
		 */
		KDvoid	addTransform ( M3GNode* bone, KDint weight, KDint firstVertex, KDint numVertices );

		/**
		 * @~English	Returns the at-rest transformation for a bone node.
		 * @param[in]	bone
		 * @param[in]	transform
		 */
		KDvoid	getBoneTransform ( M3GNode* bone, M3GTransform* transform ) const;

		/**
		 * @~English	Returns the number of vertices influenced by the given bone, filling in the vertices
		 *				and their weights to the given arrays.
		 * @param[in]	bone
		 * @param[in]	indices
		 * @param[in]	weights
		 * @return
		 */
		KDint	getBoneVertices ( M3GNode* bone, KDint* indices, KDfloat* weights ) const;

		/**
		 * @~English	Returns the skeleton Group of this SkinnedMesh.
		 * @return
		 */
		M3GGroup*  getSkeleton ( KDvoid ) const;

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  init ( M3GGroup* skeleton );

		KDvoid  _duplicate ( M3GSkinnedMesh* pObj ) const;

	private :

		KDint  addBoneIndex ( M3GNode* bone );

		KDint  getBoneIndex ( M3GNode* bone ) const;

		KDvoid  updateSkinnedVertices ( KDvoid );

	protected :

        M3GGroup*                                   m_pSkeleton;
        M3GVertexBuffer*                            m_pSkinnedVertices;
        std::vector< std::vector<M3GBoneWeight> >	m_vBoneWeights;
        std::vector<M3GBindPose>                    m_vBindPoses;

		friend class M3GLoader;
};


#endif
