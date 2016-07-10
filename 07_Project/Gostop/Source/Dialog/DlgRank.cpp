/*
 *
 *      File            DlgRank.cpp
 *      Description     Rank dialog.
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
#include "DlgRank.h"

//
//  "CDlgRank" dialog layer
//
KDbool CDlgRank::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	KDchar  szText [ 64 ];
	
	CCLabelTTF*  pName = CCLabelTTF::create 
	(
		g_pEnvManager->getName ( ),
		g_pResManager->getPath ( eFNT_Gothic_Bold ), 24,
		CCSize ( 0, 0 ),
		kCCTextAlignmentRight
	);
	{
		CC_ASSERT ( pName );

		this->addChild ( pName );

		const CCSize&  tSize = pName->getContentSize ( );

		pName->setColor ( ccYELLOW );
		pName->ignoreAnchorPointForPosition ( KD_TRUE );
		pName->setPosition ( ccp ( tLyrSize.cx / 2.f - tSize.cx + 10.f, tLyrSize.cy - 125.f ) );
	}

	kdSprintfKHR 
	(
		szText,
		"%s ( %d%s %d%s )", 
		g_pResManager->getText ( eTXT_Nim ),
		g_pEnvManager->getWin ( ),
		g_pResManager->getText ( eTXT_Win1 ),
		g_pEnvManager->getLose ( ), 
		g_pResManager->getText ( eTXT_Lose1 ) 
	); 

	CCLabelTTF*  pWinning = CCLabelTTF::create 
	(
		szText,
		g_pResManager->getPath ( eFNT_Gothic_Bold ), 14,
		CCSize ( 0, 0 ),
		kCCTextAlignmentLeft
	);
	{
		CC_ASSERT ( pWinning );

		this->addChild ( pWinning );

		pWinning->ignoreAnchorPointForPosition ( KD_TRUE );
		pWinning->setPosition ( ccp ( tLyrSize.cx / 2.f + 10.f, tLyrSize.cy - 125.f ) );
	}

	CCSprite*  pListBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_BG_List_Rank )
	);
	{
		CC_ASSERT ( pListBG );

		this->addChild ( pListBG );
		
		const CCSize&  tInfoSize = pListBG->getContentSize ( );

		pListBG->setPosition ( ccp ( tLyrSize.cx / 2.f - 10.f, tLyrSize.cy / 2.f  ) );

		for ( KDuint  i = 0; i < 3; i++ )
		{
			switch ( i )
			{
				case 0 : 

					kdStrcpy ( szText, g_pEnvManager->convCash ( g_pEnvManager->getCash ( ) ) );

					break;

				case 1 :

					kdStrcpy ( szText, g_pEnvManager->convCash ( g_pEnvManager->getWinCashMax ( ) ) );

					break;

				case 2 : 
					
					kdSprintfKHR ( szText, "%d", g_pEnvManager->getWinScoreMax ( ) );
					
					break;
			}

			CCLabelTTF*  pInfo = CCLabelTTF::create 
			(
				szText,
				g_pResManager->getPath ( eFNT_Gothic_Bold ), 18,
				CCSize ( 0, 40 ),
				kCCTextAlignmentRight
			);
			{
				CC_ASSERT ( pInfo );

				pListBG->addChild ( pInfo );

				const CCSize&  tSize = pInfo->getContentSize ( );

				pInfo->ignoreAnchorPointForPosition ( KD_TRUE );
				pInfo->setPosition ( ccp ( tInfoSize.cx - tSize.cx - 30.f, tInfoSize.cy - ( tSize.cy + 9.f ) * i - 44.f ) );
			}
		}
	}

	CCMenuItemImage*  pConnect = CCMenuItemImage::create 
	(
		g_pResManager->getPath ( eIMG_BTN_Connect_Nor ), 
		g_pResManager->getPath ( eIMG_BTN_Connect_Sel ),
		this,
		menu_selector ( CDlgRank::onConnect ) 
	);
	CC_ASSERT ( pConnect );

	CCMenu*  pMenu = CCMenu::createWithItem ( pConnect );
	{
		CC_ASSERT ( pMenu );

		this->addChild ( pMenu );

		const CCSize&  tSize = pConnect->getContentSize ( );

		pMenu->setPosition ( ccp ( tLyrSize.cx / 2.f - 8.f, tSize.cy - 3.f ) );
		pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
	}

	return KD_TRUE;
}

const KDchar* CDlgRank::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_Rank );
}

const KDchar* CDlgRank::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Rank );
}

CCPoint CDlgRank::getPosTitle ( KDvoid )
{
    return ccp ( 3.f, -3.f );
}

KDvoid CDlgRank::onConnect ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );
	g_pScrManager->setDialog ( eDlgRegister );
}

//
//  "CDlgRankList" dialog layer
//
KDbool CDlgRankList::init ( KDvoid )
{
	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}
	
	const CCSize&  tLyrSize = this->getContentSize ( );
	CCPoint  tPoint;
	CCSize   tSize;

	KDchar   szText [ 64 ];
	KDuint   i, j, uIndex;

	CCMenuItemImage*  pTabs [ 3 ];
	for ( i = 0; i < 3; i++ )
	{
		std::string  sPath [ 2 ];
	
		sPath [ 0 ] = g_pResManager->getPathSprite ( eIMG_Sprite_Rank_Tab_Nor, i );
		sPath [ 1 ] = g_pResManager->getPathSprite ( eIMG_Sprite_Rank_Tab_Sel, i );

		CCMenuItemImage*  pTab = CCMenuItemImage::create  
		(
			sPath [ 0 ].c_str ( ), 
			sPath [ 0 ].c_str ( ), 
			sPath [ 1 ].c_str ( )
		);
		{
			CC_ASSERT ( pTab );

			const CCSize&  tSize = pTab->getContentSize ( );

			pTab->ignoreAnchorPointForPosition ( KD_TRUE );
			pTab->setPosition ( ccp ( ( tSize.cx + 4 ) * i + 9.f, tLyrSize.cy - tSize.cy - 53.f ) );	

			pTabs [ i ] = pTab;
		}
	}

	CCMenuRadio*  pRadio = CCMenuRadio::create 
	(
		this, 
		menu_selector ( CDlgRankList::onTab ), 
		pTabs [ 0 ], pTabs [ 1 ], pTabs [ 2 ],
		KD_NULL
	);
	{
		CC_ASSERT ( pRadio );

		this->addChild ( pRadio );

		pRadio->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
	}

	CCLayer*  pLayer = CCLayer::create ( );
	{
		CC_ASSERT ( pLayer );

		for ( i = 0; i < RANK_LIST_COUNT; i++ )
		{
			uIndex = RANK_LIST_COUNT - i;

			tPoint.x = 0;
			tPoint.y = i * 30.f;

			tSize.cx = 0;
			tSize.cy = 30;
			
			for ( j = 0; j < 3; j++ )
			{
				tPoint.x += tSize.cx; 

				switch ( j )
				{
					case 0 :
						tSize.cx = 50.f; 
						kdSprintfKHR ( szText, "%d", uIndex );
						break;

					case 1 :
						tSize.cx = 140.f; 
						kdSprintfKHR ( szText, "%s%d", g_pResManager->getText ( eTXT_Name_KBM ), uIndex );
						break;

					case 2 :
						tSize.cx = 215.f; 
						kdSprintfKHR ( szText, "%s%s", g_pEnvManager->convCash ( 999999999 ), g_pResManager->getText ( eTXT_Won ) );
						break;
				}

				CCLabelTTF*  pLabel = CCLabelTTF::create 
				(
					szText,
					g_pResManager->getPath ( eFNT_Gothic_Bold ), 16,
					tSize,
					j == 2 ? kCCTextAlignmentRight : kCCTextAlignmentLeft
				);
				{
					CC_ASSERT ( pLabel );

					pLayer->addChild ( pLabel, 0, (KDuint) pLabel );

					pLabel->setPosition ( tPoint ); 
					pLabel->ignoreAnchorPointForPosition ( KD_TRUE );

					if ( j > 0 )
					{
						m_vList [ j - 1 ].push_front ( (CCLabelTTF *) pLayer->getChildByTag ( (KDuint) pLabel ) );
					}
				}
			}
		}

		tSize.cx = tPoint.x + tSize.cx;
		tSize.cy = tPoint.y + tSize.cy;

		pLayer->ignoreAnchorPointForPosition ( KD_FALSE );
		pLayer->setAnchorPoint ( ccp ( 0, 0.5f ) );
		pLayer->setContentSize ( tSize );
/*
		CScroll*  pScroll = CScroll::create
		(
			kCCScroll_Vertical, pLayer
		);
		{
			CC_ASSERT ( pScroll );

			this->addChild ( pScroll );

			pScroll->setContentSize ( CCSize ( tSize.cx, 210.f ) );
			pScroll->setPosition ( ccp ( 13.f, 103.f ) );
			pScroll->setIsClipping ( KD_TRUE );
			pScroll->setTargetPosition ( ccp ( 0, 210.f - tSize.cy / 2.f ) );	
		}
*/
	}

	tPoint.x = 13.f;
	tPoint.y = 33.f;

	tSize.cx = 0;
	tSize.cy = 30;
	for ( i = 0; i < 2; i++ )
	{
		tPoint.x += tSize.cx; 

		switch ( i )
		{
			case 0 :
				tSize.cx = 210.f; 
				kdSprintfKHR ( szText, "%d %s", 9999999, g_pEnvManager->getName ( ) );
				break;

			case 1 :
				tSize.cx = 195.f; 
				kdSprintfKHR ( szText, "%s%s", g_pEnvManager->convCash ( 999999999 ), g_pResManager->getText ( eTXT_Won ) );
				break;
		}

		CCLabelTTF*  pLabel = CCLabelTTF::create 
		(
			szText,
			g_pResManager->getPath ( eFNT_Gothic_Bold ), 16,
			tSize,
			i == 1 ? kCCTextAlignmentRight : kCCTextAlignmentLeft
		);
		{
			CC_ASSERT ( pLabel );

			this->addChild ( pLabel, 0, (KDuint) pLabel );

			pLabel->setColor ( ccYELLOW );
			pLabel->setPosition ( tPoint ); 
			pLabel->ignoreAnchorPointForPosition ( KD_TRUE );

			m_pMyInfo [ i ] = (CCLabelTTF *) this->getChildByTag ( (KDuint) pLabel );
		}
	}

	return KD_TRUE;
}

