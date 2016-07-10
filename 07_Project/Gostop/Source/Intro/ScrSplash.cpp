/*
 *
 *      File            ScrSplash.cpp
 *      Description     Splash scene.
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
#include "ScrSplash.h"

//
//  Main layer of "CScrSplash" Scene.
//
KDbool CScrSplash::init ( KDvoid )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pBG = CCSprite::create 
	(
		g_pResManager->getPath ( eIMG_BG_Start )
	);
	do
	{
		CC_ASSERT ( pBG );

		this->addChild ( pBG );

		pBG->setPosition ( ccpMid ( tWinSize ) );

	} while ( 0 );

	this->runAction 
	(
		(CCActionInterval *) CCSequence::create 
		(
			CCDelayTime::create ( 1.f ),
			CCCallFunc::create ( this, callfunc_selector ( CScrSplash::onActive ) ),
			KD_NULL
		)
	);

	return KD_TRUE;
}

KDvoid CScrSplash::onActive ( KDvoid )
{
	if ( g_pEnvManager->getActivate ( ) )
	{
		g_pScrManager->setScene ( eScrIntro, eTransNull );
	}
	else
	{
		g_pScrManager->setDialog ( eDlgActivate );
	}
}
