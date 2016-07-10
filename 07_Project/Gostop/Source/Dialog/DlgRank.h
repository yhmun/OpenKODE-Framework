/*
 *
 *      File            DlgRank.h
 *      Description     Rank dialog.
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

#ifndef __DlgRank_h__
#define __DlgRank_h__

//
//  "CDlgRank" dialog layer
//
class CDlgRank : public CDialog
{
	public :

		CREATE_FUNC ( CDlgRank );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );
		virtual const KDchar*  getTitle ( KDvoid );
        virtual CCPoint  getPosTitle ( KDvoid );

		KDvoid	onConnect ( CCObject* pSender );
};

//
//  "CDlgRankList" dialog layer
//
class CDlgRankList : public CDialog
{
	public :

		CREATE_FUNC ( CDlgRankList );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid  onEnterTransitionDidFinish ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );
		virtual const KDchar*  getTitle ( KDvoid );

		KDvoid  onTab ( CCObject* pSender );
		KDvoid  onRefresh ( KDvoid );

		KDvoid  refresh ( KDuint uIndex );

	protected :

		std::deque<CCLabelTTF*>		m_vList   [ 2 ];	
		CCLabelTTF*					m_pMyInfo [ 2 ];
};

//
//  "CDlgRegister" dialog layer
//
class CDlgRegister : public CDlgRequest
{
	public :

		CREATE_FUNC ( CDlgRegister );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

		virtual KDvoid  onYes ( CCObject* pSender );
};

//
//  "CDlgNickName" dialog layer
//
class CDlgNickName : public CDlgConfirm, public CCIMEDelegate, public CCTextFieldDelegate
{
	public :

		CREATE_FUNC ( CDlgNickName );

	protected :

		KDbool  init ( KDvoid );

        virtual KDvoid  onExit ( KDvoid );
    
		virtual const KDchar*  getTitle ( KDvoid );

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		virtual KDbool  onTextFieldAttachWithIME ( CCTextFieldTTF* pSender );
		virtual KDbool  onTextFieldDetachWithIME ( CCTextFieldTTF* pSender );

		virtual KDbool  onTextFieldInsertText     ( CCTextFieldTTF* pSender, const KDchar* text, KDsize nLen );
		virtual KDbool  onTextFieldDeleteBackward ( CCTextFieldTTF* pSender, const KDchar* text, KDsize nLen );

		virtual KDvoid  onConfirm ( CCObject* pSender );

	protected :

		CCTextFieldTTF*			m_pTextField;
};

//
//  "CDlgNickName2" dialog layer
//
class CDlgNickName2 : public CDlgConfirm
{
	public :

		CREATE_FUNC ( CDlgNickName2 );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );
};


#endif
