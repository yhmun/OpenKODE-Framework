/* --------------------------------------------------------------------------
 *
 *      File            GameOver.cpp
 *      Description     
 *      Author          Hae-Won Lee
 *      Contact         leehw000@gmail.com
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
#include "GameOver.h"
#include "GameLayer.h"

KDbool GameOver::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&	tLyrSize = this->getContentSize ( );

	CCLabelTTF*		pLabel = CCLabelTTF::create ( "New Game", "Font/Marker Felt.ttf", 64 );					
	CCMenuItem*		pItem  = CCMenuItemLabel::create ( pLabel, this, menu_selector ( GameOver::newGameSpriteTapped ) );
	pItem->setPosition ( ccpMid ( tLyrSize ) );
	this->addChild ( CCMenu::createWithItem ( pItem ), 1 );

	return KD_TRUE;
}		

KDvoid GameOver::newGameSpriteTapped ( CCObject* pSender )
{
	CCScene*	pScene = CCScene::create ( );
	CCLayer*	pLayer = GameLayer::create ( );
	pScene->addChild ( pLayer );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionProgressRadialCW::create ( 0.5f, pScene ) );
}
