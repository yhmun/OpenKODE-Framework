/* --------------------------------------------------------------------------
 *
 *      File            IntroLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-06
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
#include "IntroLayer.h"
#include "SLevelManager.h"
#include "SLevel.h"
#include "SThumbLayer.h"
#include "SGameLayer.h"

KDbool IntroLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCArray*	pLayers = CCArray::createWithCapacity ( 5 );
	CCArray*	pLevels = SLevelManager::sharedLevelManager ( )->getLevelArray ( );

	CCObject*	pObject;
	CCARRAY_FOREACH ( pLevels, pObject )
	{
		SLevel*			pLevel = (SLevel*) pObject;
		SThumbLayer*	pLayer = SThumbLayer::create ( pLevel );
		pLayers->addObject ( pLayer );
	}

	m_pScrollLayer = CCScrollLayer::create ( pLayers, 233, 40 );
	m_pScrollLayer->setAnchorPoint ( ccp ( 0, 0 ) );
	m_pScrollLayer->setPosition ( 0, 90 );
	m_pScrollLayer->selectPage ( 0 );
	this->addChild ( m_pScrollLayer, 5 );

	CCSprite*	pBackground = CCSprite::create ( "background.png" );
	pBackground->setAnchorPoint ( ccp ( 0, 0 ) );
	this->addChild ( pBackground, 0, 1 ); 

	CCMenuItemImage*	pStartItem = CCMenuItemImage::create ( "start.png", "start.png", this, menu_selector ( IntroLayer::onStart ) );
	( (CCSprite*) pStartItem->getSelectedImage ( ) )->setOpacity ( 128 );

	CCMenu*		pMenu = CCMenu::createWithItem ( pStartItem );
	pMenu->setPosition ( ccp ( 240, 40 ) );
	this->addChild ( pMenu );

	return KD_TRUE;
}

KDvoid IntroLayer::onStart ( CCObject* pSender )
{
	SLevelManager::sharedLevelManager ( )->setCurrentLevel ( m_pScrollLayer->getCurrentScreen ( ) );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 2, SGameLayer::scene ( ), ccWHITE ) );
}
