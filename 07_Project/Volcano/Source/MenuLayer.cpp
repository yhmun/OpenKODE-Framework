/* --------------------------------------------------------------------------
 *
 *      File            MenuLayer.cpp
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com        
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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
#include "MenuLayer.h"
#include "GameLayer.h"
#include "SoundManager.h"

KDbool MenuLayer::init ( KDvoid )
{
    if ( !CCLayer::init ( ) )
    {
        return KD_FALSE;
    }

	// 배경
	this->createBackground ( );

	// 메뉴 및 버튼 표시
	this->scheduleOnce ( schedule_selector ( MenuLayer::createMenuAndBtn ), 0.5f );
	    
    return KD_TRUE;
}

KDvoid MenuLayer::createBackground ( KDvoid )
{
	CCSprite*	pBackground = CCSprite::create ( "Images/title.png" );
	this->addChild( pBackground );
	pBackground->setPositionWithParent ( kCCAlignmentCenter );
}


KDvoid MenuLayer::createMenuAndBtn ( KDfloat fDelta )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	// 메뉴 버튼 ------------------------------------------
	CCSprite*	pMenuBg = CCSprite::create ( "Images/menu_bg.png" );
	pMenuBg->setOpacity ( 0 );
	pMenuBg->setPosition( ccp ( tLyrSize.width / 2, tLyrSize.height / 2 ) );

	CCMenu*		pMenu = CCMenu::create
	(
		CCMenuItemLabel::create ( CCLabelTTF::create ( "Start",      "Fonts/Marker Felt.ttf", 60 ), this, menu_selector ( MenuLayer::onMenuStart ) ),
//		CCMenuItemLabel::create ( CCLabelTTF::create ( "High Score", "Fonts/Marker Felt.ttf", 40 ), this, menu_selector ( MenuLayer::onMenuHigeScore ) ),
		CCMenuItemLabel::create ( CCLabelTTF::create ( "Quit",       "Fonts/Marker Felt.ttf", 40 ), this, menu_selector ( MenuLayer::onMenuQuit ) ),
		KD_NULL
	);
	pMenu->setOpacity ( 0 );
	pMenu->alignItemsVerticallyWithPadding ( 20 );

	const CCSize&	tBgSize = pMenuBg->getContentSize ( );

	pMenu->setPosition ( ccp ( tBgSize.width / 2, tBgSize.height / 2 ) );
	pMenuBg->addChild ( pMenu );

	this->addChild( pMenuBg );

	pMenuBg->runAction ( CCFadeIn::create ( 1.5f ) );
	pMenu->runAction ( CCFadeIn::create ( 1.5f ) );

	// 사운드 ------------------
	CCSize			tSize = ccsz;
	CCPoint			tPos  = ccpz;

	m_pSoundOnMenuItem   = CCMenuItemImage::create ( "Images/soundOn.png",   "Images/soundOn.png",   this, menu_selector ( MenuLayer::soundOnOffMenuCallback ) );
	m_pSoundOffMenuItem  = CCMenuItemImage::create ( "Images/soundOff.png" , "Images/soundOff.png" , this, menu_selector ( MenuLayer::soundOnOffMenuCallback ) );
  
	tSize = m_pSoundOnMenuItem->getContentSize ( );
	tPos  = ccp ( tLyrSize.cx - tSize.cx / 2 - 5, tLyrSize.cy - tSize.cy / 2 - 5 );
    m_pSoundOnMenuItem->setPosition ( tPos );
	m_pSoundOffMenuItem ->setPosition ( tPos );
    
	this->addChild ( CCMenu::create ( m_pSoundOnMenuItem, m_pSoundOffMenuItem, KD_NULL ) );
	
	SoundManager::sharedSoundManager ( )->soundOnOff ( m_pSoundOnMenuItem, m_pSoundOffMenuItem );
}


KDvoid MenuLayer::soundOnOffMenuCallback ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->soundOnOff ( m_pSoundOnMenuItem, m_pSoundOffMenuItem, !m_pSoundOffMenuItem->isVisible ( ) );
}


KDvoid MenuLayer::onMenuStart ( CCObject* pSender )
{
	CCScene*	pScene = CCScene::create ( );
	CCLayer*	pLayer = GameLayer::create ( );
	pScene->addChild ( pLayer );
	CCDirector::sharedDirector ( )->replaceScene ( pScene );
}

KDvoid MenuLayer::onMenuHigeScore ( CCObject* pSender )
{
	CCLOG ( "onMenuHigeScore" );
}

KDvoid MenuLayer::onMenuQuit ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->end ( );
}


