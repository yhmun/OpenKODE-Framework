/*
 *
 *      File            DlgResult.h
 *      Description     Result dialog.
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

#ifndef __DlgResult_h__
#define __DlgResult_h__

//
//  "CDlgResult" dialog layer
//
class CDlgResult : public CDlgGame2
{
	public :

		CREATE_FUNC ( CDlgResult );

	protected :

		KDbool  init ( KDvoid );

	public :

		virtual KDbool  hasKeyHide ( KDvoid );

	protected :

		std::string  getDetail ( KDvoid );

		KDvoid  onContinue ( CCObject* pSender );
		KDvoid  onPush     ( CCObject* pSender );
		KDvoid  onStop     ( CCObject* pSender );
};

#endif
