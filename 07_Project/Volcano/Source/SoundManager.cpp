/* --------------------------------------------------------------------------
 *
 *      File            SoundManager.cpp
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com    
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
 *y
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "SoundManager.h"

static SoundManager*	l_pSharedSoundManager = KD_NULL;

SoundManager::SoundManager ( )
{
	// 사운드 파일을 미리 읽어둔다. 미리읽어두지 않고 재생하면 게임진행시 속도저하의 우려가 있기때문에.
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "Sounds/ah.wav" );
	SimpleAudioEngine::sharedEngine ( )->preloadBackgroundMusic ( "Sounds/background.mp3" );
}

SoundManager::~SoundManager ( )
{
	CC_SAFE_DELETE ( l_pSharedSoundManager );
}

SoundManager* SoundManager::sharedSoundManager ( KDvoid )
{
	if ( !l_pSharedSoundManager )
	{
		l_pSharedSoundManager = new SoundManager ( );
	}
	
	return l_pSharedSoundManager;
}

KDvoid SoundManager::soundOnOff ( CCMenuItemImage* pSoundOnMenuItem, CCMenuItemImage* pSoundOffMenuItem )
{
	soundOnOff ( pSoundOnMenuItem, pSoundOffMenuItem, CCUserDefault::sharedUserDefault ( )->getBoolForKey ( SESSION_SOUND_OFF ) );
}

KDvoid SoundManager::soundOnOff ( CCMenuItemImage* pSoundOnMenuItem, CCMenuItemImage* pSoundOffMenuItem, KDbool bSoundOff )
{
	if ( bSoundOff )
	{
        pSoundOnMenuItem->setVisible ( KD_FALSE );
		pSoundOffMenuItem->setVisible ( KD_TRUE );
		SimpleAudioEngine::sharedEngine ( )->stopBackgroundMusic ( );
    } 
	else 
	{
		pSoundOnMenuItem->setVisible ( KD_TRUE );
		pSoundOffMenuItem->setVisible ( KD_FALSE );
		SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "Sounds/background.mp3", KD_TRUE );
    }
	CCUserDefault::sharedUserDefault ( )->setBoolForKey ( SESSION_SOUND_OFF, bSoundOff );
}


KDvoid SoundManager::playEffectDie ( KDvoid )
{
	if ( !CCUserDefault::sharedUserDefault ( )->getBoolForKey ( SESSION_SOUND_OFF ) )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/ah.wav", KD_FALSE );
	}
}