/* --------------------------------------------------------------------------
 *
 *      File            Board.cpp
 *      Created By      Project B team
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2013 XMsoft. All rights reserved.
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
#include "Board.h"
#include "Block.h"
#include "Tetromino.h"
#include "GameLayer.h"
#include "SoundManager.h"

Board::Board ( KDvoid )
{
	kdMemset ( m_aBoards, 0, sizeof ( m_aBoards ) );

	m_pTetromino	 = KD_NULL;
	m_nNextTetroType = -1;
	m_nGameSpeed	 = 1;
}

KDbool Board::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) ) 
	{
		return KD_FALSE;
	}

	return KD_TRUE;
}

KDvoid Board::onEnter ( KDvoid )
{
	CCLayer::onEnter ( );

    CCLOG ( "%d", kdRand ( ) );
    
//	kdSrand ( kdTime ( 0 ) );

	// 첫번째 블럭생성
	this->createTetromino ( );

	// 스케줄러 시작
	this->schedule ( schedule_selector ( Board::goTetromino ), m_nGameSpeed );
}

// 게임속도 설정
KDvoid Board::setGameSpeed ( KDfloat fSec )
{
	m_nGameSpeed = fSec;

	// 스케쥴러 재가동
	this->unschedule ( schedule_selector ( Board::goTetromino ) );
	this->schedule   ( schedule_selector ( Board::goTetromino ), fSec );

	CCLOG ( "setGameSpeed: %f", fSec );
}

// 랜덤 블럭타입을 반환
KDint Board::newTetroType ( KDvoid )
{
	KDint		nBlockTypeCnt;
	GameLayer*	pGameLayer = (GameLayer*) this->getParent ( );

	// 레벨별로 노출 블럭수를 다르게 함
	if		( pGameLayer->getLevel ( ) > 10 )	{ nBlockTypeCnt = BLOCK_TYPE_COUNT;     }
	else if ( pGameLayer->getLevel ( ) >  5 )	{ nBlockTypeCnt = BLOCK_TYPE_COUNT - 1; }
	else										{ nBlockTypeCnt = BLOCK_TYPE_COUNT - 3; }

	KDint		nType = kdRand ( ) % nBlockTypeCnt;

	// 다음 블럭 콜백 송신
	pGameLayer->onNextTetromino ( nType );

	return nType;
}

// 블럭생성
KDvoid Board::createTetromino ( KDvoid )
{
	// 다음 블럭이 정의되지 않았을때
	if ( m_nNextTetroType == -1 )
	{
		m_nNextTetroType = newTetroType ( );
	}

	// 블럭생성
	m_pTetromino = Tetromino::create ( m_nNextTetroType );
	m_pTetromino->setAnchorPoint ( ccpz );
	this->addChild ( m_pTetromino );

	// 블럭 위치를 하단의 중앙으로 설정
	KDint nCenterX = ( BOARD_COLUMN_CNT - m_pTetromino->getColumnCnt ( ) ) / 2;
	KDint nBottomY = BOARD_ROW_CNT - m_pTetromino->getRowCnt ( );
	m_pTetromino->setBoardPosition ( nCenterX, nBottomY );

	// 다음 블럭을 미리 정의
	m_nNextTetroType = newTetroType ( );
	CCLOG ( "createTetro: %d, %d", nCenterX, nBottomY );
}

// 블럭 이동
KDbool Board::moveTetromino ( KDint offsetX, KDint offsetY )
{
	if ( m_pTetromino == KD_NULL )
	{
		return KD_FALSE;
	}

	KDint	nMoveTop	 = m_pTetromino->getBoardY ( ) + offsetY;		// 이동 후 블럭 상단 좌표 (현재위치 + 이동값)
	KDint	nMoveLeft	 = m_pTetromino->getBoardX ( ) + offsetX;		// 이동 후 블럭 좌측 좌표 (현재위치 + 이동값)
	KDint	nMoveRight	 = nMoveLeft + m_pTetromino->getColumnCnt ( );	// 이동 후 블럭 우측 좌표 (좌측위치 + 블럭크기)

	KDbool	bIsStuck	 = KD_FALSE;
	KDbool	bIsCollision = KD_FALSE;

	// 상하 이동시 위쪽벽 또는 다른 블럭과 충돌체크 
	if ( offsetX == 0 && ( nMoveTop < 0 || isCollision ( offsetX, offsetY ) ) )
	{
		CCLOG ( "is Stuck" );
		bIsStuck = KD_TRUE;
	}

	// 좌측,우측벽,블럭 충돌체크
	else if ( nMoveLeft < 0 || nMoveRight > BOARD_COLUMN_CNT || isCollision ( offsetX, offsetY ) )
	{
		CCLOG ( "is Collision" );
		bIsCollision = KD_TRUE;
	}

	// 충돌 안했을때 이동
	if ( ! bIsCollision && ! bIsStuck )
	{
		CCLOG ( "moveTetro: %d, %d", nMoveLeft, nMoveTop );
		m_pTetromino->setBoardPosition ( nMoveLeft, nMoveTop );
	}

	// 이동종료시
	if ( bIsStuck )
	{
		// 이동하지 않으면 충돌이 아닐때
		if ( !isCollision ( 0, 0 ) )
		{
			// 블럭 이동완료
			stuckTetromino ( );

			// 새 블럭 생성
			createTetromino ( );
		}

		// 이동하지 않아도 이미 충돌일때, 블럭이 보드 마지막줄에 있으면
		else if ( m_pTetromino->getBoardY ( ) + m_pTetromino->getRowCnt ( ) >= BOARD_ROW_CNT )
		{
			// 게임오버
			gameOver ( );
		}

		return KD_FALSE;
	}

	return KD_TRUE;
}

// 블럭회전
KDvoid Board::rotateTetromino ( KDvoid )
{
	if ( m_pTetromino == KD_NULL )
	{
		return;
	}

	// 회전한 블록이 다른블록과 충돌되었을때 더 돌려주기 위한 카운터
	KDint	nRotateCount = 4;

	do 
	{
		// 회전 전 가로
		KDint	nBeforeColumnCnt = m_pTetromino->getColumnCnt ( );

		m_pTetromino->rotateBlock ( );
		
		// 회전 후 가로
		KDint nAfterColumnCnt = nBeforeColumnCnt - m_pTetromino->getColumnCnt ( );

		// 블럭 위치를 중간으로 보정
		m_pTetromino->setBoardPosition ( m_pTetromino->getBoardX ( ) + nAfterColumnCnt / 2, m_pTetromino->getBoardY ( ) );

		nRotateCount--;

	} while ( isCollision ( 0, 0 ) && nRotateCount > 0 ); // 회전한 블록이 다른블록과 충돌되었을때 최대 4번까지 반복

	SoundManager::getInstance ( )->playRotateEffect ( );
}

// 블럭 내리꽂기
KDvoid Board::shootTetromino ( KDvoid )
{
	if ( m_pTetromino == KD_NULL )
	{
		return;
	}

	// 파티클 효과. 구려서 뺌
	/*
	CCParticleSystem*	pEmitter = CCParticleExplosion::create ( 100 );
	pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/star.png" ) );
	pEmitter->setLife ( 0.1f );
	pEmitter->setLifeVar ( 0.1f );
	pEmitter->setSpeed ( 300 );
	pEmitter->setSpeedVar ( 300 );
	pEmitter->setStartSize ( 15 );
	pEmitter->setZOrder ( 0 );
	pEmitter->setAutoRemoveOnFinish ( KD_TRUE );
	pEmitter->setPosition ( ccp ( m_pTetromino->getPositionX ( ) + m_pTetromino->getColumnCnt ( ) * BLOCK_SIZE / 2, m_pTetromino->getPositionY ( ) + m_pTetromino->getRowCnt ( ) * BLOCK_SIZE / 2 ) );
	this->addChild ( pEmitter );	
	*/

	// 블럭이동이 종료될때 까지 반복
	while ( moveTetromino ( 0, -1 ) );
}

