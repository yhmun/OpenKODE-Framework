/*
 *
 *      File            ScrLadder.h
 *      Description     Ladder scene.
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

#ifndef __ScrLadder_h__
#define __ScrLadder_h__

//
//  Main layer of "CScrLadder" Scene.
//
class CScrLadder : public CCLayer
{
	public :

		CREATE_FUNC ( CScrLadder );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid  onExit ( KDvoid );

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );	

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid  onStart ( KDvoid );
		KDvoid  onMove  ( KDvoid );
		KDvoid  onEnd   ( KDvoid );

		KDvoid  onSelect ( CCObject* pSender );

	protected :

		KDuint					m_uSelect;
		CCSprite*				m_pFace;
		CCSprite*				m_pNotice;
		CCMenuItemImage*		m_pButtons [ 5 ];
		std::vector<CCNode*>	m_vLines;
};



#endif
