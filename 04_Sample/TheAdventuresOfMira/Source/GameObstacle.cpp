/* --------------------------------------------------------------------------
 *
 *      File            GameObstacle.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/10
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
#include "GameObstacle.h"

GameObstacle* GameObstacle::create ( GameObjectType eGameObjectType )
{
    GameObstacle*	pRet = new GameObstacle ( );
    
	if ( pRet && pRet->init ( eGameObjectType ) )
    {
        pRet->autorelease ( );
        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

GameObstacle::GameObstacle ( KDvoid )
{
	m_pSpawningAnimationAction	= KD_NULL;
	m_pFlyingAnimationAction	= KD_NULL;
	m_pHitAnimationAction		= KD_NULL;
	m_pCurrentAction			= KD_NULL;
}

GameObstacle::~GameObstacle ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSpawningAnimationAction );
	CC_SAFE_RELEASE ( m_pFlyingAnimationAction );
	CC_SAFE_RELEASE ( m_pHitAnimationAction );
}

KDbool GameObstacle::init ( GameObjectType eType )
{
	if ( !GameCharacter::init ( ) )
	{
		return KD_FALSE;
	}

	this->setGameObjectType ( eType );
	
	this->initAnimations ( );
	
	this->changeState ( kCharacterStateIdle );

	this->setAdjustedVelocity ( -1 * GameManager::randomValueBetween ( 1.0f, 6.0f ) );
	    
    return KD_TRUE;
}

KDvoid GameObstacle::update ( KDfloat fDelta )
{
    GameCharacter::update ( fDelta );
}

CCRect GameObstacle::adjustedBoundingBox ( KDvoid )
{
    CCRect		tOriginalBoundingBox = this->boundingBox ( );
    return CCRect ( tOriginalBoundingBox.origin.x + ( tOriginalBoundingBox.size.cx / 8 ),
					tOriginalBoundingBox.origin.y + ( tOriginalBoundingBox.size.cy / 4 ),
					tOriginalBoundingBox.size.cx * 0.7f, 
					tOriginalBoundingBox.size.cy * 0.4f );
}

KDvoid GameObstacle::checkAndClampSpritePosition ( KDvoid )
{
    
}

KDvoid GameObstacle::changeState ( CharacterState eNewState )
{    
    if ( m_pCurrentAction != KD_NULL && !m_pCurrentAction->isDone ( ) )
	{
        this->stopAction ( m_pCurrentAction );
    }
    m_pCurrentAction = KD_NULL;
    this->setCharacterState ( eNewState );
    
    switch ( eNewState )
    {
        case kCharacterStateIdle :
            this->stopAllActions ( );
            this->setRotation ( 0 );
            m_pCurrentAction = KD_NULL;
            break;

        case kCharacterStateSpawning :
            m_pCurrentAction = m_pSpawningAnimationAction;
            break;

        case kCharacterStateFlying :
            m_pCurrentAction = this->m_pFlyingAnimationAction;        
            break;

        case kCharacterStateHit :
            m_pCurrentAction = this->m_pHitAnimationAction;         
            this->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 0.6f, 360 ) ) );
        	break;

        default :
            break;
    }
    
    if ( m_pCurrentAction != KD_NULL )
    {
        this->runAction ( m_pCurrentAction );
    }
}

KDvoid GameObstacle::changeStateToIdle ( KDvoid )
{
    this->changeState ( kCharacterStateIdle );
}

KDvoid GameObstacle::changeStateToSpawning ( KDvoid )
{
    this->changeState ( kCharacterStateSpawning );
}

KDvoid GameObstacle::changeStateToFlying ( KDvoid )
{
    this->changeState ( kCharacterStateFlying );

    CCPoint		tPos = this->getPosition ( );
    this->setPosition ( ccp ( tPos.x + this->getContentSize ( ).cx, tPos.y ) );
}

KDvoid GameObstacle::changeStateToHit ( KDvoid )
{
    this->changeState ( kCharacterStateHit );
}

KDvoid GameObstacle::changeStateToDead ( KDvoid )
{
    this->changeState ( kCharacterStateDead );
}

KDvoid GameObstacle::initAnimations ( KDvoid )
{
	CCSpriteFrameCache*		pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	CCAnimation*	pSpawningAnimation	= CCAnimation::create ( );
	CCAnimation*	pFlyingAnimation	= CCAnimation::create ( );
	CCAnimation*	pHitAnimation		= CCAnimation::create ( );

    switch ( this->getGameObjectType ( ) )
    {
        case kGameObjectTypeObstacleBlueBird :
            // Initialize the spawning animation and repeat action            
            for ( KDint i = 1; i <= 4; i++ )
			{
                pSpawningAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "watchout_%d.png", i ) ) );
            }
            pSpawningAnimation->setDelayPerUnit ( 0.1f );
            this->setSpawningAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pSpawningAnimation ) ) );
            this->m_pSpawningAnimationAction->retain ( );
                    
            // Initialize the flying animation and repeat action
            for ( KDint i = 1; i <= 4; i++ )
			{
                pFlyingAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "bluebird_%d.png", i ) ) );
            }
            pFlyingAnimation->setDelayPerUnit ( 0.1f );
            this->setFlyingAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pFlyingAnimation ) ) );
            this->m_pFlyingAnimationAction->retain ( );
            
            // Initialize the hit animation and repeat action
            pHitAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( "bluebird_5.png" ) );
            this->setHitAnimationAction ( CCAnimate::create (  pHitAnimation ) );
            this->m_pHitAnimationAction->retain ( );
            break;
            
        case kGameObjectTypeObstacleBat :
            // Initialize the spawning animation and repeat action
            for ( KDint i = 1; i <= 4; i++ ) 
			{
                pSpawningAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "watchout_%d.png", i ) ) );
            }
            pSpawningAnimation->setDelayPerUnit ( 0.1f );
            this->setSpawningAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pSpawningAnimation ) ) );
            this->m_pSpawningAnimationAction->retain ( );
            
            // Initialize the flying animation and repeat action
            for ( KDint i = 1; i <= 4; i++ ) 
			{
                pFlyingAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "bat_%d.png", i ) ) );
            }
            pFlyingAnimation->setDelayPerUnit ( 0.1f );
            this->setFlyingAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pFlyingAnimation ) ) );
            this->m_pFlyingAnimationAction->retain ( );
            
            // Initialize the hit animation and repeat action      
            pHitAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( "bat_5.png" ) );
            this->setHitAnimationAction ( CCAnimate::create (  pHitAnimation ) );
            this->m_pHitAnimationAction->retain ( );
       
            break;
            
        case kGameObjectTypeObstacleSquirrel :
            // Initialize the spawning animation and repeat action
            for ( KDint i = 1; i <= 4; i++ )
			{
                pSpawningAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "watchout_%d.png", i ) ) );
            }
            pSpawningAnimation->setDelayPerUnit ( 0.1f );
            this->setSpawningAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pSpawningAnimation ) ) );
            this->m_pSpawningAnimationAction->retain ( );
  
            // Initialize the flying animation and repeat action
            for ( KDint i = 1; i <= 4; i++ )
			{
                pFlyingAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "squirrel_%d.png", i ) ) );
            }
            pFlyingAnimation->setDelayPerUnit ( 0.1f );
            this->setFlyingAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pFlyingAnimation ) ) );
            this->m_pFlyingAnimationAction->retain ( );
            
            // Initialize the hit animation and repeat action
            pHitAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( "squirrel_5.png" ) );
            this->setHitAnimationAction ( CCAnimate::create (  pHitAnimation ) );
            this->m_pHitAnimationAction->retain ( );        
            break;
            
        case kGameObjectTypeObstacleLadyBug :
            // Initialize the spawning animation and repeat action
            for ( KDint i = 1; i <= 4; i++ ) 
			{
                pSpawningAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "watchout_%d.png", i ) ) );
            }
            pSpawningAnimation->setDelayPerUnit ( 0.1f );
            this->setSpawningAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pSpawningAnimation ) ) );
            this->m_pSpawningAnimationAction->retain ( );
            
            // Initialize the flying animation and repeat action
            for ( KDint i = 1; i <= 3; i++ ) 
			{
                pFlyingAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "ladybug_%d.png", i ) ) );
            }
            pFlyingAnimation->setDelayPerUnit ( 0.1f );
            this->setFlyingAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pFlyingAnimation ) ) );
            this->m_pFlyingAnimationAction->retain ( );
                   
            // Initialize the hit animation and repeat action
            pHitAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( "ladybug_4.png" ) );
            this->setHitAnimationAction ( CCAnimate::create (  pHitAnimation ) );
            this->m_pHitAnimationAction->retain ( );
            break;
            
        default :		// kGameObstacleTypeRedBird
            // Initialize the spawning animation and repeat action
            for ( KDint i = 1; i <= 4; i++ ) 
			{
                pSpawningAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "watchout_%d.png", i ) ) );
            }
            pSpawningAnimation->setDelayPerUnit ( 0.1f );
            this->setSpawningAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pSpawningAnimation ) ) );
            this->m_pSpawningAnimationAction->retain ( );
            
            // Initialize the flying animation and repeat action
            for ( KDint i = 1; i <= 4; i++ )
			{
                pFlyingAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( ccszf ( "redbird_%d.png", i ) ) );
            }
            pFlyingAnimation->setDelayPerUnit ( 0.1f );
            this->setFlyingAnimationAction ( CCRepeatForever::create ( CCAnimate::create ( pFlyingAnimation ) ) );
            this->m_pFlyingAnimationAction->retain ( );
            
            // Initialize the hit animation and repeat action
            pHitAnimation->addSpriteFrame ( pFrameCache->spriteFrameByName ( "redbird_5.png" ) );
            this->setHitAnimationAction ( CCAnimate::create (  pHitAnimation ) );
            this->m_pHitAnimationAction->retain ( );
            break;
    }	
}

KDvoid GameObstacle::playSpawningSound ( KDvoid )
{
    
}

KDvoid GameObstacle::playHitSound ( KDvoid )
{
    
}

KDvoid GameObstacle::playDeadSound ( KDvoid )
{
    
}

