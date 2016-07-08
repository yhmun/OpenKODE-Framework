/* --------------------------------------------------------------------------
 *
 *      File            SimpleTimedGameRecipe.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __SimpleTimedGameRecipe_h__
#define __SimpleTimedGameRecipe_h__

enum 				// Game state
{
	MID_GAME			,
	GAME_OVER			,
};

enum 
{
	Z_TIMED_GAME_HUD	= 20,
};

class SimpleTimedGameRecipe : public Recipe
{
	public :

		static SimpleTimedGameRecipe*		create ( KDvoid );

	public :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			onExit ( KDvoid );

		virtual KDvoid			step ( KDfloat fDelta );

		virtual KDvoid			loadHiScores ( KDvoid );
		
		virtual KDvoid			deleteHiScores ( CCObject* pSender );
		
		virtual KDvoid			addHiScore ( KDvoid );
		
		virtual KDvoid			viewHiScores ( CCObject* pSender );		
		virtual KDvoid			hideHiScores ( CCObject* pSender );

		virtual KDvoid			addHiScoresToMenu ( KDvoid );
		
		virtual KDvoid			setHiScore ( KDvoid );
		
		virtual KDvoid			setCurrentScore ( KDvoid );
		
		virtual KDvoid			setPlayerName ( KDvoid );
		
		virtual KDvoid			startNewGame ( CCObject* pSender = KD_NULL );
		
		virtual KDvoid			gameOver ( KDvoid );

		virtual KDvoid			showNames ( CCObject* pSender );
		
		virtual KDvoid			chooseName1 ( CCObject* pSender );		
		virtual KDvoid			chooseName2 ( CCObject* pSender );		
		virtual KDvoid			chooseName3 ( CCObject* pSender );

		virtual KDvoid			chooseName ( const KDchar* szName );

	protected :

		KDint					m_nCurrentScore;
		std::string				m_sCurrentPlayerName;
		KDint					m_nHiScore;
		CCArray*				m_pHiScores;
		CCLabelBMFont*			m_pHiScoreNumberLabel;
		CCLabelBMFont*			m_pCurrentScoreNumberLabel;
		CCLabelBMFont*			m_pPlayerNameStringLabel;
		CCMenu*					m_pMainMenu;
		CCMenu*					m_pNamesToChoose;
		CCMenu*					m_pHiScoresMenu;
		CCSprite*				m_pDarkSprite;
		KDfloat					m_fGameTimer;
		CCLabelBMFont*			m_pGameTimeNumberLabel;
		KDint					m_nGameState;
};

#endif	// __SimpleTimedGameRecipe_h__
