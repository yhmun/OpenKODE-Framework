/* --------------------------------------------------------------------------
 *
 *      File            Ball.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Matsumae Kentaro on 12/03/20
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

#ifndef __Ball_h__
#define __Ball_h__

#include "BodyNode.h"

class BallTracer;

class Ball : public BodyNode, public CCTargetedTouchDelegate
{
	public :

		virtual ~Ball ( KDvoid );

		static   Ball*		create ( b2World* pWorld, const CCPoint& tPos, KDbool bTouchable );

		static   Ball*		create ( b2World* pWorld, const CCPoint& tPos, const ccColor3B& tColor );

	public :

		virtual KDbool		initWithWorld ( b2World* pWorld, const CCPoint& tPos, KDbool bTouchable );

		virtual KDbool		initWithWorld ( b2World* pWorld, const CCPoint& tPos, const ccColor3B& tColor );

		virtual KDbool		initWithWorld ( b2World* pWorld, const CCPoint& tPos, KDbool bTouchable, const ccColor3B& tColor );

		virtual KDvoid		update ( KDfloat fDelta );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		static  KDfloat		ballWidth ( KDvoid );

		virtual KDvoid		resetColor ( KDvoid );

		virtual KDbool		isMainBall ( KDvoid );

		virtual KDbool		isInHall ( KDvoid );

		virtual KDbool		isTouchForMe ( const CCPoint& tLocation );

	private :

		KDvoid				createBallInWorld ( b2World* pWorld, const CCPoint& tPos, const ccColor3B& tColor );

		KDvoid				applyForceTowardsFinger ( KDvoid );

	protected :

		KDbool				m_bMoveToFinger;
		CCPoint				m_tFingerLocation;
		KDfloat				m_fBallRadius;
		ccColor3B			m_tOriginalColor;
		KDbool				m_bIsInHall;
		BallTracer*			m_pBallTracer;
};

#endif	// __Ball_h__