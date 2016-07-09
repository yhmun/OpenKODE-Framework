/* --------------------------------------------------------------------------
 *
 *      File            Tower.cpp
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
#include "Tower.h"
#include "Enemy.h"
#include "GameLayer.h"

Tower* Tower::create ( const KDchar* szFileName )
{
    Tower*		pRet = new Tower ( );

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

KDbool Tower::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated )
{
    if ( !CCSprite::initWithTexture ( pTexture, tRect, bRotated ) )
    {
        return KD_FALSE;
    }
    
	m_fAttackRange	= 70;
	m_nDamage		= 10;
	m_fFireRate		= 1.0f;

	m_pTheGame		= KD_NULL;
	m_pChosenEnemy	= KD_NULL;

    this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid Tower::draw ( KDvoid )
{
	// Uncomment to draw attack range circle. For debug purpose only.
    CCPoint		tPos = CCNode::convertToNodeSpace ( this->getPosition ( ) );

    ccDrawColor4B ( 128, 0, 0, 128 );
    ccDrawCircle  ( tPos, m_fAttackRange, KD_DEG2RAD ( -90 ), 30, KD_TRUE );

    CCSprite::draw ( );
}

KDvoid Tower::update ( KDfloat fDelta )
{
    CCAssert ( m_pTheGame != NULL, "You should set the game for Tower" );
    
    if ( m_pChosenEnemy )
    {
        // Finding normalized vector enemy -> tower to calculate angle we should turn tower,
        CCPoint		tEnemyPos	= m_pChosenEnemy->getPosition ( );
        CCPoint		tMyPos		= this->getPosition ( );
        CCPoint		tNormalized = ccpNormalize ( ccpSub ( tEnemyPos, tMyPos ) );
        
        // Turning tower to face the enemy.
        KDfloat		fAngle = CC_RADIANS_TO_DEGREES ( kdAtan2f ( tNormalized.y, -tNormalized.x ) ) + 90;
        this->setRotation ( fAngle );
        
        // Checking if enemy is still in attack range.
        if ( !m_pTheGame->checkCirclesCollide ( this->getPosition ( ), m_fAttackRange, m_pChosenEnemy->getPosition ( ), 1.0f ) )
        {
            this->lostSightOfEnemy ( );
        }
    }
    else
    {
        // Currently we're not attaching anyone. Let's look if there are any enemies in our attack range.
        CCObject*	pObject;
        CCARRAY_FOREACH ( &m_pTheGame->getEnemies ( ), pObject )
        {
            Enemy*		pEnemy = (Enemy*) pObject;            
            if ( m_pTheGame->checkCirclesCollide ( this->getPosition ( ), m_fAttackRange, pEnemy->getPosition ( ), 1.0f ) )
            {
                // Found enemy in range. Attacking.
                this->chosenEnemyForAttack ( pEnemy );
                break;
            }
        }
    }
}

KDvoid Tower::chosenEnemyForAttack ( Enemy* pEnemy )
{
	m_pChosenEnemy = pEnemy;
	this->attackEnemy ( );
	pEnemy->getAttacked ( this );
}

KDvoid Tower::attackEnemy ( KDvoid )
{
	this->schedule ( schedule_selector ( Tower::shootWeapon ), m_fFireRate );
}

KDvoid Tower::shootWeapon ( KDfloat fDelta )
{
    CCAssert ( m_pTheGame != KD_NULL, "You should set the game for Tower" );
    
    CCSprite*	pBullet = CCSprite::create ( "bullet.png" );
    pBullet->setPosition ( this->getPosition ( ) );
    m_pTheGame->addChild ( pBullet );
    
    // Bullet actions: Move from turret to enemy, then make some damage to enemy, and finally remove bullet sprite.
    pBullet->runAction
	(
		CCSequence::create 
		(
			CCMoveTo::create ( 0.1f, m_pChosenEnemy->getPosition ( ) ),
			CCCallFunc::create ( this, callfunc_selector ( Tower::damageEnemy ) ),
			CCCallFuncN::create ( this, callfuncN_selector ( Tower::removeBullet ) ),
			KD_NULL
		) 
	);   
}

KDvoid Tower::damageEnemy ( KDvoid )
{
    if ( m_pChosenEnemy )
	{
        m_pChosenEnemy->getDamaged ( m_nDamage );
	}
}

KDvoid Tower::removeBullet ( CCNode* pBullet )
{
	pBullet->removeFromParentAndCleanup ( KD_TRUE );
}

KDvoid Tower::lostSightOfEnemy ( KDvoid )
{
    if ( m_pChosenEnemy )
    {
        m_pChosenEnemy->gotLostSight ( this );
        m_pChosenEnemy = KD_NULL;
    }
    
    this->unschedule ( schedule_selector ( Tower::shootWeapon ) );
}

KDvoid Tower::setTheGame ( GameLayer* pGame )
{
    m_pTheGame = pGame;
}

KDvoid Tower::targetKilled ( KDvoid )
{
    m_pChosenEnemy = KD_NULL;
    this->unschedule ( schedule_selector ( Tower::shootWeapon ) );
}

