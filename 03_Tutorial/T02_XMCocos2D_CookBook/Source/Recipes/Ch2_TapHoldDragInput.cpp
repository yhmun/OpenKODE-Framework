/* --------------------------------------------------------------------------
 *
 *      File            Ch2_TapHoldDragInput.cpp
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
#include "Ch2_TapHoldDragInput.h"
#include "Libraries/ColorTouchSprite.h"

KDbool Ch2_TapHoldDragInput::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Our message sprite
	m_pMessage->setString ( "Tap, hold or drag the square." );
	
	// Init the ColorTouchSprite
	m_pColorTouchSprite = ColorTouchSprite::create ( "blank.png" );
	m_pColorTouchSprite->setPosition ( ccp ( 240, 160 ) );
	m_pColorTouchSprite->setTextureRect ( CCRect ( 0, 0, 100, 100 ) );
	this->addChild ( m_pColorTouchSprite );

	this->schedule ( schedule_selector ( Ch2_TapHoldDragInput::step ) );

	return KD_TRUE;
}

KDvoid Ch2_TapHoldDragInput::step ( KDfloat fDelta )
{
	// Change color and message depending on ColorTouchSprite state
	KDint	nTouchedState = m_pColorTouchSprite->getTouchedState ( );
	if ( nTouchedState == TS_NONE )
	{
		m_pMessage->setColor ( ccc3 ( 255, 255, 255 ) );
		m_pMessage->setString ( "Tap, hold or drag the square." );
		m_pColorTouchSprite->setColor ( ccc3 ( 255, 255, 255 ) );
	}
	else if ( nTouchedState == TS_TAP )
	{
		m_pMessage->setColor ( ccc3 ( 255, 0, 0 ) );
		m_pMessage->setString ( "Tap." );
		m_pColorTouchSprite->setColor ( ccc3 ( 255, 0, 0 ) );
	}
	else if ( nTouchedState == TS_HOLD )
	{
		m_pMessage->setColor ( ccc3 ( 0, 255, 0 ) );
		m_pMessage->setString ( "Hold." );
		m_pColorTouchSprite->setColor ( ccc3 ( 0, 255, 0 ) );
	}
	else if ( nTouchedState == TS_DRAG )
	{
		m_pMessage->setColor ( ccc3 ( 0, 0, 255 ) );
		m_pMessage->setString ( "Drag." );
		m_pColorTouchSprite->setColor ( ccc3 ( 0, 0, 255 ) );
	}
}

KDvoid Ch2_TapHoldDragInput::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	
	if ( pointIsInRect ( tPoint, m_pColorTouchSprite->rect ( ) ) )
	{
		m_pColorTouchSprite->ccTouchesBegan ( pTouches, pEvent );			
	}
}

KDvoid Ch2_TapHoldDragInput::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	
	if ( pointIsInRect ( tPoint, m_pColorTouchSprite->rect ( ) ) )
	{
		m_pColorTouchSprite->ccTouchesMoved ( pTouches, pEvent );			
	}
}

KDvoid Ch2_TapHoldDragInput::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	
	if ( pointIsInRect ( tPoint, m_pColorTouchSprite->rect ( ) ) )
	{
		m_pColorTouchSprite->ccTouchesEnded ( pTouches, pEvent );			
	}
}