/*
 *
 *      File            DlgNotice.cpp
 *      Description     Notice dialog.
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
#include "DlgNotice.h"
#include "Menu/ScrStage.h"

//
//  "CDlgActivate" dialog layer
//
KDbool CDlgActivate::init ( KDvoid )
{
    m_bConfirm = KD_FALSE;
    
	if ( CDlgRequest::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Request_Activate ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pRequest );
        
		this->addChild ( pRequest );
        
		pRequest->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 40.f ) );
	}
    
   	CCLabelTTF*  pNetwork = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_Network ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        14
    );
	{
		CC_ASSERT ( pNetwork );
        
		this->addChild ( pNetwork );
        
        pNetwork->setColor ( ccGRAY );
		pNetwork->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f - 5.f ) );
	}    
    
	return KD_TRUE;
}

const KDchar* CDlgActivate::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Notice );
}

KDbool CDlgActivate::hasHide ( KDvoid )
{
	return KD_FALSE;
}

KDbool CDlgActivate::hasKeyHide ( KDvoid )
{
	return KD_FALSE;
}

KDvoid CDlgActivate::onRemove ( KDvoid )
{
    CDlgRequest::onRemove ( );
    
    if ( m_bConfirm )
    {
        g_pScrManager->setDialog ( eDlgSMS );
    }
}

KDvoid CDlgActivate::onYes ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	ENetError  eErr = g_pNetManager->activate ( );

	if ( eErr == eNet_True )
	{
		m_bConfirm = KD_TRUE;

		g_pEnvManager->setActivate ( );
		g_pEnvManager->save ( );
		
		this->hide ( );
	}
	else
	{	
		g_pScrManager->setDialog ( eDlgNetworkFail );
	}
}

KDvoid CDlgActivate::onNo ( CCObject* pSender )
{
    g_pResManager->playSound ( eSND_Etc_Click );
    
	g_pScrManager->setDialog ( eDlgActivate2 );
}

//
//  "CDlgActivate2" dialog layer
//
KDbool CDlgActivate2::init ( KDvoid )
{
	if ( CDlgRequest::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Request_Activate2 ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pRequest );
        
		this->addChild ( pRequest );
        
		pRequest->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 30.f ) );
	}
    
	return KD_TRUE;
}

const KDchar* CDlgActivate2::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Notice );
}

KDvoid CDlgActivate2::onYes ( CCObject* pSender )
{
    CDlgRequest::onYes ( pSender );
    
	CCDirector::sharedDirector ( )->end ( );
}

//
//  "CDlgSMS" dialog layer
//
KDbool CDlgSMS::init ( KDvoid )
{
	if ( CDlgRequest::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}
    
    const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Request_SMS ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pRequest );
        
		this->addChild ( pRequest );
        
		pRequest->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 30.f ) );
	}   

	return KD_TRUE;
}

const KDchar* CDlgSMS::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Notice );
}

KDvoid CDlgSMS::onRemove ( KDvoid )
{
    CDlgRequest::onRemove ( );
    
    g_pScrManager->setDialog ( eDlgStart );
}

KDvoid CDlgSMS::onYes ( CCObject* pSender )
{
	CDlgRequest::onYes ( pSender );

	g_pNetManager->agreeSMS ( );
}

//
//  "CDlgStart" dialog layer
//
KDbool CDlgStart::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_Start ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 30.f ) );
	}  
    
	return KD_TRUE;
}

const KDchar* CDlgStart::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Notice );
}

KDvoid CDlgStart::onRemove ( KDvoid )
{
    CDlgConfirm::onRemove ( );
    
    g_pScrManager->setScene ( eScrIntro, eTransNull );
}

KDbool CDlgNetworkFail::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}
    
    const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_Network_Fail ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 30.f ) );
	}  
    
	return KD_TRUE;
}

const KDchar* CDlgNetworkFail::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Notice );
}

//
//  "CDlgQuit" dialog layer
//
KDbool CDlgQuit::init ( KDvoid )
{
	if ( CDlgRequest::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Request_Quit ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pRequest );

		this->addChild ( pRequest );

		pRequest->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 30.f ) );
	}
    
	return KD_TRUE;
}

const KDchar* CDlgQuit::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Exit );
}

KDvoid CDlgQuit::onYes ( CCObject* pSender )
{
	CDlgRequest::onYes ( pSender );

	CCDirector::sharedDirector ( )->end ( );
}

//
//  "CDlgLadder" dialog layer
//
KDuint CDlgLadder::s_uPercent = 0;
KDbool CDlgLadder::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );    
	KDuint  uCash = g_pEnvManager->getCashLast ( ) * s_uPercent / 100;
	KDchar  szText [ 64 ];
	
	if ( uCash == 0 )
	{
		uCash = 5;
	}

	g_pEnvManager->setCash ( uCash );
	g_pEnvManager->save ( );

	kdSprintfKHR
	(
		szText, "%d%% %s!!",
		s_uPercent,
		g_pResManager->getText ( eTXT_Prize )
	);

	CCLabelTTF*  pPrize = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		20
	);
	{
		CC_ASSERT ( pPrize );

		this->addChild ( pPrize );

		const CCSize&  tSize = pPrize->getContentSize ( );

		pPrize->setPosition ( ccp ( tLyrSize.cx / 2.f, ( tLyrSize.cy + tSize.cy ) / 2.f + 35.f ) );
	}

	CCLabelTTF*  pCash = CCLabelTTF::create 
	(
		g_pEnvManager->convCash ( uCash ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		24
	);
	{
		CC_ASSERT ( pCash );

		this->addChild ( pCash );

		const CCSize&  tSize = pCash->getContentSize ( );
		
		pCash->setColor ( ccYELLOW );
		pCash->setPosition ( ccp ( ( tLyrSize.cx - tSize.cx ) / 2.f - 15.f, tLyrSize.cy / 2.f ) );
	}

	kdSprintfKHR
	(
		szText, "%s %s!",
		g_pResManager->getText ( eTXT_Won ),
		g_pResManager->getText ( eTXT_Obtain )
	);
	CCLabelTTF*  pObtain = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		24
	);
	{
		CC_ASSERT ( pObtain );

		this->addChild ( pObtain );

		pObtain->setPosition ( ccp ( tLyrSize.cx / 2.f + 40.f, tLyrSize.cy / 2.f ) );
	}

	g_pResManager->playSound ( eSND_Etc_Ladder );

	return KD_TRUE;
}

KDvoid CDlgLadder::onRemove ( KDvoid )
{
	CDlgConfirm::onRemove ( );

	g_pScrManager->setScene ( eScrStage, eTransSlideInR );
}

//
//  "CDlgBankrupt" dialog layer
//
KDbool CDlgBankrupt::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
    
	CCLabelTTF*  pDesc1 = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Bankrupt_Desc1 ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pDesc1 );

		this->addChild ( pDesc1 );

		const CCSize&  tSize = pDesc1->getContentSize ( );

		pDesc1->setColor ( ccYELLOW );
		pDesc1->setPosition ( ccp ( tLyrSize.cx / 2.f, ( tLyrSize.cy + tSize.cy ) / 2.f + 55.f ) );
	}

	CCLabelTTF*  pDesc2 = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Bankrupt_Desc2 ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pDesc2 );

		this->addChild ( pDesc2 );

		const CCSize&  tSize = pDesc2->getContentSize ( );

		pDesc2->setPosition ( ccp ( tLyrSize.cx / 2.f, ( tLyrSize.cy + tSize.cy ) / 2.f - 40.f ) );
	}

	CCMenuItemImage*  pGame = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Mini_Game_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Mini_Game_Sel ),
		this,
		menu_selector ( CDlgBankrupt::onGame ) 
	);
	CC_ASSERT ( pGame );

	CCMenu*  pMenu = CCMenu::createWithItem ( pGame );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pGame->getContentSize ( );

		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f - 8.f, tSize.cy - 12.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
	}

	g_pResManager->playSound ( eSND_Etc_Notice1 );

	return KD_TRUE;
}

const KDchar* CDlgBankrupt::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Bankrupt );
}

CCPoint CDlgBankrupt::getPosTitle ( KDvoid )
{
	return ccp ( 5.f, -3.f );
}

KDvoid CDlgBankrupt::onGame ( CCObject* pSender )
{
	g_pScrManager->setScene ( eScrLadder, eTransSlideInR );
}

//
//  "CDlgLevelUp" dialog layer
//
KDbool CDlgLevelUp::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	KDuint  uIndex = g_pEnvManager->getStar ( ) / 5;
	KDchar  szText [ 64 ];

	kdSprintfKHR
	(
		szText, "LV.%s",
		g_pResManager->getText ( eTXT_Level_Name1, uIndex )
	); 
   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        szText,
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        28
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setColor ( ccYELLOW );
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 45.f ) );
	}

	kdSprintfKHR 
	(
		szText, "%s!",
		g_pResManager->getText ( eTXT_LevelUp )
	);
   	CCLabelTTF*  pNotice2 = CCLabelTTF::create 
	(
        szText,
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        16
    );
	{
		CC_ASSERT ( pNotice2 );
        
		this->addChild ( pNotice2 );
        
		pNotice2->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f - 5.f ) );
	}

	g_pResManager->playSound ( eSND_Etc_Notice1 );
	g_pEnvManager->setLevel ( uIndex );
	g_pEnvManager->save ( );

	return KD_TRUE;
}

KDvoid CDlgLevelUp::onRemove ( KDvoid )
{
	CDlgConfirm::onRemove ( );

	CScrStage*  pScrStage = (CScrStage *) g_pScrManager->getScene ( );
	
	pScrStage->onCheck ( );
}

//
//  "CDlgReward" dialog layer
//
KDuint CDlgReward::s_uIndex = 0;
KDbool CDlgReward::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	KDuint  uIndex [ 2 ];
	KDchar  szText [ 64 ];

	switch ( s_uIndex )
	{
		case 0 :	uIndex [ 0 ] = 0; uIndex [ 1 ] = KDUINT_MAX;	break;
		case 1 :	uIndex [ 0 ] = 0; uIndex [ 1 ] = 1;				break;
		case 2 :	uIndex [ 0 ] = 1; uIndex [ 1 ] = KDUINT_MAX;	break;
		case 3 :	uIndex [ 0 ] = 3; uIndex [ 1 ] = 7;				break;
		case 4 :	uIndex [ 0 ] = 2; uIndex [ 1 ] = KDUINT_MAX;	break;
		case 5 :	uIndex [ 0 ] = 5; uIndex [ 1 ] = 6;				break;
		case 6 :	uIndex [ 0 ] = 3; uIndex [ 1 ] = KDUINT_MAX;	break;
		case 7 :	uIndex [ 0 ] = 2; uIndex [ 1 ] = 4;				break;
		case 8 :	uIndex [ 0 ] = 8; uIndex [ 1 ] = 9;				break;
	}

	if ( uIndex [ 1 ] == KDUINT_MAX )
	{
		// Title Reward
		CCSprite*  pTitle = CCSprite::create
		(
			g_pResManager->getPathSprite ( eIMG_Sprite_Title_Menu_Nor, uIndex [ 0 ] )
		);
		{
			CC_ASSERT ( pTitle );
			
			this->addChild ( pTitle );

			const CCSize  tSize = pTitle->getContentSize ( );

			pTitle->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, ( tLyrSize.cy + tSize.cy ) / 2.f - 3.f ) );
		}

		g_pEnvManager->setTitleMax ( uIndex [ 0 ] + 1 );
	}
	else
	{
		// Skill Reward
		for ( KDuint  i = 0; i < 2; i++ )
		{
			CCSprite*  pSkill = CCSprite::create
			(
				g_pResManager->getPathSprite ( eIMG_Sprite_Skill, uIndex [ i ] )
			);
			{
				CC_ASSERT ( pSkill );
				
				this->addChild ( pSkill );

				const CCSize  tSize = pSkill->getContentSize ( );

				pSkill->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f + ( tSize.cx / 2.f + 10.f ) * ( i == 0 ? -1.f : 1.f ), ( tLyrSize.cy + tSize.cy ) / 2.f + 13.f ) );
			}

			g_pEnvManager->setSkill ( uIndex [ i ], g_pEnvManager->getSkill ( uIndex [ i ] ) + 1 );
		}
	}

	kdSprintfKHR 
	(
		szText, "%s %s!!",
		g_pResManager->getText ( uIndex [ 1 ] == KDUINT_MAX ? eTXT_Title : eTXT_Skill ),
		g_pResManager->getText ( eTXT_Obtain )
	);
   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        szText,
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setColor ( ccYELLOW );
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f - 25.f ) );
	}

	g_pResManager->playSound ( eSND_Etc_Notice1 );
	g_pEnvManager->setReward ( s_uIndex, KD_TRUE );
	g_pEnvManager->save ( );
	
	return KD_TRUE;
}

KDvoid CDlgReward::onRemove ( KDvoid )
{
	CDlgConfirm::onRemove ( );

	CScrStage*  pScrStage = (CScrStage *) g_pScrManager->getScene ( );
	
	pScrStage->onCheck ( );
}