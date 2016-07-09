/* --------------------------------------------------------------------------
 *
 *      File            ShipSprite.cpp
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
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
#include "ShipSprite.h"
#include "GameLayer.h"

#define Y_VELOCITY_UP         2
#define Y_VELOCITY_DOWN      -2
#define Y_VELOCITY_DOWN_MAX  -5
#define Y_ACCELORATE_DOWN    -0.05
#define Y_ACCELORATE_UP       0.04
#define STREAK_TAG           99


ShipSprite* ShipSprite::create ( GameLayer* pGameLayer )
{
	ShipSprite*	pRet = new ShipSprite ( );

	if ( pRet && pRet->init ( pGameLayer ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}


KDbool ShipSprite::init ( GameLayer* pGameLayer )
{
    if ( !CCSprite::initWithFile ( "Images/ship.png" ) )
    {
        return KD_FALSE;
    }
	m_pGameLayer = pGameLayer;
    
    this->addFire ( );
    this->reset ( );
    this->scheduleUpdate ( );
    
	return KD_TRUE;
}


KDvoid ShipSprite::addFire ( KDvoid )
{
    CCParticleSystemQuad* pSmoke = CCParticleSystemQuad::create ( "Images/fire.plist" );
    pSmoke->setScale      ( 0.4 );
    pSmoke->setZOrder     ( -1 );
    pSmoke->setPosition   ( ccp ( 10, 10 ) );
    this->addChild        ( pSmoke );
}


KDvoid ShipSprite::reset ( KDvoid )
{
    this->setPosition ( ccp ( this->getContentSize ( ).cx / 2 + 100, m_pGameLayer->getScreenSize ( ).width / 2 ) );
    m_tAccelorate     = ccp ( 0, Y_ACCELORATE_DOWN);
    m_tVelocity       = ccp ( 0, 0);
}


KDvoid ShipSprite::update ( KDfloat fDelta )
{
    this->setPosition ( ccpAdd ( this->getPosition ( ), this->m_tVelocity ) );
    m_tVelocity       = ccpAdd ( m_tVelocity, m_tAccelorate );
}


KDvoid ShipSprite::up ( KDvoid )
{
    m_tAccelorate = ccp ( 0, Y_ACCELORATE_UP );
    m_tVelocity   = ccp ( 0, Y_VELOCITY_UP );
}


KDvoid ShipSprite::down ( KDvoid )
{
    m_tAccelorate = ccp ( 0, Y_ACCELORATE_DOWN );
    m_tVelocity   = ccp ( 0, Y_VELOCITY_DOWN );
}


KDvoid ShipSprite::crash ( KDvoid )
{
    //Game over crash animation
}


