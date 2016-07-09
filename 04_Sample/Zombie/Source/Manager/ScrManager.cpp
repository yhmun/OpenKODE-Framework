/* --------------------------------------------------------------------------
 *
 *      File            ScrManager.cpp
 *      Description     Scene Manager 
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "EngManager.h"
#include "EnvManager.h"
#include "ResManager.h"
#include "ScrManager.h"

#include "../Screen/ScrSplash.h"
#include "../Screen/ScrIntro.h"
#include "../Screen/ScrMain.h"
#include "../Screen/ScrLoading.h"
#include "../Screen/ScrGame.h"

CScrManager*  g_pScrMgr = KD_NULL;

CScrManager::CScrManager ( KDvoid )
{
	m_pScreen = KD_NULL;

	setScreen ( eScrSplash );
}

CScrManager::~CScrManager ( KDvoid )
{

}

KDvoid CScrManager::create  ( KDvoid )
{
	if ( !g_pScrMgr )
	{
		g_pScrMgr = new CScrManager ( );
	}

	kdAssert ( g_pScrMgr );
}

KDvoid CScrManager::release ( KDvoid )
{
	CC_SAFE_DELETE ( g_pScrMgr );
}

KDvoid CScrManager::setScreen ( EScreen eScreen, ETransition eTransition, KDfloat fDuration )
{
	CCLayer*  pLayer = KD_NULL;
	switch ( eScreen )
	{
		case eScrSplash		:	pLayer = CScrSplash		::create ( );	 break;
		case eScrIntro		:	pLayer = CScrIntro		::create ( );  break;    
		case eScrMain		:	pLayer = CScrMain		::create ( );  break;    
		case eScrLoading	:	pLayer = CScrLoading	::create ( );  break;    
		case eScrGame		:	pLayer = CScrGame		::create ( );  break;  

		default : break;
	}

	CC_ASSERT ( pLayer );

	CCScene*    pScene = CCScene::create ( );
	CC_ASSERT ( pScene );

	pScene->addChild ( pLayer, 0, eScreen );

	if ( m_pScreen )
	{
		m_pScreen->unscheduleAllSelectors ( );
	}

    m_eScreen = eScreen;
	m_pScreen = (CCLayer *) pScene->getChildByTag ( eScreen );

	if ( eScreen == eScrSplash )
	{
		CCDirector::sharedDirector ( )->runWithScene ( pScene );
	}
	else
	{
		CCScene*  pScene2 = KD_NULL;
		switch ( eTransition )
		{
			case eTranBlackFade		:	pScene2 = CCTransitionFade          ::create ( fDuration, pScene );  break;
			case eTranMoveInL		:	pScene2 = CCTransitionMoveInL       ::create ( fDuration, pScene );  break;
			case eTranMoveInR		:	pScene2 = CCTransitionMoveInR       ::create ( fDuration, pScene );  break;
			case eTranMoveInT		:	pScene2 = CCTransitionMoveInT       ::create ( fDuration, pScene );  break;
			case eTranMoveInB		:	pScene2 = CCTransitionMoveInB       ::create ( fDuration, pScene );  break;
			case eTranSlideInL		:	pScene2 = CCTransitionSlideInL      ::create ( fDuration, pScene );  break;
			case eTranSlideInR		:	pScene2 = CCTransitionSlideInR      ::create ( fDuration, pScene );  break;
			case eTranSlideInT		:	pScene2 = CCTransitionSlideInT      ::create ( fDuration, pScene );  break;
			case eTranSlideInB		:	pScene2 = CCTransitionSlideInB      ::create ( fDuration, pScene );  break;
			case eTranShrinkGrow	:	pScene2 = CCTransitionShrinkGrow    ::create ( fDuration, pScene );  break;
 			case eTranProgressInOut	:	pScene2 = CCTransitionProgressInOut ::create ( fDuration, pScene );  break;
			case eTranProgressOutIn	:	pScene2 = CCTransitionProgressOutIn ::create ( fDuration, pScene );  break;                               
            case eTranPageForward   :   pScene2 = CCTransitionPageTurn      ::create ( fDuration, pScene, KD_FALSE );  break;
            case eTranPageBackward  :   pScene2 = CCTransitionPageTurn      ::create ( fDuration, pScene, KD_TRUE  );  break;
			default					:	pScene2 = pScene;
		}

		CCDirector::sharedDirector ( )->replaceScene ( pScene2 );
		CCTextureCache::sharedTextureCache ( )->removeUnusedTextures ( );
	}
}

CCLayer* CScrManager::getScreen ( KDvoid )
{
    return m_pScreen;
}

EScreen CScrManager::getScreenID ( KDvoid )
{
    return m_eScreen;
}