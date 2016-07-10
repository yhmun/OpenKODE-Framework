/*
 *
 *      File            ScrCartoon.h
 *      Description     Cartoon scene.
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

#ifndef __ScrCartoon_h__
#define __ScrCartoon_h__

//
//  Main layer of "CScrCartoon" Scene.
//
class CScrCartoon : public CCLayer
{
	public :

		CREATE_FUNC ( CScrCartoon );

	protected :

		KDbool  init ( KDvoid );

		virtual KDvoid ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

	protected :

		KDuint		m_uIndex;
		KDuint		m_uCount;
};

#endif
