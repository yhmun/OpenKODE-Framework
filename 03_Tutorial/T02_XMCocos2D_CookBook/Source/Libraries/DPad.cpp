/* --------------------------------------------------------------------------
 *
 *      File            DPad.cpp
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
#include "DPad.h"

DPad* DPad::create ( KDvoid )
{
	DPad*		pRet = new DPad ( );

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

KDbool DPad::init ( KDvoid )
{
	if ( !TouchableSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_tPressedVector = ccp ( 0, 0 );
	m_nDirection = NO_DIRECTION;
	
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "dpad_buttons.plist" );
	
	// Set the sprite display frame
	this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_normal.png" ) );

	return KD_TRUE;
}

KDvoid DPad::processTouch ( const CCPoint& tPoint )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	
	// Set a color visual cue if pressed
	this->setColor ( ccc3 ( 255, 200, 200 ) );
	m_bPressed = KD_TRUE;
		
	CCRect		tRect	= this->rect ( );
	CCPoint		tCenter = ccp ( tRect.origin.x + tRect.size.cx / 2, tRect.origin.y + tRect.size.cy / 2 );
	
	// Process center dead zone
	if ( distanceBetweenPoints ( tPoint, tCenter ) < tRect.size.cx / 10 )
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_normal.png" ) );
		this->setRotation ( 0 );
		m_tPressedVector = ccp ( 0, 0 );
		m_nDirection	 = NO_DIRECTION;
		return;
	}
	
	// Process direction
	KDfloat		fRadians = vectorToRadians ( ccp ( tPoint.x - tCenter.x, tPoint.y - tCenter.y ) );
	KDfloat		fDegrees = radiansToDegrees ( fRadians ) + 90;
	KDfloat		fSin45   = 0.7071067812f;

	if ( fDegrees >= 337.5f || fDegrees < 22.5f )
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_horizontal.png" ) );
		this->setRotation ( 180 );
		m_tPressedVector = ccp ( -1, 0 );
		m_nDirection	 = LEFT;
	}
	else if ( fDegrees >= 22.5f && fDegrees < 67.5f )
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_diagonal.png" ) );
		this->setRotation ( -90 );
		m_tPressedVector = ccp ( -fSin45, fSin45 );
		m_nDirection	 = UP_LEFT;
	}
	else if ( fDegrees >= 67.5f && fDegrees < 112.5f )
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_horizontal.png" ) );
		this->setRotation ( -90 );
		m_tPressedVector = ccp ( 0, 1 );
		m_nDirection	 = UP;
	}
	else if ( fDegrees >= 112.5f && fDegrees < 157.5f )
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_diagonal.png" ) );
		this->setRotation ( 0 );
		m_tPressedVector = ccp ( fSin45, fSin45 );
		m_nDirection	 = UP_RIGHT;
	}
	else if ( fDegrees >= 157.5f && fDegrees < 202.5f )
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_horizontal.png" ) );
		this->setRotation ( 0 );
		m_tPressedVector = ccp ( 1, 0 );
		m_nDirection	 = RIGHT;
	}
	else if ( fDegrees >= 202.5f && fDegrees < 247.5f )
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_diagonal.png" ) );
		this->setRotation ( 90 );
		m_tPressedVector = ccp ( fSin45, -fSin45 );
		m_nDirection	 = DOWN_RIGHT;
	}
	else if ( fDegrees >= 247.5f && fDegrees < 292.5f )
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_horizontal.png" ) );
		this->setRotation ( 90 );
		m_tPressedVector = ccp ( 0, -1 );
		m_nDirection	 = DOWN;
	}
	else
	{
		this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_diagonal.png" ) );
		this->setRotation ( 180 );
		m_tPressedVector = ccp ( -fSin45, -fSin45 );
		m_nDirection	 = DOWN_LEFT;
	}
}

KDvoid DPad::processRelease ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	this->setDisplayFrame ( pCache->spriteFrameByName ( "d_pad_normal.png" ) );	
	this->setRotation ( 0 );
	this->setColor ( ccc3 ( 255, 255, 255 ) );

	m_bPressed = KD_FALSE;
	m_tPressedVector = ccp ( 0, 0 );
	m_nDirection = NO_DIRECTION;
}
 
const CCPoint& DPad::getPressedVector ( KDvoid )
{
	return m_tPressedVector;
}

KDvoid DPad::setPressedVector ( const CCPoint& tPressedVector )
{
	m_tPressedVector = tPressedVector;
}

KDint DPad::getDirection ( KDvoid )
{
	return m_nDirection;
}

KDvoid DPad::setDirection ( KDint nDirection )
{
	m_nDirection = nDirection;
}
