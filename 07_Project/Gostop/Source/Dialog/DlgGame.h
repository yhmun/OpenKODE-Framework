/*
 *
 *      File            DlgGame.h
 *      Description     Game dialogs.
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

#ifndef __DlgGame_h__
#define __DlgGame_h__

class CCard;
class CCardList;

//
//  "CDlgChongTong1" dialog layer
//
class CDlgChongTong1 : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgChongTong1 );

	protected :
    
        KDbool  init ( KDvoid );

		virtual KDvoid  onRemove ( KDvoid );

	public :

		static  KDuint	s_uMonth;
};

//
//  "CDlgChongTong2" dialog layer
//
class CDlgChongTong2 : public CDlgGame
{
	public :
    
		CREATE_FUNC ( CDlgChongTong2 );
    
	protected :
    
        KDbool  init ( KDvoid );

		virtual EAction  getShowAction ( KDvoid );

		KDvoid  onGo   ( CCObject* pSender );
		KDvoid  onStop ( CCObject* pSender );

	public :

		static  KDuint	s_uMonth;
};

//
//  "CDlgShake" dialog layer
//
class CDlgShake : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgShake );

	protected :
    
        KDbool  init ( KDvoid );

		virtual KDvoid  onRemove ( KDvoid );

	public :

		static  CCardList*	s_pList;
		static  CCard*		s_pCard;
};

//
//  "CDlgBuck" dialog layer
//
class CDlgBuck : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgBuck );
    
	protected :
    
        KDbool  init ( KDvoid );
    
		virtual KDvoid  onRemove ( KDvoid );

	protected :

		KDbool				m_bBankrupted;

	public :

		static	KDuint		s_uTurn;
		static  KDuint		s_uCount;
		static	KDbool		s_bCombo;
};

//
//  "CDlgNagari" dialog layer
//
class CDlgNagari : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgNagari );
    
	protected :
    
        KDbool  init ( KDvoid );

		virtual KDvoid  onRemove ( KDvoid );
};

//
//  "CDlgGukJin" dialog layer
//
class CDlgGukJin : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgGukJin );
    
	protected :
    
        KDbool  init ( KDvoid );

		virtual EAction  getShowAction ( KDvoid );

		KDvoid  onPoint10 ( CCObject* pSender );
		KDvoid  onPoint2  ( CCObject* pSender );

	public :

		static  CCard*		s_pCard;
};

//
//  "CDlgGostop" dialog layer
//
class CDlgGostop : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgGostop );
    
	protected :
    
        KDbool  init ( KDvoid );

		virtual EAction  getShowAction ( KDvoid );

		KDvoid  onGo   ( CCObject* pSender );
		KDvoid  onStop ( CCObject* pSender );
};

//
//  "CDlgCard" dialog layer
//
class CDlgCard : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgCard );

	protected :
    
        KDbool  init ( KDvoid );

		virtual EAction  getShowAction ( KDvoid );

		KDvoid  onSelect ( CCObject* pSender );

	public :

		static  CCardList*	s_pList;
		static  CCard*		s_pCard1;
		static  CCard*		s_pCard2;
};

//
//  "CDlgTimes" dialog layer
//
class CDlgTimes : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgTimes );
    
	protected :
    
        KDbool  init ( KDvoid );

		virtual KDvoid  onRemove ( KDvoid );
};

//
//  "CDlgPushPlayer" dialog layer
//
class CDlgPushPlayer : public CDlgRequest
{
	public :
    
        CREATE_FUNC ( CDlgPushPlayer );
    
	protected :
    
        KDbool  init ( KDvoid );

        virtual const KDchar*  getTitle ( KDvoid );
    
		virtual KDvoid  onYes ( CCObject* pSender );
		virtual KDvoid  onNo  ( CCObject* pSender );
};

//
//  "CDlgPushEnemy" dialog layer
//
class CDlgPushEnemy : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgPushEnemy );
    
	protected :
    
        KDbool  init ( KDvoid );

		virtual KDvoid  onRemove ( KDvoid );
};

//
//  "CDlgMission" dialog layer
//
class CDlgMission : public CDlgGame
{
	public :
    
        CREATE_FUNC ( CDlgMission );
    
	protected :
    
        KDbool  init ( KDvoid );

		virtual KDvoid  onRemove ( KDvoid );

	public :

		static KDuint		s_uIndex;
};

//
//  "CDlgGage" dialog layer
//
class CDlgGage : public CDlgGame2
{
	public :
	
		CREATE_FUNC ( CDlgGage );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );	

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		virtual KDvoid  onRemove ( KDvoid );

	public :

		virtual KDbool  hasKeyHide ( KDvoid );

	protected :

		CCSprite*			m_pBar;
		CCSprite*			m_pGage;

	public :

		static  CCard*		s_pCard;
		static	KDfloat		s_fTiming;
};

//
//  "CDlgSkill" dialog layer
//
class CDlgSkill : public CDlgRequest
{
	public :
    
        CREATE_FUNC ( CDlgSkill );
    
	protected :
    
        KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

		virtual KDvoid  onRemove ( KDvoid );

		virtual KDvoid  onYes ( CCObject* pSender );
		virtual KDvoid  onNo  ( CCObject* pSender );

	protected :

		KDbool				m_bConfirm;

	public :

		static  KDuint		s_uIndex;
};

//
//  "CDlgUseSkill" dialog layer
//
class CDlgUseSkill : public CDlgGame2
{
	public :

		CREATE_FUNC ( CDlgUseSkill );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );	

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid  onMenu ( CCObject* pSender ); 
};

//
//  "CDlgGameMenu" dialog layer
//
class CDlgGameMenu : public CDlgGame2
{
	public :

		CREATE_FUNC ( CDlgGameMenu );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );	

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid  onMenu ( CCObject* pSender ); 
};

#endif
