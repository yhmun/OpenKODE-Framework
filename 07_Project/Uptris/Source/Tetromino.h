/* --------------------------------------------------------------------------
 *
 *      File            Tetromino.h
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

#ifndef __Tetromino_h__
#define __Tetromino_h__

class Tetromino : public CCNode
{
	public :

		Tetromino ( KDvoid );

		static  Tetromino*	create ( KDint nType );

		virtual KDbool		init ( KDint nType );

	private :		

		KDvoid		makeBlock ( KDvoid );												// Tetromino 내부 블럭 생성

		CCSize		getSize ( KDint aArray [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ] );	// Tetromino 의 실제 크기를 반환

	public :

		KDvoid		rotateBlock			( KDvoid );				// 블럭 내부 회전

		KDint		getRowCnt			( KDvoid );				// Tetromino 의 실제 세로 크기를 반환
		KDint		getColumnCnt		( KDvoid );				// Tetromino 의 실제 가로 크기를 반환

		KDvoid		setBoardPosition	( KDint x, KDint y );	// Board 내의 Tetromino 좌측 상단기준 좌표설정
		KDint		getBoardX			( KDvoid );				// Board 내의 X좌표 반환
		KDint		getBoardY			( KDvoid );				// Board 내의 Y좌표 반환

	protected :

		KDint		m_aBlockArray [ BLOCK_MATRIX_CNT ][ BLOCK_MATRIX_CNT ]; // 블럭 배열
		KDint		m_nType;		// 블럭타입

		KDint		m_nRowCnt;		// Tetromino 의 실제 세로 크기
		KDint		m_nColumnCnt;	// Tetromino 의 실제 가로 크기
		KDint		m_nBoardX;		// Board 내의 X좌표
		KDint		m_nBoardY; 		// Board 내의 Y좌표
};

#endif  // __Tetromino_h__