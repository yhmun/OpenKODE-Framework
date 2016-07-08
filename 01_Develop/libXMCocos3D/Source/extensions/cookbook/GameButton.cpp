/* --------------------------------------------------------------------------
 *
 *      File            GameButton.cpp
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

#include "extensions/cookbook/GameButton.h"

NS_CC_EXT_BEGIN

GameButton* GameButton::create ( const KDchar* szUpSpriteFrame, const KDchar* szDownSpriteFrame )
{
	GameButton*		pRet = new GameButton ( );

	if ( pRet && pRet->initWithSpriteFrames ( szUpSpriteFrame, szDownSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool GameButton::initWithSpriteFrames ( const KDchar* szUpSpriteFrame, const KDchar* szDownSpriteFrame )
{
	if ( !TouchableSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_sUpSpriteFrame	= szUpSpriteFrame;
	m_sDownSpriteFrame  = szDownSpriteFrame;

	// Set the sprite display frame
	this->setDisplayFrameName ( szUpSpriteFrame );

	this->setTouchColor ( Color3B ( 255, 200, 200 ) );

	return KD_TRUE;
}

KDvoid GameButton::processTouch ( const Point& tPoint )
{
	TouchableSprite::processTouch ( tPoint );

	this->setDisplayFrameName ( m_sDownSpriteFrame.c_str ( ) );
}

KDvoid GameButton::processRelease ( KDvoid )
{
	TouchableSprite::processRelease ( );

	this->setDisplayFrameName ( m_sUpSpriteFrame.c_str ( ) );
}

const KDchar* GameButton::getUpSpriteFrame ( KDvoid )
{
	return m_sUpSpriteFrame.c_str ( );
}

KDvoid GameButton::setUpSpriteFrame ( const KDchar* szSpriteFrame )
{
	m_sUpSpriteFrame = szSpriteFrame;
}
 
const KDchar* GameButton::getDownSpriteFrame ( KDvoid )
{
	return m_sDownSpriteFrame.c_str ( );
}

KDvoid GameButton::setDownSpriteFrame ( const KDchar* szSpriteFrame )
{
	m_sDownSpriteFrame = szSpriteFrame;
}

const KDchar* GameButton::getName ( KDvoid )
{
	return m_sName.c_str ( );
}

KDvoid GameButton::setName ( const KDchar* szName )
{
	m_sName = szName;
}

NS_CC_EXT_END