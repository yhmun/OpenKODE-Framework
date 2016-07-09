/* --------------------------------------------------------------------------
 *
 *      File            ScrGame.cpp
 *      Description     Game Scene
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

#include "../Game/CWorld.h"
#include "../Game/CEnemy.h"
#include "../Game/CTarget.h"
#include "../Game/CWeapon.h"
#include "../Game/CBullet.h"

#include "ScrGame.h"

KDbool CScrGame::init ( KDvoid )
{    
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}
    
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

    m_uiDamaged = g_pResMgr->getSprite ( this, eIMG_Effect_Damaged );
    m_uiDamaged->setPosition ( ccpMid ( tWinSize ) );
    m_uiDamaged->setVisible ( KD_FALSE );
       
    m_uiEmergency = g_pResMgr->getSprite ( this, eIMG_Effect_Low_HP );
    m_uiEmergency->setPosition ( ccpMid ( tWinSize ) );
    m_uiEmergency->setVisible ( KD_FALSE );
    
    m_uiLayer = CCLayer::create ( );
    this->addChild ( m_uiLayer );
    
    m_uiEnergy = g_pResMgr->getProgress ( m_uiLayer, eIMG_Player_HP_Red );
    {
        const CCSize&  tSize = m_uiEnergy->getContentSize ( );
        
        m_uiEnergy->setPosition ( ccp ( tWinSize.cx - tSize.cx - 20, tWinSize.cy - tSize.cy / 2 - 20 ) );        
        m_uiEnergy->setAnchorPoint ( ccp ( 0, 0.5f ) );
        m_uiEnergy->setType ( kCCProgressTimerTypeBar );
        m_uiEnergy->setMidpoint ( ccp ( 1, 0 ) );
        m_uiEnergy->setBarChangeRate ( ccp ( 1, 0 ) );
        m_uiEnergy->setPercentage ( 100 );  
        
        CCLabelBMFont*  uiLabel = g_pResMgr->getLabel ( m_uiEnergy, "H" );        
        uiLabel->setPosition ( ccp ( -uiLabel->getContentSize ( ).cx / 2 - 10, tSize.cy / 2 ) );
    }
    
    m_uiScore = g_pResMgr->getLabel ( m_uiLayer, "Score : 0" );
    m_uiScore->setPosition ( ccp ( tWinSize.cx / 2 - 30, tWinSize.cy - m_uiScore->getContentSize ( ).cy ) );
    
    m_uiMode  = g_pResMgr->getLabel ( m_uiLayer, "Touch Mode " );
    m_uiMode->setAnchorPoint ( ccp ( 0, 0.5f ) );
    m_uiMode->setPosition ( ccp ( 150, tWinSize.cy - m_uiScore->getContentSize ( ).cy ) );
    
    m_uiRader = g_pResMgr->getSprite ( m_uiLayer, eIMG_BTN_Pad_BG );
    {
        const CCSize&  tSize = m_uiRader->getContentSize ( );
        
        m_uiRader->setPosition ( ccp ( tSize.cx / 2 + 20 , tWinSize.cy - tSize.cy / 2 - 90 ) );
    }
    
    m_uiWeapon = g_pResMgr->getSprite ( m_uiLayer, eIMG_Icon_Weapons, 0 );
    {
        const CCSize&  tSize = m_uiWeapon->getContentSize ( );
        
        m_uiWeapon->setPosition ( ccp ( tWinSize.cx - tSize.cx / 2 - 20 , tWinSize.cy - tSize.cy / 2 - 60 ) );
    }
    
	CCMenuItemSprite*  pPause = g_pResMgr->getMenuItemSprite
    (
        m_uiLayer, eIMG_BTN_Pause_Nor, ePath_Null, this, menu_selector ( CScrGame::onPause )
    );
    {
        const CCSize&  tSize = pPause->getContentSize ( );
        
        pPause->setPosition ( ccp ( tSize.cx / 2 + 20, tWinSize.cy - tSize.cy / 2 - 20 ) );    
    }
    
	CCMenuItemSprite*  pMode = g_pResMgr->getMenuItemSprite
    (
        m_uiLayer, eIMG_BTN_Pause_Nor, ePath_Null, this, menu_selector ( CScrGame::onMode )
    );
    {
        const CCSize&  tSize = pMode->getContentSize ( );
        
        pMode->setPosition ( ccp ( tSize.cx / 2 + 90, tWinSize.cy - tSize.cy / 2 - 20 ) );    
    }
    
    for ( KDuint i = 0; i < 2; i++ )
    {
        CCSprite*  uiBullet = g_pResMgr->getSprite ( m_uiLayer, eIMG_Bullet );
        {
            const CCSize&  tSize = uiBullet->getContentSize ( );
            
            uiBullet->setPosition ( ccp ( tWinSize.cx - tSize.cx / 2 - 20 , tWinSize.cy - tSize.cy / 2 - ( i == 0 ? 150 : 190 ) ) );        
            
            m_uiBulletNum[i] = g_pResMgr->getLabel ( uiBullet, "  6 x" );        
            m_uiBulletNum[i]->setPosition ( ccp ( - tSize.cx / 2, tSize.cy / 2 ) );
        }
        
        m_uiTarget[i][0] = g_pResMgr->getSprite( m_uiLayer, eIMG_Target_Out );
        m_uiTarget[i][1] = g_pResMgr->getSprite( m_uiTarget[i][0], eIMG_Target_In );        
        m_uiTarget[i][0]->setPosition ( ccp ( tWinSize.cx / 2 + ( i == 0 ? -10 : 10 ), tWinSize.cy / 2 ) );
        m_uiTarget[i][1]->setPosition ( ccpMid ( m_uiTarget[i][0]->getContentSize ( ) ) );      
        m_uiTarget[i][1]->setVisible ( KD_FALSE );
        
        m_uiPad[i][0] = g_pResMgr->getSprite ( this, eIMG_BTN_Pad_BG );
        m_uiPad[i][1] = g_pResMgr->getSprite ( m_uiPad[i][0], eIMG_BTN_Pad_Nor );
        m_uiPad[i][2] = g_pResMgr->getSprite ( m_uiPad[i][0], eIMG_BTN_Pad_Sel );
        {
            const CCSize&  tSize = m_uiPad[i][0]->getContentSize ( );
            
            m_uiPad[i][0]->setPosition ( ccp ( i == 0 ? tSize.cx / 2 + 20 : tWinSize.cx - tSize.cx / 2 - 20, tSize.cy / 2 + 20 ) );
            m_uiPad[i][1]->setPosition ( ccpMid ( tSize ) );
            m_uiPad[i][2]->setPosition ( ccpMid ( tSize ) );
            m_uiPad[i][1]->setTag ( i );
            m_uiPad[i][2]->setVisible ( KD_FALSE );
        }        
        
        CCMenuItemSprite*  uiReload = g_pResMgr->getMenuItemSprite
        (
            m_uiLayer, eIMG_BTN_Reload_Nor, ePath_Null, this, menu_selector ( CScrGame::onReload )
        );        
        {
            const CCSize&  tSize = uiReload->getContentSize ( );
            
            uiReload->setPosition ( ccp ( i == 0 ? tSize.cx / 2 + 20 : tWinSize.cx - tSize.cx / 2 - 20, tSize.cy / 2 + 160 ) );
            uiReload->setTag ( i );            
        }
    }
    
    CCMenuItemSprite*  uiChange = g_pResMgr->getMenuItemSprite
    (
        m_uiLayer, eIMG_BTN_Change_Nor, ePath_Null, this, menu_selector ( CScrGame::onChange )
    );        
    {
        const CCSize&  tSize = uiChange->getContentSize ( );
        
        uiChange->setPosition ( ccp ( tWinSize.cx - tSize.cx / 2 - 160, tSize.cy / 2 + 20 ) );
    }    
    
    // Clear Scene
    g_pEngMgr->getScene ( )->clear ( );
    g_pEngMgr->getTimer ( )->stop  ( );
    
    // Create World
    m_pWorld    = new CWorld   ( );
    m_pWeapon   = new CWeapon  ( );
    m_pTarget   = new CTarget  ( );
    m_pBullet   = new CBullet  ( );
    m_pEnemies  = new CEnemies ( );
    
    m_pWorld  ->init ( );
    m_pWeapon ->init ( );
    m_pTarget ->init ( m_pWorld->getNode ( ) );
    m_pEnemies->init ( m_pWorld->getPosition ( ), m_pWorld->getSpawnDist ( ), m_uiRader );
    
    g_pResMgr->getSound ( eSND_Effect_Heart );
    g_pResMgr->getSound ( eSND_Player_Harm );
    g_pResMgr->getSound ( eSND_Player_Dead );
    
    this->setTouchMode ( kCCTouchesOneByOne );
    this->setTouchEnabled ( KD_TRUE );
    this->setEventEnabled ( KD_TRUE );
    this->setClipping     ( KD_TRUE );
    
    m_bUseSensor   = KD_FALSE;    
    m_bTargetCtl   = KD_FALSE;
    m_uWeaponIndex = 0;
    
    g_pEnvMgr->setScore ( 0 );
    g_pEnvMgr->setEnergy ( 100 );
    
    for ( KDint i = 0; i < 2; i++ )
    {
        m_uBulletNum[i][0] = m_uBulletNum[i][1] = m_pWeapon->getBulletNum ( i );
    }
    
    refreshBulletLabel ( 0 );
    refreshBulletLabel ( 1 );
    
	return KD_TRUE;
}

KDvoid CScrGame::onEnterTransitionDidFinish ( KDvoid )
{
	CCLayer::onEnterTransitionDidFinish ( );

    CCFiniteTimeAction*  pAction = CCSequence::create 
    (
        CCSpawn ::create ( CCScaleTo ::create ( 0.7f, 3 ), CCFadeIn::create  ( 0.7f ), KD_NULL ),
        CCRepeat::create ( CCSequence::create ( CCFadeOut::create ( 0.5f ), CCFadeIn ::create ( 0.5f ), KD_NULL ), 3 ),
        CCSpawn ::create ( CCScaleTo ::create ( 0.7f, 10 ), CCFadeOut::create ( 0.7f ), KD_NULL ),     
        CCCallFuncN::create ( this, callfuncN_selector ( CScrGame::onStart ) ), 
        CCCallFuncN::create ( this, callfuncN_selector ( CScrGame::onRemove ) ), 
        KD_NULL
    );
    
    CCLabelBMFont*   pStart = g_pResMgr->getLabel ( this, "Game Start !!!" );
    
    pStart->setPosition ( ccpMid ( CCDirector::sharedDirector ( )->getWinSize ( ) ) );
    pStart->setScale ( 10 );
    pStart->setOpacity ( 0 );
    pStart->runAction ( pAction );
}

KDvoid CScrGame::onExit ( KDvoid )
{
    CCSound*  pSound = CCSound::sharedSound ( );
    
    pSound->Close ( m_uBgm );
    
    CC_SAFE_DELETE ( m_pWorld );
    CC_SAFE_DELETE ( m_pWeapon );
    CC_SAFE_DELETE ( m_pTarget );
    CC_SAFE_DELETE ( m_pEnemies );
    
    CCLayer::onExit ( );
}

KDvoid CScrGame::draw ( KDvoid )
{
    IrrlichtDevice*  pDevice = g_pEngMgr->getDevice ( );

    if ( pDevice && pDevice->run ( ) )
    {
        scene::ISceneManager*       pScene  = g_pEngMgr->getScene  ( );
        video::IVideoDriver*        pVideo  = g_pEngMgr->getVideo  ( );
        scene::ICameraSceneNode*    pCamera = g_pEngMgr->getCamera ( );    
        KDuint                      uTime   = g_pEngMgr->getTime   ( );

        if ( m_tWorldTouch.getID ( ) != 0 )
        {
            m_tCameraRotate.Y += m_tWorldDelta.Y * ( uTime - m_uTime ) / 100;
            m_tCameraRotate.X += m_tWorldDelta.X * ( uTime - m_uTime ) / 100;
        }
        
        m_tCameraRotate.X = KD_CLAMP( -60.f, 60.f, m_tCameraRotate.X );        
        pCamera->setRotation ( m_tCameraRotate );
        pCamera->updateAbsolutePosition ( );
        
        m_uiRader->setRotation ( -m_tCameraRotate.Y );
        
        CCPoint tPoint1 = m_uiTarget[0][0]->convertToEGLSpaceAR ( CCPointZero );
        CCPoint tPoint2 = m_uiTarget[1][0]->convertToEGLSpaceAR ( CCPointZero );
        
        m_pTarget->onUpdate  ( uTime, core::position2di ( (s32) tPoint1.x, (s32) tPoint1.y ), core::position2di ( (s32) tPoint2.x, (s32) tPoint2.y ) );
        m_pWeapon->onUpdate  ( uTime );  
        m_pBullet->onUpdate  ( uTime ); 
        m_pEnemies->onUpdate ( uTime );
                
        /*
        if ( m_pLyrUI->numberOfRunningActions ( ) > 0 )
        {
            core::vector3df  tCamRot = m_tCamRot;
            
            tCamRot.X += m_pLyrUI->getPosition ( ).y / 2.f;
            
            pCamera->setRotation ( tCamRot );
            pCamera->updateAbsolutePosition ( );            
        }
        */ 
        
        pVideo->beginScene ( false, true );
        
        pScene->drawAll ( );
        
        pVideo->endScene ( );

        CCDirector::sharedDirector ( )->setGLDefaultValues ( );
        
        m_uTime = uTime;
    }
}

