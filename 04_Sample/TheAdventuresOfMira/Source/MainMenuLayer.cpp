/* --------------------------------------------------------------------------
 *
 *      File            MainMenuLayer.cpp
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
#include "MainMenuLayer.h"

// Initialize our Main Menu Layer, and add the initial main menu with the title screen, play button and about button.
KDbool MainMenuLayer::init ( KDvoid )
{
    // Initialize the parent
    if ( !CCLayer::init ( ) ) 
	{
		return KD_FALSE;
	}
    
    // Set the screen size to the full dimensions of the device ( full screen )
    this->setContentSize ( SCREEN_SIZE );
    
    // Create and show the Main Menu
    this->addMainMenuChildren ( );
    
    // Initialization was successful
    return KD_TRUE;
}

// Create, initialize, add to layer and perform the initial animation for the Main Menu.
KDvoid MainMenuLayer::addMainMenuChildren ( KDvoid )
{
    // Create and initialze the game title sprite
    CCSprite*	pTitle = ( CCSprite::createWithSpriteFrameName ( "game_title.png" ) );
    // Set the tag of the game title so that we can refer to it later using getChildByTag ( ... )
    pTitle->setTag ( kMainMenuLayer_TitleSprite );
    // Set the initial position of the game title
    pTitle->setPosition ( ccp ( SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.6 ) );
    pTitle->runAction 
	(
		CCRepeatForever::create 
		(
			CCSequence::createWithTwoActions
			(
				CCScaleTo::create ( 1.0f, 1.1f ),
				CCScaleTo::create ( 1.0f, 1.0f ) 
			)
		)
	);
    
    // Create the playButton
    CCSprite*			pPlayButtonSpriteUp   = CCSprite::createWithSpriteFrameName ( "play_button.png" );
    CCSprite*			pPlayButtonSpriteDown = CCSprite::createWithSpriteFrameName ( "play_button.png" );
    // Scale the down to 90% so that it looks like the button was pressed
    pPlayButtonSpriteDown->setScale ( 0.9f );
    CCMenuItemSprite*	pPlayGameButton = CCMenuItemSprite::create ( pPlayButtonSpriteUp, pPlayButtonSpriteDown, this, menu_selector ( MainMenuLayer::playGameButtonTouched ) );
    
    // Create the aboutButton
    CCSprite*			pAboutButtonSpriteUp   = CCSprite::createWithSpriteFrameName ( "about_button.png" );
    CCSprite*			pAboutButtonSpriteDown = CCSprite::createWithSpriteFrameName ( "about_button.png" );
    // Scale the down to 90% so that it looks like the button was pressed
    pAboutButtonSpriteDown->setScale ( 0.9f );
    CCMenuItemSprite*	pAboutGameButton = CCMenuItemSprite::create ( pAboutButtonSpriteUp, pAboutButtonSpriteDown, this, menu_selector ( MainMenuLayer::aboutGameButtonTouched ) );
    
    // Create the main menu and add the buttons to it
    CCMenu*				pGameMenu = CCMenu::create ( pPlayGameButton, pAboutGameButton, KD_NULL );
    pGameMenu->alignItemsHorizontallyWithPadding ( pPlayButtonSpriteUp->getContentSize ( ).width / 1.2f );
    pGameMenu->setPosition ( ccp ( SCREEN_SIZE.width * 0.7f, -pPlayButtonSpriteUp->getContentSize ( ).height * 2 ) );
    // Set the tag of the main menu so that we can refer to it later using getChildByTag ( ... )
    pGameMenu->setTag ( kMainMenuLayer_MainMenu );
    
    // Add the title to the layer
    this->addChild ( pTitle );
    
	// Add the main menu to the layer
    this->addChild ( pGameMenu );
    
    // Create an action that makes the main menu animate coming up from below the screen to its intended position
    CCActionInterval*	pMoveAction = CCMoveTo::create ( 1.0f, ccp ( SCREEN_SIZE.width * 0.7f, SCREEN_SIZE.height * 0.15f ) );
    CCActionInterval*	pEaseAction = CCEaseIn::create ( pMoveAction, 1.0f );
    pGameMenu->runAction ( pEaseAction );
}

// Animate the main menu items to slide out and remove the main menu children from the layer and cleanup.
KDvoid MainMenuLayer::removeMainMenuChildren ( KDvoid )
{
    // Find the game title sprite and create an animation where it slides out the right of the screen
    CCSprite*			pTitle		 = (CCSprite*) this->getChildByTag ( kMainMenuLayer_TitleSprite );
    CCActionInterval*	pMoveAction  = CCMoveTo::create ( 0.3f, ccp ( SCREEN_WIDTH + 100, pTitle->getPosition ( ).y ) );
    CCFiniteTimeAction*	pEaseOut	 = CCEaseOut::create ( pMoveAction, 0.2f );
    CCFiniteTimeAction*	pRemoveTitle = CCCallFunc::create ( this, callfunc_selector ( MainMenuLayer::removeGameTitle ) );
    CCSequence*			pTitleOut	 = CCSequence::createWithTwoActions ( pEaseOut, pRemoveTitle );
    pTitle->runAction ( pTitleOut );
    
    // Find the main menu and create an animation where it slides out the right of the screen
    CCMenu*				pGameMenu	  = (CCMenu*) this->getChildByTag ( kMainMenuLayer_MainMenu );
    CCActionInterval*	pMmoveAction  = CCMoveTo::create ( 0.3f, ccp ( SCREEN_WIDTH + 100, pGameMenu->getPosition ( ).y ) );
    CCFiniteTimeAction*	pMeaseOut	  = CCEaseOut::create ( pMmoveAction, 0.2f );
    CCFiniteTimeAction*	pMremoveTitle = CCCallFunc::create ( this, callfunc_selector ( MainMenuLayer::removeGameMenu ) );
    CCSequence*			pMtitleOut	  = CCSequence::createWithTwoActions ( pMeaseOut, pMremoveTitle );
    pGameMenu->runAction ( pMtitleOut );
}

// Removes the MainMenuTitle from the layer.
KDvoid MainMenuLayer::removeGameTitle ( KDvoid )
{
    // Remove the game title from the layer
    this->removeChildByTag ( kMainMenuLayer_TitleSprite, true );
}

// Removes the MainMenu from the layer.
KDvoid MainMenuLayer::removeGameMenu ( KDvoid )
{
    // Remove the main menu from the layer
    this->removeChildByTag ( kMainMenuLayer_MainMenu, true );
    
    // Add the start button to the layer
    addStartMenuChildren ( );
}

// Create, initialize, add to layer and perform initial animation to display the start button.
KDvoid MainMenuLayer::addStartMenuChildren ( KDvoid )
{
    // Create the Easy Game Button
    CCSprite*			pEasyButtonSpriteUp   = CCSprite::createWithSpriteFrameName ( "easy_button.png" );
    CCSprite*			pEasyButtonSpriteDown = CCSprite::createWithSpriteFrameName ( "easy_button.png" );
    // Scale the down to 90% so that it looks like the button was pressed
    pEasyButtonSpriteDown->setScale ( 0.9f );
    CCMenuItemSprite*	pEasyGameButton = CCMenuItemSprite::create ( pEasyButtonSpriteUp, pEasyButtonSpriteDown, this, menu_selector ( MainMenuLayer::easyGameButtonTouched ) );
    pEasyGameButton->setTag ( kButtonDifficultyEasy );
    pEasyGameButton->setPositionX ( -pEasyGameButton->getContentSize ( ).width - ( pEasyGameButton->getContentSize ( ).width * 0.1f ) );
    
    // Create the Medium Game Button
    CCSprite*			pMediumButtonSpriteUp   = CCSprite::createWithSpriteFrameName ( "medium_button.png" );
    CCSprite*			pMediumButtonSpriteDown = CCSprite::createWithSpriteFrameName ( "medium_button.png" );
    // Scale the down to 90% so that it looks like the button was pressed
    pMediumButtonSpriteDown->setScale ( 0.9f );
    CCMenuItemSprite*	pMediumGameButton = CCMenuItemSprite::create ( pMediumButtonSpriteUp, pMediumButtonSpriteDown, this, menu_selector ( MainMenuLayer::mediumGameButtonTouched ) );
    pMediumGameButton->setTag ( kButtonDifficultyMedium );
    
    // Create the Hard Game Button
    CCSprite*			pHardButtonSpriteUp   = CCSprite::createWithSpriteFrameName ( "hard_button.png" );
    CCSprite*			pHardButtonSpriteDown = CCSprite::createWithSpriteFrameName ( "hard_button.png" );
    // Scale the down to 90% so that it looks like the button was pressed
    pHardButtonSpriteDown->setScale ( 0.9f );
    CCMenuItemSprite*	pHardGameButton = CCMenuItemSprite::create ( pHardButtonSpriteUp, pHardButtonSpriteDown, this, menu_selector ( MainMenuLayer::hardGameButtonTouched ) );
    pHardGameButton->setTag ( kButtonDifficultyHard );
    pHardGameButton->setPositionX ( pHardGameButton->getContentSize ( ).width + ( pHardGameButton->getContentSize ( ).width * 0.2f ) );
    
    // Create the start game menu
    CCMenu*				pGameMenu = CCMenu::create ( pEasyGameButton, pMediumGameButton, pHardGameButton, KD_NULL );
    // Set the initial position off screen
    pGameMenu->setPosition ( ccp ( SCREEN_WIDTH / 2, -SCREEN_HEIGHT ) );
    // Set the tag of the start menu so that we can refer to it later using getChildByTag ( ... )
    pGameMenu->setTag ( kMainMenuLayer_StartGameMenu );

    // Create and run an animation that will slide the start game menu in from below the screen to its intended location
    CCActionInterval*	pMoveAction = CCMoveTo::create ( 0.3f, ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ) );
    CCFiniteTimeAction*	pEaseIn = CCEaseIn::create ( pMoveAction, 0.2f );    
    pGameMenu->runAction ( pEaseIn );
    
    // Add the start menu to the layer
    this->addChild ( pGameMenu );
}

// Animate the start menu item to slide out and remove the start menu children from the layer and cleanup.
KDvoid MainMenuLayer::removeStartMenuChildren ( KDvoid )
{
    // Find our start menu and run an animation to make it slide out to the right, then call a method to dispose of the start menu
    CCMenu*		pGameMenu = (CCMenu*) this->getChildByTag ( kMainMenuLayer_StartGameMenu );
    if ( GameManager::sharedGameManager ( )->getGameDifficulty ( ) == kGameDifficultyEasy )
    {
        pGameMenu->getChildByTag ( kButtonDifficultyEasy	)->setVisible ( KD_TRUE  );
        pGameMenu->getChildByTag ( kButtonDifficultyMedium	)->setVisible ( KD_FALSE );
        pGameMenu->getChildByTag ( kButtonDifficultyHard	)->setVisible ( KD_FALSE );
    }
    else if ( GameManager::sharedGameManager ( )->getGameDifficulty ( ) == kGameDifficultyMedium )
    {
        pGameMenu->getChildByTag ( kButtonDifficultyEasy	)->setVisible ( KD_FALSE );
        pGameMenu->getChildByTag ( kButtonDifficultyMedium	)->setVisible ( KD_TRUE  );
        pGameMenu->getChildByTag ( kButtonDifficultyHard	)->setVisible ( KD_FALSE );
    }
    else if ( GameManager::sharedGameManager ( )->getGameDifficulty ( ) == kGameDifficultyHard )
    {
        pGameMenu->getChildByTag ( kButtonDifficultyEasy	)->setVisible ( KD_FALSE );
        pGameMenu->getChildByTag ( kButtonDifficultyMedium	)->setVisible ( KD_FALSE );
        pGameMenu->getChildByTag ( kButtonDifficultyHard	)->setVisible ( KD_TRUE  );
    }
    CCActionInterval*		pMmoveAction = CCMoveTo::create ( 0.3f, ccp ( SCREEN_WIDTH + 100, pGameMenu->getPosition ( ).y ) );
    CCFiniteTimeAction*		pMeaseOut	 = CCEaseOut::create ( pMmoveAction, 0.2f );
    CCFiniteTimeAction*		pMremove	 = CCCallFunc::create ( this, callfunc_selector ( MainMenuLayer::removeStartGameMenu ) );
    CCSequence*				pMtitleOut   = CCSequence::createWithTwoActions ( pMeaseOut, pMremove );
    pGameMenu->runAction ( pMtitleOut );
}

// Removes the start game menu from the layer, and informs the GameManager to start a new game.
KDvoid MainMenuLayer::removeStartGameMenu ( KDvoid )
{
    // Removes the start game menu from the layer
    this->removeChildByTag ( kMainMenuLayer_StartGameMenu, KD_TRUE );
    
    // Start a new game.
    GameManager::sharedGameManager ( )->startNewGame ( );
}

// Action to perform when the Play Game Button was touched.
KDvoid MainMenuLayer::playGameButtonTouched ( CCObject* pSender )
{
    // Remove the main menu and add the start game menu
    this->removeMainMenuChildren ( );
}

// Action to perform when the About Game Button was touched.
KDvoid MainMenuLayer::aboutGameButtonTouched ( CCObject* pSender )
{
    GameManager::sharedGameManager ( )->showAboutGame ( );
}

// Action to perform when the Start Easy Game Button was touched.
KDvoid MainMenuLayer::easyGameButtonTouched ( CCObject* pSender )
{
    GameManager::sharedGameManager ( )->setGameDifficulty ( kGameDifficultyEasy );
    // Remove the start game button and tell the GameManager to start a new game
    removeStartMenuChildren ( );
}

// Action to perform when the Start Medium Game Button was touched.
KDvoid MainMenuLayer::mediumGameButtonTouched ( CCObject* pSender )
{
    GameManager::sharedGameManager ( )->setGameDifficulty ( kGameDifficultyMedium );
    // Remove the start game button and tell the GameManager to start a new game
    removeStartMenuChildren ( );
}

// Action to perform when the Start Hard Game Button was touched.
KDvoid MainMenuLayer::hardGameButtonTouched ( CCObject* pSender )
{
    GameManager::sharedGameManager ( )->setGameDifficulty ( kGameDifficultyHard );
    // Remove the start game button and tell the GameManager to start a new game
    removeStartMenuChildren ( );
}

