/* --------------------------------------------------------------------------
 *
 *      File            Ch5_VertSlidingMenuGrid.cpp
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
#include "Ch5_VertSlidingMenuGrid.h"
#include "Libraries/SlidingMenuGrid.h"

KDbool Ch5_VertSlidingMenuGrid::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	this->showMessage ( "Tap a button or slide the menu grid up or down." );

	// Init item array
	CCArray*	pItems = CCArray::create ( );
	
	// Create 45 CCMenuItemSprite objects with tags, callback methods and randomized colors 
	for ( KDint i = 1; i <= 45; ++i )
	{
		CCSprite*	pNormalSprite   = CCSprite::create ( "sliding_menu_button_0.png" );
		CCSprite*	pSelectedSprite = CCSprite::create ( "sliding_menu_button_1.png" );
		ccColor3B	tColor = this->randomColor ( );

		pNormalSprite  ->setColor ( tColor );
		pSelectedSprite->setColor ( tColor );
		
		CCMenuItemSprite*	pItem = CCMenuItemSprite::create ( pNormalSprite, pSelectedSprite, this, menu_selector ( Ch5_VertSlidingMenuGrid::buttonClicked ) );
		pItem->setTag ( i );
		
		// Add each item to array
		pItems->addObject ( pItem );
	}

	// Init SlidingMenuGrid object with array and some other information
	SlidingMenuGrid*	pMenuGrid = SlidingMenuGrid::create ( pItems, 5, 3, ccp ( 70.f, 220.f ), ccp ( 90.f, 80.f ), KD_TRUE );
	this->addChild ( pMenuGrid, 1 );	
	
	return KD_TRUE;
}

// Button clicked callback
KDvoid Ch5_VertSlidingMenuGrid::buttonClicked ( CCObject* pSender )
{
	CCMenuItemSprite*	pSprite = (CCMenuItemSprite*) pSender;
	this->showMessage ( ccszf ( "Button clicked: %d", pSprite->getTag ( ) ) );
}

// Random base color method
ccColor3B Ch5_VertSlidingMenuGrid::randomColor ( KDvoid )
{
	switch ( kdRand ( ) % 13 )
	{
		case  1 :	return ccc3 ( 255, 255, 255 );
		case  2 :	return ccc3 ( 255,   0,   0 );
		case  3 :	return ccc3 (   0, 255,   0 );
		case  4 :	return ccc3 (   0,   0, 255 );
		case  5 :	return ccc3 (   0, 255, 255 );
		case  6 :	return ccc3 ( 255,   0, 255 );
		case  7 :	return ccc3 ( 255, 128,   0 );
		case  8 :	return ccc3 ( 255,   0, 128 );
		case  9 :	return ccc3 ( 128, 255,   0 );
		case 10 :	return ccc3 (   0, 255, 128 );
		case 11 :	return ccc3 ( 128,   0, 255 );
		default :	return ccc3 (   0, 128, 255 );
	}
}