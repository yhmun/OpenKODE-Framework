/* --------------------------------------------------------------------------
 *
 *      File            Mira.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
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
#include "Mira.h"

Mira* Mira::create ( KDvoid )
{
    Mira*	pRet = new Mira ( );
    
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

Mira::Mira ( KDvoid )
{
	m_pFlyingAnimationAction	= KD_NULL;
	m_pHitAnimationAction		= KD_NULL;
	m_pDeathAnimationAction		= KD_NULL;
	m_pCurrentAction			= KD_NULL;
}

Mira::~Mira ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pFlyingAnimationAction );
	CC_SAFE_RELEASE ( m_pHitAnimationAction );
	CC_SAFE_RELEASE ( m_pDeathAnimationAction );
}

KDbool Mira::init ( KDvoid )
{
	if ( !GameObject::init ( ) )
	{
		return KD_FALSE;
	}

	this->setGameObjectType ( kGameObjectTypeMira );
	this->initAnimations ( );
	this->setVulnerable ( KD_TRUE );
	this->changeState ( kCharacterStateFlying );
//	this->setScale ( 0.8f );

    return KD_TRUE;
}


CCRect Mira::adjustedBoundingBox ( KDvoid )
{
    CCRect		tOriginalBoundingBox = this->boundingBox ( );
    return CCRect ( tOriginalBoundingBox.origin.x + ( tOriginalBoundingBox.size.cx / 8 ),
					tOriginalBoundingBox.origin.y + ( tOriginalBoundingBox.size.cy / 4 ),
					tOriginalBoundingBox.size.cx * 0.7f,
					tOriginalBoundingBox.size.cy * 0.4f );
}

KDvoid Mira::checkAndClampSpritePosition ( KDvoid )
{
    
}

KDvoid Mira::changeState ( CharacterState eNewState )
{
    // Make it invisible
//	this->setVisible ( KD_FALSE );
    
    if ( m_pCurrentAction != KD_NULL && !m_pCurrentAction->isDone ( ) )
	{
        this->stopAction ( m_pCurrentAction );
    }
    m_pCurrentAction = KD_NULL;
    this->setCharacterState ( eNewState );
    
    switch ( eNewState )
    {
        case kCharacterStateIdle :
//			this->stopAllActions ( );
            break;

        case kCharacterStateFlying :
        {
            m_pCurrentAction = m_pFlyingAnimationAction;
//			this->setScale ( 0.7f );
        }	break;

        case kCharacterStateHit :
        {
            m_pCurrentAction = m_pHitAnimationAction; 
//			this->setScale ( 0.7f );
            this->changeToInvulnerable ( );
            this->minVelocity ( );
            
            CCFiniteTimeAction*		pDelayAction = CCDelayTime::create ( 3.0f );
            CCFiniteTimeAction*		pBackToFlyingAction = CCCallFunc::create ( this, callfunc_selector ( Mira::changeStateToFlying ) );
            this->runAction ( CCSequence::createWithTwoActions ( pDelayAction, pBackToFlyingAction ) );
            
            CCActionInterval*		pBlinkAction = CCBlink::create ( 5.0f, 15 );
            CCFiniteTimeAction*		pBackToVulnerableAction = CCCallFunc::create ( this, callfunc_selector ( Mira::changeToVulnerable ) );
            this->runAction ( CCSequence::createWithTwoActions ( pBlinkAction, pBackToVulnerableAction ) );
        }	break;

        case kCharacterStateDead:
		{
            m_pCurrentAction = m_pDeathAnimationAction;
//			this->setScale ( 0.7f );
            
            CCActionInterval*	pRotateAction = CCRotateBy::create ( 0.6f, 360 );
            this->runAction ( CCRepeatForever::create ( pRotateAction ) );
		}	break;

        default :
            break;
    }
    
    if ( m_pCurrentAction != KD_NULL )
    {
        this->runAction ( m_pCurrentAction );
//		this->setVisible ( KD_TRUE );
    }
}

KDvoid Mira::changeStateToIdle ( KDvoid )
{
    this->changeState ( kCharacterStateIdle );
}

KDvoid Mira::changeStateToSpawning ( KDvoid )
{
    this->changeState ( kCharacterStateSpawning );
}

KDvoid Mira::changeStateToFlying ( KDvoid )
{
    this->normalVelocity ( );
    this->changeState ( kCharacterStateFlying );
}

KDvoid Mira::changeStateToHit ( KDvoid )
{
    this->changeState ( kCharacterStateHit );
}

KDvoid Mira::changeStateToDead ( KDvoid )
{
    this->changeState ( kCharacterStateDead );
}

KDvoid Mira::changeToVulnerable ( KDvoid )
{
    this->setVulnerable ( KD_TRUE );
}

KDvoid Mira::changeToInvulnerable ( KDvoid )
{
    this->setVulnerable ( KD_FALSE );
}

KDvoid Mira::maxVelocity ( KDvoid )
{
    GameManager::sharedGameManager ( )->maxVelocity ( );
}

KDvoid Mira::normalVelocity ( KDvoid )
{
    GameManager::sharedGameManager ( )->normalVelocity ( );
}

KDvoid Mira::minVelocity ( KDvoid )
{
    GameManager::sharedGameManager ( )->minVelocity ( );
}

KDvoid Mira::initAnimations ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

    // Initialize the flying animation and repeat action    
	CCAnimation*	pFlyingAnimation = CCAnimation::create ( );
	for ( KDint i = 1; i <= 20; i++ )
	{
		pFlyingAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "bee_%d.png", i ) ) );
	}
	pFlyingAnimation->setDelayPerUnit ( 0.1f );        
	this->setFlyingAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pFlyingAnimation ) ) );
	m_pFlyingAnimationAction->retain ( );    

    // Initialize the hit animation and repeat action
	CCAnimation*	pHitAnimation = CCAnimation::create ( );
	for ( KDint i = 21; i <= 24; i++ )
	{
		pHitAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "bee_%d.png", i ) ) );
	}
	pHitAnimation->setDelayPerUnit ( 0.1f );        
	this->setHitAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pHitAnimation ) ) );
	m_pHitAnimationAction->retain ( );
    
    // Initialize the death animation and repeat action    
	CCAnimation*	pDeathAnimation = CCAnimation::create ( );
	for ( KDint i = 21; i <= 24; i++ )
	{
		pDeathAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "bee_%d.png", i ) ) );
	}
	pDeathAnimation->setDelayPerUnit ( 0.1f );
	this->setDeathAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pDeathAnimation ) ) );
	m_pDeathAnimationAction->retain ( );
}

KDvoid Mira::playHitSound ( KDvoid )
{
    
}

KDvoid Mira::playDeadSound ( KDvoid )
{
    
}
