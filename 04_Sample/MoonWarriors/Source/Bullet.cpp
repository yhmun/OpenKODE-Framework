/* --------------------------------------------------------------------------
 *
 *      File            Bullet.cpp
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
#include "Bullet.h"
#include "GameLayer.h"

Bullet* Bullet::create ( KDfloat fSpeed, const KDchar* szType, EAttackMode eMode )
{
	Bullet*  pRet = new Bullet ( );

	if ( pRet && pRet->init ( fSpeed, szType, eMode ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Bullet::Bullet ( KDvoid )
{
	m_bActive		= KD_TRUE;
	m_fVolocityX	= 0;
	m_fVolocityY	= 200;
	m_nPower		= 1;
	m_nHP			= 1;
	m_eAttackMode	= eAttackNormal;
}

KDbool Bullet::init ( KDfloat fSpeed, const KDchar* szType, EAttackMode eMode )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_fVolocityY	= -fSpeed;
	m_eAttackMode	= eMode;
	
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( PATH_PLIST_BULLET );
     
	this->initWithSpriteFrameName ( szType );

	return KD_TRUE;
}

KDvoid Bullet::update ( KDfloat fDelta )
{
	CCPoint  tPoint = this->getPosition ( );

    tPoint.x -= m_fVolocityX * fDelta;
    tPoint.y -= m_fVolocityY * fDelta;

    this->setPosition ( tPoint );

	if ( m_nHP <= 0 )
	{
        m_bActive = KD_FALSE;
    }
}

KDvoid Bullet::destroy ( KDvoid )
{
	GameLayer*  pParent = (GameLayer*) this->getParent ( );
	CCSprite*  pExplode = CCSprite::create ( PATH_IMAGE_HIT );

	pExplode->setPosition ( this->getPosition ( ) );
	pExplode->setRotation ( CCRANDOM_0_1 ( ) * 360.f );
	pExplode->setScale ( 0.75f );
	pParent->addChild ( pExplode, 9999 );

	Global::pEBulletContainer->removeObject ( this );
	Global::pSBulletContainer->removeObject ( this );
	pParent->removeChild ( this, KD_TRUE );

	pExplode->runAction ( CCScaleBy::create ( 0.3f, 2 ) );
	pExplode->runAction ( CCSequence::createWithTwoActions 
	(
		CCFadeOut::create ( 0.3f ),
		CCCallFunc::create ( pExplode, callfunc_selector ( CCNode::removeFromParentAndCleanup ) )
	) );
}

KDvoid Bullet::hurt ( KDvoid )
{
	m_nHP--;
}

CCRect Bullet::collideRect ( KDvoid )
{
	const CCPoint&  tPoint = this->getPosition ( );

	return CCRect ( tPoint.x - 3, tPoint.y - 3, 6, 6 );
}

KDbool Bullet::active ( KDvoid )
{
	return m_bActive;
}