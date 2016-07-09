/* --------------------------------------------------------------------------
 *
 *      File            SSGameActor.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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

#ifndef __SSGameActor_h__
#define __SSGameActor_h__

#include "GameActor.h"
#include "SSGameWaypoint.h"

class SSGameActor : public GameActor
{
	public :
				 SSGameActor ( KDvoid );
		virtual ~SSGameActor ( KDvoid );

		static SSGameActor*		create ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			processWaypoints ( KDvoid );
		
		virtual KDvoid			addWaypoint ( SSGameWaypoint* pWaypoint );

		virtual KDvoid			runToWaypoint ( SSGameWaypoint* pWaypoint, KDfloat fSpeedMod, KDbool bConstrain );

		virtual KDvoid			jumpToWaypoint ( SSGameWaypoint* pWaypoint );

		static  Vector3D*		canJumpFrom ( const CCPoint& tSrc, const CCPoint& tDst, KDfloat fRadius, b2World* pWorld, const CCPoint& tMaxSpeed );
		
		static  CCPoint			getLaunchVector ( const CCPoint& tVect, KDfloat fJumpHeightMod, KDfloat fGravity );

		virtual KDvoid			flyToWaypoint ( SSGameWaypoint* pWaypoint );

		virtual KDvoid			stopRunning ( KDvoid );

		virtual const CCPoint&	getMaxSpeed ( KDvoid );
		virtual KDvoid			setMaxSpeed ( const CCPoint& tMaxSpeed );

	protected :

		CCPoint					m_tMaxSpeed;	//How fast the actor can run and/or jump
		KDfloat					m_fTimeoutCounter;
};

#endif	// __SSGameActor_h__
