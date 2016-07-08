/* --------------------------------------------------------------------------
 *
 *      File            M3GMesh.cpp
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
#include <XMM3G/M3GMesh.h>
#include <XMM3G/M3GVertexArray.h>
#include <XMM3G/M3GVertexBuffer.h>
#include <XMM3G/M3GIndexBuffer.h>
#include <XMM3G/M3GAppearance.h>
#include <XMM3G/M3GRayIntersection.h>

M3GMesh::M3GMesh ( KDvoid )
{

}

M3GMesh::M3GMesh ( M3GVertexBuffer* vertices, KDint numSubmesh, M3GIndexBuffer** submeshes, KDint numAppearances, M3GAppearance** appearances )
{
	this->init ( vertices, numSubmesh, submeshes, numAppearances, appearances );
}

M3GMesh::M3GMesh ( M3GVertexBuffer* vertices, M3GIndexBuffer* submesh, M3GAppearance* appearance )
{
	this->init ( vertices, submesh, appearance );
}

M3GMesh::~M3GMesh ( KDvoid )
{

}

KDvoid M3GMesh::init ( M3GVertexBuffer* vertices, KDint numSubmesh, M3GIndexBuffer** submeshes, KDint numAppearances, M3GAppearance** appearances )
{
    if ( vertices == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "VertexBuffer is NULL." );
    }

    if ( submeshes == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "IndexBuffer is NULL." );
    }

    for ( KDint  i = 0; i < numSubmesh; i++ ) 
	{
        if ( submeshes [ i ] == KD_NULL ) 
		{
			M3GException ( "NullPointerException", __FUNCTION__, "IndexBuffer [ %d ] is NULL.", i );
        }
    }

    if ( numSubmesh < 1 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Number of submesh is invalid, numSubmesh = %d.", numSubmesh );
    }

    if ( appearances != NULL && numAppearances < numSubmesh ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Number of appearances is invalid, apps = %d < ibufs = %d.", numAppearances, numSubmesh );    
    }

    m_pVertices = vertices;

    m_vIndices    .assign ( submeshes  , submeshes   + numSubmesh     );
    m_vAppearances.assign ( appearances, appearances + numAppearances );
}

KDvoid M3GMesh::init ( M3GVertexBuffer* vertices, M3GIndexBuffer* submesh, M3GAppearance* appearance )
{
    if ( vertices == NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "VertexBuffer is NULL." );
    }

    if ( submesh == NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "IndexBuffer is NULL." );
    }

    m_pVertices = vertices;

    m_vIndices    .assign ( &submesh   , &submesh    + 1 );
    m_vAppearances.assign ( &appearance, &appearance + 1 );
}

KDint M3GMesh::animate ( KDint time )
{
	M3GNode::animate ( time );

	KDuint  i;

    if ( m_pVertices )
	{
        m_pVertices->animate ( time );
    }

    for ( i = 0; i < m_vIndices.size ( ); i++ ) 
	{
        m_vIndices [ i ]->animate ( time );
    }
    
	for ( i = 0; i < m_vAppearances.size ( ); i++ ) 
	{
        m_vAppearances [ i ]->animate ( time );
    }

	return 0;
}

M3GObject3D* M3GMesh::duplicate ( KDvoid ) const
{
	M3GMesh*  pObj = new M3GMesh ( m_pVertices, m_vIndices.size ( ), (M3GIndexBuffer**) &m_vIndices [ 0 ], m_vAppearances.size ( ), (M3GAppearance**) &m_vAppearances [ 0 ] );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GMesh::_duplicate ( M3GMesh* pObj ) const
{
	M3GNode::_duplicate ( pObj );
}

KDint M3GMesh::getReferences ( M3GObject3D** references ) const
{
	KDint   nNumber = M3GNode::getReferences ( references );
	KDuint  i;

    if ( m_pVertices )
	{
		if ( references )
		{
			references [ nNumber ] = m_pVertices;
		}

		nNumber++;
	}

    for ( i = 0; i < m_vIndices.size ( ); i++ )
	{
        if ( m_vIndices [ i ] )
		{
			if ( references )
			{
				references [ nNumber ] = m_vIndices [ i ];
			}
 
			nNumber++;
		}
    }

    for ( i = 0; i < m_vAppearances.size ( ); i++ )
	{
        if ( m_vAppearances [ i ] )
		{
			if ( references )
			{
				references [ nNumber ] = m_vAppearances [ i ];
			}
 
			nNumber++;
		}
    }

	return nNumber; 
}

M3GAppearance* M3GMesh::getAppearance ( KDint index ) const
{
    if ( index < 0 || index >= (KDint) m_vAppearances.size ( ) ) 
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d, max = %d.", index, m_vAppearances.size ( ) );
    }

    return m_vAppearances [ index ];
}

M3GIndexBuffer* M3GMesh::getIndexBuffer ( KDint index ) const
{
    if ( index < 0 || index >= (KDint) m_vIndices.size ( ) )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d, max = %d.", index, m_vIndices.size ( ) );
    }

    return m_vIndices [ index ];
}

KDint M3GMesh::getSubmeshCount ( KDvoid ) const
{
	return m_vIndices.size ( );
}

M3GVertexBuffer* M3GMesh::getVertexBuffer ( KDvoid ) const
{
	return m_pVertices;
}

KDvoid M3GMesh::setAppearance ( KDint index, M3GAppearance* appearance )
{
	if ( index < 0 || index >= (KDint) m_vAppearances.size ( ) )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Index is invalid, index = %d, max = %d.", index, m_vAppearances.size ( ) );
	}

	m_vAppearances [ index ] = appearance;
}

KDvoid M3GMesh::render ( M3GRenderState& state ) const
{
    if ( !isGlobalRenderingEnabled ( ) ) 
	{
        return;
    }

	KDuint  i;

    if ( state.m_nPass == -1 )
	{
        for ( i = 0; i < m_vAppearances.size ( ); i++ ) 
		{
            if ( m_vAppearances [ i ] ) 
			{
				state.m_vLayers.push_back ( m_vAppearances [ i ]->getLayer2 ( ) );
            }
        }
    }

	if ( state.m_nPass != 2 ) 
	{
        return;
    }

    M3GNode::render ( state );

    m_pVertices->render ( state );

    for ( i = 0; i < m_vAppearances.size ( ); i++ ) 
	{
		if ( m_vAppearances [ i ] && m_vAppearances [ i ]->getLayer2 ( ) == state.m_nLayer )
		{
            m_vAppearances [ i ]->render ( state );
            m_vIndices [ i ]->render ( state );
        }
    }
}

KDbool M3GMesh::intersect ( const M3GVector& org, const M3GVector& dir, M3GRayIntersection* ri ) const
{

    if ( org.w != 1 || dir.w != 1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "W must be 1. org.w = %f, dir.w = %f.", org.w, dir.w );
    }

    if ( ri == KD_NULL ) 
	{
        M3GException ( "NullPointerException", __FUNCTION__, "RayIntersection is NULL." );
    }

    if ( !isGlobalPickingEnabled ( ) )
	{
        return KD_FALSE;
    }

    KDfloat   fScaleBias [ 4 ];
    KDfloat&  fScale = fScaleBias [ 0 ];
    KDfloat*  fBias  = &fScaleBias[ 1 ];

    M3GVertexArray*  pPositions = m_pVertices->getPositions ( fScaleBias );

    KDbool    bRayHit = KD_FALSE;
    KDfloat   fRayU   = 0;
    KDfloat   fRayV   = 0;
	KDfloat   fRayT   = KD_FLT_MAX; 
    KDint     nRayIndexValues [ 3 ];
    KDint     nRaySubmeshIndex = 0;

    for ( KDuint  i = 0; i < m_vIndices.size ( ); i++ )
	{
        KDint  nType = m_vIndices [ i ]->getPrimitiveType ( );
        if ( nType != M3GIndexBuffer::TRIANGLES ) 
		{
			M3GException ( "IllegalStateException", __FUNCTION__, "Primitive type is invalid, type = %d.", nType );
        }

        KDint   nNumVertices = m_vIndices [ i ]->getIndexCount ( );
        KDint*  pVertices    = new KDint [ nNumVertices ];

        m_vIndices [ i ]->getIndices ( pVertices );

        KDfloat  fPositionValues [3][3];

        for ( KDint  i = 0; i < nNumVertices; i += 3 )
		{
            pPositions->get ( pVertices [i+0], 1, fScale, fBias, &fPositionValues [0][0] );
            pPositions->get ( pVertices [i+1], 1, fScale, fBias, &fPositionValues [1][0] );
            pPositions->get ( pVertices [i+2], 1, fScale, fBias, &fPositionValues [2][0] );
			
            M3GVector  tV0 = M3GVector ( fPositionValues [0] );
            M3GVector  tV1 = M3GVector ( fPositionValues [1] );
            M3GVector  tV2 = M3GVector ( fPositionValues [2] );
            KDfloat  fU, fV, fT;
            KDbool  bHit = M3GTriangleIntersect ( org, dir, tV0, tV1, tV2, &fU, &fV, &fT );

            if ( bHit && fT < fRayT ) 
			{
                bRayHit = KD_TRUE;
                fRayU = fU;
                fRayV = fV;
                fRayT = fT;
                nRaySubmeshIndex = i;
                kdMemcpy ( nRayIndexValues, &pVertices[i], sizeof ( KDint ) * 3 );
            }
        }

        delete [] pVertices;
    }

    if ( bRayHit )
	{
        if ( ri )
		{
            *ri = M3GRayIntersection ( const_cast<M3GMesh*> ( this ),
                                       org, dir, fRayT,
									   fRayU, fRayV,
									   3, nRayIndexValues,
									   nRaySubmeshIndex );
        }

        return KD_TRUE;
    }

	return KD_FALSE;
}