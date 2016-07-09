/* --------------------------------------------------------------------------
 *
 *      File            GameEnemy.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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

#ifndef __GameEnemy_h__
#define __GameEnemy_h__

enum EnemyAct
{
    kHorizon	= 1 ,
    kVertical	= 2 ,
};

class GameEnemy : public CCSprite
{
	public :

		static GameEnemy*	create ( const CCPoint& tPoint, CCLayer* pLayer );

	public :

		virtual KDbool		initAtPoint ( const CCPoint& tPoint, CCLayer* pLayer );

		KDvoid				setTileArray ( CCArray* pTileArray );

		KDfloat				getSpeed ( KDvoid );

		KDvoid				enemyMove ( KDfloat fDelta );	

		KDvoid				enemyKill ( KDvoid );

		KDvoid				removeEnemy ( KDvoid );

	private :

		KDfloat				m_fSpeed;
		EnemyAct			m_eAct;
		CCArray*			m_pTileArray;
		KDint				m_nStepCount;
		CCSprite*			m_pMoneyLabel;
};
/*
-(void) enemyMove;
-(void) enemyKill;
@property(assign) enemyAct act;
@property(assign) float speed;
@property(retain,nonatomic) NSMutableArray *tileArray; 
-(id) initAtPosion:(CGPoint) pos Inlayer:(CCLayer *)layer;
@end
*/

#endif	// __GameEnemy_h__