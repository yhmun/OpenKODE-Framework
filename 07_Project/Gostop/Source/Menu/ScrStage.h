/*
 *
 *      File            ScrStage.h
 *      Description     Select stage scene.
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

#ifndef __ScrStage_h__
#define __ScrStage_h__

//
//  Main layer of "CScrStage" Scene.
//
class CScrStage : public CCLayer
{
	public :

		CREATE_FUNC ( CScrStage );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid  onEnterTransitionDidFinish ( KDvoid );

		KDvoid  onInfo ( CCObject* pSender );
		KDvoid  onShop ( CCObject* pSender );
		KDvoid  onMain ( CCObject* pSender );

		KDvoid  onSelect ( CCObject* pSender );

	public :

		KDvoid  onCheck ( KDvoid );
};

#endif
