/* --------------------------------------------------------------------------
 *
 *      File            M3GRayIntersection.cpp
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
#include <XMM3G/M3GRayIntersection.h>
#include <XMM3G/M3GNode.h>
#include <XMM3G/M3GSprite3D.h>
#include <XMM3G/M3GMesh.h>
#include <XMM3G/M3GVertexBuffer.h>
#include <XMM3G/M3GVertexArray.h>

M3GRayIntersection::M3GRayIntersection ( KDvoid )
: m_fRayT ( 0 )
, m_pNode ( KD_NULL )
, m_fU ( 0 )
, m_fV ( 0 )
, m_nSubmeshIndex ( 0 )
, m_pNormal ( KD_NULL )
{
	m_pRayOrg = new M3GVector ( 0, 0, 0 );
	m_pRayDir = new M3GVector ( 0, 0, 0 );

	m_vTexCoord.resize ( MAX_TEXTURE_UNITS );

	for ( KDint  i = 0; i < MAX_TEXTURE_UNITS; i++ ) 
	{
		m_vTexCoord [ i ] = 0;
	}
}

M3GRayIntersection::M3GRayIntersection ( M3GNode* node,
										 const M3GVector& org, const M3GVector& dir, KDfloat t,
										 KDfloat u, KDfloat v,
										 KDint numVertices, KDint* vertices,
										 KDint submeshIndex )
: m_fRayT ( t )
, m_pNode ( node )
, m_fU ( u )
, m_fV ( v )
, m_nSubmeshIndex ( submeshIndex )
, m_pNormal ( KD_NULL )
{
	m_pRayOrg = new M3GVector ( );
	m_pRayDir = new M3GVector ( );

	*m_pRayOrg = org;
	*m_pRayDir = dir;

	m_vVertices.resize ( numVertices );

	for ( KDint  i = 0; i < numVertices; i++ )
	{
        m_vVertices.push_back ( vertices [ i ] );
    }

	m_vTexCoord.resize ( MAX_TEXTURE_UNITS );

	for ( KDint  i = 0; i < MAX_TEXTURE_UNITS; i++ ) 
	{
		m_vTexCoord [ i ] = 0;
	}
}

M3GRayIntersection::~M3GRayIntersection ( KDvoid )
{
	delete m_pRayOrg;
	delete m_pRayDir;

	if ( m_pNormal )
	{
		delete m_pNormal;
	}
}

KDfloat M3GRayIntersection::getDistance ( KDvoid ) const
{
	return m_fRayT;
}

M3GNode* M3GRayIntersection::getIntersected ( KDvoid ) const
{
	return m_pNode;
}

KDfloat M3GRayIntersection::getNormalX ( KDvoid ) const
{
    if ( !m_pNormal )
	{
        const_cast<M3GRayIntersection*> ( this )->computeNormal ( );
    }

    return m_pNormal->x;
}

KDfloat M3GRayIntersection::getNormalY ( KDvoid ) const
{
	if ( !m_pNormal )
	{
        const_cast<M3GRayIntersection*> ( this )->computeNormal ( );
    }

    return m_pNormal->y;
}

KDfloat	M3GRayIntersection::getNormalZ ( KDvoid ) const
{
	if ( !m_pNormal )
	{
        const_cast<M3GRayIntersection*> ( this )->computeNormal ( );
    }

    return m_pNormal->z;
}

KDvoid M3GRayIntersection::getRay ( KDfloat* ray ) const
{
	if ( ray == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Ray is NULL." );
    }

    ray [ 0 ] = m_pRayOrg->x;
    ray [ 1 ] = m_pRayOrg->y;
    ray [ 2 ] = m_pRayOrg->z;
    ray [ 3 ] = m_pRayDir->x;
    ray [ 4 ] = m_pRayDir->y;
    ray [ 5 ] = m_pRayDir->z;
}

KDint M3GRayIntersection::getSubmeshIndex ( KDvoid ) const
{
	return m_nSubmeshIndex;
}

KDfloat M3GRayIntersection::getTextureS ( KDint index ) const
{
	if ( index < 0 || index >= MAX_TEXTURE_UNITS ) 
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d.", index );
    }

    if ( !m_vTexCoord [ index ] )
	{
        const_cast<M3GRayIntersection*> ( this )->computeTexCoord ( index );
    }

    return m_vTexCoord [ index ]->x;
}

KDfloat M3GRayIntersection::getTextureT ( KDint index ) const
{
	if ( index < 0 || index >= MAX_TEXTURE_UNITS ) 
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d.", index );
    }

    if ( !m_vTexCoord [ index ] )
	{
        const_cast<M3GRayIntersection*> ( this )->computeTexCoord ( index );
    }

    return m_vTexCoord [ index ]->y;
}

KDvoid M3GRayIntersection::transformRay ( const M3GTransform& trans )
{
	if ( m_pRayDir->length ( ) == 0 )
	{
		M3GException ( "InternalException", __FUNCTION__, "Ray dir is 0. this never happen." );
    }

    M3GVector  tOrg  = *m_pRayOrg;
    M3GVector  tDst0 = *m_pRayOrg + *m_pRayDir;
    M3GVector  tDst1 = *m_pRayOrg + *m_pRayDir * m_fRayT;

	tOrg  = trans.transform ( tOrg  ).dividedByW ( );
    tDst0 = trans.transform ( tDst0 ).dividedByW ( );
    tDst1 = trans.transform ( tDst1 ).dividedByW ( );

    *m_pRayOrg = tOrg;
    *m_pRayDir = tDst0 - tOrg;
     m_fRayT   = ( tDst1 - tOrg ).length ( ) / ( tDst0 - tOrg ).length ( );
}

KDvoid M3GRayIntersection::computeNormal ( KDvoid )
{
	m_pNormal = new M3GVector ( 0, 0, 1);

    M3GSprite3D*  pSprite = dynamic_cast<M3GSprite3D*> ( m_pNode );
    if ( pSprite )
	{
        return;
    }

    M3GMesh*  pMesh = dynamic_cast<M3GMesh*> ( m_pNode );
    if ( pMesh ) 
	{
        KDfloat  fScaleBias [ 4 ];
        M3GVertexArray*  pNormals = pMesh->getVertexBuffer ( )->getNormals ( fScaleBias );
        if ( pNormals )
		{
            KDfloat  fScale   = fScaleBias [ 0 ];
            KDfloat  fBias[3] = { fScaleBias [ 1 ], fScaleBias [ 2 ], fScaleBias [ 3 ] };
            KDfloat  fNormalValues [ 3 ][ 3 ];

            pNormals->get (                 0, 1, fScale, fBias, fNormalValues[0] );
            pNormals->get ( m_vVertices [ 0 ], 1, fScale, fBias, fNormalValues[0] );
            pNormals->get ( m_vVertices [ 1 ], 1, fScale, fBias, fNormalValues[1] );
            pNormals->get ( m_vVertices [ 2 ], 1, fScale, fBias, fNormalValues[2] );

            M3GVector tN0 = M3GVector ( fNormalValues [ 0 ] );
            M3GVector tN1 = M3GVector ( fNormalValues [ 1 ] );
            M3GVector tN2 = M3GVector ( fNormalValues [ 2 ] );

            *m_pNormal = m3gLerp ( m_fU, m_fV, tN0, tN1, tN2 );
            if ( m_pNormal->length ( ) > 0 )
			{
                m_pNormal->normalize ( );
			}
        }
    }
}

KDvoid M3GRayIntersection::normalizeRay ( KDvoid )
{
    m_fRayT *= m_pRayDir->length ( );
    m_pRayDir->normalize ( );
}

KDvoid M3GRayIntersection::computeTexCoord ( KDint index )
{
	m_vTexCoord [ index ] = new M3GVector ( 0, 0, 0 );

    M3GSprite3D*  pSprite = dynamic_cast<M3GSprite3D*> ( m_pNode );
    if ( pSprite )
	{
        if ( m_vVertices [ 0 ] == 2 && m_vVertices [ 1 ] == 1 && m_vVertices [ 2 ] == 0 ) 
		{
            m_vTexCoord [ index ]->x = m_fU + m_fV;
            m_vTexCoord [ index ]->y = 1 - m_fU * kdSinf ( 45 );
        }
		else if ( m_vVertices [ 0 ] == 2 && m_vVertices [ 1 ] == 3 && m_vVertices [ 2 ] == 1 )
		{
            m_vTexCoord [ index ]->x = m_fV;
            m_vTexCoord [ index ]->y = 1 - ( m_fU + m_fV * kdSinf ( 45 ) );
        }
		else 
		{
			M3GException ( "InternalException", __FUNCTION__, "Unknown winding." );
        }

        return;
    }

	M3GMesh*  pMesh = dynamic_cast<M3GMesh*> ( m_pNode );
    if ( pMesh )
	{
        KDfloat  fScaleBias [ 4 ];
        M3GVertexArray*  pTexCoords = pMesh->getVertexBuffer ( )->getTexCoords ( index, fScaleBias );
        
		if ( pTexCoords ) 
		{
            KDfloat  fScale = fScaleBias [ 0 ];
            KDfloat  fBias [3] = { fScaleBias [ 0 ], fScaleBias [ 1 ], fScaleBias [ 2 ] };
            KDfloat  fTexCoordValues [ 3 ][ 3 ] = { { 0, 0, 0}, { 0, 0, 0}, { 0, 0, 0 } };

            pTexCoords->get ( m_vVertices [0], 1, fScale, fBias, fTexCoordValues [0] );
            pTexCoords->get ( m_vVertices [1], 1, fScale, fBias, fTexCoordValues [1] );
            pTexCoords->get ( m_vVertices [2], 1, fScale, fBias, fTexCoordValues [2] );

            M3GVector  tTexCoord0 = M3GVector ( fTexCoordValues [0] );
            M3GVector  tTexCoord1 = M3GVector ( fTexCoordValues [1] );
            M3GVector  tTexCoord2 = M3GVector ( fTexCoordValues [2] );

			*m_vTexCoord [ index ] = m3gLerp ( m_fU, m_fV, tTexCoord0, tTexCoord1, tTexCoord2 );
        }
    }
}