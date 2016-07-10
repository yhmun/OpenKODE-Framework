/* -----------------------------------------------------------------------------------
 *
 *      File            CasinoResultNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "CasinoResultNode.h"
#include "MyData/MyData.h"
#include "Object/Menu/SoundMenuItem.h"
#include "Object/TimeGetMoney/TimeGetMoney.h"
#include "CasinoNode/CasinoBattleNode/CasinoBattleNode.h"
#include "CasinoNode/CasinoBetNode/CasinoBetNode.h"
#include "MyRoomNode/MyRoomNode.h"

#if 0
#import "OpenFeint+Dashboard.h"
#import "OFHighScoreService.h"
#import "OFAchievementService.h"
#endif	// 0

static CasinoResultNode*	l_pCasinoResultNode = nullptr;

cResultLevelUp::cResultLevelUp ( Node* pNode, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt )
: m_pUILevelUp			( nullptr	)
, m_pUIStatusHp			( nullptr	)
, m_pUIStatusAtk		( nullptr	)
, m_pUIStatusDef		( nullptr	)
, m_pUIStatusAgl		( nullptr	)
, m_pUIStatusCrt		( nullptr	)
, m_bIsStart			( false		)
, m_bIsLevelAlphaAdd	( true		)
, m_dLevelUpLeftTime	( 1			)
, m_dStatusLeftTime		( 1.2f		)
, m_nStatusStep			( 0			)
, m_nHp					( nHp		)
, m_nAtk				( nAtk		)
, m_nDef				( nDef		)
, m_nAgl				( nAgl		)
, m_nCrt				( nCrt		)
{
	// level up UI
	m_pUILevelUp	= new cSprite ( pNode, "CasinoResult/casino_result_lvup.png", LAYER_CASINO_RESULT_BAR_UI, Point ( 0, 0 ), Point ( 15, 214 ) );
	m_pUIStatusHp	= new cSprAni ( pNode, "Game/status_effects.png", LAYER_CASINO_RESULT_BAR_UI, Point ( 65, 216 ) );
	m_pUIStatusAtk	= new cSprAni ( pNode, "Game/status_effects.png", LAYER_CASINO_RESULT_BAR_UI, Point ( 65, 216 ) );
	m_pUIStatusDef	= new cSprAni ( pNode, "Game/status_effects.png", LAYER_CASINO_RESULT_BAR_UI, Point ( 65, 216 ) );
	m_pUIStatusAgl	= new cSprAni ( pNode, "Game/status_effects.png", LAYER_CASINO_RESULT_BAR_UI, Point ( 65, 216 ) );
	m_pUIStatusCrt	= new cSprAni ( pNode, "Game/status_effects.png", LAYER_CASINO_RESULT_BAR_UI, Point ( 65, 216 ) );

	m_pUIStatusHp ->addFrame ( Rect ( 353,  0, 58, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
	m_pUIStatusAtk->addFrame ( Rect ( 146,  0, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
	m_pUIStatusDef->addFrame ( Rect ( 286,  0, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
	m_pUIStatusAgl->addFrame ( Rect (  73, 30, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
	m_pUIStatusCrt->addFrame ( Rect ( 213, 30, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );

	m_pUIStatusHp ->setVisible ( false );	m_pUIStatusHp ->setFrameInit ( 0 );		m_pUIStatusHp ->setLoop ( false );		m_pUIStatusHp ->stop ( );
	m_pUIStatusAtk->setVisible ( false );	m_pUIStatusAtk->setFrameInit ( 0 );		m_pUIStatusAtk->setLoop ( false );		m_pUIStatusAtk->stop ( );
	m_pUIStatusDef->setVisible ( false );	m_pUIStatusDef->setFrameInit ( 0 );		m_pUIStatusDef->setLoop ( false );		m_pUIStatusDef->stop ( );
	m_pUIStatusAgl->setVisible ( false );	m_pUIStatusAgl->setFrameInit ( 0 );		m_pUIStatusAgl->setLoop ( false );		m_pUIStatusAgl->stop ( );
	m_pUIStatusCrt->setVisible ( false );	m_pUIStatusCrt->setFrameInit ( 0 );		m_pUIStatusCrt->setLoop ( false );		m_pUIStatusCrt->stop ( );

	m_pUILevelUp->get ( )->setOpacity ( 0 );
}

cResultLevelUp::~cResultLevelUp ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pUILevelUp	 );
	CC_SAFE_RELEASE ( m_pUIStatusHp	 );
	CC_SAFE_RELEASE ( m_pUIStatusAtk );
	CC_SAFE_RELEASE ( m_pUIStatusDef );
	CC_SAFE_RELEASE ( m_pUIStatusAgl );
	CC_SAFE_RELEASE ( m_pUIStatusCrt );
}

KDvoid cResultLevelUp::start ( KDvoid )
{
	m_bIsStart = true;
}

KDint cResultLevelUp::update ( KDdouble dLeftTime )
{
	// check
	if ( m_bIsStart == false )
	{
		return -1;
	}

	// level up
	if ( m_bIsLevelAlphaAdd == true )
	{
		m_dLevelUpLeftTime -= dLeftTime;
		if ( m_dLevelUpLeftTime < 0 )
		{
			KDint	nAlpha = m_pUILevelUp->get ( )->getOpacity ( ) + 10;
			if ( nAlpha > 255 )
			{
				m_bIsLevelAlphaAdd = false;
				m_dLevelUpLeftTime = 4;
				nAlpha = 255;
			}

			m_pUILevelUp->get ( )->setOpacity ( nAlpha );
		}
	}
	else
	{
		m_dLevelUpLeftTime -= dLeftTime;
		if ( m_dLevelUpLeftTime < 0 )
		{
			KDint	nAlpha = m_pUILevelUp->get ( )->getOpacity ( ) - 10;
			if ( nAlpha < 0 )	
			{
				nAlpha = 0;
			}
			m_pUILevelUp->get ( )->setOpacity ( nAlpha );
		}
	}

	// status effect
	if ( m_nStatusStep == 0 )
	{
		m_dStatusLeftTime -= dLeftTime;

		if ( m_dStatusLeftTime < 0 )
		{
			if ( m_nHp != 0 )
			{
				m_pUIStatusHp->setVisible ( true );
				m_pUIStatusHp->setPointTime ( Point ( 65, 256 ), 1 );

				m_dStatusLeftTime = 1;
			}
			else
			{
				m_dStatusLeftTime = 0;
			}

			++m_nStatusStep;

			return 1;
		}
	}
	else if ( m_nStatusStep == 1 )
	{
		m_dStatusLeftTime -= dLeftTime;

		KDint	nAlpha = m_pUIStatusHp->getAlpha ( ) - 3;	
		if ( nAlpha < 0 )
		{
			nAlpha = 0;
		}
		m_pUIStatusHp->setAlpha ( nAlpha );

		if ( m_dStatusLeftTime < 0 )
		{
			if ( m_nAtk != 0 )
			{
				m_pUIStatusAtk->setVisible ( true );
				m_pUIStatusAtk->setPointTime ( Point ( 65, 256 ), 1 );

				m_dStatusLeftTime = 1;
			}
			else
			{
				m_dStatusLeftTime = 0;
			}
			
			++m_nStatusStep;

			return 2;
		}
	}
	else if ( m_nStatusStep == 2 )
	{
		m_dStatusLeftTime -= dLeftTime;

		KDint	nAlpha1 = m_pUIStatusHp	->getAlpha ( ) - 3;		if ( nAlpha1 < 0 )	nAlpha1 = 0;
		KDint	nAlpha2 = m_pUIStatusAtk->getAlpha ( ) - 3;		if ( nAlpha2 < 0 )	nAlpha2 = 0;

		m_pUIStatusHp ->setAlpha ( nAlpha1 );
		m_pUIStatusAtk->setAlpha ( nAlpha2 );

		if ( m_dStatusLeftTime < 0 )
		{
			if ( m_nDef != 0 )
			{
				m_pUIStatusDef->setVisible ( true );
				m_pUIStatusDef->setPointTime ( Point ( 65, 256 ), 1 );

				m_dStatusLeftTime = 1;
			}
			else
			{
				m_dStatusLeftTime = 0;
			}

			++m_nStatusStep;

			return 3;
		}
	}
	else if ( m_nStatusStep == 3 )
	{
		m_dStatusLeftTime -= dLeftTime;

		KDint		nAlpha1 = m_pUIStatusHp	->getAlpha ( ) - 3;		if ( nAlpha1 < 0 )	nAlpha1 = 0;
		KDint		nAlpha2 = m_pUIStatusAtk->getAlpha ( ) - 3;		if ( nAlpha2 < 0 )	nAlpha2 = 0;
		KDint		nAlpha3 = m_pUIStatusDef->getAlpha ( ) - 3;		if ( nAlpha3 < 0 )	nAlpha3 = 0;

		m_pUIStatusHp ->setAlpha ( nAlpha1 );
		m_pUIStatusAtk->setAlpha ( nAlpha2 );
		m_pUIStatusDef->setAlpha ( nAlpha3 );

		if ( m_dStatusLeftTime < 0 )
		{
			if ( m_nAgl != 0 )
			{
				m_pUIStatusAgl->setVisible ( true );
				m_pUIStatusAgl->setPointTime ( Point ( 65, 256 ), 1 );

				m_dStatusLeftTime = 1;
			}
			else
			{
				m_dStatusLeftTime = 0;
			}

			++m_nStatusStep;

			return 4;
		}
	}
	else if ( m_nStatusStep == 4)
	{
		m_dStatusLeftTime -= dLeftTime;

		KDint		nAlpha1 = m_pUIStatusHp	->getAlpha ( ) - 3;		if ( nAlpha1 < 0 )	nAlpha1 = 0;
		KDint		nAlpha2 = m_pUIStatusAtk->getAlpha ( ) - 3;		if ( nAlpha2 < 0 )	nAlpha2 = 0;
		KDint		nAlpha3 = m_pUIStatusDef->getAlpha ( ) - 3;		if ( nAlpha3 < 0 )	nAlpha3 = 0;
		KDint		nAlpha4 = m_pUIStatusAgl->getAlpha ( ) - 3;		if ( nAlpha4 < 0 )	nAlpha4 = 0;

		m_pUIStatusHp ->setAlpha ( nAlpha1 );
		m_pUIStatusAtk->setAlpha ( nAlpha2 );
		m_pUIStatusDef->setAlpha ( nAlpha3 );
		m_pUIStatusAgl->setAlpha ( nAlpha4 );

		if ( m_dStatusLeftTime < 0 )
		{
			if ( m_nCrt != 0 )
			{
				m_pUIStatusCrt->setVisible ( true );
				m_pUIStatusCrt->setPointTime ( Point ( 65, 256 ), 1 );

				m_dStatusLeftTime = 1;
			}
			else
			{
				m_dStatusLeftTime = 0;
			}

			++m_nStatusStep;

			return 5;
		}
	}
	else
	{
		KDint	nAlpha1 = m_pUIStatusHp	->getAlpha ( ) - 3;		if ( nAlpha1 < 0 )	nAlpha1 = 0;
		KDint	nAlpha2 = m_pUIStatusAtk->getAlpha ( ) - 3;		if ( nAlpha2 < 0 )	nAlpha2 = 0;
		KDint	nAlpha3 = m_pUIStatusDef->getAlpha ( ) - 3;		if ( nAlpha3 < 0 )	nAlpha3 = 0;
		KDint	nAlpha4 = m_pUIStatusAgl->getAlpha ( ) - 3;		if ( nAlpha4 < 0 )	nAlpha4 = 0;
		KDint	nAlpha5 = m_pUIStatusCrt->getAlpha ( ) - 3;		if ( nAlpha5 < 0 )	nAlpha5 = 0;

		m_pUIStatusHp ->setAlpha ( nAlpha1 );
		m_pUIStatusAtk->setAlpha ( nAlpha2 );
		m_pUIStatusDef->setAlpha ( nAlpha3 );
		m_pUIStatusAgl->setAlpha ( nAlpha4 );
		m_pUIStatusCrt->setAlpha ( nAlpha4 );
	}

	m_pUIStatusHp ->update ( dLeftTime );
	m_pUIStatusAtk->update ( dLeftTime );
	m_pUIStatusDef->update ( dLeftTime );
	m_pUIStatusAgl->update ( dLeftTime );
	m_pUIStatusCrt->update ( dLeftTime );

	return -1;
}

CasinoResultNode* CasinoResultNode::createWithFileLoad ( eIntoType eIntro )
{
	CasinoResultNode*	pRet = new CasinoResultNode ( );

	if ( pRet && pRet->initWithFileLoad ( eIntro ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CasinoResultNode* CasinoResultNode::createWithBetMob ( cUnit* pBetMob, eModeType eType )
{
	CasinoResultNode*	pRet = new CasinoResultNode ( );

	if ( pRet && pRet->initWithBetMob ( pBetMob, eType ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CasinoResultNode::initWithFileLoad ( eIntoType eIntro )
{
	if ( !this->init ( ) )
	{
		return false;
	}

	// file load
	this->setFileLoad ( eIntro );

	// init
	this->initUI ( );
	this->initFinish ( );

	return true;
}

KDbool CasinoResultNode::initWithBetMob ( cUnit* pBetMob, eModeType eType )
{
	if ( !this->init ( ) )
	{
		return false;
	}

	// set betting mob
	this->setBetMob ( pBetMob, eType );

	// my data
	this->updateMyData ( );

	// init
	this->initUI ( );

	//level
	if ( eType == eModeType_Casino )
	{
		this->levelUp ( );
	}
	else
	{
		this->plusExp ( cMyData::getObj ( )->m_fGamingExp );
	}

	// init fisish
	this->initFinish ( );

	// openfeint
	this->updateOpenFeint ( );

	// gaming exp & kill count
	cMyData::getObj ( )->m_fGamingExp		= 0;
	cMyData::getObj ( )->m_nGamingKillCount = 0;

	return true;
}

KDvoid CasinoResultNode::saveData ( KDvoid )
{
	if ( l_pCasinoResultNode )
	{
		l_pCasinoResultNode->save ( );
	}
}

CasinoResultNode::CasinoResultNode ( KDvoid )
{
	l_pCasinoResultNode			= this;

	m_pSprBack					= nullptr;
	m_pNodeChangeImg			= nullptr;
	m_pLabels					= nullptr;
	m_pBetMob					= nullptr;
	m_pSprResultBack			= nullptr;
	m_pLabelTopMoney			= nullptr;
	m_pSprResultText			= nullptr;
	m_pLabelEarnMoney			= nullptr;
	m_pLabelResultMoney			= nullptr;
	m_pUIMobBar					= nullptr;
	m_pUIMobBarMagnification	= nullptr;
	m_pUIMobBarMagnification2	= nullptr;
	m_pUIMobBarMagnification3	= nullptr;
	m_pUIMobBarHP				= nullptr;
	m_pUIMobBarATK				= nullptr;
	m_pUIMobBarDEF				= nullptr;
	m_pUIBarExpGauge1			= nullptr;
	m_pUIBarExpGauge2			= nullptr;
	m_pUIBarExpFont1			= nullptr;
	m_pUIBarExpFont2			= nullptr;
	m_pUIBarKill				= nullptr;
	m_pUISuccessiveNum			= nullptr;
	m_pBtnExit					= nullptr;
	m_pBtnRetry					= nullptr;
	m_pBtnRank					= nullptr;
	m_pUIExpBack				= nullptr;
	m_pUIExpGauge				= nullptr;
	m_pUIExpFont				= nullptr;
	m_pUIMobNum					= nullptr;
}

CasinoResultNode::~CasinoResultNode ( KDvoid )
{
	this->releaseObject ( );

	// static
	l_pCasinoResultNode = nullptr;
}

KDvoid CasinoResultNode::releaseObject ( KDvoid )
{
	// my room data save
	this->updateMyRoomData ( );

	// touch

	CC_SAFE_RELEASE ( m_pNodeChangeImg );

	// back img
	SAFE_REMOVE_CHILD ( m_pSprBack );

	// back decorations
	SAFE_RELEASE_ARRAYS ( m_aBackDecos );

	// labels
	CC_SAFE_RELEASE ( m_pLabels );

	// mobs
	CC_SAFE_RELEASE ( m_pBetMob );

	// result back
	CC_SAFE_RELEASE ( m_pSprResultBack );

	// top money
	CC_SAFE_RELEASE ( m_pLabelTopMoney );

	// result text
	CC_SAFE_RELEASE ( m_pSprResultText );

	// result money
	CC_SAFE_RELEASE ( m_pLabelEarnMoney	);
	CC_SAFE_RELEASE ( m_pLabelResultMoney );

	// bar UI
	CC_SAFE_RELEASE ( m_pUIMobBar );
	CC_SAFE_RELEASE ( m_pUIMobBarMagnification	);
	CC_SAFE_RELEASE ( m_pUIMobBarMagnification2	);
	CC_SAFE_RELEASE ( m_pUIMobBarMagnification3	);
	m_pUIMobBarHP  = nullptr;
	m_pUIMobBarATK = nullptr;
	m_pUIMobBarDEF = nullptr;

	// bar exp UI
	CC_SAFE_RELEASE ( m_pUIBarExpGauge1	);
	CC_SAFE_RELEASE ( m_pUIBarExpGauge2 );
	CC_SAFE_RELEASE ( m_pUIBarExpFont1	);
	CC_SAFE_RELEASE ( m_pUIBarExpFont2	);

	// bar kill UI
	CC_SAFE_RELEASE ( m_pUIBarKill );

	// detail UI
	CC_SAFE_RELEASE ( m_pUIMobNum );
	SAFE_RELEASE_ARRAYS ( m_aUISkillIcons );

	m_aUIMobDetailPara.clear ( );
	m_aUISkillIcons	  .clear ( );
	m_aUISkillStrs	  .clear ( );

	// level up UI
	SAFE_RELEASE_ARRAYS ( m_aUILevels );

	// successive UI
	CC_SAFE_RELEASE ( m_pUISuccessiveNum );

	// btns
	SAFE_REMOVE_CHILD ( m_pBtnExit  );
	SAFE_REMOVE_CHILD ( m_pBtnRetry );
	SAFE_REMOVE_CHILD ( m_pBtnRank  );

	// exp
	CC_SAFE_RELEASE ( m_pUIExpBack  );
	CC_SAFE_RELEASE ( m_pUIExpGauge );
	CC_SAFE_RELEASE ( m_pUIExpFont  );

	this->unschedule ( schedule_selector ( CasinoResultNode::step ) );
}

KDbool CasinoResultNode::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	// switch

	// touch
	m_nTidCount = 1;

	// time
	m_dTickCount = 0;
	m_dBeforeTickCount = 0;

	// back
	m_pSprBack = Sprite::create ( "Bet/bet_bg.png" );
	m_pSprBack->setPosition	( Point::ZERO );
	m_pSprBack->setAnchorPoint ( Point::ZERO );
	this->addChild ( m_pSprBack, LAYER_BACK );

	// back decorations
	cSprAni*	pAniDeco = new cSprAni ( this, "Bet/bet_bg_fire.png", LAYER_BACK_DECO, Point ( 3, 293 ) );

	pAniDeco->addFrame ( Rect ( 0,   0, 123, 40 ), 110, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	pAniDeco->addFrame ( Rect ( 0,  41, 123, 40 ), 110, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	pAniDeco->addFrame ( Rect ( 0,  82, 123, 40 ), 110, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	pAniDeco->addFrame ( Rect ( 0, 123, 123, 40 ), 110, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );

	pAniDeco->setFrameInit ( 0 );
	pAniDeco->setLoop ( true );
	pAniDeco->start ( );

	m_aBackDecos.push_back ( pAniDeco );

	// labels
	m_pLabels = new cLabels ( this );

	// top money
	m_pLabelTopMoney = new cLabelAtlasCustom ( this, "Bet/coin_nums_s.png", 15, LAYER_CASINO_RESULT_TOP_MONEY );
	m_pLabelTopMoney->addCharInfo ( ',',  7 );	m_pLabelTopMoney->addCharInfo ( '0', 15 );	m_pLabelTopMoney->addCharInfo ( '1', 15 );
	m_pLabelTopMoney->addCharInfo ( '2', 15 );	m_pLabelTopMoney->addCharInfo ( '3', 15 );	m_pLabelTopMoney->addCharInfo ( '4', 15 );
	m_pLabelTopMoney->addCharInfo ( '5', 15 );	m_pLabelTopMoney->addCharInfo ( '6', 15 );	m_pLabelTopMoney->addCharInfo ( '7', 15 );
	m_pLabelTopMoney->addCharInfo ( '8', 15 );	m_pLabelTopMoney->addCharInfo ( '9', 15 );

	// time get money
	cTimeGetMoney::getObj ( )->changeNode ( this, LAYER_CASINO_RESULT_TOP_MONEY, Rect ( 150, 300, 50, 15 ) );

	// btns
	m_pBtnExit	= Menu::createWithItem ( MenuItemImage::create ( "CasinoResult/casino_result_exit.png"	, "CasinoResult/casino_result_exit_push.png"	, CC_CALLBACK_1 ( CasinoResultNode::exitCallback , this ) ) );
	m_pBtnRetry	= Menu::createWithItem ( SoundMenuItem::create ( "CasinoResult/casino_result_retry.png"	, "CasinoResult/casino_result_retry_push.png", CC_CALLBACK_1 ( CasinoResultNode::retryCallback, this ) ) );
	m_pBtnRank	= Menu::createWithItem ( SoundMenuItem::create ( "CasinoResult/casino_result_rank.png"	, "CasinoResult/casino_result_rank_push.png"	, CC_CALLBACK_1 ( CasinoResultNode::rankCallback , this ) ) );

	m_pBtnExit ->setPosition ( Point (  65, 13.5f ) );
	m_pBtnRetry->setPosition ( Point ( 304, 76 ) );
	m_pBtnRank ->setPosition ( Point ( 432, 21 ) );

	m_pBtnExit ->setVisible ( true );
	m_pBtnRetry->setVisible ( false );
	m_pBtnRank ->setVisible ( true );

	m_pBtnExit ->setEnabled ( false );
	m_pBtnRetry->setEnabled ( false );
	m_pBtnRank ->setEnabled ( false );

	this->addChild ( m_pBtnExit  , LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtnRetry , LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtnRank  , LAYER_BETVIEW_BTN );

	// animation
	m_dAniStartLeftTime = 2;

	// exp
	m_fStartExp	= 0;
	m_fNowExp	= 0;
	m_fAfterExp	= 0;

	// kill
	m_nStartKillCount = 0;
	m_nAfterKillCount = 0;

	// node change
	m_bIsNodeChangeIn  = true;
	m_bIsNodeChangeOut = false;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg   = new cNodeChangeImgs ( this, false );

	return true;
}

KDvoid CasinoResultNode::setBetMob ( cUnit* pBetMob, eModeType eMode )
{
	m_eModeType = eMode;
	m_pBetMob   = pBetMob;
	m_pBetMob->retain ( );

	m_pBetMob->changeNode ( this, LAYER_CASINO_RESULT_MOB );

	// shadow
	pBetMob->setIsShadow ( false );
	
	// mob set
	if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		m_pBetMob->texLoad   ( eUnitAniType_Normal );
		m_pBetMob->texLoad   ( eUnitAniType_Attack );
		m_pBetMob->changeAni ( eUnitAniType_Normal );
		m_pBetMob->aniStart  ( );
	}
	else
	{
		if ( m_pBetMob->getDie ( ) == false )	
		{
			m_pBetMob->texLoad   ( eUnitAniType_Normal );
			m_pBetMob->changeAni ( eUnitAniType_Normal );	
			m_pBetMob->texLoad   ( eUnitAniType_Attack );	
			m_pBetMob->aniStart  ( );	
		}
		else							
		{
			m_pBetMob->texLoad ( eUnitAniType_Damage );
			m_pBetMob->changeAni ( eUnitAniType_Damage );
			m_pBetMob->setIsAutoChangeNor ( false );	
			m_pBetMob->aniStop ( );	
		}
	}

	m_pBetMob->setVisible ( true );
	m_pBetMob->setIsColorChange ( false );
	m_pBetMob->setPoint ( Point ( 65, 174 ) );
	m_pBetMob->setColor ( 255, 255, 255, 255 );

	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		m_pBetMob->texLoad ( eUnitAniType_Name_a   );
		m_pBetMob->texLoad ( eUnitAniType_Name_b   );
		m_pBetMob->texLoad ( eUnitAniType_Portrait );

		m_pBetMob->setVisibleNameA	  ( true );
		m_pBetMob->setVisibleNameB	  ( true );
		m_pBetMob->setVisiblePortrait ( true );

		m_pBetMob->setPointNameA    ( Point (  24, 316 ) );
		m_pBetMob->setPointNameB    ( Point ( 173, 292 ) );
		m_pBetMob->setPointPortrait ( Point ( 410, 291 ) );
	}
	else
	{
		m_pBetMob->texLoad ( eUnitAniType_Name_a   );
		m_pBetMob->texLoad ( eUnitAniType_Portrait );

		m_pBetMob->setVisibleNameA	  ( true );
		m_pBetMob->setVisiblePortrait ( true );

		m_pBetMob->setPointNameA    ( Point (  24, 316 ) );
		m_pBetMob->setPointPortrait ( Point ( 146, 227 ) );
	}

	// set kill count
	m_nStartKillCount = m_pBetMob->getKillCount ( ) - cMyData::getObj ( )->m_nGamingKillCount;
	m_nAfterKillCount = m_pBetMob->getKillCount ( );

	// arcade clear
	if ( m_eModeType == eModeType_MyRoom_Arcade_Easy && m_pBetMob->getDie ( ) == false )	m_pBetMob->setIsArcadeEasyClear ( true );
	if ( m_eModeType == eModeType_MyRoom_Arcade_Nor  && m_pBetMob->getDie ( ) == false )	m_pBetMob->setIsArcadeNorClear  ( true );
	if ( m_eModeType == eModeType_MyRoom_Arcade_Hard && m_pBetMob->getDie ( ) == false )	m_pBetMob->setIsArcadeHardClear ( true );
}

KDvoid CasinoResultNode::setFileLoad ( eIntoType eInto )
{
	// path
	const KDchar*		pPath = nullptr;
	if ( eInto == eIntoType_Casino )	pPath = _FILE_NAME_CASINO_RESULT_SAVE;
	else								pPath = _FILE_NAME_MYROOM_RESULT_SAVE;

	// file open
	KDint		nCreateIndex		= -1;
	KDint		nSeatIndex			= 0;
	KDint		nCreateInfoFileType = 0;
	KDint		nUniqueType			= 0;
	KDint		nSlotIndex			= 0;
	KDint		nUnitID				= 0;
	KDfloat		fExp				= 0;
	KDfloat		fClassExp			= 0;
	KDfloat		fFriendShipExp		= 0;
	KDint		nKillCount			= 0;
	KDint		nIsArcadeEasyClear  = 0;
	KDint		nIsArcadeNorClear	= 0;
	KDint		nIsArcadeHardClear	= 0;

	KDint		nHp		= 0;
	KDint		nAtk	= 0;
	KDint		nDef	= 0;
	KDint		nAgl	= 0;
	KDint		nCrt	= 0;
	KDint		nHpMax	= 0;
	KDint		nAtkMax = 0;
	KDint		nDefMax = 0;
	KDint		nAglMax	= 0;
	KDint		nCrtMax = 0;

	KDint64		nBetMoney		  = 0;
	KDfloat		fBetMagnification = 0;

	KDFile*		pFile = kdFopen ( pPath, "rb" );
	if ( pFile != nullptr )
	{
		// version
		KDint		nVersionStrCount = 0;
		kdFread ( &nVersionStrCount, sizeof ( KDint ), 1, pFile );

		KDchar*		pVersionStr = new KDchar [ nVersionStrCount + 1 ];
		kdFread ( pVersionStr, sizeof ( KDchar ), nVersionStrCount + 1, pFile );
		delete pVersionStr;

		// mode type
		KDint		nModeType = 0;
		kdFread ( &nModeType, sizeof ( KDint ), 1, pFile );
		m_eModeType = (eModeType) nModeType;

		// mob
		kdFread ( &nCreateIndex		  , sizeof ( KDint	 ), 1, pFile );
		kdFread ( &nSeatIndex		  , sizeof ( KDint	 ), 1, pFile );
		kdFread ( &nCreateInfoFileType, sizeof ( KDint	 ), 1, pFile );
		kdFread ( &nUniqueType		  , sizeof ( KDint	 ), 1, pFile );
		kdFread ( &nSlotIndex		  , sizeof ( KDint	 ), 1, pFile );
		kdFread ( &nUnitID			  , sizeof ( KDint	 ), 1, pFile );
		kdFread ( &fExp				  , sizeof ( KDfloat ), 1, pFile );
		kdFread ( &fClassExp		  , sizeof ( KDfloat ), 1, pFile );
		kdFread ( &fFriendShipExp	  , sizeof ( KDfloat ), 1, pFile );
		kdFread ( &nKillCount		  , sizeof ( KDint	 ), 1, pFile );
		kdFread ( &nIsArcadeEasyClear , sizeof ( KDint	 ), 1, pFile );
		kdFread ( &nIsArcadeNorClear  , sizeof ( KDint	 ), 1, pFile );
		kdFread ( &nIsArcadeHardClear , sizeof ( KDint	 ), 1, pFile );
		
		kdFread ( &nHp	  , sizeof ( KDint ), 1, pFile );
		kdFread ( &nAtk	  , sizeof ( KDint ), 1, pFile );
		kdFread ( &nDef	  , sizeof ( KDint ), 1, pFile );
		kdFread ( &nAgl	  , sizeof ( KDint ), 1, pFile );
		kdFread ( &nCrt	  , sizeof ( KDint ), 1, pFile );
		kdFread ( &nHpMax , sizeof ( KDint ), 1, pFile );
		kdFread ( &nAtkMax, sizeof ( KDint ), 1, pFile );
		kdFread ( &nDefMax, sizeof ( KDint ), 1, pFile );
		kdFread ( &nAglMax, sizeof ( KDint ), 1, pFile );
		kdFread ( &nCrtMax, sizeof ( KDint ), 1, pFile );

		// bet init
		kdFread ( &nBetMoney		, sizeof ( KDint64 ), 1, pFile );
		kdFread ( &fBetMagnification, sizeof ( KDfloat ), 1, pFile );

		// successive
		kdFread ( &cMyData::getObj ( )->m_nSuccessive, sizeof ( KDint ), 1, pFile );

		// close
		kdFclose ( pFile );
	}

	// 파일 삭제.
	kdUnlink ( _FILE_NAME_CASINO_BATTLE_SAVE );
	kdUnlink ( _FILE_NAME_CASINO_RESULT_SAVE );
	kdUnlink ( _FILE_NAME_MYROOM_BATTLE_SAVE );
	kdUnlink ( _FILE_NAME_MYROOM_RESULT_SAVE );

	// create info
	cUnitCreateInfos*	pMobCreate = nullptr;
	switch ( nCreateInfoFileType )
	{
		case eCreateInfoFileType_Coin_1					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_1.par"	 );		break;
		case eCreateInfoFileType_Coin_2					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_2.par"	 );		break;
		case eCreateInfoFileType_Coin_3					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_3.par"	 );		break;
		case eCreateInfoFileType_Coin_4					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_4.par"	 );		break;
		case eCreateInfoFileType_Coin_5					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_5.par"	 );		break;
		case eCreateInfoFileType_Coin_6					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_6.par"	 );		break;
		case eCreateInfoFileType_Coin_7					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_7.par"	 );		break;
		case eCreateInfoFileType_Coin_8					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_8.par"	 );		break;
		case eCreateInfoFileType_Coin_9					:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_9.par"	 );		break;
		case eCreateInfoFileType_Coin_10				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_10.par"	 );		break;
		case eCreateInfoFileType_Coin_11				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_11.par"	 );		break;
		case eCreateInfoFileType_Coin_12				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_12.par"	 );		break;
		case eCreateInfoFileType_Coin_13				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_13.par"	 );		break;
		case eCreateInfoFileType_Premium_1				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_1.par" );		break;
		case eCreateInfoFileType_Premium_2				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_2.par" );		break;
		case eCreateInfoFileType_Premium_3				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_3.par" );		break;
		case eCreateInfoFileType_Premium_4				:	pMobCreate = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_4.par" );		break;
		case eCreateInfoFileType_Casino_1				:	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos.par"			 );		break;
		case eCreateInfoFileType_Casino_2				:	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos2.par"			 );		break;
		case eCreateInfoFileType_Casino_3				:	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos3.par"			 );		break;
		case eCreateInfoFileType_Casino_4				:	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos4.par"			 );		break;
		case eCreateInfoFileType_ArcadeEasy				:	pMobCreate = new cUnitCreateInfos ( "arcade_easy.par"					 );		break;
		case eCreateInfoFileType_ArcadeEasyMiddleBoss	:	pMobCreate = new cUnitCreateInfos ( "arcade_easy_middle_boss.par"		 );		break;
		case eCreateInfoFileType_ArcadeEasyLastBoss		:	pMobCreate = new cUnitCreateInfos ( "arcade_easy_last_boss.par"			 );		break;
		case eCreateInfoFileType_ArcadeNor				:	pMobCreate = new cUnitCreateInfos ( "arcade_normal.par"					 );		break;
		case eCreateInfoFileType_ArcadeNorMiddleBoss	:	pMobCreate = new cUnitCreateInfos ( "arcade_normal_middle_boss.par"		 );		break;
		case eCreateInfoFileType_ArcadeNorLastBoss		:	pMobCreate = new cUnitCreateInfos ( "arcade_normal_last_boss.par"		 );		break;
		case eCreateInfoFileType_ArcadeHard				:	pMobCreate = new cUnitCreateInfos ( "arcade_hard.par"					 );		break;
		case eCreateInfoFileType_ArcadeHardMiddleBoss	:	pMobCreate = new cUnitCreateInfos ( "arcade_hard_middle_boss.par"		 );		break;
		case eCreateInfoFileType_ArcadeHardLastBoss		:	pMobCreate = new cUnitCreateInfos ( "arcade_hard_last_boss.par"			 );		break;
		case eCreateInfoFileType_Survival				:	pMobCreate = new cUnitCreateInfos ( "survival.par"						 );		break;
	}

	// new unit
	m_pBetMob = new cUnit ( this, pMobCreate->getUnitInfo ( nCreateIndex ), (eCreateInfoFileType) nCreateInfoFileType, LAYER_CASINO_RESULT_MOB, nUnitID, nCreateIndex, nSeatIndex );

	// set unit info
	m_pBetMob->setUniqueType		( (eUniqueType) nUniqueType );
	m_pBetMob->setSlotIndex			( nSlotIndex				);
	m_pBetMob->setExp				( fExp						);
	m_pBetMob->setClassExp			( fClassExp					);
	m_pBetMob->setFriendShipExp		( fFriendShipExp			);
	m_pBetMob->setKillCount			( nKillCount				);
	m_pBetMob->setIsArcadeEasyClear	( nIsArcadeEasyClear 		);
	m_pBetMob->setIsArcadeNorClear	( nIsArcadeNorClear 		);
	m_pBetMob->setIsArcadeHardClear	( nIsArcadeHardClear		);

	// set mob parameter
	m_pBetMob->setHP		( nHp	  );
	m_pBetMob->setATK		( nAtk	  );
	m_pBetMob->setDEF		( nDef	  );
	m_pBetMob->setAGL		( nAgl	  );
	m_pBetMob->setCRT		( nCrt	  );
	m_pBetMob->setHPMax		( nHpMax  );
	m_pBetMob->setATKMax	( nAtkMax );
	m_pBetMob->setDEFMax	( nDefMax );
	m_pBetMob->setAGLMax	( nAglMax );
	m_pBetMob->setCRTMax	( nCrtMax );

	// set bet info
	m_pBetMob->setBetMoney			    ( nBetMoney		    );
	m_pBetMob->setDividendMagnification ( fBetMagnification );

	// mob set
	if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		m_pBetMob->texLoad ( eUnitAniType_Normal );
		m_pBetMob->texLoad ( eUnitAniType_Attack );
		m_pBetMob->changeAni ( eUnitAniType_Normal );
		m_pBetMob->aniStart ( );
	}
	else
	{
		if ( m_pBetMob->getDie ( ) == false )	{ m_pBetMob->texLoad ( eUnitAniType_Normal ); m_pBetMob->changeAni ( eUnitAniType_Normal );	m_pBetMob->texLoad ( eUnitAniType_Attack );	m_pBetMob->aniStart ( ); }
		else									{ m_pBetMob->texLoad ( eUnitAniType_Damage ); m_pBetMob->changeAni ( eUnitAniType_Damage );	m_pBetMob->setIsAutoChangeNor ( false );	m_pBetMob->aniStop  ( ); }
	}

	m_pBetMob->setVisible ( true );
	m_pBetMob->setIsColorChange ( false );
	m_pBetMob->setPoint ( Point ( 65, 174 ) );
	m_pBetMob->setColor ( 255, 255, 255, 255 );

	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		m_pBetMob->texLoad ( eUnitAniType_Name_a );
		m_pBetMob->texLoad ( eUnitAniType_Name_b );
		m_pBetMob->texLoad ( eUnitAniType_Portrait );

		m_pBetMob->setVisibleNameA	  ( true );
		m_pBetMob->setVisibleNameB	  ( true );
		m_pBetMob->setVisiblePortrait ( true );

		m_pBetMob->setPointNameA    ( Point (  24, 316 ) );
		m_pBetMob->setPointNameB    ( Point ( 173, 292 ) );
		m_pBetMob->setPointPortrait ( Point ( 410, 291 ) );
	}
	else
	{
		m_pBetMob->texLoad ( eUnitAniType_Name_a   );
		m_pBetMob->texLoad ( eUnitAniType_Portrait );

		m_pBetMob->setVisibleNameA	  ( true );
		m_pBetMob->setVisiblePortrait ( true );

		m_pBetMob->setPointNameA    ( Point (  24, 316 ) );
		m_pBetMob->setPointPortrait ( Point ( 146, 227 ) );
	}

	// money
	m_nMoney			= cMyData::getObj ( )->m_nMoney;
	m_nMoneyBackup		= m_nMoney;
	m_nMoneyBackup2		= cMyData::getObj ( )->m_nMoney;
	m_nMoneyEarn		= 0;
	m_nMoneyEarnBackup	= 0;

	m_dMoneyLeftTime		= 0;
	m_dMoneyLeftTimeBackup	= m_dMoneyLeftTime;
	m_dMoneySoundLeftTime	= 0;
	m_bIsMoneySoundFinish	= true;

	// my data get exp
	for ( auto& rSaveData : cMyData::getObj ( )->m_aMyRoomUnitSaveDatas )
	{
		if ( rSaveData.nUnitID == m_pBetMob->getUnitID ( ) )
		{
			m_pBetMob->setExp ( rSaveData.fExp );
		}
	}

	// level up
	while ( m_pBetMob->getExp ( ) >= 100 )
	{
		// para
		KDint	nHp	 = _MYROOM_LEVELUP_POINT_HP_MIN  + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_HP_MAX -_MYROOM_LEVELUP_POINT_HP_MIN  );
		KDint	nAtk = _MYROOM_LEVELUP_POINT_ATK_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_ATK_MAX-_MYROOM_LEVELUP_POINT_ATK_MIN );
		KDint	nDef = _MYROOM_LEVELUP_POINT_DEF_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_DEF_MAX-_MYROOM_LEVELUP_POINT_DEF_MIN );
		KDint	nAgl = _MYROOM_LEVELUP_POINT_AGL_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_AGL_MAX-_MYROOM_LEVELUP_POINT_AGL_MIN );
		KDint	nCrt = _MYROOM_LEVELUP_POINT_CRT_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_CRT_MAX-_MYROOM_LEVELUP_POINT_CRT_MIN );
		m_pBetMob->paraUp ( nHp, nAtk, nDef, nAgl, nCrt );

		// exp
		m_pBetMob->setExp ( m_pBetMob->getExp ( ) - 100 );
	}

	if ( m_pBetMob->getIsParaMax ( ) == true )
	{
		m_fStartExp = 100;
		m_fNowExp   = 100;
		m_fAfterExp = 100;
	}
	else
	{
		m_fStartExp = m_pBetMob->getExp ( );
		m_fNowExp   = m_fStartExp;
		m_fAfterExp = m_fStartExp;
	}
}

KDvoid CasinoResultNode::initUI ( KDvoid )
{
	// result back
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		m_pSprResultBack = new cSprite ( this, "CasinoResult/casino_result_bg.png", LAYER_CASINO_RESULT_BACK, Point ( 0, 0 ), Point ( 134, 40 ) );
	}
	else
	{
		m_pSprResultBack = new cSprite ( this, "CasinoResult/casino_result_bg2.png", LAYER_CASINO_RESULT_BACK, Point ( 0, 0 ), Point ( 134, 40 ) );
	}

	// earn money
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		m_pLabelEarnMoney = new cLabelAtlasCustom ( this, "CasinoResult/casino_result_num.png", 25, LAYER_CASINO_RESULT_RESULT_MONEY );
		m_pLabelEarnMoney->addCharInfo ( '-', 17 );		m_pLabelEarnMoney->addCharInfo ( '+', 21 );		m_pLabelEarnMoney->addCharInfo ( '0', 26 );
		m_pLabelEarnMoney->addCharInfo ( '1', 26 );		m_pLabelEarnMoney->addCharInfo ( '2', 26 );		m_pLabelEarnMoney->addCharInfo ( '3', 26 );
		m_pLabelEarnMoney->addCharInfo ( '4', 26 );		m_pLabelEarnMoney->addCharInfo ( '5', 26 );		m_pLabelEarnMoney->addCharInfo ( '6', 26 );
		m_pLabelEarnMoney->addCharInfo ( '7', 26 );		m_pLabelEarnMoney->addCharInfo ( '8', 26 );		m_pLabelEarnMoney->addCharInfo ( '9', 26 );

		m_pLabelResultMoney = new cLabelAtlasCustom ( this, "CasinoResult/casino_result_num.png", 25, LAYER_CASINO_RESULT_RESULT_MONEY );
		m_pLabelResultMoney->addCharInfo ( '-', 17 );	m_pLabelResultMoney->addCharInfo ( '+', 21 );	m_pLabelResultMoney->addCharInfo ( '0', 26 );
		m_pLabelResultMoney->addCharInfo ( '1', 26 );	m_pLabelResultMoney->addCharInfo ( '2', 26 );	m_pLabelResultMoney->addCharInfo ( '3', 26 );
		m_pLabelResultMoney->addCharInfo ( '4', 26 );	m_pLabelResultMoney->addCharInfo ( '5', 26 );	m_pLabelResultMoney->addCharInfo ( '6', 26 );
		m_pLabelResultMoney->addCharInfo ( '7', 26 );	m_pLabelResultMoney->addCharInfo ( '8', 26 );	m_pLabelResultMoney->addCharInfo ( '9', 26 );

		m_pLabelResultMoney->setColor ( 255, 255, 255, 0 );
	}

	// result text
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		if ( m_pBetMob->getDie ( ) == false )	m_pSprResultText = new cSprite ( this, "CasinoResult/casino_result_youwin.png" , LAYER_CASINO_RESULT_RESULT_TEXT, Point ( 0, 0 ), Point ( 180, 213 ) );
		else									m_pSprResultText = new cSprite ( this, "CasinoResult/casino_result_youlose.png", LAYER_CASINO_RESULT_RESULT_TEXT, Point ( 0, 0 ), Point ( 180, 213 ) );
	}
	else if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		m_pSprResultText = new cSprite ( this, "CasinoResult/casino_result_font.png", LAYER_CASINO_RESULT_RESULT_TEXT, Point ( 0, 0 ), Point ( 204, 254 ) );
	}
	else
	{
		if ( m_pBetMob->getDie ( ) == false )	m_pSprResultText = new cSprite ( this, "CasinoResult/casino_result_youwin.png" , LAYER_CASINO_RESULT_RESULT_TEXT, Point ( 0, 0 ), Point ( 180, 254 ) );
		else									m_pSprResultText = new cSprite ( this, "CasinoResult/casino_result_youlose.png", LAYER_CASINO_RESULT_RESULT_TEXT, Point ( 0, 0 ), Point ( 180, 254 ) );
	}

	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		// bar UI back
		m_pUIMobBar = new cSprite ( this, "Bet/bet_mob_bar.png", LAYER_CASINO_RESULT_BAR_BACK, Point ( 0, 0 ), Point ( 147, 249 ) );

		// bar UI mob dividend magnifications
		const KDchar*	pStr1	= ccszf ( "%d", (KDint) m_pBetMob->getDividendMagnification ( ) );
		const KDchar*	pStr2	= ccszf ( "%d", (KDint) ( ( m_pBetMob->getDividendMagnification ( ) - (KDint) m_pBetMob->getDividendMagnification ( ) ) * 10 ) );
		Point			tPoint	= Point ( 348, 269 );

		m_pUIMobBarMagnification  = new cLabelAtlas ( this, pStr1, "Bet/bet_multiple_num.png" , 18, 19, '0', LAYER_CASINO_RESULT_BAR_UI, tPoint );
		m_pUIMobBarMagnification2 = new cLabelAtlas ( this, pStr2, "Bet/bet_multiple_num2.png", 12, 13, '0', LAYER_CASINO_RESULT_BAR_UI, Point ( tPoint.x + m_pUIMobBarMagnification->getSize ( ).width + 5, tPoint.y ) );
		m_pUIMobBarMagnification3 = new cSprite	    ( this, "Bet/bet_multiple_num3.png", LAYER_CASINO_RESULT_BAR_UI, Point ( 0, 0 ), Point ( tPoint.x + m_pUIMobBarMagnification->getSize ( ).width, tPoint.y ) );

		if ( m_eModeType != eModeType_Casino && m_eModeType != eModeType_MyRoom_Casino100 && m_eModeType != eModeType_MyRoom_Casino1000 )
		{
			m_pUIMobBarMagnification ->setVisible ( false );
			m_pUIMobBarMagnification2->setVisible ( false );
			m_pUIMobBarMagnification3->get ( )->setVisible ( false );
		}

		// bar UI mob parameters
		const KDchar*	szHP  = ccszf ( "HP %02d"  , m_pBetMob->getHP  ( ) );
		const KDchar*	szATK = ccszf ( "ATK %02d" , m_pBetMob->getATK ( ) );
		const KDchar*	szDEF = ccszf ( "DEF %02d" , m_pBetMob->getDEF ( ) );

		Rect			tRectHP	 = Rect ( 188, 254, 50, 20 );
		Rect			tRectATK = Rect ( 237, 254, 50, 20 );
		Rect			tRectDEF = Rect ( 297, 254, 50, 20 );

		m_pUIMobBarHP	= m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::RIGHT, tRectHP , szHP , LAYER_CASINO_RESULT_BAR_UI );
		m_pUIMobBarATK	= m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::RIGHT, tRectATK, szATK, LAYER_CASINO_RESULT_BAR_UI );
		m_pUIMobBarDEF	= m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT , tRectDEF, szDEF, LAYER_CASINO_RESULT_BAR_UI );
	}		

	// detail UI mob num
	m_pUIMobNum = new cSprite ( this );

	switch ( m_pBetMob->getSeatIndex ( ) )
	{
		case 0	:	m_pUIMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	m_pUIMobNum->addFrame ( Rect (  0, 0, 21, 21 ) );		break;
		case 1	:	m_pUIMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	m_pUIMobNum->addFrame ( Rect ( 21, 0, 21, 21 ) );		break;
		case 2	:	m_pUIMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	m_pUIMobNum->addFrame ( Rect ( 42, 0, 21, 21 ) );		break;
		case 3	:	m_pUIMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	m_pUIMobNum->addFrame ( Rect ( 63, 0, 21, 21 ) );		break;
		case 4	:	m_pUIMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	m_pUIMobNum->addFrame ( Rect ( 84, 0, 21, 21 ) );		break;
	}

	m_pUIMobNum->setFrame ( 0 );
	m_pUIMobNum->get ( )->setVisible ( true );

	// UI mob Detail parameter
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect (  73, 137, 80, 12 ), "HP : " , LAYER_BETVIEW_UI_5 ) );		// hp
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect (  56, 126, 80, 12 ), "ATK : ", LAYER_BETVIEW_UI_5 ) );		// atk
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 108, 126, 80, 12 ), "DEF : ", LAYER_BETVIEW_UI_5 ) );		// def
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect (  56, 115, 80, 12 ), "AGL : ", LAYER_BETVIEW_UI_5 ) );		// agl
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 108, 115, 80, 12 ), "CRT : ", LAYER_BETVIEW_UI_5 ) );		// crt

	m_pLabels->setStr ( m_aUIMobDetailPara [ 0 ], ccszf ( "HP : %02d / %02d" , m_pBetMob->getHP ( ), m_pBetMob->getHP ( ) ) );
	m_pLabels->setStr ( m_aUIMobDetailPara [ 1 ], ccszf ( "ATK : %02d", m_pBetMob->getATK ( ) ) );
	m_pLabels->setStr ( m_aUIMobDetailPara [ 2 ], ccszf ( "DEF : %02d", m_pBetMob->getDEF ( ) ) );
	m_pLabels->setStr ( m_aUIMobDetailPara [ 3 ], ccszf ( "AGL : %02d", m_pBetMob->getAGL ( ) ) );
	m_pLabels->setStr ( m_aUIMobDetailPara [ 4 ], ccszf ( "CRT : %02d", m_pBetMob->getCRT ( ) ) );

	// UI skill icon
	for ( KDint i = 0; i < 3; i++ )
	{
		KDint		nSkillIndex = m_pBetMob->getSkillType ( i );

		if ( nSkillIndex == -1 )
		{
			break;
		}

		cSprAni*	pAniSkillIcon = nullptr;

		switch ( i )
		{
			case 0	:	pAniSkillIcon = new cSprAni ( this, "Skills/skill_icons.png", LAYER_BACK_DECO, Point ( 6, 96 ) );	break;
			case 1	:	pAniSkillIcon = new cSprAni ( this, "Skills/skill_icons.png", LAYER_BACK_DECO, Point ( 6, 74 ) );	break;
			case 2	:	pAniSkillIcon = new cSprAni ( this, "Skills/skill_icons.png", LAYER_BACK_DECO, Point ( 6, 52 ) );	break;
		}

		for ( KDint a = 0; a < 11; a++ )
		{
			pAniSkillIcon->addFrame ( Rect ( a * 20, 0, 20, 20 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
		}

		pAniSkillIcon->setFrameInit ( nSkillIndex );
		pAniSkillIcon->setVisible ( true );
		pAniSkillIcon->setLoop ( false );
		pAniSkillIcon->stop ( );

		m_aUISkillIcons.push_back ( pAniSkillIcon );

		const KDchar*	pStr = m_pBetMob->getSkillStr ( i ).c_str ( );

		switch ( i )
		{
			case 0	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 79, 82, 100, 30 ), pStr, LAYER_BETVIEW_UI_5 ) );	break;
			case 1	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 79, 60, 100, 30 ), pStr, LAYER_BETVIEW_UI_5 ) );	break;
			case 2	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 79, 38, 100, 30 ), pStr, LAYER_BETVIEW_UI_5 ) );	break;
		}
	}

	// successive UI
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		m_pUISuccessiveNum = new cLabelAtlasCustom ( this, "CasinoResult/casino_result_casino_successive.png", 14, LAYER_CASINO_RESULT_BAR_UI );
		m_pUISuccessiveNum->addCharInfo ( '1', 13 );	m_pUISuccessiveNum->addCharInfo ( '2', 13 );	m_pUISuccessiveNum->addCharInfo ( '3', 13 );
		m_pUISuccessiveNum->addCharInfo ( '4', 13 );	m_pUISuccessiveNum->addCharInfo ( '5', 14 );	m_pUISuccessiveNum->addCharInfo ( '6', 13 );
		m_pUISuccessiveNum->addCharInfo ( '7', 12 );	m_pUISuccessiveNum->addCharInfo ( '8', 14 );	m_pUISuccessiveNum->addCharInfo ( '9', 13 );
		m_pUISuccessiveNum->addCharInfo ( '0', 13 );	m_pUISuccessiveNum->addCharInfo ( 'W', 22 );	m_pUISuccessiveNum->addCharInfo ( 'I',  9 );
		m_pUISuccessiveNum->addCharInfo ( 'N', 15 );

		if ( m_eModeType != eModeType_Casino && m_eModeType != eModeType_MyRoom_Casino100 && m_eModeType != eModeType_MyRoom_Casino1000 )
		{
			m_pUIMobBarMagnification ->setVisible ( false );
			m_pUIMobBarMagnification2->setVisible ( false );
			m_pUIMobBarMagnification3->get ( )->setVisible ( false );
		}
		else if ( m_pBetMob->getDie ( ) == false && cMyData::getObj ( )->m_nSuccessive > 1 )
		{
			m_pUISuccessiveNum->setString ( ccszf ( "%dWIN", cMyData::getObj ( )->m_nSuccessive ) );
			m_pUISuccessiveNum->setVisible ( true );
			m_pUISuccessiveNum->setPosition ( Point ( 443 - ( m_pUISuccessiveNum->getSize ( ).width / 2 ), 255 ) );
		}
		else
		{
			m_pUISuccessiveNum->setVisible ( false );
		}
	}		

	// exp UI
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		m_pUIExpBack  = new cSprite ( this, "CasinoResult/casino_result_exp_back.png" , LAYER_BETVIEW_UI_5, Point::ZERO, Point (  4, 162 ) );
		m_pUIExpGauge = new cSprite ( this, "CasinoResult/casino_result_exp_guage.png", LAYER_BETVIEW_UI_5, Point::ZERO, Point (  5, 163 ) );
		m_pUIExpFont  = new cSprite ( this, "CasinoResult/casino_result_exp_font.png" , LAYER_BETVIEW_UI_5, Point::ZERO, Point ( 56, 164 ) );

		m_pUIExpBack ->get ( )->setVisible ( false );
		m_pUIExpGauge->get ( )->setVisible ( false );
		m_pUIExpFont ->get ( )->setVisible ( false );
	}
	else
	{
		m_pUIBarExpGauge2 = new cSprite ( this, "CasinoResult/casino_result_exp_gauge2.png", LAYER_BETVIEW_UI_5, Point::ZERO, Point ( 216, 187 ) );
		m_pUIBarExpGauge1 = new cSprite ( this, "CasinoResult/casino_result_exp_gauge1.png", LAYER_BETVIEW_UI_5, Point::ZERO, Point ( 216, 187 ) );
		m_pUIBarExpFont1  = new cSprite ( this, "CasinoResult/casino_result_exp_font2.png" , LAYER_BETVIEW_UI_5, Point::ZERO, Point::ZERO );

		m_pUIBarExpFont2 = new cLabelAtlasCustom ( this, "CasinoResult/casino_result_num2.png", 13, LAYER_BETVIEW_UI_5 );
		m_pUIBarExpFont2->addCharInfo ( '1',  8 );	m_pUIBarExpFont2->addCharInfo ( '2', 12 );	m_pUIBarExpFont2->addCharInfo ( '3', 12 );
		m_pUIBarExpFont2->addCharInfo ( '4', 12 );	m_pUIBarExpFont2->addCharInfo ( '5', 12 );	m_pUIBarExpFont2->addCharInfo ( '6', 12 );
		m_pUIBarExpFont2->addCharInfo ( '7', 12 );	m_pUIBarExpFont2->addCharInfo ( '8', 12 );	m_pUIBarExpFont2->addCharInfo ( '9', 12 );
		m_pUIBarExpFont2->addCharInfo ( '0', 12 );	m_pUIBarExpFont2->addCharInfo ( '+',  9 );	m_pUIBarExpFont2->addCharInfo ( '=', 10 );
		m_pUIBarExpFont2->addCharInfo ( ' ',  5 );
	}
	// bar kill UI
	if ( m_eModeType != eModeType_Casino && m_eModeType != eModeType_MyRoom_Casino100 && m_eModeType != eModeType_MyRoom_Casino1000 )
	{
		m_pUIBarKill = new cLabelAtlasCustom ( this, "CasinoResult/casino_result_num2.png", 13, LAYER_BETVIEW_UI_5 );
		m_pUIBarKill->addCharInfo ( '1',  8 );	m_pUIBarKill->addCharInfo ( '2', 12 );	m_pUIBarKill->addCharInfo ( '3', 12 );
		m_pUIBarKill->addCharInfo ( '4', 12 );	m_pUIBarKill->addCharInfo ( '5', 12 );	m_pUIBarKill->addCharInfo ( '6', 12 );
		m_pUIBarKill->addCharInfo ( '7', 12 );	m_pUIBarKill->addCharInfo ( '8', 12 );	m_pUIBarKill->addCharInfo ( '9', 12 );
		m_pUIBarKill->addCharInfo ( '0', 12 );	m_pUIBarKill->addCharInfo ( '+',  9 );	m_pUIBarKill->addCharInfo ( '=', 10 );
		m_pUIBarKill->addCharInfo ( ' ',  5 );

		// set kill count
		m_pUIBarKill->setString ( ccszf ( "%d", m_nAfterKillCount - m_nStartKillCount ) );
		m_pUIBarKill->setPosition ( Point ( 278, 214 ) );
	}
}

KDvoid CasinoResultNode::initFinish ( KDvoid )
{
	// btns
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		if ( m_pBetMob->getDie ( ) == false )	m_pBtnRetry->setVisible ( true );
		else									m_pBtnRetry->setVisible ( false );
	}

	// UI bar exp
	if ( m_eModeType != eModeType_Casino && m_eModeType != eModeType_MyRoom_Casino100 && m_eModeType != eModeType_MyRoom_Casino1000 )
	{
		// gauge 1
		KDfloat		fWidth = 245 * (KDfloat) ( (KDint) m_fNowExp ) / 100.0f;
		m_pUIBarExpGauge1->get ( )->setTextureRect ( Rect ( 0, 0, fWidth, 22 ) );

		// gauge 2
		KDfloat		fWidth2 = 245 * (KDfloat) ( (KDint) m_fAfterExp ) / 100.0f;
		if ( fWidth2 > 245 )
		{
			fWidth2 = 245;
		}
		m_pUIBarExpGauge2->get ( )->setTextureRect ( Rect ( 0, 0, fWidth2, 22 ) );

		// font 2
		m_pUIBarExpFont2->setString ( ccszf ( "%d", (KDint) ( m_fAfterExp - m_fStartExp ) ) );		
		m_pUIBarExpFont2->setPosition ( Point ( 338 + 28 - (KDint) ( m_pUIBarExpFont2->getSize ( ).width / 2 ), 191 ) );

		// font 1
		m_pUIBarExpFont1->get ( )->setPosition ( Point ( m_pUIBarExpFont2->getPoint ( ).x - 57, 192 ) );
	}

	// update money
	this->updateMoney ( 0 );

	// schedule
	this->schedule ( schedule_selector ( CasinoResultNode::step ) );
}

KDvoid CasinoResultNode::plusExp ( KDfloat fPlusExp )
{
	// my data get exp
	for ( auto& rData : cMyData::getObj ( )->m_aMyRoomUnitSaveDatas )
	{
		if (  rData.nUnitID == m_pBetMob->getUnitID ( ) )
		{
			m_pBetMob->setExp ( rData.fExp );
		}
	}

	// level up
	while ( m_pBetMob->getExp ( ) >= 100 )
	{
		// para
		KDint	nHp	 = _MYROOM_LEVELUP_POINT_HP_MIN  + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_HP_MAX -_MYROOM_LEVELUP_POINT_HP_MIN  );
		KDint	nAtk = _MYROOM_LEVELUP_POINT_ATK_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_ATK_MAX-_MYROOM_LEVELUP_POINT_ATK_MIN );
		KDint	nDef = _MYROOM_LEVELUP_POINT_DEF_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_DEF_MAX-_MYROOM_LEVELUP_POINT_DEF_MIN );
		KDint	nAgl = _MYROOM_LEVELUP_POINT_AGL_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_AGL_MAX-_MYROOM_LEVELUP_POINT_AGL_MIN );
		KDint	nCrt = _MYROOM_LEVELUP_POINT_CRT_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_CRT_MAX-_MYROOM_LEVELUP_POINT_CRT_MIN );
		m_pBetMob->paraUp ( nHp, nAtk, nDef, nAgl, nCrt );

		// exp
		m_pBetMob->setExp ( m_pBetMob->getExp ( ) - 100 );

		// save my room data
		this->updateMyRoomData ( );
	}

	// init
	if ( m_pBetMob->getIsParaMax ( ) == true )
	{
		m_fStartExp = 100;
		m_fNowExp   = 100;
		m_fAfterExp = 100;
	}
	else
	{
		m_fStartExp = m_pBetMob->getExp ( );
		m_fNowExp   = m_fStartExp;
		m_fAfterExp = m_fStartExp + fPlusExp;
	}

	// set
	m_pBetMob->setExp ( m_fAfterExp );

	// 플러스 경험치에 대한 레벨 업 level up
	if ( m_pBetMob->getIsParaMax ( ) == false )
	{
		while ( m_pBetMob->getExp ( ) >= 100 )
		{
			// para
			KDint	nHp	 = _MYROOM_LEVELUP_POINT_HP_MIN  + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_HP_MAX -_MYROOM_LEVELUP_POINT_HP_MIN  );
			KDint	nAtk = _MYROOM_LEVELUP_POINT_ATK_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_ATK_MAX-_MYROOM_LEVELUP_POINT_ATK_MIN );
			KDint	nDef = _MYROOM_LEVELUP_POINT_DEF_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_DEF_MAX-_MYROOM_LEVELUP_POINT_DEF_MIN );
			KDint	nAgl = _MYROOM_LEVELUP_POINT_AGL_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_AGL_MAX-_MYROOM_LEVELUP_POINT_AGL_MIN );
			KDint	nCrt = _MYROOM_LEVELUP_POINT_CRT_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_CRT_MAX-_MYROOM_LEVELUP_POINT_CRT_MIN );

			m_pBetMob->paraUp ( nHp, nAtk, nDef, nAgl, nCrt );

			KDint	nHp2  = 0;
			KDint	nAtk2 = 0;
			KDint	nDef2 = 0;
			KDint	nAgl2 = 0;
			KDint	nCrt2 = 0;

			if ( nHp  > 0 )	nHp2  = m_pBetMob->getHP  ( );
			if ( nAtk > 0 )	nAtk2 = m_pBetMob->getATK ( );
			if ( nDef > 0 )	nDef2 = m_pBetMob->getDEF ( );
			if ( nAgl > 0 )	nAgl2 = m_pBetMob->getAGL ( );
			if ( nCrt > 0 )	nCrt2 = m_pBetMob->getCRT ( );

			// level effect add
			m_aUILevels.push_back ( new cResultLevelUp ( this, nHp2, nAtk2, nDef2, nAgl2, nCrt2 ) );

			// exp
			m_pBetMob->setExp( m_pBetMob->getExp ( ) - 100 );
		}
	}		

	// save my room data
	this->updateMyRoomData ( );
}

KDvoid CasinoResultNode::levelUp ( KDvoid )
{
	if ( m_pBetMob->getDie ( ) == false )
	{
		KDint	nHp  = m_pBetMob->getHP  ( ) + 2 + ( kdRand ( ) % 5 );
		KDint	nAtk = m_pBetMob->getATK ( ) + 1 + ( kdRand ( ) % 3 );
		KDint	nDef = m_pBetMob->getDEF ( ) + 1 + ( kdRand ( ) % 2 );
		KDint	nAgl = m_pBetMob->getAGL ( ) + 1 + ( kdRand ( ) % 4 );

		if ( nHp  > 99 )	nHp	 = 99;
		if ( nAtk > 30 )	nAtk = 30;
		if ( nDef > 20 )	nDef = 20;
		if ( nAgl > 99 )	nAgl = 99;

		if (  nHp  == m_pBetMob->getHP  ( ) )	nHp = 0;
		else									m_pBetMob->setHP ( nHp );
		if (  nAtk == m_pBetMob->getATK ( ) )	nAtk = 0;
		else									m_pBetMob->setATK ( nAtk );
		if (  nDef == m_pBetMob->getDEF ( ) )	nDef = 0;
		else									m_pBetMob->setDEF ( nDef );
		if (  nAgl == m_pBetMob->getAGL ( ) )	nAgl = 0;
		else									m_pBetMob->setAGL ( nAgl );

		// level effect add
		cResultLevelUp*		pLevelUpEffect = new cResultLevelUp ( this, nHp, nAtk, nDef, nAgl, 0 );
		pLevelUpEffect->start ( );
		m_aUILevels.push_back ( pLevelUpEffect );
	}
}

KDvoid CasinoResultNode::updateMyData ( KDvoid )
{
	// successive
	if ( m_pBetMob->getDie ( ) == false )
	{
		++cMyData::getObj ( )->m_nSuccessive;
	}

	// game count
	++cMyData::getObj ( )->m_nGameCount;

	// lose count
	if ( m_pBetMob->getDie ( ) == true )	
	{
		++cMyData::getObj ( )->m_nLoseCount;
	}

	// money
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		KDfloat		fMagnification = m_pBetMob->getDividendMagnification ( ) - 1;
		KDdouble	dPlusMoney	= m_pBetMob->getBetMoney ( ) * fMagnification;

		if ( dPlusMoney >= (KDdouble) ( (KDint64) dPlusMoney ) + 0.1f )	dPlusMoney = (KDint64) dPlusMoney + 1;
		else															dPlusMoney = (KDint64) dPlusMoney;

		if ( m_pBetMob->getDie ( ) == false )	m_nMoneyEarn =  dPlusMoney;
		else									m_nMoneyEarn = -m_pBetMob->getBetMoney ( );
		
		if ( m_nMoneyEarn > 0 )
		{
			cMyData::getObj ( )->m_nMoney += m_pBetMob->getBetMoney ( ) + m_nMoneyEarn;
		}
	}
	else if ( m_eModeType == eModeType_MyRoom_Arcade_Easy || m_eModeType == eModeType_MyRoom_Arcade_Nor || m_eModeType == eModeType_MyRoom_Arcade_Hard )
	{
		m_nMoneyEarn = 0;

		if ( m_pBetMob->getDie ( ) == false )
		{
			switch ( m_eModeType )
			{
				case eModeType_MyRoom_Arcade_Easy	:	m_nMoneyEarn = 100;		break;
				case eModeType_MyRoom_Arcade_Nor	:	m_nMoneyEarn = 300;		break;
				case eModeType_MyRoom_Arcade_Hard	:	m_nMoneyEarn = 1000;	break;
				default								:	break;
			}
		}

	   cMyData::getObj ( )->m_nMoney += m_nMoneyEarn;
	}
	else if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		m_nMoneyEarn = cMyData::getObj ( )->m_nGamingKillCount * 10;
		cMyData::getObj ( )->m_nMoney += m_nMoneyEarn;
	}

	m_nMoney			 = cMyData::getObj ( )->m_nMoney - m_nMoneyEarn;
	m_nMoneyBackup		 = m_nMoney;
	m_nMoneyBackup2		 = cMyData::getObj ( )->m_nMoney;
	m_nMoneyEarnBackup	 = m_nMoneyEarn;
	m_dMoneyWaitLeftTime = 1;

		 if ( m_nMoneyEarn >  -4 && m_nMoneyEarn <  4 )	m_dMoneyLeftTime = 0.2f;
	else if ( m_nMoneyEarn >  -8 && m_nMoneyEarn <  8 )	m_dMoneyLeftTime = 0.4f;
	else if ( m_nMoneyEarn > -12 && m_nMoneyEarn < 12 )	m_dMoneyLeftTime = 0.6f;
	else if ( m_nMoneyEarn > -16 && m_nMoneyEarn < 16 )	m_dMoneyLeftTime = 0.8f;
	else if ( m_nMoneyEarn > -20 && m_nMoneyEarn < 20 )	m_dMoneyLeftTime = 1.0f;
	else if ( m_nMoneyEarn > -24 && m_nMoneyEarn < 24 )	m_dMoneyLeftTime = 1.2f;
	else if ( m_nMoneyEarn > -28 && m_nMoneyEarn < 28 )	m_dMoneyLeftTime = 1.4f;
	else if ( m_nMoneyEarn > -32 && m_nMoneyEarn < 32 )	m_dMoneyLeftTime = 1.6f;
	else if ( m_nMoneyEarn > -36 && m_nMoneyEarn < 36 )	m_dMoneyLeftTime = 1.8f;
	else if ( m_nMoneyEarn > -40 && m_nMoneyEarn < 40 )	m_dMoneyLeftTime = 2.0f;
	else if ( m_nMoneyEarn > -44 && m_nMoneyEarn < 44 )	m_dMoneyLeftTime = 2.2f;
	else if ( m_nMoneyEarn > -48 && m_nMoneyEarn < 48 )	m_dMoneyLeftTime = 2.4f;
	else if ( m_nMoneyEarn > -52 && m_nMoneyEarn < 52 )	m_dMoneyLeftTime = 2.6f;
	else if ( m_nMoneyEarn > -56 && m_nMoneyEarn < 56 )	m_dMoneyLeftTime = 2.8f;
	else												m_dMoneyLeftTime = 3.0f;

	m_dMoneyLeftTimeBackup = m_dMoneyLeftTime;
	m_dMoneySoundLeftTime  = 0;
	m_bIsMoneySoundFinish  = false;

	// friend ship
	if ( m_eModeType != eModeType_Casino )
	{
		m_pBetMob->setFriendShipExp ( m_pBetMob->getFriendShipExp ( ) + 5 );
	}

	// unit class
	switch ( m_eModeType )
	{
		case eModeType_MyRoom_Casino100		:	m_pBetMob->setClassExp ( m_pBetMob->getClassExp ( ) + 0.3f );	break;
		case eModeType_MyRoom_Casino1000	:	m_pBetMob->setClassExp ( m_pBetMob->getClassExp ( ) + 0.7f );	break;
		case eModeType_MyRoom_Arcade_Easy	:	m_pBetMob->setClassExp ( m_pBetMob->getClassExp ( ) + 0.1f );	break;
		case eModeType_MyRoom_Arcade_Nor	:	m_pBetMob->setClassExp ( m_pBetMob->getClassExp ( ) + 0.5f );	break;
		case eModeType_MyRoom_Arcade_Hard	:	m_pBetMob->setClassExp ( m_pBetMob->getClassExp ( ) + 0.9f );	break;
		case eModeType_MyRoom_Survival		:	m_pBetMob->setClassExp ( m_pBetMob->getClassExp ( ) + cMyData::getObj ( )->m_nGamingKillCount * 0.03f );	break;
		default								:	break;
	}

	// global bet money
	cMyData::getObj ( )->m_nBettingMoney = 0;

	// save
	cMyData::getObj ( )->save ( );
}

KDvoid CasinoResultNode::updateMyRoomData ( KDvoid )
{
	if ( m_pBetMob == nullptr )
	{
		return;
	}

	std::vector<cUnit*>		aUnits;
	aUnits.push_back ( m_pBetMob );

	cMyData::getObj ( )->updateMyRoomUnitSaveDatas ( aUnits );
}

KDvoid CasinoResultNode::updateOpenFeint ( KDvoid )
{
#if 0
	//코인 업적 달성.
	if ( cMyData::getObj ( )->m_nMoney >= 500			)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_500_ID			andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 1000			)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_1000_ID			andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 10000			)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_10000_ID		andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 100000			)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_100000_ID		andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 1000000		)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_1000000_ID		andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 10000000		)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_10000000_ID		andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 100000000		)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_100000000_ID	andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 1000000000		)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_1000000000_ID	andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 10000000000LL	)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_10000000000_ID	andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nMoney >= 100000000000LL	)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_100000000000_ID	andPercentComplete:100 andShowNotification:true];

	//연승 업적 달성.
	if ( cMyData::getObj ( )->m_nSuccessive >= 2	  )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_2_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 5	  )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_5_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 10  )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_10_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 15  )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_15_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 20  )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_20_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 30  )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_30_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 40  )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_40_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 50  )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_50_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 100 )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_100_ID	 andPercentComplete:100 andShowNotification:true];
	if ( cMyData::getObj ( )->m_nSuccessive >= 1000)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_CONSECUTIVELY_1000_ID andPercentComplete:100 andShowNotification:true];

	//배팅 금액 업적 달성.
	if ( m_nMoneyEarn >= 100	 )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_BET_COIN_100_ID		andPercentComplete:100 andShowNotification:true];
	if ( m_nMoneyEarn >= 1000	 )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_BET_COIN_1000_ID		andPercentComplete:100 andShowNotification:true];
	if ( m_nMoneyEarn >= 10000	 )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_BET_COIN_10000_ID	andPercentComplete:100 andShowNotification:true];
	if ( m_nMoneyEarn >= 100000 )	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_BET_COIN_100000_ID	andPercentComplete:100 andShowNotification:true];
	if ( m_nMoneyEarn >= 1000000)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_BET_COIN_1000000_ID	andPercentComplete:100 andShowNotification:true];

	//fight count 업적 달성.
	double _10_Percent		 = ( (double)cMyData::getObj ( )->m_gameCount / 10	   ) * 100;
	double _100_Percent		 = ( (double)cMyData::getObj ( )->m_gameCount / 100	   ) * 100;
	double _1000_Percent	 = ( (double)cMyData::getObj ( )->m_gameCount / 1000	   ) * 100;
	double _10000_Percent	 = ( (double)cMyData::getObj ( )->m_gameCount / 10000	   ) * 100;
	double _100000_Percent	 = ( (double)cMyData::getObj ( )->m_gameCount / 100000   ) * 100;
	double _1000000_Percent	 = ( (double)cMyData::getObj ( )->m_gameCount / 1000000  ) * 100;
	double _10000000_Percent = ( (double)cMyData::getObj ( )->m_gameCount / 10000000 ) * 100;

	if ( _10_Percent	     > 100)	_10_Percent		  = 100;
	if ( _100_Percent	     > 100)	_100_Percent	  = 100;
	if ( _1000_Percent     > 100)	_1000_Percent	  = 100;
	if ( _10000_Percent    > 100)	_10000_Percent	  = 100;
	if ( _100000_Percent   > 100)	_100000_Percent   = 100;
	if ( _1000000_Percent	 > 100)	_1000000_Percent  = 100;
	if ( _10000000_Percent > 100)	_10000000_Percent = 100;

	if ( _10_Percent		 < 100)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_10_ID		 andPercentComplete:_10_Percent		  andShowNotification:false];
	else						[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_10_ID		 andPercentComplete:_10_Percent		  andShowNotification:true ];
	if ( _100_Percent		 < 100)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_100_ID		 andPercentComplete:_100_Percent	  andShowNotification:false];
	else						[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_100_ID		 andPercentComplete:_100_Percent	  andShowNotification:true ];
	if ( _1000_Percent	 < 100)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_1000_ID	 andPercentComplete:_1000_Percent	  andShowNotification:false];
	else						[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_1000_ID	 andPercentComplete:_1000_Percent	  andShowNotification:true ];
	if ( _10000_Percent	 < 100)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_10000_ID	 andPercentComplete:_10000_Percent	  andShowNotification:false];
	else						[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_10000_ID	 andPercentComplete:_10000_Percent	  andShowNotification:true ];
	if ( _100000_Percent	 < 100)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_100000_ID	 andPercentComplete:_100000_Percent	  andShowNotification:false];
	else						[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_100000_ID	 andPercentComplete:_100000_Percent	  andShowNotification:true ];
	if ( _1000000_Percent  < 100)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_1000000_ID	 andPercentComplete:_1000000_Percent  andShowNotification:false];
	else						[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_1000000_ID	 andPercentComplete:_1000000_Percent  andShowNotification:true ];
	if ( _10000000_Percent < 100)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_10000000_ID andPercentComplete:_10000000_Percent andShowNotification:false];
	else						[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_FIGHT_10000000_ID andPercentComplete:_10000000_Percent andShowNotification:true ];

	//lose 업적 달성.
	double _Lose_3_Percent	 = ( (double)cMyData::getObj ( )->m_loseCount / 3	  ) * 100;
	double _Lose_10_Percent	 = ( (double)cMyData::getObj ( )->m_loseCount / 10  ) * 100;
	double _Lose_30_Percent	 = ( (double)cMyData::getObj ( )->m_loseCount / 30  ) * 100;
	double _Lose_50_Percent	 = ( (double)cMyData::getObj ( )->m_loseCount / 50  ) * 100;
	double _Lose_100_Percent = ( (double)cMyData::getObj ( )->m_loseCount / 100 ) * 100;

	if ( _Lose_3_Percent	 > 100)	_Lose_3_Percent	  = 100;
	if ( _Lose_10_Percent	 > 100)	_Lose_10_Percent  = 100;
	if ( _Lose_30_Percent	 > 100)	_Lose_30_Percent  = 100;
	if ( _Lose_50_Percent	 > 100)	_Lose_50_Percent  = 100;
	if ( _Lose_100_Percent > 100)	_Lose_100_Percent = 100;

	if ( _Lose_3_Percent < 100)		[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_3_ID	andPercentComplete:_Lose_3_Percent	 andShowNotification:false];
	else							[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_3_ID	andPercentComplete:_Lose_3_Percent	 andShowNotification:true ];
	if ( _Lose_10_Percent < 100)		[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_10_ID	andPercentComplete:_Lose_10_Percent	 andShowNotification:false];
	else							[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_10_ID	andPercentComplete:_Lose_10_Percent	 andShowNotification:true ];
	if ( _Lose_30_Percent < 100)		[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_30_ID	andPercentComplete:_Lose_30_Percent	 andShowNotification:false];
	else							[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_30_ID	andPercentComplete:_Lose_30_Percent	 andShowNotification:true ];
	if ( _Lose_50_Percent < 100)		[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_50_ID	andPercentComplete:_Lose_50_Percent	 andShowNotification:false];
	else							[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_50_ID	andPercentComplete:_Lose_50_Percent	 andShowNotification:true ];
	if ( _Lose_100_Percent < 1000)	[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_100_ID	andPercentComplete:_Lose_100_Percent andShowNotification:false];
	else							[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_LOSE_100_ID	andPercentComplete:_Lose_100_Percent andShowNotification:true ];

	//랭크 등록.
	[OFHighScoreService setHighScore:(__int64_t)cMyData::getObj ( )->m_nMoney forLeaderboard:_OPENFEINT_LEADERBOARD_RICH_ID onSuccess:OFDelegate() onFailure:OFDelegate()];

	if ( m_eModeType != eModeType_Casino && m_eModeType != eModeType_MyRoom_Casino100 && m_eModeType != eModeType_MyRoom_Casino1000)
	{
		[OFHighScoreService setHighScore:(int)m_pBetMob->getKillCount() forLeaderboard:_OPENFEINT_LEADERBOARD_KILL_ID onSuccess:OFDelegate() onFailure:OFDelegate()];
	}

	if ( m_eModeType == eModeType_MyRoom_Survival)
	{
		[OFHighScoreService setHighScore:(int)cMyData::getObj ( )->m_gamingKillCount forLeaderboard:_OPENFEINT_LEADERBOARD_SURVIVAL_ID onSuccess:OFDelegate() onFailure:OFDelegate()];
	}
#endif	// 0
}

KDvoid CasinoResultNode::save ( KDvoid )
{
	// mode check
	if ( m_eModeType != eModeType_Casino && m_eModeType != eModeType_MyRoom_Casino100 && m_eModeType != eModeType_MyRoom_Casino1000 )
	{
		return;
	}

	// die check
	if ( m_pBetMob->getDie ( ) == true )
	{
		return;
	}

	// open
	KDFile*		pFile = kdFopen ( _FILE_NAME_MYROOM_RESULT_SAVE, "wb" );
	if ( pFile != nullptr )
	{
		// version
		std::string		sVersionStr		 = _GAME_SAVE_FILE_SIZE_LATEST_VERSION_STR;
		KDint			nVersionStrCount = sVersionStr.length ( ) + 1;
		KDchar			cStringNull		 = 0;
		kdFwrite ( &nVersionStrCount    , sizeof ( KDint  ), 1				 , pFile );
		kdFwrite ( sVersionStr.c_str ( ), sizeof ( KDchar ), nVersionStrCount, pFile );
		kdFwrite ( &cStringNull		    , sizeof ( KDchar ), 1				 , pFile );

		// mode type
		KDint			nModeType = m_eModeType;
		kdFwrite ( &nModeType, sizeof ( KDint ), 1, pFile );

		// mob
		KDint			nCreateIndex		= m_pBetMob->getCreateIndex			( );
		KDint			nSeatIndex			= m_pBetMob->getSeatIndex			( );
		KDint			nCreateInfoFileType	= m_pBetMob->getCreateInfoFileType	( );
		KDint			nUniqueType			= m_pBetMob->getUniqueType			( );
		KDint			nSlotIndex			= m_pBetMob->getSlotIndex			( );
		KDint			nUnitID				= m_pBetMob->getUnitID				( );
		KDfloat			fExp				= m_pBetMob->getExp					( );
		KDfloat			fClassExp			= m_pBetMob->getClassExp			( );
		KDfloat			fFriendShipExp		= m_pBetMob->getFriendShipExp		( );
		KDint			nKillCount			= m_pBetMob->getKillCount			( );
		KDint			nIsArcadeEasyClear	= m_pBetMob->getIsArcadeEasyClear	( );
		KDint			nIsArcadeNorClear	= m_pBetMob->getIsArcadeNorClear	( );
		KDint			nIsArcadeHardClear	= m_pBetMob->getIsArcadeHardClear	( );

		KDint			nHp		= m_pBetMob->getHP	   ( );
		KDint			nAtk	= m_pBetMob->getATK	   ( );
		KDint			nDef	= m_pBetMob->getDEF	   ( );
		KDint			nAgl	= m_pBetMob->getAGL	   ( );
		KDint			nCrt	= m_pBetMob->getCRT	   ( );
		KDint			nHpMax	= m_pBetMob->getHPMax  ( );
		KDint			nAtkMax = m_pBetMob->getATKMax ( );
		KDint			nDefMax = m_pBetMob->getDEFMax ( );
		KDint			nAglMax	= m_pBetMob->getAGLMax ( );
		KDint			nCrtMax = m_pBetMob->getCRTMax ( );

		kdFwrite ( &nCreateIndex		, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &nSeatIndex			, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &nCreateInfoFileType	, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &nUniqueType			, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &nSlotIndex			, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &nUnitID				, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &fExp				, sizeof ( KDfloat ), 1, pFile );
		kdFwrite ( &fClassExp			, sizeof ( KDfloat ), 1, pFile );
		kdFwrite ( &fFriendShipExp		, sizeof ( KDfloat ), 1, pFile );
		kdFwrite ( &nKillCount			, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &nIsArcadeEasyClear	, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &nIsArcadeNorClear	, sizeof ( KDint   ), 1, pFile );
		kdFwrite ( &nIsArcadeHardClear	, sizeof ( KDint   ), 1, pFile );

		kdFwrite ( &nHp		, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nAtk	, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nDef	, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nAgl	, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nCrt	, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nHpMax	, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nAtkMax	, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nDefMax	, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nAglMax	, sizeof ( KDint ), 1, pFile );
		kdFwrite ( &nCrtMax	, sizeof ( KDint ), 1, pFile );

		// bet init
		KDint64		nBetMoney		  = m_pBetMob->getBetMoney			    ( );
		KDfloat		fBetMagnification = m_pBetMob->getDividendMagnification ( );

		kdFwrite ( &nBetMoney		 , sizeof ( KDint64 ), 1, pFile );
		kdFwrite ( &fBetMagnification, sizeof ( KDfloat ), 1, pFile );

		// successive
		kdFwrite ( &cMyData::getObj ( )->m_nSuccessive, sizeof ( KDint ), 1, pFile );

		// close
		kdFclose ( pFile );
	}
}

KDvoid CasinoResultNode::step ( KDfloat fDelta )
{
	// tickCount
	m_dTickCount += fDelta;

	if ( m_dBeforeTickCount == 0 )	
	{
		m_dBeforeTickCount = m_dTickCount;
	}
	KDdouble	dLeftTickCount = m_dTickCount - m_dBeforeTickCount;
	m_dBeforeTickCount = m_dTickCount;

	// my data update
	cMyData::getObj ( )->update ( dLeftTickCount );

	if ( dLeftTickCount > 0.1f )
	{
		dLeftTickCount = 0.1f;
	}

	// node change
	if ( this->updateNodeChage ( dLeftTickCount ) == true )	
	{
		return;
	}

	// back decorations
	for ( auto pNode : m_aBackDecos )
	{
		pNode->update ( dLeftTickCount );
	}

	// mob ani
	m_dAniStartLeftTime -= dLeftTickCount;
	if ( m_dAniStartLeftTime < 0 )
	{
		if ( m_eModeType == eModeType_MyRoom_Survival || m_pBetMob->getDie ( ) == false )	m_pBetMob->changeAni ( eUnitAniType_Attack );
		else																				m_pBetMob->aniStart ( );

		m_dAniStartLeftTime = 2;
	}

	// mobs
	m_pBetMob->update ( dLeftTickCount );

	// exp update
	this->updateExp ( dLeftTickCount );

	// level update
	this->updateLevelUp ( dLeftTickCount );

	// money
	this->updateMoney ( dLeftTickCount );
}

KDvoid CasinoResultNode::updateExp ( KDdouble dLeftTime )
{
	// check
	if ( m_eModeType == eModeType_Casino )
	{
		return;
	}

	// my data get exp
	KDfloat		fPlusExp = 0;
	for ( auto& rData : cMyData::getObj ( )->m_aMyRoomUnitSaveDatas )
	{
		if (  rData.nUnitID == m_pBetMob->getUnitID ( ) )
		{
			fPlusExp = rData.fExp - m_pBetMob->getExp ( );
			break;
		}
	}

	// unit exp
	m_pBetMob->setExp ( m_pBetMob->getExp ( ) + fPlusExp );

	// exp
	m_fNowExp   += fPlusExp;
	m_fAfterExp += fPlusExp;

	if ( m_pBetMob->getIsParaMax ( ) == false )
	{
		// 플러스 경험치에 대한 레벨 업 level up
		while ( m_pBetMob->getExp ( ) >= 100 )
		{
			// para
			KDint	nHp	 = _MYROOM_LEVELUP_POINT_HP_MIN  + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_HP_MAX  - _MYROOM_LEVELUP_POINT_HP_MIN  );
			KDint	nAtk = _MYROOM_LEVELUP_POINT_ATK_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_ATK_MAX - _MYROOM_LEVELUP_POINT_ATK_MIN );
			KDint	nDef = _MYROOM_LEVELUP_POINT_DEF_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_DEF_MAX - _MYROOM_LEVELUP_POINT_DEF_MIN );
			KDint	nAgl = _MYROOM_LEVELUP_POINT_AGL_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_AGL_MAX - _MYROOM_LEVELUP_POINT_AGL_MIN );
			KDint	nCrt = _MYROOM_LEVELUP_POINT_CRT_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_CRT_MAX - _MYROOM_LEVELUP_POINT_CRT_MIN );

			m_pBetMob->paraUp ( nHp, nAtk, nDef, nAgl, nCrt );

			KDint	nHp2  = 0;
			KDint	nAtk2 = 0;
			KDint	nDef2 = 0;
			KDint	nAgl2 = 0;
			KDint	nCrt2 = 0;

			if ( nHp  > 0 )	nHp2  = m_pBetMob->getHP  ( );
			if ( nAtk > 0 )	nAtk2 = m_pBetMob->getATK ( );
			if ( nDef > 0 )	nDef2 = m_pBetMob->getDEF ( );
			if ( nAgl > 0 )	nAgl2 = m_pBetMob->getAGL ( );
			if ( nCrt > 0 )	nCrt2 = m_pBetMob->getCRT ( );

			// level effect add
			m_aUILevels.push_back ( new cResultLevelUp ( this, nHp2, nAtk2, nDef2, nAgl2, nCrt2 ) );

			// exp
			m_pBetMob->setExp ( m_pBetMob->getExp ( ) - 100 );

			// save my room data
			this->updateMyRoomData ( );
		}
	}		

	// exp add
	if ( m_fNowExp < m_fAfterExp )
	{
		m_fNowExp += dLeftTime * 10;
		if ( m_fNowExp > m_fAfterExp )
		{
			m_fNowExp = m_fAfterExp;
		}

		// level up check
		while ( m_fNowExp >= 100 )
		{
			for ( auto pLevel : m_aUILevels )
			{
				if ( pLevel->getIsStart ( ) == false )
				{
					pLevel->start ( );
					break;
				}
			}

			m_fStartExp	= 0;
			m_fNowExp   -= 100;
			m_fAfterExp -= 100;
		}
	}

	// UI
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		m_pUIExpBack ->get ( )->setVisible ( true );
		m_pUIExpGauge->get ( )->setVisible ( true );
		m_pUIExpFont ->get ( )->setVisible ( true );

		KDfloat		fWidth = 119 * (KDfloat) ( (KDint) m_fNowExp ) / 100.0f;
		m_pUIExpGauge->get ( )->setTextureRect ( Rect ( 0, 0, fWidth, 6 ) );
	}
	else
	{
		// gauge 1
		KDfloat		fWidth = 245 * (KDfloat) ( (KDint) m_fNowExp ) / 100.0f;
		m_pUIBarExpGauge1->get ( )->setTextureRect ( Rect ( 0, 0, fWidth, 22 ) );

		// gauge 2
		KDfloat		fWidth2 = 245 * (KDfloat) ( (KDint) m_fAfterExp ) / 100.0f;
		if ( fWidth2 > 245 )
		{
			fWidth2 = 245;
		}
		m_pUIBarExpGauge2->get ( )->setTextureRect ( Rect ( 0, 0, fWidth2, 22 ) );
	}
}

KDvoid CasinoResultNode::updateLevelUp ( KDdouble dLeftTime )
{
	// loop
	for ( auto pLevel : m_aUILevels )
	{
		// update
		KDint	nResult = pLevel->update ( dLeftTime );

		// status change
		// hp
		if ( nResult == 1)
		{
			KDint		nHp = pLevel->getHp ( );

			if ( nHp != 0 )
			{
				m_pLabels->setStr   ( m_aUIMobDetailPara [ 0 ], ccszf ( "HP : %02d / %02d", nHp, nHp ) );
				m_pLabels->setColor ( m_aUIMobDetailPara [ 0 ], 0, 255, 0 );

				if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000)
				{
					m_pLabels->setStr   ( m_pUIMobBarHP, ccszf ( "HP : %02d", nHp ) );
					m_pLabels->setColor ( m_pUIMobBarHP, 0, 255, 0 );
				}
			}
		}
		// atk
		else if ( nResult == 2 )
		{
			KDint		nAtk = pLevel->getAtk ( );

			if ( nAtk != 0 )
			{
				m_pLabels->setStr   ( m_aUIMobDetailPara [ 1 ], ccszf ( "ATK : %02d", nAtk ) );
				m_pLabels->setColor ( m_aUIMobDetailPara [ 1 ], 0, 255, 0 );

				if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
				{
					m_pLabels->setStr   ( m_pUIMobBarATK, ccszf ( "ATK : %02d", nAtk ) );
					m_pLabels->setColor ( m_pUIMobBarATK, 0, 255, 0 );
				}
			}
		}
		// def
		else if ( nResult == 3 )
		{
			KDint		nDef = pLevel->getDef ( );

			if ( nDef != 0 )
			{
				m_pLabels->setStr   ( m_aUIMobDetailPara [ 2 ], ccszf ( "DEF : %02d", nDef ) );
				m_pLabels->setColor ( m_aUIMobDetailPara [ 2 ], 0, 255, 0 );

				if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
				{
					m_pLabels->setStr   ( m_pUIMobBarDEF, ccszf ( "DEF : %02d", nDef ) );
					m_pLabels->setColor ( m_pUIMobBarDEF, 0, 255, 0 );
				}
			}
		}
		// agl
		else if ( nResult == 4)
		{
			KDint		nAgl = pLevel->getAgl ( );

			if ( nAgl != 0 )
			{
				m_pLabels->setStr   ( m_aUIMobDetailPara [ 3 ], ccszf ( "AGL : %02d", nAgl ) );
				m_pLabels->setColor ( m_aUIMobDetailPara [ 3 ], 0, 255, 0 );
			}
		}
		// crt
		else if ( nResult == 5 )
		{
			KDint		nCrt = pLevel->getCrt ( );

			if ( nCrt != 0 )
			{
				m_pLabels->setStr   ( m_aUIMobDetailPara [ 4 ], ccszf ( "CRT : %02d", nCrt ) );
				m_pLabels->setColor ( m_aUIMobDetailPara [ 4 ], 0, 255, 0 );
			}
		}
	}
}

KDbool CasinoResultNode::updateNodeChage ( KDdouble dLeftTIme )
{
	if ( m_pNodeChangeImg != nullptr )
	{
		if ( m_pNodeChangeImg->update ( dLeftTIme ) == true )
		{
			if ( m_bIsNodeChangeIn == true )
			{
				// play
				if ( cMyData::getObj ( )->m_bIsOptionSound != false )
				{
					if ( m_eModeType == eModeType_MyRoom_Survival )
					{
						SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "bm_resultpage.mp3", false );
					}
					else
					{
						if ( m_pBetMob->getDie ( ) == false )	SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "bm_resultpage.mp3", false );
						else									SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "result_lose.mp3"  , false );
					}
				}

				m_bIsNodeChangeIn = false;
				CC_SAFE_RELEASE ( m_pNodeChangeImg );

				m_pBtnExit->setEnabled ( true );
				m_pBtnRank->setEnabled ( true );

				if ( m_pBetMob->getDie ( ) == false )	m_pBtnRetry->setEnabled ( true  );
				else									m_pBtnRetry->setEnabled ( false );
			}
			else if ( m_bIsNodeChangeOut == true )
			{
				m_bIsNodeChangeOut = false;
				CC_SAFE_RELEASE ( m_pNodeChangeImg );

				cUnit*	pBetMob = nullptr;
				if ( m_bIsGoBattle == true || m_eModeType != eModeType_Casino )
				{
					pBetMob = m_pBetMob;
				}				

				// cenece change
				if ( m_bIsGoBattle == false )
				{
					if ( m_eModeType == eModeType_Casino )	
					{
						Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBetNode::create ( ) ) );
					}
					else
					{
						Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, MyRoomNode::create ( pBetMob->getUnitID ( ) ) ) );
					}
				}
				else
				{
					this->sceneGoBattle ( pBetMob );
				}

				this->releaseObject ( );

				return true;
			}
		}
	}

	return false;
}

KDvoid CasinoResultNode::updateMoney ( KDdouble dLeftTime )
{
	KDint64			nMoney			= m_nMoney + cMyData::getObj ( )->m_nMoney - m_nMoneyBackup2;
	std::string		sMoney			= ccszf ( "%lld", nMoney );
	std::string		sResultMoney	= ccszf ( "%lld", nMoney );
	std::string		sEarnMoney		= "";

		 if ( m_nMoneyEarn < 0 )	sEarnMoney = ccszf ( "- %lld", -m_nMoneyEarn );
	else if ( m_nMoneyEarn > 0 )	sEarnMoney = ccszf ( "+ %lld",  m_nMoneyEarn );
	else							sEarnMoney = ccszf ( "0" );

	for ( KDint i = sMoney.size ( ) - 3; i > 0; i -= 3 )
	{
		sMoney.insert ( i, "," );		
	}

	// top money
	m_pLabelTopMoney->setString ( sMoney.c_str ( ) );
	m_pLabelTopMoney->setPosition ( Point ( 459 - m_pLabelTopMoney->getSize ( ).width, 300 ) );

	// time get money
	cTimeGetMoney::getObj ( )->update ( dLeftTime, cMyData::getObj ( )->m_nMoney );
	cTimeGetMoney::getObj ( )->setPoint ( Point ( 432 - m_pLabelTopMoney->getSize ( ).width, 302 ) );

	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		// earnMoney
		m_pLabelEarnMoney->setString ( sEarnMoney.c_str ( ) );
		m_pLabelEarnMoney->setPosition ( Point ( 444 - m_pLabelEarnMoney->getSize ( ).width, 137 ) ); 

		// result Money
		m_pLabelResultMoney->setString ( sResultMoney.c_str ( ) );
		m_pLabelResultMoney->setPosition ( Point ( 444 - m_pLabelResultMoney->getSize ( ).width, 95 ) ); 
	}		

	// update money
	if ( m_dMoneyWaitLeftTime > 0 )
	{
		m_dMoneyWaitLeftTime -= dLeftTime;
		return;
	}

	if ( m_nMoneyEarn == 0 )
	{
		if ( m_bIsMoneySoundFinish == false )
		{
			if ( cMyData::getObj ( )->m_bIsOptionSound != false )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "casino_result_coin_2.mp3" );
			}

			m_bIsMoneySoundFinish = true;
		}

		return;
	}

	m_dMoneySoundLeftTime -= dLeftTime;
	if ( m_dMoneySoundLeftTime <= 0 )
	{
		if ( cMyData::getObj ( )->m_bIsOptionSound != false )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "casino_result_coin_1.mp3" );
		}

		m_dMoneySoundLeftTime = 0.07f;
	}

	m_dMoneyLeftTime -= dLeftTime;

	if ( m_dMoneyLeftTime <= 0 )
	{
		m_nMoneyEarn = 0;
		m_nMoney = m_nMoneyBackup + m_nMoneyEarnBackup;
	}
	else
	{
		m_nMoneyEarn = m_nMoneyEarnBackup * (KDdouble) ( m_dMoneyLeftTime / m_dMoneyLeftTimeBackup );
		m_nMoney = m_nMoneyBackup + ( m_nMoneyEarnBackup - m_nMoneyEarn );
	}
}

KDvoid CasinoResultNode::sceneGoBattle ( cUnit* pBetMob )
{
	// init
	cUnitCreateInfos*		pMobCreate1	= nullptr;
	cUnitCreateInfos*		pMobCreate2	= nullptr;
	eCreateInfoFileType		eFileType1	= eCreateInfoFileType_Casino_1;
	eCreateInfoFileType		eFileType2	= eCreateInfoFileType_Casino_2;
	std::vector<cUnit*>		aMobs;

	// new create info
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 )
	{
		pMobCreate1 = new cUnitCreateInfos ( "casinoMobCreateInfos.par"  );
		pMobCreate2 = new cUnitCreateInfos ( "casinoMobCreateInfos2.par" );

		eFileType1 = eCreateInfoFileType_Casino_1;
		eFileType2 = eCreateInfoFileType_Casino_2;
	}
	else
	{
		pMobCreate1 = new cUnitCreateInfos ( "casinoMobCreateInfos3.par" );
		pMobCreate2 = new cUnitCreateInfos ( "casinoMobCreateInfos4.par" );

		eFileType1 = eCreateInfoFileType_Casino_3;
		eFileType2 = eCreateInfoFileType_Casino_4;
	}

	pMobCreate1->autorelease ( );
	pMobCreate2->autorelease ( );

	// create info 를 어떻게 세팅할지 정한다.
	std::vector<KDint>		aSetCreateInfoTypes;

	aSetCreateInfoTypes.push_back ( 0 );
	aSetCreateInfoTypes.push_back ( 0 );
	aSetCreateInfoTypes.push_back ( 0 );
	aSetCreateInfoTypes.push_back ( 0 );

	if ( cMyData::getObj ( )->m_nSuccessive >= 72 )
	{
		aSetCreateInfoTypes [ 0 ] = 1;
		aSetCreateInfoTypes [ 1 ] = 1;
		aSetCreateInfoTypes [ 2 ] = 1;
		aSetCreateInfoTypes [ 3 ] = 1;
	}
	else if ( cMyData::getObj ( )->m_nSuccessive >= 62 )
	{
		aSetCreateInfoTypes [ 0 ] = 1;
		aSetCreateInfoTypes [ 1 ] = 1;
		aSetCreateInfoTypes [ 2 ] = 1;
		aSetCreateInfoTypes [ 3 ] = kdRand ( ) % 2;
	}
	else if ( cMyData::getObj ( )->m_nSuccessive >= 52 )
	{
		aSetCreateInfoTypes [ 0 ] = 1;
		aSetCreateInfoTypes [ 1 ] = 1;
		aSetCreateInfoTypes [ 2 ] = 1;
	}
	else if ( cMyData::getObj ( )->m_nSuccessive >= 42 )
	{
		aSetCreateInfoTypes [ 0 ] = 1;
		aSetCreateInfoTypes [ 1 ] = 1;
		aSetCreateInfoTypes [ 2 ] = kdRand ( ) % 2;
	}
	else if ( cMyData::getObj ( )->m_nSuccessive >= 32 )
	{
		aSetCreateInfoTypes [ 0 ] = 1;
		aSetCreateInfoTypes [ 1 ] = 1;
	}
	else if ( cMyData::getObj ( )->m_nSuccessive >= 22 )
	{
		aSetCreateInfoTypes [ 0 ] = 1;
		aSetCreateInfoTypes [ 1 ] = kdRand ( ) % 2;
	}
	else if ( cMyData::getObj ( )->m_nSuccessive >= 12 )
	{
		aSetCreateInfoTypes [ 0 ] = 1;
	}
	else if ( cMyData::getObj ( )->m_nSuccessive >= 5 )
	{
		aSetCreateInfoTypes [ 0 ] = kdRand ( ) % 2;
	}

	for ( KDint i = 0; i < 100; i++ )
	{
		KDint	nChangeSeat1 = kdRand ( ) % 4;
		KDint	nChangeSeat2 = kdRand ( ) % 4;

		KDint	nSave						 = aSetCreateInfoTypes [ nChangeSeat1 ];
		aSetCreateInfoTypes [ nChangeSeat1 ] = aSetCreateInfoTypes [ nChangeSeat2 ];
		aSetCreateInfoTypes [ nChangeSeat2 ] = nSave;
	}

	// mobs
	KDint		nInsertRand		  = 0;
	KDint		nSetInfoTypeIndex = 0;
	if ( m_eModeType == eModeType_Casino )
	{
		nInsertRand = kdRand ( ) % 5;
	}

	for ( KDint i = 0; i < 5; i++ )
	{
		if ( i == nInsertRand )
		{
			// bet money
			pBetMob->setBetMoney ( pBetMob->getBetMoney ( ) + m_nMoneyEarnBackup );

			// money
			cMyData::getObj ( )->m_nMoney -= pBetMob->getBetMoney ( );
			cMyData::getObj ( )->save ( );

			// texture
			pBetMob->texRelease	( eUnitAniType_Name_a   );
			pBetMob->texRelease	( eUnitAniType_Name_b   );
			pBetMob->texLoad	( eUnitAniType_Portrait );
			pBetMob->texLoad	( eUnitAniType_Normal   );
			pBetMob->texLoad	( eUnitAniType_Attack   );
			pBetMob->texLoad	( eUnitAniType_Special  );
			pBetMob->texLoad	( eUnitAniType_Damage   );

			// reset parameter
			pBetMob->setUseHp ( pBetMob->getHP ( ) );
			pBetMob->resetParaAdds ( );

			// change ani
			pBetMob->changeAni ( eUnitAniType_Normal );

			// set color
			pBetMob->setColor ( 255, 255, 255, 255 );

			// seat index
			pBetMob->setSeatIndex ( i );

			// switches
			pBetMob->setIsAutoChangeNor ( true );

			// push
			pBetMob->retain ( );
			aMobs.push_back ( pBetMob );
			continue;
		}

		// create
		const cUnitCreateInfo*	pCreateInfo = nullptr;
		if ( aSetCreateInfoTypes [ nSetInfoTypeIndex ] == 0 )	pCreateInfo = pMobCreate1->getUnitInfoRand ( );
		else													pCreateInfo = pMobCreate2->getUnitInfoRand ( );

		cUnit*					pMob = nullptr;

		if ( aSetCreateInfoTypes [ nSetInfoTypeIndex ] == 0 )	pMob = new cUnit ( this, pCreateInfo, eFileType1, LAYER_CASINO_RESULT_MOB, -1, pCreateInfo->nCreateIndex, i );
		else													pMob = new cUnit ( this, pCreateInfo, eFileType2, LAYER_CASINO_RESULT_MOB, -1, pCreateInfo->nCreateIndex, i );

		// 필요한 텍스쳐만 로드.
		pMob->texLoad ( eUnitAniType_Normal  );
		pMob->texLoad ( eUnitAniType_Attack  );
		pMob->texLoad ( eUnitAniType_Special );
		pMob->texLoad ( eUnitAniType_Damage  );

		// add
		aMobs.push_back ( pMob );

		// info type index
		++nSetInfoTypeIndex;
	}

	// mob dividend magnifications, win percent 
	cMobDividend*	pMobDividend = new cMobDividend ( aMobs );
	pMobDividend->autorelease ( );
	for ( KDuint i = 0; i < aMobs.size ( ); i++ )
	{
		aMobs [ i ]->setDividendMagnification ( pMobDividend->getMagnification ( i ) );
	}

	// change node
	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithMobs ( aMobs, m_eModeType ) ) );

	// clear
	SAFE_RELEASE_ARRAYS ( aMobs );
}

KDvoid CasinoResultNode::exitCallback ( Object* pSender )
{
	m_bIsNodeChangeOut = true;
	m_bIsGoBattle	   = false;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg = new cNodeChangeImgs ( this, true );

	m_pBtnExit	 ->setEnabled ( false );
	m_pBtnRetry	 ->setEnabled ( false );
	m_pBtnRank	 ->setEnabled ( false );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid CasinoResultNode::retryCallback ( Object* pSender )
{
	m_bIsNodeChangeOut = true;
	m_bIsGoBattle	   = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );

	m_pBtnExit	 ->setEnabled ( false );
	m_pBtnRetry	 ->setEnabled ( false );
	m_pBtnRank	 ->setEnabled ( false );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid CasinoResultNode::faceBookCallback ( Object* pSender )
{

}

KDvoid CasinoResultNode::twitterCallback ( Object* pSender )
{
	
}

KDvoid CasinoResultNode::rankCallback ( Object* pSender )
{
#if 0
	[ OpenFeint launchDashboardWithListLeaderboardsPage];
#endif	//  0
}
