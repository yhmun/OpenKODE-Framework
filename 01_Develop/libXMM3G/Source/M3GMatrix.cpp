/* --------------------------------------------------------------------------
 *
 *      File            M3GMatrix.cpp
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

M3GMatrix::M3GMatrix ( KDvoid )
{
	setIdentity ( );
}

M3GMatrix::M3GMatrix ( KDfloat* m4x4 )
{
	for ( KDint i = 0; i < 16; i++ )
	{
		m[ i ] = m4x4 [ i ];
	}
}

M3GMatrix::M3GMatrix 
(
	KDfloat m00, KDfloat m01, KDfloat m02, KDfloat m03, 
	KDfloat m10, KDfloat m11, KDfloat m12, KDfloat m13, 
	KDfloat m20, KDfloat m21, KDfloat m22, KDfloat m23, 
	KDfloat m30, KDfloat m31, KDfloat m32, KDfloat m33 
)
{
	m[ 0] = m00;  m[ 1] = m01;  m[ 2] = m02;  m[ 3] = m03;
	m[ 4] = m10;  m[ 5] = m11;  m[ 6] = m12;  m[ 7] = m13;
	m[ 8] = m20;  m[ 9] = m21;  m[10] = m22;  m[11] = m23;
	m[12] = m30;  m[13] = m31;  m[14] = m32;  m[15] = m33;
}

M3GMatrix::~M3GMatrix ( KDvoid )
{

}

M3GMatrix M3GMatrix::getInverse ( KDvoid ) const
{
	M3GMatrix  mat = *this;
   
    mat.invert ( );

	return mat;
}

M3GMatrix M3GMatrix::get3x3 ( KDvoid ) const
{
	M3GMatrix  mat = *this;

    mat[ 3] = mat[ 7] = mat[11] = 0;
    mat[12] = mat[13] = mat[14] = 0;
    mat[15] = 1;

	return mat;
}

KDvoid M3GMatrix::set ( const KDfloat* matrix )
{
	kdMemcpy ( m, matrix, sizeof ( KDfloat ) * 16 );
}

KDvoid M3GMatrix::setIdentity ( KDvoid )
{
    m [ 0] = 1;  m [ 1] = 0;  m [ 2] = 0;  m [ 3] = 0;
    m [ 4] = 0;  m [ 5] = 1;  m [ 6] = 0;  m [ 7] = 0;
    m [ 8] = 0;  m [ 9] = 0;  m [10] = 1;  m [11] = 0;
    m [12] = 0;  m [13] = 0;  m [14] = 0;  m [15] = 1;
}

KDvoid M3GMatrix::setRotate ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az )
{
	angle = 2 * KD_PI_F * angle / 360.f;

	KDfloat  c = kdCosf ( angle );
	KDfloat  s = kdSinf ( angle );

	m[ 0] = ax*ax*(1-c)+c;
	m[ 1] = ax*ay*(1-c)-az*s;
	m[ 2] = ax*az*(1-c)+ay*s;
	m[ 3] = 0;
	m[ 4] = ay*ax*(1-c)+az*s;
	m[ 5] = ay*ay*(1-c)+c;
	m[ 6] = ay*az*(1-c)-ax*s;
	m[ 7] = 0;
	m[ 8] = ax*az*(1-c)-ay*s;
	m[ 9] = ay*az*(1-c)+ax*s;
	m[10] = az*az*(1-c)+c;
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}

KDvoid M3GMatrix::setScale ( KDfloat sx, KDfloat sy, KDfloat sz )
{
	m[ 0] = sx;
	m[ 5] = sy;
	m[10] = sz;
}

KDvoid M3GMatrix::setTranslate ( KDfloat tx, KDfloat ty, KDfloat tz )
{
	m[ 3] = tx;
	m[ 7] = ty;
	m[11] = tz;
}

M3GMatrix& M3GMatrix::invert ( KDvoid )
{
    KDfloat  n[16];
    kdMemcpy ( n, m, sizeof ( KDfloat ) * 16 );

    KDfloat  det = 
        + n[0]*n[5]*n[10]*n[15] + n[0]*n[6]*n[11]*n[13] + n[0]*n[7]*n[9 ]*n[14]
        + n[1]*n[4]*n[11]*n[14] + n[1]*n[6]*n[8 ]*n[15] + n[1]*n[7]*n[10]*n[12]
        + n[2]*n[4]*n[9 ]*n[15] + n[2]*n[5]*n[11]*n[12] + n[2]*n[7]*n[8 ]*n[13]
        + n[3]*n[4]*n[10]*n[13] + n[3]*n[5]*n[8 ]*n[14] + n[3]*n[6]*n[9 ]*n[12]
        - n[0]*n[5]*n[11]*n[14] - n[0]*n[6]*n[9 ]*n[15] - n[0]*n[7]*n[10]*n[13]
        - n[1]*n[4]*n[10]*n[15] - n[1]*n[6]*n[11]*n[12] - n[1]*n[7]*n[8 ]*n[14]
        - n[2]*n[4]*n[11]*n[13] - n[2]*n[5]*n[8 ]*n[15] - n[2]*n[7]*n[9 ]*n[12]
        - n[3]*n[4]*n[9 ]*n[14] - n[3]*n[5]*n[10]*n[12] - n[3]*n[6]*n[8 ]*n[13];

    if ( det == 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Can't invert this matrix." );
    }

    KDfloat  invDet = 1 / det;

    m[0]  = invDet*(n[5]*n[10]*n[15] + n[6]*n[11]*n[13] + n[7]*n[9 ]*n[14] - n[5]*n[11]*n[14] - n[6]*n[9 ]*n[15] - n[7]*n[10]*n[13]);
    m[1]  = invDet*(n[1]*n[11]*n[14] + n[2]*n[9 ]*n[15] + n[3]*n[10]*n[13] - n[1]*n[10]*n[15] - n[2]*n[11]*n[13] - n[3]*n[9 ]*n[14]);
    m[2]  = invDet*(n[1]*n[6 ]*n[15] + n[2]*n[7 ]*n[13] + n[3]*n[5 ]*n[14] - n[1]*n[7 ]*n[14] - n[2]*n[5 ]*n[15] - n[3]*n[6 ]*n[13]);
    m[3]  = invDet*(n[1]*n[7 ]*n[10] + n[2]*n[5 ]*n[11] + n[3]*n[6 ]*n[9 ] - n[1]*n[6 ]*n[11] - n[2]*n[7 ]*n[9 ] - n[3]*n[5 ]*n[10]);
    m[4]  = invDet*(n[4]*n[11]*n[14] + n[6]*n[8 ]*n[15] + n[7]*n[10]*n[12] - n[4]*n[10]*n[15] - n[6]*n[11]*n[12] - n[7]*n[8 ]*n[14]);
    m[5]  = invDet*(n[0]*n[10]*n[15] + n[2]*n[11]*n[12] + n[3]*n[8 ]*n[14] - n[0]*n[11]*n[14] - n[2]*n[8 ]*n[15] - n[3]*n[10]*n[12]);
    m[6]  = invDet*(n[0]*n[7 ]*n[14] + n[2]*n[4 ]*n[15] + n[3]*n[6 ]*n[12] - n[0]*n[6 ]*n[15] - n[2]*n[7 ]*n[12] - n[3]*n[4 ]*n[14]);
    m[7]  = invDet*(n[0]*n[6 ]*n[11] + n[2]*n[7 ]*n[8 ] + n[3]*n[4 ]*n[10] - n[0]*n[7 ]*n[10] - n[2]*n[4 ]*n[11] - n[3]*n[6 ]*n[8 ]);
    m[8]  = invDet*(n[4]*n[9 ]*n[15] + n[5]*n[11]*n[12] + n[7]*n[8 ]*n[13] - n[4]*n[11]*n[13] - n[5]*n[8 ]*n[15] - n[7]*n[9 ]*n[12]);
    m[9]  = invDet*(n[0]*n[11]*n[13] + n[1]*n[8 ]*n[15] + n[3]*n[9 ]*n[12] - n[0]*n[9 ]*n[15] - n[1]*n[11]*n[12] - n[3]*n[8 ]*n[13]);
    m[10] = invDet*(n[0]*n[5 ]*n[15] + n[1]*n[7 ]*n[12] + n[3]*n[4 ]*n[13] - n[0]*n[7 ]*n[13] - n[1]*n[4 ]*n[15] - n[3]*n[5 ]*n[12]);
    m[11] = invDet*(n[0]*n[7 ]*n[9 ] + n[1]*n[4 ]*n[11] + n[3]*n[5 ]*n[8 ] - n[0]*n[5 ]*n[11] - n[1]*n[7 ]*n[8 ] - n[3]*n[4 ]*n[9 ]);
    m[12] = invDet*(n[4]*n[10]*n[13] + n[5]*n[8 ]*n[14] + n[6]*n[9 ]*n[12] - n[4]*n[9 ]*n[14] - n[5]*n[10]*n[12] - n[6]*n[8 ]*n[13]);
    m[13] = invDet*(n[0]*n[9 ]*n[14] + n[1]*n[10]*n[12] + n[2]*n[8 ]*n[13] - n[0]*n[10]*n[13] - n[1]*n[8 ]*n[14] - n[2]*n[9 ]*n[12]);
    m[14] = invDet*(n[0]*n[6 ]*n[13] + n[1]*n[4 ]*n[14] + n[2]*n[5 ]*n[12] - n[0]*n[5 ]*n[14] - n[1]*n[6 ]*n[12] - n[2]*n[4 ]*n[13]);
    m[15] = invDet*(n[0]*n[5 ]*n[10] + n[1]*n[6 ]*n[8 ] + n[2]*n[4 ]*n[9 ] - n[0]*n[6 ]*n[9 ] - n[1]*n[4 ]*n[10] - n[2]*n[5 ]*n[8 ]);

    return *this;  
}

M3GMatrix& M3GMatrix::transpose ( KDvoid )
{
    KDfloat  n[16];

    kdMemcpy ( n, m, sizeof ( KDfloat ) * 16 );

    m[ 0] = n[ 0];
    m[ 1] = n[ 4];
    m[ 2] = n[ 8];
    m[ 3] = n[12];

    m[ 4] = n[ 1];
    m[ 5] = n[ 5];
    m[ 6] = n[ 9];
    m[ 7] = n[13];

    m[ 8] = n[ 2];
    m[ 9] = n[ 6];
    m[10] = n[10];
    m[11] = n[14];

    m[12] = n[ 3];
    m[13] = n[ 7];
    m[14] = n[11];
    m[15] = n[15];

    return *this;
}

M3GMatrix& M3GMatrix::operator*= ( const M3GMatrix& rhs )
{
	KDfloat  m1[16];
	KDfloat  m2[16];

	kdMemcpy ( m1, m, sizeof ( KDfloat ) * 16 );
	kdMemcpy ( m2, rhs.m, sizeof ( KDfloat ) * 16 );

	for ( KDint  i = 0; i < 4; i++) 
	{
		for ( KDint  j = 0; j < 4; j++ )
		{
			m[i*4+j] = 0;
			for ( KDint  k = 0; k < 4; k++) 
			{
				m[i*4+j] += m1[i*4+k] * m2[k*4+j];
			}
		}
	}

	return *this;
}

KDfloat& M3GMatrix::operator[] ( KDint n )
{
    if ( n < 0 || n > 15 )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, i = %d.", n );
    }

    return m[n];
}

const KDfloat& M3GMatrix::operator[] ( KDint n ) const
{
    if ( n < 0 || n > 15 )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, i = %d.", n );
    }

    return m[n];
}



M3GMatrix m3gMakeParallelProjection ( KDfloat height, KDfloat aspect, KDfloat zNear, KDfloat zFar )
{
	KDfloat  width = height * aspect;
	M3GMatrix  proj;

	proj[ 0] =  2 / width;
	proj[ 5] =  2 / height;
	proj[10] = -2 / ( zFar - zNear );
	proj[11] = -( zNear + zFar ) / ( zFar - zNear );
	proj[15] = 1;

	return proj;
}

M3GMatrix m3gMakePerspectiveProjection ( KDfloat fovy, KDfloat aspect, KDfloat zNear, KDfloat zFar )
{
	fovy = fovy * 2 * KD_PI_F / 360.f;

	KDfloat  height = kdTanf ( fovy / 2.f );
	KDfloat   width = height * aspect;
	M3GMatrix  proj;

	proj[ 0] =  1 / width;
	proj[ 5] =  1 / height;
	proj[10] = -( zNear + zFar ) / ( zFar - zNear );
	proj[11] = -2 * zNear * zFar / ( zFar - zNear );
	proj[14] = -1;
	proj[15] = 0;

	return proj;
}

M3GMatrix operator* ( const M3GMatrix& lhs, const M3GMatrix& rhs )
{
	M3GMatrix  r = lhs;
	
	r *= rhs;

	return r;
}

M3GVector operator* ( const M3GMatrix& m, const M3GVector& v )
{
    M3GVector r ( 0, 0, 0, 0 );

    for ( KDint i = 0; i < 4; i++ ) 
	{
        for ( KDint  j = 0; j < 4; j++ )
		{
            r[i] += m.m[i*4+j] * v[j];
        }
    }

    return r;
}