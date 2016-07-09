/* --------------------------------------------------------------------------
 *
 *      File            CEnemy.cpp
 *      Description     Game Enemy
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
#include "../Screen/ScrGame.h"

#include "CEnemy.h"
 
#define STEP_MOVE         50.f
#define MIN_DIST_ENEMY    25.f
#define MAX_DIST_RADER    500.f

struct SConstant
{
    EPath               eMesh;
    KDuint              uEnergy;
    KDfloat             fSpeed;
    core::vector3df     tPoint;
    core::vector3df     tAngle;
    core::vector3df     tScale;
    KDfloat              fDelay; 
};

const struct SConstant  l_aConstants [ ] =
{
    { eMDL_Zombie1, 100, 1, core::vector3df (   0, 0,   0 ), core::vector3df ( 0,   0, 0 ), core::vector3df ( 20 ) },
    { eMDL_Zombie2, 100, 1, core::vector3df (   0, 0,   0 ), core::vector3df ( 0,   0, 0 ), core::vector3df ( 2 ) },
    { eMDL_Zombie3, 100, 1, core::vector3df (   0, 0,   0 ), core::vector3df ( 0,   0, 0 ), core::vector3df ( 2 ) },
    { eMDL_Zombie4, 100, 1, core::vector3df (   0, 0,   0 ), core::vector3df ( 0,   0, 0 ), core::vector3df ( 2 ) },
    { eMDL_Zombie5, 100, 1, core::vector3df (   0, 0,   0 ), core::vector3df ( 0,   0, 0 ), core::vector3df ( 2 ) },
    
    { eMDL_Enemy1, 100, 1, core::vector3df (   0, -66,   0 ), core::vector3df ( 0,   0, 0 ), core::vector3df ( 2 ) },
    { eMDL_Enemy2, 100, 1, core::vector3df (   0, -25,   0 ), core::vector3df ( 0,  90, 0 ), core::vector3df ( 2 ) },
    { eMDL_Enemy3, 100, 1, core::vector3df (   0, -25, -10 ), core::vector3df ( 0,  90, 0 ), core::vector3df ( 2 ) },
    { eMDL_Enemy4, 100, 1, core::vector3df (   0, -25, -10 ), core::vector3df ( 0,  90, 0 ), core::vector3df ( 2 ) },    
    { eMDL_Enemy4, 100, 1, core::vector3df (   0, -25, -10 ), core::vector3df ( 0,  90, 0 ), core::vector3df ( 2 ) },    
    { eMDL_Enemy5, 100, 1, core::vector3df (   0, -66,   0 ), core::vector3df ( 0, 180, 0 ), core::vector3df ( 14 ) },  
	{ eMDL_Enemy1, 100, 1, core::vector3df (   0, -66,   0 ), core::vector3df ( 0,   0, 0 ), core::vector3df ( 2 ) },
};

static  core::vector3df    l_tWorldPos;

CEnemy::CEnemy ( KDvoid )
{
    m_pNode     = 0;
    m_uType     = 0;
    
    m_fDir      = 0;
    m_fDist     = 0;
    m_fCos      = 0;
    m_fSin      = 0;
    
    m_uAniBegin = 0;
    m_uAniEnd   = 0;
    
    m_uFrames   = 0;
    m_uEnergy   = 0;
    
    m_pRaderBG  = 0;
    m_pRader    = 0;
}

CEnemy::~CEnemy ( KDvoid )
{
    if ( m_pNode )
    {        
        m_pNode->setAnimationEndCallback ( 0 );        
        m_pNode->remove ( );
        m_pNode = 0;
    }
    
    if ( m_pRaderBG && m_pRader )
    {
        m_pRaderBG->removeChild ( m_pRader, KD_TRUE );
        m_pRader = 0;
    }
}

KDbool CEnemy::load ( KDuint uType, KDfloat fDir, KDfloat fDist, CCSprite* pRaderBG )
{
    scene::ISceneManager*           pScene    = g_pEngMgr->getScene ( );
    scene::ICameraSceneNode*        pCamera   = g_pEngMgr->getCamera ( );
    core::vector3df                 tCamPos   = pCamera->getPosition ( );
    scene::IAnimatedMesh*           pMesh     = 0;
    scene::IAnimatedMeshSceneNode*  pNode     = 0; 
    KDfloat                         fCos      = 0;
    KDfloat                         fSin      = 0;
    KDuint                          uFrames   = 0;
    
    fCos = kdCosf ( KD_DEG2RAD ( fDir ) );
    fSin = kdSinf ( KD_DEG2RAD ( fDir ) );
    
    pMesh = pScene->getMesh
    (
        g_pResMgr->getPath ( l_aConstants [ uType ].eMesh )
    );
    
    pNode = pScene->addAnimatedMeshSceneNode ( pMesh );
    uFrames = pNode->getEndFrame ( );
    
    pNode->setVisible ( false );
    pNode->setMaterialFlag ( video::EMF_LIGHTING, true );
    pNode->setAnimationSpeed ( 5000 );
    pNode->setFrameLoop ( 0, 0 );
    pNode->setLoopMode ( false );
    pNode->setAnimationEndCallback ( this );
    pNode->setUserPtr ( this );
    
	pNode->getMaterial(0).Shininess = 0.10f; 
	pNode->getMaterial(0).AmbientColor .set ( 255, 224, 224, 224 ); 
	pNode->getMaterial(0).DiffuseColor .set ( 255, 224, 224, 224 ); 
	pNode->getMaterial(0).EmissiveColor.set ( 255,   5,   5,   5 ); 
    pNode->getMaterial(0).SpecularColor.set ( 255, 255, 255, 255 ); 
    
    pNode->setPosition ( l_aConstants [ uType ].tPoint + l_tWorldPos + core::vector3df ( fCos * fDist, 0, fSin * fDist ) );
    pNode->setRotation ( l_aConstants [ uType ].tAngle + core::vector3df ( 0, 90 - fDir, 0 ) );
    pNode->setScale    ( l_aConstants [ uType ].tScale );   
    
    m_pNode     = pNode;
    m_uType     = uType;
    m_fDir      = fDir;
    m_fDist     = fDist;
    m_fCos      = fCos;
    m_fSin      = fSin;
    m_uFrames   = uFrames;
    m_uEnergy   = l_aConstants [ uType ].uEnergy;
    m_pRaderBG  = pRaderBG;
         
    // Rader 
    m_pRader = g_pResMgr->getSprite ( pRaderBG, eIMG_ETC_Rader, 1 );
    m_pRader->setOpacity ( 128 );
    m_pRader->setVisible ( KD_FALSE );
  
	setAction ( eActSpawn );

    return KD_TRUE;
}

/*
KDvoid CEnemy::setPosition ( core::vector3df tPoint )
{
    scene::ICameraSceneNode*  pCamera = g_pEngMgr->getCamera ( );
    core::vector3df           tCamPos = pCamera->getPosition ( );

    tCamPos.Y = 0;
    
    m_pNode->setPosition ( tPoint + tCamPos + l_aConstants [ m_uIndex ].tPoint );
}

core::vector3df CEnemy::getPosition ( KDvoid )
{
    scene::ICameraSceneNode*  pCamera = g_pEngMgr->getCamera ( );
    core::vector3df           tCamPos = pCamera->getPosition ( );
    
    tCamPos.Y = 0;
    
    return m_pNode->getPosition ( ) - tCamPos - l_aConstants [ m_uIndex ].tPoint;
}

KDvoid CEnemy::setRotate ( core::vector3df tRotate )
{
    tRotate += l_aConstants [ m_uIndex ].tAngle;
    
    m_pNode->setRotation ( tRotate );
}

core::vector3df CEnemy::getRotate ( KDvoid )
{
    core::vector3df  tRotate = m_pNode->getRotation ( );
    
    tRotate -= l_aConstants [ m_uIndex ].tAngle;
    
    return tRotate;
}

KDvoid CEnemy::setScale ( core::vector3df tScale )
{
    tScale *= l_aConstants [ m_uIndex ].tScale;
    
    m_pNode->setScale ( tScale );
}

core::vector3df CEnemy::getScale ( KDvoid )
{
    core::vector3df  tPoint = m_pNode->getScale ( );
    
    tPoint /= l_aConstants [ m_uIndex ].tScale;
    
    return tPoint;
}
*/

