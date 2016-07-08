/* --------------------------------------------------------------------------
 *
 *      File            xm_vector.h
 *      Description     Vector calculation function.
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
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

#ifndef __xm_vector_h__
#define __xm_vector_h__

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
	KDfloat        x;
	KDfloat        y;
	KDfloat        z;
} XMVectorf;

typedef struct 
{
	KDfixed        x;
	KDfixed        y;
	KDfixed        z;
} XMVectorx;

KD_API XMVectorf KD_APIENTRY xmVectorCastFloat ( const XMVectorx* v );
KD_API XMVectorx KD_APIENTRY xmVectorCastFixed ( const XMVectorf* v );

KD_API KDfloat   KD_APIENTRY xmVectorLength    ( const XMVectorf* v );
KD_API XMVectorf KD_APIENTRY xmVectorNormalize ( const XMVectorf* v );
KD_API KDfloat   KD_APIENTRY xmVectorDot       ( const XMVectorf* v1, const XMVectorf* v2 );
KD_API XMVectorf KD_APIENTRY xmVectorCross     ( const XMVectorf* v1, const XMVectorf* v2 );
KD_API XMVectorf KD_APIENTRY xmVectorAdd       ( const XMVectorf* v1, const XMVectorf* v2 );
KD_API XMVectorf KD_APIENTRY xmVectorSubstract ( const XMVectorf* v1, const XMVectorf* v2 );
KD_API XMVectorf KD_APIENTRY xmVectorMultiply  ( const XMVectorf* v, KDfloat s );
KD_API XMVectorf KD_APIENTRY xmVectorDivide    ( const XMVectorf* v, KDfloat s );

KD_API XMVectorf KD_APIENTRY xmVectorTranform  ( const KDfloat* m, const XMVectorf* v );

#ifdef __cplusplus
}
#endif

#endif 



