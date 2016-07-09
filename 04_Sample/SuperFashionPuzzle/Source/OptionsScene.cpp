/* --------------------------------------------------------------------------
 *
 *      File            OptionsScene.cpp
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
#include "OptionsScene.h"
#include "MenuMainScene.h"

KDbool OptionsScene::init ( KDvoid )
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
	CCSprite*		pForeground = CCSprite::create ( "menu_options_foreground.png" );
	pForeground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pForeground );

	// ok button
	CCMenuItem*		pOkItem = CCMenuItemImage::create 
	(
		"shared_ok_pink_released.png",
		"shared_ok_pink_pressed.png", 
		this,
		menu_selector ( OptionsScene::okButtonPressed )
	);
    pOkItem->setPosition ( ccp ( 240, 43 ) );
	this->addChild ( CCMenu::createWithItem ( pOkItem ) );

	// Radio buttons
	CCMenuItem*		pMenuItem1 = CCMenuItemImage::create ( "menu_options_layout_right_off.png", "menu_options_layout_right_off.png", "menu_options_layout_right_on.png" );
	CCMenuItem*		pMenuItem2 = CCMenuItemImage::create ( "menu_options_layout_left_off.png", "menu_options_layout_left_off.png", "menu_options_layout_left_on.png" );
	CCMenuRadio*	pRadioMenu = CCMenuRadio::create ( this, menu_selector ( OptionsScene::boardRadioButtonSelected ), pMenuItem1, pMenuItem2, KD_NULL );
	pRadioMenu->alignItemsHorizontally ( );
	pRadioMenu->setPosition ( ccp ( 240, 146 ) );
	this->addChild ( pRadioMenu );

	// where is board, right or left?
	pRadioMenu->setSelectedIndex ( CCUserDefault::sharedUserDefault ( )->getBoolForKey ( "boardAtTheLeft" ) ? 1 : 0 );

	// login/logut butttons
	KDbool			bIsConnected = KD_FALSE; 
	const KDint		nLoginX = 370, nLoginY = 45;

	// login button
	CCMenuItem*		pLoginItem = CCMenuItemImage::create 
	(
		"facebook_login_released.png",
		"facebook_login_pressed.png", 
		this,
		menu_selector ( OptionsScene::loginButtonPressed )
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
		menu_selector ( OptionsScene::logoutButtonPressed )
	);
    pLogoutItem->setPosition ( ccp ( nLoginX, nLoginY ) );
	m_pLogoutMenu = CCMenu::createWithItem ( pLogoutItem );
	m_pLogoutMenu->setVisible ( bIsConnected );
	this->addChild ( m_pLogoutMenu );

	// init vars
	m_bPopAtExit = KD_FALSE;

	return KD_TRUE;
}

KDvoid OptionsScene::okButtonPressed ( CCObject* pSender )
{
	CCLOG ( "okButtonPressed" );

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	
	if ( m_bPopAtExit )
	{
		// called from pause menu
		CCDirector::sharedDirector ( )->popScene ( );					
	}
	else 
	{
		// called from main main menu
		CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, MenuMainScene::create ( ), ccWHITE ) );
	}
}

KDvoid OptionsScene::boardRadioButtonSelected ( CCObject* pSender )
{
	CCMenuRadio*	pRadio = (CCMenuRadio*) pSender;
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCUserDefault::sharedUserDefault ( )->setBoolForKey ( "boardAtTheLeft", pRadio->getSelectedIndex ( ) == 1 );
	CCUserDefault::sharedUserDefault ( )->flush ( );
}

KDvoid OptionsScene::loginButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
}

KDvoid OptionsScene::logoutButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
}		


KDvoid OptionsScene::setPopAtExit ( KDvoid )
{
	m_bPopAtExit = KD_TRUE;
}