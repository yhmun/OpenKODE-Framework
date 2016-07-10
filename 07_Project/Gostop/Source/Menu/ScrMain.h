/*
 *
 *      File            ScrMain.h
 *      Description     Main scene.
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

#ifndef __ScrMain_h__
#define __ScrMain_h__

//
//  Main layer of "CScrMain" Scene.
//
class CScrMain : public CCLayer
{
	public :

		CREATE_FUNC ( CScrMain );

	protected :

		KDbool  init ( KDvoid );

		KDvoid  onQuit   ( CCObject* pSender );
		KDvoid  onConfig ( CCObject* pSender );

		KDvoid  onGame ( CCObject* pSender );
		KDvoid  onRank ( CCObject* pSender );
		KDvoid  onHelp ( CCObject* pSender );
};

#endif