KDvoid CScrGame::onEvent ( const KDEvent* pEvent )
{
    g_pEngMgr->dispatch ( pEvent );
}

KDbool CScrGame::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    KDbool  bRet = KD_FALSE;
    
    for ( KDuint  i = 0; i < 2; i++ )
    {
		CCRect  tRect ( CCPointZero, m_uiPad[i][0]->getContentSize ( ) );
        if ( tRect.containsPoint ( m_uiPad[i][0]->convertTouchToNodeSpace ( pTouch ) ) )
        {
            bRet = KD_TRUE;
            
            m_uiPad[i][1]->setUserData  ( pTouch );
            m_uiPad[i][1]->setVisible ( KD_FALSE );
            m_uiPad[i][2]->setVisible ( KD_TRUE );
            
            if ( m_pWeapon->getAction ( i ) != CWeapon::eActHold || m_uBulletNum [ m_uWeaponIndex ][ i ] == 0 )
            {
                continue;
            }
            
            CCFiniteTimeAction*  pAction = ( CCSequence::create
			(
                CCRepeat::create
                (
                    CCSequence::create
                    (
                        CCCallFuncN::create ( this, callfuncN_selector ( CScrGame::onFire ) ), 
                        CCDelayTime::create ( m_pWeapon->getFireDelay ( ) / 1000.f ),
                        KD_NULL
                    ),
                    KD_MIN ( m_pWeapon->getFireRepeat ( ), m_uBulletNum [ m_uWeaponIndex ][ i ] )  
                ),
                CCCallFuncN::create ( this, callfuncN_selector ( CScrGame::onFired ) ),
                KD_NULL
            ) ); 
    
            m_uiPad[i][1]->runAction ( pAction ); 
        }
    }
    
    if ( !bRet && m_tWorldTouch.getID ( ) == 0 )
    {
        m_tWorldTouch = *pTouch;
    }
    
    return KD_TRUE;
}

