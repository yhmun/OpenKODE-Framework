/* -----------------------------------------------------------------------------------
 *
 *      File            PhysicalWorld.h
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

#ifndef __PhysicalWorld_h__
#define __PhysicalWorld_h__

#include "Player.h"
#include "EnemyListBuilder.h"
#include "ColliderToolKits.h"

class PhysicalWorld : public Layer
{
	public :

		static PhysicalWorld*		create ( EnemyListBuilder* pBuilder = KD_NULL, Player* pPlayer = KD_NULL );

		~PhysicalWorld ( KDvoid );

		virtual KDbool				init ( EnemyListBuilder* pBuilder, Player* pPlayer );

		virtual KDvoid				update ( KDfloat fDelta );
		virtual KDvoid				draw ( );

		Player*						getPlayer ( KDvoid ) { return m_pPlayer; }
		KDbool						isGameOver ( KDvoid ) { return m_pPlayer->IsGameOver ( ); }
		KDbool						isPassStage ( KDvoid ) { return m_pEnemyListBuilder->isEnemyAllDied ( ); }

	private :

		KDvoid						initPhysics ( KDvoid );
		KDvoid						initPlayer ( Player* pPlayer );
		KDvoid						setPlayerFilterInfo ( b2Body* pBody );

		KDvoid						initEnemy ( EnemyListBuilder* pBuilder );
		KDvoid						addEnemyIntoWorld ( Enemy* pEnemy );
		KDvoid						setEnemyFilterInfo ( b2Body* pBody,Enemy* pEnemy );

		KDvoid						onFrameEvent ( Bone* pBone, const KDchar* pEvt, KDint nOriginFrameIndex, KDint nCurrentFrameIndex );

		KDvoid						createBullet ( Point tStartPosition, ActionInterval* pBulletAction );
		KDvoid						setBulletFilterInfo ( b2Body* pBody, PhysicsSprite* pBullet );
		KDvoid						destroyBullet ( Node* pSender, KDvoid* pData );

		KDvoid						processContactList ( KDvoid );

		KDvoid						processDiedEnemy ( KDvoid );
		KDvoid						destroyEnemy ( Enemy* pEnemy );

		b2World*					m_pWorld;
		MyContactListener*			m_pContactListener;
		Box2DDebugDraw*				m_pDebugDraw;

		Player*						m_pPlayer;
		EnemyListBuilder*			m_pEnemyListBuilder;
};

#endif	// __PhysicalWorld_h__