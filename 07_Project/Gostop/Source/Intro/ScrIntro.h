/*
 *
 *      File            ScrIntro.h
 *      Description     Intro scene.
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

#ifndef __ScrIntro_h__
#define __ScrIntro_h__

//
//  Main layer of "CScrIntro" Scene.
//
class CScrIntro : public CCLayer
{
	public :

		CREATE_FUNC ( CScrIntro );

	protected :

		KDbool  init ( KDvoid );

		KDvoid  onNextScene ( KDvoid );
};

#endif
