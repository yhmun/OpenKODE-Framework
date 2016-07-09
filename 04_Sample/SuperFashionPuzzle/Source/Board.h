/* --------------------------------------------------------------------------
 *
 *      File            Board.h
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

#ifndef __Board_h__
#define __Board_h__

#include "BoardPiecePlace.h"
#include "BoardPiecePlacesPivotNode.h"

class PlayingScene;

enum BoardState
{
	REGENERATING_BOARD,
	CHANGING_GIRL,
	WAITING_USER_INPUT_WO_CHIP_SELECTED,
	WAITING_USER_INPUT_WITH_CHIP_SELECTED,
};

class Board : public CCNode, public CCTargetedTouchDelegate
{
	public :
				 Board ( KDvoid );
		virtual ~Board ( KDvoid );

		static Board*		create ( PlayingScene* pPlayingScene );

	public :

		virtual KDbool		initWithPlayingScene ( PlayingScene* pPlayingScene );

		virtual KDvoid		onEnter ( KDvoid );
		virtual KDvoid		onExit  ( KDvoid );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		BoardPiecePlace*	getBoardPiecePlaceAtColumn ( KDuint uColumn, KDuint uRow );

		Piece*				getPiece ( KDuint uPiece, KDuint uGirl );

		KDvoid				reset ( KDvoid );

		KDvoid				regenerate ( KDuint uLevel );

		KDvoid				inTheMiddleOfRegenerationSequenceCallback ( CCNode* pSender );

		KDvoid				regenerationSquenceWasDone ( CCNode* pSender );
		
		KDvoid				inTheMiddleOfChangeGirlSequenceCallback ( CCNode* pSender );
		
		KDvoid				changeGirlSequenceWasDoneCallback ( CCNode* pSender );

		KDvoid				correctMatchWasDone ( KDbool bWithWildcard, BoardPiecePlace* pBoardPiecePlace );

		KDvoid				incorrectMatchWasDone ( BoardPiecePlace* pBoardPiecePlace );

		KDuint				getNumberOfWildcardsForLevel ( KDuint uLevel );

		// Used by BoardPiecePlace
		KDbool				canBoardPiecePlaceBeSelected ( KDvoid );

		KDvoid				boardPiecePlaceWasSelected ( BoardPiecePlace* pBoardPiecePlace );

		// FSM managment
		KDvoid				changeBoardState ( BoardState eState );

		KDvoid				onEnterBoardState ( BoardState eState );

		KDvoid				onExitBoardState ( BoardState eState );

		KDbool				containsTouchLocation ( CCTouch* pTouch );

		CCTexture2D*		getSelectedTexture ( KDvoid );

		CCTexture2D*		getStartingMatchingTexture ( KDvoid );
		
		CCRepeatForever*	getSelectedAction ( KDvoid );

		CCAnimate*			getStartingMatchingAction ( KDvoid );

		CCAnimate*			getCorrectMatchingEndingAction ( KDvoid );

		CCAnimate*			getIncorrectMatchingEndingAction ( KDvoid );		

	private :

		PlayingScene*				m_pPlayingScene;
		CCArray*					m_pBoardPiecePlaces;
		CCArray*					m_pPiecesDataBase;
		CCTexture2D*				m_pPiecesSpriteSheetTexture;
		Piece*						m_pWildcardPiece;
		BoardPiecePlacesPivotNode*	m_pBoardPiecePlacesPivotNode;
		CCRect						m_tBoardRect;

		// Used by BoardPiecePlace to create selection and matching animations
		CCTexture2D*				m_pSelectedTexture;
		CCRepeatForever*			m_pSelectedAction;
		CCTexture2D*				m_pStartingMatchingTexture;
		CCAnimate*					m_pStartingMatchingAction;
		CCAnimate*					m_pCorrectMatchingEndingAction;
		CCAnimate*					m_pIncorrectMatchingEndingAction;

		// FSM
		BoardPiecePlace*			m_pSelectedBoardPiecePlace;
		KDuint						m_uNumberOfRegenerations;
		KDuint						m_uNumberOfMatches;
		BoardState					m_eState;
};

#endif	// __Board_h__