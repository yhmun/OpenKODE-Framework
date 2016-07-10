/*
 *
 *      File            DlgHelp.h
 *      Description     Help dialog.
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

#ifndef __DlgHelp_h__
#define __DlgHelp_h__

//
//  "CDlgHelp" dialog layer
//
class CDlgHelp : public CDlgMulti
{
	public :

		CREATE_FUNC ( CDlgHelp );

	protected :

		virtual KDuint  getCount ( KDvoid );

		virtual const KDchar*  getTitle ( KDvoid );

		virtual CCNode*  getContent ( KDvoid );
};

#endif
