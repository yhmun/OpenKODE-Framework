/* --------------------------------------------------------------------------
 *
 *      File            GameState.cpp
 *      Description     GameState
 *      Author          kimkc
 *      Contact         redfreek2c@gmail.com
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
#include "GameState.h"
#include "ScrMainMenu.h"
#include "ScrMainGame.h"

GameState*		l_pShared = KD_NULL;

static const KDchar*	l_aKorTexts [ ] =
{
	"\xea\xb2\x8c\xec\x9e\x84\x20\xed\x83\x80\xec\x9d\xb4\xed\x8b\x80"	,	// 게임 타이틀",
	"\xea\xb2\x8c\xec\x9e\x84"											,	// 게임
};

static const KDchar*	l_aEngTexts [ ] =
{
	"Game Title",
	"Game"		,
};

static const KDchar*	l_aEtcTexts [ ] =
{
	"asdkfjdsafjlk",
	"asdkfjdsafjlk"		,
};


GameState::GameState ( KDvoid )
{
	m_nState = 0;
}

GameState::~GameState ( KDvoid )
{
	
}

GameState* GameState::sharedGameState ( KDvoid )
{
    if ( !l_pShared )
    {
        l_pShared = new GameState ( );
    }

    return l_pShared;
}

KDvoid GameState::purgeGameState ( KDvoid )
{
	CC_SAFE_DELETE ( l_pShared );
}

const KDchar* GameState::getText ( EText eText )
{	
	KDint   nState = 0; 
	
	switch ( nState )
	{
		case  0 :	return l_aKorTexts [ eText ];
		case  1 :	return l_aEngTexts [ eText ];
		default :	return l_aEtcTexts [ eText ];
	}
}

KDbool GameState::setScene ( EScene eScrID, ETransition eTranID )
{
	CCScene*	pScene = CCScene::create ( );
	CCScene*	pTrans = KD_NULL;
	CCLayer*	pLayer = KD_NULL;
	
	switch ( eScrID )
	{
		case eScrMainMenu :	pLayer = CScrMainMenu::create ( );	break;
		case eScrMainGame :	pLayer = CScrMainGame::create ( );	break;
		default :												break;		
	}

	CCAssert ( pLayer, "pLayer is null" );

	pScene->addChild ( pLayer );

	switch ( eTranID )
	{
		case eTranFade		:	pTrans = CCTransitionCrossFade::create ( 1, pScene );	break;
		case eTranSlideIn	:	pTrans = CCTransitionSlideInR ::create ( 1, pScene );	break;
		default				:	pTrans = pScene;										break;
	}

	if ( !CCDirector::sharedDirector ( )->getRunningScene ( ) )
	{
		CCDirector::sharedDirector ( )->runWithScene ( pScene );
	}
	else
	{
		CCDirector::sharedDirector ( )->replaceScene ( pTrans );
	}

	return KD_TRUE;
}
