/* --------------------------------------------------------------------------
 *
 *      File            Ch7_LineOfSight.cpp
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
#include "Ch7_LineOfSight.h"
#include "Libraries/RayCast.h"

KDbool Ch7_LineOfSight::init ( KDvoid )
{	
	// Set game area size
	this->setGameAreaSize ( );

	// Superclass initalization and message
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}
	
	m_pMessage->setString ( "Use the DPad to avoid the AI actors." );

	// Add DPad
	this->addDPad ( );

	// Add random geometry
	this->addRandomPolygons ( 5 );

	// Create Actor
	this->addActor ( );
	
	// Create enemies
	this->addEnemies ( );
	
	//Add draw layer
	this->addDrawLayer ( );
	
	//Create level boundaries
	this->addLevelBoundaries ( );
	
	//Initial variables
	m_fCameraZoom = 0.45f;
	
	return KD_TRUE;
}

KDvoid Ch7_LineOfSight::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pEnemies );

	GameArea2D::onExit ( );
}

KDvoid Ch7_LineOfSight::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

	// Process DPad
	b2Body*		pBody = m_pActor->getBody ( );

	if ( m_pDPad->isPressed ( ) )
	{		
		pBody->ApplyForce ( b2Vec2 ( m_pDPad->getPressedVector ( ).x * 25.0f, m_pDPad->getPressedVector ( ).y * 25.0f ), pBody->GetPosition ( ) );
		pBody->SetAngularVelocity ( 0.0f );

		CCPoint		tMovementVector = ccp ( pBody->GetLinearVelocity ( ).x, pBody->GetLinearVelocity  ().y );
		pBody->SetTransform ( pBody->GetPosition ( ), -1 * GameHelper::vectorToRadians ( tMovementVector ) + PI_CONSTANT / 2 );
	}
	else
	{
		m_pActor->stopRunning ( );
	}

	// Follow the actor with the camera
	CCPoint		tActorPosition = ccp ( pBody->GetPosition().x * PTM_RATIO, pBody->GetPosition ( ).y * PTM_RATIO );
	this->centerCameraOnGameCoord ( tActorPosition );
	
	// Make the enemies follow the actor
	this->followActorWithEnemies ( );
}

/// DPad input 
KDbool Ch7_LineOfSight::hudBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesBegan ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

KDbool Ch7_LineOfSight::hudMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesMoved ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

KDbool Ch7_LineOfSight::hudEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesEnded ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

KDvoid Ch7_LineOfSight::drawLayer ( KDvoid )
{
	ccDrawColor4B ( 255, 255, 0, 128 );	

	b2Vec2		tActorPos = m_pActor->getBody ( )->GetPosition ( );
	for ( KDuint i = 0; i < m_pEnemies->count ( ); i++ )
	{
		GameActor*		pEnemy = (GameActor*) m_pEnemies->objectAtIndex ( i );
		b2Vec2			tEnemyPos = pEnemy->getBody ( )->GetPosition ( );
	
		if ( pEnemy->getTag ( ) == 1 )
		{							
			CCPoint		tActorPosition = ccp ( tActorPos.x * PTM_RATIO, tActorPos.y * PTM_RATIO );
			CCPoint		tEnemyPosition = ccp ( tEnemyPos.x * PTM_RATIO, tEnemyPos.y * PTM_RATIO );
			
			ccDrawLine ( tActorPosition, tEnemyPosition );
		}
	}
}

// Set game area size
KDvoid Ch7_LineOfSight::setGameAreaSize ( KDvoid )
{
	m_tGameAreaSize = ccs ( 1000 / PTM_RATIO, 600 / PTM_RATIO );	// Box2d units
}

KDvoid Ch7_LineOfSight::followActorWithEnemies ( KDvoid )
{
	b2Vec2		tActorPos = m_pActor->getBody ( )->GetPosition ( );
	// If enemies can see the actor they follow
	for ( KDuint i = 0; i < m_pEnemies->count ( ); i++ )
	{
		// Align enemies
		GameActor*		pEnemy = (GameActor*) m_pEnemies->objectAtIndex ( i );
		b2Body*			pEnemyBody = pEnemy->getBody ( );
		CCPoint			tDirectionVector = ccp ( tActorPos.x - pEnemyBody->GetPosition ( ).x, tActorPos.y - pEnemyBody->GetPosition ( ).y );
		KDfloat			fRadians = GameHelper::vectorToRadians ( tDirectionVector );
		pEnemyBody->SetTransform ( pEnemyBody->GetPosition ( ), -1 * fRadians + PI_CONSTANT / 2 );
		
		RayCastClosestCallback		tCallback;
		m_pWorld->RayCast ( &tCallback, pEnemyBody->GetPosition ( ), tActorPos );
				
		// Did the raycast hit anything?
		pEnemy->setTag ( 0 );				// Assume we can't
		if ( tCallback.m_bHit )
		{
			// Is the closest point the actor?
			if ( tCallback.m_pFixture->GetBody ( ) == m_pActor->getBody ( ) )
			{
				// If so, follow the actor
				b2Vec2		tNormal = b2Vec2 ( tCallback.m_tNormal.x * -5.0f, tCallback.m_tNormal.y * -5.0f );
				pEnemyBody->ApplyForce ( tNormal, tActorPos );
				pEnemy->setTag ( 1 );		// Set seeing flag to true
			}
		}
	}
}

/// Add DPad 
KDvoid Ch7_LineOfSight::addDPad ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "dpad_buttons.plist" );
	
	m_pDPad = DPad::create ( );
	m_pDPad->setPosition ( ccp ( 75, 75 ) );
	m_pDPad->setOpacity ( 100 );
	this->addChild ( m_pDPad, 5 ); 
}

/// Add actor 
KDvoid Ch7_LineOfSight::addActor ( KDvoid )
{
	m_pActor = GameActor::create ( );
	m_pActor->setGameArea ( this );
	m_pActor->setTag ( GO_TAG_ACTOR );
	
	b2BodyDef*		pBodyDef = m_pActor->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( m_tGameAreaSize.cx / 2, m_tGameAreaSize.cy / 2 );
	pBodyDef->userData = m_pActor;
		
	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pActor->setBody ( pBody );
	
	b2CircleShape*	pShape = new b2CircleShape ( );
	m_pActor->setCircleShape ( pShape );
	pShape->m_radius = 0.5f;

	b2FixtureDef*	pFixtureDef = m_pActor->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	
	pBody->CreateFixture ( pFixtureDef );

	pBody->SetLinearDamping ( 2.0f );
}

/// Add 3 enemies 
KDvoid Ch7_LineOfSight::addEnemies ( KDvoid )
{		
	m_pEnemies = new CCArray ( );

	for ( KDint i = 0; i < 3; i++ )
	{
		GameActor*		pEnemy = GameActor::create ( );
		pEnemy->setGameArea ( this );
		pEnemy->setTag ( 0 );								// We use this to store whether or not we can see the actor

		b2BodyDef*		pBodyDef = pEnemy->getBodyDef ( );
		pBodyDef->type = b2_dynamicBody;
		pBodyDef->position.Set ( ( kdRand ( ) % ( (KDint) ( m_tGameAreaSize.cx - 2 ) ) ) + 1.0f, ( kdRand ( ) % ( (KDint) ( m_tGameAreaSize.cy - 2 ) ) ) + 1.0f );
		pBodyDef->userData = pEnemy;
		
		b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
		pEnemy->setBody ( pBody );
						
		b2CircleShape*	pShape = new b2CircleShape ( );
		pEnemy->setCircleShape ( pShape );
		pShape->m_radius = 0.5f;

		b2FixtureDef*	pFixtureDef = pEnemy->getFixtureDef ( );
		pFixtureDef->shape = pShape;
		
		pBody->CreateFixture ( pFixtureDef );

		m_pEnemies->addObject ( pEnemy );
	}
}

/// Add draw layer 
KDvoid Ch7_LineOfSight::addDrawLayer ( KDvoid )
{
	m_pDrawLayer = DrawLayer::create ( );
	m_pDrawLayer->setPosition ( ccp ( 0, 0 ) );
	m_pDrawLayer->setRecipe ( this );
	m_pGameNode->addChild ( m_pDrawLayer, 10 );
}