/* --------------------------------------------------------------------------
 *
 *      File            M3GMorphingMesh.cpp
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
#include <XMM3G/M3GMorphingMesh.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>
#include <XMM3G/M3GVertexArray.h>
#include <XMM3G/M3GVertexBuffer.h>
#include <XMM3G/M3GIndexBuffer.h>
#include <XMM3G/M3GAppearance.h>

M3GMorphingMesh::M3GMorphingMesh ( KDvoid )
{

}

M3GMorphingMesh::M3GMorphingMesh ( M3GVertexBuffer* base, KDint numTargets, M3GVertexBuffer** targets, KDint numSubmesh, M3GIndexBuffer** submeshes, KDint numAppearance, M3GAppearance**	appearances )
: M3GMesh ( base, numSubmesh, submeshes, numAppearance, appearances )
{
    if ( numTargets < 0 ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Number of target is invalid, numTargets = %d.", numTargets );
    }

    if ( targets == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Morph targets are NULL." );
    }

    for ( KDint  i = 0; i < numTargets; i++ )
	{
        if ( targets [ i ] == KD_NULL )
		{
			M3GException ( "NullPointerException", __FUNCTION__, "Target vertices is NULL, i = %d.", i );
        }
    }

    if ( base->getPositions ( 0 ) == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Base vertices has no positions" );
    }

	this->init ( numTargets, targets );
}

M3GMorphingMesh::M3GMorphingMesh ( M3GVertexBuffer* base, KDint numTargets, M3GVertexBuffer** targets, M3GIndexBuffer* submesh, M3GAppearance* appearance )
: M3GMesh ( base, submesh, appearance )
{
    if ( numTargets < 0 ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Number of target is invalid, numTargets = %d.", numTargets );
    }

    if ( targets == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Morph targets are NULL." );
    }

    for ( KDint  i = 0; i < numTargets; i++ )
	{
        if ( targets [ i ] == KD_NULL )
		{
			M3GException ( "NullPointerException", __FUNCTION__, "Target vertices is NULL, i = %d.", i );
        }
    }

    if ( base->getPositions ( 0 ) == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Base vertices has no positions" );
    }

	this->init ( numTargets, targets );
}

M3GMorphingMesh::~M3GMorphingMesh ( KDvoid )
{

}

KDvoid M3GMorphingMesh::init ( KDint numTargets, M3GVertexBuffer** targets )
{
    m_pMorphedVertices = (M3GVertexBuffer*) m_pVertices->duplicate ( );

    KDfloat          fScaleBias [ 4 ];
    M3GVertexArray*  base_positions = m_pVertices->getPositions ( fScaleBias );
    if ( base_positions ) 
	{
        M3GVertexArray*  pMorphedPositions = (M3GVertexArray*) base_positions->duplicate ( );
        m_pMorphedVertices->setPositions ( pMorphedPositions, fScaleBias [0], &fScaleBias [1] );
    }
   
    M3GVertexArray*  base_normals = m_pVertices->getNormals ();
    if ( base_normals )
	{
        M3GVertexArray*  pMorphedNormals = (M3GVertexArray*) base_normals->duplicate ( );
        m_pMorphedVertices->setNormals ( pMorphedNormals );
    }

    M3GVertexArray*  base_colors = m_pVertices->getColors ();
    if ( base_colors ) 
	{
        M3GVertexArray*  pMorphedColors = (M3GVertexArray*) base_colors->duplicate ( );
        m_pMorphedVertices->setColors ( pMorphedColors );
    }

    m_vMorphTargets.assign ( targets, targets + numTargets );
    m_vMorphWeights.assign ( numTargets, 0);
}

KDvoid M3GMorphingMesh::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::MORPH_WEIGHTS )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GMesh::addAnimationTrack ( animationTrack );
	}
}

KDint M3GMorphingMesh::animate ( KDint time )
{
	M3GMesh::animate ( time );

    KDbool    isWeightsModefied = KD_FALSE;
    KDfloat*  fWeights = new KDfloat [ m_vMorphWeights.size ( ) ];
	kdMemset ( fWeights, 0, sizeof ( KDfloat ) * m_vMorphWeights.size ( ) );

    for ( KDint  i = 0; i < getAnimationTrackCount ( ); i++ ) 
	{
		M3GAnimationTrack*      pTrack      = getAnimationTrack ( i );
		M3GKeyframeSequence*    pKeyframe   = pTrack->getKeyframeSequence ( );
		M3GAnimationController* pController = pTrack->getController ( );

		if ( !pController || !pController->isActive ( time ) )
		{
			continue;
		}

		KDfloat  fWeight   = pController->getWeight ( );
		KDfloat  fSequence = pController->getPosition ( time );

		switch ( pTrack->getTargetProperty ( ) )
		{
			case M3GAnimationTrack::MORPH_WEIGHTS :
			{
				KDint     nNumber = KD_MAX ( (KDint) m_vMorphWeights.size ( ), pKeyframe->getComponentCount ( ) );
				KDfloat*  fValue = new KDfloat [ nNumber ];
				kdMemset ( fValue, 0, sizeof ( KDfloat ) * nNumber );
				
				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				for ( KDuint  i = 0; i < m_vMorphWeights.size ( ); i++ )
				{
					fWeights [i] += fValue [i] * fWeight;
				}
				
				isWeightsModefied = KD_TRUE;
				
				delete [] fValue;

				break;
			}

			default :
			{
				// Unknown target should be ignored.
				// animate() of Base class handle it.
			}
		}
    }

    if ( isWeightsModefied ) 
	{
        for ( KDuint  i = 0; i < m_vMorphWeights.size ( ); i++ )
		{
            m_vMorphWeights [ i ] = fWeights [ i ];
        }

        updateMorphedVertices ( );
    }

    delete [] fWeights;

	return 0;
}

M3GObject3D* M3GMorphingMesh::duplicate ( KDvoid ) const
{
	M3GMorphingMesh*  pObj = new M3GMorphingMesh 
	(
		m_pVertices,
		m_vMorphTargets.size ( ),
		(M3GVertexBuffer**) &m_vMorphTargets [0],
		m_vIndices.size ( ),
		(M3GIndexBuffer**) &m_vIndices [0],
		m_vAppearances.size ( ),
		(M3GAppearance**) &m_vAppearances [0]
	);

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GMorphingMesh::_duplicate ( M3GMorphingMesh* pObj ) const
{
	M3GMesh::_duplicate ( pObj );

	pObj->m_vMorphWeights = m_vMorphWeights;
}

KDint M3GMorphingMesh::getReferences ( M3GObject3D** references ) const
{
	KDint  nNumber = M3GMesh::getReferences ( references );

	for ( KDuint  i = 0; i < m_vMorphTargets.size ( ); i++ )
	{
		if ( m_vMorphTargets [ i ] )
		{
			if ( references )
			{
				references [ nNumber ] = m_vMorphTargets [ i ];
			}

			nNumber++;
		}
	}

	return nNumber;
}

M3GVertexBuffer* M3GMorphingMesh::getMorphTarget ( KDint index ) const
{
    if ( index < 0 || index >= (KDint) m_vMorphTargets.size ( ) )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Target index is invalid. index=%d, max=%d.", index, m_vMorphTargets.size ( ) );
    }

    return m_vMorphTargets [ index ];
}

KDint M3GMorphingMesh::getMorphTargetCount ( KDvoid ) const
{
	return m_vMorphTargets.size ( );
}

KDvoid M3GMorphingMesh::getWeights ( KDfloat* weights ) const
{
    if ( weights == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Weights is NULL." );
    }

    for ( KDuint  i = 0; i < m_vMorphWeights.size ( ); i++ )
	{
        *weights++ = m_vMorphWeights [ i ];
    }
}

KDvoid M3GMorphingMesh::setWeights ( KDuint numWeights, KDfloat* weights )
{
    if ( weights == KD_NULL ) 
	{
        M3GException ( "NullPointerException", __FUNCTION__, "Weights is NULL." );
    }

    for ( KDuint  i = 0; i < m_vMorphWeights.size ( ); i++ )
	{
        if ( i < numWeights ) 
		{
            m_vMorphWeights [i] = *weights++;
        } 
		else 
		{
            m_vMorphWeights [i] = 0;
        }
    }

    updateMorphedVertices ( );
}

KDvoid M3GMorphingMesh::render ( M3GRenderState& state ) const
{
    if ( !isGlobalRenderingEnabled ( ) )
	{
        return;
    }

    M3GVertexBuffer*  pTemp = m_pVertices;
    ( const_cast<M3GMorphingMesh*> (this) )->m_pVertices = m_pMorphedVertices;

    M3GMesh::render ( state );

    ( const_cast<M3GMorphingMesh*> (this) )->m_pVertices = pTemp;
}

KDvoid M3GMorphingMesh::updateMorphedVertices ( KDvoid )
{
    KDfloat                fScaleBias [ 4 ];
    const M3GVertexArray*  pBasePositions = m_pVertices->getPositions ( fScaleBias );
    const M3GVertexArray*  pBaseNormals   = m_pVertices->getNormals ( );
    const M3GVertexArray*  pBaseColors    = m_pVertices->getColors ( );

	std::vector<const M3GVertexArray*>  vMorphTargetPositions;
    std::vector<const M3GVertexArray*>  vMorphTargetNormals;
    std::vector<const M3GVertexArray*>  vMorphTargetColors;
    
	for ( KDuint  i = 0; i < m_vMorphTargets.size ( ); i++ ) 
	{
        vMorphTargetPositions.push_back ( m_vMorphTargets [i]->getPositions ( 0 ) );
        vMorphTargetNormals.push_back ( m_vMorphTargets [i]->getNormals ( ) );
        vMorphTargetColors.push_back ( m_vMorphTargets [i]->getColors ( ) );
    }

    M3GVertexArray*  pMorphedPositions = m_pMorphedVertices->getPositions ( 0 );
    M3GVertexArray*  pMorphedNormals   = m_pMorphedVertices->getNormals ( );
    M3GVertexArray*  pMorphedColors    = m_pMorphedVertices->getColors ( );

    if ( pBasePositions ) 
	{
        pMorphedPositions->setMorphing ( pBasePositions, vMorphTargetPositions, m_vMorphWeights );
    }

    if ( pBaseNormals ) 
	{
        pMorphedNormals->setMorphing ( pBaseNormals, vMorphTargetNormals, m_vMorphWeights );
    }

    if ( pBaseColors ) 
	{
        pMorphedColors->setMorphing ( pBaseColors, vMorphTargetColors, m_vMorphWeights );
    }

    if ( 1 )
	{
        KDint  nBaseColor    = m_pVertices->getDefaultColor ( );
        KDint  nMorphedColor = m_pVertices->getDefaultColor ( );
        
		for ( KDuint  t = 0; t < m_vMorphTargets.size ( ); t++ )
		{
            KDint  nTargetColor = m_vMorphTargets [ t ]->getDefaultColor ( );
            nMorphedColor += (KDint) ( m_vMorphWeights [ t ] * ( nTargetColor - nBaseColor ) );
        }

        nMorphedColor = KD_CLAMP ( 0, 1, nMorphedColor );
        m_pMorphedVertices->setDefaultColor ( nMorphedColor );
    }
}