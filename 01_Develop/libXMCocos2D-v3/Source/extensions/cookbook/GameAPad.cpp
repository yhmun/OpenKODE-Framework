/* --------------------------------------------------------------------------
 *
 *      File            GameAPad.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. 
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

#include "extensions/cookbook/GameAPad.h"

NS_CC_EXT_BEGIN

GameAPad* GameAPad::create ( const KDchar* szPadSpriteFrame, const KDchar* szBarSpriteFrame, const KDchar* szNubSpriteFrame )
{
	GameAPad*	pRet = new GameAPad ( );

	if ( pRet && pRet->initWithSpriteFrames ( szPadSpriteFrame, szBarSpriteFrame, szNubSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool GameAPad::initWithSpriteFrames ( const KDchar* szPadSpriteFrame, const KDchar* szBarSpriteFrame, const KDchar* szNubSpriteFrame )
{
	if ( !TouchableSprite::init ( ) )
	{
		return KD_FALSE;
	}
		
	m_tPressedVector	= Point::ZERO;
	m_nDirection		= NO_DIRECTION;
	m_sPadSpriteFrame	= szPadSpriteFrame;
	m_sBarSpriteFrame	= szBarSpriteFrame;
	m_sNubSpriteFrame	= szNubSpriteFrame;
		
	// Set the sprite display frame
	this->setDisplayFrameName ( szPadSpriteFrame );
		
	this->setTouchColor ( Color3B ( 255, 200, 200 ) );

	// Init the bar, set position and display frame
	m_pBar = Sprite::create ( );
	m_pBar->setDisplayFrameName ( szBarSpriteFrame );	
	this->addChild ( m_pBar );
	
	// Init the nub, set position and display frame
	m_pNub = Sprite::create ( );	
	m_pNub->setDisplayFrameName ( szNubSpriteFrame );
	this->addChild ( m_pNub );	

	this->repositionBarWithPoint ( this->getPosition ( ) );
	this->repositionNub ( );	

	return KD_TRUE;
}

KDvoid GameAPad::processTouch ( const Point& tPoint )
{
	TouchableSprite::processTouch ( tPoint );

	m_pNub->setColor ( m_tTouchColor );
	m_pBar->setColor ( m_tTouchColor );

	Rect		tRect = this->rect ( );
	Point		tCenter = Point ( tRect.origin.x + tRect.size.cx / 2, tRect.origin.y + tRect.size.cy / 2 );
	m_tPressedVector = Point ( ( tPoint.x - tCenter.x ) / ( tRect.size.cx / 2 ), ( tPoint.y - tCenter.y ) / ( tRect.size.cy / 2 ) );

	this->repositionNub ( );
	this->repositionBarWithPoint ( tPoint );
	this->resetDirection ( );
}

KDvoid GameAPad::processRelease ( KDvoid )
{
	TouchableSprite::processRelease ( );

	m_pNub->setColor ( Color3B ( 255, 255, 255 ) );
	m_pBar->setColor ( Color3B ( 255, 255, 255 ) );

	m_tPressedVector = Point ( 0, 0 );

	this->repositionNub ( );
	this->repositionBarWithPoint ( this->getPosition ( ) );
}
 
KDvoid GameAPad::repositionNub ( KDvoid )
{
	const Size&		tSize = this->getContentSize ( );

	m_pNub->setPosition ( Point ( m_tPressedVector.x * ( tSize.cx / 2 ) + tSize.cx / 2, m_tPressedVector.y * ( tSize.cy / 2 ) + tSize.cy / 2 ) );
}

KDvoid GameAPad::repositionBarWithPoint ( const Point& tPoint )
{
	const Size	tSize = this->getContentSize ( );

	// Rotation	
	KDfloat		fRadians = Point::pointToRadians ( m_tPressedVector );
	KDfloat		fDegrees = CC_RADIANS_TO_DEGREES ( fRadians );	
	m_pBar->setRotation ( fDegrees );

	m_pBar->setDisplayFrameName ( m_sBarSpriteFrame.c_str ( ) );
	m_pNub->setDisplayFrameName ( m_sNubSpriteFrame.c_str ( ) );

	Rect		tBarTexRect = m_pBar->getTextureRect ( );
	Rect		tNubTexRect = m_pNub->getTextureRect ( );

	KDfloat		fDistFromCenter = tPoint.getDistance ( this->getPosition ( ) );
	KDfloat		fSizeMod = fDistFromCenter / tSize.cx;
	KDfloat		fOldHeight = tBarTexRect.size.cy;
	KDfloat		fNewHeight = fOldHeight * fSizeMod * 2;

	// Custom fixes
	if ( fNewHeight < tNubTexRect.size.cx * 0.75 )
	{
		fNewHeight = tNubTexRect.size.cx * 0.75;
	}
	if ( fDistFromCenter < 3 )
	{
		fNewHeight = 0.0f; 
	}

	m_pBar->setTextureRect ( Rect ( tBarTexRect.origin.x, tBarTexRect.origin.y + ( fOldHeight - fNewHeight ), tBarTexRect.size.cx, fNewHeight ) );	
	m_pBar->setAnchorPoint ( Point ( 0.5f, 0 ) );

	Point		tDirectionVector = Point::radiansToPoint ( fRadians - KD_PI_F / 2 );
	m_pBar->setPosition ( Point ( tSize.cx / 2 + tDirectionVector.x * tSize.cx / 4, tSize.cy / 2 + tDirectionVector.y * tSize.cy / 4 ) );
}

KDvoid GameAPad::resetDirection ( KDvoid )
{
	if ( m_tPressedVector.x == 0 && m_tPressedVector.y == 0 )
	{
		m_nDirection = NO_DIRECTION;
		return;
	}

	KDfloat		fRadians = Point::pointToRadians ( m_tPressedVector );
	KDfloat		fDegrees = CC_RADIANS_TO_DEGREES ( fRadians ) + 90;
	
	if ( fDegrees >= 337.5f || fDegrees < 22.5f )
	{
		m_nDirection = LEFT;
	}
	else if ( fDegrees >= 22.5f && fDegrees < 67.5f )
	{
		m_nDirection = UP_LEFT;
	}
	else if ( fDegrees >= 67.5f && fDegrees < 112.5f )
	{
		m_nDirection = UP;
	}
	else if ( fDegrees >= 112.5f && fDegrees < 157.5f )
	{
		m_nDirection = UP_RIGHT;
	}
	else if ( fDegrees >= 157.5f && fDegrees < 202.5f )
	{
		m_nDirection = RIGHT;
	}
	else if ( fDegrees >= 202.5f && fDegrees < 247.5f )
	{
		m_nDirection = DOWN_RIGHT;
	}
	else if ( fDegrees >= 247.5f && fDegrees < 292.5f )
	{
		m_nDirection = DOWN;
	}
	else
	{
		m_nDirection = DOWN_LEFT;
	}
}

KDfloat GameAPad::magnitude ( KDvoid )
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

Point GameAPad::getPressedVector ( KDvoid )
{
	KDfloat		fMagnitude = kdSqrtf ( kdPowf ( m_tPressedVector.x, 2 ) + kdPowf ( m_tPressedVector.y, 2 ) );
	fMagnitude += 0.25f;
	
	Point		tPressedVector = Point ( m_tPressedVector.x * 1.25f, m_tPressedVector.y * 1.25f );
	
	// 25% end deadzone to make it easier to hold highest magnitude
	if ( fMagnitude > 1 )
	{
		KDfloat		fRadians = Point::pointToRadians ( m_tPressedVector );
		tPressedVector = Point::radiansToPoint ( fRadians + KD_PI_F / 2 );
	}
	
	return tPressedVector;
}

KDint GameAPad::getDirection ( KDvoid )
{
	return m_nDirection;
}

KDvoid GameAPad::setDirection ( KDint nDirection )
{
	m_nDirection = nDirection;
}

Sprite* GameAPad::getNub ( KDvoid )
{
	return m_pNub;
}

KDvoid GameAPad::setNub ( Sprite* pNub )
{
	m_pNub = pNub;
}

Sprite* GameAPad::getBar ( KDvoid )
{
	return m_pBar;
}

KDvoid GameAPad::setBar ( Sprite* pBar )
{
	m_pBar = pBar;
}

NS_CC_EXT_END