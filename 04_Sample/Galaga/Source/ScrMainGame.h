/* --------------------------------------------------------------------------
 *
 *      File            ScrMainGame.h
 *      Description     GameState
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __ScrMainGame_h__
#define __ScrMainGame_h__

enum EType
{
	ePlayer		= 0	,
	eEnemy1			,
	eEnemy2			,
};

class CScrMainGame : public CCLayer
{
	public :
				 CScrMainGame ( KDvoid );
		virtual ~CScrMainGame ( KDvoid );

		CREATE_FUNC ( CScrMainGame );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onEnterTransitionDidFinish ( KDvoid );

		virtual KDvoid		ccTouchesBegan		( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesMoved		( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesEnded		( CCSet* pTouches, CCEvent* pEvent );

	protected :

		CCSprite*			createPlayer		( const CCPoint& tPoint );
		CCSprite*			createEnemy			( const CCPoint& tPoint, EType eType );
		CCSprite*			createExplosion		( const CCPoint& tPoint, EType eType );

		CCSprite*			createMissile		( const CCPoint& tPoint, EType eType );
		KDvoid				removeMissile		( CCNode* pNode );

		KDvoid				onGameLogic			( KDfloat fDelta );

		KDvoid				onRebornPlayer		( KDfloat fDelta );

		KDvoid				onMovingPlayer		( KDfloat fDelta );

	private :

		CCTexture2D*		m_pTexture;
		CCSprite*			m_pPlayer;

		CCArray*			m_pEnemies;
		CCArray*			m_pLifes;

		CCArray*			m_pPlayerMissiles;
		CCArray*			m_pEnemyMissiles;

		KDint				m_nTouchID;
		KDbool				m_bDirection;

		KDfloat				m_fPlayerMissileDelay;
		KDfloat				m_fEnemyMissileDelay;
};


#endif	// __ScrMainGame_h__
