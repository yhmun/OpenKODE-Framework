/* --------------------------------------------------------------------------
 *
 *      File            Ch2_PinchZooming.cpp
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
#include "Ch2_PinchZooming.h"

KDbool Ch2_PinchZooming::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Set initial variables
	m_bArrowsIn = KD_FALSE;
	m_fCameraZoom = 1.0f;
	m_fLastMultiTouchZoomDistance = 0.0f;
		
	// Init background
	m_pBg = CCSprite::create ( "dracula_castle.jpg" );
	m_pBg->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( m_pBg );
	
	// Set initial zoom
	this->setCameraZoom ( 1 );

	return KD_TRUE;
}

KDvoid Ch2_PinchZooming::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	
	// If HUD has not been touched we reset lastMultiTouchZoomDistance
	if ( !this->hudPressedWithPoint ( tPoint ) )
	{
		m_fLastMultiTouchZoomDistance = 0.0f;
	}
}

KDvoid Ch2_PinchZooming::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	const CCSize&	tLyrSize = this->getContentSize ( );
	
	// Check for only 2 touches
	if ( pTouches->count ( ) == 2 )
	{
		CCArray*	pTwoTouch = pTouches->allObjects ( );

		// Get both touches
		CCTouch*	pTouchOne = (CCTouch*) pTwoTouch->objectAtIndex ( 0 );
		CCTouch*	pTouchTwo = (CCTouch*) pTwoTouch->objectAtIndex ( 1 );

		CCPoint		tFirstTouch  = pTouchOne->getLocation ( );
		CCPoint		tSecondTouch = pTouchTwo->getLocation ( );
			
		// If HUD hasn't been touched we use this distance and last distance to calculate zooming
		if ( !this->hudPressedWithPoint ( tFirstTouch ) && !this->hudPressedWithPoint ( tSecondTouch ) )
		{
			KDfloat		fCurrentDistance = distanceBetweenPoints ( tFirstTouch, tSecondTouch );
		
			if ( m_fLastMultiTouchZoomDistance == 0 )
			{
				m_fLastMultiTouchZoomDistance = fCurrentDistance;
			}
			else
			{
				KDfloat		fDifference = fCurrentDistance - m_fLastMultiTouchZoomDistance;

				KDfloat		fNewZoom = ( m_fCameraZoom + ( fDifference * m_fCameraZoom / tLyrSize.cy ) );
				if ( fNewZoom < 1.0f ) { fNewZoom = 1.0f; }
				if ( fNewZoom > 4.0f ) { fNewZoom = 4.0f; }
				this->setCameraZoom ( fNewZoom );
				m_fLastMultiTouchZoomDistance = fCurrentDistance;
			}			
		}		
	}
}

KDvoid Ch2_PinchZooming::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	// If HUD has not been touched we reset lastMultiTouchZoomDistance
	if ( !this->hudPressedWithPoint ( tPoint ) )
	{
		m_fLastMultiTouchZoomDistance = 0.0f;
	}
}

/// Check for HUD input 
KDbool Ch2_PinchZooming::hudPressedWithPoint ( const CCPoint& tPoint )
{
	// There is no HUD.
	return KD_FALSE;
}

KDvoid Ch2_PinchZooming::setCameraZoom ( KDfloat fZoom )
{
	m_fCameraZoom = fZoom;
	m_pBg->setScale ( fZoom );
	this->setArrows ( );
}

KDvoid Ch2_PinchZooming::setArrows ( KDvoid )
{
	if ( m_bArrowsIn && m_fCameraZoom > 2 )
	{
		m_bArrowsIn = KD_FALSE;
		this->removeChildByTag ( 0 );
		this->removeChildByTag ( 1 );

		CCSprite*	pArrowUp = CCSprite::create ( "arrow_up.png" );
		pArrowUp->setPosition ( ccp ( 240, 260 ) );
		pArrowUp->setScale ( 0.25f );
		pArrowUp->setRotation ( 180 );

		pArrowUp->runAction 
		(
			CCRepeatForever::create 
			(
				CCSequence::create 
				(
					CCSpawn::create 
					(
						CCMoveBy ::create ( 1, ccp ( 0, -100 ) ),
						CCFadeOut::create ( 1 ),
						KD_NULL 
					),
					CCSpawn::create 
					(
						CCMoveBy::create ( 0, ccp ( 0, 100 ) ),
						CCFadeIn::create ( 0 ),
						KD_NULL 
					),
					KD_NULL
				)
			)
		);

		this->addChild ( pArrowUp, 1, 0 );

		CCSprite*	pArrowDown = CCSprite::create ( "arrow_up.png" );
		pArrowDown->setPosition ( ccp ( 240, 60 ) );
		pArrowDown->setScale ( 0.25f );

		pArrowDown->runAction 
		(
			CCRepeatForever::create 
			(
				CCSequence::create 
				(
					CCSpawn::create 
					(
						CCMoveBy ::create ( 1, ccp ( 0, 100 ) ),
						CCFadeOut::create ( 1 ),
						KD_NULL 
					),
					CCSpawn::create 
					(
						CCMoveBy::create ( 0, ccp ( 0, -100 ) ),
						CCFadeIn::create ( 0 ),
						KD_NULL 
					),
					KD_NULL
				)
			)
		);

		this->addChild ( pArrowDown, 1, 1 );
	}
	else if ( !m_bArrowsIn && m_fCameraZoom <= 2 )
	{
		m_bArrowsIn = KD_TRUE;
		this->removeChildByTag ( 0 );
		this->removeChildByTag ( 1 );
		
		CCSprite*	pArrowUp = CCSprite::create ( "arrow_up.png" );
		pArrowUp->setPosition ( ccp ( 240, 210 ) );
		pArrowUp->setScale ( 0.25f );

		pArrowUp->runAction 
		(
			CCRepeatForever::create 
			(
				CCSequence::create 
				(
					CCSpawn::create 
					(
						CCMoveBy ::create ( 1, ccp ( 0, 100 ) ),
						CCFadeOut::create ( 1 ),
						KD_NULL 
					),
					CCSpawn::create 
					(
						CCMoveBy::create ( 0, ccp ( 0, -100 ) ),
						CCFadeIn::create ( 0 ),
						KD_NULL 
					),
					KD_NULL
				)
			)
		);

		this->addChild ( pArrowUp, 1, 0 );

		CCSprite*	pArrowDown = CCSprite::create ( "arrow_up.png" );
		pArrowDown->setPosition ( ccp ( 240, 110 ) );
		pArrowDown->setScale ( 0.25f );
		pArrowDown->setRotation ( 180 );

		pArrowDown->runAction 
		(
			CCRepeatForever::create 
			(
				CCSequence::create 
				(
					CCSpawn::create 
					(
						CCMoveBy ::create ( 1, ccp ( 0, -100 ) ),
						CCFadeOut::create ( 1 ),
						KD_NULL 
					),
					CCSpawn::create 
					(
						CCMoveBy::create ( 0, ccp ( 0, 100 ) ),
						CCFadeIn::create ( 0 ),
						KD_NULL 
					),
					KD_NULL
				)
			)
		);

		this->addChild ( pArrowDown, 1, 1 );
	}
}
