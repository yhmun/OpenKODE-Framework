/* --------------------------------------------------------------------------
 *
 *      File            SettingsLayer.cpp
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
#include "SettingsLayer.h"
#include "SysMenu.h"

KDbool SettingsLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pLoading = CCSprite::create ( PATH_IMAGE_LOADING );
	pLoading->setAnchorPoint ( CCPointZero );
	this->addChild ( pLoading );

	CCSprite*  pTitle = CCSprite::create ( PATH_IMAGE_MENU_TITLE, CCRect ( 0, 0, 134, 34 ) );
	pTitle->setPosition ( ccp ( tWinSize.cx / 2, tWinSize.cy - 120 ) );
	this->addChild ( pTitle );

	CCMenuItemFont::setFontName ( PATH_FONT_DEFAULT );

	CCMenuItemFont::setFontSize ( 18 );
	CCMenuItemFont*  pTitle1 = CCMenuItemFont::create ( "Sound" );
	pTitle1->setEnabled ( KD_FALSE );

	CCMenuItemFont::setFontSize ( 26 );
	CCMenuItemToggle*  pItem1 = CCMenuItemToggle::createWithTarget 
	(
		this, menu_selector ( SettingsLayer::onSound ), 
		CCMenuItemFont::create ( "On" ), 
		CCMenuItemFont::create ( "Off" ),
		KD_NULL
	);

	CCMenuItemFont::setFontSize ( 18 );
	CCMenuItemFont*  pTitle2 = CCMenuItemFont::create ( "Mode" );
	pTitle2->setEnabled ( KD_FALSE );

	CCMenuItemFont::setFontSize ( 26 );
	CCMenuItemToggle*  pItem2 = CCMenuItemToggle::createWithTarget 
	(
		this, menu_selector ( SettingsLayer::onMode ), 
		CCMenuItemFont::create ( "Easy" ), 
		CCMenuItemFont::create ( "Normal" ),
		CCMenuItemFont::create ( "Hard" ),
		KD_NULL
	);

	CCMenuItemLabel*  pBack = CCMenuItemLabel::create 
	(
		CCLabelTTF::create ( "Go back", PATH_FONT_DEFAULT, 14 ),
		this, menu_selector ( SettingsLayer::onBack )
	);

	CCMenu*  pMenu = CCMenu::create ( pTitle1, pTitle2, pItem1, pItem2, pBack, KD_NULL );
	pMenu->setPosition ( ccpMid ( tWinSize ) );
	pMenu->alignItemsInColumns ( 2, 2, 1, KD_NULL );
	this->addChild ( pMenu );

	pBack->setPosition ( ccp ( pBack->getPosition ( ).x, pBack->getPosition ( ).y - 50 ) );

	return KD_TRUE;
}

KDvoid SettingsLayer::onSound ( CCObject* pSender )
{
	Global::setSound ( !Global::bSound );
}

KDvoid SettingsLayer::onMode ( CCObject* pSender )
{

}

KDvoid SettingsLayer::onBack ( CCObject* pSender )
{
	Global::playEffect ( PATH_SOUND_BUTTON );

	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( SysMenu::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.2f, pScene ) );
}