KDvoid CScrGame::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint  tLocation = pTouch->getLocation ( );
	CCPoint  tPrev     = pTouch->getPreviousLocation ( );
    CCPoint  tDiff;
    
    if ( m_bTargetCtl )
    {
        tDiff = ccpSub ( tLocation, tPrev );
        for ( KDuint  i = 0; i < 2; i++ )
        {
            if ( m_uiPad[i][1]->getUserData ( ) == pTouch )
            {
                const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
     
                CCPoint  tPoint = m_uiPad[i][2]->getPosition ( );
                CCPoint  tMove  = ccpAdd ( tPoint, tDiff ); 
 
                tMove = ccpClamp ( tMove,
                                   ccpSub ( m_uiPad[i][1]->getPosition ( ), ccpMid ( m_uiPad[i][2]->getContentSize ( ) ) ),
                                   ccpAdd ( m_uiPad[i][1]->getPosition ( ), ccpMid ( m_uiPad[i][2]->getContentSize ( ) ) ) );
                                      
                m_uiPad[i][2]->setPosition ( tMove );
            
                //tMove  = ccpAdd ( ccp ( tWinSize.cx / 2 + ( i == 0 ? -30 : 30 ), tWinSize.cy / 2 ), 
                //                  ccpMult ( ccpSub ( tPoint, tMove ), 1.5f ) );
            
                tMove  = ccpSub ( m_uiPad[i][2]->getPosition ( ), tPoint );
                tPoint = ccp ( tWinSize.cx / 2 + ( i == 0 ? -60 : 60 ), tWinSize.cy / 2 );            
                tMove  = ccpClamp ( ccpAdd ( ccpMult ( tMove, 2.0f ) , m_uiTarget[i][0]->getPosition ( ) ),
                                    ccpSub ( tPoint,  ( m_uiPad[i][1]->getContentSize ( ) ) ),
                                    ccpAdd ( tPoint,  ( m_uiPad[i][1]->getContentSize ( ) ) ) );
            
                m_uiTarget[i][0]->setPosition ( tMove );
            }
        }
    }
    
    if ( m_tWorldTouch.getID ( ) == pTouch->getID ( ) )
    {
		tDiff = ccpSub ( tLocation, m_tWorldTouch.getLocation ( ) );

        m_tWorldDelta.Y = kdFabsf ( tDiff.x ) > 10 ? tDiff.x *  0.2f : 0;
        m_tWorldDelta.X = kdFabsf ( tDiff.y ) > 20 ? tDiff.y * -0.2f : 0;
    }
}

