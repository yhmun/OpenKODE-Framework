/*
 *
 *      File            Main.cpp
 *      Description     Application Entrance
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
#include "platform.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( 800, 480, kResolutionExactFit );

	CResManager::create ( );
	CEnvManager::create ( );
	CNetManager::create ( );
	CScrManager::create ( );

	g_pScrManager->setScene ( eScrSplash );
    
	return KD_TRUE;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{
	CEnvManager::release ( );
	CResManager::release ( );
	CNetManager::release ( );
	CScrManager::release ( );
}

KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{
    
}

KDvoid AppDelegate::applicationDidEnterBackground  ( KDvoid )
{
    
}
