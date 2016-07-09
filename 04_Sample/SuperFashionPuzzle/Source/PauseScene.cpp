/* --------------------------------------------------------------------------
 *
 *      File            PauseScene.cpp
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
#include "PauseScene.h"
#include "MenuMainScene.h"
#include "OptionsScene.h"
#include "PlayingScene.h"
#include "HowToPlayScene.h"

KDbool PauseScene::init ( KDvoid )
{
	if ( !CCScene::init ( ) )
	{
		return KD_FALSE;
	}

	// background
	CCSprite*		pBackground = CCSprite::create ( "shared_pink_background.pvr" );
	pBackground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pBackground );

	// spot_lights sprite
	CCSprite*		pSpotLightsSprite = CCSprite::create ( "shared_spot_lights.png" );
	pSpotLightsSprite->setScale ( 2 );
	pSpotLightsSprite->setPosition ( ccp ( 400, 360 ) );
	pSpotLightsSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( BACKGROUND_ROTATION_TIME, -360 ) ) );
	this->addChild ( pSpotLightsSprite );

	// foreground
	CCSprite*		pForeground = CCSprite::create ( "menu_pause_foreground.png" );
	pForeground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pForeground );

	// resume button
	CCMenuItem*		pResumeItem = CCMenuItemImage::create 
	(
		"menu_pause_resume_button_released.png",
		"menu_pause_resume_button_pressed.png", 
		this,
		menu_selector ( PauseScene::resumeButtonPressed )
	);
    pResumeItem->setPosition ( ccp ( 240, 211 ) );
	this->addChild ( CCMenu::createWithItem ( pResumeItem ) );

	// how to play button
	CCMenuItem*		pHowToPlayItem = CCMenuItemImage::create 
	(
		"menu_pause_how_to_play_button_released.png",
		"menu_pause_button_how_to_play_pressed.png", 
		this,
		menu_selector ( PauseScene::howToPlayButtonPressed )
	);
    pHowToPlayItem->setPosition ( ccp ( 240, 156 ) );
	this->addChild ( CCMenu::createWithItem ( pHowToPlayItem ) );

	// options button
	CCMenuItem*		pOptionsItem = CCMenuItemImage::create 
	(
		"menu_pause_options_button_released.png",
		"menu_pause_options_button_pressed.png", 
		this,
		menu_selector ( PauseScene::optionsButtonPressed )
	);
    pOptionsItem->setPosition ( ccp ( 240, 101 ) );
	this->addChild ( CCMenu::createWithItem ( pOptionsItem ) );

	// menu button
	CCMenuItem*		pMenuItem = CCMenuItemImage::create 
	(
		"menu_pause_menu_button_released.png",
		"menu_pause_menu_button_pressed.png", 
		this,
		menu_selector ( PauseScene::menuButtonPressed )
	);
    pMenuItem->setPosition ( ccp ( 240, 43 ) );
	this->addChild ( CCMenu::createWithItem ( pMenuItem ) );

	m_pPlayingScene = KD_NULL;

	return KD_TRUE;
}

KDvoid PauseScene::resumeButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->popScene ( );
	m_pPlayingScene->onResume ( );
}

KDvoid PauseScene::optionsButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	OptionsScene*	pOptionsScene = OptionsScene::create ( );
	pOptionsScene->setPopAtExit ( );
	CCDirector::sharedDirector ( )->pushScene ( CCTransitionFade::create ( TRANSITION_DURATION, pOptionsScene, ccWHITE ) );	
}

KDvoid PauseScene::menuButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->popScene ( );	// remove pause scene and replace playing scene
	
	SoundManager::sharedSoundManager ( )->playSoundMusicMenu ( );
	CCDirector::sharedDirector ( )->pushScene ( CCTransitionFade::create ( TRANSITION_DURATION, MenuMainScene::create ( ), ccWHITE ) );	
}	

KDvoid PauseScene::howToPlayButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );

	HowToPlayScene*		pHowToPlayScene = HowToPlayScene::create ( );
	pHowToPlayScene->setPopAtExit ( );
	CCDirector::sharedDirector ( )->pushScene ( CCTransitionFade::create ( TRANSITION_DURATION, pHowToPlayScene, ccWHITE ) );	
}

KDvoid PauseScene::setPlayingScene ( PlayingScene* pScene )
{
	m_pPlayingScene = pScene;
}
