/* --------------------------------------------------------------------------
 *
 *      File            ScoresScene.cpp
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
#include "ScoresScene.h"
#include "MenuMainScene.h"

KDbool ScoresScene::init ( KDvoid )
{
	if ( !CCScene::init ( ) )
	{
		return KD_FALSE;
	}

	// background
	CCSprite*		pBackground = CCSprite::create ( "shared_pink_background.pvr" );
	pBackground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pBackground );
	
	// foreground
	CCSprite*		pForeground = CCSprite::create ( "menu_scores_foreground.png" );
	pForeground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pForeground );
	
	// ok button
	CCMenuItem*		pOkItem = CCMenuItemImage::create 
	(
		"menu_scores_ok_pink_wide_button_released.png",
		"menu_scores_ok_pink_wide_button_pressed.png", 
		this,
		menu_selector ( ScoresScene::okButtonPressed )
	);
    pOkItem->setPosition ( ccp ( 337, 33 ) );
	this->addChild ( CCMenu::createWithItem ( pOkItem ) );
				
	// reset local scores button
	CCMenuItem*		pResetLocalItem = CCMenuItemImage::create 
	(
		"menu_scores_reset_local_button_released.png",
		"menu_scores_reset_local_button_pressed.png", 
		this,
		menu_selector ( ScoresScene::resetLocalButtonPressed )
	);
    pResetLocalItem->setPosition ( ccp ( 147, 33 ) );
	this->addChild ( CCMenu::createWithItem ( pResetLocalItem ) );

	// radio buttons
	CCMenuItem*		pMenuItem1 = CCMenuItemImage::create ( "menu_scores_local_button_released.png", "menu_scores_local_button_released.png", "menu_scores_local_button_pressed.png" );
	CCMenuItem*		pMenuItem2 = CCMenuItemImage::create ( "menu_scores_global_button_released.png", "menu_scores_global_button_released.png", "menu_scores_global_button_pressed.png" );
	CCMenuRadio*	pRadioMenu = CCMenuRadio::create ( this, menu_selector ( ScoresScene::radioButtonPressed ), pMenuItem1, pMenuItem2, KD_NULL );
	pRadioMenu->alignItemsHorizontally ( );
	pRadioMenu->setPosition ( ccp ( 240, 230 ) );
	pRadioMenu->setSelectedIndex ( 0 );
	this->addChild ( pRadioMenu );

	// init scores controllers
	m_eState = LOCAL;
		
	return KD_TRUE;
}

KDvoid ScoresScene::okButtonPressed ( CCObject* pSender )
{
	CCLOG ( "okButtonPressed" );
	if ( m_eState == LOCAL )
	{

	}
	else if ( m_eState == GLOBAL )
	{

	}
	else 
	{
		CCLOG ( "Unknow scores state.");
		kdExit ( 1 );
	}

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, MenuMainScene::create ( ), ccWHITE ) );
}

KDvoid ScoresScene::resetLocalButtonPressed ( CCObject* pSender )
{
	CCLOG ( "resetLocalButtonPressed" );
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
}

KDvoid ScoresScene::radioButtonPressed ( CCObject* pSender )
{
	CCLOG ( "radioButtonPressed");
	CCMenuRadio*	pRadio = (CCMenuRadio*) pSender;
	this->changeScoresState ( pRadio->getSelectedIndex ( ) ? LOCAL : GLOBAL );
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
}

KDvoid ScoresScene::changeScoresState ( ScoresState eScoresState )
{
	if ( m_eState == LOCAL && eScoresState == GLOBAL )
	{

	}
	else if ( m_eState == GLOBAL && eScoresState == LOCAL )
	{

	}
	else 
	{
		CCLOG ( "Unknown transition in ScoresScene object." );
		return; // ignore it, maybe local button was pressed when local state is activated
	}

	m_eState = eScoresState;
}
