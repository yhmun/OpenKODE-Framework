/* --------------------------------------------------------------------------
 *
 *      File            TheMenu.cpp    
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
#include "TheMenu.h"
#include "TheLevel.h"
#include "GameData.h"
#include "GameSounds.h"

KDbool TheMenu::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCPoint		tMenu1Position;
	CCPoint		tMenu2Position;

	m_sMenuBackgroundName			= "menu_background.png";  
	// will use "menu_background.png" for non-Retina Phones 
	// will use "menu_background-hd.png"; for retina phones
	// will use "menu_background-ipad.png";
	
	// same goes for images below..

	m_aLvlButtonNames		[ 0 ]	= "levelButton1.png";
	m_aLvlLockedButtonNames	[ 0 ]	= "levelButton1_locked.png";

	m_aLvlButtonNames		[ 1 ]	= "levelButton2.png";
	m_aLvlLockedButtonNames	[ 1 ]	= "levelButton2_locked.png";

	m_aLvlButtonNames		[ 2 ]	= "levelButton3.png";
	m_aLvlLockedButtonNames	[ 2 ]	= "levelButton3_locked.png";

	m_aLvlButtonNames		[ 3 ]	= "levelButton4.png";
	m_aLvlLockedButtonNames	[ 3 ]	= "levelButton4_locked.png";

	m_aLvlButtonNames		[ 4 ]	= "levelButton5.png";
	m_aLvlLockedButtonNames	[ 4 ]	= "levelButton5_locked.png";

	m_aLvlButtonNames		[ 5 ]	= "levelButton6.png";
	m_aLvlLockedButtonNames	[ 5 ]	= "levelButton6_locked.png";

	m_aLvlButtonNames		[ 6 ]	= "levelButton7.png";
	m_aLvlLockedButtonNames	[ 6 ]	= "levelButton7_locked.png";

	m_aLvlButtonNames		[ 7 ]	= "levelButton8.png";
	m_aLvlLockedButtonNames	[ 7 ]	= "levelButton8_locked.png";

	m_aLvlButtonNames		[ 8 ]	= "levelButton9.png";
	m_aLvlLockedButtonNames	[ 8 ]	= "levelButton9_locked.png";

	m_aLvlButtonNames		[ 9 ]	= "levelButton10.png";
	m_aLvlLockedButtonNames	[ 9 ]	= "levelButton10_locked.png";

	m_sVoiceButtonName				= "voiceFX.png";
	m_sVoiceButtonNameDim			= "voiceFX_dim.png";

	m_sSoundButtonName				= "soundFX.png";
	m_sSoundButtonNameDim			= "soundFX_dim.png";

	m_sAmbientButtonName			= "ambientFX.png";
	m_sAmbientButtonNameDim			= "ambientFX_dim.png";

	if ( TheLevel::sharedLevel ( )->IS_IPAD )
	{
		// iPADs..
		
		tMenu1Position = ccp ( m_tContentSize.cx / 2, 430 );
		tMenu2Position = ccp ( m_tContentSize.cx / 2, 290 );
		
		m_tSoundFXMenuLocation   = ccp ( 240, 170 );
		m_tVoiceFXMenuLocation   = ccp ( 480, 170 );
		m_tAmbientFXMenuLocation = ccp ( 750, 170 );

		if ( !TheLevel::sharedLevel ( )->IS_RETINA )
		{
			CCLOG ( "must be iPad 1 or 2" );

			// change nothing 
		}
		else
		{
			CCLOG ( "retina display is on-must be iPAd 3" );

			// change files names for iPad 3 

			m_sMenuBackgroundName			= "menu_background-ipad.png";		// will use @"menu_background-ipad-hd.png"; 

			m_aLvlButtonNames		[ 0 ]	= "levelButton1-ipad.png";
			m_aLvlLockedButtonNames	[ 0 ]	= "levelButton1_locked-ipad.png";
			
			m_aLvlButtonNames		[ 1 ]	= "levelButton2-ipad.png";
			m_aLvlLockedButtonNames	[ 1 ]	= "levelButton2_locked-ipad.png";
			
			m_aLvlButtonNames		[ 2 ]	= "levelButton3-ipad.png";
			m_aLvlLockedButtonNames	[ 2 ]	= "levelButton3_locked-ipad.png";
			
			m_aLvlButtonNames		[ 3 ]	= "levelButton4-ipad.png";
			m_aLvlLockedButtonNames	[ 3 ]	= "levelButton4_locked-ipad.png";
			
			m_aLvlButtonNames		[ 4 ]	= "levelButton5-ipad.png";
			m_aLvlLockedButtonNames	[ 4 ]	= "levelButton5_locked-ipad.png";
			
			m_aLvlButtonNames		[ 5 ]	= "levelButton6-ipad.png";
			m_aLvlLockedButtonNames	[ 5 ]	= "levelButton6_locked-ipad.png";
			
			m_aLvlButtonNames		[ 6 ]	= "levelButton7-ipad.png";
			m_aLvlLockedButtonNames	[ 6 ]	= "levelButton7_locked-ipad.png";
			
			m_aLvlButtonNames		[ 7 ]	= "levelButton8-ipad.png";
			m_aLvlLockedButtonNames	[ 7 ]	= "levelButton8_locked-ipad.png";
			
			m_aLvlButtonNames		[ 8 ]	= "levelButton9-ipad.png";
			m_aLvlLockedButtonNames	[ 8 ]	= "levelButton9_locked-ipad.png";
			
			m_aLvlButtonNames		[ 9 ]	= "levelButton10-ipad.png";
			m_aLvlLockedButtonNames	[ 9 ]	= "levelButton10_locked-ipad.png";

			m_sVoiceButtonName				= "voiceFX-ipad.png";
			m_sVoiceButtonNameDim			= "voiceFX_dim-ipad.png";

			m_sSoundButtonName				= "soundFX-ipad.png";
			m_sSoundButtonNameDim			= "soundFX_dim-ipad.png";

			m_sAmbientButtonName			= "ambientFX-ipad.png";
			m_sAmbientButtonNameDim			= "ambientFX_dim-ipad.png";
		}
	}
	else
	{
		// IPHONES..

		tMenu1Position = ccp ( m_tContentSize.cx / 2, 185 );
		tMenu2Position = ccp ( m_tContentSize.cx / 2, 115 );
		
		m_tSoundFXMenuLocation   = ccp ( 110, 55 );
		m_tVoiceFXMenuLocation   = ccp ( 230, 55 );
		m_tAmbientFXMenuLocation = ccp ( 355, 55 );
	}

	CCSprite*		pTheBackground = CCSprite::create ( m_sMenuBackgroundName.c_str ( ) );
	pTheBackground->setPosition ( ccpMid ( m_tContentSize ) );
	this->addChild ( pTheBackground, 0 );

	this->setTouchEnabled ( KD_TRUE );

	CCMenuItem*		aButtons [ 10 ];

	for ( KDint i = 0; i < 10; i++ )
	{
		if ( GameData::sharedData ( )->canYouGoToTheFirstLevelOfThisSection ( i + 1 ) == KD_FALSE )
		{
			aButtons [ i ] = CCMenuItemImage::create ( m_aLvlLockedButtonNames [ i ].c_str ( ), m_aLvlLockedButtonNames [ i ].c_str ( ), this, menu_selector ( TheMenu::playNegativeSound ) );
		}
		else
		{
			aButtons [ i ] = CCMenuItemImage::create ( m_aLvlButtonNames [ i ].c_str ( ), m_aLvlButtonNames [ i ].c_str ( ), this, menu_selector ( TheMenu::goToFirstLevelSection ) );
		}

		aButtons [ i ]->setTag ( i );
	}

	CCMenu*			pMenu1 = CCMenu::create ( aButtons [ 0 ], aButtons [ 1 ], aButtons [ 2 ], aButtons [ 3 ], aButtons [ 4 ], KD_NULL );
	pMenu1->setPosition ( tMenu1Position );
	pMenu1->alignItemsHorizontallyWithPadding ( 10 );
	this->addChild ( pMenu1, 1 );

	CCMenu*			pMenu2 = CCMenu::create ( aButtons [ 5 ], aButtons [ 6 ], aButtons [ 7 ], aButtons [ 8 ], aButtons [ 9 ], KD_NULL );
	pMenu2->setPosition ( tMenu2Position );
	pMenu2->alignItemsHorizontallyWithPadding ( 10 );
	this->addChild ( pMenu2, 1 );

	m_pVoiceFXMenu	 = KD_NULL;
	m_pSoundFXMenu	 = KD_NULL;
	m_pAmbientFXMenu = KD_NULL;

	this->setSoundFXMenuActive   ( !GameData::sharedData ( )->areSoundFXMuted   ( ) );
	this->setVoiceFXMenuActive   ( !GameData::sharedData ( )->areVoiceFXMuted   ( ) );
	this->setAmbientFXMenuActive ( !GameData::sharedData ( )->areAmbientFXMuted ( ) );

	return KD_TRUE;
}

KDvoid TheMenu::goToFirstLevelSection ( CCObject* pSender )
{
	CCNode*		pNode = (CCNode*) pSender;
	KDint		nSection = pNode->getTag ( ) + 1;

	if ( GameData::sharedData ( )->canYouGoToTheFirstLevelOfThisSection ( nSection ) )
	{
		GameData::sharedData ( )->changeLevelToFirstInThisSection ( nSection );

		this->popAndTransition ( );	
	}
}

KDvoid TheMenu::playNegativeSound ( CCObject* pSender )
{
	GameSounds::sharedGameSounds ( )->playSoundFX ( "bloop.wav" );
}

KDvoid TheMenu::popAndTransition ( KDvoid )
{
	CCDirector::sharedDirector ( )->popScene ( );

	// when TheLevel scene reloads it will start with a new level
	TheLevel::sharedLevel ( )->transitionAfterMenuPop ( );
}

KDvoid TheMenu::setSoundFXMenuActive ( KDbool bValue )
{
	if ( m_pSoundFXMenu )
	{
		this->removeChild ( m_pSoundFXMenu, KD_TRUE );
	}

	CCMenuItem*		pButton1;
	if ( !bValue )
	{
		pButton1 = CCMenuItemImage::create ( m_sSoundButtonNameDim.c_str ( ), m_sSoundButtonNameDim.c_str ( ), this, menu_selector ( TheMenu::turnSoundFXOn ) );
	}
	else
	{
		pButton1 = CCMenuItemImage::create ( m_sSoundButtonName.c_str ( ), m_sSoundButtonName.c_str ( ), this, menu_selector ( TheMenu::turnSoundFXOff ) );
	}

	m_pSoundFXMenu = CCMenu::createWithItem ( pButton1 );
	m_pSoundFXMenu->setPosition ( m_tSoundFXMenuLocation );
	
	this->addChild ( m_pSoundFXMenu, 10 );
}

KDvoid TheMenu::turnSoundFXOn ( CCObject* pSender )
{
	GameData::sharedData ( )->setSoundFXMuted ( KD_FALSE );			
	GameSounds::sharedGameSounds ( )->setSoundFXMuted ( KD_FALSE );
	this->setSoundFXMenuActive ( KD_TRUE );
}

KDvoid TheMenu::turnSoundFXOff ( CCObject* pSender )
{
	GameData::sharedData ( )->setSoundFXMuted ( KD_TRUE );			
	GameSounds::sharedGameSounds ( )->setSoundFXMuted ( KD_TRUE );
	this->setSoundFXMenuActive ( KD_FALSE );
}

KDvoid TheMenu::setVoiceFXMenuActive ( KDbool bValue )
{
	if ( m_pVoiceFXMenu )
	{
		this->removeChild ( m_pVoiceFXMenu, KD_TRUE );
	}

	CCMenuItem*		pButton1;
	if ( !bValue )
	{
		pButton1 = CCMenuItemImage::create ( m_sVoiceButtonNameDim.c_str ( ), m_sVoiceButtonNameDim.c_str ( ), this, menu_selector ( TheMenu::turnVoiceFXOn ) );
	}
	else
	{
		pButton1 = CCMenuItemImage::create ( m_sVoiceButtonName.c_str ( ), m_sVoiceButtonName.c_str ( ), this, menu_selector ( TheMenu::turnVoiceFXOff ) );
	}

	m_pVoiceFXMenu = CCMenu::createWithItem ( pButton1 );
	m_pVoiceFXMenu->setPosition ( m_tVoiceFXMenuLocation );
	
	this->addChild ( m_pVoiceFXMenu, 10 );
}

KDvoid TheMenu::turnVoiceFXOn ( CCObject* pSender )
{
	GameData::sharedData ( )->setVoiceFXMuted ( KD_FALSE );			
	GameSounds::sharedGameSounds ( )->setVoiceFXMuted ( KD_FALSE );
	this->setVoiceFXMenuActive ( KD_TRUE );
}

KDvoid TheMenu::turnVoiceFXOff ( CCObject* pSender )
{
	GameData::sharedData ( )->setVoiceFXMuted ( KD_TRUE );			
	GameSounds::sharedGameSounds ( )->setVoiceFXMuted ( KD_TRUE );
	this->setVoiceFXMenuActive ( KD_FALSE );
}

KDvoid TheMenu::setAmbientFXMenuActive ( KDbool bValue )
{
	if ( m_pAmbientFXMenu )
	{
		this->removeChild ( m_pAmbientFXMenu, KD_TRUE );
	}

	CCMenuItem*		pButton1;
	if ( !bValue )
	{
		pButton1 = CCMenuItemImage::create ( m_sAmbientButtonNameDim.c_str ( ), m_sAmbientButtonNameDim.c_str ( ), this, menu_selector ( TheMenu::turnAmbientFXOn ) );
	}
	else
	{
		pButton1 = CCMenuItemImage::create ( m_sAmbientButtonName.c_str ( ), m_sAmbientButtonName.c_str ( ), this, menu_selector ( TheMenu::turnAmbientFXOff ) );
	}

	m_pAmbientFXMenu = CCMenu::createWithItem ( pButton1 );
	m_pAmbientFXMenu->setPosition ( m_tAmbientFXMenuLocation );
	
	this->addChild ( m_pAmbientFXMenu, 10 );
}

KDvoid TheMenu::turnAmbientFXOn ( CCObject* pSender )
{
	GameData::sharedData ( )->setAmbientFXMuted ( KD_FALSE );			
	GameSounds::sharedGameSounds ( )->restartBackgroundMusic ( );
	this->setAmbientFXMenuActive ( KD_TRUE );
}

KDvoid TheMenu::turnAmbientFXOff ( CCObject* pSender )
{
	GameData::sharedData ( )->setAmbientFXMuted ( KD_TRUE );			
	GameSounds::sharedGameSounds ( )->stopBackgroundMusic ( );
	this->setAmbientFXMenuActive ( KD_FALSE );
}