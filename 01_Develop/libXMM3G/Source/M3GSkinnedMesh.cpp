/* --------------------------------------------------------------------------
 *
 *      File            M3GSkinnedMesh.cpp
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
#include <XMM3G/M3GSkinnedMesh.h>
#include <XMM3G/M3GVertexBuffer.h>
#include <XMM3G/M3GIndexBuffer.h>
#include <XMM3G/M3GVertexArray.h>
#include <XMM3G/M3GAppearance.h>
#include <XMM3G/M3GGroup.h>

M3GSkinnedMesh::M3GSkinnedMesh ( KDvoid )
{

}

M3GSkinnedMesh::M3GSkinnedMesh ( M3GVertexBuffer* vertices, KDint numSubmesh, M3GIndexBuffer** submeshes, KDint numAppearance, M3GAppearance** appearances, M3GGroup* skeleton )
: M3GMesh ( vertices, numSubmesh, submeshes, numAppearance, appearances )
{
    if ( vertices->getPositions ( 0 ) == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Vertices has no positions." );
    }

    if ( skeleton == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Skeleton is NULL." );
    }

	this->init ( skeleton );
}

M3GSkinnedMesh::M3GSkinnedMesh ( M3GVertexBuffer* vertices, M3GIndexBuffer* submesh, M3GAppearance* appearance, M3GGroup* skeleton )
: M3GMesh ( vertices, submesh, appearance )
{
    if ( vertices->getPositions ( 0 ) == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Vertices has no positions." );
    }

    if ( skeleton == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Skeleton is NULL." );
    }

	this->init ( skeleton );
}

M3GSkinnedMesh::~M3GSkinnedMesh ( KDvoid )
{
    if ( m_pSkinnedVertices )
	{
        delete m_pSkinnedVertices;
    }
}

KDvoid M3GSkinnedMesh::init ( M3GGroup* skeleton )
{
    m_pSkeleton = skeleton;
    m_pSkeleton->setParent ( this );

    m_pSkinnedVertices = (M3GVertexBuffer*) m_pVertices->duplicate ( );

    KDfloat  fScaleBias [ 4 ];
    M3GVertexArray*  pBindPositions = m_pVertices->getPositions ( fScaleBias );
    if ( pBindPositions )
	{
        M3GVertexArray*  pSkinnedPositions = (M3GVertexArray*) pBindPositions->duplicate ( );
        pSkinnedPositions->convert ( 4 );
        m_pSkinnedVertices->setPositions ( pSkinnedPositions, fScaleBias[0], &fScaleBias[1] );
    }

    M3GVertexArray*  pBindNormals = m_pVertices->getNormals ( );
    if ( pBindNormals )
	{
        M3GVertexArray*  pSkinnedNormals = (M3GVertexArray*) pBindNormals->duplicate ( );
        m_pSkinnedVertices->setNormals ( pSkinnedNormals );
    }

    KDint  nVertexCount = pBindPositions->getVertexCount ( );
    m_vBoneWeights.reserve ( nVertexCount );
    
	for ( KDint  v = 0; v < nVertexCount; v++ ) 
	{
        m_vBoneWeights.push_back ( std::vector<M3GBoneWeight> ( ) );
    }

    m_vBindPoses.clear ( );
}

KDint M3GSkinnedMesh::animate ( KDint time )
{
    M3GMesh:: animate ( time );

    if ( m_pSkeleton )
	{
        m_pSkeleton->animate ( time );
    }

    return 0;
}

M3GObject3D* M3GSkinnedMesh::duplicate ( KDvoid ) const
{
	M3GSkinnedMesh*  pObj = new M3GSkinnedMesh ( m_pVertices, m_vIndices.size ( ), (M3GIndexBuffer**) &m_vIndices [ 0 ], m_vAppearances.size ( ), (M3GAppearance**) &m_vAppearances [ 0 ], m_pSkeleton );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GSkinnedMesh::_duplicate ( M3GSkinnedMesh* pObj ) const
{
	M3GMesh::_duplicate ( pObj );

    pObj->m_vBoneWeights = m_vBoneWeights;
    pObj->m_vBindPoses   = m_vBindPoses;

    for ( KDuint  i = 0; i < m_vBindPoses.size ( ); i++ ) 
	{
		pObj->m_vBindPoses [ i ].m_pBone = (M3GNode *) m_vBindPoses [ i ].m_pBone->duplicate ( );
    }
}

KDint M3GSkinnedMesh::getReferences ( M3GObject3D** references ) const
{
	KDint   nNumber = M3GMesh::getReferences ( references );

    if ( m_pSkeleton )
	{
		if ( references )
		{
			references [ nNumber ] = m_pSkeleton;
		}

		nNumber++;
	}

	return nNumber;
}

KDvoid M3GSkinnedMesh::addTransform ( M3GNode* bone, KDint weight, KDint firstVertex, KDint numVertices )
{
    if ( bone == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Bone node is NULL." );
    }

    if ( dynamic_cast<M3GGroup*> ( bone ) == KD_NULL )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Bone node must be Group." );
    }

    if ( !m_pSkeleton || !m_pSkeleton->isDescendant ( bone ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Node is not descendant of this skeleton. node = %p.", bone );
    }

    if ( weight <= 0 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Bone weight must be positive integer, weight = %f.", weight );
    }

    if ( firstVertex < 0 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "First vertex is invalid, firstVertex = %d.", firstVertex );
    }

    if ( numVertices <= 0 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Number of vertices is invalid, numVertices = %d.", numVertices );
    }

    if ( firstVertex + numVertices > 65535 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "First vertex + number of vertices is invalid, firstVertex = %d, numVertices = %d.", firstVertex, numVertices );
    }

    KDint  index = addBoneIndex ( bone );
    for ( KDint  v = firstVertex; v < firstVertex + numVertices; v++ )
	{
        m_vBoneWeights [ v ].push_back ( M3GBoneWeight ( index, weight ) );
    }
}

KDvoid M3GSkinnedMesh::getBoneTransform ( M3GNode* bone, M3GTransform* transform ) const
{
    if ( bone == KD_NULL )
	{
        M3GException ( "NullPointerException", __FUNCTION__, "Bone node is NULL." );
    }

    if ( transform == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Transform is NULL." );
    }

    if ( getBoneIndex ( bone ) == -1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Node is not bone of this SkinnedmEsh, node = %p.", bone );
    }
  
    M3GMatrix  tGlobalPose = bone->getGlobalPose ( this );
    tGlobalPose.invert ( );

    transform->set ( tGlobalPose.m );
}

KDint M3GSkinnedMesh::getBoneVertices ( M3GNode* bone, KDint* indices, KDfloat* weights ) const
{
    if ( bone == KD_NULL )
	{
        M3GException ( "NullPointerException", __FUNCTION__, "Bone node is NULL." );
    }

    if ( m_pVertices->getPositions ( 0 ) == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Positions are not set." );
    }

    if ( getBoneIndex ( bone ) == -1 )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Node is not bone of this SkinnedMesh., node = %p.", bone );
    }

    KDint  nBoneIndex   = getBoneIndex ( bone );
    KDint  nVertexCount = m_vBoneWeights.size ( );
    KDint  nFind        = 0;

    for ( KDint  v = 0; v < nVertexCount; v++ ) 
	{
        KDfloat  fWeight = 0;
        KDint    nBoneCount = m_vBoneWeights [ v ].size ( );

        for ( KDint  b = 0; b < nBoneCount; b++ ) 
		{
            fWeight += m_vBoneWeights [ v ][ b ].m_nWeight;
        }

        for ( KDint  b = 0; b < nBoneCount; b++ )
		{
            if ( m_vBoneWeights [ v ][ b ].m_nIndex == nBoneIndex )
			{
                nFind++;
                if ( indices )
				{
                    *indices++ = v;
				}

                if ( weights )
				{
                    *weights++ = m_vBoneWeights [ v ][ b ].m_nWeight / fWeight;
				}
            }
        }
    }

    return nFind;
}

M3GGroup* M3GSkinnedMesh::getSkeleton ( KDvoid ) const
{
	return m_pSkeleton;
}

KDvoid M3GSkinnedMesh::render ( M3GRenderState& state ) const
{
    if ( !isGlobalRenderingEnabled ( ) )
	{
        return;
    }

    ( const_cast<M3GSkinnedMesh*> ( this ) )->updateSkinnedVertices ( );

    M3GVertexBuffer*  pTemp = m_pVertices;
    ( const_cast<M3GSkinnedMesh*> ( this ) )->m_pVertices = m_pSkinnedVertices;

    glPushMatrix ( );
    M3GMesh::render ( state );
    glPopMatrix ( );

    M3GTransformable::render ( state );
    m_pSkeleton->render ( state );

    ( const_cast<M3GSkinnedMesh*> ( this ) )->m_pVertices = pTemp;
}

KDint M3GSkinnedMesh::addBoneIndex ( M3GNode* bone )
{
    for ( KDuint  i = 0; i < m_vBindPoses.size ( ); i++ ) 
	{
        if ( m_vBindPoses [ i ].m_pBone == bone ) 
		{
            M3GMatrix  tBindPose = bone->getGlobalPose ( this );
            m_vBindPoses [ i ].m_tInverse = tBindPose.invert ( );
            return i;
        }
    }

    M3GMatrix  tBindPose = bone->getGlobalPose ( this );
    m_vBindPoses.push_back ( M3GBindPose ( bone, tBindPose.invert ( ) ) );
    
	return m_vBindPoses.size ( ) - 1;
}

KDint M3GSkinnedMesh::getBoneIndex ( M3GNode* bone ) const
{
    for ( KDuint  i = 0; i < m_vBindPoses.size ( ); i++ ) 
	{
        if ( m_vBindPoses [ i ].m_pBone == bone )
		{
            return i;
		}
    }

    return -1;
}

KDvoid M3GSkinnedMesh::updateSkinnedVertices ( KDvoid )
{
    KDfloat          fScaleBias [ 4 ];
    M3GVertexArray*  pBindPositions    = m_pVertices->getPositions ( fScaleBias );
    M3GVertexArray*  pBindNormals      = m_pVertices->getNormals ( );
    M3GVertexArray*  pSkinnedPositions = m_pSkinnedVertices->getPositions ( 0 );
    M3GVertexArray*  pSkinnedNormals   = m_pSkinnedVertices->getNormals ( );

    KDint  nBoneCount = m_vBindPoses.size ( );
    std::vector<M3GMatrix>  vMatrixPalette ( nBoneCount );
    for ( KDint  b = 0; b < nBoneCount; b++ ) 
	{
		M3GMatrix  tGlobalPose = m_vBindPoses [ b ].m_pBone->getGlobalPose (this);
		vMatrixPalette [ b ] = tGlobalPose * m_vBindPoses [ b ].m_tInverse;
    }

    std::vector<M3GMatrix>  vPositionsMatrixPalette ( nBoneCount );
    M3GMatrix  tMatrix;
    tMatrix.setTranslate ( fScaleBias [ 1 ], fScaleBias [ 2 ], fScaleBias [ 3 ] );
    tMatrix.setScale ( fScaleBias [ 0 ], fScaleBias [ 0 ], fScaleBias [ 0 ] );

    for ( KDint  b = 0; b < nBoneCount; b++ )
	{
        vPositionsMatrixPalette [ b ] = tMatrix.getInverse ( ) * vMatrixPalette [ b ] * tMatrix;
    }

    std::vector<M3GMatrix>  vNormalsMatrixPalette ( nBoneCount );
    if ( pBindNormals ) 
	{
        for ( KDint  b = 0; b < nBoneCount; b++ )
		{
            vNormalsMatrixPalette [b] = vMatrixPalette [ b ];
            vNormalsMatrixPalette [b].invert ( ).transpose ( );
            vNormalsMatrixPalette [b][3]  = vNormalsMatrixPalette [b][ 7] = vNormalsMatrixPalette [b][11] = 0;
            vNormalsMatrixPalette [b][12] = vNormalsMatrixPalette [b][13] = vNormalsMatrixPalette [b][14] = 0;
            vNormalsMatrixPalette [b][15] = 1;
        }
    }
  
    if ( pBindPositions )
	{
        pSkinnedPositions->setSkinning ( pBindPositions, m_vBoneWeights, vPositionsMatrixPalette );
    }

    if ( pBindNormals )
	{
        pSkinnedNormals->setSkinning ( pBindNormals, m_vBoneWeights, vNormalsMatrixPalette );
    }

}