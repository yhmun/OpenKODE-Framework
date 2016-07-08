/* --------------------------------------------------------------------------
 *
 *      File            M3GGroup.cpp
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
#include <XMM3G/M3GGroup.h>
#include <XMM3G/M3GNode.h>
#include <XMM3G/M3GWorld.h>
#include <XMM3G/M3GSkinnedMesh.h>
#include <XMM3G/M3GRayIntersection.h>
#include <XMM3G/M3GTransform.h>
#include <XMM3G/M3GCamera.h>
#include <XMM3G/M3GSprite3D.h>

M3GGroup::M3GGroup ( KDvoid )
{

}

M3GGroup::~M3GGroup ( KDvoid )
{

}

KDint M3GGroup::animate ( KDint time )
{
	M3GNode::animate ( time );

    for ( KDuint  i = 0; i < m_vChildren.size(); i++ ) 
	{
        m_vChildren [ i ]->animate ( time );
    }

	return 0;
}

M3GObject3D* M3GGroup::duplicate ( KDvoid ) const
{
	M3GGroup*  pObj = new M3GGroup ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GGroup::_duplicate ( M3GGroup* pObj ) const
{
	M3GNode::_duplicate ( pObj );

    pObj->m_vChildren.resize ( m_vChildren.size ( ) );

    for ( KDuint i = 0; i < m_vChildren.size ( ); i++ )
	{
        pObj->m_vChildren [ i ] = (M3GNode*) m_vChildren [ i ]->duplicate ( );
        pObj->m_vChildren [ i ]->setParent ( pObj );
    }
}

KDint M3GGroup::getReferences ( M3GObject3D** references ) const
{
	KDint  nNumber = M3GNode::getReferences ( references );

    for ( KDuint  i = 0; i < m_vChildren.size(); i++ )
	{
		if ( references )
		{
			references [ nNumber ] = m_vChildren [ i ];
		}

		nNumber++;
    }

	return nNumber;
}

KDvoid M3GGroup::addChild ( M3GNode* child )
{
    if ( child == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Child is NULL." );
    }

    if ( child == this )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Can't add myself." );
    }

    if ( dynamic_cast<M3GWorld*> ( child ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Child is invalid, World" );
    }

    if ( child->getParent ( ) != KD_NULL && child->getParent ( ) != this )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Added child is already a member of other group." );
    }

    child->setParent ( this );

    m_vChildren.push_back ( child );
}

M3GNode* M3GGroup::getChild ( KDint index ) const
{
    if ( index < 0 || index >= (KDint) m_vChildren.size ( ) )
	{
        M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Children index is invalid, index = %d.", index );
    }

    return m_vChildren [ index ];
}

KDint M3GGroup::getChildCount ( KDvoid ) const
{
	return m_vChildren.size ( );
}

KDbool M3GGroup::pick ( KDint scope, KDfloat x, KDfloat y, M3GCamera* camera, M3GRayIntersection* ri ) const
{
    if ( camera == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Camera is NULL." );
    }

    if ( !isGlobalPickingEnabled ( ) )
	{
        return KD_FALSE;
    }

    if ( this->getGlobalParent ( ) != camera->getGlobalParent ( ) )
	{
		M3GException ( "IllegalStateException", __FUNCTION__, "Can't translate from Group coordinate to given Camera coordinate." );
    }

    // NDC
    M3GVector  tP0Ndc = M3GVector ( 2*x-1, 1-2*y, -1 );
    M3GVector  tP1Ndc = M3GVector ( 2*x-1, 1-2*y,  1 );

    // Camera
    M3GTransform  tProjInvert;
    camera->getProjection ( &tProjInvert );
    tProjInvert.invert ( );

    M3GVector  tP0Cam = tProjInvert.transform ( tP0Ndc );
    M3GVector  tP1Cam = tProjInvert.transform ( tP1Ndc );

	tP0Cam.dividedByW ( );
    tP1Cam.dividedByW ( );
 
    M3GRayIntersection  tMinRay;

    for ( KDuint  i = 0; i < m_vChildren.size ( ); i++ )
	{
        M3GRayIntersection  tRay;
        M3GTransform        tTrans;

        M3GGroup*  pGroup = dynamic_cast<M3GGroup*> ( m_vChildren[i] );
        if ( pGroup )
		{
            pGroup->pick ( scope, x, y, camera, &tRay );
            pGroup->getTransformTo ( this, &tTrans );

            tRay.transformRay ( tTrans );
        }
		
        M3GMesh*  pMesh = dynamic_cast<M3GMesh*> ( m_vChildren[i] );
        if ( pMesh && ( pMesh->getScope ( ) & scope ) )
		{
            camera->getTransformTo ( pMesh, &tTrans );

            M3GVector  tP0Mesh = tTrans.transform ( tP0Cam );
            M3GVector  tP1Mesh = tTrans.transform ( tP1Cam );
            M3GVector  tOrg = tP0Mesh;
            M3GVector  tDir = tP1Mesh - tP0Mesh;

            pMesh->intersect ( tOrg, tDir, &tRay );
            pMesh->getTransformTo ( this, &tTrans );

            tRay.transformRay ( tTrans );
            tRay.normalizeRay ( );
        }
		
        M3GSprite3D*  pSprite = dynamic_cast<M3GSprite3D*> ( m_vChildren [ i ] );
        if ( pSprite && pSprite->isScaled ( ) && ( pSprite->getScope ( ) & scope ) )
		{
            M3GVector  tOrg = tP0Ndc;
            M3GVector  tDir = tP1Ndc - tP0Ndc;

            pSprite->intersect ( tOrg, tDir, camera, &tRay );
            
            tRay.transformRay ( tProjInvert );
            camera->getTransformTo ( this, &tTrans );
            tRay.transformRay ( tTrans );
            tRay.normalizeRay ( );
        }

        if ( tRay.getIntersected ( ) )
		{
            if ( tMinRay.getIntersected ( ) == KD_NULL || tRay.getDistance ( ) < tMinRay.getDistance ( ) )
			{
                tMinRay = tRay;
            }
        }
    }

    if ( ri )
	{
        *ri = tMinRay;
	}

	return tMinRay.getIntersected ( ) ? KD_TRUE : KD_FALSE;
}

KDbool M3GGroup::pick ( KDint scope, KDfloat ox, KDfloat oy, KDfloat oz, KDfloat dx, KDfloat dy, KDfloat dz, M3GRayIntersection* ri ) const
{
    if ( dx == 0 && dy == 0 && dz == 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Given ray Direction is 0." );
    }

    M3GVector  tP0Grp ( ox, oy, oz );
    M3GVector  tP1Grp ( ox + dx, oy + dy, oz + dz );

    M3GRayIntersection  tMinRay;

    for ( KDuint i = 0; i < m_vChildren.size ( ); i++ )
	{
        M3GRayIntersection  tRay;
        M3GTransform        tTrans;
        M3GGroup*			pGroup2 = dynamic_cast<M3GGroup*> ( m_vChildren [ i ] );

        if ( pGroup2 )
		{            
            this->getTransformTo ( pGroup2, &tTrans );

            M3GVector  tP0Grp2 = tTrans.transform ( tP0Grp );
            M3GVector  tP1Grp2 = tTrans.transform ( tP1Grp );
            M3GVector  tOrg    = tP0Grp2;
            M3GVector  tDir    = tP1Grp2 - tP0Grp2;

            pGroup2->pick ( scope, 
                            tOrg.x, tOrg.y, tOrg.z,
                            tDir.x, tDir.y, tDir.z,
                            &tRay );
            
            tTrans.invert ( );
            tRay.transformRay ( tTrans );
        }

        M3GMesh*  pMesh = dynamic_cast<M3GMesh*> ( m_vChildren [ i ] );
        if ( pMesh && ( pMesh->getScope ( ) & scope ) )
		{
            this->getTransformTo ( pMesh, &tTrans );

            M3GVector  tP0Mesh = tTrans.transform ( tP0Grp );
            M3GVector  tP1Mesh = tTrans.transform ( tP1Grp );
            M3GVector  tOrg    = tP0Mesh;
            M3GVector  tDir    = tP1Mesh - tP0Mesh;

            pMesh->intersect ( tOrg, tDir, &tRay );
            pMesh->getTransformTo ( this, &tTrans );

            tRay.transformRay ( tTrans );
        }

        if ( tRay.getIntersected ( ) )
		{
            if ( tMinRay.getIntersected ( ) == KD_NULL || tRay.getDistance ( ) < tMinRay.getDistance ( ) )
			{
                tMinRay = tRay;
            }
        }
    }

    if ( ri )
	{
        *ri = tMinRay;
	}

	return tMinRay.getIntersected ( ) ? KD_TRUE : KD_FALSE;
}

KDvoid M3GGroup::removeChild ( M3GNode* child )
{
    M3GNode*  pNode = this;

    while ( ( pNode = pNode->getParent ( ) ) != KD_NULL )
	{
        if ( dynamic_cast<M3GSkinnedMesh*> ( pNode ) ) 
		{
			M3GException ( "IllegalArgumentException", __FUNCTION__, "Skeleton node is not removed." );
        }
    }

    std::vector<M3GNode*>::iterator  iter;
    for ( iter = m_vChildren.begin ( ); iter < m_vChildren.end ( ); iter++ )
	{
        if ( *iter == child )
		{
            m_vChildren.erase ( iter );
            child->setParent ( KD_NULL );
            break;
        }
    }
}

KDvoid M3GGroup::render ( M3GRenderState& state ) const
{
    if ( !isGlobalRenderingEnabled ( ) ) 
	{
        return;
    }

    glPushMatrix ( );

    M3GNode::render ( state );

    for ( KDuint  i = 0; i < m_vChildren.size ( ); i++ )
	{
        glPushMatrix ();
        
		m_vChildren [ i ]->render ( state );
        
		glPopMatrix ();
    }

    glPopMatrix ( );
}

KDbool M3GGroup::isDescendant ( const M3GNode* node ) const
{
    if ( node == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Node is null." );
    }

    if ( node == this ) 
	{
        return KD_TRUE;
    }

    for ( KDuint  i = 0; i < m_vChildren.size ( ); i++ ) 
	{
        if ( m_vChildren [ i ] == node )
		{
            return KD_TRUE;
        }

        M3GGroup*  pGroup = dynamic_cast<M3GGroup*> ( m_vChildren [ i ] );
        if ( pGroup && pGroup->isDescendant ( node ) ) 
		{
            return KD_TRUE;
        }
    }

    return KD_FALSE;
}