/* --------------------------------------------------------------------------
 *
 *      File            Ch5_UsingCCLayerMultiplex.cpp
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
#include "Ch5_UsingCCLayerMultiplex.h"

KDbool Ch5_UsingCCLayerMultiplex::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Font size/name
	CCMenuItemFont::setFontSize ( 30 );
	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );

	// Create our multiplex layer with three MultiplexLayerMenu objects
	CCLayerMultiplex*	pLayer = CCLayerMultiplex::create 
	(
		MultiplexLayerMenu::createWithLayerNumber ( 0 ), 
		MultiplexLayerMenu::createWithLayerNumber ( 1 ),
		MultiplexLayerMenu::createWithLayerNumber ( 2 ),
		KD_NULL
	);
	this->addChild ( pLayer, 0 );

	return KD_TRUE;
}

MultiplexLayerMenu* MultiplexLayerMenu::createWithLayerNumber ( KDint nLayerNumber )
{
	MultiplexLayerMenu*		pRet = new MultiplexLayerMenu ( );

	if ( pRet && pRet->initWithLayerNumber ( nLayerNumber ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool MultiplexLayerMenu::initWithLayerNumber ( KDint nLayerNumber )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	// Random background color
	CCSprite*			pBG = CCSprite::create ( "blank.png" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	pBG->setTextureRect ( CCRect ( 0, 0, 480, 320 ) );
	pBG->setColor ( ccc3 ( kdRand ( ) % 150, kdRand ( ) % 150, kdRand ( ) % 150 ) );
	this->addChild ( pBG );

	// Layer number as message
	CCLabelBMFont*		pMessage = CCLabelBMFont::create ( ccszf ( "Layer %d", nLayerNumber + 1 ), "eurostile_30.fnt" );
	pMessage->setPosition ( ccp ( 160, 270 ) );
	pMessage->setScale ( 0.75f );
	pMessage->setColor ( ccc3 ( 255, 255, 255 ) );
	this->addChild ( pMessage, 10 );
	
	// Buttons to go to different layers
	CCMenuItemFont*		pGoToLayer1 = CCMenuItemFont::create ( "Go To Layer 1", this, menu_selector ( MultiplexLayerMenu::goToLayer ) );
	CCMenuItemFont*		pGoToLayer2 = CCMenuItemFont::create ( "Go To Layer 2", this, menu_selector ( MultiplexLayerMenu::goToLayer ) );
	CCMenuItemFont*		pGoToLayer3 = CCMenuItemFont::create ( "Go To Layer 3", this, menu_selector ( MultiplexLayerMenu::goToLayer ) );

	pGoToLayer1->setTag ( 0 );
	pGoToLayer2->setTag ( 1 );
	pGoToLayer3->setTag ( 2 );

	// Add menu items
	CCMenu*				pMenu = CCMenu::create ( pGoToLayer1, pGoToLayer2, pGoToLayer3, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	pMenu->alignItemsVertically ( 10 );
	this->addChild ( pMenu );		

	return KD_TRUE;
}

// Switch to a different layer
KDvoid MultiplexLayerMenu::goToLayer ( CCObject* pSender )
{
	CCMenuItemFont*		pItem = (CCMenuItemFont*) pSender;

	( (CCLayerMultiplex*) this->getParent ( ) )->switchTo ( pItem->getTag ( ) );
}