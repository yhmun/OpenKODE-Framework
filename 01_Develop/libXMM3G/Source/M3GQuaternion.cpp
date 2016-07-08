/* --------------------------------------------------------------------------
 *
 *      File            M3GQuaternion.cpp
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

M3GQuaternion::M3GQuaternion ( KDvoid )
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

M3GQuaternion::M3GQuaternion ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az )
{
    if ( angle < 0 || angle > 0 )
	{
        M3GVector  axis ( ax, ay, az );
        axis.normalize ( );
  
        KDfloat  th = 2 * KD_PI_F * angle / 360.f;

        x = axis.x * kdSinf ( th / 2.f );
        y = axis.y * kdSinf ( th / 2.f );
        z = axis.z * kdSinf ( th / 2.f );
        w =          kdCosf ( th / 2.f );
    }
	else
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}
}

M3GQuaternion::~M3GQuaternion ( KDvoid )
{

}

M3GQuaternion M3GQuaternion::getInverse ( KDvoid ) const
{
	M3GQuaternion  q;

	q.x = -x;
	q.y = -y;
	q.z = -z;
	q.w =  w;

	return q;
}

KDvoid M3GQuaternion::setIdentity ( KDvoid )
{
	x = y = z = 0;
	w = 1;
}

KDvoid M3GQuaternion::setZero ( KDvoid )
{
	x = y = z = w = 0;
}

M3GQuaternion& M3GQuaternion::normalize ( KDvoid )
{
	KDfloat len = kdSqrtf ( x*x + y*y + z*z + w*w );

	if (len == 0)
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Divided by 0." );
	}

	x = x / len;
	y = y / len;
	z = z / len;
	w = w / len;

	return *this;
}

KDvoid M3GQuaternion::set ( KDfloat qx, KDfloat qy, KDfloat qz, KDfloat qw )
{
	x = qx;
	y = qy;
	z = qz;
	w = qw;
}

KDvoid M3GQuaternion::getAngleAxis ( KDfloat* angleAxis ) const
{
	if ( angleAxis == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "angleAxis is NULL." );
	}

	KDfloat  th = 2 * kdAcosf ( w );
	angleAxis[0] = th * 360 / ( 2.f * KD_PI_F );

	if ( kdSinf ( th / 2.f ) > 0 || kdSinf ( th / 2.f ) < 0 )
	{
		angleAxis[1] = x / kdSinf ( th / 2.f );
		angleAxis[2] = y / kdSinf ( th / 2.f );
		angleAxis[3] = z / kdSinf ( th / 2.f );
	}
	else
	{
		angleAxis[0] = 0;
		angleAxis[1] = x;
		angleAxis[2] = y;
		angleAxis[3] = z;
	}
}

KDfloat M3GQuaternion::getLength ( KDvoid ) const
{
	return kdSqrtf ( x*x + y*y + z*z + w*w );
}

M3GQuaternion m3gSlerp ( const M3GQuaternion& q0, const M3GQuaternion& q1, KDfloat alpha )
{
	KDfloat  th = kdAcosf ( q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.w*q1.w );

	if ( kdSinf ( th ) > 0 || kdSinf ( th ) < 0 ) 
	{
		KDfloat  w0 = kdSinf ( (1 - alpha) * th ) / kdSinf ( th );
		KDfloat  w1 = kdSinf ( alpha * th ) / kdSinf ( th );
		return w0*q0 + w1*q1;
	}
	else 
	{
		return ( alpha < 0.5 ) ? q0 : q1;
	}
}

M3GQuaternion m3gSquad ( const M3GQuaternion& q0,
                         const M3GQuaternion& q1,
                         const M3GQuaternion& q2,
                         const M3GQuaternion& q3,
                         KDfloat t0, KDfloat t1, KDfloat t2, KDfloat t3, KDfloat alpha )
{
    KDfloat  f1 = 2 * (t2-t1) / (t2-t0);
    KDfloat  f2 = 2 * (t2-t1) / (t3-t1);

    M3GQuaternion a1 = q1 * m3gExp ( ( f1 * ( m3gLog ( q1.getInverse ( ) * q2 ) + m3gLog ( q0.getInverse ( ) * q1 ) ) * 0.5f - m3gLog ( q1.getInverse ( ) * q2 ) ) * 0.5f );
    M3GQuaternion b2 = q2 * m3gExp ( ( m3gLog ( q1.getInverse ( ) * q2 ) - f2 * ( m3gLog ( q2.getInverse ( ) * q3 ) + m3gLog ( q1.getInverse ( ) * q2 ) ) * 0.5f) * 0.5f );
    M3GQuaternion q  = m3gSlerp ( m3gSlerp ( q1, q2, alpha ), m3gSlerp ( a1, b2, alpha ), 2 * alpha * (1-alpha) );

    return q;
}

M3GQuaternion m3gLog ( const M3GQuaternion& q )
{
    KDfloat  x,y,z,w;
    KDfloat  alpha = kdAcosf ( q.w );
    
	if (alpha > 0 || alpha < 0) 
	{
        x = alpha * q.x / kdSinf ( alpha );
        y = alpha * q.y / kdSinf ( alpha );
        z = alpha * q.z / kdSinf ( alpha );
        w = 0;
    }
	else
	{
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    M3GQuaternion  q2;
    q2.set ( x, y, z, w );
    
	return q2;
}

M3GQuaternion m3gExp ( const M3GQuaternion& q )
{
	if ( q.w != 0 ) 
	{
		M3GException ( "ArithmeticException", __FUNCTION__, "W component of quaternion must be 0 for exponential, w = %f.", q.w );
	}

	KDfloat  x,y,z,w;
	KDfloat  alpha = q.getLength ( );

	if ( alpha > 0 || alpha < 0 )
	{
		x = q.x * kdSinf ( alpha ) / alpha;
		y = q.y * kdSinf ( alpha ) / alpha;
		z = q.z * kdSinf ( alpha ) / alpha;
		w = kdCosf ( alpha );
	} 
	else
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}

	M3GQuaternion  q2;
	q2.set ( x, y, z, w );
	
	return q2;
}

M3GQuaternion m3gMatrix2quat ( const M3GMatrix& m )
{
    KDfloat  q[4];
    KDfloat  trace = m[0] + m[5] + m[10];

    if ( trace > 0 )
	{
        KDfloat  s = kdSqrtf ( trace + 1.f );
        KDfloat  t = 0.5f / s;

        q[0] = ( m[9] - m[6] ) * t;
        q[1] = ( m[2] - m[8] ) * t;
        q[2] = ( m[4] - m[1] ) * t;
        q[3] = s * 0.5f;
    } 
	else 
	{
        KDint i = 0; 

        i = ( m[5]  > m[0]   ) ? 1 : i;
        i = ( m[10] > m[i*5] ) ? 2 : i;
        
		KDint    j = ( i+1 ) % 3;
        KDint    k = ( j+1 ) % 3;
        KDfloat  s = kdSqrtf ( ( m[i*5] - ( m[j*5] +m[k*5] ) ) + 1.0f );
        KDfloat  t = ( s != 0 ) ? 0.5f / s : s;

        q[i] = s * 0.5f;
        q[j] = ( m[j*4+i] + m[i*4+j] ) * t;
        q[k] = ( m[k*4+i] + m[i*4+k] ) * t;
        q[3] = ( m[k*4+j] - m[j*4+k] ) * t;

    }

    M3GQuaternion  quat;
    quat.set ( q[0], q[1], q[2], q[3] );

    return quat;
}

M3GQuaternion operator* ( KDfloat f, const M3GQuaternion& q )
{
	M3GQuaternion  r;

	r.x = f * q.x;
	r.y = f * q.y;
	r.z = f * q.z;
	r.w = f * q.w;
	
	return r;
}

M3GQuaternion operator* ( const M3GQuaternion& p, KDfloat f )
{
    return f * p;
}

M3GQuaternion operator* ( const M3GQuaternion& q1, const M3GQuaternion& q2 )
{
    M3GQuaternion  r;

    r.x = q1.w*q2.x + q2.w*q1.x + q1.y*q2.z - q1.z*q2.y;
    r.y = q1.w*q2.y + q2.w*q1.y + q1.z*q2.x - q1.x*q2.z;
    r.z = q1.w*q2.z + q2.w*q1.z + q1.x*q2.y - q1.y*q2.x;
    r.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
    
	return r;
}

M3GQuaternion operator+ ( const M3GQuaternion& q1, const M3GQuaternion& q2 )
{
	M3GQuaternion  r;

	r.x = q1.x + q2.x;
	r.y = q1.y + q2.y;
	r.z = q1.z + q2.z;
	r.w = q1.w + q2.w;
	
	return r;
}

M3GQuaternion operator- ( const M3GQuaternion& q1, const M3GQuaternion& q2 )
{
	M3GQuaternion  r;

	r.x = q1.x - q2.x;
	r.y = q1.y - q2.y;
	r.z = q1.z - q2.z;
	r.w = q1.w - q2.w;
	
	return r;
}