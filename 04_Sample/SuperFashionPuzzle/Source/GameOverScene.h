/* --------------------------------------------------------------------------
 *
 *      File            GameOverScene.h
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

#ifndef __GameOverScene_h__
#define __GameOverScene_h__

#include "UIntegerLabel.h"
#include "StringLabel.h"

class PlayingScene;

class GameOverScene : public CCScene 
{
	public :

		CREATE_FUNC ( GameOverScene );

	public :

		virtual	KDbool			init ( KDvoid );

		KDvoid					tryToUploadScore ( KDvoid );
		
		KDvoid					goToMainMenu ( KDvoid );

		KDvoid					goToPlayAgain ( KDvoid );

		const KDchar*			buildFacebookMessageWithPoints ( KDuint uPoints, KDuint uLevel );

		// buttons handlers
		KDvoid					submitButtonPressed ( CCObject* pSender );

		KDvoid					renameButtonPressed ( CCObject* pSender );
		
		KDvoid					playAgainButtonPressed ( CCObject* pSender );

		KDvoid					menuButtonPressed ( CCObject* pSender );

		#ifdef FREE_VERSION
		KDvoid					buyFullVersionButtonPressed ( CCObject* pSender );
		#endif

		KDvoid					loginButtonPressed ( CCObject* pSender );

		KDvoid					logoutButtonPressed ( CCObject* pSender );

		// modify labels information
		KDvoid					setHighScore ( KDuint uHighScore );

		KDvoid					setCurrentScore ( KDuint uCurrentScore );

		KDvoid					setPlayedTooMuch ( KDbool bTooMuch );
	
		KDvoid					setLevel ( KDuint uLevel );

		KDvoid					setPlayingScene ( PlayingScene* pScene );

	private :

		UIntegerLabel*			m_pCurrentScoreLabel;
		UIntegerLabel*			m_pHighScoreLabel;
		StringLabel*			m_pNameLabel;
		KDuint					m_uLevel;
		PlayingScene*			m_pPlayingScene;
		KDbool					m_bScoreWasSentProperly;
		KDbool					m_bFacebookWallWasWrittenProperly;
		KDbool					m_bPlayedTooMuch;
		CCMenu*					m_pLoginMenu;
		CCMenu*					m_pLogoutMenu;
};

#endif	// __GameOverScene_h__