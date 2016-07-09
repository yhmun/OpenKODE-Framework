/* --------------------------------------------------------------------------
 *
 *      File            GameScene.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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

#include "Utils/MyContactListener.h"

class LevelFileHelper;

class GameScene : public CCLayer
{
	public :
				 GameScene ( LevelFileHelper* pLevelHelper );
		virtual ~GameScene ( KDvoid );

		static CCScene*		GameSceneWithLevelHandler ( LevelFileHelper* pLevelHelper );

	public :

		KDvoid				createMenu ( KDvoid );

		KDvoid				initPhysics ( KDvoid );

		KDvoid				initLevel ( KDvoid );

		virtual KDvoid		draw ( KDvoid );

		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid		update ( KDfloat fDelta );

		b2Body*				createCandyAt ( CCPoint tPos );

		KDvoid				createRope ( b2Body* pBodyA, b2Body* pBodyB, b2Vec2 tAnchorA, b2Vec2 tAnchorB, KDfloat fSagity );

		b2Body*				createRopeTipBody ( KDvoid );

		KDvoid				changeCrocAttitude ( KDfloat fDelta );

		KDbool				checkLineIntersection ( CCPoint p1, CCPoint p2, CCPoint p3, CCPoint p4 );

		KDvoid				switchToEditor ( CCObject* pSender );
		KDvoid				switchToMenu   ( CCObject* pSender );

		KDvoid				checkLevelFinish ( KDbool bForceFinish );

		KDvoid				finishLevel ( KDvoid );

		KDvoid				finishAction ( CCObject* pSender );

		KDvoid				restartGame ( KDfloat fDelta );

	private :

		CCSprite*				m_pCroc;
		CCSpriteBatchNode*		m_pRopeSpriteSheet;
		LevelFileHelper*		m_pLevelEditor;
		CCBox2DDebugDraw*		m_pDebugDraw;

		b2World*				m_pWorld;
		b2Body*					m_pGroundBody;
		b2Body*					m_pCrocMouth; 
		b2Fixture*				m_pCrocMouthBottom;
		MyContactListener*		m_pContactListener;

		KDbool					m_bCrocMouthOpened;
		
		std::vector<b2Body*>	m_aCandies;	
		CCArray*				m_pRopes;
		CCArray*				m_pSticks;

		CCLabelTTF*				m_pWinLabel;
		CCLabelTTF*				m_pLoseLabel;
};

#endif  // __GameScene_h__