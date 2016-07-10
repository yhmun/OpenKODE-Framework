/* -----------------------------------------------------------------------------------
 *
 *      File            MyRoomNode.h
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

#ifndef __MyRoomNode_h__
#define __MyRoomNode_h__

#include "Object/Label/Labels.h"
#include "Object/Label/LabelAtlasCustom.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Sprite/SprAni.h"
#include "Object/Sprite/SprAnis.h"
#include "Object/NodeChageImgs/NodeChangeImgs.h"
#include "Object/Particle/Particle.h"
#include "Unit/Unit/Unit.h"

class cUnit;
class cSprProp;
class ShopNode;

#define _SLOT_IN_PAGE_COUNT			3
#define _NUM_IN_PAGE_COUNT			5

class cMachine : public Object
{
	public :
				 cMachine ( Node* pNode, cUnitCreateInfos* pUnitCreateInfo, eCreateInfoFileType eCreateInfoFile, const KDchar* pSprTopName, const KDchar* pSprUnitListName, const KDchar* pSprNotCoin, KDint z, Point tPoint, KDint64 nNeedCoin, eDollarMachineType eDollarMachine, KDdouble bBuyDelay, KDdouble* pBuyLeftTime, KDbool bIsHaveCheckMark );
		virtual ~cMachine ( KDvoid );

		KDvoid							setVisible ( KDbool bVisible );
		KDvoid							setVisibleSelect ( KDbool bVisible );
		KDvoid							setVisibleUnitList ( KDbool bVisible, KDbool bIsMove );
		KDvoid							setVisibleNotCoinTrue ( KDvoid );
		KDvoid							setPoint ( Point tPoint );
		KDvoid							setHandleRotate ( KDfloat fRotation );
		KDvoid							setAlpha ( KDubyte cAlpha );
		KDvoid							setIsMark ( std::vector<cUnit*> aUnits );
		KDvoid							setColorLabelNum ( KDubyte r, KDubyte g, KDubyte b );

		const cUnitCreateInfo*			getUnitInfoRand ( KDvoid );
		inline KDubyte					getAlpha ( KDvoid )	{ return m_cAlpha; }
		inline KDint64					getNeedCoin ( KDvoid )	{ return m_nNeedCoin; }
		inline eDollarMachineType		getDollarMachineType ( KDvoid )	{ return m_eDollarMachineType; }
		inline KDdouble					buyDelayTime ( KDvoid )	{ return m_dBuyDelay; }
		inline KDdouble*				buyLeftTime ( KDvoid )	{ return m_pBuyLeftTime; }
		inline eCreateInfoFileType		getCreateInfoFileType ( KDvoid ) { return m_eCreateInfoFileType; }
		KDbool							getVisibleUnitList ( KDvoid );

		KDvoid							update ( KDdouble dLeftTime );

	private :

		Node*							m_pNode;
		KDint							m_nZoder;

		cUnitCreateInfos*				m_pUnitCreateInfo;
		eCreateInfoFileType				m_eCreateInfoFileType;
		std::vector<std::string>		m_sCreateInfoNames;

		cSprite*						m_pSprBack;
		cSprite*						m_pSprFront;
		cSprite*						m_pSprTop;
		cSprite*						m_pSprHandle;
		cSprite*						m_pSprNotCoin;
		cSprite*						m_pSprSelectL;
		cSprite*						m_pSprSelectT;
		cSprite*						m_pSprSelectR;
		cSprite*						m_pSprSelectB;
		cSprite*						m_pSprUnitList;
		std::vector<cSprite*>			m_pSprHaveMarks;
		cLabelAtlasCustom*				m_pLabelNum;

		std::string						m_sSprUnitListName;

		Point							m_tPoint;

		KDint64							m_nNeedCoin;
		eDollarMachineType				m_eDollarMachineType;

		KDdouble						m_dBuyDelay;
		KDdouble*						m_pBuyLeftTime;

		KDbool							m_bIsHaveCheckMark;
		std::vector<KDbool>				m_aIsHaveMarks;

		KDbool							m_bUnitListMoveIn;
		KDbool							m_bUnitListMoveOut;
		KDdouble						m_dUnitListMoveLeftTime;

		KDubyte							m_cAlpha;
};

class cMyRoomLvlUpEffect : public Object
{
	public :
				 cMyRoomLvlUpEffect ( Node* pNode, KDint z, Point tCenter, Point tSpecial, KDint nSlot, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt );
		virtual ~cMyRoomLvlUpEffect ( KDvoid );

		KDbool							update ( KDdouble dLeftTime );

		KDvoid							setVisible ( KDbool bVisible );
		KDvoid							setPoint ( Point ptCenter, Point ptSpecial );
		inline KDint					getSlotIndex ( KDvoid ) { return m_nSlot; }

	private :

		// level up UI
		cSprite*						m_pUILevelUp;
		cSprAni*						m_pUIStatusHp;
		cSprAni*						m_pUIStatusAtk;
		cSprAni*						m_pUIStatusDef;
		cSprAni*						m_pUIStatusAgl;
		cSprAni*						m_pUIStatusCrt;

		KDint							m_nSlot;

		KDint							m_nHp;
		KDint							m_nAtk;
		KDint							m_nDef;
		KDint							m_nAgl;
		KDint							m_nCrt;

		KDdouble						m_dLeftTime;

		KDdouble						m_dHpStartLeftTime;
		KDdouble						m_dAtkStartLeftTime;
		KDdouble						m_dDefStartLeftTime;
		KDdouble						m_dAglStartLeftTime;
		KDdouble						m_dCrtStartLeftTime;

		KDdouble						m_dLvLeftTime;
		KDdouble						m_dHpLeftTime;
		KDdouble						m_dAtkLeftTime;
		KDdouble						m_dDefLeftTime;
		KDdouble						m_dAglLeftTime;
		KDdouble						m_dCrtLeftTime;

		Point							m_tPointCenter;
		Point							m_tPointSpecial;
		Point							m_tPointHp;
		Point							m_tPointAtk;
		Point							m_tPointDef;
		Point							m_tPointAgl;
		Point							m_tPointCrt;
};

class cLevelPopup : public Object
{
	public :
				 cLevelPopup ( Node* pNode, KDint z, cLabels* pLabels, const KDchar* pStr, Point tPoint );
		virtual ~cLevelPopup ( KDvoid );

		inline Point					getPoint ( KDvoid )	{ return m_tPoint; }

		KDbool							update ( KDdouble dLeftTime );

	private :

		cSprite*						m_pSprBack;
		cLabels*						m_pLabels;
		LabelTTF*						m_pLabel;

		Point							m_tPoint;
		KDdouble						m_dLeftTime;
};

class cSprProp : public Object
{
	public :

				 cSprProp ( Node* pNode, KDint z, Point tPoint );
		virtual ~cSprProp ( KDvoid );

		KDvoid							setSelect ( eUniqueType eType );
		KDvoid							setVisible ( KDbool bVisible );
		KDvoid							setColor ( KDubyte r, KDubyte g, KDubyte b );

	private :

		cSprite*						m_pNot;
		cSprite*						m_pCommon;
		cSprite*						m_pUncommon;
		cSprite*						m_pRare;
		cSprite*						m_pUnique;
};

class cBtnSprite : public Object
{
	public :

				 cBtnSprite ( Node* pNode, const KDchar* pPathNor, const KDchar* pPathPush, KDint z, Point tPoint, Size tSizeTouch, KDbool bIsSound );
		virtual ~cBtnSprite ( KDvoid );

		KDbool							touchesBegan ( Touch* pTouch );
		KDbool							touchesMoved ( Touch* pTouch );
		KDbool							touchesEnded ( Touch* pTouch );

		KDvoid							setVisible ( KDbool bIsVisible, KDbool bIsPush = false );
		KDvoid							setIsTouchEnable ( KDbool	bIsTouchEnable );
		KDvoid							setPoint ( Point tPoint );
		KDvoid							setColor ( KDubyte r, KDubyte g, KDubyte b );

		inline KDbool					getVisible ( KDvoid ) { return m_bIsVisible; }

	private :

		cSprite*						m_pSprite;
		cSprite*						m_pSpritePush;

		KDbool							m_bIsVisible;
		KDbool							m_bIsTouchEnable;

		Point							m_tPoint;
		Size							m_tSizeTouch;
		Rect							m_tRectTouch;

		KDbool							m_bIsTouch;
		KDint							m_nTouchID;

		KDbool							m_bIsSound;
};

class cBtnDragSprite : public Object
{
	public :

				 cBtnDragSprite ( Node* pNode, const KDchar* pPathNor, const KDchar* pPathPush, KDint z, Point tPoint, Size tSizeTouch );
		virtual ~cBtnDragSprite ( KDvoid );

		KDbool							touchesMoved ( Touch* pTouch );

		KDvoid							setVisible ( KDbool isVisible );
		KDvoid							setIsTouchEnable ( KDbool isTouchEnable );
		KDvoid							setPoint ( Point tPoint );
		KDvoid							setColor ( KDubyte r, KDubyte g, KDubyte b );

		KDbool							update ( KDdouble dLeftTime );

		inline KDbool					getVisible ( KDvoid ) { return m_bIsVisible; }

	private :

		cSprite*						m_pSprite;
		cSprite*						m_pSpritePush;

		KDbool							m_bIsVisible;
		KDbool							m_bIsTouchEnable;

		Point							m_tPoint;
		Size							m_tSizeTouch;
		Rect							m_tRectTouch;

		KDbool							m_bIsTouch;
		KDint							m_nTouchID;

		KDdouble						m_dTouchEndedLeftTime;
};

class cBtnNum : public Object
{
	public :
				 cBtnNum ( Node* pNode, KDint z, Point tPoint, Size tSizeTouch, KDint nNum );
		virtual ~cBtnNum ( KDvoid );

		KDbool							touchesBegan ( Touch* pTouch );
		KDbool							touchesMoved ( Touch* pTouch );
		KDbool							touchesEnded ( Touch* pTouch );

		KDvoid							setNum ( KDint nNum );
		KDvoid							setVisible ( KDbool isVisible );
		KDvoid							setIsSelect	( KDbool isSelect );
		KDvoid							setIsTouchEnable ( KDbool bIsTouchEnable );
		KDvoid							setPoint ( Point tPoint );
		KDvoid							setColor ( KDubyte r, KDubyte g, KDubyte b );

		inline KDbool					getVisible ( KDvoid ) {	return m_bIsVisible; }
		inline KDint					getNum ( KDvoid ) { return m_nNum; }

	private :

		cLabelAtlasCustom*				m_pLabelNum;
		cLabelAtlasCustom*				m_pLabelNumSelect;
		cLabelAtlasCustom*				m_pLabelNumPush;

		KDint							m_nNum;

		KDbool							m_bIsVisible;
		KDbool							m_bIsSelect;
		KDbool							m_bIsTouchEnable;

		Point							m_tPoint;
		Size							m_tSizeTouch;
		Rect							m_tRectTouch;

		KDbool							m_bIsTouch;
		KDint							m_nTouchID;
};

class MyRoomNode : public Scene
{
	public :
				 MyRoomNode ( KDvoid );
		virtual ~MyRoomNode ( KDvoid );

		static MyRoomNode*				create ( KDint nSelectUnitID );

	public :

		virtual KDbool					init ( KDint nSelectUnitID );

		KDvoid							releaseObject ( KDvoid );

		KDvoid							step ( KDfloat fDelta );

		KDbool							updateNodeChage ( KDdouble dLeftTIme );
		KDvoid							updateMoney ( KDvoid );
		KDvoid							updateUnit ( KDdouble dLeftTime );
		KDvoid							updateUnitExp ( KDvoid );
		KDvoid							updateUnitStatus ( KDvoid );
		KDvoid							updateCapsuleMode ( KDdouble dLeftTime );
		KDvoid							updateLevelUpEffect ( KDdouble dLeftTime );
		KDvoid							updateBuyingCheck ( KDvoid );
		
		KDvoid							selectUnitAndPage ( KDint nUnitID );
		KDvoid							selectUnit ( cUnit* pUnit );
		KDvoid							setExitTouchEnabled ( KDbool bIsEnabled );
		cUnit*							getUnit ( KDint nSlotIndex );
		KDvoid							setPage ( KDint nPage, KDbool bIsForcibly );
		KDvoid							setPageRange ( KDvoid );

		KDvoid							setUnitLevelUp ( cUnit* pUnit, bool bIsEffect ); 
		
		KDvoid							backCallback				( KDvoid );
		KDvoid							shop2Callback				( KDvoid );
		KDvoid							pageLeftCallback			( KDvoid );
		KDvoid							pageRightCallback			( KDvoid );
		KDvoid							socialCallback				( KDvoid );
		KDvoid							capsuleCallback				( KDvoid );
		KDvoid							wastebasketCallback			( KDvoid );
		KDvoid							battleCallback				( KDvoid );

		KDvoid							exitCallback				( Object* pSender );
		KDvoid							shopCallback				( Object* pSender );
		KDvoid							shopExitCallback			( Object* pSender );
		KDvoid							unitDeleteYesCallback		( Object* pSender );
		KDvoid							unitDeleteNoCallback		( Object* pSender );
		KDvoid							machineLeftCallback			( Object* pSender );
		KDvoid							machineRightCallback		( Object* pSender );
		KDvoid							machineHandleCallback		( Object* pSender );
		KDvoid							gainCallback				( Object* pSender );
		KDvoid							modeExitCallback			( Object* pSender );
		KDvoid							modeCasino100Callback		( Object* pSender );
		KDvoid							modeCasino1000Callback		( Object* pSender );
		KDvoid							modeArcadeEasyCallback		( Object* pSender );
		KDvoid							modeArcadeNormalCallback	( Object* pSender );
		KDvoid							modeArcadeHardCallback		( Object* pSender );
		KDvoid							modeSurvivalCallback		( Object* pSender );

		KDbool							onTouchBegan ( Touch* pTouch, Event* pEvent ); 
		KDvoid							onTouchMoved ( Touch* pTouch, Event* pEvent ); 
		KDvoid							onTouchEnded ( Touch* pTouch, Event* pEvent ); 

		KDvoid							touchBeganMachineMode ( Touch* pTouch );
		KDvoid							touchMovedMachineMode ( Touch* pTouch );
		KDvoid							touchEndedMachineMode ( Touch* pTouch );
		KDvoid							setMachineScroll ( KDfloat fScroll );
		KDvoid							buyCapsule ( KDint nSelectMachine );

	private :

		// touch
		KDint							m_nTidCount;
		std::list<sTouch>				m_aListTouch;

		// time
		KDdouble						m_dTickCount;
		KDdouble						m_dBeforeTickCount;

		// node change
		KDbool							m_bIsNodeChangeIn;
		KDbool							m_bIsNodeChangeOut;
		KDbool							m_bIsGoTitle;
		KDbool							m_bIsGoCasino100;
		KDbool							m_bIsGoCasino1000;
		KDbool							m_bIsGoArcadeEasy;
		KDbool							m_bIsGoArcadeNormal;
		KDbool							m_bIsGoArcadeHard;
		KDbool							m_bIsGoSurvival;
		cNodeChangeImgs*				m_pNodeChangeImg;

		// shop
		ShopNode*						m_pShopNode;

		// back img
		cSprite*						m_pSprBack;
		cSprite*						m_pSprBackLeft;
		cSprite*						m_pSprBackTop;
		cSprite*						m_pSprBackTopAdd;
		cSprite*						m_pSprBackBottom;
		cSprite*						m_pSprBack2;

		// cover color
		LayerColor*						m_pCoverColor;
		LayerColor*						m_pCoverColor2;

		// labels
		cLabels*						m_pLabels;

		// unit crate info
		cUnitCreateInfos*				m_pUnitCreateCoin1;
		cUnitCreateInfos*				m_pUnitCreateCoin2;
		cUnitCreateInfos*				m_pUnitCreateCoin3;
		cUnitCreateInfos*				m_pUnitCreateCoin4;
		cUnitCreateInfos*				m_pUnitCreateCoin5;
		cUnitCreateInfos*				m_pUnitCreateCoin6;
		cUnitCreateInfos*				m_pUnitCreateCoin7;
		cUnitCreateInfos*				m_pUnitCreateCoin8;
		cUnitCreateInfos*				m_pUnitCreateCoin9;
		cUnitCreateInfos*				m_pUnitCreateCoin10;
		cUnitCreateInfos*				m_pUnitCreateCoin11;
		cUnitCreateInfos*				m_pUnitCreateCoin12;
		cUnitCreateInfos*				m_pUnitCreateCoin13;
		cUnitCreateInfos*				m_pUnitCreatePremium1;
		cUnitCreateInfos*				m_pUnitCreatePremium2;
		cUnitCreateInfos*				m_pUnitCreatePremium3;
		cUnitCreateInfos*				m_pUnitCreatePremium4;

		// unit
		std::vector<cUnit*>				m_aUnits;
		cUnit*							m_pUnitSelect;
		cUnit*							m_pUnitDrag;
		KDint							m_nChangeSlotIndex;

		// btns
		Menu*							m_pBtnExit;
		Menu*							m_pBtnShop;
		Menu*							m_pBtnShopExit;
		Menu*							m_pBtnUnitDeleteYes;
		Menu*							m_pBtnUnitDeleteNo;
		Menu*							m_pBtnMachineLeft;
		Menu*							m_pBtnMachineRight;
		Menu*							m_pBtnMachineHandle;
		Menu*							m_pBtnGain;
		Menu*							m_pBtnModeExit;
		Menu*							m_pBtnModeCasino100;
		Menu*							m_pBtnModeCasino1000;
		Menu*							m_pBtnModeArcadeEasy;
		Menu*							m_pBtnModeArcadeNormal;
		Menu*							m_pBtnModeArcadeHard;
		Menu*							m_pBtnModeSurvival;
		cBtnSprite*						m_pBtnBack;
		cBtnSprite*						m_pBtnShop2;
		cBtnSprite*						m_pBtnPageLeft;
		cBtnSprite*						m_pBtnPageRight;
		cBtnDragSprite*					m_pBtnPageDragLeft;
		cBtnDragSprite*					m_pBtnPageDragRight;
		cBtnSprite*						m_pBtnSocial;
		cBtnSprite*						m_pBtnCapsule;
		cBtnSprite*						m_pBtnWastebasket;
		cBtnSprite*						m_pBtnBattle;
		std::vector<cBtnNum*>			m_aBtnPageNums;

		// UIs
		cLabelAtlasCustom*				m_pUIMoney;
		std::vector<cSprProp*>			m_aUIProps;
		cSprite*						m_pUISelMob;
		cSprite*						m_pUISelMobText;
		std::vector<cSprite*>			m_aUIUniqueType;
		std::vector<LabelTTF*>			m_aUIMobDetailPara;			//  [ 0 ] == hp,  [ 1 ] == atk,  [ 2 ] == def, [3] == agl, [4] == crt
		std::vector<cSprite*>			m_aUIUnitParaGauge;
		cSprite*						m_pUIUnitExpGaugeLarge;
		cSprite*						m_pUIUnitExpGaugeSmall;
		std::vector<cSprAni*>			m_aUISkillIcons;
		std::vector<LabelTTF*>			m_aUISkillStrs;
		cSprAni*						m_pUIClass;
		cSprAni*						m_pUIFriendship;
		LabelTTF*						m_pUIKillCount;
		cSprite*						m_pUINotCoinPopup;
		KDbool							m_bUIIsNotCoin;
		KDdouble						m_dUINotCoinLeftTime;
		cSprAnis*						m_pUIShopCursor;
		cSprite*						m_pUIUnitDeleteMsgBox;
		LabelTTF*						m_pUIUnitDeleteMsg;
		KDbool							m_bUIIsNotCoinView;
		cSprite*						m_pUIModeBack;
		cSprite*						m_pUIBtnSocialGray;
		cSprite*						m_pUIBtnBattleGray;
		cSprite*						m_pUIArcadeEasyClear;
		cSprite*						m_pUIArcadeNorClear;
		cSprite*						m_pUIArcadeHardClear;

		// level up
		std::list<cMyRoomLvlUpEffect*>	m_aLvUpEffects;
		std::list<cLevelPopup*>			m_aLvUpPopups;

		// particle
		cParticle*						m_pParticleUnitDelect;
		cParticle*						m_pParticleGetUnit;
		KDbool							m_bIsDeleteParticleStop;
		KDdouble						m_dDeleteParticleStopLeftTime;

		// machine
		std::vector<cMachine*>			m_aMachines;
		cSprite*						m_pMachineDragCursor;
		cSprite*						m_pMachineTouchMe;
		cSprite*						m_pSprCapsule1;
		cSprite*						m_pSprCapsule2;
		cSprite*						m_pSprCapsule3;
		cSprite*						m_pSprCapsule4;
		cSprite*						m_pSprCapsuleTouchMeFont;
		cSprite*						m_pUIMachineCoin;
		cSprAnis*						m_pUIMachineTimeCursor;
		cSprAnis*						m_pUIMachineNotCoinCursor;
		cSprite*						m_pUINotSlotPopup;
		cSprite*						m_pUIGettingBack;
		cSprProp*						m_pUIGettingProp;
		Point							m_tMachine;
		KDint							m_nViewMachineGap;
		KDfloat							m_fScrollMachine;
		KDfloat							m_fScrollMachineBackup;
		KDbool							m_bIsTouchMachine;
		KDbool							m_bIsTouchMachineTouch;
		KDbool							m_bIsTouchMachineDrag;
		Touch							m_tMachineTouchStartInfo;
		KDint							m_nSelectMachine;
		KDint							m_nSelectMachineBefore;
		KDdouble						m_dNotSlotLeftTime;
		KDint							m_nCapsuleRotate1;
		KDint							m_nCapsuleRotate2;
		KDint							m_nMachineHandleRotateCount;
		KDbool							m_bIsCapsuleTouch;
		KDbool							m_bIsCapsuleShake;
		KDfloat							m_fCapsuleShake;

		// page
		KDint							m_nPageMax;
		KDint							m_nViewFirstPageIndex;

		// point & rect
		Point							m_aUnit			[ _SLOT_IN_PAGE_COUNT ];
		Rect							m_aRectChange	[ _SLOT_IN_PAGE_COUNT ];
		Point							m_tDragStart;

		// switch
		KDbool							m_bIsUnitDeleting;
		KDbool							m_bIsUnitDeleteYes;
		KDdouble						m_dUnitDeleteEffectLeftTime;
		KDbool							m_bGetCapsuleMode;
		KDint							m_nCapsuleModeStep;
		KDint							m_nCapsuleModeStep2;
		KDdouble						m_dCapsuleModeLeftTime;
		KDbool							m_bCapsuleModeTouchMeAlphaUp;
		KDbool							m_bIsModePopup;
		KDbool							m_bIsBuyingDollarCapsule;

		// select unit
		KDdouble						m_dSelectUnitAniLeftTime;

		// touch unit
		KDbool							m_bIsTouchUnit;
		Touch							m_tUnitTouchStartInfo;
		Touch							m_tUnitTouchDragInfo;
};

#endif	// __MyRoomNode_h__