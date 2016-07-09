/* --------------------------------------------------------------------------
 *
 *      File            Ship.cpp
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
#include "Ship.h"
#include "Bullet.h"
#include "Explosion.h"

Ship::Ship ( KDvoid )
{
	m_nSpeed				= 220,
	m_fBulletSpeed			= 900,
	m_nHP					= 10,
	m_nBulletTypeValue		= 1,
	m_nBulletPowerValue		= 1,
	m_bThworBombing			= KD_FALSE;
	m_bCanBeAttack			= KD_TRUE;
	m_bIsThrowingBomb		= KD_FALSE;
	m_nMaxBulletPowerValue	= 4;
	m_nHurtColorLife		= 0;
	m_bActive				= KD_TRUE;
	m_fTimeTick				= 0;
}

KDbool Ship::init ( KDvoid )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

    // init life
	CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( PATH_IMAGE_SHIP );
	this->initWithTexture ( pTexture, CCRect ( 0, 0, 60, 38 ) );
    this->setTag ( 3000 );
    this->setPosition ( ccp ( 160, 60 ) );

    // set frame
	CCAnimation*  pAnimation = CCAnimation::create ( );
    pAnimation->addSpriteFrameWithTexture ( pTexture, CCRect (  0, 0, 60, 38 ) );
    pAnimation->addSpriteFrameWithTexture ( pTexture, CCRect ( 60, 0, 60, 38 ) );

    // ship animate
	CCAnimate*    pAction = CCAnimate::create ( pAnimation );
	pAction->setDuration ( 0.1f );
	this->runAction ( CCRepeatForever::create ( pAction ) );
	this->schedule ( schedule_selector ( Ship::onShoot ), 1 / 6.f );

    // revive effect
    m_bCanBeAttack = KD_FALSE;

	CCSprite*  pGhostSprite = CCSprite::create ( );
    pGhostSprite->initWithTexture ( pTexture, CCRect ( 0, 45, 60, 38 ) );
    pGhostSprite->setScale ( 8 );
    pGhostSprite->setPosition ( ccp ( this->getContentSize ( ).cx / 2, 12 ) );
    this->addChild ( pGhostSprite, 3000, 99999 );

	pGhostSprite->runAction ( CCScaleTo::create ( 0.5f, 1, 1 ) );
	this->runAction ( CCSequence::create 
	(
		CCDelayTime	::create ( 0.5f ),
		CCBlink		::create ( 3, 9 ),
		CCCallFuncN	::create ( this, callfuncN_selector ( Ship::onMakeBeAttack ) ),
		KD_NULL
	) );

	return KD_TRUE;
}

KDvoid Ship::destroy ( KDvoid )
{
	CCNode*        pParent = this->getParent ( );
	const CCPoint&  tPoint = this->getPosition ( );

	Global::nLife--;

	Explosion*  pExplosion = Explosion::create ( );
	pExplosion->setPosition ( tPoint );
	pParent->addChild ( pExplosion );
	pParent->removeChild ( this, KD_TRUE );

	Global::playEffect ( PATH_SOUND_SHIP_DESTROY );
}

KDvoid Ship::update ( KDfloat fDelta )
{
	const CCSize&   tWinSize  = CCDirector::sharedDirector ( )->getWinSize ( );
	const CCPoint&  tPosition = this->getPosition ( );

	CCPoint  tNewPos = tPosition;

	if ( Global::bKeys [ eKeyUp ] && tPosition.y <= tWinSize.cy )
	{
        tNewPos.y += fDelta * m_nSpeed;
    }

	if ( Global::bKeys [ eKeyDown ] && tPosition.y >= 0 )
	{
        tNewPos.y -= fDelta * m_nSpeed;
    }

    if ( Global::bKeys [ eKeyLeft ] && tPosition.x >= 0 )
	{
        tNewPos.x -= fDelta * m_nSpeed;
    }

    if ( Global::bKeys [ eKeyRight ] && tPosition.x <= tWinSize.cx ) 
	{
        tNewPos.x += fDelta * m_nSpeed;
    }

    this->setPosition ( tNewPos );

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

KDvoid Ship::hurt ( KDvoid )
{
    if ( m_bCanBeAttack ) 
	{
        m_nHurtColorLife = 2;
        m_nHP--;
        this->setColor ( ccRED );
    }
}

CCRect Ship::collideRect ( KDvoid )
{
	const CCSize&    tSize = this->getContentSize ( );
	const CCPoint&  tPoint = this->getPosition ( );

	return CCRect ( tPoint.x - tSize.cx / 2, tPoint.y - tSize.cy / 2, tSize.cx, tSize.cy / 2 );
}

KDbool Ship::active ( KDvoid )
{
	return m_bActive;
}

KDvoid Ship::onMakeBeAttack ( CCNode* pSender )
{
	m_bCanBeAttack = KD_TRUE;
	this->setVisible ( KD_TRUE );
	this->removeChildByTag ( 99999, KD_TRUE );
}

KDvoid Ship::onShoot ( KDfloat fDelta )
{
	CCNode*  pParent = this->getParent ( );
	CCPoint  tPoint  = this->getPosition ( );
	CCSize   tSize   = this->getContentSize ( );
	KDfloat  fOffset = 13;
	
	for ( KDint i = 0; i < 2; i++ )
	{
		Bullet*  pBullet = Bullet::create ( m_fBulletSpeed, "W1.png", eAttackNormal );
		pBullet->setPosition ( ccp ( tPoint.x + fOffset * ( i == 0 ? 1 : -1 ), tPoint.y + 3 + tSize.cy * 0.3f ) );
		pParent->addChild ( pBullet, 3000, eTagShipBullet );
		Global::pSBulletContainer->addObject ( pBullet );
	}
}