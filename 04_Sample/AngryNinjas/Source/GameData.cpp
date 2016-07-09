/* --------------------------------------------------------------------------
 *
 *      File            GameData.cpp
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

#include "Precompiled.h"
#include "GameData.h"

static GameData*	l_pShared = KD_NULL;

GameData* GameData::sharedData ( KDvoid )
{
	if ( !l_pShared )
	{
		l_pShared = new GameData ( );
	}

	return l_pShared;
}

GameData::GameData ( KDvoid )
{
	m_bFirstRunEver						= KD_TRUE;

	m_pDefaults							= CCUserDefault::sharedUserDefault ( );

	m_bSoundFXMuted						= m_pDefaults->getBoolForKey ( "soundFXMutedKey"   );	// will default to NO if there's no previous default value
	m_bVoiceFXMuted						= m_pDefaults->getBoolForKey ( "voiceFXMutedKey"   );	// will default to NO if there's no previous default value
	m_bAmbientFXMuted					= m_pDefaults->getBoolForKey ( "ambientFXMutedKey" );	// will default to NO if there's no previous default value

	m_nPointTotalForAllLevels			= 0;

	m_nLevelsCompleted					= m_pDefaults->getIntegerForKey ( "levelsCompletedAllTime" );

	m_nLevel							= 1;													// use 0 to show a testbed of shapes
	m_nMaxLevels						= MAX_LEVEL;											// change to whatever, could be more or less

	CCLOG ( " Levels completed over all time: %d", m_nLevelsCompleted );

	m_nEachLevelSectionIsHowManyLevels	= 1;
	
	for ( KDuint i = 0; i < MAX_LEVEL; i++ )
	{
		m_aHighScoreLevel [ i ] = m_pDefaults->getIntegerForKey ( ccszf ( "highScoreLevel%d", i + 1 ) );
	}
}

KDint GameData::getLevel ( KDvoid )
{
	return m_nLevel;
}

KDvoid GameData::levelUp ( KDvoid )
{
	m_nLevel++;

	if ( m_nLevel > m_nLevelsCompleted ) 
	{
		m_nLevelsCompleted = m_nLevel - 1;

		m_pDefaults->setIntegerForKey ( "levelsCompletedAllTime", m_nLevelsCompleted );
		m_pDefaults->flush ( );

		CCLOG ( "Level {0} completed", m_nLevelsCompleted );
	}
	else 
	{
		CCLOG ( "That level was completed before" );
	}

	if ( m_nLevel > m_nMaxLevels )
	{
		m_nLevel = 1;
	}
}

KDbool GameData::isFirstRunEver ( KDvoid )
{
	return m_bFirstRunEver;
}

KDvoid GameData::setFirstRunEver ( KDbool bValue )
{
	m_bFirstRunEver = bValue;
}

const KDchar* GameData::getBackgroundCloudsFileName ( KDvoid )
{
	switch ( m_nLevel )
	{
		case 0 :	return "background_clouds.png";			
		case 1 :	return "background_clouds.png";	
		case 2 :	return "background_clouds.png";
		
		// keep adding new cases for each level, the default for all levels after 2 is below  (these are all the same file anyway)		
		default :	return "background_clouds.png";
	}
}

const KDchar* GameData::getBackgroundHillsFileName ( KDvoid )
{
	switch ( m_nLevel )
	{
		case 0 :	return "background_hills.png";			
		case 1 :	return "background_hills.png";	
		case 2 :	return "background_hills.png";
		
		// keep adding new cases for each level, the default for all levels after 2 is below (these are all the same file anyway)
		default :	return "background_hills.png";
	}
}

const KDchar* GameData::getGroundPlaneFileName ( KDvoid )
{
	switch ( m_nLevel )
	{
		case 0 :	return "ground_plane.png";			
		case 1 :	return "ground_plane.png";	
		case 2 :	return "ground_plane.png";
		
		// keep adding new cases for each level, the default for all levels after 2 is below (these are all the same file anyway)
		default :	return "ground_plane.png";
	}
}

KDint GameData::getNumberOfNinjasToTossThisLevel ( KDvoid )
{
	switch ( m_nLevel )
	{
		case 0 :	return 5;			
		case 1 :	return 2;	
		case 2 :	return 5;
		
		// keep adding new cases for each level, the default for all levels after 2 is below
		default :	return 5;
	}
}

KDint GameData::getPointsToPassLevel ( KDvoid )
{
	switch ( m_nLevel )
	{
		case 0 :	return 8000;			
		case 1 :	return 9000;	
		case 2 :	return 20000;
		
		// keep adding new cases for each level, the default for all levels after 2 is below
		default :	return 10000;
	}
}

KDint GameData::getBonusPerExtraNinja ( KDvoid )
{
	switch ( m_nLevel )
	{
		case 0 :	return 10000;			
		case 1 :	return 10000;	
		case 2 :	return 10000;
		
		// keep adding new cases for each level, the default for all levels after 2 is below
		default :	return 10000;
	}
}

KDint GameData::getHighScoreForLevel ( KDvoid )
{
	return m_aHighScoreLevel [ m_nLevel ];
}

KDvoid GameData::setHighScoreForLevel ( KDint nScore )
{
	CCLOG ( "checking to see if %d is a new high score", nScore );
	
	if ( m_nLevel < MAX_LEVEL && nScore > m_aHighScoreLevel [ m_nLevel ] )
	{
		m_aHighScoreLevel [ m_nLevel ] = nScore;
		m_pDefaults->setIntegerForKey ( ccszf ( "highScoreLevel%d", m_nLevel ), nScore );
		m_pDefaults->flush ( );
	}	
}

KDbool GameData::canYouGoToTheFirstLevelOfThisSection ( KDint nTheSection )
{
	KDint	nThePreviousSection = nTheSection - 1;
	
	if ( m_nLevelsCompleted >= ( nThePreviousSection * m_nEachLevelSectionIsHowManyLevels ) )
	{		
		return KD_TRUE;
	}  
	else
	{		
		CCLOG ( "you need to pass level %d before jumping to here",  ( nThePreviousSection * m_nEachLevelSectionIsHowManyLevels ) );
		return KD_FALSE;		
	}
}

KDvoid GameData::changeLevelToFirstInThisSection ( KDint nTheSection )
{
	KDint	nThePreviousSection = nTheSection - 1;

	m_nLevel = ( nThePreviousSection * m_nEachLevelSectionIsHowManyLevels ) + 1;
	CCLOG ( "Level now equals %d, which is the first level in Section: %d", m_nLevel, nTheSection );
}

KDvoid GameData::attemptToGoToFirstLevelOfSection ( KDint nTheSection )
{
	if ( this->canYouGoToTheFirstLevelOfThisSection ( nTheSection ) ) 
	{	
		this->changeLevelToFirstInThisSection ( nTheSection );		
	}
}

KDvoid GameData::resetGame ( KDvoid )
{
	// this method never gets called in my version. Not really a need to since I'm not showing the pointTotalForAllLevels ever
	m_nLevel = 1;
	m_nPointTotalForAllLevels = 0;
}

KDvoid GameData::addToPointTotalForAllLevels ( KDint nPointThisLevel )
{
	// this method gets called, but at no point am I ever showing the pointTotalForAllLevels
	m_nPointTotalForAllLevels += nPointThisLevel;
}

KDbool GameData::areSoundFXMuted ( KDvoid )
{
	return m_bSoundFXMuted;
}

KDvoid GameData::setSoundFXMuted ( KDbool bMute )
{
	m_bSoundFXMuted = bMute;
	m_pDefaults->setBoolForKey ( "soundFXMutedKey", bMute );
	m_pDefaults->flush ( );
}

KDbool GameData::areVoiceFXMuted ( KDvoid )
{
	return m_bVoiceFXMuted;
}

KDvoid GameData::setVoiceFXMuted ( KDbool bMute )
{
	m_bVoiceFXMuted = bMute;
	m_pDefaults->setBoolForKey ( "voiceFXMutedKey", bMute );
	m_pDefaults->flush ( );
}

KDbool GameData::areAmbientFXMuted ( KDvoid )
{
	return m_bAmbientFXMuted;
}

KDvoid GameData::setAmbientFXMuted ( KDbool bMute )
{
	m_bAmbientFXMuted = bMute;
	m_pDefaults->setBoolForKey ( "ambientFXMutedKey", bMute );
	m_pDefaults->flush ( );
}