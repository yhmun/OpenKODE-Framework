/* --------------------------------------------------------------------------
 *
 *      File            Goomba.cpp
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
#include "Goomba.h"

Goomba* Goomba::create ( b2World* pWorld, const CCPoint& tPos )
{
	Goomba*	pRet = new Goomba ( );

	if ( pRet && pRet->initWithWorld ( pWorld, tPos ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool Goomba::initWithWorld ( b2World* pWorld, const CCPoint& tPos )
{
	if ( !BodyNode::init ( ) )
	{
		return KD_FALSE;
	}

	b2BodyDef		tBodyDef;
	tBodyDef.position = Helper::toMeters ( tPos );
	
	const KDchar*	szSpriteFrameName = "Goomba_32.png";
	CCSprite*		pTempSprite = CCSprite::createWithSpriteFrameName ( szSpriteFrameName ); 

	b2CircleShape	tCircleShape;
	KDfloat			fRadiusInMeters = ( pTempSprite->getContentSize ( ).cx / PTM_RATIO ) * 0.5f;
	tCircleShape.m_radius = fRadiusInMeters;

	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tCircleShape;
	tFixtureDef.density = 0.0f;
	tFixtureDef.friction = 0.8f;
	tFixtureDef.userData = pTempSprite;
	// restitution > 1 makes objects bounce off faster than they hit
	tFixtureDef.restitution = 1.5f;

	BodyNode::createBodyInWorld ( pWorld, &tBodyDef, &tFixtureDef, szSpriteFrameName );
	m_pSprite->setColor ( ccORANGE );

	return KD_TRUE;
}

