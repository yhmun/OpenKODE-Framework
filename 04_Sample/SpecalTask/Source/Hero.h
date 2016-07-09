/* --------------------------------------------------------------------------
 *
 *      File            Hero.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-10-26
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

#ifndef __Hero_h__
#define __Hero_h__

class Hero : public CCSprite
{
	public :

		virtual ~Hero ( KDvoid );

		static Hero*			getInstance ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			setOpacity ( GLubyte cOpacity );

		KDvoid					jumpDownFromTruck ( KDvoid );

		KDbool					isJumping ( KDvoid );

		KDvoid					run ( KDvoid );

		KDvoid					stop ( KDvoid );

		KDvoid					jump ( KDfloat* pGroundPlane );

		KDvoid					selfJumpDone ( KDvoid );

		KDvoid					fire ( KDvoid );

	private :

		CCSprite*				m_pHead;
		CCSprite*				m_pLeg;
		CCSprite*				m_pHand;

		CCFiniteTimeAction*		m_pActIdle;
		CCFiniteTimeAction*		m_pActRun;
		CCFiniteTimeAction*		m_pActJump;
		CCFiniteTimeAction*		m_pActSit;
		CCFiniteTimeAction*		m_pActFire;

		KDbool					m_bIsRun;
		KDbool					m_bIsJump;
};

#endif	// __Hero_h__
