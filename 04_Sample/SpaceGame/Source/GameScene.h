/* --------------------------------------------------------------------------
 *
 *      File            GameScene.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012 Jean-yves mengant on 2/6/12 week 6. All rights reserved.
 *
 *         http://www.raywenderlich.com/11338/cocos2d-x-for-ios-and-android-space-game
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

typedef enum 
{
  KENDREASONWIN,
  KENDREASONLOSE
} EndReason;

class GameScene : public CCLayer
{
	public :

		CREATE_FUNC ( GameScene );

	public :
				 GameScene ( KDvoid );
		virtual ~GameScene ( KDvoid );

	public :

		virtual KDbool				init ( KDvoid );  

		virtual KDvoid				update ( KDfloat fDelta );

		virtual KDvoid				ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid 				didAccelerate ( CCAcceleration* pAccelerationValue );

		// Game Over
		KDvoid						endScene ( EndReason eEndReason );

		KDvoid						restartTapped ( CCObject* pSender );   

	private :

		CCSpriteBatchNode*			m_pBatchNode;
		CCSprite*					m_pShip;

		CCParallaxNode*				m_pBackgroundNode;  
		CCSprite*					m_pSpacedust1;
		CCSprite*					m_pSpacedust2;
		CCSprite*					m_pPlanetsunrise;
		CCSprite*					m_pGalaxy;
		CCSprite*					m_pSpacialanomaly;
		CCSprite*					m_pSpacialanomaly2;  

		// Accelerometer 
		KDfloat						m_fShipPointsPerSecY;  

		// Asteroids
		CCArray*					m_pAsteroids; 
		KDint						m_nNextAsteroid;
		KDfloat						m_fNextAsteroidSpawn;   

		// ShipLasers
		CCArray*					m_pShipLasers; 
		KDint						m_nNextShipLaser;
  
		// GameOver Stuff
		KDint						m_nLives;

		// GameOver
		KDfloat						m_fGameOverTime;
		KDbool						m_bGameOver;
    
        KDfloat                     m_fPrevPitch;
};

#endif  // __GameScene_h__