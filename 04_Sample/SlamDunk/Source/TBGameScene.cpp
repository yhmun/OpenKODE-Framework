/* --------------------------------------------------------------------------
 *
 *      File            TBGameScene.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Rana Hammad Hussain on 04/22/11
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Azosh & Co. All rights reserved.
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
#include "TBGameScene.h"
#include "TBMainMenuScene.h"

#define GAME_TIME						120

#define CHARACTER_WIDTH					68
#define CHARACTER_HEIGHT				75
#define PLAYER_START_Y					CHARACTER_HEIGHT / 2
#define PLAYER_MOVE_X					15
#define PLAYER_JUMP_HEIGHT				100
#define PLAYER_JUMP_SPEED				0.6f

#define BALL_RADIUS						22
#define HEAD_RADIUS						20
#define BALL_MOVE_X						4.0f	// (48/6.0)
#define BALL_MOVE_Y						5.0f	// 32/6

#define BALL_X_DELTA					0.0f
#define BALL_Y_DELTA					0.0f

#define BALL_MOVE_SPEED					0.5f

KDbool TBGameScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	this->initGamePlayVariables ( );

	CCSprite*	pBackground = CCSprite::create ( "level_bg.png" );
	pBackground->setPosition ( GameState->getScreenCenter ( ) );
	this->addChild ( pBackground );

	// add poles
	this->addBaskets ( );

	this->setTouchEnabled ( KD_TRUE );
	this->setAccelerometerEnabled ( KD_TRUE );

	m_pMenuInstructions		= KD_NULL;
	m_pMenuPlayerSelection	= KD_NULL;
	m_bPause				= KD_FALSE;

	if ( GameState->isFirstTimeRunning ( ) )
	{
		// show instructions menu
		this->showInstructions ( );
		
		GameState->setFirstTimeRunning ( KD_FALSE );
		GameState->saveState ( );

		// then show select player
	}
	else
	{
		this->selectCharacter ( );
	}

	return KD_TRUE;
};

KDvoid TBGameScene::update ( KDfloat fDelta )
{
	if ( !m_bGameOver )
	{
		if ( !m_bPause && !m_bBallOverBasket )
		{					
			this->updateComputerMovements ( );		
		}
	}
}

KDvoid TBGameScene::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( !m_bPause && !m_bGameOver )
	{
		if (!m_bPlayerJumping )
		{
			m_bPlayerJumping = KD_TRUE;

			m_pPlayer->runAction 
			(
				CCSequence::create 
				(
					CCJumpBy::create ( PLAYER_JUMP_SPEED, ccp ( 0, 0 ), PLAYER_JUMP_HEIGHT, 1 ),
					CCCallFunc::create ( this, callfunc_selector ( TBGameScene::playerLanded ) ),
					KD_NULL
				)
			);
		}
	}
}

KDvoid TBGameScene::didAccelerate ( CCAcceleration* pAccelerationValue )
{
	if ( kdFabsf ( pAccelerationValue->x ) < 0.02f )
	{
		return;
	}
	
	if ( !m_bPause && !m_bGameOver && !m_bPlayerJumping )
	{
		if ( pAccelerationValue->x > 0 )
		{
			m_pPlayer->setPosition ( ccp ( m_pPlayer->getPosition ( ).x + PLAYER_MOVE_X, m_pPlayer->getPosition ( ).y ) );
			if ( m_pPlayer->getPosition ( ).x >= ( GameState->getScreenSize ( ).cx - 60 ) )
			{
				m_pPlayer->setPosition ( ccp ( GameState->getScreenSize ( ).cx - 60, m_pPlayer->getPosition ( ).y ) );			
			}
		}
		else
		{
			m_pPlayer->setPosition ( ccp ( m_pPlayer->getPosition ( ).x - PLAYER_MOVE_X, m_pPlayer->getPosition ( ).y ) );
			if ( m_pPlayer->getPosition ( ).x <= 60 )
			{
				m_pPlayer->setPosition ( ccp ( 60, m_pPlayer->getPosition ( ).y ) );
			}
		}
	}
}

KDvoid TBGameScene::initGamePlayVariables ( KDvoid )
{
	GameState->setGameSceneRunning ( KD_TRUE );

	m_nCharacterIdx		= GameState->getSelectedCharacterIndex ( );
	m_bPause			= KD_TRUE;
	m_bGameOver			= KD_TRUE;
	m_nTimer			= GAME_TIME;
	m_nPlayerScore		= 0;
	m_nComputerScore	= 0;
	
	m_bPlayerJumping	= KD_FALSE;
	m_bComputerJumping	= KD_FALSE;
	m_bShowingGoLabel	= KD_FALSE;

	m_pBackMenu			= KD_NULL;
	m_pGoLabel3			= KD_NULL;
	m_pGoLabel2			= KD_NULL;
	m_pGoLabel1			= KD_NULL;
	m_pGoLabelGO		= KD_NULL;
}

KDvoid TBGameScene::addBall ( KDvoid )
{
	m_pBall = CCSprite::create ( "ball.png" );
		
	m_pBall->setPosition ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenCenter ( ).y + BALL_RADIUS ) );
	m_tBallMovementDelta = ccp ( -BALL_MOVE_X, -BALL_MOVE_Y );
	m_fBallRadius = BALL_RADIUS;
	
	m_bBallOverBasket = KD_FALSE;
	this->addChild ( m_pBall, 3 ); 
}

KDvoid TBGameScene::addBaskets ( KDvoid )
{
	// add left basket
	m_pLeftBasket = CCSprite::create ( "Rightpole.png" );
	m_pLeftBasket->setPosition ( ccp ( 44, GameState->getScreenCenter ( ).y - 25 ) );
	this->addChild ( m_pLeftBasket, 2 ); 
	
	// add right basket
	m_pRightBasket = CCSprite::create ( "Leftpole.png" );
	m_pRightBasket->setPosition ( ccp ( GameState->getScreenSize ( ).cx - 44, GameState->getScreenCenter ( ).y - 25 ) );
	this->addChild ( m_pRightBasket, 2 );
}

KDvoid TBGameScene::addCharacters ( KDvoid )
{
	KDint	nRand2 = kdRand ( ) % 2;

	// add player character
	switch ( m_nCharacterIdx )
	{
		case 1 :
			m_pPlayer = CCSprite::create ( "Blue.png" );
			if ( nRand2 == 0 )
			{
				m_nComputerIdx = 3;
				m_pComputer = CCSprite::create ( "Yellow.png" );
			}
			else 
			{
				m_nComputerIdx = 2;
				m_pComputer = CCSprite::create ( "Orange.png" );
			}
			break;

		case 2 :
			m_pPlayer = CCSprite::create ( "Orange.png" );
			if ( nRand2 == 0 )
			{
				m_nComputerIdx = 3;
				m_pComputer = CCSprite::create ( "Yellow.png" );
			}
			else 
			{
				m_nComputerIdx = 1;
				m_pComputer = CCSprite::create ( "Blue.png" );
			}			
			break;

		default :
			m_pPlayer = CCSprite::create ( "Yellow.png" );
			if ( nRand2 == 0 )
			{
				m_nComputerIdx = 1;
				m_pComputer = CCSprite::create ( "Blue.png" );
			}
			else 
			{
				m_nComputerIdx = 2;
				m_pComputer = CCSprite::create ( "Orange.png" );
			}
			
			break;
	}
	

	m_pPlayer->setPosition ( ccp ( GameState->getScreenCenter ( ).x - 40, PLAYER_START_Y ) );
	m_pComputer->setPosition ( ccp ( GameState->getScreenCenter ( ).x + 40, PLAYER_START_Y ) );
	this->addChild ( m_pPlayer, 9 ); 

	// add controls to handle it	
	this->addChild ( m_pComputer, 9 ); 

	m_bPlayerJumping	= KD_FALSE;
	m_bMoveLeft			= KD_FALSE;
	m_bMoveRight		= KD_FALSE;
	
	m_pLeftControl  = CCMenuItemImage::create ( "backarrow.png", KD_NULL, this, menu_selector ( TBGameScene::leftClicked  ) );	
	m_pRightControl = CCMenuItemImage::create ( "nextarrow.png", KD_NULL, this, menu_selector ( TBGameScene::rightClicked ) );

	m_pLeftControl ->setPosition ( ccp ( 40, 20 ) );
	m_pRightControl->setPosition ( ccp ( GameState->getScreenSize ( ).cx - 40, 20 ) );
	
	m_pLeftControl ->setScale ( 1.4f );
	m_pRightControl->setScale ( 1.4f );
	
//	CCMenu*		pMenu = CCMenu::create ( m_pLeftControl, m_pRightControl, KD_NULL );
}

KDvoid TBGameScene::addGameObjects ( KDvoid )
{	
	CCSprite*	pScoreHolder = CCSprite::create ( "scoreboard.png" );
	pScoreHolder->setPosition ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenSize ( ).cy - 40 ) );
	this->addChild ( pScoreHolder ); 

	m_pPlayerScoreLabel = CCLabelTTF::create ( "0", "Helvetica.ttf", 25 );
	m_pPlayerScoreLabel->setColor ( ccc3 ( 255, 200, 40 ) );
	m_pPlayerScoreLabel->setPosition ( ccp ( 50, 35 ) );
	pScoreHolder->addChild ( m_pPlayerScoreLabel );
	
	m_pComputerScoreLabel = CCLabelTTF::create ( "0", "Helvetica.ttf", 25 );
	m_pComputerScoreLabel->setColor ( ccc3 ( 255, 200, 40 ) );
	m_pComputerScoreLabel->setPosition ( ccp ( 200, 35 ) );
	pScoreHolder->addChild ( m_pComputerScoreLabel );
	
	m_pTimerLabel = CCLabelTTF::create ( "2:00", "Helvetica.ttf", 25 );
	m_pTimerLabel->setPosition ( ccp ( 125, 35 ) );
	pScoreHolder->addChild ( m_pTimerLabel );

	CCMenuItem*		pBack = CCMenuItemImage::create ( "Quit_btn.png", KD_NULL, this, menu_selector ( TBGameScene::backClicked ) );
	pBack->setPosition ( ccp ( 50, 290 ) );
	m_pBackMenu = CCMenu::createWithItem ( pBack );
	this->addChild ( m_pBackMenu );

	// add characters
	this->addCharacters ( );
	
	// add ball
	this->addBall ( );	
}

KDvoid TBGameScene::showInstructions ( KDvoid )
{
	m_pInstructionsBg = CCSprite::create ( "Instructions_bg.png" );
	m_pInstructionsBg->setPosition ( GameState->getScreenCenter ( ) );
	this->addChild ( m_pInstructionsBg, 5 );
	
	CCMenuItem*		pMenuBack = CCMenuItemImage::create ( "Back_button.png", KD_NULL, this, menu_selector ( TBGameScene::menuClicked ) );
	pMenuBack->setPosition ( ccp ( 80, 280 ) );

	CCMenuItem*		pMenuNext = CCMenuItemImage::create ( "Start_btn.png", KD_NULL, this, menu_selector ( TBGameScene::nextClicked ) );
	pMenuNext->setPosition ( ccp ( 400, 40 ) );

	m_pMenuInstructions = CCMenu::create ( pMenuBack, pMenuNext, KD_NULL );
	this->addChild ( m_pMenuInstructions, 6 );
}

KDvoid TBGameScene::showGoLabel ( KDvoid )
{
	m_nStartTime		= 3;
	m_bPause			= KD_FALSE;
	m_bGameOver			= KD_TRUE;
	m_bShowingGoLabel	= KD_TRUE;

	if ( m_pGoLabel3 )
	{
		m_pGoLabel3->stopAllActions ( );
		this->removeChild ( m_pGoLabel3, KD_TRUE );
		m_pGoLabel3 = KD_NULL;
	}
	if ( m_pGoLabel2 )
	{
		m_pGoLabel2->stopAllActions ( );
		this->removeChild ( m_pGoLabel2, KD_TRUE );
		m_pGoLabel2 = KD_NULL;
	}
	if ( m_pGoLabel1 )
	{
		m_pGoLabel1->stopAllActions ( );
		this->removeChild ( m_pGoLabel1, KD_TRUE );
		m_pGoLabel1 = KD_NULL;
	}
	if ( m_pGoLabelGO )
	{
		m_pGoLabelGO->stopAllActions ( );
		this->removeChild ( m_pGoLabelGO, KD_TRUE );
		m_pGoLabelGO = KD_NULL;
	}

	m_pGoLabel3  = CCSprite::create ( "GOThree.png" );
	m_pGoLabel2  = CCSprite::create ( "GOTwo.png" );
	m_pGoLabel1  = CCSprite::create ( "GOOne.png" );	
	m_pGoLabelGO = CCSprite::create ( "GOGo.png" );

	m_pGoLabel3->setOpacity ( 0 );
	this->addChild ( m_pGoLabel3, 10 );

	m_pGoLabel2->setOpacity ( 0 );
	this->addChild ( m_pGoLabel2, 10 );

	m_pGoLabel1->setOpacity ( 0 );
	this->addChild ( m_pGoLabel1, 10 );

	m_pGoLabelGO->setOpacity ( 0 );
	this->addChild ( m_pGoLabelGO, 10 );

	m_pGoLabel3 ->setPosition ( GameState->getScreenCenter ( ) );
	m_pGoLabel2 ->setPosition ( GameState->getScreenCenter ( ) );
	m_pGoLabel1 ->setPosition ( GameState->getScreenCenter ( ) );
	m_pGoLabelGO->setPosition ( GameState->getScreenCenter ( ) );

	m_pGoLabel3->runAction 
	(
		CCSequence::create 
		(
			CCFadeTo::create ( 0.5f, 255 ),
			CCScaleTo::create ( 0.5f, 1.5f ),
			CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateGoLabel ) ),
			KD_NULL 
		)
	);
}

KDvoid TBGameScene::selectCharacter ( KDvoid )
{
	m_pSelectedCharacterBg =  CCSprite::create ( "Select_box.png" );
	m_pCharactersBg =  CCSprite::create ( "Select_Character.png" );
	m_pCharactersBg->setPosition ( GameState->getScreenCenter ( ) );
	this->addChild ( m_pCharactersBg, 5 ); 
	
	CCMenuItem*		pMenuBack		 = CCMenuItemImage::create ( "menu_btn.png"	 , KD_NULL, this, menu_selector ( TBGameScene::menuClicked ) );
	CCMenuItem*		pBlueCharacter   = CCMenuItemImage::create ( "Blue.png"		 , KD_NULL, this, menu_selector ( TBGameScene::blueCharacterClicked ) );
	CCMenuItem*		pOrangeCharacter = CCMenuItemImage::create ( "Orange.png"	 , KD_NULL, this, menu_selector ( TBGameScene::orangeCharacterClicked ) );
	CCMenuItem*		pYellowCharacter = CCMenuItemImage::create ( "Yellow.png"	 , KD_NULL, this, menu_selector ( TBGameScene::yellowCharacterClicked ) );
	CCMenuItem*		pMenuNext		 = CCMenuItemImage::create ( "Select_btn.png", KD_NULL, this, menu_selector ( TBGameScene::characterSelected ) );

	m_pMenuPlayerSelection = CCMenu::create ( pMenuBack, pBlueCharacter, pOrangeCharacter, pYellowCharacter, pMenuNext, KD_NULL );
	this->addChild ( m_pSelectedCharacterBg, 6 );
	this->addChild ( m_pMenuPlayerSelection, 7 );

	m_pMenuPlayerSelection->setPosition ( ccp ( 0, 0 ) );
	pMenuBack->setPosition ( ccp ( 80, 280 ) );
	pMenuNext->setPosition ( ccp ( 390, 50 ) );
	
	pBlueCharacter  ->setPosition ( ccp ( GameState->getScreenCenter ( ).x - 100, GameState->getScreenCenter ( ).y - 40 ) );
	pOrangeCharacter->setPosition ( ccp ( GameState->getScreenCenter ( ).x      , GameState->getScreenCenter ( ).y - 40 ) );
	pYellowCharacter->setPosition ( ccp ( GameState->getScreenCenter ( ).x + 100, GameState->getScreenCenter ( ).y - 40 ) );

	m_nCharacterIdx = 1;
	m_pSelectedCharacterBg->setPosition ( ccp ( pBlueCharacter->getPosition ( ).x, pBlueCharacter->getPosition ( ).y ) );
}

KDvoid TBGameScene::menuClicked ( CCObject* pSender )
{
	GameState->playClick ( );
	
	if ( m_pBackMenu )
	{
		m_pBackMenu->setOpacity ( 0 );
	}

	GameState->stopGamemusic ( );

	GameState->setGameSceneRunning ( KD_FALSE );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionSlideInL::create ( 0.5f, TBMainMenuScene::scene ( ) ) );
}

KDvoid TBGameScene::nextClicked ( CCObject* pSender )
{
	this->removeChild ( m_pMenuInstructions, KD_TRUE );
	m_pMenuInstructions = KD_NULL;	
	
	m_pInstructionsBg->runAction
	(
		CCSequence::create 
		(
			CCFadeOut::create ( 1.0f ),
			CCCallFunc::create ( this, callfunc_selector ( TBGameScene::instructionsEnded ) ),
			KD_NULL 
		)
	);
}

KDvoid TBGameScene::backClicked ( CCObject* pSender )
{
	GameState->playClick ( );

	m_pBackMenu->runAction ( CCMoveBy::create ( 0.5, ccp ( -100, 0 ) ) );
	
	if ( m_nTimer > 0 )
	{
		this->pause ( );
	}
	else
	{
		this->gameOver ( );
	}
}

KDvoid TBGameScene::instructionsEnded ( KDvoid )
{
	this->selectCharacter ( );
}

KDvoid TBGameScene::blueCharacterClicked ( CCObject* pSender )
{
	GameState->playClick ( );
	m_nCharacterIdx = 1;
	m_pSelectedCharacterBg->setPosition ( ccp ( GameState->getScreenCenter ( ).x - 100, GameState->getScreenCenter ( ).y - 40 ) );
}

KDvoid TBGameScene::orangeCharacterClicked ( CCObject* pSender )
{
	GameState->playClick ( );
	m_nCharacterIdx = 2;
	m_pSelectedCharacterBg->setPosition ( ccp ( GameState->getScreenCenter ( ).x , GameState->getScreenCenter ( ).y - 40 ) );
}

KDvoid TBGameScene::yellowCharacterClicked ( CCObject* pSender )
{
	GameState->playClick ( );
	m_nCharacterIdx = 3;
	m_pSelectedCharacterBg->setPosition ( ccp ( GameState->getScreenCenter ( ).x + 100, GameState->getScreenCenter ( ).y - 40 ) );
}

KDvoid TBGameScene::characterSelected ( CCObject* pSender )
{
	GameState->playClick ( );
	this->removeChild ( m_pMenuPlayerSelection, KD_TRUE );
	this->removeChild ( m_pSelectedCharacterBg, KD_TRUE ); 
	m_pMenuPlayerSelection = KD_NULL;
	
	m_pCharactersBg->runAction
	(
		CCSequence::create
		(
			CCFadeOut::create ( 1.0 ),
			CCCallFunc::create ( this, callfunc_selector ( TBGameScene::characterSelectedEnded ) ),
			KD_NULL
		)
	);
}

KDvoid TBGameScene::characterSelectedEnded ( KDvoid )
{
	this->addGameObjects ( );
	this->showGoLabel ( );
}

KDvoid TBGameScene::pause ( KDvoid )
{
	m_bPause = KD_TRUE;
	m_pBall->setOpacity ( 0 );
	m_pBall->stopAllActions ( );
	
	m_pPauseBg = CCSprite::create ( "Black_opacity_layer.png" );
	m_pPauseBg->setPosition ( GameState->getScreenCenter ( ) );
	this->addChild ( m_pPauseBg, 10 );
	
	CCSprite*	pBg = CCSprite::create ( "Game_Pause_popup.png" );
	pBg->setPosition ( GameState->getScreenCenter ( ) );
	m_pPauseBg->addChild ( pBg );
	
	CCMenuItem*		pResumeMenu  = CCMenuItemImage::create ( "Resume_btn.png" , KD_NULL, this, menu_selector ( TBGameScene::resumeClicked ) );	
	CCMenuItem*		pRestartMenu = CCMenuItemImage::create ( "Restart_btn.png", KD_NULL, this, menu_selector ( TBGameScene::restartClicked ) );		
	CCMenuItem*		pQuitToMenu  = CCMenuItemImage::create ( "menu_btn.png"   , KD_NULL, this, menu_selector ( TBGameScene::menuClicked ) );
		
	CCMenu*			pMenu = CCMenu::create ( pQuitToMenu, pResumeMenu, pRestartMenu, KD_NULL );
	m_pPauseBg->addChild ( pMenu );
	pResumeMenu->setPosition  ( ccp ( m_pPauseBg->getPosition ( ).x, m_pPauseBg->getPosition ( ).y - 60 ) );
	pRestartMenu->setPosition ( ccp ( m_pPauseBg->getPosition ( ).x, m_pPauseBg->getPosition ( ).y - 20 ) );
	pQuitToMenu->setPosition  ( ccp ( m_pPauseBg->getPosition ( ).x, m_pPauseBg->getPosition ( ).y + 20 ) );
}

KDvoid TBGameScene::gameOver ( KDvoid )
{
	this->unschedule ( schedule_selector ( TBGameScene::updateTimer ) );

	m_bGameOver = KD_TRUE;

	m_pBall->setOpacity ( 0 );
	m_pBall->stopAllActions ( );

	// add Background
	m_pGameOverBg = CCSprite::create ( "Gameover_bg.png" );
	m_pGameOverBg->setPosition ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenCenter ( ).y ) );
	this->addChild ( m_pGameOverBg, 10 ); 

	// add Label
	CCSprite*		pGameStateLabel;

	// add menu buttons - menu/share/restart	
	if ( m_nPlayerScore > m_nComputerScore )
	{
		pGameStateLabel = CCSprite::create ( "Winner.png" );
		
		pGameStateLabel->setPosition ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenCenter ( ).y + 35 ) );
		m_pGameOverBg->addChild ( pGameStateLabel );
		m_pGameOverBg->addChild ( this->addScores ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenCenter ( ).y - 20 ) ) );
	}
	else if ( m_nPlayerScore < m_nComputerScore )
	{
		pGameStateLabel = CCSprite::create ( "Loser.png" );
		
		pGameStateLabel->setPosition ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenCenter ( ).y + 35 ) );
		m_pGameOverBg->addChild ( pGameStateLabel );
		m_pGameOverBg->addChild ( this->addScores ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenCenter ( ).y - 20 ) ) );
	}
	else
	{
		pGameStateLabel = CCSprite::create ( "draw.png" );		
		pGameStateLabel->setPosition ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenCenter ( ).y + 15 ) );
		m_pGameOverBg->addChild ( pGameStateLabel );
	}

	CCMenuItem*		pBackMenu1 = CCMenuItemImage::create ( "menu_btn.png"		, KD_NULL, this, menu_selector ( TBGameScene::menuClicked ) );
	CCMenuItem*		pShareMenu = CCMenuItemImage::create ( "share_game_btn.png"	, KD_NULL, this, menu_selector ( TBGameScene::shareClicked ) );
	CCMenuItem*		pRestartMenu = CCMenuItemImage::create ( "Restart_btn.png"	, KD_NULL, this, menu_selector ( TBGameScene::restartClicked ) );
	
	CCMenu*			pMenu = CCMenu::create ( pBackMenu1, pShareMenu, pRestartMenu, KD_NULL );
	m_pGameOverBg->addChild ( pMenu );

	pBackMenu1->setPosition ( ccp ( m_pGameOverBg->getPosition ( ).x - 50, m_pGameOverBg->getPosition ( ).y - 70 ) );
	pShareMenu->setPosition ( ccp ( m_pGameOverBg->getPosition ( ).x + 50, m_pGameOverBg->getPosition ( ).y - 70 ) );
	pRestartMenu->setPosition ( ccp ( m_pGameOverBg->getPosition ( ).x, m_pGameOverBg->getPosition ( ).y - 105 ) );

	CCSprite*		pPlayerSprite;
	if ( m_nCharacterIdx == 1 )
	{
		if ( m_nPlayerScore > m_nComputerScore )
		{
			pPlayerSprite = CCSprite::create ( "Blue.png" );
		}
		else
		{
			pPlayerSprite = CCSprite::create ( "Blue_sad.png" );
		}
	}
	else if ( m_nCharacterIdx == 2 )
	{
		if ( m_nPlayerScore > m_nComputerScore )
		{
			pPlayerSprite = CCSprite::create ( "Orange.png" );
		}
		else
		{
			pPlayerSprite = CCSprite::create ( "orange_sad.png" );
		}
	}
	else
	{
		if ( m_nPlayerScore > m_nComputerScore )
		{
			pPlayerSprite = CCSprite::create ( "Yellow.png" );
		}
		else
		{
			pPlayerSprite = CCSprite::create ( "yellow_sad.png" );
		}
	}	

	pPlayerSprite->setPosition ( ccp ( GameState->getScreenCenter ( ).x + 160, PLAYER_START_Y ) );
	m_pGameOverBg->addChild ( pPlayerSprite );
}

KDvoid TBGameScene::leftClicked ( CCObject* pSender )
{

}
 
KDvoid TBGameScene::rightClicked ( CCObject* pSender )
{

}

KDvoid TBGameScene::updateGoLabel ( KDvoid )
{
	if ( m_bPause )
	{
		m_pGoLabel3->runAction
		(
			CCSequence::create
			(
				CCDelayTime::create ( 0.1f ),
				CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateGoLabel ) ),
				KD_NULL
			)
		);
	}
	else 
	{
		m_pGoLabel3 ->setOpacity ( 0 );
		m_pGoLabel2 ->setOpacity ( 0 );
		m_pGoLabel1 ->setOpacity ( 0 );
		m_pGoLabelGO->setOpacity ( 0 );
		
		m_nStartTime--;

		if ( m_nStartTime == 2 )
		{
			m_pGoLabel2->runAction
			(
				CCSequence::create
				(
					CCFadeTo::create ( 0.5f, 255 ),
					CCScaleTo::create ( 0.5f, 1.5f ),
					CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateGoLabel ) ),
					KD_NULL
				)
			);			
		}
		else if ( m_nStartTime == 1 )
		{
			m_pGoLabel1->runAction
			(
				CCSequence::create
				(
					CCFadeTo::create ( 0.5f, 255 ),
					CCScaleTo::create ( 0.5f, 1.5f ),
					CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateGoLabel ) ),
					KD_NULL
				)
			);	
		}
		else if ( m_nStartTime == 0 )
		{
			m_pGoLabelGO->runAction
			(
				CCSequence::create
				(
					CCFadeTo::create ( 0.5f, 255 ),
					CCScaleTo::create ( 0.5f, 1.5f ),
					CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateGoLabel ) ),
					KD_NULL
				)
			);	
		}
		else 
		{			
			this->removeChild ( m_pGoLabel3, KD_TRUE );
			m_pGoLabel3 = KD_NULL;

			this->removeChild ( m_pGoLabel2, KD_TRUE );
			m_pGoLabel2 = KD_NULL;

			this->removeChild ( m_pGoLabel1, KD_TRUE );
			m_pGoLabel1 = KD_NULL;

			this->removeChild ( m_pGoLabelGO, KD_TRUE );
			m_pGoLabelGO = KD_NULL;
			
			m_bShowingGoLabel = KD_FALSE;
			this->startClicked ( );
		}
	}
}

KDvoid TBGameScene::startClicked ( KDvoid )
{
	m_nPlayerScore	 = 0;
	m_nComputerScore = 0;
	m_bGameOver		 = KD_FALSE;
	m_nTimer		 = GAME_TIME;
	
	m_pBall->setOpacity  ( 255 );
	m_pBall->setPosition ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenSize ( ).cy + 20 ) );
	m_tBallMovementDelta = ccp ( -BALL_MOVE_X, -BALL_MOVE_Y );
	m_fBallRadius		 = BALL_RADIUS;	
	m_bBallOverBasket	 = KD_FALSE;

	m_pPlayerScoreLabel->setString ( ccszf ( "%d", m_nPlayerScore ) );
	m_pComputerScoreLabel->setString ( ccszf ( "%d", m_nComputerScore ) );

	KDint	nMinutes = m_nTimer % 60;
	if ( nMinutes > 9 )
	{
		m_pTimerLabel->setString ( ccszf ( "%d:%d", m_nTimer / 60, nMinutes ) );
	}
	else
	{
		m_pTimerLabel->setString ( ccszf ( "%d:0%d", m_nTimer / 60, nMinutes ) );
	}
	
	m_bPause = KD_FALSE;

	m_pBall->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 0.1f, 30.f ) ) );

	m_pBall->runAction 
	(
		CCSequence::create 
		(
			CCMoveBy::create ( 1 / 60.f, m_tBallMovementDelta ),
			CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateBallPosition ) ),
			KD_NULL
		) 
	);

	this->unscheduleUpdate ( );
	this->scheduleUpdate ( );

	this->unschedule ( schedule_selector ( TBGameScene::updateTimer ) );
	this->schedule ( schedule_selector ( TBGameScene::updateTimer ), 1.f );

	GameState->playGamemusic ( );
}

KDint TBGameScene::detectOverBasket ( KDvoid )
{
	KDint	nDetected = 0;
	
	if ( m_pBall->getPosition ( ).x < GameState->getScreenSize ( ).cx / 2 )
	{
		if ( ( m_pBall->getPosition ( ).y - m_fBallRadius >= ( m_pLeftBasket->getPosition ( ).y + 40 ) ) &&
			 ( m_pBall->getPosition ( ).x - m_fBallRadius <= ( m_pLeftBasket->getPosition ( ).x ) ) )
		{
			if ( m_tBallMovementDelta.y < 0)
			{
				nDetected = -1;
			}
		}
	}
	else 
	{
		if ( ( m_pBall->getPosition ( ).y - m_fBallRadius >= ( m_pRightBasket->getPosition ( ).y + 40 ) ) &&
		     ( m_pBall->getPosition ( ).x + m_fBallRadius >= ( m_pRightBasket->getPosition ( ).x ) ) ) 
		{
			if ( m_tBallMovementDelta.y < 0 )
			{
				nDetected = 1;
			}
		}
	}
	
	return nDetected;
}

KDbool TBGameScene::detectBallBoundaryCheck ( KDvoid )
{
	KDbool		bBallCollided = KD_FALSE;

	// horizontal check
	if ( m_pBall->getPosition ( ).x - m_fBallRadius < 0 && m_tBallMovementDelta.x < 0 )
	{
		m_tBallMovementDelta.x = -m_tBallMovementDelta.x;
		bBallCollided = KD_TRUE;
		GameState->playBallHit ( );
	}
	else if ( m_pBall->getPosition ( ).x + m_fBallRadius > GameState->getScreenSize ( ).cx && m_tBallMovementDelta.x > 0 )
	{
		m_tBallMovementDelta.x = -m_tBallMovementDelta.x;
		bBallCollided = KD_TRUE;
		GameState->playBallHit ( );
	}
	
	// vertical check
	if ( m_pBall->getPosition ( ).y - m_fBallRadius < 0 && m_tBallMovementDelta.y < 0 )
	{
		m_tBallMovementDelta.y = -m_tBallMovementDelta.y;
		bBallCollided = KD_TRUE;
		GameState->playBallHit ( );
	}
	else if ( m_pBall->getPosition ( ).y + m_fBallRadius > GameState->getScreenSize ( ).cy && m_tBallMovementDelta.y > 0 )
	{
		m_tBallMovementDelta.y = -m_tBallMovementDelta.y;
		bBallCollided = KD_TRUE;
		GameState->playBallHit ( );
	}
	
	// beneath baskets check
	if ( ( ( m_pBall->getPosition ( ).y - m_fBallRadius >= ( m_pLeftBasket ->getPosition ( ).y ) ) &&
		   ( m_pBall->getPosition ( ).x - m_fBallRadius <= ( m_pLeftBasket ->getPosition ( ).x + 22 ) ) ) || 

	     ( ( m_pBall->getPosition ( ).y - m_fBallRadius >= ( m_pRightBasket->getPosition ( ).y ) ) &&
	       ( m_pBall->getPosition ( ).x + m_fBallRadius >= ( m_pRightBasket->getPosition ( ).x - 22 ) ) ) )
	{
		if ( m_tBallMovementDelta.y > 0)
		{
			m_tBallMovementDelta.y = -BALL_MOVE_Y;
			bBallCollided = KD_TRUE;
		}
	}

	return bBallCollided;
}
 
KDbool TBGameScene::detectBallPlayerCollision ( CCSprite* pPlayer )
{
	// detect collision with player and reflect update 
	CCRect		tPlayerRect   = pPlayer->boundingBox ( );
	CCRect		tBallRect     = m_pBall->boundingBox ( );
	
	CCPoint		tHeadCenter   = ccp ( pPlayer->getPosition ( ).x, tPlayerRect.origin.y + tPlayerRect.size.cy - HEAD_RADIUS );
	CCPoint		tBallCenter   = m_pBall->getPosition ( );	
	CCPoint		tPlayerCenter = pPlayer->getPosition ( );
	
	if ( tPlayerRect.intersectsRect ( tBallRect ) )
	{
		KDfloat		x2 = ( tBallCenter.x - tHeadCenter.x );
		x2 *= x2;
		
		KDfloat		y2 = ( tBallCenter.y - tHeadCenter.y );
		y2 *= y2;
		
		KDfloat		r2 = ( m_fBallRadius - HEAD_RADIUS );
		r2 *= r2;
		
		if ( x2 + y2 - r2 <= 0 )
		{			
			if ( tBallCenter.y > tHeadCenter.y )
			{
				KDfloat		fDiffX = tBallCenter.x - tHeadCenter.x;
				if ( fDiffX == 0 )
				{
					if ( m_tBallMovementDelta.y < 0 )
					{
						m_tBallMovementDelta = ccp ( m_tBallMovementDelta.x, -m_tBallMovementDelta.y );
					}
				}
				else 
				{

					KDfloat		fDiffY = tBallCenter.y - tHeadCenter.y;
					KDfloat		fSlope = kdFabsf ( fDiffY / fDiffX );
									
					if ( fSlope < 1 )
					{
						if ( tBallCenter.x < tPlayerCenter.x )
						{
							m_tBallMovementDelta = ccp ( -( BALL_MOVE_X / 2 ) * BALL_MOVE_X, -BALL_MOVE_Y );
						}
						else
						{
							m_tBallMovementDelta = ccp ( ( BALL_MOVE_X / 2 ) * BALL_MOVE_X, -BALL_MOVE_Y );
						}
					}
					else if ( fSlope == 1 )
					{							
						if ( tBallCenter.x < tPlayerCenter.x )
						{
							m_tBallMovementDelta = ccp ( -BALL_MOVE_X, -BALL_MOVE_Y );
						}
						else
						{
							m_tBallMovementDelta = ccp ( BALL_MOVE_X, BALL_MOVE_Y );
						}
					}
					else
					{
						if ( tBallCenter.x < tPlayerCenter.x )
						{
							m_tBallMovementDelta = ccp ( -BALL_MOVE_X, -( BALL_MOVE_Y / 2 ) * BALL_MOVE_Y );
						}
						else
						{
							m_tBallMovementDelta = ccp ( BALL_MOVE_X, -( BALL_MOVE_Y / 2 ) * BALL_MOVE_Y );
						}
					}					
				}				
			}			
		}
		else
		{
			// heading under head
			if ( tBallCenter.x > tPlayerCenter.x  && m_tBallMovementDelta.x < 0 )
			{
				m_tBallMovementDelta = ccp ( -m_tBallMovementDelta.x, m_tBallMovementDelta.y );
			}
			else if ( tBallCenter.x < tPlayerCenter.x && m_tBallMovementDelta.x > 0 )
			{
				m_tBallMovementDelta = ccp ( -m_tBallMovementDelta.x, m_tBallMovementDelta.y );
			}
		}
	
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid TBGameScene::resetBallPosition ( KDvoid )
{
	m_bBallOverBasket = KD_FALSE;
	this->reorderChild ( m_pBall, 3 ); 

	m_pBall->setPosition ( ccp ( GameState->getScreenCenter ( ).x, GameState->getScreenSize ( ).cy + 20 ) );

	m_pBall->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 0.1f, 30.f ) ) );

	m_pBall->runAction 
	(
		CCSequence::create 
		(
			CCMoveBy::create ( 1 / 60.f, m_tBallMovementDelta ),
			CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateBallPosition ) ),
			KD_NULL
		) 
	);
}

KDvoid TBGameScene::updateBallPosition ( KDvoid )
{
	if ( m_bBallOverBasket )
	{
		return;
	}
	
	KDint	nOverBasket = this->detectOverBasket ( );

	if ( nOverBasket == 0 )
	{
		this->detectBallBoundaryCheck ( );
		this->detectBallPlayerCollision ( m_pComputer );
		this->detectBallPlayerCollision ( m_pPlayer );

		m_pBall->runAction 
		(
			CCSequence::create 
			(
				CCMoveBy::create ( 1 / 60.f, m_tBallMovementDelta ),
				CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateBallPosition ) ),
				KD_NULL
			) 
		);
	}
	else 
	{
		m_bBallOverBasket = KD_TRUE;
		this->reorderChild ( m_pBall, 1 ); 

		m_pBall->stopAllActions ( );

		if ( nOverBasket > 0 )
		{
			m_nPlayerScore++;
		
			// score player
			m_tBallMovementDelta = ccp ( BALL_MOVE_X, -BALL_MOVE_Y );
			m_pBall->runAction 
			(
				CCSequence::create 
				(
					CCMoveTo::create ( 0.1f, ccp ( m_pRightBasket->getPosition ( ).x, m_pRightBasket->getPosition ( ).y + 70 ) ),
					CCMoveBy::create ( 0.6f, ccp ( 0, -80 ) ),
					CCMoveBy::create ( 0.1f, ccp ( nOverBasket * 10, -20 ) ),
					CCMoveBy::create ( 0.25f, ccp ( nOverBasket * 50, 0 ) ),
					CCCallFunc::create ( this, callfunc_selector ( TBGameScene::resetBallPosition ) ),
					KD_NULL
				) 
			);					
		}
		else 
		{
			m_nComputerScore++;
			m_tBallMovementDelta = ccp ( -BALL_MOVE_X, -BALL_MOVE_Y );
			m_pBall->runAction 
			(
				CCSequence::create 
				(
					CCMoveTo::create ( 0.1f, ccp ( m_pLeftBasket->getPosition ( ).x, m_pLeftBasket->getPosition ( ).y + 70 ) ),
					CCMoveBy::create ( 0.6f, ccp ( 0, -80 ) ),
					CCMoveBy::create ( 0.1f, ccp ( nOverBasket * 10, -20 ) ),
					CCMoveBy::create ( 0.25f, ccp ( nOverBasket * 50, 0 ) ),
					CCCallFunc::create ( this, callfunc_selector ( TBGameScene::resetBallPosition ) ),
					KD_NULL
				) 
			);	
		}
				
		// animate ball to go through the pipe
		// update points based on left/right pipe	
	}
}

KDvoid TBGameScene::updateComputerMovements ( KDvoid )
{
	if ( !m_bComputerJumping )
	{
		if ( m_pBall->getPosition ( ).x + m_fBallRadius <= m_pComputer->getPosition ( ).x )
		{
			m_pComputer->setPosition ( ccp ( m_pComputer->getPosition ( ).x - PLAYER_MOVE_X, m_pComputer->getPosition ( ).y ) );
			if ( m_pComputer->getPosition ( ).x <= 60 )
			{
				m_pComputer->setPosition ( ccp ( 60, m_pComputer->getPosition ( ).y ) );
			}
		}		
		else if ( m_pBall->getPosition ( ).x - m_fBallRadius >= m_pComputer->getPosition ( ).x )
		{
			m_pComputer->setPosition ( ccp ( m_pComputer->getPosition ( ).x + PLAYER_MOVE_X, m_pComputer->getPosition ( ).y ) );
			if ( m_pComputer->getPosition ( ).x >= ( GameState->getScreenSize ( ).cx - 60 ) )
			{
				m_pComputer->setPosition ( ccp ( GameState->getScreenSize ( ).cx - 60, m_pComputer->getPosition ( ).y ) );
			}				
		}
		else if ( kdFabsf ( m_pComputer->getPosition ( ).y - m_pBall->getPosition ( ).y ) < ( m_pComputer->getPosition ( ).y + PLAYER_JUMP_HEIGHT )
			 && ( m_pBall->getPosition ( ).x <= m_pComputer->getPosition ( ).x ) )
		{
			m_bComputerJumping = KD_TRUE;
			
			m_pComputer->runAction 
			(
				CCSequence::create 
				(
					CCJumpBy::create ( PLAYER_JUMP_SPEED, ccp ( 0, 0 ), PLAYER_JUMP_HEIGHT, 1 ),
					CCCallFunc::create ( this, callfunc_selector ( TBGameScene::computerLanded ) ),
					KD_NULL
				)
			);
		}
	}
}

KDvoid TBGameScene::updateTimer ( KDfloat fDelta )
{
	if ( !m_bPause )
	{
		m_nTimer--;
		m_pPlayerScoreLabel->setString ( ccszf ( "%d", m_nPlayerScore ) );
		m_pComputerScoreLabel->setString ( ccszf ( "%d", m_nComputerScore ) );

		KDint	nMinutes = m_nTimer % 60;
		if ( nMinutes >= 10 )
		{
			m_pTimerLabel->setString ( ccszf ( "%d:%d", m_nTimer / 60, nMinutes ) );
		}
		else
		{
			m_pTimerLabel->setString ( ccszf ( "%d:0%d", m_nTimer / 60, nMinutes ) );
		}

		if ( m_nTimer > 0 )
		{
			
		}
		else 
		{			
			this->gameOver ( );
		}
	}
}

KDvoid TBGameScene::playerLanded ( KDvoid )
{
	m_pPlayer->stopAllActions ( );
	m_pPlayer->setScale ( 1.0f );
	m_pPlayer->setPosition ( ccp ( m_pPlayer->getPosition ( ).x, PLAYER_START_Y ) );

	m_bPlayerJumping = KD_FALSE;
}

KDvoid TBGameScene::computerLanded ( KDvoid )
{
	m_pComputer->stopAllActions ( );
	m_pComputer->setScale ( 1.0f );
	m_pComputer->setPosition ( ccp ( m_pComputer->getPosition ( ).x, PLAYER_START_Y ) );

	m_bComputerJumping = KD_FALSE;
}

KDvoid TBGameScene::resumeClicked ( CCObject* pSender )
{
	GameState->playClick ( );

	m_pBackMenu->runAction ( CCMoveTo::create ( 0.5f, ccp ( 0, 0 ) ) ); 

	this->removeChild ( m_pPauseBg, KD_TRUE );
	m_bPause = KD_FALSE;
	m_pBall->setOpacity ( 255 );

	if ( m_bBallOverBasket )
	{
		this->resetBallPosition ( );
	}
		
	if ( !m_bShowingGoLabel )
	{
		m_pBall->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 0.1f, 30.f ) ) );

		m_pBall->runAction 
		(
			CCSequence::create 
			(
				CCMoveBy::create ( 1 / 60.f, m_tBallMovementDelta ),
				CCCallFunc::create ( this, callfunc_selector ( TBGameScene::updateBallPosition ) ),
				KD_NULL
			) 
		);
	}
}

KDvoid TBGameScene::restartClicked ( CCObject* pSender )
{
	GameState->playClick ( );

	m_pBackMenu->runAction ( CCMoveTo::create ( 0.5f, ccp ( 0, 0 ) ) ); 

	if ( m_bGameOver )
	{
		this->removeChild ( m_pGameOverBg, KD_TRUE );
	}

	if ( m_bPause )
	{
		this->removeChild ( m_pPauseBg, KD_TRUE );
	}

	m_pBall->setOpacity ( 0 );
	m_pBall->stopAllActions ( );
	GameState->stopGamemusic ( );

	m_pPlayer->setScale ( 1.0f );
	m_pComputer->setScale ( 1.0f );
	m_pPlayer->setPosition ( ccp ( GameState->getScreenCenter ( ).x - 40, PLAYER_START_Y ) );
	m_pComputer->setPosition ( ccp ( GameState->getScreenCenter ( ).x + 40, PLAYER_START_Y ) );

	this->showGoLabel ( );
}

KDvoid TBGameScene::shareClicked ( CCObject* pSender )
{
	GameState->playClick ( );
}

CCLayer* TBGameScene::addScores ( const CCPoint& tCenter )
{
	CCLayer*	pLayer = CCLayer::create ( );
	
	if ( m_nPlayerScore < 10 )
	{
		CCSprite*	pSprite = CCSprite::create ( ccszf ( "%d.png", m_nPlayerScore ) );
		pLayer->addChild ( pSprite );
		pSprite->setPosition ( tCenter );
	}
	else if ( m_nPlayerScore < 100 )
	{
		CCSprite*	pSprite1 = CCSprite::create ( ccszf ( "%d.png", m_nPlayerScore / 10 ) );
		pLayer->addChild ( pSprite1 );
		pSprite1->setPosition ( ccp ( tCenter.x - 20, tCenter.y ) );

		CCSprite*	pSprite2 = CCSprite::create ( ccszf ( "%d.png", m_nPlayerScore % 10 ) );
		pLayer->addChild ( pSprite2 );
		pSprite2->setPosition ( ccp ( tCenter.x + 20, tCenter.y ) );	
	}
	else if ( m_nPlayerScore < 1000 )
	{
		CCSprite*	pSprite2 = CCSprite::create ( ccszf ( "%d.png", m_nPlayerScore % 10 ) );
		pLayer->addChild ( pSprite2 );
		pSprite2->setPosition ( ccp ( tCenter.x + 40, tCenter.y ) );

		m_nPlayerScore /= 10;

		CCSprite*	pSprite1 = CCSprite::create ( ccszf ( "%d.png", m_nPlayerScore / 10 ) );
		pLayer->addChild ( pSprite1 );
		pSprite1->setPosition ( ccp ( tCenter.x - 40, tCenter.y ) );	

		CCSprite*	pSprite3 = CCSprite::create ( ccszf ( "%d.png", m_nPlayerScore % 10 ) );
		pLayer->addChild ( pSprite3 );
		pSprite3->setPosition ( ccp ( tCenter.x, tCenter.y ) );	
	}

	return pLayer;
}