KDvoid CScrGame::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    for ( KDuint  i = 0; i < 2; i++ )
    {
        if ( m_uiPad[i][1]->getUserData ( ) == pTouch )
        {
            m_uiPad[i][1]->setUserData  ( KD_NULL );
            m_uiPad[i][1]->setVisible ( KD_TRUE );
            m_uiPad[i][2]->setVisible ( KD_FALSE );
            m_uiPad[i][2]->setPosition  ( m_uiPad[i][1]->getPosition () );
            
            this->onFired ( m_uiPad[i][1] );
        }
    }
    
    if ( m_tWorldTouch.getID ( ) == pTouch->getID ( ) )
    {
        m_tWorldDelta = core::vector3df ( 0, 0, 0 );
        m_tWorldTouch.setTouchInfo ( 0, 0, 0 );
    }
}

KDvoid CScrGame::onNextMain ( CCObject* pSender )
{
	g_pScrMgr->setScreen ( eScrMain, eTranProgressInOut );
}

KDvoid CScrGame::onPause  ( CCObject* pSender )
{
    g_pScrMgr->setScreen ( eScrMain, eTranProgressInOut );
}

KDvoid CScrGame::onResume ( CCObject* pSender )
{
    
}

KDvoid CScrGame::onMode ( CCObject* pSender )
{
    m_bUseSensor = !m_bUseSensor;
    
    if ( m_bUseSensor )
    {
        m_fOffsetPitch   = m_fPrevPitch;
        m_fOffsetBearing = m_fPrevBearing;
    }
    
    m_uiMode->setString ( m_bUseSensor ? "Sensor Mode" : "Touch Mode" );
}

