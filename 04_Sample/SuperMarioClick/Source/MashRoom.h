/* --------------------------------------------------------------------------
 *
 *      File            MashRoom.h
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

#ifndef __MashRoom_h__
#define __MashRoom_h__

#include "BodyNode.h"

class MashRoom : public BodyNode
{
	public :

		static MashRoom*		create ( b2World* pWorld );

		static MashRoom*		sharedMashRoom ( KDvoid );
				
	public :

		virtual KDbool			initWithWorld ( b2World* pWorld );

		virtual KDvoid			setBody ( b2Body* pBody );

		virtual KDvoid			setFixture ( b2Fixture* pFixture );

		virtual b2Body*			sharedMashRoomBody ( KDvoid );

		virtual b2Fixture*		shareMashRoomFixture ( KDvoid );

		virtual KDvoid			setDoPlunge ( KDbool bYes );

	private :

		b2PrismaticJoint*		m_pJoint;
		KDbool					m_bDoPlunge;
		KDfloat					m_fPlungeTime;
    
	    b2Body*					m_pPlungerBody;
	    b2Fixture*				m_pPlungerFixture;

		b2Body*					m_pSharedMashRoomBody;
		b2Fixture*				m_pShareMashRoomFixture;
};

#endif	// __MashRoom_h__
