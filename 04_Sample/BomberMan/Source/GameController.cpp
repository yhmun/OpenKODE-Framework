/* --------------------------------------------------------------------------
 *
 *      File            GameController.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "GameController.h"
#include "GameHero.h"

KDbool GameController::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	this->setTouchEnabled ( KD_TRUE );

	CCSprite*	pKey = CCSprite::create ( "key.png" );
	this->addChild ( pKey );
	pKey->setPosition ( ccp ( 1.5 * TileW, 1.5 * TileH ) );

	pKey = CCSprite::create ( "key.png" );
	this->addChild ( pKey );
	pKey->setPosition ( ccp ( 1.5 * TileW, 0.5 * TileH ) );
	pKey->setRotation ( 180 );

	pKey = CCSprite::create ( "key.png" );
	this->addChild ( pKey );
	pKey->setPosition ( ccp ( 0.5 * TileW, 0.5 * TileH ) );
	pKey->setRotation ( -90 );

	pKey = CCSprite::create ( "key.png" );
	this->addChild ( pKey );
	pKey->setPosition ( ccp ( 2.5 * TileW, 0.5 * TileH ) );
	pKey->setRotation ( 90 );

	CCSprite*	pBomb = CCSprite::create ( "bomb.png" );
	this->addChild ( pBomb );
	pBomb->setPosition ( ccp ( 14.5 * TileW, 0.5 * TileH ) );

	m_pFingerSprite = KD_NULL;

	return KD_TRUE;
}

KDvoid GameController::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( m_pFingerSprite ) 
    {
        this->removeChild ( m_pFingerSprite, KD_TRUE ); 
    }

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );

	m_tTouchPos = this->convertToTilePos ( tLocation );
  
    if ( m_tTouchPos.x == 0.5f * TileW && m_tTouchPos.y == 0.5f * TileH )
    {
		m_pHero->setAct ( kLeft );
    }
    else if ( m_tTouchPos.x == 2.5f * TileW && m_tTouchPos.y == 0.5f * TileH )
    {
		m_pHero->setAct ( kRight );
    }
    else if ( m_tTouchPos.x == 1.5f * TileW && m_tTouchPos.y == 0.5f * TileH )
    {
		m_pHero->setAct ( kDown );
    }
    else if ( m_tTouchPos.x == 1.5f * TileW && m_tTouchPos.y == 1.5f * TileH )
    {
		m_pHero->setAct ( kUp );
    }
    else if ( m_tTouchPos.x == 14.5f * TileW && m_tTouchPos.y == 0.5f * TileH )
    {
		m_pHero->setAct ( kFire );
    }
}

KDvoid GameController::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pHero->setAct ( kStay );
}

KDvoid GameController::setHero ( GameHero* pHero )
{
	m_pHero = pHero;
}

CCPoint GameController::convertToTilePos ( const CCPoint& tPos )
{
	KDint	x = (KDint) ( tPos.x / TileW );
	KDint	y = (KDint) ( tPos.y / TileH );
	return ccp ( ( x + 0.5f ) * TileW, ( y + 0.5f ) * TileH );
}
