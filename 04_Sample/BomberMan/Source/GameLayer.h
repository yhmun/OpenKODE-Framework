/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
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

#ifndef __GameLayer_h__
#define __GameLayer_h__

class GameHero;

class GameLayer : public CCLayer
{
	public :

		CREATE_FUNC ( GameLayer );

				 GameLayer ( KDvoid );
		virtual ~GameLayer ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );

		KDvoid				initTile ( KDvoid );
		
		KDvoid				initHero ( KDvoid );
		
		KDvoid				initEnemy ( KDvoid );
		
		KDvoid				initBomb ( KDvoid );
		
		KDvoid				gameUpdate ( KDfloat fDelta );
		
		KDvoid				checkHeroState ( KDvoid );
		
		KDvoid				checkGameState ( KDvoid );
		
		KDvoid				updateTime ( KDvoid );
		
		KDvoid				gameOver ( KDvoid );
		
		KDvoid				gameWin ( KDvoid );

		KDvoid				reInit ( KDvoid );

		KDvoid				putBomb ( KDvoid );

		KDvoid				putBombEffect ( KDvoid );

		KDvoid				bombDown ( KDfloat fDelta );

		KDvoid				loadNextlevel ( KDint nLevel );

		CCPoint				convertToTilePos ( const CCPoint& tPos );

		GameHero*			getHero ( KDvoid );

	private :

		KDint				m_nW;
		KDint				m_nH;
		KDint				m_nTileW;
		KDint				m_nTileH;
		KDint				m_nScore;
		KDint				m_nLevel;
		KDint				m_nTime;

		CCLabelTTF*			m_pScoreLabel;
		CCLabelTTF*			m_pLevelLabel;
		CCLabelTTF*			m_pTimeLabel;

		CCPoint				m_tHeroStartPostion;
		CCArray*			m_pNullPoints; 
		CCArray*			m_pTileArray;
		CCArray*			m_pEnemyArray;

		GameHero*			m_pHero;
		CCSprite*			m_pBomb;
		CCSprite*			m_pBombEffect;
		CCSprite*			m_pKey;

		KDbool				m_bIsBomb;
		KDbool				m_bIsKey;
};

#endif	// __GameLayer_h__