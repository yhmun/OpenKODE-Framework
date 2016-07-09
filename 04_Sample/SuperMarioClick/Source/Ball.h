/* --------------------------------------------------------------------------
 *
 *      File            Ball.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Liu weimu on 5/21/13
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

class Ball : public BodyNode, public CCTouchDelegate
{
	public :

		static Ball*		create ( b2World* pWorld );

		static Ball*		sharedBall ( KDvoid );
	
	public :

		virtual KDbool		initWithWorld ( b2World* pWorld );

		virtual KDvoid		update ( KDfloat fDelta );

		KDvoid				createBallInWorld ( b2World* pWorld );

		KDvoid				gameReset ( KDvoid );

	private :

		KDbool				m_bMoveToFinger;
		CCPoint				m_tFingerLocation;
};

#endif	// __Ball_h__
