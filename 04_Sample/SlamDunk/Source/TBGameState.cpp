/* --------------------------------------------------------------------------
 *
 *      File            TBGameState.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Rana Hammad Hussain on 04/22/11
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Azosh & Co. All rights reserved.
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
#include "TBGameState.h"

static TBGameState*		l_pSharedGameState = KD_NULL;

TBGameState* TBGameState::sharedGameState ( KDvoid )
{
	if ( !l_pSharedGameState )
	{
		l_pSharedGameState = new TBGameState ( );
		l_pSharedGameState->init ( );
	}

	return l_pSharedGameState;
}

KDvoid TBGameState::purgedGameState ( KDvoid )
{
	if ( l_pSharedGameState )
	{
		l_pSharedGameState->saveState ( );
	}

	CC_SAFE_DELETE ( l_pSharedGameState );
}

KDbool TBGameState::init ( KDvoid )
{
	m_tScreenSize = CCDirector::sharedDirector ( )->getWinSize ( );
	m_tScreenCenter = ccpMid ( m_tScreenSize );
	m_bGameSceneRunning = KD_FALSE;
	
	this->loadState ( );

	return KD_TRUE;
}

KDvoid TBGameState::loadState ( KDvoid )
{
	CCUserDefault*		pDefault = CCUserDefault::sharedUserDefault ( );

	m_bFirstTimeRunning = pDefault->getBoolForKey ( "GAME.STATE.FIRSTTIMERUNNING", KD_TRUE );
	m_nSelectedCharacterIndex = pDefault->getIntegerForKey ( "GAME.STATE.SELECTEDCHARACTER", 1 );
}

KDvoid TBGameState::saveState ( KDvoid )
{
	CCUserDefault*		pDefault = CCUserDefault::sharedUserDefault ( );

	pDefault->setBoolForKey ( "GAME.STATE.FIRSTTIMERUNNING", m_bFirstTimeRunning );
	pDefault->setIntegerForKey ( "GAME.STATE.SELECTEDCHARACTER", m_nSelectedCharacterIndex );
	pDefault->flush ( );
}

KDvoid TBGameState::playGamemusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "gamemusic.wav", KD_TRUE );
}

KDvoid TBGameState::stopGamemusic ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->stopBackgroundMusic ( );
}

KDvoid TBGameState::playClick ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "click.wav" );
}	

KDvoid TBGameState::playBallHit ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "ballhit_option2.wav" );
}