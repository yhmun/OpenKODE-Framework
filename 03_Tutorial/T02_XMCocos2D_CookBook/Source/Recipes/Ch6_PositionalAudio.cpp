/* --------------------------------------------------------------------------
 *
 *      File            Ch6_PositionalAudio.cpp
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
#include "Ch6_PositionalAudio.h"

KDbool Ch6_PositionalAudio::init ( KDvoid )
{	
	// Run our top-down isometric game recipe
	if ( !Ch4_TopDownIsometric::init ( ) )
	{
		return KD_FALSE;
	}

	// Initialize max audible range
	m_fAudibleRange = 20.0f;

	// Initialize the audio engine
	m_pSAE = SimpleAudioEngine::sharedEngine ( );

	// Preload the sounds & Add the sounds
	m_uBallSource		 = m_pSAE->preloadEffect ( "kick_ball_bounce.wav"		);
	m_uGunmanJumpSource	 = m_pSAE->preloadEffect ( "gunman_jump.wav"			);
	m_uFireBallSource	 = m_pSAE->preloadEffect ( "bullet_fire_no_shell.wav"	);
	
	// Start playing forest bird source
	m_pSAE->setBackgroundMusicVolume ( 0 );
	m_pSAE->playBackgroundMusic ( "forest_birds_ambience.mp3" );
	
	// Customize fire ball sound
	m_pSAE->setEffectPitch  ( m_uFireBallSource, 2.0f );
	m_pSAE->setEffectVolume ( m_uFireBallSource, 0.5f );

	return KD_TRUE;
}

KDvoid Ch6_PositionalAudio::onExit ( KDvoid )
{
	m_pSAE->stopBackgroundMusic ( );

	m_pSAE->unloadEffect ( m_uBallSource		);
	m_pSAE->unloadEffect ( m_uGunmanJumpSource	);
	m_pSAE->unloadEffect ( m_uFireBallSource	);

	Ch4_TopDownIsometric::onExit ( );
}

KDvoid Ch6_PositionalAudio::step ( KDfloat fDelta )
{
	Ch4_TopDownIsometric::step ( fDelta );

	// Play forest bird source with gain based on distance from gunman
	KDfloat		fDistance = 10000.0f;
	for ( KDuint i = 0; i < m_pTrees->count ( ); i++ )
	{
		GameObject*		pTree = (GameObject*) m_pTrees->objectAtIndex ( i );		
		KDfloat			fThisDistance = distanceBetweenPoints 
		(
			ccp (     pTree->getBody ( )->GetPosition ( ).x,     pTree->getBody ( )->GetPosition ( ).y ),
			ccp ( m_pGunman->getBody ( )->GetPosition ( ).x, m_pGunman->getBody ( )->GetPosition ( ).y ) 
		);		
		if ( fThisDistance < fDistance )
		{
			fDistance = fThisDistance; 
		}
	}
	
	// If closest tree is outside of audible range we set gain to 0.0f
	if ( fDistance < m_fAudibleRange )
	{
		m_pSAE->setBackgroundMusicVolume ( ( m_fAudibleRange - fDistance ) / m_fAudibleRange );
	}
	else
	{
		m_pSAE->setBackgroundMusicVolume ( 0 );		
	}
}

KDvoid Ch6_PositionalAudio::handleCollisionWithGroundWithObj ( GameObject* pObject )
{
	Ch4_TopDownIsometric::handleCollisionWithGroundWithObj ( pObject );

	// Play ball bounce sound with gain based on distance from gunman
	if ( pObject->getTypeTag ( ) == TYPE_OBJ_BALL )
	{
		b2Vec2		tObjectPos =   pObject->getBody ( )->GetPosition ( );
		b2Vec2		tGunmanPos = m_pGunman->getBody ( )->GetPosition ( );
		KDfloat		fDistance  = distanceBetweenPoints
		(
			ccp ( tObjectPos.x, tObjectPos.y ),
			ccp ( tGunmanPos.x, tGunmanPos.y )
		);
			
		if ( fDistance < m_fAudibleRange )
		{
			KDfloat		fGain  = ( m_fAudibleRange - fDistance ) / m_fAudibleRange;
			KDfloat		fPan   = ( tObjectPos.x - tGunmanPos.x ) / fDistance;
			KDfloat		fPitch = ( ( ( (GameIsoObject*) pObject )->getInGameSize ( ) / 10.0f ) * -1 ) + 2;
						
			if ( fDistance < m_fAudibleRange )
			{
				this->playBallSoundWithGain ( fGain, fPan, fPitch );
			}
		}
	}
}

/// Fire ball sound override
KDvoid Ch6_PositionalAudio::fireBall ( KDvoid )
{
	if ( m_fFireCount < 0 )
	{
		m_pSAE->playEffect ( m_uFireBallSource );
	}

	Ch4_TopDownIsometric::fireBall ( );
}

/// Jump sound override
KDvoid Ch6_PositionalAudio::processJump ( KDvoid )
{
	if ( m_pGunman->getBody ( )->GetZPosition ( ) <= 1.0f )
	{
		m_pSAE->playEffect ( m_uGunmanJumpSource );
	}

	Ch4_TopDownIsometric::processJump ( );
}

KDvoid Ch6_PositionalAudio::playBallSoundWithGain ( KDfloat fGain, KDfloat fPan, KDfloat fPitch )
{
	// Play the sound using the non-interruptible source group

	m_pSAE->setEffectPitch  ( m_uBallSource, fPitch );
	m_pSAE->setEffectVolume ( m_uBallSource, fGain  );

	m_pSAE->playEffect ( m_uBallSource );
}
