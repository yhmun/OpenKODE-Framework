/* --------------------------------------------------------------------------
 *
 *      File            ScrManager.h
 *      Description     Scene Manager 
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __ScrManager_h__
#define __ScrManager_h__

enum EScreen
{
	eScrSplash			,
	eScrIntro			,
	eScrMain			,
	eScrLoading			,
	eScrGame			,
};

enum ETransition
{
	eTranNull			,
	eTranBlackFade		,
	eTranMoveInL		,
	eTranMoveInR		,
	eTranMoveInT		,
	eTranMoveInB		,
	eTranSlideInL		,
	eTranSlideInR		,
	eTranSlideInT		,
	eTranSlideInB		,
	eTranShrinkGrow		,
    eTranProgressInOut  ,
    eTranProgressOutIn  ,
    eTranPageForward    ,
    eTranPageBackward   ,
};

class CScrManager
{
	protected :

		 CScrManager ( KDvoid );
		~CScrManager ( KDvoid );

	public :

		static KDvoid  create  ( KDvoid );
		static KDvoid  release ( KDvoid );

	public :

		KDvoid    setScreen ( EScreen eSceen, ETransition eTransition = eTranNull, KDfloat fDuration = 0.3f );

        CCLayer*  getScreen   ( KDvoid );
        EScreen   getScreenID ( KDvoid );
    
	private :

		CCLayer*	m_pScreen;
        EScreen     m_eScreen;
};

extern CScrManager*  g_pScrMgr;

#endif