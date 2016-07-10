/* --------------------------------------------------------------------------
 *
 *      File            Tetromino.cpp
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
#include "Tetromino.h"
#include "Block.h"

Tetromino::Tetromino ( KDvoid )
{
	m_nRowCnt = 0;
	m_nColumnCnt = 0;

	m_nBoardX = 0;
	m_nBoardY = 0;
}

Tetromino* Tetromino::create ( KDint nType )
{
	Tetromino*	pTetromino = new Tetromino ( );	
	if ( pTetromino && pTetromino->init ( nType ) )
	{
		pTetromino->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pTetromino );
	}
	
	return pTetromino;
}; 

KDbool Tetromino::init ( KDint nType )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_nType = nType;

	// 블럭타입에 따른 블럭배열 로딩
	switch ( nType )
	{
		case BLOCK_TYPE_I :
		{
			KDint	aBlockI [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_I;
			kdMemcpy ( m_aBlockArray, aBlockI, sizeof ( aBlockI ) );
		}
		break;

		case BLOCK_TYPE_O :
		{
			KDint	aBlockO [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_O;
			kdMemcpy ( m_aBlockArray, aBlockO, sizeof ( aBlockO ) );
		}
		break;

		case BLOCK_TYPE_S :
		{
			KDint	aBlockS [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_S;
			kdMemcpy ( m_aBlockArray, aBlockS, sizeof ( aBlockS ) );
		}
		break;

		case BLOCK_TYPE_Z :
		{
			KDint	aBlockZ [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_Z;
			kdMemcpy ( m_aBlockArray, aBlockZ, sizeof( aBlockZ ) );
		}
		break;

		case BLOCK_TYPE_L :
		{
			KDint	aBlockL [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_L;
			kdMemcpy ( m_aBlockArray, aBlockL, sizeof ( aBlockL ) );
		}
		break;

		case BLOCK_TYPE_J :
		{
			KDint	aBlockJ [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_J;
			kdMemcpy ( m_aBlockArray, aBlockJ, sizeof ( aBlockJ ) );
		}
		break;

		case BLOCK_TYPE_T :
		{
			KDint	aBlockT [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_T;
			kdMemcpy ( m_aBlockArray, aBlockT, sizeof ( aBlockT ) );
		}
		break;

		case BLOCK_TYPE_P :
		{
			KDint	aBlockP [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_P;
			kdMemcpy ( m_aBlockArray, aBlockP, sizeof ( aBlockP ) );
		}
		break;

		case BLOCK_TYPE_Q :
		{
			KDint	aBlockQ [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_Q;
			kdMemcpy ( m_aBlockArray, aBlockQ, sizeof ( aBlockQ ) );
		}
		break;

		case BLOCK_TYPE_U :
		{
			KDint	aBlockU [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] = BLOCK_ARRAY_U;
			kdMemcpy ( m_aBlockArray, aBlockU, sizeof ( aBlockU ) );
		}
		break;
	}
	
	makeBlock ( );

	return KD_TRUE;
}

// Tetrimino 내부 블럭을 정의된 모양대로 생성
KDvoid Tetromino::makeBlock ( KDvoid )
{
	// 블럭 사이즈 구함
	CCSize		tSize = this->getSize ( m_aBlockArray );
	m_nRowCnt = tSize.cy;
	m_nColumnCnt = tSize.cx;

	KDint		nBlockCnt = 0;	// 추가 된 블럭수
	for ( KDint y = 0; y < m_nRowCnt; y++ )
	{
		for ( KDint x = 0; x < m_nColumnCnt; x++ )
		{
			// 블럭 존재시
			if ( m_aBlockArray [ y ][ x ] == 1 )
			{
				// 블럭 생성하여 위치설정
				Block*		pBlock = Block::create ( m_nType );
				pBlock->setTetroPosition ( x, y, m_nRowCnt );
				this->addChild ( pBlock );

				nBlockCnt++;				// 블럭 갯수 증가
				if ( m_nRowCnt < y + 1 )	
				{
					m_nRowCnt = y + 1;		// 행 갯수 증가
				}

				if ( m_nColumnCnt < x + 1 )
				{
					m_nColumnCnt = x + 1;	// 열 갯수 증가
				}
			}

			if ( nBlockCnt == BLOCK_MAX_CNT )
			{
				break;
			}
		}

		if ( nBlockCnt == BLOCK_MAX_CNT )
		{
			break;
		}
	}
}

// 블럭 내부 회전
KDvoid Tetromino::rotateBlock ( KDvoid )
{
	// 배열 회전
	KDint	aTemp [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ];
	for ( KDint y = 0; y < BLOCK_MATRIX_CNT; y++ )
	{
		// 회전 후 Y좌표
		KDint	nRotateY = m_nColumnCnt - y - 1;

		for ( KDint x = 0; x < BLOCK_MATRIX_CNT; x++ )
		{
			// 회전 후 좌표가 지정된 배열내일때
			if ( nRotateY >= 0 && nRotateY < BLOCK_MATRIX_CNT )
			{
				aTemp [ y ][ x ] = m_aBlockArray [ x ][ nRotateY ];
			}
			// 회전 후 좌표가 지정된 배열밖일때 0으로 초기화
			else
			{
				aTemp [ y ][ x ] = 0;
			}
		}
	}
	kdMemcpy ( m_aBlockArray, aTemp, sizeof ( m_aBlockArray ) ); 

	// 블럭 제거 후 새로 생성
	removeAllChildrenWithCleanup ( KD_TRUE );

	makeBlock ( );
}

// Tetromino 의 실제 크기를 반환
CCSize Tetromino::getSize ( KDint aArray [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] )
{
	KDint	nMinX = 100, nMaxX = 0, nMinY = 100, nMaxY = 0;

	for ( KDint y = 0; y < BLOCK_MATRIX_CNT; y++ )
	{
		for ( KDint x = 0; x < BLOCK_MATRIX_CNT; x++ )
		{
			if( aArray [ y ][ x ] == 1 )
			{
				if ( nMinX > x )	nMinX = x;
				if ( nMaxX < x )	nMaxX = x;
				if ( nMinY > y )	nMinY = y;
				if ( nMaxY < y )	nMaxY = y;
			}
		}
	}
	return CCSizeMake ( nMaxX - nMinX + 1, nMaxY - nMinY + 1 );
}

// Tetromino 의 실제 세로 크기를 반환
KDint Tetromino::getRowCnt ( KDvoid )
{
	return m_nRowCnt;
}

// Tetromino 의 실제 가로 크기를 반환
KDint Tetromino::getColumnCnt ( KDvoid )
{
	return m_nColumnCnt;
}

// Board 내의 Tetromino 좌측 상단기준 좌표설정 (Board 내의 실제좌표)
KDvoid Tetromino::setBoardPosition ( KDint x, KDint y )
{
	CCLOG ( "Tetromino setBoardPosition: %d, %d", x, y );

	m_nBoardX = x;
	m_nBoardY = y;

	// 좌측 블럭이 보드 왼쪽으로 넘어갔을때 보드 안쪽으로 위치 보정
	if ( m_nBoardX < 0 )
	{
		m_nBoardX = 0;
	}
	// 우측 블럭이 보드 오른쪽으로 넘어갔을때 보드 안쪽으로 위치 보정
	else if ( m_nBoardX + getColumnCnt ( ) > BOARD_COLUMN_CNT )
	{
		m_nBoardX = BOARD_COLUMN_CNT - getColumnCnt ( );
	}

	// 상단 블럭이 보드 위쪽으로 넘어갔을때 보드 안쪽으로 위치 보정
	if ( m_nBoardY < 0 )
	{
		m_nBoardY = 0;
	}
	// 하단 블럭이 보드 아래쪽으로 넘어갔을때 보드 안쪽으로 위치 보정
	else if ( m_nBoardY + getRowCnt ( ) > BOARD_ROW_CNT )
	{
		m_nBoardY = BOARD_ROW_CNT - getRowCnt ( );
	}

	// y축 좌표의 아래쪽이 0, 위쪽으로 올라갈 수록 증가하는 좌표를 반전시키기 위한 보정값
	KDint nReversePosition = BOARD_ROW_CNT - getRowCnt ( );

	setPosition 
	( 
		ccp 
		( 
			(					 m_nBoardX ) * BLOCK_SIZE, 
			( nReversePosition - m_nBoardY ) * BLOCK_SIZE
		) 
	);
}

// Board 내의 실제 X좌표
KDint Tetromino::getBoardX ( KDvoid )
{
	return m_nBoardX;
}

// Board 내의 실제 Y좌표
KDint Tetromino::getBoardY ( KDvoid )
{
	return m_nBoardY;
}
