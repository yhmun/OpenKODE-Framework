/* --------------------------------------------------------------------------
 *
 *      File            M3GQuaternion.h
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

#ifndef __M3GQuaternion_h__
#define __M3GQuaternion_h__

/**
 * @internal
 * @~English	
 */
class M3GQuaternion
{
	public :

		/**
		 * @internal
		 * @~English	Construct this object as unit quaternion.
		 */
		M3GQuaternion ( KDvoid ); 

		/**
		 * @internal	
		 * @~English	Construct this object by angle-axis form, no need to be unit vector.
		 */
		M3GQuaternion ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az );


		/**
		 * @internal
		 * @~English    Destruct this object.
		 */
		~M3GQuaternion ( KDvoid );

		/**
		 * @internal
		 * @~English	Return inverse of this quaternion
		 */
		M3GQuaternion  getInverse ( KDvoid ) const;

		/**
		 * @internal
		 * @~English	Make vector components to be 0, scaler componet to be 1.
		 */
		KDvoid  setIdentity ( KDvoid );

		/**
		 * @internal
		 * @~English	Make all components to zero.
		 */
		KDvoid  setZero ( KDvoid );

		/**
		 * @internal
		 * @~English	Make this unit quaternion. 
		 */
		M3GQuaternion&  normalize ( KDvoid );

		/**
		 * @internal
		 * @~English	Set quaternion in quaternion form.
		 */
		KDvoid  set ( KDfloat qx, KDfloat qy, KDfloat qz, KDfloat qw );

		/**
		 * @internal
		 * @~English	Retrieves this as angle-axis form.
		 */
		KDvoid  getAngleAxis ( KDfloat* angleAxis ) const;

		/**
		 * @internal
		 * @~English	Retrieves length of this quaternion.
		 */
		KDfloat getLength ( KDvoid ) const;

	public :

		KDfloat			x;   
		KDfloat			y;   
		KDfloat			z;   
		KDfloat			w;   
};

/**
 * @internal
 * @~English   slerp 2 quaternions.
 */
M3GQuaternion  m3gSlerp ( const M3GQuaternion& q0, const M3GQuaternion& q1, KDfloat alpha );

/**
 * @internal
 * @~English	Squad 2 quaternions.
 */
M3GQuaternion  m3gSquad ( const M3GQuaternion& q0,
                          const M3GQuaternion& q1,
                          const M3GQuaternion& q2,
                          const M3GQuaternion& q3,
                          KDfloat t0, KDfloat t1, KDfloat t2, KDfloat t3, KDfloat alpha );

/**
 * @internal
 * @~English	Logarithm of a quaternion.
 */
M3GQuaternion  m3gLog ( const M3GQuaternion& q );

/**
 * @internal
 * @~English	 Exponential of a quaternion.
 */
M3GQuaternion  m3gExp ( const M3GQuaternion& q );

/**
 * @internal
 * @~English	Convert from rotation matrix to quaternion.
 */
M3GQuaternion  m3gMatrix2quat ( const M3GMatrix& m );

/**
 * @internal
 * @~English
 */
M3GQuaternion operator* ( KDfloat f, const M3GQuaternion& q );

/**
 * @internal
 * @~English
 */
M3GQuaternion operator* ( const M3GQuaternion& p, KDfloat f );

/**
 * @internal
 * @~English
 */
M3GQuaternion operator* ( const M3GQuaternion& q1, const M3GQuaternion& q2 );

/**
 * @internal
 * @~English
 */
M3GQuaternion operator+ ( const M3GQuaternion& q1, const M3GQuaternion& q2 );

/**
 * @internal
 * @~English
 */
M3GQuaternion operator- ( const M3GQuaternion& q1, const M3GQuaternion& q2 );

#endif