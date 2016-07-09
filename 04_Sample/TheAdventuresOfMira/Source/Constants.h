/* --------------------------------------------------------------------------
 *
 *      File            Constants.h
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

#ifndef __Constants_h__
#define __Constants_h__

// Constants used in the game
#define kMiraSpriteZValue				100
#define kMiraSpriteTagValue				0
#define kMaxScrollVelocity				-15.0f
#define kNormalScrollVelocity			-7.0f
#define kMinScrollVelocity				-5.0f
#define kMiraSugarRushTime				400
#define kMiraMagnetTime					200

// Screen size macros
#define SCREEN_SIZE						CCDirector::sharedDirector ( )->getWinSize ( )
#define SCREEN_WIDTH					CCDirector::sharedDirector ( )->getWinSize ( ).width
#define SCREEN_HEIGHT					CCDirector::sharedDirector ( )->getWinSize ( ).height

#define SCREEN_SIZE_PX					CCDirector::sharedDirector ( )->getWinSizeInPixels ( )
#define SCREEN_WIDTH_PX					CCDirector::sharedDirector ( )->getWinSizeInPixels ( ).width
#define SCREEN_HEIGHT_PX				CCDirector::sharedDirector ( )->getWinSizeInPixels ( ).height

// Screen resolutions for AppDelegate
#define WVGA							 800,  480
#define HVGA							 480,  320
#define IPHONE4							 960,  640
#define IPHONE5							1136,  640
#define GALAXYNEXUS						1280,  720
#define IPAD							1024,  768
#define IPAD3							2048, 1536

// Labels and stuff
#define kLabel_Lives                    4001
#define kLabel_Score                    4002
#define kLabel_Distance                 4003

// Hud
#define kHudLayer                       4500

// Game Manager
#define kGameManager_GameSceneLayer     5001
#define kGameManager_MainMenuLayer      5002
#define kGameManager_BackgroundLayer    5003
#define kGameManager_AboutGameLayer     5004
#define kGameManager_GameOverLayer      5005
#define kGameManager_GameplayLayer      5006

// Main Menu Layer
#define kMainMenuLayer_TitleSprite      6001
#define kMainMenuLayer_MainMenu         6002
#define kMainMenuLayer_StartGameMenu    6003

// GameOver Layer
#define kGameOverLayer_PlayAgainMenu    6501

// AboutGame Layer
#define kAboutGameLayer_BackMenu        6401

// Gameplay Layer
#define kGameplayLayer_GetReadySprite   7001

#define kWindTag1                       3020
#define kWindTag2                       3021

// Mira
#define kMiraMaxLives					5

enum DeviceSize 
{
    kDeviceSizeSmall					,				// iPhone, iPod Touch
    kDeviceSizeNormal					,				// iPhone4, iPhone5, Android-854x480, Android-800x480, Android-800x400, Android-720x480
    kDeviceSizeLarge					,				// iPad1, iPad2, Android-1024x600, Android-1280x800, Android-1280x800
    kDeviceSizeXLarge					,				// iPad3
};

enum DeviceType 
{
    kDeviceTypeiPhone					,				// Includes iPhone, iPod touch
    kDeviceTypeiPhone4					,				// iPhone 4
    kDeviceTypeiPhone5					,				// iPhone 5
    kDeviceTypeiPad						,				// iPad 1 & 2
    kDeviceTypeiPad3					,				// iPad 3
    kDeviceTypeAndroid1280x800			,				// Android device with 1280x800 resolution
    kDeviceTypeAndroid1280x720			,				// Android device with 1280x720 resolution
    kDeviceTypeAndroid1024x600			,				// Android device with 1024x600 resolution
    kDeviceTypeAndroid854x480			,				// Android device with 854x480 resolution
    kDeviceTypeAndroid800x480			,				// Android device with 800x480 resolution
    kDeviceTypeAndroid800x400			,				// Android device with 800x400 resolution
    kDeviceTypeAndroid720x480			,				// Android device with 729x480 resolution
};

enum GameState 
{
    kGameStateMainMenu					,
    kGameStateAboutMenu					,
    kGameStateStartGame					,
    kGameStatePlayGame					,
    kGameStateGameOver					,
    kGameStatePauseGame					,
};

enum CharacterState
{
    kCharacterStateInactive				,
    kCharacterStateSpawning				,
    kCharacterStateIdle					,
    kCharacterStateFlying				,
    kCharacterStateHit					,
    kCharacterStateDead					,
};

enum GameObjectType 
{
    kObjectTypeNone						,
    kGameObjectTypeMira					,
    kGameObjectTypePowerUpCandy			,
    kGameObjectTypePowerUpMagnet		,
    kGameObjectTypeCollectableFlower1	,
    kGameObjectTypeCollectableFlower2	,
    kGameObjectTypeCollectableFlower3	,
    kGameObjectTypeCollectableFlower4	,
    kGameObjectTypeCollectableFlower5	,
    kGameObjectTypeObstacleRedBird		,
    kGameObjectTypeObstacleBlueBird		,
    kGameObjectTypeObstacleBat			,
    kGameObjectTypeObstacleLadyBug		,
    kGameObjectTypeObstacleSquirrel		,
};

enum GameDifficulty 
{
    kGameDifficultyEasy					,
    kGameDifficultyMedium				,
    kGameDifficultyHard					,
};

#define AUDIO_MAX_WAITTIME				150

enum AudioManagerState 
{
    kAudioManagerStateUninitialized		,
    kAudioManagerStateFailed			,
    kAudioManagerStateInitializing		,
    kAudioManagerStateInitialized		,
    kAudioManagerStateLoading			,
    kAudioManagerStateReady				,
};

#define SFX_NOTLOADED					KD_FALSE
#define SFX_LOADED						KD_TRUE

#define PLAY_SOUND_EFFECT( ... )		GameManager::sharedGameManager ( )->playSoundEffect ( #__VA_ARGS__ )

#define STOP_SOUND_EFFECT( ... )		GameManager::sharedGameManager ( )->stopSoundEffect ( #__VA_ARGS__ )

// Background Music
// Menu Scenes
#define BACKGROUND_TRACK_MAIN_MENU      "MenuMusic.mp3"
// Game Play Scene
#define BACKGROUND_TRACK_GAMEPLAY_01    "GameplayMusic01.mp3"
#define BACKGROUND_TRACK_GAMEPLAY_02    "GameplayMusic02.mp3"
#define BACKGROUND_TRACK_GAMEPLAY_03    "GameplayMusic03.mp3"
// Game Over Scene
#define BACKGROUND_TRACK_GAME_OVER      "GameOverMusic.mp3"

// The pixel to meter ratio
#define PTM_RATIO						50.0

class GameplayLayerDelegate
{
	public :

		virtual KDvoid		createObjectOfType ( GameObjectType eObjectType, const CCPoint& tSpawnLocation, KDint zValue ) = 0;
};

#endif	// __Constants_h__
