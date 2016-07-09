/* --------------------------------------------------------------------------
 *
 *      File            Ch5_UsingCCMenu.cpp
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
#include "Ch5_UsingCCMenu.h"

KDbool Ch5_UsingCCMenu::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "" );

	// Set font size/name
	CCMenuItemFont::setFontSize ( 30 );
	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );
	
	// Image Button
	CCMenuItemImage*	pImageButton = CCMenuItemImage::create ( "button_unselected.png", "button_selected.png", "button_disabled.png", this, menu_selector ( Ch5_UsingCCMenu::buttonTouched ) );  
	
	// Enable Options Label
	CCLabelBMFont*		pEnableOptionsLabel = CCLabelBMFont::create ( "Enable Options", "eurostile_30.fnt" );
	CCMenuItemLabel*	pEnableOptions = CCMenuItemLabel::create ( pEnableOptionsLabel, this,  menu_selector ( Ch5_UsingCCMenu::enableOptions ) );

	// Options Label
	m_pOptionsItem = CCMenuItemFont::create ( "Options", this, menu_selector ( Ch5_UsingCCMenu::options ) );
	m_pOptionsItem->setEnabled ( KD_FALSE );
	
	// Re-Align Label
	CCMenuItemFont*		pReAlign = CCMenuItemFont::create ( "Re-Align", this, menu_selector ( Ch5_UsingCCMenu::reAlign ) );
	
	// Add menu items
	m_pMenu = CCMenu::create ( pImageButton, pEnableOptions, m_pOptionsItem, pReAlign, KD_NULL );
	m_pMenu->alignItemsVertically ( );
	m_pMenu->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( m_pMenu );

	return KD_TRUE;
}

KDvoid Ch5_UsingCCMenu::buttonTouched ( CCObject* pSender )
{
	this->showMessage ( "Button touched!" );
}

KDvoid Ch5_UsingCCMenu::enableOptions ( CCObject* pSender )
{
	m_pOptionsItem->setEnabled ( !m_pOptionsItem->isEnabled ( ) );
}

KDvoid Ch5_UsingCCMenu::options ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->pushScene ( OptionsMenu::scene ( ) );
}

// Randomly re-align our menu
KDvoid Ch5_UsingCCMenu::reAlign ( CCObject* pSender )
{
	switch ( kdRand ( ) % 6 )
	{
		case 0 :
			m_pMenu->alignItemsVertically ( );	
			break;

		case 1 :
			m_pMenu->alignItemsHorizontally ( );	
			break;

		case 2 :
			m_pMenu->alignItemsHorizontallyWithPadding ( (KDfloat) ( kdRand ( ) % 30 ) );
			break;

		case 3 :
			m_pMenu->alignItemsVerticallyWithPadding ( (KDfloat) (  kdRand ( ) % 30 ) );
			break;

		case 4 :
			m_pMenu->alignItemsInColumns ( 2, 2, KD_NULL );
			break;

		case 5 :
			m_pMenu->alignItemsInRows ( 2, 2, KD_NULL );
			break;
	}
}

KDbool OptionsMenu::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// Random background color
	CCSprite*	pBG = CCSprite::create ( "blank.png" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	pBG->setTextureRect ( CCRect ( 0, 0, 480, 320 ) );
	pBG->setColor ( ccc3 ( kdRand ( ) % 150, kdRand ( ) % 150, kdRand ( ) % 150 ) );
	this->addChild ( pBG );

	// Init our message label
	m_pMessage = CCLabelBMFont::create ( "", "eurostile_30.fnt" );
	m_pMessage->setPosition ( ccp ( 160, 270 ) );
	m_pMessage->setScale ( 0.5f );
	m_pMessage->setColor ( ccc3 ( 255, 255, 255 ) );
	this->addChild ( m_pMessage, 10 );

	// Disabled title label for Sound option
	CCMenuItemFont*		pTitle1 = CCMenuItemFont::create ( "Sound" );
	pTitle1->setEnabled ( KD_FALSE );
	pTitle1->setColor ( ccc3 ( 0, 0, 0 ) );

	// Toggleable item for Sound option
	CCMenuItemToggle*	pItem1 = CCMenuItemToggle::createWithTarget
	(
		this,
		menu_selector ( OptionsMenu::soundToggle ),
		CCMenuItemFont::create ( "On" ),
		CCMenuItemFont::create ( "Off" ),
		KD_NULL
	);

	// Disabled title label for Difficulty option
	CCMenuItemFont*		pTitle2 = CCMenuItemFont::create ( "Difficulty" );
	pTitle2->setEnabled ( KD_FALSE );
	pTitle2->setColor ( ccc3 ( 0, 0, 0 ) );

	// Toggleable item for Difficulty option
	CCMenuItemToggle*	pItem2 = CCMenuItemToggle::createWithTarget
	(
		this,
		menu_selector ( OptionsMenu::difficultyToggle ),
		CCMenuItemFont::create ( "Easy" ),
		CCMenuItemFont::create ( "Medium" ),
		CCMenuItemFont::create ( "Hard" ),
		CCMenuItemFont::create ( "Insane" ),
		KD_NULL
	);

	// Back button
	CCMenuItemFont*		pBack = CCMenuItemFont::create ( "Back", this, menu_selector ( OptionsMenu::back ) );

	// Finally, create our menu
	CCMenu*				pMenu = CCMenu::create ( pTitle1, pTitle2, pItem1, pItem2, pBack, KD_NULL );		// 5 items.

	// Align items in columns
	pMenu->alignItemsInColumns ( 2, 2, 1, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pMenu );

	return KD_TRUE;
}

KDvoid OptionsMenu::back ( CCObject* pSender )
{	
	CCDirector::sharedDirector ( )->popScene ( );
}

// Use the 'selectedIndex' variable to identify the touched item
KDvoid OptionsMenu::soundToggle ( CCObject* pSender )
{
	CCMenuItemToggle*		pItem = (CCMenuItemToggle*) pSender;
	m_pMessage->setString ( ccszf ( "Selected Sound Index:%d", pItem->getSelectedIndex ( ) ) );
}

KDvoid OptionsMenu::difficultyToggle ( CCObject* pSender )
{
	CCMenuItemToggle*		pItem = (CCMenuItemToggle*) pSender;
	m_pMessage->setString ( ccszf ( "Selected Difficulty Index:%d", pItem->getSelectedIndex ( ) ) );
}