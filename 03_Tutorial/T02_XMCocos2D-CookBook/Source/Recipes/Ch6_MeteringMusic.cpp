/* --------------------------------------------------------------------------
 *
 *      File            Ch6_MeteringMusic.cpp
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
#include "Ch6_MeteringMusic.h"

KDbool Ch6_MeteringMusic::init ( KDvoid )
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
			CCCallFunc ::create ( this, callfunc_selector ( Ch6_MeteringMusic::startRecipe ) ),
			KD_NULL
		) 
	);

	return KD_TRUE;
}

KDvoid Ch6_MeteringMusic::onExit ( KDvoid )
{
	m_pSAE->stopBackgroundMusic ( );

	Recipe::onExit ( );
}

KDvoid Ch6_MeteringMusic::startRecipe ( KDvoid )
{
	// Show message
	this->showMessage ( "" );

	// Initialize the audio engine
	m_pSAE = SimpleAudioEngine::sharedEngine ( );

	// Set peak and average power initially
	m_fPeakPower = 0;
	m_fAvgPower  = 0;
			
	// Init Speaker
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "cartoon_speaker.plist" );
	m_pSpeakerBase  = CCSprite::createWithSpriteFrameName ( "cartoon_speaker_base.png" );
	m_pSpeakerLarge = CCSprite::createWithSpriteFrameName ( "cartoon_speaker_big_speaker.png" );
	m_pSpeakerSmall = CCSprite::createWithSpriteFrameName ( "cartoon_speaker_small_speaker.png" );

	m_pSpeakerBase ->setPosition ( ccp ( 340, 160 ) );
	m_pSpeakerLarge->setPosition ( ccp ( 340, 160 ) );
	m_pSpeakerSmall->setPosition ( ccp ( 340, 160 ) );
	
	this->addChild ( m_pSpeakerBase , 1 );
	this->addChild ( m_pSpeakerLarge, 2 );
	this->addChild ( m_pSpeakerSmall, 2 );
		
	// Init meter
	m_pAvgMeter = CCSprite::create ( "blank.png" );
	m_pAvgMeter->setTextureRect ( CCRect ( 0, 0, 10, 0 ) );
	m_pAvgMeter->setColor ( ccc3 ( 255, 0, 0 ) );
	m_pAvgMeter->setPosition ( ccp ( 100, 20 ) );
	m_pAvgMeter->setAnchorPoint ( ccp ( 0.5f, 0 ) );
	this->addChild ( m_pAvgMeter );
	
	m_pPeakMeter = CCSprite::create ( "blank.png" );
	m_pPeakMeter->setTextureRect ( CCRect ( 0, 0, 10, 5 ) );
	m_pPeakMeter->setColor ( ccc3 ( 255, 0, 0 ) );
	m_pPeakMeter->setPosition ( ccp ( 100, 20 ) );
	m_pPeakMeter->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	this->addChild ( m_pPeakMeter );
		
	// Add the background music
	m_pSAE->playBackgroundMusic ( "technogeek_by_mrpoly.mp3" );
	
	// Enable metering
//	[CDAudioManager sharedManager].backgroundMusic.audioSourcePlayer.meteringEnabled = YES;
	
	// Schedule step method 
	this->schedule ( schedule_selector ( Ch6_MeteringMusic::step ) );
}

KDvoid Ch6_MeteringMusic::step ( KDfloat fDelta )
{
	this->setPeakAndAveragePower ( );

	this->animateMeterAndSpeaker ( );
}

KDvoid Ch6_MeteringMusic::setPeakAndAveragePower ( KDvoid )
{
	// Update meters
//	[[CDAudioManager sharedManager].backgroundMusic.audioSourcePlayer updateMeters];

	// Get channels
	KDint		nChannels = 2; // [CDAudioManager sharedManager].backgroundMusic.audioSourcePlayer.numberOfChannels;

	// Average all the channels
	KDfloat		fPeakPowerNow = 0;
	KDfloat		fAvgPowerNow  = 0;

	for ( KDint i = 0; i < nChannels; i++ )
	{
		KDfloat		fPeak = -15.0f; //[[CDAudioManager sharedManager].backgroundMusic.audioSourcePlayer peakPowerForChannel:i];
		KDfloat		fAvg  = -15.0f; //[[CDAudioManager sharedManager].backgroundMusic.audioSourcePlayer averagePowerForChannel:i];
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

KDvoid Ch6_MeteringMusic::animateMeterAndSpeaker ( KDvoid )
{
	// Average meter
	m_pAvgMeter->setTextureRect ( CCRect ( 0, 0, 10, m_fAvgPower * 500.0f ) );
	
	// Peak meter
	m_pPeakMeter->setPosition ( ccp ( 100, 20 + m_fPeakPower * 500.0f ) );
	
	// Animate speaker
	m_pSpeakerLarge->setScale ( kdPowf ( m_fAvgPower, 0.4f ) * 2 );
	m_pSpeakerSmall->setScale ( kdPowf ( m_fAvgPower, 0.4f ) * 2 );
}