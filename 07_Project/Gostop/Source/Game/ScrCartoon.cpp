/*
 *
 *      File            ScrCartoon.cpp
 *      Description     Cartoon scene.
 *      Version         0.95.1007
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2011 XMSoft.
 *      Copyright (C) 2011 Blue River Ltd.
 *      Copyright (C) 2011 Blueplay Ltd. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *                     chris@blueplay.co.kr
 *                     	
 */ 

#include "Precompiled.h"
#include "ScrCartoon.h"

#define TAG_CARTOON					1000

//
//  Main layer of "CScrCartoon" Scene.
//
KDbool CScrCartoon::init ( KDvoid )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	KDuint  uStage    = g_pEnvManager->getStage ( );
	KDbool  bOpenning = g_pEnvManager->getEnemyCash ( ) > 0;
	
	switch ( uStage )
	{
		case 0 : m_uCount = bOpenning ? 3 : 2; break;
		case 1 : m_uCount = bOpenning ? 3 : 1; break;
		case 2 : m_uCount = bOpenning ? 4 : 1; break;
		case 3 : m_uCount = bOpenning ? 3 : 1; break;
		case 4 : m_uCount = bOpenning ? 3 : 1; break;
		case 5 : m_uCount = bOpenning ? 4 : 1; break;
		case 6 : m_uCount = bOpenning ? 3 : 1; break;
		case 7 : m_uCount = bOpenning ? 3 : 1; break;
		case 8 : m_uCount = bOpenning ? 5 : 3; break;

		case KDUINT_MAX : m_uCount = 3; bOpenning = KD_TRUE; break;
	}

	m_uIndex = 0;

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pCartoon = CCSprite::create
	(
		g_pResManager->getPathCartoon ( uStage, m_uIndex, bOpenning )
	);
	{
		CC_ASSERT ( pCartoon );

		this->addChild ( pCartoon, 0, TAG_CARTOON );
		
		pCartoon->setPosition ( ccpMid ( tWinSize ) );
	}

	this->setTouchEnabled ( KD_TRUE );

	g_pResManager->playBGM ( bOpenning ? eSND_BGM_Cartoon : eSND_BGM_Ending );
	g_pResManager->playSound ( eSND_Etc_Page );

	return KD_TRUE;
}

KDvoid CScrCartoon::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool  bOpenning = g_pEnvManager->getEnemyCash ( ) > 0;

	m_uIndex++;

	g_pResManager->playSound ( eSND_Etc_Page );

	if ( m_uIndex < m_uCount )
	{
		CCSprite*  pCartoon = (CCSprite *) this->getChildByTag ( TAG_CARTOON );

		pCartoon->initWithFile 
		(
			g_pResManager->getPathCartoon ( g_pEnvManager->getStage ( ), m_uIndex, bOpenning )
		);
	}
	else
	{
		if ( g_pEnvManager->getStage ( ) == KDUINT_MAX )
		{
			g_pEnvManager->setStageMax ( 1 );
			g_pEnvManager->save ( );

			g_pScrManager->setScene ( eScrStage, eTransSlideInR );
		}
		else 
		{
			if ( bOpenning )
			{
				g_pScrManager->setScene ( eScrVersus );
			}
			else
			{
				KDuint  uStageMax = g_pEnvManager->getStage ( ) + 2;
				if ( uStageMax > g_pEnvManager->getStageMax ( ) )
				{
					g_pEnvManager->setStageMax ( uStageMax );
				}

				g_pEnvManager->setEnemyCash ( g_pEnvManager->getEnemyCashMax ( ) );
				g_pEnvManager->save ( );

				g_pScrManager->setScene ( eScrStage, eTransSlideInR );
			}
		}
	}
}

