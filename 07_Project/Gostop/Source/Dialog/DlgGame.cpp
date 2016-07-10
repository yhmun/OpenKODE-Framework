/*
 *
 *      File            DlgGame.cpp
 *      Description     Game dialogs.
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
#include "DlgBase.h"
#include "DlgGame.h"
#include "Game/ScrGame.h"

#define INTERVAL_TIMING			0.5f

//
//  "CDlgChongTong1" dialog layer
//
KDuint CDlgChongTong1::s_uMonth = KDUINT_MAX;
KDbool CDlgChongTong1::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );
	TScore*    pScore   = pScrGame->getScore ( );
	KDuint     uTurn    = pScrGame->getTurn ( );
	KDchar     szText [ 64 ];

	pScore->bChongTong = KD_TRUE;
	pScore->uScore = 10;

	kdSprintfKHR 
	(
		szText, "[ %s %s ]",
		g_pResManager->getText ( eTXT_ChongTong ),
		g_pResManager->getText ( uTurn == 0 ? eTXT_Win2 : eTXT_Lose2 )
	);

	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		const CCSize&  tSize = pTitle->getContentSize ( );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy - tSize.cy / 2.f - 25.f ) );
	}

    for ( KDuint  i = 0; i < 4; i++ )
    {
        CCSprite*  pCard = CCSprite::create
        (
            g_pResManager->getPathSprite ( eIMG_Sprite_Card, s_uMonth * 4 + i )
        ); 
        {
            CC_ASSERT ( pCard );
            
            const CCSize&  tSize = pCard->getContentSize ( );
            
			this->addChild ( pCard );
            
			pCard->ignoreAnchorPointForPosition ( KD_TRUE );
			pCard->setPosition ( ccp ( ( tLyrSize.cx - ( tSize.cx + 5.f ) * 4 ) / 2.f + ( tSize.cx + 5.f ) * i, ( tLyrSize.cy - tSize.cy ) / 2.f + 5.f ) );
        }
    }

	kdSprintfKHR 
	(
		szText, "%d%s %s!!",
		pScore->uScore,
		g_pResManager->getText ( eTXT_Point ),
		g_pResManager->getText ( uTurn == 0 ? eTXT_Win2 : eTXT_Lose2 )
	);

	CCLabelTTF*  pTxtScore = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pTxtScore );

		this->addChild ( pTxtScore );

		const CCSize&  tSize = pTxtScore->getContentSize ( );

		pTxtScore->setPosition ( ccp ( tLyrSize.cx / 2.f, tSize.cy / 2.f + 40.f ) );
	}

    return KD_TRUE;
}

KDvoid CDlgChongTong1::onRemove ( KDvoid )
{
	CDlgGame::onRemove ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->postEvent ( eType_Sound, pScrGame->getTurn ( ) == 0 ? eSND_Attack_ChongTong : eSND_Harm_ChongTong );
	pScrGame->postEvent ( eType_Talk, eTALK_ChongTong );
	pScrGame->postEvent ( eType_Result );
	pScrGame->waitEvent ( );
}

//
//  "CDlgChongTong2" dialog layer
//
KDuint CDlgChongTong2::s_uMonth = KDUINT_MAX;
KDbool CDlgChongTong2::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );
	KDchar  szText [ 64 ];
    
	kdSprintfKHR ( szText, "[ %s ]", g_pResManager->getText ( eTXT_ChongTong ) );
	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		const CCSize&  tSize = pTitle->getContentSize ( );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy - tSize.cy / 2.f - 15.f ) );
	}

    for ( KDuint  i = 0; i < 4; i++ )
    {
        CCSprite*  pCard = CCSprite::create
        (
            g_pResManager->getPathSprite ( eIMG_Sprite_Card, s_uMonth * 4 + i )
        ); 
        {
            CC_ASSERT ( pCard );
            
            const CCSize&  tSize = pCard->getContentSize ( );
            
			this->addChild ( pCard );
            
			pCard->ignoreAnchorPointForPosition ( KD_TRUE );
			pCard->setPosition ( ccp ( ( tLyrSize.cx - ( tSize.cx + 5.f ) * 4 ) / 2.f + ( tSize.cx + 5.f ) * i, ( tLyrSize.cy - tSize.cy ) / 2.f + 25.f) );
        }
    }

	CCMenuItemImage*  pGo = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_ChongTong_Go_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_ChongTong_Go_Sel ),
		this,
		menu_selector ( CDlgChongTong2::onGo ) 
	);
	CC_ASSERT ( pGo );

	CCMenuItemImage*  pStop = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_ChongTong_Stop_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_ChongTong_Stop_Sel ),
		this,
		menu_selector ( CDlgChongTong2::onStop ) 
	);
	CC_ASSERT ( pStop );

	CCMenu*  pMenu = CCMenu::create ( pGo, pStop, KD_NULL );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pGo->getContentSize ( );
		
		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f, tSize.cy / 2.f + 30.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsHorizontallyWithPadding ( 10.f );
	}

    return KD_TRUE;
}

EAction CDlgChongTong2::getShowAction ( KDvoid )
{
	return eActionNull;
}

KDvoid CDlgChongTong2::onGo ( CCObject* pSender )
{
	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );

	pScrGame->keepChongTong ( s_uMonth );
}

KDvoid CDlgChongTong2::onStop ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	this->hide ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );
	TScore*    pScore   = KD_NULL;

	pScrGame->setTurn ( 0 );
	pScore = pScrGame->getScore ( );
	pScore->bChongTong = KD_TRUE;
	pScore->uScore = 10;

	pScrGame->postEvent ( eType_Sound, eSND_Attack_ChongTong );
	pScrGame->postEvent ( eType_Talk, eTALK_ChongTong );
	pScrGame->postEvent ( eType_Result );
	pScrGame->waitEvent ( );
}

//
//  "CDlgShake" dialog layer
//
CCardList* CDlgShake::s_pList = KD_NULL;
CCard*     CDlgShake::s_pCard = KD_NULL;
KDbool CDlgShake::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }

    const CCSize&  tLyrSize = this->getContentSize ( );
	KDuint  uMonth = s_pCard->getMonth ( );
	KDchar  szText [ 64 ];

	kdSprintfKHR ( szText, "[ %s ]", g_pResManager->getText ( eTXT_Shake ) );
	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		const CCSize&  tSize = pTitle->getContentSize ( );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy - tSize.cy / 2.f - 25.f ) );
	}

	KDuint  uIndex = 0;
    for ( KDuint  i = 0; i < 4; i++ )
    {
		if ( s_pList->at ( uMonth * 4 + i )->getWhere ( ) == s_pCard->getWhere ( ) )
		{
			CCSprite*  pCard = CCSprite::create
			(
				g_pResManager->getPathSprite ( eIMG_Sprite_Card, uMonth * 4 + i )
			); 
			{
				CC_ASSERT ( pCard );
	            
				const CCSize&  tSize = pCard->getContentSize ( );
	            
				this->addChild ( pCard );
	            
				pCard->ignoreAnchorPointForPosition ( KD_FALSE );
				pCard->setPosition ( ccp ( ( tLyrSize.cx - ( tSize.cx + 5.f ) * 3 ) / 2.f + ( tSize.cx + 5.f ) * uIndex, ( tLyrSize.cy - tSize.cy ) / 2.f + 5.f ) );

				uIndex++;
			}
		}
    }

	return KD_TRUE;
}

KDvoid CDlgShake::onRemove ( KDvoid )
{
	CDlgGame::onRemove ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->postEvent ( eType_Sound, pScrGame->getTurn ( ) == 0 ? eSND_Attack_Shake : eSND_Harm_Shake );
	pScrGame->postEvent ( eType_CardToGround, s_pCard->getNumber ( ) );

	pScrGame->waitEvent ( );
}

//
//  "CDlgBuck" dialog layer
//
KDuint CDlgBuck::s_uTurn  = 0;
KDuint CDlgBuck::s_uCount = 0;
KDbool CDlgBuck::s_bCombo = KD_FALSE;

KDbool CDlgBuck::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );

	KDuint  uTimes        = (KDuint) kdPowf ( 2, (KDfloat) g_pEnvManager->getPushCount ( ) );
	KDuint  uCashes [ 2 ] = { g_pEnvManager->getCash ( ), g_pEnvManager->getEnemyCash ( ) };
	KDuint  uBuckCash     = g_pEnvManager->getBet ( ) * 7 * uTimes * ( s_bCombo ? s_uCount : 1 );
	KDuint  uObtainCash   = 0;

	KDchar  szText [ 64 ];

	m_bBankrupted = uCashes [ !s_uTurn ] <= uBuckCash;
	uObtainCash = m_bBankrupted ? uCashes [ !s_uTurn ] : uBuckCash;

	if ( s_bCombo )
	{
		if ( s_uTurn == 0 )
		{
			g_pEnvManager->setCash ( uCashes [ 0 ] + uObtainCash );
			g_pEnvManager->setEnemyCash ( uCashes [ 1 ] - uObtainCash );
		}
		else
		{
			g_pEnvManager->setCash ( uCashes [ 0 ] - uObtainCash );
			g_pEnvManager->setEnemyCash ( uCashes [ 1 ] + uObtainCash );
		}
	}

	g_pEnvManager->save ( );

	if ( s_uCount == 1 )
	{
		kdSprintfKHR ( szText, "%s %s!", g_pResManager->getText ( eTXT_First ), g_pResManager->getText ( eTXT_Buck ) );
	}
	else if ( s_bCombo )
	{
		kdSprintfKHR ( szText, "%d%s%s!", s_uCount, g_pResManager->getText ( eTXT_Combo ), g_pResManager->getText ( eTXT_Buck ) );
	}
	else
	{
		kdSprintfKHR ( szText, "%d%s!", s_uCount, g_pResManager->getText ( eTXT_Buck ) );
	}

	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		44
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy / 2.f + 50.f ) );
	}

	if ( s_bCombo )
	{
		CCLabelTTF*  pObtain = CCLabelTTF::create 
		(
			g_pEnvManager->convCash ( uObtainCash ),
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			18
		);
		{
			CC_ASSERT ( pObtain );

			this->addChild ( pObtain );

			const CCSize&  tSize = pObtain->getContentSize ( );

			pObtain->setColor ( ccYELLOW );
			pObtain->setAnchorPoint ( ccp ( 0, 0.5f ) );
			pObtain->setPosition ( ccp ( tLyrSize.cx / 2.f - tSize.cx, tLyrSize.cy / 2.f - 25.f ) );

			kdSprintfKHR 
			(
				szText, "%s %s!", 
				g_pResManager->getText ( eTXT_Won ),
				g_pResManager->getText ( s_uTurn == 0 ? eTXT_Obtain : eTXT_Losing )
			);
			CCLabelTTF*  pObtain2 = CCLabelTTF::create 
			(
				szText,
				g_pResManager->getPath ( eFNT_Gothic_Bold ),
				18
			);
			{
				CC_ASSERT ( pObtain2 );

				pObtain->addChild ( pObtain2 );

				pObtain2->ignoreAnchorPointForPosition ( KD_TRUE );
				pObtain2->setPosition ( ccp ( tSize.cx - 7.f, 0 ) );
			}
		}
	}

	if ( s_uCount == 3 )
	{
		kdSprintfKHR 
		(
			szText, "%d%s %s!", 
			7 * uTimes,
			g_pResManager->getText ( eTXT_Point ),
			g_pResManager->getText ( s_uTurn == 0 ? eTXT_Win2 : eTXT_Lose2 )
		);

		CCLabelTTF*  pScore = CCLabelTTF::create 
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			18
		);
		{
			CC_ASSERT ( pScore );

			this->addChild ( pScore );

			pScore->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy / 2.f - 65.f ) );
		}
	}

    return KD_TRUE;
}

KDvoid CDlgBuck::onRemove ( KDvoid )
{
	CDlgGame::onRemove ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );
	EPath      ePath;

	if ( m_bBankrupted )
	{
		TScore*  pScore = pScrGame->getScore ( );

		kdMemset ( pScore, 0, sizeof ( TScore ) );

		pScrGame->postEvent ( eType_Result );
	}
	else 
	{
		if ( s_bCombo )
		{
			switch ( s_uCount )
			{
				case 1 : ePath = s_uTurn == 0 ? eSND_Attack_Buck1 : eSND_Harm_Buck1; break;
				case 2 : ePath = s_uTurn == 0 ? eSND_Attack_Buck2 : eSND_Harm_Buck2; break;
				case 3 : ePath = s_uTurn == 0 ? eSND_Attack_Buck3 : eSND_Harm_Buck3; break;
			}
		}
		else
		{
			ePath = s_uTurn == 0 ? eSND_Attack_Buck : eSND_Harm_Buck;
		}

		pScrGame->postEvent ( eType_Sound, ePath );
		pScrGame->postEvent ( eType_Talk, eTALK_Buck );
	
		if ( s_uCount == 3 )
		{
			pScrGame->postEvent ( eType_Result );
		}
		else
		{
			pScrGame->postEvent ( eType_RefreshCash );
			pScrGame->postEvent ( eType_Obtain );
		}	
	}

	pScrGame->waitEvent ( );
}

//
//  "CDlgNagari" dialog layer
//
KDbool CDlgNagari::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );
    KDchar  szText [ 64 ];

	kdSprintfKHR ( szText, "%s!", g_pResManager->getText ( eTXT_Nagari ) );
	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		44
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy / 2.f + 50.f ) );
	}

	CCLabelTTF*  pDesc = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Nagari_Desc ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pDesc );

		this->addChild ( pDesc );

		const CCSize&  tSize = pDesc->getContentSize ( );

		pDesc->setPosition ( ccp ( tLyrSize.cx / 2.f - 40.f, tLyrSize.cy / 2.f - 25.f ) );

		kdSprintfKHR ( szText, "2%s!!!", g_pResManager->getText ( eTXT_Times ) );
		CCLabelTTF*  pDesc2 = CCLabelTTF::create 
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			18
		);
		{
			CC_ASSERT ( pDesc2 );

			pDesc->addChild ( pDesc2 );

			pDesc2->setColor ( ccYELLOW );
			pDesc2->setAnchorPoint ( ccp ( 0, 0.5f ) );
			pDesc2->setPosition ( ccp ( tSize.cx, tSize.cy / 2.f ) );
		}
	}

    return KD_TRUE;
}

KDvoid CDlgNagari::onRemove ( KDvoid )
{
	g_pScrManager->setScene ( eScrGame, eTransSlideInR );
}

//
//  "CDlgGukJin" dialog layer
//
CCard* CDlgGukJin::s_pCard = KD_NULL;
KDbool CDlgGukJin::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );
	KDchar  szText [ 64 ];
    
	kdSprintfKHR 
	(
		szText, "[ %s %s ]", 
		g_pResManager->getText ( eTXT_GukJin ),
		g_pResManager->getText ( eTXT_Select )
	);

	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		const CCSize&  tSize = pTitle->getContentSize ( );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy - tSize.cy / 2.f - 15.f ) );
	}

    CCSprite*  pCard = CCSprite::create
    (
        g_pResManager->getPathSprite ( eIMG_Sprite_Card, 32 )
    ); 
    {
        CC_ASSERT ( pCard );
        
        const CCSize&  tSize = pCard->getContentSize ( );
        
		this->addChild ( pCard );
        
		pCard->ignoreAnchorPointForPosition ( KD_TRUE );
		pCard->setPosition ( ccp ( ( tLyrSize.cx - tSize.cx ) / 2.f, ( tLyrSize.cy - tSize.cy ) / 2.f + 25.f) );
    }

	CCMenuItemImage*  pPoint10 = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Gukjin_10_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Gukjin_10_Sel ),
		this,
		menu_selector ( CDlgGukJin::onPoint10 ) 
	);
	CC_ASSERT ( pPoint10 );

	CCMenuItemImage*  pPoint2 = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Gukjin_2_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Gukjin_2_Sel ),
		this,
		menu_selector ( CDlgGukJin::onPoint2 ) 
	);
	CC_ASSERT ( pPoint2 );

	CCMenu*  pMenu = CCMenu::create ( pPoint10, pPoint2, KD_NULL );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pPoint10->getContentSize ( );
		
		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f, tSize.cy / 2.f + 30.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsHorizontallyWithPadding ( 10.f );
	}

    return KD_TRUE;
}

EAction CDlgGukJin::getShowAction ( KDvoid )
{
	return eActionNull;
}

KDvoid CDlgGukJin::onPoint10 ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	this->hide ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->waitEvent ( );
}

KDvoid CDlgGukJin::onPoint2 ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	this->hide ( );

	s_pCard->setType ( eCard_Type_2Pi );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->waitEvent ( );
}

//
//  "CDlgGostop" dialog layer
//
KDbool CDlgGostop::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );

	CScrGame*	pScrGame = (CScrGame *) g_pScrManager->getScene ( );
	TScore*		pScore   = pScrGame->getTotal ( );
	KDuint		uWinCash = pScore->uTotalScore * g_pEnvManager->getBet ( );

	KDchar		szText [ 64 ];
    
	kdSprintfKHR ( szText, "%d%s", pScore->uGo + 1, g_pResManager->getText ( eTXT_Go ) );
	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		50
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		const CCSize&  tSize = pTitle->getContentSize ( );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f - 117.f, tLyrSize.cy / 2.f + 60.f ) );

		CCLabelTTF*  pTitle2 = CCLabelTTF::create 
		(
			g_pResManager->getText ( eTXT_Request ),
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			28
		);
		{
			CC_ASSERT ( pTitle2 );

			pTitle->addChild ( pTitle2 );

			pTitle2->ignoreAnchorPointForPosition ( KD_TRUE );
			pTitle2->setPosition ( ccp ( tSize.cx, 15.f ) );
		}
	}

	CCLabelTTF*  pObtain = CCLabelTTF::create 
	(
		g_pEnvManager->convCash ( uWinCash ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pObtain );

		this->addChild ( pObtain );

		const CCSize&  tSize = pObtain->getContentSize ( );

		pObtain->setColor ( ccYELLOW );
		pObtain->setAnchorPoint ( ccp ( 0, 0.5f ) );
		pObtain->setPosition ( ccp ( tLyrSize.cx / 2.f - tSize.cx - 45.f, tLyrSize.cy / 2.f + 5.f ) );

		kdSprintfKHR 
		(
			szText, "%s %s %s", 
			g_pResManager->getText ( eTXT_Won ),
			g_pResManager->getText ( eTXT_Obtain ),
			g_pResManager->getText ( eTXT_Able )
		);
		CCLabelTTF*  pObtain2 = CCLabelTTF::create 
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			18
		);
		{
			CC_ASSERT ( pObtain2 );

			pObtain->addChild ( pObtain2 );

			pObtain2->ignoreAnchorPointForPosition ( KD_TRUE );
			pObtain2->setPosition ( ccp ( tSize.cx - 7.f, 0 ) );
		}
	}

	CCMenuItemImage*  pGo = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Go_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Go_Sel ),
		this,
		menu_selector ( CDlgGostop::onGo ) 
	);
	CC_ASSERT ( pGo );

	CCMenuItemImage*  pStop = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Stop_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Stop_Sel ),
		this,
		menu_selector ( CDlgGostop::onStop ) 
	);
	CC_ASSERT ( pStop );

	CCMenu*  pMenu = CCMenu::create ( pGo, pStop, KD_NULL );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pGo->getContentSize ( );
		
		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f, tSize.cy / 2.f + 30.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsHorizontallyWithPadding ( 10.f );
	}

    return KD_TRUE;
}

EAction CDlgGostop::getShowAction ( KDvoid )
{
	return eActionNull;
}

KDvoid CDlgGostop::onGo ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	this->hide ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->setGostop ( KD_TRUE );
	pScrGame->waitEvent ( );
}

KDvoid CDlgGostop::onStop ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	this->hide ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->setGostop ( KD_FALSE );
	pScrGame->waitEvent ( );
}

//
//  "CDlgCard" dialog layer
//

CCardList* CDlgCard::s_pList  = KD_NULL;
CCard*	   CDlgCard::s_pCard1 = KD_NULL;
CCard*     CDlgCard::s_pCard2 = KD_NULL;

KDbool CDlgCard::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );
	KDchar  szText [ 64 ];
    
	kdSprintfKHR 
	(
		szText, "[ %s %s ]", 
		g_pResManager->getText ( eTXT_Card ),
		g_pResManager->getText ( eTXT_Select )
	);

	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		const CCSize&  tSize = pTitle->getContentSize ( );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy - tSize.cy / 2.f - 15.f ) );
	}

	CCMenuItemImage*  pCard1 = CCMenuItemImage::create 
	(
		g_pResManager->getPathSprite ( eIMG_Sprite_Card, s_pCard1->getNumber ( ) ), 
		KD_NULL,
		this,
		menu_selector ( CDlgCard::onSelect ) 
	);
	CC_ASSERT ( pCard1 );
	pCard1->setTag ( 0 );

	CCMenuItemImage*  pCard2 = CCMenuItemImage::create 
	(
		g_pResManager->getPathSprite ( eIMG_Sprite_Card, s_pCard2->getNumber ( ) ), 
		KD_NULL,
		this,
		menu_selector ( CDlgCard::onSelect ) 
	);
	CC_ASSERT ( pCard2 );
	pCard2->setTag ( 1 );

	CCMenu*  pMenu = CCMenu::create ( pCard1, pCard2, KD_NULL );
	{
		CC_ASSERT ( pMenu );
		
		pMenu->setPosition ( ccpMid ( tLyrSize ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsHorizontallyWithPadding ( 10.f );

		this->addChild ( pMenu );
	}

    return KD_TRUE;
}

EAction CDlgCard::getShowAction ( KDvoid )
{
	return eActionNull;
}

KDvoid CDlgCard::onSelect ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	this->hide ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );
	CCNode*    pNode = (CCNode *) pSender;

	s_pList->push_back ( pNode->getTag ( ) == 0 ? s_pCard1 : s_pCard2 );
	pScrGame->waitEvent ( );
}

//
//  "CDlgTimes" dialog layer
//
KDbool CDlgTimes::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );

	KDuint  uTimes = (KDuint) kdPowf ( 2.f, (KDfloat) g_pEnvManager->getPushCount ( ) );
    KDchar  szText [ 64 ];

	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_This_Game ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		44
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		const CCSize&  tSize = pTitle->getContentSize ( );

		pTitle->setPosition ( ccp ( ( tLyrSize.cx - tSize.cx ) / 2.f, tLyrSize.cy / 2.f ) );

		kdSprintfKHR ( szText, "%d%s!!", uTimes, g_pResManager->getText ( eTXT_Times ) );
		CCLabelTTF*  pTimes = CCLabelTTF::create 
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			44
		);
		{
			CC_ASSERT ( pTimes );

			pTitle->addChild ( pTimes );

			pTimes->setColor ( ccYELLOW );
			pTimes->ignoreAnchorPointForPosition ( KD_TRUE );
			pTimes->setPosition ( ccp ( tSize.cx, 0 ) );
		}
	}

    return KD_TRUE;
}

KDvoid CDlgTimes::onRemove ( KDvoid )
{
	CDlgGame::onRemove ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->postEvent ( eType_Shuffle );
	pScrGame->waitEvent ( );
}

//
//  "CDlgPlayerPush" dialog layer
//
KDbool CDlgPushPlayer::init ( KDvoid )
{
    if ( CDlgRequest::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }

    const CCSize&  tLyrSize = this->getContentSize ( );   
	KDchar  szText [ 64 ];
    
	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Push_Player ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pRequest );

		this->addChild ( pRequest );

		const CCSize&  tSize = pRequest->getContentSize ( );

        pRequest->setColor ( ccYELLOW );
		pRequest->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, ( tLyrSize.cy + tSize.cy ) / 2.f + 47.f ) );
	}

	CCLabelTTF*  pDesc = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Push_Desc ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		16
	);
	{
		CC_ASSERT ( pDesc );

		this->addChild ( pDesc );

		pDesc->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy / 2.f + 15.f ) );
	}

   	kdSprintfKHR
	(
        szText, "( %s %d%s, %s %d%s )",
        g_pResManager->getText ( eTXT_Current ), 
        (KDuint) kdPowf ( 2.f, (KDfloat) g_pEnvManager->getPushCount ( ) ),
        g_pResManager->getText ( eTXT_Times ),
        g_pResManager->getText ( eTXT_Max ), 
        16,
        g_pResManager->getText ( eTXT_Times )
    );
    
   	CCLabelTTF*  pCount = CCLabelTTF::create 
	(
        szText,
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        16
    );
	{
		CC_ASSERT ( pCount );
        
		this->addChild ( pCount );
        
		pCount->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy / 2.f - 25.f ) );
	}
    
	return KD_TRUE;
}

const KDchar* CDlgPushPlayer::getTitle ( KDvoid )
{
    return g_pResManager->getText ( eTXT_Title_Push );
}

KDvoid CDlgPushPlayer::onYes ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );

	g_pEnvManager->rollback ( );
    g_pEnvManager->setFirstTurn ( 0 );
	g_pEnvManager->setPushCount ( g_pEnvManager->getPushCount ( ) + 1 );
	g_pEnvManager->save ( );
	
	g_pScrManager->setScene ( eScrGame, eTransSlideInR );
}

KDvoid CDlgPushPlayer::onNo ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );
}

//
//  "CDlgPushEnemy" dialog layer
//
KDbool CDlgPushEnemy::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );

	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Push_Enemy ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		32
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		pTitle->setPosition ( ccpMid ( tLyrSize ) );
		pTitle->setColor ( ccYELLOW );
	}

    return KD_TRUE;
}

KDvoid CDlgPushEnemy::onRemove ( KDvoid )
{
	CDlgGame::onRemove ( );

	KDuint  uCount = g_pEnvManager->getPushCount ( );

    g_pEnvManager->setFirstTurn ( 1 );
	g_pEnvManager->setPushCount ( uCount + 1 );

	g_pScrManager->setScene ( eScrGame, eTransSlideInR );
}

//
//  "CDlgMission" dialog layer
//
KDuint CDlgMission::s_uIndex = 0;

KDbool CDlgMission::init ( KDvoid )
{
    if ( CDlgGame::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );
	const KDuint*  pMission = g_pEnvManager->getMission ( s_uIndex );
	KDuint  uCount = pMission [ 4 ] == KDUINT_MAX ? 3 : 4;
	KDuint  uTimes = pMission [ 0 ];
    KDchar  szText [ 64 ];

	kdSprintfKHR ( szText, "[ %s ]", g_pResManager->getText ( eTXT_Mission ) );
	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pTitle );

		this->addChild ( pTitle );

		const CCSize&  tSize = pTitle->getContentSize ( );

		pTitle->setColor ( ccYELLOW );
		pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy - tSize.cy / 2.f - 25.f ) );
	}

	for ( KDuint  i = 0; i < uCount; i++ )
    {
        CCSprite*  pCard = CCSprite::create
        (
            g_pResManager->getPathSprite ( eIMG_Sprite_Card, pMission [ i + 1 ] )
        ); 
        {
            CC_ASSERT ( pCard );
            
            const CCSize&  tSize = pCard->getContentSize ( );
            
			this->addChild ( pCard );
            
			pCard->ignoreAnchorPointForPosition ( KD_TRUE );
			pCard->setPosition ( ccp ( ( tLyrSize.cx - ( tSize.cx + 5.f ) * uCount ) / 2.f + ( tSize.cx + 5.f ) * i, ( tLyrSize.cy - tSize.cy ) / 2.f + 5.f ) );

			if ( i == ( uCount - 1 ) )
			{
				kdSprintfKHR ( szText, "x%d", uTimes );
				CCLabelTTF*  pTimes = CCLabelTTF::create 
				(
					szText,
					g_pResManager->getPath ( eFNT_Gothic_Bold ),
					38
				);
				{
					CC_ASSERT ( pTimes );

					pCard->addChild ( pTimes );

					pTimes->setColor ( ccYELLOW );
					pTimes->ignoreAnchorPointForPosition ( KD_TRUE );
					pTimes->setPosition ( ccp ( tSize.cx + 5.f, 0 ) );
				}
			}
        }
    }

    return KD_TRUE;
}

KDvoid CDlgMission::onRemove ( KDvoid )
{
	CDlgGame::onRemove ( );

	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	pScrGame->postEvent ( eType_PutPlayer );
	pScrGame->waitEvent ( );
}

//
//  "CDlgGage" dialog layer
//
CCard*  CDlgGage::s_pCard   = KD_NULL;
KDfloat CDlgGage::s_fTiming = 1.f;
KDbool CDlgGage::init ( KDvoid )
{
	if ( CDlgGame2::init ( ) == KD_FALSE )
	{
		return KD_TRUE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDfloat        fTiming = INTERVAL_TIMING * s_fTiming;

	CCSprite*  pBar = CCSprite::create 
	(
		g_pResManager->getPath ( eIMG_ETC_Timing_Bar )
	);
	{
		CC_ASSERT ( pBar );

		this->addChild ( pBar, 0, (KDuint) pBar );

		const CCSize&  tBarSize = pBar->getContentSize ( );

		pBar->setPosition ( ccp ( tWinSize.cx / 2.f, tWinSize.cy / 2.f + 50.f ) );

		m_pBar = (CCSprite *) this->getChildByTag ( (KDuint) pBar );

		CCSprite*  pGage = CCSprite::create 
		(
			g_pResManager->getPath ( eIMG_ETC_Timing_Gage )
		);
		{
			CC_ASSERT ( pGage );

			pBar->addChild ( pGage, 0, (KDuint) pGage );

			pGage->setPosition ( ccp ( 0, tBarSize.cy / 2.f + 3.f ) );
			pGage->runAction
			(
				CCRepeatForever::create
				(
					(CCActionInterval *) CCSequence::create
					(
						CCMoveTo::create ( fTiming, ccp (           0, tBarSize.cy / 2.f + 3.f ) ),
						CCMoveTo::create ( fTiming, ccp ( tBarSize.cx, tBarSize.cy / 2.f + 3.f ) ),
						KD_NULL
					)
				)
			);

			m_pGage = (CCSprite *) pBar->getChildByTag ( (KDuint) pGage );
		}
	}

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CDlgGage::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, g_pScrManager->getTopDialogPrioty ( ), KD_TRUE );
}

KDbool CDlgGage::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	const CCSize&   tSize1 = m_pBar ->getContentSize ( );
	const CCSize&   tSize2 = m_pGage->getContentSize ( );
	const CCPoint&  tPosition = m_pGage->getPosition ( );

	KDuint  uFlag  = s_pCard->getFlag ( ) & 0x0fff;
	KDfloat  fDelay = 0.3f;

	m_pGage->stopAllActions ( );
	
	if ( ( ( tSize1.cx - tSize2.cx ) / 2.f < tPosition.x ) && ( ( tSize1.cx + tSize2.cx ) / 2.f > tPosition.x ) )
	{
		s_pCard->setFlag ( uFlag | eCard_Flag_Gage );

		g_pResManager->playSound ( kdRand ( ) % 2 ? eSND_Attack_Gage_Success1 : eSND_Attack_Gage_Success2 );
	}
	else
	{
		g_pResManager->playSound ( kdRand ( ) % 2 ? eSND_Attack_Gage_Fail1 : eSND_Attack_Gage_Fail2 );
	}

	this->runAction 
	(
		(CCActionInterval *) CCSequence::create
		(
			CCDelayTime::create ( fDelay ),
			CCCallFunc ::create ( this, callfunc_selector ( CDlgGage::onRemove ) ),
			KD_NULL
		)
	);
	
	return KD_TRUE;
}

KDvoid CDlgGage::onRemove ( KDvoid )
{
	CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	CDialog::onRemove ( );

	pScrGame->putCard ( s_pCard );
}

KDbool CDlgGage::hasKeyHide ( KDvoid )
{
	return KD_FALSE;
}

//
//  "CDlgSkill" dialog layer
//
KDuint CDlgSkill::s_uIndex = KDUINT_MAX;
KDbool CDlgSkill::init ( KDvoid )
{
	m_bConfirm = KD_FALSE;

    if ( CDlgRequest::init ( ) == KD_FALSE )
    {
        return KD_FALSE;
    }
    
    const CCSize&  tLyrSize = this->getContentSize ( );    
	KDchar  szText [ 64 ];

	CCSprite*  pSkill = CCSprite::create 
	(
		g_pResManager->getPathSprite ( eIMG_Sprite_Skill, s_uIndex )
	);
	{
		CC_ASSERT ( pSkill );

		this->addChild ( pSkill );

		const CCSize&  tSize = pSkill->getContentSize ( );

		pSkill->setPosition ( ccp ( ( tLyrSize.cx - tSize.cx ) / 2.f - 110.f, ( tLyrSize.cy + tSize.cy ) / 2.f + 30.f ) );
	}

	kdSprintfKHR
	(
		szText, "%s %s",
		g_pResManager->getText ( eTXT_Use ), 
		g_pResManager->getText ( eTXT_Request )
	);
	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pRequest );

		this->addChild ( pRequest );

		const CCSize&  tSize = pRequest->getContentSize ( );

		pRequest->setPosition ( ccp ( tLyrSize.cx / 2.f + 20.f, ( tLyrSize.cy + tSize.cy ) / 2.f + 42.f ) );
	}

	CCLabelTTF*  pDesc = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Skill_Desc1, s_uIndex ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		16
	);
	{
		CC_ASSERT ( pDesc );

		this->addChild ( pDesc );

		pDesc->setColor ( ccYELLOW );
		pDesc->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f - 10.f ) );
	}

    return KD_TRUE;
}

const KDchar* CDlgSkill::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Use_Skill );
}

KDvoid CDlgSkill::onRemove ( KDvoid )
{
	CDlgRequest::onRemove ( );

	if ( m_bConfirm )
	{
		( (CDialog *) g_pScrManager->getTopDialog ( ) )->hide ( );
		( (CScrGame *) g_pScrManager->getScene ( ) )->useSkill ( s_uIndex );
	}
}

KDvoid CDlgSkill::onYes ( CCObject* pSender )
{
	m_bConfirm = KD_TRUE;

	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );	
}

KDvoid CDlgSkill::onNo ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	this->hide ( );
}

//
//  "CDlgUseSkill" dialog layer
//
KDbool CDlgUseSkill::init ( KDvoid )
{
	if ( CDlgGame2::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	CScrGame*	pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	KDchar  szText [ 64 ];
	CCSize  tLyrSize;

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_BTN_Game_Skill2 )
	);
	{
		CC_ASSERT ( pBG );

		tLyrSize = pBG->getContentSize ( );

		this->setContentSize ( tLyrSize );
		this->addChild ( pBG );

		pBG->ignoreAnchorPointForPosition ( KD_TRUE );
	}

	CCMenu*  pMenus = KD_NULL;
	for ( KDuint  i = 0; i < COUNT_SKILL; i++ )
	{
		CCMenuItemImage*  pMenu = CCMenuItemImage::create 
		(
			g_pResManager->getPathSprite ( eIMG_Sprite_Skill, i ), 
			KD_NULL,
			this,
			menu_selector ( CDlgUseSkill::onMenu ) 
		);
		{
			KDuint  uSkill = g_pEnvManager->getSkill ( i );

			CC_ASSERT ( pMenu );

			const CCSize&  tMenuSize = pMenu->getContentSize ( );

			pMenu->setTag ( i );
			pMenu->ignoreAnchorPointForPosition ( KD_TRUE );
			pMenu->setPosition ( ccp ( ( tMenuSize.cx + 10.f ) * i + 13.f, 27.f ) );

			if ( uSkill > 0 )
			{
				CCSprite*  pBox = CCSprite::create
				(
					g_pResManager->getPath ( eIMG_ETC_Number_Box )
				);
				{
					CC_ASSERT ( pBox );

					const CCSize&  tSize = pBox->getContentSize ( );

					pMenu->addChild ( pBox );

					pBox->ignoreAnchorPointForPosition ( KD_TRUE );
					pBox->setPosition ( ccp ( 1, tMenuSize.cy - tSize.cy - 1 ) );

					kdSprintfKHR ( szText, "%02d", uSkill );
					CCLabelTTF*  pNumber = CCLabelTTF::create 
					(
						szText,
						g_pResManager->getPath ( eFNT_Gothic_Bold ),
						9
					);
					{
						CC_ASSERT ( pNumber );

						pBox->addChild ( pNumber );
						
						pNumber->ignoreAnchorPointForPosition ( KD_TRUE );
						pNumber->setColor ( ccYELLOW );
						pNumber->setPosition ( ccp ( -1.f, -1.f ) );
					}
				}
			}

			if ( uSkill == 0 || !pScrGame->isSkillAvailable ( i ) )
			{
				pMenu->setEnabled ( KD_FALSE );
				pMenu->addChild ( CCLayerColor::create ( TRANSPARENT_COLOR, tMenuSize ) );
			}
		}

		if ( pMenus )
		{
			pMenus->addChild ( pMenu, i );
		}
		else
		{
			pMenus = CCMenu::createWithItem ( pMenu );
			CC_ASSERT ( pMenus );

			this->addChild ( pMenus );	

			pMenus->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		}
	}

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CDlgUseSkill::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, g_pScrManager->getTopDialogPrioty ( ), KD_TRUE );
}

KDbool CDlgUseSkill::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCRect  tRect;
	
	tRect.origin.y = 20.f;
	tRect.size = this->getContentSize ( );
	tRect.size.cx -= 35.f;
	tRect.size.cy -= tRect.origin.y;

	if ( !tRect.containsPoint ( this->convertToNodeSpace ( pTouch->getLocation ( ) ) ) )
	{
		this->hide ( );

		g_pResManager->playSound ( eSND_Etc_Click );
	}

	return KD_TRUE;
}

KDvoid CDlgUseSkill::onMenu ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	CDlgSkill::s_uIndex = ( (CCNode *) pSender )->getTag ( );

	g_pScrManager->setDialog ( eDlgSkill );
}

//
//  "CDlgGameMenu" dialog layer
//
KDbool CDlgGameMenu::init ( KDvoid )
{
	if ( CDlgGame2::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	CCSize  tLyrSize;

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_BTN_Game_Menu2 )
	);
	{
		CC_ASSERT ( pBG );

		tLyrSize = pBG->getContentSize ( );

		this->setContentSize ( tLyrSize );
		this->addChild ( pBG );

		pBG->ignoreAnchorPointForPosition ( KD_TRUE );
	}

	KDfloat  fIncr = 0;
	CCMenu*  pMenus = KD_NULL;
	for ( KDuint  i = 0; i < 3; i++ )
	{
		CCMenuItemImage*  pMenu = CCMenuItemImage::create 
		(
			g_pResManager->getPath ( (EPath) ( eIMG_BTN_Game_Help_Nor + i ) ), 
			g_pResManager->getPath ( (EPath) ( eIMG_BTN_Game_Help_Sel + i ) ),
			this,
			menu_selector ( CDlgGameMenu::onMenu ) 
		);
		{
			CC_ASSERT ( pMenu );

			const CCSize&  tSize = pMenu->getContentSize ( );

			pMenu->setTag ( i );
			pMenu->ignoreAnchorPointForPosition ( KD_TRUE );
			pMenu->setPosition ( ccp ( fIncr, 20.f ) );

			fIncr += tSize.cx;
		}

		if ( pMenus )
		{
			pMenus->addChild ( pMenu, i );
		}
		else
		{
			pMenus = CCMenu::createWithItem ( pMenu );
			CC_ASSERT ( pMenus );

			this->addChild ( pMenus );	

			pMenus->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		}
	}

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CDlgGameMenu::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, g_pScrManager->getTopDialogPrioty ( ), KD_TRUE );
}

KDbool CDlgGameMenu::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCRect  tRect;
	
	tRect.origin.y = 20.f;
	tRect.size = this->getContentSize ( );
	tRect.size.cx -= 35.f;
	tRect.size.cy -= tRect.origin.y;

	if ( !tRect.containsPoint ( this->convertToNodeSpace ( pTouch->getLocation ( ) ) ) )
	{
		this->hide ( );

		g_pResManager->playSound ( eSND_Etc_Click );
	}

	return KD_TRUE;
}

KDvoid CDlgGameMenu::onMenu ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	CCNode*  pNode = (CCNode *) pSender;

	switch ( pNode->getTag ( ) )
	{
		case 0 : g_pScrManager->setDialog ( eDlgHelp   ); break;
		case 1 : g_pScrManager->setDialog ( eDlgConfig ); break;
		case 2 : g_pScrManager->setDialog ( eDlgShop   ); break;
	}
}
