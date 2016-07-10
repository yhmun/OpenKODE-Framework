/* -----------------------------------------------------------------------------------
 *
 *      File            Commander.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "Commander.h"
#include "Game/GameMode/GameMode.h"
#include "Game/Combo/Combo.h"
#include "Game/Character/Char.h"
#include "Game/Character/CharHelper.h"
#include "Game/Firewood/Firewood.h"

cCommander::cCommander ( JBaseGameScene* pScene, KDint nStage, KDint nHouseLevel )
: m_pScene					( pScene					     )
, m_pGameMode				( new cGameMode ( pScene, this ) )
, m_pCombo					( new cCombo ( pScene		   ) )
, m_bIsTouch				( true							 )
, m_bIsPlay					( false							 )
, m_bIsPose					( false							 )
, m_bIsTimeModeIntro		( false							 )
, m_bIsBerserkerModeIntro	( false							 )
, m_bIsPerfectSuccess		( false							 )
, m_bIsBerserkSuccess		( false							 )
, m_bIsTimeStopSuccess		( false							 )
, m_nMaxPowerCount			( 0								 )
{
	// error check
	if ( nStage != 0 )
	{
		CCAssert ( false, "" );
	}
}

cCommander::~cCommander ( KDvoid )
{
	// player
	for ( auto pPlayer : m_aPlayers )
	{
		// character
		CC_SAFE_RELEASE ( pPlayer.pChar );
		CC_SAFE_RELEASE ( pPlayer.pCharHelper );

		CC_SAFE_RELEASE ( pPlayer.pFirewoodMgr );
	}
	m_aPlayers.clear ( );

	// gauge
	CC_SAFE_RELEASE ( m_pGameMode );
	
	// combo
	CC_SAFE_RELEASE ( m_pCombo );
}

KDvoid cCommander::addPlayer ( const sPlayerInitInfo& tInfo )
{
	// init
	sPlayer		tPlayer;
	
	// id
	tPlayer.nId = getPlayerNewID ( );
	
	// character
	tPlayer.pChar = new cChar ( m_pScene, this, tInfo.eCharType, tInfo.eLookAt, tPlayer.nId, tInfo.tCharPoint );
	
	// character helper
	tPlayer.pCharHelper = new cCharHelper ( m_pScene, tInfo.eLookAt, tInfo.tCharHelper );
	
	// firewood
	tPlayer.pFirewoodMgr = new cFirewoodMgr ( m_pScene, this, tPlayer.nId, tInfo.tFirewoodIntro, tInfo.tFirewoodPreview, tInfo.tFirewoodStandBy, tInfo.tFirewoodProduct );
	
	// firewood first setting
	tPlayer.pFirewoodMgr->firstSetting ( true );

	// other
	tPlayer.bIsSplitting	= false;
	tPlayer.dSplittingDelay = 0;
	
	// add
	m_aPlayers.push_back ( tPlayer );
}

KDint cCommander::getPlayerNewID ( KDvoid )
{
	return m_aPlayers.back ( ).nId + 1;
}

KDvoid cCommander::getPlayer ( KDint nId, sPlayer& tRetPlayer )
{
	for ( auto tPlayer : m_aPlayers )
	{
		if ( tPlayer.nId == nId )
		{
			tRetPlayer = tPlayer;			
			return;
		}
	}
	
	// error
	CCAssert ( false, "" );
}

KDvoid cCommander::playInit ( KDvoid )
{
	// character
	for ( auto tPlayer : m_aPlayers )
	{
		// firewood first setting
		tPlayer.pFirewoodMgr->firstSetting ( false );
	}
	
	// play
	m_bIsPlay = false;
	
	// pose
	m_bIsPose = false;

	// game mode
	m_pGameMode->setGaugeZero ( );

	// combo
	m_pCombo->setZero ( );
}

KDvoid cCommander::playStart ( KDvoid )
{
	// switch
	m_bIsPlay = true;
	
	// loop
	for ( auto tPlayer : m_aPlayers )
	{
		// character
		tPlayer.pChar->setPowerGetting ( true );

		// character helper
		tPlayer.pCharHelper->setAniThrow ( );
		
		// firewood
		tPlayer.pFirewoodMgr->request ( );

		// splitting
		tPlayer.bIsSplitting = true;
		tPlayer.dSplittingDelay = 0;
	}

	m_bIsPerfectSuccess	 = true;
	m_bIsBerserkSuccess	 = false;
	m_bIsTimeStopSuccess = false;
	m_nMaxPowerCount	 = 0;
}

KDvoid cCommander::playStop ( KDvoid )
{
	// switch
	m_bIsPlay = false;
	
	// loop
	for ( auto tPlayer : m_aPlayers )
	{
		// character
		tPlayer.pChar->setPowerGetting ( false );

		// character
		tPlayer.pChar->setNextAniNor ( );
	}
}

KDvoid cCommander::playEnd ( KDvoid )
{
	// switch
	m_bIsPlay = false;
	
	// loop
	for ( auto tPlayer : m_aPlayers )
	{
		// character
		tPlayer.pChar->setPowerGetting ( false );

		// character
		tPlayer.pChar->setAniNor ( );
	}
}

KDvoid cCommander::setPose ( KDbool bIsPose )
{
	m_bIsPose = bIsPose;
}

KDbool cCommander::getIsTimeStopMode ( KDvoid )
{
	return m_pGameMode->getIsTimeMode ( );
}

KDvoid cCommander::msgFirewoodPreviewStandby ( KDint nPlayerID )
{
	// init
	sPlayer		tPlayer;
	
	// getPlayer
	getPlayer( nPlayerID, tPlayer );
	
	// character helper animatin select
	tPlayer.pCharHelper->setAniGet ( );
}

KDvoid cCommander::msgFirewoodProduct ( KDint nPlayerID, KDint nLevel )
{
	
}

KDvoid cCommander::msgIsComboSuccess ( KDint nPlayerID, KDbool bIsSuccess )
{
	
}

KDvoid cCommander::msgCloudAdd ( KDint z, KDdouble dStartDelay, Point tPoint, Point tAdd, KDdouble dLeftTime, KDfloat fScale, KDfloat fScaleAdd, KDfloat fRotation, KDfloat fRotationAdd, KDbool bIsFlipX )
{
	// add
	m_pScene->msgCloudAdd ( z, dStartDelay,tPoint, tAdd, dLeftTime, fScale, fScaleAdd, fRotation, fRotationAdd, bIsFlipX ); 
}

KDvoid cCommander::msgTimeModeIntro ( KDbool bIsTimeModeIntro )
{
	m_bIsTimeModeIntro = bIsTimeModeIntro;
}

KDvoid cCommander::msgBerserkerModeIntro ( KDbool bIsBerserkerModeIntro )
{
	m_bIsBerserkerModeIntro = bIsBerserkerModeIntro;
}

KDvoid cCommander::msgSprBackGray ( KDbool bIsGray )
{
	m_pScene->msgSprBackGray ( bIsGray );
}

KDvoid cCommander::msgCharAngry ( KDbool bIsAngry )
{
	for ( auto tPlayer : m_aPlayers )
	{
		tPlayer.pChar->setIsAngry ( bIsAngry );
	}
}

KDvoid cCommander::touchBegan ( KDvoid )
{
	// check
	if ( m_bIsPlay == false || m_bIsTouch == false || m_bIsTimeModeIntro == true || m_bIsBerserkerModeIntro == true )
	{
		return;
	}
	
	// loop
	for ( sPlayer& tPlayer : m_aPlayers )
	{
		KDint		nStandbyFirewoodLv	= tPlayer.pFirewoodMgr->getStandbyLevel ( );
		KDint		nCharPowerLv		= tPlayer.pChar->getPowerLv ( );
		KDbool		bIsSuccess			= false;
		KDint		nFirewoodLv			= 0;
		
		// character, firewood check
		if( tPlayer.pChar->getIsPowerGetting ( ) == false || tPlayer.pFirewoodMgr->getIsStandBy ( ) == false )
		{
			continue;
		}
		
		// splitting
		tPlayer.pFirewoodMgr->splitting ( nCharPowerLv, bIsSuccess, nFirewoodLv );

		// score
		if ( bIsSuccess == true )
		{
			KDint	nScore = 0;
			
			switch ( nFirewoodLv )
			{
				case 0	:	nScore += 100 + ( m_pCombo->getComboCount ( ) * 10 );	break;
				case 1	:	nScore += 300 + ( m_pCombo->getComboCount ( ) * 30 );	break;
				case 2	:	nScore += 600 + ( m_pCombo->getComboCount ( ) * 60 );	break;
			}

			if ( tPlayer.pChar->getIsMaxPower ( ) == true )
			{
				nScore += 1000;
			}

			m_pScene->msgAddScore ( nScore );
		}
		
		// clear point
		if ( bIsSuccess == true )
		{
			if ( tPlayer.pChar->getIsMaxPower ( ) == true )
			{
				switch ( nFirewoodLv )
				{
					case 0	:	m_pScene->msgClearPointAdd ( 4 );	break;
					case 1	:	m_pScene->msgClearPointAdd ( 4 );	break;
					case 2	:	m_pScene->msgClearPointAdd ( 5 );	break;
				}
			}
			else
			{
				switch ( nFirewoodLv )
				{
					case 0	:	m_pScene->msgClearPointAdd ( 1 );	break;
					case 1	:	m_pScene->msgClearPointAdd ( 2 );	break;
					case 2	:	m_pScene->msgClearPointAdd ( 4 );	break;
				}
			}
		}
		
		// sound
		if ( cGlobalData::getObj ( )->m_bIsEffectMusic == true )
		{
			if ( bIsSuccess == true )
			{
				switch ( nFirewoodLv )
				{
					case 0	:	SimpleAudioEngine::getInstance ( )->playEffect ( "axe_1_small.wav"  );	break;
					case 1	:	SimpleAudioEngine::getInstance ( )->playEffect ( "axe_2_medium.wav" );	break;
					case 2	:	SimpleAudioEngine::getInstance ( )->playEffect ( "axe_3_large.wav"  );	break;
				}
			}
			else
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "axe_miss.wav" );
			}
		}
		
		// combo & perfect effect 
		KDbool	bIsComboAdd = false;
		if ( bIsSuccess == true )
		{
			if ( tPlayer.pChar->getIsMaxPower ( ) == true )
			{
				tPlayer.pChar->addMaxPowerEffect ( );
				m_pCombo->setAdd ( );
				bIsComboAdd = true;
			}
			else if ( ( nCharPowerLv == 0 && nStandbyFirewoodLv == 0 ) || ( nCharPowerLv == 1 && nStandbyFirewoodLv == 1 ) || ( nCharPowerLv == 3 && nStandbyFirewoodLv == 2 ) )
			{
				tPlayer.pChar->addPerfectEffect ( );
				m_pCombo->setAdd ( );
				bIsComboAdd = true;
			}
			else
			{
				if ( m_pGameMode->getIsBerserkerMode ( ) == true )
				{
					tPlayer.pChar->addPerfectEffect ( );
					m_pCombo->setAdd ( );
				}
				else
				{
					m_pCombo->setZero ( );
				}
			}
		}
		else
		{
			m_pCombo->setZero ( );
		}
		
		// time add
		if ( m_pGameMode->getIsTimeMode ( ) == true && bIsSuccess == true )
		{
			m_pScene->msgAddTime ( 1 );
		}
		
		// game mode gauge
		m_pGameMode->setAddTimeGauge ( !( bIsSuccess ), bIsComboAdd );
		m_pGameMode->setAddBerserkerGauge ( !(bIsSuccess), m_pCombo->getComboCount ( ), tPlayer.pChar->getIsMaxPower ( ) );

		// result data check
		if ( bIsSuccess == true )
		{
			++m_nMaxPowerCount;
		}
		else
		{
			m_bIsPerfectSuccess = false;
		}
		
		if ( m_pGameMode->getIsBerserkerMode ( ) == true )	m_bIsBerserkSuccess	 = true;
		if ( m_pGameMode->getIsTimeMode	     ( ) == true )	m_bIsTimeStopSuccess = true;
		
		// character animation
		if ( bIsSuccess == false )	tPlayer.pChar->setAniMiss   ( );
		else						tPlayer.pChar->setAniFinish ( );
		
		// character power getting no
		tPlayer.pChar->setPowerGetting ( false );
		
		// splitting
		if ( bIsSuccess == true )
		{
			tPlayer.bIsSplitting	= false;
			tPlayer.dSplittingDelay = 0.5f;
			tPlayer.dFirewoodDelay  = 0.31f;
		}
		else
		{
			tPlayer.bIsSplitting	= false;
			tPlayer.dSplittingDelay = 0.7f;
			tPlayer.dFirewoodDelay  = 0.51f;
		}
	}
}

KDvoid cCommander::update ( KDdouble dLeftTime )
{
	// pose check
	if ( m_bIsPose == true )
	{
		return;
	}

	// intro
	if ( m_bIsPlay == false )
	{
		updateIntro ( dLeftTime );
		
		return;
	}
	
	// gameing
	updateGame ( dLeftTime );
}

KDvoid cCommander::updateIntro ( KDdouble dLeftTime )
{
	// loop
	for ( auto& tPlayer : m_aPlayers )
	{		
		// char
		tPlayer.pChar->update ( dLeftTime );
		
		// char helper
		tPlayer.pCharHelper->update ( dLeftTime );
		
		// firewood
		tPlayer.pFirewoodMgr->update ( dLeftTime );
	}
}

KDvoid cCommander::updateGame ( KDdouble dLeftTime )
{
	// backup
	KDdouble	dLeftTimeBackup = dLeftTime;
	
	// time mode intro
		 if ( m_bIsBerserkerModeIntro		 == true )	dLeftTime /= 3.5;
	else if ( m_bIsTimeModeIntro			 == true )	dLeftTime /= 3.5;
	else if ( m_pGameMode->getIsTimeMode ( ) == true )	dLeftTime *= 0.7;
	
	// combo
	m_pCombo->update ( dLeftTime );

	// game mode
	m_pGameMode->update ( dLeftTimeBackup );
	
	// loop
	for ( auto& tPlayer : m_aPlayers )
	{		
		// splitting check
		if ( tPlayer.bIsSplitting == false )
		{
			// time
			tPlayer.dSplittingDelay -= dLeftTime;
			tPlayer.dFirewoodDelay  -= dLeftTime;
			
			// firewood request delay
			if ( tPlayer.dFirewoodDelay < 0 && tPlayer.pFirewoodMgr->getIsStandByThrowing ( ) == false && tPlayer.pFirewoodMgr->getIsStandBy ( ) == false )
			{
				// firewood request
				tPlayer.pFirewoodMgr->request ( );
				
				// character helper
				tPlayer.pCharHelper->setAniThrow ( );
			}
			
			// splitting check
			if ( tPlayer.dSplittingDelay < 0 )
			{
				// switch
				tPlayer.bIsSplitting = true;
				
				// character
				tPlayer.pChar->setPowerGetting ( true );
			}
		}
		
		// char
		tPlayer.pChar->update ( dLeftTime );
		
		// char helper
		tPlayer.pCharHelper->update ( dLeftTime );
		
		// firewood
		tPlayer.pFirewoodMgr->update ( dLeftTime );
	}
}
