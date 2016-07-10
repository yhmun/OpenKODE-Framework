/*
 *
 *      File            ScrVersus.h
 *      Description     Versus scene.
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

#ifndef __ScrVersus_h__
#define __ScrVersus_h__

//
//  Main layer of "CScrVersus" Scene.
//
class CScrVersus : public CCLayer
{
	public :

		CREATE_FUNC ( CScrVersus );

	protected :

		KDbool  init ( KDvoid );

		KDvoid  onNextScene ( KDvoid );
};

#endif
