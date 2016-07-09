/* --------------------------------------------------------------------------
 *
 *      File            Ninja.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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

#ifndef __Ninja_h__
#define __Ninja_h__

#include "BodyNode.h"

class Ninja : public BodyNode
{
	public :

		static Ninja*		create ( b2World* pWorld, const CCPoint& tLocation, const KDchar* szBaseFileName );

	public :

		virtual KDbool		InitWithWorld ( b2World* pWorld, const CCPoint& tLocation, const KDchar* szBaseFileName );	

		virtual KDbool		isOnGround ( KDvoid );

		virtual KDvoid		spriteInSlingState ( KDvoid );

		virtual KDvoid		spriteInStandingState ( KDvoid );

		virtual KDvoid		spriteInPulledBackSlingState ( KDvoid );

		virtual KDvoid		spriteInGroundState ( KDvoid );

		virtual KDvoid		spriteInRollStateWithAnimationFirst ( KDvoid );

		virtual KDvoid		rollAnimation ( KDfloat fDelta );

		virtual KDvoid		spriteInAirState ( KDvoid );

		virtual KDvoid		blink ( KDfloat fDelta );

		virtual KDvoid		openEyes ( KDfloat fDelta );

	private :

		KDvoid				createNinja ( KDvoid );

	protected :

		b2World*			m_pTheWorld;
		std::string			m_sBaseImageName;
		std::string			m_sSpriteImageName;
		CCPoint				m_tInitialLocation;
		KDbool				m_bOnGround;
		KDshort				m_nCounter;
};

#endif	// __Ninja_h__
