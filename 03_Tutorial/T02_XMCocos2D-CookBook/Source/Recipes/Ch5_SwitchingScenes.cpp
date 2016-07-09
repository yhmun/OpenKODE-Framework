/* --------------------------------------------------------------------------
 *
 *      File            Ch5_SwitchingScenes.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Ch5_SwitchingScenes.h"

KDbool Ch5_SwitchingScenes::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Font size/name
	CCMenuItemFont::setFontSize ( 30 );
	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );

	// Go to our initial scene
	CCMenuItemFont*		pGoToScene1 = CCMenuItemFont::create ( "Go To Scene 1", this, menu_selector ( Ch5_SwitchingScenes::goToScene1 ) );
	CCMenu*				pMenu = CCMenu::create ( pGoToScene1, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	pMenu->alignItemsVertically ( );
	this->addChild ( pMenu );

	return KD_TRUE;
}

// Push initial scene
KDvoid Ch5_SwitchingScenes::goToScene1 ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->pushScene ( TreeSceneMenu::sceneWithString ( "1" ) );
}

CCScene* TreeSceneMenu::sceneWithString ( const KDchar* szStr )
{
	TreeSceneMenu*	pLayer = new TreeSceneMenu ( );

	if ( pLayer && pLayer->initWithString ( szStr ) )
	{
		CCScene*	pScene = CCScene::create ( );
		pScene->addChild ( pLayer );
		pLayer->autorelease ( );

		return pScene;
	}

	CC_SAFE_DELETE ( pLayer );

	return KD_NULL;
}

KDbool TreeSceneMenu::initWithString ( const KDchar* szStr )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// Set scene name
	m_sName = szStr;

	// Random background color
	CCSprite*	pBG = CCSprite::create ( "blank.png" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	pBG->setTextureRect ( CCRect ( 0, 0, 480, 320 ) );
	pBG->setColor ( ccc3 ( kdRand ( ) % 150, kdRand ( ) % 150, kdRand ( ) % 150 ) );
	this->addChild ( pBG );

	// Buttons to push new scenes onto the stack
	CCMenuItemFont*		pScene1Item = CCMenuItemFont::create ( ccszf ( "Scene %s.1", szStr ), this, menu_selector ( TreeSceneMenu::goToScene1 ) );
	CCMenuItemFont*		pScene2Item = CCMenuItemFont::create ( ccszf ( "Scene %s.2", szStr ), this, menu_selector ( TreeSceneMenu::goToScene2 ) );

	// If we are at the root we "Quit" instead of going "Back"
	const KDchar*		szBackStr = "Back";
	if ( !kdStrcmp ( szStr, "1" ) )
	{
		szBackStr = "Quit";
	}
	
	CCMenuItemFont*		pBackItem = CCMenuItemFont::create ( szBackStr, this, menu_selector ( TreeSceneMenu::back ) );

	CCMenu*				pMenu = CCMenu::create ( pScene1Item, pScene2Item, pBackItem, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	pMenu->alignItemsVertically ( );
	this->addChild ( pMenu );

	return KD_TRUE;
}

// Push scene 1
KDvoid TreeSceneMenu::goToScene1 ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->pushScene ( TreeSceneMenu::sceneWithString ( ccszf ( "%s.1", m_sName.c_str ( ) ) ) );
}

// Push scene 2
KDvoid TreeSceneMenu::goToScene2 ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->pushScene ( TreeSceneMenu::sceneWithString ( ccszf ( "%s.2", m_sName.c_str ( ) ) ) );
}

// Pop scene
KDvoid TreeSceneMenu::back ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->popScene (  );
}