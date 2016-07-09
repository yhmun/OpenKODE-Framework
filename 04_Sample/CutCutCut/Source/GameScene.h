/* -----------------------------------------------------------------------------------
 *
 *      File            GameScene.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Allen Benson G Tan on 5/19/12 
 *      Copyright (c) 2012      WhiteWidget Inc. All rights reserved.    
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 * 
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 * 
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __GameScene_h__
#define __GameScene_h__

#include "RaycastCallback.h"

enum TossType
{
    kTossConsecutive	= 0	,
    kTossSimultaneous		
};

class GameScene : public CCLayer
{
	public :
				 GameScene ( KDvoid );
		virtual ~GameScene ( KDvoid );

		CREATE_FUNC ( GameScene );

	public :

		virtual	KDbool			init ( KDvoid );

		virtual KDvoid			draw ( KDvoid );

		virtual KDvoid			update ( KDfloat fDelta );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					initPhysics ( KDvoid );

		KDvoid					initBackground ( KDvoid );

		KDvoid					initSprites ( KDvoid );

		KDvoid					initHUD ( KDvoid );

		KDvoid					spriteLoop ( KDvoid );

		KDvoid					cleanSprites ( KDvoid );

		KDvoid					tossSprite ( PolygonSpriteEx* pSprite );

		KDvoid					checkAndSliceObjects ( KDvoid );

		KDvoid					clearSlices ( KDvoid );

		KDvoid					splitPolygonSprite ( PolygonSpriteEx* pSprite );

		b2Vec2*					arrangeVertices ( b2Vec2* pVertices, KDint nCount );

		KDbool					areVerticesAcceptable ( b2Vec2* pVertices, KDint nCount );

		b2Body*					createBodyWithPosition ( b2Vec2 tPosition, KDfloat fRotation, b2Vec2* pVertices, KDint nCount, KDfloat fDensity, KDfloat fFriction, KDfloat fRestitution );

		KDvoid					subtractLife ( KDvoid );

		KDvoid					endGame ( KDvoid );

		KDvoid					restart ( CCObject* pSender );

	private :

		// the physics simulation world
		b2World*				m_pWorld;
		CCBox2DDebugDraw*		m_pDebugDraw;

		// the sprite cache
		CCArray*				m_pCaches;

		// variables for the blade effect
		CCArray*				m_pBlades;
		CCBlade*				m_pBlade;
		KDfloat					m_fDeltaRemainder;

		// variables for ray casting
		CCPoint					m_tStartPoint;
		CCPoint					m_tEndPoint;
	    
		RaycastCallback*		m_pRaycastCallback;

		// variables for tossing mechanic
		KDfloat					m_fNextTossTime;
		KDfloat					m_fTossInterval;
		KDint					m_nQueuedForToss;
		TossType				m_eCurrentTossType;
	    
		// variables for scoring
		KDint					m_nCuts;
		KDint					m_nLives;
		CCLabelTTF*				m_pCutsLabel;
	    
		// particle effect for the blade
		CCParticleSystemQuad*	m_pBladeSparkle;
	    
		// variables for playing the swoosh sound
		KDfloat					m_fTimeCurrent;
		KDfloat					m_fTimePrevious;
		KDuint					m_uSwoosh;
};

#endif  // __GameScene_h__