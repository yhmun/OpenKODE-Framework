/* --------------------------------------------------------------------------
 *
 *      File            TestTouches.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#ifndef __TestTouches_h__
#define __TestTouches_h__

#include "../TestBasic.h"

typedef enum tagPaddleState 
{
	kPaddleStateGrabbed,
	kPaddleStateUngrabbed
} PaddleState; 

class Paddle : public CCSprite, public CCTargetedTouchDelegate
{
	public :

		static  Paddle*		create ( CCTexture2D* aTexture );

	public :

		CCRect				rect ( KDvoid );
		KDbool				initWithTexture ( CCTexture2D* aTexture );
		KDbool				containsTouchLocation ( CCTouch* touch );

		virtual KDvoid		onEnter ( KDvoid );
		virtual KDvoid		onExit ( KDvoid );

		virtual KDbool		ccTouchBegan ( CCTouch* touch, CCEvent* event );
		virtual KDvoid		ccTouchEnded ( CCTouch* touch, CCEvent* event );
		virtual KDvoid		ccTouchMoved ( CCTouch* touch, CCEvent* event );

		virtual KDvoid		touchDelegateRetain  ( KDvoid );
		virtual KDvoid		touchDelegateRelease ( KDvoid );

	protected :

		PaddleState			m_state;
};

class Ball : public CCSprite
{
	public :

		static Ball*		create ( CCTexture2D* aTexture );

	public :

		KDfloat				radius ( KDvoid );
		KDvoid				move ( KDfloat fElapsed );
		KDvoid				collideWithPaddle ( Paddle* paddle );

		inline KDvoid		setVelocity ( CCPoint velocity ) { m_velocity = velocity; }
		inline CCPoint		getVelocity ( KDvoid )	{ return m_velocity; }

	protected :

		CCPoint				m_velocity;

};

class TestTouches : public TestBasic
{
	public :

		virtual ~TestTouches ( KDvoid );

		CREATE_FUNC ( TestTouches );

	protected :		

		virtual KDbool			init ( KDvoid );	

	public :

		virtual const KDchar*	subtitle ( KDvoid );

		KDvoid					resetAndScoreBallForPlayer ( KDint player );

		KDvoid					doStep ( KDfloat fElapsed );

	protected :

		Ball*					m_pBall;
		CCArray*				m_paddles;
		CCPoint					m_pBallStartingVelocity; 
};

#endif	// __TestTouches_h__
