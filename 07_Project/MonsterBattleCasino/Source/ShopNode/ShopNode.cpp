/* -----------------------------------------------------------------------------------
 *
 *      File            ShopNode.cpp
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
#include "ShopNode.h"
#include "CasinoNode/CasinoBetNode/CasinoBetNode.h"
#include "MyRoomNode/MyRoomNode.h"
#include "Object/Menu/SoundMenuItem.h"
#include "MyData/MyData.h"

static ShopNode*		l_pShopNode = nullptr;

ShopNode::cItemTapResource::cItemTapResource ( Node* pNode, Point tPoint, KDint z, eBuyItemType eItemType, KDint nBuyCount, KDdouble nPrice, const KDchar* pItemImgPath, const KDchar* pItemNamePath )
: pSprInfoBack		( nullptr	)
, pSprItemImg		( nullptr	)
, pSprItemName		( nullptr	)
, pLabelBuyCountNum	( nullptr	)
, pLabelBuyCoinNum	( nullptr	)
, tPoint			( tPoint	)
, nBuyCount			( nBuyCount	)
, dPrice			( nPrice	)
{
	if ( eItemType == eBuyItemType_Slot )	pSprInfoBack = new cSprite ( pNode, "Shop/shop_item_back3.png", z, Point::ZERO, Point ( tPoint.x + 54, tPoint.y ) );
	else									pSprInfoBack = new cSprite ( pNode, "Shop/shop_item_back2.png", z, Point::ZERO, Point ( tPoint.x + 54, tPoint.y ) );

	if ( pItemImgPath  != nullptr )			pSprItemImg	 = new cSprite ( pNode, pItemImgPath , z, Point::ZERO, Point ( tPoint.x	-  5, tPoint.y	    ) );
	if ( pItemNamePath != nullptr )			pSprItemName = new cSprite ( pNode, pItemNamePath, z, Point::ZERO, Point ( tPoint.x + 57, tPoint.y + 26 ) );

	pLabelBuyCountNum = new cLabelAtlasCustom ( pNode, "Shop/shop_item_num2.png", 12, z );
	pLabelBuyCountNum->addCharInfo ( '1',  9 );		pLabelBuyCountNum->addCharInfo ( '2', 13 );		pLabelBuyCountNum->addCharInfo ( '3', 13 );
	pLabelBuyCountNum->addCharInfo ( '4', 13 );		pLabelBuyCountNum->addCharInfo ( '5', 13 );		pLabelBuyCountNum->addCharInfo ( '6', 13 );
	pLabelBuyCountNum->addCharInfo ( '7', 13 );		pLabelBuyCountNum->addCharInfo ( '8', 13 );		pLabelBuyCountNum->addCharInfo ( '9', 13 );
	pLabelBuyCountNum->addCharInfo ( '0', 13 );		pLabelBuyCountNum->addCharInfo ( 'X', 14 );		pLabelBuyCountNum->addCharInfo ( 'C', 12 );
	pLabelBuyCountNum->addCharInfo ( 'E', 11 );		pLabelBuyCountNum->addCharInfo ( 'A', 14 );		pLabelBuyCountNum->addCharInfo ( ' ',  6 );

	if ( eItemType == eBuyItemType_Coin )
	{
		pLabelBuyCountNum->setString ( ccszf ( "%dC", nBuyCount ) );
		pLabelBuyCountNum->setPosition ( Point ( tPoint.x + 201 - pLabelBuyCountNum->getSize ( ).width, tPoint.y + 21 ) );
	}
	else
	{
		pLabelBuyCountNum->setString ( ccszf ( "EA X %d" , nBuyCount ) );
		pLabelBuyCountNum->setPosition ( Point ( tPoint.x + 196 - pLabelBuyCountNum->getSize ( ).width, tPoint.y + 21 ) );
	}

	pLabelBuyCoinNum = new cLabelAtlasCustom ( pNode, "Shop/shop_coin_num.png", 15, z );
	pLabelBuyCoinNum->addCharInfo ( '1',  9 );	pLabelBuyCoinNum->addCharInfo ( '2', 13 );	pLabelBuyCoinNum->addCharInfo ( '3', 13 );
	pLabelBuyCoinNum->addCharInfo ( '4', 13 );	pLabelBuyCoinNum->addCharInfo ( '5', 13 );	pLabelBuyCoinNum->addCharInfo ( '6', 13 );
	pLabelBuyCoinNum->addCharInfo ( '7', 13 );	pLabelBuyCoinNum->addCharInfo ( '8', 13 );	pLabelBuyCoinNum->addCharInfo ( '9', 13 );
	pLabelBuyCoinNum->addCharInfo ( '0', 13 );	pLabelBuyCoinNum->addCharInfo ( 'C', 15 );	pLabelBuyCoinNum->addCharInfo ( '$', 13 );
	pLabelBuyCoinNum->addCharInfo ( ',',  7 );	pLabelBuyCoinNum->addCharInfo ( 'K', 15 );	pLabelBuyCoinNum->addCharInfo ( '.',  6 );

	if ( eItemType == eBuyItemType_Coin || eItemType == eBuyItemType_Slot )
	{
		pLabelBuyCoinNum->setString ( ccszf ( "%.2lf$", dPrice ) );
		pLabelBuyCoinNum->setPosition ( Point ( tPoint.x + 196 - pLabelBuyCoinNum->getSize ( ).width, tPoint.y + 4 ) );
	}
	else
	{
		std::string		sByCoin = ccszf ( "%lld", (KDint64) ( nBuyCount * dPrice ) );	
		for ( KDint i = sByCoin.size ( ) - 3; i > 0; i -= 3 )	sByCoin.insert ( i, "," );	
		sByCoin += "C";
	
		pLabelBuyCoinNum->setString ( sByCoin.c_str ( ) );
		pLabelBuyCoinNum->setPosition ( Point ( tPoint.x + 196 - pLabelBuyCoinNum->getSize ( ).width, tPoint.y + 4 ) );
	}

	// visible
	setVisible ( false );
}

ShopNode::cItemTapResource::~cItemTapResource ( KDvoid )
{
	CC_SAFE_RELEASE ( pSprInfoBack );
	CC_SAFE_RELEASE ( pSprItemImg  );
	CC_SAFE_RELEASE ( pSprItemName );
	CC_SAFE_RELEASE ( pLabelBuyCountNum	);
	CC_SAFE_RELEASE ( pLabelBuyCoinNum	);
}

KDvoid ShopNode::cItemTapResource::setVisible ( KDbool bIsVisible )
{
	pSprInfoBack->get ( )->setVisible ( bIsVisible );

	if ( pSprItemImg  != nullptr )	pSprItemImg ->get ( )->setVisible ( bIsVisible );
	if ( pSprItemName != nullptr )	pSprItemName->get ( )->setVisible ( bIsVisible );

	pLabelBuyCountNum->setVisible ( bIsVisible );
	pLabelBuyCoinNum ->setVisible ( bIsVisible );
}

KDvoid ShopNode::cItemTapResource::setAlpha ( KDint nAlpha )
{
	if ( nAlpha <	0 )	nAlpha = 0;
	if ( nAlpha > 255 )	nAlpha = 255;

	pSprInfoBack->get ( )->setOpacity ( nAlpha );

	if ( pSprItemImg  != nullptr )	pSprItemImg ->get ( )->setOpacity ( nAlpha );
	if ( pSprItemName != nullptr )	pSprItemName->get ( )->setOpacity ( nAlpha );

	pLabelBuyCountNum->setColor ( nAlpha, 255, 255, 255 );
	pLabelBuyCoinNum ->setColor ( nAlpha, 255, 255, 255 );
}

ShopNode* ShopNode::getObj ( KDvoid )
{
	return l_pShopNode;
}

ShopNode* ShopNode::createWithBetNode ( CasinoBetNode* pNode )
{
	ShopNode*	pRet = new ShopNode ( );

	if ( pRet && pRet->initWithBetNode ( pNode ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}	

ShopNode* ShopNode::createWithMyRoomNode  ( MyRoomNode* pNode )
{
	ShopNode*	pRet = new ShopNode ( );

	if ( pRet && pRet->initWithMyRoomNode ( pNode ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}	

ShopNode::ShopNode ( KDvoid )
{
	// static
	l_pShopNode		= this;

	m_pLabels		= nullptr;
	m_pSprTapCoin1	= nullptr;
	m_pSprTapCoin2	= nullptr;
	m_pSprTapSlot1	= nullptr;
	m_pSprTapSlot2	= nullptr;
}

ShopNode::~ShopNode ( KDvoid )
{
	l_pShopNode = nullptr;

	CC_SAFE_RELEASE ( m_pLabels );

	CC_SAFE_RELEASE ( m_pSprTapCoin1 );
	CC_SAFE_RELEASE ( m_pSprTapCoin2 );
	CC_SAFE_RELEASE ( m_pSprTapSlot1 );
	CC_SAFE_RELEASE ( m_pSprTapSlot2 );

	SAFE_RELEASE_ARRAYS ( m_aCoinTapResources );
	SAFE_RELEASE_ARRAYS ( m_aSlotTapResources );
}

KDbool ShopNode::initShop ( KDvoid )
{
	if ( !Layer::init ( ) )
	{
		return false;
	}

	// switch
	auto	pListener = EventListenerTouchOneByOne::create ( );
	pListener->onTouchBegan = CC_CALLBACK_2 ( ShopNode::onTouchBegan, this );
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pListener, this );

	// touch
	m_nTidCount = 1;

	// time
	m_dTickCount = 0;
	m_dBeforeTickCount = 0;

	// casino bet node & my room node
	m_pNodeCasinoBetNode = nullptr;
	m_pNodeMyRoomNode	 = nullptr;

	// back
	m_pSprBack = Sprite::create ( "Shop/shop_back.png" );
	m_pSprBack->setPosition	( Point ( 5, 79 ) );
	m_pSprBack->setAnchorPoint ( Point::ZERO );
	this->addChild ( m_pSprBack , LAYER_BACK );

	// switch
	m_bIsShop = false;

	// labels
	m_pLabels = new cLabels ( this );

	// tab imgs
	m_pSprTapCoin1 = new cSprite ( this, "Shop/shop_burton_coin.png"		, LAYER_BACK, Point::ZERO, Point ( 288, 83 ) );
	m_pSprTapCoin2 = new cSprite ( this, "Shop/shop_burton_coin_push.png"	, LAYER_BACK, Point::ZERO, Point ( 288, 83 ) );
	m_pSprTapSlot1 = new cSprite ( this, "Shop/shop_burton_slot.png"		, LAYER_BACK, Point::ZERO, Point ( 378, 83 ) );
	m_pSprTapSlot2 = new cSprite ( this, "Shop/shop_burton_slot_push.png"	, LAYER_BACK, Point::ZERO, Point ( 378, 83 ) );

	// tab touch rects
	m_tRectTabCoin = Rect ( 288, 83, 89, 26 );
	m_tRectTabSlot = Rect ( 378, 83, 89, 26 );

	// itune
	m_pSprConnectItunesBack = new cSprite ( this, "Shop/shop_itunes_bg.png"	, LAYER_SHOP_ITUNES, Point::ZERO, Point (  81, 149 ) );
	m_pSprConnectItunes	    = new cSprite ( this, "Shop/shop_itunes.png"	, LAYER_SHOP_ITUNES, Point::ZERO, Point ( 124, 153 ) );
	m_pSprConnectItunesBack->get ( )->setVisible ( false );
	m_pSprConnectItunes	   ->get ( )->setVisible ( false );

	m_dItuneLeftTime = 0.25f;
	m_nItuneStep	 = 0;

	// coin tab
	m_aCoinTapResources.push_back ( new cItemTapResource ( this, Point (  27, 192 ), LAYER_SHOP_UI_2, eBuyItemType_Coin,  10000, 0.99, "Item/item_coin_1.png", nullptr ) );
	m_aCoinTapResources.push_back ( new cItemTapResource ( this, Point ( 249, 192 ), LAYER_SHOP_UI_2, eBuyItemType_Coin,  33000, 2.99, "Item/item_coin_2.png", nullptr ) );
	m_aCoinTapResources.push_back ( new cItemTapResource ( this, Point (  27, 127 ), LAYER_SHOP_UI_2, eBuyItemType_Coin,  60000, 4.99, "Item/item_coin_3.png", nullptr ) );
	m_aCoinTapResources.push_back ( new cItemTapResource ( this, Point ( 249, 127 ), LAYER_SHOP_UI_2, eBuyItemType_Coin, 130000, 9.99, "Item/item_coin_4.png", nullptr ) );

	// btn
	m_pCoinBuy1 = SoundMenuItem::create ( "Shop/shop_buy.png", "Shop/shop_buy_push.png", CC_CALLBACK_1 ( ShopNode::buyCoinCallback, this ) );
	m_pCoinBuy2 = SoundMenuItem::create ( "Shop/shop_buy.png", "Shop/shop_buy_push.png", CC_CALLBACK_1 ( ShopNode::buyCoinCallback, this ) );
	m_pCoinBuy3 = SoundMenuItem::create ( "Shop/shop_buy.png", "Shop/shop_buy_push.png", CC_CALLBACK_1 ( ShopNode::buyCoinCallback, this ) );
	m_pCoinBuy4 = SoundMenuItem::create ( "Shop/shop_buy.png", "Shop/shop_buy_push.png", CC_CALLBACK_1 ( ShopNode::buyCoinCallback, this ) );

	m_pBtnCoinTapBuy1 = Menu::createWithItem ( m_pCoinBuy1 );
	m_pBtnCoinTapBuy2 = Menu::createWithItem ( m_pCoinBuy2 );
	m_pBtnCoinTapBuy3 = Menu::createWithItem ( m_pCoinBuy3 );
	m_pBtnCoinTapBuy4 = Menu::createWithItem ( m_pCoinBuy4 );

	m_pBtnCoinTapBuy1->setPosition ( Point ( 112, 219 ) );
	m_pBtnCoinTapBuy2->setPosition ( Point ( 334, 219 ) );
	m_pBtnCoinTapBuy3->setPosition ( Point ( 112, 154 ) );
	m_pBtnCoinTapBuy4->setPosition ( Point ( 334, 154 ) );

	m_pBtnCoinTapBuy1->setVisible ( false );
	m_pBtnCoinTapBuy2->setVisible ( false );
	m_pBtnCoinTapBuy3->setVisible ( false );
	m_pBtnCoinTapBuy4->setVisible ( false );

	m_pBtnCoinTapBuy1->setEnabled ( false );
	m_pBtnCoinTapBuy2->setEnabled ( false );
	m_pBtnCoinTapBuy3->setEnabled ( false );
	m_pBtnCoinTapBuy4->setEnabled ( false );

	this->addChild ( m_pBtnCoinTapBuy1 , LAYER_SHOP_UI_3 );
	this->addChild ( m_pBtnCoinTapBuy2 , LAYER_SHOP_UI_3 );
	this->addChild ( m_pBtnCoinTapBuy3 , LAYER_SHOP_UI_3 );
	this->addChild ( m_pBtnCoinTapBuy4 , LAYER_SHOP_UI_3 );

	m_nCoinType = -1;

	// slot tab
	m_aSlotTapResources.push_back ( new cItemTapResource ( this, Point (  27, 192 ), LAYER_SHOP_UI_2, eBuyItemType_Slot,  3, 0.99, "Item/item_slot_2.png", nullptr ) );
	m_aSlotTapResources.push_back ( new cItemTapResource ( this, Point ( 249, 192 ), LAYER_SHOP_UI_2, eBuyItemType_Slot,  9, 1.99, "Item/item_slot_3.png", nullptr ) );
	m_aSlotTapResources.push_back ( new cItemTapResource ( this, Point (  27, 127 ), LAYER_SHOP_UI_2, eBuyItemType_Slot, 30, 5.99, "Item/item_slot_4.png", nullptr ) );

	// btn
	m_pSlotBuy1 = SoundMenuItem::create ( "Shop/shop_buy.png", "Shop/shop_buy_push.png", CC_CALLBACK_1 ( ShopNode::buySlotCallback, this ) );
	m_pSlotBuy2 = SoundMenuItem::create ( "Shop/shop_buy.png", "Shop/shop_buy_push.png", CC_CALLBACK_1 ( ShopNode::buySlotCallback, this ) );
	m_pSlotBuy3 = SoundMenuItem::create ( "Shop/shop_buy.png", "Shop/shop_buy_push.png", CC_CALLBACK_1 ( ShopNode::buySlotCallback, this ) );
	m_pSlotBuy4 = SoundMenuItem::create ( "Shop/shop_buy.png", "Shop/shop_buy_push.png", CC_CALLBACK_1 ( ShopNode::buySlotCallback, this ) );

	m_pBtnSlotTapBuy1 = Menu::createWithItem ( m_pSlotBuy1 );
	m_pBtnSlotTapBuy2 = Menu::createWithItem ( m_pSlotBuy2 );
	m_pBtnSlotTapBuy3 = Menu::createWithItem ( m_pSlotBuy3 );
	m_pBtnSlotTapBuy4 = Menu::createWithItem ( m_pSlotBuy4 );

	m_pBtnSlotTapBuy1->setPosition ( Point ( 112, 219 ) );
	m_pBtnSlotTapBuy2->setPosition ( Point ( 334, 219 ) );
	m_pBtnSlotTapBuy3->setPosition ( Point ( 112, 154 ) );
	m_pBtnSlotTapBuy4->setPosition ( Point ( 334, 154 ) );

	m_pBtnSlotTapBuy1->setVisible ( false );
	m_pBtnSlotTapBuy2->setVisible ( false );
	m_pBtnSlotTapBuy3->setVisible ( false );
	m_pBtnSlotTapBuy4->setVisible ( false );

	m_pBtnSlotTapBuy1->setEnabled ( false );
	m_pBtnSlotTapBuy2->setEnabled ( false );
	m_pBtnSlotTapBuy3->setEnabled ( false );
	m_pBtnSlotTapBuy4->setEnabled ( false );

	this->addChild ( m_pBtnSlotTapBuy1 , LAYER_SHOP_UI_3 );
	this->addChild ( m_pBtnSlotTapBuy2 , LAYER_SHOP_UI_3 );
	this->addChild ( m_pBtnSlotTapBuy3 , LAYER_SHOP_UI_3 );
	this->addChild ( m_pBtnSlotTapBuy4 , LAYER_SHOP_UI_3 );

	m_nSlotType = -1;

	// set tab
	this->setTab ( eShopTapType_Coin );

	// schedule
	this->schedule ( schedule_selector ( ShopNode::step ) );

	return true;
}

KDbool ShopNode::initWithBetNode ( CasinoBetNode* pNode )
{
	if ( !this->initShop ( ) )
	{
		return false;
	}

	// casino bet node
	m_pNodeCasinoBetNode = pNode;

	return true;
}

KDbool ShopNode::initWithMyRoomNode ( MyRoomNode* pNode )
{
	if ( !this->initShop ( ) )
	{
		return false;
	}

	// my room node
	m_pNodeMyRoomNode = pNode;

	return true;
}

KDvoid ShopNode::open ( eShopType eShop, eDollarMachineType eDollarMachine )
{
	if ( eShop == eShopType_Normal )
	{
		// back
		m_pSprBack->setVisible ( true );

		// itune
		m_pSprConnectItunesBack->get ( )->setVisible ( false );

		// tap
		this->setTab ( eShopTapType_Coin );

		// type
		m_nCoinType = -1;
		m_nSlotType = -1;
	}
	else if ( eShop == eShopType_Machine )
	{
		// back
		m_pSprBack->setVisible ( false );

		// tap
		m_pSprTapCoin1->get ( )->setVisible ( false );
		m_pSprTapCoin2->get ( )->setVisible ( false );
		m_pSprTapSlot1->get ( )->setVisible ( false );
		m_pSprTapSlot2->get ( )->setVisible ( false );

		// itune
		m_pSprConnectItunesBack->get ( )->setVisible ( true );
		m_pSprConnectItunes	   ->get ( )->setVisible ( true );
		m_dItuneLeftTime = 0.25f;
		m_nItuneStep = 0;

		// coin tap
		for ( auto pNode : m_aCoinTapResources )	
		{
			pNode->setVisible ( false );
		}

		m_pBtnCoinTapBuy1->setVisible ( false );
		m_pBtnCoinTapBuy2->setVisible ( false );
		m_pBtnCoinTapBuy3->setVisible ( false );
		m_pBtnCoinTapBuy4->setVisible ( false );
		m_pBtnCoinTapBuy1->setEnabled ( false );
		m_pBtnCoinTapBuy2->setEnabled ( false );
		m_pBtnCoinTapBuy3->setEnabled ( false );
		m_pBtnCoinTapBuy4->setEnabled ( false );

		// slot tap
		for ( auto pNode : m_aSlotTapResources )	
		{
			pNode->setVisible ( false );
		}

		m_pBtnSlotTapBuy1->setVisible ( false );
		m_pBtnSlotTapBuy2->setVisible ( false );
		m_pBtnSlotTapBuy3->setVisible ( false );
		m_pBtnSlotTapBuy4->setVisible ( false );
		m_pBtnSlotTapBuy1->setEnabled ( false );
		m_pBtnSlotTapBuy2->setEnabled ( false );
		m_pBtnSlotTapBuy3->setEnabled ( false );
		m_pBtnSlotTapBuy4->setEnabled ( false );

		// buy
		if		( eDollarMachine == eDollarMachineType_Unusual			)	buy_machine_unusual ( );
		else if ( eDollarMachine == eDollarMachineType_Prettiness		)	buy_machine_prettiness ( );
		else if ( eDollarMachine == eDollarMachineType_RegendOfHeroRink )	buy_machine_regendOfHeroRink ( );
		else if ( eDollarMachine == eDollarMachineType_Warvel_vs_Hell	)	buy_machine_warvel_vs_hell ( );

		// machine type
		m_eDollarMachineType = eDollarMachine;

		// buy capsule
		m_bIsBuyEnd			  = false;
		m_bIsBuyDollarCapsule = false;
	}

	m_eShopType = eShop;
	m_bIsShop   = true;
}

KDvoid ShopNode::close ( KDvoid )
{
	// coin tap
	m_pBtnCoinTapBuy1->setEnabled ( false );
	m_pBtnCoinTapBuy2->setEnabled ( false );
	m_pBtnCoinTapBuy3->setEnabled ( false );
	m_pBtnCoinTapBuy4->setEnabled ( false );

	// slot tap
	m_pBtnSlotTapBuy1->setEnabled ( false );
	m_pBtnSlotTapBuy2->setEnabled ( false );
	m_pBtnSlotTapBuy3->setEnabled ( false );
	m_pBtnSlotTapBuy4->setEnabled ( false );

	m_pSprConnectItunesBack->get ( )->setVisible ( false );
	m_pSprConnectItunes	   ->get ( )->setVisible ( false );
		
	m_bIsShop = false;
}

KDbool ShopNode::getIsShop ( KDvoid )
{
	return m_bIsShop;
}

KDbool ShopNode::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	Point	tLocation = this->convertTouchToNodeSpace ( pTouch );

	// tab select
	if ( m_eShopType == eShopType_Normal )
	{
		if      ( RectInPoint ( m_tRectTabCoin, tLocation ) )	
		{
			this->setTab ( eShopTapType_Coin );
			return true;
		}
		else if ( RectInPoint ( m_tRectTabSlot, tLocation ) )
		{
			this->setTab ( eShopTapType_Slot );
			return true;
		}
	}

	return false;
}

KDvoid ShopNode::step ( KDfloat fDelta )
{
	// tickCount
	m_dTickCount += fDelta;

	if ( m_dBeforeTickCount == 0 )
	{
		m_dBeforeTickCount = m_dTickCount;
	}
	KDdouble	dLeftTickCount = m_dTickCount - m_dBeforeTickCount;
	m_dBeforeTickCount = m_dTickCount;

	if ( dLeftTickCount > 0.1f )	
	{
		dLeftTickCount = 0.1f;
	}

	// coin tap
	if ( m_nItuneStep != -1 )
	{
		m_dItuneLeftTime -= dLeftTickCount;

		if ( m_dItuneLeftTime < 0 )
		{
			m_dItuneLeftTime = 0.25f;
			++m_nItuneStep;

			if ( m_nItuneStep > 5 )
			{
				m_nItuneStep = 0;
			}
		}

		switch ( m_nItuneStep )
		{
			case 0	:	m_pSprConnectItunes->get ( )->setTextureRect ( Rect ( 0, 0, 204, 14 ) );	break;
			case 1	:	m_pSprConnectItunes->get ( )->setTextureRect ( Rect ( 0, 0, 210, 14 ) );	break;
			case 2	:	m_pSprConnectItunes->get ( )->setTextureRect ( Rect ( 0, 0, 215, 14 ) );	break;
			case 3	:	m_pSprConnectItunes->get ( )->setTextureRect ( Rect ( 0, 0, 221, 14 ) );	break;
			case 4	:	m_pSprConnectItunes->get ( )->setTextureRect ( Rect ( 0, 0, 227, 14 ) );	break;
			case 5	:	m_pSprConnectItunes->get ( )->setTextureRect ( Rect ( 0, 0, 232, 14 ) );	break;
		}
	}
}

KDvoid ShopNode::setTab ( eShopTapType eType )
{
	m_eTabType = eType;

	// tap btn
	m_pSprTapCoin1->get ( )->setVisible ( true  );
	m_pSprTapCoin2->get ( )->setVisible ( false );
	m_pSprTapSlot1->get ( )->setVisible ( true  );
	m_pSprTapSlot2->get ( )->setVisible ( false );

	// coin tap
	for ( auto pNode : m_aCoinTapResources )	
	{
		pNode->setVisible ( false );
	}

	m_pBtnCoinTapBuy1->setVisible ( false );
	m_pBtnCoinTapBuy2->setVisible ( false );
	m_pBtnCoinTapBuy3->setVisible ( false );
	m_pBtnCoinTapBuy4->setVisible ( false );
	m_pBtnCoinTapBuy1->setEnabled ( false );
	m_pBtnCoinTapBuy2->setEnabled ( false );
	m_pBtnCoinTapBuy3->setEnabled ( false );
	m_pBtnCoinTapBuy4->setEnabled ( false );

	// slot tap
	for ( auto pNode : m_aSlotTapResources )	
	{
		pNode->setVisible ( false );
	}

	m_pBtnSlotTapBuy1->setVisible ( false );
	m_pBtnSlotTapBuy2->setVisible ( false );
	m_pBtnSlotTapBuy3->setVisible ( false );
	m_pBtnSlotTapBuy4->setVisible ( false );
	m_pBtnSlotTapBuy1->setEnabled ( false );
	m_pBtnSlotTapBuy2->setEnabled ( false );
	m_pBtnSlotTapBuy3->setEnabled ( false );
	m_pBtnSlotTapBuy4->setEnabled ( false );

	if ( m_eTabType == eShopTapType_Coin )
	{
		m_pSprTapCoin1->get ( )->setVisible ( false );
		m_pSprTapCoin2->get ( )->setVisible ( true  );

		for ( auto pNode : m_aCoinTapResources )	
		{
			pNode->setVisible ( true );
		}

		for ( KDuint i = 0; i < m_aCoinTapResources.size ( ); i++ )
		{
			switch ( i )
			{
				case 0	:	m_pBtnCoinTapBuy1->setVisible ( true );	m_pBtnCoinTapBuy1->setEnabled ( true );		break;
				case 1	:	m_pBtnCoinTapBuy2->setVisible ( true );	m_pBtnCoinTapBuy2->setEnabled ( true );		break;
				case 2	:	m_pBtnCoinTapBuy3->setVisible ( true );	m_pBtnCoinTapBuy3->setEnabled ( true );		break;
				case 3	:	m_pBtnCoinTapBuy4->setVisible ( true );	m_pBtnCoinTapBuy4->setEnabled ( true );		break;
			}
		}
	}

	if ( m_eTabType == eShopTapType_Slot )
	{
		m_pSprTapSlot1->get ( )->setVisible ( false );
		m_pSprTapSlot2->get ( )->setVisible ( true  );

		for ( auto pNode : m_aSlotTapResources )	
		{
			pNode->setVisible ( true );
		}

		for ( KDuint i = 0; i < m_aSlotTapResources.size ( ); i++ )
		{
			switch ( i )
			{
				case 0	:	m_pBtnSlotTapBuy1->setVisible ( true );	m_pBtnSlotTapBuy1->setEnabled ( true );		break;
				case 1	:	m_pBtnSlotTapBuy2->setVisible ( true );	m_pBtnSlotTapBuy2->setEnabled ( true );		break;
				case 2	:	m_pBtnSlotTapBuy3->setVisible ( true );	m_pBtnSlotTapBuy3->setEnabled ( true );		break;
				case 3	:	m_pBtnSlotTapBuy4->setVisible ( true );	m_pBtnSlotTapBuy4->setEnabled ( true );		break;
			}
		}
	}
}

KDvoid ShopNode::buyCoinCallback ( Object* pSender )
{
	KDint		nSelect = -1;

		 if ( pSender == m_pCoinBuy1 )	nSelect = 0;
	else if ( pSender == m_pCoinBuy2 )	nSelect = 1;
	else if ( pSender == m_pCoinBuy3 )	nSelect = 2;
	else if ( pSender == m_pCoinBuy4 )	nSelect = 3;

	if		( nSelect == 0 ) { m_nCoinType = 0;	buy_10000_coin  ( ); }	
	else if ( nSelect == 1 ) { m_nCoinType = 1; buy_33000_coin  ( ); }	
	else if ( nSelect == 2 ) { m_nCoinType = 2; buy_60000_coin  ( ); }	
	else if ( nSelect == 3 ) { m_nCoinType = 3; buy_130000_coin ( ); }	

	if ( m_pNodeCasinoBetNode != nullptr )	m_pNodeCasinoBetNode->setExitTouchEnabled ( false );
	if ( m_pNodeMyRoomNode	  != nullptr )	m_pNodeMyRoomNode	->setExitTouchEnabled ( false );

	m_pBtnCoinTapBuy1->setEnabled ( false );
	m_pBtnCoinTapBuy2->setEnabled ( false );
	m_pBtnCoinTapBuy3->setEnabled ( false );
	m_pBtnCoinTapBuy4->setEnabled ( false );

	m_pSprConnectItunesBack->get ( )->setVisible ( true );
	m_pSprConnectItunes	   ->get ( )->setVisible ( true );
	m_dItuneLeftTime = 0.25f;
	m_nItuneStep = 0;
}

KDvoid ShopNode::buySlotCallback ( Object* pSender )
{
	KDint		nSelect = -1;

		 if ( pSender == m_pSlotBuy1 )	nSelect = 0;
	else if ( pSender == m_pSlotBuy2 )	nSelect = 1;
	else if ( pSender == m_pSlotBuy3 )	nSelect = 2;
	else if ( pSender == m_pSlotBuy4 )	nSelect = 3;

	if      ( nSelect == 0 ) { m_nSlotType = 0; buy_3_slot  ( ); }	
	else if ( nSelect == 1 ) { m_nSlotType = 1;	buy_9_slot  ( ); }	
	else if ( nSelect == 2 ) { m_nSlotType = 2;	buy_30_slot ( ); }	

	if ( m_pNodeCasinoBetNode != nullptr )	m_pNodeCasinoBetNode->setExitTouchEnabled ( false );
	if ( m_pNodeMyRoomNode	  != nullptr )	m_pNodeMyRoomNode   ->setExitTouchEnabled ( false );

	m_pBtnSlotTapBuy1->setEnabled ( false );
	m_pBtnSlotTapBuy2->setEnabled ( false );
	m_pBtnSlotTapBuy3->setEnabled ( false );
	m_pBtnSlotTapBuy4->setEnabled ( false );

	m_pSprConnectItunesBack->get ( )->setVisible ( true );
	m_pSprConnectItunes	   ->get ( )->setVisible ( true );
	m_dItuneLeftTime = 0.25f;
	m_nItuneStep = 0;
}

KDvoid ShopNode::checkCoinAchievements ( KDvoid )
{
	KDint64		nMoney	= cMyData::getObj ( )->m_nMoney;

	if ( nMoney >= 500			  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_500_ID			andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 1000			  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_1000_ID			andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 10000		  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_10000_ID		andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 100000		  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_100000_ID		andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 1000000		  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_1000000_ID		andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 10000000		  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_10000000_ID		andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 100000000	  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_100000000_ID	andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 1000000000	  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_1000000000_ID	andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 10000000000LL  )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_10000000000_ID	andPercentComplete:100 andShowNotification:true );
	if ( nMoney >= 100000000000LL )	{}	//[OFAchievementService updateAchievement:_OPENFEINT_ACHIEVEMENT_COIN_100000000000_ID	andPercentComplete:100 andShowNotification:true );
}

eDollarMachineType ShopNode::getDollarMachineType ( KDvoid )
{
	return m_eDollarMachineType;
}

KDbool ShopNode::isBuyEnd ( KDvoid )
{
	return m_bIsBuyEnd;
}

KDbool ShopNode::isBuyDollarCapsule ( KDvoid )
{
	return m_bIsBuyDollarCapsule;
}

// In App Purchase delegate------------------------------------------------------------------------------------------------------------------------------------------
KDvoid ShopNode::productsRequest ( SKProductsRequest* pRequest, SKProductsResponse* pResponse )
{ 
#if 0
	if ( [response.products count] > 0 )
	{
		SKProduct *product = [response.products objectAtIndex:0 );

		SKPayment *payment = [SKPayment paymentWithProduct:product );
		[[SKPaymentQueue defaultQueue] addPayment:payment );
	}
	else
#endif	// 0
	{
		if ( m_eShopType == eShopType_Normal )
		{
			if ( m_pNodeCasinoBetNode != nullptr )	m_pNodeCasinoBetNode->setExitTouchEnabled ( true );
			if ( m_pNodeMyRoomNode	  != nullptr )	m_pNodeMyRoomNode	->setExitTouchEnabled ( true );

			if ( m_eTabType == eShopTapType_Coin )
			{			
				for ( KDuint i = 0; i < m_aCoinTapResources.size ( ); i++ )
				{
					switch ( i )
					{
						case 0	:	m_pBtnCoinTapBuy1->setEnabled ( true );		break;
						case 1	:	m_pBtnCoinTapBuy2->setEnabled ( true );		break;
						case 2	:	m_pBtnCoinTapBuy3->setEnabled ( true );		break;
						case 3	:	m_pBtnCoinTapBuy4->setEnabled ( true );		break;
					}
				}
			}

			if ( m_eTabType == eShopTapType_Slot )
			{
				for ( KDuint i = 0; i < m_aSlotTapResources.size ( ); i++ )
				{
					switch ( i )
					{
						case 0	:	m_pBtnSlotTapBuy1->setEnabled ( true );		break;
						case 1	:	m_pBtnSlotTapBuy2->setEnabled ( true );		break;
						case 2	:	m_pBtnSlotTapBuy3->setEnabled ( true );		break;
						case 3	:	m_pBtnSlotTapBuy4->setEnabled ( true );		break;
					}
				}
			}
		}
		else if ( m_eShopType == eShopType_Machine )
		{
			m_bIsBuyEnd = true;
			m_bIsBuyDollarCapsule = false;
		}

		m_pSprConnectItunesBack->get ( )->setVisible ( false );
		m_pSprConnectItunes	   ->get ( )->setVisible ( false );
		m_dItuneLeftTime = 0;
		m_nItuneStep = -1;
	}
#if 0
	[request autorelease ); 
#endif	// 0
}

KDvoid ShopNode::completeTransaction ( SKPaymentTransaction* pTransaction )
{
	if ( m_eShopType == eShopType_Normal )
	{
		if ( m_eTabType == eShopTapType_Coin )
		{			
			for ( KDuint i = 0; i < m_aCoinTapResources.size ( ); i++ )
			{
				switch ( i )
				{
					case 0	:	m_pBtnCoinTapBuy1->setEnabled ( true );		break;
					case 1	:	m_pBtnCoinTapBuy2->setEnabled ( true );		break;
					case 2	:	m_pBtnCoinTapBuy3->setEnabled ( true );		break;
					case 3	:	m_pBtnCoinTapBuy4->setEnabled ( true );		break;
				}
			}
		}
		else if ( m_eTabType == eShopTapType_Slot )
		{
			for ( KDuint i = 0; i < m_aSlotTapResources.size ( ); i++ )
			{
				switch ( i )
				{
					case 0	:	m_pBtnSlotTapBuy1->setEnabled ( true );		break;
					case 1	:	m_pBtnSlotTapBuy2->setEnabled ( true );		break;
					case 2	:	m_pBtnSlotTapBuy3->setEnabled ( true );		break;
					case 3	:	m_pBtnSlotTapBuy4->setEnabled ( true );		break;
				}
			}
		}
		
		if ( m_eTabType == eShopTapType_Coin )
		{
			if ( m_nCoinType == 0 )
			{
				cMyData::getObj ( )->m_nMoney += 10000;
				cMyData::getObj ( )->save ( );

#if 0
				[OFHighScoreService setHighScore:(KDint64)cMyData::getObj ( )->m_money forLeaderboard:_OPENFEINT_LEADERBOARD_RICH_ID onSuccess:OFDelegate() onFailure:OFDelegate() );
#endif
				// check achievements
				this->checkCoinAchievements ( );
			}
			else if ( m_nCoinType == 1 )
			{
				cMyData::getObj ( )->m_nMoney += 33000;
				cMyData::getObj ( )->save ( );
#if 0
				[OFHighScoreService setHighScore:(KDint64)cMyData::getObj ( )->m_money forLeaderboard:_OPENFEINT_LEADERBOARD_RICH_ID onSuccess:OFDelegate() onFailure:OFDelegate() );
#endif 
				// check achievements
				this->checkCoinAchievements ( );
			}
			else if ( m_nCoinType == 2 )
			{
				cMyData::getObj ( )->m_nMoney += 60000;
				cMyData::getObj ( )->save ( );

#if 0
				[OFHighScoreService setHighScore:(KDint64)cMyData::getObj ( )->m_money forLeaderboard:_OPENFEINT_LEADERBOARD_RICH_ID onSuccess:OFDelegate() onFailure:OFDelegate() );
#endif	// 0
				// check achievements
				this->checkCoinAchievements ( );
			}
			else if ( m_nCoinType == 3 )
			{
				cMyData::getObj ( )->m_nMoney += 130000;
				cMyData::getObj ( )->save ( );
#if 0
				[OFHighScoreService setHighScore:(KDint64)cMyData::getObj ( )->m_money forLeaderboard:_OPENFEINT_LEADERBOARD_RICH_ID onSuccess:OFDelegate() onFailure:OFDelegate() );
#endif
				// check achievements
				this->checkCoinAchievements ( );
			}
		}
		else if ( m_eTabType == eShopTapType_Slot )
		{
			if ( m_nSlotType == 0 )
			{
				cMyData::getObj ( )->m_nSlotCount += 3;
				cMyData::getObj ( )->save (  );
			}
			else if ( m_nSlotType == 1 )
			{
				cMyData::getObj ( )->m_nSlotCount += 9;
				cMyData::getObj ( )->save (  );
			}
			else if ( m_nSlotType == 2 )
			{
				cMyData::getObj ( )->m_nSlotCount += 30;
				cMyData::getObj ( )->save (  );
			}
		}

		m_nCoinType = -1;
		m_nSlotType = -1;

		m_pSprConnectItunesBack->get ( )->setVisible ( false );
		m_pSprConnectItunes	   ->get ( )->setVisible ( false );
		m_dItuneLeftTime = 0;
		m_nItuneStep = -1;

		if ( m_pNodeCasinoBetNode != nullptr )	m_pNodeCasinoBetNode->setExitTouchEnabled ( true );
		if ( m_pNodeMyRoomNode	  != nullptr )	m_pNodeMyRoomNode	->setExitTouchEnabled ( true );
	}
	else if ( m_eShopType == eShopType_Machine )
	{
		m_bIsBuyEnd = true;
		m_bIsBuyDollarCapsule = true;
	}

//	[[SKPaymentQueue defaultQueue] finishTransaction: transaction );
}

KDvoid ShopNode::restoreTransaction ( SKPaymentTransaction* pTransaction )
{
	if ( m_eShopType == eShopType_Normal )
	{
		m_nCoinType = -1;
		m_nSlotType = -1;

		if ( m_pNodeCasinoBetNode != nullptr )	m_pNodeCasinoBetNode->setExitTouchEnabled ( true );
		if ( m_pNodeMyRoomNode	  != nullptr )	m_pNodeMyRoomNode	->setExitTouchEnabled ( true );

		if ( m_eTabType == eShopTapType_Coin )
		{			
			for ( KDuint i = 0; i < m_aCoinTapResources.size ( ); i++ )
			{
				switch ( i )
				{
					case 0	:	m_pBtnCoinTapBuy1->setEnabled ( true );		break;
					case 1	:	m_pBtnCoinTapBuy2->setEnabled ( true );		break;
					case 2	:	m_pBtnCoinTapBuy3->setEnabled ( true );		break;
					case 3	:	m_pBtnCoinTapBuy4->setEnabled ( true );		break;
				}
			}
		}

		if ( m_eTabType == eShopTapType_Slot )
		{			
			for ( KDuint i = 0; i < m_aSlotTapResources.size ( ); i++ )
			{
				switch ( i )
				{
					case 0	:	m_pBtnSlotTapBuy1->setEnabled ( true );		break;
					case 1	:	m_pBtnSlotTapBuy2->setEnabled ( true );		break;
					case 2	:	m_pBtnSlotTapBuy3->setEnabled ( true );		break;
					case 3	:	m_pBtnSlotTapBuy4->setEnabled ( true );		break;
				}
			}
		}

		m_pSprConnectItunesBack->get ( )->setVisible ( false );
		m_pSprConnectItunes	   ->get ( )->setVisible ( false );
		m_dItuneLeftTime = 0;
		m_nItuneStep = -1;
	}
	else if ( m_eShopType == eShopType_Machine )
	{
		m_bIsBuyEnd = true;
		m_bIsBuyDollarCapsule = false;
	}
#if 0
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction ); 
#endif
}

KDvoid ShopNode::failedTransaction ( SKPaymentTransaction* pTransaction )
{ 
	if ( m_eShopType == eShopType_Normal )
	{
		m_nCoinType = -1;
		m_nSlotType = -1;

		if ( m_pNodeCasinoBetNode != nullptr )	m_pNodeCasinoBetNode->setExitTouchEnabled ( true );
		if ( m_pNodeMyRoomNode	  != nullptr )	m_pNodeMyRoomNode	->setExitTouchEnabled ( true );
		
		if ( m_eTabType == eShopTapType_Coin )
		{			
			for ( KDuint i = 0; i < m_aCoinTapResources.size ( ); i++ )
			{
				switch ( i )
				{
					case 0	:	m_pBtnCoinTapBuy1->setEnabled ( true );	break;
					case 1	:	m_pBtnCoinTapBuy2->setEnabled ( true );	break;
					case 2	:	m_pBtnCoinTapBuy3->setEnabled ( true );	break;
					case 3	:	m_pBtnCoinTapBuy4->setEnabled ( true );	break;
				}
			}
		}
		if ( m_eTabType == eShopTapType_Slot )
		{			
			for ( KDuint i = 0; i < m_aSlotTapResources.size ( ); i++ )
			{
				switch ( i )
				{
					case 0	:	m_pBtnSlotTapBuy1->setEnabled ( true );	break;
					case 1	:	m_pBtnSlotTapBuy2->setEnabled ( true );	break;
					case 2	:	m_pBtnSlotTapBuy3->setEnabled ( true );	break;
					case 3	:	m_pBtnSlotTapBuy4->setEnabled ( true );	break;
				}
			}
		}

		m_pSprConnectItunesBack->get ( )->setVisible ( false );
		m_pSprConnectItunes	   ->get ( )->setVisible ( false );
		m_dItuneLeftTime = 0;
		m_nItuneStep = -1;
	}
	else if ( m_eShopType == eShopType_Machine )
	{
		m_bIsBuyEnd = true;
		m_bIsBuyDollarCapsule = false;
	}

#if 0
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction ); 
#endif
}

KDvoid ShopNode::paymentQueue ( SKPaymentQueue* pQueue, Array* pTransactions )
{ 
#if 0
    for (SKPaymentTransaction *transaction in transactions) 
    { 
        switch (transaction.transactionState) 
        { 
			case SKPaymentTransactionStatePurchased	:	[self completeTransaction:transaction );		break; 
			case SKPaymentTransactionStateFailed	:	[self failedTransaction	 :transaction );		break; 
			case SKPaymentTransactionStateRestored	:	[self restoreTransaction :transaction );		break; 
        }
    }
#endif	 // 0
}
