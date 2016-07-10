/* --------------------------------------------------------------------------
 *
 *      File            Block.cpp
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
#include "Block.h"

Block::Block ( KDvoid )
{
	m_nTetroX = 0;
	m_nTetroY = 0;
	m_nBoardX = 0;
	m_nBoardY = 0;
}

Block* Block::create ( KDint nType )
{
	Block*		pBlock = new Block ( );	

	if ( pBlock && pBlock->init ( nType ) )
	{
		pBlock->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pBlock );
	}
	
	return pBlock;
}; 

KDbool Block::init ( KDint nType )
{
	if ( ! CCLayerGradient::init ( ) ) 
	{
		return KD_FALSE;
	}
	
	ccColor4B	tBlockColor1;
	ccColor4B	tBlockColor2;

	// 블럭타입에 따른 블럭배열 로딩
	switch ( nType )
	{
		case BLOCK_TYPE_I :		
			tBlockColor1 = ccc4 ( 255,   0,   0, 255 );	// 빨
			tBlockColor2 = ccc4 ( 255,  92,  92, 255 );	// 빨		
			break;

		case BLOCK_TYPE_O :		
			tBlockColor1 = ccc4 ( 255, 136,   0, 255 );	// 주
			tBlockColor2 = ccc4 ( 255, 178,  46, 255 );	// 주		
			break;

		case BLOCK_TYPE_S :
			tBlockColor1 = ccc4 ( 255, 213,   0, 255 );	// 노
			tBlockColor2 = ccc4 ( 255, 255,  46, 255 );	// 노		
			break;

		case BLOCK_TYPE_Z :
			tBlockColor1 = ccc4 (   0, 128,   0, 255 );	// 초
			tBlockColor2 = ccc4 (  46, 171,  46, 255 );	// 초		
			break;

		case BLOCK_TYPE_L :
			tBlockColor1 = ccc4 (   0,   0, 255, 255 ); // 파
			tBlockColor2 = ccc4 (  92,  92, 255, 255 ); // 파		
			break;

		case BLOCK_TYPE_J :
			tBlockColor1 = ccc4 (   6,  12,  86, 255 ); // 남
			tBlockColor2 = ccc4 (  96, 102, 171, 255 ); // 남
			break;

		case BLOCK_TYPE_T :
			tBlockColor1 = ccc4 ( 128,   0, 128, 255 ); // 보
			tBlockColor2 = ccc4 ( 214,  92, 214, 255 ); // 보		
			break;

		case BLOCK_TYPE_P :
			tBlockColor1 = ccc4 ( 255, 105, 180, 255 ); // 핑크
			tBlockColor2 = ccc4 ( 255, 153, 226, 255 ); // 핑크		
			break;

		case BLOCK_TYPE_Q :
			tBlockColor1 = ccc4 ( 189, 183, 107, 255 ); // 카키
			tBlockColor2 = ccc4 ( 232, 224, 153, 255 ); // 카키		
			break;

		case BLOCK_TYPE_U :
			tBlockColor1 = ccc4 ( 111, 127, 143, 255 ); // 점판암회색
			tBlockColor2 = ccc4 ( 155, 170, 186, 255 ); // 점판암회색		
			break;
	}

	ccColor3B	tColor = ccc3 ( 100, 100, 100 );

	// 블럭 우측 그림자
	CCSprite*	pRightSprite = CCSprite::create ( );
	pRightSprite->setTextureRect ( CCRect ( 0, 0, 1, BLOCK_SIZE ) );
	pRightSprite->setAnchorPoint ( ccpz );
	pRightSprite->setColor ( tColor );
	pRightSprite->setPositionX ( BLOCK_SIZE - 1 );

	// 블럭 하단 그림자
	CCSprite*	pBottomSprite = CCSprite::create ( );
	pBottomSprite->setTextureRect ( CCRect ( 0, 0, BLOCK_SIZE, 1 ) );
	pBottomSprite->setAnchorPoint ( ccpz );
	pBottomSprite->setColor ( tColor );

	// 그라데이션 블럭
	this->initWithColor
	( 
		tBlockColor1, 
		tBlockColor2, 
		ccp ( 0.5f, 0.5f ) 
	);
	this->setContentSize ( ccs ( BLOCK_SIZE, BLOCK_SIZE ) );
	this->setAnchorPoint ( ccpz );

	this->addChild ( pRightSprite );
	this->addChild ( pBottomSprite );

	return KD_TRUE;
}

// 해당 블럭이 속한 Tetromino의 상대좌표 설정
KDvoid Block::setTetroPosition ( KDint x, KDint y, KDint nTetroRow )
{
	m_nTetroX = x;
	m_nTetroY = y;

	// y축 좌표의 아래쪽이 0, 위쪽으로 올라갈 수록 증가하는 좌표를 반전시키기 위한 보정값
	KDint	nReversePosition = nTetroRow - 1;

	this->setPosition 
	( 
		ccp 
		( 
			(					 m_nTetroX ) * BLOCK_SIZE, 
			( nReversePosition - m_nTetroY ) * BLOCK_SIZE 
		) 
	);
}

// 해당 블럭이 속한 Tetromino의 상대 x 좌표 반환
KDint Block::getTetroX ( KDvoid )
{
	return m_nTetroX;
}

// 해당 블럭이 속한 Tetromino의 상대 y 좌표 반환
KDint Block::getTetroY ( KDvoid )
{
	return m_nTetroY;
}

// 해당 블럭이 속한 Board의 상대좌표 설정
KDvoid Block::setBoardPosition ( KDint x, KDint y )
{
	m_nBoardX = x;
	m_nBoardY = y;

	// y축 좌표의 아래쪽이 0, 위쪽으로 올라갈 수록 증가하는 좌표를 반전시키기 위한 보정값
	KDint	nReversePosition = BOARD_ROW_CNT - 1;

	this->setPosition 
	( 
		ccp 
		(
			(					 m_nBoardX ) * BLOCK_SIZE, 
			( nReversePosition - m_nBoardY ) * BLOCK_SIZE
		)
	);
}

// 해당 블럭이 속한 Board의 상대 x 좌표 반환
KDint Block::getBoardX ( KDvoid )
{
	return m_nBoardX;
}

// 해당 블럭이 속한 Board의 상대 y 좌표 반환
KDint Block::getBoardY ( KDvoid )
{
	return m_nBoardY;
}