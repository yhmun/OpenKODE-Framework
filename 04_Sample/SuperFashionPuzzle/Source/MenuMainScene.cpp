/* --------------------------------------------------------------------------
 *
 *      File            MenuMainScene.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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
#include "MenuMainScene.h"
#include "HowToPlayScene.h"
#include "ScoresScene.h"
#include "OptionsScene.h"
#include "CreditsScene.h"
#include "LoadingGameScene.h"

KDbool MenuMainScene::init ( KDvoid )
{
	if ( !CCScene::init ( ) )
	{
		return KD_FALSE;
	}

	// background
	CCSprite*		pBackground = CCSprite::create ( "shared_blue_background.pvr" );
	pBackground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pBackground );

	// sun_lights texture used by 2 sprites
	CCTexture2D*	pSunLightsTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "shared_sun_lights.png" );			
	
	// 2 sprites that use the same texture
	CCSprite*		pSunLightClockWiseSprite = CCSprite::createWithTexture ( pSunLightsTexture );
	pSunLightClockWiseSprite->setScale ( 2 );
	pSunLightClockWiseSprite->setPosition ( ccp ( 240, 0 ) );
	pSunLightClockWiseSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( BACKGROUND_MENU_ROTATION_TIME, 360 ) ) );
	this->addChild ( pSunLightClockWiseSprite );

	// the other one
	CCSprite*		pSunLightCounterClockWiseSprite = CCSprite::createWithTexture ( pSunLightsTexture );
	pSunLightCounterClockWiseSprite->setScale ( 2 );
	pSunLightCounterClockWiseSprite->setPosition ( ccp ( 240, 0 ) );
	pSunLightCounterClockWiseSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( BACKGROUND_MENU_ROTATION_TIME, -360 ) ) );
	this->addChild ( pSunLightCounterClockWiseSprite );

	// arc sprite
	CCSprite*		pArc = CCSprite::create ( "menu_main_light_arch.pvr" );
	pArc->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pArc );
	
	// how to play button
	CCMenuItem*		pHowToPlayItem = CCMenuItemImage::create 
	(
		"menu_main_how_to_play_button_released.png",
		"menu_main_how_to_play_button_pressed.png", 
		this,
		menu_selector ( MenuMainScene::howToPlayButtonPressed )
	);
    pHowToPlayItem->setPosition ( ccp ( 240, 72 ) );
	this->addChild ( CCMenu::createWithItem ( pHowToPlayItem ) );

	// play button
	CCMenuItem*		pPlayItem = CCMenuItemImage::create 
	(
		"menu_main_play_button_released.png",
		"menu_main_play_button_pressed.png", 
		this,
		menu_selector ( MenuMainScene::playButtonPressed )
	);
    pPlayItem->setPosition ( ccp ( 240, 110 ) );
	this->addChild ( CCMenu::createWithItem ( pPlayItem ) );

	// scores button
	CCMenuItem*		pScoresItem = CCMenuItemImage::create 
	(
		"menu_main_scores_button_released.png",
		"menu_main_scores_button_pressed.png", 
		this,
		menu_selector ( MenuMainScene::scoresButtonPressed )
	);
    pScoresItem->setPosition ( ccp ( 240, 30 ) );
	this->addChild ( CCMenu::createWithItem ( pScoresItem ) );

	// options button
	CCMenuItem*		pOptionsItem = CCMenuItemImage::create 
	(
		"menu_main_options_button_released.png",
		"menu_main_options_button_pressed.png", 
		this,
		menu_selector ( MenuMainScene::optionsButtonPressed )
	);
    pOptionsItem->setPosition ( ccp ( 142, 30 ) );
	this->addChild ( CCMenu::createWithItem ( pOptionsItem ) );

	// credits button
	CCMenuItem*		pCreditsItem = CCMenuItemImage::create 
	(
		"menu_main_credits_button_released.png",
		"menu_main_credits_button_pressed.png", 
		this,
		menu_selector ( MenuMainScene::creditsButtonPressed )
	);
    pCreditsItem->setPosition ( ccp ( 334, 30 ) );
	this->addChild ( CCMenu::createWithItem ( pCreditsItem ) );

	CCSprite*		pSuperFashionPuzzle = CCSprite::create ( "menu_main_super_fashion_puzzle.png" );
	pSuperFashionPuzzle->setPosition ( ccp ( 240, 205 ) );
	this->addChild ( pSuperFashionPuzzle );

	return KD_TRUE;
}

KDvoid MenuMainScene::howToPlayButtonPressed ( CCObject* pSender )
{
	CCLOG ( "howToPlayButtonPressed" );

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, HowToPlayScene::create ( ), ccWHITE ) );
}

KDvoid MenuMainScene::playButtonPressed ( CCObject* pSender )
{
	CCLOG ( "playButtonPressed" );

	// increment number of played games
	KDint		nGameWasStarted = CCUserDefault::sharedUserDefault ( )->getIntegerForKey ( "gameWasStarted" );
	nGameWasStarted++;

	CCUserDefault::sharedUserDefault ( )->setIntegerForKey ( "gameWasStarted", nGameWasStarted );
	CCUserDefault::sharedUserDefault ( )->flush ( );

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	SoundManager::sharedSoundManager ( )->stopBackgroundMusic ( );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, LoadingGameScene::create ( ), ccWHITE ) );
}

KDvoid MenuMainScene::optionsButtonPressed ( CCObject* pSender )
{
	CCLOG ( "optionsButtonPressed" );

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, OptionsScene::create ( ), ccWHITE ) );
}

KDvoid MenuMainScene::scoresButtonPressed ( CCObject* pSender )
{
	CCLOG ( "scoresButtonPressed" );

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, ScoresScene::create ( ), ccWHITE ) );
}

KDvoid MenuMainScene::creditsButtonPressed ( CCObject* pSender )
{
	CCLOG ( "creditsButtonPressed" );

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, CreditsScene::create ( ), ccWHITE ) );
}
