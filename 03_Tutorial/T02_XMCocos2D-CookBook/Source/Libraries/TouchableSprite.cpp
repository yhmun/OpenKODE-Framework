/* --------------------------------------------------------------------------
 *
 *      File            TouchableSprite.cpp
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
#include "TouchableSprite.h"

TouchableSprite* TouchableSprite::create ( KDvoid )
{
	TouchableSprite*		pRet = new TouchableSprite ( );

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

KDbool TouchableSprite::init ( KDvoid )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_bPressed = KD_FALSE;

	return KD_TRUE;
}

KDvoid TouchableSprite::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	// We use circle collision for our buttons
	if ( pointIsInCircle ( tPoint, this->getPosition ( ), this->rect ( ).size.cx / 2 ) )
	{		
		m_nTouchHash = pTouch->getID ( );
		this->processTouch ( tPoint );
	}
}

KDvoid TouchableSprite::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	
	if ( pointIsInCircle ( tPoint, this->getPosition ( ), this->rect ( ).size.cx / 2 ) )
	{
		if ( m_nTouchHash == pTouch->getID ( ) )	// If we moved on this sprite
		{		
			this->processTouch ( tPoint );	
		}
		else if ( !m_bPressed )						// If a new touch moves onto this sprite
		{					
			m_nTouchHash = pTouch->getID ( );
			this->processTouch ( tPoint );	
		}
	}
	else if ( m_nTouchHash == pTouch->getID ( ) )	// If we moved off of this sprite
	{
		this->processRelease ( );
	}
}

KDvoid TouchableSprite::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	
	if ( m_nTouchHash == pTouch->getID ( ) )
	{
		// If the touch which pressed this sprite ended we release
		this->processRelease ( );
	}
}

KDbool TouchableSprite::checkTouchWithPoint ( const CCPoint& tPoint )
{
	if ( pointIsInRect ( tPoint, this->rect ( ) ) )
	{
		return KD_TRUE;
	}
	else
	{
		return KD_FALSE;
	}
}

CCRect TouchableSprite::rect ( KDvoid )
{
	// We set our scale mod to make sprite easier to press.
	// This also lets us press 2 sprites with 1 touch if they are sufficiently close.
	KDfloat		fScaleMod = 1.5f;
	KDfloat		fScale = this->getScale ( );
	
	CCSize		tSize = this->getContentSize ( );
	tSize.cx = tSize.cx * fScale * fScaleMod;
	tSize.cy = tSize.cy * fScale * fScaleMod;
	
	CCPoint		tPoint = this->getPosition ( );
	tPoint.x = tPoint.x - tSize.cx / 2;
	tPoint.y = tPoint.y - tSize.cy / 2;
	
	return CCRect ( tPoint.x, tPoint.y, tSize.cx, tSize.cy ); 
}

KDvoid TouchableSprite::processTouch ( const CCPoint& tPoint )
{
	m_bPressed = KD_TRUE;
}

KDvoid TouchableSprite::processRelease ( KDvoid )
{
	m_bPressed = KD_FALSE;
}

KDbool TouchableSprite::isPressed ( KDvoid )
{
	return m_bPressed;
}

KDvoid TouchableSprite::setPressed ( KDbool bPressed )
{
	m_bPressed = bPressed;
}

KDint TouchableSprite::getTouchHash ( KDvoid )
{
	return m_nTouchHash;
}

KDvoid TouchableSprite::setTouchHash ( KDint nTouchHash )
{
	m_nTouchHash = nTouchHash;
}