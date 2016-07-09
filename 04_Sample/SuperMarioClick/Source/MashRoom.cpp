/* --------------------------------------------------------------------------
 *
 *      File            MashRoom.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Liu weimu on 5/21/13
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
#include "MashRoom.h"

//#import "SuperMarioClick.h"

static MashRoom*		l_pMashRoomInstance;

MashRoom* MashRoom::create ( b2World* pWorld )
{
	MashRoom*	pRet = new MashRoom ( );

	if ( pRet && pRet->initWithWorld ( pWorld ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

MashRoom* MashRoom::sharedMashRoom ( KDvoid )
{
	return l_pMashRoomInstance;
}

KDbool MashRoom::initWithWorld ( b2World* pWorld )
{
	if ( !BodyNode::init ( ) )
	{
		return KD_FALSE;
	}

	l_pMashRoomInstance = this;       

	CCSize		tScreenSize = CCDirector::sharedDirector ( )->getWinSize ( );
	CCPoint		tPlungerPos = ccp ( tScreenSize.cx - 13, 32 );

	// Create a body definition
	b2BodyDef	tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position = Helper::toMeters ( tPlungerPos );

	b2CircleShape	tShape;
	KDfloat			fRadiusInMeters = ( 50 / PTM_RATIO ) * 0.5f;
	tShape.m_radius = fRadiusInMeters;

	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tShape;
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 0.99f;
	tFixtureDef.restitution = 0.01f;

	BodyNode::createBodyInWorld ( pWorld, &tBodyDef, &tFixtureDef, "Mushroom_32.png" );

	m_pSprite->setPosition ( tPlungerPos );

	this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid MashRoom::setBody ( b2Body* pBody )
{
	m_pSharedMashRoomBody = pBody;
}

KDvoid MashRoom::setFixture ( b2Fixture* pFixture )
{
	m_pShareMashRoomFixture = pFixture;
}

b2Body* MashRoom::sharedMashRoomBody ( KDvoid )
{
	CCAssert ( l_pMashRoomInstance != KD_NULL, "table not yet initialized!" );

	return m_pSharedMashRoomBody;
}

b2Fixture* MashRoom::shareMashRoomFixture ( KDvoid )
{
	return m_pShareMashRoomFixture;
}

KDvoid MashRoom::setDoPlunge ( KDbool bYes )
{
	m_bDoPlunge = bYes;
}
