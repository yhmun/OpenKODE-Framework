/* --------------------------------------------------------------------------
 *
 *      File            Ch2_DPad.cpp
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
#include "Ch2_DPad.h"
#include "Libraries/SimpleAnimObject.h"
#include "Libraries/DPad.h"

KDbool Ch2_DPad::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Add gunman sprites
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "gunman.plist" );

	// Initialize gunman
	m_pGunman = SimpleAnimObject::createWithSpriteFrame ( pCache->spriteFrameByName ( "gunman_stand_down.png" ) );
	m_pGunman->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( m_pGunman );
	m_nGunmanDirection = DOWN;

	// Initialize message
	m_pMessage->setString ( "DPad Vector:" );
		
	// Initialize DPad
	pCache->addSpriteFramesWithFile ( "dpad_buttons.plist" );
	m_pDPad = DPad::create ( );
	m_pDPad->setPosition ( ccp ( 100, 100 ) );
	this->addChild ( m_pDPad );

	this->schedule ( schedule_selector ( Ch2_DPad::step ) );

	return KD_TRUE;
}

KDvoid Ch2_DPad::step ( KDfloat fDelta )
{
	CCPoint		tPressedVector = m_pDPad->getPressedVector ( );
	KDint		nDirection = m_pDPad->getDirection ( );
	
	// Tell the user our DPad vector
	m_pMessage->setString ( ccszf ( "DPad Vector:%f %f", tPressedVector.x, tPressedVector.y ) );
	
	KDbool		bResetAnimation = KD_FALSE;
	
	// We reset the animation if the gunman changes direction
	if ( nDirection != NO_DIRECTION )
	{ 
		if ( m_nGunmanDirection != nDirection )
		{
			bResetAnimation = KD_TRUE;
			m_nGunmanDirection = nDirection;
		}
	}

	CCPoint		tGunmanVelocity = m_pGunman->getVelocity ( );
	if ( tGunmanVelocity.x != tPressedVector.x * 2 || tGunmanVelocity.y != tPressedVector.y * 2 )
	{
		m_pGunman->setVelocity ( ccp ( tPressedVector.x * 2, tPressedVector.y * 2 ) );
		bResetAnimation = KD_TRUE;
	}
	
	// Update gunman position
	m_pGunman->update ( fDelta );

	// Re-animate if necessary
	if ( bResetAnimation )
	{
		this->animateGunman ( );
	}
}

KDvoid Ch2_DPad::animateGunman ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Animate our gunman 
	CCAnimation*	pAnimation = CCAnimation::create ( ); 
	pAnimation->setDelayPerUnit ( 0.15f );

	const KDchar*	szDirection;
	KDbool			bFlipX  = KD_FALSE;
	KDbool			bMoving = KD_TRUE;

	CCPoint			tVelocity = m_pGunman->getVelocity ( );
	if ( tVelocity.x == 0 && tVelocity.y == 0 )
	{
		bMoving = KD_FALSE; 
	}

	switch ( m_nGunmanDirection )
	{
		case LEFT			:	szDirection = "right";		bFlipX = KD_TRUE;	break;
		case UP_LEFT		:	szDirection = "up_right";	bFlipX = KD_TRUE;	break;
		case UP			:	szDirection = "up";								break;
		case UP_RIGHT		:	szDirection = "up_right";						break;
		case RIGHT			:	szDirection = "right";							break;
		case DOWN_RIGHT	:	szDirection = "down_right";						break;
		case DOWN			:	szDirection = "down";							break;
		case DOWN_LEFT		:	szDirection = "down_right";	bFlipX = KD_TRUE;	break;
	}

	// Our simple running loop
	if ( bMoving )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_run_%s_01.png", szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_run_%s_02.png", szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
	}
	else
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
	}
	
	m_pGunman->setFlipX ( bFlipX );
	m_pGunman->stopAllActions ( );
	m_pGunman->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );
}

KDvoid Ch2_DPad::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pDPad->ccTouchesBegan ( pTouches, pEvent );
}

KDvoid Ch2_DPad::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pDPad->ccTouchesMoved ( pTouches, pEvent );
}

KDvoid Ch2_DPad::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pDPad->ccTouchesEnded ( pTouches, pEvent );
}
