/* --------------------------------------------------------------------------
 *
 *      File            ScrMainGame.cpp
 *      Description     Main Menu Screen
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
#include "ScrMainGame.h"
#include "GameState.h"

static const CCPoint	l_aPoints [ ] =
{
	ccp ( 20, 20 ),		// 첫번째 생명 표시용 
	ccp ( 80, 20 ),		// 두번째 생명 표시용
};

#define LIMIT_BOUNDRY	30

CScrMainGame::CScrMainGame ( KDvoid )
{
	m_pTexture = KD_NULL;
	m_pPlayer  = KD_NULL;

	m_pEnemies = new CCArray ( 24 );
	m_pLifes   = new CCArray (  2 );

	m_pPlayerMissiles = new CCArray ( );
	m_pEnemyMissiles  = new CCArray ( );

	m_bDirection = KD_TRUE;
	m_nTouchID   = -1;

	m_fPlayerMissileDelay = 0;
	m_fEnemyMissileDelay  = 0;
}

CScrMainGame::~CScrMainGame ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pEnemies );
	CC_SAFE_RELEASE ( m_pLifes );
	CC_SAFE_RELEASE ( m_pPlayerMissiles );
	CC_SAFE_RELEASE ( m_pEnemyMissiles );
}

KDbool CScrMainGame::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&		tLyrSize = this->getContentSize ( );

//	this->addChild ( CCLayerColor::create ( ccc4 ( 255, 0, 0, 128 ), this->getContentSize ( ) ) );

    // 텍스처 로딩
    m_pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/galagasheet.png" );
    
    // 아군
    m_pPlayer = createPlayer ( ccp ( tLyrSize.cx / 2, 60 ) );
    this->addChild ( m_pPlayer );
	m_pPlayer->setVisible ( KD_FALSE );

	// 아군 생명 표시용
	for ( KDint i = 0; i < 2; i++ )
	{
		 CCSprite*	pLife = createPlayer ( l_aPoints [ i ] );
		 this->addChild ( pLife );

		 // 아군 생명 관리자에 추가
		 m_pLifes->addObject ( pLife );
	}

    // 4기씩 3줄로 적기 생성
    for ( KDint i = 0; i < 4; i++ ) 
	{
        for ( KDint j = 0; j < 3; j++ ) 
		{
            CCSprite*	pEnemy = createEnemy ( ccp ( tLyrSize.cx / 2 - 225 + i * 50, tLyrSize.cy - 40 - j * 50 ), eEnemy1 );
            this->addChild ( pEnemy );
            
            // 생성한 적기를 관리하기 위해 Array에 추가
            m_pEnemies->addObject ( pEnemy );
        }
    }

    // 다른 적기 추가 부분
    for( KDint i = 0; i < 4; i++ ) 
	{
        for( KDint j = 0; j < 3; j++ ) 
		{       
            CCSprite*	pEnemy = createEnemy ( ccp ( tLyrSize.cx / 2 + 75 + i * 50, tLyrSize.cy - 40 - j * 50 ), eEnemy2 );
            this->addChild ( pEnemy );
            
            // 생성한 적기를 관리하기 위해 Array에 추가
            m_pEnemies->addObject ( pEnemy );
        }
    }

	// 스케줄러 시작
	this->schedule ( schedule_selector ( CScrMainGame::onGameLogic ), 0.05f );

	// 터치를 받을수 있게
	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CScrMainGame::onEnterTransitionDidFinish ( KDvoid )
{
	CCLayer::onEnterTransitionDidFinish ( );

	m_pPlayer->setVisible ( KD_TRUE );
}	

KDvoid CScrMainGame::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	// 아무 터치 받아 오는법
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	// 멀티 터치 탐색용
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ ) 
	{
		pTouch = (CCTouch*) ( *it );

		// 확보된 터치가 없으면, 이 터치 아이디를 기억한다.
		if ( m_nTouchID == -1 )
		{
			m_nTouchID = pTouch->getID ( );

			// 아군기 움직임 시작
			this->schedule ( schedule_selector ( CScrMainGame::onMovingPlayer ), 0.01f );

			// 화면의 왼쪽을 눌렀는지 오른쪽을 눌렀는지에 따른 판정
			const CCSize&	tLyrSize = this->getContentSize ( );
			CCPoint			tLocation = pTouch->getLocation ( );
			tLocation = this->convertToNodeSpace ( tLocation );

			CCLOG ( "Pos %.2f, %.2f", tLocation.x, tLocation.y );
			m_bDirection = ( tLocation.x < tLyrSize.cx / 2 ) ? KD_FALSE : KD_TRUE;
		
			break;
		}
	}
}

KDvoid CScrMainGame::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	// 아무 터치 받아 오는법
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	// 멀티 터치 탐색용
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ ) 
	{
		pTouch = (CCTouch*) ( *it );

		// 확보된 터치가 없으면, 이 터치 아이디를 기억한다.
		if ( pTouch->getID ( ) == m_nTouchID  )
		{
			// 화면의 왼쪽을 눌렀는지 오른쪽을 눌렀는지에 따른 판정
			const CCSize&	tLyrSize = this->getContentSize ( );
			CCPoint			tLocation = pTouch->getLocation ( );
			tLocation = this->convertToNodeSpace ( tLocation );

			CCLOG ( "Pos %.2f, %.2f", tLocation.x, tLocation.y );
			m_bDirection = ( tLocation.x < tLyrSize.cx / 2 ) ? KD_FALSE : KD_TRUE;
		
			break;
		}
	}
}

KDvoid CScrMainGame::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch  = KD_NULL;

	// 멀티 터치 탐색용
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ ) 
	{
		pTouch = (CCTouch*) ( *it );

		const CCSize&	tLyrSize = this->getContentSize ( );
		CCPoint			tLocation = pTouch->getLocation ( );
		tLocation = this->convertToNodeSpace ( tLocation );
		
		if ( tLocation.x < 64 && tLocation.y > tLyrSize.cy - 64 )
		{
			GSTAT->setScene ( eScrMainMenu, eTranFade );
		}

		// 터치들 중에 아이디가 현재 기억된 아이디와 같은게 있으면, 
		if ( pTouch->getID ( ) == m_nTouchID )
		{
			m_nTouchID = -1;

			// 아군기 움직임 풀어줌
			this->unschedule ( schedule_selector ( CScrMainGame::onMovingPlayer ) );
			break;
		}
	}
}

CCSprite* CScrMainGame::createPlayer ( const CCPoint& tPoint )
{
    CCSprite*	pAirplane = CCSprite::createWithTexture ( m_pTexture, CCRectMake ( 184, 55, 15, 17 ) );
    
	pAirplane->setScale ( 2.0f );
    pAirplane->setPosition ( tPoint );    

    return pAirplane;
}

CCSprite* CScrMainGame::createEnemy ( const CCPoint& tPoint, EType eType )
{
	// 움직이는 애니메이션 프레임 명시 ( 두장면 )
	CCArray*	 pFrames = CCArray::createWithCapacity ( 2 );

	// 적기의 종류가 다를 경우 이미지 프레임을 바꿔준다.
	switch ( eType )
	{
		case eEnemy1 :
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 161, 103, 15, 16 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 185, 103, 15, 16 ) ) );
			break;

		case eEnemy2 :
		default		 :
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 161, 127, 15, 16 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 185, 127, 15, 16 ) ) );
			break;
	}

    CCSprite*	pEnemy = CCSprite::createWithSpriteFrame ( (CCSpriteFrame*) pFrames->objectAtIndex ( 0 ) );
    pEnemy->setScale ( 2 );
    pEnemy->setPosition( tPoint );

	// 움직이는 그림 표현 ( 애니메이션 )
    CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pFrames, 0.4f );
    pEnemy->runAction	
	( 
		CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) 
	);

	return pEnemy;
}

CCSprite* CScrMainGame::createExplosion ( const CCPoint& tPoint, EType eType )
{
    CCArray*		pFrames = CCArray::createWithCapacity ( 5 );
	CCSprite*		pExplosion = KD_NULL;
    
	switch ( eType )
	{
		case ePlayer :
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 208,  47, 32, 32 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 248,  47, 32, 32 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 288,  47, 32, 32 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 283, 193, 27, 28 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 328,  47, 32, 32 ) ) );
			break;

		case eEnemy1 :
		case eEnemy2 :
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 211, 202,  7,  8 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 234, 200, 12, 13 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 211, 202,  7,  8 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 234, 200, 12, 13 ) ) );
			pFrames->addObject ( CCSpriteFrame::createWithTexture ( m_pTexture, CCRectMake ( 211, 202,  7,  8 ) ) );
			break;
	}
    
    pExplosion = CCSprite::createWithSpriteFrame ( (CCSpriteFrame*) pFrames->objectAtIndex ( 0 ) );
    pExplosion->setScale ( 2 );
    pExplosion->setPosition ( tPoint );

    // 폭발 애니메이션 후 폭발 대상 객체 제거
    // 폭발 애니메이션 액션 생성
	CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pFrames, 0.4f );
    pExplosion->runAction 
	(
		CCSequence::create 
		(
			CCAnimate   ::create ( pAnimation ),
			CCRemoveSelf::create ( ),
			KD_NULL 
		) 
	);

    return pExplosion;
}

CCSprite* CScrMainGame::createMissile ( const CCPoint& tPoint, EType eType )
{
	const CCRect	aRects [ ] =
	{
		CCRect ( 374,  51, 3, 8 ),		// 아군 미사일
		CCRect ( 366, 195, 3, 8 )		// 적군 미사일
	};

	CCSprite*	pMissile = CCSprite::createWithTexture ( m_pTexture, aRects [ eType ] );
    pMissile->setScale ( 2 );
    pMissile->setPosition ( tPoint );
	this->addChild ( pMissile, 0, eType );

	// 총알을 화면 위로 쏘고, 영역을 벗어나면 총알 제거 콜백 함수 호출
	const CCSize&	tLyrSize  = this->getContentSize ( );
	const CCPoint	tPosition = pMissile->getPosition ( );

	KDfloat		yPos = 0;

	// 발사한 총알을 관리하기 위해 배열에 추가
	switch ( eType )
	{
		case ePlayer :	m_pPlayerMissiles->addObject ( pMissile ); yPos = tLyrSize.cy + 50;	break;

		case eEnemy1 :
		case eEnemy2 :	m_pEnemyMissiles ->addObject ( pMissile ); yPos = -20;				break;
	}

	pMissile->runAction 
	(
		CCSequence::create 
		(
			CCMoveTo   ::create ( 2, ccp ( tPosition.x, yPos ) ),
			CCCallFuncN::create ( this, callfuncN_selector ( CScrMainGame::removeMissile ) ),
			KD_NULL
		) 
	);

	return pMissile;
}

KDvoid CScrMainGame::removeMissile ( CCNode* pNode )
{
	switch ( pNode->getTag ( ) )
	{
		case ePlayer :	m_pPlayerMissiles->removeObject ( pNode );	break;
		case eEnemy1 :	
		case eEnemy2 :	m_pEnemyMissiles ->removeObject ( pNode );	break;
	}

	// 잘못된 예
//	pNode->removeFromParent ( );

	// 잘된 예
	this->removeChild ( pNode );
}

KDvoid CScrMainGame::onGameLogic ( KDfloat fDelta )
{
	CCObject*	pObject1;
	CCObject*	pObject2;


	// 아군 총알 발사 부분 
    // 총알 준비 시간이 1초가 넘으면
    if( m_fPlayerMissileDelay > 1.0f ) 
	{
        // 아군기의 위치를 기준으로 총알 생성
		if ( m_pPlayer->isVisible ( ) )
		{
			createMissile ( m_pPlayer->getPosition ( ), ePlayer );
		}

        m_fPlayerMissileDelay = 0;
    }
	// 총알 준비 시간을 0.2초 증가
	m_fPlayerMissileDelay += fDelta;			

	// 적기가 총알을 발사하는 루틴
    // 총알 준비 시간이 0.5초가 넘어가면
	KDuint		uCount = m_pEnemies->count ( );
    if ( uCount > 0 && m_fEnemyMissileDelay > 0.5f )
	{
		KDuint		uIndex = (KDuint) CCRANDOM_BETWEEN ( 0, uCount - 1 );
        CCSprite*	pEnemy = (CCSprite*) m_pEnemies->objectAtIndex ( uIndex );
    
		// 총알 발사 시 적기 회전
        pEnemy->runAction		
		(
			CCRotateBy::create ( 1, 360 ) 
		);	
        
        this->createMissile ( pEnemy->getPosition ( ), eEnemy1 );

        // 총알 준비 시간 초기화
        m_fEnemyMissileDelay = 0;
    }
	
	m_fEnemyMissileDelay += fDelta;


	// 아군 총알 제거 목록
	CCArray*	pPlayerMissilesToRemove = CCArray::create ( );

	// 아군 총알이 적기에 맞았는지 검사	
	CCARRAY_FOREACH ( m_pPlayerMissiles, pObject1 )
	{
		CCSprite*		pPlayerMissile = (CCSprite*) pObject1;

		// 아군 총알에 맞은 적기 판정 및 목록 추출 
		CCArray*		pEnemiesToRemove = CCArray::create ( );
		CCARRAY_FOREACH ( m_pEnemies, pObject2 )
		{
			CCSprite*	pEnemy = (CCSprite*) pObject2;

			if ( pPlayerMissile->boundingBoxToWorld ( ).intersectsRect ( pEnemy->boundingBoxToWorld ( ) ) )
			{
				pEnemiesToRemove->addObject ( pEnemy );				

//				break;	// 브레이크 있으면, 원샷에 한기씩 맞출수 있음
			}
		}

		// 총알 맞은 적기들을 실제 제거
		CCARRAY_FOREACH ( pEnemiesToRemove, pObject2 )
		{
			CCSprite*	pEnemy = (CCSprite*) pObject2;

			m_pEnemies->removeObject ( pEnemy );
			this->removeChild ( pEnemy );

			// 적기 폭발 애니메이션 생성
			this->addChild ( createExplosion ( pEnemy->getPosition ( ), eEnemy1 ) );
		}

		// 맞춘 적기가 있으면 아군 총알 제거 목록에 등록
		if ( pEnemiesToRemove->count ( ) > 0 )
		{
			pPlayerMissilesToRemove->addObject ( pPlayerMissile );
		}
	}

	// 아군 총알 제거
	CCARRAY_FOREACH ( pPlayerMissilesToRemove, pObject1 )
	{
		CCSprite*		pPlayerMissile = (CCSprite*) pObject1;

		this->removeChild ( pPlayerMissile );
		m_pPlayerMissiles->removeObject ( pPlayerMissile );
	}

	// 적군 총알 제거 목록
	CCArray*	pEnemyMissilesToRemove = CCArray::create ( );

	// 적군 총알이 아군기에 맞았는지 검사	
	CCARRAY_FOREACH ( m_pEnemyMissiles, pObject1 )
	{
		CCSprite*		pEnemyMissile = (CCSprite*) pObject1;

		// 적군 총알이 아군기에 맞았는지 판정	
		if ( pEnemyMissile->boundingBoxToWorld ( ).intersectsRect ( m_pPlayer->boundingBoxToWorld ( ) ) )
		{			
			pEnemyMissilesToRemove->addObject ( pEnemyMissile );

			if ( m_pPlayer->isVisible ( ) )
			{
				m_pPlayer->setVisible ( KD_FALSE );

				// 아군기 폭발 애니메이션
				this->addChild ( createExplosion ( m_pPlayer->getPosition ( ), ePlayer ) );

				// 아군기 재 등장			
				this->scheduleOnce ( schedule_selector ( CScrMainGame::onRebornPlayer ), 2.0f );
			}
		}
	}

	// 적군 총알 제거
	CCARRAY_FOREACH ( pEnemyMissilesToRemove, pObject1 )
	{
		CCSprite*		pEnemyMissile = (CCSprite*) pObject1;

		this->removeChild ( pEnemyMissile );
		m_pEnemyMissiles->removeObject ( pEnemyMissile );
	}

	// 적군을 다 맞추었을때 처리
	if ( m_pEnemies->count ( ) == 0 )
	{
		// TODO : 폭발 애니메이션을 다 처리하게끔 딜레이를 준후 화면 전환 또는 로직 전환해야 한다.
	}
}

KDvoid CScrMainGame::onRebornPlayer ( KDfloat fDelta )
{
	// 생명이 남아 있는지 판별
	if ( m_pLifes->count ( ) > 0 )
	{
		m_pPlayer->setVisible ( KD_TRUE );

		// 생명 하나를 줄인다.
		this->removeChild ( (CCSprite*) m_pLifes->lastObject ( ) );
		m_pLifes->removeLastObject ( );
	}
	else
	{
		// 게임 종료 처리	
		GSTAT->setScene ( eScrMainMenu, eTranSlideIn );
	}
}

KDvoid CScrMainGame::onMovingPlayer  ( KDfloat fDelta )
{
	if ( !m_pPlayer->isVisible ( ) )
	{
		return;
	}

	CCPoint		tPoint = m_pPlayer->getPosition ( );
	CCPoint		tVelocity = ccp ( m_bDirection ? 3 : -3, 0 );

	const CCSize&	tLyrSize = this->getContentSize ( );

	tPoint = ccpAdd ( tPoint, tVelocity );

	// 화면 움직임 제한 범위 설정
	if ( tPoint.x < LIMIT_BOUNDRY )
	{
		tPoint.x = LIMIT_BOUNDRY;
	}

	if ( tPoint.x > ( tLyrSize.cx - LIMIT_BOUNDRY ) )
	{
		tPoint.x = tLyrSize.cx - LIMIT_BOUNDRY;
	}

	m_pPlayer->setPosition ( tPoint );
}
