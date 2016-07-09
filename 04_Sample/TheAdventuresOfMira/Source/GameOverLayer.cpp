/* --------------------------------------------------------------------------
 *
 *      File            GameOverLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "GameOverLayer.h"

// Initialize our Main Menu Layer, and add the initial main menu with the title screen, play button and about button.
KDbool GameOverLayer::init ( KDvoid )
{
    // Initialize the parent
    if ( !CCLayer::init ( ) )		
	{
		return KD_FALSE;
	}
    
    // Load the high scores
    CCUserDefault*	pStore = CCUserDefault::sharedUserDefault ( );
    KDint			nBestScore = pStore->getIntegerForKey ( "best-score", 0 );
    KDint			nBestDistance = pStore->getIntegerForKey ( "best-distance", 0 );
    KDint			nBestDifficulty = pStore->getIntegerForKey ( "best-difficulty", 0 );
    KDbool			bNewRecord = KD_FALSE;

	GameManager*	pGameManager = GameManager::sharedGameManager ( );
	DeviceType		eDeviceType  = pGameManager->getDeviceType ( );
	DeviceSize		eDeviceSize  = pGameManager->getDeviceSize ( );

    if ( pGameManager->getScore ( ) > nBestScore && pGameManager->getDistance ( ) > nBestDistance )
    {
        nBestScore = pGameManager->getScore ( );
        nBestDistance = pGameManager->getDistance ( );
        nBestDifficulty = pGameManager->getDifficulty ( );
        pStore->setIntegerForKey ( "best-score", nBestScore );
        pStore->setIntegerForKey ( "best-distance", nBestDistance );
        pStore->setIntegerForKey ( "best-difficulty", nBestDifficulty );
        pStore->flush ( );
        bNewRecord = KD_TRUE;
    }
    
    // Set the screen size to the full dimensions of the device ( full screen )
    this->setContentSize ( SCREEN_SIZE );
    
    KDfloat			fLabelScaleText = 1.2f;
    if ( eDeviceType == kDeviceTypeAndroid854x480 ||
         eDeviceType == kDeviceTypeAndroid800x480 ||
         eDeviceType == kDeviceTypeAndroid800x400 ||
         eDeviceType == kDeviceTypeAndroid720x480 )
    {
        fLabelScaleText = 0.9f;
    }
    else if ( eDeviceSize == kDeviceSizeNormal )
    {
        fLabelScaleText = 1.0f;
    }
    else if ( eDeviceSize == kDeviceSizeSmall )
    {
        fLabelScaleText = 0.5f;
    }
    
    CCSprite*		pGameOverTitle = CCSprite::createWithSpriteFrameName ( "game_over.png" );
    pGameOverTitle->setScale ( 0.9f );
    pGameOverTitle->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.8 ) );
    this->addChild ( pGameOverTitle );
    
    CCLabelBMFont*	pScoreLabel = CCLabelBMFont::create ( ccszf ( "YOUR SCORE: %d", pGameManager->getScore ( ) ), "myGlyphs.fnt" );
    pScoreLabel->setScale ( fLabelScaleText );
    pScoreLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.65 ) );
    this->addChild ( pScoreLabel );
    
    CCLabelBMFont*	pDistanceLabel = CCLabelBMFont::create ( ccszf ( "YOUR DISTANCE: %d", pGameManager->getDistance ( ) ), "myGlyphs.fnt" );
    pDistanceLabel->setScale ( fLabelScaleText );
    pDistanceLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.59 ) );
    this->addChild ( pDistanceLabel );
    
    const KDchar*	szdifficulty;
    switch ( pGameManager->getDifficulty ( ) )
    {
        case 3  :	szdifficulty = "HARD";		break;
        case 2  :	szdifficulty = "MEDIUM";	break;
        default :	szdifficulty = "EASY";
    }
    
    CCLabelBMFont*	pDifficultyLabel = CCLabelBMFont::create ( ccszf ( "YOUR DIFFICULTY: %s", szdifficulty ), "myGlyphs.fnt" );
    pDifficultyLabel->setScale ( fLabelScaleText );
    pDifficultyLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.53 ) );
    this->addChild ( pDifficultyLabel );
    
    CCLabelBMFont*	pHighScoreLabel = CCLabelBMFont::create ( ccszf ( "HIGH SCORE: %d", nBestScore ), "myGlyphs.fnt" );
    pHighScoreLabel->setScale ( fLabelScaleText );
    pHighScoreLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.43 ) );
    this->addChild ( pHighScoreLabel );
    
    CCLabelBMFont*	pHighDistanceLabel = CCLabelBMFont::create ( ccszf ( "BEST DISTANCE: %d", nBestDistance ), "myGlyphs.fnt" );
    pHighDistanceLabel->setScale ( fLabelScaleText );
    pHighDistanceLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.37 ) );
    this->addChild ( pHighDistanceLabel );
    
    switch ( nBestDifficulty )
    {
        case 3  :	szdifficulty = "HARD";		break;
        case 2  :	szdifficulty = "MEDIUM";	break;
        default :	szdifficulty = "EASY";
    }
    
    CCLabelBMFont*	pHighDifficultyLabel = CCLabelBMFont::create ( ccszf ( "DIFFICULTY: %s", szdifficulty ), "myGlyphs.fnt" );
    pHighDifficultyLabel->setScale ( fLabelScaleText );
    pHighDifficultyLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.31 ) );
    this->addChild ( pHighDifficultyLabel );
    
    // Play again button
    this->createPlayAgainButton ( );
    
    // Play Game Over Sound
    SimpleAudioEngine::sharedEngine ( )->setEffectsVolume ( 1.0f );
    SimpleAudioEngine::sharedEngine ( )->playEffect ( "sfx_gameover.mp3", KD_FALSE );
    
    // Do new record animation
    if ( bNewRecord )
    {
        // TODO
    }
    
    // Initialization was successful
    return KD_TRUE;
}

KDvoid GameOverLayer::createPlayAgainButton ( KDvoid )
{
    // Create the startButton
    CCSprite*			pPlayAgainButtonSpriteUp   = CCSprite::createWithSpriteFrameName ( "playagain_button.png" );
    CCSprite*			pPlayAgainButtonSpriteDown = CCSprite::createWithSpriteFrameName ( "playagain_button.png" );
    // Scale the down to 90% so that it looks like the button was pressed
    pPlayAgainButtonSpriteDown->setScale ( 0.9f );
    CCMenuItemSprite*	pPlayAgainButton = CCMenuItemSprite::create ( pPlayAgainButtonSpriteUp, pPlayAgainButtonSpriteDown, this, menu_selector ( GameOverLayer::playAgainButtonTouched ) );
    pPlayAgainButton->setPositionX ( -pPlayAgainButton->getContentSize ( ).width / 1.2f );
    pPlayAgainButton->runAction 
	(
		CCRepeatForever::create 
		(
			CCSequence::createWithTwoActions 
			(
				CCScaleTo::create ( 1.0f, 1.2f ), 
				CCScaleTo::create ( 1.0f, 1.0f ) 
			) 
		) 
	);
    
    // Create the mainButton
    CCSprite*			pMainButtonSpriteUp   = CCSprite::createWithSpriteFrameName ( "main_button.png" );
    CCSprite*			pMainButtonSpriteDown = CCSprite::createWithSpriteFrameName ( "main_button.png" );
    // Scale the down to 90% so that it looks like the button was pressed
    pMainButtonSpriteDown->setScale ( 0.9f );
    CCMenuItemSprite*	pMainButton = CCMenuItemSprite::create ( pMainButtonSpriteUp, pMainButtonSpriteDown, this, menu_selector ( GameOverLayer::mainButtonTouched ) );
    pMainButton->setPositionX ( pMainButton->getContentSize ( ).width / 1.2f );
    
    // Create the start game menu
    CCMenu*				pGameMenu = CCMenu::create ( pPlayAgainButton, pMainButton, KD_NULL );
    // Set the initial position off screen
    pGameMenu->setPosition ( ccp ( SCREEN_WIDTH / 2, -SCREEN_HEIGHT ) );
    // Set the tag of the start menu so that we can refer to it later using getChildByTag ( ... )
    pGameMenu->setTag ( kGameOverLayer_PlayAgainMenu );
    
    // Create and run an animation that will slide the start game menu in from below the screen to its intended location
    CCActionInterval*	pMoveAction = CCMoveTo::create ( 0.3f, ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.15 ) );
    CCFiniteTimeAction*	pEaseIn = CCEaseIn::create ( pMoveAction, 0.2f );
    pGameMenu->runAction ( pEaseIn );
    
    // Add the start menu to the layer
    this->addChild ( pGameMenu );
}

KDvoid GameOverLayer::playAgainButtonTouched ( CCObject* pSender )
{
    GameManager::sharedGameManager ( )->startNewGame ( );
}

KDvoid GameOverLayer::mainButtonTouched ( CCObject* pSender )
{
    this->getChildByTag ( kGameOverLayer_PlayAgainMenu )->stopAllActions ( );

    GameManager::sharedGameManager ( )->showMainMenu ( );
}
