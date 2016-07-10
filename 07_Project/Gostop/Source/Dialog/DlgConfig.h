/*
 *
 *      File            DlgConfig.h
 *      Description     Config dialog.
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

#ifndef __DlgConfig_h__
#define __DlgConfig_h__

//
//  "CDlgConfig" dialog layer
//
class CDlgConfig : public CDialog
{
	public :

		CREATE_FUNC ( CDlgConfig );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid  onExit ( KDvoid );

		virtual const KDchar*  getPathBG ( KDvoid );
		virtual const KDchar*  getTitle ( KDvoid );
		virtual CCPoint        getPosTitle ( KDvoid );

		KDvoid  refresh  ( KDvoid );

		KDvoid  onSpeed  ( CCObject* pSender );
		KDvoid  onToggle ( CCObject* pSender );
		KDvoid  onReset  ( CCObject* pSender );

	protected :

		KDbool					m_bRefresh;
		CCMenuRadio*			m_pRadio;
		CCMenuItemToggle*		m_pToggles [ 3 ];
};

//
//  "CDlgResetGame" dialog layer
//
class CDlgResetGame : public CDlgRequest
{
	public :
    
        CREATE_FUNC ( CDlgResetGame );
    
	protected :
    
        KDbool  init ( KDvoid );
    
        virtual const KDchar*  getTitle ( KDvoid );
    
        virtual KDvoid  onYes ( CCObject* pSender );
};

#endif
