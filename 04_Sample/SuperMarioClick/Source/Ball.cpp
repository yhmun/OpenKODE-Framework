/* --------------------------------------------------------------------------
 *
 *      File            Ball.cpp
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
#include "Ball.h"
#include "SuperMarioClick.h"

static Ball*		l_pBallInstance;

Ball* Ball::create ( b2World* pWorld )
{
	Ball*	pRet = new Ball ( );

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

Ball* Ball::sharedBall ( KDvoid )
{
	return l_pBallInstance;
}

KDbool Ball::initWithWorld ( b2World* pWorld )
{
	if ( !BodyNode::init ( ) )
	{
		return KD_FALSE;
	}

	l_pBallInstance = this;
	m_bMoveToFinger = KD_FALSE;

	this->createBallInWorld ( pWorld );

	this->scheduleUpdate ( );
		
	return KD_TRUE;
}

KDvoid Ball::update ( KDfloat fDelta )
{
	if ( m_bMoveToFinger == KD_TRUE )
	{
		// disabled: not needed anymore		
	}
	
	if ( m_pSprite->getPosition ( ).y <= m_pSprite->getContentSize ( ).cy )
	{
		// create a new ball and remove the old one
		SuperMarioClick::sharedSuperMario ( )->setContactCount ( 0 );
	}
}

KDvoid Ball::createBallInWorld ( b2World* pWorld )
{
	CCSize		tScreenSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDfloat		fRandomOffset = CCRANDOM_0_1 ( ) * 10.0f - 5.0f;
	CCPoint		tStartPos = ccp ( tScreenSize.cx / 2 + fRandomOffset, 400 );

	// Create a body definition and set it to be a dynamic body
	b2BodyDef	tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position = Helper::toMeters ( tStartPos );
	tBodyDef.angularDamping = 0.9f;

	const KDchar*	szSpriteFrameName = "magic_ball.png";
	CCSprite*		pTempSprite = CCSprite::createWithSpriteFrameName ( szSpriteFrameName );

	b2CircleShape	tCircleShape;
	KDfloat			fRadiusInMeters = ( pTempSprite->getContentSize ( ).cx / PTM_RATIO ) * 0.5f;
	tCircleShape.m_radius = fRadiusInMeters;

	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tCircleShape;
	tFixtureDef.density = 0.8f;
	tFixtureDef.friction = 0.7f;
	tFixtureDef.restitution = 0.8f;

	BodyNode::createBodyInWorld ( pWorld, &tBodyDef, &tFixtureDef, szSpriteFrameName );
	m_pSprite->setColor ( ccRED );
}

KDvoid Ball::gameReset ( KDvoid )
{
	this->createBallInWorld ( m_pBody->GetWorld ( ) );
}
