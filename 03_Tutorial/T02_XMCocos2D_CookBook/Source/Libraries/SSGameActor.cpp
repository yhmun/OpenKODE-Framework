/* --------------------------------------------------------------------------
 *
 *      File            SSGameActor.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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
#include "SSGameActor.h"
#include "GameArea2D.h"
#include "RayCast.h"

#define WP_TIMEOUT					 50.0f
#define WP_RUN_DIST_THRESHOLD_X		  4.0f
#define WP_RUN_DIST_THRESHOLD_Y		 64.0f
#define WP_JUMP_DIST_THRESHOLD_X	256.0f
#define WP_JUMP_DIST_THRESHOLD_Y	 64.0f
#define TIMES_BLOCKED_FAIL			  2

SSGameActor* SSGameActor::create ( KDvoid )
{
	SSGameActor*	pRet = new SSGameActor ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

SSGameActor::SSGameActor ( KDvoid )
{
	m_pWaypoints = KD_NULL;
}

SSGameActor::~SSGameActor ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pWaypoints );
}

KDbool SSGameActor::init ( KDvoid )
{
	if ( !GameActor::init ( ) )
	{
		return KD_FALSE;
	}

	m_tMaxSpeed = ccp ( 10.0f, 20.0f );
	m_fTimeoutCounter = 0.0f;

	return KD_TRUE;
}

KDvoid SSGameActor::processWaypoints ( KDvoid )
{
	if ( m_pWaypoints->count ( ) == 0 )
	{
		return; 
	}

	// Here we need to process waypoints not by basic distance but by how quickly they can be EITHER jumped to or run to
	// in 2D space. Estimating run time will require a little bit of calc. We already have the jump time.
	KDbool		bRemoveFirstWaypoint = KD_FALSE;
	KDbool		bBlocked = KD_FALSE;
	
	CCPoint		tWorldPosition = ccp ( m_pBody->GetPosition ( ).x * PTM_RATIO, m_pBody->GetPosition ( ).y * PTM_RATIO );

	SSGameWaypoint*		pWaypoint = (SSGameWaypoint*) m_pWaypoints->objectAtIndex ( 0 );
	CCPoint				tVectorToNextPoint = ccp
	(
		GameHelper::absoluteValue ( tWorldPosition.x - pWaypoint->getPosition ( ).x ), 
		GameHelper::absoluteValue ( tWorldPosition.y - pWaypoint->getPosition ( ).y )
	);
	KDfloat		fDistanceToNextPoint = GameHelper::distanceP1 ( tWorldPosition, pWaypoint->getPosition ( ) );

	if ( fDistanceToNextPoint >= pWaypoint->getLastDistance ( ) )
	{
		m_fTimeoutCounter += 1.0f;
		if ( m_fTimeoutCounter > WP_TIMEOUT )
		{
			bBlocked = KD_TRUE;					// Looks like you are blocked. Dropping all waypoints
		}
	}
	else
	{
		pWaypoint->setLastDistance ( fDistanceToNextPoint );
		pWaypoint->processPreCallback ( );
	}

	KDint	nMoveType = pWaypoint->getMoveType ( );
			
	if ( nMoveType == MOVE_TYPE_RUN && tVectorToNextPoint.x <= WP_RUN_DIST_THRESHOLD_X && tVectorToNextPoint.y <= WP_RUN_DIST_THRESHOLD_Y )
	{
		bRemoveFirstWaypoint = KD_TRUE;
		this->stopRunning ( );
		pWaypoint->processPostCallback ( );		
	}
	else if ( nMoveType == MOVE_TYPE_JUMP && tVectorToNextPoint.x <= WP_JUMP_DIST_THRESHOLD_X && tVectorToNextPoint.y <= WP_JUMP_DIST_THRESHOLD_Y )
	{
		bRemoveFirstWaypoint = KD_TRUE;
		this->stopRunning ( );
		pWaypoint->processPostCallback ( );			
	}
	else
	{
		if ( nMoveType == MOVE_TYPE_RUN )
		{				
			KDfloat		fSpeedMod = pWaypoint->getSpeedMod ( );
			if ( fDistanceToNextPoint < this->getRunSpeed ( ) / PTM_RATIO )
			{
				fSpeedMod = ( fDistanceToNextPoint ) / ( this->getRunSpeed ( ) / PTM_RATIO );
			}
			this->runToWaypoint ( pWaypoint, fSpeedMod, KD_FALSE );			
		}
		else if ( nMoveType == MOVE_TYPE_JUMP && pWaypoint->isActive ( ) )
		{			
			// We can't jump while in the air
			if ( GameHelper::absoluteValue ( m_pBody->GetLinearVelocity ( ).y ) < 0.5f )
			{
				this->jumpToWaypoint ( pWaypoint );
				pWaypoint->setActive ( KD_FALSE );
			}			
		}
		else if ( nMoveType == MOVE_TYPE_JUMP && !pWaypoint->isActive ( ) )
		{			
			//Do nothing
		}
		else if ( nMoveType == MOVE_TYPE_FLY )
		{
			this->flyToWaypoint ( pWaypoint );
		}
	}

	if ( bBlocked )
	{
		m_fTimeoutCounter = 0.0f;
		m_nTimesBlocked = 0;
		this->stopRunning ( );
		m_pWaypoints->removeAllObjects ( );
	}
	else if ( bRemoveFirstWaypoint )
	{	
		// Stop the actor if he's running
		if ( m_pWaypoints->count ( ) == 1 )
		{
			b2Vec2		tVelocity = m_pBody->GetLinearVelocity ( );
			m_pBody->SetLinearVelocity ( b2Vec2 ( 0, tVelocity.y / 10 ) );
		}
	
		m_fTimeoutCounter = 0.0f;
		m_pWaypoints->removeObjectAtIndex ( 0 );
		m_nTimesBlocked = 0;
	}
}

KDvoid SSGameActor::addWaypoint ( SSGameWaypoint* pWaypoint )
{
	m_pWaypoints->addObject ( pWaypoint );
}

KDvoid SSGameActor::runToWaypoint ( SSGameWaypoint* pWaypoint, KDfloat fSpeedMod, KDbool bConstrain )
{
	// Run either left or right
	
	//We can't run while in the air...
	if ( GameHelper::absoluteValue ( m_pBody->GetLinearVelocity ( ).y ) != 0 )
	{
		return;
	}
	
	CCPoint		tSrc = ccp ( m_pBody->GetPosition ( ).x * PTM_RATIO, m_pBody->GetPosition ( ).y * PTM_RATIO );
	CCPoint		tDst = pWaypoint->getPosition ( );
	
	b2Vec2		tVelocity = m_pBody->GetLinearVelocity ( );
	if ( GameHelper::absoluteValue ( tDst.x - tSrc.x ) < WP_RUN_DIST_THRESHOLD_X )
	{
		m_pBody->SetLinearVelocity ( b2Vec2 ( 0, tVelocity.y ) );
	}
	else if ( tDst.x < tSrc.x )
	{
		m_pBody->SetLinearVelocity ( b2Vec2 ( -1 * fSpeedMod * m_fRunSpeed, tVelocity.y ) );
	}
	else
	{
		m_pBody->SetLinearVelocity ( b2Vec2 ( fSpeedMod * m_fRunSpeed, tVelocity.y ) );
	}
}

KDvoid SSGameActor::jumpToWaypoint ( SSGameWaypoint* pWaypoint )
{
	// Jump to a location
	m_pBody->SetLinearVelocity ( b2Vec2 ( pWaypoint->getLaunchVector ( ).x, pWaypoint->getLaunchVector ( ).y ) );
}

Vector3D* SSGameActor::canJumpFrom ( const CCPoint& tSrc, const CCPoint& tDst, KDfloat fRadius, b2World* pWorld, const CCPoint& tMaxSpeed )
{
	KDfloat		x = tDst.x - tSrc.x;
	KDfloat		y = tDst.y - tSrc.y;
	if ( y == 0 )
	{
		y = 0.00001f;	// Prevent divide by zero
	}

	KDbool		bFoundJumpSolution	= KD_FALSE;
	KDbool		bTriedAngles		= KD_FALSE;
	CCPoint		tLaunchVector;
	KDfloat		fJumpHeightMod		= 0.5f;
	
	while ( !bTriedAngles ) 
	{		
		// Gravity
		KDfloat		fGravity = pWorld->GetGravity ( ).y;
		if ( fGravity == 0 )
		{
			fGravity = 0.00001f;	// Prevent divide by zero
		}

		tLaunchVector = SSGameActor::getLaunchVector ( ccp ( x, y ), fJumpHeightMod, fGravity );	
				
		KDbool		bHitObject = KD_FALSE;
		KDbool		bMovingTooFast = KD_FALSE;
	
		// Make sure jump doesn't hit an object 
		Vector3D*	pAirVect = GameHelper::quadraticA ( fGravity * 0.5f, tLaunchVector.y, y * -1 );
		KDfloat		fAirTime;
		if ( pAirVect->x > pAirVect->y )
		{
			fAirTime = pAirVect->x;
		}
		else
		{
			fAirTime = pAirVect->y;
		}
		
		// Do a ray test sequence (from 0.1 to 0.9 of airTime)
		for ( KDfloat t = fAirTime / 10; t < fAirTime - fAirTime / 10; t += fAirTime / 10 )
		{
			if ( bHitObject )
			{
				break; 
			}
		
			KDfloat		t1 = t + fAirTime / 10;
	
			KDfloat		x1 = tLaunchVector.x * t + tSrc.x;
			KDfloat		y1 = tLaunchVector.y * t + ( 0.5f ) * fGravity * kdPowf ( t, 2 ) + tSrc.y;
			
			KDfloat		x2 = tLaunchVector.x * t1 + tSrc.x;
			KDfloat		y2 = tLaunchVector.y * t1 + ( 0.5f ) * fGravity * kdPowf ( t1, 2 ) + tSrc.y;
										
			// Point Test	
			for ( b2Body* pBody = pWorld->GetBodyList ( ); pBody; pBody = pBody->GetNext ( ) )
			{
				if ( pBody->GetUserData ( ) != KD_NULL )
				{
					GameObject*		pObject = (GameObject*) pBody->GetUserData ( );
					if ( pObject->getTag ( ) == GO_TAG_WALL && pObject->getPolygonShape ( ) )
					{
						for ( KDfloat x_ = -fRadius; x_ <= fRadius; x_ += fRadius * 2 )
						{
							for ( KDfloat y_ = -fRadius; y_ <= fRadius; y_ += fRadius * 2 )
							{	
								if ( pObject->getPolygonShape ( )->TestPoint ( pBody->GetTransform ( ), b2Vec2 ( x1 + x_, y1 + y_ ) ) ) 
								{
									bHitObject = KD_TRUE;
									break; break;
								}
							}
						}
						if ( bHitObject )
						{
							break; 
						}
					}
				}
			}
			
			// RayCast Test
			for ( KDfloat x_ = -fRadius; x_ <= fRadius; x_+= fRadius * 2 )
			{
				for ( KDfloat y_ = -fRadius; y_ <= fRadius; y_+= fRadius * 2 )
				{								
					if ( GameHelper::distanceP1 ( ccp ( x1 + x_, y1 + y_ ), ccp ( x2 + x_, y2 + y_ ) ) > 0 )
					{
						RayCastMultipleCallback		tCallback;
						pWorld->RayCast ( &tCallback, b2Vec2 ( x1 + x_, y1 + y_ ), b2Vec2 ( x2 + x_, y2 + y_ ) );
					
						for ( KDint i = 0; i < tCallback.m_nCount; i++ )
						{
							GameObject*		pObject = (GameObject*) tCallback.m_aFixtures [ i ]->GetBody ( )->GetUserData ( );
							if ( pObject->getTag ( ) == GO_TAG_WALL )
							{
								bHitObject = KD_TRUE;
								break;
							}
						}
					}
					if ( bHitObject )
					{
						break; break; 
					}
				}
			}
		}
		
		// Make sure the launchVector is not too fast for this actor
		if ( !bHitObject )
		{
			if ( GameHelper::absoluteValue ( tLaunchVector.x ) > tMaxSpeed.x || GameHelper::absoluteValue ( tLaunchVector.y ) > tMaxSpeed.y )
			{
				bMovingTooFast = KD_TRUE;
			}
		}

		if ( bHitObject || bMovingTooFast )
		{
			// This jump failed, try another
			if ( fJumpHeightMod <= 0.5f && fJumpHeightMod >= 0.2f )
			{
				// First, try between 0.5f and 0.1f
				fJumpHeightMod -= 0.1f;
			}
			else if ( fJumpHeightMod > 0.5f && fJumpHeightMod < 1.0f )
			{
				// Then try between 0.6f and 1.0f
				fJumpHeightMod += 0.1f;
			}
			else if ( fJumpHeightMod < 0.2f )
			{
				fJumpHeightMod = 0.6f;
			}
			else if ( fJumpHeightMod >= 1.0f )
			{
				// FAIL
				bTriedAngles = KD_TRUE;
			}
		}
		else
		{
			// SUCCESS
			bFoundJumpSolution = KD_TRUE;
			bTriedAngles = KD_TRUE;
		}
	}

	if ( bFoundJumpSolution )
	{
		return Vector3D::create ( tLaunchVector.x, tLaunchVector.y, 0 );
	}
	else
	{
		return KD_NULL;
	}
}

CCPoint SSGameActor::getLaunchVector ( const CCPoint& tVect, KDfloat fJumpHeightMod, KDfloat fGravity )
{
	// Gravity
	if ( fGravity == 0 )
	{
		fGravity = 0.00001f;		// Prevent divide by zero
	}
	
	// The angle between the points
	KDfloat		fDirectionAngle = GameHelper::vectorToRadians ( ccp ( tVect.x, tVect.y ) );

	// Jump height is a percentage of X distance, usually 0.5f
	KDfloat		fApexX;
	if ( tVect.y > 0 )
	{
		fApexX = tVect.x - ( tVect.x * 0.5f * kdPowf ( GameHelper::absoluteValue ( kdSinf ( fDirectionAngle ) ), 0.5f / fJumpHeightMod ) );
	}
	else
	{
		fApexX = tVect.x * 0.5f * kdPowf ( GameHelper::absoluteValue ( kdSinf ( fDirectionAngle ) ), 0.5f / fJumpHeightMod );
	}
	
	KDfloat		fApexY;
	if ( tVect.y > 0 )
	{
		fApexY = tVect.y + GameHelper::absoluteValue ( tVect.x * fJumpHeightMod ) * GameHelper::absoluteValue ( kdSinf ( fDirectionAngle ) );
	}
	else
	{
		fApexY = GameHelper::absoluteValue ( tVect.x * fJumpHeightMod ) * GameHelper::absoluteValue ( kdSinf ( fDirectionAngle ) );
	}

	// Get launch vector
	KDfloat		fVectY = kdSqrtf ( 2 * ( -1 ) * fGravity * fApexY );
	KDfloat		fVectX = ( fApexX * ( -1 ) * fGravity ) / fVectY;
	
	return ccp ( fVectX, fVectY );
}

KDvoid SSGameActor::flyToWaypoint ( SSGameWaypoint* pWaypoint )
{
	// Apply anti-gravity force
	m_pBody->ApplyForce ( -m_pBody->GetMass ( ) * m_pBody->GetWorld ( )->GetGravity ( ), m_pBody->GetWorldCenter ( ) );

	// Apply directional flying force
	CCPoint		tWorldPosition = ccp ( m_pBody->GetPosition ( ).x * PTM_RATIO, m_pBody->GetPosition ( ).y * PTM_RATIO );
	CCPoint		tVector = ccp ( pWaypoint->getPosition ( ).x - tWorldPosition.x, pWaypoint->getPosition ( ).y - tWorldPosition.y );
	
	KDfloat		fRadians = GameHelper::vectorToRadians ( tVector );
	CCPoint		tNormalVector = GameHelper::radiansToVector ( fRadians );
		
	KDfloat		fFlySpeed = 10.0f;
		
	m_pBody->ApplyForce ( b2Vec2 ( tNormalVector.x * fFlySpeed, tNormalVector.y * fFlySpeed ), m_pBody->GetPosition ( ) );
}

KDvoid SSGameActor::stopRunning ( KDvoid )
{

}

const CCPoint& SSGameActor::getMaxSpeed ( KDvoid )
{
	return m_tMaxSpeed;
}

KDvoid SSGameActor::setMaxSpeed ( const CCPoint& tMaxSpeed )
{
	m_tMaxSpeed = tMaxSpeed;
}

