/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsBodyInfo_chipmunk.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2013      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __CCPhysicsBodyInfo_chipmunk_h__
#define __CCPhysicsBodyInfo_chipmunk_h__

#include "../CCPhysicsSetting.h"

#if ( CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK )
#include "../../platform/CCPlatformMacros.h"
#include "XMChipmunk/chipmunk.h"
#include "../../base/CCObject.h"

NS_CC_BEGIN

class PhysicsBodyInfo
{
	public :

		inline cpBody*		getBody ( KDvoid ) const { return m_pBody; }
		inline KDvoid		setBody ( cpBody* pBody ) { m_pBody = pBody; }
    
	private :

		 PhysicsBodyInfo ( KDvoid );
		~PhysicsBodyInfo ( KDvoid );
    
	private :

		cpBody*				m_pBody;
    
		friend class PhysicsBody;
};

NS_CC_END

#endif	// CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK
#endif	// __CCPhysicsBodyInfo_chipmunk_h__
