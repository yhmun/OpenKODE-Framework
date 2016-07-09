/* --------------------------------------------------------------------------
 *
 *      File            TBCredits.cpp
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
#include "TBCredits.h"
#include "TBMainMenuScene.h"

KDbool TBCredits::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*		pBackground = CCSprite::create ( "credits.png" );
	pBackground->setPosition ( GameState->getScreenCenter ( ) );
	this->addChild ( pBackground );

	CCMenuItem*		pBack = CCMenuItemImage::create ( "Back_button.png"	 , KD_NULL, this, menu_selector ( TBCredits::backClicked ) );
	pBack->setPosition ( ccp ( 430, 30 ) );
	this->addChild ( CCMenu::createWithItem ( pBack ) );

	CCMenuItem*		pCrowdInfo = CCMenuItemImage::create ( "credits_crowd.png", KD_NULL, this, menu_selector ( TBCredits::crowdsClicked ) );
	pCrowdInfo->setPosition ( ccp ( 82, 258 ) );
	this->addChild ( CCMenu::createWithItem ( pCrowdInfo ) );

	return KD_TRUE;
};

KDvoid TBCredits::backClicked ( CCObject* pSender )
{
	GameState->playClick ( );

	CCDirector::sharedDirector ( )->replaceScene
	(
		CCTransitionSlideInL::create ( 0.5f, TBMainMenuScene::scene ( ) )
	);
}

KDvoid TBCredits::crowdsClicked ( CCObject* pSender )
{
	GameState->playClick ( );

	xmOpenURL ( "http://www.3scrowd.com" );
}
