/* --------------------------------------------------------------------------
 *
 *      File            M3GTransform.h 
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

#ifndef __M3GTransform_h__
#define __M3GTransform_h__

class M3GVertexArray;
class M3GMatrix;
class M3GVector;

/**
 * @~English	A generic 4x4 floating point matrix, representing a transformation.
 */
class M3GTransform 
{
	public :

		/**
		 * @~English	Constructs a new Transform object and initializes it to the 4x4 identity matrix.
		 */
		M3GTransform ( KDvoid );

		/**
		 * @~English	Constructs a new Transform object and initializes it by copying in the contents of the given Transform.
		 * @param[in]	transform
		 */
		M3GTransform ( const M3GTransform& transform );

		/**
		 * @~English	Destructs this object.
		 */
		~M3GTransform ( KDvoid );

	public :

		/**
		 * @~English	Retrieves the contents of this transformation as a 16-element float array.
		 * @param[in]	matrix
		 */
		KDvoid	get ( KDfloat* matrix ) const;

		/**
		 * @~English	Inverts this matrix, if possible.
		 */
		KDvoid	invert ( KDvoid );

		/**
		 * @~English	Multiplies this transformation from the right by the given transformation.
		 * @param[in]	transform
		 */
		KDvoid	postMultiply ( const M3GTransform& transform );

		/**
		 * @~English	Multiplies this transformation from the right by the given rotation matrix, specified in axis-angle form.
		 * @param[in]	angle
		 * @param[in]	ax
		 * @param[in]	ay
		 * @param[in]	az
		 */
		KDvoid	postRotate ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az );

		/**
		 * @~English	Multiplies this transformation from the right by the given rotation matrix, specified in quaternion form.
		 * @param[in]	qx
		 * @param[in]	qy
		 * @param[in]	qz
		 * @param[in]	qw
		 */
		KDvoid	postRotateQuat ( KDfloat qx, KDfloat qy, KDfloat qz, KDfloat qw );

		/**
		 * @~English	Multiplies this transformation from the right by the given scale matrix.
		 * @param[in]	sx
		 * @param[in]	sy
		 * @param[in]	sz
		 */
		KDvoid	postScale ( KDfloat sx, KDfloat sy, KDfloat sz );

		/**
		 * @~English	Multiplies this transformation from the right by the given translation matrix.
		 * @param[in]	tx
		 * @param[in]	ty
		 * @param[in]	tz
		 */
		KDvoid	postTranslate ( KDfloat tx, KDfloat ty, KDfloat tz );

		/**
		 * @~English	Sets this transformation by copying from the given 16-element float array.
		 * @param[in]	matrix
		 */
		KDvoid	set ( const KDfloat* matrix );

		/**
		 * @~English	Sets this transformation by copying the contents of the given Transform.
		 * @param[in]	transform
		 */
		KDvoid	set ( const M3GTransform* transform );

		/**
		 * @~English	Replaces this transformation with the 4x4 identity matrix.
		 */
		KDvoid	setIdentity ( KDvoid );

		/**
		 * @~English	Multiplies the given array of 4D vectors with this matrix.
		 * @param[in]	numVectors
		 * @param[in,out]  vectors
		 */
		KDvoid	transform ( KDint numVectors, KDfloat* vectors ) const;

		/**
		 * @~English	Multiplies the elements of the given VertexArray with this matrix, storing the transformed values in a float array.
		 * @param[in]	in
		 * @param[out]	out
		 * @param[in]	W
		 */
		KDvoid	transform ( const M3GVertexArray* in, KDfloat* out, KDbool W ) const;

		/**
		 * @~English	Transposes this matrix.
		 */
		KDvoid	transpose ( KDvoid );

	public :

		M3GMatrix  getMatrix ( KDvoid ) const;

		M3GVector  transform ( const M3GVector& vector ) const;

		M3GVector  transform3x3 ( const M3GVector& vector ) const;

	private :

		M3GMatrix*			m_pMatrix;
};


#endif
