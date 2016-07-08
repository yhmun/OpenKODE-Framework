/* --------------------------------------------------------------------------
 *
 *      File            SSGameWaypoint.h
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

#ifndef __SSGameWaypoint_h__
#define __SSGameWaypoint_h__

#include "GameWaypoint.h"

enum 
{
	MOVE_TYPE_RUN	= 0,
	MOVE_TYPE_JUMP	= 1,
	MOVE_TYPE_FLY	= 2,
};

class SSGameWaypoint : public GameWaypoint
{
	public :

		static  SSGameWaypoint*		create ( const CCPoint& tPoint, KDint nType, KDfloat fSpeed );

	public :

		virtual KDbool				initWithPosition ( const CCPoint& tPoint, KDint nType, KDfloat fSpeed );

		virtual KDint				getMoveType ( KDvoid );
		virtual KDvoid				setMoveType ( KDint nMoveType );

		virtual KDbool				 isActive ( KDvoid );
		virtual KDvoid				setActive ( KDbool bActive );

		virtual const CCPoint&		getLaunchVector ( KDvoid );
		virtual KDvoid				setLaunchVector ( const CCPoint& tLaunchVector );

	protected :

		KDint						m_nMoveType;		// RUN, JUMP, FLY, etc
		KDbool						m_bActive;			// Is the waypoint active?
		CCPoint						m_tLaunchVector;	// If we need to JUMP, the launch vector
};

#endif	// __SSGameWaypoint_h__