KDvoid CEnemy::setDamage ( KDuint uDamage )
{
    if ( m_uEnergy == 0 )
    {
        return;
    }
    
    g_pEnvMgr->plusScore ( 1 );
    ((CScrGame*) g_pScrMgr->getScreen ( ))->refreshScore ( );
    
    if ( uDamage >= m_uEnergy )
    {
        m_uEnergy = 0;
        setAction ( eActDead );
    }
    else
    {
        m_uEnergy -= uDamage;
        setAction ( eActDamage );
    }
}

KDfloat CEnemy::getCos ( KDvoid )
{
    return m_fCos;
}

KDfloat CEnemy::getSin ( KDvoid )
{
    return m_fSin;
}

KDfloat CEnemy::getDistance  ( KDvoid )
{
    return m_fDist;
}

KDfloat CEnemy::getDirection ( KDvoid )
{
    return m_fDir;
}

KDvoid CEnemy::setAction ( EAction eAction )
{
    KDuint  uDelay    = 0;
    KDuint  uDuration = 0;
    
    if ( m_eAction == eAction )
    {
        return;
    }
    
    m_pNode->setLoopMode ( false );
    
    switch ( eAction )
    {
        case eActSpawn :
            
            uDelay    = 1500;
            uDuration = 1000;
            
            //loadSpawn ( );
            
            break;
            
        case eActHold :
            
            uDuration = 300;
            
            m_pNode->setCurrentFrame ( 0 );
            m_pNode->setFrameLoop ( 0, 0 );
            
            break;
            
        case eActWalk :
            
            uDuration = 2000;
            
            m_pNode->setLoopMode ( true );
            m_pNode->setCurrentFrame ( 0 );        
            m_pNode->setFrameLoop ( 0, (s32) ( m_uFrames * 0.3 ) );
            
            break;
            
        case eActAttack :        
            
            m_pNode->setCurrentFrame ( 0 );
            m_pNode->setFrameLoop ( 0, (s32) ( m_uFrames * 0.3 ) );
                        
            break;
            
        case eActDamage :
            
            uDuration = 1000;
            
            g_pResMgr->playSound ( eSND_Enemy_Harm );
                        
            m_pNode->setCurrentFrame ( 0 );
            m_pNode->setFrameLoop ( (s32) ( m_uFrames * 0.5 ), (s32) ( m_uFrames * 0.65 ) );
            
            break;
            
        case eActDead :
            
            g_pResMgr->playSound ( eSND_Enemy_Dead );
            
            m_pNode->setID ( -1 );
            m_pNode->setCurrentFrame ( 0 );
            m_pNode->setFrameLoop ( (s32) ( m_uFrames * 0.8 ), (s32) ( m_uFrames * 0.9 ) );            
            
            break;
            
        case eActDisappear :
            
            uDuration = 2000;
            
            break;
            
        default :
            
            break;
    }
 
    m_eAction   = eAction;
    m_uAniBegin = g_pEngMgr->getTime ( ) + uDelay;
    m_uAniEnd   = m_uAniBegin + uDuration;    
}

