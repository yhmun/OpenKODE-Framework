/* --------------------------------------------------------------------------
 *
 *      File            xm_matrix.h
 *      Description     Matrix calculation function.
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

#ifndef __xm_matrix_h__
#define __xm_matrix_h__

#include <KD/kd.h>
#include "xm_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef KDfloat          XMMatrixf[16];
typedef KDfixed          XMMatrixx[16];

KD_API KDvoid KD_APIENTRY xmMatrixCopy        ( KDvoid*  m, const KDvoid*  m1 );
KD_API KDvoid KD_APIENTRY xmMatrixCastFloat   ( KDfloat* m, const KDfixed* m1 );
KD_API KDvoid KD_APIENTRY xmMatrixCastFixed   ( KDfixed* m, const KDfloat* m1 );

KD_API KDvoid KD_APIENTRY xmMatrixIdentity    ( KDfloat* m ); 
KD_API KDvoid KD_APIENTRY xmMatrixInverse     ( KDfloat* m, const KDfloat* m1 );
KD_API KDvoid KD_APIENTRY xmMatrixMultiply    ( KDfloat* m, const KDfloat* m1 );
KD_API KDvoid KD_APIENTRY xmMatrixMultiply2   ( KDfloat* m, const KDfloat* m1, const KDfloat* m2 );

KD_API KDvoid KD_APIENTRY xmMatrixTranslate   ( KDfloat* m, KDfloat x, KDfloat y, KDfloat z );
KD_API KDvoid KD_APIENTRY xmMatrixScale       ( KDfloat* m, KDfloat x, KDfloat y, KDfloat z );
KD_API KDvoid KD_APIENTRY xmMatrixRotate      ( KDfloat* m, KDfloat angle, KDfloat x, KDfloat y, KDfloat z );

KD_API KDvoid KD_APIENTRY xmMatrixOrtho       ( KDfloat* m, KDfloat left, KDfloat right, KDfloat bottom, KDfloat top, KDfloat znear, KDfloat zfar );
KD_API KDvoid KD_APIENTRY xmMatrixFrustum     ( KDfloat* m, KDfloat left, KDfloat right, KDfloat bottom, KDfloat top, KDfloat znear, KDfloat zfar );
KD_API KDvoid KD_APIENTRY xmMatrixPerspective ( KDfloat* m, KDfloat fovy, KDfloat aspect, KDfloat znear, KDfloat zfar );

KD_API KDvoid KD_APIENTRY xmMatrixLookat      ( KDfloat* m, KDfloat eye_x, KDfloat eye_y, KDfloat eye_z, KDfloat center_x, KDfloat center_y, KDfloat center_z, KDfloat up_x, KDfloat up_y, KDfloat up_z );

#ifdef __cplusplus
}
#endif

#endif 



