/* --------------------------------------------------------------------------
 *
 *      File            TestDenshion.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestDenshion.h"

TestDenshion* TestDenshion::create ( KDvoid )
{
	TestDenshion*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new TestSimpleSound	( ); break;
		case  1 : pLayer = new TestSound		( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDbool TestDenshion::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&	tLyrSize = this->getContentSize ( );

	CCTableView*  pTableView = CCTableView::create ( this, ccs ( tLyrSize.cx, tLyrSize.cy - 170.f ) );
	this->addChild ( pTableView );
	pTableView->setDelegate   ( this );	
	pTableView->setDataSource ( this );
	pTableView->setPosition   ( ccp ( 0, 80 ) );
	
	CCMenuItemFont::setFontSize ( 28 );
	CCMenuItemFont::setFontName ( "fonts/ThonburiBold.ttf" );

	return KD_TRUE;
}

KDuint TestDenshion::count ( KDvoid )
{
	return 2;
}

KDuint TestDenshion::numberOfCellsInTableView ( CCTableView* pTable )
{
	return 0;
}

const KDchar* TestDenshion::getName ( KDuint uIndex )
{
	return "";
}

KDvoid TestDenshion::action  ( KDuint uIndex )
{

}

CCSize TestDenshion::cellSizeForTable ( CCTableView* pTable )
{
	const CCSize&  tLyrSize = this->getContentSize ( );

	return CCSize ( tLyrSize.cx, 35.f );
}

CCTableViewCell* TestDenshion::tableCellAtIndex ( CCTableView* pTable, KDuint uIndex )
{
	CCTableViewCell*  pCell = pTable->dequeueCell ( );
	const KDchar*    szName = this->getName ( uIndex );

	if ( !pCell )
	{
		pCell = CCTableViewCell::create ( );
		
		CCMenuItemFont*   pItem = CCMenuItemFont::create ( szName );	
		pCell->addChild ( pItem, 0, 1 );
		pItem->setPosition ( ccpMid ( cellSizeForTable ( pTable ) ) );
	}
	else
	{
		CCMenuItemFont*   pItem = (CCMenuItemFont*) pCell->getChildByTag ( 1 );
		pItem->setString ( szName );	
	}

	return pCell;
}

KDvoid TestDenshion::tableCellTouched ( CCTableView* pTable, CCTableViewCell* pCell )
{
	this->action ( pCell->getIdx ( ) );
}

KDvoid TestDenshion::tableCellHighlight ( CCTableView* pTable, CCTableViewCell* pCell )
{
	( (CCMenuItemFont*) pCell->getChildByTag ( 1 ) )->selected ( );
}

KDvoid TestDenshion::tableCellUnhighlight ( CCTableView* pTable, CCTableViewCell* pCell )
{
	( (CCMenuItemFont*) pCell->getChildByTag ( 1 ) )->unselected ( );
}

KDvoid TestDenshion::scrollViewDidScroll ( CCScrollView* pView )
{

}

KDvoid TestDenshion::scrollViewDidZoom ( CCScrollView* pView )
{

}

//------------------------------------------------------------------
//
// TestSimpleSound
//
//------------------------------------------------------------------

static const KDchar*  l_aTestItems [ ] =
{
    "Play background music"			,
    "Stop background music"			,
    "Pause background music"		,
    "Resume background music"		,
    "Rewind background music"		,
    "Is background music playing"	,
    "Play effect"					,
    "Play effect repeatly"			,
    "Stop effect"					,
    "Unload effect"					,
    "Add background music volume"	,
    "Sub background music volume"	,
    "Add effects volume"			,
    "Sub effects volume"			,
    "Pause effect"					,
    "Resume effect"					,
    "Pause all effects"				,
    "Resume all effects"			,
    "Stop all effects"
};

KDvoid TestSimpleSound::onEnter ( KDvoid )
{
	TestDenshion::onEnter ( );

	SimpleAudioEngine*  pSimpleAudio = SimpleAudioEngine::sharedEngine ( );

    // preload background music and effect
    pSimpleAudio->preloadBackgroundMusic ( "xm_root/background.mp3" );
    pSimpleAudio->preloadEffect ( "xm_root/effect1.wav" );
    
    // set default volume
    pSimpleAudio->setEffectsVolume ( 0.5f );
    pSimpleAudio->setBackgroundMusicVolume ( 0.5f );
}

KDvoid TestSimpleSound::onExit ( KDvoid )
{
	SimpleAudioEngine::end ( );

	TestDenshion::onExit ( );
}

const KDchar* TestSimpleSound::subtitle ( KDvoid )
{
	return "Cocos Denshion Test";
}

KDuint TestSimpleSound::numberOfCellsInTableView ( CCTableView* pTable )
{
	return sizeof ( l_aTestItems ) / sizeof ( l_aTestItems [ 0 ] );
}

const KDchar* TestSimpleSound::getName ( KDuint uIndex )
{
	return l_aTestItems [ uIndex ];
}

KDvoid TestSimpleSound::action  ( KDuint uIndex )
{
	SimpleAudioEngine*  pSimpleAudio = SimpleAudioEngine::sharedEngine ( );

    switch ( uIndex )
    {		
		case  0 :	pSimpleAudio->playBackgroundMusic	( "xm_root/background.mp3", KD_TRUE );						break;
		case  1 :	pSimpleAudio->stopBackgroundMusic	( );														break;
		case  2 :	pSimpleAudio->pauseBackgroundMusic	( );														break;
		case  3 :	pSimpleAudio->resumeBackgroundMusic	( );														break;
		case  4 :	pSimpleAudio->rewindBackgroundMusic	( );														break;
		case  5 :	CCLOG ( "background music is %s", pSimpleAudio->isBackgroundMusicPlaying ( ) ? "playing" : "not playing" );	break;
		case  6 :	m_uSoundID = pSimpleAudio->playEffect ( "xm_root/effect1.wav" );									break;
		case  7 :	m_uSoundID = pSimpleAudio->playEffect ( "xm_root/effect1.wav", KD_TRUE );							break;
		case  8 :	pSimpleAudio->stopEffect   ( m_uSoundID );														break;
		case  9 :	pSimpleAudio->unloadEffect ( "xm_root/effect1.wav" );												break;
		case 10 :	pSimpleAudio->setBackgroundMusicVolume ( pSimpleAudio->getBackgroundMusicVolume ( ) + 0.1f );	break;
		case 11 :	pSimpleAudio->setBackgroundMusicVolume ( pSimpleAudio->getBackgroundMusicVolume ( ) - 0.1f );	break;
		case 12 :	pSimpleAudio->setEffectsVolume	( pSimpleAudio->getEffectsVolume ( ) + 0.1f );					break;			
		case 13 :	pSimpleAudio->setEffectsVolume  ( pSimpleAudio->getEffectsVolume ( ) - 0.1f );					break;
		case 14 :	pSimpleAudio->pauseEffect		( m_uSoundID );													break;
		case 15	:	pSimpleAudio->resumeEffect		( m_uSoundID );													break;
		case 16 :	pSimpleAudio->pauseAllEffects	( );															break;
		case 17 :	pSimpleAudio->resumeAllEffects	( );															break;
		case 18	:	pSimpleAudio->stopAllEffects	( );															break;
    }
}

//------------------------------------------------------------------
//
// TestSound
//
//------------------------------------------------------------------

static const KDchar*  l_aTestSounds [ ] =
{
	"Play : Sound 1"		,
	"Stop : Sound 1"		,
	"Pause : Sound 1"		,
	"Resume : Sound 1"		,
	"Rewind : Sound 1"		,
	"Repeat On : Sound 1"	,
	"Repeat Off : Sound 1"	,
	"Play : Sound 2"		,
	"Stop : Sound 2"		,
	"Rewind : Sound 3"		,
	"Rewind : Sound 4"		,
	"Callback : Sound 5"	,
    "Vibrate : 1000 msec"
};

KDvoid TestSound::onEnter ( KDvoid )
{
	TestDenshion::onEnter ( );

    m_pLabel = CCLabelTTF::create ( "", "fonts/Marker Felt.ttf", 24, CCSizeZero, kCCTextAlignmentLeft );
	this->addChild ( m_pLabel );
	m_pLabel->setColor ( ccRED );
	m_pLabel->ignoreAnchorPointForPosition ( KD_TRUE );
	m_pLabel->setPositionWithParent ( kCCAlignmentLeft, ccp ( 50, 0 ) );

	this->schedule ( schedule_selector ( TestSound::onUpdate ), 0.2f );

	CCSound*  pSound = CCSound::sharedSound ( );

	m_aSoundID [ 0 ] = pSound->Open ( "xm_sounds/background.mp3"	, CCSoundPlay   );
	m_aSoundID [ 1 ] = pSound->Open ( "xm_sounds/background.wav"	, CCSoundRepeat );
	m_aSoundID [ 2 ] = pSound->Open ( "xm_sounds/effect1.wav"		, CCSoundEffect );
	m_aSoundID [ 3 ] = pSound->Open ( "xm_sounds/bullet2.wav"		, CCSoundEffect );
    m_aSoundID [ 4 ] = 0;
    m_aSoundID [ 5 ] = 0;

    m_nIdxNotify = 0;
    m_nIdxSpeed  = 0;
}

KDvoid TestSound::onExit ( KDvoid )
{
	CCSound*  pSound = CCSound::sharedSound ( );

	for ( KDuint i = 0; i < 6; i++ )
	{
		if ( m_aSoundID [ i ] )
		{
			pSound->Close ( m_aSoundID [ i ] );
		}

	}

	TestDenshion::onExit ( );
}

const KDchar* TestSound::subtitle ( KDvoid )
{
	return "CCSound Test";
}

KDuint TestSound::numberOfCellsInTableView ( CCTableView* pTable )
{
	return sizeof ( l_aTestSounds ) / sizeof ( l_aTestSounds [ 0 ] );
}

const KDchar* TestSound::getName ( KDuint uIndex )
{
	return l_aTestSounds [ uIndex ];
}

KDvoid TestSound::action  ( KDuint uIndex )
{
	CCSound*  pSound = CCSound::sharedSound ( );

	switch ( uIndex )
	{
		case  0 :  pSound->Play		 ( m_aSoundID [ 0 ] );				break; 
		case  1 :  pSound->Stop		 ( m_aSoundID [ 0 ] );				break; 
		case  2 :  pSound->Pause	 ( m_aSoundID [ 0 ] );				break; 
		case  3 :  pSound->Resume	 ( m_aSoundID [ 0 ] );				break;
		case  4 :  pSound->Rewind	 ( m_aSoundID [ 0 ] );				break; 
		case  5 :  pSound->setRepeat ( m_aSoundID [ 0 ], KD_TRUE  );	break;
		case  6 :  pSound->setRepeat ( m_aSoundID [ 0 ], KD_FALSE );	break;
		case  7 :  pSound->Play		 ( m_aSoundID [ 1 ] );				break; 
		case  8 :  pSound->Stop		 ( m_aSoundID [ 1 ] );				break;
		case  9 :  pSound->Rewind	 ( m_aSoundID [ 2 ] );				break; 
		case 10 :  pSound->Rewind	 ( m_aSoundID [ 3 ] );				break;            

		case 11 : 
            
            if ( m_aSoundID [ 4 ] )
			{
				pSound->Close ( m_aSoundID [ 4 ] );
			}
    
			if ( m_aSoundID [ 5 ] )
			{
				pSound->Close ( m_aSoundID [ 5 ] );
			}
            
			switch ( m_nIdxNotify ) 
			{
				case 0 : m_aSoundID [ 4 ] = pSound->Open ( "xm_sounds/notify1.wav" ); break;
				case 1 : m_aSoundID [ 4 ] = pSound->Open ( "xm_sounds/notify2.wav" ); break;
				case 2 : m_aSoundID [ 4 ] = pSound->Open ( "xm_sounds/notify3.wav" ); break;
			}
		    
			switch ( m_nIdxSpeed )
			{
				case 0 : m_aSoundID [ 5 ] = pSound->Open ( "xm_sounds/speed_40.wav" ); break;
				case 1 : m_aSoundID [ 5 ] = pSound->Open ( "xm_sounds/speed_50.wav" ); break;
				case 2 : m_aSoundID [ 5 ] = pSound->Open ( "xm_sounds/speed_60.wav" ); break;
				case 3 : m_aSoundID [ 5 ] = pSound->Open ( "xm_sounds/speed_70.wav" ); break;      
				case 4 : m_aSoundID [ 5 ] = pSound->Open ( "xm_sounds/speed_80.wav" ); break;
				case 5 : m_aSoundID [ 5 ] = pSound->Open ( "xm_sounds/speed_90.wav" ); break;
				case 6 : m_aSoundID [ 5 ] = pSound->Open ( "xm_sounds/speed_100.wav" ); break;            
			}
		    
			pSound->setCallFunc ( m_aSoundID [ 4 ], this, sound_selector ( TestSound::onSound ) );
			pSound->Play ( m_aSoundID [ 4 ] );
		    
			m_nIdxNotify = m_nIdxNotify == 2 ? 0 : m_nIdxNotify + 1;
			m_nIdxSpeed  = m_nIdxSpeed  == 6 ? 0 : m_nIdxSpeed  + 1;
            
            break;
            
        case 12 :
            
            xmVibrate ( 1000 );
            
            break;
	}
}

KDvoid TestSound::onUpdate ( KDfloat fDelta )
{
	CCSound*  pSound = CCSound::sharedSound ( );

	m_pLabel->setString 
	(
		ccszf 
		(
			"Duration : %d\nPosition : %d",
			pSound->getDuration ( m_aSoundID [ 0 ] ),
			pSound->getPosition ( m_aSoundID [ 0 ] )
		)
	);
}

KDvoid TestSound::onSound ( KDuint uID, KDint uParam )
{
	CCSound*  pSound = CCSound::sharedSound ( );

	pSound->Rewind ( m_aSoundID [ 5 ] );
}
