/* --------------------------------------------------------------------------
 *
 *      File            PlayingScene.h
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

#ifndef __PlayingScene_h__
#define __PlayingScene_h__

#include "Board.h"
#include "BackgroundGirls.h"
#include "TimeBar.h"
#include "UIntegerLabel.h"

typedef enum 
{
	STARTING_STATE_GS	,
	READY				,
	PLAYING				,		// uses a nested state chart
	PAUSED				,
	PAUSE_BY_LOCK		,
	GAME_OVER			,
	ENDING_STATE_GS		,
} GameState;

class PlayingScene : public CCScene
{
	public :

		CREATE_FUNC ( PlayingScene );

	public :

		virtual	KDbool		init ( KDvoid );

		KDvoid				pauseButtonPressed ( CCObject* pSender );

		KDvoid				readySequenceFinished ( CCNode* pSender );

		KDvoid				playReadySoundFX ( CCNode* pSender );

		KDvoid				timeUp ( KDvoid );

		KDvoid				gameOverAnmationFinished ( CCNode* pSender );

		KDvoid				reset ( KDvoid );

		KDvoid				onResume ( KDvoid );

		KDbool				isPlaying ( KDvoid );

		KDuint				getLevel ( KDvoid );

		KDvoid				checkGameLayout ( KDvoid );

		KDvoid				applicationWillResginActive ( KDvoid );

		KDvoid				playedTooMuch ( KDvoid );

		// Game FSM managment
		KDvoid				changeGameState ( GameState eState );
		
		KDvoid				onEnterGameState ( GameState eState );

		KDvoid				onExitGameState ( GameState eState );

		TimeBar*			getTimeBar ( KDvoid );

		BackgroundGirls*	getBackgroundGirls ( KDvoid );

		UIntegerLabel*		getLevelLabel ( KDvoid );

		UIntegerLabel*		getScoreLabel ( KDvoid );

		CCSprite*			getGirlTransition ( KDvoid );

	private :

		Board*				m_pBoard;	
		CCSprite*			m_pReadySprite;
		CCSprite*			m_pGameOverSprite;
		CCSprite*			m_pGirlTransition;			// used when current girl have to be changed
	
		CCSprite*			m_pScoreAndLevelSprite;
		UIntegerLabel*		m_pScoreLabel;
		UIntegerLabel*		m_pLevelLabel;
		TimeBar*			m_pTimeBar;
		BackgroundGirls*	m_pBackgroundGirls;
		CCMenu*				m_pPauseMenu;
		KDbool				m_bPlayedTooMuch;
		
		// vars for "game" FSM
		GameState			m_eState;
};

#endif	// __PlayingScene_h__