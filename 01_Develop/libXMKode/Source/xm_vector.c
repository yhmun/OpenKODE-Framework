/* --------------------------------------------------------------------------
 *
 *      File            xm_vector.c
 *      Description     Vector calculation function.
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
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

#include "kd_library.h"

KD_API XMVectorf KD_APIENTRY xmVectorCastFloat ( const XMVectorx* v )
{
	XMVectorf    vr;

	vr.x = KD_X2F ( v->x );
	vr.y = KD_X2F ( v->y );
	vr.z = KD_X2F ( v->z );

	return vr;
}

KD_API XMVectorx KD_APIENTRY xmVectorCastFixed ( const XMVectorf* v )
{
	XMVectorx    vr;

	vr.x = KD_F2X ( v->x );
	vr.y = KD_F2X ( v->y );
	vr.z = KD_F2X ( v->z );

	return vr;
}

KD_API KDfloat KD_APIENTRY xmVectorLength ( const XMVectorf* v )
{
	return kdSqrtf ( v->x * v->x + v->y * v->y + v->z * v->z );
}

KD_API XMVectorf KD_APIENTRY xmVectorNormalize ( const XMVectorf* v )
{
	XMVectorf    vr;
	KDfloat      l = xmVectorLength ( v );

	if ( l > -KD_FLT_EPSILON && l < KD_FLT_EPSILON )
	{
		kdLogMessage ( "xmVectorNormalize failed." ); 

		vr.x = 0;
		vr.y = 0;
		vr.z = 0;
	}
	else
	{
		vr.x = v->x / l;
		vr.y = v->y / l;
		vr.z = v->z / l;
	}

	return vr;
}

KD_API KDfloat KD_APIENTRY xmVectorDot ( const XMVectorf* v1, const XMVectorf* v2 )
{
	return ( v1->x * v2->x + v1->y * v2->y + v1->z * v2->z );
}

KD_API XMVectorf KD_APIENTRY xmVectorCross ( const XMVectorf* v1, const XMVectorf* v2 )
{
	XMVectorf    vr;

	vr.x = v1->y * v2->z - v1->z * v2->y;
	vr.y = v1->z * v2->x - v1->x * v2->z;
	vr.z = v1->x * v2->y - v1->y * v2->x;

	return vr;
}

KD_API XMVectorf KD_APIENTRY xmVectorAdd ( const XMVectorf* v1, const XMVectorf* v2 )
{
	XMVectorf    vr;

	vr.x = v1->x + v2->x;
	vr.y = v1->y + v2->y;
	vr.z = v1->z + v2->z;

	return vr;
}

KD_API XMVectorf KD_APIENTRY xmVectorSubstract ( const XMVectorf* v1, const XMVectorf* v2 )
{
	XMVectorf    vr;

	vr.x = v1->x - v2->x;
	vr.y = v1->y - v2->y;
	vr.z = v1->z - v2->z;

	return vr;
}

KD_API XMVectorf KD_APIENTRY xmVectorMultiply ( const XMVectorf* v, KDfloat s )
{
	XMVectorf    vr;

	vr.x = v->x * s;
	vr.y = v->y * s;
	vr.z = v->z * s;

	return vr;
}

KD_API XMVectorf KD_APIENTRY xmVectorDivide ( const XMVectorf* v, KDfloat s )
{
	XMVectorf    vr;

	if ( s == 0 )
	{
		kdLogMessage ( "xmVectorDivide failed." ); 

		vr.x = 0;
		vr.y = 0;
		vr.z = 0;
	}
	else
	{
		vr.x = v->x / s;
		vr.y = v->y / s;
		vr.z = v->z / s;
	}

	return vr;
}

KD_API XMVectorf KD_APIENTRY xmVectorTranform ( const KDfloat* m, const XMVectorf* v )
{
	XMVectorf    vr;

	vr.x = v->x * m[0] + v->y * m[4] + v->z * m[8];
    vr.y = v->x * m[1] + v->y * m[5] + v->z * m[9];
    vr.z = v->x * m[2] + v->y * m[6] + v->z * m[10];

	return vr;
}
