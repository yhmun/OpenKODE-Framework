/* --------------------------------------------------------------------------
 *
 *      File            GameActor.cpp
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
#include "GameActor.h"

#define WAYPOINT_DIST_THRESHOLD		16.0f
#define TIMES_BLOCKED_FAIL			2

GameActor* GameActor::create ( KDvoid )
{
	GameActor*		pRet = new GameActor ( );

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

GameActor::GameActor ( KDvoid )
{
	m_pWaypoints = KD_NULL;
}

GameActor::~GameActor ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pWaypoints );
}

KDbool GameActor::init ( KDvoid )
{
	if ( !GameObject::init ( ) )
	{
		return KD_FALSE;
	}

	m_pWaypoints = CCArray::create ( );
	m_pWaypoints->retain ( );

	m_fRunSpeed				= 10.0f;
	m_nTimesBlocked			= 0;
	m_fLastAngularVelocity	= 0.0f;
	m_fAnimCount			= 0.0f;
	m_tLastVelocity			= ccpz;
	m_nDirection			= DOWN_LEFT;
	m_sColor				= "lightblue";

	return KD_TRUE;
}

KDvoid GameActor::addWaypoint ( GameWaypoint* pWaypoint )
{
	m_pWaypoints->addObject ( pWaypoint );
}

KDvoid GameActor::processWaypoints ( KDvoid )
{
	KDbool		bRemoveFirstWaypoint = KD_FALSE;
	
	// The actor's position onscreen
	CCPoint		tWorldPosition = ccp ( m_pBody->GetPosition ( ).x * PTM_RATIO, m_pBody->GetPosition ( ).y * PTM_RATIO );

	// Process waypoints
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pWaypoints, pObject )
	{
		GameWaypoint*	pWaypoint = (GameWaypoint*) pObject;

		KDfloat			fDistanceToNextPoint = GameHelper::distanceP1 ( tWorldPosition, pWaypoint->getPosition ( ) );
		
		// If we didn't make progress to the next point, increment timesBlocked
		if ( fDistanceToNextPoint >= pWaypoint->getLastDistance ( ) )
		{
			m_nTimesBlocked++;
			
			// Drop this waypoint if we failed to move a number of times
			if ( m_nTimesBlocked > TIMES_BLOCKED_FAIL )
			{
				fDistanceToNextPoint = 0.0f;
			}
		}
		else
		{
			// If we are just starting toward this point we run our pre-callback
			pWaypoint->setLastDistance ( fDistanceToNextPoint );
			pWaypoint->processPreCallback ( );
		}

		// If we are close enough to the waypoint we move onto the next one
		if ( fDistanceToNextPoint <= WAYPOINT_DIST_THRESHOLD )
		{
			bRemoveFirstWaypoint = KD_TRUE;
			this->stopRunning ( );
			
			// Run post callback
			pWaypoint->processPostCallback ( );
		}
		else
		{
			// Keep running toward the waypoint
			KDfloat		fSpeedMod = pWaypoint->getSpeedMod ( );
			
			// Slow down close to the waypoint
			if ( fDistanceToNextPoint < this->getRunSpeed ( ) / PTM_RATIO )
			{
				fSpeedMod = ( fDistanceToNextPoint ) / ( this->getRunSpeed ( ) / PTM_RATIO );
			}
			this->runWithVector ( ccpSub ( pWaypoint->getPosition ( ), tWorldPosition ), fSpeedMod, KD_FALSE );
			break;
		}
	}

	if ( bRemoveFirstWaypoint )
	{
		m_pWaypoints->removeObjectAtIndex ( 0 );
		m_nTimesBlocked = 0;
	}
}

KDvoid GameActor::runWithVector ( const CCPoint& tVect, KDfloat fSpeedMod, KDbool bConstrain )
{
	// Change animation depending on angle
	KDfloat		fRadians = GameHelper::vectorToRadians ( tVect );
	KDfloat		fDegrees = GameHelper::radiansToDegrees ( fRadians );
	CCPoint		tConstrainedVector;														// Vector constained to only the 8 angles
//	CCPoint		tUnconstrainedVector = GameHelper::radiansToVector ( fRadians );		// Unconstrained vector

	fDegrees += 90.0f;
		
	if ( fDegrees >= 337.5f || fDegrees < 22.5f )
	{
		m_nDirection = LEFT;
	}
	else if ( fDegrees >= 22.5f && fDegrees < 67.5f )
	{
		m_nDirection = UP_LEFT;
	}
	else if ( fDegrees >= 67.5f && fDegrees < 112.5f )
	{
		m_nDirection = UP;
	}
	else if ( fDegrees >= 112.5f && fDegrees < 157.5f )
	{
		m_nDirection = UP_RIGHT;
	}
	else if ( fDegrees >= 157.5f && fDegrees < 202.5f )
	{
		m_nDirection = RIGHT;
	}
	else if ( fDegrees >= 202.5f && fDegrees < 247.5f )
	{
		m_nDirection = DOWN_RIGHT;
	}
	else if ( fDegrees >= 247.5f && fDegrees < 292.5f )
	{
		m_nDirection = DOWN;
	}
	else
	{
		m_nDirection = DOWN_LEFT;
	}
	
	tConstrainedVector = GameActor::getNormalVectorFromDirection ( m_nDirection );

	if ( bConstrain )
	{
		m_pBody->SetLinearVelocity ( b2Vec2 ( tConstrainedVector.x * m_fRunSpeed * fSpeedMod, tConstrainedVector.y * m_fRunSpeed * fSpeedMod ) );
	} 
	else
	{
		m_pBody->SetLinearVelocity ( b2Vec2 ( tConstrainedVector.x * m_fRunSpeed * fSpeedMod, tConstrainedVector.y * m_fRunSpeed * fSpeedMod ) );
	}
					
	if ( m_fLastAngularVelocity != 0.0f && m_fLastAngularVelocity == m_pBody->GetAngularVelocity ( ) )
	{
		m_pBody->SetAngularVelocity ( 0.0f );
	}
	m_fLastAngularVelocity = m_pBody->GetAngularVelocity ( );
	m_tLastVelocity = ccp ( m_pBody->GetLinearVelocity ( ).x, m_pBody->GetLinearVelocity ( ).y );
}

KDvoid GameActor::stopRunning ( KDvoid )
{
	m_pBody->SetLinearVelocity ( b2Vec2 ( 0.0f, 0.0f ) );
}

CCPoint GameActor::getNormalVectorFromDirection ( KDint nDirection )
{
	switch ( nDirection )
	{
		case LEFT		:	return ccp ( -1,  0 );
		case UP_LEFT	:	return ccp ( -0.7071067812,  0.7071067812 );
		case UP			:	return ccp (  0,  1 );
		case UP_RIGHT	:	return ccp (  0.7071067812,  0.7071067812 );
		case RIGHT		:	return ccp (  1,  0 );
		case DOWN_RIGHT :	return ccp (  0.7071067812, -0.7071067812 );
		case DOWN		:	return ccp (  0, -1 );
		case DOWN_LEFT	:	return ccp ( -0.7071067812, -0.7071067812 );
		default			:	return ccp (  0,  0 );
	}

	return ccpz;
}

KDvoid GameActor::animateActor ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Animation delay is inverse speed
	KDfloat			fDelay = 0.5f / kdSqrtf ( kdPowf ( m_tLastVelocity.x, 2 ) + kdPowf ( m_tLastVelocity.y, 2 ) );
	if ( fDelay > 0.5f )
	{
		fDelay = 0.5f; 
	}

	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( fDelay );
	
	const KDchar*	szDirection;
	KDbool			bFlipX  = KD_FALSE;
	KDbool			bMoving = KD_TRUE;

	if ( kdSqrtf ( kdPowf ( m_tLastVelocity.x, 2 ) + kdPowf ( m_tLastVelocity.y, 2 ) ) < 0.2f )
	{
		bMoving = KD_FALSE; 
	}

	switch ( m_nDirection )
	{
		case LEFT		:	szDirection = "right";		bFlipX = KD_TRUE;	break;
		case UP_LEFT	:	szDirection = "up_right";	bFlipX = KD_TRUE;	break;
		case UP			:	szDirection = "up";								break;
		case UP_RIGHT	:	szDirection = "up_right";						break;
		case RIGHT		:	szDirection = "right";							break;
		case DOWN_RIGHT	:	szDirection = "down_right";						break;
		case DOWN		:	szDirection = "down";							break;
		case DOWN_LEFT	:	szDirection = "down_right";	bFlipX = KD_TRUE;	break;
	}

	CCSprite*	pSprite = (CCSprite*) m_pSprite->getChildByTag ( 0 );
	pSprite->setFlipX ( bFlipX );

	// Our simple running loop
	if ( bMoving )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_%s_run_%s_01.png", m_sColor.c_str ( ), szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_%s_stand_%s.png" , m_sColor.c_str ( ), szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_%s_run_%s_02.png", m_sColor.c_str ( ), szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_%s_stand_%s.png" , m_sColor.c_str ( ), szDirection ) ) );
	}
	else
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_%s_stand_%s.png" , m_sColor.c_str ( ), szDirection ) ) );
	}

	// animateActor calls itself indefinitely
	pSprite->runAction
	(
		CCSequence::create 
		(
			CCAnimate::create ( pAnimation ),
			CCCallFunc::create ( this, callfunc_selector ( GameActor::animateActor ) ),
			KD_NULL 
		)
	);
}

KDvoid GameActor::timeShift ( KDfloat fSeconds, const CCPoint& tVect, KDfloat fMagnitude )
{
	b2Vec2		tForce = b2Vec2 ( tVect.x * fMagnitude, tVect.y * fMagnitude );

	m_pBody->SetLinearVelocity ( m_pBody->GetLinearVelocity ( ) + fSeconds * ( 1.0f / m_pBody->GetMass ( ) * tForce ) );
		
	KDfloat		fClamp = clampf ( 1.0f - fSeconds * m_pBody->GetLinearDamping ( ), 0.0f, 1.0f );
	b2Vec2		tVect2 = m_pBody->GetLinearVelocity ( );
	m_pBody->SetLinearVelocity ( b2Vec2 ( tVect2.x * fClamp, tVect2.y * fClamp ) );
}

CCArray* GameActor::getWaypoints ( KDvoid )
{
	return m_pWaypoints;
}

KDvoid GameActor::setWaypoints ( CCArray* pWaypoints )
{
	CC_SAFE_RELEASE ( m_pWaypoints );
	m_pWaypoints = pWaypoints;
	CC_SAFE_RETAIN  ( m_pWaypoints );
}

KDfloat GameActor::getRunSpeed ( KDvoid )
{
	return m_fRunSpeed;
}

KDvoid GameActor::setRunSpeed ( KDfloat fRunSpeed )
{
	m_fRunSpeed = fRunSpeed;
}

KDint GameActor::getDirection ( KDvoid )
{
	return m_nDirection;
}

KDvoid GameActor::setDirection ( KDint nDirection )
{
	m_nDirection = nDirection;
}

const CCPoint& GameActor::getLastVelocity ( KDvoid )
{
	return m_tLastVelocity;
}

KDvoid GameActor::setLastVelocity ( const CCPoint& tLastVelocity )
{
	m_tLastVelocity = tLastVelocity;
}

const KDchar* GameActor::getColor ( KDvoid )
{
	return m_sColor.c_str ( );
}

KDvoid GameActor::setColor ( const KDchar* szColor )
{
	m_sColor = szColor;
}

KDfloat GameActor::getAnimCount ( KDvoid )
{
	return m_fAnimCount;
}

KDvoid GameActor::setAnimCount ( KDfloat fAnimCount )
{
	m_fAnimCount = fAnimCount;
}