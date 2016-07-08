/* --------------------------------------------------------------------------
 *
 *      File            Ch2_DepthTestingInput.cpp
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
#include "Ch2_DepthTestingInput.h"
#include "Libraries/ColorTouchSprite.h"

KDbool Ch2_DepthTestingInput::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Our message sprite
	m_pMessage->setString ( "Tap, hold or drag the square." );

	// Init the ColorTouchSprites
	this->initSprites ( );

	return KD_TRUE;
}

KDvoid Ch2_DepthTestingInput::onExit ( KDvoid )
{
	m_pSprites->release ( );

	Recipe::onExit ( );
}

KDvoid Ch2_DepthTestingInput::initSprites ( KDvoid )
{
	m_pSprites = CCArray::create ( );
	m_pSprites->retain ( );

	// We add 10 randomly colored sprites 
	for ( KDint x = 0; x < 10; x++ )
	{
		CCSprite*	pSprite = ColorTouchSprite::create ( "blank.png" );
		pSprite->setPosition ( ccp ( x * 30 + 100, x * 15 + 75 ) );
		pSprite->setTextureRect ( CCRect ( 0, 0, 75, 75 ) );
		pSprite->setColor ( ccc3 ( kdRand ( ) % 255, kdRand ( ) % 255, kdRand ( ) % 255 ) );
		this->addChild ( pSprite, 11 - x );	
		m_pSprites->addObject ( pSprite );
	}
}

KDvoid Ch2_DepthTestingInput::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	CCObject*	pObject;

	// Process input for all sprites
	CCARRAY_FOREACH ( m_pSprites, pObject )
	{
		ColorTouchSprite*	pSprite = (ColorTouchSprite*) pObject;
		if ( pointIsInRect ( tPoint, pSprite->rect ( ) ) )
		{
			// Swallow the input
			pSprite->ccTouchesBegan ( pTouches, pEvent );	
			return;
		}
	}
}

KDvoid Ch2_DepthTestingInput::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	CCObject*	pObject;

	// Process input for all sprites
	CCARRAY_FOREACH ( m_pSprites, pObject )
	{
		ColorTouchSprite*	pSprite = (ColorTouchSprite*) pObject;
		if ( pointIsInRect ( tPoint, pSprite->rect ( ) ) )
		{
			pSprite->ccTouchesMoved ( pTouches, pEvent );	
			return;
		}
	}
}

KDvoid Ch2_DepthTestingInput::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	CCObject*	pObject;
	
	// Process input for all sprites
	CCARRAY_FOREACH ( m_pSprites, pObject )
	{
		ColorTouchSprite*	pSprite = (ColorTouchSprite*) pObject;
		if ( pointIsInRect ( tPoint, pSprite->rect ( ) ) )
		{
			// End all input when you lift up your finger
			pSprite->ccTouchesEnded ( pTouches, pEvent );	
			return;
		}
	}
}