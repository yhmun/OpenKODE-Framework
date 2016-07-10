/* --------------------------------------------------------------------------
 *
 *      File            SoundManager.cpp
 *      Created By      Project B team
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2013 XMsoft. All rights reserved.
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
#include "SoundManager.h"

#define BGM			"Sounds/bgm_1.mp3"
#define GAMEOVER	"Sounds/gameover.mp3"
#define DROPDOWN	"Sounds/dropdown.mp3"
#define ROTATE		"Sounds/rotate.mp3"
#define BOOM		"Sounds/boom.wav"
#define LINECLEAR	"Sounds/good.mp3"
#define LEVELUP		"Sounds/levelup.mp3"

static SoundManager* m_pSoundManager;

SoundManager::SoundManager ( KDvoid )
{
	m_pSoundManager = KD_NULL;
}

SoundManager* SoundManager::getInstance ( KDvoid )
{
	if ( m_pSoundManager == KD_NULL ) 
	{
		m_pSoundManager = new SoundManager( );

		SimpleAudioEngine::sharedEngine ( )->preloadEffect ( DROPDOWN );
		SimpleAudioEngine::sharedEngine ( )->preloadEffect ( ROTATE );
		SimpleAudioEngine::sharedEngine ( )->preloadEffect ( LINECLEAR );
		SimpleAudioEngine::sharedEngine ( )->preloadEffect ( LEVELUP );
		SimpleAudioEngine::sharedEngine ( )->preloadBackgroundMusic ( BGM );
	}

	return m_pSoundManager;
}

KDvoid SoundManager::playBackgroundMusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( BGM );
}

KDvoid SoundManager::gameOverBackgroundMusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( GAMEOVER );
}

KDvoid SoundManager::pauseBackgroundMusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->pauseBackgroundMusic ( );
}

KDvoid SoundManager::resumeBackgroundMusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->resumeBackgroundMusic ( );
}

KDvoid SoundManager::stopBackgroundMusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->stopBackgroundMusic ( );
}

KDvoid SoundManager::playStuckEffect ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( DROPDOWN );
}

KDvoid SoundManager::playRotateEffect ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( ROTATE );
}

KDvoid SoundManager::playLevelupEffect ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( LEVELUP );
}

KDvoid SoundManager::playLineClearEffect ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( LINECLEAR );
}

KDvoid SoundManager::playBoomEffect ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( BOOM );
}