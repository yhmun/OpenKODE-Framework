/*
 *
 *      File            ScrManager.cpp
 *      Description     Scene Manager 
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
#include "Dialog/DlgBase.h"
#include "Dialog/DlgConfig.h"
#include "Dialog/DlgRank.h"
#include "Dialog/DlgHelp.h"
#include "Dialog/DlgInfo.h"
#include "Dialog/DlgShop.h"
#include "Dialog/DlgResult.h"
#include "Dialog/DlgNotice.h"
#include "Dialog/DlgGame.h"
#include "Intro/ScrSplash.h"
#include "Intro/ScrIntro.h"
#include "Menu/ScrMain.h"
#include "Menu/ScrStage.h"
#include "Game/ScrCartoon.h"
#include "Game/ScrVersus.h"
#include "Game/ScrGame.h"
#include "Game/ScrLadder.h"

//
// Screen Manager ( Scene )
//
CScrManager*  g_pScrManager = KD_NULL;

CScrManager::CScrManager ( KDvoid )
{
	CC_ASSERT ( !g_pScrManager );
}

CScrManager::~CScrManager ( KDvoid )
{

}

KDvoid CScrManager::create ( KDvoid )
{
    g_pScrManager = new CScrManager ( );
    
    CC_ASSERT ( g_pScrManager );
}

KDvoid CScrManager::release ( KDvoid )
{
	CC_SAFE_DELETE ( g_pScrManager );
}

CCLayer* CScrManager::setScene ( EScene eID, ETransition eTrans, KDfloat fDuration )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	CCScene*     pScene = CCScene::create ( );
	CC_ASSERT ( pScene );

	m_eSceneID = eID;
	g_pResManager->stopBGM ( );
	m_vDialog.clear ( );
	
	CCLayer*  pLayer = KD_NULL;
	switch ( eID )
	{
		case eScrSplash :

			pLayer = CScrSplash::create ( );
			CC_ASSERT ( pLayer );

			pScene->addChild ( pLayer );
			pDirector->runWithScene ( pScene );

			return pLayer;

		case eScrIntro		:	pLayer = CScrIntro		::create ( );  break;    
		case eScrMain		:	pLayer = CScrMain		::create ( );  break;  
		case eScrStage		:	pLayer = CScrStage		::create ( );  break;  
		case eScrCartoon	:   pLayer = CScrCartoon	::create ( );	 break;
		case eScrVersus		:   pLayer = CScrVersus		::create ( );	 break;
		case eScrGame		:	pLayer = CScrGame		::create ( );	 break;
		case eScrLadder		:	pLayer = CScrLadder		::create ( );	 break;
			
		default : ;
	}

	CC_ASSERT ( pLayer );
	pScene->addChild ( pLayer, 0, eID );
	pScene->addChild ( CKeypad::create ( ) );
	m_pScene = (CCLayer *) pScene->getChildByTag ( eID );

	CCScene*  pScene2 = KD_NULL;
	switch ( eTrans )
	{
		case eTransBlackFade	:	pScene2 = CCTransitionFade		::create ( fDuration, pScene );  break;
		case eTransMoveInL		:	pScene2 = CCTransitionMoveInL	::create ( fDuration, pScene );  break;
		case eTransMoveInR		:	pScene2 = CCTransitionMoveInR	::create ( fDuration, pScene );  break;
		case eTransMoveInT		:	pScene2 = CCTransitionMoveInT	::create ( fDuration, pScene );  break;
		case eTransMoveInB		:	pScene2 = CCTransitionMoveInB	::create ( fDuration, pScene );  break;
		case eTransSlideInL		:	pScene2 = CCTransitionSlideInL	::create ( fDuration, pScene );  break;
		case eTransSlideInR		:	pScene2 = CCTransitionSlideInR	::create ( fDuration, pScene );  break;
		case eTransSlideInT		:	pScene2 = CCTransitionSlideInT	::create ( fDuration, pScene );  break;
		case eTransSlideInB		:	pScene2 = CCTransitionSlideInB	::create ( fDuration, pScene );  break;
		case eTransShrinkGrow	:	pScene2 = CCTransitionShrinkGrow::create ( fDuration, pScene );  break;

		default					:	pScene2 = pScene;
	}

	pDirector->replaceScene ( pScene2 );

	return pLayer;
}

CCLayer* CScrManager::getScene ( KDvoid )
{
	return m_pScene;
}

EScene CScrManager::getSceneID ( KDvoid )
{
	return m_eSceneID;
}

CCLayer* CScrManager::getTopDialog ( KDvoid )
{
	KDuint  uSize = m_vDialog.size ( );

	return uSize ? m_vDialog [ uSize - 1 ] : KD_NULL;
}

KDint CScrManager::getTopDialogPrioty ( KDvoid )
{
	KDuint  uSize = m_vDialog.size ( );

	return kCCMenuHandlerPriority - ( uSize == 0 ? 1 : uSize );
}

KDvoid CScrManager::pushDialog ( CCLayer* pLayer )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	CCScene*     pScene = pDirector->getRunningScene ( );

	KDuint  uOrder = m_vDialog.size ( );

	if ( uOrder == 0 )
	{
		CTransparent*  pTransparent = CTransparent::create ( );

		pScene->addChild ( pTransparent, 1, (KDuint) pTransparent );

		m_vDialog.push_back ( (CDialog *) pScene->getChildByTag ( (KDuint) pTransparent ) );

		uOrder = m_vDialog.size ( );
	}
	else
	{
		CC_SWAP ( m_vDialog [ uOrder - 2 ], m_vDialog [ uOrder - 1 ], CCLayer* );

		pScene->reorderChild ( m_vDialog [ uOrder - 2 ], uOrder - 1 );
		pScene->reorderChild ( m_vDialog [ uOrder - 1 ], uOrder - 0 );
	}

	pDirector->getTouchDispatcher ( )->setPriority ( kCCMenuHandlerPriority - uOrder, m_vDialog [ uOrder - 1 ] );

	pScene->addChild ( pLayer, uOrder + 1, (KDuint) pLayer );

	m_vDialog.push_back ( (CDialog *) pScene->getChildByTag ( (KDuint) pLayer ) );
}

KDvoid CScrManager::popDialog ( KDvoid )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	CCScene*     pScene = pDirector->getRunningScene ( );
	KDuint       uOrder = m_vDialog.size ( );

	if ( uOrder == 0 )
	{
		return;
	}

	pScene->removeChild ( m_vDialog [ uOrder - 1 ], KD_TRUE );
	m_vDialog.pop_back ( );
	uOrder = m_vDialog.size ( );

	if ( uOrder > 1 )
	{
		pDirector->getTouchDispatcher ( )->setPriority ( kCCMenuHandlerPriority - uOrder + 2, m_vDialog [ uOrder - 1 ] );

		CC_SWAP ( m_vDialog [ uOrder - 2 ], m_vDialog [ uOrder - 1 ], CCLayer* );

		pScene->reorderChild ( m_vDialog [ uOrder - 2 ], uOrder - 1 );
		pScene->reorderChild ( m_vDialog [ uOrder - 1 ], uOrder - 0 );
	}
	else
	{
		pScene->removeChild ( m_vDialog [ uOrder - 1 ], KD_TRUE );
		m_vDialog.pop_back ( );
	}
}

CCLayer* CScrManager::setDialog ( EDialog eID )
{
	CCLayer*  pDialog = KD_NULL;
	
	switch ( eID )
	{
		case eDlgActivate	 : pDialog = CDlgActivate	::create ( ); break;
		case eDlgActivate2	 : pDialog = CDlgActivate2	::create ( ); break;
		case eDlgSMS		 : pDialog = CDlgSMS		::create ( ); break;
		case eDlgStart		 : pDialog = CDlgStart		::create ( ); break;
        case eDlgNetworkFail : pDialog = CDlgNetworkFail::create ( ); break;
		case eDlgQuit		 : pDialog = CDlgQuit		::create ( ); break;
		case eDlgRank		 : pDialog = CDlgRank		::create ( ); break;
		case eDlgRankList	 : pDialog = CDlgRankList	::create ( ); break;
		case eDlgRegister	 : pDialog = CDlgRegister	::create ( ); break;
		case eDlgNickName	 : pDialog = CDlgNickName	::create ( ); break;
		case eDlgNickName2   : pDialog = CDlgNickName2  ::create ( ); break;
		case eDlgConfig  	 : pDialog = CDlgConfig		::create ( ); break;
		case eDlgResetGame	 : pDialog = CDlgResetGame	::create ( ); break;
		case eDlgInfo		 : pDialog = CDlgInfo		::create ( ); break;
		case eDlgShop		 : pDialog = CDlgShop		::create ( ); break;
		case eDlgShopCoin	 : pDialog = CDlgShopCoin	::create ( ); break;
		case eDlgShopCash	 : pDialog = CDlgShopCash	::create ( ); break;
		case eDlgShopSkill	 : pDialog = CDlgShopSkill	::create ( ); break;
		case eDlgBuyRequest	 : pDialog = CDlgBuyRequest	::create ( ); break;
		case eDlgBuyDone	 : pDialog = CDlgBuyDone	::create ( ); break;
		case eDlgBuyFail	 : pDialog = CDlgBuyFail	::create ( ); break;
		case eDlgNotEnough	 : pDialog = CDlgNotEnough	::create ( ); break;
		case eDlgBankrupt	 : pDialog = CDlgBankrupt	::create ( ); break;
		case eDlgLevelUp	 : pDialog = CDlgLevelUp	::create ( ); break;
		case eDlgReward		 : pDialog = CDlgReward		::create ( ); break;
		case eDlgHelp		 : pDialog = CDlgHelp		::create ( ); break;
		case eDlgResult		 : pDialog = CDlgResult		::create ( ); break;
            
        case eDlgChongTong1  : pDialog = CDlgChongTong1	::create ( ); break;
        case eDlgChongTong2  : pDialog = CDlgChongTong2	::create ( ); break;
		case eDlgShake       : pDialog = CDlgShake		::create ( ); break;
        case eDlgBuck        : pDialog = CDlgBuck       ::create ( ); break;
        case eDlgNagari      : pDialog = CDlgNagari		::create ( ); break;
        case eDlgGukJin      : pDialog = CDlgGukJin		::create ( ); break;
        case eDlgSkill       : pDialog = CDlgSkill		::create ( ); break;
        case eDlgGostop      : pDialog = CDlgGostop		::create ( ); break;
        case eDlgCard        : pDialog = CDlgCard		::create ( ); break;
        case eDlgTimes       : pDialog = CDlgTimes		::create ( ); break;
        case eDlgPushPlayer	 : pDialog = CDlgPushPlayer	::create ( ); break;
		case eDlgPushEnemy	 : pDialog = CDlgPushEnemy	::create ( ); break;
        case eDlgMission     : pDialog = CDlgMission	::create ( ); break;
		case eDlgGage		 : pDialog = CDlgGage		::create ( ); break;
		case eDlgGameMenu	 : pDialog = CDlgGameMenu	::create ( ); break;
		case eDlgUseSkill	 : pDialog = CDlgUseSkill	::create ( ); break;
		case eDlgLadder		 : pDialog = CDlgLadder		::create ( ); break;
	}

	if ( pDialog )
	{
		this->pushDialog ( pDialog );
	}

	return pDialog;
}

CCLayer* CScrManager::getDialog ( KDuint uOrder )
{
	KDuint  uSize = m_vDialog.size ( );

	if ( uSize || uOrder >= ( uSize - 1 ) )
	{
		return KD_NULL;
	}

	if ( uOrder == ( uSize - 2 ) )
	{
		return m_vDialog [ uSize - 1 ];
	}
	else
	{
		return m_vDialog [ uOrder ];
	}
}

KDvoid CScrManager::showEffect ( EPath eID, CCNode* pParent )
{
	CEffect::create ( eID, pParent ? pParent : CCDirector::sharedDirector ( )->getRunningScene ( ) );
}

KDvoid CScrManager::showEffect ( EPath eID, KDuint uIndex, CCNode* pParent )
{
	CEffect::create ( (EPath) ( eID + uIndex ), pParent );
}

KDvoid CScrManager::showTalk ( ETalk eID )
{
    CTalk::create ( eID );
}

KDvoid CScrManager::showTalk ( ETalk eID, KDuint uIndex )
{
	CTalk::create ( (ETalk) ( eID + uIndex ) );
}