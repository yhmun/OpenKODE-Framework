/* --------------------------------------------------------------------------
 *
 *      File            CTarget.cpp
 *      Description     Game Taget
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

#include "Precompiled.h"
#include "../Manager/EngManager.h"
#include "../Manager/EnvManager.h"
#include "../Manager/ResManager.h"
#include "../Manager/ScrManager.h"

#include "CTarget.h"

CTarget::CTarget ( KDvoid )
{
    m_pSelector  = 0;
    m_pNodes[0]  = 0;
    m_pNodes[1]  = 0;
    m_pTagets[0] = 0;
    m_pTagets[1] = 0;
}

CTarget::~CTarget ( KDvoid )
{
    
}

KDbool CTarget::init ( scene::IAnimatedMeshSceneNode* pWorld )
{
    scene::ISceneManager*           pScene    = g_pEngMgr->getScene ( );
    video::IVideoDriver*            pVideo    = g_pEngMgr->getVideo ( );
    scene::ICameraSceneNode*        pCamera   = g_pEngMgr->getCamera ( );
    scene::ITriangleSelector*       pSelector = 0;
    video::ITexture*                pTexture  = 0;
    scene::IBillboardSceneNode*     pNode     = 0;
    scene::ISceneNode*              pArm      = 0;
    
    pSelector = pScene->createTriangleSelector ( pWorld );
    pTexture  = pVideo->getTexture ( g_pResMgr->getPath ( eTEX_Pointing ) );
    
    for ( KDuint  i = 0; i < 2; i++ )
    {
        pNode = pScene->addBillboardSceneNode ( );
        
        pNode->setMaterialTexture ( 0, pTexture );
        pNode->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
        pNode->setMaterialFlag ( video::EMF_LIGHTING, false );
        pNode->setMaterialFlag ( video::EMF_ZBUFFER , false );
        pNode->setSize ( core::dimension2d<f32> ( 20.0f, 20.0f ) );      
        pNode->setVisible ( KD_FALSE );
                
        pArm = pScene->addEmptySceneNode ( pCamera );        
        pArm->setPosition ( core::vector3df ( i == 0 ? -10.f : 10.f, -10.f, 20.f ) );
        
        m_pArms  [ i ] = pArm;
        m_pNodes [ i ] = pNode;
    }  
    
    m_pSelector = pSelector;
    
    return KD_TRUE;
}

KDvoid CTarget::onUpdate ( KDuint uTime, const core::position2di& tPoint1, const core::position2di& tPoint2 )
{
//    video::IVideoDriver*            pVideo     = g_pEngMgr->getVideo ( );
    scene::ISceneCollisionManager*  pCollision = g_pEngMgr->getCollision ( );
//    const core::dimension2d<u32>    tWinSize   = pVideo->getScreenSize ( );
    core::position2di               tTarget;
    core::line3df                   tRay;
    
    for ( KDuint  i = 0; i < 2; i++ )
    {     
        if ( m_pTagets [ i ] && m_pTagets [ i ]->getID ( ) != -1 )
        {
            m_pTagets [ i ]->setMaterialFlag ( video::EMF_LIGHTING, true );
            m_pTagets [ i ] = 0;
        }
        
        tTarget = i == 0 ? tPoint1 : tPoint2;       
        tRay = pCollision->getRayFromScreenCoordinates ( tTarget );
        
        if ( ( m_pTagets [ i ] = pCollision->getSceneNodeAndCollisionPointFromRay ( tRay, m_tRays [ i ].end, m_tTris [ i ], 1, 0 ) ) )
        {
            m_pTagets [ i ]->setMaterialFlag ( video::EMF_LIGHTING, false );
        }
        else if ( pCollision->getCollisionPoint ( tRay, m_pSelector, m_tRays [ i ].end, m_tTris [ i ], m_pTagets [ i ] ) )
        {

        }
        else
        {
            m_tRays [ i ].end = tRay.start + ( tRay.end - tRay.start ).normalize ( ) * 1500.f;
        }

        m_tRays  [ i ].start = m_pArms [ i ]->getAbsolutePosition ( );
        m_pNodes [ i ]->setPosition ( m_tRays [ i ].end ); 
        m_pArms  [ i ]->updateAbsolutePosition ( );                       
    }
}

scene::ISceneNode* CTarget::getTarget ( KDuint uIndex )
{
    return m_pTagets [ uIndex ];
}

const core::line3df& CTarget::getRayLine ( KDuint uIndex )
{    
    return m_tRays [ uIndex ];
}

const core::triangle3df& CTarget::getTriangle ( KDuint uIndex )
{
    return m_tTris [ uIndex ];
}
