/* -----------------------------------------------------------------------------------
 *
 *      File            CasinoBetNode.h
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

#ifndef __CasinoBetNode_h__
#define __CasinoBetNode_h__

#include "Object/Label/Labels.h"
#include "Object/Label/LabelAtlas.h"
#include "Object/Label/LabelAtlasCustom.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Sprite/SprAni.h"
#include "Object/NumScroll/NumScroll.h"
#include "Object/NodeChageImgs/NodeChangeImgs.h"
#include "Unit/Create/MobDividend.h"
#include "Unit/Create/UnitCreateInfos.h"
#include "Unit/Unit/Unit.h"

class ShopNode;
class cLabelAtlas;

class CasinoBetNode : public Scene
{
	public :
				 CasinoBetNode ( KDvoid );
		virtual ~CasinoBetNode ( KDvoid );

		CREATE_FUNC ( CasinoBetNode );

	public :

		KDbool						init ( KDvoid );

		KDvoid						releaseObject ( KDvoid );

		KDbool						onTouchBegan ( Touch* pTouch, Event* pEvent ); 
		KDvoid						onTouchMoved ( Touch* pTouch, Event* pEvent ); 
		KDvoid						onTouchEnded ( Touch* pTouch, Event* pEvent ); 

		KDvoid						step ( KDfloat fDelta );

		KDbool						updateNodeChage ( KDdouble dLeftTime );

		KDvoid						updateMoney ( KDvoid );

		KDvoid						updateAllToBetChage ( KDdouble dLeftTime );
		KDvoid						updateBetToAllChage ( KDdouble dLeftTime );

		KDvoid						selectMob ( KDint nIndex );

		KDvoid						setExitTouchEnabled ( KDbool bIsEnabled );

		KDvoid						exitCallback		( Object* pSender );
		KDvoid						shopCallback		( Object* pSender );
		KDvoid						gameStartCallback	( Object* pSender );
		KDvoid						backCallback		( Object* pSender );
		KDvoid						_1coinCallback		( Object* pSender );
		KDvoid						_10coinCallback		( Object* pSender );
		KDvoid						_100coinCallback	( Object* pSender );
		KDvoid						_1000coinCallback	( Object* pSender );
		KDvoid						coinMinCallback		( Object* pSender );
		KDvoid						coinMaxCallback		( Object* pSender );
		KDvoid						shopExitCallback	( Object* pSender );

	private :

		// touch
		KDint						m_nTidCount;
		std::list<sTouch>			m_aListTouch;

		// time
		KDdouble					m_dTickCount;
		KDdouble					m_dBeforeTickCount;

		// node change
		KDbool						m_bIsNodeChangeIn;
		KDbool						m_bIsNodeChangeOut;
		KDbool						m_bIsGoTitle;
		cNodeChangeImgs*			m_pNodeChangeImg;

		// shop
		ShopNode*					m_pShopNode;

		// back img
		Sprite*						m_pSprBack;

		// cover color
		LayerColor*					m_pCoverColor;

		// betState
		eBatStateType				m_eState;
		KDdouble					m_dStateChangeLeftTime;

		// back decorations
		std::vector<cSprAni*>		m_aBackDecos;

		// labels
		cLabels*					m_pLabels;

		// mob dividend
		cMobDividend*				m_pMobDividend;

		// mobs
		std::vector<cUnit*>			m_aMobs;

		// btns
		Menu*						m_pBtnExit;
		Menu*						m_pBtnShop;
		Menu*						m_pBtnGameStart;
		Menu*						m_pBtnBack;
		Menu*						m_pBtn1Coin;
		Menu*						m_pBtn10Coin;
		Menu*						m_pBtn100Coin;
		Menu*						m_pBtn1000Coin;
		Menu*						m_pBtnMinCoin;
		Menu*						m_pBtnMaxCoin;
		Menu*						m_pBtnShopExit;

		// UIs
		cLabelAtlasCustom*			m_pUIMoney;
		cSprite*					m_pUISelMob;
		cSprite*					m_pUISelMobText;
		std::vector<cSprite*>		m_aUIMobNums;
		std::vector<cSprite*>		m_aUIMobBars;
		std::vector<cSprite*>		m_aUIMobBarNums;
		std::vector<LabelTTF*>		m_aUIMobBarHPs;
		std::vector<LabelTTF*>		m_aUIMobBarATKs;
		std::vector<LabelTTF*>		m_aUIMobBarDEFs;
		std::vector<cLabelAtlas*>	m_aUIMobBarMagnifications;
		std::vector<cLabelAtlas*>	m_aUIMobBarMagnifications2;
		std::vector<cSprite*>		m_aUIMobBarMagnifications3;
		cSprAnis*					m_pUIMobBarSelect;
		cSprAnis*					m_pUIMobBarSelect2;
		cSprite*					m_pUIMobBarSelect3;
		std::vector<LabelTTF*>		m_aUIMobDetailPara;			// [0] == hp, [1] == atk, [2] == def, [3] == agl, [4] == crt
		// id						m_UI_mobDetail;
		std::vector<cSprAni*>		m_aUISkillIcons;
		std::vector<LabelTTF*>		m_aUISkillStrs;
		cSprite*					m_pUICoinBack;
		cSprite*					m_pUICoinCover;
		std::vector<cNumScroll*>	m_aUIBetCoins;
		KDbool						m_bUIIsNotCoinView;
		cSprite*					m_pUINotCoinPopup;
		cSprAnis*					m_pUIShopCursor;

		//
		KDint						m_nSelectMob;

		//
		std::vector<Rect>			m_aBarSelectRects;

		// sel mob time
		KDdouble					m_dUISelMobTextLeftTime;
};

#endif	// __CasinoBetNode_h__