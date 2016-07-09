/* --------------------------------------------------------------------------
 *
 *      File            GameOverScene.cpp
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
#include "GameOverScene.h"
#include "MenuMainScene.h"
#include "PlayingScene.h"

#define DEFAULT_NAME								"(Press rename)"

// every dialog has a tag in order to identify it when delegate is called
#define CONFIRM_MENU_WO_SUBMITTING_DIALOG			1
#define CONFIRM_PLAY_AGAIN_WO_SUBMITTING_DIALOG		2
#define GAME_OVER_FINISHED_BY_FREE_VERSION_DIALOG	3
#define ASK_FOR_FEEDBACK_DIALOG						4
#define SCORE_SENT_SUCCESSFULLY_DIALOG				5
#define SCORE_SENDING_FAILED_DIALOG					6
#define SCORE_ALREADY_SENT_DIALOG					7
#define WRITING_ON_WALL_FAILED_DIALOG				8


KDbool GameOverScene::init ( KDvoid )
{
	if ( !CCScene::init ( ) )
	{
		return KD_FALSE;
	}

	// background
	CCSprite*	pBackground = CCSprite::create ( "shared_pink_background.pvr" );
	pBackground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pBackground );

	// spot_lights sprite
	CCSprite*	pSpotLightsSprite = CCSprite::create ( "shared_spot_lights.png" );
	pSpotLightsSprite->setScale ( 2 );
	pSpotLightsSprite->setPosition ( ccp ( 400, 360 ) );
	pSpotLightsSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( BACKGROUND_ROTATION_TIME, -360 ) ) );
	this->addChild ( pSpotLightsSprite );

	// game over label sprite
	CCSprite*	pGameOverLabel = CCSprite::create ( "game_over_label.png" );
	pGameOverLabel->setPosition ( ccp ( 240, 250 ) );
	this->addChild ( pGameOverLabel );

	// game over text sprite
	CCSprite*	pGameOverText = CCSprite::create ( "menu_game_over_text.png" );
	pGameOverText->setPosition ( ccp ( 145, 145 ) );
	this->addChild ( pGameOverText );

	// play again button
	CCMenuItem*		pPlayAgainItem = CCMenuItemImage::create 
	(
		"menu_game_over_play_again_button_released.png",
		"menu_game_over_play_again_button_pressed.png", 
		this,
		menu_selector ( GameOverScene::playAgainButtonPressed )
	);
    pPlayAgainItem->setPosition ( ccp ( 62, 35 ) );
	this->addChild ( CCMenu::createWithItem ( pPlayAgainItem ) );

	// rename button
	CCMenuItem*		pRenameItem = CCMenuItemImage::create 
	(
		"menu_game_over_rename_button_released.png",
		"menu_game_over_rename_button_pressed.png", 
		this,
		menu_selector ( GameOverScene::renameButtonPressed )
	);
    pRenameItem->setPosition ( ccp ( 180, 35 ) );
	this->addChild ( CCMenu::createWithItem ( pRenameItem ) );

	// submit button
	CCMenuItem*		pSubmitItem = CCMenuItemImage::create 
	(
		"menu_game_over_submit_button_released.png",
		"menu_game_over_submit_button_pressed.png", 
		this,
		menu_selector ( GameOverScene::submitButtonPressed )
	);
    pSubmitItem->setPosition ( ccp ( 298, 35 ) );
	this->addChild ( CCMenu::createWithItem ( pSubmitItem ) );

	// main menu button
	CCMenuItem*		pMenuItem = CCMenuItemImage::create 
	(
		"menu_game_over_menu_button_released.png",
		"menu_game_over_menu_button_pressed.png", 
		this,
		menu_selector ( GameOverScene::menuButtonPressed )
	);
    pMenuItem->setPosition ( ccp ( 417, 35 ) );
	this->addChild ( CCMenu::createWithItem ( pMenuItem ) );

	// local high score label
	m_pHighScoreLabel = UIntegerLabel::create ( );
	m_pHighScoreLabel->setUInteger ( 0 );
	m_pHighScoreLabel->setPosition ( ccp ( 256, 111 ) );
	this->addChild ( m_pHighScoreLabel );

	// name label
	m_pNameLabel = StringLabel::create ( );
	m_pNameLabel->setPosition ( ccp ( 170, 187 ) );
	m_pNameLabel->setString ( CCUserDefault::sharedUserDefault ( )->getStringForKey ( "playerName", DEFAULT_NAME ).c_str ( ) );
	this->addChild ( m_pNameLabel );
	
	// current score label
	m_pCurrentScoreLabel = UIntegerLabel::create ( );
	m_pCurrentScoreLabel->setUInteger ( 0 );
	m_pCurrentScoreLabel->setPosition ( ccp ( 217, 149 ) );	
	this->addChild ( m_pCurrentScoreLabel );

	#ifdef FREE_VERSION
	CCMenuItem*		pBuyFullVersionItem = CCMenuItemImage::create 
	(
		"buy_full_version_label.png",
		"buy_full_version_label.png", 
		this,
		menu_selector ( GameOverScene::buyFullVersionButtonPressed )
	);
    pBuyFullVersionItem->setPosition ( ccp ( 480 - 56.5, 320 - 56.5 ) );
	this->addChild ( CCMenu::createWithItem ( pBuyFullVersionItem ) );
	#endif

	// login/logut butttons
	KDbool			bIsConnected = KD_FALSE; 
	const KDint		nLoginX = 417, nLoginY = 86;

	// login button
	CCMenuItem*		pLoginItem = CCMenuItemImage::create 
	(
		"facebook_login_released.png",
		"facebook_login_pressed.png", 
		this,
		menu_selector ( GameOverScene::loginButtonPressed )
	);
    pLoginItem->setPosition ( ccp ( nLoginX, nLoginY ) );
	m_pLoginMenu = CCMenu::createWithItem ( pLoginItem );
	m_pLoginMenu->setVisible ( !bIsConnected );
	this->addChild ( m_pLoginMenu );

	// logout button
	CCMenuItem*		pLogoutItem = CCMenuItemImage::create 
	(
		"facebook_logout_released.png",
		"facebook_logout_pressed.png", 
		this,
		menu_selector ( GameOverScene::logoutButtonPressed )
	);
    pLogoutItem->setPosition ( ccp ( nLoginX, nLoginY ) );
	m_pLogoutMenu = CCMenu::createWithItem ( pLogoutItem );
	m_pLogoutMenu->setVisible ( bIsConnected );
	this->addChild ( m_pLogoutMenu );

	// waiting view used when submitting a score
//	CCRect	tRect = CCRect ( 0, 0, 480, 320 );

	// init vars
	m_pPlayingScene = KD_NULL;
	m_bScoreWasSentProperly = KD_FALSE;
	m_bFacebookWallWasWrittenProperly = KD_FALSE;
	m_bPlayedTooMuch = KD_FALSE;

	return KD_TRUE;
}

KDvoid GameOverScene::tryToUploadScore ( KDvoid )
{

}

KDvoid GameOverScene::goToMainMenu ( KDvoid )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->popScene ( );				// remove gameover scene and replace playing scene

	SoundManager::sharedSoundManager ( )->playSoundMusicMenu ( );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, MenuMainScene::create ( ), ccWHITE ) );
}

KDvoid GameOverScene::goToPlayAgain ( KDvoid )
{
	KDint		nGameWasStarted = CCUserDefault::sharedUserDefault ( )->getIntegerForKey ( "gameWasStarted" );
	nGameWasStarted++;

	CCUserDefault::sharedUserDefault ( )->setIntegerForKey ( "gameWasStarted", nGameWasStarted );
	CCUserDefault::sharedUserDefault ( )->flush ( );
	
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	m_pPlayingScene->reset ( );
	CCDirector::sharedDirector ( )->popScene ( );				// remove gameover scene and replace playing scene	
}

const KDchar* GameOverScene::buildFacebookMessageWithPoints ( KDuint uPoints, KDuint uLevel )
{
	return KD_NULL;
}

KDvoid GameOverScene::submitButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
}

KDvoid GameOverScene::renameButtonPressed ( CCObject* pSender )
{
	CCLOG ( "renameButtonPressed." );
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );	
}

KDvoid GameOverScene::playAgainButtonPressed ( CCObject* pSender )
{
	CCLOG ( "playAgainButtonPressed" );
	
	this->goToPlayAgain ( );
}

KDvoid GameOverScene::menuButtonPressed ( CCObject* pSender )
{
	CCLOG ( "menuButtonPressed" );
	
	this->goToMainMenu ( );
}

#ifdef FREE_VERSION
KDvoid GameOverScene::buyFullVersionButtonPressed ( CCObject* pSender )
{
	CCLOG ( "buyFullVersion" );
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	xmOpenURL ( "http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=384946158&mt=8" );
}
#endif

KDvoid GameOverScene::loginButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
}

KDvoid GameOverScene::logoutButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
}

KDvoid GameOverScene::setPlayedTooMuch ( KDbool bTooMuch )
{
	m_bPlayedTooMuch = bTooMuch;
}

KDvoid GameOverScene::setHighScore ( KDuint uHighScore )
{
	m_pHighScoreLabel->setUInteger ( uHighScore );
}

KDvoid GameOverScene::setCurrentScore ( KDuint uCurrentScore )
{
	m_pCurrentScoreLabel->setUInteger ( uCurrentScore );
}

KDvoid GameOverScene::setLevel ( KDuint uLevel )
{
	m_uLevel = uLevel;
}

KDvoid GameOverScene::setPlayingScene ( PlayingScene* pScene )
{
	m_pPlayingScene = pScene;
}
