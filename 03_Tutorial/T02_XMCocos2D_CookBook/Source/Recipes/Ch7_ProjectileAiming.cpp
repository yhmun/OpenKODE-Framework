/* --------------------------------------------------------------------------
 *
 *      File            Ch7_ProjectileAiming.cpp
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
#include "Ch7_ProjectileAiming.h"
#include "Libraries/GameMisc.h"

enum 
{
	GO_TAG_ENEMY	= 2,
	GO_TAG_MISSILE	= 3,
};

enum 
{
	CB_EVERYTHING	= 0xFFFF,
	CB_WALL			= 0x0001,
	CB_ACTOR		= 0x0002,
	CB_ENEMY		= 0x0004,
	CB_MISSILE		= 0x0008,
};

KDbool Ch7_ProjectileAiming::init ( KDvoid )
{	
	// Set game area size
	m_tGameAreaSize = ccp ( 1000 / PTM_RATIO, 600 / PTM_RATIO );	// Box2d units

	// Superclass initialization and message
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}
	m_pMessage->setString ( "Use the DPad to dodge the projectiles." );

	// Add DPad
	this->addDPad ( );

	// Add some geometry
	this->addRandomPolygons ( 5 );

	// Create Actor
	this->addActor ( );
		
	// Create enemies
	this->addEnemies ( );
	
	// Create level boundaries
	this->addLevelBoundaries ( );
	
	// Initial variables
	m_fCameraZoom	= 0.45f;
	m_fFireCount	= 0;
	m_pMissiles		= new CCArray ( );

	return KD_TRUE;
}

KDvoid Ch7_ProjectileAiming::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pEnemies );
	CC_SAFE_DELETE ( m_pMissiles );

	GameArea2D::onExit ( );
}

KDvoid Ch7_ProjectileAiming::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

	b2Body*		pBody = m_pActor->getBody ( );

	// Process DPad
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

	// Align enemies
	for ( KDuint i = 0; i < m_pEnemies->count ( ); i++ )
	{
		GameActor*		pEnemy = (GameActor*) m_pEnemies->objectAtIndex ( i );
		b2Body*			pEnemyBody = pEnemy->getBody ( );
		CCPoint			tDirectionVector = ccp ( pBody->GetPosition ( ).x - pEnemyBody->GetPosition ( ).x, pBody->GetPosition ( ).y - pEnemyBody->GetPosition ( ).y );
		KDfloat			fRadians = GameHelper::vectorToRadians ( tDirectionVector );
		pEnemyBody->SetTransform ( pEnemyBody->GetPosition ( ), -1 * fRadians + PI_CONSTANT / 2 );
	}

	// Firing projectiles
	m_fFireCount += fDelta;
	if ( m_fFireCount > 1.0f )
	{ 
		m_fFireCount = 0;
		this->fireMissiles ( );
	}
}

/// DPad input 
KDbool Ch7_ProjectileAiming::hudBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesBegan ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

KDbool Ch7_ProjectileAiming::hudMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesMoved ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

KDbool Ch7_ProjectileAiming::hudEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesEnded ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

/// Our base collision handling routine 
KDvoid Ch7_ProjectileAiming::handleCollisionWithObjA ( GameObject* pObjectA, GameObject* pObjectB )
{
	// General collision handling
	if ( pObjectA->getTag ( ) == GO_TAG_MISSILE )
	{
		this->markBodyForDestruction ( pObjectA );
	}
	else if ( pObjectB->getTag ( ) == GO_TAG_MISSILE )
	{
		this->markBodyForDestruction ( pObjectB );
	}
}

/// Find the intercept angle given projectile speed and a moving target 
Vector3D* Ch7_ProjectileAiming::interceptSrc ( GameObject* pSrc, GameObject* pDst, KDfloat fProjSpeed )
{
	b2Body*		pSrcBody = pSrc->getBody ( );
	b2Body*		pDstBody = pDst->getBody ( );

	KDfloat		tx  = pDstBody->GetPosition ( ).x - pSrcBody->GetPosition ( ).x;
	KDfloat		ty  = pDstBody->GetPosition ( ).y - pSrcBody->GetPosition ( ).y;
	KDfloat		tvx = pDstBody->GetLinearVelocity ( ).x;
	KDfloat		tvy = pDstBody->GetLinearVelocity ( ).y;

	// Get quadratic equation components
	KDfloat		a = tvx * tvx + tvy * tvy - fProjSpeed * fProjSpeed;
	KDfloat		b = 2 * ( tvx * tx + tvy * ty );
	KDfloat		c = tx * tx + ty * ty;

	// Solve quadratic equation
	Vector3D*	ts = GameHelper::quadraticA ( a, b, c );
	
	// Find the smallest positive solution
	Vector3D*	pSolution = KD_NULL;
	if ( ts )
	{
		KDfloat		t0 = ts->x;
		KDfloat		t1 = ts->y;
		KDfloat		t = KD_MIN ( t0, t1 );

		if ( t < 0 ) 
		{
			t = KD_MAX ( t0, t1 ); 
		}

		if ( t > 0 ) 
		{
			KDfloat		x = pDstBody->GetPosition ( ).x + pDstBody->GetLinearVelocity ( ).x * t;
			KDfloat		y = pDstBody->GetPosition ( ).y + pDstBody->GetLinearVelocity ( ).y * t;

			pSolution = Vector3D::create ( x, y, 0 );
		}
	}

	return pSolution;
}

/// Add DPad 
KDvoid Ch7_ProjectileAiming::addDPad ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "dpad_buttons.plist" );
	
	m_pDPad = DPad::create ( );
	m_pDPad->setPosition ( ccp ( 75, 75 ) );
	m_pDPad->setOpacity ( 100 );
	this->addChild ( m_pDPad, 5 ); 
}

/// Add Actor 
KDvoid Ch7_ProjectileAiming::addActor ( KDvoid )
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
	pFixtureDef->filter.categoryBits = CB_ACTOR;
	pFixtureDef->filter.maskBits = CB_EVERYTHING;
	
	pBody->CreateFixture ( pFixtureDef );

	pBody->SetLinearDamping ( 2.0f );
}

/// Add 3 enemies at random positions 
KDvoid Ch7_ProjectileAiming::addEnemies ( KDvoid )
{
	m_pEnemies = new CCArray ( );

	for ( KDint i = 0; i < 3; i++ )
	{
		GameActor*		pEnemy = GameActor::create ( );
		pEnemy->setGameArea ( this );
		pEnemy->setTag ( GO_TAG_ENEMY );

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
		pFixtureDef->filter.categoryBits = CB_ENEMY;
		pFixtureDef->filter.maskBits = CB_EVERYTHING & ~CB_MISSILE;
		
		pBody->CreateFixture ( pFixtureDef );

		m_pEnemies->addObject ( pEnemy );
	}
}

///  Each enemy fires a missile object 
KDvoid Ch7_ProjectileAiming::fireMissiles ( KDvoid )
{
	for ( KDuint i = 0; i < m_pEnemies->count ( ); i++ )
	{		
		GameActor*		pEnemy = (GameActor*) m_pEnemies->objectAtIndex ( i );
	
		// Create missile
		GameMisc*		pMissile = GameMisc::create ( );
		pMissile->setGameArea ( this );
		pMissile->setTag ( GO_TAG_MISSILE );

		b2BodyDef*		pBodyDef = pMissile->getBodyDef ( );
		pBodyDef->type = b2_dynamicBody;
		pBodyDef->position.Set ( pEnemy->getBody ( )->GetPosition ( ).x, pEnemy->getBody ( )->GetPosition ( ).y );
		pBodyDef->userData = pMissile;
		
		b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
		pMissile->setBody ( pBody );
						
		b2PolygonShape*	pShape = new b2PolygonShape ( );
		pMissile->setPolygonShape ( pShape );
		pShape->SetAsBox ( 0.5f, 0.2f );

		b2FixtureDef*	pFixtureDef = pMissile->getFixtureDef ( );
		pFixtureDef->shape = pShape;
		pFixtureDef->density = 10.f;
		pFixtureDef->filter.categoryBits = CB_MISSILE;
		pFixtureDef->filter.maskBits = CB_EVERYTHING & ~CB_MISSILE & ~CB_ENEMY;
		
		pBody->CreateFixture ( pFixtureDef );
		
		// Calculate intercept trajectory
		Vector3D*		pPoint = this->interceptSrc ( pMissile, m_pActor, 20.0f );
		if ( pPoint )
		{			
			// Align missile
			CCPoint		tPointToFireAt = ccp ( pPoint->x, pPoint->y );
			CCPoint		tDirectionVector = ccp ( tPointToFireAt.x - pBody->GetPosition ( ).x, tPointToFireAt.y - pBody->GetPosition ( ).y );
			KDfloat		fRadians = GameHelper::vectorToRadians ( tDirectionVector );
			pBody->SetTransform ( pBody->GetPosition ( ), -1 * fRadians + PI_CONSTANT / 2 );
		
			// Fire missile
			CCPoint		tNormalVector = GameHelper::radiansToVector ( fRadians );
			pBody->SetLinearVelocity ( b2Vec2 ( tNormalVector.x * 20.0f, tNormalVector.y * 20.0f ) );
		}

		m_pMissiles->addObject ( pMissile );
	}
}