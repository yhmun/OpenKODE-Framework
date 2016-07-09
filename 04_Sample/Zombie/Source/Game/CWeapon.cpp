/* --------------------------------------------------------------------------
 *
 *      File            CWeapon.cpp
 *      Description     Game Weapon
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

#include "CWeapon.h"

#define ANI_FIRE        100
#define ANI_RELOAD      500
#define BOUNCE_RELOAD   15.f

struct SConstant
{
    EPath               eMesh;
    EPath               eTexture;
    core::vector3df     tPoint;
    core::vector3df     tAngle;
    core::vector3df     tScale;
    KDfloat             fAniSpeed;
    KDuint              uFireDelay;
    KDuint              uFireRepeat;
    KDuint              uBulletNum;
};

static struct SConstant  l_aConstants [ ] = 
{
    { 
        eMDL_Gun2, eTEX_Gun2,
        core::vector3df ( 0, -10, 5 ), core::vector3df ( 0,  0, 0 ), core::vector3df ( 0.1f ),
        2000.f, 300, 1, 12 
    },
    { 
        eMDL_Gun2, eTEX_Rock,
        core::vector3df ( 0, -10, 5 ), core::vector3df ( 0,  0, 0 ), core::vector3df ( 0.1f ),
        2000.f, 150, 30, 60 
    },     
};

CWeapon::CWeapon ( KDvoid )
{
    m_uType         = 0;
    m_uTime         = 0;
    m_pNodes[0]     = 0;
    m_pNodes[1]     = 0;  
    m_pSounds[0][0] = 0;
    m_pSounds[0][1] = 0;
    m_pSounds[1][0] = 0;
    m_pSounds[1][1] = 0;
    m_uSndTime[0]   = 0;
    m_uSndTime[1]   = 0;
    m_bSndFire[0]   = 0;
    m_bSndFire[1]   = 0;   
    m_uActDone[0]   = 0;
    m_uActDone[1]   = 0;
}

CWeapon::~CWeapon ( KDvoid )
{
    this->clear ( );
}

KDbool CWeapon::init ( KDvoid )
{
    scene::ISceneManager*   pScene = g_pEngMgr->getScene ( );
    video::IVideoDriver*    pVideo = g_pEngMgr->getVideo ( );    

    KDuint  uMax = sizeof ( l_aConstants ) / sizeof ( l_aConstants[0] );
    
    for ( KDuint  i = 0; i < uMax; i++ )
    {
        pScene->getMesh    ( g_pResMgr->getPath ( l_aConstants [ i ].eMesh    ) );
        pVideo->getTexture ( g_pResMgr->getPath ( l_aConstants [ i ].eTexture ) );
    }
    
    pVideo->getTexture ( g_pResMgr->getPath ( eTEX_Effect_Fires, 0 ) );
    pVideo->getTexture ( g_pResMgr->getPath ( eTEX_Effect_Fires, 1 ) );
    
    g_pResMgr->getSound ( eSND_Machine_Gun, 0, 0 );
    g_pResMgr->getSound ( eSND_Machine_Gun, 0, 1 );
    g_pResMgr->getSound ( eSND_Machine_Gun, 0, 2 );
    g_pResMgr->getSound ( eSND_Machine_Gun, 0, 3 );
    
    g_pResMgr->getSound ( eSND_Ripple, 0, 0 );
    g_pResMgr->getSound ( eSND_Ripple, 0, 1 );
    
    g_pResMgr->getSound ( eSND_Effect_Change );
    g_pResMgr->getSound ( eSND_Effect_Reload );
    
    load ( 0 );
    
    return KD_TRUE;
}

KDbool CWeapon::load ( KDuint uType )
{
    scene::ISceneManager*           pScene   = g_pEngMgr->getScene ( );
    video::IVideoDriver*            pVideo   = g_pEngMgr->getVideo ( );
    scene::ICameraSceneNode*        pCamera  = g_pEngMgr->getCamera ( );
    scene::IAnimatedMeshSceneNode*  pNode    = 0;
    scene::IAnimatedMesh*           pMesh    = 0;
    scene::ISceneNodeAnimator*      pAnimate = 0;
    video::ITexture*                pTexture = 0;
    KDbool                          bOldNode = KD_FALSE;
    core::line3df                   tLine;
    
    pMesh = pScene->getMesh ( g_pResMgr->getPath ( l_aConstants [ uType ].eMesh ) );
    pTexture = pVideo->getTexture ( g_pResMgr->getPath ( l_aConstants [ uType ].eTexture ) );
    
    for ( KDuint  i = 0; i < 2; i++ )
    {   
        if ( m_pNodes [ i ] )
        {
            pNode = m_pNodes [ i ];    
            
            tLine.start  = pNode->getPosition ( );
            tLine.end    = tLine.start; 
            tLine.end.Z -= BOUNCE_RELOAD;
            
            pAnimate = pScene->createFlyStraightAnimator ( tLine.start, tLine.end, ANI_RELOAD );
            pNode->addAnimator ( pAnimate );
            pAnimate->drop ( );
            
            pAnimate = pScene->createDeleteAnimator ( ANI_RELOAD );
            pNode->addAnimator ( pAnimate );
            pAnimate->drop ( );    
            
            bOldNode = KD_TRUE;                        
        }
        
        pNode = pScene->addAnimatedMeshSceneNode ( pMesh, pCamera );
        
        pNode->setPosition ( l_aConstants[ uType ].tPoint + core::vector3df ( i == 0 ? -10.f : 10.f, 0, 0 ) );	
        pNode->setRotation ( l_aConstants[ uType ].tAngle );
        pNode->setScale    ( l_aConstants[ uType ].tScale );
        
        pNode->setMaterialTexture ( 0, pTexture	);
        pNode->setMaterialFlag ( video::EMF_LIGHTING, false );
        pNode->setAnimationSpeed ( 0 );
        pNode->setAnimationEndCallback ( this );
        
        if ( bOldNode )
        {
            tLine.end      = pNode->getPosition ( );
            tLine.start    = tLine.start; 
            tLine.start.Z -= BOUNCE_RELOAD * 2;
        
            pAnimate = pScene->createFlyStraightAnimator ( tLine.start, tLine.end, ANI_RELOAD * 2 );
            pNode->addAnimator ( pAnimate );
            pAnimate->drop ( );
            
            g_pResMgr->playSound ( eSND_Effect_Change );
        }
        
        m_pNodes  [ i ] = pNode;
        m_eAction [ i ] = eActLoad;
        m_uActDone[ i ] = g_pEngMgr->getTime ( ) + ANI_RELOAD * 2;
        
        EPath  ePath = uType == 0 ? eSND_Ripple : eSND_Machine_Gun;
        
        for ( KDuint  j = 0; j < 2; j++ )
        {
            m_pSounds [ i ][ j ] = g_pResMgr->getSound ( ePath, 0, i * 2 + j );
        }                
    }
    
    m_uType = uType;
    
    return KD_TRUE;
}

KDvoid CWeapon::onUpdate ( KDuint uTime )
{
    for ( KDuint  i = 0; i < 2; i++ )
    {
        if ( l_aConstants [ m_uType ].uFireRepeat > 1 )
        {
            if ( m_eAction [ i ] == eActFire )
            {
                KDuint  uDiff = uTime - m_uSndTime [ i ];
            
                if ( uDiff > l_aConstants [ m_uType ].uFireDelay * 2 )
                {
					m_pSounds [ i ][ m_bSndFire [ i ] ? 0 : 1 ]->Rewind ( );
                    m_bSndFire [ i ] = !m_bSndFire [ i ];
                    m_uSndTime [ i ] = uTime;
                }        
            }
        }
        
        if ( uTime > m_uActDone [ i ] )
        {
            if ( m_eAction [ i ] == eActFire && l_aConstants [ m_uType ].uFireRepeat > 1 )
            {
                
            }
            else
            {
                setAction ( eActHold, i );
            }
        }
    }
    
    m_uTime = uTime;
}

CWeapon::EAction CWeapon::getAction ( KDuint uIndex )
{
    return m_eAction [ uIndex ];
}

KDvoid CWeapon::setAction ( EAction eAction, KDuint uIndex )
{
    if ( eAction == eActFire )
    {
        scene::ISceneManager*           pScene   = g_pEngMgr->getScene ( );
        video::IVideoDriver*            pVideo   = g_pEngMgr->getVideo ( );
        scene::ICameraSceneNode*        pCamera  = g_pEngMgr->getCamera ( );
        scene::ISceneNode*              pNode    = 0;
        scene::ISceneNodeAnimator*      pAnimate = 0;
        core::array<video::ITexture*>   aTextures;
        
        aTextures.push_back ( pVideo->getTexture ( g_pResMgr->getPath ( eTEX_Effect_Fires, 0 ) ) );
        aTextures.push_back ( pVideo->getTexture ( g_pResMgr->getPath ( eTEX_Effect_Fires, 1 ) ) );
        
        pAnimate = pScene->createTextureAnimator ( aTextures, ANI_FIRE );
        
        pNode = pScene->addBillboardSceneNode 
        (
            pCamera, core::dimension2d<f32> ( 16, 16 ), core::vector3df ( uIndex == 0 ? -10.f : 9.f, -8.f, 17 ) 
        );        
        pNode->setMaterialTexture ( 0, pVideo->getTexture ( g_pResMgr->getPath ( eTEX_Effect_Fires, 0 ) ) );        
        pNode->setMaterialFlag ( video::EMF_LIGHTING, false );        
        pNode->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
        
        pNode->addAnimator ( pAnimate );
        pAnimate->drop ( );
        
        pAnimate = pScene->createDeleteAnimator ( ANI_FIRE );
        pNode->addAnimator ( pAnimate );
        pAnimate->drop ( );           
    }
    
    if ( m_eAction [ uIndex ] == eAction )
    {
        return;
    }
    
    switch ( eAction )
    {
        case eActHold :
            
            m_pNodes [ uIndex ]->setAnimationSpeed ( 0 );
            m_uActDone [ uIndex ] = 0;
            m_uSndTime [ uIndex ] = 0;
            
            break;
            
        case eActReload :
        {
            if ( m_eAction [ uIndex ] != eActHold )
            {
                return;
            }
            
            scene::ISceneManager*           pScene   = g_pEngMgr->getScene ( );
            scene::IAnimatedMeshSceneNode*  pNode    = m_pNodes [ uIndex ];
            scene::ISceneNodeAnimator*      pAnimate = 0;
            core::line3df                   tLine;            
              
            pNode->setPosition ( l_aConstants[ m_uType ].tPoint + core::vector3df ( uIndex == 0 ? -10.f : 10.f, 0, 0 ) );
            
            tLine.start  = pNode->getPosition ( );
            tLine.end    = tLine.start; 
            tLine.end.Z -= BOUNCE_RELOAD;
            
            pAnimate = pScene->createFlyStraightAnimator ( tLine.start, tLine.end, ANI_RELOAD, false, true );
            pNode->addAnimator ( pAnimate );
            pAnimate->drop ( );
            
            m_uActDone [ uIndex ] = m_uTime + ANI_RELOAD;
            g_pResMgr->playSound ( eSND_Effect_Reload );
            
            
        }   break;
            
        case eActFire :

            if ( m_eAction [ uIndex ] != eActHold )
            {
                return;
            }
            
            m_pNodes   [ uIndex ]->setAnimationSpeed ( l_aConstants [ m_uType ].fAniSpeed );    
            m_uActDone [ uIndex ] = m_uTime + l_aConstants [ m_uType ].uFireDelay;
            
            if ( l_aConstants [ m_uType ].uFireRepeat == 1 )
            {
				m_pSounds [ uIndex ][ 0 ]->Rewind ( );
            }
            
            m_uSndTime [ uIndex ] = 1;        
           
            break;
            
        case eActFired :
            
            if ( l_aConstants [ m_uType ].uFireRepeat > 1 )
            {
                m_uActDone [ uIndex ] = m_uTime + l_aConstants [ m_uType ].uFireDelay * 2;
            }
            
            break;
            
        default:
            
            break;
    }
    
    m_eAction [ uIndex ] = eAction;
}

KDvoid CWeapon::OnAnimationEnd ( scene::IAnimatedMeshSceneNode* pNode )
{
    for ( KDint i = 0; i < 2; i++ )
    {
        if ( m_pNodes [ i ] == pNode )
        {
           /* 
            switch ( m_eAction [ i ] )
            {
                case eActReload :
                     
                    setAction ( eActHold, i );
                    
                    break;
                    
                default:
                    break;
            } 
        */
            break;
        }
    }
}

KDvoid CWeapon::onSound ( KDuint uIndex )
{
    
}

KDvoid CWeapon::clear ( KDvoid )
{
    for ( KDuint  i = 0; i < 2; i++ )
    {
        if ( m_pSounds [ i ] )
        {
            m_pSounds [ i ][ 0 ] = 0;
            m_pSounds [ i ][ 1 ] = 0;
        }
        
        if ( m_pNodes [ i ] )
        {
            m_pNodes [ i ]->setAnimationEndCallback ( 0 );
            m_pNodes [ i ]->remove ( );
            m_pNodes [ i ] = 0;
        }
    }
}

KDuint CWeapon::getFireDelay ( KDvoid )
{
    return l_aConstants [ m_uType ].uFireDelay;
}

KDuint CWeapon::getFireRepeat ( KDvoid )
{
    return l_aConstants [ m_uType ].uFireRepeat;
}

KDuint CWeapon::getBulletNum ( KDuint uType )
{
    return l_aConstants [ uType ].uBulletNum;
}