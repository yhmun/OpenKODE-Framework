/* --------------------------------------------------------------------------
 *
 *      File            GameScene.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org  
 *         https://code.google.com/p/tweejump-cocos2dx
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __GameScene_h__
#define __GameScene_h__

#include "MainScene.h"

class GameScene : public MainScene
{
	public :

		CREATE_FUNC ( GameScene );

	public :

		virtual  KDbool init ( KDvoid );

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid  ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		virtual KDvoid  didAccelerate ( CCAcceleration* pAccelerationValue );
	
	protected :

		KDvoid	initPlatforms ( KDvoid );
		KDvoid	initPlatform  ( KDvoid );

		KDvoid	startGame ( KDvoid );

		KDvoid	resetPlatforms ( KDvoid );
		KDvoid	resetPlatform  ( KDvoid );

		KDvoid	resetBird  ( KDvoid );
		KDvoid	resetBonus ( KDvoid );

		KDvoid	step ( KDfloat fDelta );

		KDvoid	jump ( KDvoid );

		KDvoid	showHighscores ( KDvoid );

	private :

		CCPoint			m_tBirdPos;
		ccVertex2F		m_tBirdVel;
		ccVertex2F		m_tBirdAcc;	

		KDfloat			m_fCurrentPlatformY;
		KDint			m_nCurrentPlatformTag;
		KDfloat			m_fCurrentMaxPlatformStep;
		KDint			m_nCurrentBonusPlatformIndex;
		KDint			m_nCurrentBonusType;
		KDint			m_nPlatformCount;
		
		KDbool			m_bGameSuspended;
		KDbool			m_bBirdLookingRight;
		
		KDint			m_nScore;
};

#endif	// __GameScene_h__

