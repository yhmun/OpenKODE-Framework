/* --------------------------------------------------------------------------
 *
 *      File            M3GVector.h
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

#ifndef __M3GVector_h__
#define __M3GVector_h__

/**
 * @internal
 * @~English	
 */
class M3GVector
{
	public :

		/**
		 * @internal
		 * @~English	
		 */
		M3GVector ( KDvoid );

		/**
		 * @internal
		 * @~English	
		 * @param[in]	xyz
		 */
		M3GVector ( KDfloat* xyz );

		/**
		 * @internal
		 * @~English	
		 * @param[in]	x
		 * @param[in]	y
		 * @param[in]	z
		 * @param[in]	w 
		 */
		M3GVector ( KDfloat x, KDfloat y, KDfloat z, KDfloat w = 1 );
		
		/**
		 * @internal
		 * @~English	Destruct this object.
		 */
		~M3GVector ( KDvoid );

		/**
		 * @internal
		 * @~English	
		 * @param[out]	xyz
		 * @return
		 */
		KDvoid  get ( KDfloat* xyz ) const;

		/**
		 * @internal
		 * @~English	
		 * @param[out]	xyzw
		 * @return
		 */
		KDvoid  get4 ( KDfloat* xyzw ) const;

		/**
		 * @internal
		 * @~English	
		 * @return
		 */
		KDfloat length ( KDvoid ) const;

		/**
		 * @internal
		 * @~English	
		 * @return
		 */
		M3GVector&  normalize ( KDvoid );

		/**
		 * @internal
		 * @~English	
		 * @return
		 */
		M3GVector&  dividedByW ( KDvoid );

		/**
		 * @internal
		 * @~English	
		 */
		KDfloat&  operator[] ( KDint index );

		/**
		 * @internal
		 * @~English	
		 * @return
		 */
		const KDfloat&  operator[] ( KDint index ) const;

		/**
		 * @internal
		 * @~English	
		 * @return		
		 */
		M3GVector  operator- ( KDvoid ) const;

		/**
		 * @internal
		 * @~English	
		 * @return		
		 */
		M3GVector& operator+= ( const M3GVector& rhs );

		public :

			KDfloat			x;
			KDfloat			y;
			KDfloat			z;
			KDfloat			w;
};

/**
 * @internal
 * @~English	
 * @param[in]	v0
 * @param[in]	v1
 * @return  
 */
KDfloat  m3gDot ( const M3GVector& v0, const M3GVector& v1 );

/**
 * @internal
 * @~English	
 * @param[in]	v0
 * @param[in]	v1
 * @return  
 */
M3GVector  m3gCross ( const M3GVector& v0, const M3GVector& v1 );

/**
 * @internal
 * @~English	
 * @param[in]	u
 * @param[in]	v
 * @param[in]	v0
 * @param[in]	v1
 * @param[in]	v2
 * @return  
 */
M3GVector  m3gLerp ( KDfloat u, KDfloat v, const M3GVector& v0, const M3GVector& v1, const M3GVector& v2 );

/**
 * @internal
 * @~English	
 * @return  
 */
M3GVector operator* ( const M3GVector& lhs, KDfloat f );

/**
 * @internal
 * @~English	
 * @return  
 */
M3GVector operator* ( KDfloat f, const M3GVector& rhs );

/**
 * @internal
 * @~English	
 * @return  
 */
M3GVector operator/ ( const M3GVector& lhs, KDfloat f );

/**
 * @internal
 * @~English	
 * @return  
 */
M3GVector operator+ ( const M3GVector& lhs, const M3GVector& rhs );

/**
 * @internal
 * @~English	
 * @return  
 */
M3GVector operator- ( const M3GVector& lhs, const M3GVector& rhs );


#endif