KDbool CEnemy::onUpdate ( KDuint uTime )
{
    if ( uTime >= m_uAniBegin )
    {
        if ( m_eAction == eActWalk && m_fDist > MIN_DIST_ENEMY )
        {
            scene::ICameraSceneNode*    pCamera = g_pEngMgr->getCamera ( );
            core::vector3df             tCamPos = pCamera->getPosition ( );         
            
            m_fDist -= STEP_MOVE * ( uTime - m_uTime ) / ( m_uAniEnd - m_uAniBegin );                        
            
            if ( m_fDist < MIN_DIST_ENEMY )
            {
                m_fDist = MIN_DIST_ENEMY;
            }
            
            m_pNode->setPosition 
            ( 
                l_aConstants [ m_uType ].tPoint + l_tWorldPos +
                core::vector3df ( m_fCos * m_fDist, 0, m_fSin * m_fDist ) 
            );   
            
            // Display
            if ( m_fDist < MAX_DIST_RADER )
            {
                KDfloat  fDist = m_fDist / 7.f;                
                
                m_pRader->setVisible ( KD_TRUE );
                m_pRader->setPosition ( ccpAdd 
                (
                    ccp ( m_fCos * fDist, m_fSin * fDist ), 
                    ccpMid ( m_pRaderBG->getContentSize ( ) )
                ) );
            }     
        }
        
        if ( uTime >= m_uAniEnd )
        {
            switch ( m_eAction )
            {
                case eActSpawn :                                        
                    {
                        scene::ISceneManager*       pScene    = g_pEngMgr->getScene ( );
                        scene::ITriangleSelector*   pSelector = 0;  
                    
                        pSelector = pScene->createTriangleSelector ( m_pNode );
                        m_pNode->setTriangleSelector ( pSelector );
                        pSelector->drop ( ); 
                        
                        m_pNode->setVisible ( true );
                        m_pNode->setID ( 1 );
                    
                        this->setAction ( eActHold );                                            
                    }
                    break;
                    
                case eActDisappear :
                                        
                    return KD_FALSE;
                                              
                case eActHold :
                    
                    if ( m_fDist > MIN_DIST_ENEMY )
                    {
                        this->setAction ( eActWalk );
                    }
                    else
                    {
                        this->setAction ( eActAttack );
                    }
                    
                    break;
                    
                case eActWalk :                    

                    this->setAction ( eActHold );
                    
                    break;                
                    
                default :
                    
                    break;
            }
        }
        else
        {
            switch ( m_eAction )
            {
                case eActSpawn :
                    
                    m_pNode->setVisible ( ( ( ( m_uAniEnd - uTime ) / 150 ) % 2 ) == 1 );
                    
                    break;                                
                    
                case eActDisappear :
                    
                    m_pNode->setVisible ( ( ( ( m_uAniEnd - uTime ) / 150 ) % 2 ) == 1 );
                    
                    break;
                    
                default :
                    
                    break;
            }
        }
    }
    
    m_uTime = uTime;
    
    return KD_TRUE;
}

