/* -----------------------------------------------------------------------------------
 *
 *      File            CasinoBetNode.cpp
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
#include "CasinoBetNode.h"
#include "MyData/MyData.h"
#include "Object/Menu/SoundMenuItem.h"
#include "Object/TimeGetMoney/TimeGetMoney.h"
#include "ShopNode/ShopNode.h"
#include "TitleNode/Main/MainNode.h"
#include "CasinoNode/CasinoBattleNode/CasinoBattleNode.h"

#define _CHANGE_STATE_TIME_1	0.06f
#define _CHANGE_STATE_TIME_2	0.08f
#define _CHANGE_STATE_TIME_3	0.1f
#define _CHANGE_STATE_TIME_4	0.12f
#define _CHANGE_STATE_TIME_5	0.14f

CasinoBetNode::CasinoBetNode ( KDvoid )
{
	m_pNodeChangeImg	= nullptr;
	m_pShopNode			= nullptr;
	m_pSprBack			= nullptr;
	m_pCoverColor		= nullptr;
	m_pBtnExit			= nullptr;
	m_pBtnShop			= nullptr;
	m_pBtnGameStart		= nullptr;
	m_pBtnBack			= nullptr;
	m_pBtn1Coin			= nullptr;
	m_pBtn10Coin		= nullptr;
	m_pBtn100Coin		= nullptr;
	m_pBtn1000Coin		= nullptr;
	m_pBtnMinCoin		= nullptr;
	m_pBtnMaxCoin		= nullptr;
	m_pBtnShopExit		= nullptr;

	m_pLabels			= nullptr;
	m_pMobDividend		= nullptr;
	m_pUIMoney			= nullptr;
	m_pUISelMob			= nullptr;
	m_pUISelMobText		= nullptr;
	m_pUIMobBarSelect	= nullptr;
	m_pUIMobBarSelect2	= nullptr;
	m_pUIMobBarSelect3	= nullptr;	
	m_pUICoinBack		= nullptr;
	m_pUICoinCover		= nullptr;
	m_pUINotCoinPopup	= nullptr;
	m_pUIShopCursor		= nullptr;
}

CasinoBetNode::~CasinoBetNode ( KDvoid )
{
	this->releaseObject ( );
}

KDvoid CasinoBetNode::releaseObject ( KDvoid )
{
	// node chage
	CC_SAFE_RELEASE ( m_pNodeChangeImg );

	// shop node
	SAFE_REMOVE_CHILD ( m_pShopNode );

	// back img
	SAFE_REMOVE_CHILD ( m_pSprBack );

	// back decorations
	SAFE_RELEASE_ARRAYS ( m_aBackDecos );

	// cover color
	SAFE_REMOVE_CHILD ( m_pCoverColor );

	// Btns
	SAFE_REMOVE_CHILD ( m_pBtnExit		);
	SAFE_REMOVE_CHILD ( m_pBtnShop		);
	SAFE_REMOVE_CHILD ( m_pBtnGameStart );
	SAFE_REMOVE_CHILD ( m_pBtnBack		);
	SAFE_REMOVE_CHILD ( m_pBtn1Coin		);
	SAFE_REMOVE_CHILD ( m_pBtn10Coin	);
	SAFE_REMOVE_CHILD ( m_pBtn100Coin	);
	SAFE_REMOVE_CHILD ( m_pBtn1000Coin	);
	SAFE_REMOVE_CHILD ( m_pBtnMinCoin	);
	SAFE_REMOVE_CHILD ( m_pBtnMaxCoin	);
	SAFE_REMOVE_CHILD ( m_pBtnExit		);

	// labels
	CC_SAFE_RELEASE ( m_pLabels );

	// mob dividend
	CC_SAFE_RELEASE ( m_pMobDividend );

	// UI money
	CC_SAFE_RELEASE ( m_pUIMoney );

	// sel mob
	CC_SAFE_RELEASE ( m_pUISelMob );
	CC_SAFE_RELEASE ( m_pUISelMobText );

	CC_SAFE_RELEASE ( m_pUIMobBarSelect );
	CC_SAFE_RELEASE ( m_pUIMobBarSelect2 );
	CC_SAFE_RELEASE ( m_pUIMobBarSelect3 );
	CC_SAFE_RELEASE ( m_pNodeChangeImg );

	// UI mob num
	SAFE_RELEASE_ARRAYS ( m_aUIMobNums );

	// UI mob bar
	SAFE_RELEASE_ARRAYS ( m_aUIMobBars );

	// UI mob num
	SAFE_RELEASE_ARRAYS ( m_aUIMobBarNums );

	// UI mob parameters
	m_aUIMobBarHPs .clear ( );
	m_aUIMobBarATKs.clear ( );
	m_aUIMobBarDEFs.clear ( );

	//UI mob Detail parameter
	m_aUIMobDetailPara.clear ( );

	// UI mob dividend magnifications
	SAFE_RELEASE_ARRAYS ( m_aUIMobBarMagnifications  );
	SAFE_RELEASE_ARRAYS ( m_aUIMobBarMagnifications2 );
	SAFE_RELEASE_ARRAYS ( m_aUIMobBarMagnifications3 );

	// UI skill
	SAFE_RELEASE_ARRAYS ( m_aUISkillIcons  );
	m_aUISkillStrs .clear ( );

	// UI coin state
	CC_SAFE_RELEASE ( m_pUICoinBack  );
	CC_SAFE_RELEASE ( m_pUICoinCover );

	// UI bet coin scroller
	SAFE_RELEASE_ARRAYS ( m_aUIBetCoins );

	// UI not coin
	CC_SAFE_RELEASE ( m_pUINotCoinPopup );
	CC_SAFE_RELEASE ( m_pUIShopCursor   );

	m_aBarSelectRects.clear ( );

	// mobs
	SAFE_RELEASE_ARRAYS ( m_aMobs );
}

KDbool CasinoBetNode::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	// init random
	kdSrand ( kdTime ( 0 ) );

	// switch
	auto	pListener = EventListenerTouchOneByOne::create ( );
	pListener->onTouchBegan = CC_CALLBACK_2 ( CasinoBetNode::onTouchBegan, this );
	pListener->onTouchMoved = CC_CALLBACK_2 ( CasinoBetNode::onTouchMoved, this );
	pListener->onTouchEnded = CC_CALLBACK_2 ( CasinoBetNode::onTouchEnded, this );
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pListener, this );

	// touch
	m_nTidCount = 1;

	// time
	m_dTickCount = 0;
	m_dBeforeTickCount	= 0;

	// back img
	m_pSprBack = Sprite::create ( "Bet/bet_bg.png" );
	m_pSprBack->setPosition	( Point ( m_tContentSize ) / 2 );
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

	// cover color
	Color4B		tColor;

	tColor.r = 0;
	tColor.g = 0;
	tColor.b = 0;	
	tColor.a = 100;

	m_pCoverColor = LayerColor::create ( tColor, _SCR_W, _SCR_H ); 
	m_pCoverColor->setPosition ( Point::ZERO );
	m_pCoverColor->setVisible ( false );

	this->addChild ( m_pCoverColor, LAYER_BETVIEW_COVER );

	// state
	m_eState = eBatStateType_AllView;
	m_dStateChangeLeftTime = 0;

	// labels
	m_pLabels = new cLabels ( this );

	// pMob crate info
	cUnitCreateInfos*	pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos.par" );
	pMobCreate->autorelease ( );
	
	// mobs
	for ( KDint i = 0; i < 5; i++ )
	{
		// get save data
		const cBetMobData*	pSaveData = cMyData::getObj ( )->getBetMobData ( i );

		//  create
		cUnit*		pMob = nullptr;
		if ( pSaveData != nullptr )
		{
			const cUnitCreateInfo*		pCreateInfo = pMobCreate->getUnitInfo ( pSaveData->nCreateIndex );

			if ( pCreateInfo != nullptr )
			{
				pMob = new cUnit ( this, pCreateInfo, eCreateInfoFileType_Casino_1, LAYER_BETVIEW_MOB, -1, pCreateInfo->nCreateIndex, i );
			}
			else
			{
				const cUnitCreateInfo*	pCreateInfo = pMobCreate->getUnitInfoRand ( );
				pMob = new cUnit ( this, pCreateInfo, eCreateInfoFileType_Casino_1, LAYER_BETVIEW_MOB, -1, pCreateInfo->nCreateIndex, i );
			}
		}
		else
		{
			const cUnitCreateInfo*		pCreateInfo = pMobCreate->getUnitInfoRand ( );
			pMob = new cUnit ( this, pCreateInfo, eCreateInfoFileType_Casino_1, LAYER_BETVIEW_MOB, -1, pCreateInfo->nCreateIndex, i );
		}

		// load
		if ( pSaveData != nullptr )
		{
			pMob->load ( pSaveData->nHp, pSaveData->nAtk, pSaveData->nDef, pSaveData->nAgl, pSaveData->nCrt );
		}

		// 필요한 텍스쳐만 로드.
		pMob->texLoad ( eUnitAniType_Name_a   );
		pMob->texLoad ( eUnitAniType_Name_b   );
		pMob->texLoad ( eUnitAniType_Portrait );
		pMob->texLoad ( eUnitAniType_Normal   );

		pMob->setVisibleNameB ( true );
		pMob->setVisiblePortrait ( true );

		// point
		pMob->setPoint      ( Point ( 65, 174 ) );
		pMob->setPointNameA ( Point ( 24, 316 ) );
		switch(i)
		{
			case 0	:	pMob->setPointNameB ( Point ( 173, 278 ) );	pMob->setPointPortrait ( Point ( 410, 277 ) );		break;
			case 1	:	pMob->setPointNameB ( Point ( 173, 223 ) );	pMob->setPointPortrait ( Point ( 410, 222 ) );		break;
			case 2	:	pMob->setPointNameB ( Point ( 173, 168 ) );	pMob->setPointPortrait ( Point ( 410, 167 ) );		break;
			case 3	:	pMob->setPointNameB ( Point ( 173, 113 ) );	pMob->setPointPortrait ( Point ( 410, 112 ) );		break;
			case 4	:	pMob->setPointNameB ( Point ( 173,  58 ) );	pMob->setPointPortrait ( Point ( 410,  57 ) );		break;
		}

		// add
		m_aMobs.push_back ( pMob );
	}

	// save bet mob
	cMyData::getObj ( )->clearBetMobData ( );

	for ( auto pMob : m_aMobs )
	{
		cMyData::getObj ( )->addBetMobSaveData
		(
			pMob->getCreateIndex ( ),
			pMob->getHP	 ( ), 
			pMob->getATK ( ),
			pMob->getDEF ( ), 
			pMob->getAGL ( ), 
			pMob->getCRT ( ) 
		);
	}

	cMyData::getObj ( )->save ( );

	// mob dividend magnifications, win percent 
	m_pMobDividend = new cMobDividend ( m_aMobs );
	for ( KDuint i = 0; i < m_aMobs.size ( ); i++ )
	{
		m_aMobs [ i ]->setDividendMagnification ( m_pMobDividend->getMagnification ( i ) );
	}

	// btns
	m_pBtnExit		= Menu::createWithItem ( MenuItemImage::create ( "Bet/bet_exit.png"			, "Bet/bet_exit_push.png"		 , CC_CALLBACK_1 ( CasinoBetNode::exitCallback		, this ) ) );
	m_pBtnShop		= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_shop.png"			, "Bet/bet_shop_push.png"		 , CC_CALLBACK_1 ( CasinoBetNode::shopCallback		, this ) ) );
	m_pBtnGameStart	= Menu::createWithItem ( MenuItemImage::create ( "Bet/bet_btn_start.png"	, "Bet/bet_btn_start_push.png"	 , CC_CALLBACK_1 ( CasinoBetNode::gameStartCallback	, this ) ) );
	m_pBtnBack		= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_btn_back.png"		, "Bet/bet_btn_back_push.png"	 , CC_CALLBACK_1 ( CasinoBetNode::backCallback		, this ) ) );
	m_pBtn1Coin		= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_btn_1coin.png"	, "Bet/bet_btn_1coin_push.png"	 , CC_CALLBACK_1 ( CasinoBetNode::_1coinCallback	, this ) ) );
	m_pBtn10Coin	= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_btn_10coin.png"	, "Bet/bet_btn_10coin_push.png"	 , CC_CALLBACK_1 ( CasinoBetNode::_10coinCallback	, this ) ) );
	m_pBtn100Coin	= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_btn_100coin.png"	, "Bet/bet_btn_100coin_push.png" , CC_CALLBACK_1 ( CasinoBetNode::_100coinCallback	, this ) ) );
	m_pBtn1000Coin	= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_btn_1000coin.png"	, "Bet/bet_btn_1000coin_push.png", CC_CALLBACK_1 ( CasinoBetNode::_1000coinCallback	, this ) ) );
	m_pBtnMinCoin	= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_btn_mincoin.png"	, "Bet/bet_btn_mincoin_push.png" , CC_CALLBACK_1 ( CasinoBetNode::coinMinCallback	, this ) ) );
	m_pBtnMaxCoin	= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_btn_maxcoin.png"	, "Bet/bet_btn_maxcoin_push.png" , CC_CALLBACK_1 ( CasinoBetNode::coinMaxCallback	, this ) ) );
	m_pBtnShopExit	= Menu::createWithItem ( SoundMenuItem::create ( "Shop/shop_exit.png"		, "Shop/shop_exit_push.png"		 , CC_CALLBACK_1 ( CasinoBetNode::shopExitCallback	, this ) ) );

	m_pBtnExit		->setPosition ( Point (  33,  13.5f ) );
	m_pBtnShop		->setPosition ( Point (  98,  13.5f ) );
	m_pBtnGameStart	->setPosition ( Point ( 392,  28 ) );
	m_pBtnBack		->setPosition ( Point ( 191,  75 ) );
	m_pBtn1Coin		->setPosition ( Point ( 433, 225 ) );
	m_pBtn10Coin	->setPosition ( Point ( 349, 225 ) );
	m_pBtn100Coin	->setPosition ( Point ( 265, 225 ) );
	m_pBtn1000Coin	->setPosition ( Point ( 181, 225 ) );
	m_pBtnMinCoin	->setPosition ( Point ( 305,  76 ) );
	m_pBtnMaxCoin	->setPosition ( Point ( 420,  76 ) );
	m_pBtnShopExit	->setPosition ( Point (  62,  96 ) );

	m_pBtnExit		->setVisible ( true );
	m_pBtnShop		->setVisible ( true );
	m_pBtnGameStart	->setVisible ( false );
	m_pBtnBack		->setVisible ( false );
	m_pBtn1Coin		->setVisible ( false );
	m_pBtn10Coin	->setVisible ( false );
	m_pBtn100Coin	->setVisible ( false );
	m_pBtn1000Coin	->setVisible ( false );
	m_pBtnMinCoin	->setVisible ( false );
	m_pBtnMaxCoin	->setVisible ( false );
	m_pBtnShopExit	->setVisible ( false );

	m_pBtnExit		->setEnabled ( false );
	m_pBtnShop		->setEnabled ( false );
	m_pBtnGameStart	->setEnabled ( false );
	m_pBtnBack		->setEnabled ( false );
	m_pBtnShopExit	->setEnabled ( false );

	m_pBtn1Coin		->setEnabled ( false );
	m_pBtn10Coin	->setEnabled ( false );
	m_pBtn100Coin	->setEnabled ( false );
	m_pBtn1000Coin	->setEnabled ( false );
	m_pBtnMinCoin	->setEnabled ( false );
	m_pBtnMaxCoin	->setEnabled ( false );

	this->addChild ( m_pBtnExit		, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtnShop		, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtnGameStart, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtnBack		, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtn1Coin	, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtn10Coin	, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtn100Coin	, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtn1000Coin	, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtnMinCoin	, LAYER_BETVIEW_BTN );
	this->addChild ( m_pBtnMaxCoin	, LAYER_BETVIEW_BTN	);
	this->addChild ( m_pBtnShopExit , LAYER_BETVIEW_SHOP_BTN );

	// UI money
	m_pUIMoney = new cLabelAtlasCustom ( this, "Bet/coin_nums_s.png", 15, LAYER_BETVIEW_UI_5 );
	m_pUIMoney->addCharInfo ( ',',  7 );	m_pUIMoney->addCharInfo ( '0', 15 );	m_pUIMoney->addCharInfo ( '1', 15 );
	m_pUIMoney->addCharInfo ( '2', 15 );	m_pUIMoney->addCharInfo ( '3', 15 );	m_pUIMoney->addCharInfo ( '4', 15 );
	m_pUIMoney->addCharInfo ( '5', 15 );	m_pUIMoney->addCharInfo ( '6', 15 );	m_pUIMoney->addCharInfo ( '7', 15 );
	m_pUIMoney->addCharInfo ( '8', 15 );	m_pUIMoney->addCharInfo ( '9', 15 );

	// UI select Mob
	m_pUISelMob		= new cSprite ( this, "Bet/bet_sel_mob.png"		, LAYER_BETVIEW_MOB	, Point ( 0, 0 ), Point ( 33, 166 ) );
	m_pUISelMobText = new cSprite ( this, "Bet/bet_sel_mob_text.png", LAYER_BETVIEW_UI_5, Point ( 0, 0 ), Point ( 13, 219 ) );

	m_dUISelMobTextLeftTime = 0.5f;

	// UI mob num
	for ( KDint i = 0; i < 5; i++ )
	{
		cSprite*	pSprite = new cSprite ( this );

		switch ( i )
		{
			case 0	:	pSprite->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	pSprite->addFrame ( Rect (  0, 0, 21, 21 ) );	break;
			case 1	:	pSprite->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	pSprite->addFrame ( Rect ( 21, 0, 21, 21 ) );	break;
			case 2	:	pSprite->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	pSprite->addFrame ( Rect ( 42, 0, 21, 21 ) );	break;
			case 3	:	pSprite->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	pSprite->addFrame ( Rect ( 63, 0, 21, 21 ) );	break;
			case 4	:	pSprite->initTexture ( "Bet/bet_mob_num.png", LAYER_BETVIEW_UI_5, Point ( 2, 297 ) );	pSprite->addFrame ( Rect ( 84, 0, 21, 21 ) );	break;
		}

		pSprite->setFrame ( 0 );
		pSprite->get ( )->setVisible ( false );

		m_aUIMobNums.push_back ( pSprite );
	}

	// UI mob bar
	for ( KDint i = 0; i < 5; i++ )
	{
		cSprite*	pSprite = nullptr;

		switch ( i )
		{
			case 0	:	pSprite = new cSprite ( this, "Bet/bet_mob_bar.png", LAYER_BETVIEW_UI_4, Point ( 0, 0 ), Point ( 147, 235 ) );	break;
			case 1	:	pSprite = new cSprite ( this, "Bet/bet_mob_bar.png", LAYER_BETVIEW_UI_4, Point ( 0, 0 ), Point ( 147, 180 ) );	break;
			case 2	:	pSprite = new cSprite ( this, "Bet/bet_mob_bar.png", LAYER_BETVIEW_UI_4, Point ( 0, 0 ), Point ( 147, 125 ) );	break;
			case 3	:	pSprite = new cSprite ( this, "Bet/bet_mob_bar.png", LAYER_BETVIEW_UI_4, Point ( 0, 0 ), Point ( 147,  70 ) );	break;
			case 4	:	pSprite = new cSprite ( this, "Bet/bet_mob_bar.png", LAYER_BETVIEW_UI_4, Point ( 0, 0 ), Point ( 147,  15 ) );	break;
		}

		m_aUIMobBars.push_back ( pSprite );
	}

	// mob bar select
	m_pUIMobBarSelect2 = new cSprAnis ( this, "Bet/bet_mob_bar_select2.spr", LAYER_BETVIEW_UI_5, Point ( 0, 0 ) );
	m_pUIMobBarSelect3 = new cSprite  ( this, "Bet/bet_icon_text.png"	   , LAYER_BETVIEW_UI_5, Point ( 0, 0 ), Point ( 0, 0 ) );

	m_pUIMobBarSelect2->texLoad ( 0 );
	m_pUIMobBarSelect2->setVisible ( false );
	m_pUIMobBarSelect2->stop ( );
	m_pUIMobBarSelect3->get ( )->setVisible ( false );

	// UI mob bar num
	for ( KDint i = 0; i < 5; i++ )
	{
		cSprite*	pSprite = new cSprite ( this );

		switch ( i )
		{
			case 0	:	pSprite->initTexture ( "Bet/bet_bar_num.png", LAYER_BETVIEW_UI_5, Point ( 140, 252 ) );	pSprite->addFrame ( Rect (  0, 0, 33, 33 ) );	break;
			case 1	:	pSprite->initTexture ( "Bet/bet_bar_num.png", LAYER_BETVIEW_UI_5, Point ( 140, 197 ) );	pSprite->addFrame ( Rect ( 33, 0, 33, 33 ) );	break;
			case 2	:	pSprite->initTexture ( "Bet/bet_bar_num.png", LAYER_BETVIEW_UI_5, Point ( 140, 142 ) );	pSprite->addFrame ( Rect ( 66, 0, 33, 33 ) );	break;
			case 3	:	pSprite->initTexture ( "Bet/bet_bar_num.png", LAYER_BETVIEW_UI_5, Point ( 140,  87 ) );	pSprite->addFrame ( Rect ( 99, 0, 33, 33 ) );	break;
			case 4	:	pSprite->initTexture ( "Bet/bet_bar_num.png", LAYER_BETVIEW_UI_5, Point ( 140,  32 ) );	pSprite->addFrame ( Rect (132, 0, 33, 33 ) );	break;
		}

		pSprite->setFrame ( 0 );
		m_aUIMobBarNums.push_back ( pSprite );
	}

	// UI mob parameters
	for ( KDint i = 0; i < 5; i++ )
	{
		const KDchar*	szHP  = szHP  = ccszf ( "HP %02d"  , m_aMobs [ i ]->getHP  ( ) );
		const KDchar*	szATK = szATK = ccszf ( "ATK %02d" , m_aMobs [ i ]->getATK ( ) );
		const KDchar*	szDEF = szDEF = ccszf ( "DEF %02d" , m_aMobs [ i ]->getDEF ( ) );

		Rect			tRectHP	 = Rect ( 0, 0, 0, 0 );
		Rect			tRectATK = Rect ( 0, 0, 0, 0 );
		Rect			tRectDEF = Rect ( 0, 0, 0, 0 );

		switch ( i )
		{
			case 0	:	tRectHP = Rect ( 188, 240, 50, 20 );	tRectATK = Rect ( 237, 240, 50, 20 );	tRectDEF = Rect ( 297, 240, 50, 20 );	break;
			case 1	:	tRectHP = Rect ( 188, 185, 50, 20 );	tRectATK = Rect ( 237, 185, 50, 20 );	tRectDEF = Rect ( 297, 185, 50, 20 );	break;
			case 2	:	tRectHP = Rect ( 188, 130, 50, 20 );	tRectATK = Rect ( 237, 130, 50, 20 );	tRectDEF = Rect ( 297, 130, 50, 20 );	break;
			case 3	:	tRectHP = Rect ( 188,  75, 50, 20 );	tRectATK = Rect ( 237,  75, 50, 20 );	tRectDEF = Rect ( 297,  75, 50, 20 );	break;
			case 4	:	tRectHP = Rect ( 188,  20, 50, 20 );	tRectATK = Rect ( 237,  20, 50, 20 );	tRectDEF = Rect ( 297,  20, 50, 20 );	break;
		}

		m_aUIMobBarHPs .push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::RIGHT, tRectHP , szHP , LAYER_BETVIEW_UI_5 ) );
		m_aUIMobBarATKs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::RIGHT, tRectATK, szATK, LAYER_BETVIEW_UI_5 ) );
		m_aUIMobBarDEFs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT , tRectDEF, szDEF, LAYER_BETVIEW_UI_5 ) );
	}

	// UI mob dividend magnifications
	for ( KDint i = 0; i < 5; i++ )
	{
		const KDchar*	szStr1	= ccszf ( "%d", (KDint) m_aMobs [ i ]->getDividendMagnification ( ) );
		const KDchar*	szStr2	= ccszf ( "%d", (KDint) ( ( m_aMobs [ i ]->getDividendMagnification ( ) - (KDint) m_aMobs [ i ]->getDividendMagnification ( ) ) * 10 ) );
		Point			tPoint	= Point ( 0, 0 );

		switch(i)
		{
			case 0	:	tPoint = Point ( 348, 255 );	break;
			case 1	:	tPoint = Point ( 348, 200 );	break;
			case 2	:	tPoint = Point ( 348, 145 );	break;
			case 3	:	tPoint = Point ( 348,  90 );	break;
			case 4	:	tPoint = Point ( 348,  35 );	break;
		}

		cLabelAtlas*	pLabel1 = new cLabelAtlas ( this, szStr1, "Bet/bet_multiple_num.png" , 18, 19, '0', LAYER_BETVIEW_UI_5, tPoint );
		cLabelAtlas*	pLabel2 = new cLabelAtlas ( this, szStr2, "Bet/bet_multiple_num2.png", 12, 13, '0', LAYER_BETVIEW_UI_5, Point ( tPoint.x + pLabel1->getSize ( ).width + 5, tPoint.y ) );
		cSprite*		pLabel3 = new cSprite	  ( this, "Bet/bet_multiple_num3.png", LAYER_BETVIEW_UI_5, Point ( 0, 0 ), Point ( tPoint.x + pLabel1->getSize ( ).width, tPoint.y ) );

		m_aUIMobBarMagnifications .push_back ( pLabel1 );
		m_aUIMobBarMagnifications2.push_back ( pLabel2 );
		m_aUIMobBarMagnifications3.push_back ( pLabel3 );
	}

	// mob bar select
	m_pUIMobBarSelect = new cSprAnis ( this, "Bet/bet_mob_bar_select.spr", LAYER_BETVIEW_UI_5, Point ( 0, 0 ) );
	m_pUIMobBarSelect->texLoad ( 0 );
	m_pUIMobBarSelect->setVisible ( false );
	m_pUIMobBarSelect->stop ( );

	// UI skill icon
	for ( KDint i = 0; i < 3; i++ )
	{
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

		pAniSkillIcon->setVisible ( false );
		pAniSkillIcon->setLoop ( false );
		pAniSkillIcon->stop ( );

		m_aUISkillIcons.push_back ( pAniSkillIcon );

		switch ( i )
		{
			case 0	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 79, 82, 100, 30 ), "", LAYER_BETVIEW_UI_5 ) );	break;
			case 1	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 79, 60, 100, 30 ), "", LAYER_BETVIEW_UI_5 ) );	break;
			case 2	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 79, 38, 100, 30 ), "", LAYER_BETVIEW_UI_5 ) );	break;
		}
	}

	// UI mob Detail parameter
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect (  73, 137, 80, 12 ), "HP : " , LAYER_BETVIEW_UI_5 ) );	// nHp
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect (  56, 126, 80, 12 ), "ATK : ", LAYER_BETVIEW_UI_5 ) );	// nAtk
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 108, 126, 80, 12 ), "DEF : ", LAYER_BETVIEW_UI_5 ) );	// nDef
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect (  56, 115, 80, 12 ), "AGL : ", LAYER_BETVIEW_UI_5 ) );	// nAgl
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 108, 115, 80, 12 ), "CRT : ", LAYER_BETVIEW_UI_5 ) );	// nCrt

	// UI coin state
	m_pUICoinBack  = new cSprite ( this, "Bet/bet_coin_bg.png"	 , LAYER_BETVIEW_UI_1, Point ( 0, 0 ), Point ( 137, 91 ) );
	m_pUICoinCover = new cSprite ( this, "Bet/bet_coin_cover.png", LAYER_BETVIEW_UI_3, Point ( 0, 0 ), Point ( 131,  0 ) );

	m_pUICoinBack ->get ( )->setVisible ( false );
	m_pUICoinCover->get ( )->setVisible ( false );

	// UI bet coin scroller
	cNumScroll*		pNum1 = new cNumScroll ( this, "Bet/bet_coin_num.png", LAYER_BETVIEW_UI_2, Point ( 143, 130 ), Size ( 76, 44 ), Rect ( 143, 103, 76, 100 ), Rect ( 143, 93, 76, 116 ) );
	cNumScroll*		pNum2 = new cNumScroll ( this, "Bet/bet_coin_num.png", LAYER_BETVIEW_UI_2, Point ( 227, 130 ), Size ( 76, 44 ), Rect ( 227, 103, 76, 100 ), Rect ( 227, 93, 76, 116 ) );
	cNumScroll*		pNum3 = new cNumScroll ( this, "Bet/bet_coin_num.png", LAYER_BETVIEW_UI_2, Point ( 311, 130 ), Size ( 76, 44 ), Rect ( 311, 103, 76, 100 ), Rect ( 311, 93, 76, 116 ) );
	cNumScroll*		pNum4 = new cNumScroll ( this, "Bet/bet_coin_num.png", LAYER_BETVIEW_UI_2, Point ( 395, 130 ), Size ( 76, 44 ), Rect ( 395, 103, 76, 100 ), Rect ( 395, 93, 76, 116 ) );

	
	KDint64			nBetMoney = cMyData::getObj ( )->m_nMoney / 10LL;
	
	if ( nBetMoney <= 0    )	nBetMoney = 1;
	if ( nBetMoney >  9999 )	nBetMoney = 9999;

	pNum1->setNum ( ( nBetMoney % 10000LL ) / 1000 );
	pNum2->setNum ( ( nBetMoney %  1000LL ) /  100 );
	pNum3->setNum ( ( nBetMoney %   100LL ) /   10 );
	pNum4->setNum ( ( nBetMoney % 	 10LL )		   );
	pNum1->setVisible ( false );
	pNum2->setVisible ( false );
	pNum3->setVisible ( false );
	pNum4->setVisible ( false );
	
	m_aUIBetCoins.push_back ( pNum1 );
	m_aUIBetCoins.push_back ( pNum2 );
	m_aUIBetCoins.push_back ( pNum3 );
	m_aUIBetCoins.push_back ( pNum4 );

	// time get money
	cTimeGetMoney::getObj (  )->changeNode ( this, LAYER_BETVIEW_UI_5, Rect ( 150, 300, 50, 15 ) );
		
	// update money
	this->updateMoney ( );

	// betting money
	cMyData::getObj ( )->m_nBettingMoney = 0;
		
	// not coin
	m_pUINotCoinPopup = new cSprite  ( this, "Bet/bet_notcoin_popup.png", LAYER_BETVIEW_POPUP, Point ( 0,   0 ), Point ( 131, 242 ) );
	m_pUIShopCursor	  = new cSprAnis ( this, "Bet/bet_shop_cursor.spr"	, LAYER_BETVIEW_UI_7 , Point ( 80, 50 ) );

	m_pUINotCoinPopup->get ( )->setVisible ( false );
	m_pUIShopCursor->texLoad ( 0 );
	m_pUIShopCursor->setVisible ( false );
	m_pUIShopCursor->stop ( );

	m_bUIIsNotCoinView = false;

	//
	m_nSelectMob = -1;
	
	//
	for ( KDint i = 0; i < 5; i++ )
	{
		switch ( i )
		{
			case 0	:	m_aBarSelectRects.push_back ( Rect ( 147, 235, 328, 44 ) );		break;
			case 1	:	m_aBarSelectRects.push_back ( Rect ( 147, 180, 328, 44 ) );		break;
			case 2	:	m_aBarSelectRects.push_back ( Rect ( 147, 125, 328, 44 ) );		break;
			case 3	:	m_aBarSelectRects.push_back ( Rect ( 147,  70, 328, 44 ) );		break;
			case 4	:	m_aBarSelectRects.push_back ( Rect ( 147,  15, 328, 44 ) );		break;
		}
	}

	// successive
	cMyData::getObj ( )->m_nSuccessive = 0;

	// shop node
	m_pShopNode = ShopNode::createWithBetNode ( this );
	this->addChild ( m_pShopNode, LAYER_BETVIEW_SHOP );
	m_pShopNode->setPosition ( Point ( 0, _SCR_H ) );
	m_pShopNode->setVisible	( false );
	
	// node change
	m_bIsNodeChangeIn	= true;
	m_bIsNodeChangeOut	= false;
	m_bIsGoTitle		= false;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg	= new cNodeChangeImgs ( this, false );

	// schedule
	this->schedule ( schedule_selector ( CasinoBetNode::step ) );
	
	return true;
}

KDbool CasinoBetNode::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	Point	tLocation = this->convertTouchToNodeSpace ( pTouch );

	if ( m_bIsNodeChangeOut == false && m_pShopNode->getIsShop ( ) == false )
	{
		if ( m_eState == eBatStateType_AllView )
		{
			for ( KDuint i = 0; i != m_aBarSelectRects.size ( ); ++i )
			{
				Rect&	rRect = m_aBarSelectRects [ i ];

				if ( RectInPoint ( rRect, tLocation ) )
				{
					this->selectMob ( i );
					return true;
				}
			}
		}
		else if ( m_eState == eBatStateType_BetView )
		{
			for ( auto pNode : m_aUIBetCoins )
			{
				if ( pNode->touchBegan ( pTouch->getID ( ), tLocation ) == true )
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

KDvoid CasinoBetNode::onTouchMoved ( Touch* pTouch, Event* pEvent )
{
	Point	tLocation = this->convertTouchToNodeSpace ( pTouch );

	if ( m_bIsNodeChangeOut == false )
	{
		if ( m_eState == eBatStateType_BetView )
		{
			for ( auto pNode : m_aUIBetCoins )
			{
				pNode->touchMoved ( pTouch->getID ( ), tLocation );
			}
		}
	}
}

KDvoid CasinoBetNode::onTouchEnded ( Touch* pTouch, Event* pEvent )
{
	Point	tLocation = this->convertTouchToNodeSpace ( pTouch );

	if ( m_bIsNodeChangeOut == false )
	{
		if ( m_eState == eBatStateType_BetView )
		{
			for ( auto pNode : m_aUIBetCoins )
			{
				pNode->touchEnded ( pTouch->getID ( ), tLocation );
			}
		}
	}
}

KDvoid CasinoBetNode::step ( KDfloat fDelta )
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

	// shop
	if ( m_pShopNode->getIsShop ( ) == true )
	{
		Point	tPoint = m_pShopNode->getPosition ( );

		if ( tPoint.y > 0 )
		{
			tPoint.y -= dLeftTickCount * 1500;
			if ( tPoint.y < 0 )
			{
				tPoint.y = 0;
			}

			m_pShopNode->setPosition ( tPoint );
			m_pBtnShopExit->setPosition ( Point ( 62, tPoint.y + 96 ) );
		}
	}
	else
	{
		Point	tPoint = m_pShopNode->getPosition ( );

		if ( tPoint.y < _SCR_H )
		{
			tPoint.y += dLeftTickCount * 1500;
			if ( tPoint.y > _SCR_H )
			{
				tPoint.y = _SCR_H;
			}

			m_pShopNode->setPosition ( tPoint );
			m_pBtnShopExit->setPosition ( Point ( 62, tPoint.y + 96 ) );
		}
	}

	// update mob
	for ( auto pNode : m_aMobs )
	{
		pNode->update ( dLeftTickCount );
	}

	// UI mob bar select
	m_pUIMobBarSelect ->update ( dLeftTickCount );
	m_pUIMobBarSelect2->update ( dLeftTickCount );

	// update all view to detail view changing
	if ( m_eState == eBatStateType_AllToDetailChanging )
	{
		this->updateAllToBetChage ( dLeftTickCount );
	}
	else if ( m_eState == eBatStateType_BetView )
	{
		// update money
		this->updateMoney ( );

		for ( auto pNode : m_aUIBetCoins )
		{
			pNode->update ( dLeftTickCount );
		}
	}
	else if ( m_eState == eBatStateType_DetailToAllChanging )
	{
		this->updateBetToAllChage ( dLeftTickCount );
	}

	// sel mob
	if ( m_nSelectMob == -1 )
	{
		m_dUISelMobTextLeftTime -= dLeftTickCount;

		if ( m_dUISelMobTextLeftTime < 0 )
		{
			m_dUISelMobTextLeftTime += 0.5f;

			if ( m_pUISelMobText->get ( )->isVisible ( ) == true )	m_pUISelMobText->get ( )->setVisible ( false );
			else													m_pUISelMobText->get ( )->setVisible ( true  );
		}
	}

	// not coin
	if ( m_bUIIsNotCoinView == true )
	{
		m_pUINotCoinPopup->update ( dLeftTickCount );
		m_pUIShopCursor	 ->update ( dLeftTickCount );
	}

	// time get money
	cTimeGetMoney::getObj ( )->update ( dLeftTickCount, cMyData::getObj ( )->m_nMoney );

	// update money
	this->updateMoney ( );
}

KDbool CasinoBetNode::updateNodeChage ( KDdouble dLeftTime )
{
	if ( m_pNodeChangeImg != nullptr )
	{
		if ( m_pNodeChangeImg->update ( dLeftTime ) == true )
		{
			if ( m_bIsNodeChangeIn == true )
			{
				// play background music
				if ( cMyData::getObj ( )->m_bIsOptionSound != 0 )
				{
					SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "myroom.mp3", true );
				}

				m_bIsNodeChangeIn = false;
				CC_SAFE_RELEASE ( m_pNodeChangeImg );		

				m_pBtnExit->setEnabled ( true );
				m_pBtnShop->setEnabled ( true );
			}
			else if ( m_bIsNodeChangeOut == true )
			{
				CC_SAFE_RELEASE ( m_pNodeChangeImg );		

				if ( m_bIsGoTitle == false )
				{
					// bet money
					KDint64		nBetMoney = 0;
					nBetMoney += m_aUIBetCoins [ 0 ]->getNum ( ) * 1000;
					nBetMoney += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
					nBetMoney += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
					nBetMoney += m_aUIBetCoins [ 3 ]->getNum ( );

					if ( nBetMoney > cMyData::getObj ( )->m_nMoney )
					{
						nBetMoney = cMyData::getObj ( )->m_nMoney;
					}

					m_aMobs [ m_nSelectMob ]->setBetMoney ( nBetMoney );

					// global money
					cMyData::getObj ( )->m_nMoney -= nBetMoney;
					cMyData::getObj ( )->m_nBettingMoney = nBetMoney;

					// clear bet mob
					cMyData::getObj ( )->clearBetMobData ( );

					// save
					cMyData::getObj ( )->save ( );

					// unschedule
					this->unschedule ( schedule_selector ( CasinoBetNode::step ) );

					for ( auto pMob : m_aMobs )
					{
						pMob->texRelease ( eUnitAniType_Name_a );
						pMob->texRelease ( eUnitAniType_Name_b );
						if ( m_nSelectMob != pMob->getSeatIndex ( ) )
						{
							pMob->texRelease ( eUnitAniType_Portrait );
						}

						pMob->texLoad ( eUnitAniType_Damage );
					}

					// cenece change
					Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithMobs ( m_aMobs, eModeType_Casino ) ) );
				}
				else
				{
					// unschedule
					this->unschedule ( schedule_selector ( CasinoBetNode::step ) );

					// cenece change
					Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, MainNode::create ( true ) ) );
				}

				this->releaseObject ( );

				return true;
			}
		}
	}

	return false;
}

KDvoid CasinoBetNode::updateMoney ( KDvoid )
{
	// money
	std::string		sStr = ccszf ( "%lld", cMyData::getObj ( )->m_nMoney );

	for ( KDint i = sStr.size ( ) - 3; i > 0; i -= 3 )
	{
		sStr.insert ( i, "," );		
	}

	m_pUIMoney->setString ( sStr.c_str ( ) );
	m_pUIMoney->setPosition ( Point ( 459 - m_pUIMoney->getSize ( ).width, 300 ) );

	KDint64			nMoney = cMyData::getObj ( )->m_nMoney;

	// get money
	cTimeGetMoney::getObj ( )->setPoint ( Point ( 432 - m_pUIMoney->getSize ( ).width, 302 ) );

	// scroll money
	if ( nMoney >= 1000 )
	{
		KDint		nCheckMoney = 0;
		nCheckMoney += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
		nCheckMoney += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
		nCheckMoney += m_aUIBetCoins [ 3 ]->getNum ( );

		if ( nCheckMoney > nMoney % 1000 )	m_aUIBetCoins [ 0 ]->setMax ( ( nMoney / 1000 ) - 1	);
		else								m_aUIBetCoins [ 0 ]->setMax ( nMoney / 1000 );

		m_aUIBetCoins [ 1 ]->setMax ( -1 );
		m_aUIBetCoins [ 2 ]->setMax ( -1 );
		m_aUIBetCoins [ 3 ]->setMax ( -1 );
	}
	else if ( nMoney >= 100 )
	{
		KDint		nCheckMoney = 0;
		nCheckMoney += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
		nCheckMoney += m_aUIBetCoins [ 3 ]->getNum ( );

		m_aUIBetCoins [ 0 ]->setMax ( 0 );

		if ( nCheckMoney > nMoney % 100 )	m_aUIBetCoins [ 1 ]->setMax ( ( nMoney / 100 ) - 1 );
		else								m_aUIBetCoins [ 1 ]->setMax ( nMoney / 100 );

		m_aUIBetCoins [ 2 ]->setMax ( -1 );
		m_aUIBetCoins [ 3 ]->setMax ( -1 );
	}
	else if ( nMoney >= 10 )
	{
		KDint		nCheckMoney = 0;
		nCheckMoney += m_aUIBetCoins [ 3 ]->getNum ( );

		m_aUIBetCoins [ 0 ]->setMax ( 0 );
		m_aUIBetCoins [ 1 ]->setMax ( 0 );

		if ( nCheckMoney > nMoney % 10 )	m_aUIBetCoins [ 2 ]->setMax ( ( nMoney / 10 ) - 1 );
		else								m_aUIBetCoins [ 2 ]->setMax ( nMoney / 10 );

		m_aUIBetCoins [ 3 ]->setMax ( -1 );
	}
	else
	{
		m_aUIBetCoins [ 0 ]->setMax ( 0	);
		m_aUIBetCoins [ 1 ]->setMax ( 0 );
		m_aUIBetCoins [ 2 ]->setMax ( 0 );
		m_aUIBetCoins [ 3 ]->setMax ( nMoney );
	}
}

KDvoid CasinoBetNode::exitCallback ( Object* pSender )
{
	m_bIsNodeChangeOut	= true;
	m_bIsGoTitle		= true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg	= new cNodeChangeImgs ( this, true );

	m_pBtnExit		->setEnabled ( false );
	m_pBtnShop		->setEnabled ( false );
	m_pBtnGameStart	->setEnabled ( false );
	m_pBtnBack		->setEnabled ( false );
	m_pBtn1Coin		->setEnabled ( false );
	m_pBtn10Coin	->setEnabled ( false );
	m_pBtn100Coin	->setEnabled ( false );
	m_pBtn1000Coin	->setEnabled ( false );
	m_pBtnMinCoin	->setEnabled ( false );
	m_pBtnMaxCoin	->setEnabled ( false );
	m_pBtnShopExit	->setEnabled ( false );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid CasinoBetNode::shopCallback ( Object* pSender )
{
	m_pBtnExit		->setEnabled ( false );
	m_pBtnShop		->setEnabled ( false );
	m_pBtnGameStart	->setEnabled ( false );
	m_pBtnBack		->setEnabled ( false );
	m_pBtn1Coin		->setEnabled ( false );
	m_pBtn10Coin	->setEnabled ( false );
	m_pBtn100Coin	->setEnabled ( false );
	m_pBtn1000Coin	->setEnabled ( false );
	m_pBtnMinCoin	->setEnabled ( false );
	m_pBtnMaxCoin	->setEnabled ( false );

	m_pBtnShopExit->setVisible ( true );	
	m_pBtnShopExit->setEnabled ( true );

	m_pBtnShopExit->setPosition ( Point ( 62, _SCR_H + 96 ) );

	m_pCoverColor->setVisible ( true );	

	m_pShopNode->setVisible ( true );	
	m_pShopNode->setPosition ( Point ( 0, _SCR_H ) );

	m_bUIIsNotCoinView = false;
	m_pUINotCoinPopup->get ( )->setVisible ( false );
	m_pUIShopCursor->setVisible ( false );
	m_pUIShopCursor->stop ( );

	m_pShopNode->open ( eShopType_Normal, eDollarMachineType_Not );
}

KDvoid CasinoBetNode::gameStartCallback	( Object* pSender )
{
	KDint64		nBetCoin = 0;

	nBetCoin += m_aUIBetCoins [ 0 ]->getNum ( ) * 1000;
	nBetCoin += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
	nBetCoin += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
	nBetCoin += m_aUIBetCoins [ 3 ]->getNum ( );

	if ( nBetCoin == 0 )
	{
		if ( cMyData::getObj ( )->m_bIsOptionSound != 0 )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "button_select.mp3" );
		}

		m_aUIBetCoins [ 0 ]->setEffect ( );
		m_aUIBetCoins [ 1 ]->setEffect ( );
		m_aUIBetCoins [ 2 ]->setEffect ( );
		m_aUIBetCoins [ 3 ]->setEffect ( );
		return;
	}

	m_bIsNodeChangeOut = true;
	m_bIsGoTitle = false;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg = new cNodeChangeImgs ( this, true );

	m_pBtnExit		->setEnabled ( false );
	m_pBtnShop		->setEnabled ( false );
	m_pBtnGameStart	->setEnabled ( false );
	m_pBtnBack		->setEnabled ( false );
	m_pBtn1Coin		->setEnabled ( false );
	m_pBtn10Coin	->setEnabled ( false );
	m_pBtn100Coin	->setEnabled ( false );
	m_pBtn1000Coin	->setEnabled ( false );
	m_pBtnMinCoin	->setEnabled ( false );
	m_pBtnMaxCoin	->setEnabled ( false );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid CasinoBetNode::backCallback ( Object* pSender )
{
	// sound
	if ( cMyData::getObj ( )->m_bIsOptionSound != false )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "bet_move.mp3" );
	}

	// state
	m_eState = eBatStateType_DetailToAllChanging;

	// btns
	m_pBtnGameStart	->setVisible ( false );
	m_pBtnBack		->setVisible ( false );
	m_pBtn1Coin		->setVisible ( false );
	m_pBtn10Coin	->setVisible ( false );
	m_pBtn100Coin	->setVisible ( false );
	m_pBtn1000Coin	->setVisible ( false );
	m_pBtnMinCoin	->setVisible ( false );
	m_pBtnMaxCoin	->setVisible ( false );
	m_pBtnGameStart	->setEnabled ( false );
	m_pBtnBack		->setEnabled ( false );
	m_pBtn1Coin		->setEnabled ( false );
	m_pBtn10Coin	->setEnabled ( false );
	m_pBtn100Coin	->setEnabled ( false );
	m_pBtn1000Coin	->setEnabled ( false );
	m_pBtnMinCoin	->setEnabled ( false );
	m_pBtnMaxCoin	->setEnabled ( false );

	// bet view UIs
	m_pUICoinBack ->get ( )->setVisible ( false );
	m_pUICoinCover->get ( )->setVisible ( false );

	for ( auto pNode : m_aUIBetCoins )
	{
		pNode->setVisible ( false );
	}

	// time
	KDdouble	dMaxTime	= 0;
	switch ( m_nSelectMob )
	{
		case 0	:	dMaxTime = _CHANGE_STATE_TIME_1;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_1;	break;
		case 1	:	dMaxTime = _CHANGE_STATE_TIME_2;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_2;	break;
		case 2	:	dMaxTime = _CHANGE_STATE_TIME_3;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_3;	break;
		case 3	:	dMaxTime = _CHANGE_STATE_TIME_4;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_4;	break;
		case 4	:	dMaxTime = _CHANGE_STATE_TIME_5;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_5;	break;
	}

	// UIs potion
	switch ( m_nSelectMob )
	{
		case 0 :	
			m_aMobs			[ m_nSelectMob ]->setPonitTimeNameB		  ( Point ( 173, 278 ), dMaxTime );		// bar name_b and portrait
			m_aMobs			[ m_nSelectMob ]->setPointTimePortrait	  ( Point ( 410, 277 ), dMaxTime );
			m_aUIMobBars	[ m_nSelectMob ]->setPointTime			  ( Point ( 147, 235 ), dMaxTime );		// bar
			m_aUIMobBarNums	[ m_nSelectMob ]->setPointTime			  ( Point ( 140, 252 ), dMaxTime );		// bar num
			m_pLabels->setPointTime ( m_aUIMobBarHPs  [ m_nSelectMob ], Point ( 188, 240 ), dMaxTime );		// bar para nHp
			m_pLabels->setPointTime ( m_aUIMobBarATKs [ m_nSelectMob ], Point ( 237, 240 ), dMaxTime );		// bar para nAtk
			m_pLabels->setPointTime ( m_aUIMobBarDEFs [ m_nSelectMob ], Point ( 297, 240 ), dMaxTime );		// bar para nDef
			m_aUIMobBarMagnifications  [ m_nSelectMob ]->setPointTime ( Point ( 348																       , 255 ), dMaxTime );		// UI pMob dividend magnifications
			m_aUIMobBarMagnifications2 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize ( ).width + 5, 255 ), dMaxTime );
			m_aUIMobBarMagnifications3 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize ( ).width	   , 255 ), dMaxTime );
			break;

		case 1 :	
			m_aMobs			[ m_nSelectMob ]->setPonitTimeNameB		  ( Point ( 173, 223 ), dMaxTime );		// bar name_b and portrait
			m_aMobs			[ m_nSelectMob ]->setPointTimePortrait	  ( Point ( 410, 222 ), dMaxTime );
			m_aUIMobBars	[ m_nSelectMob ]->setPointTime			  ( Point ( 147, 180 ), dMaxTime );		// bar
			m_aUIMobBarNums	[ m_nSelectMob ]->setPointTime			  ( Point ( 140, 197 ), dMaxTime );		// bar num
			m_pLabels->setPointTime ( m_aUIMobBarHPs  [ m_nSelectMob ], Point ( 188, 185 ), dMaxTime );		// bar para nHp
			m_pLabels->setPointTime ( m_aUIMobBarATKs [ m_nSelectMob ], Point ( 237, 185 ), dMaxTime );		// bar para nAtk
			m_pLabels->setPointTime ( m_aUIMobBarDEFs [ m_nSelectMob ], Point ( 297, 185 ), dMaxTime );		// bar para nDef
			m_aUIMobBarMagnifications  [ m_nSelectMob ]->setPointTime ( Point ( 348																	  , 200 ), dMaxTime );		// UI pMob dividend magnifications
			m_aUIMobBarMagnifications2 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize( ).width + 5, 200 ), dMaxTime );
			m_aUIMobBarMagnifications3 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize( ).width	  , 200 ), dMaxTime );
			break;

		case 2 :	
			m_aMobs			[ m_nSelectMob ]->setPonitTimeNameB		  ( Point ( 173, 168 ), dMaxTime );		// bar name_b and portrait
			m_aMobs			[ m_nSelectMob ]->setPointTimePortrait	  ( Point ( 410, 167 ), dMaxTime );
			m_aUIMobBars	[ m_nSelectMob ]->setPointTime			  ( Point ( 147, 125 ), dMaxTime );		// bar
			m_aUIMobBarNums	[ m_nSelectMob ]->setPointTime			  ( Point ( 140, 142 ), dMaxTime );		// bar num
			m_pLabels->setPointTime ( m_aUIMobBarHPs  [ m_nSelectMob ], Point ( 188, 130 ), dMaxTime );		// bar para nHp
			m_pLabels->setPointTime ( m_aUIMobBarATKs [ m_nSelectMob ], Point ( 237, 130 ), dMaxTime );		// bar para nAtk
			m_pLabels->setPointTime ( m_aUIMobBarDEFs [ m_nSelectMob ], Point ( 297, 130 ), dMaxTime );		// bar para nDef
			m_aUIMobBarMagnifications  [ m_nSelectMob ]->setPointTime ( Point ( 348																	  , 145 ), dMaxTime );		// UI pMob dividend magnifications
			m_aUIMobBarMagnifications2 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize( ).width + 5, 145 ), dMaxTime );
			m_aUIMobBarMagnifications3 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize( ).width    , 145 ), dMaxTime );
			break;

		case 3 :	
			m_aMobs			[ m_nSelectMob ]->setPonitTimeNameB		  ( Point ( 173, 113 ), dMaxTime );		// bar name_b and portrait
			m_aMobs			[ m_nSelectMob ]->setPointTimePortrait	  ( Point ( 410, 112 ), dMaxTime );
			m_aUIMobBars	[ m_nSelectMob ]->setPointTime			  ( Point ( 147,  70 ), dMaxTime );		// bar
			m_aUIMobBarNums	[ m_nSelectMob ]->setPointTime			  ( Point ( 140,  87 ), dMaxTime );		// bar num
			m_pLabels->setPointTime ( m_aUIMobBarHPs  [ m_nSelectMob ], Point ( 188,  75 ), dMaxTime );		// bar para nHp
			m_pLabels->setPointTime ( m_aUIMobBarATKs [ m_nSelectMob ], Point ( 237,  75 ), dMaxTime );		// bar para nAtk
			m_pLabels->setPointTime ( m_aUIMobBarDEFs [ m_nSelectMob ], Point ( 297,  75 ), dMaxTime );		// bar para nDef
			m_aUIMobBarMagnifications  [ m_nSelectMob ]->setPointTime ( Point ( 348																	  , 90 ), dMaxTime );		// UI pMob dividend magnifications
			m_aUIMobBarMagnifications2 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize( ).width + 5, 90 ), dMaxTime );
			m_aUIMobBarMagnifications3 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize( ).width	  , 90 ), dMaxTime );
			break;

		case 4 :	
			m_aMobs			[ m_nSelectMob ]->setPonitTimeNameB		  ( Point ( 173, 58 ), dMaxTime );		// bar name_b and portrait
			m_aMobs			[ m_nSelectMob ]->setPointTimePortrait	  ( Point ( 410, 57 ), dMaxTime );
			m_aUIMobBars	[ m_nSelectMob ]->setPointTime			  ( Point ( 147, 15 ), dMaxTime );		// bar
			m_aUIMobBarNums	[ m_nSelectMob ]->setPointTime			  ( Point ( 140, 32 ), dMaxTime );		// bar num
			m_pLabels->setPointTime ( m_aUIMobBarHPs  [ m_nSelectMob ], Point ( 188, 20 ), dMaxTime );		// bar para nHp
			m_pLabels->setPointTime ( m_aUIMobBarATKs [ m_nSelectMob ], Point ( 237, 20 ), dMaxTime );		// bar para nAtk
			m_pLabels->setPointTime ( m_aUIMobBarDEFs [ m_nSelectMob ], Point ( 297, 20 ), dMaxTime );		// bar para nDef
			m_aUIMobBarMagnifications  [ m_nSelectMob ]->setPointTime ( Point ( 348																	  , 35 ), dMaxTime );		// UI pMob dividend magnifications
			m_aUIMobBarMagnifications2 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize( ).width + 5, 35 ), dMaxTime );
			m_aUIMobBarMagnifications3 [ m_nSelectMob ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ m_nSelectMob ]->getSize( ).width	  , 35 ), dMaxTime );
			break;
	}

	// UI not coin
	m_bUIIsNotCoinView = false;

	m_pUINotCoinPopup->get ( )->setVisible ( false );
	m_pUIShopCursor->setVisible ( false );
	m_pUIShopCursor->stop ( );
}

KDvoid CasinoBetNode::_1coinCallback ( Object* pSender )
{
	KDint64		nMyBetCoin = cMyData::getObj ( )->m_nMoney;
	KDint64		nBetCoin   = 0;
	nBetCoin += m_aUIBetCoins [ 0 ]->getNum ( ) * 1000;
	nBetCoin += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
	nBetCoin += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
	nBetCoin += m_aUIBetCoins [ 3 ]->getNum ( );

	// 효과음 출력.
	if ( nBetCoin == nMyBetCoin )
	{

	}

	nBetCoin += 1;
	if ( nBetCoin > nMyBetCoin ) nBetCoin = nMyBetCoin;
	if ( nBetCoin >	      9999 ) nBetCoin = 9999;

	KDbool		bIsChange1000	= false;
	KDbool		bIsChange100	= false;
	KDbool		bIsChange10		= false;
	KDbool		bIsChange1		= false;

	if ( cMyData::getObj ( )->m_nMoney == 0 )
	{
		m_aUIBetCoins [ 0 ]->setEffect ( );
		m_aUIBetCoins [ 1 ]->setEffect ( );
		m_aUIBetCoins [ 2 ]->setEffect ( );
		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
	else
	{
		if (   nBetCoin			 / 1000 != m_aUIBetCoins [ 0 ]->getNum ( ) )	bIsChange1000	= true;
		if ( ( nBetCoin % 1000 ) / 100  != m_aUIBetCoins [ 1 ]->getNum ( ) )	bIsChange100	= true;
		if ( ( nBetCoin % 100  ) / 10   != m_aUIBetCoins [ 2 ]->getNum ( ) )	bIsChange10		= true;
		if (   nBetCoin % 10			!= m_aUIBetCoins [ 3 ]->getNum ( ) )	bIsChange1		= true;
	}

	m_aUIBetCoins [ 0 ]->setNum (   nBetCoin		  / 1000 );
	m_aUIBetCoins [ 1 ]->setNum ( ( nBetCoin % 1000 ) / 100  );
	m_aUIBetCoins [ 2 ]->setNum ( ( nBetCoin % 100  ) / 10   );
	m_aUIBetCoins [ 3 ]->setNum (   nBetCoin % 10			 );

	if ( bIsChange1000	)	m_aUIBetCoins [ 0 ]->setEffect ( );
	if ( bIsChange100	)	m_aUIBetCoins [ 1 ]->setEffect ( );
	if ( bIsChange10	)	m_aUIBetCoins [ 2 ]->setEffect ( );
	if ( bIsChange1		)	m_aUIBetCoins [ 3 ]->setEffect ( );

	if ( bIsChange1000 == false && bIsChange100 == false && bIsChange10 == false && bIsChange1 == false )
	{
			 if ( m_aUIBetCoins [ 0 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 0 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 1 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 1 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 2 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 2 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 3 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 3 ]->setEffect ( );
	}
}

KDvoid CasinoBetNode::_10coinCallback ( Object* pSender )
{
	KDint64		nMyBetCoin = cMyData::getObj ( )->m_nMoney;
	KDint64		nBetCoin   = 0;
	nBetCoin += m_aUIBetCoins [ 0 ]->getNum ( ) * 1000;
	nBetCoin += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
	nBetCoin += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
	nBetCoin += m_aUIBetCoins [ 3 ]->getNum ( );
	
	// 효과음 출력.
	if ( nBetCoin == nMyBetCoin )
	{
	}
	
	nBetCoin += 10;
	if ( nBetCoin > nMyBetCoin )	nBetCoin = nMyBetCoin;
	if ( nBetCoin >	      9999 )	nBetCoin = 9999;

	KDbool		bIsChange1000	= false;
	KDbool		bIsChange100	= false;
	KDbool		bIsChange10		= false;
	KDbool		bIsChange1		= false;

	if ( cMyData::getObj ( )->m_nMoney == 0 )
	{
		m_aUIBetCoins [ 0 ]->setEffect ( );
		m_aUIBetCoins [ 1 ]->setEffect ( );
		m_aUIBetCoins [ 2 ]->setEffect ( );
		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
	else
	{
		if (   nBetCoin			 / 1000 != m_aUIBetCoins [ 0 ]->getNum ( ) )	bIsChange1000	= true;
		if ( ( nBetCoin % 1000 ) / 100  != m_aUIBetCoins [ 1 ]->getNum ( ) )	bIsChange100	= true;
		if ( ( nBetCoin % 100  ) / 10   != m_aUIBetCoins [ 2 ]->getNum ( ) )	bIsChange10		= true;
		if (   nBetCoin % 10			!= m_aUIBetCoins [ 3 ]->getNum ( ) )	bIsChange1		= true;
	}			

	m_aUIBetCoins [ 0 ]->setNum (   nBetCoin		  / 1000 );
	m_aUIBetCoins [ 1 ]->setNum ( ( nBetCoin % 1000 ) / 100  );
	m_aUIBetCoins [ 2 ]->setNum ( ( nBetCoin % 100  ) / 10   );
	m_aUIBetCoins [ 3 ]->setNum (   nBetCoin % 10			 );

	if ( bIsChange1000	)	m_aUIBetCoins [ 0 ]->setEffect ( );
	if ( bIsChange100	)	m_aUIBetCoins [ 1 ]->setEffect ( );
	if ( bIsChange10	)	m_aUIBetCoins [ 2 ]->setEffect ( );
	if ( bIsChange1		)	m_aUIBetCoins [ 3 ]->setEffect ( );

	if ( bIsChange1000 == false && bIsChange100 == false && bIsChange10 == false && bIsChange1 == false )
	{
			 if ( m_aUIBetCoins [ 0 ]->getNum ( ) > 0 )		m_aUIBetCoins [ 0 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 1 ]->getNum ( ) > 0 )		m_aUIBetCoins [ 1 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 2 ]->getNum ( ) > 0 )		m_aUIBetCoins [ 2 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 3 ]->getNum ( ) > 0 )		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
}

KDvoid CasinoBetNode::_100coinCallback ( Object* pSender )
{
	KDint64		nMyBetCoin = cMyData::getObj ( )->m_nMoney;
	KDint64		nBetCoin   = 0;
	nBetCoin += m_aUIBetCoins [ 0 ]->getNum ( ) * 1000;
	nBetCoin += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
	nBetCoin += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
	nBetCoin += m_aUIBetCoins [ 3 ]->getNum ( );

	// 효과음 출력.
	if ( nBetCoin == nMyBetCoin )
	{
	}

	nBetCoin += 100;
	if ( nBetCoin > nMyBetCoin )	nBetCoin = nMyBetCoin;
	if ( nBetCoin >	      9999 )	nBetCoin = 9999;

	KDbool		bIsChange1000	= false;
	KDbool		bIsChange100	= false;
	KDbool		bIsChange10		= false;
	KDbool		bIsChange1		= false;

	if ( cMyData::getObj ( )->m_nMoney == 0 )
	{
		m_aUIBetCoins [ 0 ]->setEffect ( );
		m_aUIBetCoins [ 1 ]->setEffect ( );
		m_aUIBetCoins [ 2 ]->setEffect ( );
		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
	else
	{
		if (   nBetCoin			 / 1000 != m_aUIBetCoins [ 0 ]->getNum ( ) )	bIsChange1000	= true;
		if ( ( nBetCoin % 1000 ) / 100  != m_aUIBetCoins [ 1 ]->getNum ( ) )	bIsChange100	= true;
		if ( ( nBetCoin % 100  ) / 10   != m_aUIBetCoins [ 2 ]->getNum ( ) )	bIsChange10		= true;
		if (   nBetCoin % 10			!= m_aUIBetCoins [ 3 ]->getNum ( ) )	bIsChange1		= true;
	}			

	m_aUIBetCoins [ 0 ]->setNum (   nBetCoin		  / 1000 );
	m_aUIBetCoins [ 1 ]->setNum ( ( nBetCoin % 1000 ) / 100  );
	m_aUIBetCoins [ 2 ]->setNum ( ( nBetCoin % 100  ) / 10   );
	m_aUIBetCoins [ 3 ]->setNum (   nBetCoin % 10			 );

	if ( bIsChange1000	)	m_aUIBetCoins [ 0 ]->setEffect ( );
	if ( bIsChange100	)	m_aUIBetCoins [ 1 ]->setEffect ( );
	if ( bIsChange10	)	m_aUIBetCoins [ 2 ]->setEffect ( );
	if ( bIsChange1		)	m_aUIBetCoins [ 3 ]->setEffect ( );

	if ( bIsChange1000 == false && bIsChange100 == false && bIsChange10 == false && bIsChange1 == false )
	{
			 if ( m_aUIBetCoins [ 0 ]->getNum ( ) > 0 )		m_aUIBetCoins [ 0 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 1 ]->getNum ( ) > 0 )		m_aUIBetCoins [ 1 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 2 ]->getNum ( ) > 0 )		m_aUIBetCoins [ 2 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 3 ]->getNum ( ) > 0 )		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
}

KDvoid CasinoBetNode::_1000coinCallback	( Object* pSender )
{
	KDint64		nMyBetCoin = cMyData::getObj ( )->m_nMoney;
	KDint64		nBetCoin	= 0;
	nBetCoin += m_aUIBetCoins [ 0 ]->getNum ( ) * 1000;
	nBetCoin += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
	nBetCoin += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
	nBetCoin += m_aUIBetCoins [ 3 ]->getNum ( );

	// 효과음 출력.
	if ( nBetCoin == nMyBetCoin )
	{
	}

	nBetCoin += 1000;
	if ( nBetCoin > nMyBetCoin )	nBetCoin = nMyBetCoin;
	if ( nBetCoin >	      9999 )	nBetCoin = 9999;
	
	KDbool		bIsChange1000	= false;
	KDbool		bIsChange100	= false;
	KDbool		bIsChange10		= false;
	KDbool		bIsChange1		= false;

	if ( cMyData::getObj ( )->m_nMoney == 0 )
	{
		m_aUIBetCoins [ 0 ]->setEffect ( );
		m_aUIBetCoins [ 1 ]->setEffect ( );
		m_aUIBetCoins [ 2 ]->setEffect ( );
		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
	else
	{
		if (   nBetCoin			 / 1000 != m_aUIBetCoins [ 0 ]->getNum ( ) )	bIsChange1000	= true;
		if ( ( nBetCoin % 1000 ) / 100  != m_aUIBetCoins [ 1 ]->getNum ( ) )	bIsChange100	= true;
		if ( ( nBetCoin % 100  ) / 10   != m_aUIBetCoins [ 2 ]->getNum ( ) )	bIsChange10		= true;
		if (   nBetCoin % 10			!= m_aUIBetCoins [ 3 ]->getNum ( ) )	bIsChange1		= true;
	}			

	m_aUIBetCoins [ 0 ]->setNum (   nBetCoin		   / 1000 );
	m_aUIBetCoins [ 1 ]->setNum ( ( nBetCoin % 1000 ) / 100   );
	m_aUIBetCoins [ 2 ]->setNum ( ( nBetCoin % 100  ) / 10    );
	m_aUIBetCoins [ 3 ]->setNum (   nBetCoin % 10			  );

	if ( bIsChange1000	)	m_aUIBetCoins [ 0 ]->setEffect ( );
	if ( bIsChange100	)	m_aUIBetCoins [ 1 ]->setEffect ( );
	if ( bIsChange10	)	m_aUIBetCoins [ 2 ]->setEffect ( );
	if ( bIsChange1		)	m_aUIBetCoins [ 3 ]->setEffect ( );

	if ( bIsChange1000 == false && bIsChange100 == false && bIsChange10 == false && bIsChange1 == false )
	{
			 if ( m_aUIBetCoins [ 0 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 0 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 1 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 1 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 2 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 2 ]->setEffect ( );
		else if ( m_aUIBetCoins [ 3 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 3 ]->setEffect ( );
	}
}

KDvoid CasinoBetNode::coinMinCallback ( Object* pSender )
{
	KDint64		nNowBetMoney = 0;

	nNowBetMoney += m_aUIBetCoins [ 0 ]->getNum ( ) * 1000;
	nNowBetMoney += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
	nNowBetMoney += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
	nNowBetMoney += m_aUIBetCoins [ 3 ]->getNum ( );

	// 효과음 출력.
	if ( nNowBetMoney <= 1 )
	{
	}


	if ( cMyData::getObj ( )->m_nMoney == 0 )
	{
		m_aUIBetCoins [ 0 ]->setNum ( 0 );
		m_aUIBetCoins [ 1 ]->setNum ( 0 );
		m_aUIBetCoins [ 2 ]->setNum ( 0 );
		m_aUIBetCoins [ 3 ]->setNum ( 1 );

		m_aUIBetCoins [ 0 ]->setEffect ( );
		m_aUIBetCoins [ 1 ]->setEffect ( );
		m_aUIBetCoins [ 2 ]->setEffect ( );
		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
	else
	{
		m_aUIBetCoins [ 0 ]->setNum ( 0 );
		m_aUIBetCoins [ 1 ]->setNum ( 0 );
		m_aUIBetCoins [ 2 ]->setNum ( 0 );
		m_aUIBetCoins [ 3 ]->setNum ( 1 );

		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
}

KDvoid CasinoBetNode::coinMaxCallback ( Object* pSender )
{
	KDint64		nBetCoin = 9999;
	if ( nBetCoin > cMyData::getObj ( )->m_nMoney )
	{
		nBetCoin = cMyData::getObj ( )->m_nMoney;
	}

	KDint64		nNowBetMoney = 0;
	nNowBetMoney += m_aUIBetCoins [ 0 ]->getNum ( ) * 1000;
	nNowBetMoney += m_aUIBetCoins [ 1 ]->getNum ( ) * 100;
	nNowBetMoney += m_aUIBetCoins [ 2 ]->getNum ( ) * 10;
	nNowBetMoney += m_aUIBetCoins [ 3 ]->getNum ( );

	// 효과음 출력.
	if ( nBetCoin == nNowBetMoney )
	{
	}

	m_aUIBetCoins [ 0 ]->setNum (   nBetCoin		  / 1000 );
	m_aUIBetCoins [ 1 ]->setNum ( ( nBetCoin % 1000 ) / 100  );
	m_aUIBetCoins [ 2 ]->setNum ( ( nBetCoin % 100  ) / 10   );
	m_aUIBetCoins [ 3 ]->setNum (   nBetCoin % 10			 );

	if ( cMyData::getObj ( )->m_nMoney == 0 )
	{
		m_aUIBetCoins [ 0 ]->setEffect ( );
		m_aUIBetCoins [ 1 ]->setEffect ( );
		m_aUIBetCoins [ 2 ]->setEffect ( );
		m_aUIBetCoins [ 3 ]->setEffect ( );
	}
	else
	{
		if ( m_aUIBetCoins [ 0 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 0 ]->setEffect ( );
		if ( m_aUIBetCoins [ 1 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 1 ]->setEffect ( );
		if ( m_aUIBetCoins [ 2 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 2 ]->setEffect ( );
		if ( m_aUIBetCoins [ 3 ]->getNum ( ) > 0 )	m_aUIBetCoins [ 3 ]->setEffect ( );
	}
}

KDvoid CasinoBetNode::shopExitCallback ( Object* pSender )
{
	m_pBtnExit		->setEnabled ( true );
	m_pBtnShop		->setEnabled ( true );
	m_pBtnGameStart	->setEnabled ( true );
	m_pBtnBack		->setEnabled ( true );
	m_pBtn1Coin		->setEnabled ( true );
	m_pBtn10Coin	->setEnabled ( true );
	m_pBtn100Coin	->setEnabled ( true );
	m_pBtn1000Coin	->setEnabled ( true );
	m_pBtnMinCoin	->setEnabled ( true );
	m_pBtnMaxCoin	->setEnabled ( true );
	m_pBtnShopExit	->setEnabled ( false );

	m_pCoverColor	->setVisible ( false );

	if ( m_eState != eBatStateType_BetView || cMyData::getObj ( )->m_nMoney > 0 )
	{
		m_bUIIsNotCoinView = false;

		m_pUINotCoinPopup->get ( )->setVisible ( false );
		m_pUIShopCursor->setVisible ( false );
		m_pUIShopCursor->stop ( );
	}
	else
	{
		m_bUIIsNotCoinView = true;

		m_pUINotCoinPopup->get ( )->setVisible ( true );
		m_pUIShopCursor->setVisible ( true );
		m_pUIShopCursor->start ( );
	}

	if ( cMyData::getObj ( )->m_bIsOptionSound != false && SimpleAudioEngine::getInstance ( )->isBackgroundMusicPlaying ( ) == false )
	{
		SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "myroom.mp3", true );
	}

	m_pShopNode->close ( );
}

KDvoid CasinoBetNode::updateAllToBetChage ( KDdouble dLeftTIme )
{
	if ( m_eState != eBatStateType_AllToDetailChanging )
	{
		return;
	}

	// pts update
	m_pLabels->update ( dLeftTIme );

	m_aUIMobBars			   [ m_nSelectMob ]->update ( dLeftTIme );
	m_aUIMobBarNums			   [ m_nSelectMob ]->update ( dLeftTIme );
	m_aUIMobBarMagnifications  [ m_nSelectMob ]->update ( dLeftTIme );
	m_aUIMobBarMagnifications2 [ m_nSelectMob ]->update ( dLeftTIme );
	m_aUIMobBarMagnifications3 [ m_nSelectMob ]->update ( dLeftTIme );

	// time
	m_dStateChangeLeftTime -= dLeftTIme;

	// time end
	if ( m_dStateChangeLeftTime < 0 )
	{
		m_pBtnGameStart	->setVisible ( true );
		m_pBtnBack		->setVisible ( true );
		m_pBtn1Coin		->setVisible ( true );
		m_pBtn10Coin	->setVisible ( true );
		m_pBtn100Coin	->setVisible ( true );
		m_pBtn1000Coin	->setVisible ( true );
		m_pBtnMinCoin	->setVisible ( true );
		m_pBtnMaxCoin	->setVisible ( true );
		m_pBtnGameStart	->setEnabled ( true );
		m_pBtnBack		->setEnabled ( true );
		m_pBtn1Coin		->setEnabled ( true );
		m_pBtn10Coin	->setEnabled ( true );
		m_pBtn100Coin	->setEnabled ( true );
		m_pBtn1000Coin	->setEnabled ( true );
		m_pBtnMinCoin	->setEnabled ( true );
		m_pBtnMaxCoin	->setEnabled ( true );

		m_pUICoinBack ->get ( )->setVisible ( true );
		m_pUICoinCover->get ( )->setVisible ( true );

		for ( auto pNode : m_aUIBetCoins )
		{
			pNode->setVisible ( true );
		}

		m_eState = eBatStateType_BetView;
	}
}

KDvoid CasinoBetNode::updateBetToAllChage ( KDdouble dLeftTIme )
{
	if ( m_eState != eBatStateType_DetailToAllChanging )
	{
		return;
	}

	// pts update
	m_pLabels->update ( dLeftTIme );

	m_aUIMobBars			   [ m_nSelectMob ]->update ( dLeftTIme );
	m_aUIMobBarNums			   [ m_nSelectMob ]->update ( dLeftTIme );
	m_aUIMobBarMagnifications  [ m_nSelectMob ]->update ( dLeftTIme );
	m_aUIMobBarMagnifications2 [ m_nSelectMob ]->update ( dLeftTIme );
	m_aUIMobBarMagnifications3 [ m_nSelectMob ]->update ( dLeftTIme );

	// time
	m_dStateChangeLeftTime -= dLeftTIme;

	// time end
	if ( m_dStateChangeLeftTime < 0 )
	{
		// UIs visible
		for ( auto pNode : m_aMobs						)	{ pNode->setVisibleNameB ( true ); pNode->setVisiblePortrait ( true ); }
		for ( auto pNode : m_aUIMobBars					)	pNode->get ( )->setVisible ( true );
		for ( auto pNode : m_aUIMobBarNums				)	pNode->get ( )->setVisible ( true );
		for ( auto pNode : m_aUIMobBarHPs				)	m_pLabels->setVisible ( pNode, true );
		for ( auto pNode : m_aUIMobBarATKs				)	m_pLabels->setVisible ( pNode, true );
		for ( auto pNode : m_aUIMobBarATKs				)	m_pLabels->setVisible ( pNode, true );
		for ( auto pNode : m_aUIMobBarMagnifications	)	pNode->setVisible ( true );
		for ( auto pNode : m_aUIMobBarMagnifications2	)	pNode->setVisible ( true );
		for ( auto pNode : m_aUIMobBarMagnifications3	)	pNode->get ( )->setVisible ( true );

		// UI mob bar select
		m_pUIMobBarSelect ->setVisible ( true );
		m_pUIMobBarSelect2->setVisible ( true );
		m_pUIMobBarSelect3->get ( )->setVisible ( true );
		m_pUIMobBarSelect ->start ( );
		m_pUIMobBarSelect2->start ( );

		// state
		m_eState = eBatStateType_AllView;
	}
}

KDvoid CasinoBetNode::selectMob ( KDint nIndex )
{
	if ( m_eState != eBatStateType_AllView || nIndex < 0 || nIndex >= (KDint) m_aMobs.size ( ) )
	{
		return;
	}

	if ( nIndex == m_nSelectMob )
	{
		// sound
		if ( cMyData::getObj ( )->m_bIsOptionSound != false )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "bet_move.mp3" );			
		}

		// state
		m_eState = eBatStateType_AllToDetailChanging;

		// time
		KDdouble	dMaxTime	= 0;
		switch ( m_nSelectMob )
		{
			case 0	:	dMaxTime = _CHANGE_STATE_TIME_1;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_1;	break;
			case 1	:	dMaxTime = _CHANGE_STATE_TIME_2;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_2;	break;
			case 2	:	dMaxTime = _CHANGE_STATE_TIME_3;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_3;	break;
			case 3	:	dMaxTime = _CHANGE_STATE_TIME_4;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_4;	break;
			case 4	:	dMaxTime = _CHANGE_STATE_TIME_5;	m_dStateChangeLeftTime = _CHANGE_STATE_TIME_5;	break;
		}

		// bars
		for ( KDuint i = 0; i < m_aUIMobBars.size ( ); i++ )
		{
			if ( i == m_nSelectMob )	m_aUIMobBars [ i ]->setPointTime ( Point ( 147, 248 ), dMaxTime );
			else						m_aUIMobBars [ i ]->get ( )->setVisible ( false );
		}

		// bar name_b and portrait
		for ( KDuint i = 0; i < m_aMobs.size ( ); i++ )
		{
			if ( i == m_nSelectMob )
			{
				m_aMobs [ i ]->setPonitTimeNameB	( Point ( 173, 291 ), dMaxTime );
				m_aMobs [ i ]->setPointTimePortrait ( Point ( 410, 290 ), dMaxTime );
			}
			else
			{
				m_aMobs [ i ]->setVisibleNameB	  ( false );
				m_aMobs [ i ]->setVisiblePortrait ( false );
			}
		}

		// bar nums
		for ( KDuint i = 0; i < m_aUIMobBarNums.size ( ); i++ )
		{
			m_aUIMobBarNums [ i ]->get ( )->setVisible ( false );
		}

		// bar para nHp
		for ( KDuint i = 0; i < m_aUIMobBarHPs.size ( ); i++ )
		{
			if ( i == m_nSelectMob )	m_pLabels->setPointTime	( m_aUIMobBarHPs [ i ], Point ( 188, 253 ), dMaxTime );
			else						m_pLabels->setVisible   ( m_aUIMobBarHPs [ i ], false );
		}

		// bar para nAtk
		for ( KDuint i = 0; i < m_aUIMobBarATKs.size ( ); i++ )
		{
			if ( i == m_nSelectMob )	m_pLabels->setPointTime	( m_aUIMobBarATKs [ i ], Point ( 237, 253 ), dMaxTime );
			else						m_pLabels->setVisible   ( m_aUIMobBarATKs [ i ], false );
		}

		// bar para nDef
		for ( KDuint i = 0; i < m_aUIMobBarATKs.size ( ); i++ )
		{
			if ( i == m_nSelectMob )	m_pLabels->setPointTime	( m_aUIMobBarDEFs [ i ], Point ( 297, 253 ), dMaxTime );
			else						m_pLabels->setVisible	( m_aUIMobBarDEFs [ i ], false );
		}

		// UI mob dividend magnifications
		for ( KDuint i = 0; i < m_aUIMobBarMagnifications.size ( ); i++ )
		{
			if ( i == m_nSelectMob )
			{
				m_aUIMobBarMagnifications  [ i ]->setPointTime ( Point ( 348, 268 ), dMaxTime );
				m_aUIMobBarMagnifications2 [ i ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ i ]->getSize ( ).width + 5, 268 ), dMaxTime );
				m_aUIMobBarMagnifications3 [ i ]->setPointTime ( Point ( 348 + m_aUIMobBarMagnifications [ i ]->getSize ( ).width	 , 268 ), dMaxTime );
			}
			else
			{
				m_aUIMobBarMagnifications  [ i ]->setVisible ( false );
				m_aUIMobBarMagnifications2 [ i ]->setVisible ( false );
				m_aUIMobBarMagnifications3 [ i ]->get ( )->setVisible ( false );
			}
		}

		// UI mob bar select
		m_pUIMobBarSelect ->setVisible ( false );
		m_pUIMobBarSelect2->setVisible ( false );
		m_pUIMobBarSelect3->get ( )->setVisible ( false );
		m_pUIMobBarSelect ->stop ( );
		m_pUIMobBarSelect2->stop ( );

		// not coin
		if ( cMyData::getObj ( )->m_nMoney == 0 )
		{
			m_bUIIsNotCoinView = true;

			m_pUINotCoinPopup->get ( )->setVisible ( true );
			m_pUIShopCursor->setVisible ( true );
			m_pUIShopCursor->start ( );
		}
	}
	else
	{
		// before
		if ( m_nSelectMob >= 0 && m_nSelectMob < (KDint) m_aMobs.size ( ) )
		{
			m_aMobs [ m_nSelectMob ]->setVisibleNameA ( false );
			m_aMobs [ m_nSelectMob ]->setVisible ( false );
			m_aMobs [ m_nSelectMob ]->aniStop ( );

			m_aUIMobNums [ m_nSelectMob ]->get ( )->setVisible ( false );
		}

		// after
		cUnit*		pMob = m_aMobs [ nIndex ];

		pMob->setVisibleNameA ( true );
		pMob->setVisible ( true );
		pMob->aniStart ( );

		Point		tSelect3 = Point::ZERO;

		switch ( nIndex )
		{
			case 0	:	m_pUIMobBarSelect->setPoint ( Point ( 127, 270 ) );	m_pUIMobBarSelect2->setPoint ( Point ( 147, 279 ) ); tSelect3 = Point ( 410, 235 );		break;
			case 1	:	m_pUIMobBarSelect->setPoint ( Point ( 127, 215 ) );	m_pUIMobBarSelect2->setPoint ( Point ( 147, 224 ) ); tSelect3 = Point ( 410, 180 );		break;
			case 2	:	m_pUIMobBarSelect->setPoint ( Point ( 127, 160 ) );	m_pUIMobBarSelect2->setPoint ( Point ( 147, 169 ) ); tSelect3 = Point ( 410, 125 );		break;
			case 3	:	m_pUIMobBarSelect->setPoint ( Point ( 127, 105 ) );	m_pUIMobBarSelect2->setPoint ( Point ( 147, 114 ) ); tSelect3 = Point ( 410,  70 );		break;
			case 4	:	m_pUIMobBarSelect->setPoint ( Point ( 127,  50 ) );	m_pUIMobBarSelect2->setPoint ( Point ( 147,  59 ) ); tSelect3 = Point ( 410,  15 );		break;
		}

		m_pUIMobBarSelect ->setVisible ( true );
		m_pUIMobBarSelect2->setVisible ( true );
		m_pUIMobBarSelect ->start ( );
		m_pUIMobBarSelect2->start ( );
		m_pUIMobBarSelect3->get ( )->setVisible	( true );
		m_pUIMobBarSelect3->get ( )->setPosition ( tSelect3 );

		m_aUIMobNums [ nIndex ]->get ( )->setVisible ( true );

		m_pLabels->setStr ( m_aUIMobDetailPara [ 0 ], ccszf ( "HP : %02d / %02d", pMob->getHP ( ), pMob->getHP ( ) ) );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 1 ], ccszf ( "ATK : %02d", pMob->getATK ( ) ) );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 2 ], ccszf ( "DEF : %02d", pMob->getDEF ( ) ) );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 3 ], ccszf ( "AGL : %02d", pMob->getAGL ( ) ) );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 4 ], ccszf ( "CRT : %02d", pMob->getCRT ( ) ) );

		for ( KDuint i = 0; i < m_aUISkillIcons.size ( ); i++ )
		{
			KDint			nSkillIndex = pMob->getSkillType ( i );

			if ( nSkillIndex == -1 )
			{
				m_pLabels->setStr ( m_aUISkillStrs [ i ], "" );
				m_aUISkillIcons[i]->setVisible ( false );
				continue;
			}

			m_aUISkillIcons [ i ]->setVisible ( true );
			m_aUISkillIcons [ i ]->setFrameInit ( nSkillIndex );

			std::string		sStr = pMob->getSkillStr ( i );
			if ( sStr.size ( ) > 0 )
			{
				m_pLabels->setStr ( m_aUISkillStrs [ i ], sStr.c_str ( ) );
			}
		}

		// index
		m_nSelectMob = nIndex;

		// sel pMob
		m_pUISelMob	   ->get ( )->setVisible ( false );
		m_pUISelMobText->get ( )->setVisible ( false );
	}
}

KDvoid CasinoBetNode::setExitTouchEnabled ( KDbool bIsEnabled )
{
	m_pBtnShopExit->setEnabled ( bIsEnabled );
}



