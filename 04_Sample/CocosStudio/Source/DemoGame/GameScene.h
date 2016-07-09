/* -----------------------------------------------------------------------------------
 *
 *      File            GameScene.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __GameScene_h__
#define __GameScene_h__

#include "PhysicalWorld.h"

class GameScene	: public Scene
{
	public :

		// create GameScene depend on stage
		static GameScene*		create ( KDint nStage = 1 );
		
	public :

		virtual KDbool			init ( KDint nStage );

		KDvoid					menuPopSceneCallback ( Object* pSender );
};

class StageBasic : public Layer
{
	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid			update ( KDfloat fDelta );

		virtual KDbool			initWorld ( KDvoid );

		KDvoid					overStage ( KDbool bPlayerDied );

		virtual KDvoid			changeToNextStage ( KDvoid ) = 0;
		KDvoid					changeToOverScene ( KDvoid );

		KDvoid					menuTestCallback ( Object* pSender );

	protected :

		Layer*					m_pStageLayer;
		Player*					m_pPlayer;
		EnemyListBuilder*		m_pEnemyBuilder;

		PhysicalWorld*			m_pPhysicalWorld;
};

class StageOne : public StageBasic
{
	public :

		static StageOne*		create ( KDvoid );

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			changeToNextStage ( KDvoid );

		KDvoid					menuNextStageCallback ( Object* pSender );
};

class StageTwo : public StageBasic
{
	public :

		static StageTwo*		create ( KDvoid );

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			changeToNextStage ( KDvoid );
};

class GameOverScene : public Scene
{
	public :

		static GameOverScene*	create ( KDvoid );

		virtual KDbool			init ( KDvoid );

		KDvoid					menuPopSceneCallback ( Object* pSender );

		KDvoid					menuCloseCallback ( Object* pSender );
};

class VictoryScene : public Scene
{
	public :

		// create a victory scene,define pItem depend on stage
		static VictoryScene*	create ( KDvoid );

		virtual KDbool			init ( KDvoid );

		KDvoid					menuPopSceneCallback ( Object* pSender );

		KDvoid					menuCloseCallback ( Object* pSender );
};


#endif	// __GameScene_h__