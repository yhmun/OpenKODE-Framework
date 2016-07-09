/* --------------------------------------------------------------------------
 *
 *      File            Enemy.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors   
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "Bullet.h"
#include "Explosion.h"
#include "Effect.h"

struct EnemyType
{
	const KDchar*	szTextureName;
	const KDchar*	szBulletName;
	KDint			nHP;
	EMoveType		eMoveType;
	EAttackMode		eAttackMode;
	KDint			nScore;
};

static struct EnemyType		l_aEnemyTable [] =
{
	{ "E0.png", "W2.png",  2, eMoveVertical	 , eAttackNormal	 ,  20 },
	{ "E1.png", "W2.png",  4, eMoveAttack	 , eAttackNormal	 ,  40 },
	{ "E2.png", "W2.png",  6, eMoveHorizontal, eAttackTsuihikidan,  60 },
	{ "E3.png", "W2.png",  8, eMoveOverlap	 , eAttackNormal	 ,  80 },
	{ "E4.png", "W2.png", 15, eMoveHorizontal, eAttackTsuihikidan, 150 },
	{ "E5.png", "W2.png", 20, eMoveHorizontal, eAttackNormal	 , 200 },
};

Enemy* Enemy::create ( KDuint uType )
{
	Enemy*  pRet = new Enemy ( );

	if ( pRet && pRet->init ( uType ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Enemy::Enemy ( KDvoid )
{
	m_bActive			= KD_TRUE;
	m_nSpeed			= 200;
	m_fBulletSpeed		= -200;
	m_nHP				= 15;
	m_nBulletPowerValue	= 1;
	m_eMoveType			= eMoveAttack;
	m_nScoreValue		= 200;
	m_fDelayTime		= 1 + 1.2f * CCRANDOM_0_1 ( );
	m_eAttackMode		= eAttackNormal;
	m_nHurtColorLife	= 0;
	m_fTimeTick			= 0;
}

KDbool Enemy::init ( KDuint uType )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}
	
	EnemyType*  pEnemyType = &l_aEnemyTable [ uType ];

	m_nHP			= pEnemyType->nHP;
	m_eMoveType		= pEnemyType->eMoveType;
	m_nScoreValue	= pEnemyType->nScore;
	m_eAttackMode	= pEnemyType->eAttackMode;

	this->initWithSpriteFrameName ( pEnemyType->szTextureName );
	this->schedule ( schedule_selector ( Enemy::onShoot ), m_fDelayTime );

	return KD_TRUE;
}

KDvoid Enemy::destroy ( KDvoid )
{
	CCNode*        pParent = this->getParent ( );
	const CCPoint&  tPoint = this->getPosition ( );

	if ( !m_bActive )
	{
		Global::nScore += m_nScoreValue;
		Global::playEffect ( PATH_SOUND_EXPLODE );

		Explosion*  pExplosion = Explosion::create ( );
		pExplosion->setPosition ( tPoint );
		pParent->addChild ( pExplosion );
    
		spark ( tPoint, pParent, 1.2f, 0.7f );
	}

	Global::pEnemyContainer->removeObject ( this );
    pParent->removeChild ( this, KD_TRUE );
}

KDvoid Enemy::update ( KDfloat fDelta )
{
    if ( m_nHP <= 0 )
	{
        m_bActive = KD_FALSE;
    }

    m_fTimeTick += fDelta;
    if ( m_fTimeTick > 0.1f ) 
	{
        m_fTimeTick = 0;
        if ( m_nHurtColorLife > 0 )
		{
            m_nHurtColorLife--;
        }
        if ( m_nHurtColorLife == 1 ) 
		{
            this->setColor ( ccWHITE );
        }
    }
}

KDvoid Enemy::hurt ( KDvoid )
{
    m_nHurtColorLife = 2;
    m_nHP--;
    this->setColor ( ccRED );
}

CCRect Enemy::collideRect ( KDvoid )
{
	const CCSize&    tSize = this->getContentSize ( );
	const CCPoint&  tPoint = this->getPosition ( );

	return CCRect ( tPoint.x - tSize.cx / 2, tPoint.y - tSize.cy / 4, tSize.cx, tSize.cy / 2 );
}

KDbool Enemy::active ( KDvoid )
{
	return m_bActive;
}

KDvoid Enemy::sharedEnemy ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( PATH_PLIST_ENEMY, PATH_IMAGE_ENEMY );
}

EMoveType Enemy::getMoveType ( KDvoid )
{
	return m_eMoveType;
}

KDvoid Enemy::onShoot ( KDfloat fDelta )
{
	Bullet*        pBullet = Bullet::create ( m_fBulletSpeed, "W2.png", m_eAttackMode );
	const CCPoint&  tPoint = this->getPosition ( );
	const CCSize&    tSize = this->getContentSize ( );

	pBullet->setPosition ( ccp ( tPoint.x, tPoint.y - tSize.cy * 0.2f ) );
	this->getParent ( )->addChild ( pBullet, 3000, eTagEnemyBullet );
	Global::pEBulletContainer->addObject ( pBullet );	
}