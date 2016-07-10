/* --------------------------------------------------------------------------
 *
 *      File            Board.h
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

#ifndef __Board_h__
#define __Board_h__

class Block;
class Tetromino;

class Board : public CCLayer
{
	public :

		Board ( KDvoid );

		CREATE_FUNC ( Board );

	public :

		virtual KDbool	init ( KDvoid );

		virtual KDvoid	onEnter ( KDvoid );

		KDvoid			setGameSpeed		( KDfloat fSec );		// 게임속도 설정
		KDint			newTetroType		( KDvoid );				// 랜덤 블럭타입 생성

		KDvoid			createTetromino		( KDvoid );				// 블럭 생성
		KDbool			moveTetromino		( KDint x, KDint y );	// 블럭 이동
		KDvoid			rotateTetromino		( KDvoid );				// 블럭 회전
		KDvoid			shootTetromino		( KDvoid );				// 블럭 내리꽂기

		KDvoid			goTetromino			( KDfloat fDelta );		// 지정된 시간마다 블럭 위로이동
		KDvoid			stuckTetromino		( KDvoid );				// 블럭 이동완료
		KDvoid			saveTetromino		( KDvoid );				// Tetromino 내부 블럭을 보드에 저장

		KDbool			isCollision			( KDint x, KDint y );	// Tetromino 내부 블럭과 보드에 있는 블럭 충돌체크
		KDvoid			clearLine			( KDvoid );				// 완성된 라인 제거
		KDvoid			clearBlock			( CCObject* pSender );	// 완성된 라인 제거 애니메이션 후 실제 블럭 제거
		KDvoid			gameOver			( KDvoid );				// 게임오버

	protected :

		Block*			m_aBoards [ BOARD_COLUMN_CNT ] [ BOARD_ROW_CNT ]; // 쌓여있는 블럭정보
		Tetromino*		m_pTetromino;		// 이동대상 블럭

		KDint			m_nNextTetroType;	// 다음블럭 타입
		KDfloat			m_nGameSpeed;		// 게임속도
};

#endif	//__Board_h__