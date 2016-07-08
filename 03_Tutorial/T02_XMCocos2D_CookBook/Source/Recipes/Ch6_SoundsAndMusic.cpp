/* --------------------------------------------------------------------------
 *
 *      File            Ch6_SoundsAndMusic.cpp
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
#include "Ch6_SoundsAndMusic.h"

KDbool Ch6_SoundsAndMusic::init ( KDvoid )
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
			CCCallFunc ::create ( this, callfunc_selector ( Ch6_SoundsAndMusic::startRecipe ) ), 
			KD_NULL 
		) 
	);

	return KD_TRUE;
}

KDvoid Ch6_SoundsAndMusic::onExit ( KDvoid )
{
	m_pSAE->stopBackgroundMusic ( );

	Recipe::onExit ( );
}

KDvoid Ch6_SoundsAndMusic::startRecipe ( KDvoid )
{
	// Show message	
	this->showMessage ( "Tap the icons to play sounds and music." );

	// Initialize the audio engine
	m_pSAE = SimpleAudioEngine::sharedEngine ( );

	// Add the sounds
	this->loadSoundEffect ( "crazy_chimp.wav" );
	this->loadSoundEffect ( "rapid_gunfire.wav" );
	this->loadSoundEffect ( "howie_scream.wav" );
	this->loadSoundEffect ( "air_horn.wav" );
	this->loadSoundEffect ( "slide_whistle.wav" );
	
	// Add the background music
	this->loadBackgroundMusic ( "hiphop_boss_man_by_p0ss.mp3" );
	
	// Create our menu
	CCMenu*		pMenu = CCMenu::create 
	(
		this->menuItemFromSpriteFile ( "music_note.png"			, 0 ), 
		this->menuItemFromSpriteFile ( "you_stupid_monkey.png"	, 1 ), 
		this->menuItemFromSpriteFile ( "tommy_gun.png"			, 2 ), 
		this->menuItemFromSpriteFile ( "yaaargh.png"			, 3 ), 
		this->menuItemFromSpriteFile ( "air_horn.png"			, 4 ), 
		this->menuItemFromSpriteFile ( "slide_whistle.png"		, 5 ), 
		KD_NULL
	);
	pMenu->alignItemsInColumns ( 3, 3, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 140 ) );
	this->addChild ( pMenu );
}

// Helper method to create sprite buttons
CCMenuItemSprite* Ch6_SoundsAndMusic::menuItemFromSpriteFile ( const KDchar* szFile, KDint nTag )
{
	ccBlendFunc			tBlendFunc		= { GL_ONE, GL_ONE };
	CCSprite*			pNormalSprite   = CCSprite::create ( szFile );
	CCSprite*			pSelectedSprite = CCSprite::create ( szFile );
	pSelectedSprite->setColor ( ccc3 ( 128, 128, 180 ) );
	pSelectedSprite->setBlendFunc ( tBlendFunc );

	CCMenuItemSprite*	pItem = CCMenuItemSprite::create ( pNormalSprite, pSelectedSprite, this, menu_selector ( Ch6_SoundsAndMusic::playSoundNumber ) );
	pItem->setTag ( nTag );
	pItem->setScale ( 0.9f );

	return pItem;
}

KDvoid Ch6_SoundsAndMusic::loadSoundEffect ( const KDchar* szFile )
{
	// Pre-load sound
	m_pSAE->preloadEffect ( szFile );
}

KDvoid Ch6_SoundsAndMusic::loadBackgroundMusic ( const KDchar* szFile )
{
	// Pre-load background music
	m_pSAE->preloadBackgroundMusic ( szFile );
}

KDvoid Ch6_SoundsAndMusic::playBackgroundMusic ( const KDchar* szFile )
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

//Play sound callback
KDvoid Ch6_SoundsAndMusic::playSoundNumber ( CCObject* pSender )
{
	CCMenuItem*		pItem = (CCMenuItem*) pSender;

	switch ( pItem->getTag ( ) )
	{
		case 0 :	this->playBackgroundMusic ( "hiphop_boss_man_by_p0ss.mp3" );	break;

		case 1 :	this->playSoundFile ( "crazy_chimp.wav" );		break;
		case 2 :	this->playSoundFile ( "rapid_gunfire.wav" );	break;
		case 3 :	this->playSoundFile ( "howie_scream.wav" );		break;
		case 4 :	this->playSoundFile ( "air_horn.wav" );			break;
		case 5 :	this->playSoundFile ( "slide_whistle.wav" );	break;
	}
}

KDvoid Ch6_SoundsAndMusic::playSoundFile ( const KDchar* szFile )
{
	// Play sound
	m_pSAE->playEffect ( szFile );
}