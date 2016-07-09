/* --------------------------------------------------------------------------
 *
 *      File            Game.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Sergey Tikhonov. All rights reserved. 
 *
 *      https://www.github.com/haqu/climbers
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

class Hero;
class Rock;
class Grab;

class Game : public CCLayer
{
	public :	

		SCENE_FUNC ( Game );

		virtual ~Game ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );	
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );	

		virtual KDvoid		update ( KDfloat fDelta );

		KDvoid				loadLevel ( KDvoid );

		KDvoid				resetLevel ( KDvoid );

		KDvoid				updateStarsCollectedLabel ( KDvoid );

		KDvoid				updateUIPosition ( KDvoid );

		KDvoid				updateCamera ( KDvoid );

		KDvoid				scheduleRockAlert ( KDvoid );

		KDvoid				showRockAlert ( KDfloat fDelta );

		KDvoid				dropRock ( KDvoid );

		KDvoid				levelCompleted ( KDvoid );

		KDvoid				levelFailed ( KDvoid );

		KDbool				dragHeroNearGrab ( Grab* pGrab );

		KDvoid				sparkleAt ( const CCPoint& tPosition );

		KDvoid				updateDragHeroPositionWithTouchLocation ( const CCPoint& tTouchLocation );

		KDvoid				showPopupMenu ( KDvoid );

	private :

		CCSpriteBatchNode*	m_pBatch1;
		CCSpriteBatchNode*	m_pBatch2;
		CCSpriteBatchNode*	m_pRopeBatch;
		Hero*				m_pHero1;
		Hero*				m_pHero2;
		VRope*				m_pRope;
		CCArray*			m_pGrabs;
		CCArray*			m_pStars;
		KDbool				m_bDragInProgress;
		Hero*				m_pDragHero;
		Hero*				m_pDragOtherHero;
		CCPoint				m_tDragOffset;
		CCPoint				m_tCameraOffset;
		KDfloat				m_fLevelHeight;
		KDbool				m_bGameInProgress;
		KDint				m_nCurrentLevel;
		KDint				m_nNextLevel;
		CCPoint				m_tStartPosition1;
		CCPoint				m_tStartPosition2;
		CCSprite*			m_pSnapFeedback;
		Rock*				m_pRock;
		CCSprite*			m_pRockAlert;
		CCPoint				m_tLastTouchLocation;
		KDfloat				m_fRopeLength;
		KDfloat				m_fHeroStarDist;
		KDfloat				m_fHeroRockDist;
		KDfloat				m_fSnapDist;
		KDint				m_nStarsCollected;
		KDint				m_nStarsTotal;
		CCSprite*			m_pStarIcon;
		CCLabelBMFont*		m_pStarsCollectedLabel;
		CCSprite*			m_pMenuButton;

		KDfloat				sw;
		KDfloat				sh;
};

#if 0
@class Hero;
@class VRope;
@class Rock;

@interface Game : CCLayer {

}
+ (CCScene*)scene;

@end

#endif	// 0

#endif // __Game_h__