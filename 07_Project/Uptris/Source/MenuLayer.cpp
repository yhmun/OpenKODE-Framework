/* --------------------------------------------------------------------------
 *
 *      File            MenuLayer.cpp
 *      Created By      Project B team
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2013 XMsoft. All rights reserved.
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
#include "MyRanking.h"

KDbool MenuLayer::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&	tLyrSize = this->getContentSize ( );

	CCSprite*	pBg = CCSprite::create ( "Images/menu_bg.png" );
	pBg->setAnchorPoint ( ccpz );
	this->addChild ( pBg );

	CCMenu*		pMenu = CCMenu::create ( );
	this->initMenuButton ( pMenu, "GameStart", 0 );
	this->initMenuButton ( pMenu, "Score", 1 );
	this->initMenuButton ( pMenu, "Exit", 2 );

	pMenu->alignItemsVertically ( 10 );
	pMenu->setPosition ( ccp ( tLyrSize.cx / 2 , tLyrSize.cy / 2 - 60 ) );
	this->addChild ( pMenu );

	return KD_TRUE;
}

KDvoid MenuLayer::onButtonClick ( CCObject*	pSender )
{
	CCMenuItemImage*	pItem = ( CCMenuItemImage* ) pSender;
	
	CCScene*	pScene = CCScene::create ( );
	CCLayer*	pLayer;

	switch ( pItem->getTag ( ) )
	{
		case 0 :
			CCLOG ( "GAMESTART" );
			pLayer = GameLayer::create ( );
			pScene->addChild ( pLayer );
			
			CCDirector::sharedDirector ( )->replaceScene ( CCTransitionTurnOffTiles::create ( 1.0f, pScene ) );
			break;

		case 1 :
			CCLOG ( "SCORE" );
			pLayer = MyRanking::create ( );
			pScene->addChild ( pLayer );
			
			CCDirector::sharedDirector ( )->replaceScene ( pScene );
			break;

		case 2 :
			CCLOG ( "EXIT" );
			CCDirector::sharedDirector ( )->end ( );
			break;
		
		default :
			break;
	}
}

KDvoid MenuLayer::initMenuButton ( CCMenu* pMenu, const KDchar* name, KDint tag )
{
	CCMenuItemImage*  pItem = CCMenuItemImage::create 
	(
		"Images/main_menu_button_bg_nomal.png", 
		"Images/main_menu_button_bg_press.png",
		this,
		menu_selector ( MenuLayer::onButtonClick )
	);

	pItem->setTag ( tag );

	pMenu->addChild ( pItem, tag );

	CCLabelTTF*		pLabel = CCLabelTTF::create ( name, "Fonts/NanumGothicBold.ttf", 26 );
	pLabel->setPosition ( ccpMid ( pItem->getContentSize ( ) ) );
	pLabel->setColor ( ccBLACK );
	pItem->addChild ( pLabel );
}