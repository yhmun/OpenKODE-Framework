/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
 *      Description     
 *      Author          Hae-Won Lee
 *      Contact         leehw000@gmail.com
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

class GameLayer : public CCLayer 
{
	protected :
				 GameLayer ( KDvoid );
		virtual ~GameLayer ( KDvoid );

	public :

		CREATE_FUNC ( GameLayer );

	protected :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid				initializeBricks ( KDvoid );
		KDvoid				initializeBall   ( KDvoid );
		KDvoid				initializePaddle ( KDvoid );
		KDvoid				initializeDragon ( KDvoid );

		KDvoid				startGame ( KDfloat fDelta );

		KDvoid				gameLogic ( KDfloat fDelta );

		KDvoid				processCollision ( CCSprite* pBrick );

		KDvoid				nextStepTapped ( CCObject* pSender );

	private :

		// 벽돌 숫자 조정용
		KDint				BRICK_HEIGHT;
		KDint				BRICK_WIDTH;

		// 스트라이프용
		CCArray*			m_pTargets;
		CCSprite*			m_pBall;
		CCSprite*			m_pPaddle;
		CCSprite*			m_pDragon;

		// 게임이 진행중인가를 체크하기 위한 인스턴스 변수
		KDbool				m_bIsPlaying;
		// 패들 터치 감지용 인스턴스 변수
		KDbool				m_bIsPaddleTouched;
		//
	    
		// 공의 움직임 저장용 인스턴스 변수
		CCPoint				m_tBallMovement;
};

#endif	// __GameLayer_h__