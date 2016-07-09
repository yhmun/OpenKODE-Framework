/* --------------------------------------------------------------------------
 *
 *      File            BoardPiecePlace.cpp
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

#include "Precompiled.h"
#include "BoardPiecePlace.h"
#include "Board.h"

BoardPiecePlace* BoardPiecePlace::create ( Board* pBoard, KDuint uColumn, KDuint uRow, const CCSize& tSize )
{
	BoardPiecePlace*	pRet = new BoardPiecePlace ( );

	if ( pRet && pRet->initWithBoard ( pBoard, uColumn, uRow, tSize ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool BoardPiecePlace::initWithBoard ( Board* pBoard, KDuint uColumn, KDuint uRow, const CCSize& tSize )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_uColumn		= uColumn;
	m_uRow			= uRow;
	m_pBoard		= pBoard;
	m_pCurrentPiece	= KD_NULL;
	m_eState		= EMPTY;
	m_tRect			= CCRect ( -tSize.cx / 2, -tSize.cy / 2, tSize.cx, tSize.cy );

	return KD_TRUE;
}

// Dummy method for CCRGBAProtocol protocol.
KDvoid BoardPiecePlace::setColor ( const ccColor3B& tColor )
{

}

// Dummy method for CCRGBAProtocol protocol
const ccColor3B& BoardPiecePlace::getColor ( KDvoid )
{
	return ccWHITE;
}

const ccColor3B& BoardPiecePlace::getDisplayedColor ( KDvoid )
{
	return ccWHITE;
}

GLubyte BoardPiecePlace::getDisplayedOpacity ( KDvoid )
{
	return 255;
}

// Dummy method for CCRGBAProtocol protocol.
GLubyte BoardPiecePlace::getOpacity ( KDvoid )
{
	return 255;
}

KDvoid BoardPiecePlace::setOpacity ( GLubyte cOpacity )
{
	switch ( m_eState )
	{
		case EMPTY :
			break;

		case UNSELECTED_PIECE :
			( (CCSprite*) this->getChildByTag ( PIECE_SPRITE_TAG ) )->setOpacity ( cOpacity );
			break;

		case SELECTED_PIECE :
			( (CCSprite*) this->getChildByTag ( PIECE_SPRITE_TAG ) )->setOpacity ( cOpacity );
			( (CCSprite*) this->getChildByTag ( SELECTION_SPRITE_TAG ) )->setOpacity ( cOpacity );
			break;

		case ENDING_CORRECT_MATCHING :
			( (CCSprite*) this->getChildByTag ( MATCHING_SPRITE_TAG ) )->setOpacity ( cOpacity );
			break;

		case STARTING_CORRECT_MATCHING :
			( (CCSprite*) this->getChildByTag ( PIECE_SPRITE_TAG ) )->setOpacity ( cOpacity );
			( (CCSprite*) this->getChildByTag ( MATCHING_SPRITE_TAG ) )->setOpacity ( cOpacity );
			break;

		case INCORRECT_MATCHING :
			( (CCSprite*) this->getChildByTag ( PIECE_SPRITE_TAG ) )->setOpacity ( cOpacity );
			( (CCSprite*) this->getChildByTag ( MATCHING_SPRITE_TAG ) )->setOpacity ( cOpacity );
			break;

		default :
			CCAssert ( KD_FALSE, "Unknown BoardPiecePlace state in setOpacity." );	
	}
}

KDvoid BoardPiecePlace::setOpacityModifyRGB ( KDbool bValue )
{

}

KDbool BoardPiecePlace::isOpacityModifyRGB ( KDvoid )
{
	return KD_FALSE;
}

KDbool BoardPiecePlace::isCascadeColorEnabled ( KDvoid )
{
	return KD_FALSE;
}

KDvoid BoardPiecePlace::setCascadeColorEnabled ( KDbool bCascadeColorEnabled )
{

}

KDvoid BoardPiecePlace::updateDisplayedColor ( const ccColor3B& tColor )
{

}

KDbool BoardPiecePlace::isCascadeOpacityEnabled ( KDvoid )
{
	return KD_FALSE;
}

KDvoid BoardPiecePlace::setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled )
{

}

KDvoid BoardPiecePlace::updateDisplayedOpacity ( GLubyte cOpacity )
{

}

KDbool BoardPiecePlace::containsTouchLocation ( CCTouch* pTouch )
{
	return m_tRect.containsPoint ( this->convertTouchToNodeSpaceAR ( pTouch ) );
}

// A BoardPiecePlace may have or may not have a Piece. This Piece maybe is being killed, in this case is not selectable.
// Also, if there is no Piece, obviously is not selectable.
KDbool BoardPiecePlace::isPieceSelectable ( KDvoid )
{
	return m_eState == UNSELECTED_PIECE;
}

KDbool BoardPiecePlace::processTouch ( CCTouch* pTouch )
{
	// is game state in playing mode and board state in waiting for input state?
	if ( this->containsTouchLocation ( pTouch ) && m_pBoard->canBoardPiecePlaceBeSelected ( ) && 
	   ( m_eState == UNSELECTED_PIECE || m_eState == SELECTED_PIECE ) )
	{
		m_pBoard->boardPiecePlaceWasSelected ( this ); 
		return KD_TRUE;
	}

	return KD_FALSE;
}

// Sets currentPiece and change state to UNSELECTED. 
// Precondition: state must be EMPTY.
// Call reset in order to change to EMPTY state.
KDvoid BoardPiecePlace::setPiece ( Piece* pPiece )
{
	CCAssert ( m_eState == EMPTY, "m_eState must be EMPTY in setPiece.");
	m_pCurrentPiece = pPiece;
	this->changeBoardPiecePlaceState ( UNSELECTED_PIECE );
}

Piece* BoardPiecePlace::getCurrentPiece ( KDvoid )
{
	return m_pCurrentPiece;
}

// Removes Piece if used and selection or matching animation.
// Change state to EMPTY.
KDvoid BoardPiecePlace::reset ( KDvoid )
{
	m_eState = EMPTY;
	m_pCurrentPiece = KD_NULL;
	this->removeAllChildrenWithCleanup ( KD_TRUE );
}

KDvoid BoardPiecePlace::select ( KDvoid )
{
	CCAssert ( m_eState == UNSELECTED_PIECE, "m_eState must be UNSELECTED_PIECE in select." );
	this->changeBoardPiecePlaceState ( SELECTED_PIECE );
}

KDvoid BoardPiecePlace::unselect ( KDvoid )
{
	CCAssert ( m_eState == SELECTED_PIECE, "m_eState must be SELECTED_PIECE in unselect." );
	this->changeBoardPiecePlaceState ( UNSELECTED_PIECE );
}

// Called by Board to start a correct matching sequence.
KDvoid BoardPiecePlace::correctMatching ( KDvoid )
{
	CCAssert ( ( m_eState == SELECTED_PIECE || m_eState == UNSELECTED_PIECE ), "m_eState must be SELECTED_PIECE or UNSELECTED_PIECE in correctMatching." );
	this->changeBoardPiecePlaceState ( STARTING_CORRECT_MATCHING );
}
 
// Called by Board. 
KDvoid BoardPiecePlace::incorrectMatching ( KDvoid )
{
	CCAssert ( ( m_eState == SELECTED_PIECE || m_eState == UNSELECTED_PIECE ), "m_eState must be SELECTED_PIECE or UNSELECTED_PIECE in incorrectMatching." );
	this->changeBoardPiecePlaceState ( INCORRECT_MATCHING );
}

// Called in the middle of a correct matching sequence.
// Used to play correct matching sound and remove piece sprite.
KDvoid BoardPiecePlace::correctMatchingWillBeDoneCallback ( CCNode* pSender )
{
	this->changeBoardPiecePlaceState ( ENDING_CORRECT_MATCHING );
}

// Called after correct matching sequence was done. Used to change object's state. 
KDvoid BoardPiecePlace::correctMatchingWasDoneCallback ( CCNode* pSender )
{
	this->changeBoardPiecePlaceState ( EMPTY );
}

// Called after incorrect matching sequence was done. Used to change object's state. 
KDvoid BoardPiecePlace::incorrectMatchingWasDoneCallback ( CCNode* pSender )
{
	this->changeBoardPiecePlaceState ( UNSELECTED_PIECE );
}

KDvoid BoardPiecePlace::onEnterBoardPiecePlaceState ( BoardPiecePlaceState eState )
{
	if ( eState == EMPTY )
	{
		
	}
	else if ( eState == UNSELECTED_PIECE )
	{
		
	}
	else if ( eState == SELECTED_PIECE )
	{
		// add a sprite for selection and its animation
		CCSprite*	pSelectedSprite = CCSprite::createWithTexture ( m_pBoard->getSelectedTexture ( ) );
		pSelectedSprite->setTag ( SELECTION_SPRITE_TAG );
		this->addChild ( pSelectedSprite );

		CCRepeatForever*	pSelectedAction = (CCRepeatForever*) m_pBoard->getSelectedAction ( )->copy ( )->autorelease ( ); 
		pSelectedSprite->runAction ( pSelectedAction );
	}
	else if ( eState == STARTING_CORRECT_MATCHING )
	{

		// add sprite where matching sequence will be executed	
		CCSprite*	pStartingMatchingSprite = CCSprite::createWithTexture ( m_pBoard->getStartingMatchingTexture ( ) );
		pStartingMatchingSprite->setTag ( MATCHING_SPRITE_TAG );
		this->addChild ( pStartingMatchingSprite );

		// create sequence action
		CCAnimate*	pStartingMatchingAction = (CCAnimate*) m_pBoard->getStartingMatchingAction ( )->copy ( )->autorelease ( ); 
		CCAnimate*	pCorrectMatchingEndingAction = (CCAnimate*) m_pBoard->getCorrectMatchingEndingAction ( )->copy ( )->autorelease ( ); 
	
		CCAction*	pAction = CCSequence::create
		(
			pStartingMatchingAction,
			CCCallFuncN::create ( this, callfuncN_selector ( BoardPiecePlace::correctMatchingWillBeDoneCallback ) ),
			pCorrectMatchingEndingAction,
			CCCallFuncN::create ( this, callfuncN_selector ( BoardPiecePlace::correctMatchingWasDoneCallback ) ),
			KD_NULL 
		);
		
		// run action
		pStartingMatchingSprite->runAction ( pAction );
	}
	else if ( eState == ENDING_CORRECT_MATCHING )
	{
		
	}
	else if ( eState == INCORRECT_MATCHING )
	{
		// add sprite where matching sequence will be executed	
		CCSprite*	pStartingMatchingSprite = CCSprite::createWithTexture ( m_pBoard->getStartingMatchingTexture ( ) );
		pStartingMatchingSprite->setTag ( MATCHING_SPRITE_TAG );
		this->addChild ( pStartingMatchingSprite );
		m_pBoard->reorderChild ( this, 1 );

		// create sequence action
		CCAnimate*	pStartingMatchingAction = (CCAnimate*) m_pBoard->getStartingMatchingAction ( )->copy ( )->autorelease ( ); 
		CCAnimate*	pIncorrectMatchingEndingAction = (CCAnimate*) m_pBoard->getIncorrectMatchingEndingAction ( )->copy ( )->autorelease ( ); 

		CCAction*	pAction = CCSequence::create
		(
			pStartingMatchingAction,			
			pIncorrectMatchingEndingAction,
			CCCallFuncN::create ( this, callfuncN_selector ( BoardPiecePlace::incorrectMatchingWasDoneCallback ) ),
			KD_NULL 
		);

		// run action
		pStartingMatchingSprite->runAction ( pAction );
	}	
}

KDvoid BoardPiecePlace::onExitBoardPiecePlaceState ( BoardPiecePlaceState eState )
{
	if ( eState == EMPTY )
	{
		CCSpriteFrame*	pPieceSpriteFrame = m_pCurrentPiece->getSpriteFrame ( );
		CCSprite*		pSprite = CCSprite::createWithSpriteFrame ( pPieceSpriteFrame );
		pSprite->setTag ( PIECE_SPRITE_TAG );
		this->addChild ( pSprite );			
	}
	else if ( eState == UNSELECTED_PIECE )
	{
		
	}
	else if ( eState == SELECTED_PIECE )
	{
		this->removeChildByTag ( SELECTION_SPRITE_TAG, KD_TRUE );
	}
	else if ( eState == STARTING_CORRECT_MATCHING )
	{
		this->removeChildByTag ( PIECE_SPRITE_TAG, KD_TRUE );
	}
	else if ( eState == ENDING_CORRECT_MATCHING )
	{
		this->removeChildByTag ( MATCHING_SPRITE_TAG, KD_TRUE );
	} 
	else if ( eState == INCORRECT_MATCHING )
	{
		m_pBoard->reorderChild ( this, 1 );
		this->removeChildByTag ( MATCHING_SPRITE_TAG, KD_TRUE );
	}	
}

KDvoid BoardPiecePlace::changeBoardPiecePlaceState ( BoardPiecePlaceState eState )
{
	this->onExitBoardPiecePlaceState ( m_eState );

	if ( m_eState == EMPTY && eState == UNSELECTED_PIECE ) 
	{

	} 
	else if ( m_eState == UNSELECTED_PIECE && eState == SELECTED_PIECE )
	{
		
	} 
	else if ( m_eState == SELECTED_PIECE && eState == UNSELECTED_PIECE )
	{
		
	} 
	else if ( m_eState == SELECTED_PIECE && eState == STARTING_CORRECT_MATCHING )
	{
		
	} 
	else if ( m_eState == STARTING_CORRECT_MATCHING && eState == ENDING_CORRECT_MATCHING )
	{
		
	}
	else if ( m_eState == ENDING_CORRECT_MATCHING && eState == EMPTY )
	{
		
	} 
	else if ( m_eState == SELECTED_PIECE && eState == INCORRECT_MATCHING )
	{
		
	}
	else if ( m_eState == INCORRECT_MATCHING && eState == UNSELECTED_PIECE )
	{
		
	} 
	else if ( m_eState == UNSELECTED_PIECE && eState == STARTING_CORRECT_MATCHING )
	{
		
	}
	else if ( m_eState == UNSELECTED_PIECE && eState == INCORRECT_MATCHING )
	{
		
	} 
	else 
	{
		CCAssert ( KD_FALSE, "Unknown transition." );
	}
	
	m_eState = eState;

	this->onEnterBoardPiecePlaceState ( eState );
}

