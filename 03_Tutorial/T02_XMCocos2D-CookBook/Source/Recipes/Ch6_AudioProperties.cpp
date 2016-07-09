/* --------------------------------------------------------------------------
 *
 *      File            Ch6_AudioProperties.cpp
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
#include "Ch6_AudioProperties.h"

KDbool Ch6_AudioProperties::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Show message
	this->showMessage ( "Drag your fingers across \nthe screen to play a tune." );

	// Add background
	CCSprite*		pBg = CCSprite::create ( "synth_tone_sheet.png" );
	pBg->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pBg );

	// Initialize the audio engine
	m_pSAE = SimpleAudioEngine::sharedEngine ( );

	// Initialize note container
	m_pNotes = new CCDictionary ( );
	m_pNoteSprites = new CCDictionary ( );
	
	// Preload tone
	m_pSAE->preloadEffect ( "synth_tone_mono.wav" );

	return KD_TRUE;
}

KDvoid Ch6_AudioProperties::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pNotes );
	CC_SAFE_DELETE ( m_pNoteSprites );

	Recipe::onExit ( );
}

KDvoid Ch6_AudioProperties::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	// Process multiple touches
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ )
	{
		CCTouch*		pTouch = (CCTouch*) ( *iter );
		CCPoint			tPoint = pTouch->getLocation ( );
		const KDchar*	szKey  = ccszf ( "%d", pTouch->getID ( ) );

		// Play our sound with custom pitch and gain
		KDuint			uID = m_pSAE->preloadEffect ( "synth_tone_mono.mp3", szKey );		
		m_pSAE->playEffect ( uID, KD_TRUE );
		m_pSAE->setEffectPitch  ( uID, tPoint.x / 240.f );
		m_pSAE->setEffectVolume ( uID, tPoint.y / 320.f );
		m_pNotes->setObject ( CCInteger::create ( uID ), szKey );
	
		// Show music note where you touched
		CCSprite*	pSprite = CCSprite::create ( "music_note.png" );
		pSprite->setPosition ( tPoint );
		m_pNoteSprites->setObject ( pSprite, szKey ); 
		pSprite->setScale ( ( tPoint.y / 320.0f ) / 2 + 0.25f );
		this->addChild ( pSprite );	
	}
}

KDvoid Ch6_AudioProperties::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	// Adjust sound sources and music note positions
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ )
	{
		CCTouch*		pTouch = (CCTouch*) ( *iter );
		CCPoint			tPoint = pTouch->getLocation ( );
		const KDchar*	szKey  = ccszf ( "%d", pTouch->getID ( ) );

		CCInteger*		pID = (CCInteger*) m_pNotes->objectForKey ( szKey );
		if ( !pID )
		{
			continue;
		}

		KDuint			uID = pID->getValue ( );
		m_pSAE->setEffectPitch  ( uID, tPoint.x / 240.f );
		m_pSAE->setEffectVolume ( uID, tPoint.y / 320.f );			
			
		CCSprite*	pSprite = (CCSprite*) m_pNoteSprites->objectForKey ( szKey );
		pSprite->setPosition ( tPoint );
		pSprite->setScale ( ( tPoint.y / 320.0f ) / 2 + 0.25f );	
	}
}

KDvoid Ch6_AudioProperties::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// Stop sounds and remove sprites
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ )
	{
		CCTouch*		pTouch = (CCTouch*) ( *iter );
		CCPoint			tPoint = pTouch->getLocation ( );
		const KDchar*	szKey  = ccszf ( "%d", pTouch->getID ( ) );

		CCInteger*		pID = (CCInteger*) m_pNotes->objectForKey ( szKey );
		if ( !pID )
		{
			continue;
		}

		KDuint			uID = pID->getValue ( );
		m_pSAE->unloadEffect ( uID );
		m_pNotes->removeObjectForKey ( szKey );

		// Remove sprite
		CCSprite*	pSprite = (CCSprite*) m_pNoteSprites->objectForKey ( szKey );
		this->removeChild ( pSprite );
		m_pNoteSprites->removeObjectForKey ( szKey );	
	}
}