KDvoid CScrGame::onReload ( CCObject* pSender )
{    
    KDuint  uIndex = ((CCNode*) pSender)->getTag ( );
    
    m_pWeapon->setAction ( CWeapon::eActReload, uIndex );   
    
    m_uBulletNum [ m_uWeaponIndex ][ uIndex ] = m_pWeapon->getBulletNum ( m_uWeaponIndex );
    
    refreshBulletLabel ( uIndex );
}

KDvoid CScrGame::onChange ( CCObject* pSender )
{
    m_uWeaponIndex = m_uWeaponIndex < 1 ? m_uWeaponIndex + 1 : 0;

    m_pWeapon->load ( m_uWeaponIndex );
    
    const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
    
    m_uiWeapon->removeFromParentAndCleanup ( KD_TRUE );
    m_uiWeapon = g_pResMgr->getSprite ( m_uiLayer, eIMG_Icon_Weapons, m_uWeaponIndex );
    {
        const CCSize&  tSize = m_uiWeapon->getContentSize ( );
        
        m_uiWeapon->setPosition ( ccp ( tWinSize.cx - tSize.cx / 2 - 20 , tWinSize.cy - tSize.cy / 2 - 60 ) );
    }    
    
    switch ( m_uWeaponIndex ) 
    {
        case 1  : m_bTargetCtl = KD_TRUE;  break;            
        default : m_bTargetCtl = KD_FALSE; break;
    }
    
    KDfloat  fGap = m_bTargetCtl ? 60.f : 10.f;
    for ( KDuint i = 0; i < 2; i++ )
    {
        m_uiTarget[i][0]->runAction 
        (
            CCMoveTo::create ( 0.2f, ccp ( tWinSize.cx / 2 + ( i == 0 ? -fGap : fGap ), tWinSize.cy / 2 ) )
        );
        
        refreshBulletLabel ( i );
    }
}

