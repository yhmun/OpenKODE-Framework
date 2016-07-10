/*
 *
 *      File            ScrVersus.cpp
 *      Description     Versus scene.
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
#include "ScrVersus.h"

#define INTERVAL_BG				0.5f
#define INTERVAL_NEXT_SCENE		1.5f

//
//  Main layer of "CScrVersus" Scene.
//
KDbool CScrVersus::init ( KDvoid )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDchar  szText [ 64 ];

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_Sprite_Versus )
	);
	{
		CC_ASSERT ( pBG );

		this->addChild ( pBG );
		
		pBG->setPosition ( ccpMid ( tWinSize ) );

		CCAnimation*  pAnimation = CCAnimation::create ( );

		for ( KDuint i = 0; i < 10; i++ )
		{
			pAnimation->addSpriteFrameWithFileName ( g_pResManager->getPathSprite ( eIMG_Sprite_Versus, i ) );
		}
		pAnimation->setDelayPerUnit ( INTERVAL_BG / 10 );
		pAnimation->setRestoreOriginalFrame ( KD_TRUE );

		pBG->runAction 
		(
			CCSequence::create
			(
				CCAnimate::create ( pAnimation ),
				CCDelayTime::create ( INTERVAL_NEXT_SCENE ),
				CCCallFunc::create ( this, callfunc_selector ( CScrVersus::onNextScene ) ),
				KD_NULL
			)
		);
	}

	for ( KDuint  i = 0; i < 2; i++ )
	{
		CCSprite*  pCharacter = CCSprite::create
		(
            g_pResManager->getPathCharacter ( i == 0 ? 0 : g_pEnvManager->getStage ( ) + 1, 1 )
		);
		{
			CC_ASSERT ( pCharacter );

			this->addChild ( pCharacter );

			const CCSize&  tSize = pCharacter->getContentSize ( );

			pCharacter->ignoreAnchorPointForPosition ( KD_TRUE );
			pCharacter->setPosition ( ccp ( i == 0 ? 0 : tWinSize.cx - tSize.cx, 78.f ) );
		}

		if ( i == 0 )
		{
			kdSprintfKHR 
			(
				szText, "%s %s",
				g_pResManager->getText ( eTXT_Level_Name1, g_pEnvManager->getLevel ( ) ),
				g_pResManager->getText ( eTXT_Name_KBM )
			);
		}
		else
		{
			kdStrcpy ( szText, g_pResManager->getText ( eTXT_Enemy_Name1, g_pEnvManager->getStage ( ) ) );
		}

		CCLabelTTF*  pName = CCLabelTTF::create 
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			18
		);
		{
			CC_ASSERT ( pName );
	        
			this->addChild ( pName );
	        
			const CCSize&  tSize = pName->getContentSize ( );
	        
			pName->ignoreAnchorPointForPosition ( KD_TRUE );
			pName->setPosition ( ccp ( i == 0 ? 10 : tWinSize.cx - tSize.cx - 10, 38.f ) );
		} 

		KDchar  szCash [ 64 ];
		kdSprintfKHR
		(
			szCash,
			"%s %s%s",
			g_pResManager->getText ( eTXT_Sojigum ),
			g_pEnvManager->convCash ( i == 0 ? g_pEnvManager->getCash ( ) : g_pEnvManager->getEnemyCash ( ) ),
			g_pResManager->getText ( eTXT_Won )
		);

		CCLabelTTF*  pCash = CCLabelTTF::create 
		(
			szCash,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			18
		);
		{
			CC_ASSERT ( pCash );
	        
			this->addChild ( pCash );
	        
			const CCSize&  tSize = pCash->getContentSize ( );
	        
			pCash->ignoreAnchorPointForPosition ( KD_TRUE );
			pCash->setPosition ( ccp ( i == 0 ? 10 : tWinSize.cx - tSize.cx - 10, 7.f ) );
		} 
	}

	g_pResManager->playSound ( eSND_BGM_Versus );

	return KD_TRUE;
}

KDvoid CScrVersus::onNextScene ( KDvoid )
{
	CCDirector::sharedDirector ( )->purgeCachedData ( );

	g_pEnvManager->setPushCount ( 0 );
	g_pScrManager->setScene ( eScrGame, eTransSlideInR );
}