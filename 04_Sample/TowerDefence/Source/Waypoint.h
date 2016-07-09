/* --------------------------------------------------------------------------
 *
 *      File            Waypoint.h
 *      Description     
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Kirill Muzykov on 8/22/13.
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

#ifndef __Waypoint_h__
#define __Waypoint_h__

/**
 *	Waypoints are required for enemies to move. Enemy moves from one waypoint to another, 
 *	when enemy reaches waypoint it takes nextWaypoint and moves to it.
 */
class Waypoint : public CCNode
{
	public :

		CREATE_FUNC ( Waypoint );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		draw ( KDvoid );

		virtual Waypoint*	getNextWaypoint ( KDvoid );
		virtual KDvoid		setNextWaypoint ( Waypoint* pNext );

	private :

		Waypoint*			m_pNextWaypoint;
};

#endif	// __Waypoint_h__