// 1초마다 블럭 위로이동
KDvoid Board::goTetromino ( KDfloat fDelta )
{
	moveTetromino ( 0, -1 );
}

// 블럭 이동완료
KDvoid Board::stuckTetromino ( KDvoid )
{
	// 스케쥴러 재가동
	setGameSpeed ( m_nGameSpeed );

	// Tetromino 내부 블럭을 보드에 저장
	saveTetromino ( );

	// 완성된 라인 제거
	clearLine ( );

	SoundManager::getInstance ( )->playStuckEffect ( );
}

// Tetromino 내부 블럭을 보드에 저장
KDvoid Board::saveTetromino ( KDvoid )
{
	// 내부의 블럭 배열
	CCArray*	pBlockArray = m_pTetromino->getChildren ( );
	KDint nCount = pBlockArray->count ( );

	// 내부의 블럭 배열 복사
	CCArray*	pCopyArray = CCArray::create ( );
	pCopyArray->addObjectsFromArray ( pBlockArray );
	
	// 내부 블럭 모두 제거
	m_pTetromino->removeAllChildren ( );

	CCLOG ( "saveParticle" );

	// 충돌 파티클 효과를 삽입할 블럭 저장
	CCObject*	pObject;
	CCArray*	pParticleArray = CCArray::create ( );
	CCARRAY_FOREACH ( pCopyArray, pObject )
	{
		Block*		pBlock = (Block*) pObject;

		KDint x = m_pTetromino->getBoardX ( ) + pBlock->getTetroX ( );
		KDint y = m_pTetromino->getBoardY ( ) + pBlock->getTetroY ( );

		// 가장 윗줄이거나 위쪽에 블럭이 있을때
		if ( y == 0 || m_aBoards [ x ] [ y - 1 ] != KD_NULL )
		{
			pParticleArray->addObject ( pBlock );
		}
	}

	CCLOG ( "saveTetro" );

	// 제거한 블럭을 보드에 붙임
	for ( KDint i = 0; i < nCount; i++ )
	{
		Block*		pBlock = (Block*) pCopyArray->objectAtIndex ( i );

		KDint x = m_pTetromino->getBoardX ( ) + pBlock->getTetroX ( );
		KDint y = m_pTetromino->getBoardY ( ) + pBlock->getTetroY ( );
		pBlock->setBoardPosition ( x, y );
		this->addChild ( pBlock );

		m_aBoards [ x ] [ y ] = pBlock;
	}

	CCLOG ( "showParticle" );

	CCARRAY_FOREACH ( pParticleArray, pObject )
	{
		Block*		pBlock = (Block*) pObject;

		// 충돌 파티클 효과
		CCParticleSystem*	pEmitter = CCParticleSun::create ( 100 );
		pEmitter->setLife ( 1 );
		pEmitter->setSpeedVar ( 1 );
		pEmitter->setDuration ( 0.1f );
		pEmitter->setStartSize ( 50 );
		pEmitter->setZOrder ( 0 );
		pEmitter->setAutoRemoveOnFinish ( KD_TRUE );
		pEmitter->setPosition ( ccp ( pBlock->getPositionX ( ) + BLOCK_SIZE / 2, pBlock->getPositionY ( ) + BLOCK_SIZE ) );
		this->addChild ( pEmitter );
	}
}

