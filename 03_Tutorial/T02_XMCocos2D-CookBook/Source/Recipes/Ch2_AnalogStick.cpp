/* --------------------------------------------------------------------------
 *
 *      File            Ch2_AnalogStick.cpp
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
#include "Ch2_AnalogStick.h"
#include "Libraries/SimpleAnimObject.h"
#include "Libraries/AnalogStick.h"

KDbool Ch2_AnalogStick::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "gunman.plist" );

	// Initialize gunman
	m_pGunman = SimpleAnimObject::createWithSpriteFrame ( pCache->spriteFrameByName ( "gunman_stand_down.png" ) );
	m_pGunman->setPosition ( ccp ( 240, 160 ) );
	m_pGunman->setVelocity ( ccp ( 0, 0 ) );
	this->addChild ( m_pGunman );
	m_nGunmanDirection = AS_DOWN;

	m_pMessage->setString ( "Analog Vector:" );
		
	// Initialize analog stick
	pCache->addSpriteFramesWithFile ( "analog_stick.plist" );
	m_pAnalogStick = AnalogStick::create ( );
	m_pAnalogStick->setPosition ( ccp ( 100, 100 ) );
	this->addChild ( m_pAnalogStick );

	// This sets off a chain reaction.
	this->animateGunman ( );

	this->schedule ( schedule_selector ( Ch2_AnalogStick::step ) );

	return KD_TRUE;
}

KDvoid Ch2_AnalogStick::step ( KDfloat fDelta )
{
	CCPoint		tPressedVector = m_pAnalogStick->getPressedVector ( );
	KDint		nDirection = m_pAnalogStick->getDirection ( );
	
	m_pMessage->setString ( ccszf ( "Analog Vector:%f %f", tPressedVector.x, tPressedVector.y ) );
	
	if ( nDirection != AS_NO_DIRECTION )
	{ 
		if ( m_nGunmanDirection != nDirection )
		{
			m_pGunman->stopAllActions ( );
			m_nGunmanDirection = nDirection;
			this->animateGunman ( );
		}
	}

	m_pGunman->setVelocity ( ccp ( tPressedVector.x * 4, tPressedVector.y * 4 ) );
	
	m_pGunman->update ( fDelta );
}

KDvoid Ch2_AnalogStick::animateGunman ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	KDfloat			fSpeed = m_pAnalogStick->magnitude ( );

	// Animation delay is inverse speed
	KDfloat			fDelay = 0.075f / fSpeed;
	if ( fDelay > 0.5f )
	{
		fDelay = 0.5f; 
	}
	CCAnimation*	pAnimation = CCAnimation::create ( ); 
	pAnimation->setDelayPerUnit ( fDelay );

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
		case AS_LEFT		:	szDirection = "right";		bFlipX = KD_TRUE;	break;
		case AS_UP_LEFT		:	szDirection = "up_right";	bFlipX = KD_TRUE;	break;
		case AS_UP			:	szDirection = "up";								break;
		case AS_UP_RIGHT	:	szDirection = "up_right";						break;
		case AS_RIGHT		:	szDirection = "right";							break;
		case AS_DOWN_RIGHT	:	szDirection = "down_right";						break;
		case AS_DOWN		:	szDirection = "down";							break;
		case AS_DOWN_LEFT	:	szDirection = "down_right";	bFlipX = KD_TRUE;	break;
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

	// animateGunman calls itself indefinitely
	m_pGunman->runAction 
	(
		CCSequence::create 
		(
			CCAnimate::create ( pAnimation ),
			CCCallFunc::create ( this, callfunc_selector ( Ch2_AnalogStick::animateGunman ) ),
			KD_NULL
		)
	);
}

KDvoid Ch2_AnalogStick::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pAnalogStick->ccTouchesBegan ( pTouches, pEvent );
}

KDvoid Ch2_AnalogStick::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pAnalogStick->ccTouchesMoved ( pTouches, pEvent );
}

KDvoid Ch2_AnalogStick::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pAnalogStick->ccTouchesEnded ( pTouches, pEvent );
}
