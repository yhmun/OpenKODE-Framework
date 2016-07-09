/* --------------------------------------------------------------------------
 *
 *      File            GameWaypoint.h
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

#ifndef __GameWaypoint_h__
#define __GameWaypoint_h__

#include "ObjectCallback.h"

class GameWaypoint : public CCObject
{
	public :
				 GameWaypoint ( KDvoid );
		virtual ~GameWaypoint ( KDvoid );

		static  GameWaypoint*		create ( const CCPoint& tPoint, KDfloat fSpeed );

	public :

		virtual KDbool				initWithPosition ( const CCPoint& tPoint, KDfloat fSpeed );

		virtual KDvoid				processPreCallback ( KDvoid );

		virtual KDvoid				processPostCallback ( KDvoid );

		virtual const CCPoint&		getPosition ( KDvoid );
		virtual KDvoid				setPosition ( const CCPoint& tPoint );

		virtual KDfloat				getSpeedMod ( KDvoid );
		virtual KDvoid				setSpeedMod ( KDfloat fSpeedMod );

		virtual KDfloat				getLastDistance ( KDvoid );
		virtual KDvoid				setLastDistance ( KDfloat fLastDistance );

		virtual ObjectCallback*		getPreCallback ( KDvoid );
		virtual KDvoid				setPreCallback ( ObjectCallback* pCallback );

		virtual ObjectCallback*		getPostCallback ( KDvoid );
		virtual KDvoid				setPostCallback ( ObjectCallback* pCallback );

	protected :

		CCPoint						m_tPosition;			// Where you are going to
		KDfloat						m_fSpeedMod;			// The relative speed. 0 is not moving. 1 is going as fast as possible.
		KDfloat						m_fLastDistance;		// How far you were from the waypoint last iteration.
		ObjectCallback*				m_pPreCallback;			// Call this when we start moving toward the waypoint
		ObjectCallback*				m_pPostCallback;		// Call this after we reach the waypoint
};

#endif	// __GameWaypoint_h__
