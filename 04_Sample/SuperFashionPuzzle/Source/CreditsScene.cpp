/* --------------------------------------------------------------------------
 *
 *      File            CreditsScene.cpp
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
#include "CreditsScene.h"
#include "MenuMainScene.h"

KDbool CreditsScene::init ( KDvoid )
{
	if ( !CCScene::init ( ) )
	{
		return KD_FALSE;
	}

	// background
	CCSprite*		pBackground = CCSprite::create ( "shared_blue_background.pvr" );
	pBackground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pBackground );

	// sun_lights texture
	CCTexture2D*	pRectangleLightsTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "shared_rectangle_lights.png" );			

	// 3 sprites with the same texture
	CCSprite*		pRectangle1LightClockWiseSprite = CCSprite::createWithTexture ( pRectangleLightsTexture );
	pRectangle1LightClockWiseSprite->setScale ( 1 );
	pRectangle1LightClockWiseSprite->setPosition ( ccp ( 0, 100 ) );
	pRectangle1LightClockWiseSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( BACKGROUND_ROTATION_TIME, 360 ) ) );
	this->addChild ( pRectangle1LightClockWiseSprite );

	CCSprite*		pRectangle2LightClockWiseSprite = CCSprite::createWithTexture ( pRectangleLightsTexture );
	pRectangle2LightClockWiseSprite->setScale ( 1 );
	pRectangle2LightClockWiseSprite->setPosition ( ccp ( 350, -70 ) );
	pRectangle2LightClockWiseSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( BACKGROUND_ROTATION_TIME, 360 ) ) );
	this->addChild ( pRectangle2LightClockWiseSprite );

	CCSprite*		pRectangleLightCounterClockWiseSprite = CCSprite::createWithTexture ( pRectangleLightsTexture );
	pRectangleLightCounterClockWiseSprite->setScale ( 1.5f );
	pRectangleLightCounterClockWiseSprite->setPosition ( ccp ( 480, 360 ) );
	pRectangleLightCounterClockWiseSprite->runAction ( CCRepeatForever::create ( CCRotateBy::create ( BACKGROUND_ROTATION_TIME, -360 ) ) );
	this->addChild ( pRectangleLightCounterClockWiseSprite );

	// foreground
	CCSprite*		pForeground = CCSprite::create ( "menu_credits_foreground.png" );
	pForeground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pForeground );

	// ok button
	CCMenuItem*		pOkItem = CCMenuItemImage::create 
	(
		"menu_credits_ok_blue_button_released.png",
		"menu_credits_ok_blue_button_pressed.png", 
		this,
		menu_selector ( CreditsScene::okButtonPressed )
	);
    pOkItem->setPosition ( ccp ( 360, 42 ) );
	this->addChild ( CCMenu::createWithItem ( pOkItem ) );

	// send feedback button
	CCMenuItem*		pSendFeedbackItem = CCMenuItemImage::create 
	(
		"menu_credits_send_feedback_button_released.png",
		"menu_credits_send_feedback_button_pressed.png", 
		this,
		menu_selector ( CreditsScene::sendFeedbackButtonPressed )
	);
    pSendFeedbackItem->setPosition ( ccp ( 160, 42 ) );
	this->addChild ( CCMenu::createWithItem ( pSendFeedbackItem ) );

	return KD_TRUE;
}

KDvoid CreditsScene::okButtonPressed ( CCObject* pSender )
{
	CCLOG ( "okButtonPressed" );

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( TRANSITION_DURATION, MenuMainScene::create ( ), ccWHITE ) );
}

KDvoid CreditsScene::sendFeedbackButtonPressed ( CCObject* pSender )
{
	CCLOG ( "Asking for feedback." );

	SoundManager::sharedSoundManager ( )->playSoundFxTap ( );

	xmOpenURL ( ccszf ( "http://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%s&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software", "384946158" ) ); 
}
