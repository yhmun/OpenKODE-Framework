/* --------------------------------------------------------------------------
 *
 *      File            M3GRayIntersection.h
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

#ifndef __M3GRayIntersection_h__
#define __M3GRayIntersection_h__

class M3GNode;
class M3GVector;
class M3GTransform;

/**
 * @~English	A RayInersection objet is filled in by the pick methods in Group.
 *				RayIntersection stores a reference to the intersected Mesh or Sprite3D and information about the intersection point. 
 */
class M3GRayIntersection
{
    public :

		/**
		 * @~English	Constructs a new RayIntersection object with default values.
		 */
        M3GRayIntersection ( KDvoid );

		/**
		 * @~English	Constructs a new RayIntersection object.
		 */
		M3GRayIntersection ( M3GNode* node,
							 const M3GVector& org, const M3GVector& dir, KDfloat t,
							 KDfloat u, KDfloat v,
							 KDint numVertices, KDint* vertices,
							 KDint submeshIndex );

		/**
		 * @~English	Destructs this object.
		 */
        ~M3GRayIntersection ( KDvoid );

	public :

		/**
		 * @~English	Retrieves the distance from the pick ray origin to the intersection point.
		 * @return	
		 */
        KDfloat	 getDistance ( KDvoid ) const;

		/**
		 * @~English	Retrieves the picked Mesh or Sprite3D object.
		 * @return	
		 */
        M3GNode*  getIntersected ( KDvoid ) const;

		/**
		 * @~English	Retrieves the X component of the surface normal at the intersection point.
		 * @return	
		 */
        KDfloat  getNormalX ( KDvoid ) const;

		/**
		 * @~English	Retrieves the Y component of the surface normal at the intersection point.
		 * @return	
		 */
        KDfloat  getNormalY ( KDvoid ) const;

		/**
		 * @~English	Retrieves the Z component of the surface normal at the intersection point.
		 * @return	
		 */
        KDfloat	 getNormalZ ( KDvoid ) const;

		/**
		 * @~English	Retrieves the origin (ox oy oz) and direction (dx dy dz) of the pick ray, in that order.
		 * @param[out]	ray
		 */
        KDvoid	getRay ( KDfloat* ray ) const;

		/**
		 * @~English	Retrieves the index of the submesh where the intersection point is located within the intersected Mesh.
		 * @return	
		 */
        KDint  getSubmeshIndex ( KDvoid ) const;

		/**
		 * @~English	Retrieves the S texture coordinate at the intersection point on the picked Mesh or Sprite3D.
		 * @param[in]	index
		 * @return	
		 */
        KDfloat  getTextureS ( KDint index ) const;

		/**
		 * @~English	Retrieves the T texture coordinate at the intersection point on the picked Mesh or Sprite3D.
		 * @param[in]	index
		 * @return	
		 */
        KDfloat  getTextureT ( KDint index ) const;

	public :

		KDvoid  transformRay ( const M3GTransform& transform );

		KDvoid	normalizeRay ( KDvoid );

    private :

        KDvoid   computeNormal ( KDvoid );

        KDvoid   computeTexCoord ( KDint index );

    private :
	
		M3GVector*					m_pRayOrg;
		M3GVector*					m_pRayDir;
		KDfloat						m_fRayT;
        M3GNode*					m_pNode;
        KDfloat						m_fU;
		KDfloat						m_fV;
        std::vector<KDint>			m_vVertices;        
        KDint						m_nSubmeshIndex;
        M3GVector*					m_pNormal;
        std::vector<M3GVector*>		m_vTexCoord;
};


#endif
