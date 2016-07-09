/* --------------------------------------------------------------------------
 *
 *      File            PlayingScene.cpp
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
#include "PlayingScene.h"
#include "MenuMainScene.h"
#include "PauseScene.h"
#include "GameOverScene.h"

KDbool PlayingScene::init ( KDvoid )
{
	if ( !CCScene::init ( ) )
	{
		return KD_FALSE;
	}

	// board
	m_pBoard = Board::create ( this ); 
	m_pBoard->setPosition ( ccp ( 480 - 320 * 0.5, 160 ) );
	this->addChild ( m_pBoard );

	// background girls
	m_pBackgroundGirls = BackgroundGirls::create ( );
	m_pBackgroundGirls->setPosition ( ccp ( 125 * 0.5, 160 ) );
	this->addChild ( m_pBackgroundGirls );

	// add time bar
	m_pTimeBar = TimeBar::create ( this ); 
	m_pTimeBar->setPosition ( ccp ( 125 + 35 * 0.5, 160 ) );
	this->addChild ( m_pTimeBar );

	// score and level label
	m_pScoreAndLevelSprite = CCSprite::create ( "level_and_score_texture.png" );
	m_pScoreAndLevelSprite->setPosition ( ccp ( 120 * 0.5, 45 * 0.5 ) );
	this->addChild (  m_pScoreAndLevelSprite );

	// pause button
	CCMenuItem*		pPauseItem = CCMenuItemImage::create 
	(
		"pause_button_released.png", 
		"pause_button_pressed.png",
		this,
		menu_selector ( PlayingScene::pauseButtonPressed )
	);
	m_pPauseMenu = CCMenu::createWithItem ( pPauseItem );
	m_pPauseMenu->setPosition ( ccp ( 20, 300 ) );
	this->addChild ( m_pPauseMenu );

	// level label
	m_pLevelLabel = UIntegerLabel::create ( 15, 1, "Helvetica.ttf", ccWHITE, ccBLACK );
	m_pLevelLabel->setUInteger ( 1 );
	m_pLevelLabel->setPosition ( ccp ( 36, 32 ) );
	this->addChild ( m_pLevelLabel );

	// score label
	m_pScoreLabel = UIntegerLabel::create ( 15, 1, "Helvetica.ttf", ccWHITE, ccBLACK );
	m_pScoreLabel->setUInteger ( 0 );
	m_pScoreLabel->setPosition ( ccp ( 36, 14 ) );
	this->addChild ( m_pScoreLabel );

	// ready sprite
	m_pReadySprite = CCSprite::create ( "ready_texture.png" );
	m_pReadySprite->setVisible ( KD_FALSE );
	this->addChild (  m_pReadySprite );

	// gameover sprite
	m_pGameOverSprite = CCSprite::create ( "game_over_label.png" );
	m_pGameOverSprite->setVisible ( KD_FALSE );
	m_pGameOverSprite->setPosition ( ccp ( 240, 250 ) );
	this->addChild ( m_pGameOverSprite );

	// used for girl transition
	m_pGirlTransition = CCSprite::create ( "change_girl_transition.pvr" );
	m_pGirlTransition->setVisible ( KD_FALSE );
	m_pGirlTransition->setPosition ( ccp ( 240, 160 ) );
	m_pGirlTransition->setOpacity ( 0 );
	this->addChild ( m_pGirlTransition );

	// game layout
	this->checkGameLayout ( );

	// start the game and start both FSMs
	this->reset ( );

	return KD_TRUE;
}

KDvoid PlayingScene::pauseButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	
	if ( m_eState == PLAYING )
	{
		this->changeGameState ( PAUSED );
	}
}

KDvoid PlayingScene::readySequenceFinished ( CCNode* pSender )
{
	this->changeGameState ( PLAYING );
}

// Called in the middle of ready sequence.
KDvoid PlayingScene::playReadySoundFX ( CCNode* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxReady ( );
}

// Called by TimeBar when time is up. Callback method.
KDvoid PlayingScene::timeUp ( KDvoid )
{
	this->changeGameState ( GAME_OVER );
}

// Called after game over animation
KDvoid PlayingScene::gameOverAnmationFinished ( CCNode* pSender )
{
	this->changeGameState ( ENDING_STATE_GS );
}

// Reset all game components in order to start a new game.
KDvoid PlayingScene::reset ( KDvoid )
{
	m_pBackgroundGirls->setGirlNumber ( 0 );
	m_pBoard->reset ( );
	m_pBoard->regenerate ( 0 );
	m_pTimeBar->reset ( );
	m_pLevelLabel->setUInteger ( 0 );
	m_pScoreLabel->setUInteger ( 0 );
	m_pGameOverSprite->setVisible ( KD_FALSE );
	m_pReadySprite->setPosition ( ccp ( -m_pReadySprite->getTextureRect ( ).size.cx * 0.5, 150 ) );
	m_bPlayedTooMuch = KD_FALSE;
	
	// vars for "game" FSM
	m_eState = STARTING_STATE_GS;
	
	this->changeGameState ( READY );
}

// called when resume button in pause menu is pressed
KDvoid PlayingScene::onResume ( KDvoid )
{
	CCLOG ( "resume" );	

	this->checkGameLayout ( );
	this->changeGameState ( PLAYING );
}

// Used by board.
KDbool PlayingScene::isPlaying ( KDvoid )
{
	return m_eState == PLAYING;
}

// Used by Board. 
KDuint PlayingScene::getLevel ( KDvoid )
{
	return m_pLevelLabel->getUInteger ( );
}

// Re-layout game components.
// Called at start up and after options menu was used.
KDvoid PlayingScene::checkGameLayout ( KDvoid )
{
	KDbool		bBoardAtTheLeft = CCUserDefault::sharedUserDefault ( )->getBoolForKey ( "boardAtTheLeft" );
	if ( bBoardAtTheLeft )
	{
		 m_pBoard				->setPosition ( ccp ( 320 * 0.5, 160 ) );
		 m_pBackgroundGirls		->setPosition ( ccp ( 480 - 125 * 0.5, 160 ) );
		 m_pTimeBar				->setPosition ( ccp ( 480 - 125 - 35 * 0.5, 160 ) );
		 m_pScoreAndLevelSprite	->setPosition ( ccp ( 480 - 120 * 0.5 - 5, 45 * 0.5 ) );
		 m_pPauseMenu			->setPosition ( ccp ( 480 - 20, 300 ) );
		 m_pLevelLabel			->setPosition ( ccp ( 480 - 89, 32 ) );
		 m_pScoreLabel			->setPosition ( ccp ( 480 - 89, 14 ) );
	} 
	else
	{
		// board at the right
		m_pBoard				->setPosition ( ccp ( 480 - 320 * 0.5, 160 ) );
		m_pBackgroundGirls		->setPosition ( ccp ( 125 * 0.5, 160 ) );
		m_pTimeBar				->setPosition ( ccp ( 125 + 35 * 0.5, 160 ) );
		m_pScoreAndLevelSprite	->setPosition ( ccp ( 120 * 0.5, 45 * 0.5 ) );
		m_pPauseMenu			->setPosition ( ccp ( 20, 300 ) );
		m_pLevelLabel			->setPosition ( ccp ( 36, 32 ) );
		m_pScoreLabel			->setPosition ( ccp ( 36, 14 ) );
	}
}

KDvoid PlayingScene::applicationWillResginActive ( KDvoid )
{
	CCLOG ( "applicationWillResginActive" );
	if ( this->isPlaying ( ) ) 
	{
		this->changeGameState ( PAUSE_BY_LOCK );
	}
}

// Played too much. Called only in free version 
KDvoid PlayingScene::playedTooMuch ( KDvoid )
{
	m_bPlayedTooMuch = KD_TRUE;
	this->changeGameState ( GAME_OVER );
}

KDvoid PlayingScene::changeGameState ( GameState eState )
{
	this->onExitGameState ( m_eState );

	if ( m_eState == STARTING_STATE_GS && eState == READY )
	{
		
	} 
	else if ( m_eState == READY && eState == PLAYING )
	{
		m_pTimeBar->activate ( );	
	}
	else if ( m_eState == PLAYING && eState == PAUSED )
	{
		m_pTimeBar->deactivate ( );
		PauseScene*		pPauseScene = PauseScene::create ( );
		pPauseScene->setPlayingScene ( this );
		CCDirector::sharedDirector ( )->pushScene ( CCTransitionFade::create ( TRANSITION_DURATION, pPauseScene, ccWHITE ) );	
	}
	else if ( m_eState == PAUSED && eState == PLAYING )
	{
		m_pTimeBar->activate ( );	
	}
	else if ( m_eState == PLAYING && eState == PAUSE_BY_LOCK )
	{
		m_pTimeBar->deactivate ( );
		PauseScene*		pPauseScene = PauseScene::create ( );
		pPauseScene->setPlayingScene ( this );
		CCDirector::sharedDirector ( )->pushScene ( pPauseScene );		
	}
	else if ( m_eState == PAUSE_BY_LOCK && eState == PLAYING )
	{
		m_pTimeBar->activate ( );	
	}
	else if ( m_eState == PLAYING && eState == GAME_OVER )
	{		
		#define GAME_OVER_ANIMATION_DURATION	1.5f
		
		CCActionInterval*	pActionWithBounce = CCEaseBounceOut::create 
		( 
			CCMoveTo::create ( GAME_OVER_ANIMATION_DURATION, ccp ( 240, 250 ) )
		);
		
		CCActionInterval*	pActions = CCSpawn::create 
		(
			pActionWithBounce,
			CCFadeIn::create ( GAME_OVER_ANIMATION_DURATION ),
			KD_NULL
		);
		
		CCActionInterval*	pFinalAction = CCSequence::create 
		(
			CCShow::create ( ),
			pActions,
			CCCallFuncN::create ( this, callfuncN_selector ( PlayingScene::gameOverAnmationFinished ) ), 
			KD_NULL
		);
		m_pGameOverSprite->runAction ( pFinalAction );		
	} 
	else if ( m_eState == GAME_OVER && eState == ENDING_STATE_GS )
	{
		GameOverScene*	pGameOverScene = GameOverScene::create ( );
		pGameOverScene->setPlayedTooMuch ( m_bPlayedTooMuch );
		pGameOverScene->setCurrentScore ( m_pScoreLabel->getUInteger ( ) );
		pGameOverScene->setLevel ( m_pLevelLabel->getUInteger ( ) );
		pGameOverScene->setPlayingScene ( this );
		CCDirector::sharedDirector ( )->pushScene ( CCTransitionFade::create ( TRANSITION_DURATION, pGameOverScene, ccWHITE ) );	
	} 
	else 
	{
		CCAssert ( KD_FALSE, "Unknown transition in Game object." );	
	}

	m_eState = eState;
	this->onEnterGameState ( eState );
}

KDvoid PlayingScene::onEnterGameState ( GameState eState )
{
	switch ( eState )
	{
		case STARTING_STATE_GS :
			break;

		case READY :
		{
			// an animation with 3 parts, and eventually a method call (in order to transitate to next state) 
			#define READY_FIRST_STAGE_DURATION 1.5
			#define READY_SECOND_STAGE_DURATION 0.5
			#define READY_THIRD_STAGE_DURATION 0.5
	
			// part 1
			CCActionInterval*		pAuxFirstAction = CCSpawn::create 
			(
				CCEaseElasticOut::create ( CCMoveTo::create ( READY_FIRST_STAGE_DURATION, ccp ( 240, 200 ) ) ),
				CCFadeIn::create ( READY_FIRST_STAGE_DURATION ),
				KD_NULL
			);

			CCActionInterval*		pFirstAction = CCSequence::create 
			(
				pAuxFirstAction,
				CCCallFuncN::create ( this, callfuncN_selector ( PlayingScene::playReadySoundFX ) ),
				KD_NULL 
			);

			// part 2, formed by 2 parts
			CCActionInterval*		pAuxAction = CCScaleBy::create ( READY_SECOND_STAGE_DURATION * 0.5f, 1.4f );
			CCActionInterval*		pAuxAction2 = CCSequence::create ( pAuxAction, pAuxAction->reverse ( ), KD_NULL );
			CCActionInterval*		pSecondAction= CCEaseSineInOut::create ( pAuxAction2 );
									
			// part 3
			CCActionInterval*		pThirdAction = CCSpawn::create
			(
				CCEaseBackIn::create ( CCMoveTo::create ( READY_THIRD_STAGE_DURATION, ccp ( 480 + m_pReadySprite->getTextureRect ( ).size.cx * 0.5f, 250 ) ) ),
				CCFadeOut::create ( READY_THIRD_STAGE_DURATION ),
				KD_NULL
			);

			// final animation or action
			CCActionInterval*		pFinalAction = CCSequence::create
			(
				CCShow::create ( ),
				pFirstAction, 
				pSecondAction, 
				pThirdAction, 
				CCHide::create ( ),
				CCCallFuncN::create ( this, callfuncN_selector ( PlayingScene::readySequenceFinished ) ),
				KD_NULL
			);
			
			m_pReadySprite->runAction ( pFinalAction );

		}	break;

		case PLAYING :
			SoundManager::sharedSoundManager ( )->playSoundMusicGame ( );
			break;

		case PAUSED :
			break;

		case PAUSE_BY_LOCK :
			break;

		case GAME_OVER :
			m_pGameOverSprite->setPosition ( ccp ( 240, 320 + m_pGameOverSprite->getTextureRect ( ).size.cy * 0.5 ) );
			SoundManager::sharedSoundManager ( )->playSoundFxGameOver ( );
			break;

		case ENDING_STATE_GS :
			CCLOG ( "Unknown Game state onEnterGameState." );
			break;
	}
}

KDvoid PlayingScene::onExitGameState ( GameState eState )
{
	switch ( eState )
	{
		case STARTING_STATE_GS :
			break;

		case READY :
			break;

		case PLAYING :
			SoundManager::sharedSoundManager ( )->stopBackgroundMusic ( );
			break;

		case PAUSED :
			break;

		case PAUSE_BY_LOCK :
			break;

		case GAME_OVER :
			m_pTimeBar->deactivate ( );
			break;

		case ENDING_STATE_GS :
			m_pGameOverSprite->setVisible ( KD_FALSE );
			break;

		default :
			CCLOG ( "Unknown Game state onExitGameState." );
			break;
	}
}

TimeBar* PlayingScene::getTimeBar ( KDvoid )
{
	return m_pTimeBar;
}

BackgroundGirls* PlayingScene::getBackgroundGirls ( KDvoid )
{
	return m_pBackgroundGirls;
}

UIntegerLabel* PlayingScene::getLevelLabel ( KDvoid )
{
	return m_pLevelLabel;
}

UIntegerLabel* PlayingScene::getScoreLabel ( KDvoid )
{
	return m_pScoreLabel;
}

CCSprite* PlayingScene::getGirlTransition ( KDvoid )
{
	return m_pGirlTransition;
}