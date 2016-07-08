/* --------------------------------------------------------------------------
 *
 *      File            M3GTransformable.h
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

#ifndef __M3GTransformable_h__
#define __M3GTransformable_h__

#include "M3GObject3D.h"
#include "M3GTransform.h"

class M3GQuaternion;

/**
 * @~English	An abstract base class for Node and Texture2D, defining common methods for manipulating node and texture.
 */
class M3GTransformable : public M3GObject3D
{
	public :

		/**
		 * @~English	Construct a new Tranformable object.
		 */
		M3GTransformable ( KDvoid );
	 
		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GTransformable ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Retrieves the composite transformation matrix of this Transformable.
		 */
		KDvoid	getCompositeTransform ( M3GTransform* transform ) const;

		/**
		 * @~English	Retrieves the orientation component of this Transformable.
		 */
		KDvoid	getOrientation ( KDfloat* angleAxis ) const;

		/**
		 * @~English	Retrieves the scale component of this Transformable.
		 */
		KDvoid	getScale ( KDfloat* xyz ) const;

		/**
		 * @~English	Retrieves the matrix component of this Transformable.
		 */
		KDvoid	getTransform ( M3GTransform* transform ) const;

		/**
		 * @~English	Retrieves the translation component of this Transformable.
		 */
		KDvoid	getTranslation ( KDfloat* xyz ) const;

		/**
		 * @~English	Multiplies the current orientation component from the right by the given orientation.
		 */
		KDvoid	postRotate ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az );

		/**
		 * @~English	Multiplies the current orientation component from the left by the given orientation.
		 */
		KDvoid	preRotate ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az );

		/**
		 * @~English	Multiplies the current scale component by the given scale factors.
		 */
		KDvoid	scale ( KDfloat sx, KDfloat sy, KDfloat sz );

		/**
		 * @~English	Sets the orientation component of this Transformable.
		 */
		KDvoid	setOrientation ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az );

		/**
		 * @~English	Sets the scale component of this Transformable.
		 */
		KDvoid	setScale ( KDfloat sx, KDfloat sy, KDfloat sz );

		/**
		 * @~English	Sets the matrix component of this Transformable by copying in the given Transform.
		 */
		KDvoid	setTransform ( const M3GTransform* transform );

		/**
		 * @~English	Sets the translation component of this Transformable.
		 */
		KDvoid	setTranslation ( KDfloat tx, KDfloat ty, KDfloat tz );

		/**
		 * @~English	Adds the given offset to the current translation component.
		 */
		KDvoid	translate ( KDfloat tx, KDfloat ty, KDfloat tz );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GTransformable* pObj ) const;

	private :

		KDfloat				m_fScaling [ 3 ];
        KDfloat				m_fTranslation [ 3 ];
        M3GQuaternion*		m_pOrientation;
        M3GTransform		m_tTransform;
};


#endif
