/* --------------------------------------------------------------------------
 *
 *      File            Ch4_CollisionResponse.cpp
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
#include "Ch4_CollisionResponse.h"
#include "Libraries/GameSensor.h"
#include "Libraries/GameMisc.h"

KDbool Ch4_CollisionResponse::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Collision response and sensors." );
	
	// Show debug drawing so we can see the sensor
	this->showDebugDraw ( );

	// Create Sensor
	GameSensor*		pGameObjectSensor = GameSensor::create ( );
	pGameObjectSensor->setGameArea ( this );
	
	b2BodyDef*		pBodyDef = pGameObjectSensor->getBodyDef ( );
	pBodyDef->type = b2_staticBody;
	pBodyDef->position.Set ( 240 / PTM_RATIO, 160 / PTM_RATIO );
	pBodyDef->userData = pGameObjectSensor;
				
	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pGameObjectSensor->setBody ( pBody );
	
	b2CircleShape*	pShape = new b2CircleShape ( );
	pGameObjectSensor->setCircleShape ( pShape );
	pShape->m_radius = 1.0f;

	b2FixtureDef*	pFixtureDef = pGameObjectSensor->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->isSensor = KD_TRUE;
	
	pBody->CreateFixture ( pFixtureDef );

	// Create level boundaries
	this->addLevelBoundaries ( );

	// Add block batch sprite
	CCSpriteBatchNode*		pBatch = CCSpriteBatchNode::create ( "blocks.png", 150 );
	this->addChild ( pBatch, 0, 10 );

	return KD_TRUE;
}

KDvoid Ch4_CollisionResponse::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );
		CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );
	
		this->addNewObjectWithCoords ( tLocation );
	}
}

/// Our base collision handling routine 
KDvoid Ch4_CollisionResponse::handleCollisionWithObjA ( GameObject* pObjectA, GameObject* pObjectB )
{
	KDint	nTypeA = pObjectA->getType ( );
	KDint	nTypeB = pObjectB->getType ( );

	// SENSOR to MISC collision
	if ( nTypeA == GO_TYPE_SENSOR && nTypeB == GO_TYPE_MISC )
	{
		this->handleCollisionWithSensor ( (GameSensor*) pObjectA, (GameMisc*) pObjectB );
	}
	else if ( nTypeA == GO_TYPE_MISC && nTypeB == GO_TYPE_SENSOR )
	{
		this->handleCollisionWithSensor ( (GameSensor*) pObjectB, (GameMisc*) pObjectA );
	}
	// MISC to MISC collision
	else if ( nTypeA == GO_TYPE_MISC && nTypeB == GO_TYPE_MISC )
	{
		this->handleCollisionWithMisc ( (GameMisc*) pObjectA, (GameMisc*) pObjectB );
	}
}

/// Handling collision between specific types of objects 
KDvoid Ch4_CollisionResponse::handleCollisionWithSensor ( GameSensor* pSensor, GameMisc* pMisc )
{
	m_pMessage->setString ( "Box collided with sensor" );
	
	this->runAction 
	( 
		CCSequence::create
		(
			CCDelayTime::create ( 0.5f ),
			CCCallFunc::create ( this, callfunc_selector ( Ch4_CollisionResponse::resetMessage ) ),
			KD_NULL 
		)
	);
}

KDvoid Ch4_CollisionResponse::handleCollisionWithMisc ( GameMisc* pMiscA, GameMisc* pMiscB )
{
	m_pMessage->setString ( "Box collided with another box" );
	
	this->runAction 
	( 
		CCSequence::create
		(
			CCDelayTime::create ( 0.5f ),
			CCCallFunc::create ( this, callfunc_selector ( Ch4_CollisionResponse::resetMessage ) ),
			KD_NULL 
		)
	);
}

/// Adding a new block 
KDvoid Ch4_CollisionResponse::addNewObjectWithCoords ( const CCPoint& tPoint )
{
	CCSpriteBatchNode*	pBatch = (CCSpriteBatchNode*) this->getChildByTag ( 10 );

	GameMisc*			pObject = GameMisc::create ( );
	pObject->setGameArea ( this );

	KDint				nX = ( CCRANDOM_0_1 ( ) > 0.5f ? 0 : 1 );
	KDint				nY = ( CCRANDOM_0_1 ( ) > 0.5f ? 0 : 1 );

	CCSprite*			pSprite = CCSprite::createWithSpriteBatchNode ( pBatch, ccr ( 32 * nX, 32 * nY, 32, 32 ) );
	pSprite->setPosition ( tPoint );
	pObject->setSprite ( pSprite );
	pBatch->addChild ( pSprite );
	
	b2BodyDef*			pBodyDef = pObject->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	pBodyDef->userData = pObject;

	b2Body*				pBody = m_pWorld->CreateBody ( pObject->getBodyDef ( ) );
	pObject->setBody ( pBody );
		
	// Define another box shape for our dynamic body.
	b2PolygonShape*		pShape = new b2PolygonShape ( );
	pObject->setPolygonShape ( pShape );
	pShape->SetAsBox ( 0.5f, 0.5f );		// These are mid points for our 1m box

	b2FixtureDef*	pFixtureDef = pObject->getFixtureDef ( );
	pFixtureDef->shape = pShape;	
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = 0.3f;
	
	pBody->CreateFixture ( pFixtureDef );
}