/* --------------------------------------------------------------------------
 *
 *      File            GameData.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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

#ifndef __GameData_h__
#define __GameData_h__

class GameData
{
	public :
		
		GameData ( KDvoid );

		static GameData*	sharedData ( KDvoid );

	public :

		KDint				getLevel ( KDvoid );

		// Level Up
		KDvoid				levelUp ( KDvoid );

		KDbool				 isFirstRunEver ( KDvoid );
		KDvoid				setFirstRunEver ( KDbool bValue );

		// region METHODS TheLevel Class calls to return settings for the current level being played

		const KDchar*		getBackgroundCloudsFileName ( KDvoid );

		const KDchar*		getBackgroundHillsFileName ( KDvoid );

		const KDchar*		getGroundPlaneFileName ( KDvoid );

		KDint				getNumberOfNinjasToTossThisLevel ( KDvoid );
		
		KDint				getPointsToPassLevel ( KDvoid );

		KDint				getBonusPerExtraNinja ( KDvoid );

		KDint				getHighScoreForLevel ( KDvoid );
		KDvoid				setHighScoreForLevel ( KDint nScore );

		// LEVEL JUMPING
		KDbool				canYouGoToTheFirstLevelOfThisSection ( KDint nTheSection );

		KDvoid				changeLevelToFirstInThisSection ( KDint nTheSection );

		KDvoid				attemptToGoToFirstLevelOfSection ( KDint nTheSection );

		// RESET GAME / POINT TOTAL ALL LEVELS (not called ever) 

		KDvoid				resetGame ( KDvoid );

		KDvoid				addToPointTotalForAllLevels ( KDint nPointThisLevel );

		// sounds
		KDbool				areSoundFXMuted ( KDvoid );
		KDvoid				setSoundFXMuted ( KDbool bMute );

		KDbool				areVoiceFXMuted ( KDvoid );
		KDvoid				setVoiceFXMuted ( KDbool bMute );

		KDbool				areAmbientFXMuted ( KDvoid );
		KDvoid				setAmbientFXMuted ( KDbool bMute );

	private :
		
		KDint				m_nLevel;
		KDint				m_nMaxLevels;

		KDbool				m_bSoundFXMuted;
		KDbool				m_bVoiceFXMuted;
		KDbool				m_bAmbientFXMuted;

		KDbool				m_bFirstRunEver;

		CCUserDefault*		m_pDefaults;

		KDint				m_nPointTotalForAllLevels;

		KDint				m_nLevelsCompleted;

		KDint				m_nEachLevelSectionIsHowManyLevels;

		KDint				m_aHighScoreLevel [ MAX_LEVEL ];
};

#endif	// __GameData_h__