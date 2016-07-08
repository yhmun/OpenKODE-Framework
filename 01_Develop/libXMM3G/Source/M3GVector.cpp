/* --------------------------------------------------------------------------
 *
 *      File            M3GVector.cpp
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

#include "M3GInternal.h"

M3GVector::M3GVector ( KDvoid )
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

M3GVector::M3GVector ( KDfloat* xyz )
{
	x = xyz[0];
	y = xyz[1];
	z = xyz[2];
	w = 1;
}

M3GVector::M3GVector ( KDfloat x, KDfloat y, KDfloat z, KDfloat w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

M3GVector::~M3GVector ( KDvoid )
{

}

KDvoid M3GVector::get ( KDfloat* xyz ) const
{
	if ( w != 0 )
	{
		xyz[0] = x / w; 
		xyz[1] = y / w;
		xyz[2] = z / w;
	}
	else
	{
		xyz[0] = x;
		xyz[1] = y;
		xyz[2] = z;
	}
}

KDvoid M3GVector::get4 ( KDfloat* xyzw ) const
{
	xyzw[0] = x;
	xyzw[1] = y;
	xyzw[2] = z;
	xyzw[3] = w;
}

KDfloat M3GVector::length ( KDvoid ) const
{
	if ( w == 0 )
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "Lenght of vector, but w is 0." );
	}

	return kdSqrtf ( ( x*x + y*y + z*z ) / ( w*w ) );
}

M3GVector& M3GVector::normalize ( KDvoid )
{
	KDfloat  len = length ( );

	if ( len == 0 )
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "Normalized, but length of vector is 0." );
	}
	
	x /= len;
	y /= len;
	z /= len;

	return *this;
}

M3GVector& M3GVector::dividedByW ( KDvoid )
{
	if ( w == 0 ) 
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "Divied by w = 0." );
	}

	x /= w;
	y /= w;
	z /= w;
	w  = 1;
	
	return *this;
}

KDfloat& M3GVector::operator[] ( KDint index )
{
	if ( index < 0 || index > 3 )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d.", index );
	}

	return (&x) [ index ];
}

const KDfloat& M3GVector::operator[] ( KDint index ) const
{
	if ( index < 0 || index > 3 )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d.", index );
	}

	return (&x) [ index ];
}

M3GVector M3GVector::operator- ( KDvoid ) const
{
    M3GVector  v;
    
	v.x = -x;
    v.y = -y;
    v.z = -z;
    v.w =  w;

    return v;
}

M3GVector& M3GVector::operator+= ( const M3GVector& rhs )
{
    if ( rhs.w == 0 ) 
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "Divided by 0." );
    }

    x += rhs.x * w/rhs.w;
    y += rhs.y * w/rhs.w;
    z += rhs.z * w/rhs.w;

    return *this;
}

KDfloat m3gDot ( const M3GVector& v0, const M3GVector& v1 )
{
    if ( kdFabsf ( v0.w - 1 ) > KD_FLT_EPSILON || kdFabsf ( v1.w - 1 ) > KD_FLT_EPSILON ) 
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "Dot, but w is not 1. v0.w = %f, v1.w = %f.", v0.w, v1.w );
    }

    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

M3GVector m3gCross ( const M3GVector& v0, const M3GVector& v1 )
{
	if ( v0.w != 1 || v1.w != 1 ) 
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "Cross, but w is not 1. v0.w = %f, v1.w = %f.", v0.w, v1.w );
	}

	return M3GVector ( v0.y*v1.z - v0.z*v1.y, v0.z*v1.x - v0.x*v1.z, v0.x*v1.y - v0.y*v1.x );
}

M3GVector m3gLerp ( KDfloat u, KDfloat v, const M3GVector& v0, const M3GVector& v1, const M3GVector& v2 )
{
    if ( v0.w != 1 || v1.w != 1 || v2.w != 1 )
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "lerp, w must be 1, v0.w = %f, v1.w = %f, v2.w = %f.", v0.w, v1.w, v2.w );
    }

    M3GVector  e1 = v1 - v0;
    M3GVector  e2 = v2 - v0;
    return v0 + e1*u + e2*v;
}

M3GVector operator* ( const M3GVector& lhs, KDfloat f )
{
    M3GVector v ( lhs );

    v.x *= f;
    v.y *= f;
    v.z *= f;

    return v;
}

M3GVector operator* ( KDfloat f, const M3GVector& rhs )
{
	return rhs * f;
}

M3GVector operator/ ( const M3GVector& lhs, KDfloat f )
{
    if ( f == 0 )
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "Divided by 0. in Vector / f." );
    }

    M3GVector v ( lhs );

    v.x /= f;
    v.y /= f;
    v.z /= f;
    
	return v;
}

M3GVector operator+ ( const M3GVector& lhs, const M3GVector& rhs )
{
    if ( lhs.w != rhs.w && lhs.w != 0 && rhs.w != 0 ) 
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Add vectors with different w. lhs.w = %f, rhs.w = %f.", lhs.w, rhs.w );
    }

    if ( lhs.w == rhs.w )
	{
        return M3GVector ( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w );
    }
	else 
	{
        return M3GVector ( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w );
    }
}

M3GVector operator- ( const M3GVector& lhs, const M3GVector& rhs )
{
    if ( lhs.w != rhs.w && lhs.w != 0 && rhs.w != 0 ) 
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Subtracted vectors with different w. lhs.w = %f, rhs.w = %f.", lhs.w, rhs.w );
    }

    if ( lhs.w == rhs.w )
	{
        return M3GVector ( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w );
    }
	else if ( lhs.w == 0 )
	{
        return M3GVector ( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, rhs.w );
    }
	else 
	{
        return M3GVector ( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w );
    }
}