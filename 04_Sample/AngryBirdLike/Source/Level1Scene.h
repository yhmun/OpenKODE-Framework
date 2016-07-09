/* --------------------------------------------------------------------------
 *
 *      File            Level1Scene.h
 *      Ported By       Hyeon-Ho Jeong
 *      Contact         skdldnjs@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      
 *
 *      http://www.usmanacademy.com/2012/07/rage-of-war-game-using-cocos2d-x-and.html
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

#ifndef __Level1Scene_h__
#define __Level1Scene_h__

#include "Level1ContactListener.h"
#include "GameOverScene.h"
#include "WinnerScene.h"
#include "MainScene.h"

class Level1Scene : public CCLayer 
{
	public:

		virtual	~Level1Scene ( );

		CREATE_FUNC ( Level1Scene );				

	public :

		virtual KDbool		init				( KDvoid ); 

		virtual KDvoid		ccTouchesBegan		( CCSet* touches, CCEvent* events );
		virtual KDvoid		ccTouchesMoved		( CCSet* touches, CCEvent* events );
		virtual KDvoid		ccTouchesEnded		( CCSet* touches, CCEvent* events );		

		virtual KDvoid		draw				( KDvoid );

		KDvoid				initPhysics			( KDvoid );

		KDvoid				onMenu				( CCObject* pSender );

		KDvoid				tick				( KDfloat fDelta );

		KDvoid				update				( KDfloat fDelta );
		KDvoid				updateTargets		( KDvoid );
		KDvoid				updateScore			( KDint nScore );

		KDvoid				spMoveFinished		( CCNode* pSender );

		KDvoid				GameSetup			( KDvoid );
		
		KDvoid				AddTargets			( KDvoid );
		KDvoid				AddRocks			( CCPoint tPos );
		KDvoid				AddWoods			( CCPoint tPos );
		KDvoid				AddClowns			( CCPoint tPos );
		KDvoid				AddBallons			( CCPoint tPos );
		KDvoid				AddClownHeads		( CCPoint tPos );
		KDvoid				AddLongWoods		( CCPoint tPos );
		KDvoid				AddLongRocks		( CCPoint tPos );
		KDvoid				AddBalls			( CCPoint tPos );
		KDvoid				AddDarts			( CCPoint tPos );
		KDvoid				AddTomatos			( CCPoint tPos );
		
		KDvoid				generateMap			( KDvoid );
		
		KDvoid				ScoreAnimation		( CCPoint tPos, const KDchar* szImgName, const KDchar* szImgName2 );
		KDvoid				ExplosionAnimation  ( CCPoint tPos ,const KDchar* szImgName );

	private :
		
		KDint						m_nScoreCount;
		KDint						m_nClownCount;
		KDint						m_nTargetCount;
		KDint						m_nTargetIndex;
		KDint						m_nBirdindex;
		KDbool						m_bTargetThrough;
		KDbool						m_bSecondTab;
		KDbool						m_bHit;


		Level1ContactListener*		m_pContact;
		CCBox2DDebugDraw*			m_pDebugDraw;
		CCSprite*					m_pExplosion; 
		CCLabelTTF*					m_pScoreLabel;

		CCParallaxNode*				m_pBackgroundNode;


		CCSprite*					m_pCloud1;
		CCSprite*					m_pCloud2;
		CCSprite*					m_pCloud3;
		CCSprite*					m_pCloud4;

		CCSprite*					m_pBird1;
		CCSprite*					m_pBird2;
		CCSprite*					m_pBird3;
		CCSprite*					m_pBird4;

		std::vector<CCSprite*>*		m_pAngle;
		b2World*					m_pWorld;
		
		b2Body*						m_pSlingBody;
		b2Body*						m_pTargetBody;
		b2Body*						m_pBody;
		b2RevoluteJoint*			m_pSlingJoint;
		b2MouseJoint*				m_pMouseJoint;
		b2WeldJoint*				m_pTargetJoint;

		std::vector<b2Body*>*		m_pTargets;
		std::vector<b2Body*>*		m_pRocks;
		std::vector<b2Body*>*		m_pWoods;
		std::vector<b2Body*>*		m_pStructures;
		std::vector<b2Body*>*		m_pClowns;
		std::vector<b2Body*>*		m_pClownheads;	
};

#endif	// __Level1Scene_h__