KDvoid CScrGame::onSoundEffect ( CCNode* pTarget, KDvoid* pData )
{
	if ( pData )
	{
		( (CSound*) pData )->Rewind ( );
	}
}

KDvoid CScrGame::setVibrate ( KDbool bOn )
{
    if ( bOn )
    {
        if ( m_uiLayer->numberOfRunningActions ( ) > 0 )
        {
            return;
        }
        
        CCFiniteTimeAction*  pVibrate = CCSequence::create 
        (
            CCMoveTo::create ( 0.07f, ccp (  20, 20 ) ),
            CCMoveTo::create ( 0.07f, ccp (   0,  0 ) ), 
            KD_NULL
        );    
        
        m_uiLayer->runAction ( pVibrate );
    }
    else
    {
        //m_uiLayer->runAction ( CCMoveTo::actionWithDuration ( 0.1f, ccp ( 0, 0 ) ) );
    }
}

KDvoid CScrGame::setDamaged ( KDbool bOn )
{
    if ( bOn )
    {
        CCFiniteTimeAction*  pAction = CCSequence::create 
        (
            CCCallFunc::create ( g_pResMgr->getSound ( eSND_Player_Harm ), callfunc_selector ( CSound::Rewind ) ),
            CCFadeOut::create ( 1.f ),
            CCHide::create ( ),
            KD_NULL
        );
        
        m_uiDamaged->runAction ( pAction );
        m_uiDamaged->setOpacity ( 255 );
        m_uiDamaged->setVisible ( KD_TRUE );
    }
    else
    {
        m_uiDamaged->setVisible ( KD_FALSE );
        m_uiDamaged->stopAllActions ( );
    }
}

KDvoid CScrGame::setEmergency ( KDbool bOn )
{
    if ( bOn )
    {
        if ( m_uiEmergency->numberOfRunningActions ( ) > 0 )
        {
            return;
        }
        
        CCFiniteTimeAction*  pAction = CCSequence::create 
        (
            CCCallFunc::create ( g_pResMgr->getSound ( eSND_Effect_Heart ), callfunc_selector ( CSound::Rewind ) ),
            CCFadeOut::create ( 1.0f ),
            CCFadeIn ::create ( 1.0f ),
            KD_NULL
        );      
        
        m_uiEmergency->runAction ( CCRepeatForever::create ( (CCActionInterval*) pAction ) );
        m_uiEmergency->setVisible ( KD_TRUE );
        m_uiEmergency->setOpacity ( 255 );
    }
    else
    {
        m_uiEmergency->setVisible ( KD_FALSE );
        m_uiEmergency->stopAllActions ( );
    }
}

KDvoid CScrGame::refreshBulletLabel ( KDuint uIndex )
{
    KDchar  szTemp [ 16 ];
    
    kdSprintf ( szTemp, "%3d x", m_uBulletNum [ m_uWeaponIndex ][ uIndex ] );
    
    m_uiBulletNum [ uIndex ]->setString ( szTemp );
}

