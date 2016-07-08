/* --------------------------------------------------------------------------
 *
 *      File            TestScene.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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
#include "TestScene.h"

//------------------------------------------------------------------
//
// TestScene
//
//------------------------------------------------------------------
enum
{
	MID_PUSHSCENE       = 100 ,
	MID_PUSHSCENETRAN         ,
	MID_QUIT                  ,
	MID_REPLACESCENE          ,
	MID_REPLACESCENETRAN      ,
	MID_GOBACK
};

KDbool TestScene::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCSprite*    pSprite = CCSprite::create ( "Images/grossini.png" );	
	pSprite->setPosition ( ccp ( tLyrSize.cx - pSprite->getContentSize ( ).cx / 2 - 10, tLyrSize.cy / 2 - 50 ) );
	pSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 2, 360 ) ) );
	this->addChild ( pSprite, 1 );

	CCMenuItemFont::setFontSize ( 28 );
	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );

	CCMenu*  pMenu = CCMenu::create
	(
		CCMenuItemFont::create ( "Test pushScene"				, this, menu_selector ( TestScene::onPushScene ) ),
		CCMenuItemFont::create ( "Test pushScene w/transition"	, this, menu_selector ( TestScene::onPushSceneTran ) ),
		CCMenuItemFont::create ( "Quit"							, this, menu_selector ( TestScene::onQuit ) ),
		KD_NULL
	);
	pMenu->setPosition ( ccpMid ( tLyrSize ) );
	pMenu->alignItemsVertically ( );	
	this->addChild ( pMenu );	

	return KD_TRUE;
}

const KDchar* TestScene::subtitle ( KDvoid )
{
	return "Scene 1";
}

KDvoid TestScene::onPushScene ( CCObject* pSender )
{
	CCScene*   pScene = CCScene::create ( );
	pScene->addChild ( TestScene2::create ( ) );
	CCDirector::sharedDirector ( )->pushScene ( pScene );
}

KDvoid TestScene::onPushSceneTran ( CCObject* pSender )
{
	CCScene*   pScene = CCScene::create ( );
	pScene->addChild ( TestScene2::create ( ) );
	CCDirector::sharedDirector ( )->pushScene ( CCTransitionSlideInT::create ( 1, pScene ) );
}

KDvoid TestScene::onQuit ( CCObject* pSender )
{

}

//------------------------------------------------------------------
//
// TestScene2
//
//------------------------------------------------------------------
KDbool TestScene2::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	
	CCSprite*    pSprite = CCSprite::create ( "Images/grossini.png" );	
	pSprite->setPosition ( ccp ( tLyrSize.cx - pSprite->getContentSize ( ).cx / 2 - 10, tLyrSize.cy / 2 - 50 ) );
	pSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 2, 360 ) ) );
	this->addChild ( pSprite, 1 );

	CCMenu*  pMenu = CCMenu::create 
	(
		CCMenuItemFont::create ( "replaceScene"				, this, menu_selector ( TestScene2::onReplaceScene ) ),
		CCMenuItemFont::create ( "replaceScene w/transition", this, menu_selector ( TestScene2::onReplaceSceneTran ) ),
		CCMenuItemFont::create ( "Go Back"					, this, menu_selector ( TestScene2::onGoBack ) ),
		KD_NULL
	);
	pMenu->setPosition ( ccpMid ( tLyrSize ) );
	pMenu->alignItemsVertically ( );	
	this->addChild ( pMenu );

	return KD_TRUE;
}

const KDchar* TestScene2::subtitle ( KDvoid )
{
	return "Scene 2";
}

KDvoid TestScene2::onGoBack ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->popScene ( );
}

KDvoid TestScene2::onReplaceScene ( CCObject* pSender )
{
	CCScene*   pScene = CCScene::create ( );
	pScene->addChild ( TestScene3::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( pScene );
}

KDvoid TestScene2::onReplaceSceneTran ( CCObject* pSender )
{
	CCScene*   pScene = CCScene::create ( );
	pScene->addChild ( TestScene3::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFlipX::create ( 2, pScene ) );
}

//------------------------------------------------------------------
//
// TestScene3
//
//------------------------------------------------------------------
KDbool TestScene3::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&   tLyrSize = this->getContentSize ( );

	CCSprite*    pSprite = CCSprite::create ( "Images/grossini.png" );	
	pSprite->setPosition ( ccp ( tLyrSize.cx - pSprite->getContentSize ( ).cx / 2 - 10, tLyrSize.cy / 2 - 50 ) );
	pSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 2, 360 ) ) );
	this->addChild ( pSprite, 1 );

	CCMenu*   pMenu = CCMenu::create 
	(
		CCMenuItemFont::create ( "Touch to pushScene (self)", this, menu_selector ( TestScene3::onPushScene ) ),
		CCMenuItemFont::create ( "Touch to popScene"		, this, menu_selector ( TestScene3::onPopScene ) ),
		CCMenuItemFont::create ( "Touch to popToRootScene"	, this, menu_selector ( TestScene3::onPopToRootScene ) ),
		KD_NULL
	);
	pMenu->setPosition ( ccpMid ( tLyrSize ) );
	pMenu->alignItemsVertically ( );	
	this->addChild ( pMenu );

	return KD_TRUE;
}

const KDchar* TestScene3::subtitle ( KDvoid )
{
	return "Scene 3";
}

KDvoid TestScene3::onPushScene ( CCObject* pSender )
{
    CCScene*  pScene = CCScene::create ( );
    pScene->addChild ( TestScene3::create ( ) );
    CCDirector::sharedDirector ( )->pushScene ( CCTransitionFade::create ( 0.5, pScene, ccc3 ( 0, 255, 255 ) ) );
}

KDvoid TestScene3::onPopScene ( CCObject* pSender )
{	
	CCDirector::sharedDirector ( )->popScene ( );
}

KDvoid TestScene3::onPopToRootScene ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->popToRootScene ( );
}