/* --------------------------------------------------------------------------
 *
 *      File            HowToPlayScene.cpp
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
#include "HowToPlayScene.h"
#include "MenuMainScene.h"

HowToPlayScene::HowToPlayScene ( KDvoid )
{
	m_pPages = KD_NULL;
}

HowToPlayScene::~HowToPlayScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pPages );
}

KDbool HowToPlayScene::init ( KDvoid )
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

	// load 4 pages, CCSprites, foreground
	m_pPages = new CCArray ( );
	for ( KDint i = 0; i < 4; i++ )
	{
		CCSprite*	pPageSprite = CCSprite::create ( ccszf ( "menu_how_to_play_page_%d.png", i ) );
		pPageSprite->setPosition ( ccp ( 240, 160 ) );
		pPageSprite->setVisible ( i == 0 );
		this->addChild ( pPageSprite );
		m_pPages->addObject ( pPageSprite );
	}

	// ok button
	CCMenuItem*		pOkItem = CCMenuItemImage::create 
	(
		"shared_ok_pink_released.png",
		"shared_ok_pink_pressed.png", 
		this,
		menu_selector ( HowToPlayScene::okButtonPressed )
	);
    pOkItem->setPosition ( ccp ( 240, 43 ) );
	this->addChild ( CCMenu::createWithItem ( pOkItem ) );

	// left arrow button
	CCMenuItem*		pLeftArrowItem = CCMenuItemImage::create 
	(
		"menu_how_to_play_left_arrow_released.png",
		"menu_how_to_play_left_arrow_pressed.png", 
		this,
		menu_selector ( HowToPlayScene::leftArrowButtonPressed )
	);
    pLeftArrowItem->setPosition ( ccp ( 150, 43 ) );
	this->addChild ( CCMenu::createWithItem ( pLeftArrowItem ) );

	// right arrow button
	CCMenuItem*		pRightArrowItem = CCMenuItemImage::create 
	(
		"menu_how_to_play_right_arrow_released.png",
		"menu_how_to_play_right_arrow_pressed.png", 
		this,
		menu_selector ( HowToPlayScene::rightArrowButtonPressed )
	);
    pRightArrowItem->setPosition ( ccp ( 480 - 150, 43 ) );
	this->addChild ( CCMenu::createWithItem ( pRightArrowItem ) );

	m_bPopAtExit = KD_FALSE;
	m_nIndex	 = 0;

	return KD_TRUE;
}

KDvoid HowToPlayScene::okButtonPressed ( CCObject* pSender )
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
		
KDvoid HowToPlayScene::leftArrowButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );

	m_nIndex = ( m_nIndex - 1 ) % m_pPages->count ( );
	for ( KDuint i = 0; i < m_pPages->count ( ); i++ )
	{
		CCSprite*	pSprite = (CCSprite*) m_pPages->objectAtIndex ( i );
		pSprite->setVisible ( i == m_nIndex );		
	}
}

KDvoid HowToPlayScene::rightArrowButtonPressed ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );

	m_nIndex = ( m_nIndex + 1 ) % m_pPages->count ( );
	for ( KDuint i = 0; i < m_pPages->count ( ); i++ )
	{
		CCSprite*	pSprite = (CCSprite*) m_pPages->objectAtIndex ( i );
		pSprite->setVisible ( i == m_nIndex );		
	}
}

KDvoid HowToPlayScene::setPopAtExit ( KDvoid )
{
	m_bPopAtExit = KD_TRUE;
}
