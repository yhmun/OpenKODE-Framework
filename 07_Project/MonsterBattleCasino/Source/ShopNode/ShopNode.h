/* -----------------------------------------------------------------------------------
 *
 *      File            ShopNode.h
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

#ifndef __ShopNode_h__
#define __ShopNode_h__

#include "Object/Sprite/Sprite.h"
#include "Object/Label/Labels.h"
#include "Object/Label/LabelAtlasCustom.h"

class CasinoBetNode;
class MyRoomNode;
//#import <StoreKit/StoreKit.h>
//#import <UIKit/UIKit.h>

typedef enum
{
	eShopType_Normal		,
	eShopType_Machine		,
} eShopType;


typedef enum
{
	eShopTapType_Coin		,
	eShopTapType_Slot		,
} eShopTapType;


typedef enum
{
	eBuyItemType_Coin		,
	eBuyItemType_Slot		,
} eBuyItemType;

class SKProductsRequest;
class SKProductsResponse;
class SKPaymentTransaction;
class SKPaymentQueue;

class ShopNode : public Layer
{
	class cItemTapResource : public Object
	{
		public :
					 cItemTapResource ( Node* pNode, Point tPoint, KDint z, eBuyItemType eItemType, KDint nBuyCount, KDdouble nPrice, const KDchar* pItemImgPath, const KDchar* pItemNamePath );
			virtual ~cItemTapResource ( KDvoid );

			KDvoid						setVisible ( KDbool bIsVisible );

			KDvoid						setAlpha ( KDint nAlpha );

			inline KDint				getBuyCount ( KDvoid ) { return nBuyCount; }
			inline KDdouble				getPrice ( KDvoid ) { return dPrice; }

		private :

			cSprite*					pSprInfoBack;
			cSprite*					pSprItemImg;
			cSprite*					pSprItemName;
			cLabelAtlasCustom*			pLabelBuyCountNum;
			cLabelAtlasCustom*			pLabelBuyCoinNum;

			Point						tPoint;
			KDint						nBuyCount;
			KDdouble					dPrice;
	};

	public :
				 ShopNode ( KDvoid );
		virtual ~ShopNode ( KDvoid );

		static ShopNode*				getObj ( KDvoid );

		static ShopNode*				createWithBetNode ( CasinoBetNode* pNode );

		static ShopNode*				createWithMyRoomNode ( MyRoomNode* pNode );

	public :

		virtual KDbool					initShop ( KDvoid );

		virtual KDbool					initWithBetNode ( CasinoBetNode* pNode );

		virtual KDbool					initWithMyRoomNode ( MyRoomNode* pNode );

		KDbool							onTouchBegan ( Touch* pTouch, Event* pEvent ); 

		KDvoid							step ( KDfloat fDelta );

		KDvoid							open  ( eShopType eShop, eDollarMachineType eDollarMachine );
		KDvoid							close ( KDvoid );

		KDbool							getIsShop ( KDvoid );

		KDvoid							setTab ( eShopTapType eType );

		KDvoid							buyCoinCallback ( Object* pSender );
		KDvoid							buySlotCallback ( Object* pSender );

		KDvoid							checkCoinAchievements ( KDvoid );

		// dollar machine type
		eDollarMachineType				getDollarMachineType ( KDvoid );

		// buy capsule
		KDbool							isBuyEnd ( KDvoid );
		KDbool							isBuyDollarCapsule ( KDvoid );

		// In App Purchase delegate
		KDvoid							productsRequest		( SKProductsRequest* pRequest, SKProductsResponse* pResponse );
		KDvoid							completeTransaction ( SKPaymentTransaction* pTransaction );
		KDvoid							restoreTransaction  ( SKPaymentTransaction* pTransaction );
		KDvoid							failedTransaction	( SKPaymentTransaction* pTransaction );
		KDvoid							paymentQueue		( SKPaymentQueue* pQueue, Array* pTransactions );

	private : 

		// touch
		KDint							m_nTidCount;
		std::list<sTouch>				m_aListTouch;

		// time
		KDdouble						m_dTickCount;
		KDdouble						m_dBeforeTickCount;

		// back
		Sprite*							m_pSprBack;

		// switch
		KDbool							m_bIsShop;

		// labels
		cLabels*						m_pLabels;

		CasinoBetNode*					m_pNodeCasinoBetNode;
		MyRoomNode*						m_pNodeMyRoomNode;

		// tap imgs
		cSprite*						m_pSprTapCoin1;
		cSprite*						m_pSprTapCoin2;
		cSprite*						m_pSprTapSlot1;
		cSprite*						m_pSprTapSlot2;

		// tab touch rects
		Rect							m_tRectTabCoin;
		Rect							m_tRectTabSlot;

		// itunes
		cSprite*						m_pSprConnectItunesBack;
		cSprite*						m_pSprConnectItunes;

		KDdouble						m_dItuneLeftTime;
		KDint							m_nItuneStep;

		// coin tap
		std::vector<cItemTapResource*>	m_aCoinTapResources;
		MenuItemImage*					m_pCoinBuy1;
		MenuItemImage*					m_pCoinBuy2;
		MenuItemImage*					m_pCoinBuy3;
		MenuItemImage*					m_pCoinBuy4;
		Menu*							m_pBtnCoinTapBuy1;
		Menu*							m_pBtnCoinTapBuy2;
		Menu*							m_pBtnCoinTapBuy3;
		Menu*							m_pBtnCoinTapBuy4;

		KDint							m_nCoinType;

		//slot tap
		std::vector<cItemTapResource*>	m_aSlotTapResources;
		MenuItemImage*					m_pSlotBuy1;
		MenuItemImage*					m_pSlotBuy2;
		MenuItemImage*					m_pSlotBuy3;
		MenuItemImage*					m_pSlotBuy4;
		Menu*							m_pBtnSlotTapBuy1;
		Menu*							m_pBtnSlotTapBuy2;
		Menu*							m_pBtnSlotTapBuy3;
		Menu*							m_pBtnSlotTapBuy4;

		KDint							m_nSlotType;

		// tab
		eShopTapType					m_eTabType;

		// shop type
		eShopType						m_eShopType;

		// machine type
		eDollarMachineType				m_eDollarMachineType;

		// buy capsule
		KDbool							m_bIsBuyEnd;
		KDbool							m_bIsBuyDollarCapsule;
};

#endif	// __ShopNode_h__
