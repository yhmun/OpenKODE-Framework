/* --------------------------------------------------------------------------
 *
 *      File            SimpleTimedGameRecipe.cpp
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
#include "SimpleTimedGameRecipe.h"

SimpleTimedGameRecipe* SimpleTimedGameRecipe::create ( KDvoid )
{
	SimpleTimedGameRecipe*		pRet = new SimpleTimedGameRecipe ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool SimpleTimedGameRecipe::init ( KDvoid )
{
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	this->setTouchEnabled ( KD_TRUE );

	CCMenuItemFont::setFontSize ( 20 );
	
	m_nGameState = GAME_OVER;
	m_fGameTimer = 0;
	m_nHiScore	 = 0;
	m_pHiScores  = KD_NULL;
	m_nCurrentScore = 0;

	this->loadHiScores ( );

	m_sCurrentPlayerName = "Player1";

	m_pDarkSprite = CCSprite::create ( "blank.png" );
	m_pDarkSprite->setTextureRect ( CCRect ( 0, 0, 480, 320 ) );
	m_pDarkSprite->setPosition ( ccp ( 240, 160 ) );
	m_pDarkSprite->setColor ( ccc3 ( 0, 0, 0 ) );
	m_pDarkSprite->setOpacity ( 100 );
	m_pDarkSprite->setVisible ( KD_TRUE );
	this->addChild ( m_pDarkSprite, Z_TIMED_GAME_HUD );

	m_pMessage->setColor ( ccc3 ( 255, 0, 0 ) );

	CCLabelBMFont*	pHiScoreLabel = CCLabelBMFont::create ( "HI SCORE:", "eurostile_30.fnt" );
	pHiScoreLabel->setPosition ( ccp ( 330, 300 ) );
	pHiScoreLabel->setScale ( 0.5f );
	this->addChild ( pHiScoreLabel, Z_TIMED_GAME_HUD );

	m_pHiScoreNumberLabel = CCLabelBMFont::create ( "", "eurostile_30.fnt" );
	m_pHiScoreNumberLabel->setPosition ( ccp ( 330, 280 ) );
	m_pHiScoreNumberLabel->setScale ( 0.5f );
	this->addChild ( m_pHiScoreNumberLabel, Z_TIMED_GAME_HUD );


	CCLabelBMFont*	pCurrentScoreLabel = CCLabelBMFont::create ( "YOUR SCORE:", "eurostile_30.fnt" );
	pCurrentScoreLabel->setPosition ( ccp ( 430, 300 ) );
	pCurrentScoreLabel->setScale ( 0.5f );
	this->addChild ( pCurrentScoreLabel, Z_TIMED_GAME_HUD );

	m_pCurrentScoreNumberLabel = CCLabelBMFont::create ( "", "eurostile_30.fnt" );
	m_pCurrentScoreNumberLabel->setPosition ( ccp ( 430, 280 ) );
	m_pCurrentScoreNumberLabel->setScale ( 0.5f );
	this->addChild ( m_pCurrentScoreNumberLabel, Z_TIMED_GAME_HUD );


	CCLabelBMFont*	pGameTimeLabel = CCLabelBMFont::create ( "TIME LEFT:", "eurostile_30.fnt" );
	pGameTimeLabel->setPosition ( ccp ( 430, 250 ) );
	pGameTimeLabel->setScale ( 0.5f );
	this->addChild ( pGameTimeLabel, Z_TIMED_GAME_HUD );

	m_pGameTimeNumberLabel = CCLabelBMFont::create ( "", "eurostile_30.fnt" );
	m_pGameTimeNumberLabel->setPosition ( ccp ( 430, 230 ) );
	m_pGameTimeNumberLabel->setScale ( 0.5f );
	this->addChild ( m_pGameTimeNumberLabel, Z_TIMED_GAME_HUD );

	
	CCLabelBMFont*	pPlayerNameLabel = CCLabelBMFont::create ( "YOUR NAME:", "eurostile_30.fnt" );
	pPlayerNameLabel->setPosition ( ccp ( 430, 200 ) );
	pPlayerNameLabel->setScale ( 0.5f );
	this->addChild ( pPlayerNameLabel, Z_TIMED_GAME_HUD );

	m_pPlayerNameStringLabel = CCLabelBMFont::create ( "", "eurostile_30.fnt" );
	m_pPlayerNameStringLabel->setPosition ( ccp ( 430, 170 ) );
	m_pPlayerNameStringLabel->setScale ( 0.5f );
	this->addChild ( m_pPlayerNameStringLabel, Z_TIMED_GAME_HUD );


	CCMenuItemFont*		pNewGameMIF = CCMenuItemFont::create ( "NEW GAME", this, menu_selector ( SimpleTimedGameRecipe::startNewGame ) );
	CCMenuItemFont*		pChooseNameMIF = CCMenuItemFont::create ( "CHOOSE GAME", this, menu_selector ( SimpleTimedGameRecipe::showNames ) );
	CCMenuItemFont*		pViewHiScoresMIF = CCMenuItemFont::create ( "VIEW HI SCORES", this, menu_selector ( SimpleTimedGameRecipe::viewHiScores ) );
	CCMenuItemFont*		pDeleteHiScoresMIF = CCMenuItemFont::create ( "DELETE HI SCORES", this, menu_selector ( SimpleTimedGameRecipe::deleteHiScores ) );

	m_pMainMenu = CCMenu::create ( pNewGameMIF, pChooseNameMIF, pViewHiScoresMIF, pDeleteHiScoresMIF, KD_NULL );
	m_pMainMenu->alignItemsVertically ( );
	m_pMainMenu->setPosition ( ccp ( 240, 140 ) );
    this->addChild ( m_pMainMenu, Z_TIMED_GAME_HUD );


	CCMenuItemFont*		pName1 = CCMenuItemFont::create ( "Player 1", this, menu_selector ( SimpleTimedGameRecipe::chooseName1 ) );
	CCMenuItemFont*		pName2 = CCMenuItemFont::create ( "Player 2", this, menu_selector ( SimpleTimedGameRecipe::chooseName2 ) );
	CCMenuItemFont*		pName3 = CCMenuItemFont::create ( "Player 3", this, menu_selector ( SimpleTimedGameRecipe::chooseName3 ) );
		
	m_pNamesToChoose = CCMenu::create ( pName1, pName2, pName3, KD_NULL );
	m_pNamesToChoose->alignItemsVertically ( );
    m_pNamesToChoose->setPosition ( ccp ( 240, 180 ) );
	m_pNamesToChoose->setVisible ( KD_FALSE );
	this->addChild ( m_pNamesToChoose, Z_TIMED_GAME_HUD );
				
	this->setCurrentScore ( );
	
	this->setHiScore ( );

	this->setPlayerName ( );
		
	this->schedule ( schedule_selector ( SimpleTimedGameRecipe::step ) );

	return KD_TRUE;
}

KDvoid SimpleTimedGameRecipe::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pHiScores );

	Recipe::onExit ( );
}

KDvoid SimpleTimedGameRecipe::step ( KDfloat fDelta )
{
	if ( m_nGameState == MID_GAME )
	{
		m_fGameTimer -= fDelta;
		m_pGameTimeNumberLabel->setString ( ccszf ( "%d", (KDint) m_fGameTimer ) );
	}
	
	if ( m_fGameTimer < 0 && m_nGameState == MID_GAME )
	{
		this->gameOver ( );
	}
}

KDvoid SimpleTimedGameRecipe::loadHiScores ( KDvoid )
{
	/// ABSTRACT 
}

KDvoid SimpleTimedGameRecipe::deleteHiScores ( CCObject* pSender )
{
	/// ABSTRACT 
}

KDvoid SimpleTimedGameRecipe::addHiScore ( KDvoid )
{
	/// ABSTRACT 
}

KDvoid SimpleTimedGameRecipe::viewHiScores ( CCObject* pSender )
{
	this->loadHiScores ( );

	this->removeChild ( m_pHiScoresMenu ); 
	
	m_pHiScoresMenu = CCMenu::create ( ); 
	this->addHiScoresToMenu ( );

	m_pHiScoresMenu->addChild ( CCMenuItemFont::create ( "<back>", this, menu_selector ( SimpleTimedGameRecipe::hideHiScores ) ) );
	m_pHiScoresMenu->alignItemsVertically ( );
	m_pHiScoresMenu->setPosition ( ccp ( 240, 180 ) );
    this->addChild ( m_pHiScoresMenu, Z_TIMED_GAME_HUD );

	m_pHiScoresMenu->setVisible ( KD_TRUE ); 
	m_pMainMenu->setVisible ( KD_FALSE );
}

KDvoid SimpleTimedGameRecipe::hideHiScores ( CCObject* pSender )
{
	m_pHiScoresMenu->setVisible ( KD_FALSE ); 
	m_pMainMenu->setVisible ( KD_TRUE );
}

KDvoid SimpleTimedGameRecipe::addHiScoresToMenu ( KDvoid )
{
	/// ABSTRACT 
}

KDvoid SimpleTimedGameRecipe::setHiScore ( KDvoid )
{
	m_pHiScoreNumberLabel->setString ( ccszf ( "%d", m_nHiScore ) );
}

KDvoid SimpleTimedGameRecipe::setCurrentScore ( KDvoid )
{
	m_pCurrentScoreNumberLabel->setString ( ccszf ( "%d", m_nCurrentScore ) );
	
	if ( m_nCurrentScore > m_nHiScore )
	{
		m_nHiScore = m_nCurrentScore;
		this->setHiScore ( );
	}
}

KDvoid SimpleTimedGameRecipe::setPlayerName ( KDvoid )
{
	m_pPlayerNameStringLabel->setString ( m_sCurrentPlayerName.c_str ( ) );
}

KDvoid SimpleTimedGameRecipe::startNewGame ( CCObject* pSender )
{
	m_fGameTimer = 30;
	m_nGameState = MID_GAME;
	m_nCurrentScore = 0;
		
	m_pDarkSprite->setVisible ( KD_FALSE );
	m_pMainMenu->setVisible ( KD_FALSE );

	this->setCurrentScore ( );
	
	this->setHiScore ( );

	this->setPlayerName ( );
}

KDvoid SimpleTimedGameRecipe::gameOver ( KDvoid )
{
	m_nGameState = GAME_OVER;

	m_pDarkSprite->setVisible ( KD_TRUE );
	m_pMainMenu->setVisible ( KD_TRUE );
	m_pNamesToChoose->setVisible ( KD_FALSE );
	
	m_pMessage->setString ( "GAME OVER!" );
		
	this->addHiScore ( );
}

KDvoid SimpleTimedGameRecipe::showNames ( CCObject* pSender )
{
	m_pMainMenu->setVisible ( KD_FALSE );
	m_pNamesToChoose->setVisible ( KD_TRUE );
}

KDvoid SimpleTimedGameRecipe::chooseName1 ( CCObject* pSender )
{
	this->chooseName ( "Player1" );
}	

KDvoid SimpleTimedGameRecipe::chooseName2 ( CCObject* pSender )
{
	this->chooseName ( "Player2" );
}	

KDvoid SimpleTimedGameRecipe::chooseName3 ( CCObject* pSender )
{
	this->chooseName ( "Player3" );
}

KDvoid SimpleTimedGameRecipe::chooseName ( const KDchar* szName )
{
	m_sCurrentPlayerName = szName;
	m_pMainMenu->setVisible ( KD_TRUE );
	m_pNamesToChoose->setVisible ( KD_FALSE );
	this->setPlayerName ( );
}
