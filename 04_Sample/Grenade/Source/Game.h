/* --------------------------------------------------------------------------
 *
 *      File            Game.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2011 Robert Blackwood 01/20/11
 *
 *         Mobile Bros. All rights reserved.
 *
 *         http://www.mobile-bros.com  
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

#ifndef __Game_h__
#define __Game_h__

#include "Bomb.h"

// Collision Defines
#define kEnemyCollisionType		1
#define kGroundCollisionType	2
#define kBlockCollisionType		3
#define kBombCollisionType		4

// tags
#define GAME_TAG				1

class Game : public CCLayer
{
	public :

		virtual ~Game ( KDvoid );

		CREATE_FUNC ( Game );

	public :

		virtual KDbool  init ( KDvoid );  

		virtual KDvoid  onExit ( KDvoid );

	public :

		CC_SYNTHESIZE ( CCSpaceManager*, m_pSpaceManager, SpaceManager );

		KDvoid  enemyKilled ( KDvoid );

	private :

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid  ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid  ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );		

		KDbool  handleEnemyCollision ( CollisionMoment eMoment, cpArbiter* pArbiter, cpSpace* pSpace );
		KDbool  handleBombCollision  ( CollisionMoment eMoment, cpArbiter* pArbiter, cpSpace* pSpace );

		KDvoid  createTriangleAt   ( cpVect tPos, cpFloat fSize , cpFloat fMass );
		KDvoid  createBlockAt      ( cpVect tPos, cpFloat fWidth, cpFloat fHeight, KDfloat fMass );
		KDvoid  createTriPillarsAt ( cpVect tPos, cpFloat fWidth, cpFloat fHeight );

		KDvoid  setupShapes		( KDvoid );
		KDvoid  setupEnemies	( KDvoid );
		KDvoid  setupBackground ( KDvoid );
		KDvoid  setupBombs		( KDvoid );
		KDvoid  setupNextBomb	( KDvoid );
		KDvoid  setupRestart	( KDvoid );

		KDvoid  restart ( CCObject* pSender );

	private :

		Bomb*					m_pCurBomb;
		CCArray*				m_pBombs;
		KDint					m_nEnemiesLeft;
};

#endif  // __Game_h__