/* --------------------------------------------------------------------------
 *
 *      File            Ch6_MeteringDialogue.cpp
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
#include "Ch6_MeteringDialogue.h"

KDbool Ch6_MeteringDialogue::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Show loading message
	this->showMessage ( "LOADING..." );

	// Start recipe asynchronously
	this->runAction
	(
		CCSequence::create
		(
			CCDelayTime::create ( 0.5f ), 
			CCCallFunc ::create ( this, callfunc_selector ( Ch6_MeteringDialogue::startRecipe ) ),
			KD_NULL
		) 
	);

	return KD_TRUE;
}

KDvoid Ch6_MeteringDialogue::onExit ( KDvoid )
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

	Recipe::onExit ( );
}

KDvoid Ch6_MeteringDialogue::startRecipe ( KDvoid )
{
	// Show message
	this->showMessage ( "Tap the 'Speak' button to make \nSenator Claghorn speak." );

	// Initial values
	m_fLastAudioLevel = 0;
	m_bIsBlinking = KD_FALSE;
	
	// Initialize the audio engine
	m_pSAE = SimpleAudioEngine::sharedEngine ( );

	// Initialize source container
	m_pSoundSources = new CCDictionary ( );
	
	// Schedule step
	this->schedule ( schedule_selector ( Ch6_MeteringDialogue::step ) );
			
	// Add the sounds
	this->loadLongAudioSource ( "claghorn_a_joke_son.wav"		 );
	this->loadLongAudioSource ( "claghorn_carolina.wav"			 );
	this->loadLongAudioSource ( "claghorn_eliminate.wav"		 );
	this->loadLongAudioSource ( "claghorn_howdy.wav"			 );
	this->loadLongAudioSource ( "claghorn_make_canada_north.wav" );
	this->loadLongAudioSource ( "claghorn_rivers_flow_south.wav" );
	this->loadLongAudioSource ( "claghorn_transfusion.wav"		 );
	
	// Add the background music
	this->loadBackgroundMusic ( "dixie_1916.mp3" );
	
	// Add menu items
	CCMenuItemFont::setFontSize ( 30 );
	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );

	CCMenuItemSprite*	pMusicItem = this->menuItemFromSpriteFile ( "music_note.png", 0 );
	CCMenuItemFont*		pTalkItem  = CCMenuItemFont::create ( "Speak", this, menu_selector ( Ch6_MeteringDialogue::playSoundNumber ) );
	pTalkItem->setTag ( 1 );

	// Create our menu
	CCMenu*		pMenu = CCMenu::create ( pMusicItem, pTalkItem, KD_NULL );
	pMenu->alignItemsHorizontallyWithPadding ( 10.0f );
	pMenu->setPosition ( ccp ( 400, 250 ) );
	this->addChild ( pMenu );
		
	// Init Claghorn
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "claghorn.plist" );
	m_pClaghornBase		 = CCSprite::createWithSpriteFrameName ( "claghorn_normal.png"	  );
	m_pClaghornPupils	 = CCSprite::createWithSpriteFrameName ( "claghorn_pupils.png"	  );
	m_pClaghornEyebrows  = CCSprite::createWithSpriteFrameName ( "claghorn_eyebrows.png"  );
	m_pClaghornMouth	 = CCSprite::createWithSpriteFrameName ( "claghorn_mouth.png"	  );
	m_pClaghornMoustache = CCSprite::createWithSpriteFrameName ( "claghorn_moustache.png" );
	
	m_pClaghornBase		->setPosition ( ccp ( 240,120 ) );
	m_pClaghornPupils	->setPosition ( ccp ( 240,120 ) );
	m_pClaghornEyebrows	->setPosition ( ccp ( 240,120 ) );
	m_pClaghornMouth	->setPosition ( ccp ( 240,120 ) );
	m_pClaghornMoustache->setPosition ( ccp ( 240,120 ) );
	
	this->addChild ( m_pClaghornBase	 , 1 );
	this->addChild ( m_pClaghornPupils	 , 2 );
	this->addChild ( m_pClaghornEyebrows , 2 );
	this->addChild ( m_pClaghornMouth	 , 2 );
	this->addChild ( m_pClaghornMoustache, 3 );
	
	// Play background music
	this->playBackgroundMusic ( "dixie_1916.mp3" );
	
	// Have Claghorn introduce himself
	this->playLongAudioSource ( "claghorn_howdy.wav" );
}

KDvoid Ch6_MeteringDialogue::step ( KDfloat fDelta )
{
	// Random blinking
	KDint	nRand = kdRand ( ) % 200;
	if ( nRand < 2 && !m_bIsBlinking )
	{
		this->blink ( );
		this->runAction
		(
			CCSequence::create
			(
				CCDelayTime::create ( 0.2f ), 
				CCCallFunc ::create ( this, callfunc_selector ( Ch6_MeteringDialogue::unblink ) ),
				KD_NULL
			) 
		);
	}
	
	// Random looking
	if ( nRand > 198 )
	{
		m_pClaghornPupils->stopAllActions ( );
		this->resetPupils ( );

		CCPoint		tMoveVect = ccp ( 240 + kdRand ( ) % 10 - 5.0f, 120 + kdRand ( ) % 10 - 5.0f );
		KDfloat		fDuration = ( (KDfloat) ( kdRand ( ) % 2000 ) ) / 1000.0f + 0.5f;
		m_pClaghornPupils->runAction
		(
			CCSequence::create 
			(
				CCEaseExponentialOut::create ( CCMoveTo::create ( fDuration, tMoveVect ) ),
				CCEaseExponentialOut::create ( CCMoveTo::create ( fDuration, ccp ( 240, 120 ) ) ),
				KD_NULL
			)
		);
	}
	
	this->setPeakAndAveragePower ( );
	this->animateClaghorn ( );
}

/// Blink animation
KDvoid Ch6_MeteringDialogue::blink ( KDvoid )
{
	m_bIsBlinking = KD_TRUE;
	m_pClaghornBase->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "claghorn_squint.png" ) );
	m_pClaghornPupils->setVisible ( KD_FALSE );
}

/// Unblink animation
KDvoid Ch6_MeteringDialogue::unblink ( KDvoid )
{
	m_bIsBlinking = KD_FALSE;
	m_pClaghornBase->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "claghorn_normal.png" ) );
	m_pClaghornPupils->setVisible ( KD_TRUE );
}

/// Reset pupil position
KDvoid Ch6_MeteringDialogue::resetPupils ( KDvoid )
{
	m_pClaghornPupils->setPosition ( ccp ( 240, 120 ) );
}

/// Helper method to create sprite buttons
CCMenuItemSprite* Ch6_MeteringDialogue::menuItemFromSpriteFile ( const KDchar* szFile, KDint nTag )
{
	ccBlendFunc			tBlendFunc		= { GL_ONE, GL_ONE };
	CCSprite*			pNormalSprite   = CCSprite::create ( szFile );
	CCSprite*			pSelectedSprite = CCSprite::create ( szFile );
	pSelectedSprite->setColor ( ccc3 ( 128, 128, 180 ) );
	pSelectedSprite->setBlendFunc ( tBlendFunc );

	CCMenuItemSprite*	pItem = CCMenuItemSprite::create ( pNormalSprite, pSelectedSprite, this, menu_selector ( Ch6_MeteringDialogue::playSoundNumber ) );
	pItem->setTag ( nTag );
	pItem->setScale ( 0.5f );

	return pItem;
}

KDvoid Ch6_MeteringDialogue::loadLongAudioSource ( const KDchar* szFile )
{
	// Pre-load sound
	KDuint  uID = m_pSAE->preloadEffect ( szFile );
	
	// Add sound to container
	m_pSoundSources->setObject ( CCInteger::create ( uID ), szFile );
}

KDvoid Ch6_MeteringDialogue::loadBackgroundMusic ( const KDchar* szFile )
{
	// Pre-load background music
	m_pSAE->preloadBackgroundMusic ( szFile );

	// Lower the background music volume
	m_pSAE->setBackgroundMusicVolume ( 0.3f );
}

KDvoid Ch6_MeteringDialogue::playBackgroundMusic ( const KDchar* szFile )
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

/// Play voice clip or music callback
KDvoid Ch6_MeteringDialogue::playSoundNumber ( CCObject* pSender )
{
	this->resetPupils ( );

	CCMenuItem*		pItem = (CCMenuItem*) pSender;
	KDint			nNumber = pItem->getTag ( );

	if ( nNumber == 0 )
	{
		this->playBackgroundMusic ( "dixie_1916.mp3" );
	}
	else
	{		
		const KDchar*	aFiles [ ] =
		{	
			"claghorn_a_joke_son.wav",
			"claghorn_carolina.wav",
			"claghorn_eliminate.wav",
			"claghorn_howdy.wav",
			"claghorn_make_canada_north.wav",
			"claghorn_rivers_flow_south.wav",
			"claghorn_transfusion.wav"
		};	

		KDint			nNum = kdRand ( ) % 7;

		this->playLongAudioSource ( aFiles [ nNum ] );
	}
}

KDvoid Ch6_MeteringDialogue::playLongAudioSource ( const KDchar* szFile )
{
	// Get sound
	KDuint		uID = ( (CCInteger*) m_pSoundSources->objectForKey ( szFile ) )->getValue ( );

	// Play sound
	m_pSAE->playEffect ( uID );
}

KDvoid Ch6_MeteringDialogue::setPeakAndAveragePower ( KDvoid )
{
	// Find our playing audio source
//	CDLongAudioSource *audioSource = nil;
//	for(id s in soundSources)
//	{
//		CDLongAudioSource *source = [soundSources objectForKey:s];
//		if(source.isPlaying){
//			audioSource = source;
//			break;
//		}
//	}

	// Update meters
//	[audioSource.audioSourcePlayer updateMeters];

	// Get channels
	KDint		nChannels = 2;			// audioSource.audioSourcePlayer.numberOfChannels;

//	if ( !audioSource )
	{ 
		m_fPeakPower = 0;
		m_fAvgPower  = 0;
		return;
	}

	// Average all the channels
	KDfloat		fPeakPowerNow = 0;
	KDfloat		fAvgPowerNow  = 0;

	for ( KDint i = 0; i < nChannels; i++ )
	{
		KDfloat		fPeak = -15.0f;		// [audioSource.audioSourcePlayer peakPowerForChannel:i];
		KDfloat		fAvg  = -15.0f;		// [audioSource.audioSourcePlayer averagePowerForChannel:i];
		fPeakPowerNow += fPeak / nChannels;
		fAvgPowerNow  += fAvg  / nChannels;
	}

	// Change from a DB level to a 0 to 1 ratio
	KDfloat		fAdjustedPeak = kdPowf ( 10, ( 0.05f * fPeakPowerNow ) );
	KDfloat		fAdjustedAvg  = kdPowf ( 10, ( 0.05f * fAvgPowerNow  ) );
		
	// Average it out for smoothing
	m_fPeakPower = ( m_fPeakPower + fAdjustedPeak ) / 2;
	m_fAvgPower  = ( m_fAvgPower  + fAdjustedAvg  ) / 2;
}

KDvoid Ch6_MeteringDialogue::animateClaghorn ( KDvoid )
{
	// Custom mouth animation 
	KDfloat		fLevel = m_fAvgPower;
	
	// Make sure he's actually speaking
	if ( fLevel == 0 )
	{
		m_pClaghornEyebrows->setPosition ( ccp ( 240, 120 ) );
		m_pClaghornMouth   ->setPosition ( ccp ( 240, 120 ) );
		m_fLastAudioLevel = fLevel;
		return;
	}
	
	// Level bounds
	if ( fLevel <= 0 )
	{
		fLevel = 0.01f; 
	}

	if ( fLevel >= 1 ) 
	{
		fLevel = 0.99f; 
	}

	// Exaggerate level ebb and flow
	if ( fLevel < m_fLastAudioLevel )
	{	
		// Closing mouth
		m_fLastAudioLevel = fLevel;
		fLevel = kdPowf ( fLevel, 1.5f );
	}
	else
	{	
		// Opening mouth
		m_fLastAudioLevel = fLevel;
		fLevel = kdPowf ( fLevel, 0.75f );
	}

	// If mouth is almost closed, close mouth
	if ( fLevel < 0.1f ) 
	{
		fLevel = 0.01f; 
	}

	// Blink if level > 0.8f
	if ( fLevel > 0.8f && !m_bIsBlinking )
	{
		this->blink ( );
		this->runAction 
		(
			CCSequence::create 
			( 
				CCDelayTime::create ( 0.5f ), 
				CCCallFunc ::create ( this, callfunc_selector ( Ch6_MeteringDialogue::unblink ) ),
				KD_NULL
			)
		);
	}
	
	// Raise eyebrows if level > 0.6f
	if ( fLevel > 0.6f )
	{
		m_pClaghornEyebrows->setPosition ( ccp ( 240, 120 + fLevel * 5.0f ) );
	}
	else
	{
		m_pClaghornEyebrows->setPosition ( ccp ( 240, 120 ) );
	}

	// Set mouth position
	m_pClaghornMouth->setPosition ( ccp ( 240, 120 - fLevel * 19.0f ) );
}