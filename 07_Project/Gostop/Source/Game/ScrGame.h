/*
 *
 *      File            ScrGame.h
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

#ifndef __ScrGame_h__
#define __ScrGame_h__

#include "Card.h"

enum EType
{
	eType_Sound				,
	eType_Vibrate			,
	eType_Effect			,
	eType_Talk				,
	eType_Dialog			,
	eType_GameStart			,
	eType_Shuffle			,
	eType_DealCard			,
	eType_ChongTong			,
	eType_Mission			,
	eType_Result			,
	eType_PutPlayer			,
	eType_PutShuffled		,
	eType_Obtain			,
	eType_Score				,
	eType_Gostop			,
	eType_EndTurn			,
	eType_CardAttach		,
	eType_CardToGround		,
	eType_CardToPlayer1     ,
	eType_CardToPlayer2     ,
	eType_CardToObtain		,
	eType_ReorderPlayer1	,
	eType_ReorderPlayer2	,
	eType_RefreshObtainable	,
	eType_RefreshObtain		,
	eType_RefreshCount		,
	eType_RefreshScore		,
	eType_RefreshTimes		,
	eType_RefreshCash		,
};

typedef struct TEvent
{
	EType		eType;
	KDuint		uParam1;
	KDuint		uParam2;
} TEvent;

typedef std::list<TEvent>	TEventList;

typedef struct 
{
	KDuint      uMax;
	KDuint		uScore;		
	KDuint      uTimes;
	KDuint      uBuck;
	KDuint		uGage;
	KDuint		uGo;
	KDuint		uGwang;
	KDuint		u10Pi;
	KDuint		u5Pi;
	KDuint		u1Pi;
	KDuint		uGodori;
	KDuint		uChoDan;
	KDuint		uHongDan;
	KDuint		uChungDan;
	KDuint      uBGwang;
	KDuint		uShake;
	KDuint		uTotalScore;
	KDuint      uTotalTimes;
	KDuint		uMission;
	KDuint		uSkill;
	KDbool		bGwangBak;
	KDbool		bPiBak;
	KDbool		bMungBak;
	KDbool		bGoBak;
	KDbool		bChongTong;
	KDbool		bChongTongGo;
} TScore;

//
//  Main layer of "CScrGame" Scene.
//
class CScrGame : public CCLayer
{
	public :

		CREATE_FUNC ( CScrGame );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid  onEnterTransitionDidFinish ( KDvoid );

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );	

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

	public :

		KDvoid	  postEvent ( EType eType, KDuint uParam1 = KDUINT_MAX, KDuint uParam2 = KDUINT_MAX );
		KDvoid	  postEvent ( const TEvent& tEvent );
		KDvoid	  waitEvent ( KDvoid );    

	protected :

		KDvoid	  onEvent   ( KDvoid );

		KDvoid    onVibrate ( KDvoid );
		KDvoid    onSound   ( EPath ePath );
		KDvoid	  onEffect  ( EPath ePath, KDuint uNumber );
		KDvoid	  onTalk    ( ETalk eTalk );
		KDvoid	  onDialog  ( EDialog eDialog );
		KDvoid    onShine   ( KDfloat fTime );
		KDvoid	  onMenu    ( CCObject* pSender );

		KDvoid	  onStart       ( KDvoid );
		KDvoid    onShuffle     ( KDvoid );
		KDvoid    onDealCard    ( KDvoid );
		KDvoid    onChongTong   ( KDvoid );
		KDvoid    onMission     ( KDvoid );
		KDvoid    onResult	    ( KDvoid );
		KDvoid    onPutPlayer   ( KDvoid );
		KDvoid	  onPutShuffled ( KDvoid );
		KDvoid    onObtain      ( KDvoid );
		KDvoid    onScore       ( KDvoid );
		KDvoid	  onGostop		( KDvoid );
		KDvoid	  onEndTurn	    ( KDvoid );

		KDvoid    onCardAttach	  ( KDuint uIndex );
		KDvoid    onCardToGround  ( KDuint uNumber );
		KDvoid    onCardToPlayer1 ( KDuint uNumber );
		KDvoid    onCardToPlayer2 ( KDuint uNumber );
		KDvoid    onCardToObtain  ( KDuint uNumber );

		KDvoid    onReorderPlayer1 ( KDvoid );
		KDvoid    onReorderPlayer2 ( KDvoid );	

		KDvoid    onRefreshObtainable ( KDvoid );
		KDvoid    onRefreshTextCash   ( KDvoid );
		KDvoid    onRefreshTextCount  ( KDvoid );
		KDvoid    onRefreshTextScore  ( KDvoid );
		KDvoid    onRefreshTextTimes  ( KDvoid );
		KDvoid    onRefreshTextObtain ( KDvoid );

	public :

		KDvoid    keepChongTong ( KDuint uMonth );
		KDbool    checkMission  ( KDvoid );

		KDvoid    select  ( CCard* pCard );
		KDvoid    putCard ( CCard* pCard );

		KDbool    steal1Pi	 ( KDuint uCount );
		KDbool	  steal2Pi	 ( KDuint uCount );
		KDbool	  stealGwang ( KDuint uCount );

		KDvoid	  swapCards  ( KDvoid );
		
	public :

		KDuint    getTurn ( KDvoid );
		KDvoid    setTurn ( KDuint uTurn );

		TScore*	  getTotal  ( KDvoid );
		TScore*   getScore  ( KDvoid );
		TScore*   getScore  ( KDuint uIndex );
		KDvoid    setGostop ( KDbool bGo );

		KDbool	  isSkillAvailable ( KDuint uIndex );
		KDvoid	  useSkill ( KDuint uIndex );

	protected :

		KDbool    aiGukJin  ( KDvoid );
		KDbool    aiGoStop  ( KDvoid );
		KDbool    aiPush    ( KDvoid );
		CCard*	  aiPutCard ( KDvoid );
		CCard*	  aiSelect  ( CCard* pCard1, CCard* pCard2 );

		KDuint    findGround  ( KDuint uMonth );
		KDuint    countGround ( KDuint uIndex );  // Without Bonus

		CCardList getObtainable  ( KDvoid );
		CCard*    getObtainable  ( CCard* pCard );
		CCard*    getPossible    ( KDuint uNum1, KDuint uNum2, KDuint uNum3 );

		CCard*	  getBonusTiming ( KDvoid );
		CCard*    getBonus		 ( KDvoid );
		CCard*    getBomb		 ( KDvoid );
		CCard*    getHeading     ( KDvoid );
		CCard*	  getZock		 ( KDvoid );

	protected :

		TEventList			m_lEvent;
		TScore				m_tScore [ 2 ];
		KDuint				m_uSkillAvailable [ COUNT_SKILL ];
		KDuint				m_uGageCondition;

		KDuint              m_uRound;
		KDuint				m_uTurn;
		KDbool				m_bMission;
		KDuint				m_uMissionTimes;
		KDuint				m_uPrevGround;
		KDuint				m_uObtainGround [ 2 ];
		KDbool				m_bObtainSelect [ 2 ];
		KDuint				m_uObtainSteal;
		CCard*				m_pSelect;

		CCardList			m_lCard;
		CCardList			m_lShuffled;
		CCardList			m_lObtainable;
		CCardList			m_lPlayer [ 2 ];
		CCardList			m_lObtain [ 2 ][ 4 ];
		CCardList			m_lGround [ 12 ];
		CCardList			m_lMission;
		
		CText*				m_pTxtObtain [ 2 ][ 4 ];
		CText*				m_pTxtCount  [ 2 ][ 3 ];
		CText*				m_pTxtScore  [ 2 ];
		CText*				m_pTxtTimes  [ 2 ];
		CText*				m_pTxtCash   [ 2 ];

		CCSize				m_tWinSize;
		CCSize				m_tSize [ 4 ];
		CCPoint				m_tPosition;
};

#endif
