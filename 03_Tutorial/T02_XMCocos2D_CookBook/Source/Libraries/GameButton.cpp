/* --------------------------------------------------------------------------
 *
 *      File            GameButton.cpp
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
#include "GameButton.h"

GameButton* GameButton::create ( KDvoid )
{
	GameButton*		pRet = new GameButton ( );

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

KDvoid GameButton::processTouch ( const CCPoint& tPoint )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	this->setDisplayFrame ( pCache->spriteFrameByName ( m_sDownSpriteFrame.c_str ( ) ) );
	m_bPressed = KD_TRUE;
	this->setColor ( ccc3 ( 255, 200, 200 ) );
}

KDvoid GameButton::processRelease ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	this->setDisplayFrame ( pCache->spriteFrameByName ( m_sUpSpriteFrame.c_str ( ) ) );	
	m_bPressed = KD_FALSE;
	this->setColor ( ccc3 ( 255, 255, 255 ) );
}

const KDchar* GameButton::getUpSpriteFrame ( KDvoid )
{
	return m_sUpSpriteFrame.c_str ( );
}

KDvoid GameButton::setUpSpriteFrame ( const KDchar* szFramename )
{
	m_sUpSpriteFrame = szFramename;
}
 
const KDchar* GameButton::getDownSpriteFrame ( KDvoid )
{
	return m_sDownSpriteFrame.c_str ( );
}

KDvoid GameButton::setDownSpriteFrame ( const KDchar* szFramename )
{
	m_sDownSpriteFrame = szFramename;
}

const KDchar* GameButton::getName ( KDvoid )
{
	return m_sName.c_str ( );
}

KDvoid GameButton::setName ( const KDchar* szName )
{
	m_sName = szName;
}
