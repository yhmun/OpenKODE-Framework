/* --------------------------------------------------------------------------
 *
 *      File            Tower.h
 *      Description     
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Kirill Muzykov on 8/22/13.
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

#ifndef __Tower_h__
#define __Tower_h__

#define kTOWER_COST		300

class Enemy;
class GameLayer;

class Tower : public CCSprite
{
	public :

		// Looks like overriding static create method is the best way to subclass CCSprite.
		static Tower*		create ( const KDchar* szFileName );

	protected :

		virtual KDbool		initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated );	// designated initializer (it will be called by create in the end)

		virtual KDvoid		draw ( KDvoid );

		virtual KDvoid		update ( KDfloat fDelta );

		KDvoid				chosenEnemyForAttack ( Enemy* pEnemy );

		KDvoid				attackEnemy ( KDvoid );

		KDvoid				shootWeapon ( KDfloat fDelta = KD_NULL );

		KDvoid				damageEnemy ( KDvoid );

		KDvoid				removeBullet ( CCNode* pBullet );

		KDvoid				lostSightOfEnemy ( KDvoid );

	public :

		KDvoid				setTheGame ( GameLayer* pGame );

		KDvoid				targetKilled ( KDvoid );	

	private :

		KDfloat				m_fAttackRange;
		KDint				m_nDamage;
		KDfloat				m_fFireRate;
    
		GameLayer*			m_pTheGame;
		Enemy*				m_pChosenEnemy;
};

#endif	// __Tower_h__