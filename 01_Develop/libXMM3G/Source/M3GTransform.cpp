/* --------------------------------------------------------------------------
 *
 *      File            M3GTransform.cpp
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
#include <XMM3G/M3GTransform.h>
#include <XMM3G/M3GVertexArray.h>

M3GTransform::M3GTransform ( KDvoid )
{
	m_pMatrix = new M3GMatrix ( );
}

M3GTransform::M3GTransform ( const M3GTransform& transform )
{
	m_pMatrix = new M3GMatrix ( transform.m_pMatrix->m ); 
}

M3GTransform::~M3GTransform ( KDvoid )
{
	delete m_pMatrix;
}

KDvoid M3GTransform::get ( KDfloat* matrix ) const
{
	if ( matrix == NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Matrix is NULL." );
	}

	kdMemcpy ( matrix, &m_pMatrix->m, sizeof ( m_pMatrix->m ) );
}

KDvoid M3GTransform::invert ( KDvoid )
{
	m_pMatrix->invert ();
}

KDvoid M3GTransform::postMultiply ( const M3GTransform& transform )
{
	*m_pMatrix *= *transform.m_pMatrix;
}

KDvoid M3GTransform::postRotate ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az )
{
	KDfloat  len2 = ax*ax + ay*ay + az*az;

	if ( len2 != 1.f ) 
	{
		KDfloat  len = kdSqrtf ( len2 );

		ax /= len;
		ay /= len;
		az /= len;
	}

	M3GMatrix  tRotate;

	tRotate.setRotate ( angle, ax, ay, az );

	*m_pMatrix *= tRotate;
}

KDvoid M3GTransform::postRotateQuat ( KDfloat qx, KDfloat qy, KDfloat qz, KDfloat qw )
{
	M3GQuaternion  tQ;
	tQ.set ( qx, qy, qz, qw );

	KDfloat  a[4];
	tQ.getAngleAxis ( a );

	M3GMatrix  tRotate;
	tRotate.setRotate ( a[0], a[1], a[2], a[3] );

	*m_pMatrix *= tRotate;
}

KDvoid M3GTransform::postScale ( KDfloat sx, KDfloat sy, KDfloat sz )
{
    M3GMatrix  tScale;
    tScale.setScale ( sx, sy, sz );

    *m_pMatrix *= tScale;
}

KDvoid M3GTransform::postTranslate ( KDfloat tx, KDfloat ty, KDfloat tz )
{
    M3GMatrix  tTranslate;
    tTranslate.setTranslate ( tx, ty, tz );

    *m_pMatrix *= tTranslate;
}

KDvoid M3GTransform::set ( const KDfloat* matrix )
{
    if ( matrix == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Matrix is NULL." );
    }

    m_pMatrix->set ( matrix );
}

KDvoid M3GTransform::set ( const M3GTransform* transform )
{
    if ( transform == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Transform is NULL." );
    }

    *m_pMatrix = *transform->m_pMatrix;
}

KDvoid M3GTransform::setIdentity ( KDvoid )
{
	m_pMatrix->setIdentity ( );
}

KDvoid M3GTransform::transform ( KDint numVectors, KDfloat* vectors ) const
{
	if ( vectors == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Vectors is NULL." );
	}

	if ( numVectors < 0 || numVectors % 4 != 0 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Vector num is invalid. numVectors = %d", numVectors );
	}

	for ( KDint  v = 0; v < numVectors; v += 4 )
	{
		M3GVector  tIn ( vectors [ v + 0 ], vectors [ v + 1 ], vectors [ v + 2 ], vectors [ v + 3 ] );        
		M3GVector  tOut = *m_pMatrix * tIn;
		tOut.get4 ( vectors + v );
	}
}

KDvoid M3GTransform::transform (const M3GVertexArray* in, KDfloat* out, KDbool W ) const
{
    if ( in == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "VertexArray is NULL." );
    }

    if ( out == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Out values is NULL." );
    }

    if ( in->getComponentCount ( ) == 4 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Component count must be 2 or 3." );
    }

    KDfloat  fScale   = 1;
    KDfloat  fBias[4] = { 0, 0, 0, 0 };

    for ( KDint  v = 0; v < in->getVertexCount ( ); v++ ) 
	{
		KDfloat*  pValues = out + v * 4;

		pValues [ 0 ] = pValues [ 1 ] = pValues [ 2 ] = 0;
		pValues [ 3 ] = W ? 1.f : 0;

		in->get ( v, 1, fScale, fBias, pValues );
		
		transform ( 4, pValues );
    }	
}

M3GVector M3GTransform::transform ( const M3GVector& vector ) const
{
	return *m_pMatrix * vector;
}

M3GVector M3GTransform::transform3x3 ( const M3GVector& vector ) const
{
	return m_pMatrix->get3x3 ( ) * vector;
}

KDvoid M3GTransform::transpose ( KDvoid )
{
	m_pMatrix->transpose ( );
}

M3GMatrix M3GTransform::getMatrix ( KDvoid ) const
{
	return *m_pMatrix;
}