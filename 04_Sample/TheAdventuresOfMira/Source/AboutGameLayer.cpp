/* --------------------------------------------------------------------------
 *
 *      File            AboutGameLayer.cpp
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
#include "AboutGameLayer.h"

// Initialize our Main Menu Layer, and add the initial main menu with the title screen, play button and about button.
KDbool AboutGameLayer::init ( KDvoid )
{
    // Initialize the parent
    if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}
    
    // Set the screen size to the full dimensions of the device ( full screen )
    this->setContentSize ( SCREEN_SIZE );
    
    CCSprite*		pAboutGameTitle = CCSprite::createWithSpriteFrameName ( "about_game.png" );
    pAboutGameTitle->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.2f ) );
    pAboutGameTitle->setScale ( 0.8f );
    this->addChild ( pAboutGameTitle );
    
	DeviceType		eDeviceType = GameManager::sharedGameManager ( )->getDeviceType ( );
	DeviceSize		eDeviceSize = GameManager::sharedGameManager ( )->getDeviceSize ( );

    KDfloat			fLabelScaleNormal = 1.0f;
    if ( eDeviceType == kDeviceTypeAndroid854x480 ||
         eDeviceType == kDeviceTypeAndroid800x480 ||
         eDeviceType == kDeviceTypeAndroid800x400 ||
         eDeviceType == kDeviceTypeAndroid720x480 )
    {
        fLabelScaleNormal = 0.8f;
    }
    else if ( eDeviceSize == kDeviceSizeNormal )
    {
        fLabelScaleNormal = 0.9f;
    }
    else if ( eDeviceSize == kDeviceSizeSmall )
    {
        fLabelScaleNormal = 0.45f;
    }
    
    KDfloat			fLabelScaleSmall = 0.8f;
    if ( eDeviceType == kDeviceTypeAndroid854x480 ||
         eDeviceType == kDeviceTypeAndroid800x480 ||
         eDeviceType == kDeviceTypeAndroid800x400 ||
         eDeviceType == kDeviceTypeAndroid720x480 )
    {
        fLabelScaleSmall = 0.6f;
    }
    else if ( eDeviceSize == kDeviceSizeNormal )
    {
        fLabelScaleSmall = 0.7f;
    }
    else if ( eDeviceSize == kDeviceSizeSmall )
    {
        fLabelScaleSmall = 0.4f;
    }
    
    CCLabelBMFont*	pDescriptionLabel1 = CCLabelBMFont::create ( "HELP MIRA THE BEE COLLECT AS MANY FLOWERS AS", "myGlyphs.fnt" );
    pDescriptionLabel1->setScale ( fLabelScaleNormal );
    pDescriptionLabel1->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.45 ) );
    this->addChild ( pDescriptionLabel1 );
    
    CCLabelBMFont*	pDescriptionLabel2 = CCLabelBMFont::create ( "POSSIBLE TO MAKE HONEY FOR HER HIVE", "myGlyphs.fnt" );
    pDescriptionLabel2->setScale ( fLabelScaleNormal );
    pDescriptionLabel2->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.55 ) );
    this->addChild ( pDescriptionLabel2 );
    
    CCLabelBMFont*	pCreatedLabel = CCLabelBMFont::create ( "CONCEPT   PROGRAMMING   MUSIC:", "myGlyphs.fnt" );
    pCreatedLabel->setScale ( fLabelScaleSmall );
    pCreatedLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.85 ) );
    this->addChild ( pCreatedLabel );
    
    CCLabelBMFont*	pCraigLabel = CCLabelBMFont::create ( "C R A I G   N E W T O N", "myGlyphs.fnt" );
    pCraigLabel->setScale ( fLabelScaleNormal );
    pCraigLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.0 ) );
    this->addChild ( pCraigLabel );
    
    CCLabelBMFont*	pArtworkLabel = CCLabelBMFont::create ( "ARTWORK  AND  GRAPHIC  DESIGN:", "myGlyphs.fnt" );
    pArtworkLabel->setScale ( fLabelScaleSmall );
    pArtworkLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.45 ) );
    this->addChild ( pArtworkLabel );
    
    CCLabelBMFont*	pArtistsLabel = CCLabelBMFont::create ( "R A Y N A L D O   P E R E Z    AND    J O S E   F E R N A N D E Z", "myGlyphs.fnt" );
    pArtistsLabel->setScale ( fLabelScaleNormal );
    pArtistsLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.75 ) );
    this->addChild ( pArtistsLabel );
    
    CCLabelBMFont*	pCopyrightLabel = CCLabelBMFont::create ( "COPYRIGHT 2012 NEWTON DEVELOPMENT STUDIO", "myGlyphs.fnt" );
    pCopyrightLabel->setScale ( fLabelScaleNormal );
    pCopyrightLabel->setPosition ( ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3.75 ) );
    this->addChild ( pCopyrightLabel );
    
    // Back button
    this->createBackButton ( );
    
    // Initialization was successful
    return true;
}

KDvoid AboutGameLayer::createBackButton ( KDvoid )
{
    // Create the backButton
    CCSprite*			pBackButtonSpriteUp   = CCSprite::createWithSpriteFrameName ( "main_button.png" );
    CCSprite*			pBackButtonSpriteDown = CCSprite::createWithSpriteFrameName ( "main_button.png" );
    // Scale the down to 90% so that it looks like the button was pressed
    pBackButtonSpriteDown->setScale ( 0.9f );
    CCMenuItemSprite*	pBackButton = CCMenuItemSprite::create ( pBackButtonSpriteUp, pBackButtonSpriteDown, this, menu_selector ( AboutGameLayer::backButtonTouched ) );
    pBackButton->runAction ( CCRepeatForever::create ( CCSequence::createWithTwoActions ( CCScaleTo::create ( 0.4f, 1.1f ), CCScaleTo::create ( 1.0f, 1.0f ) ) ) );
    
    // Create the start game menu
    CCMenu*				pGameMenu = CCMenu::create ( pBackButton, KD_NULL );
    // Set the initial position off screen
    pGameMenu->setPosition ( ccp ( SCREEN_WIDTH / 2, -SCREEN_HEIGHT ) );
    // Set the tag of the start menu so that we can refer to it later using getChildByTag ( ... )
    pGameMenu->setTag ( kAboutGameLayer_BackMenu );
    
    // Create and run an animation that will slide the start game menu in from below the screen to its intended location
    CCActionInterval*	pMoveAction = CCMoveTo::create ( 0.3f, ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8 ) );
    CCFiniteTimeAction*	pEaseIn = CCEaseIn::create ( pMoveAction, 0.2f );
    pGameMenu->runAction ( pEaseIn );
    
    // Add the start menu to the layer
    this->addChild ( pGameMenu );
}

KDvoid AboutGameLayer::backButtonTouched ( CCObject* pSender )
{
    this->getChildByTag ( kAboutGameLayer_BackMenu )->stopAllActions ( );
    GameManager::sharedGameManager ( )->showMainMenu ( );
}
