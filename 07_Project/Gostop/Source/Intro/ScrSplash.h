/*
 *
 *      File            ScrSplash.h
 *      Description     Splash scene.
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

#ifndef __ScrSplash_h__
#define __ScrSplash_h__

//
//  Main layer of "CScrSplash" Scene.
//
class CScrSplash : public CCLayer
{
	public :

		CREATE_FUNC ( CScrSplash );

	protected :

		KDbool  init ( KDvoid );

		KDvoid  onActive ( KDvoid );
};

#endif