KDvoid CScrGame::refreshScore ( KDvoid )
{
    KDchar  szTemp [ 32 ];
    
    kdSprintf ( szTemp, "Score : %d", g_pEnvMgr->getScore ( ) );
    
    m_uiScore->setString ( szTemp );
}

KDvoid CScrGame::refreshEnergy ( KDvoid )
{    
    const KDuint&  uEnergy = g_pEnvMgr->getEnergy ( );
    
    setVibrate ( KD_TRUE );
    setDamaged ( KD_TRUE );
    
    m_uiEnergy->setPercentage ( (KDfloat) uEnergy );
    
    if ( uEnergy <= 30 )
    {
        setEmergency ( KD_TRUE );
    }
    else if ( uEnergy == 0 )
    {
        //setEmergency ( KD_FALSE );
    }
}

#define MIN_STEP            0.02f
#define NOISE_ATTENUATION   3.0f
#define ADAPTATION          1

KDvoid CScrGame::onPitch ( KDfloat fPitch )
{
    KDfloat  fFilter = 0.1f;
    KDfloat  fAlpha  = fFilter;    
    
#if ( ADAPTATION )    
    KDfloat  fBeta   = KD_CLAMP ( 0.0f, 1.0f, kdFabsf ( m_fPrevPitch - fPitch ) / MIN_STEP - 1.0f );    
    fAlpha = ( 1.0f - fBeta ) * fFilter / NOISE_ATTENUATION + fBeta * fFilter;
#endif
    m_fPrevPitch = fPitch * fAlpha + m_fPrevPitch * ( 1.0f - fAlpha );
    
    if ( m_bUseSensor )
    {
        m_tCameraRotate.X  = ( ( m_fPrevPitch - m_fOffsetPitch ) * 90.f );
    }
}

KDvoid CScrGame::onBearing ( KDfloat fBearing )
{    
    KDfloat  fFilter = 0.1f;
    KDfloat  fAlpha  = fFilter;    
    
#if ( ADAPTATION )     
    KDfloat  fBeta   = KD_CLAMP ( 0.0f, 1.0f, kdFabsf ( m_fPrevBearing - fBearing ) / MIN_STEP - 1.0f );    
    fAlpha = ( 1.0f - fBeta ) * fFilter / NOISE_ATTENUATION + fBeta * fFilter;
#endif
    
    if ( kdFabsf ( m_fPrevBearing - fBearing ) > 0.9f )
    {
        m_fPrevBearing = fBearing;
    }
    else
    {
        m_fPrevBearing = fBearing * fFilter + m_fPrevBearing * ( 1.0f - fFilter );
    }
    
    if ( m_bUseSensor )
    {    
        m_tCameraRotate.Y = ( ( m_fPrevBearing - m_fOffsetBearing ) * 360.f );
    }
}

KDvoid CScrGame::onRemove ( CCNode* pSender )
{
    pSender->removeFromParentAndCleanup ( KD_TRUE );
}

KDvoid CScrGame::onStart ( CCNode* pSender )
{    
    //CCSound::sharedSound ( )->Play ( m_uBgm );
    
    g_pEngMgr->getTimer ( )->start ( );
}

KDvoid CScrGame::onFire ( CCNode* pSender )
{
    KDuint uIndex = pSender->getTag ( );
    
    if ( m_uBulletNum [ m_uWeaponIndex ][ uIndex ] == 0 )
    {
        return;
    }
        
    m_uBulletNum [ m_uWeaponIndex ][ uIndex ]--;
    
    refreshBulletLabel ( uIndex );
    
    m_pWeapon->setAction ( CWeapon::eActFire, uIndex );    
    
    m_pBullet->fire ( m_pTarget->getTarget ( uIndex ), m_pTarget->getRayLine ( uIndex ), m_pTarget->getTriangle ( uIndex ) );
      
    setVibrate ( KD_TRUE );
}

KDvoid CScrGame::onFired ( CCNode* pSender )
{
    KDuint uIndex = pSender->getTag ( );
    
    pSender->stopAllActions ( );
    
    m_pWeapon->setAction ( CWeapon::eActFired, uIndex );
    
    setVibrate ( KD_FALSE );
}