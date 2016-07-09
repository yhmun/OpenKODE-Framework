/* --------------------------------------------------------------------------
 *
 *      File            SoundManager.cpp
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

#include "Precompiled.h"
#include "SoundManager.h"

// singleton stuff
static SoundManager*		l_pSharedSoundManager = KD_NULL;

SoundManager* SoundManager::sharedSoundManager ( KDvoid )
{
	if ( !l_pSharedSoundManager )
	{
		l_pSharedSoundManager = new SoundManager ( );
		l_pSharedSoundManager->init ( );
	}

	return l_pSharedSoundManager;
}

KDbool SoundManager::init ( KDvoid )
{
	SimpleAudioEngine*		pSE = SimpleAudioEngine::sharedEngine ( );

	// sound loading
	pSE->preloadEffect ( "sound_fx_chip_was_unselected.wav" );
	pSE->preloadEffect ( "sound_fx_regeneration.wav" );
	pSE->preloadEffect ( "sound_fx_selected_chip.wav" );
	pSE->preloadEffect ( "sound_fx_tap.wav" );
	pSE->preloadEffect ( "sound_fx_ready.wav" );
	pSE->preloadEffect ( "sound_fx_new_girl.mp3" );
	pSE->preloadEffect ( "sound_fx_incorrect_matching.mp3" );
	pSE->preloadEffect ( "sound_fx_game_over.mp3" );
	pSE->preloadEffect ( "sound_fx_correct_matching_with_wildcard.mp3" );
	pSE->preloadEffect ( "sound_fx_correct_matching.mp3" );
	
	// music
	pSE->preloadBackgroundMusic ( "sound_music_game.mp3" );
	pSE->preloadBackgroundMusic ( "sound_music_menu.mp3" );

	return KD_TRUE;
}

KDvoid SoundManager::showVersion ( KDvoid )
{
	CCLOG ( "SoundManager. Calling this method to force effects preloading." );
}

KDvoid SoundManager::playSoundFxCorrectMatching ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_correct_matching.mp3" );
}

KDvoid SoundManager::playSoundFxCorrectMatchingWithWildcard ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_correct_matching_with_wildcard.mp3" );
}

KDvoid SoundManager::playSoundFxChipWasUnselected ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_chip_was_unselected.wav" );
}

KDvoid SoundManager::playSoundFxGameOver ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_game_over.mp3" );
}

KDvoid SoundManager::playSoundFxIncorrectMatching ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_incorrect_matching.mp3" );
}

KDvoid SoundManager::playSoundFxNewGirl ( KDvoid )
{	
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_new_girl.mp3" );
}

KDvoid SoundManager::playSoundFxRegeneration ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_regeneration.wav" );
}

KDvoid SoundManager::playSoundFxSelectedChip ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_selected_chip.wav" );
}

KDvoid SoundManager::playSoundFxTap ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_tap.wav" );
}

KDvoid SoundManager::playSoundFxReady ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "sound_fx_ready.wav" );
}

KDvoid SoundManager::playSoundMusicGame ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "sound_music_game.mp3" );
}	

KDvoid SoundManager::playSoundMusicMenu ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "sound_music_menu.mp3" );
}

KDvoid SoundManager::stopBackgroundMusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->stopBackgroundMusic ( );
}