KDvoid CDlgRankList::onEnterTransitionDidFinish ( KDvoid )
{
	CDialog::onEnterTransitionDidFinish ( );

	this->runAction 
	(
        (CCActionInterval *) CCSequence::create
		(
			CCDelayTime::create ( 0 ),
			CCCallFunc::create ( this, callfunc_selector ( CDlgRankList::onRefresh ) ),
			KD_NULL
		)
	);
}

const KDchar* CDlgRankList::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_List );
}

const KDchar* CDlgRankList::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Rank2 );
}

KDvoid CDlgRankList::onTab ( CCObject* pSender )
{
	CCMenuRadio*  pRadio = (CCMenuRadio *) pSender;

	g_pResManager->playSound ( eSND_Etc_Focus );
	this->refresh ( pRadio->getSelectedIndex ( ) );
}

KDvoid CDlgRankList::onRefresh ( KDvoid )
{
	this->refresh ( 0 );
}

KDvoid CDlgRankList::refresh ( KDuint uIndex )
{
	return;

	ENetError  eErr;
	SRankList  tRankList;
	KDchar     szText [ 64 ];
	KDuint     i;
	
	eErr = g_pNetManager->getRankList ( &tRankList );

	if ( eErr != eNet_True )
	{
        g_pScrManager->setDialog ( eDlgNetworkFail );
        
		for ( i = 0; i < RANK_LIST_COUNT; i++ )
		{
			kdSprintfKHR ( tRankList.szName [ i ], "%s%d", g_pResManager->getText ( eTXT_Name_KBM ), i + 1 );
			tRankList.uNumber [ i ] = 999999999;
		}

		tRankList.uMyOrder = 9999999;
	}

	for ( i = 0; i < RANK_LIST_COUNT; i++ )
	{
		m_vList [ 0 ].at ( i )->setString ( tRankList.szName [ i ] );

		if ( uIndex == 2 )
		{
			kdSprintfKHR 
			(
				szText, "%d%s",
				tRankList.uNumber [ i ], 
				g_pResManager->getText ( eTXT_Point ) 
			);
		}
		else
		{
			kdSprintfKHR 
			(
				szText, "%s%s", 
				g_pEnvManager->convCash ( tRankList.uNumber [ i ] / 100 ),
				g_pResManager->getText ( eTXT_Won )
			);
		}

		m_vList [ 1 ].at ( i )->setString ( szText );
	}

	kdSprintfKHR 
	(
		szText, "%d %s",
		tRankList.uMyOrder,
		g_pEnvManager->getName ( ) 
	);
	m_pMyInfo [ 0 ]->setString ( szText );

	switch ( uIndex )
	{
		case 0 :

			kdSprintfKHR 
			( 
				szText, "%s%s", 
				g_pEnvManager->convCash ( g_pEnvManager->getCash ( ) ),
				g_pResManager->getText ( eTXT_Won )
			);

			break;

		case 1 : 
			
			kdSprintfKHR 
			(
				szText, "%s%s",
				g_pEnvManager->convCash ( g_pEnvManager->getWinCashMax ( ) ),
				g_pResManager->getText ( eTXT_Won )
			);

			break;

		case 2 :
			
			kdSprintfKHR 
			(
				szText, "%d%s",
				g_pEnvManager->getWinScoreMax ( ),
				g_pResManager->getText ( eTXT_Point )
			);

			break;
	}

	m_pMyInfo [ 1 ]->setString ( szText );
}


