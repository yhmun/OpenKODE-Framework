/*
 *
 *      File            ScrGame.cpp
 *      Description     Game scene.
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
#include "Dialog/DlgGame.h"
#include "ScrGame.h"

#define INTERVAL_BG			0.5f
#define INTERVAL_SHINE		0.3f

#define TAG_TRANSPARENCY	1000
#define TAG_MENU_LAYER		1001

//
//  Main layer of "CScrGame" Scene.
//
KDbool CScrGame::init ( KDvoid )
{
	m_uRound				= 0;
	m_bMission				= KD_TRUE;
	m_uMissionTimes			= 0;
	m_tWinSize				= CCDirector::sharedDirector ( )->getWinSize ( );
	m_uTurn					= g_pEnvManager->getFirstTurn ( );
	m_pSelect				= KD_NULL;
	m_uObtainGround [ 0 ]	= KDUINT_MAX;
	m_uObtainGround [ 1 ]	= KDUINT_MAX;
	m_uObtainSteal			= 0;

	m_lObtainable.clear ( );

	kdMemset ( m_tScore, 0, sizeof ( m_tScore ) );
	
	m_uSkillAvailable [ 0 ] = 3;
	m_uSkillAvailable [ 1 ] = 2;
	m_uSkillAvailable [ 2 ] = 1;
	m_uSkillAvailable [ 3 ] = 1;
	m_uSkillAvailable [ 4 ] = 1;
	m_uSkillAvailable [ 5 ] = 3;
	m_uSkillAvailable [ 6 ] = 3;
	m_uSkillAvailable [ 7 ] = 3;
	m_uSkillAvailable [ 8 ] = 1;
	m_uSkillAvailable [ 9 ] = 1;

	m_uGageCondition = 2;
	CDlgGage::s_fTiming = 1.f;

	if ( CCLayer::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDuint  i, j;

	CCSprite*  pBG = CCSprite::create
	(
		g_pResManager->getPath ( eIMG_BG_Game )
	);
	{
		CC_ASSERT ( pBG );

		this->addChild ( pBG );
		
		pBG->setPosition ( ccpMid ( tWinSize ) );
	}

	CCMenu*  pMenus = KD_NULL;
	for ( i = 0; i < 2; i++ )
	{
		CCMenuItemImage*  pMenu = CCMenuItemImage::create 
		(
			g_pResManager->getPath ( i == 0 ? eIMG_BTN_Game_Menu1 : eIMG_BTN_Game_Skill1 ), 
			KD_NULL,
			this,
			menu_selector ( CScrGame::onMenu ) 
		);
		{
			CC_ASSERT ( pMenu );

			pMenu->setTag ( i );
			pMenu->ignoreAnchorPointForPosition ( KD_TRUE );
			pMenu->setPosition ( ccp ( 0, i == 0 ? 90.f : -5.f ) );
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
		}
	}

	CCSprite*  pBet = CCSprite::create
	(
		g_pResManager->getPathSprite ( eIMG_Sprite_Bet, g_pEnvManager->getStage ( ) )
	);
	{
		CC_ASSERT ( pBet );

		this->addChild ( pBet );
		
		pBet->ignoreAnchorPointForPosition ( KD_TRUE );
		pBet->setPosition ( ccp ( 35.f, 312.f ) );
	}

	CCSprite*  pFirstTurn = CCSprite::create
	(
		g_pResManager->getPath ( m_uTurn == 0 ? eIMG_ETC_First_Player1 : eIMG_ETC_First_Player2 )
	);
	{
		CC_ASSERT ( pFirstTurn );

		this->addChild ( pFirstTurn );
		
		const CCSize&  tSize = pFirstTurn->getContentSize ( );

		pFirstTurn->ignoreAnchorPointForPosition ( KD_TRUE );
		pFirstTurn->setPosition ( ccp ( m_tWinSize.cx - tSize.cx - 35.f, 262.f ) );
	}

	CCPoint  tPosition;
	for ( i = 0; i < 2; i++ )
	{
		for ( j = 0; j < 4; j++ )
		{
			CText*  pObtain = CText::create ( "00", eIMG_Font_A );
			{
				CC_ASSERT ( pObtain );

				tPosition = i == 0 ? ccp ( 124.f, 162.f ) : ccp ( 29.f, 406.f );

				switch ( j )
				{
					case 0 : tPosition.x +=   0.f; break;
					case 1 : tPosition.x += 105.f; break;
					case 2 : tPosition.x += 255.f; break;
					case 3 : tPosition.x += 415.f; break;
				}

				this->addChild ( pObtain, 20, (KDuint) pObtain );

				pObtain->setPosition ( tPosition );

				m_pTxtObtain [ i ][ j ] = (CText *) this->getChildByTag ( (KDuint) pObtain );
			}
		}

		for ( j = 0; j < 3; j++ )
		{
			CText*  pCount = CText::create ( "0", eIMG_Font_A );
			{
				CC_ASSERT ( pCount );

				tPosition.y = i == 0 ? 183.f : 387.f;

				switch ( j )
				{
					case 0 : tPosition.x = 301.f; break;
					case 1 : tPosition.x = 338.f; break;
					case 2 : tPosition.x = 375.f; break;
				}

				this->addChild ( pCount, 0, (KDuint) pCount );

				pCount->setPosition ( tPosition );

				m_pTxtCount [ i ][ j ] = (CText *) this->getChildByTag ( (KDuint) pCount );
			}
		}

		CText*  pScore = CText::create ( "00,000", eIMG_Font_A );
		{
			CC_ASSERT ( pScore );

			tPosition = ccp ( 470.f, i == 0 ? 183.f : 387.f );

			this->addChild ( pScore, 0, (KDuint) pScore );

			pScore->setPosition ( tPosition );

			m_pTxtScore [ i ] = (CText *) this->getChildByTag ( (KDuint) pScore );
		}

		CText*  pTimes = CText::create ( "001", eIMG_Font_A );
		{
			CC_ASSERT ( pTimes );

			tPosition = ccp ( 535.f, i == 0 ? 183.f : 387.f );

			this->addChild ( pTimes, 0, (KDuint) pTimes );

			pTimes->setPosition ( tPosition );

			m_pTxtTimes [ i ] = (CText *) this->getChildByTag ( (KDuint) pTimes );
		}

		CText*  pCash = CText::create ( "2,000,000,000,000", eIMG_Font_A );
		{
			CC_ASSERT ( pCash );

			tPosition = ccp ( 760.f, i == 0 ? 183.f : 387.f );

			this->addChild ( pCash, 0, (KDuint) pCash );

			pCash->setPosition ( tPosition );

			m_pTxtCash [ i ] = (CText *) this->getChildByTag ( (KDuint) pCash );
		}
	}
	
	for ( i = 0; i < 4; i++ )
	{
		CCSprite*  pCard = CCSprite::create 
		(
			g_pResManager->getPath ( eIMG_Sprite_Card, i )
		);
		{
			CC_ASSERT ( pCard );

			m_tSize [ i ] = pCard->getContentSize ( );
		}
	}

	this->onRefreshTextCount  ( );
	this->onRefreshTextTimes  ( );
	this->onRefreshTextObtain ( );
	this->onRefreshTextCash   ( );
	this->waitEvent ( );

	return KD_TRUE;
}

KDvoid CScrGame::onEnterTransitionDidFinish ( KDvoid )
{
	CCLayer::onEnterTransitionDidFinish ( );

	this->postEvent ( eType_GameStart );
	this->waitEvent ( );
}

KDvoid CScrGame::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_TRUE );
}

KDbool CScrGame::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_uTurn == 0 )
	{
		KDuint  uSize = m_lPlayer [ 0 ].size ( );

		for ( KDuint  i = 0; i < uSize; i++ )
		{
			CCard*  pCard = m_lPlayer [ 0 ].at ( i );
			CCRect  tRect ( CCPointZero, pCard->getContentSize ( ) );

			if ( tRect.containsPoint ( pCard->convertToNodeSpace ( pTouch->getLocation ( ) ) ) )
			{
				this->select ( pCard );

				return KD_TRUE;
			}
		}
	}

	return KD_FALSE;
}

KDvoid CScrGame::postEvent ( EType eType, KDuint uParam1, KDuint uParam2 )
{
	TEvent  tEvent = { eType, uParam1, uParam2 };

	this->postEvent ( tEvent );
}

KDvoid CScrGame::postEvent ( const TEvent& tEvent )
{
	m_lEvent.push_back ( tEvent );

//	kdLogMessagefKHR ( "Post Event : Type %02d, Param1 %d, Param2 %d, remain %d", tEvent.eType, tEvent.uParam1, tEvent.uParam2, m_lEvent.size ( ) );
}

KDvoid CScrGame::waitEvent ( KDvoid )
{
	this->runAction 
	(
		CCSequence::create
		(
			CCDelayTime::create ( 0 ),
			CCCallFunc::create ( this, callfunc_selector ( CScrGame::onEvent ) ),
			KD_NULL
		)
	);
}

KDvoid CScrGame::onEvent ( KDvoid )
{
	if ( m_lEvent.size ( ) > 0 )
	{
		TEvent  tEvent = m_lEvent.front ( );
		
		m_lEvent.pop_front ( );

//		kdLogMessagefKHR ( "Pop Event : Type %02d, Param1 %d, Param2 %d, remain %d", tEvent.eType, tEvent.uParam1, tEvent.uParam2, m_lEvent.size ( ) );

		switch ( tEvent.eType )
		{
			case eType_Vibrate				:	this->onVibrate			  ( );											break;
			case eType_Sound				:	this->onSound			  ( (EPath) tEvent.uParam1 );					break;
			case eType_Effect				:	this->onEffect			  ( (EPath) tEvent.uParam1, tEvent.uParam2 );	break;
			case eType_Talk					:	this->onTalk			  ( (ETalk) tEvent.uParam1 );					break;
			case eType_Dialog				:	this->onDialog			  ( (EDialog) tEvent.uParam1 );					break;

			case eType_GameStart			:	this->onStart			  ( );							break;
			case eType_Shuffle				:	this->onShuffle			  ( );							break;
			case eType_DealCard				:	this->onDealCard		  ( );							break;
			case eType_ChongTong			:	this->onChongTong		  ( );							break;
			case eType_Mission				:	this->onMission			  ( );							break;
			case eType_Result				:	this->onResult			  ( );							break;
			case eType_PutPlayer			:	this->onPutPlayer		  ( );							break;
			case eType_PutShuffled			:	this->onPutShuffled		  ( );							break;
			case eType_Obtain				:	this->onObtain			  ( );							break;
			case eType_Score				:	this->onScore			  ( );							break;
			case eType_Gostop				:	this->onGostop			  ( );							break;
			case eType_EndTurn				:	this->onEndTurn			  ( );							break;
			case eType_CardAttach			:	this->onCardAttach		  ( tEvent.uParam1 );			break;
			case eType_CardToGround			:	this->onCardToGround	  ( tEvent.uParam1 );			break;
			case eType_CardToPlayer1		:	this->onCardToPlayer1	  ( tEvent.uParam1 );			break;
			case eType_CardToPlayer2		:	this->onCardToPlayer2	  ( tEvent.uParam1 );			break;
			case eType_CardToObtain			:	this->onCardToObtain      ( tEvent.uParam1 );			break;
			case eType_ReorderPlayer1		:	this->onReorderPlayer1	  ( );							break;
			case eType_ReorderPlayer2		:	this->onReorderPlayer2	  ( );							break;
			case eType_RefreshObtainable	:	this->onRefreshObtainable ( );							break;
			case eType_RefreshObtain		:	this->onRefreshTextObtain ( );							break;
			case eType_RefreshCount			:	this->onRefreshTextCount  ( );							break;
			case eType_RefreshScore			:	this->onRefreshTextScore  ( );							break;
			case eType_RefreshTimes			:	this->onRefreshTextTimes  ( );							break;
			case eType_RefreshCash			:	this->onRefreshTextCash   ( );							break;
		}
	}
}

KDvoid CScrGame::onVibrate ( KDvoid )
{
	g_pResManager->vibrate ( );

	this->onEvent ( );
}

KDvoid CScrGame::onSound ( EPath ePath )
{
	g_pResManager->playSound ( ePath ); 

	this->onEvent ( );
}

KDvoid CScrGame::onEffect ( EPath ePath, KDuint uNumber )
{
	if ( uNumber != KDUINT_MAX )
	{
		g_pScrManager->showEffect ( ePath, m_lCard.at ( uNumber ) );
	}
	else
	{
		g_pScrManager->showEffect ( ePath );
	}
}

KDvoid CScrGame::onTalk ( ETalk eTalk )
{
	g_pScrManager->showTalk ( eTalk );
}

KDvoid CScrGame::onDialog ( EDialog eDialog )
{
	g_pScrManager->setDialog ( eDialog );
}

KDvoid CScrGame::onShine ( KDfloat fTime )
{
	KDuint  uIndex = kdRand ( ) % m_lCard.size ( );
	CCard*  pCard  = m_lCard.at ( uIndex );
	
	pCard->shine ( );
}

KDvoid CScrGame::onMenu ( CCObject* pSender )
{   
	if ( this->isTouchEnabled ( ) )
	{
		CCNode*   pNode = (CCNode *) pSender;
		CCLayer*  pDialog = g_pScrManager->setDialog ( pNode->getTag ( ) == 0 ? eDlgGameMenu : eDlgUseSkill );

		pDialog->setPosition ( pNode->getPosition ( ) );
		g_pResManager->playSound ( eSND_Etc_Click );
	}
}

KDvoid CScrGame::onStart ( KDvoid )
{
	if ( g_pEnvManager->getPushCount ( ) > 0 )
	{
		g_pScrManager->setDialog ( eDlgTimes );
	}
	else
	{	
		this->postEvent ( eType_Shuffle );
		this->waitEvent ( );
	}
}

KDvoid CScrGame::onShuffle ( KDvoid )
{
	kdSrand ( KD_GET_UST2MSEC );

	std::vector<KDuint>   vNumber;
	for ( KDuint i = 0; i < 51; i++ )
	{
		vNumber.push_back ( i );
	}
	
	m_lCard.resize ( 51 );
	for ( KDuint i = 0; i < 51; i++ )
	{
		KDuint  uIndex  = kdRand ( ) % vNumber.size ( );
		KDuint  uNumber = vNumber [ uIndex ];

		CCard*   pCard = CCard::create ( uNumber, eCard_Where_Shuffled ); 
		{
			CC_ASSERT ( pCard );

			this->addChild ( pCard, 0, (KDuint) pCard );

			pCard->setPosition ( ccp ( m_tWinSize.cx / 2.f, m_tWinSize.cy / 2.f + 50.f + i ) );

			m_lCard [ uNumber ] = m_lShuffled.addFromParent ( pCard );

			vNumber.erase ( vNumber.begin ( ) + uIndex );
		}
	}

	this->schedule ( schedule_selector ( CScrGame::onShine ), INTERVAL_SHINE );
	this->postEvent ( eType_DealCard );
	this->waitEvent ( );
}

KDvoid CScrGame::onDealCard ( KDvoid )
{
	CCardList  lBonus;
	CCard*     pCard;
	EType	   eType;
	
	KDuint  uSize = m_lShuffled.size ( );
	KDuint  i, uIndex;

	for ( i = 0; i < 28; i++ )
	{
		uIndex = uSize - i - 1;

		pCard = m_lShuffled.at ( uIndex );
		pCard->setFlag ( eCard_Flag_Deal );

		if      ( i < 4 || ( i > 13 && i < 18 ) )
		{
			if ( pCard->isBonus ( ) )
			{
				lBonus.push_back ( pCard );
			}

			eType = eType_CardToGround;
		}
		else if ( i < 9 || ( i > 17 && i < 23 ) )
		{
			eType = m_uTurn == 0 ? eType_CardToPlayer1 : eType_CardToPlayer2;
		}
		else if ( i < 14 || ( i > 22 && i < 28 ) )
		{
			eType = m_uTurn == 0 ? eType_CardToPlayer2 : eType_CardToPlayer1;
		}

		this->postEvent ( eType_Sound, eSND_Effect_Shuffle1 + i % 3 );
		this->postEvent ( eType, pCard->getNumber ( ) );
	}

	while ( lBonus.size ( ) )
	{
		uSize = lBonus.size ( );

		for ( i = 0; i < uSize; i++ )
		{
			pCard = lBonus.at ( i );

			this->postEvent ( eType_CardToObtain, pCard->getNumber ( ) );
		}

		for ( i = 0; i < uSize; i++ )
		{
			lBonus.pop_front ( );

			uIndex--;
			pCard = m_lShuffled.at ( uIndex );
			pCard->setFlag ( eCard_Flag_Deal );

			if ( pCard->isBonus ( ) )
			{
				lBonus.push_back ( pCard );
			}

			this->postEvent ( eType_CardToGround, pCard->getNumber ( ) );
		}
	}

	this->postEvent ( eType_Sound, eSND_BGM_Play );
	this->postEvent ( eType_ReorderPlayer1 );
	this->postEvent ( eType_RefreshObtainable );
	this->postEvent ( eType_ChongTong );
	this->waitEvent ( );
}

KDvoid CScrGame::onChongTong ( KDvoid )
{
	ECardWhere  eWheres [ 4 ];

	KDuint  uFound [ 3 ] = { KDUINT_MAX, KDUINT_MAX, KDUINT_MAX };
	KDuint  uMonth;
	KDuint  uNumber;

	KDuint  i, j;

	for ( i = 0; i < 12; i++ )
	{
		uMonth  = 11 - i;
		uNumber = uMonth * 4;

		for ( j = 0; j < 4; j++ )
		{
			eWheres [ j ] = m_lCard.at ( uNumber + j )->getWhere ( );
		}

		if ( eWheres [ 0 ] == eWheres [ 1 ] &&
			 eWheres [ 0 ] == eWheres [ 2 ] &&
			 eWheres [ 0 ] == eWheres [ 3 ] )
		{
			switch ( eWheres [ 0 ] )
			{
				case eCard_Where_Ground  : uFound [ 0 ] = uMonth; break;
				case eCard_Where_Player1 : uFound [ 1 ] = uMonth; break;
				case eCard_Where_Player2 : uFound [ 2 ] = uMonth; break;
				default : ;
			}
		}
	}

	if      ( uFound [ 0 ] != KDUINT_MAX )
	{
		CDlgChongTong1::s_uMonth = uFound [ 0 ];

		g_pScrManager->setDialog ( eDlgChongTong1 );

		return;
	}

	for ( i = 0; i < 2; i++ )
	{
		if ( m_uTurn == i )
		{
			if ( uFound [ 1 ] != KDUINT_MAX )
			{
				CDlgChongTong2::s_uMonth = uFound [ 1 ];

				g_pScrManager->setDialog ( eDlgChongTong2 );

				return;
			}
		}
		else
		{
			if ( uFound [ 2 ] != KDUINT_MAX )
			{
				m_uTurn = 1;
				CDlgChongTong1::s_uMonth = uFound [ 2 ];

				g_pScrManager->setDialog ( eDlgChongTong1 );

				return;
			}
		}
	}

	this->postEvent ( eType_Mission );
	this->waitEvent ( );
}

KDvoid CScrGame::onMission ( KDvoid )
{
	const KDuint*  pMission = KD_NULL;
	KDuint  uIndex  = 0;
	KDuint  uCount  = 0;
	KDuint  uNumber = 0;
	KDuint  i;

	uIndex   = kdRand ( ) % 21;
	pMission = g_pEnvManager->getMission ( uIndex );
	uCount   = pMission [ 4 ] == KDUINT_MAX ? 3 : 4;

	m_uMissionTimes = pMission [ 0 ];

	for ( i = 0; i < uCount; i++ )
	{
		uNumber = pMission [ i + 1 ];

		CCard*  pCard = CCard::create ( uNumber, eCard_Where_Mission );
		{
			CC_ASSERT ( pCard );

			const CCSize&  tSize = pCard->getContentSize ( );

			this->addChild ( pCard, 0, (KDuint) pCard );

			pCard->setPosition ( ccp ( ( tSize.cx + 1 ) * i + tSize.cx / 2.f + 8.f, tSize.cy / 2.f + 232.f ) );
			pCard->setUserData ( m_lCard.at ( uNumber ) );

			m_lCard.at ( uNumber )->setMission ( KD_TRUE );
			m_lMission.addFromParent ( pCard );
		}
	}

	KDchar  szText [ 64 ];
	kdSprintfKHR ( szText, "x%d", pMission [ 0 ] );
	CText*  pTimes = CText::create ( szText, eIMG_Font_C );
	{
		CC_ASSERT ( pTimes );

		this->addChild ( pTimes );

		pTimes->setPosition ( ccp ( 122.f, 275.f ) );
	}

	CDlgMission::s_uIndex = uIndex;
	g_pScrManager->setDialog ( eDlgMission );

	// If Visible Title
	if ( m_uTurn == 0 && g_pEnvManager->getTitle ( ) == 0 )
	{
		CCard*  pCard = m_lShuffled.back ( );

		pCard->setFront ( KD_TRUE );
		pCard->setSize  ( eCard_Size_Medium );
	}
}

KDvoid CScrGame::onResult ( KDvoid )
{
	if ( m_uTurn == 1 && ( m_uSkillAvailable [ 2 ] == 0 || this->aiPush ( ) ) )
	{
		g_pScrManager->setDialog ( eDlgPushEnemy );
	}
	else
	{
		g_pScrManager->setDialog ( eDlgResult );
	}
}

KDvoid CScrGame::onPutPlayer ( KDvoid )
{
	if ( m_uTurn == 0 )
	{
		this->setTouchEnabled ( KD_TRUE );
	}
	
	this->select ( this->aiPutCard ( ) );
}

KDvoid CScrGame::onPutShuffled ( KDvoid )
{
	this->postEvent ( eType_CardToGround, m_lShuffled.back ( )->getNumber ( ) );
	this->waitEvent ( );
}

KDvoid CScrGame::onObtain ( KDvoid )
{
	CCardList*  pList = KD_NULL;
	CCard*      pCard = KD_NULL;
	KDuint      i;

	if ( m_uObtainGround [ 0 ] != KDUINT_MAX )
	{
		KDuint  uCount = this->countGround ( m_uObtainGround [ 0 ] );  
		KDbool  bRecursive = m_uObtainGround [ 1 ] != KDUINT_MAX;

		pList = &m_lGround [ m_uObtainGround [ 0 ] ];

		m_uObtainGround [ 0 ] = m_uObtainGround [ 1 ];
		m_uObtainGround [ 1 ] = KDUINT_MAX;

		for ( i = uCount == 3 ? 2 : 0; i < pList->size ( ); i++ )
		{
			pCard = pList->at ( i );
			if ( uCount == 1 )
			{
				if ( pCard->isBonus ( ) )
				{
					m_lObtainable.push_back ( pCard );
				}
			}
			else
			{
				m_lObtainable.push_back ( pCard );
			}
		}

		if ( uCount == 3 )
		{
			CCard*  pCard1 = pList->at ( 0 );
			CCard*  pCard2 = pList->at ( 1 );

			if ( pCard1->getType ( ) == eCard_Type_1Pi && 
				 pCard2->getType ( ) == eCard_Type_1Pi )
			{
				m_lObtainable.push_back ( pCard2 );
			}
			else
			{
				if ( m_uTurn == 0 )
				{
					CDlgCard::s_pList  = &m_lObtainable;
					CDlgCard::s_pCard1 = pCard1;
					CDlgCard::s_pCard2 = pCard2;

					this->postEvent ( eType_Dialog, eDlgCard );
				}
				else
				{
					m_lObtainable.push_back ( this->aiSelect ( pCard1, pCard2 ) );
				}
			}
		}

		if ( bRecursive )
		{
			this->onObtain ( );
			return;
		}
		else
		{
			this->postEvent ( eType_Obtain );
		}
	}
	else if ( m_lObtainable.size ( ) )
	{
		pList = &m_lObtainable;

		// If GukJin
		for ( i = 0; i < pList->size ( ); i++ )
		{
			pCard = pList->at ( i );

			if ( pCard->getNumber ( ) == 32 )
			{
				if ( m_uTurn == 0 )
				{
					CDlgGukJin::s_pCard = pCard;

					this->postEvent ( eType_Dialog, eDlgGukJin );
				}
				else
				{
					this->aiGukJin ( );
				}
			}
		}

		while ( pList->size ( ) )
		{
			pCard = pList->getLast ( );
			pCard->setFlag ( pList->size ( ) > 1 ? eCard_Flag_Combo : eCard_Flag_Null );

			this->postEvent ( eType_CardToObtain, pCard->getNumber ( ) );
			pList->pop_back ( );
		}

		this->postEvent ( eType_Obtain );
	}
	else
	{
		// If SackSul
		if ( m_uRound < 18 )
		{
			for ( i = 0; i < 12; i++ )
			{
				if ( m_lGround [ i ].size ( ) > 0 )
				{
					break;
				}
			}

			if ( i == 12 )
			{
				m_uObtainSteal++;

				this->postEvent ( eType_Effect, eIMG_Effect_SackSul );
				this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_SackSul : eSND_Harm_SackSul );

				if ( g_pEnvManager->rand ( ) < 50 )
				{
					this->postEvent ( eType_Talk, eTALK_SackSul );
				}
			}
		}

		this->steal1Pi ( m_uObtainSteal );

		if ( m_uTurn == 0 )
		{
			this->postEvent ( eType_ReorderPlayer1 );
		}

		this->postEvent ( eType_RefreshObtainable );
		
		m_uObtainGround [ 0 ] = KDUINT_MAX;
		m_uObtainGround [ 1 ] = KDUINT_MAX;
		m_uObtainSteal = 0;
		m_lObtainable.clear ( );

		this->postEvent ( eType_Score );
	}

	this->waitEvent ( );
}

KDvoid CScrGame::onScore ( KDvoid )
{
	ECardWhere  eWhere  = m_uTurn == 0 ? eCard_Where_Obtain1 : eCard_Where_Obtain2;
	CCardList*   pList  = KD_NULL;
	TScore*     pScore1 = &m_tScore [  m_uTurn ];
	TScore*     pScore2 = &m_tScore [ !m_uTurn ];
	KDuint      uCount  = 0;

	// Check Mission
	if ( m_bMission )
	{
		if ( this->checkMission ( ) == KD_TRUE )
		{
			if      ( m_tScore [ 0 ].uMission > 0 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_MissionS );
				this->postEvent ( eType_Sound, eSND_Attack_Gage_Success1 );
			}
			else if ( m_tScore [ 1 ].uMission > 0 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_MissionS );
			}
			else
			{
				this->postEvent ( eType_Effect, eIMG_Effect_MissionF );
			}

			m_bMission = KD_FALSE;
		}
	}

	// Calc Gwang
	pList = &m_lObtain [ m_uTurn ][ 0 ];
	if ( pScore1->uGwang != pList->size ( ) )
	{
		pScore1->uGwang = pList->size ( );
		if ( m_lCard.at ( 44 )->getWhere ( ) == eWhere )
		{
			pScore1->uBGwang = 1;
		}
		
		switch ( pScore1->uGwang )
		{
			case 3 : 

				if ( pScore1->uBGwang )
				{
					this->postEvent ( eType_Effect, eIMG_Effect_BGwang );
					this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_BGwang : eSND_Harm_BGwang );

					if ( g_pEnvManager->rand ( ) < 40 )
					{	
						this->postEvent ( eType_Talk, eTALK_BGwang );
					}
				}
				else
				{
					this->postEvent ( eType_Effect, eIMG_Effect_3Gwang );
					this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_3Gwang : eSND_Harm_3Gwang );

					if ( g_pEnvManager->rand ( ) < 50 )
					{
						this->postEvent ( eType_Talk, eTALK_3Gwang );
					}
				}

				break;

			case 4 : 

				this->postEvent ( eType_Effect, eIMG_Effect_4Gwang );
				this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_4Gwang : eSND_Harm_4Gwang );

				if ( g_pEnvManager->rand ( ) < 50 )
				{
					this->postEvent ( eType_Talk, eTALK_3Gwang );
				}

				break;

			case 5 : 

				this->postEvent ( eType_Effect, eIMG_Effect_5Gwang );
				this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_5Gwang : eSND_Harm_5Gwang );
				this->postEvent ( eType_Talk, eTALK_5Gwang );

				break;
		}
	}

	// Calc 10Pi
	pList = &m_lObtain [ m_uTurn ][ 1 ];
	if ( pScore1->u10Pi != pList->size ( ) )
	{
		pScore1->u10Pi = pList->size ( );

		// Count Godori 
		uCount = 0;
		uCount += m_lCard [  4 ]->getWhere ( ) == eWhere ? 1 : 0;
		uCount += m_lCard [ 12 ]->getWhere ( ) == eWhere ? 1 : 0;
		uCount += m_lCard [ 29 ]->getWhere ( ) == eWhere ? 1 : 0;
		if ( pScore1->uGodori != uCount )
		{
			pScore1->uGodori = uCount;

			if ( uCount == 3 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_Godori );
				this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_Godori : eSND_Harm_Godori );
				this->postEvent ( eType_Talk, eTALK_Godori );
			}
			else if ( uCount == 2 && pScore2->uGodori == 0 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_Bell, pList->getLast ( )->getNumber ( ) );
			}
		}
	}

	// Calc 5Pi
	pList = &m_lObtain [ m_uTurn ][ 2 ];
	if ( pScore1->u5Pi != pList->size ( ) )
	{
		pScore1->u5Pi = pList->size ( );

		// Count HongDan 
		uCount = 0;
		uCount += m_lCard [  1 ]->getWhere ( ) == eWhere ? 1 : 0;
		uCount += m_lCard [  5 ]->getWhere ( ) == eWhere ? 1 : 0;
		uCount += m_lCard [  9 ]->getWhere ( ) == eWhere ? 1 : 0;
		if ( pScore1->uHongDan != uCount )
		{
			pScore1->uHongDan = uCount;

			if ( uCount == 3 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_HongDan );
				this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_HongDan : eSND_Harm_HongDan );

				if ( g_pEnvManager->rand ( ) < 50 )
				{
					this->postEvent ( eType_Talk, eTALK_HongDan );
				}
			}
			else if ( uCount == 2 && pScore2->uHongDan == 0 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_Bell, pList->getLast ( )->getNumber ( ) );
			}
		}

		// Count ChoDan 
		uCount = 0;
		uCount += m_lCard [ 13 ]->getWhere ( ) == eWhere ? 1 : 0;
		uCount += m_lCard [ 17 ]->getWhere ( ) == eWhere ? 1 : 0;
		uCount += m_lCard [ 25 ]->getWhere ( ) == eWhere ? 1 : 0;
		if ( pScore1->uChoDan != uCount )
		{
			pScore1->uChoDan = uCount;

			if ( uCount == 3 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_ChoDan );
				this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_ChoDan : eSND_Harm_ChoDan );

				if ( g_pEnvManager->rand ( ) < 50 )
				{
					this->postEvent ( eType_Talk, eTALK_ChoDan );
				}
			}
			else if ( uCount == 2 && pScore2->uChoDan == 0 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_Bell, pList->getLast ( )->getNumber ( ) );
			}
		}

		// Count ChungDan 
		uCount = 0;
		uCount += m_lCard [ 21 ]->getWhere ( ) == eWhere ? 1 : 0;
		uCount += m_lCard [ 33 ]->getWhere ( ) == eWhere ? 1 : 0;
		uCount += m_lCard [ 37 ]->getWhere ( ) == eWhere ? 1 : 0;
		if ( pScore1->uChungDan != uCount )
		{
			pScore1->uChungDan = uCount;

			if ( uCount == 3 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_ChungDan );
				this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_ChungDan : eSND_Harm_ChungDan );

				if ( g_pEnvManager->rand ( ) < 50 )
				{
					this->postEvent ( eType_Talk, eTALK_ChungDan );
				}
			}
			else if ( uCount == 2 && pScore2->uChungDan == 0 )
			{
				this->postEvent ( eType_Effect, eIMG_Effect_Bell, pList->getLast ( )->getNumber ( ) );
			}
		}
	}

	this->postEvent ( eType_RefreshScore );
	this->postEvent ( eType_Gostop );
	this->waitEvent ( );
}

KDvoid CScrGame::onGostop ( KDvoid )
{
	TScore*  pScore = &m_tScore [ m_uTurn ];

	if ( pScore->uMax < pScore->uScore )
	{
		pScore->uMax = pScore->uScore;

		// Go or Stop
		if ( pScore->uScore > 6 )
		{
			KDuint  uCash = m_uTurn == 0 ? g_pEnvManager->getCash ( ) : g_pEnvManager->getEnemyCash ( );
			KDuint  uWinCash = pScore->uTotalScore * g_pEnvManager->getBet ( );

			if ( uCash < uWinCash )
			{
				this->setGostop ( KD_FALSE );
			}
			else if ( m_uRound < 18 )
			{
				if ( m_uTurn == 0 )
				{
					this->postEvent ( eType_Dialog, eDlgGostop );
				}
				else
				{
					KDbool  bGoStop = this->aiGoStop ( );

					this->setGostop ( bGoStop );
				}
			}
			else
			{
				this->setGostop ( KD_FALSE );	
			}
		}
		else
		{
			this->postEvent ( eType_EndTurn );
		}
	}
	else
	{
		this->postEvent ( eType_EndTurn );
	}

	this->waitEvent ( );
}

KDvoid CScrGame::onEndTurn ( KDvoid )
{
	m_uRound++;
	m_uTurn = !m_uTurn;

	if ( m_uRound == 20 )
	{
		g_pEnvManager->setPushCount ( g_pEnvManager->getPushCount ( ) + 1 );
		g_pScrManager->setDialog ( eDlgNagari );
	}
	else
	{
		if ( m_uTurn == 0 )
		{
			g_pResManager->playSound ( eSND_Effect_Turn );
		}
		
		this->postEvent ( eType_PutPlayer );
		this->waitEvent ( );
	}
}

KDvoid CScrGame::onCardAttach ( KDuint uIndex )
{
	KDuint     uCount  = this->countGround ( uIndex );
	CCardList*  pList   = &m_lGround [ uIndex ];
	CCard*     pCard   = pList->getLast ( );
	KDuint     uNumber = pCard->getNumber ( );

	if ( m_pSelect == pCard )
	{
		if ( uCount == 1 )
		{
			this->postEvent ( eType_Effect, eIMG_Effect_Heading, uNumber );
		}
		else
		{
			KDuint  uFlag = pCard->getFlag ( );

			if ( uCount == 4 )
			{
				if ( uFlag & eCard_Flag_Bomb )
				{
					this->postEvent ( eType_Effect, ( uFlag & eCard_Flag_Gage ) ? eIMG_Effect_Gage : eIMG_Effect_Bomb, uNumber ); 
					this->postEvent ( eType_Vibrate );
					this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_Bomb : eSND_Harm_Bomb );
					this->postEvent ( eType_Talk, eTALK_Bomb );

					m_uObtainSteal++;
				}
				else
				{
					this->postEvent ( eType_Effect, ( uFlag & eCard_Flag_Gage ) ? eIMG_Effect_Gage : eIMG_Effect_DaDack, uNumber ); 

					if ( m_uRound < 18 )
					{
						// If Buck Eat
						this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_BuckEat : eSND_Harm_BuckEat );

						m_uObtainSteal++;
					}
				}
			}
			else if ( uFlag & eCard_Flag_Gage )
			{
				this->postEvent ( eType_Vibrate );
				this->postEvent ( eType_Effect, eIMG_Effect_Gage, uNumber ); 
			}
			else
			{
				this->postEvent ( eType_Effect, eIMG_Effect_Zack, uNumber ); 
			}

			m_uObtainGround [ 0 ] = uIndex;
		}

		this->postEvent ( eType_PutShuffled );
	}
	else
	{
		if ( pCard->isBonus ( ) )
		{
			m_uObtainGround [ 0 ] = uIndex;

			this->postEvent ( eType_Effect, eIMG_Effect_Zack, uNumber ); 
			this->postEvent ( eType_PutShuffled );
		}
		else
		{
			KDbool  bObtain = KD_TRUE;

			switch ( uCount )
			{
				case 1 :

					this->postEvent ( eType_Effect, eIMG_Effect_Heading, uNumber ); 

					if ( m_uObtainGround [ 0 ] == KDUINT_MAX )
					{
						if ( m_uTurn == 0 )
						{
							this->postEvent ( eType_Sound, eSND_Attack_Heading );
						}

						if ( g_pEnvManager->rand ( ) < 20 )
						{
							this->postEvent ( eType_Talk, eTALK_Heading );
						}
					}

					break;

				case 2 :

					if ( m_uObtainGround [ 0 ] == KDUINT_MAX )
					{
						m_uObtainGround [ 0 ] = uIndex;
					}
					else if ( m_uObtainGround [ 0 ] != uIndex )
					{
						m_uObtainGround [ 1 ] = uIndex;
					}

					if ( m_uRound < 18 && m_pSelect && m_pSelect->getMonth ( ) == pCard->getMonth ( ) )
					{
						// If Zock
						m_uObtainSteal++;

						this->postEvent ( eType_Effect, eIMG_Effect_Zock, uNumber ); 
						this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_Zock : eSND_Harm_Zock );

						if ( g_pEnvManager->rand ( ) < 50 )
						{
							this->postEvent ( eType_Talk, eTALK_Zock );
						}
					}
					else
					{
						this->postEvent ( eType_Effect, eIMG_Effect_Zack, uNumber ); 
					}

					break;

				case 3 :

					if ( m_uRound < 18 && m_pSelect && m_pSelect->getMonth ( ) == pCard->getMonth ( ) )
					{
						// If Buck
						m_uObtainGround [ 0 ] = KDUINT_MAX;
						m_tScore [ m_uTurn ].uBuck++;

						this->postEvent ( eType_RefreshCount );
						this->postEvent ( eType_Effect, eIMG_Effect_Buck, uNumber ); 
	
						CDlgBuck::s_uTurn  = m_uTurn;
						CDlgBuck::s_uCount = m_tScore [ m_uTurn ].uBuck;
						CDlgBuck::s_bCombo = ( m_uRound / 2 ) == ( m_tScore [ m_uTurn ].uBuck - 1 );
						if ( CDlgBuck::s_bCombo || CDlgBuck::s_uCount == 3 )
						{
							bObtain = KD_FALSE;
							this->postEvent ( eType_Dialog, eDlgBuck );
						}
						else
						{
							this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_Buck : eSND_Harm_Buck );

							if ( g_pEnvManager->rand ( ) < 50 )
							{
								this->postEvent ( eType_Talk, eTALK_Buck );
							}
						}
					}
					else
					{
						if ( m_uObtainGround [ 0 ] == KDUINT_MAX )
						{
							m_uObtainGround [ 0 ] = uIndex;
						}
						else if ( m_uObtainGround [ 0 ] != uIndex )
						{
							m_uObtainGround [ 1 ] = uIndex;
						}

						this->postEvent ( eType_Effect, eIMG_Effect_Zack, uNumber ); 
					}

					break;

				case 4 :

					if ( m_uObtainGround [ 0 ] == KDUINT_MAX )
					{
						m_uObtainGround [ 0 ] = uIndex;
					}
					else if ( m_uObtainGround [ 0 ] != uIndex )
					{
						m_uObtainGround [ 1 ] = uIndex;
					}

					this->postEvent ( eType_Effect, eIMG_Effect_DaDack, uNumber ); 	

					if ( m_uRound < 18 && m_pSelect && m_pSelect->getMonth ( ) == pCard->getMonth ( ) )
					{
						// If DaDack
						m_uObtainSteal++;

						this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_DaDack : eSND_Harm_DaDack );
						this->postEvent ( eType_Talk, eTALK_DaDack );
					}
					else if ( m_uRound < 18 )
					{
						// If Buck Eat 
						m_uObtainSteal++;
						
						this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_BuckEat : eSND_Harm_BuckEat );
					}

					break;
			}

			// If Gwang Gage Success
			if ( m_pSelect && ( m_pSelect->getFlag ( ) & eCard_Flag_Gage ) )
			{
				m_tScore [ 0 ].uGage++;
				m_pSelect->setFlag ( m_pSelect->getFlag ( ) & 0x0fff );

				this->postEvent ( eType_RefreshTimes );
				this->postEvent ( eType_Effect, eIMG_Effect_Gage2 );
			}

			if ( bObtain )
			{
				this->postEvent ( eType_Obtain );
			}
		}
	}

	this->waitEvent ( );
}

KDvoid CScrGame::onCardToGround ( KDuint uNumber )
{
	const CCSize&  tSize = m_tSize [ eCard_Size_Medium ];
	CCPoint    tPosition ( ccpMid ( m_tWinSize ) );

	CCard*      pCard  = m_lCard.at ( uNumber );
	ECardWhere  eWhere = pCard->getWhere ( );
	KDuint		uMonth = pCard->getMonth ( );
	KDuint      uFlag  = pCard->getFlag  ( );
	KDuint		uIndex = KDUINT_MAX;
	KDuint		uOrder = 0;
	KDuint		i;

	if ( pCard->isBonus ( ) )
	{
		if ( uFlag != eCard_Flag_Deal )
		{
			uIndex = m_uPrevGround;
		}
	}
	else
	{
		uIndex = this->findGround ( uMonth );
	}

	if ( uIndex == KDUINT_MAX )
	{
		for ( i = 0; i < 12; i++ )
		{
			if ( m_lGround [ i ].size ( ) == 0 )
			{
				uIndex = i;
				break;
			}
		}
	}

	m_uPrevGround = uIndex;
	m_lGround [ uIndex ].push_back ( pCard );
	uOrder = m_lGround [ uIndex ].size ( );

	tPosition.x += ( ( tSize.cx + 20.f ) * ( uIndex / 4 + 1 ) + 10.f ) * ( uIndex % 2 ? 1 : -1 );
	tPosition.y += ( tSize.cy / 2.f + 8.f ) * ( uIndex / 2 % 2 ? -1 : 1 ) + 57.f;

	switch ( eWhere )
	{
		case eCard_Where_Shuffled :

			m_lShuffled.remove ( pCard );

			if ( uFlag & eCard_Flag_Deal )
			{
				pCard->setFlag ( eCard_Flag_Null );
			}
			else
			{
				pCard->setPosition ( ccpAdd ( pCard->getPosition ( ), ccp ( 0, tSize.cy ) ) );
				pCard->setScale ( 3.0f );
			}

			break;

		case eCard_Where_Player1 :

			pCard->setPosition ( ccpAdd ( pCard->getPosition ( ), ccp ( 0, tSize.cy ) ) );
			pCard->setScale ( 3.0f );
			m_lPlayer [ 0 ].remove ( pCard );

			break;

		case eCard_Where_Player2 :

			pCard->setPosition ( ccp ( m_tWinSize.cx - tSize.cx, m_tWinSize.cy - tSize.cy ) );
			pCard->setScale ( 3.0f );
			m_lPlayer [ 1 ].remove ( pCard );

			this->onReorderPlayer2 ( );

			break;

		default : ;
	}

	for ( i = 0; i < uOrder; i++ )
	{
		CCard*   pCard2 = m_lGround [ uIndex ].at ( i );
		CCPoint	 tPosition2 = tPosition;

		if ( i > 3 )
		{
			tPosition2.x +=  5.f + 2.f * ( i - 3 ); 
			tPosition2.y -= 12.f - 2.f * ( i - 3 );
		}
		else if ( i == 2 )
		{
			tPosition2.y -= 10.f; 
		}
		else if ( i == 1 )
		{
			tPosition2.x += 10.f; 
			tPosition2.y -=  5.f;
		}

		if ( ( uOrder - 1 ) == i )
		{
			if ( uFlag & eCard_Flag_Combo )
			{
				this->onEvent ( );
			}
			else if ( !( uFlag & eCard_Flag_Deal ) )
			{
				this->postEvent ( eType_CardAttach, uIndex );
			}

			pCard2->moveTo 
			(
				tPosition2, eCard_Where_Ground, 5 + i, 
				( uFlag & eCard_Flag_Deal ) ? 0 : g_pEnvManager->getAnimate ( ) / 2.f,
				( uFlag & eCard_Flag_Combo ) ? KD_NULL : CCCallFunc::create ( this, callfunc_selector ( CScrGame::onEvent ) )
			);
		}
		else
		{
			pCard2->reorder ( 5 + i );
			pCard2->setPosition ( tPosition2 );
		}
	}
}

KDvoid CScrGame::onCardToPlayer1 ( KDuint uNumber )
{
	const CCSize&  tSize = m_tSize [ eCard_Size_Normal ];
	CCPoint  tPosition;

	CCard*   pCard = m_lCard.at ( uNumber );

	if ( pCard->getFlag ( ) & eCard_Flag_Deal )
	{
		pCard->setFlag ( eCard_Flag_Null );

		tPosition.x = tSize.cx / 2.f + 100.f;
		tPosition.y = tSize.cy / 2.f + 6.f;
		tPosition.x += ( tSize.cx + 6 ) * m_lPlayer [ 0 ].size ( );
	}
	else
	{
		tPosition = m_tPosition;
	}

	m_lPlayer [ 0 ].push_back ( pCard );
	m_lShuffled.remove ( pCard );

	pCard->moveTo 
	(
		tPosition, eCard_Where_Player1, 5, 0,
		CCCallFunc::create ( this, callfunc_selector ( CScrGame::onEvent ) )
	);
}

KDvoid CScrGame::onCardToPlayer2 ( KDuint uNumber )
{
	const CCSize&  tSize = m_tSize [ eCard_Size_Mini ];
	CCPoint  tPosition ( tSize.cx / 2.f + 705.f, m_tWinSize.cy - tSize.cy / 2.f - 5.f );

	CCard*   pCard  = m_lCard.at ( uNumber );
	KDuint   uIndex = m_lPlayer [ 1 ].size ( );

	tPosition.x += ( tSize.cx + 1.f ) * ( uIndex % 5 );
	tPosition.y -= ( tSize.cy + 2.f ) * ( uIndex / 5 );

	if ( pCard->getFlag ( ) & eCard_Flag_Deal )
	{
		pCard->setFlag ( eCard_Flag_Null );
	}

	m_lPlayer [ 1 ].push_back ( pCard );
	m_lShuffled.remove ( pCard );

	pCard->moveTo
	(
		tPosition, eCard_Where_Player2, 5 + uIndex, 0,
		CCCallFunc::create ( this, callfunc_selector ( CScrGame::onEvent ) )
	);
}

KDvoid CScrGame::onCardToObtain ( KDuint uNumber )
{
	const CCSize&   tSize = m_tSize [ eCard_Size_Medium ];
	CCPoint     tPosition = m_uTurn == 0 ? ccp ( 123, 142.f ) : ccp ( tSize.cx / 2.f + 5, m_tWinSize.cy - tSize.cy / 2.f - 5.f );

	CCard*		pCard  = m_lCard.at ( uNumber );
	ECardType   eType  = pCard->getType ( );
	KDuint		uFlag  = pCard->getFlag ( );
	KDuint      uIndex = eType > eCard_Type_5Pi ? 3 : eType;
	KDuint      uSize  = m_lObtain [ m_uTurn ][ uIndex ].size ( ) + 1;
	KDfloat     fIncr  = tSize.cx / 4.f;
	KDuint      uCount = 0;

	switch ( uIndex )
	{
		case 0 : tPosition.x +=   0.f; uCount = uSize + 4; break;
		case 1 : tPosition.x += 105.f; uCount = uSize + 2; break;
		case 2 : tPosition.x += 255.f; uCount = uSize + 2; break;
		case 3 : tPosition.x += 415.f; uCount = uSize + 0; break;
	}

	if ( uCount < 5 )
	{
		fIncr *= 7 - uCount;
	}

	switch ( pCard->getWhere ( ) )
	{
		case eCard_Where_Shuffled :
			m_lShuffled.remove ( pCard );
			break;

		case eCard_Where_Ground	:
			m_lGround [ this->findGround ( pCard->getMonth ( ) ) ].remove ( pCard );
			break;

		case eCard_Where_Player1 :
			m_lPlayer [ 0 ].remove ( pCard ); 
			break;

		case eCard_Where_Player2 :
			m_lPlayer [ 1 ].remove ( pCard ); 
			break;

		case eCard_Where_Obtain1 :
			m_lObtain [ 0 ][ uIndex ].remove ( pCard );
			break;

		case eCard_Where_Obtain2 :
			m_lObtain [ 1 ][ uIndex ].remove ( pCard );
			break;
            
        default : ;
	}

	m_lObtain [ m_uTurn ][ uIndex ].push_back ( pCard );

	if ( uFlag & eCard_Flag_Combo )
	{
		this->onEvent ( );
	}

	for ( KDuint  i = 0; i < uSize ; i++ )
	{
		CCard*   pCard2 = m_lObtain [ m_uTurn ][ uIndex ].at ( i );
		CCPoint  tPosition2 ( tPosition.x + fIncr * i, tPosition.y );

		if ( ( i < uSize - 1 ) || ( uFlag & eCard_Flag_Combo ) )
		{
			pCard2->moveTo ( tPosition2, m_uTurn == 0 ? eCard_Where_Obtain1 : eCard_Where_Obtain2, 5 + i );
		}
		else
		{
			this->onRefreshTextObtain ( );

			pCard2->moveTo
			(
				tPosition2, m_uTurn == 0 ? eCard_Where_Obtain1 : eCard_Where_Obtain2, 5 + i, 0,
				CCCallFunc::create ( this, callfunc_selector ( CScrGame::onEvent ) )
			);
		}
	}
}

KDvoid CScrGame::onReorderPlayer1 ( KDvoid )
{
	const CCSize&  tSize = m_tSize [ eCard_Size_Normal ];
	CCPoint    tPosition ( tSize.cx / 2.f + 100.f, tSize.cy / 2.f + 6.f );

	CCardList*  pList  = &m_lPlayer [ 0 ];
	CCard*     pCard1 = KD_NULL;
	CCard*     pCard2 = KD_NULL;
	KDuint     uSize  = pList->size ( );
	KDuint     uIndex = 10 - uSize;
	KDuint     i, j;

	if ( uSize > 0 )
	{
		for ( i = 0; i < ( uSize - 1 ); i++, uIndex++ )
		{	
			for ( j = i + 1; j < uSize; j++ )
			{
				pCard1 = pList->at ( i );
				pCard2 = pList->at ( j );

				if ( pCard1->getNumber ( ) > pCard2->getNumber ( ) )
				{
					m_lPlayer [ 0 ][ i ] = pCard2;
					m_lPlayer [ 0 ][ j ] = pCard1;
				}
			}

			pCard1 = pList->at ( i );
			pCard1->setPosition ( ccpAdd ( tPosition, ccp ( ( tSize.cx + 6 ) * uIndex, 0 ) ) );
		}

		pList->getLast ( )->setPosition ( ccpAdd ( tPosition, ccp ( ( tSize.cx + 6 ) * 9, 0 ) ) );
	}

	this->onEvent ( );
}

KDvoid CScrGame::onReorderPlayer2 ( KDvoid )
{
	const CCSize&  tSize = m_tSize [ eCard_Size_Mini ];
	CCPoint    tPosition ( tSize.cx / 2.f + 705.f, m_tWinSize.cy - tSize.cy / 2.f - 5.f );

	CCardList*  pList = &m_lPlayer [ 1 ];
	KDuint     uSize = pList->size ( );
	KDuint     i;

	for ( i = 0; i < uSize; i++ )
	{	
		CCPoint  tPosition2 = tPosition;

		tPosition2.x += ( tSize.cx + 1.f ) * ( i % 5 );
		tPosition2.y -= ( tSize.cy + 2.f ) * ( i / 5 );

		pList->at ( i )->setPosition ( tPosition2 );
	}
}

KDvoid CScrGame::onRefreshObtainable ( KDvoid )
{
	CCardList*   pList  = &m_lPlayer [ 0 ];
	CCard*      pCard  = KD_NULL;
	CCard*      pCard2 = KD_NULL;
	ECardWhere  eWhere;
	KDbool		hasObtain;
	KDbool		hasGround;
	KDuint		uMonth;
	KDuint		i, j;

	for ( i = 0; i < pList->size ( ); i++ )
	{
		pCard = pList->at ( i );

		if ( pCard->isNormal ( ) )
		{
			hasObtain = KD_FALSE;
			hasGround = KD_FALSE;
			uMonth = pCard->getMonth ( );
		
			for ( j = 0; j < 4; j++ )
			{
				pCard2 = m_lCard.at ( uMonth * 4 + j );
				eWhere = pCard2->getWhere ( );

				switch ( eWhere )
				{
					case eCard_Where_Ground : 
						hasGround = KD_TRUE;
						break;

					case eCard_Where_Obtain1 :
					case eCard_Where_Obtain2 :
						hasObtain = KD_TRUE;
						break;
                        
                    default : ;
				}
			}

			pCard->setDark  ( hasObtain );
			pCard->setArrow ( hasGround );
		}
	}

	this->onEvent ( );
}

KDvoid CScrGame::onRefreshTextCash ( KDvoid )
{
	KDuint  uCash1 = g_pEnvManager->getCash ( );
	KDuint  uCash2 = g_pEnvManager->getEnemyCash ( );

	m_pTxtCash [ 0 ]->setText ( g_pEnvManager->convCash ( uCash1 ) );
	m_pTxtCash [ 1 ]->setText ( g_pEnvManager->convCash ( uCash2 ) );

	this->onEvent ( );
}

KDvoid CScrGame::onRefreshTextCount ( KDvoid )
{
	TScore*  pScore;
	KDchar   szText [ 64 ];
	KDuint   i, j, uValue, uTimes;

	for ( i = 0; i < 2; i++ )
	{
		pScore = &m_tScore [ i ];
		uTimes = (KDuint) kdPowf ( 2.f, (KDfloat) g_pEnvManager->getPushCount ( ) );

		if ( pScore->bChongTongGo )
		{
			uTimes *= 4;
		}

		if ( pScore->uMission )
		{
			uTimes *= m_uMissionTimes;
		}

		if ( pScore->uGage )
		{
			uTimes *= (KDuint) kdPowf ( 2.f, (KDfloat) pScore->uGage );
		}

		if ( pScore->uShake )
		{
			uTimes *= (KDuint) kdPowf ( 2.f, (KDfloat) pScore->uShake );
		}

		if ( pScore->uGo > 2 )
		{
			uTimes *= (KDuint) kdPowf ( 2.f, (KDfloat) ( pScore->uGo - 2 ) );
		}

		if ( pScore->uSkill )
		{
			uTimes *= (KDuint) kdPowf ( 3.f, (KDfloat) pScore->uSkill );
		}

		pScore->uTimes = uTimes;

		for ( j = 0; j < 3; j++ )
		{
			switch ( j )
			{
				case 0 : uValue = pScore->uGo;		break;
				case 1 : uValue = pScore->uShake;	break;
				case 2 : uValue = pScore->uBuck;	 break;
			}

			kdSprintfKHR ( szText, "%d", uValue );
			m_pTxtCount [ i ][ j ]->setText ( szText );
		}
	}

	this->onEvent ( );
}

KDvoid CScrGame::onRefreshTextScore ( KDvoid )
{
	KDuint   i;
	TScore*  pScore;
	KDchar   szText [ 64 ];
	
	for ( i = 0; i < 2; i++ )
	{
		pScore = &m_tScore [ i ];

		pScore->uScore  = pScore->uGo > 2 ? 2 : pScore->uGo;
		pScore->uGwang  = m_lObtain [ i ][ 0 ].size ( );
		pScore->uBGwang = m_lCard.at ( 44 )->getWhere ( ) == ( i == 0 ? eCard_Where_Obtain1 : eCard_Where_Obtain2 ) ? KD_TRUE : KD_FALSE;

		switch ( pScore->uGwang )
		{
			case 3 : pScore->uScore += pScore->uBGwang ? 2 : 3; break;
			case 4 : pScore->uScore += 4;  break;
			case 5 : pScore->uScore += 15; break;
		}

		pScore->uScore += pScore->uGodori   == 3 ? 5 : 0;
		pScore->uScore += pScore->uChoDan   == 3 ? 3 : 0;
		pScore->uScore += pScore->uHongDan  == 3 ? 3 : 0;
		pScore->uScore += pScore->uChungDan == 3 ? 3 : 0;

		pScore->uScore += pScore->u10Pi > 4 ? pScore->u10Pi - 4 : 0;
		pScore->uScore += pScore->u5Pi  > 4 ? pScore->u5Pi  - 4 : 0;
		pScore->uScore += pScore->u1Pi  > 9 ? pScore->u1Pi  - 9 : 0;

		kdSprintfKHR ( szText, "%04d", pScore->uScore );
		m_pTxtScore [ i ]->setText ( szText );
	}

	this->onEvent ( );
}

KDvoid CScrGame::onRefreshTextTimes ( KDvoid )
{
	KDuint  i;
	KDchar  szText [ 64 ];

	for ( i = 0; i < 2; i++ )
	{
		kdSprintfKHR ( szText, "%03d", m_tScore [ i ].uTimes );
		m_pTxtTimes [ i ]->setText ( szText );
	}

	this->onEvent ( );
}

KDvoid CScrGame::onRefreshTextObtain ( KDvoid )
{
	TScore*     pScore = KD_NULL;
	CCardList*  pList  = KD_NULL;
	CCard*      pCard  = KD_NULL;
	CText*      pText  = KD_NULL;
	KDuint      uCount = 0;
	KDuint      i, j, k;
	KDchar      szText [ 64 ];

	for ( i = 0; i < 2; i++ )
	{
		pScore = &m_tScore [ i ];

		for ( j = 0; j < 4; j++ )
		{
			pList = &m_lObtain    [ i ][ j ]; 
			pText =  m_pTxtObtain [ i ][ j ];

			if ( pList->size ( ) > 0 )
			{
				pText->setVisible ( KD_TRUE );

				if ( j == 3 )
				{
					uCount = 0;

					for ( k = 0; k < pList->size ( ); k++ )
					{
						pCard = pList->at ( k );

						switch ( pCard->getType ( ) )
						{
							case eCard_Type_3Pi :	uCount += 3;	break;
							case eCard_Type_2Pi :	uCount += 2;	break;
							default				:	uCount += 1;	break;
						}
					}

					pScore->u1Pi = uCount;
				}
				else
				{
					uCount = pList->size ( );
				}

				kdSprintfKHR ( szText, "%02d", uCount );
				pText->setText ( szText );
			}
			else
			{
				pText->setVisible ( KD_FALSE );
			}
		}
	}
}

KDvoid CScrGame::keepChongTong ( KDuint uMonth )
{
	CCard*  pCard = KD_NULL;

	for ( KDuint  i = 0; i < 4; i++ )
	{
		pCard = m_lCard.at ( uMonth * 4 + i );
		pCard->setFlag ( eCard_Flag_ChongTong );
	}

	m_tScore [ 0 ].bChongTongGo = KD_TRUE;

	this->postEvent ( eType_RefreshTimes );
	this->postEvent ( eType_Mission );
	this->waitEvent ( );
}

KDbool CScrGame::checkMission  ( KDvoid )
{
	CCardList*   pList   = &m_lMission;
	CCard*      pCard1  = KD_NULL;
	CCard*      pCard2  = KD_NULL;
	KDuint		uCount  = 0;
	KDuint      uCount1 = 0;
	KDuint      uCount2 = 0;
	ECardWhere  eWhere;
	KDuint      i;
	
	for ( i = 0; i < pList->size ( ); i++ )
	{
		pCard1 = pList->at ( i );
		pCard2 = (CCard *) pCard1->getUserData ( );

		eWhere = pCard2->getWhere ( );
		
		if ( eWhere == eCard_Where_Obtain1 || eWhere == eCard_Where_Obtain2 )
		{
			if ( pCard1->getFlag ( ) != (KDuint) eWhere )
			{
				pCard1->setFlag ( eWhere );

				pCard1->removeAllChildrenWithCleanup ( KD_TRUE );

				CCSprite*  pArrow = CCSprite::create 
				(
					g_pResManager->getPath ( eWhere == eCard_Where_Obtain1 ? eIMG_ETC_Mission_Arrow1 : eIMG_ETC_Mission_Arrow2 )
				);
				{
					CC_ASSERT ( pArrow );

					pCard1->addChild ( pArrow );

					const CCSize&  tSize = pCard1->getContentSize ( );

					pArrow->setPosition ( ccp ( tSize.cx / 2.f + 1.f, tSize.cy / 2.f - 1.f ) );
				}
			}

			uCount++;

			switch ( eWhere )
			{
				case eCard_Where_Obtain1 : uCount1++; break;
				case eCard_Where_Obtain2 : uCount2++; break;

				default : ;
			}
		}
	}

	if ( uCount == pList->size ( ) )
	{
		EPath  ePath;

		for ( i = 0; i < pList->size ( ); i++ )
		{
			pCard1 = pList->at ( i );
			pCard1->removeAllChildrenWithCleanup ( KD_TRUE );
		}

		if      ( uCount1 == uCount )
		{
			ePath = eIMG_ETC_Mission_Success1;
			m_tScore [ 0 ].uMission = m_uMissionTimes;
		}
		else if ( uCount2 == uCount )
		{
			ePath = eIMG_ETC_Mission_Success2;
			m_tScore [ 1 ].uMission = m_uMissionTimes;
		}
		else
		{
			ePath = eIMG_ETC_Mission_Fail;
		}

		CCSprite*  pResult = CCSprite::create 
		(
			g_pResManager->getPath ( ePath )
		);
		{
			CC_ASSERT ( pResult );

			this->addChild ( pResult, 2 );

			const CCSize&  tSize = pResult->getContentSize ( );

			pResult->setPosition ( ccp ( tSize.cx / 2.f + 10, tSize.cy / 2.f + 233.f ) );
		}

		this->postEvent ( eType_RefreshTimes );

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CScrGame::select ( CCard* pCard )
{
	CCardList*  pList;
	KDuint     uIndex;
	KDuint     i;

	if ( m_pSelect )
	{
		uIndex = this->findGround ( m_pSelect->getMonth ( ) );

		if ( uIndex != KDUINT_MAX )
		{
			pList = &m_lGround [ uIndex ];

			for ( i = 0; i < pList->size ( ); i++ )
			{
				pList->at ( i )->setSelect ( KD_FALSE );
			}
		}

		m_pSelect->setSelect ( KD_FALSE );
	}

	if ( pCard )
	{
		if ( m_uTurn == 1 )
		{
			m_pSelect = pCard;
		}

		if ( pCard == m_pSelect )
		{
			this->setTouchEnabled ( KD_FALSE );

			KDbool	bGage = m_uTurn == 0 && m_tScore [ 0 ].uGo > m_uGageCondition && pCard->isNormal ( ) && this->findGround ( pCard->getMonth ( ) ) != KDUINT_MAX;

			if ( bGage )
			{
				// If Gwang gage
				CDlgGage::s_pCard = pCard;

				g_pScrManager->setDialog ( eDlgGage );
			}
			else
			{
				this->putCard ( pCard );
			}
		}
		else
		{
			m_pSelect = pCard;

			if ( pCard->isNormal ( ) )
			{
				uIndex = this->findGround ( pCard->getMonth ( ) );

				if ( uIndex != KDUINT_MAX )
				{
					pList = &m_lGround [ uIndex ];

					for ( i = 0; i < pList->size ( ); i++ )
					{
						pList->at ( i )->setSelect ( KD_TRUE );
					}
				}
			}

			pCard->setSelect ( KD_TRUE );

			g_pResManager->playSound ( eSND_Etc_Focus );
		}
	}
	else
	{
		m_pSelect = KD_NULL;
	}
}

KDvoid CScrGame::putCard ( CCard* pCard )
{	
	CCardList*   pList  = &m_lPlayer [ m_uTurn ];
	ECardWhere  eWhere = pCard->getWhere ( );
	KDuint      uMonth = pCard->getMonth ( );
	KDuint      uCount = 0;
	KDuint      i;

	// If Bomb card
	if ( pCard->isBomb ( ) )
	{
		this->removeChild ( pCard, KD_TRUE );

		m_pSelect = KD_NULL;
		pList->remove ( pCard );
		m_lCard.remove ( pCard );

		this->postEvent ( eType_PutShuffled );
		this->waitEvent ( );
		
		return;
	}

	// If Bonus card
	if ( pCard->isBonus ( ) )
	{
		m_tPosition = pCard->getPosition ( );

		this->postEvent ( eType_CardToObtain, pCard->getNumber ( ) );
		this->postEvent ( m_uTurn == 0 ? eType_CardToPlayer1 : eType_CardToPlayer2, m_lShuffled.getLast ( )->getNumber ( ) );

		if ( m_uTurn == 0 )
		{
			this->postEvent ( eType_ReorderPlayer1 );
			this->postEvent ( eType_RefreshObtainable );
		}

		if ( this->steal1Pi ( 1 ) )
		{
			this->postEvent ( eType_Sound, m_uTurn == 0 ? eSND_Attack_Bonus : eSND_Harm_Bonus );

			if ( g_pEnvManager->rand ( ) < 20 )
			{
				this->postEvent ( eType_Talk, eTALK_Take );
			}
		}

		this->postEvent ( eType_PutPlayer );
		this->waitEvent ( );

		return;
	}

	// Find shake except ChonTong
	if ( !( pCard->getFlag ( ) & eCard_Flag_ChongTong ) )
	{
		for ( i = 0; i < 4; i++ )
		{
			if ( m_lCard.at ( uMonth * 4 + i )->getWhere ( ) == eWhere )
			{
				uCount++;
			}
		}

		if ( uCount == 3 )
		{
			m_tScore [ m_uTurn ].uShake++;

			this->postEvent ( eType_RefreshCount );
			this->postEvent ( eType_RefreshTimes );

			// If Bomb case
			if ( this->findGround ( uMonth ) != KDUINT_MAX )
			{
				for ( i = 0; i < 4; i++ )
				{
					CCard*  pCard2 = m_lCard.at ( uMonth * 4 + i );

					if ( pCard2->getWhere ( ) == eWhere )
					{
						if ( pCard != pCard2 )
						{
							CCard*  pBomb = CCard::create ( 51, m_uTurn == 0 ? eCard_Where_Player1 : eCard_Where_Player2 );
							{
								CC_ASSERT ( pBomb );

								this->addChild ( pBomb, 10, (KDuint) pBomb );

								pBomb->setPosition ( pCard2->getPosition ( ) );

								m_lCard.push_back ( pList->addFromParent ( pBomb ) );
							}

							pCard2->setFlag ( ( pCard2->getFlag ( ) & 0xff0f ) | eCard_Flag_Combo );
							this->postEvent ( eType_CardToGround, pCard2->getNumber ( ) );
						}

						if ( uCount == 1 )
						{
							pCard->setFlag ( ( pCard->getFlag ( ) & 0xff0f ) | eCard_Flag_Bomb );
							this->postEvent ( eType_CardToGround, pCard->getNumber ( ) );
							this->waitEvent ( );
							return;
						}
						
						uCount--;
					}
				}
			}
			else
			{
				// If Shake case 
				CDlgShake::s_pList = &m_lCard;
				CDlgShake::s_pCard = pCard;
				
				this->postEvent ( eType_Vibrate );
				this->postEvent ( eType_Dialog, eDlgShake );
				this->waitEvent ( );
				return;
			}
		}
	}

	this->postEvent ( eType_CardToGround, pCard->getNumber ( ) );
	this->postEvent ( eType_RefreshObtainable );
	this->waitEvent ( );
}

KDbool CScrGame::steal1Pi ( KDuint uCount )
{
	CCardList   lList [ 3 ]; 
	CCardList*  pList = &m_lObtain [ !m_uTurn ][ 3 ];
	CCard*      pCard = KD_NULL;
	KDuint      i, j;

	if ( uCount == 0 || pList->size ( ) == 0 )
	{
		return KD_FALSE;
	}

	for ( i = 0; i < pList->size ( ); i++ )
	{
		pCard = pList->at ( i );
		
		switch ( pCard->getType ( ) )
		{
			case eCard_Type_1Pi :	lList [ 0 ].push_back ( pCard );	break;
			case eCard_Type_2Pi :	lList [ 1 ].push_back ( pCard );	break;
			case eCard_Type_3Pi :	lList [ 2 ].push_back ( pCard );	break;
			default : ;
		}	
	}

	this->postEvent ( eType_Sound, eSND_Effect_Take );

	for ( i = 0; i < uCount; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			if ( lList [ j ].size ( ) )
			{
				pCard = lList [ j ].back ( );

				pCard->setFlag ( i < uCount - 1 ? eCard_Flag_Combo : eCard_Flag_Null );
				this->postEvent ( eType_CardToObtain, pCard->getNumber ( ) );

				lList [ j ].pop_back ( );

				break;
			}
		}
	}

	return KD_TRUE;
}

KDbool CScrGame::steal2Pi ( KDuint uCount )
{
	CCardList   lList [ 2 ]; 
	CCardList*  pList = &m_lObtain [ !m_uTurn ][ 3 ];
	CCard*      pCard = KD_NULL;
	KDuint      i, j;

	if ( uCount == 0 || pList->size ( ) == 0 )
	{
		return KD_FALSE;
	}

	for ( i = 0; i < pList->size ( ); i++ )
	{
		pCard = pList->at ( i );
		
		switch ( pCard->getType ( ) )
		{
			case eCard_Type_2Pi :	lList [ 0 ].push_back ( pCard );	break;
			case eCard_Type_3Pi :	lList [ 1 ].push_back ( pCard );	break;
			default : ;
		}	
	}

	this->postEvent ( eType_Sound, eSND_Effect_Take );

	for ( i = 0; i < uCount; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			if ( lList [ j ].size ( ) )
			{
				pCard = lList [ j ].back ( );

				pCard->setFlag ( i < uCount - 1 ? eCard_Flag_Combo : eCard_Flag_Null );
				this->postEvent ( eType_CardToObtain, pCard->getNumber ( ) );

				lList [ j ].pop_back ( );

				break;
			}
		}
	}

	return KD_TRUE;
}

KDbool CScrGame::stealGwang ( KDuint uCount )
{
	CCardList*  pList = &m_lObtain [ !m_uTurn ][ 0 ];
	CCard*      pCard = KD_NULL;
	KDuint      i;

	if ( uCount == 0 || pList->size ( ) == 0 )
	{
		return KD_FALSE;
	}

	if ( pList->size ( ) < uCount )
	{
		uCount = pList->size ( );
	}

	this->postEvent ( eType_Sound, eSND_Effect_Take );

	for ( i = 0; i < uCount; i++ )
	{
		pCard = pList->at ( pList->size ( ) - i - 1 );
		pCard->setFlag ( i < uCount - 1 ? eCard_Flag_Combo : eCard_Flag_Null );
		this->postEvent ( eType_CardToObtain, pCard->getNumber ( ) );
	}

	return KD_TRUE;
}

KDvoid CScrGame::swapCards ( KDvoid )
{
	std::vector<KDuint>  vIndex;

	CCardList*	pList1 = &m_lShuffled;
	CCardList*  pList2 = &m_lPlayer [ 0 ];
	CCard*		pCard1 = KD_NULL;
	CCard*		pCard2 = KD_NULL;
	KDuint		i, uIndex1, uIndex2;

	for ( i = 0; i < pList1->size ( ); i++ )
	{
		vIndex.push_back ( i );
	}

	for ( i = 0; i < pList2->size ( ); i++ )
	{
		uIndex1 = kdRand ( ) % vIndex.size ( );
		uIndex2 = vIndex [ uIndex1 ];
		vIndex.erase ( vIndex.begin ( ) + uIndex1 );

		pCard1 = pList1->at ( uIndex2 ); 
		pCard2 = pList2->at ( i );

		if ( pCard2->isBomb ( ) )
		{
			continue;
		}

		pCard1->setDark  ( KD_FALSE );
		pCard1->setArrow ( KD_FALSE );
		pCard1->setWhere ( eCard_Where_Player1 );

		pCard2->setDark  ( KD_FALSE );
		pCard2->setArrow ( KD_FALSE );
		pCard2->setWhere ( eCard_Where_Shuffled );

		m_lShuffled [ uIndex2 ] = pCard2;
		m_lPlayer   [ 0 ] [ i ] = pCard1;
	}

	for ( i = 0; i < pList1->size ( ); i++ )
	{
		pCard1 = pList1->at ( i );

		pCard1->setPosition ( ccp ( m_tWinSize.cx / 2.f, m_tWinSize.cy / 2.f + 50.f + i ) );
		pCard1->reorder ( i );
	}

	this->postEvent ( eType_ReorderPlayer1 );
	this->postEvent ( eType_RefreshObtainable );
	this->postEvent ( eType_PutPlayer );
}

KDuint CScrGame::getTurn ( KDvoid )
{
	return m_uTurn;
}

KDvoid CScrGame::setTurn ( KDuint uTurn )
{
	m_uTurn = uTurn;
}

TScore* CScrGame::getScore ( KDvoid )
{
	return &m_tScore [ m_uTurn ];
}

TScore* CScrGame::getScore ( KDuint uIndex )
{
	return &m_tScore [ uIndex ];
}

KDvoid CScrGame::setGostop ( KDbool bGo )
{
	KDuint  uIndex = 0;

	if ( bGo )
	{
		TScore*  pScore = &m_tScore [ m_uTurn ];
		KDbool   bTalk  = KD_FALSE;

		pScore->uGo++;
		uIndex = pScore->uGo - 1;

		this->postEvent ( eType_RefreshCount );

		if ( pScore->uGo > 2 )
		{
			this->postEvent ( eType_RefreshTimes );
		}
		else
		{
			pScore->uMax++;
			this->postEvent ( eType_RefreshScore );
		}

		this->postEvent ( eType_Effect, eIMG_Effect_1Go + uIndex );
		this->postEvent ( eType_Sound, ( m_uTurn == 0 ? eSND_Attack_1Go : eSND_Harm_1Go ) + uIndex );

		if      ( pScore->uGo < 2 && g_pEnvManager->rand ( ) < 30 )
		{
			bTalk = KD_TRUE;
		}
		else if ( pScore->uGo < 4 && g_pEnvManager->rand ( ) < 70 )
		{
			bTalk = KD_TRUE;
		}
		else
		{
			bTalk = KD_TRUE;
		}

		if ( bTalk )
		{
			this->postEvent ( eType_Talk, eTALK_1Go + ( uIndex == 1 ? 0 : uIndex ) );
		}

		this->postEvent ( eType_EndTurn );
	}
	else
	{
		this->postEvent ( eType_Effect, eIMG_Effect_Stop );
	
		if ( m_uTurn == 0 )
		{
			this->postEvent ( eType_Sound, m_tScore [ 0 ].uGo > 2 ? eSND_Attack_Stop2 : eSND_Attack_Stop );
		}

		this->postEvent ( eType_Talk, eTALK_Stop );
		this->postEvent ( eType_Result );
	}
}

TScore* CScrGame::getTotal ( KDvoid )
{
	TScore*  pScore1 = &m_tScore [  m_uTurn ];
	TScore*  pScore2 = &m_tScore [ !m_uTurn ];
	KDuint   uTimes  = 1;

	if ( pScore1->bChongTong )
	{
		kdMemset ( pScore1, 0, sizeof ( TScore ) );

		pScore1->uTimes = (KDuint) kdPowf ( 2.f, (KDfloat) g_pEnvManager->getPushCount ( ) );
		pScore1->uScore = 10;
	}
	else if ( pScore1->uBuck == 3 )
	{
		kdMemset ( pScore1, 0, sizeof ( TScore ) );

		pScore1->uTimes = (KDuint) kdPowf ( 2.f, (KDfloat) g_pEnvManager->getPushCount ( ) );
		pScore1->uScore = 7;
	}
	else
	{
		if ( pScore2->uGo > 0 )
		{ 
			pScore1->bGoBak = KD_TRUE;
			uTimes *= 2;
		}
		else
		{
			pScore1->bGoBak = KD_FALSE;
		}

		if ( pScore1->u1Pi > 9 && pScore2->u1Pi > 0 && pScore2->u1Pi < 7 )
		{
			pScore1->bPiBak = KD_TRUE;
			uTimes *= 2;
		}
		else
		{
			pScore1->bPiBak = KD_FALSE;
		}

		if ( pScore1->uGwang > 2 && pScore2->uGwang == 0 )
		{
			pScore1->bGwangBak = KD_TRUE;
			uTimes *= 2;
		}
		else
		{
			pScore1->bGwangBak = KD_FALSE;
		}

		if ( pScore1->u10Pi > 6 )
		{
			pScore1->bMungBak = KD_TRUE;
			uTimes *= 2;
		}
		else
		{
			pScore1->bMungBak = KD_FALSE;
		}
	}

	pScore1->uTotalTimes = pScore1->uTimes * uTimes;
	pScore1->uTotalScore = pScore1->uScore * pScore1->uTotalTimes;

	return pScore1;
}

KDbool CScrGame::aiGukJin ( KDvoid )
{
	CCard*  pCard = m_lCard.at ( 32 );

	// If MungBak Possible
	if ( m_lObtain [ 1 ][ 1 ].size ( ) < 6 )
	{
		pCard->setType ( eCard_Type_2Pi );
	}

	return KD_TRUE;
}

KDbool CScrGame::aiGoStop ( KDvoid )
{
	KDuint uPercent = 0;

	if ( m_tScore [ 0 ].uGo != 0 )
	{
		if ( m_tScore [ 0 ].uScore > 3 )
		{
			if ( m_lPlayer [ 1 ].size ( ) > 3 )
			{
				uPercent = 70;
			}
			else
			{
				uPercent = 10;
			}
		}
		else
		{
			uPercent = 80;
		}
	}

	if ( uPercent > 0 )
	{
		if ( g_pEnvManager->rand ( ) < uPercent )
		{
			return KD_TRUE;
		}
	}

	return KD_FALSE;
}

KDbool CScrGame::aiPush ( KDvoid )
{		
	if ( g_pEnvManager->getPushCount ( ) < 4 )
	{
		KDuint  uScore   = this->getTotal ( )->uTotalScore;
		KDuint  uPercent = 0;

		if ( uScore < 35 )
		{
			uPercent = 30;
		}
		else if ( uScore < 70 )
		{
			uPercent = 20;
		}
		else if ( uScore < 100 )
		{
			uPercent = 10;
		}
		else if ( uScore < 130 )
		{
			uPercent = 5;
		}

		if ( uPercent > 0 )
		{
			if ( g_pEnvManager->rand ( ) < uPercent )
			{
				return KD_TRUE;
			}
		}
	}

	return KD_FALSE;
}

CCard* CScrGame::aiPutCard ( KDvoid )
{
	KDuint      uStage   = g_pEnvManager->getStage ( );
	KDuint		uAiLevel = 99;
	KDuint      uHeading = 0;
	KDuint		uZock    = 0;

	CCard*      pCard = KD_NULL;
	CCardList*  pList = KD_NULL;
	CCardList   tList;
	KDuint      i;

	if ( m_uTurn == 1 )
	{
		switch ( uStage )
		{
			case 0  : uHeading = 40; uAiLevel = kdRand ( ) % 400; break;
			case 1  : uHeading = 30; uAiLevel = kdRand ( ) % 350; break;
			case 2  : uHeading = 30; uAiLevel = kdRand ( ) % 300; break;
			case 3  : uHeading = 25; uAiLevel = kdRand ( ) % 250; break;
			case 4  : uHeading = 20; uAiLevel = kdRand ( ) % 200; break;
			case 5  : uHeading = 15; uAiLevel = kdRand ( ) % 150; break;
			case 6  : uHeading = 10; uAiLevel = kdRand ( ) % 150; uZock = 10; break;
			case 7  : uZock = 20;
			case 8  : uZock = 25;
		}
	}

	// If Heading
	if ( uHeading > 0 && g_pEnvManager->rand ( ) < uHeading )
	{
		if ( ( pCard = this->getHeading ( ) ) )
		{
			return pCard;
		}
	}

	// If Bonus timing
	if ( m_uTurn == 0 || ( uStage > 4 && g_pEnvManager->rand ( ) < 50 ) )
	{
		if ( ( pCard = this->getBonusTiming ( ) ) )
		{
			return pCard;
		}
	}

	tList = this->getObtainable ( );

	// If Obtainable
	if ( tList.size ( ) )
	{
		if ( uAiLevel < 100 )
		{
			// If Godori possible
			if ( ( pCard = this->getPossible ( 4, 12, 29 ) ) )
			{
				if ( ( pCard = this->getObtainable ( pCard ) ) )
				{
					return pCard;
				}
			}

			// If HongDan possible
			if ( ( pCard = this->getPossible ( 1, 5, 9 ) ) )
			{
				if ( ( pCard = this->getObtainable ( pCard ) ) )
				{
					return pCard;
				}
			}

			// If ChoDan possible
			if ( ( pCard = this->getPossible ( 13, 17, 25 ) ) )
			{
				if ( ( pCard = this->getObtainable ( pCard ) ) )
				{
					return pCard;
				}
			}

			// If ChungDan possible
			if ( ( pCard = this->getPossible ( 21, 33, 37 ) ) )
			{
				if ( ( pCard = this->getObtainable ( pCard ) ) )
				{
					return pCard;
				}
			}

			// If 2Point or GukJin
			pList = &m_lObtain [ m_uTurn ][ 3 ];
			for ( i = 0; i < pList->size ( ); i++ )
			{
				pCard = pList->at ( i );
				if ( pCard->getType ( ) == eCard_Type_1Pi )
				{
					if      ( ( pCard = this->getObtainable ( m_lCard.at ( 41 ) ) ) )
					{
						return pCard;
					}
					else if ( ( pCard = this->getObtainable ( m_lCard.at ( 47 ) ) ) )
					{
						return pCard;
					}
					else if ( ( pCard = this->getObtainable ( m_lCard.at ( 32 ) ) ) )
					{
						return pCard;
					}

					break;
				}
			}

			// If Gwang
			if      ( ( pCard = this->getObtainable ( m_lCard.at ( 0 ) ) ) )
			{
				return pCard;
			}
			else if ( ( pCard = this->getObtainable ( m_lCard.at ( 8 ) ) ) )
			{
				return pCard;
			}
			else if ( ( pCard = this->getObtainable ( m_lCard.at ( 28 ) ) ) )
			{
				return pCard;
			}
			else if ( ( pCard = this->getObtainable ( m_lCard.at ( 40 ) ) ) )
			{
				return pCard;
			}
			else if ( ( pCard = this->getObtainable ( m_lCard.at ( 44 ) ) ) )
			{
				return pCard;
			}
		}

		// Random
		pCard = tList.at ( kdRand ( ) % tList.size ( ) );
	}
	else
	{
		// If Zock
		if ( uZock > 0 && g_pEnvManager->rand ( ) < uZock )
		{
			if ( ( pCard = this->getZock ( ) ) )
			{
				return pCard;
			}
		}

		if ( uAiLevel < 100 )
		{
			// If Bonus
			pList = &m_lObtain [ m_uTurn ][ 3 ];
			for ( i = 0; i < pList->size ( ); i++ )
			{
				pCard = pList->at ( i );
				if ( pCard->getType ( ) == eCard_Type_1Pi )
				{
					pList = &m_lPlayer [ m_uTurn ];
					for ( i = 0; i < pList->size ( ); i++ )
					{
						pCard = pList->at ( i );

						if ( pCard->isBonus ( ) )
						{
							return pCard;
						}
					}

					break;
				}
			}

			// If Bomb
			pList = &m_lPlayer [ m_uTurn ];
			for ( i = 0; i < pList->size ( ); i++ )
			{
				pCard = pList->at ( i );

				if ( pCard->isBomb ( ) )
				{
					return pCard;
				}
			}

			// Except
			pList = &m_lPlayer [ m_uTurn ];
			for ( i = 0; i < pList->size ( ); i++ )
			{
				pCard = pList->at ( i );

				switch ( pCard->getNumber ( ) )
				{
					case  0 :
					case  8 :
					case 28 :
					case 29 :
					case 32 :
					case 40 :
					case 41 :
						break;

					default : 
						return pCard;
				}
			}
		}

		// Random
		pList = &m_lPlayer [ m_uTurn ];
		pCard = pList->at ( kdRand ( ) % pList->size ( ) );
	}

	return pCard;
}

CCard* CScrGame::aiSelect ( CCard* pCard1, CCard* pCard2 )
{
	CCard*  pCard [ 2 ] = { pCard1, pCard2 };
	KDuint  uNum  [ 2 ] = { pCard1->getNumber ( ), pCard2->getNumber ( ) };

	KDbool  bGodori		= this->getPossible (  4, 12, 29 ) != KD_NULL;
	KDbool  bChoDan		= this->getPossible ( 13, 17, 25 ) != KD_NULL;
	KDbool  bHongDan	= this->getPossible (  1,  5,  9 ) != KD_NULL;
	KDbool  bChungDan	= this->getPossible ( 21, 33, 37 ) != KD_NULL;

	KDuint  i;
	
	for ( i = 0; i < 2; i++ )
	{
		if ( bGodori && ( uNum [ i ] == 4 || uNum [ i ] == 12 || uNum [ i ] == 29 ) )
		{
			return pCard [ i ];
		}

		if ( bChoDan && ( uNum [ i ] == 13 || uNum [ i ] == 17 || uNum [ i ] == 25 ) )
		{
			return pCard [ i ];
		}

		if ( bHongDan && ( uNum [ i ] == 1 || uNum [ i ] == 5 || uNum [ i ] == 9 ) )
		{
			return pCard [ i ];
		}

		if ( bChungDan && ( uNum [ i ] == 21 || uNum [ i ] == 33 || uNum [ i ] == 37 ) )
		{
			return pCard [ i ];
		}

		if ( pCard [ i ]->getType ( ) == eCard_Type_2Pi )
		{
			return pCard [ i ];
		}

		if ( uNum [ i ] == 45 || uNum [ i ] == 46 )
		{
			return pCard [ !i ];
		}
	}

	return pCard [ kdRand ( ) % 2 ];
}

KDuint CScrGame::findGround ( KDuint uMonth )
{
	for ( KDuint  i = 0; i < 12; i++ )
	{
		CCardList*  pList = &m_lGround [ i ];

		if ( pList->size ( ) > 0 )
		{
			if ( uMonth < 12 )
			{
				if ( pList->at ( 0 )->getMonth ( ) == uMonth )
				{
					return i;
				}
			}
			else
			{
				for ( KDuint  j = 0; j < pList->size ( ); j++ )
				{
					if ( pList->at ( j )->getMonth ( ) == uMonth )
					{
						return i;
					}
				}
			}
		}
	}

	return KDUINT_MAX;
}

KDuint CScrGame::countGround ( KDuint uIndex )
{
	CCardList*  pList  = &m_lGround [ uIndex ];
	CCard*     pCard  = KD_NULL;
	KDuint     uCount = 0;

	KDuint  i;

	for ( i = 0; i < pList->size ( ); i++ )
	{
		pCard = pList->at ( i );
		if ( pCard->isNormal ( ) )
		{
			uCount++;
		}
	}

	return uCount;
}

CCardList CScrGame::getObtainable  ( KDvoid )
{
	CCardList*  pList  = &m_lPlayer [ m_uTurn ];
	CCard*     pCard  = KD_NULL;
	CCard*     pCard2 = KD_NULL;

	CCardList   tList;
	KDuint     uMonth;

	KDuint  i, j;

	for ( i = 0; i < pList->size ( ); i++ )
	{
		pCard = pList->at ( i );
		if ( pCard->isNormal ( ) )
		{
			uMonth = pCard->getMonth ( );
			for ( j = 0; j < 4; j++ )
			{
				pCard2 = m_lCard.at ( uMonth * 4 + j );
				if ( pCard2->getWhere ( ) == eCard_Where_Ground )
				{
					tList.push_back ( pCard );
					break;
				}
			}
		}
	}

	return tList;
}

CCard* CScrGame::getObtainable ( CCard* pCard )
{
	CCardList*   pList  = KD_NULL;
	CCard*      pCard2 = KD_NULL;
	KDuint      uMonth = pCard->getMonth ( );
	ECardWhere  eWhere = pCard->getWhere ( );

	if ( eWhere == eCard_Where_Ground )
	{
		pList = &m_lPlayer [ m_uTurn ];
		for ( KDuint  i = 0; i < pList->size ( ); i++ )
		{
			pCard2 = pList->at ( i );
			if ( pCard2->getMonth ( ) == uMonth )
			{
				return pCard2;
			}
		}
	}
	else if ( eWhere == ( m_uTurn == 0 ? eCard_Where_Player1 : eCard_Where_Player2 ) )
	{
		if ( this->findGround ( uMonth ) != KDUINT_MAX )
		{
			return pCard;
		}
	}

	return KD_NULL;
}

CCard* CScrGame::getPossible ( KDuint uNum1, KDuint uNum2, KDuint uNum3 )
{
	CCard*  	pList [ 3 ] = { m_lCard [ uNum1 ], m_lCard [ uNum2 ], m_lCard [ uNum3 ] };
	CCard*      pCard = KD_NULL;
	ECardWhere  eWhere;

	KDuint	uCount [ 2 ] = { 0, 0 };

	for ( KDuint  i = 0; i < 3; i++ )
	{
		eWhere = pList [ i ]->getWhere ( );

		if ( eWhere == eCard_Where_Obtain1 )
		{
			uCount [ m_uTurn ]++;
		}
		else if ( eWhere == eCard_Where_Obtain2 )
		{
			uCount [ !m_uTurn ]++;
		}
		else
		{
			pCard = pList [ i ];
		}
	}

	if ( uCount [ 0 ] == 2 && uCount [ 1 ] == 0 && pCard )
	{
		return pCard;
	}

	return KD_NULL;
}

CCard* CScrGame::getBonusTiming ( KDvoid )
{
	CCardList*  pList = &m_lObtain [ !m_uTurn ][ 3 ];
	CCard*     pCard = KD_NULL;

	if ( pList->size ( ) > 0 )
	{
		for ( KDuint  i = 0; i < pList->size ( ); i++ )
		{
			pCard = pList->at ( i );

			if ( pCard->getType ( ) == eCard_Type_1Pi )
			{
				return KD_NULL;
			}
		}

		return this->getBonus ( );
	}

	return KD_NULL;
}

CCard* CScrGame::getBonus ( KDvoid )
{
	CCardList*  pList = &m_lPlayer [ m_uTurn ];
	CCard*     pCard = KD_NULL;

	for ( KDuint  i = 0; i < pList->size ( ); i++ )
	{
		pCard = pList->at ( i );
		if ( pCard->isBonus ( ) )
		{
			return pCard;
		}
	}

	return KD_NULL;
}

CCard* CScrGame::getBomb ( KDvoid )
{
	CCardList*  pList = &m_lPlayer [ m_uTurn ];
	CCard*     pCard = KD_NULL;

	for ( KDuint  i = 0; i < pList->size ( ); i++ )
	{
		pCard = pList->at ( i );
		if ( pCard->isBomb ( ) )
		{
			return pCard;
		}
	}

	return KD_NULL;
}

CCard* CScrGame::getHeading ( KDvoid )
{
	CCardList*  pList = &m_lPlayer [ m_uTurn ];
	CCard*      pCard = KD_NULL;

	for ( KDuint  i = 0; i < pList->size ( ); i++ )
	{
		pCard = pList->at ( i );

		if ( pCard->isNormal ( ) && this->findGround ( pCard->getMonth ( ) ) == KDUINT_MAX )
		{
			return pCard;
		}
	}

	return KD_NULL;
}

CCard* CScrGame::getZock ( KDvoid )
{
	CCardList*  pList = &m_lPlayer [ m_uTurn ];
	CCard*      pCard = KD_NULL;
	CCard*		pZock = m_lShuffled.back ( );

	if ( pZock->isNormal ( ) )
	{
		for ( KDuint  i = 0; i < pList->size ( ); i++ )
		{
			pCard = pList->at ( i );

			if ( pCard->isNormal ( ) && pCard->getMonth ( ) == pZock->getMonth ( ) )
			{
				return pCard;
			}
		}
	}

	return KD_NULL;
}

KDbool CScrGame::isSkillAvailable ( KDuint uIndex )
{
	KDbool  bAvailable = KD_FALSE;

	if ( m_uSkillAvailable [ uIndex ] > 0 )
	{
		switch ( uIndex )
		{
			case 0 :	// Open Shuffled

				if ( m_lShuffled.back ( )->getFront ( ) == KD_FALSE )
				{
					bAvailable = KD_TRUE;
				}

				break;

			case 1 :	// Steal 2Pi

				for ( KDuint  i = 0; i < m_lObtain [ 1 ][ 3 ].size ( ); i++ )
				{
					CCard*  pCard = m_lObtain [ 1 ][ 3 ].at ( i );
					if ( pCard->getType ( ) == eCard_Type_3Pi || pCard->getType ( ) == eCard_Type_2Pi )
					{
						bAvailable = KD_TRUE;
						break;
					}
				}

				break;

			case 4 :
			case 5 :	// Steal Gwang

				if ( m_lObtain [ 1 ][ 0 ].size ( ) > 0 )
				{
					bAvailable = KD_TRUE;
				}

				break;

			case 8 :	// Gwang Gage Condition

				if ( m_tScore [ 0 ].uGo < 3 )
				{
					bAvailable = KD_TRUE;
				}

				break;

			case 9 :	// Gwang Gage Speed

				if ( m_tScore [ 0 ].uGo > m_uGageCondition )
				{
					bAvailable = KD_TRUE;
				}

				break;

			default :

				bAvailable = KD_TRUE;

				break;
		}
	}

	return bAvailable;
}

KDvoid CScrGame::useSkill ( KDuint uIndex )
{
	KDuint    uCount = g_pEnvManager->getSkill ( uIndex );
	CCard*    pCard  = KD_NULL;
	KDuint	  i;

	g_pEnvManager->setSkill ( uIndex, uCount - 1 );
	g_pEnvManager->save ( );
	m_uSkillAvailable [ uIndex ]--;
	
	switch ( uIndex )
	{
		case 0 :	// Open Shuffled

			pCard = m_lShuffled.back ( );

			pCard->setFront ( KD_TRUE );
			pCard->setSize  ( eCard_Size_Medium );

			break;

		case 1 :	// Steal 2Pi

			this->steal2Pi ( 1 );
			this->postEvent ( eType_RefreshScore );
			this->waitEvent ( );

			break;

		case 2 :	// Enemy Push

			break;

		case 3 :	// Restart Game

			g_pScrManager->setScene ( eScrGame, eTransSlideInR );

			break;

		case 4 :	// Steal Gwang All

			this->stealGwang ( 5 );
			this->postEvent ( eType_RefreshScore );
			this->waitEvent ( );

			break;

		case 5 :	// Steal Gwang

			this->stealGwang ( 1 );
			this->postEvent ( eType_RefreshScore );
			this->waitEvent ( );

			break;

		case 6 :	// Swap Cards

			this->swapCards ( );
			this->waitEvent ( );

			break;

		case 7 :	// 3 Times

			for ( i = 0; i < 2; i++ )
			{
				m_tScore [ i ].uSkill++;
			}

			this->postEvent ( eType_RefreshTimes );
			this->waitEvent ( );

			break;

		case 8 :	// Gwang Gage Condition

			m_uGageCondition = 0;
			
			break;

		case 9 :	// Gwang Gage Speed

			CDlgGage::s_fTiming = 1.2f;

			break;
	}
}
