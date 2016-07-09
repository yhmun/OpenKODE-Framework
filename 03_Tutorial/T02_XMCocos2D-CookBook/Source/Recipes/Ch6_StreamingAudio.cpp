/* --------------------------------------------------------------------------
 *
 *      File            Ch6_StreamingAudio.cpp
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
#include "Ch6_StreamingAudio.h"

KDbool Ch6_StreamingAudio::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Show message
	this->showMessage ( "Hit the play button to start the stream." );

	// Create music player buttons
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "music_player.plist" );

	// Create menu
	CCMenu*		pMenu = CCMenu::create 
	(
		this->menuItemFromSpriteFile ( "music_player_prev.png" , this, menu_selector ( Ch6_StreamingAudio::previousSong ) ),
		this->menuItemFromSpriteFile ( "music_player_stop.png" , this, menu_selector ( Ch6_StreamingAudio::stopMusic	) ),
		this->menuItemFromSpriteFile ( "music_player_play.png" , this, menu_selector ( Ch6_StreamingAudio::playMusic	) ),
		this->menuItemFromSpriteFile ( "music_player_pause.png", this, menu_selector ( Ch6_StreamingAudio::pauseMusic	) ),
		this->menuItemFromSpriteFile ( "music_player_next.png" , this, menu_selector ( Ch6_StreamingAudio::nextSong		) ),
		KD_NULL
	);
	pMenu->alignItemsHorizontally ( );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pMenu );
	
	//Initial variable values
	m_nSourceIndex = 0;
	m_bIsPlaying   = KD_FALSE;
	m_pMessage->setScale ( 0.45f );
//	m_pMessage->setPosition ( ccp ( 240, 220 ) );
	
	// Streaming sources
	m_pStreamingSources = new CCArray ( );
	m_pStreamingSources->addObject ( CCString::create ( "http://shoutmedia.abc.net.au:10326" ) );
	m_pStreamingSources->addObject ( CCString::create ( "http://audioplayer.wunderground.com/drgruver/Philadelphia.mp3.m3u" ) );
	m_pStreamingSources->addObject ( CCString::create ( "http://s8.mediastreaming.it:7050/" ) );
	m_pStreamingSources->addObject ( CCString::create ( "http://www.radioparadise.com/musiclinks/rp_64aac.m3u" ) );
	m_pStreamingSources->addObject ( CCString::create ( "http://streaming.wrek.org:8000/wrek_HD-2.m3u" ) );
	
	//Init movie playing (music streamer in this case)
//	moviePlayer = [[MPMoviePlayerController alloc] init];
//	moviePlayer.movieSourceType = MPMovieSourceTypeStreaming;
//	moviePlayer.view.hidden = YES;
//	[[AppDelegate sharedAppDelegate].window addSubview:moviePlayer.view];	
//	[((AppDelegate*)[UIApplication sharedApplication].delegate).window addSubview:moviePlayer.view];

	// Set initial stream source
	this->setStreamSource ( );

	return KD_TRUE;
}

KDvoid Ch6_StreamingAudio::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pStreamingSources );

	Recipe::onExit ( );
}

CCMenuItemSprite* Ch6_StreamingAudio::menuItemFromSpriteFile ( const KDchar* szFile, CCObject* pListener, SEL_MenuHandler pSelector )
{
	ccBlendFunc			tBlendFunc		= { GL_ONE, GL_ONE };
	CCSprite*			pNormalSprite   = CCSprite::createWithSpriteFrameName ( szFile );
	CCSprite*			pSelectedSprite = CCSprite::createWithSpriteFrameName ( szFile );
	pSelectedSprite->setColor ( ccc3 ( 128, 128, 180 ) );
	pSelectedSprite->setBlendFunc ( tBlendFunc );

	CCMenuItemSprite*	pItem = CCMenuItemSprite::create ( pNormalSprite, pSelectedSprite, pListener, pSelector );
	pItem->setScale ( 0.5f );

	return pItem;
}

/// Play callback
KDvoid Ch6_StreamingAudio::playMusic ( CCObject* pSender )
{
//	[moviePlayer play];
	const KDchar*	szURL = ( (CCString*) m_pStreamingSources->objectAtIndex ( m_nSourceIndex ) )->getCString ( );
	this->showMessage ( ccszf ( "Now Playing URL: \n%s", szURL ) );
//		[NSURL URLWithString:[streamingSources objectAtIndex:sourceIndex]]]];
}

/// Pause callback
KDvoid Ch6_StreamingAudio::pauseMusic ( CCObject* pSender )
{
//	[moviePlayer pause];
}

/// Stop callback
KDvoid Ch6_StreamingAudio::stopMusic ( CCObject* pSender )
{
//	[moviePlayer stop];
}

/// Next callback
KDvoid Ch6_StreamingAudio::nextSong ( CCObject* pSender )
{
	this->setIsPlaying ( );
	
	m_nSourceIndex++;
	if ( m_nSourceIndex > (KDint) m_pStreamingSources->count ( ) - 1 )
	{
		m_nSourceIndex = 0;
	}

	this->setStreamSource ( );
}

/// Previous callback
KDvoid Ch6_StreamingAudio::previousSong ( CCObject* pSender )
{
	this->setIsPlaying ( );

	m_nSourceIndex--;
	if ( m_nSourceIndex < 0 )
	{
		m_nSourceIndex = m_pStreamingSources->count ( ) - 1;
	}
	
	this->setStreamSource ( );
}


KDvoid Ch6_StreamingAudio::setIsPlaying ( KDvoid )
{
//	if ( moviePlayer.playbackState == MPMoviePlaybackStatePlaying )
	{
//		m_bIsPlaying = KD_TRUE;
	}
}

KDvoid Ch6_StreamingAudio::setStreamSource ( KDvoid )
{
//	[moviePlayer stop];
	
//	moviePlayer.contentURL = [NSURL URLWithString:[streamingSources objectAtIndex:sourceIndex]];
	
	if ( m_bIsPlaying ) 
	{
		this->playMusic ( KD_NULL );
	}
}
