/* --------------------------------------------------------------------------
 *
 *      File            TBSplashScene.cpp
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
#include "TBMainMenuScene.h"
#include "TBGameScene.h"
#include "TBInstructions.h"
#include "TBCredits.h"

KDbool TBMainMenuScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*	pBackgroundSprite = CCSprite::create ( "background.png" );
	pBackgroundSprite->setPosition ( GameState->getScreenCenter ( ) );
	this->addChild ( pBackgroundSprite );

	CCSprite*	pBall = CCSprite::create ( "ball.png" );
	pBall->setPosition ( ccp ( 86, 178 ) );
	pBall->runAction ( CCRepeatForever::create ( CCJumpBy::create ( 1, ccpz, 50, 1 ) ) );
	this->addChild ( pBall );

	CCSprite*	pMenuHolder = CCSprite::create ( "Menu.png" );
	pMenuHolder->setPosition ( ccp ( 360, 160 ) );
	this->addChild ( pMenuHolder );

	CCMenu*		pMenu = CCMenu::create 
	(
		CCMenuItemImage::create ( "Play_btn.png"		, KD_NULL, this, menu_selector ( TBMainMenuScene::playClicked ) ),
		CCMenuItemImage::create ( "Instructions_btn.png", KD_NULL, this, menu_selector ( TBMainMenuScene::instructionsClicked ) ),
		CCMenuItemImage::create ( "Share_btn.png"		, KD_NULL, this, menu_selector ( TBMainMenuScene::shareClicked ) ),
		CCMenuItemImage::create ( "Credits_btn.png"		, KD_NULL, this, menu_selector ( TBMainMenuScene::creditsClicked ) ),
		KD_NULL
	);
	pMenu->alignItemsVertically ( );
	pMenu->setPosition ( ccp ( 360, 120 ) );
	this->addChild ( pMenu );

	return KD_TRUE;
}

KDvoid TBMainMenuScene::playClicked ( CCObject* pSender )
{
	GameState->playClick ( );

	CCDirector::sharedDirector ( )->replaceScene
	(
		CCTransitionSlideInR::create ( 0.5f, TBGameScene::scene ( ) )
	);
}

KDvoid TBMainMenuScene::instructionsClicked ( CCObject* pSender )
{
	GameState->playClick ( );

	CCDirector::sharedDirector ( )->replaceScene
	(
		CCTransitionSlideInR::create ( 0.5f, TBInstructions::scene ( ) )
	);
}

KDvoid TBMainMenuScene::shareClicked ( CCObject* pSender )
{
	GameState->playClick ( );
}

KDvoid TBMainMenuScene::creditsClicked ( CCObject* pSender )
{
	GameState->playClick ( );

	CCDirector::sharedDirector ( )->replaceScene
	(
		CCTransitionSlideInR::create ( 0.5f, TBCredits::scene ( ) )
	);
}

