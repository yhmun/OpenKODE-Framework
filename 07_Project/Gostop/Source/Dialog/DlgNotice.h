/*
 *
 *      File            DlgNotice.h
 *      Description     Notice dialog.
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

#ifndef __DlgNotice_h__
#define __DlgNotice_h__

//
//  "CDlgActivate" dialog layer
//
class CDlgActivate : public CDlgRequest
{
	public :

		CREATE_FUNC ( CDlgActivate );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );
		virtual KDbool  hasHide ( KDvoid );
		virtual KDbool	hasKeyHide ( KDvoid );

        virtual KDvoid  onRemove ( KDvoid );
		virtual KDvoid  onYes ( CCObject* pSender );
		virtual KDvoid  onNo  ( CCObject* pSender );
    
    protected :
    
        KDbool      m_bConfirm;
};

//
//  "CDlgActivate2" dialog layer
//
class CDlgActivate2 : public CDlgRequest
{
	public :

		CREATE_FUNC ( CDlgActivate2 );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

		virtual KDvoid  onYes ( CCObject* pSender );
};

//
//  "CDlgSMS" dialog layer
//
class CDlgSMS : public CDlgRequest
{
	public :

		CREATE_FUNC ( CDlgSMS );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

        virtual	KDvoid  onRemove ( KDvoid );
    
		virtual KDvoid  onYes ( CCObject* pSender );
};

//
//  "CDlgStart" dialog layer
//
class CDlgStart : public CDlgConfirm
{
	public :

		CREATE_FUNC ( CDlgStart );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

		virtual KDvoid  onRemove ( KDvoid );
};

//
//  "CDlgNetworkFail" dialog layer
//
class CDlgNetworkFail : public CDlgConfirm
{
	public :
    
        CREATE_FUNC ( CDlgNetworkFail );
    
	protected :
    
        KDbool  init ( KDvoid );
    
        virtual const KDchar*  getTitle ( KDvoid );
};

//
//  "CDlgQuit" dialog layer
//
class CDlgQuit : public CDlgRequest
{
	public :

		CREATE_FUNC ( CDlgQuit );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

		virtual KDvoid  onYes ( CCObject* pSender );
};

//
//  "CDlgLadder" dialog layer
//
class CDlgLadder : public CDlgConfirm
{
	public :

		CREATE_FUNC ( CDlgLadder );

	protected :

		KDbool  init ( KDvoid );

		virtual	KDvoid  onRemove ( KDvoid );

	public :

		static	KDuint		s_uPercent;
};

//
//  "CDlgBankrupt" dialog layer
//
class CDlgBankrupt : public CDialog
{
	public :

		CREATE_FUNC ( CDlgBankrupt );

	protected :

		KDbool  init ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

        virtual CCPoint  getPosTitle ( KDvoid );

		KDvoid  onGame ( CCObject* pSender );
};

//
//  "CDlgLevelUp" dialog layer
//
class CDlgLevelUp : public CDlgConfirm
{
	public :

		CREATE_FUNC ( CDlgLevelUp );

	protected :

		KDbool  init ( KDvoid );

		virtual	KDvoid  onRemove ( KDvoid );
};

//
//  "CDlgReward" dialog layer
//
class CDlgReward : public CDlgConfirm
{
	public :

		CREATE_FUNC ( CDlgReward );

	protected :

		KDbool  init ( KDvoid );

		virtual	KDvoid  onRemove ( KDvoid );

	public :

		static	KDuint		s_uIndex;
};

#endif
