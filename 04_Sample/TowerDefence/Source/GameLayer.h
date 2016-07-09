/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
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

#ifndef __GameLayer_h__
#define __GameLayer_h__

class Waypoint;
class Enemy;

class GameLayer : public CCLayer
{
	public :

		SCENE_FUNC ( GameLayer );

	protected :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid				loadTowerPositions ( KDvoid );
		
		KDbool				loadWave ( KDvoid );

		KDvoid				addWaypoints ( KDvoid );

		Waypoint*			addWaypoint ( const CCPoint& tPosition, Waypoint* pNext );

		KDvoid				doGameOver ( KDvoid );

		KDbool				canBuyTower ( KDvoid );

	public :

		KDvoid				awardGold ( KDint nGold );

		KDbool				checkCirclesCollide ( const CCPoint& tCenter1, KDfloat fRadius1, const CCPoint& tCenter2, KDfloat fRadius2 );

		KDvoid				getHpDamage ( KDvoid );

		KDvoid				enemyGotKilled ( Enemy* pEnemy );

		const CCArray&		getEnemies ( KDvoid );    

	private :

		KDint				m_nWave;
		KDint				m_nPlayerHP;
		KDint				m_nPlayerGold;
		KDbool				m_bGameEnded;

		CCLabelBMFont*		m_pWave;
		CCLabelBMFont*		m_pHp;
		CCLabelBMFont*		m_pGold;

		CCArray				m_aTowerBases;
		CCArray				m_aWaypoints;
		CCArray				m_aEnemies;
    
};

#endif	// __GameLayer_h__
