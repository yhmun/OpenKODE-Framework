/*
 *
 *      File            ScrIntro.cpp
 *      Description     Intro scene.
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
#include "ScrIntro.h"

//
//  Main layer of "CScrIntro" Scene.
//
KDbool CScrIntro::init ( KDvoid )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_BG_Start2 )
	);
	{
		CC_ASSERT ( pBG );

		this->addChild ( pBG, 0, (KDuint) pBG );
		
		pBG->setPosition ( ccpMid ( tWinSize ) );
	}

	this->runAction 
	(
        (CCActionInterval *) CCSequence::create 
        (
            CCDelayTime::create ( 0.1f ),
            CCCallFunc::create ( this, callfunc_selector ( CScrIntro::onNextScene ) ),
            KD_NULL
        )
    );
    
	return KD_TRUE;
}

KDvoid CScrIntro::onNextScene ( KDvoid )
{
	CCSound*  pSound = CCSound::sharedSound ( );

	pSound->Open ( g_pResManager->getPath ( eSND_BGM_Play    , 0 ) );
	pSound->Open ( g_pResManager->getPath ( eSND_BGM_Versus  , 0 ) );
	pSound->Open ( g_pResManager->getPath ( eSND_Effect_Shuffle1 ) );
	pSound->Open ( g_pResManager->getPath ( eSND_Effect_Shuffle2 ) );
	pSound->Open ( g_pResManager->getPath ( eSND_Effect_Shuffle3 ) );

	g_pScrManager->setScene ( eScrMain, eTransMoveInR );
}
