/* --------------------------------------------------------------------------
 *
 *      File            xm_matrix.c
 *      Description     Matrix calculation function.
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

KD_API KDvoid KD_APIENTRY xmMatrixCopy ( KDvoid* m, const KDvoid* m1 )
{
	kdMemcpy ( m, m1, 16 * 4 );
}

KD_API KDvoid KD_APIENTRY xmMatrixCastFloat ( KDfloat* m, const KDfixed* m1 )
{
	m[ 0] = KD_X2F ( m1[ 0] );	m[ 1] = KD_X2F ( m1[ 1] );	m[ 2] = KD_X2F ( m1[ 2] );	m[ 3] = KD_X2F ( m1[ 3] );
	m[ 4] = KD_X2F ( m1[ 4] );	m[ 5] = KD_X2F ( m1[ 5] );	m[ 6] = KD_X2F ( m1[ 6] );	m[ 7] = KD_X2F ( m1[ 7] );
	m[ 8] = KD_X2F ( m1[ 8] );	m[ 9] = KD_X2F ( m1[ 9] );	m[10] = KD_X2F ( m1[10] );	m[11] = KD_X2F ( m1[11] );
	m[12] = KD_X2F ( m1[12] );	m[13] = KD_X2F ( m1[13] );	m[14] = KD_X2F ( m1[14] );	m[15] = KD_X2F ( m1[15] );
}

KD_API KDvoid KD_APIENTRY xmMatrixCastFixed ( KDfixed* m, const KDfloat* m1 )
{
	m[ 0] = KD_F2X ( m1[ 0] );	m[ 1] = KD_F2X ( m1[ 1] );	m[ 2] = KD_F2X ( m1[ 2] );	m[ 3] = KD_F2X ( m1[ 3] );
	m[ 4] = KD_F2X ( m1[ 4] );	m[ 5] = KD_F2X ( m1[ 5] );	m[ 6] = KD_F2X ( m1[ 6] );	m[ 7] = KD_F2X ( m1[ 7] );
	m[ 8] = KD_F2X ( m1[ 8] );	m[ 9] = KD_F2X ( m1[ 9] );	m[10] = KD_F2X ( m1[10] );	m[11] = KD_F2X ( m1[11] );
	m[12] = KD_F2X ( m1[12] );	m[13] = KD_F2X ( m1[13] );	m[14] = KD_F2X ( m1[14] );	m[15] = KD_F2X ( m1[15] );
}

KD_API KDvoid KD_APIENTRY xmMatrixIdentity ( KDfloat* m )
{
	m[ 0] = 1.0f;	m[ 1] = 0.0f;	m[ 2] = 0.0f;	m[ 3] = 0.0f;
	m[ 4] = 0.0f;	m[ 5] = 1.0f;	m[ 6] = 0.0f;	m[ 7] = 0.0f;
	m[ 8] = 0.0f;	m[ 9] = 0.0f;	m[10] = 1.0f;	m[11] = 0.0f;
	m[12] = 0.0f;	m[13] = 0.0f;	m[14] = 0.0f;	m[15] = 1.0f;
}

KD_API KDvoid KD_APIENTRY xmMatrixInverse ( KDfloat* m, const KDfloat* m1 )
{
    KDint       i, j, k, s;
	KDfloat     tmp;
    KDfloat     t[4][4];
    
    xmMatrixIdentity ( m );
    
    for ( i = 0; i < 4; i++ ) 
	{
        for ( j = 0; j < 4; j++ )
		{
            t[ i ][ j ] = m1[ i * 4 + j ];
        }
    }
    
    for ( i = 0; i < 4; i++ ) 
	{
        s = i;                           // look for largest element in column.

        for ( j = i + 1; j < 4; j++ ) 
		{
            if ( kdFabsf ( t[ j ][ i ] ) > kdFabsf ( t[ i ][ i ] ) )
			{
                s = j;
            }
        }
        
        if ( s != i ) 
		{            
            for ( k = 0; k < 4; k++ )      // swap rows.
			{
                tmp         = t[ i ][ k ];
                t[ i ][ k ] = t[ s ][ k ];
                t[ s ][ k ] = tmp;
                
                tmp            = m[ i * 4 + k ];
                m[ i * 4 + k ] = m[ s * 4 + k ];
                m[ s * 4 + k ] = tmp;
            }
        }
            
        tmp = t[ i ][ i ];
		if ( tmp == 0 )
		{
			kdLogMessage ( "xmMatrixInverse failed." ); 
			return;
		}

        for ( k = 0; k < 4; k++ )
		{
            t[ i ][ k ] /= tmp;
            m[ i * 4 + k ] /= tmp;
        }

        for ( j = 0; j < 4; j++ ) 
		{
            if ( j != i ) 
			{
                tmp = t[ j ][ i ];

                for ( k = 0; k < 4; k++ )
				{
                    t[ j ][ k ] -= t[ i ][ k ] * tmp;
                    m[ j * 4 + k ] -= m[ i * 4 + k ] * tmp;
                }
            }
        }
    }
}

KD_API KDvoid KD_APIENTRY xmMatrixMultiply ( KDfloat* m, const KDfloat* m1 )
{
	XMMatrixf    mt;

	xmMatrixMultiply2 ( mt, m, m1 );

	kdMemcpy ( m, mt, sizeof ( KDfloat ) * 16 );
}

KD_API KDvoid KD_APIENTRY xmMatrixMultiply2 ( KDfloat* m, const KDfloat* m1, const KDfloat* m2 )
{
	m[ 0] = m1[ 0] * m2[ 0] + m1[ 4] * m2[ 1] + m1[ 8] * m2[ 2] + m1[12] * m2[ 3];
	m[ 1] = m1[ 1] * m2[ 0] + m1[ 5] * m2[ 1] + m1[ 9] * m2[ 2] + m1[13] * m2[ 3];
	m[ 2] = m1[ 2] * m2[ 0] + m1[ 6] * m2[ 1] + m1[10] * m2[ 2] + m1[14] * m2[ 3];
	m[ 3] = m1[ 3] * m2[ 0] + m1[ 7] * m2[ 1] + m1[11] * m2[ 2] + m1[15] * m2[ 3];
	
	m[ 4] = m1[ 0] * m2[ 4] + m1[ 4] * m2[ 5] + m1[ 8] * m2[ 6] + m1[12] * m2[ 7];
	m[ 5] = m1[ 1] * m2[ 4] + m1[ 5] * m2[ 5] + m1[ 9] * m2[ 6] + m1[13] * m2[ 7];
	m[ 6] = m1[ 2] * m2[ 4] + m1[ 6] * m2[ 5] + m1[10] * m2[ 6] + m1[14] * m2[ 7];
	m[ 7] = m1[ 3] * m2[ 4] + m1[ 7] * m2[ 5] + m1[11] * m2[ 6] + m1[15] * m2[ 7];
	
	m[ 8] = m1[ 0] * m2[ 8] + m1[ 4] * m2[ 9] + m1[ 8] * m2[10] + m1[12] * m2[11];
	m[ 9] = m1[ 1] * m2[ 8] + m1[ 5] * m2[ 9] + m1[ 9] * m2[10] + m1[13] * m2[11];
	m[10] = m1[ 2] * m2[ 8] + m1[ 6] * m2[ 9] + m1[10] * m2[10] + m1[14] * m2[11];
	m[11] = m1[ 3] * m2[ 8] + m1[ 7] * m2[ 9] + m1[11] * m2[10] + m1[15] * m2[11];
	
	m[12] = m1[ 0] * m2[12] + m1[ 4] * m2[13] + m1[ 8] * m2[14] + m1[12] * m2[15];
	m[13] = m1[ 1] * m2[12] + m1[ 5] * m2[13] + m1[ 9] * m2[14] + m1[13] * m2[15];
	m[14] = m1[ 2] * m2[12] + m1[ 6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	m[15] = m1[ 3] * m2[12] + m1[ 7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];	
}

KD_API KDvoid KD_APIENTRY xmMatrixTranslate ( KDfloat* m, KDfloat x, KDfloat y, KDfloat z )
{
	m[12] = m[ 0] * x + m[ 4] * y + m[ 8] * z + m[12];
	m[13] = m[ 1] * x + m[ 5] * y + m[ 9] * z + m[13];
	m[14] = m[ 2] * x + m[ 6] * y + m[10] * z + m[14];
	m[15] = m[ 3] * x + m[ 7] * y + m[11] * z + m[15];
}

KD_API KDvoid KD_APIENTRY xmMatrixScale ( KDfloat* m, KDfloat x, KDfloat y, KDfloat z )
{
	m[ 0] *= x; m[ 4] *= y; m[ 8] *= z;  
	m[ 1] *= x; m[ 5] *= y; m[ 9] *= z; 
	m[ 2] *= x; m[ 6] *= y; m[10] *= z; 
}

KD_API KDvoid KD_APIENTRY xmMatrixRotate ( KDfloat* m, KDfloat angle, KDfloat x, KDfloat y, KDfloat z )
{
	XMMatrixf	mr;	

	KDfloat     s  = kdSinf ( KD_DEG2RAD ( angle ) );
	KDfloat     c  = kdCosf ( KD_DEG2RAD ( angle ) );
	KDfloat     l  = kdSqrtf ( x * x + y * y + z * z );
	KDfloat     ux = 0; 
	KDfloat     uy = 0; 
	KDfloat     uz = 0; 
	KDfloat     c1 = 0; 

	if ( l > -KD_FLT_EPSILON && l < KD_FLT_EPSILON )
	{
		kdLogMessage ( "xmMatrixRotate failed." ); 

		return;
	}

	ux = x / l;
	uy = y / l;
	uz = z / l;
	c1 = 1 - c;

	mr[ 0] = ux * ux * c1 + c;
	mr[ 1] = uy * ux * c1 + uz * s;
	mr[ 2] = ux * uz * c1 - uy * s;
	mr[ 3] = 0.0f;

	mr[ 4] = ux * uy * c1 - uz * s;
	mr[ 5] = uy * uy * c1 + c;
	mr[ 6] = uy * uz * c1 + ux * s;
	mr[ 7] = 0.0f;
	
	mr[ 8] = ux * uz * c1 + uy * s;
	mr[ 9] = uy * uz * c1 - ux * s;
	mr[10] = uz * uz * c1 + c;
	mr[11] = 0.0f;

	mr[12] = 0.0f;
	mr[13] = 0.0f;
	mr[14] = 0.0f;
	mr[15] = 1.0f;

	xmMatrixMultiply ( m, mr );	
}

KD_API KDvoid KD_APIENTRY xmMatrixOrtho ( KDfloat* m, KDfloat left, KDfloat right, KDfloat bottom, KDfloat top, KDfloat znear, KDfloat zfar )
{
	KDfloat    diff[3];

	diff[ 0 ] = right - left;
	diff[ 1 ] = top - bottom;
	diff[ 2 ] = zfar - znear;

	if ( ( diff[ 0 ] > -KD_FLT_EPSILON && diff[ 0 ] < KD_FLT_EPSILON ) ||
	     ( diff[ 1 ] > -KD_FLT_EPSILON && diff[ 1 ] < KD_FLT_EPSILON ) ||
	     ( diff[ 2 ] > -KD_FLT_EPSILON && diff[ 2 ] < KD_FLT_EPSILON ) )
	{
		kdLogMessage ( "xmMatrixOrtho failed" );

		return;
	}

	m[ 0] = 2.0f / ( right - left );	
	m[ 1] = 0.0f;	
	m[ 2] = 0.0f;
	m[ 3] = 0.0f;

	m[ 4] = 0.0f;
	m[ 5] = 2.0f / ( top - bottom );
	m[ 6] = 0.0f;
	m[ 7] = 0.0f;

	m[ 8] = 0.0f;		
	m[ 9] = 0.0f;
	m[10] = -2.f / ( zfar - znear );
	m[11] = 0.0f;

	m[12] = -( right + left ) / ( right - left );	
	m[13] = -( top + bottom ) / ( top - bottom );
	m[14] = -( zfar + znear ) / ( zfar - znear );
	m[15] = 1.0f;
}

KD_API KDvoid KD_APIENTRY xmMatrixFrustum ( KDfloat* m, KDfloat left, KDfloat right, KDfloat bottom, KDfloat top, KDfloat znear, KDfloat zfar )
{
	KDfloat    diff[3];

	diff[ 0 ] = right - left;
	diff[ 1 ] = top - bottom;
	diff[ 2 ] = zfar - znear;

	if ( ( diff[ 0 ] > -KD_FLT_EPSILON && diff[ 0 ] < KD_FLT_EPSILON ) ||
	     ( diff[ 1 ] > -KD_FLT_EPSILON && diff[ 1 ] < KD_FLT_EPSILON ) ||
	     ( diff[ 2 ] > -KD_FLT_EPSILON && diff[ 2 ] < KD_FLT_EPSILON ) )
	{
		kdLogMessage ( "xmMatrixFrustum failed" );

		return;
	}

	m[ 0] = 2.0f * znear / ( right - left );	
	m[ 1] = 0.0f;	
	m[ 2] = 0.0f;
	m[ 3] = 0.0f;

	m[ 4] = 0.0f;
	m[ 5] = 2.0f * znear / ( top - bottom );
	m[ 6] = 0.0f;
	m[ 7] = 0.0f;

	m[ 8] = ( right + left ) / ( right - left );		
	m[ 9] = ( top + bottom ) / ( top - bottom );
	m[10] = ( -zfar - znear ) / ( zfar - znear );
	m[11] = -1.0f;

	m[12] = 0.0f;	
	m[13] = 0.0f;
	m[14] = ( -( 2.0f * znear) * zfar ) / ( zfar - znear );
	m[15] = 0.0f;
}

KD_API KDvoid KD_APIENTRY xmMatrixPerspective ( KDfloat* m, KDfloat fovy, KDfloat aspect, KDfloat znear, KDfloat zfar )
{
	KDfloat     w, h;
	KDfloat     w2, h2;

	if ( fovy <= 0.0f || fovy >= 180.0f )
	{
		kdLogMessage ( "xmMatrixPerspective failed." ); 

		return;
	}

	h  = 2.0f * znear * kdTanf ( KD_DEG2RAD ( fovy ) * 0.5f );
	w  = aspect * h;
	h2 = h * 0.5f;
	w2 = w * 0.5f;

	xmMatrixFrustum ( m, -w2, w2, -h2, h2, znear, zfar );
}

KD_API KDvoid KD_APIENTRY xmMatrixLookat ( KDfloat* m, KDfloat eye_x, KDfloat eye_y, KDfloat eye_z, KDfloat center_x, KDfloat center_y, KDfloat center_z, KDfloat up_x, KDfloat up_y, KDfloat up_z )
{	
	XMVectorf   ve   = { eye_x   , eye_y   , eye_z    };
	XMVectorf   vc   = { center_x, center_y, center_z };
	XMVectorf   vu   = { up_x    , up_y    , up_z     };

	XMVectorf   vf   = xmVectorSubstract ( &ve, &vc );
	XMVectorf   vnf  = xmVectorNormalize ( &vf );
	XMVectorf   vnu  = xmVectorNormalize ( &vu );
	XMVectorf   vs   = xmVectorCross ( &vnf, &vnu );
	XMVectorf   vns  = xmVectorNormalize ( &vs );

	XMVectorf   vuu  = xmVectorCross ( &vns , &vnf );
	XMVectorf   vnuu = xmVectorNormalize ( &vuu );

	m[0] =   vns.x; m[4] =  vns.y; m[ 8] =  vns.z; m[12] = 0.0f;
	m[1] =  vnuu.x; m[5] = vnuu.y; m[ 9] = vnuu.z; m[13] = 0.0f;
	m[2] =  -vnf.x; m[6] = -vnf.y; m[10] = -vnf.z; m[14] = 0.0f;
	m[3] =    0.0f; m[7] =   0.0f; m[11] =   0.0f; m[15] = 1.0f;

	xmMatrixTranslate ( m, -vc.x, -vc.y, -vc.z );
}