// Tetromino 내부 블럭과 보드에 있는 블럭 충돌체크
KDbool Board::isCollision ( KDint offsetX, KDint offsetY )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pTetromino->getChildren ( ), pObject )
	{
		Block*		pBlock = (Block*) pObject;

		KDint x = m_pTetromino->getBoardX ( ) + pBlock->getTetroX ( ) + offsetX;
		KDint y = m_pTetromino->getBoardY ( ) + pBlock->getTetroY ( ) + offsetY;

		if ( m_aBoards [ x ] [ y ] != KD_NULL )
		{
			CCLOG ( "%d, %d", m_pTetromino->getBoardX ( ), pBlock->getTetroX ( ), offsetX );
			return KD_TRUE;
		}
	}
	return KD_FALSE;
}

// 완성된 라인 제거
KDvoid Board::clearLine ( KDvoid )
{
	KDint	nClearCnt = 0;

	for ( KDint y = BOARD_ROW_CNT - 1; y >= 0; y-- )
	{
		KDbool	bIsFull = KD_TRUE;

		// 라인 완성여부 확인
		for ( KDint x = 0; x < BOARD_COLUMN_CNT; x++ )
		{
			if ( m_aBoards [ x ] [ y ] == KD_NULL )
			{
				bIsFull = KD_FALSE;
			}
		}

		// 완성된 라인일때
		if ( bIsFull )
		{
			nClearCnt++;
		}
		// 완성된 라인이 아닐때
		else
		{
			continue;
		}

		for ( KDint targetY = y; targetY < BOARD_ROW_CNT; targetY++ )
		{
			for ( KDint targetX = 0; targetX < BOARD_COLUMN_CNT; targetX++ )
			{
				Block*&		pBlock = m_aBoards [ targetX ] [ targetY ];

				// 제거 시작 라인일때
				if ( targetY == y )
				{
					KDfloat		fDelay = kdRand ( ) % 8 / 10.f;
					KDint		nPostionx = ( kdRand ( ) % ( BOARD_WIDTH * 3 ) ) - BOARD_WIDTH;

					// 블럭제거 애니메이션 구동
					CCAction*	pActions = CCSequence::create 
					(
						CCEaseOut::create																// 가속도를 주어
						( 
							CCSpawn::create 
							(
								CCFadeOut ::create ( 1 ),												// 점점사라지며
								CCRotateBy::create ( 0.5f, 360 * 5 ),									// 회전시키며
								CCMoveTo  ::create ( fDelay, ccp ( nPostionx, -300 ) ),					// 랜덤속도, 랜덤위치로 이동
								CCScaleBy ::create ( fDelay, 3 ),
								KD_NULL
							),
							0.8f
						),
						CCCallFuncO::create ( this, callfuncO_selector ( Board::clearBlock ), pBlock ),	// 이후 보드에서 블럭 제거
						KD_NULL
					);

					pBlock->setZOrder ( 0 );
					pBlock->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
					pBlock->runAction ( pActions );

					// 터지는 파티클 효과
					CCParticleSystem*	pEmitter = CCParticleFlower::create ( 100 );
					pEmitter->setLife ( 0.5f );
					pEmitter->setSpeedVar ( 0.5f );
					pEmitter->setDuration ( 0.5f );
					pEmitter->setStartSize ( 50 );
					pEmitter->setZOrder ( 0 );
					pEmitter->setAutoRemoveOnFinish ( KD_TRUE );
					pEmitter->setPosition ( ccp ( pBlock->getPositionX ( ) + BLOCK_SIZE / 2, pBlock->getPositionY ( ) + BLOCK_SIZE / 2 ) );
					this->addChild ( pEmitter );
				}
				// 마지막라인 빼고 블럭 한칸씩 위로 이동
				else if ( targetY < BOARD_ROW_CNT - 1 && pBlock != KD_NULL)
				{
					pBlock->setBoardPosition ( pBlock->getBoardX ( ), pBlock->getBoardY ( ) - 1 );
				}

				// 마지막라인 빼고 배열정보 한칸씩 위로 이동
				if ( targetY < BOARD_ROW_CNT - 1 )
				{
					pBlock = m_aBoards [ targetX ] [ targetY + 1 ];
				}
				// 마지막 라인은 null
				else
				{
					pBlock = KD_NULL;
				}
			}
		}
	}

	if ( nClearCnt > 0 )
	{
		// 라인 제거 콜백 송신
		GameLayer*		pGameLayer = (GameLayer*) this->getParent ( );
		pGameLayer->onClearLine ( nClearCnt );

		SoundManager::getInstance ( )->playBoomEffect ( );
	}
}

// 완성된 라인 제거 애니메이션 후 실제 블럭 제거
KDvoid Board::clearBlock ( CCObject* pSender )
{
	Block*		pBlock = (Block*) pSender;
	pBlock->removeFromParent ( );
}

// 게임오버
KDvoid Board::gameOver ( KDvoid )
{
	this->unschedule ( schedule_selector ( Board::goTetromino ) );

	// 게임 오버 콜백 송신
	GameLayer*	pGameLayer = (GameLayer*) this->getParent ( );
	pGameLayer->onGameOver ( );
}