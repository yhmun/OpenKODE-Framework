/* --------------------------------------------------------------------------
 *
 *      File            M3GInternal.cpp
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

KDvoid M3GException ( const KDchar* szName, const KDchar* szFunc, const KDchar* szFormat, ... )
{
	KDchar  szBuffer [ 256 ];

    KDVaListKHR  Args;
    KD_VA_START_KHR ( Args, szFormat );
    kdVsnprintfKHR ( szBuffer, 256, szFormat, Args);
    KD_VA_END_KHR ( Args );

	kdLogMessagefKHR ( "[ %s ] %s -> %s", szName, szFunc, szBuffer );
	kdExit ( 1 );
}

KDbool M3GTriangleIntersect ( const M3GVector& org, const M3GVector& dir, 
                              const M3GVector& v0, const M3GVector& v1, const M3GVector& v2,
                              KDfloat* u_, KDfloat* v_, KDfloat* t_ )
{
    if ( org.w != 1 || dir.w != 1 || v0.w != 1 || v1.w != 1 || v2.w != 1 ) 
	{
    //    throw IllegalArgumentException (__FILE__, __func__, "W of input vectors must be 1, org.w=%f, dir.w=%f, v0.w=%f, v1.w=%f, v2.w=%f",
      //                                  org.w, dir.w, v0.w, v1.w, v2.w);
    }

    M3GVector  e1 = v1 - v0;
    M3GVector  e2 = v2 - v0;

    M3GVector  p  = m3gCross ( dir, e2 );

    KDfloat  a  = m3gDot ( e1, p );
    if ( a < KD_FLT_EPSILON )
	{
        return KD_FALSE;
	}

    M3GVector  s = org - v0;
    KDfloat  u = m3gDot ( s, p );
    if ( u < 0 || u > a )
	{
        return KD_FALSE;
	}

    M3GVector  q = m3gCross ( s, e1 );

    KDfloat  v = m3gDot ( dir, q );
    if ( v < 0 || u + v > a )
	{
        return KD_FALSE;
	}

    KDfloat  t = m3gDot ( e2, q );
    KDfloat  f = 1 / a;

    t *= f;
    u *= f;
    v *= f;

    if ( t < 0 )
	{
        return KD_FALSE;
	}

    if ( u_ )
	{
        *u_ = u;
	}

    if ( v_ )
	{
        *v_ = v;
	}

    if ( t_ )
	{
        *t_ = t;
	}

    return KD_TRUE;
}