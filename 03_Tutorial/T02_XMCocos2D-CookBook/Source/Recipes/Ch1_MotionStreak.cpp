/* --------------------------------------------------------------------------
 *
 *      File            Ch1_MotionStreak.cpp
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
#include "Ch1_MotionStreak.h"

KDbool Ch1_MotionStreak::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&	tLyrSize = this->getContentSize ( );

	// Set the rocket initially in a random direction.
	m_tRocketDirection = ccp ( kdRand ( ) % 4 + 1, kdRand ( ) % 4 + 1 );
		
	// Add the rocket sprite.
	m_pRocket = CCSprite::create ( "rocket.png" );
	m_pRocket->setPosition ( ccpMid ( tLyrSize ) );
	m_pRocket->setScale ( 0.5f );
	this->addChild ( m_pRocket );
		
	// Create the streak object and add it to the scene.
	m_pStreak = CCMotionStreak::create ( 1, 1, 32, ccc3 ( 255, 255, 255 ), "streak.png" );
	m_pStreak->setPosition ( ccpMid ( tLyrSize ) );
	this->addChild ( m_pStreak );	
	
	this->schedule ( schedule_selector ( Ch1_MotionStreak::step ) );

	return KD_TRUE;
}

KDvoid Ch1_MotionStreak::step ( KDfloat fDelta )
{
	const CCSize&	tLyrSize = this->getContentSize ( );
	CCPoint			tRocketPos = m_pRocket->getPosition ( );

	// Make rocket bounce off walls
	if ( tRocketPos.x > tLyrSize.cx || tRocketPos.x < 0 )
	{
		m_tRocketDirection = ccp ( -m_tRocketDirection.x, m_tRocketDirection.y );
	}
	else if ( tRocketPos.y > tLyrSize.cy || tRocketPos.y < 0 )
	{
		m_tRocketDirection = ccp ( m_tRocketDirection.x, -m_tRocketDirection.y );
	}

	// Slowly turn the rocket
	m_tRocketDirection = ccp ( m_tRocketDirection.x, m_tRocketDirection.y + 0.05f );

	// Update rocket position based on direction
	tRocketPos = ccpAdd ( tRocketPos, m_tRocketDirection );
	m_pRocket->setPosition ( tRocketPos );
	m_pStreak->setPosition ( tRocketPos );

	// Set the rocket's rotation
	m_pRocket->setRotation ( radiansToDegrees ( vectorToRadians ( m_tRocketDirection ) ) );
}