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
#include "TBSplashScene.h"
#include "TBMainMenuScene.h"

KDbool TBSplashScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*	pLoadingSprite = CCSprite::create ( "splash.png" );
	pLoadingSprite->setPosition ( GameState->getScreenCenter ( ) );
	this->addChild ( pLoadingSprite );
		
	pLoadingSprite->runAction 
	(
		CCSequence::create 
		(
			CCDelayTime::create ( 1.f ),
			CCCallFunc::create ( this, callfunc_selector ( TBSplashScene::show3Logo ) ),
			CCFadeOut::create ( 1.f ),
			KD_NULL 
		)
	);

	return KD_TRUE;
}

KDvoid TBSplashScene::show3Logo ( KDvoid )
{
	CCSprite*	pThreeLogo = CCSprite::create ( "3logo.png" );
	pThreeLogo->setPosition ( GameState->getScreenCenter ( ) );
	this->addChild ( pThreeLogo );

	pThreeLogo->setOpacity ( 0 );
	
	pThreeLogo->runAction 
	(
		CCSequence::create 
		(
			CCFadeIn::create ( 1.f ),
			CCDelayTime::create ( 1.f ),
			CCCallFunc::create ( this, callfunc_selector ( TBSplashScene::showMainMenu ) ),
			KD_NULL 
		)
	);
}

KDvoid TBSplashScene::showMainMenu ( KDvoid )
{
	CCDirector::sharedDirector ( )->replaceScene
	(
		CCTransitionRotoZoom::create ( 0.6f, TBMainMenuScene::scene ( ) )
	);
}
