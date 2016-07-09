/* --------------------------------------------------------------------------
 *
 *      File            SysMenu.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors    
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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
#include "SysMenu.h"
#include "GameLayer.h"
#include "AboutLayer.h"
#include "SettingsLayer.h"
#include "Effect.h"

KDbool SysMenu::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pLoading = CCSprite::create ( PATH_IMAGE_LOADING );
	pLoading->setAnchorPoint ( CCPointZero );
	this->addChild ( pLoading );

	CCSprite*  pLogo = CCSprite::create ( PATH_IMAGE_LOGO );
	pLogo->setAnchorPoint ( CCPointZero );
	pLogo->setPosition ( ccp ( 0, 250 ) );
	this->addChild ( pLogo, 10 );

	CCMenuItemSprite*  pNewGame = CCMenuItemSprite::create
	(
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 0, 0, 126, 33 ) ),
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 0, 33, 126, 33 ) ),
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 0, 33 * 2, 126, 33 ) ),
		this, menu_selector ( SysMenu::onFlareEffect )
	);

	CCMenuItemSprite*  pSetting = CCMenuItemSprite::create
	(
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 126, 0, 126, 33 ) ),
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 126, 33, 126, 33 ) ),
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 126, 33 * 2, 126, 33 ) ),
		this, menu_selector ( SysMenu::onSetting )
	);

	CCMenuItemSprite*  pAbout = CCMenuItemSprite::create
	(
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 252, 0, 126, 33 ) ),
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 252, 33, 126, 33 ) ),
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 252, 33 * 2, 126, 33 ) ),
		this, menu_selector ( SysMenu::onAbout )
	);
 
	CCMenu*  pMenu = CCMenu::create ( pNewGame, pSetting, pAbout, KD_NULL );
	pMenu->setPosition ( ccp ( tWinSize.cx / 2, tWinSize.cy / 2 - 80 ) );
	pMenu->alignItemsVerticallyWithPadding ( 10 );
	this->addChild ( pMenu, 1 );

	m_pShip = CCSprite::create ( PATH_IMAGE_SHIP, CCRect ( 0, 45, 60, 38 ) );
	m_pShip->setPosition ( ccp ( CCRANDOM_0_1 ( ) * tWinSize.cx, 0 ) );
	m_pShip->runAction ( CCMoveBy::create ( 2, ccp ( CCRANDOM_0_1 ( ) * tWinSize.cx, m_pShip->getPosition ( ).y + tWinSize.cy + 100 ) ) );
	this->addChild ( m_pShip );

	this->schedule ( schedule_selector ( SysMenu::onUpdate ), 0.1f );

	Global::playBGM ( PATH_MUSIC_MAIN );

	return KD_TRUE;
}

KDvoid SysMenu::onFlareEffect ( CCObject* pSender )
{
	flareEffect ( this, this, callfuncN_selector ( SysMenu::onNewGame ) );
}

KDvoid SysMenu::onNewGame ( CCNode* pSender )
{
	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( GameLayer::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.2f, pScene ) );
}

KDvoid SysMenu::onSetting ( CCObject* pSender )
{
	Global::playEffect ( PATH_SOUND_BUTTON );

	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( SettingsLayer::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.2f, pScene ) );
}

KDvoid SysMenu::onAbout ( CCObject* pSender )
{
	Global::playEffect ( PATH_SOUND_BUTTON );

	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( AboutLayer::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.2f, pScene ) );
}

KDvoid SysMenu::onUpdate ( KDfloat fDelta )
{
	if ( m_pShip->getPosition ( ).y > 480 )
	{
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

		m_pShip->setPosition ( ccp ( CCRANDOM_0_1 ( ) * tWinSize.cx, 10 ) );
		m_pShip->runAction ( CCMoveBy::create ( CCRANDOM_0_1 ( ) * 5, ccp ( CCRANDOM_0_1 ( ) * tWinSize.cx, m_pShip->getPosition ( ).y + 480 ) ) );
	}
}
