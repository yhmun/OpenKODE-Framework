/* --------------------------------------------------------------------------
 *
 *      File            GameSounds.cpp
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
#include "GameSounds.h"
#include "GameData.h"

static GameSounds*		l_pSharedGameSounds = KD_NULL;

GameSounds* GameSounds::sharedGameSounds ( KDvoid )
{
	if ( !l_pSharedGameSounds )
	{
		l_pSharedGameSounds = new GameSounds ( );
	}

	return l_pSharedGameSounds;
}

GameSounds::GameSounds ( KDvoid )
{
	m_bVoiceFXTurnedOff   = GameData::sharedData ( )->areVoiceFXMuted   ( );
	m_bSoundFXTurnedOff   = GameData::sharedData ( )->areSoundFXMuted   ( );
	m_bAmbientFXTurnedOff = GameData::sharedData ( )->areAmbientFXMuted ( );
}

KDvoid GameSounds::preloadSounds ( KDvoid )
{
	SimpleAudioEngine*		pSound = SimpleAudioEngine::sharedEngine ( );

	pSound->preloadEffect ( "grunt1.wav" );
	pSound->preloadEffect ( "grunt2.wav" );
	pSound->preloadEffect ( "grunt3.wav" );
	pSound->preloadEffect ( "grunt4.wav" );
	pSound->preloadEffect ( "grunt5.wav" );
	pSound->preloadEffect ( "grunt6.wav" );
	pSound->preloadEffect ( "grunt7.wav" );
	pSound->preloadEffect ( "grunt8.wav" );

	pSound->preloadEffect ( "break1.wav" );
	pSound->preloadEffect ( "break2.wav" );
	pSound->preloadEffect ( "break3.wav" );

	pSound->preloadEffect ( "impact1.wav" );
	pSound->preloadEffect ( "impact2.wav" );
	pSound->preloadEffect ( "impact3.wav" );
	pSound->preloadEffect ( "impact4.wav" );
	pSound->preloadEffect ( "impact5.wav" );
}

KDbool GameSounds::areSoundFXMuted ( KDvoid )
{
	return m_bSoundFXTurnedOff;
}

KDvoid GameSounds::setSoundFXMuted ( KDbool bValue )
{
	m_bSoundFXTurnedOff = bValue;
}

KDbool GameSounds::areVoiceFXMuted ( KDvoid )
{
	return m_bVoiceFXTurnedOff;
}

KDvoid GameSounds::setVoiceFXMuted ( KDbool bValue )
{
	m_bVoiceFXTurnedOff = bValue;
}

KDvoid GameSounds::playSoundFX ( const KDchar* szFileToPlay )
{
	if ( !m_bSoundFXTurnedOff )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( szFileToPlay );
	}
}

KDvoid GameSounds::introTag ( KDvoid )
{
	if ( !m_bSoundFXTurnedOff ) 
	{		
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "gong.mp3" );		
	}
}

KDvoid GameSounds::playStackImpactSound ( KDvoid )
{
	if ( !m_bSoundFXTurnedOff )
	{
		SimpleAudioEngine*		pSound = SimpleAudioEngine::sharedEngine ( );
		KDint					nRandNum = (KDint) CCRANDOM_BETWEEN ( 0, 4 );

		switch ( nRandNum )
		{
			case 0	:	pSound->playEffect ( "impact1.wav" );	break;
			case 1	:	pSound->playEffect ( "impact2.wav" );	break;
			case 2	:	pSound->playEffect ( "impact3.wav" );	break;
			case 3	:	pSound->playEffect ( "impact4.wav" );	break;
			case 4	:	pSound->playEffect ( "impact5.wav" );	break;
		}
	}
}

KDvoid GameSounds::playBreakSound ( KDvoid )
{
	if ( !m_bSoundFXTurnedOff )
	{
		SimpleAudioEngine*		pSound = SimpleAudioEngine::sharedEngine ( );
		KDint					nRandNum = (KDint) CCRANDOM_BETWEEN ( 0, 2 );

		switch ( nRandNum )
		{
			case 0	:	pSound->playEffect ( "break1.wav" );	break;
			case 1	:	pSound->playEffect ( "break2.wav" );	break;
			case 2	:	pSound->playEffect ( "break3.wav" );	break;
		}
	}
}

KDvoid GameSounds::releaseSlingSounds ( KDvoid )
{
	SimpleAudioEngine*			pSound = SimpleAudioEngine::sharedEngine ( );

	if ( !m_bSoundFXTurnedOff )
	{
		pSound->playEffect ( "whoosh.wav" );
	}

	if ( !m_bVoiceFXTurnedOff )
	{
		KDint					nRandNum = (KDint) CCRANDOM_BETWEEN ( 0, 7 );

		switch ( nRandNum )
		{
			case 0	:	pSound->playEffect ( "grunt1.wav" );	break;
			case 1	:	pSound->playEffect ( "grunt2.wav" );	break;
			case 2	:	pSound->playEffect ( "grunt3.wav" );	break;
			case 3	:	pSound->playEffect ( "grunt4.wav" );	break;
			case 4	:	pSound->playEffect ( "grunt5.wav" );	break;
			case 5	:	pSound->playEffect ( "grunt6.wav" );	break;
			case 6	:	pSound->playEffect ( "grunt7.wav" );	break;
			case 7	:	pSound->playEffect ( "grunt8.wav" );	break;
		}
	}
}

KDvoid GameSounds::playBackgroundMusic ( AmbientFXSounds eTrack )
{
	m_eMusicChoice = eTrack;

	SimpleAudioEngine*		pSound = SimpleAudioEngine::sharedEngine ( );
	pSound->stopBackgroundMusic ( );

	if ( !m_bAmbientFXTurnedOff )
	{				
		switch ( eTrack )
		{
			case Frogs		:	pSound->playBackgroundMusic ( "birds.mp3" );	break;
			case Insects	:	pSound->playBackgroundMusic ( "frogs.mp3" );	break;
			default			:	;
		}		
	}
}

KDvoid GameSounds::stopBackgroundMusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->stopBackgroundMusic ( );

	m_bAmbientFXTurnedOff = KD_TRUE;
}

KDvoid GameSounds::restartBackgroundMusic ( KDvoid )
{
	m_bAmbientFXTurnedOff = KD_FALSE;

	this->playBackgroundMusic ( m_eMusicChoice );
}