/*
 *
 *      File            ScrMain.cpp
 *      Description     Main scene.
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
#include "ScrMain.h"

#define INTERVAL_BG			0.2f

//
//  Main layer of "CScrMain" Scene.
//
KDbool CScrMain::init ( KDvoid )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_Sprite_Main )
	);
	{
		CC_ASSERT ( pBG );

		this->addChild ( pBG );
		
		pBG->setPosition ( ccpMid ( tWinSize ) );

		CCAnimation*  pAnimation = CCAnimation::create ( );

		for ( KDuint  i = 0; i < 10; i++ )
		{
			pAnimation->addSpriteFrameWithFileName ( g_pResManager->getPathSprite ( eIMG_Sprite_Main, i ) );
		}
		pAnimation->setDelayPerUnit ( INTERVAL_BG / 10 );
		pAnimation->setRestoreOriginalFrame ( KD_TRUE );

		pBG->runAction 
		(
			CCRepeatForever::create
			(
				CCAnimate::create ( pAnimation )
			)
		);
	}

	CCMenuItemImage*  pQuit = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Quit_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Quit_Sel ),
		this,
		menu_selector ( CScrMain::onQuit ) 
	);
	{
		CC_ASSERT ( pQuit );

		this->addChild ( CCMenu::createWithItem ( pQuit ) ); 

		const CCSize&  tSize = pQuit->getContentSize ( );

		pQuit->setPosition ( ccp ( tWinSize.cx - tSize.cx / 2.f - 5.f, tWinSize.cy - tSize.cy / 2.f - 5.f ) );
	} 

	CCMenuItemImage*  pConfig = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Config_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Config_Sel ),
		this,
		menu_selector ( CScrMain::onConfig ) 
	);
	{
		CC_ASSERT ( pConfig );

		this->addChild ( CCMenu::createWithItem ( pConfig ) ); 

		const CCSize&  tSize = pConfig->getContentSize ( );

		pConfig->setPosition ( ccp ( tSize.cx / 2.f + 10.f, tWinSize.cy - tSize.cy / 2.f - 10.f ) );
	}

	CCMenuItemImage*  pGame = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Game_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Game_Sel ),
		this,
		menu_selector ( CScrMain::onGame ) 
	);
	CC_ASSERT ( pGame );

	CCMenuItemImage*  pRank = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Rank_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Rank_Sel ),
		this,
		menu_selector ( CScrMain::onRank ) 
	);
	CC_ASSERT ( pRank );

	CCMenuItemImage*  pHelp = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Help_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Help_Sel ),
		this,
		menu_selector ( CScrMain::onHelp ) 
	);
	CC_ASSERT ( pHelp );

	CCMenu*  pMenu = CCMenu::create ( pGame, pRank, pHelp, KD_NULL );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pGame->getContentSize ( );

		pMenu->setPosition ( ccp ( tWinSize.cx / 2.f, tSize.cy / 2.f + 10.f ) );

		pMenu->alignItemsHorizontallyWithPadding ( 10.f );
	}

	this->setKeypadEnabled ( KD_TRUE );

	g_pResManager->playBGM ( eSND_BGM_Main );

	return KD_TRUE;
}

KDvoid CScrMain::onQuit ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	g_pScrManager->setDialog ( eDlgQuit );
}

KDvoid CScrMain::onConfig ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	g_pScrManager->setDialog ( eDlgConfig );
}

KDvoid CScrMain::onGame ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
    
	if ( g_pEnvManager->getStageMax ( ) == 0 )
	{
		g_pEnvManager->setStage ( KDUINT_MAX );
		g_pScrManager->setScene ( eScrCartoon, eTransSlideInR );
	}
	else
	{
		g_pScrManager->setScene ( eScrStage, eTransSlideInR );
	}
}

KDvoid CScrMain::onRank ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	g_pScrManager->setDialog ( eDlgRank );
}

KDvoid CScrMain::onHelp ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	g_pScrManager->setDialog ( eDlgHelp );
}