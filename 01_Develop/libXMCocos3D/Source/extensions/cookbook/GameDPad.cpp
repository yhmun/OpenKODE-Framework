/* --------------------------------------------------------------------------
 *
 *      File            GameDPad.cpp
 *      Ported By       Young-Hwan Mun, Jin-Su Park
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

#include "extensions/cookbook/GameDPad.h"

NS_CC_EXT_BEGIN

GameDPad* GameDPad::create ( const KDchar* szNormalSpriteFrame, const KDchar* szHorizontalSpriteFrame, const KDchar* szDiagonalSpriteFrame )
{
	GameDPad*	pRet = new GameDPad ( );

	if ( pRet && pRet->initWithSpriteFrames ( szNormalSpriteFrame, szHorizontalSpriteFrame, szDiagonalSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool GameDPad::initWithSpriteFrames ( const KDchar* szNormalSpriteFrame, const KDchar* szHorizontalSpriteFrame, const KDchar* szDiagonalSpriteFrame )
{
	if ( !TouchableSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_tPressedVector		 = Point ( 0, 0 );
	m_nDirection			 = NO_DIRECTION;	
	m_sNormalSpriteFrame	 = szNormalSpriteFrame;
	m_sHorizontalSpriteFrame = szHorizontalSpriteFrame;
	m_sDiagonalSpriteFrame	 = szDiagonalSpriteFrame;

	// Set the sprite display frame
	this->setDisplayFrameName ( szNormalSpriteFrame );

	this->setTouchColor ( Color3B ( 255, 200, 200 ) );

	return KD_TRUE;
}

KDvoid GameDPad::processTouch ( const Point& tPoint )
{
	TouchableSprite::processTouch ( tPoint );
		
	Rect		tRect	= this->rect ( );
	Point		tCenter = Point ( tRect.origin.x + tRect.size.cx / 2, tRect.origin.y + tRect.size.cy / 2 );
	
	// Process center dead zone
	if ( tPoint.getDistance ( tCenter ) < tRect.size.cx / 10 )
	{
		this->setDisplayFrameName ( m_sNormalSpriteFrame.c_str ( ) );
		this->setRotation ( 0 );
		m_tPressedVector = Point ( 0, 0 );
		m_nDirection	 = NO_DIRECTION;
		return;
	}
	
	// Process direction
	KDfloat		fRadians = Point::pointToRadians ( Point ( tPoint.x - tCenter.x, tPoint.y - tCenter.y ) );
	KDfloat		fDegrees = CC_RADIANS_TO_DEGREES ( fRadians ) + 90;
	KDfloat		fSin45   = 0.7071067812f;

	if ( fDegrees >= 337.5f || fDegrees < 22.5f )
	{
		this->setDisplayFrameName ( m_sHorizontalSpriteFrame.c_str ( ) );
		this->setRotation ( 180 );
		m_tPressedVector = Point ( -1, 0 );
		m_nDirection	 = LEFT;
	}
	else if ( fDegrees >= 22.5f && fDegrees < 67.5f )
	{
		this->setDisplayFrameName ( m_sDiagonalSpriteFrame.c_str ( ) );
		this->setRotation ( -90 );
		m_tPressedVector = Point ( -fSin45, fSin45 );
		m_nDirection	 = UP_LEFT;
	}
	else if ( fDegrees >= 67.5f && fDegrees < 112.5f )
	{
		this->setDisplayFrameName ( m_sHorizontalSpriteFrame.c_str ( ) );
		this->setRotation ( -90 );
		m_tPressedVector = Point ( 0, 1 );
		m_nDirection	 = UP;
	}
	else if ( fDegrees >= 112.5f && fDegrees < 157.5f )
	{
		this->setDisplayFrameName ( m_sDiagonalSpriteFrame.c_str ( ) );
		this->setRotation ( 0 );
		m_tPressedVector = Point ( fSin45, fSin45 );
		m_nDirection	 = UP_RIGHT;
	}
	else if ( fDegrees >= 157.5f && fDegrees < 202.5f )
	{
		this->setDisplayFrameName ( m_sHorizontalSpriteFrame.c_str ( ) );
		this->setRotation ( 0 );
		m_tPressedVector = Point ( 1, 0 );
		m_nDirection	 = RIGHT;
	}
	else if ( fDegrees >= 202.5f && fDegrees < 247.5f )
	{
		this->setDisplayFrameName ( m_sDiagonalSpriteFrame.c_str ( ) );
		this->setRotation ( 90 );
		m_tPressedVector = Point ( fSin45, -fSin45 );
		m_nDirection	 = DOWN_RIGHT;
	}
	else if ( fDegrees >= 247.5f && fDegrees < 292.5f )
	{
		this->setDisplayFrameName ( m_sHorizontalSpriteFrame.c_str ( ) );
		this->setRotation ( 90 );
		m_tPressedVector = Point ( 0, -1 );
		m_nDirection	 = DOWN;
	}
	else
	{
		this->setDisplayFrameName ( m_sDiagonalSpriteFrame.c_str ( ) );
		this->setRotation ( 180 );
		m_tPressedVector = Point ( -fSin45, -fSin45 );
		m_nDirection	 = DOWN_LEFT;
	}
}

KDvoid GameDPad::processRelease ( KDvoid )
{
	TouchableSprite::processRelease ( );

	this->setDisplayFrameName ( m_sNormalSpriteFrame.c_str ( ) );	
	this->setRotation ( 0 );

	m_tPressedVector = Point ( 0, 0 );
	m_nDirection = NO_DIRECTION;
}
 
const Point& GameDPad::getPressedVector ( KDvoid )
{
	return m_tPressedVector;
}

KDvoid GameDPad::setPressedVector ( const Point& tPressedVector )
{
	m_tPressedVector = tPressedVector;
}

KDint GameDPad::getDirection ( KDvoid )
{
	return m_nDirection;
}

KDvoid GameDPad::setDirection ( KDint nDirection )
{
	m_nDirection = nDirection;
}

NS_CC_EXT_END