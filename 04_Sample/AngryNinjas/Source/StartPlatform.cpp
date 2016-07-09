/* --------------------------------------------------------------------------
 *
 *      File            StartPlatform.cpp    
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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
#include "StartPlatform.h"

StartPlatform* StartPlatform::create ( b2World* pWorld, const CCPoint& tLocation, const KDchar* szSpriteFileName )
{
	StartPlatform*	pRet = new StartPlatform ( );

	if ( pRet && pRet->InitWithWorld ( pWorld, tLocation, szSpriteFileName ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool StartPlatform::InitWithWorld ( b2World* pWorld, const CCPoint& tLocation, const KDchar* szSpriteFileName )
{
	if ( !BodyNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_pTheWorld			= pWorld;
	m_sSpriteImageName	= szSpriteFileName;
	m_tInitialLocation	= tLocation;

	this->createPlatform ( );

	return KD_TRUE;
}

KDvoid StartPlatform::createPlatform ( KDvoid )
{
	// Define the dynamic body.
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_staticBody;					// or you could use b2_staticBody
	tBodyDef.position.Set ( m_tInitialLocation.x / PTM_RATIO, m_tInitialLocation.y / PTM_RATIO );

	b2PolygonShape*	pShape = new b2PolygonShape ( );
	
	KDint			nNum = 4;
	b2Vec2			aVertices [ ] =
	{
		b2Vec2 ( -102.0f / PTM_RATIO,  49.0f / PTM_RATIO ),
		b2Vec2 ( -113.0f / PTM_RATIO, -81.0f / PTM_RATIO ),
		b2Vec2 (  113.0f / PTM_RATIO, -84.0f / PTM_RATIO ),
		b2Vec2 (  106.0f / PTM_RATIO,  47.0f / PTM_RATIO )		
	};

	pShape->Set ( aVertices, nNum );

	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = pShape;	
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 0.3f;
	tFixtureDef.restitution =  0.1f;

	this->createBodyWithSpriteAndFixture ( m_pTheWorld, &tBodyDef, &tFixtureDef, m_sSpriteImageName.c_str ( ) );

	// CONTINUING TO ADD BODY SHAPE....
	// THIS IS THE Sling base....

	// row 1, col 1
	KDint			nNum2 = 4;
	b2Vec2			aVertices2 [ ] =
	{
		b2Vec2 ( -41.0f / PTM_RATIO,  -6.5f / PTM_RATIO ),
		b2Vec2 ( -35.0f / PTM_RATIO, -57.5f / PTM_RATIO ),
		b2Vec2 (  57.0f / PTM_RATIO, -65.5f / PTM_RATIO ),
		b2Vec2 (  49.0f / PTM_RATIO,   7.5f / PTM_RATIO )		
	};

	pShape->Set ( aVertices2, nNum2 );
	tFixtureDef.shape = pShape;
	m_pBody->CreateFixture ( &tFixtureDef );
}
