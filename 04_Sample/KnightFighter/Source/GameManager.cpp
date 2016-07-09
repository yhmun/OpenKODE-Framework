/* --------------------------------------------------------------------------
 *
 *      File            GameManager.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 27/04/2011
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "GameManager.h"
#include "InstructionsScene.h"
#include "GameScene.h"
#include "ShootOutScene.h"
#include "CoordinateFunctions.h"

static GameManager*		l_pManager = KD_NULL;

GameManager* GameManager::sharedManager ( KDvoid )
{
	if ( !l_pManager )
	{
		l_pManager = new GameManager ( );
	}

	return l_pManager;
}

GameManager::GameManager ( KDvoid )
{
	m_bSoundOn			= KD_TRUE;
	m_bMusicOn			= KD_TRUE;
	m_bIsIPad			= KD_FALSE;

	m_nMaxPlayerLives	= 0;
	m_nPlayerLives		= 0;
	m_nLevel			= 0;
	m_nMaxLevels		= 2;

	m_pCoordinateFunctions = KD_NULL;

	this->setCoordinateFunctions ( CoordinateFunctions::create ( ) );
}

GameManager::~GameManager ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pCoordinateFunctions );
}

KDvoid GameManager::showMenu ( KDvoid )
{
	m_eGameState = MainMenu;
	CCDirector::sharedDirector ( )->popScene ( );

	SimpleAudioEngine*		pSE = SimpleAudioEngine::sharedEngine ( );
	if ( !pSE->isBackgroundMusicPlaying ( ) && m_bMusicOn )
	{
		pSE->preloadBackgroundMusic ( "titletheme.mp3" );
	}
}

KDvoid GameManager::showInstructions ( KDvoid )
{	
	m_eGameState = Instructions;
	CCDirector::sharedDirector ( )->pushScene ( InstructionsScene::scene ( ) );
}

KDvoid GameManager::startGame ( KDvoid )
{
	m_eGameState		= Play;
	m_nMaxPlayerLives	= 3;
	m_nLevel			= 1;
	
	CCDirector::sharedDirector ( )->pushScene ( KnightFight::scene ( ) );

	SimpleAudioEngine::sharedEngine ( )->stopBackgroundMusic ( );
	SimpleAudioEngine::sharedEngine ( )->setEffectsVolume ( 0.3f );
}

KDvoid GameManager::shootOut ( KDvoid )
{
	CCLOG ( "In shootOut, in app delegate" );
	CCDirector::sharedDirector ( )->pushScene ( ShootOutScene::scene ( ) );
}
