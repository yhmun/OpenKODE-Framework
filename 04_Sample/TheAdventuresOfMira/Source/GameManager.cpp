/* --------------------------------------------------------------------------
 *
 *      File            GameManager.cpp
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "GameManager.h"
//#include "GameObstacle.h"
#include "HudLayer.h"

static GameManager*		l_pSharedGameManager = KD_NULL;
static KDbool			l_bFirstRun = KD_TRUE;

GameManager::GameManager ( KDvoid  )
{
	m_pListOfSoundEffectFiles	= KD_NULL;
	m_pSoundEffectsState		= KD_NULL;
}

GameManager::~GameManager ( KDvoid  )
{
	CC_SAFE_RELEASE ( m_pListOfSoundEffectFiles  );
    //CC_SAFE_RELEASE ( soundEffectsState );
}

GameManager* GameManager::sharedGameManager ( KDvoid  )
{
    if ( l_bFirstRun  )
    {
        if ( !l_pSharedGameManager  )
        {
            l_pSharedGameManager = new GameManager ( );
            l_pSharedGameManager->init ( );
        }
        l_bFirstRun = KD_FALSE;
    }
    return l_pSharedGameManager;
}

KDvoid GameManager::purgedGameManager ( KDvoid  )
{
	// Safe release for sharedGameManager
	CC_SAFE_RELEASE ( l_pSharedGameManager  );
}

KDbool GameManager::init ( KDvoid  )
{
    this->minVelocity ( );

    m_bIsMusicON				= KD_TRUE;
    m_bIsSoundEffectsON			= KD_TRUE;
    m_bHasPlayerDied			= KD_FALSE;
    m_bHasFinishedLoading		= KD_FALSE;
    m_bHasAudioBeenInitialized	= KD_FALSE;
    m_pSoundEngine				= KD_NULL;
    m_eAudioManagerState		= kAudioManagerStateUninitialized;
    
	this->setupAudioEngine ( );

    // Initialize layers, etc.
    this->setBackgroundLayer ( BackgroundLayer::create ( ) );

    m_pBackgroundLayer->retain ( );
    m_pBackgroundLayer->setTag ( kGameManager_BackgroundLayer  );

	return KD_TRUE;
}

KDvoid GameManager::initMainMenu ( KDvoid )
{
    if ( m_pAboutGameLayer != KD_NULL )
	{
        m_pAboutGameLayer = KD_NULL;
    }
    
    this->setMainMenuLayer ( MainMenuLayer::create ( ) );
    m_pMainMenuLayer->retain ( );
    m_pMainMenuLayer->setTag ( kGameManager_MainMenuLayer );
}

KDvoid GameManager::initAboutGameMenu ( KDvoid )
{
    this->setAboutGameLayer ( AboutGameLayer::create ( ) );
    m_pAboutGameLayer->retain ( );
    m_pAboutGameLayer->setTag ( kGameManager_AboutGameLayer );
}

KDvoid GameManager::showAboutGame ( KDvoid )
{
    if ( m_pGameSceneLayer->getChildByTag ( kGameManager_MainMenuLayer ) != KD_NULL )
    {
        m_pGameSceneLayer->removeChildByTag ( kGameManager_MainMenuLayer, KD_TRUE );
    }
    
    this->initAboutGameMenu ( );
    m_pGameSceneLayer->addChild ( m_pAboutGameLayer, 1 );
}

KDvoid GameManager::showMainMenu ( KDvoid )
{	
    if ( m_pSoundEngine->isBackgroundMusicPlaying ( ) )
	{
        m_pSoundEngine->stopBackgroundMusic ( KD_FALSE );
    }
    m_pSoundEngine->setBackgroundMusicVolume ( 0.35f );
	m_pSoundEngine->playBackgroundMusic ( "song_intro.mp3", KD_TRUE );
    
    if ( m_pGameSceneLayer->getChildByTag ( kGameManager_AboutGameLayer ) != KD_NULL )
    {
        m_pGameSceneLayer->removeChildByTag ( kGameManager_AboutGameLayer, KD_TRUE );
    }
    if ( m_pGameSceneLayer->getChildByTag ( kGameManager_GameOverLayer ) != KD_NULL )
    {
        m_pGameSceneLayer->removeChildByTag ( kGameManager_GameOverLayer, KD_TRUE );
    }
    
	this->initMainMenu ( );

    m_pGameSceneLayer->addChild ( m_pMainMenuLayer, 1 );
}

KDvoid GameManager::startNewGame ( KDvoid )
{
    // Play random music
    this->playRandomSong ( );
    
    // Remove GameOver Screen
    if ( m_pGameSceneLayer->getChildByTag ( kGameManager_GameOverLayer ) != KD_NULL )
    {
        m_pGameSceneLayer->removeChildByTag ( kGameManager_GameOverLayer, KD_TRUE );
    }
    
    this->setGameplayLayer ( GameplayLayer::create ( ) );
    m_pGameplayLayer->retain ( );
    m_pGameplayLayer->setTag ( kGameManager_GameplayLayer );
    
    m_nScore = 0;
    m_nLives = this->getStartingLives ( );
    m_nDistance = 0;
    
    if ( m_pGameSceneLayer->getChildByTag ( kGameManager_MainMenuLayer ) != KD_NULL )
    {
        m_pGameSceneLayer->removeChildByTag ( kGameManager_MainMenuLayer, KD_TRUE );
    }
    m_pGameSceneLayer->addChild ( m_pGameplayLayer );
    m_pGameplayLayer->startNewGame ( );
}

KDvoid GameManager::endGame ( )
{
    if ( m_pGameSceneLayer->getChildByTag ( kGameManager_GameplayLayer ) != KD_NULL )
    {
        m_pGameSceneLayer->removeChildByTag ( kGameManager_GameplayLayer, KD_TRUE );
    }
    
    this->setGameOverLayer ( GameOverLayer::create ( ) );
    m_pGameOverLayer->retain ( );
    m_pGameOverLayer->setTag ( kGameManager_GameOverLayer );
    
   m_pGameSceneLayer->addChild ( m_pGameOverLayer );
}

KDvoid GameManager::maxVelocity ( KDvoid )
{
    if ( this->getGameDifficulty ( ) == kGameDifficultyHard )
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kMaxScrollVelocity * 0.9f;
        }
        else
        {
            m_fVelocity = kMaxScrollVelocity * 1.8f;
        }
    }
    else if ( this->getGameDifficulty ( ) == kGameDifficultyMedium )
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kMaxScrollVelocity;
        }
        else
        {
            m_fVelocity = kMaxScrollVelocity * 1;
        }
    }
    else
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kMaxScrollVelocity * 0.5f;
        }
        else
        {
            m_fVelocity = kMaxScrollVelocity;
        }
    }
}

KDvoid GameManager::normalVelocity ( KDvoid )
{
    if ( this->getGameDifficulty ( ) == kGameDifficultyHard )
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kNormalScrollVelocity;
        }
        else
        {
            m_fVelocity = kNormalScrollVelocity * 1.8f;
        }
    }
    else if ( this->getGameDifficulty ( ) == kGameDifficultyMedium )
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kNormalScrollVelocity * 0.8f;
        }
        else
        {
            m_fVelocity = kNormalScrollVelocity * 1;
        }
    }
    else
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kNormalScrollVelocity * 0.5f;
        }
        else
        {
            m_fVelocity = kNormalScrollVelocity;
        }
    }
}

KDvoid GameManager::minVelocity ( KDvoid )
{
    if ( this->getGameDifficulty ( ) == kGameDifficultyHard )
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kMinScrollVelocity;
        }
        else
        {
            m_fVelocity = kMinScrollVelocity * 1.8f;
        }
    }
    else if ( this->getGameDifficulty ( ) == kGameDifficultyMedium )
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kMinScrollVelocity * 0.8f;
        }
        else
        {
            m_fVelocity = kMinScrollVelocity * 1;
        }
    }
    else
    {
        if ( m_eDeviceSize == kDeviceSizeNormal || m_eDeviceSize == kDeviceSizeSmall )
        {
            m_fVelocity = kMinScrollVelocity * 0.5f;
        }
        else
        {
            m_fVelocity = kMinScrollVelocity;
        }
    }
}

KDvoid GameManager::launchGameObstacle ( KDvoid )
{
    m_pGameplayLayer->launchGameObstacle ( );
}

KDfloat GameManager::randomValueBetween ( KDfloat fLow, KDfloat fHigh )
{
    return CCRANDOM_BETWEEN ( fLow, fHigh );
}

KDint GameManager::randomIntValueBetween ( KDint nLow, KDint nHigh )
{
    return (KDint) CCRANDOM_BETWEEN ( nLow, nHigh );
}

KDvoid GameManager::setupAudioEngine ( KDvoid )
{
    if ( m_bHasAudioBeenInitialized == KD_TRUE )
    {
        return;
    }
    else
    {
        m_pSoundEngine = SimpleAudioEngine::sharedEngine ( );
        m_bHasAudioBeenInitialized = KD_TRUE;
    }
}

KDvoid GameManager::initAudioAsync ( KDvoid )
{
    
}

KDvoid GameManager::increaseScore ( KDint nAmount )
{
	m_nScore = m_nScore + nAmount;

	HudLayer*	pHudLayer = (HudLayer*) m_pGameplayLayer->getChildByTag ( kHudLayer );
	pHudLayer->getScoreLabel ( )->setString ( ccszf ( "%d", m_nScore ) );
}

KDvoid GameManager::increaseDistance ( KDint nAmount )
{
	m_nDistance = m_nDistance + nAmount;

	HudLayer*	pHudLayer = (HudLayer*) m_pGameplayLayer->getChildByTag ( kHudLayer );
	pHudLayer->getDistanceLabel ( )->setString ( ccszf ( "%d", m_nDistance ) );
}

KDvoid GameManager::takeLife ( KDvoid )
{
	m_nLives = m_nLives - 1;

	HudLayer*	pHudLayer = (HudLayer*) m_pGameplayLayer->getChildByTag ( kHudLayer );
	pHudLayer->getLivesLabel ( )->setString ( ccszf ( "%d", m_nLives ) );
}

KDvoid GameManager::giveLife ( KDvoid )
{
	m_nLives = m_nLives + 1;

	HudLayer*	pHudLayer = (HudLayer*) m_pGameplayLayer->getChildByTag ( kHudLayer );
	pHudLayer->getLivesLabel ( )->setString ( ccszf ( "%d", m_nLives ) );
}

KDvoid GameManager::playBackgroundTrack ( const KDchar* szTrackFileName )
{
	if ( m_pSoundEngine->isBackgroundMusicPlaying ( ) )
	{
		m_pSoundEngine->stopBackgroundMusic ( );
	}
   
	m_pSoundEngine->preloadBackgroundMusic ( szTrackFileName );
	if ( m_bIsMusicON == KD_TRUE )
	{
		m_pSoundEngine->playBackgroundMusic ( szTrackFileName, KD_TRUE );
	}
}

KDvoid GameManager::playRandomSong ( KDvoid )
{   
    if ( m_pSoundEngine->isBackgroundMusicPlaying ( ) )
    {
        m_pSoundEngine->stopBackgroundMusic ( );
    }
    
    // Generate a new randomSong number and make sure it is not the
    // previously played track.
    KDint	nRandomSong = 0;
    while ( ( nRandomSong = ( kdRand ( ) % 4 ) + 1 ) == m_nCurrentSong )
	{
		;
	}
    
    m_pSoundEngine->setBackgroundMusicVolume ( 0.35f );
    m_pSoundEngine->playBackgroundMusic ( ccszf ( "song_%d.mp3", nRandomSong ), KD_FALSE );
    
    m_nCurrentSong = nRandomSong;
}

KDint GameManager::getStartingLives ( KDvoid )
{
	switch ( this->getGameDifficulty ( ) )
	{
        case kGameDifficultyHard	:	return 2;
        case kGameDifficultyMedium	:	return 4;
        default						:	return 6;
	}
}

KDint GameManager::getDifficulty ( KDvoid )
{
    switch ( m_eGameDifficulty )
    {
        case kGameDifficultyHard	:	return 3;
        case kGameDifficultyMedium	:	return 2;
        default						:	return 1;
    }
}
