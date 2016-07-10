/* --------------------------------------------------------------------------
 *
 *      File            Block.h
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

#ifndef __Block_h__
#define __Block_h__

class Block : public CCLayerGradient
{
	public :

		Block ( KDvoid );

		static  Block*	create ( KDint nType );

		virtual KDbool	init   ( KDint nType );

	public :

		KDvoid			setTetroPosition	( KDint x, KDint y, KDint nTetroRow );	// Tetromino 내의 블럭의 위치를 설정
		KDint			getTetroX			( KDvoid );								// Tetromino 내의 실제 X좌표 반환
		KDint			getTetroY			( KDvoid );								// Tetromino 내의 실제 Y좌표 반환

		KDvoid			setBoardPosition	( KDint x, KDint y );					// Board 내의 블럭의 위치를 설정
		KDint			getBoardX			( KDvoid );								// Board 내의 X좌표 반환
		KDint			getBoardY			( KDvoid );								// Board 내의 Y좌표 반환

	protected :
	
		KDint			m_nTetroX;	// Tetromino 내의 X좌표
		KDint 			m_nTetroY;	// Tetromino 내의 Y좌표
	
		KDint 			m_nBoardX;	// Board 내의 X좌표
		KDint 			m_nBoardY;	// Board 내의 Y좌표
};

#endif	//__Block_h__