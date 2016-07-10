/*
 *
 *      File            DlgResult.cpp
 *      Description     Result dialog.
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
#include "DlgResult.h"
#include "DlgGame.h"
#include "Game/ScrGame.h"
 
KDbool CDlgResult::init ( KDvoid )
{
	if ( CDlgGame2::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	CScrGame*      pScrGame = (CScrGame *) g_pScrManager->getScene ( );

	TScore*	 pScore = pScrGame->getTotal ( );
	KDuint   uTurn  = pScrGame->getTurn ( );
	EPath    eFont  = uTurn == 0 ? eIMG_Font_D : eIMG_Font_E;

	KDuint   uCashes [ 2 ] = { g_pEnvManager->getCash ( ), g_pEnvManager->getEnemyCash ( ) };
	KDuint	 uWinCash = pScore->uTotalScore * g_pEnvManager->getBet ( );

	// Title Reward ( 5% )
	if ( uTurn == 0 && g_pEnvManager->getTitle ( ) == 1 )
	{
		uWinCash += (KDuint) ( uWinCash * 0.05f );
	}

	KDbool   bBankrupted = uCashes [ !uTurn ] <= uWinCash;
	KDuint	 uObtainCash = bBankrupted ? uCashes [ !uTurn ] : uWinCash;
	
	KDchar   szText [ 64 ];
    KDuint   i;

	g_pEnvManager->setResult ( !uTurn, uObtainCash, pScore->uScore, pScore->uTotalTimes );
	g_pEnvManager->save ( );

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_BG_Result )
	);
	{
		CC_ASSERT ( pBG );

		this->addChild ( pBG );
		
		pBG->setPosition ( ccpMid ( tWinSize ) );
	}

	CCSprite*  pResult = CCSprite::create
	(
		g_pResManager->getPath ( uTurn == 0 ? eIMG_ETC_Win : eIMG_ETC_Lose )
	);
	{
		CC_ASSERT ( pResult );

		this->addChild ( pResult );

		const CCSize&  tSize = pResult->getContentSize ( );
		
		pResult->setPosition ( ccp ( tWinSize.cx / 2.f, tWinSize.cy - tSize.cy / 2.f - 10.f ) );
	}

	kdSprintfKHR ( szText, "%04d", pScore->uScore );
	CText*  pTxtScore = CText::create ( szText, eFont );
	{
		CC_ASSERT ( pTxtScore );

		this->addChild ( pTxtScore );

		pTxtScore->setPosition ( ccp ( 395.f, 280.f ) );
	}

	CCSprite*  pPointTimes = CCSprite::create 
	(
		g_pResManager->getPath ( eIMG_ETC_Point_Times )
	);
	{
		CC_ASSERT ( pPointTimes );

		this->addChild ( pPointTimes );

		pPointTimes->ignoreAnchorPointForPosition ( KD_TRUE );
		pPointTimes->setPosition ( ccp ( 396.f, 280.f ) );
	}

	kdSprintfKHR ( szText, "%04d", pScore->uTotalTimes );
	CText*  pTimes = CText::create ( szText, eFont );
	{
		CC_ASSERT ( pTimes );

		this->addChild ( pTimes );

		pTimes->setPosition ( ccp ( 622.f, 280.f ) );
	}

	kdSprintfKHR ( szText, "%04d", pScore->uTotalScore );
	CText*  pTotal = CText::create ( szText, eFont );
	{
		CC_ASSERT ( pTotal );

		this->addChild ( pTotal );

		pTotal->setPosition ( ccp ( 578.f, 225.f ) );
	}

	kdSprintfKHR ( szText, "%s%s", uTurn == 0 ? "+" : "-", g_pEnvManager->convCash ( uObtainCash ) );
	CText*  pCash = CText::create ( szText, uTurn == 0 ? eIMG_Font_D : eIMG_Font_E );
	{
		CC_ASSERT ( pCash );

		this->addChild ( pCash );

		pCash->setPosition ( ccp ( 578.f, 170.f ) );
	}

	CCLabelTTF*  pDetail = CCLabelTTF::create 
	(
		this->getDetail ( ).c_str ( ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ), 10,
		CCSize ( 310.f, 60.f ),
		kCCTextAlignmentCenter
	);
	{
		this->addChild ( pDetail );

		pDetail->setAnchorPoint ( ccp ( 0, 0.5f ) );
		pDetail->setPosition ( ccp ( 230.f, 125.f ) );
	}

	for ( i = 0; i < 2; i++ )
	{
		KDuint  uIndex;
		EState  eState;
		
		if ( i == 0 )
		{
			uIndex = 0;
			eState = uTurn == 0 ? eStatSmile : ( bBankrupted ? eStatSad : eStatAngry );
		}
		else
		{
			uIndex = g_pEnvManager->getStage ( ) + 1;
			eState = uTurn == 1 ? eStatSmile : ( bBankrupted ? eStatSad : eStatAngry );
		}

		CCSprite*  pCharacter = CCSprite::create
		(
			g_pResManager->getPathCharacter ( uIndex, eState )
		);
		{
			CC_ASSERT ( pCharacter );
	        
			this->addChild ( pCharacter );
	        
			const CCSize&  tSize = pCharacter->getContentSize ( );
	        
			pCharacter->ignoreAnchorPointForPosition ( KD_TRUE );
			pCharacter->setPosition ( ccp ( i == 0 ? -tSize.cx / 4.f : tWinSize.cx - tSize.cx * 0.75f, 158.f ) );
		}

		if ( eState == eStatSad )
		{
			CCSprite*  pBankrupted = CCSprite::create
			(
				g_pResManager->getPath ( eIMG_ETC_Bankrupted )
			);
			{
				CC_ASSERT ( pBankrupted );

				this->addChild ( pBankrupted );

				const CCSize&  tSize = pBankrupted->getContentSize ( );
				
				pBankrupted->ignoreAnchorPointForPosition ( KD_TRUE );
				pBankrupted->setPosition ( ccp ( i == 0 ? 10.f : tWinSize.cx - tSize.cx - 10.f, 170.f ) );
			}
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
			14
		);
		{
			CC_ASSERT ( pName );
	        
			this->addChild ( pName );
	        
			const CCSize&  tSize = pName->getContentSize ( );
	        
			pName->ignoreAnchorPointForPosition ( KD_TRUE );
			pName->setPosition ( ccp ( i == 0 ? 10 : tWinSize.cx - tSize.cx - 10, 121.f ) );
		} 

		kdSprintfKHR
		(
			szText,
			"%s%s",
			g_pEnvManager->convCash ( i == 0 ? g_pEnvManager->getCash ( ) : g_pEnvManager->getEnemyCash ( ) ),
			g_pResManager->getText ( eTXT_Won )
		);

		CCLabelTTF*  pCash = CCLabelTTF::create 
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			14
		);
		{
			CC_ASSERT ( pCash );
	        
			this->addChild ( pCash );
	        
			const CCSize&  tSize = pCash->getContentSize ( );
	        
			pCash->ignoreAnchorPointForPosition ( KD_TRUE );
			pCash->setPosition ( ccp ( i == 0 ? 10 : tWinSize.cx - tSize.cx - 10, 95.f ) );
		} 
	}

	CCMenuItemImage*  pContinue = CCMenuItemImage::create  
	(
		g_pResManager->getPath ( eIMG_BTN_Continue_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Continue_Sel ),
		this,
		menu_selector ( CDlgResult::onContinue ) 	
	);
	CC_ASSERT ( pContinue );

	CCMenuItemImage*  pPush = CCMenuItemImage::create  
	(
		g_pResManager->getPath ( eIMG_BTN_Push_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Push_Sel ),
		this,
		menu_selector ( CDlgResult::onPush ) 	
	);
	CC_ASSERT ( pPush );

	CCMenuItemImage*  pStop = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Stop2_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Stop2_Sel ),
		this,
		menu_selector ( CDlgResult::onStop ) 	
	);
	CC_ASSERT ( pStop );

	CCMenu*  pMenu = KD_NULL;
	KDuint   uPushLimit = g_pEnvManager->getTitle ( ) == 2 ? 5 : 4;
	if ( bBankrupted )
	{
		pMenu = CCMenu::create ( pContinue, KD_NULL );
	}
	else if ( uTurn == 0 && g_pEnvManager->getPushCount ( ) < uPushLimit )
	{
		pMenu = CCMenu::create ( pContinue, pPush, pStop, KD_NULL );
	}
	else
	{
		pMenu = CCMenu::create ( pContinue, pStop, KD_NULL );
	}
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pContinue->getContentSize ( );

		pMenu->setPosition ( ccp ( tWinSize.cx / 2.f, tSize.cy / 2.f + 10.f ) );
		
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsHorizontallyWithPadding ( 10.f );
	}

	g_pResManager->playSound
	(
		uTurn == 0 ? ( bBankrupted ? eSND_BGM_Win2 : eSND_BGM_Win ) : ( bBankrupted ? eSND_BGM_Lose2 : eSND_BGM_Lose ), 0
	);

	return KD_TRUE;
}

KDbool CDlgResult::hasKeyHide ( KDvoid )
{
	return KD_FALSE;
}

std::string CDlgResult::getDetail ( KDvoid )
{
	CScrGame*		pScrGame = (CScrGame *) g_pScrManager->getScene ( );
	TScore*			pScore = pScrGame->getScore ( );
	std::string		sText  = "";
	KDchar			szText [ 64 ];
	KDuint			uCount = 0;

	if ( g_pEnvManager->getPushCount ( ) )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_Push ),
			(KDint) kdPowf ( 2, (KDfloat) g_pEnvManager->getPushCount ( ) ),
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->bChongTong )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_ChongTong ),
			10,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;

		return sText;
	}

	if ( pScore->uBuck == 3 )
	{
		kdSprintfKHR 
		(
			szText, "3%s%d%s", 
			g_pResManager->getText ( eTXT_Buck ),
			7,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;

		return sText;
	}

	if ( pScore->bChongTongGo )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_ChongTong ),
			4,
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uMission )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_Mission ),
			pScore->uMission,
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uSkill )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_Skill ),
			(KDuint) kdPowf ( 3, (KDfloat) pScore->uSkill ),
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uGo )
	{
		kdSprintfKHR 
		(
			szText, "%d%s%d%s", 
			pScore->uGo,
			g_pResManager->getText ( eTXT_Go ),
			pScore->uGo < 3 ? pScore->uGo : (KDint) kdPowf ( 2, (KDfloat) ( pScore->uGo - 2 ) ),
			g_pResManager->getText ( pScore->uGo < 3 ? eTXT_Point : eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uShake )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_Shake ),
			(KDint) kdPowf ( 2, (KDfloat) pScore->uShake ),
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uGage )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_Gage ),
			(KDint) kdPowf ( 2, (KDfloat) pScore->uGage ),
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->bGoBak )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_GoBak ),
			2,
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->bGwangBak )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_GwangBak ),
			2,
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->bMungBak )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_MungBak ),
			2,
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->bPiBak )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_PiBak ),
			2,
			g_pResManager->getText ( eTXT_Times )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uGwang > 2 )
	{
		EText   eText;
		KDuint  uScore = 0;

		switch ( pScore->uGwang )
		{
			case 3 : 

				if ( pScore->uBGwang )
				{
					eText  = eTXT_BGwang;
					uScore = 2;
				}
				else
				{
					eText  = eTXT_3Gwang;
					uScore = 3;
				}

				break;

			case 4 :

				eText  = eTXT_4Gwang;
				uScore = 4;  

				break;

			case 5 :

				eText  = eTXT_5Gwang; 
				uScore = 15; 

				break;
		}

		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eText ),
			uScore,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uGodori == 3 )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_Godori ),
			5,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uChoDan == 3 )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_ChoDan ),
			3,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uHongDan == 3 )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_HongDan ),
			3,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->uChungDan == 3 )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_ChungDan ),
			3,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->u10Pi > 4 )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_10Pi ),
			pScore->u10Pi - 4,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->u5Pi > 4 )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_5Pi ),
			pScore->u5Pi - 4,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	if ( pScore->u1Pi > 9 )
	{
		kdSprintfKHR 
		(
			szText, "%s%d%s", 
			g_pResManager->getText ( eTXT_Pi ),
			pScore->u1Pi - 9,
			g_pResManager->getText ( eTXT_Point )
		);

		uCount++;
		sText += uCount % 6 == 0 ?  "\n " : " "; 
		sText += szText;
	}

	return sText;
}

KDvoid CDlgResult::onContinue ( CCObject* pSender )
{
	if ( g_pEnvManager->getEnemyCash ( ) == 0 )
	{
		g_pScrManager->setScene ( eScrCartoon, eTransSlideInR );
	}
	else if ( g_pEnvManager->getCash ( ) == 0 )
	{
		g_pScrManager->setScene ( eScrStage, eTransSlideInR );
	}
	else
	{
		g_pEnvManager->setPushCount ( 0 );
		g_pScrManager->setScene ( eScrGame, eTransSlideInR );
	}
}

KDvoid CDlgResult::onPush ( CCObject* pSender )
{
	g_pScrManager->setDialog ( eDlgPushPlayer );
}

KDvoid CDlgResult::onStop ( CCObject* pSender )
{
	g_pEnvManager->setPushCount ( 0 );
	g_pScrManager->setScene ( eScrStage, eTransSlideInL );
}