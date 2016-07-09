/* --------------------------------------------------------------------------
 *
 *      File            GameManager.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 27/04/2011
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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

#ifndef __GameManager_h__
#define __GameManager_h__

#define GameMgr					GameManager::sharedManager ( )

enum GameState
{
	GameOver	,
	Play		,
	MainMenu	,
	Instructions,
	PowerUp		,
	ShootOut	,
};

#include "CoordinateFunctions.h"

class GameManager
{
	public :
				 GameManager ( KDvoid );
		virtual ~GameManager ( KDvoid );

		static GameManager*		sharedManager ( KDvoid );

	public :

		KDvoid					showMenu ( KDvoid );

		KDvoid					showInstructions ( KDvoid );

		KDvoid					startGame ( KDvoid );

		KDvoid					shootOut ( KDvoid );

	public :

		CC_SYNTHESIZE_BOOL ( m_bSoundOn, SoundOn );
		CC_SYNTHESIZE_BOOL ( m_bMusicOn, MusicOn );
		CC_SYNTHESIZE_BOOL ( m_bIsIPad , IPad    );

		CC_SYNTHESIZE_RETAIN ( CoordinateFunctions*, m_pCoordinateFunctions, CoordinateFunctions );

		CC_SYNTHESIZE ( CCTMXTiledMap*	, m_pTileMap		, TileMap		 );
		CC_SYNTHESIZE ( GameState		, m_eGameState		, GameState		 );
		CC_SYNTHESIZE ( CCLayer*		, m_eGameScene		, GameScene		 );
		CC_SYNTHESIZE ( KDint			, m_nMaxPlayerLives	, MaxPlayerLives );
		CC_SYNTHESIZE ( KDint			, m_nPlayerLives	, PlayerLives	 );
		CC_SYNTHESIZE ( KDint			, m_nLevel			, Level			 );
		CC_SYNTHESIZE ( KDint			, m_nMaxLevels		, MaxLevels		 ); 
};

#endif	// __GameManager_h__