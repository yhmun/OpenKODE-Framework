/* --------------------------------------------------------------------------
 *
 *      File            M3GKeyframe.cpp
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

M3GKeyframe::M3GKeyframe ( KDvoid )
: m_nTime ( -1 )
, m_pValue ( KD_NULL )
{

}

M3GKeyframe::M3GKeyframe ( KDint time, KDfloat* value )
: m_nTime ( time )
, m_pValue ( value )
{

}

M3GKeyframe::~M3GKeyframe ( KDvoid )
{

}

KDvoid m3gStep ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, KDint componentCount, KDfloat* value )
{
    if ( value == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Value is NULL." );
    }

    if ( s < 0 || s > 1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "S is invalid, s = %f.", s );
    }

    for ( KDint  i = 0; i < componentCount; i++ )
	{
		value [ i ] = ( s < 1 ) ? k0.m_pValue [ i ] : k1.m_pValue [ i ];
    }
}

KDvoid m3gLinear ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, KDint componentCount, KDfloat* value )
{
    if ( value == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Value is NULL." );
    }

    if ( s < 0 || s > 1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "S is invalid, s = %f.", s );
    }

    for ( KDint  i = 0; i < componentCount; i++ )
	{
        value [ i ] = k0.m_pValue [ i ] * ( 1 - s ) + k1.m_pValue [ i ] * ( s );
    }
}

KDvoid m3gSlerp ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, KDint componentCount, KDfloat* value )
{
    if ( value == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Value is NULL." );
    }

    if ( s < 0 || s > 1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "S is invalid, s = %f.", s );
    }

    if ( componentCount != 4 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Component count must be 4, count = %d.", componentCount );
    }

    M3GQuaternion  q0, q1, q2;

    q0.set ( k0.m_pValue[0], k0.m_pValue[1], k0.m_pValue[2], k0.m_pValue[3] );
    q1.set ( k1.m_pValue[0], k1.m_pValue[1], k1.m_pValue[2], k1.m_pValue[3] );
    
	q2 = m3gSlerp ( q0, q1, s );

    value[0] = q2.x;
    value[1] = q2.y;
    value[2] = q2.z;
    value[3] = q2.w;
}

KDvoid m3gSpline ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, const M3GKeyframe& k2, const M3GKeyframe& k3, KDint componentCount, KDfloat* value )
{
    if ( value == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Value is NULL." );
    }

    if ( s < 0 || s > 1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "S is invalid, s = %f.", s );
    }

    KDfloat  sh0 =  2*s*s*s - 3*s*s + 1;
    KDfloat  sh1 = -2*s*s*s + 3*s*s;
    KDfloat  sh2 =    s*s*s - 2*s*s + s;
    KDfloat  sh3 =    s*s*s -   s*s;

    for ( KDint  i = 0; i < componentCount; i++ )
	{
		KDfloat  tan1 = ( k0.m_nTime == -1 ) ? 0 : (k2.m_nTime - k1.m_nTime) / (KDfloat)( k2.m_nTime - k0.m_nTime ) * ( k2.m_pValue[i] - k0.m_pValue[i] );
        KDfloat  tan2 = ( k3.m_nTime == -1 ) ? 0 : (k2.m_nTime - k1.m_nTime) / (KDfloat)( k3.m_nTime - k1.m_nTime ) * ( k3.m_pValue[i] - k1.m_pValue[i] );

        value[i] = sh0*k1.m_pValue[i] + sh1*k2.m_pValue[i] + sh2*tan1 + sh3*tan2;
    }
}

KDvoid m3gSquad ( KDfloat s, const M3GKeyframe& k0, const M3GKeyframe& k1, const M3GKeyframe& k2, const M3GKeyframe& k3, KDint componentCount, KDfloat* value )
{
    if ( value == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Value is NULL." );
    }

    if ( s < 0 || s > 1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "S is invalid, s = %f.", s );
    }

    if ( componentCount != 4 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Component count must be 4, count = %d.", componentCount );
    }

    M3GQuaternion  q0, q1, q2, q3;
    
	if ( k0.m_nTime == -1 )
	{
        q0.set ( k1.m_pValue[0], k1.m_pValue[1], k1.m_pValue[2], k1.m_pValue[3] );
	}
    else
	{
        q0.set ( k0.m_pValue[0], k0.m_pValue[1], k0.m_pValue[2], k0.m_pValue[3] );
	}

    q1.set ( k1.m_pValue[0], k1.m_pValue[1], k1.m_pValue[2], k1.m_pValue[3] );
    q2.set ( k2.m_pValue[0], k2.m_pValue[1], k2.m_pValue[2], k2.m_pValue[3] );

    if ( k3.m_nTime == -1 )
	{
        q3.set ( k2.m_pValue[0], k2.m_pValue[1], k2.m_pValue[2], k2.m_pValue[3] );
	}
    else
	{
        q3.set ( k3.m_pValue[0], k3.m_pValue[1], k3.m_pValue[2], k3.m_pValue[3] );
	}
  
    KDfloat  t0, t1, t2, t3;

    t0 = (KDfloat) ( ( k0.m_nTime == -1 ) ? k1.m_nTime : k0.m_nTime );
    t1 = (KDfloat) ( k1.m_nTime );
    t2 = (KDfloat) ( k2.m_nTime );
    t3 = (KDfloat) ( ( k3.m_nTime == -1 ) ? k2.m_nTime : k3.m_nTime );

    M3GQuaternion  q4 = m3gSquad ( q0, q1, q2, q3, t0, t1, t2, t3, s );

    value[0] = q4.x;
    value[1] = q4.y;
    value[2] = q4.z;
    value[3] = q4.w;
}
