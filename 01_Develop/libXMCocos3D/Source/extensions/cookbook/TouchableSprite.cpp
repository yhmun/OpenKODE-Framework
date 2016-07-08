/* --------------------------------------------------------------------------
 *
 *      File            TouchableSprite.cpp
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

#include "extensions/cookbook/TouchableSprite.h"

NS_CC_EXT_BEGIN

TouchableSprite* TouchableSprite::create ( KDvoid )
{
	TouchableSprite*	pRet = new TouchableSprite ( );

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
	if ( !Sprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_bPressed		= KD_FALSE;
	m_fScaleMod		= 1.5f;
	m_tTouchColor	= Color3B ( 255, 255, 255 );

	return KD_TRUE;
}

KDvoid TouchableSprite::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	if ( this->getParent ( ) == KD_NULL )
	{
		return;
	}

	Touch*		pTouch = (Touch*) aTouches [ 0 ];
	Point		tPoint = this->getParent ( )->convertTouchToNodeSpace ( pTouch );

	// We use circle collision for our buttons
	if ( tPoint.isInCircle ( this->getPosition ( ), this->rect ( ).size.cx / 2 ) )
	{		
		m_nTouchHash = pTouch->getID ( );
		this->processTouch ( tPoint );
	}
}

KDvoid TouchableSprite::onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	if ( this->getParent ( ) == KD_NULL )
	{
		return;
	}

	Touch*		pTouch = (Touch*) aTouches [ 0 ];
	Point		tPoint = this->getParent ( )->convertTouchToNodeSpace ( pTouch );
	
	if ( tPoint.isInCircle ( this->getPosition ( ), this->rect ( ).size.cx / 2 ) )
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

KDvoid TouchableSprite::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	Touch*		pTouch = (Touch*) aTouches [ 0 ];

	if ( m_nTouchHash == pTouch->getID ( ) )
	{
		// If the touch which pressed this sprite ended we release
		this->processRelease ( );
	}
}

KDbool TouchableSprite::checkTouchWithPoint ( const Point& tPoint )
{	
	if ( tPoint.isInRect ( this->rect ( ) ) )
	{
		return KD_TRUE;
	}
	else
	{
		return KD_FALSE;
	}
}

Rect TouchableSprite::rect ( KDvoid )
{
	// We set our scale mod to make sprite easier to press.
	// This also lets us press 2 sprites with 1 touch if they are sufficiently close.
	KDfloat		fScale = this->getScale ( );	
	Size		tSize  = this->getContentSize ( );
	tSize.cx = tSize.cx * fScale * m_fScaleMod;
	tSize.cy = tSize.cy * fScale * m_fScaleMod;
	
	Point		tPoint = this->getPosition ( );
	tPoint.x = tPoint.x - tSize.cx / 2;
	tPoint.y = tPoint.y - tSize.cy / 2;
	
	return Rect ( tPoint.x, tPoint.y, tSize.cx, tSize.cy ); 
}

KDvoid TouchableSprite::processTouch ( const Point& tPoint )
{
	m_bPressed = KD_TRUE;

	this->setColor ( m_tTouchColor );
}

KDvoid TouchableSprite::processRelease ( KDvoid )
{
	m_bPressed = KD_FALSE;

	this->setColor ( Color3B ( 255, 255, 255 ) );
}

KDbool TouchableSprite::isPressed ( KDvoid )
{
	return m_bPressed;
}

KDvoid TouchableSprite::setPressed ( KDbool bPressed )
{
	m_bPressed = bPressed;

	if ( bPressed )
	{
		this->setColor ( m_tTouchColor );
	}
	else
	{
		this->setColor ( Color3B ( 255, 255, 255 ) );
	}
}

KDint TouchableSprite::getTouchHash ( KDvoid )
{
	return m_nTouchHash;
}

KDvoid TouchableSprite::setTouchHash ( KDint nTouchHash )
{
	m_nTouchHash = nTouchHash;
}

KDfloat TouchableSprite::getScaleMod ( KDvoid )
{
	return m_fScaleMod;
}

KDvoid TouchableSprite::setScaleMod ( KDfloat fScaleMod )
{
	m_fScaleMod = fScaleMod;
}

const Color3B& TouchableSprite::getTouchColor ( KDvoid )
{
	return m_tTouchColor;
}

KDvoid TouchableSprite::setTouchColor ( const Color3B& tColor )
{
	m_tTouchColor = tColor;
}

NS_CC_EXT_END