/* --------------------------------------------------------------------------
 *
 *      File            Board.cpp
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
#include "Board.h"
#include "PlayingScene.h"

#define LIMIT_GIRLS_IN_FREE_VERSION			3

Board* Board::create ( PlayingScene* pPlayingScene )
{
	Board*	pRet = new Board ( );

	if ( pRet && pRet->initWithPlayingScene ( pPlayingScene ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Board::Board ( KDvoid )
{
	m_pBoardPiecePlaces				 = KD_NULL;
	m_pPiecesDataBase				 = KD_NULL;
	m_pSelectedAction				 = KD_NULL;
	m_pStartingMatchingAction		 = KD_NULL;
	m_pCorrectMatchingEndingAction	 = KD_NULL;
	m_pIncorrectMatchingEndingAction = KD_NULL;
	m_pWildcardPiece				 = KD_NULL;
}

Board::~Board ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pBoardPiecePlaces				);
	CC_SAFE_RELEASE ( m_pPiecesDataBase					);
	CC_SAFE_RELEASE ( m_pSelectedAction					);
	CC_SAFE_RELEASE ( m_pStartingMatchingAction			);
	CC_SAFE_RELEASE ( m_pCorrectMatchingEndingAction	);	
	CC_SAFE_RELEASE ( m_pIncorrectMatchingEndingAction	);
	CC_SAFE_RELEASE ( m_pWildcardPiece					);
}

KDbool Board::initWithPlayingScene ( PlayingScene* pPlayingScene )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	// board background texture
	CCSprite*	pBoardSprite = CCSprite::create ( "board.png" );
	this->addChild ( pBoardSprite );

	// board rect
	m_tBoardRect = pBoardSprite->getTextureRect ( );

	// calculate board size
	CCSize		tSize = pBoardSprite->getContentSize ( );
	m_tBoardRect = CCRect ( -tSize.cx / 2, -tSize.cy / 2, tSize.cx, tSize.cy );

	// pieces texture		
	m_pPiecesSpriteSheetTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "pieces.pvr" );	

	CCSpriteFrameCache*		pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pSpriteFrameCache->addSpriteFramesWithFile ( "pieces.plist", m_pPiecesSpriteSheetTexture );	

	// create 10*5 instances of Piece class
	m_pPiecesDataBase = new CCArray ( ); 
	for ( KDuint uGirl = 0; uGirl < NUMBER_OF_GIRLS; uGirl++ )
	{
		CCArray*	pPieces = CCArray::create ( );
		for ( KDuint uPiece = 0; uPiece < NUMBER_OF_PIECES_PER_GIRL; uPiece++ )
		{
			const KDchar*	szOriginalTextureFilename = ccszf ( "g%d_f%d.png", uGirl + 1, uPiece + 1 );
			CCSpriteFrame*	pSpriteFrame = pSpriteFrameCache->spriteFrameByName ( szOriginalTextureFilename );
			Piece*			pPieceInstance = Piece::create ( pSpriteFrame, uPiece, uGirl );
			pPieces->addObject ( pPieceInstance );			
		}
		m_pPiecesDataBase->addObject ( pPieces );
	}

	// wildcard piece
	CCSpriteFrame*	pWildcardSpriteFrame = pSpriteFrameCache->spriteFrameByName ( "wildcard.png" );
	CCAssert ( pWildcardSpriteFrame != KD_NULL, "wildcardSpriteFrame cannot be null." );
	m_pWildcardPiece = Piece::create ( pWildcardSpriteFrame );
	m_pWildcardPiece->retain ( );

	// used by BoardPiecePlace when any of them is selected with m_selectedAction
	m_pSelectedTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "selection_animated_0001.png" );

	// used by BoardPiecePlace when any of them is selected with m_selectedSprite
	CCAnimation*	pSelectedAnimation = CCAnimation::create ( );
	pSelectedAnimation->setDelayPerUnit ( 1 / 12.f );
	for ( KDint i = 1; i <= 11; i++ )
	{
		const KDchar*	szOriginalTextureFilename = ccszf ( "selection_animated_%04d.png", i );
		pSelectedAnimation->addSpriteFrameWithFileName ( szOriginalTextureFilename );
	}
	m_pSelectedAction = CCRepeatForever::create ( CCAnimate::create ( pSelectedAnimation ) );
	m_pSelectedAction->retain ( );

	// matching starting sequence, used by BoardPiecePlace		
	CCAnimation*	pStartingMatchingAnimation = CCAnimation::create ( );
	pStartingMatchingAnimation->setDelayPerUnit ( 1 / 12.f );
	for ( KDuint i = 1; i <= 5; i++ )
	{
		const KDchar*	szOriginalTextureFilename = ccszf ( "starting_matching_sequence_%04d.png", i );
		pStartingMatchingAnimation->addSpriteFrameWithFileName ( szOriginalTextureFilename );
	}
	m_pStartingMatchingAction = CCAnimate::create ( pStartingMatchingAnimation ); 
	m_pStartingMatchingAction->retain ( );

	// correct matching ending sequence, used by BoardPiecePlace
	CCAnimation*	pCorrectMatchingEndingAnimation = CCAnimation::create ( );
	pCorrectMatchingEndingAnimation->setDelayPerUnit ( 1 / 12.f );
	for ( KDuint i = 6; i <= 11; i++ )
	{
		const KDchar*	szOriginalTextureFilename = ccszf ( "correct_matching_ending_sequence_%04d.png", i );
		pCorrectMatchingEndingAnimation->addSpriteFrameWithFileName ( szOriginalTextureFilename );
	}
	m_pCorrectMatchingEndingAction = CCAnimate::create ( pCorrectMatchingEndingAnimation ); 
	m_pCorrectMatchingEndingAction->retain ( );

	// incorrect matching ending sequence, used by BoardPiecePlace
	CCAnimation*	pIncorrectMatchingEndingAnimation = CCAnimation::create ( );
	pIncorrectMatchingEndingAnimation->setDelayPerUnit ( 1 / 12.f );
	for ( KDuint i = 6; i <= 11; i++ )
	{
		const KDchar*	szOriginalTextureFilename = ccszf ( "incorrect_matching_ending_sequence_%04d.png", i );
		pIncorrectMatchingEndingAnimation->addSpriteFrameWithFileName ( szOriginalTextureFilename );		
	}
	m_pIncorrectMatchingEndingAction = CCAnimate::create ( pIncorrectMatchingEndingAnimation ); 
	m_pIncorrectMatchingEndingAction->retain ( );

	// starting matching texture, used by BoardPiecePlace
	m_pStartingMatchingTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "starting_matching_sequence_0001.png" );	

	// pivot node for BoardPiecePlace
	m_pBoardPiecePlacesPivotNode = BoardPiecePlacesPivotNode::create ( );
	this->addChild ( m_pBoardPiecePlacesPivotNode );

	// add 7*7 BoardPiecePlace, each one has a node, and this node is also taken by Board
	// each one with a nodes with their position offsets to create 49 nodes
	// also store it in m_boardPiecePlaces matrix
	KDfloat		fCorrectedBoardWidth  = m_tBoardRect.size.width  - BORDER_MARGIN_IN_PIXELS * 2;
	KDfloat		fCorrectedBoardHeight = m_tBoardRect.size.height - BORDER_MARGIN_IN_PIXELS * 2;
	KDfloat		fPieceWidth  = fCorrectedBoardWidth  / NUMBER_OF_COLUMNS_IN_BOARD;
	KDfloat		fPieceHeight = fCorrectedBoardHeight / NUMBER_OF_ROWS_IN_BOARD;
	m_pBoardPiecePlaces = new CCArray ( ); 
	
	for ( KDuint uColumn = 0; uColumn < NUMBER_OF_COLUMNS_IN_BOARD; uColumn++ )
	{
		CCArray*	pRows = CCArray::create ( );
		
		for ( KDuint uRow = 0; uRow < NUMBER_OF_ROWS_IN_BOARD; uRow++ )
		{			
			KDfloat		x = fPieceWidth * 0.5f + fPieceWidth * uColumn - fCorrectedBoardWidth * 0.5f;
			KDfloat		y = fPieceHeight * 0.5f + fPieceHeight * uRow - fCorrectedBoardHeight * 0.5f;
			CCSize		tSize;

			tSize.cx = fPieceWidth;
			tSize.cy = fPieceHeight;

			BoardPiecePlace*	pBoardPiecePlace = BoardPiecePlace::create ( this, uColumn, uRow, tSize );
			pBoardPiecePlace->setPosition ( ccp ( x, y ) );
			m_pBoardPiecePlacesPivotNode->addChild ( pBoardPiecePlace );
			pRows->addObject ( pBoardPiecePlace );
		}		

		m_pBoardPiecePlaces->addObject ( pRows );
	}

	// init vars
	m_pPlayingScene = pPlayingScene;

	this->reset ( );	

	return KD_TRUE;
}

KDvoid Board::onEnter ( KDvoid )
{
	CCNode::onEnter ( );

	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_TRUE );
}

KDvoid Board::onExit ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->removeDelegate ( this );
	CCNode::onExit ( );
}

KDbool Board::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	for ( KDuint uColumn = 0; uColumn < NUMBER_OF_COLUMNS_IN_BOARD; uColumn++ )
	{
		for ( KDuint uRow = 0; uRow < NUMBER_OF_ROWS_IN_BOARD; uRow++ )
		{
			BoardPiecePlace*	pBoardPiece = this->getBoardPiecePlaceAtColumn ( uColumn, uRow );
			if ( pBoardPiece->processTouch ( pTouch ) )
			{
				return KD_TRUE;
			}
		}
	}

	return KD_FALSE;
}

BoardPiecePlace* Board::getBoardPiecePlaceAtColumn ( KDuint uColumn, KDuint uRow )
{
	CCAssert ( uColumn < NUMBER_OF_COLUMNS_IN_BOARD, "column must be less than NUMBER_OF_COLUMNS_IN_BOARD" );
	CCAssert ( uRow < NUMBER_OF_ROWS_IN_BOARD, "row must be less than NUMBER_OF_ROWS_IN_BOARD" );
	CCArray*			pRows = (CCArray*) m_pBoardPiecePlaces->objectAtIndex ( uColumn );
	BoardPiecePlace*	pBoardPiecePlace = (BoardPiecePlace*) pRows->objectAtIndex ( uRow );
	return pBoardPiecePlace;
}

Piece* Board::getPiece ( KDuint uPiece, KDuint uGirl )
{
	CCAssert ( uPiece < NUMBER_OF_PIECES_PER_GIRL, "piece must be less than NUMBER_OF_PIECES_PER_GIRL" );
	CCAssert ( uGirl < NUMBER_OF_GIRLS, "girl must be less than NUMBER_OF_GIRLS" );
	CCArray*	pPieces = (CCArray*) m_pPiecesDataBase->objectAtIndex ( uGirl );
	Piece*		pPieceInstance = (Piece*) pPieces->objectAtIndex ( uPiece );
	return pPieceInstance;
}

// Resets all Board vars.
KDvoid Board::reset ( KDvoid )
{
	m_pSelectedBoardPiecePlace = KD_NULL;
	m_uNumberOfRegenerations = 0;
	m_uNumberOfMatches = 0;
	m_eState = WAITING_USER_INPUT_WO_CHIP_SELECTED;
	for ( KDuint uColumn = 0; uColumn < NUMBER_OF_COLUMNS_IN_BOARD; uColumn++ )
	{
		for ( KDuint uRow = 0; uRow < NUMBER_OF_ROWS_IN_BOARD; uRow++ )
		{
			this->getBoardPiecePlaceAtColumn ( uColumn, uRow )->reset ( );
		}
	}
}

// Resets all BoardPiecePlaces and gives them new Pieces.
KDvoid Board::regenerate ( KDuint uLevel )
{	
	CCLOG ( "Regenerating board for level %d", uLevel );

	// reset all BoardPiecePlace
	for ( KDuint uColumn = 0; uColumn < NUMBER_OF_COLUMNS_IN_BOARD; uColumn++ )
	{
		for ( KDuint uRow = 0; uRow < NUMBER_OF_ROWS_IN_BOARD; uRow++ )
		{
			BoardPiecePlace*	pBoardPiecePlace = this->getBoardPiecePlaceAtColumn ( uColumn, uRow );
			pBoardPiecePlace->reset ( );			
		}
	}
	
	KDuint		uCurrrentGirl = uLevel % NUMBER_OF_GIRLS;
	CCArray*	pAuxPieces = CCArray::create ( );

	// how many correct pairs?
	KDuint		uPairs = MATCHES_PER_REGENERATION;
	if ( uLevel < MEDIUM_MODE_STARTS_IN_LEVEL )
	{
		uPairs = MATCHES_PER_REGENERATION + EXTRA_PAIRS_IN_EASY_MODE;
	}
	else if ( uLevel >= MEDIUM_MODE_STARTS_IN_LEVEL && uLevel < HARD_MODE_STARTS_IN_LEVEL )
	{
		uPairs = MATCHES_PER_REGENERATION + EXTRA_PAIRS_IN_MEDIUM_MODE;
	}
	else
	{ 
		// uLevel >= HARD_MODE_STARTS_IN_LEVEL
		uPairs = MATCHES_PER_REGENERATION + EXTRA_PAIRS_IN_HARD_MODE;
	}

	// create 4 matches using current girl and place them in random positions
	for ( KDuint i = 0; i < uPairs; i++ )
	{
		Piece*		pRandomCurrentGirlPiece = this->getPiece ( kdRand ( )  % NUMBER_OF_PIECES_PER_GIRL, uCurrrentGirl );
		pAuxPieces->addObject ( pRandomCurrentGirlPiece );
		pAuxPieces->addObject ( pRandomCurrentGirlPiece );
	}

	// add some wildcards
	KDuint		uNumberOfWildcards = this->getNumberOfWildcardsForLevel ( uLevel );
	for ( KDuint i = 0; i < uNumberOfWildcards; i++ )
	{
		pAuxPieces->addObject ( m_pWildcardPiece );
	}
	
	// add random pairs (not with current girl)
	while ( pAuxPieces->count ( ) < NUMBER_OF_COLUMNS_IN_BOARD * NUMBER_OF_ROWS_IN_BOARD )
	{
		KDuint		uGirl = kdRand ( ) % NUMBER_OF_GIRLS;
		if ( uCurrrentGirl == uGirl )
		{
			continue;
		}

		Piece*		pPieceInstance = this->getPiece ( kdRand ( ) % NUMBER_OF_PIECES_PER_GIRL, uGirl ); 
		pAuxPieces->addObject ( pPieceInstance );
		pAuxPieces->addObject ( pPieceInstance );
	}

	// there must be exactly NUMBER_OF_COLUMNS_IN_BOARD*NUMBER_OF_ROWS_IN_BOARD pieces (maybe there are more)
	while ( pAuxPieces->count ( ) != NUMBER_OF_COLUMNS_IN_BOARD * NUMBER_OF_ROWS_IN_BOARD )
	{
		pAuxPieces->removeLastObject ( );
	}

	// shuffle or randomize this array
	for ( KDint i = pAuxPieces->count ( ); i > 1; i-- )
	{
		pAuxPieces->exchangeObjectAtIndex ( i - 1, kdRand ( ) % i );			// introduces modulo bias (see below)
	}

	for ( KDuint i = 0; i < pAuxPieces->count ( ); i++ )
	{

	}

	// fill with random pieces
	KDuint		i = 0;
	for ( KDuint uColumn = 0; uColumn < NUMBER_OF_COLUMNS_IN_BOARD; uColumn++ )
	{
		for ( KDuint uRow = 0; uRow < NUMBER_OF_ROWS_IN_BOARD; uRow++ )
		{
			BoardPiecePlace*	pBoardPiecePlace = this->getBoardPiecePlaceAtColumn ( uColumn, uRow );
			pBoardPiecePlace->setPiece ( (Piece*) pAuxPieces->objectAtIndex ( i ) );
			i++;
		}
	}
}

// A new board is generated.
KDvoid Board::inTheMiddleOfRegenerationSequenceCallback ( CCNode* pSender )
{
	KDuint		uLevel = m_pPlayingScene->getLevel ( );

	this->regenerate ( uLevel );
	
	// second part of sequence
	CCActionInterval*	pActionSecondStage = CCSpawn::create 
	(
		CCFadeIn::create ( REGENERATION_BOARD_DURATION * 0.5f ),
		KD_NULL
	);
	
	CCActionInterval*	pAction = CCSequence::create 
	(
	   pActionSecondStage,
	   CCCallFuncN::create ( this, callfuncN_selector ( Board::regenerationSquenceWasDone ) ),
	   KD_NULL 
	);
	
	m_pBoardPiecePlacesPivotNode->runAction ( pAction );
}


KDvoid Board::regenerationSquenceWasDone ( CCNode* pSender )
{
	this->changeBoardState ( WAITING_USER_INPUT_WO_CHIP_SELECTED );
}

// Next girls is used, level is incremented and a new board is generated.
KDvoid Board::inTheMiddleOfChangeGirlSequenceCallback ( CCNode* pSender )
{
	// next level
	KDuint		uLevel = m_pPlayingScene->getLevel ( );
	m_pPlayingScene->getLevelLabel ( )->setUInteger ( uLevel + 1 );

	// change timebar time
	if ( uLevel < MEDIUM_MODE_STARTS_IN_LEVEL ) 
	{
		m_pPlayingScene->getTimeBar ( )->setDuration ( TIMEBAR_DURATION_IN_EASY_MODE );
	}
	else if ( uLevel >= MEDIUM_MODE_STARTS_IN_LEVEL && uLevel < HARD_MODE_STARTS_IN_LEVEL ) 
	{
		m_pPlayingScene->getTimeBar ( )->setDuration ( TIMEBAR_DURATION_IN_MEDIUM_MODE );
	} 
	else
	{
		// level>=HARD_MODE_STARTS_IN_LEVEL
		m_pPlayingScene->getTimeBar ( )->setDuration ( TIMEBAR_DURATION_IN_HARD_MODE );
	}
	
	// next girl
	m_pPlayingScene->getBackgroundGirls ( )->incrementGirl ( );
	this->regenerate ( uLevel + 1 );
}

KDvoid Board::changeGirlSequenceWasDoneCallback ( CCNode* pSender )
{
	this->changeBoardState ( WAITING_USER_INPUT_WO_CHIP_SELECTED );
}

// Called by boardPiecePlaceWasSelected after a correct match was done. 
KDvoid Board::correctMatchWasDone ( KDbool bWithWildcard, BoardPiecePlace* pBoardPiecePlace )
{
	// more points 
	if ( bWithWildcard ) 
	{
		m_pPlayingScene->getScoreLabel ( )->setUInteger ( POINTS_ADDED_WHEN_WILDCARD_MATCH + m_pPlayingScene->getScoreLabel ( )->getUInteger ( ) );
	}
	else 
	{
		m_pPlayingScene->getScoreLabel ( )->setUInteger ( POINTS_ADDED_WHEN_NORMAL_MATCH + m_pPlayingScene->getScoreLabel ( )->getUInteger ( ) );
	}

	// more time
	KDuint		uLevel = m_pPlayingScene->getLevelLabel ( )->getUInteger ( );
	
	if ( uLevel < MEDIUM_MODE_STARTS_IN_LEVEL )
	{
		// easy
		m_pPlayingScene->getTimeBar ( )->addSomeTime ( TIME_GIVEN_IN_EASY_MODE );	
	}
	else if ( uLevel >= MEDIUM_MODE_STARTS_IN_LEVEL && uLevel < HARD_MODE_STARTS_IN_LEVEL )
	{
		// medium
		m_pPlayingScene->getTimeBar ( )->addSomeTime ( TIME_GIVEN_IN_MEDIUM_MODE );
	} 
	else 
	{
		// hard level >= HARD_MODE_STARTS_IN_LEVEL
		m_pPlayingScene->getTimeBar ( )->addSomeTime ( TIME_GIVEN_IN_HARD_MODE );
	}
	m_uNumberOfMatches++;

	// start corrrect matching animation in both places
	m_pSelectedBoardPiecePlace->correctMatching ( );
	pBoardPiecePlace->correctMatching ( );

	if ( m_uNumberOfMatches == MATCHES_PER_REGENERATION  && m_uNumberOfRegenerations < REGENERATIONS_PER_GIRL )
	{
		// regeneration
		CCActionInterval*	pActionFirstStage = CCSpawn::create 
		(
			CCFadeOut::create ( REGENERATION_BOARD_DURATION * 0.5f ),
			KD_NULL 
		);
		
		CCActionInterval*	pAction = CCSequence::create 
		(
			pActionFirstStage,
			CCCallFuncN::create ( this, callfuncN_selector ( Board::inTheMiddleOfRegenerationSequenceCallback ) ),
			KD_NULL
		);
		
		// note: second stage will be done in inTheMiddleOfRegenerationSequenceCallback in order to avoid a time problem 
		
		m_pBoardPiecePlacesPivotNode->runAction ( pAction );
		this->changeBoardState ( REGENERATING_BOARD );
	}
	else if ( m_uNumberOfMatches == MATCHES_PER_REGENERATION && m_uNumberOfRegenerations == REGENERATIONS_PER_GIRL )
	{		 
		 // change girl (or game over in free version if 3 girls were played)

		 // is free version?
		 KDbool		bFreeVersion = KD_FALSE;
#ifdef FREE_VERSION
		 bFreeVersion = KD_TRUE;
#endif
		 		 
		if ( m_pPlayingScene->getLevel ( ) + 1 == LIMIT_GIRLS_IN_FREE_VERSION && bFreeVersion ) 
		{
			// force game over if free version after 3 girls or levels
			m_pPlayingScene->playedTooMuch ( );			 		
		}
		else 
		{
			// normal change girl

			CCActionInterval*	pAction = CCSequence::create
			(
				CCShow::create ( ),
				CCFadeIn::create ( CHANGE_GIRL_DURATION * 0.5f ),
				CCCallFuncN::create ( this, callfuncN_selector ( Board::inTheMiddleOfChangeGirlSequenceCallback ) ),
				CCFadeOut::create ( CHANGE_GIRL_DURATION * 0.5f ),
				CCCallFuncN::create ( this, callfuncN_selector ( Board::changeGirlSequenceWasDoneCallback ) ),
				CCHide::create ( ),
				KD_NULL
			);

			m_pPlayingScene->getGirlTransition ( )->runAction ( pAction );
			this->changeBoardState ( CHANGING_GIRL );
		}
	}
	else 
	{
		// normal match (with or wo wildcard), m_numberOfMatches<MATCHES_PER_REGENERATION
		 if ( bWithWildcard )
		 {
			 SoundManager::sharedSoundManager ( )->playSoundFxCorrectMatchingWithWildcard ( );
		 } 
		 else 
		 {
			 SoundManager::sharedSoundManager ( )->playSoundFxCorrectMatching ( );
		 }
		 this->changeBoardState ( WAITING_USER_INPUT_WO_CHIP_SELECTED );			
	}
}

// Called by boardPiecePlaceWasSelected when an incorrect match is done. 
KDvoid Board::incorrectMatchWasDone ( BoardPiecePlace* pBoardPiecePlace )
{
	CCLOG ( "Incorrect match." );
	
	// less time
	KDuint	uLevel = m_pPlayingScene->getLevelLabel ( )->getUInteger ( );
	
	if ( uLevel < MEDIUM_MODE_STARTS_IN_LEVEL )
	{
		// easy
		m_pPlayingScene->getTimeBar ( )->removeSomeTime ( TIME_REMOVED_IN_EASY_MODE );		
	}
	else if ( uLevel >= MEDIUM_MODE_STARTS_IN_LEVEL && uLevel < HARD_MODE_STARTS_IN_LEVEL )
	{
		// medium
		m_pPlayingScene->getTimeBar ( )->removeSomeTime ( TIME_REMOVED_IN_EASY_MODE );
	}
	else 
	{
		// hard level >= HARD_MODE_STARTS_IN_LEVEL
		m_pPlayingScene->getTimeBar ( )->removeSomeTime ( TIME_REMOVED_IN_EASY_MODE );
	}

	m_pSelectedBoardPiecePlace->incorrectMatching ( );
	pBoardPiecePlace->incorrectMatching ( );
	SoundManager::sharedSoundManager ( )->playSoundFxIncorrectMatching ( );
	this->changeBoardState ( WAITING_USER_INPUT_WO_CHIP_SELECTED );
}

// Gets a random wildcard number for this level.
KDuint Board::getNumberOfWildcardsForLevel ( KDuint uLevel )
{
	KDfloat		p3wc = 0;
	KDfloat		p2wc = 0;
	KDfloat		p1wc = 0;

	if ( uLevel < MEDIUM_MODE_STARTS_IN_LEVEL )
	{
		// EASY

		p3wc = 0.1f;
		p2wc = 0.25f;
		p1wc = 0.5f;	
	}
	else if ( uLevel >= MEDIUM_MODE_STARTS_IN_LEVEL && uLevel < HARD_MODE_STARTS_IN_LEVEL )
	{
		// MEDIUM

		p3wc = 0.1f;
		p2wc = 0.1f;
		p1wc = 0.2f;			
	} 
	else 
	{
		// HARD
		// uLevel >= HARD_MODE_STARTS_IN_LEVEL
		p3wc = 0;
		p2wc = 0;
		p1wc = 0;
	}
	
	// between 0 and 10000
	KDuint		uAux = kdRand ( ) % ( 10000 + 1 );
	KDfloat		r = uAux / 10000.0f;
	
	// 3 wildcards
	if ( r >= 0 && r < p3wc ) 
	{
		return 3;		
	
	}
	else if ( r >= p3wc && r < p3wc + p2wc ) 
	{
		// 2 wildcards
		return 2;		
	} 
	else if ( r >= p3wc + p2wc && r < p3wc + p2wc + p1wc )
	{
		// 1 wildcard
		return 1;	
	} 

	// no wildcards ( r >= p3wc + p2wc + p1wc && <=1 )
	return 0;
}

// Called by BoardPiecePlace in ccTouchBegin. 
KDbool Board::canBoardPiecePlaceBeSelected ( KDvoid )
{
	return m_pPlayingScene->isPlaying ( ) && ( m_eState == WAITING_USER_INPUT_WO_CHIP_SELECTED || m_eState == WAITING_USER_INPUT_WITH_CHIP_SELECTED );	
}

// Called by BoardPiecePlace when it user taps it.
KDvoid Board::boardPiecePlaceWasSelected ( BoardPiecePlace* pBoardPiecePlace )
{
	// there was no BoardPiecePlace selected
	if ( m_eState == WAITING_USER_INPUT_WO_CHIP_SELECTED && m_pSelectedBoardPiecePlace == KD_NULL )
	{
		CCLOG ( "There was nothing selected." );
		pBoardPiecePlace->select ( );
		m_pSelectedBoardPiecePlace = pBoardPiecePlace;
		this->changeBoardState ( WAITING_USER_INPUT_WITH_CHIP_SELECTED );			
	} 
	else if ( m_eState == WAITING_USER_INPUT_WITH_CHIP_SELECTED && m_pSelectedBoardPiecePlace != KD_NULL )
	{
		// there was a BoardPiecePlace selected

		CCLOG ( "Something was selected." );
		
		Piece*		pPieceA = m_pSelectedBoardPiecePlace->getCurrentPiece ( );
		Piece*		pPieceB = pBoardPiecePlace->getCurrentPiece ( );
		KDuint		uCurrentGirl = m_pPlayingScene->getBackgroundGirls ( )->getCurrentGirlNumber ( );
				
		if ( pBoardPiecePlace == m_pSelectedBoardPiecePlace )
		{
			// is it the same BoardPiecePlace that was already selected?

			CCLOG ( "Unselecting a BoardPiecePlace that was already selected." );			
			SoundManager::sharedSoundManager ( )->playSoundFxChipWasUnselected ( );
			pBoardPiecePlace->unselect ( );
			this->changeBoardState ( WAITING_USER_INPUT_WO_CHIP_SELECTED );		
		} 
		else if ( pPieceA != m_pWildcardPiece &&
				  pPieceB != m_pWildcardPiece &&
				  pPieceA->getGirl ( ) == uCurrentGirl &&
				  pPieceB->getGirl ( ) == uCurrentGirl &&
				  pPieceA->getPiece ( ) == pPieceB->getPiece ( ) ) 
		{
			// a normal correct match (no wildcards involved), both pieces belong to the current girl and both contain the same piece
			this->correctMatchWasDone ( KD_FALSE, pBoardPiecePlace );					
		}
		else if ( ( pPieceA == m_pWildcardPiece && pPieceB->getGirl ( ) == uCurrentGirl ) ||
				  ( pPieceB == m_pWildcardPiece && pPieceA->getGirl ( ) == uCurrentGirl ) ||
				  ( pPieceB == m_pWildcardPiece && pPieceA == m_pWildcardPiece ) ) 
		{
			// a corrrect match using a wildcard (or two)			
			this->correctMatchWasDone ( KD_TRUE, pBoardPiecePlace );					
		} 
		else 
		{
			// there is an incorrect match
			this->incorrectMatchWasDone ( pBoardPiecePlace );
		}
	} 
	else 
	{
		// something strange
		CCAssert ( KD_FALSE, "Unknown state in boardPiecePlaceWasSelected." );		
	}
}

KDvoid Board::changeBoardState ( BoardState eState )
{
	this->onExitBoardState ( m_eState );
	
	if ( m_eState == WAITING_USER_INPUT_WO_CHIP_SELECTED && eState == WAITING_USER_INPUT_WITH_CHIP_SELECTED )
	{
		
	}
	else if ( m_eState == WAITING_USER_INPUT_WITH_CHIP_SELECTED && eState == WAITING_USER_INPUT_WO_CHIP_SELECTED ) 
	{
		
	}
	else if ( m_eState == WAITING_USER_INPUT_WITH_CHIP_SELECTED && eState == CHANGING_GIRL ) 
	{
			
	}
	else if ( m_eState == WAITING_USER_INPUT_WITH_CHIP_SELECTED && eState == REGENERATING_BOARD ) 
	{
		
	}
	else if ( m_eState == REGENERATING_BOARD && eState == WAITING_USER_INPUT_WO_CHIP_SELECTED ) 
	{
		
	}
	else if ( m_eState == CHANGING_GIRL && eState == WAITING_USER_INPUT_WO_CHIP_SELECTED ) 
	{
	
	}
	else 
	{
		CCAssert ( KD_FALSE, "Unknown transition.");
	}

	m_eState = eState;
	this->onEnterBoardState ( eState );
}

KDvoid Board::onEnterBoardState ( BoardState eState )
{
	switch ( eState )
	{
		case REGENERATING_BOARD :
			SoundManager::sharedSoundManager ( )->playSoundFxRegeneration ( );
			m_uNumberOfMatches = 0;
			m_uNumberOfRegenerations++;
			m_pPlayingScene->getTimeBar ( )->deactivate ( );
			break;

		case CHANGING_GIRL :
			SoundManager::sharedSoundManager ( )->playSoundFxNewGirl ( );
			m_uNumberOfMatches = 0;
			m_uNumberOfRegenerations = 0;
			m_pPlayingScene->getTimeBar ( )->deactivate ( );
			break;

		case WAITING_USER_INPUT_WO_CHIP_SELECTED :
			m_pSelectedBoardPiecePlace = KD_NULL;
			break;

		case WAITING_USER_INPUT_WITH_CHIP_SELECTED :
			SoundManager::sharedSoundManager ( )->playSoundFxSelectedChip ( );
			break;

		default :
			CCAssert ( KD_FALSE, "Unknown state in onEnterBoardState." );
			break;
	}
}

KDvoid Board::onExitBoardState ( BoardState eState )
{
	switch ( eState )
	{
		case REGENERATING_BOARD :
			m_pPlayingScene->getTimeBar ( )->activate ( );
			break;

		case CHANGING_GIRL :
			m_pPlayingScene->getTimeBar ( )->activate ( );
			break;

		case WAITING_USER_INPUT_WO_CHIP_SELECTED :
			break;

		case WAITING_USER_INPUT_WITH_CHIP_SELECTED :
			SoundManager::sharedSoundManager ( )->playSoundFxSelectedChip ( );
			break;

		default :
			CCAssert ( KD_FALSE, "Unknown state in onExitBoardState.");
			break;
	}
}

CCTexture2D* Board::getSelectedTexture ( KDvoid )
{
	return m_pSelectedTexture;
}

KDbool Board::containsTouchLocation ( CCTouch* pTouch )
{
	return m_tBoardRect.containsPoint ( this->convertTouchToNodeSpaceAR ( pTouch ) );
}

CCTexture2D* Board::getStartingMatchingTexture ( KDvoid )
{
	return m_pStartingMatchingTexture;
}

CCRepeatForever* Board::getSelectedAction ( KDvoid )
{
	return m_pSelectedAction;
}

CCAnimate* Board::getStartingMatchingAction ( KDvoid )
{
	return m_pStartingMatchingAction;
}

CCAnimate* Board::getCorrectMatchingEndingAction ( KDvoid )
{
	return m_pCorrectMatchingEndingAction;
}

CCAnimate* Board::getIncorrectMatchingEndingAction ( KDvoid )
{
	return m_pIncorrectMatchingEndingAction;
}

