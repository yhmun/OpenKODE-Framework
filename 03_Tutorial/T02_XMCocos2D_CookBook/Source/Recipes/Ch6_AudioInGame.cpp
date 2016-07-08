/* --------------------------------------------------------------------------
 *
 *      File            Ch6_AudioInGame.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Ch6_AudioInGame.h"

KDbool Ch6_AudioInGame::init ( KDvoid )
{	
	if ( !Ch4_Bullets::init ( ) )
	{
		return KD_FALSE;
	}

	// Initialize the audio engine
	m_pSAE = SimpleAudioEngine::sharedEngine ( );

	// Initialize source container
	m_pSoundSources = new CCDictionary ( );
	
	// Add the sounds
	this->loadSoundEffect ( "bullet_fire_no_shell.wav", 1.0f );
	this->loadSoundEffect ( "bullet_casing_tink.wav", 0.25f );
	this->loadSoundEffect ( "gunman_jump.wav", 1.5f );
	this->loadSoundEffect ( "box_break.wav", 1.5f );
	
	// Add the background music
	this->loadBackgroundMusic ( "hiphop_boss_man_by_p0ss.mp3" );
	m_pSAE->setBackgroundMusicVolume ( 0.5f );
	this->playBackgroundMusic ( "hiphop_boss_man_by_p0ss.mp3" );

	return KD_TRUE;
}

KDvoid Ch6_AudioInGame::onExit ( KDvoid )
{
	// Stop background music
	m_pSAE->stopBackgroundMusic ( );
	
	CCArray*	pKeys = m_pSoundSources->allKeys ( );
	CCObject*	pObject;
	CCARRAY_FOREACH ( pKeys, pObject )
	{
		CCString*	pKey = (CCString*) pObject;
		CCInteger*	pID = (CCInteger*) m_pSoundSources->objectForKey ( pKey->getCString ( ) );

		m_pSAE->unloadEffect ( pID->getValue ( ) );		
	}

	CC_SAFE_DELETE ( m_pSoundSources );

	Ch4_Bullets::onExit ( );
}

/// Jump sound override
KDvoid Ch6_AudioInGame::processJump ( KDvoid )
{
	if ( m_bOnGround && m_fJumpCounter < 0 )
	{	
		this->playSoundFile ( "gunman_jump.wav" );
	}
	
	Ch4_Bullets::processJump ( );
}

/// Fire gun sound override
KDvoid Ch6_AudioInGame::fireGun ( KDvoid )
{
	if ( m_fFireCount <= 0 )
	{
		this->playSoundFile ( "bullet_fire_no_shell.wav" );
	}
	
	Ch4_Bullets::fireGun ( );
}

/// Box explosion sound override
KDvoid Ch6_AudioInGame::boxExplosionAt ( const CCPoint& tPoint, KDfloat fRotate )
{
	this->playSoundFile ( "box_break.wav" );
	
	Ch4_Bullets::boxExplosionAt ( tPoint, fRotate );
}

/// Bullet casing sound override
KDvoid Ch6_AudioInGame::handleCollisionWithMisc ( GameMisc* pMiscA, GameMisc* pMiscB )
{
	if ( pMiscA->getTypeTag ( ) == TYPE_OBJ_SHELL || pMiscB->getTypeTag ( ) == TYPE_OBJ_SHELL )
	{
		this->playSoundFile ( "bullet_casing_tink.wav" );
	}
	
	Ch4_Bullets::handleCollisionWithMisc ( pMiscA, pMiscB );
}

KDvoid Ch6_AudioInGame::loadSoundEffect ( const KDchar* szFile, KDfloat fGain )
{
	// Pre-load sound
	KDuint  uID = m_pSAE->preloadEffect ( szFile );

	// Init sound
	m_pSAE->setEffectVolume ( uID, fGain );
	
	// Add sound to container
	m_pSoundSources->setObject ( CCInteger::create ( uID ), szFile );
}

KDvoid Ch6_AudioInGame::loadBackgroundMusic ( const KDchar* szFile )
{
	// Pre-load background music
	m_pSAE->preloadBackgroundMusic ( szFile );
}

KDvoid Ch6_AudioInGame::playBackgroundMusic ( const KDchar* szFile )
{
	if ( !m_pSAE->isBackgroundMusicPlaying ( ) )
	{
		// Play background music
		m_pSAE->playBackgroundMusic ( szFile );
	}
	else
	{
		// Stop music if its currently playing
		m_pSAE->stopBackgroundMusic ( );
	}
}

KDvoid Ch6_AudioInGame::playSoundFile ( const KDchar* szFile )
{
	// Get sound
	KDuint		uID = ( (CCInteger*) m_pSoundSources->objectForKey ( szFile ) )->getValue ( );

	// Play sound
	m_pSAE->playEffect ( uID );
}
