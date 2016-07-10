/*
 *
 *      File            DlgShop.cpp
 *      Description     Shop dialog.
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
#include "DlgShop.h"
#include "Game/ScrGame.h"

//
//  "CDlgShop" dialog layer
//
EDialog CDlgShop::s_eDialog;
KDuint  CDlgShop::s_uIndex;
KDbool CDlgShop::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	
	CCMenuItemImage*  pBtns [ 3 ];
	for ( KDuint  i = 0; i < 3; i++ )
	{
		std::string  sPathNor = g_pResManager->getPathSprite ( eIMG_BTN_Shop_Menu_Nor, i );
		std::string  sPathSel = g_pResManager->getPathSprite ( eIMG_BTN_Shop_Menu_Sel, i );

		CCMenuItemImage*  pBtn = CCMenuItemImage::create 
		(
			sPathNor.c_str ( ), 
			sPathSel.c_str ( ),
			this,
			menu_selector ( CDlgShop::onMenu ) 	
		);
		{
			CC_ASSERT ( pBtn );

			pBtn->setTag ( i );

			pBtns [ i ] = pBtn;
		}
	}

	CCMenu*  pMenu = CCMenu::create 
	(
		pBtns [ 0 ], pBtns [ 1 ], pBtns [ 2 ], KD_NULL 
	);
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f - 9.f, tLyrSize.cy / 2.f - 22.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsVertically ( 4.f );
	}

	return KD_TRUE;
}

const KDchar* CDlgShop::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_Shop );
}

const KDchar* CDlgShop::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Shop );
}

CCPoint CDlgShop::getPosTitle ( KDvoid )
{
    return ccp ( 3.f, -3.f );
}

KDvoid CDlgShop::onMenu ( CCObject* pSender )
{
	CCNode*  pNode = (CCNode *) pSender;
	KDuint    uTag = pNode->getTag ( );

	switch ( uTag )
	{
		case 0 : s_eDialog = eDlgShopCoin; break;
		case 1 : s_eDialog = eDlgShopCash; break;
		case 2 : s_eDialog = eDlgShopSkill; break;
	}

	g_pResManager->playSound ( eSND_Etc_Click );
	g_pScrManager->setDialog ( s_eDialog );
}

//
//  "CLyrCoin" sub layer
//
CLyrCoin* CLyrCoin::s_pLyrCoin = KD_NULL;
KDbool CLyrCoin::init ( KDvoid )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	KDuint  i;

	for ( i = 0; i < 3; i++ )
	{
		const KDchar*  szText;

		switch ( i )
		{
			case 0 : szText = g_pResManager->getText ( eTXT_Unit ); break;
			case 1 : szText = ""; break;
			case 2 : szText = g_pResManager->getText ( eTXT_Obtain_Coin ); break;
		}

		CCLabelTTF*  pLabel = CCLabelTTF::create
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			i == 1 ? 18.f : 16.f,
			CCSize ( 0, 0 ),
			kCCTextAlignmentRight
		);
		{
			CC_ASSERT ( pLabel );

			this->addChild ( pLabel, 0, (KDuint) pLabel );

			if ( i == 1 )
			{
				pLabel->setColor ( ccYELLOW );
			}

			pLabel->ignoreAnchorPointForPosition ( KD_TRUE );

			m_pLabel [ i ] = (CCLabelTTF *) this->getChildByTag ( (KDuint) pLabel );
		}
	}

	this->refresh ( );

	return KD_TRUE;
}

KDvoid CLyrCoin::refresh ( KDvoid )
{
	KDuint   uCoin = g_pEnvManager->getCoin ( );
	KDfloat  fPos  = 0;
	KDuint   i;
	
	m_pLabel [ 1 ]->setString 
	(
		g_pEnvManager->convCoin ( uCoin )
	);

	for ( i = 0; i < 3; i++ )
	{
		fPos -= m_pLabel [ i ]->getContentSize ( ).cx - ( i == 2 ? 0 : 8.f );

		m_pLabel [ i ]->setPosition ( ccp ( fPos, 0 ) );
	}
}

//
//  "CDlgShopCoin" dialog layer
//
KDbool CDlgShopCoin::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	
	CCMenuItemImage*  pBtns [ 5 ];
	for ( KDuint  i = 0; i < 5; i++ )
	{
		std::string  sPathNor = g_pResManager->getPathSprite ( eIMG_BTN_Shop_Coin_Nor, i );
		std::string  sPathSel = g_pResManager->getPathSprite ( eIMG_BTN_Shop_Coin_Sel, i );

		CCMenuItemImage*  pBtn = CCMenuItemImage::create  
		(
			sPathNor.c_str ( ), 
			sPathSel.c_str ( ),
			this,
			menu_selector ( CDlgShopCoin::onMenu ) 	
		);
		{
			CC_ASSERT ( pBtn );

			pBtn->setTag ( i );

			pBtns [ i ] = pBtn;
		}
	}

	CCMenu*  pMenu = CCMenu::create 
	(
		pBtns [ 0 ], pBtns [ 1 ], pBtns [ 2 ], pBtns [ 3 ], pBtns [ 4 ], KD_NULL 
	);
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f - 9.f, tLyrSize.cy / 2.f + 3.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsVertically ( 4.f );
	}

	CLyrCoin*  pCoin = CLyrCoin::create ( );
	{
		CC_ASSERT ( pCoin );

		this->addChild ( pCoin, 0, (KDuint) pCoin );

		pCoin->setPosition ( ccp ( tLyrSize.cx - 40.f, 23.f ) );

		CLyrCoin::s_pLyrCoin = (CLyrCoin *) this->getChildByTag ( (KDuint) pCoin );
	}
	
	return KD_TRUE;
}

const KDchar* CDlgShopCoin::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_Shop2 );
}

const KDchar* CDlgShopCoin::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Shop_Coin );
}

CCPoint CDlgShopCoin::getPosTitle ( KDvoid )
{
    return ccp ( 3.f, -3.f );
}

KDvoid CDlgShopCoin::onMenu ( CCObject* pSender )
{
	CCNode*  pNode  = (CCNode *) pSender;
	KDuint   uTag   = pNode->getTag ( );

	CDlgShop::s_uIndex = uTag;

	g_pScrManager->setDialog ( eDlgBuyRequest );
}

//
//  "CDlgShopCash" dialog layer
//
KDbool CDlgShopCash::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	
	CCMenuItemImage*  pBtns [ 5 ];
	for ( KDuint  i = 0; i < 5; i++ )
	{
		std::string  sPathNor = g_pResManager->getPathSprite ( eIMG_BTN_Shop_Cash_Nor, i );
		std::string  sPathSel = g_pResManager->getPathSprite ( eIMG_BTN_Shop_Cash_Sel, i );

		CCMenuItemImage*  pBtn = CCMenuItemImage::create 
		(
			sPathNor.c_str ( ), 
			sPathSel.c_str ( ),
			this,
			menu_selector ( CDlgShopCash::onMenu ) 	
		);
		{
			CC_ASSERT ( pBtn );

			pBtn->setTag ( i );

			pBtns [ i ] = pBtn;
		}
	}

	CCMenu*  pMenu = CCMenu::create 
	(
		pBtns [ 0 ], pBtns [ 1 ], pBtns [ 2 ], pBtns [ 3 ], pBtns [ 4 ], KD_NULL 
	);
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f - 9.f, tLyrSize.cy / 2.f + 3.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
		pMenu->alignItemsVertically ( 4.f );
	}

	CLyrCoin*  pCoin = CLyrCoin::create ( );
	{
		CC_ASSERT ( pCoin );

		this->addChild ( pCoin, 0, (KDuint) pCoin );

		pCoin->setPosition ( ccp ( tLyrSize.cx - 40.f, 23.f ) );

		CLyrCoin::s_pLyrCoin = (CLyrCoin *) this->getChildByTag ( (KDuint) pCoin );
	}

	return KD_TRUE;
}

const KDchar* CDlgShopCash::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_Shop2 );
}

const KDchar* CDlgShopCash::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Shop_Cash );
}

CCPoint CDlgShopCash::getPosTitle ( KDvoid )
{
    return ccp ( 3.f, -3.f );
}

KDvoid CDlgShopCash::onMenu ( CCObject* pSender )
{
	CCNode*  pNode  = (CCNode *) pSender;
	KDuint   uTag   = pNode->getTag ( );
	KDuint   uCoin  = g_pEnvManager->getCoin ( );		
	KDuint   uPrice = g_pEnvManager->getPriceCash ( uTag );

	g_pResManager->playSound ( eSND_Etc_Click );

	if ( uCoin >= uPrice )
	{
		CDlgShop::s_uIndex = uTag;

		g_pScrManager->setDialog ( eDlgBuyRequest );
	}
	else
	{
		g_pScrManager->setDialog ( eDlgNotEnough );
	}
}

//
//  "CDlgShopSkill" dialog layer
//
KDuint CDlgShopSkill::m_uSelect = KDUINT_MAX;

KDbool CDlgShopSkill::init ( KDvoid )
{
	m_uSelect = KDUINT_MAX;

	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	KDchar   szText [ 64 ];

	CCMenu*  pMenus = KD_NULL;
	for ( KDuint  i = 0; i < COUNT_SKILL; i++ )
	{
		CCMenuItemImage*  pMenu = CCMenuItemImage::create  
		(
			g_pResManager->getPathSprite ( eIMG_Sprite_Skill, i ), 
			KD_NULL,
			this,
			menu_selector ( CDlgShopSkill::onSelect ) 	
		);
		{
			CC_ASSERT ( pMenu );

			const CCSize&  tSize = pMenu->getContentSize ( );
			CCPoint  tPosition = i < ( COUNT_SKILL / 2 ) ?
				ccp ( ( tSize.cx + 15.f ) * i, tSize.cy + 15.f ) :
				ccp ( ( tSize.cx + 15.f ) * ( i % ( COUNT_SKILL / 2 ) ), 0 );

			pMenu->setTag ( i );
			pMenu->setPosition ( tPosition );
			pMenu->ignoreAnchorPointForPosition ( KD_TRUE );

			if ( pMenus )
			{
				pMenus->addChild ( pMenu, i );
			}
			else
			{
				pMenus = CCMenu::createWithItem ( pMenu );
				{
					CC_ASSERT ( pMenus );

					this->addChild ( pMenus, 1 );

					pMenus->setPosition ( ccp ( 35.f, 253.f ) );
					pMenus->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
				}
			}
		}
	}

	CCSprite*  pSelect = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_ETC_Skill_Menu_Border1 ) 
	);
	{
		CC_ASSERT ( pSelect );

		this->addChild ( pSelect, 0, (KDuint) pSelect );

		pSelect->setVisible ( KD_FALSE );
		pSelect->ignoreAnchorPointForPosition ( KD_TRUE );

		m_pSelect = (CCSprite *) this->getChildByTag ( (KDuint) pSelect );
		m_pSelect->setTag ( KDUINT_MAX );
	}

	CCLabelTTF*  pDesc = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Select_Skill_Buy ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		16
	);
	{
		CC_ASSERT ( pDesc );

		this->addChild ( pDesc, 0, (KDuint) pDesc );

		pDesc->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, + 177.f ) );

		m_pDesc = (CCLabelTTF *) this->getChildByTag ( (KDuint) pDesc );
	}

	CCSprite*  pIcon = CCSprite::create 
	(
		g_pResManager->getPath ( eIMG_ETC_Coin )
	);
	{
		CC_ASSERT ( pIcon );

		this->addChild ( pIcon );

		pIcon->setPosition ( ccp ( 20.f, 60.f ) );
		pIcon->ignoreAnchorPointForPosition ( KD_TRUE );
	}

	kdSprintfKHR 
	(
		szText, "x 0%s", 
		g_pResManager->getText ( eTXT_Unit )
	);
	CCLabelTTF*  pCost = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		18
	);
	{
		CC_ASSERT ( pCost );

		this->addChild ( pCost, 0, (KDuint) pCost );

		pCost->setColor ( ccYELLOW );
		pCost->setPosition ( ccp ( 60.f, 65.f ) );
		pCost->ignoreAnchorPointForPosition ( KD_TRUE );

		m_pCost = (CCLabelTTF *) this->getChildByTag ( (KDuint) pCost );
	}

	CCMenuItemImage*  pBuy = CCMenuItemImage::create  
	(
		g_pResManager->getPath ( eIMG_BTN_Buy_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Buy_Sel ),
		this,
		menu_selector ( CDlgShopSkill::onBuy ) 
	); 
	CC_ASSERT ( pBuy );

	CCMenu*  pBuyMenu = CCMenu::createWithItem ( pBuy );
	{
		CC_ASSERT ( pBuyMenu );

		this->addChild ( pBuyMenu );

		const CCSize&  tSize = pBuy->getContentSize ( );

		pBuyMenu->setPosition ( ccp ( tLyrSize.cx - tSize.cx / 2.f - 32.f, tSize.cy / 2.f + 52.f ) );
		pBuyMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
	}
    
	CLyrCoin*  pCoin = CLyrCoin::create ( );
	{
		CC_ASSERT ( pCoin );

		this->addChild ( pCoin, 0, (KDuint) pCoin );

		pCoin->setPosition ( ccp ( tLyrSize.cx - 40.f, 20.f ) );

		CLyrCoin::s_pLyrCoin = (CLyrCoin *) this->getChildByTag ( (KDuint) pCoin );
	}

	return KD_TRUE;
}

const KDchar* CDlgShopSkill::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_Shop3 );
}

const KDchar* CDlgShopSkill::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Shop_Skill );
}

CCPoint CDlgShopSkill::getPosTitle ( KDvoid )
{
    return ccp ( 3.f, -3.f );
}

KDvoid CDlgShopSkill::onSelect ( CCObject* pSender )
{
	CCNode*  pNode = (CCNode *) pSender;
	KDuint    uTag = pNode->getTag ( );

	if ( uTag != (KDuint) m_pSelect->getTag ( ) )
	{
		KDuint  uPrice = g_pEnvManager->getPriceSkill ( uTag );
		KDchar  szText [ 64 ];

		kdSprintfKHR 
		(
			szText, "x %s%s", 
			g_pEnvManager->convCoin ( uPrice ),
			g_pResManager->getText ( eTXT_Unit )
		);

		m_pCost->setString ( szText );
		m_pDesc->setString ( g_pResManager->getText ( eTXT_Skill_Desc1, uTag ) );

		m_pSelect->setVisible ( KD_TRUE );
		m_pSelect->setPosition ( ccpSub ( ccpAdd ( pNode->getPosition ( ), pNode->getParent ( )->getPosition ( ) ), ccp ( 3.f, 13.f ) ) );
		m_pSelect->setTag ( uTag );

		g_pResManager->playSound ( eSND_Etc_Focus );
	}
}

KDvoid CDlgShopSkill::onBuy ( CCObject* pSender )
{
	KDuint  uTag = m_pSelect->getTag ( );

	if ( uTag != KDUINT_MAX )
	{
		g_pResManager->playSound ( eSND_Etc_Click );

		KDuint  uCoin  = g_pEnvManager->getCoin ( );		
		KDuint  uPrice = g_pEnvManager->getPriceSkill ( uTag );

		if ( uCoin >= uPrice )
		{
			CDlgShop::s_uIndex = uTag;

			g_pScrManager->setDialog ( eDlgBuyRequest );
		}
		else
		{
			g_pScrManager->setDialog ( eDlgNotEnough );
		}
	}
}

//
//  "CDlgBuyRequest" dialog layer
//
KDbool CDlgBuyRequest::init ( KDvoid )
{
	if ( CDlgRequest::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );
    KDbool  bNetwork = CDlgShop::s_eDialog == eDlgShopCoin;

   	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Request_Buy ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pRequest );
        
		this->addChild ( pRequest );
        
		pRequest->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + ( bNetwork ? 67.f : 27.f ) ) );
	}

	if ( bNetwork )
	{
   		CCLabelTTF*  pNotice = CCLabelTTF::create 
		(
			g_pResManager->getText ( eTXT_Notice_Pay ),
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			16
		);
		{
			CC_ASSERT ( pNotice );
	        
			this->addChild ( pNotice );
	        
			pNotice->setColor ( ccYELLOW );
			pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 15.f ) );
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
			pNetwork->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f - 25.f ) );
		}  
	}

	return KD_TRUE;
}

const KDchar* CDlgBuyRequest::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Buy );
}

KDvoid CDlgBuyRequest::onYes ( CCObject* pSender )
{
	ENetError  eErr;
	EDialog    eDialog;

	CDlgRequest::onYes ( pSender );

	if ( CDlgShop::s_eDialog == eDlgShopCoin )
	{
		KDuint64  uPrice = g_pEnvManager->getPriceCoin ( CDlgShop::s_uIndex );

		eErr = g_pNetManager->puchase ( uPrice );
	}
	else
	{
		eErr = eNet_True;
	}

	switch ( eErr )
	{
		case eNet_True	:	eDialog = eDlgBuyDone;		break;
		case eNet_False :	eDialog = eDlgBuyFail;		break;
		default			:	eDialog = eDlgNetworkFail;	break;
	}

	g_pScrManager->setDialog ( eDialog );
}

//
//  "CDlgBuyDone" dialog layer
//
KDbool CDlgBuyDone::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );

	KDuint  uIndex = CDlgShop::s_uIndex;
	KDuint  uCash = g_pEnvManager->getCash ( );
	KDuint  uCoin = g_pEnvManager->getCoin ( );

	switch ( CDlgShop::s_eDialog )
	{
		case eDlgShopCoin :

			g_pEnvManager->setCoin ( uCoin + g_pEnvManager->getExchageCoin ( uIndex ) );

			break;

		case eDlgShopCash :

			g_pEnvManager->setCash ( uCash + g_pEnvManager->getExchageCash ( uIndex ) );
			g_pEnvManager->setCoin ( uCoin - g_pEnvManager->getPriceCash ( uIndex ) );

			if ( g_pScrManager->getSceneID ( ) == eScrGame )
			{
				CScrGame*  pScrGame = (CScrGame *) g_pScrManager->getScene ( );

				pScrGame->postEvent ( eType_RefreshCash );
				pScrGame->waitEvent ( );
			}

			break;

		case eDlgShopSkill : 

			g_pEnvManager->setCoin ( uCoin - g_pEnvManager->getPriceSkill ( uIndex ) );
			g_pEnvManager->setSkill ( uIndex, g_pEnvManager->getSkill ( uIndex ) + 1 );

			break;
            
        default : ;
	}

	g_pEnvManager->save ( );
	CLyrCoin::s_pLyrCoin->refresh ( );

   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_Buy_Done ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 27.f ) );
	}

	return KD_TRUE;
}

const KDchar* CDlgBuyDone::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Buy );
}

//
//  "CDlgBuyFail" dialog layer
//
KDbool CDlgBuyFail::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_Buy_Fail ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setColor ( ccYELLOW );
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 27.f ) );
	}

	return KD_TRUE;
}

const KDchar* CDlgBuyFail::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Buy );
}

//
//  "CDlgNotEnough" dialog layer
//
KDbool CDlgNotEnough::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

    const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_Not_Enough ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 27.f ) );
	}

	return KD_TRUE;
}

const KDchar* CDlgNotEnough::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Buy );
}
