/*
 *
 *      File            ScrManager.h
 *      Description     Scene Manager 
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

#ifndef __ScrManager_h__
#define __ScrManager_h__

enum EScene
{
	eScrSplash			,		
	eScrIntro			,
	eScrMain			,
	eScrStage			,
	eScrCartoon			,
	eScrVersus			,
	eScrGame			,
	eScrLadder			,
};

enum EDialog
{
	eDlgActivate		,
	eDlgActivate2		,
	eDlgSMS				,
	eDlgStart			,
    eDlgNetworkFail     ,
	eDlgQuit			,
	eDlgConfig			,
	eDlgResetGame		,
	eDlgRank			,
	eDlgRankList		,
	eDlgRegister		,
	eDlgNickName		,
	eDlgNickName2		,
	eDlgHelp			,
    eDlgInfo            ,
    eDlgShop            ,
	eDlgShopCoin		,
	eDlgShopCash		,
	eDlgShopSkill		,
	eDlgBuyRequest		,
	eDlgBuyDone			,
	eDlgBuyFail			,
	eDlgNotEnough		,
	eDlgBankrupt		,
	eDlgLevelUp			,
	eDlgReward			,
	eDlgResult			,
    eDlgChongTong1      ,
    eDlgChongTong2      ,
	eDlgShake			,
    eDlgBuck            ,
    eDlgNagari          ,
    eDlgGukJin          ,
    eDlgSkill           ,
    eDlgGostop          ,
    eDlgCard            ,
    eDlgTimes           ,
	eDlgPushPlayer		,
    eDlgPushEnemy       ,
    eDlgMission         ,
	eDlgGage			,
	eDlgGameMenu		,
	eDlgUseSkill		,
	eDlgLadder			,
};

enum ETalker
{
	eTalkerBM			,
	eTalkerRD			,
	eTalkerEnemy		,
};

enum EState
{
	eStatNormal			,
	eStatAngry			,
	eStatSad			,
	eStatSmile			,
};

enum ETransition
{
	eTransNull			,
	eTransBlackFade		,
	eTransMoveInL		,
	eTransMoveInR		,
	eTransMoveInT		,
	eTransMoveInB		,
	eTransSlideInL		,
	eTransSlideInR		,
	eTransSlideInT		,
	eTransSlideInB		,
	eTransShrinkGrow	,
};

enum EAction
{
	eActionNull			,
	eActionMoveInT		,
	eActionMoveOutT		,
	eActionAutoHide		,
};

enum 
{
	eTagDialog			= 1000,
	eTagDialogBG		= 1001,
	eTagTalk			= 2000,
	eTagTalkBG			= 2001,
	eTagRequest			= 3000,
	eTagRequestBG		= 3001,
};

//
// Screen Manager ( Scene )
//
class CScrManager
{
	protected :

		 CScrManager ( KDvoid );
		~CScrManager ( KDvoid );

	public :

		static KDvoid  create ( KDvoid );
		static KDvoid  release ( KDvoid );

		CCLayer*  setScene ( EScene eID, ETransition eTrans = eTransNull, KDfloat fDuration = 0.3f );
		CCLayer*  getScene ( KDvoid );
		EScene	  getSceneID ( KDvoid );

		CCLayer*  setDialog ( EDialog eID );
		CCLayer*  getDialog ( KDuint uOrder );

		KDvoid    showEffect ( EPath eID, CCNode* pParent = KD_NULL );
		KDvoid    showEffect ( EPath eID, KDuint uIndex, CCNode* pParent = KD_NULL );

        KDvoid    showTalk ( ETalk eID );
		KDvoid	  showTalk ( ETalk eID, KDuint uIndex );
    
		CCLayer*  getTopDialog ( KDvoid );
		KDint     getTopDialogPrioty ( KDvoid );

		KDvoid    pushDialog ( CCLayer* pLayer );
		KDvoid	  popDialog  ( KDvoid );

	protected :

		CCLayer*					m_pScene;
		EScene						m_eSceneID;
		std::vector<CCLayer*>		m_vDialog;
};

extern CScrManager*  g_pScrManager;

//
// "CLyrTalk" sub layer of Running Scene.
//
class CLyrTalk : public CCLayer
{
	public :

		 
		CREATE_FUNC ( CLyrTalk );

	protected :

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );	

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		virtual KDvoid  keyBackClicked ( KDvoid );

	public :

		KDvoid  hide ( KDvoid );

		KDvoid  setTalker ( ETalker eID, EState eState, const KDchar* szTalk, KDfloat fLive );
};

#endif