KDvoid CEnemy::OnAnimationEnd ( scene::IAnimatedMeshSceneNode* pNode )
{
    switch ( m_eAction )
    {
        case eActWalk :
            
            this->setAction ( eActWalk );
            
            break;        
            
        case eActAttack :
            {
                KDuint  uEnergy = g_pEnvMgr->getEnergy ( );
                
                if ( uEnergy > 0 )
                {                    
                    uEnergy = uEnergy > 5 ? uEnergy - 5 : 0;
                    if ( uEnergy > 0 )
                    {
                        g_pResMgr->playSound ( eSND_Player_Harm );
                        g_pEnvMgr->setEnergy ( uEnergy );                    
                    }
                    else                    
                    {
                        g_pResMgr->playSound ( eSND_Player_Dead );
                        g_pEnvMgr->setEnergy ( 0 );
                    }
                    
                    ((CScrGame*) g_pScrMgr->getScreen ( ))->refreshEnergy ( );
                }
                
                this->setAction ( eActHold );
            }
            break;
            
        case eActDamage :
            
            this->setAction ( eActHold );
            
            break;
            
        case eActDead :
        {                        
            g_pEnvMgr->plusScore ( 100 );
            ((CScrGame*) g_pScrMgr->getScreen ( ))->refreshScore ( );
            
            m_pRader->setVisible ( KD_FALSE );
            
            this->setAction ( eActDisappear );
            
        }   break;
            
        default:
            
            break;
    }
}