//
//  "CDlgRegister" dialog layer
//
KDbool CDlgRegister::init ( KDvoid )
{
	if ( CDlgRequest::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Request_Rank ),
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

const KDchar* CDlgRegister::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Rank );
}

KDvoid CDlgRegister::onYes ( CCObject* pSender )
{
	CDlgRequest::onYes ( pSender );
	
	( (CDialog *) g_pScrManager->getTopDialog ( ) )->hide ( );

	g_pScrManager->setDialog ( eDlgNickName );
}

//
//  "CDlgNickName" dialog layer
//
KDbool CDlgNickName::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_NickName ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 80.f ) );
	}   

  	CCLabelTTF*  pNotice2 = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_Input_Limit ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        14
    );
	{
		CC_ASSERT ( pNotice2 );
        
		this->addChild ( pNotice2 );
        
        pNotice2->setColor ( ccGRAY );
		pNotice2->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 47.f ) );
	}    

	CCSprite*  pInput = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_ETC_Text_Input )
	);
	{
		CC_ASSERT ( pInput );

		this->addChild ( pInput );

		const CCSize&  tSize = pInput->getContentSize ( );

		pInput->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f - 5.f ) );

		CCTextFieldTTF*  pTextField = CCTextFieldTTF::create
		(
			g_pResManager->getText ( eTXT_Notice_NickName ), 
			tSize,
			kCCTextAlignmentCenter,
			g_pResManager->getPath ( eFNT_Gothic_Bold ),
			18
		);
		{
			CC_ASSERT ( pTextField );

			this->addChild ( pTextField, 0, (KDuint) pTextField );
			m_pTextField = (CCTextFieldTTF *) this->getChildByTag ( (KDuint) pTextField );

			pTextField->setColorSpaceHolder ( ccGRAY );
			pTextField->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f - 5.f ) );
			pTextField->setString ( g_pEnvManager->getName ( ) );
			pTextField->setDelegate ( this );
			pTextField->attachWithIME ( );
		}
	}

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CDlgNickName::onExit ( KDvoid )
{
    m_pTextField->detachWithIME ( );
    
    CDlgConfirm::onExit ( );
}

