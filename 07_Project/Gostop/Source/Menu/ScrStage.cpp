/*
 *
 *      File            ScrMain.cpp
 *      Description     Menu scene.
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
#include "Dialog/DlgNotice.h"
#include "ScrStage.h"

#define CNT_STAGE						9
#define TAG_SELECT1						1011
#define TAG_SELECT2						1012

//
//  Main layer of "CScrStage" Scene.
//
KDbool CScrStage::init ( KDvoid )
{
	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDuint  i, j;

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_BG_Stage )
	);
	{
		CC_ASSERT ( pBG );

		this->addChild ( pBG );
		
		pBG->setPosition ( ccpMid ( tWinSize ) );
	}

	CCMenuItemImage*  pShop = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Shop_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Shop_Sel ),
		this,
		menu_selector ( CScrStage::onShop ) 
	);
	{
		CC_ASSERT ( pShop );

		this->addChild ( CCMenu::createWithItem ( pShop ) ); 

		const CCSize&  tSize = pShop->getContentSize ( );

		pShop->ignoreAnchorPointForPosition ( KD_TRUE );
		pShop->setPosition ( ccp ( tWinSize.cx - tSize.cx - 39.f, tWinSize.cy - tSize.cy - 40.f ) );
	} 

	CCMenuItemImage*  pInfo = CCMenuItemImage::create  
	(
		g_pResManager->getPath ( eIMG_BTN_Info_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Info_Sel ),
		this,
		menu_selector ( CScrStage::onInfo ) 
	);
	{
		CC_ASSERT ( pInfo );

		this->addChild ( CCMenu::createWithItem ( pInfo ) ); 

		const CCSize&  tSize = pInfo->getContentSize ( );
		const CCPoint& tPosition = pShop->getPosition ( );

		pInfo->ignoreAnchorPointForPosition ( KD_TRUE );
		pInfo->setPosition ( ccp ( tPosition.x - tSize.cx, tPosition.y ) );
	} 

	CCMenuItemImage*  pMain = CCMenuItemImage::create  
	(
		g_pResManager->getPath ( eIMG_BTN_Main_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Main_Sel ),
		this,
		menu_selector ( CScrStage::onMain ) 
	);
	{
		CC_ASSERT ( pMain );

		this->addChild ( CCMenu::createWithItem ( pMain ) ); 

		const CCSize&  tSize = pMain->getContentSize ( );

		pMain->setPosition ( ccp ( tSize.cx / 2.f + 5.f, tSize.cy / 2.f + 5.f ) );
	} 

	KDuint   uCount  = CNT_STAGE;
	KDuint   uEnable = g_pEnvManager->getStageMax ( );

	CCMenu*  pMenus  = KD_NULL;
	for ( i = 0; i < uCount; i++ )
	{
		CCMenuItemImage*  pMenu = CCMenuItemImage::create  
		(
			g_pResManager->getPathSprite ( i < uEnable ? eIMG_Sprite_Stage_Menu_Nor : eIMG_Sprite_Stage_Menu_Dis, i ), 
			KD_NULL,
			this,
			menu_selector ( CScrStage::onSelect ) 	
		);
		{
			CC_ASSERT ( pMenu );

			const CCSize&  tSize = pMenu->getContentSize ( );
			CCPoint  tPosition;

			if ( i < ( uCount / 2 ) )
			{
				tPosition = ccp ( tSize.cx * i, tSize.cy );
			}
			else if ( i < ( uCount - 1 ) )
			{
				tPosition = ccp ( tSize.cx * ( i % ( uCount / 2 ) ), 0 );
			}
			else
			{
				tPosition = ccp ( tSize.cx * ( uCount / 2 ), 1.f );
			}

			pMenu->setTag ( i );
			pMenu->setPosition ( tPosition );
			pMenu->ignoreAnchorPointForPosition ( KD_TRUE );

			if ( i < uEnable )
			{
				KDuint  uStar = g_pEnvManager->getStar ( i );

				for ( j = 0; j < 5; j++ )
				{
					EPath  ePath;

					if ( i == ( uEnable - 1 ) )
					{
						ePath = eIMG_ETC_Star2;
					}
					else
					{
						ePath = j < uStar ? eIMG_ETC_Star1 : eIMG_ETC_Star2;
					}

					CCSprite*  pStar = CCSprite::create 	
					(
						g_pResManager->getPath ( ePath )
					);
					{
						CC_ASSERT ( pStar );

						pMenu->addChild ( pStar );

						pStar->ignoreAnchorPointForPosition ( KD_TRUE );
						pStar->setPosition ( ccp ( j * 25.f + 10.f, 18.f ) );
					}
				}
			}

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

					pMenus->setPosition ( ccp ( 43.f, 85.f ) );
				}
			}
		}
	}

	CCSprite*  pSelect1 = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_ETC_Stage_Menu_Border1 ) 
	);
	{
		CC_ASSERT ( pSelect1 );

		this->addChild ( pSelect1, 2, TAG_SELECT1 );

		pSelect1->setVisible ( KD_FALSE );
		pSelect1->ignoreAnchorPointForPosition ( KD_TRUE );
	}

	CCSprite*  pSelect2 = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_ETC_Stage_Menu_Border2 ) 
	);
	{
		CC_ASSERT ( pSelect2 );

		this->addChild ( pSelect2, 2, TAG_SELECT2 );

		pSelect2->setVisible ( KD_FALSE );
		pSelect2->ignoreAnchorPointForPosition ( KD_TRUE );
	}

	g_pResManager->playBGM ( eSND_BGM_Stage );

	return KD_TRUE;
}

KDvoid CScrStage::onEnterTransitionDidFinish ( KDvoid )
{
	CCLayer::onEnterTransitionDidFinish ( );

	this->runAction 
	(
		(CCActionInterval *) CCSequence::create 
		(
			CCDelayTime::create ( 0.1f ),
			CCCallFunc::create ( this, callfunc_selector ( CScrStage::onCheck ) ),
			KD_NULL
		)
	);
}

KDvoid CScrStage::onInfo ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	g_pScrManager->setDialog ( eDlgInfo );
}

KDvoid CScrStage::onShop ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	g_pScrManager->setDialog ( eDlgShop );
}

KDvoid CScrStage::onMain ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	g_pScrManager->setScene ( eScrMain, eTransSlideInL );
}

KDvoid CScrStage::onSelect ( CCObject* pSender )
{
	CCNode*    pNode    = (CCNode *) pSender;
	CCSprite*  pSelect1 = (CCSprite *) this->getChildByTag ( TAG_SELECT1 );
	CCSprite*  pSelect2 = (CCSprite *) this->getChildByTag ( TAG_SELECT2 );

	KDuint  uCash = g_pEnvManager->getCash ( );
	KDuint   uTag = pNode->getTag ( );

	if ( uTag < g_pEnvManager->getStageMax ( ) )
	{
		if ( uTag == g_pEnvManager->getStage ( ) )
		{
			g_pScrManager->setScene ( eScrCartoon, eTransSlideInR );
		}
		else if ( uCash < ( uTag == 0 ? 1 : g_pEnvManager->getEnemyCashMax ( uTag - 1 ) ) )
		{
			g_pScrManager->showTalk ( eTALK_Limit );
		}
		else
		{
			g_pResManager->playSound ( eSND_Etc_Focus );
			g_pEnvManager->setStage ( uTag );

			if ( uTag < (KDint) ( CNT_STAGE - 1 ) )
			{
				pSelect1->setVisible ( KD_TRUE );
				pSelect2->setVisible ( KD_FALSE );

				pSelect1->setPosition ( ccpAdd ( ccpAdd ( pNode->getPosition ( ), pNode->getParent ( )->getPosition ( ) ), ccp ( 1.f, 7.f ) ) );
			}
			else
			{
				pSelect1->setVisible ( KD_FALSE );
				pSelect2->setVisible ( KD_TRUE );

				pSelect2->setPosition ( ccpAdd ( ccpAdd ( pNode->getPosition ( ), pNode->getParent ( )->getPosition ( ) ), ccp ( 2.f, 7.f ) ) );
			}
		}
	}
	else
	{
        g_pScrManager->showTalk ( eTALK_Slow );
	}
}

KDvoid CScrStage::onCheck ( KDvoid )
{
	if ( g_pEnvManager->getCash ( ) == 0 )
	{
		g_pScrManager->setDialog ( eDlgBankrupt );
	}
	else
	{
		KDuint  uCount = g_pEnvManager->getStageMax ( ) - 1;
		KDuint  i;

		if ( uCount > 0 )
		{
			if ( g_pEnvManager->getStar ( ) / 5 > g_pEnvManager->getLevel ( ) )
			{
				g_pScrManager->setDialog ( eDlgLevelUp );
			}
			else
			{
				for ( i = 0; i < uCount; i++ )
				{
					if ( g_pEnvManager->getStar ( i ) == 5 && !g_pEnvManager->getReward ( i ) )
					{
						CDlgReward::s_uIndex = i;

						g_pScrManager->setDialog ( eDlgReward );

						break;
					}
				}
			}
		}
	}
}