/* --------------------------------------------------------------------------
 *
 *      File            M3GCamera.h
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

#ifndef __M3GCamera_h__
#define __M3GCamera_h__

#include "M3GNode.h"
#include "M3GTransform.h"

/**
 * @~English	A scene graph node that defines the position of the viewer in the scene and the projection from 3D to 2D.
 */
class M3GCamera : public M3GNode
{
	public :

		/**
		 * @~English	Specifies a generic 4x4 projection matrix.
		 */
		static const KDint	GENERIC     = 48;

		/**
		 * @~English	Specifies a parallel projection matrix.
		 */
		static const KDint	PARALLEL    = 49;

		/**
		 * @~English	specifies a perspective projection matrix.
		 */
		static const KDint	PERSPECTIVE = 50;

	public :

		/**
		 * @~English	Constructs a new Camera node with default values.
		 */
		M3GCamera ( KDvoid );

		/**
		 * @~English	Destruct a Camera node.
		 */
		virtual ~M3GCamera ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Gets the current projection parameters and type.
		 * @param[out]	params
		 * @return
		 */
		KDint	getProjection ( KDfloat* params ) const;

		/**
		 * @~English	Gets the current projection matrix and type.
		 * @param[out]	transorm
		 * @return
		 */
		KDint	getProjection ( M3GTransform* transform ) const;

		/**
		 * @~English	Sets the given 4x4 transformation as the current projection matrix.
		 * @param[in]	transform
		 */
		KDvoid	setGeneric ( const M3GTransform* transform );

		/**
		 * @~English	Constructs a parallel projection matrix and sets that as the current projection matrix.
		 * @param[in]	fovy
		 * @param[in]	aspectRatio
		 * @param[in]	zNear
		 * @param[in]	zFar
		 */
		KDvoid	setParallel ( KDfloat fovy, KDfloat aspectRatio, KDfloat zNear, KDfloat zFar );

		/**
		 * @~English	Constructs a perspective projection matrix and sets that as the current projection matrix.
		 * @param[in]	fovy
		 * @param[in]	aspectRatio
		 * @param[in]	zNear
		 * @param[in]	zFar
		 */
		KDvoid	setPerspective ( KDfloat fovy, KDfloat aspectRatio, KDfloat zNear, KDfloat zFar );

	public :

		/**
		 * @~English	Same as glut's lookat() function. this is not in M3G.
		 */
        KDvoid  lookAt ( KDfloat fromX, KDfloat fromY, KDfloat fromZ,
                         KDfloat   toX, KDfloat   toY, KDfloat   toZ, 
                         KDfloat   upX, KDfloat   upY, KDfloat   upZ );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GCamera* pObj ) const;
	
	private :

        KDint				m_nType;
        KDfloat				m_fFovy;
        KDfloat				m_fAspect;
        KDfloat				m_fNear;
        KDfloat				m_fFar;
        M3GTransform		m_tProjection;
};

#endif
