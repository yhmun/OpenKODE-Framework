/* --------------------------------------------------------------------------
 *
 *      File            GameScene.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      t2cn on 2009/10/10
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

#ifndef __GameScene_h__
#define __GameScene_h__

class OverScene;

class GameScene : public CCLayer 
{
	public :
		
		SCENE_FUNC ( GameScene );

		virtual	~GameScene ( KDvoid );

	public  :

		virtual KDbool		init		 ( KDvoid );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid				initBubble	 ( KDvoid );

		KDvoid				initTiles	 ( KDint nCapacity );

		KDint				getTile		 ( KDvoid );

		KDvoid				tick		 ( KDfloat fDelta );	

		KDbool				shouldHit	 ( KDint x, KDint y );

		KDvoid				playAnimationWithBeans ( KDvoid );

		KDint				getTopScore  ( KDvoid );

		KDint				getScore	 ( KDvoid );

	private :

		CCSpriteBatchNode*	m_pSheet;

		CCArray*			m_pTiles;
		CCArray*			m_pBubbles;
		CCArray*			m_pRemain;
		CCArray*			m_pUpArray;
		CCArray*			m_pDownArray;
		CCArray*			m_pLeftArray;
		CCArray*			m_pRightArray;
		CCArray*			m_pActionList;

		CCSprite*			m_pNo;
		KDint				m_nTime;
		KDint				m_nScore;
		KDint				m_nTopScore;

		CCLabelTTF*			m_pScoreLabel;

		CCLayer*			m_pTimeLine;
		CCSprite*			m_pTimeMask;

		CCSprite*			m_pOpen;
		CCSprite*			m_pClose;

		CCPoint				m_aVertices [ 4 ];	

		OverScene*			m_pOver;
};

#endif	// __GameScene_h__