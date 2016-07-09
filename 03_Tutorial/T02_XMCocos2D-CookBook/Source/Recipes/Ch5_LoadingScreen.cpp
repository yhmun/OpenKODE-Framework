/* --------------------------------------------------------------------------
 *
 *      File            Ch5_LoadingScreen.cpp
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
#include "Ch5_LoadingScreen.h"
#include "Libraries/CCJson.h"

KDbool Ch5_LoadingScreen::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// The load level button
	CCMenuItemFont*		pLoadLevelItem = CCMenuItemFont::create ( "Load Level", this, menu_selector ( Ch5_LoadingScreen::loadLevel ) );
	CCMenu*				pMenu = CCMenu::create ( pLoadLevelItem, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pMenu );

	return KD_TRUE;
}

// Callback to load the level
KDvoid Ch5_LoadingScreen::loadLevel ( CCObject* pObject )
{
	CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );

	CCDirector::sharedDirector ( )->pushScene ( LoadingScene::scene ( "level1.json" ) );
}


CCScene* LoadingScene::scene ( const KDchar* szStr )
{
	LoadingScene*	pLayer = new LoadingScene ( );

	if ( pLayer && pLayer->initWithLevel ( szStr ) )
	{
		CCScene*	pScene = CCScene::create ( );
		pScene->addChild ( pLayer );
		pLayer->autorelease ( );

		return pScene;
	}

	CC_SAFE_DELETE ( pLayer );

	return KD_NULL;
}

KDbool LoadingScene::initWithLevel ( const KDchar* szStr )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// Set levelStr
	m_sLevelStr = szStr;

	// Random colored background
	CCSprite*	pBg = CCSprite::create ( "blank.png" );
	pBg->setPosition ( ccp ( 240, 160 ) );
	pBg->setTextureRect ( CCRect ( 0, 0, 480, 320 ) );
	pBg->setColor ( ccc3 ( kdRand ( ) % 150, kdRand ( ) % 150, kdRand ( ) % 150 ) );
	this->addChild ( pBg, 0 );
	
	// Set the initial loading message
	m_pLoadingMessage = CCLabelBMFont::create ( "Loading, Please Wait...0%", "eurostile_30.fnt" );
	m_pLoadingMessage->setPosition ( ccp ( 160, 270 ) );
	m_pLoadingMessage->setScale ( 0.75f );
	m_pLoadingMessage->setColor ( ccc3 ( 255, 255, 255 ) );
	this->addChild ( m_pLoadingMessage, 10 );
	
	// Create an initial '0%' loading bar
	m_pLoadingBar = CCSprite::create ( "blank.png" );
	m_pLoadingBar->setColor ( ccc3 ( 255, 0, 0 ) );
	m_pLoadingBar->setTextureRect ( CCRect ( 0, 0, 10, 25 ) );
	m_pLoadingBar->setPosition ( ccp ( 50, 50 ) );
	m_pLoadingBar->setAnchorPoint ( ccp ( 0, 0 ) );
	this->addChild ( m_pLoadingBar, 10 );
	
	// Start level pre-load 
	this->preloadLevel ( );
		
	return KD_TRUE;
}

// Asynchronously load all required textures
KDvoid LoadingScene::preloadLevel ( KDvoid )
{
	CCDictionary*	pDict  = CCJson::create ( m_sLevelStr.c_str ( ) );
	CCArray*		pNodes = (CCArray*) pDict->objectForKey ( "nodes" );
	CCObject*		pObject;

	m_uNodesLoaded = 0;
	m_uNodesToLoad = pNodes->count ( ); 

	CCARRAY_FOREACH ( pNodes, pObject )
	{
		CCDictionary*	pNode = (CCDictionary*) pObject;

		if ( !kdStrcmp ( ( (CCString*) pNode->objectForKey ( "type" ) )->getCString ( ), "spriteFile" ) )
		{
			this->preloadSpriteFile ( pNode );
		}
	}
}

// Asynchronously load a texture and call the specified callback when finished
KDvoid LoadingScene::preloadSpriteFile ( CCDictionary* pNode )
{
	CCString*	pFile = (CCString*) pNode->objectForKey ( "file" );

	CCLOG ( "Preload %s", pFile->getCString ( ) );

	CCTextureCache::sharedTextureCache ( )->addImageAsync ( pFile->getCString ( ), this, callfuncO_selector ( LoadingScene::nodeLoaded ) );
}

// The loading callback
// This increments nodesLoaded and reloads the indicators accordingly
KDvoid LoadingScene::nodeLoaded ( CCObject* pSender )
{
	m_uNodesLoaded++;

	CCLOG ( "nodeLoaded %d", m_uNodesLoaded );

	KDfloat		fPercentComplete = 100.0f * m_uNodesLoaded / m_uNodesToLoad;

	m_pLoadingMessage->setString ( ccszf ( "Loading, Please Wait...%d%%", (KDint) fPercentComplete ) );

	// When we are 100% complete we run the game
	if ( fPercentComplete >= 100.0f )
	{
		this->runAction
		(
			CCSequence::create
			(
				CCDelayTime::create ( 0.25f ), 
				CCCallFunc::create ( this, callfunc_selector ( LoadingScene::runGame ) ), 
				KD_NULL
			) 
		);
	}

	// Grow the loading bar
	m_pLoadingBar->setTextureRect ( CCRect ( 0, 0, fPercentComplete * 4, 25 ) );
}

// First pop this scene then load the game scene
KDvoid LoadingScene::runGame ( KDvoid )
{
	CCDirector::sharedDirector ( )->popScene ( );
	CCDirector::sharedDirector ( )->pushScene ( GameScene::scene ( "level1.json" ) );
}

CCScene* GameScene::scene ( const KDchar* szStr )
{
	GameScene*	pLayer = new GameScene ( );

	if ( pLayer && pLayer->initWithLevel ( szStr ) )
	{
		CCScene*	pScene = CCScene::create ( );
		pScene->addChild ( pLayer );
		pLayer->autorelease ( );

		return pScene;
	}

	CC_SAFE_DELETE ( pLayer );

	return KD_NULL;
}

KDbool GameScene::initWithLevel ( const KDchar* szStr )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// Load our level
	this->loadLevel ( szStr );

	// Random colored background
	CCSprite*			pBg = CCSprite::create ( "blank.png" );
	pBg->setPosition ( ccp ( 240, 160 ) );
	pBg->setTextureRect ( CCRect ( 0, 0, 480, 320 ) );
	pBg->setColor ( ccc3 ( kdRand ( ) % 150, kdRand ( ) % 150, kdRand ( ) % 150 ) );
	this->addChild ( pBg, -1 );
	
	// Create a label to indicate that this is the loaded level
	CCLabelBMFont*		pLabel = CCLabelBMFont::create ( "The Loaded Level:", "eurostile_30.fnt" );
	pLabel->setPosition ( ccp ( 160, 300 ) );
	pLabel->setScale ( 0.75f );
	pLabel->setColor ( ccc3 ( 255, 255, 255 ) );
	this->addChild ( pLabel, 10 );
	
	// Quit button
	CCMenuItemFont*		pQuitItem = CCMenuItemFont::create ( "Quit", this, menu_selector ( GameScene::quit ) );
	CCMenu*				pMenu = CCMenu::create ( pQuitItem, KD_NULL );
	pMenu->setPosition ( ccp ( 350, 300 ) );
	this->addChild ( pMenu, 10 );

	return KD_TRUE;
}
 
KDvoid GameScene::loadLevel ( const KDchar* szStr )
{
	CCDictionary*	pDict  = CCJson::create ( szStr );
	CCArray*		pNodes = (CCArray*) pDict->objectForKey ( "nodes" );
	CCObject*		pObject;
	CCARRAY_FOREACH ( pNodes, pObject )
	{
		CCDictionary*	pNode = (CCDictionary*) pObject;

		if ( !kdStrcmp ( ( (CCString*) pNode->objectForKey ( "type" ) )->getCString ( ), "spriteFile" ) )
		{
			this->processSpriteFile ( pNode );
		}
	}
}

KDvoid GameScene::processSpriteFile ( CCDictionary* pNode )
{
	// Init the sprite
	CCString*	pFile = (CCString*) pNode->objectForKey ( "file" );
	CCSprite*	pSprite = CCSprite::create ( pFile->getCString ( ) );
	
	// Set sprite position
	pSprite->setPosition ( ccp ( kdRand ( ) % 480, kdRand ( ) % 200 ) );
	
	// Each numeric value is an NSString or NSNumber that must be cast into a float
	pFile = (CCString*) pNode->objectForKey ( "scale" );
	if ( pFile )
	{
		pSprite->setScale ( pFile->floatValue ( ) );
	}
	
	// Set the anchor point so objects are positioned from the bottom-up
	pSprite->setAnchorPoint ( ccp ( 0.5f, 0 ) );
		
	// Finally, add the sprite
	this->addChild ( pSprite, 2 );
}

// Quit callback
KDvoid GameScene::quit ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->popScene ( );
}