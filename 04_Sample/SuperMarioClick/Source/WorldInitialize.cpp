/* --------------------------------------------------------------------------
 *
 *      File            WorldInitialize.cpp
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
#include "WorldInitialize.h"
#include "MashRoom.h"
#include "Goomba.h"
#include "Ball.h"

WorldInitialize* WorldInitialize::create ( b2World* pWorld )
{
	WorldInitialize*	pRet = new WorldInitialize ( );

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

KDbool WorldInitialize::initWithWorld ( b2World* pWorld )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	// weak reference to world for convenience
	m_pWorld = pWorld;
	
//	CCSpriteBatchNode*	pBatch = CCSpriteBatchNode::create ( "table.png" );
//	this->addChild ( pBatch );

	this->addGoombaAt ( ccp ( 150, 330 ) );
	this->addGoombaAt ( ccp ( 100, 390 ) );
	this->addGoombaAt ( ccp ( 230, 380 ) );
	this->addGoombaAt ( ccp (  40, 350 ) );
           
	MashRoom*	pMashRoom = MashRoom::create ( pWorld );
	this->addChild ( pMashRoom, -2 );

	Ball*		pBall = Ball::create ( pWorld );
	this->addChild ( pBall, -1 );

	return KD_TRUE;
}

KDvoid WorldInitialize::addGoombaAt ( const CCPoint& tPos )
{
	Goomba*		pGoomba = Goomba::create ( m_pWorld, tPos );
	this->addChild ( pGoomba );
}

