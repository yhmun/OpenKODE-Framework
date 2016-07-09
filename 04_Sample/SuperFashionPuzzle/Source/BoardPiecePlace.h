/* --------------------------------------------------------------------------
 *
 *      File            BoardPiecePlace.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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

#ifndef __BoardPiecePlace_h__
#define __BoardPiecePlace_h__

#include "Piece.h"

class Board;

enum BoardPiecePlaceState 
{
	EMPTY,
	UNSELECTED_PIECE,
	SELECTED_PIECE,
	STARTING_CORRECT_MATCHING,
	ENDING_CORRECT_MATCHING,
	INCORRECT_MATCHING
};

// A node that represents a place where a piece can be used.
// It may have up to 3 different sprites: Piece, Selection, Matching. All of tem use a tag value in Constants.h
class BoardPiecePlace : public CCNode, public CCRGBAProtocol 
{
	public :

		static BoardPiecePlace*		create ( Board* pBoard, KDuint uColumn, KDuint uRow, const CCSize& tSize );

	public :

		virtual KDbool				initWithBoard ( Board* pBoard, KDuint uColumn, KDuint uRow, const CCSize& tSize );

		virtual KDvoid				setColor ( const ccColor3B& tColor );
		virtual const ccColor3B&	getColor ( KDvoid );

		virtual const ccColor3B&	getDisplayedColor ( KDvoid );
		virtual GLubyte				getDisplayedOpacity ( KDvoid );

		virtual GLubyte				getOpacity ( KDvoid );
		virtual KDvoid				setOpacity ( GLubyte cOpacity );

		virtual KDvoid				setOpacityModifyRGB ( KDbool bValue );
		virtual KDbool				 isOpacityModifyRGB ( KDvoid );

		virtual KDbool				 isCascadeColorEnabled ( KDvoid );
		virtual KDvoid				setCascadeColorEnabled ( KDbool bCascadeColorEnabled );
	    
		virtual KDvoid				updateDisplayedColor ( const ccColor3B& tColor );

		virtual KDbool				 isCascadeOpacityEnabled ( KDvoid );
		virtual KDvoid				setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled );
	    
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cOpacity );

		KDbool						containsTouchLocation ( CCTouch* pTouch );

		KDbool						isPieceSelectable ( KDvoid );

		KDbool						processTouch ( CCTouch* pTouch );

		// Used by Board to change object state.
		KDvoid						setPiece ( Piece* pPiece );

		Piece*						getCurrentPiece ( KDvoid );
		
		KDvoid						reset ( KDvoid );
		
		KDvoid						select ( KDvoid );
		
		KDvoid						unselect ( KDvoid );
		
		KDvoid						correctMatching ( KDvoid );

		KDvoid						incorrectMatching ( KDvoid );

		// Callbacks
		KDvoid						correctMatchingWillBeDoneCallback ( CCNode* pSender );

		KDvoid						correctMatchingWasDoneCallback ( CCNode* pSender );

		KDvoid						incorrectMatchingWasDoneCallback ( CCNode* pSender );

		// FSM managment
		KDvoid						onEnterBoardPiecePlaceState ( BoardPiecePlaceState eState );
		
		KDvoid						onExitBoardPiecePlaceState ( BoardPiecePlaceState eState );

		KDvoid						changeBoardPiecePlaceState ( BoardPiecePlaceState eState );
		
	private :

		Piece*						m_pCurrentPiece;
		CCRect						m_tRect;
		Board*						m_pBoard;
		KDuint						m_uColumn;
		KDuint						m_uRow;
		BoardPiecePlaceState		m_eState;
};

#endif	// __BoardPiecePlace_h__