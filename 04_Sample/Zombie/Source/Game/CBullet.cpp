/* --------------------------------------------------------------------------
 *
 *      File            CBullet.cpp
 *      Description     Game Bullet
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

#include "CBullet.h"
#include "CEnemy.h"

CBullet::CBullet ( KDvoid )
{
    
}

CBullet::~CBullet ( KDvoid )
{
    
}

KDvoid CBullet::onUpdate ( KDuint uTime )
{
    scene::ISceneManager*               pScene    = g_pEngMgr->getScene  ( );
    video::IVideoDriver*                pVideo    = g_pEngMgr->getVideo  ( );    
    scene::IParticleSystemSceneNode*    pParticle = 0;
    scene::IParticleEmitter*            pEmitter  = 0;
    scene::IParticleAffector*           pAffector = 0;
    scene::ISceneNodeAnimator*          pAnimate  = 0;
    video::ITexture*                    pTexture  = 0;
    core::vector3df                     tNormal;
    
    for ( KDuint  i = 0; i < m_aImpacts.size ( ); ++i )
	{
		if ( uTime > m_aImpacts [ i ].uTime )
		{
            tNormal = m_aImpacts [ i ].tTriangle.getNormal ( );
            tNormal.setLength ( 0.03f );    
            
            pTexture = pVideo->getTexture 
            (
                g_pResMgr->getPath ( m_aImpacts [ i ].pTarget->getID ( ) == -1 ? eTEX_Smoke : eTEX_Harm )
            );
            
            pParticle = pScene->addParticleSystemSceneNode ( false, 0, -1, m_aImpacts [ i ].tPoint );            
            pParticle->setParticleSize ( core::dimension2d<f32> ( 25.0f, 25.0f ) );
            
            pEmitter = pParticle->createBoxEmitter 
            ( 
                core::aabbox3d<f32> ( -10, -10, -10, 10, 10, 10 ),
                tNormal, 1, 3, 
                video::SColor ( 0, 255, 255, 255 ),
                video::SColor ( 0, 255, 255, 255 ),
                250, 500, 20, core::dimension2df(5.0f,5.0f), core::dimension2df(20.0f,20.0f) 
            );
            
            pParticle->setEmitter ( pEmitter );
            pEmitter->drop ( );
            
            pAffector = pParticle->createFadeOutParticleAffector ( );
            pParticle->addAffector ( pAffector );
            pAffector->drop ( );
            
            pParticle->setMaterialTexture ( 0, pTexture );
            
            pParticle->setMaterialFlag ( video::EMF_LIGHTING, false );
            pParticle->setMaterialFlag ( video::EMF_ZWRITE_ENABLE, false );
            pParticle->setMaterialType ( video::EMT_TRANSPARENT_VERTEX_ALPHA );
            
            pAnimate = pScene->createDeleteAnimator ( 500 );
            pParticle->addAnimator ( pAnimate );
            pAnimate->drop ( );                    
            
            if ( m_aImpacts [ i ].pTarget->getUserPtr ( ) )
            {
                CEnemy*   pEnemy = (CEnemy*) m_aImpacts [ i ].pTarget->getUserPtr ( );
                
                pEnemy->setDamage ( 10 );
            }
            
			m_aImpacts.erase ( i );
			i--;
		}
        else
        {
            break;
        }
	}   
}

KDuint CBullet::fire ( scene::ISceneNode* pTarget, const core::line3df& tLine, const core::triangle3df& tTriangle )
{
    scene::ISceneManager*       pScene   = g_pEngMgr->getScene  ( );
    video::IVideoDriver*        pVideo   = g_pEngMgr->getVideo  ( );
    scene::ICameraSceneNode*    pCamera  = g_pEngMgr->getCamera ( );    
    core::vector3df             tStart   = pCamera->getPosition ( );
    scene::ISceneNode*          pNode    = 0;
    scene::ISceneNodeAnimator*  pAnimate;
    
    KDfloat  fLength = tLine.getLength ( );
    KDfloat  fSpeed  = 0.3f;
    KDuint   uTime   = (u32) ( fLength / fSpeed );
    
    //pNode = pScene->addMeshSceneNode ( pMesh );
    pNode = pScene->addBillboardSceneNode ( 0, core::dimension2d<f32> ( 16, 16 ), tStart );    
    pNode->setMaterialTexture
    (
        0, pVideo->getTexture ( g_pResMgr->getPath ( eTEX_Bullet1 ) ) 
    );
        
    pNode->setMaterialFlag ( video::EMF_LIGHTING, false );        
    pNode->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
        
    pAnimate = pScene->createFlyStraightAnimator ( tLine.start, tLine.end, uTime );
    pNode->addAnimator ( pAnimate );
    pAnimate->drop ( );
        
    pAnimate = pScene->createDeleteAnimator ( uTime );
    pNode->addAnimator ( pAnimate );
    pAnimate->drop ( );            
    
    if ( pTarget )
    {
        SImpact  tImpact = { g_pEngMgr->getTime ( ) + uTime, pTarget, tLine.end, tTriangle };
        m_aImpacts.push_back ( tImpact );
    }
    
    return uTime;
}