const KDchar* CDlgNickName::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_NickName );
}

KDvoid CDlgNickName::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, g_pScrManager->getTopDialogPrioty ( ), KD_TRUE );
}

KDbool CDlgNickName::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCRect  tRect;
	
	tRect.size = m_pTextField->getContentSize ( );

	if ( tRect.containsPoint ( m_pTextField->convertToNodeSpace ( pTouch->getLocation ( ) ) ) )
	{
		m_pTextField->attachWithIME ( );

		return KD_TRUE;
	}
	else
	{
		m_pTextField->detachWithIME ( );
	}

	return KD_FALSE;
}

KDbool CDlgNickName::onTextFieldAttachWithIME ( CCTextFieldTTF* pSender )
{
#if defined ( __APPLE__ ) ||  defined ( ANDROID )
    
    const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
    
    this->setPosition ( ccp ( tWinSize.cx / 2.f, tWinSize.cy * 0.75f ) );
    
#endif
    
	m_pTextField->stopAllActions ( );
	m_pTextField->runAction 
	(
		CCRepeatForever::create
		(
			(CCActionInterval *) CCSequence::create
			(
				CCFadeOut::create ( 0.25f ),
				CCFadeIn ::create ( 0.25f ),
				KD_NULL
			)
		)
	);

	return KD_FALSE;
}

