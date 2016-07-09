/* --------------------------------------------------------------------------
 *
 *      File            EnemySprite.cpp
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
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
#include "EnemySprite.h"
#include "GameLayer.h"

EnemySprite::EnemySprite ( KDvoid )
{
	m_pWalkAnimate		= KD_NULL;
	m_pAttackAnimate	= KD_NULL;
}

EnemySprite::~EnemySprite ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pWalkAnimate );
	CC_SAFE_RELEASE ( m_pAttackAnimate );
}

EnemySprite* EnemySprite::create ( const KDchar* szFrameName, const CCPoint& tPos, KDfloat fSpeed, GameLayer* pLayer )
{
	EnemySprite*	pRet = new EnemySprite ( );

	if ( pRet && pRet->initWithSpriteFrameName ( szFrameName, tPos, fSpeed, pLayer ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool EnemySprite::initWithSpriteFrameName ( const KDchar* szFrameName, const CCPoint& tPos, KDfloat fSpeed, GameLayer* pLayer )
{
	if ( !CCSprite::initWithSpriteFrameName ( szFrameName ) )
	{
		return KD_FALSE;
	}

	CCAnimate*		pAnimate = KD_NULL;

	m_pGameLayer	= pLayer;
	m_fWalkingSpeed = fSpeed;

	// GameLayer에서 만들어 놓은 CCAnimation을 이용하여 CCAnimate 객체를 만듭니다.
	pAnimate = CCAnimate::create ( pLayer->getEnemyWalkAnmation ( ) );
	this->setWalkAnimate ( pAnimate );

	pAnimate = CCAnimate::create ( pLayer->getEnemyAttackAnimation ( ) );
	this->setAttackAnimate ( pAnimate );

	this->setAnchorPoint ( ccp ( 0.5, 0 ) );
	this->setPosition ( tPos );

    return KD_TRUE;
}

KDvoid EnemySprite::onEnter ( KDvoid )
{
	CCSprite::onEnter ( );

	this->startWalking ( );
}

CCAnimate* EnemySprite::getWalkAnimate ( KDvoid )
{
	return m_pWalkAnimate;
}

KDvoid EnemySprite::setWalkAnimate ( CCAnimate* pAnimate )
{
	CC_SAFE_RELEASE ( m_pWalkAnimate );
	m_pWalkAnimate = pAnimate;
	CC_SAFE_RETAIN ( m_pWalkAnimate );
}

CCAnimate* EnemySprite::getAttackAnimate ( KDvoid )
{
	return m_pAttackAnimate;
}

KDvoid EnemySprite::setAttackAnimate ( CCAnimate* pAnimate )
{
	CC_SAFE_RELEASE ( m_pAttackAnimate );
	m_pAttackAnimate = pAnimate;
	CC_SAFE_RETAIN ( m_pAttackAnimate );
}

KDvoid EnemySprite::startWalking ( KDvoid ) 
{
	this->schedule ( schedule_selector ( EnemySprite::walking ) );

	this->runAction ( CCRepeatForever::create ( m_pWalkAnimate ) );
}

KDvoid EnemySprite::stopWalking ( KDvoid )
{
	this->unschedule ( schedule_selector ( EnemySprite::walking ) );

	this->stopAllActions ( );
}

KDvoid EnemySprite::walking ( KDfloat fDelta )
{
	const CCPoint&	tPrincePos = m_pGameLayer->getPrinceSprite ( )->getPosition ( );
	const CCPoint&	tEnemyPos  = this->getPosition ( );

    // 주인공에게 가까이 왔으면 (40픽셀 이내) 움직임을 멈춥니다.
    if ( kdFabsf ( tEnemyPos.x - tPrincePos.x ) <= 40 )
	{
        // 걷는 동작을 멈춥니다.
        this->stopWalking ( );
        
        // 0.5에서 1.4초 사이의 랜덤 시간만큼 기다렸다가 공격합니다.
        // (레벨이 올라갈수록 지체하는 시간을 줄이면 그만큼 더 강한 적이 되겠죠?)
        KDfloat		fDelayTime = ( ( kdRand ( ) % 10 ) + 5 ) / 10.0f;
        
		this->runAction
		(
			CCSequence::create 
			(
				CCDelayTime::create ( fDelayTime ),
				CCCallFunc ::create ( this, callfunc_selector ( EnemySprite::attackPrince ) ),
				KD_NULL
			)
		);
   }
    
    // 주인공과 가깝지 않다면 계속 이동합니다.
    KDfloat		fTmpWalkingSpeed = m_fWalkingSpeed;
    
    // 주인공이 왼쪽에 있는지 오른쪽에 있는지 알아봅니다.
    if ( tPrincePos.x > tEnemyPos.x )
	{
        this->setFlipX ( KD_TRUE );
    }
	else 
	{
		this->setFlipX ( KD_FALSE );
        fTmpWalkingSpeed *= -1.0;
    }
    
    // 새로운 위치를 업데이트합니다.
	this->setPosition ( ccp ( tEnemyPos.x + fTmpWalkingSpeed, tEnemyPos.y ) );
}

KDvoid EnemySprite::attackPrince ( KDvoid )
{
	this->runAction 
	(
		CCSequence::create 
		(
			m_pAttackAnimate,
			CCCallFunc::create ( this, callfunc_selector ( EnemySprite::attackCompleteHandler ) ),
			KD_NULL
		)
	);
}

KDvoid EnemySprite::attackCompleteHandler ( KDvoid )
{
    // 공격하는 애니메이션을 시작합니다. 다음 장에서 실제 애니메이션을 구현해보겠습니다.
    // 주인공과의 거리를 다시 계산합니다. 이렇게 다시 계산하는 이유는 위에서 잠깐 지체하는 동안 주인공이 이동했을 수도 있게 때문입니다.
    if ( kdFabsf ( m_pGameLayer->getPrinceSprite ( )->getPosition ( ).x - this->getPosition ( ).x ) <= 60 )
	{
        FallDirection		eFallDirection = kFallLeft;
		if ( this->isFlipX ( ) == KD_TRUE )
		{
            eFallDirection = kFallRight;
		}

		m_pGameLayer->princeAttacked ( eFallDirection );      
    } 	
	else
	{
		CCLOG ( "Enemy Attack Failed." );
    }

	this->startWalking ( );   
}


// 이 메서드가 바로 주인공이 이동할 때 적에게 뒤쳐지도록 만드는 부분입니다.
// 주인공의 이동 방향과 적의 이동 방향에 맞쳐 x 좌표의 값을 변경합니다.
KDvoid EnemySprite::adjustPosition ( KDvoid )
{
	const CCPoint&	tPrincePos  = m_pGameLayer->getPrinceSprite ( )->getPosition ( );
	KDbool			bPrinceFlip = m_pGameLayer->getPrinceSprite ( )->isFlipX ( );

	const CCPoint&	tEnemyPos   = this->getPosition ( );	
	KDbool			bEnemyFlip  = this->isFlipX ( );

	CCPoint			tNewPos = tEnemyPos;

    if ( tPrincePos.x > tEnemyPos.x && bPrinceFlip == KD_TRUE )
	{
		tNewPos.x += 1;
    }
	else if ( tPrincePos.x < tEnemyPos.x && bPrinceFlip == KD_FALSE ) 
	{
		tNewPos.x -= 1;
    } 
	else if ( bPrinceFlip == KD_TRUE && bEnemyFlip == KD_FALSE )
	{
        tNewPos.x += 1;
    }
	else if ( bPrinceFlip == KD_FALSE && bEnemyFlip == KD_TRUE ) 
	{
        tNewPos.x -= 1;
    }

	this->setPosition ( tNewPos );
}
