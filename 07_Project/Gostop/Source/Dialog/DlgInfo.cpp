/*
 *
 *      File            DlgInfo.cpp
 *      Description     My information dialog.
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
#include "DlgInfo.h"

//
//  "CDlgInfo" dialog layer
//
KDuint CDlgInfo::getCount ( KDvoid )
{
	return 3;
}

const KDchar* CDlgInfo::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( m_uSelected == 0 ? eIMG_BG_Dlg_Info : eIMG_BG_Dlg_Multi2 );
}

const KDchar* CDlgInfo::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Info1, m_uSelected );
}

CCNode* CDlgInfo::getContent ( KDvoid )
{
	CCNode*  pContent = KD_NULL;

	switch ( m_uSelected )
	{
		case 0 : pContent = CLyrInfo ::create ( ); break;
		case 1 : pContent = CLyrTitle::create ( ); break;
		case 2 : pContent = CLyrSkill::create ( ); break;
	}

	return pContent;
}

KDvoid CDlgInfo::addContent ( KDvoid )
{
	CDlgMulti::addContent ( );

	CCPoint  tPoint;

	if ( m_uSelected == 0 )
	{
		tPoint.x = -10.f;
		tPoint.y =  10.f;
	}

	m_pBG->setPosition ( tPoint );
}

//
//  "CLyrInfo" sub layer of "CDlgInfo" layer.
//
KDvoid CLyrInfo::onEnter ( KDvoid )
{
	CCLayer::onEnter ( );

	const CCSize&  tLyrSize = this->getParent ( )->getContentSize ( );
	KDuint  uTitle = g_pEnvManager->getTitle ( );
	KDchar  szText [ 256 ];
	KDuint  i;

  	kdSprintfKHR 
	(
        szText,
        "%s %s", 
		g_pResManager->getText ( eTXT_Title_Name0, uTitle == KDUINT_MAX ? 0 : uTitle + 1 ),
		g_pResManager->getText ( eTXT_Name_KBM )
    ); 
	CCLabelTTF*  pName = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		24
	);
	{
		CC_ASSERT ( pName );

		this->addChild ( pName );

		pName->setColor ( ccYELLOW );
		pName->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy - 80.f ) );
	}

	kdSprintfKHR
	(
		szText, 
		"LV.%s ( %d%% )",
		g_pResManager->getText ( eTXT_Level_Name1, g_pEnvManager->getLevel ( ) ),
		100 * g_pEnvManager->getStar ( ) / 45
	); 
	CCLabelTTF*  pLevel = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		16
	);
	{
		CC_ASSERT ( pLevel );

		this->addChild ( pLevel );

		pLevel->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy - 117.f ) );
	}

	for ( i = 0; i < 6; i++ )
	{
		switch ( i )
		{
			case 0 :

				kdSprintfKHR 
				(
					szText, "%s %s",
					g_pEnvManager->convCoin ( g_pEnvManager->getCoin ( ) ),
					g_pResManager->getText ( eTXT_Unit )
				);

				break;

			case 1 :
				
				{
					KDuint  uWin   = g_pEnvManager->getWin  ( );
					KDuint  uLose  = g_pEnvManager->getLose ( );
					KDuint  uTotal = uWin + uLose;

					kdSprintfKHR
					(
						szText,
						"%d%% ( %d%s %d%s )", 
						uTotal == 0 ? 0 : 100 * uWin / uTotal,
						uWin,
						g_pResManager->getText ( eTXT_Win1 ),
						uLose, 
						g_pResManager->getText ( eTXT_Lose1 ) 
					); 
				}
						    
				break;

			case 2 : 

				kdSprintfKHR 
				(
					szText, "%d %s",
					g_pEnvManager->getWinComboMax ( ),
					g_pResManager->getText ( eTXT_Win1 )
				);

				break;
				
			case 3 :

				kdSprintfKHR 
				(
					szText, "%d %s",
					g_pEnvManager->getWinScoreMax ( ),
					g_pResManager->getText ( eTXT_Point )
				);

				break;

			case 4 :

				kdSprintfKHR 
				(
					szText, "%s %s",
					g_pEnvManager->convCash ( g_pEnvManager->getWinCashMax ( ) ),
					g_pResManager->getText ( eTXT_Won )
				);

				break;

			case 5 :

				kdSprintfKHR 
				(
					szText, "%s %s",
					g_pEnvManager->convCash ( g_pEnvManager->getCash ( ) ),
					g_pResManager->getText ( eTXT_Won )
				);

				break;
		}

		CCLabelTTF*  pInfo = CCLabelTTF::create 
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ), 16,
			CCSize ( 0, 35 ),
			kCCTextAlignmentRight
		);
		{
			CC_ASSERT ( pInfo );

			this->addChild ( pInfo );

			const CCSize&  tSize = pInfo->getContentSize ( );

			pInfo->ignoreAnchorPointForPosition ( KD_TRUE );
			pInfo->setPosition ( ccp ( tLyrSize.cx - tSize.cx - 30.f, tLyrSize.cy - ( tSize.cy + 3.f ) * i - 178.f ) );

			if ( i == 0 || i == 5 )
			{
				pInfo->setColor ( ccYELLOW );
			}
		}
	}
}

//
//  "CLyrTitle" sub layer of "CDlgInfo" layer.
//
KDvoid CLyrTitle::onEnter ( KDvoid )
{
	CCLayer::onEnter ( );

	const CCSize&  tLyrSize = this->getParent ( )->getContentSize ( );

	CCMenu*  pMenus = KD_NULL;
	KDuint   uCount = g_pEnvManager->getTitleMax ( );
	for ( KDuint  i = 0; i < COUNT_TITLE; i++ )
	{
		CCMenuItemImage*  pMenu = CCMenuItemImage::create 
		(
			g_pResManager->getPathSprite ( i < uCount ? eIMG_Sprite_Title_Menu_Nor : eIMG_Sprite_Title_Menu_Dis, i ), 
			KD_NULL,
			this,
			menu_selector ( CLyrTitle::onSelect ) 	
		);
		{
			CC_ASSERT ( pMenu );

			const CCSize&  tSize = pMenu->getContentSize ( );
			CCPoint  tPosition = i < ( COUNT_TITLE / 2 ) ?
				ccp ( ( tSize.cx + 5.f ) * i, tSize.cy + 5.f ) :
				ccp ( ( tSize.cx + 5.f ) * ( i % ( COUNT_TITLE / 2 ) ), 0 );

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

					pMenus->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
					pMenus->setPosition ( ccp ( 12.f, 194.f ) );

					this->addChild ( pMenus, 1 );
				}
			}
		}
	}

	CCSprite*  pSelect = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_ETC_Title_Menu_Border ) 
	);
	{
		CC_ASSERT ( pSelect );

		this->addChild ( pSelect, 2, (KDuint) pSelect );

		pSelect->setVisible ( KD_FALSE );
		pSelect->ignoreAnchorPointForPosition ( KD_TRUE );

		m_pSelect = this->getChildByTag ( (KDuint) pSelect );
		m_pSelect->setTag ( KDUINT_MAX );
	}

	CCLabelTTF*  pDesc = CCLabelTTF::create 
	(
		g_pResManager->getText ( eTXT_Select_Title ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		16
	);
	{
		CC_ASSERT ( pDesc );

		this->addChild ( pDesc, 0, (KDuint) pDesc );

		pDesc->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, + 127.f ) );

		m_pDesc = (CCLabelTTF *) this->getChildByTag ( (KDuint) pDesc );
	}
}

KDvoid CLyrTitle::onExit ( KDvoid )
{
	g_pEnvManager->save ( );

	CCLayer::onExit ( );
}

KDvoid CLyrTitle::onSelect ( CCObject* pSender )
{
	CCNode*  pNode = (CCNode *) pSender;
	KDuint    uTag = pNode->getTag ( );

	if ( uTag < g_pEnvManager->getTitleMax ( ) && uTag != (KDuint) m_pSelect->getTag ( ) )
	{
		g_pResManager->playSound ( eSND_Etc_Focus );
		g_pEnvManager->setTitle ( uTag );

		m_pDesc->setString ( g_pResManager->getText ( eTXT_Title_Desc1, uTag ) );

		m_pSelect->setVisible ( KD_TRUE );
		m_pSelect->setPosition ( ccpAdd ( pNode->getPosition ( ), pNode->getParent ( )->getPosition ( ) ) );
		m_pSelect->setTag ( uTag );
	}
}

//
//  "CLyrSkill" sub layer of "CDlgInfo" layer.
//
KDvoid CLyrSkill::onEnter ( KDvoid )
{
	CCLayer::onEnter ( );

	const CCSize&  tLyrSize = this->getParent ( )->getContentSize ( );
	KDbool  bHasSkill = KD_FALSE;
	KDuint  uSkill = 0;
	KDchar  szText [ 64 ];

	CCMenu*  pMenus = KD_NULL;
	for ( KDuint  i = 0; i < COUNT_SKILL; i++ )
	{
		CCMenuItemImage*  pMenu = CCMenuItemImage::create  
		(
			g_pResManager->getPathSprite ( eIMG_Sprite_Skill, i ), 
			KD_NULL,
			this,
			menu_selector ( CLyrSkill::onSelect ) 	
		);
		{
			CC_ASSERT ( pMenu );

			const CCSize&  tMenuSize = pMenu->getContentSize ( );
			CCPoint  tPosition = i < ( COUNT_SKILL / 2 ) ?
				ccp ( ( tMenuSize.cx + 15.f ) * i, tMenuSize.cy + 15.f ) :
				ccp ( ( tMenuSize.cx + 15.f ) * ( i % ( COUNT_SKILL / 2 ) ), 0 );

			pMenu->setTag ( i );
			pMenu->setPosition ( tPosition );
			pMenu->ignoreAnchorPointForPosition ( KD_TRUE );

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

				uSkill = g_pEnvManager->getSkill ( i );
				if ( uSkill )
				{
					bHasSkill = KD_TRUE;
				}

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

			if ( pMenus )
			{
				pMenus->addChild ( pMenu, i );
			}
			else
			{
				pMenus = CCMenu::createWithItem ( pMenu );
				{
					CC_ASSERT ( pMenus );

					pMenus->setPosition ( ccp ( 35.f, 222.f ) );
					pMenus->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );

					this->addChild ( pMenus, 1 );
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
		g_pResManager->getText ( bHasSkill ? eTXT_Select_Skill_Desc : eTXT_Select_Skill_None ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ),
		16
	);
	{
		CC_ASSERT ( pDesc );

		this->addChild ( pDesc, 0, (KDuint) pDesc );

		pDesc->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, + 127.f ) );

		m_pDesc = (CCLabelTTF *) this->getChildByTag ( (KDuint) pDesc );
	}
}

KDvoid CLyrSkill::onSelect ( CCObject* pSender )
{
	CCNode*  pNode = (CCNode *) pSender;
	KDuint    uTag = pNode->getTag ( );

	if ( uTag != (KDuint) m_pSelect->getTag ( ) )
	{
		m_pDesc->setString ( g_pResManager->getText ( eTXT_Skill_Desc1, uTag ) );

		m_pSelect->setVisible ( KD_TRUE );
		m_pSelect->setPosition ( ccpSub ( ccpAdd ( pNode->getPosition ( ), pNode->getParent ( )->getPosition ( ) ), ccp ( 3.f, 13.f ) ) );
		m_pSelect->setTag ( uTag );

		g_pResManager->playSound ( eSND_Etc_Focus );
	}
}