KDbool CDlgNickName::onTextFieldDetachWithIME ( CCTextFieldTTF* pSender )
{
#if defined ( __APPLE__ ) ||  defined ( ANDROID )
    
    const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
    
    this->setPosition ( ccpMid ( tWinSize ) );
    
#endif
    
	m_pTextField->stopAllActions ( );
	m_pTextField->setOpacity ( 255 );

	return KD_FALSE;
}

KDbool CDlgNickName::onTextFieldInsertText ( CCTextFieldTTF* pSender, const KDchar* text, KDsize nLen )
{
    if ( '\n' == *text )
    {
        return KD_FALSE;
    }

	if ( pSender->getCharCount ( ) < 5 )
	{
		return KD_FALSE;
	}

	return KD_TRUE;
}

KDbool CDlgNickName::onTextFieldDeleteBackward ( CCTextFieldTTF* pSender, const KDchar* text, KDsize nLen )
{
	return KD_FALSE;
}

KDvoid CDlgNickName::onConfirm ( CCObject* pSender )
{
	const KDchar*  szNickName = m_pTextField->getString ( );
	ENetError      eErr;

	g_pResManager->playSound ( eSND_Etc_Click );

	if ( kdStrlen ( szNickName ) )
	{
		g_pEnvManager->setName ( szNickName );

		eErr = g_pNetManager->isUsedNickName ( szNickName );
		if ( eErr == eNet_False )
		{
			KDuint64  uCash     = (KDuint64) g_pEnvManager->getCash        ( ) * 100UL;
			KDuint64  uCashMax  = (KDuint64) g_pEnvManager->getWinCashMax  ( ) * 100UL;
			KDuint64  uScoreMax = (KDuint64) g_pEnvManager->getWinScoreMax ( );

			eErr = g_pNetManager->registerRank ( szNickName, uCash, uCashMax, uScoreMax );

			if ( eErr == eNet_True )
			{
				this->hide ( );

				g_pScrManager->setDialog ( eDlgRankList );
			}
			else
			{
				g_pScrManager->setDialog ( eDlgNetworkFail );
			}
		}
		else if ( eErr == eNet_True )
		{
			g_pScrManager->setDialog ( eDlgNickName2 );
		}
		else
		{
			g_pScrManager->setDialog ( eDlgNetworkFail );
		}
	}
}

//
//  "CDlgNickName2" dialog layer
//
KDbool CDlgNickName2::init ( KDvoid )
{
	if ( CDlgConfirm::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
	KDchar  szText [ 64 ];

	kdSprintfKHR ( szText, "[ %s ]", g_pEnvManager->getName ( ) );

   	CCLabelTTF*  pNickName = CCLabelTTF::create 
	(
        szText,
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        24
    );
	{
		CC_ASSERT ( pNickName );
        
		this->addChild ( pNickName );
        
		pNickName->setColor ( ccYELLOW );
		pNickName->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy / 2.f + 60.f ) );
	} 

   	CCLabelTTF*  pNotice = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Notice_NickName2 ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pNotice );
        
		this->addChild ( pNotice );
        
		pNotice->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f ) );
	}   

	return KD_TRUE;
}

const KDchar* CDlgNickName2::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_NickName );
}

