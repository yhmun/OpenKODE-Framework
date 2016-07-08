/* --------------------------------------------------------------------------
 *
 *      File            M3GMatrix.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 UEDA.Takashi
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __M3GMatrix_h__
#define __M3GMatrix_h__

/**
 * @internal
 * @~English	4x4 Matrix, for inner use.
 */
class M3GMatrix
{
	public :

		/**
		 * @internal
		 * @~English	Create a new Indentity Matrix object.
		 */
		M3GMatrix ( KDvoid );

		/**
		 * @internal
		 * @~English	Create a new Materix object with specified transformation.
		 */
		M3GMatrix ( KDfloat* m4x4 );

		/**
		 * @internal
		 * @~English	Create a new Matrix object with specified transformation.
		 */
		M3GMatrix ( KDfloat m00, KDfloat m01, KDfloat m02, KDfloat m03, 
					KDfloat m10, KDfloat m11, KDfloat m12, KDfloat m13, 
					KDfloat m20, KDfloat m21, KDfloat m22, KDfloat m23, 
					KDfloat m30, KDfloat m31, KDfloat m32, KDfloat m33 );

		/**
		 * @internal
		 * @~English	Destruct this object.
		 */
		~M3GMatrix ( KDvoid );

		/**
		 * @internal
		 * @~English	Retrieves the invers matrix.
		 * @exception	IllegalArgumentException  If not able to calculate inverse.
		 * @return    
		 */
		M3GMatrix  getInverse ( KDvoid ) const;

		/**
		 * @internal
		 * @~English  Retrives the 3x3 matrix from this 4x4 matrix.
		 * @return    3x3 elements of this matrix.
		 */
		M3GMatrix  get3x3 ( KDvoid ) const;

		/**
		 * @internal
		 * @~English	Set transformation 4x4 matrix.
		 * @param[in]	matrix  Transformation matrix represented by 16 floats.
		 */
		KDvoid	set ( const KDfloat* matrix );

		/**
		 * @internal
		 * @~English	Replace transformation matrix as identity matrix.
		 */
		KDvoid	setIdentity ( KDvoid );

		/**
		 * @internal
		 * @~English	Replace transformation matrix as specified rotation.
		 * @param[in]	angle  angle of rotation (degree).
		 * @param[in]	ax     x element of rotation axis.
		 * @param[in]	ay     y element of rotation axis.
		 * @param[in]	az     z element of rotation axis.
		 */
		KDvoid	setRotate ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az );

		/**
		 * @internal
		 * @~English   Replace transformation matrix as specified scaling.
		 */
		KDvoid	setScale ( KDfloat sx, KDfloat sy, KDfloat sz );

		/**
		 * @internal
		 * @~English	Replace transformation matrix as specified tranlation.
		 */
		KDvoid	setTranslate ( KDfloat tx, KDfloat ty, KDfloat tz );

		/**
		 * @internal
		 * @~English	Replace transformation matrix as inverted matrix.
		 */
		M3GMatrix&  invert ( KDvoid );

		/**
		 * @internal
		 * @~English	Replace transformation matrix as transposed matrix.
		 */
		M3GMatrix&  transpose ( KDvoid );

		/**
		 * @internal
		 * @~English	Multiplay matrix by right.
		 */
		M3GMatrix&  operator*= ( const M3GMatrix& rhs );

		/**
		 * @internal
		 * @~English	Retrives transformation matrix by index.
		 */
		KDfloat&  operator[] ( KDint n );

		/**
		 * @internal
		 * @~English	Retrives transformation matrix by index.
		 */
		const KDfloat&  operator[] ( KDint n ) const;

	public :

		KDfloat			m [ 16 ];    
};

/**
 * @internal
 * @~English	Create a parallel projection matrix by specified parameters.
 */
M3GMatrix  m3gMakeParallelProjection ( KDfloat height, KDfloat aspect, KDfloat zNear, KDfloat zFar );

/**
 * @internal
 * @~English	Create a new perspective projection matrix by specified parameters.
 */
M3GMatrix  m3gMakePerspectiveProjection ( KDfloat fovy, KDfloat aspect, KDfloat zNear, KDfloat zFar );

/**
 * @internal
 * @~English	Binary operator of matrix-matrix multiply.
 */
M3GMatrix  operator* ( const M3GMatrix& lhs, const M3GMatrix& rhs );

/**
 * @internal
 * @~English	Binary operator of matrix-vector multiply.
 */
M3GVector operator* ( const M3GMatrix& m, const M3GVector& v );

#endif