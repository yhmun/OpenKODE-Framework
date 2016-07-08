/* --------------------------------------------------------------------------
 *
 *      File            M3GMorphingMesh.h
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

#ifndef __M3GMorphingMesh_h__
#define __M3GMorphingMesh_h__

#include "M3GMesh.h"

class M3GVertexBuffer;

/**
 * @~English	A scene graph node that represents a vertex morphing polygon mesh.
 */
class M3GMorphingMesh : public M3GMesh
{
	protected :

		M3GMorphingMesh ( KDvoid );

	public :

		/**
		 * @~English	Constructs a new MorphingMesh with the given base mesh and morph targets.
         * @param[in]	base  
         * @param[in]	numTargets  
         * @param[in]	targets  
         * @param[in]	numSubmesh  
         * @param[in]	submeshes  
         * @param[in]	numAppearance  
         * @param[in]	appearances  
		 */
		M3GMorphingMesh ( M3GVertexBuffer*  base, 
						  KDint				numTargets,
						  M3GVertexBuffer** targets, 
						  KDint				numSubmesh, 
						  M3GIndexBuffer**	submeshes,
						  KDint				numAppearance,
						  M3GAppearance**	appearances );

		/**
		 * @~English	Constructs a new MorphingMesh with the given base mesh and morph targets.
         * @param[in]	base  
         * @param[in]	numTargets  
         * @param[in]	targets  
         * @param[in]	submesh  
         * @param[in]	appearance  
		 */
		M3GMorphingMesh ( M3GVertexBuffer*  base, 
						  KDint				numTargets,
						  M3GVertexBuffer** targets, 
						  M3GIndexBuffer*	submesh ,
						  M3GAppearance*	appearance );

		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GMorphingMesh ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Returns the morph target VertexBuffer at the given index.
         * @param[in]	index  
		 * @return	
		 */
		M3GVertexBuffer*  getMorphTarget ( KDint index ) const;

		/**
		 * @~English	Returns the number of morph targets in this MorphingMesh.
		 * @return	
		 */
		KDint	getMorphTargetCount ( KDvoid ) const;

		/**
		 * @~English	Gets the current morph target weights for this mesh.
         * @param[out]	weights  
		 */
		KDvoid	getWeights ( KDfloat* weights ) const;

		/**
		 * @~English	Sets the weights for all morph targets in this mesh.
         * @param[in]	numWeights    
         * @param[in]	weights  
		 */
		KDvoid	setWeights ( KDuint numWeights, KDfloat* weights );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  init ( KDint numTargets, M3GVertexBuffer** targets );

		KDvoid  _duplicate ( M3GMorphingMesh* pObj ) const;

	private :

		KDvoid  updateMorphedVertices ( KDvoid );

	protected :

		M3GVertexBuffer*					m_pMorphedVertices;
		std::vector<M3GVertexBuffer*>		m_vMorphTargets;
		std::vector<KDfloat>				m_vMorphWeights;

		friend class M3GLoader;
};

#endif
