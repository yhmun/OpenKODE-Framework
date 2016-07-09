/* --------------------------------------------------------------------------
 *
 *      File            GameManager.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
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

#include "MainMenuLayer.h"
#include "AboutGameLayer.h"
#include "GameOverLayer.h"
#include "BackgroundLayer.h"
#include "GameSceneLayer.h"
#include "GameplayLayer.h"

/**
 * Responsible for the layers and the logic of the game.
 */
class GameManager : public CCObject
{
	protected :
				 GameManager ( KDvoid );
		virtual ~GameManager ( KDvoid );

	public :

		// Singleton instance
		static GameManager*		sharedGameManager ( KDvoid );
		static KDvoid			purgedGameManager ( KDvoid );

	public :

		// Initialize the Game Manager
		KDbool			init ( KDvoid );

		// Show Main Menu
		KDvoid			initMainMenu ( KDvoid );

		KDvoid			showMainMenu ( KDvoid );
	    
		// Show About Game Menu
		KDvoid			initAboutGameMenu ( KDvoid );

		KDvoid			showAboutGame ( KDvoid );
	    
		// Start a new game
		KDvoid			startNewGame ( KDvoid );
	    
		// End game
		KDvoid			endGame ( KDvoid );
	    
		// Set the flight velocity to maximum
		KDvoid			maxVelocity ( KDvoid );
		
		// Set the flight velcoity to normal
		KDvoid			normalVelocity ( KDvoid );

		// Set the flight velocity to minimum
		KDvoid			minVelocity ( KDvoid );
	    
		// Launches a new game obstacle
		KDvoid			launchGameObstacle ( KDvoid );
	    
		// Generates a random float value between the low and high values specified
		static KDfloat	randomValueBetween ( KDfloat fLow, KDfloat fHigh );
	    
		// Generates a random int value between the low and high values specified
		static KDint	randomIntValueBetween ( KDint nLow, KDint nHigh );
	    
		// Purges the shared game manager and all objects and variable values
		static KDvoid	purgeSharedGameManager ( KDvoid );
	    
		// Increases the player score by the amount specified
		KDvoid			increaseScore ( KDint nAmount );

		// Increases the distance travelled by the amount specified
		KDvoid			increaseDistance ( KDint nAmount );

		// Take a life from the player
		KDvoid			takeLife ( KDvoid );

		// Give a life to the player
		KDvoid			giveLife ( KDvoid );
	    
		KDint			getStartingLives ( KDvoid );

		KDint			getDifficulty ( KDvoid );
	    
		// Added for audio
		KDvoid			initAudioAsync ( KDvoid );

		KDvoid			setupAudioEngine ( KDvoid );

		KDvoid			playBackgroundTrack ( const KDchar* szTrackFileName );

		KDvoid			playRandomSong ( KDvoid );

	public :

		// Whether the music is enabled or disabled
		CC_SYNTHESIZE	( KDbool, m_bIsMusicON, IsMusicON );
		
		// Whether the sound effects are enabled or disabled
		CC_SYNTHESIZE	( KDbool, m_bIsSoundEffectsON, IsSoundEffectsON );
		
		// Has the player died?
		CC_SYNTHESIZE	( KDbool, m_bHasPlayerDied, HasPlayerDied );
	    
		// The parallax scrolling background layer
		CC_SYNTHESIZE	( BackgroundLayer*, m_pBackgroundLayer, BackgroundLayer );
		
		// The layer than contains the main menu and buttons
		CC_SYNTHESIZE	( MainMenuLayer*, m_pMainMenuLayer, MainMenuLayer );
		
		// The layer that contains the about game layer and buttons
		CC_SYNTHESIZE	( AboutGameLayer*, m_pAboutGameLayer, AboutGameLayer );
		
		// The layer that displays the game over screen and buttons
		CC_SYNTHESIZE	( GameOverLayer*, m_pGameOverLayer, GameOverLayer );
		
		// The layer that contains the game scene
		CC_SYNTHESIZE	( GameSceneLayer*, m_pGameSceneLayer, GameSceneLayer );

		// The layer that contains the gameplay elements
		CC_SYNTHESIZE	( GameplayLayer*, m_pGameplayLayer, GameplayLayer );
	    
		// The current state the game is in i.e. playing, game over, etc
		CC_SYNTHESIZE	( GameState, m_eGameState, GameState );
	    
		// The current velocity of flight
		CC_SYNTHESIZE	( KDfloat, m_fVelocity, Velocity );
	    
		// The current player score
		CC_SYNTHESIZE	( KDint, m_nScore, Score );

		// The amount of lives the player has
		CC_SYNTHESIZE	( KDint, m_nLives, Lives );

		// The total distance travelled
		CC_SYNTHESIZE	( KDint, m_nDistance, Distance );
	    
		CC_SYNTHESIZE	( GameDifficulty, m_eGameDifficulty, GameDifficulty );
	    
		CC_SYNTHESIZE	( DeviceSize, m_eDeviceSize, DeviceSize );
		CC_SYNTHESIZE	( DeviceType, m_eDeviceType, DeviceType );

	protected :

		// The audio manager responsible for the music and sound
		CC_SYNTHESIZE	( AudioManagerState, m_eAudioManagerState, AudioManagerState );

		// Check if Loading has finished
		CC_SYNTHESIZE	( KDbool, m_bHasFinishedLoading, HasFinishedLoading );
	    
		CC_SYNTHESIZE	( KDint, m_nCurrentSong, CurrentSong );

		// Has the audio been initialized
		KDbool							m_bHasAudioBeenInitialized;

		// Our sound engine responsible for loading and playing music and sounds
		SimpleAudioEngine*				m_pSoundEngine;
	    
		// Our list of sound files retain!
		CCDictionary*					m_pListOfSoundEffectFiles;
		std::map<std::string, KDbool>*	m_pSoundEffectsState;
};

#endif	// __GameManager_h__
