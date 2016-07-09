/* --------------------------------------------------------------------------
 *
 *      File            Enemy.cpp
 *      Description     
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Kirill Muzykov on 8/22/13.
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
#include "Enemy.h"
#include "Tower.h"
#include "GameLayer.h"
#include "Waypoint.h"

#define HEALTH_BAR_WIDTH		 20
#define HEALTH_BAR_ORIGIN		-10
#define PLAYER_AWARD			200

Enemy* Enemy::create ( const KDchar* szFileName )
{
    Enemy*		pRet = new Enemy ( );

    if ( pRet && pRet->initWithFile ( szFileName ) )
    {
        pRet->autorelease ( );
	}
	else
	{
        CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

KDbool Enemy::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated )
{
    if ( !CCSprite::initWithTexture ( pTexture, tRect, bRotated ) )
    {
        return KD_FALSE;
    }
    
    m_nMaxHP		= 40;
    m_nCurrentHP	= m_nMaxHP;
    m_bActive		= KD_FALSE;
    m_fWalkingSpeed = 1.0f;

	m_pTheGame		= KD_NULL;
	m_pDestinationWaypoint = KD_NULL;
    
    this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid Enemy::draw ( KDvoid )
{
    CCPoint		tLocalPos = CCNode::convertToNodeSpace ( this->getPosition ( ) );
    
    ccDrawSolidRect
	(
		ccp ( tLocalPos.x + HEALTH_BAR_ORIGIN, tLocalPos.y + 16 ),
		ccp ( tLocalPos.x + HEALTH_BAR_ORIGIN + HEALTH_BAR_WIDTH, tLocalPos.y + 14 ),
		ccc4f ( 1.0, 0, 0, 1.0 )
	);
    
    ccDrawSolidRect
	(
		ccp ( tLocalPos.x + HEALTH_BAR_ORIGIN, tLocalPos.y + 16 ),
		ccp ( tLocalPos.x + HEALTH_BAR_ORIGIN + (KDfloat) ( m_nCurrentHP * HEALTH_BAR_WIDTH ) / m_nMaxHP, tLocalPos.y + 14 ),
		ccc4f ( 0, 1.0, 0, 1.0 ) 
	);
    
    CCSprite::draw ( );
}

KDvoid Enemy::update ( KDfloat fDelta )
{
    if ( !m_bActive )
	{
        return;
	}
    
    CCAssert ( m_pTheGame != NULL, "You have to set the game for the Enemy before you can use it");
    
    // Checking if we've reached destination waypoint.
    if ( m_pTheGame->checkCirclesCollide ( this->getPosition ( ), 1.0f, m_pDestinationWaypoint->getPosition ( ), 1.0f ) )
    {
        if ( m_pDestinationWaypoint->getNextWaypoint ( ) )
        {
            // This is not the end of the way. Taking next waypoint and moving towards it.
            m_pDestinationWaypoint = m_pDestinationWaypoint->getNextWaypoint ( );
        }
        else
        {
            // We've reached the end of the path. Means we're reached player base. Making some damage.
            m_pTheGame->getHpDamage ( );
            this->getRemoved ( );
        }
    }
    
    // Actual moving to next waypoint.
    CCPoint		tTargetPoint = m_pDestinationWaypoint->getPosition ( );
    CCPoint		tNormalized  = ccpNormalize ( ccpSub ( tTargetPoint, this->getPosition ( ) ) );
    
    KDfloat		fMovementSpeed = m_fWalkingSpeed;
    KDfloat		fAngle = CC_RADIANS_TO_DEGREES ( kdAtan2f ( tNormalized.y, -tNormalized.x ) );
    CCPoint		tDesiredPosition = ccpAdd ( this->getPosition ( ), ccpMult ( tNormalized, fMovementSpeed ) );
    
    this->setPosition ( tDesiredPosition );
    this->setRotation ( fAngle );
}

KDvoid Enemy::getRemoved ( KDvoid )
{
    // Notifying each tower that they should pick other target, since we're dead.
    CCObject*	pObject = KD_NULL;
    CCARRAY_FOREACH ( &m_aAttackedBy, pObject )
    {
		Tower*		pTower = (Tower*) pObject;
		pTower->targetKilled ( );
    }
    
    // Notifying game layer & removing self.
    m_pTheGame->enemyGotKilled ( this );
    this->removeFromParentAndCleanup ( KD_TRUE );
}

KDvoid Enemy::setTheGame ( GameLayer* pGame )
{
	m_pTheGame = pGame;
}

KDvoid Enemy::setDestinationWaypoint ( Waypoint* pWaypoint )
{
	m_pDestinationWaypoint = pWaypoint;
}

KDvoid Enemy::doActivate ( KDfloat fDelta )
{
    m_bActive = KD_TRUE;
}

KDvoid Enemy::getAttacked ( Tower* pAttacker )
{
	m_aAttackedBy.addObject ( pAttacker );
}

KDvoid Enemy::getDamaged ( KDint nDamage )
{
    SimpleAudioEngine::sharedEngine ( )->playEffect ( "laser_shoot.wav" );
    
    m_nCurrentHP -= nDamage;
    if ( m_nCurrentHP <= 0 )
    {
        // We're killed. Need to reward player and remove self.
        m_pTheGame->awardGold ( PLAYER_AWARD );
        this->getRemoved ( );
    }
}

KDvoid Enemy::gotLostSight ( Tower* pAttacker )
{
	m_aAttackedBy.removeObject ( pAttacker );
}
