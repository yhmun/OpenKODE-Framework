/* --------------------------------------------------------------------------
 *
 *      File            Ch7_AIFlocking.cpp
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
#include "Ch7_AIFlocking.h"

enum
{
	CB_EVERYTHING	= 0xFFFF,
	CB_ACTOR		= 0x0002,
	CB_ENEMY		= 0x0004,
};

KDbool Ch7_AIFlocking::init ( KDvoid )
{	
	if ( !Ch7_LineOfSight::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Use the DPad to avoid the AI actors." );

	// Add some geometry
	this->addRandomPolygons ( 15 );
	
	// Initial variables
	m_fCameraZoom = 0.225f;

	return KD_TRUE;
}

KDvoid Ch7_AIFlocking::step ( KDfloat fDelta )
{
	Ch7_LineOfSight::step ( fDelta );

	// Process the 'boids' flocking algorithm
	this->processBoids ( );
}

// Set game area size
KDvoid Ch7_AIFlocking::setGameAreaSize ( KDvoid )
{
	m_tGameAreaSize = ccp ( 2000 / PTM_RATIO, 1200 / PTM_RATIO );		// Box2d units
}	

/// Add actor 
KDvoid Ch7_AIFlocking::addActor ( KDvoid )
{
	m_pActor = GameActor::create ( );
	m_pActor->setGameArea ( this );
	
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
	pFixtureDef->filter.maskBits = CB_EVERYTHING & ~CB_ENEMY & ~CB_ACTOR;
	
	pBody->CreateFixture ( pFixtureDef );

	pBody->SetLinearDamping ( 2.0f );
}

/// Add 30 'boid' enemies 
KDvoid Ch7_AIFlocking::addEnemies ( KDvoid )
{		
	m_pEnemies = new CCArray ( );

	for ( KDint i = 0; i < 30; i++ )
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
		pFixtureDef->filter.categoryBits = CB_ENEMY;
		pFixtureDef->filter.maskBits = CB_EVERYTHING & ~CB_ACTOR;

		pBody->CreateFixture ( pFixtureDef );

		m_pEnemies->addObject ( pEnemy );
	}
}

/// Make the flock of 'boids' follow the actor 
KDvoid Ch7_AIFlocking::followActorWithEnemies ( KDvoid )
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
		
		
		b2Vec2		tNormal = tActorPos - pEnemyBody->GetPosition ( );
		CCPoint		tVector = ccp ( tNormal.x, tNormal.y );
		CCPoint		tNormalVector = GameHelper::radiansToVector ( GameHelper::vectorToRadians ( tVector ) );

		// If so, follow the actor
		b2Vec2		tVelocity = pEnemyBody->GetLinearVelocity ( );
		pEnemyBody->SetLinearVelocity ( b2Vec2 ( tVelocity.x + tNormalVector.x * 0.2f, tVelocity.y + tNormalVector.y * 0.2f ) );
	}
}

/// Process boids algorithm 
KDvoid Ch7_AIFlocking::processBoids ( KDvoid )
{
	for ( KDuint i = 0; i < m_pEnemies->count ( ); i++ )
	{
		GameActor*	pBoid = (GameActor*) m_pEnemies->objectAtIndex ( i );
		b2Body*		pBoidBody = pBoid->getBody ( );
		
		b2Vec2		tV1 = b2Vec2 ( 0, 0 );
		b2Vec2		tV2 = b2Vec2 ( 0, 0 );
		b2Vec2		tV3 = b2Vec2 ( 0, 0 );
		
		tV1 = this->boidRule1 ( pBoid );
		tV2 = this->boidRule2 ( pBoid );
		tV3 = this->boidRule3 ( pBoid );
	
		b2Vec2		tV = pBoidBody->GetLinearVelocity ( );
		b2Vec2		tNewV = tV + tV1 + tV2 + tV3;
			
		// Limit velocity 
		KDfloat		fLimit = 7.5f;
		b2Vec2		tAbsV = b2Vec2 ( GameHelper::absoluteValue ( tNewV.x ), GameHelper::absoluteValue ( tNewV.y ) );
		if ( tAbsV.x > fLimit || tAbsV.y > fLimit )
		{
			KDfloat		fRatio;
			if ( tAbsV.x > tAbsV.y )
			{
				fRatio = fLimit / tAbsV.x;			
			}
			else
			{
				fRatio = fLimit / tAbsV.y;
			}
			tNewV = b2Vec2 ( tNewV.x * fRatio, tNewV.y * fRatio );
		}		
		pBoidBody->SetLinearVelocity ( tNewV );
	}
}

/// Clump the Boids together 
b2Vec2 Ch7_AIFlocking::boidRule1 ( GameActor* pBoidJ )
{
	b2Vec2		tBoidJ = b2Vec2 ( 0, 0 );
	KDfloat		fCount = (KDfloat) m_pEnemies->count ( );

	for ( KDuint i = 0; i < m_pEnemies->count ( ); i++ )
	{
		GameActor*		pBoid = (GameActor*) m_pEnemies->objectAtIndex ( i );

		if ( pBoid != pBoidJ ) 
		{
			tBoidJ += pBoid->getBody ( )->GetPosition ( );
		}
	}
	
	tBoidJ = b2Vec2 ( tBoidJ.x / ( fCount - 1 ), tBoidJ.y / ( fCount - 1 ) );
	
	b2Vec2		tBoidJPos = pBoidJ->getBody ( )->GetPosition ( );
	return b2Vec2 ( ( tBoidJ.x - tBoidJPos.x ) / 100.0f, ( tBoidJ.y - tBoidJPos.y ) / 100.0f );
}

/// Keep the Boids apart from each other 
b2Vec2 Ch7_AIFlocking::boidRule2 ( GameActor* pBoidJ )
{
	// Set optimal distance boids should keep between themselves
	KDfloat		fPadding = 1.5f;
	b2Vec2		c = b2Vec2 ( 0, 0 );
	
	b2Vec2		tBoidJPos = pBoidJ->getBody ( )->GetPosition ( );	
	for ( KDuint i = 0; i < m_pEnemies->count ( ); i++ )
	{
		GameActor*		pBoid = (GameActor*) m_pEnemies->objectAtIndex ( i );
		if ( pBoid != pBoidJ ) 
		{
			b2Vec2		tBoidPos = pBoid->getBody ( )->GetPosition ( );		

			if ( GameHelper::distanceP1 ( ccp ( tBoidPos.x, tBoidPos.y ), ccp ( tBoidJPos.x, tBoidJPos.y ) ) < fPadding )
			{
				// This was 100 in the example
				c = c - ( tBoidPos - tBoidJPos );
			}
		}
	}
	
	return c;
}

/// Match up all Boid velocities 
b2Vec2 Ch7_AIFlocking::boidRule3 ( GameActor* pBoidJ )
{
	b2Vec2		tBoidJ = b2Vec2 ( 0, 0 );

	KDint		nCount = m_pEnemies->count ( );
	for ( KDint i = 0; i < nCount; i++ )
	{
		GameActor*		pBoid = (GameActor*) m_pEnemies->objectAtIndex ( i );
		if ( pBoid != pBoidJ ) 
		{
			tBoidJ += pBoid->getBody ( )->GetLinearVelocity ( );
		}
	}
	
	tBoidJ = b2Vec2 ( tBoidJ.x / nCount - 1, tBoidJ.y / nCount - 1 );
	
	b2Vec2		tVelocity = pBoidJ->getBody ( )->GetLinearVelocity ( );
	return b2Vec2 ( ( tBoidJ.x - tVelocity.x ) / 30.0f, ( tBoidJ.y - tVelocity.y ) / 30.0f );
}