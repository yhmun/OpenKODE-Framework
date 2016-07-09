/* --------------------------------------------------------------------------
 *
 *      File            AnalogStick.cpp
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
#include "AnalogStick.h"

AnalogStick* AnalogStick::create ( KDvoid )
{
	AnalogStick*	pRet = new AnalogStick ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool AnalogStick::init ( KDvoid )
{
	if ( !TouchableSprite::init ( ) )
	{
		return KD_FALSE;
	}

	this->setScale ( 0.5f );
		
	m_tPressedVector = ccp ( 0, 0 );
	m_nDirection = AS_NO_DIRECTION;
		
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "analog_stick.plist" );
		
	// Set the sprite display frame
	this->setDisplayFrame ( pCache->spriteFrameByName ( "analog_pad.png" ) );
		
	// Init the bar, set position and display frame
	m_pBar = CCSprite::create ( );
	m_pBar->setDisplayFrame ( pCache->spriteFrameByName ( "analog_bar.png" ) );
	this->repositionBarWithPoint ( this->getPosition ( ) );
	this->addChild ( m_pBar );
	
	// Init the nub, set position and display frame
	m_pNub = CCSprite::create ( );
	this->repositionNub ( );
	m_pNub->setDisplayFrame ( pCache->spriteFrameByName ( "analog_nub.png" ) );
	this->addChild ( m_pNub );	

	return KD_TRUE;
}

KDvoid AnalogStick::processTouch ( const CCPoint& tPoint )
{
	m_bPressed = KD_TRUE;

	this  ->setColor ( ccc3 ( 255, 200, 200 ) );
	m_pNub->setColor ( ccc3 ( 255, 200, 200 ) );
	m_pBar->setColor ( ccc3 ( 255, 200, 200 ) );

	CCRect		tRect = this->rect ( );
	CCPoint		tCenter = ccp ( tRect.origin.x + tRect.size.cx / 2, tRect.origin.y + tRect.size.cy / 2 );
	m_tPressedVector = ccp ( ( tPoint.x - tCenter.x ) / ( tRect.size.cx / 2 ), ( tPoint.y - tCenter.y ) / ( tRect.size.cy / 2 ) );

	this->repositionNub ( );
	this->repositionBarWithPoint ( tPoint );
	this->resetDirection ( );
}

KDvoid AnalogStick::processRelease ( KDvoid )
{
	this  ->setColor ( ccc3 ( 255, 255, 255 ) );
	m_pNub->setColor ( ccc3 ( 255, 255, 255 ) );
	m_pBar->setColor ( ccc3 ( 255, 255, 255 ) );
	
	m_bPressed = KD_FALSE;

	m_tPressedVector = ccp ( 0, 0 );

	this->repositionNub ( );
	this->repositionBarWithPoint ( this->getPosition ( ) );
}
 
KDvoid AnalogStick::repositionNub ( KDvoid )
{
	const CCSize&		tSize = this->getContentSize ( );

	m_pNub->setPosition ( ccp ( m_tPressedVector.x * ( tSize.cx / 2 ) + tSize.cx / 2, m_tPressedVector.y * ( tSize.cy / 2 ) + tSize.cy / 2 ) );
}

KDvoid AnalogStick::repositionBarWithPoint ( const CCPoint& tPoint )
{
	const CCSize	tSize = this->getContentSize ( );

	// Rotation	
	KDfloat		fRadians = vectorToRadians ( m_tPressedVector );
	KDfloat		fDegrees = radiansToDegrees ( fRadians );	
	m_pBar->setRotation ( fDegrees );


	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	m_pBar->setDisplayFrame ( pCache->spriteFrameByName ( "analog_bar.png" ) );

	CCRect		tBarTexRect = m_pBar->getTextureRect ( );

	KDfloat		fDistFromCenter = distanceBetweenPoints ( tPoint, this->getPosition ( ) );
	KDfloat		fSizeMod = fDistFromCenter / tSize.cx;
	KDfloat		fOldHeight = tBarTexRect.size.cy;
	KDfloat		fNewHeight = fOldHeight * fSizeMod * 5;

	// Custom fixes
	if ( fNewHeight < 100 )
	{
		fNewHeight = 100.0f; 
	}
	if ( fDistFromCenter < 3 )
	{
		fNewHeight = 0.0f; 
	}

	m_pBar->setTextureRect ( CCRect ( tBarTexRect.origin.x, tBarTexRect.origin.y + ( fOldHeight - fNewHeight ), tBarTexRect.size.cx, fNewHeight ) );	
	m_pBar->setAnchorPoint ( ccp ( 0.5f, 0 ) );

	CCPoint		tDirectionVector = radiansToVector ( fRadians - KD_PI_F / 2 );
	m_pBar->setPosition ( ccp ( tSize.cx / 2 + tDirectionVector.x * tSize.cx / 4, tSize.cy / 2 + tDirectionVector.y * tSize.cy / 4 ) );
}

KDvoid AnalogStick::resetDirection ( KDvoid )
{
	if ( m_tPressedVector.x == 0 && m_tPressedVector.y == 0 )
	{
		m_nDirection = AS_NO_DIRECTION;
		return;
	}

	KDfloat		fRadians = vectorToRadians ( m_tPressedVector );
	KDfloat		fDegrees = radiansToDegrees ( fRadians ) + 90;
	
	if ( fDegrees >= 337.5f || fDegrees < 22.5f )
	{
		m_nDirection = AS_LEFT;
	}
	else if ( fDegrees >= 22.5f && fDegrees < 67.5f )
	{
		m_nDirection = AS_UP_LEFT;
	}
	else if ( fDegrees >= 67.5f && fDegrees < 112.5f )
	{
		m_nDirection = AS_UP;
	}
	else if ( fDegrees >= 112.5f && fDegrees < 157.5f )
	{
		m_nDirection = AS_UP_RIGHT;
	}
	else if ( fDegrees >= 157.5f && fDegrees < 202.5f )
	{
		m_nDirection = AS_RIGHT;
	}
	else if ( fDegrees >= 202.5f && fDegrees < 247.5f )
	{
		m_nDirection = AS_DOWN_RIGHT;
	}
	else if ( fDegrees >= 247.5f && fDegrees < 292.5f )
	{
		m_nDirection = AS_DOWN;
	}
	else
	{
		m_nDirection = AS_DOWN_LEFT;
	}
}

KDfloat AnalogStick::magnitude ( KDvoid )
{
	KDfloat		fMagnitude = kdSqrtf ( kdPowf ( m_tPressedVector.x, 2 ) + kdPowf ( m_tPressedVector.y, 2 ) );

	// 25% end deadzone to make it easier to hold highest magnitude
	fMagnitude += 0.25f;
	if ( fMagnitude > 1.0f )
	{
		fMagnitude = 1.0f; 
	}

	return fMagnitude;
}

CCPoint AnalogStick::getPressedVector ( KDvoid )
{
	KDfloat		fMagnitude = kdSqrtf ( kdPowf ( m_tPressedVector.x, 2 ) + kdPowf ( m_tPressedVector.y, 2 ) );
	fMagnitude += 0.25f;
	
	CCPoint		tPressedVector = ccp ( m_tPressedVector.x * 1.25f, m_tPressedVector.y * 1.25f );
	
	// 25% end deadzone to make it easier to hold highest magnitude
	if ( fMagnitude > 1 )
	{
		KDfloat		fRadians = vectorToRadians ( m_tPressedVector );
		tPressedVector = radiansToVector ( fRadians + KD_PI_F / 2 );
	}
	
	return tPressedVector;
}

KDint AnalogStick::getDirection ( KDvoid )
{
	return m_nDirection;
}

KDvoid AnalogStick::setDirection ( KDint nDirection )
{
	m_nDirection = nDirection;
}

CCSprite* AnalogStick::getNub ( KDvoid )
{
	return m_pNub;
}

KDvoid AnalogStick::setNub ( CCSprite* pNub )
{
	m_pNub = pNub;
}

CCSprite* AnalogStick::getBar ( KDvoid )
{
	return m_pBar;
}

KDvoid AnalogStick::setBar ( CCSprite* pBar )
{
	m_pBar = pBar;
}