KDvoid CEnemy::loadSpawn ( KDvoid )
{
    scene::ISceneManager*           pScene = g_pEngMgr->getScene ( );
    video::IVideoDriver*            pVideo = g_pEngMgr->getVideo ( );
    scene::ISceneNodeAnimator*      pAnimate;   
    video::ITexture*                pTexture;
    core::array<video::ITexture*>   aTextures;
    scene::IBillboardSceneNode*     pSpawn;
    
    for ( KDuint i = 0; i < 7; i++ )
    {
        pTexture = pVideo->getTexture ( g_pResMgr->getPath ( eTEX_Spawn, i ) );        
        aTextures.push_back ( pTexture );
    }            
    
    pAnimate = pScene->createTextureAnimator ( aTextures, 100 );
    pSpawn   = pScene->addBillboardSceneNode 
    (
        0, core::dimension2d<f32> ( 300, 300 ),
        m_pNode->getPosition ( ) - l_aConstants [ m_uType ].tPoint + l_tWorldPos + core::vector3df ( 0, 100, 0 ) 
    );
    
    pSpawn->setMaterialFlag ( video::EMF_LIGHTING, false );
    pSpawn->setMaterialTexture ( 0, pTexture );            
    pSpawn->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
    pSpawn->addAnimator ( pAnimate );
    
    pAnimate->drop ( );               
    
    pAnimate = pScene->createDeleteAnimator ( 2000 );
    pSpawn->addAnimator ( pAnimate );
    pAnimate->drop ( ); 
    
    g_pResMgr->playSound ( eSND_Enemy_Spawn );
}

CEnemies::CEnemies ( KDvoid )
{    
    m_fSpawnDist    = 0;
    m_uSpawnTime    = 0;
    m_uSpawnDirMax  = 24;
    m_pRaderBG      = 0;
    
    m_fSpawnDirs.clear ( );
    
    g_pResMgr->getSound ( eSND_Enemy_Spawn );
    g_pResMgr->getSound ( eSND_Enemy_Harm );
    g_pResMgr->getSound ( eSND_Enemy_Dead );
    g_pResMgr->getSound ( eSND_Player_Harm );
    g_pResMgr->getSound ( eSND_Player_Dead );
}

CEnemies::~CEnemies ( KDvoid )
{
    for ( KDuint  i = 0; i < m_aEnemies.size ( ); i++ )
    {
        CC_SAFE_DELETE ( m_aEnemies [ i ] );
    }
    
    m_aEnemies.clear ( );
}

KDbool CEnemies::init ( const core::vector3df& tWorldPos, KDfloat fSpawnDist, CCSprite* pRaderBG )
{
    l_tWorldPos  = tWorldPos;
    m_fSpawnDist = fSpawnDist;
    m_pRaderBG   = pRaderBG;
    
    for ( KDuint  i = 0; i < m_uSpawnDirMax; i++ )
    {
        m_fSpawnDirs.push_back ( ( 360.f / m_uSpawnDirMax ) * i );
    }
    
    return KD_TRUE;
}

KDvoid CEnemies::onUpdate ( KDuint uTime )
{    
    if ( g_pEngMgr->getTimer ( )->isStopped ( ) == false )
    {
        if ( ( uTime - m_uSpawnTime ) > 3000 && m_aEnemies.size ( ) < 6 )
        {
            CEnemy*  pEnemy = new CEnemy ( ); 
            KDuint   uIndex = kdRand ( ) % m_fSpawnDirs.size ( );
            KDfloat  fDir   = m_fSpawnDirs [ uIndex ];  
             
            if ( pEnemy && pEnemy->load ( 0, fDir, m_fSpawnDist, m_pRaderBG ) )
            {
                m_aEnemies.push_back ( pEnemy );                            
            }
            else
            {
                CC_SAFE_DELETE ( pEnemy );
            }
            
            m_uSpawnTime = uTime;
            m_fSpawnDirs.erase ( uIndex );
        }        
    }
    
    for ( KDuint  i = 0; i < m_aEnemies.size ( ); i++ )
    {
        CEnemy*   pEnemy = m_aEnemies [ i ];        
        
        if ( pEnemy->onUpdate ( uTime ) == KD_FALSE )
        {
            m_fSpawnDirs.push_back ( pEnemy->getDirection ( ) );
            
            delete pEnemy;
            m_aEnemies.erase ( i );
            i--;
        }
    }
}