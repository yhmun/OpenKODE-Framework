/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsShapeInfo_chipmunk.h
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

#ifndef __CCPhysicsShapeInfo_chipmunk_h__
#define __CCPhysicsShapeInfo_chipmunk_h__

#include "../CCPhysicsSetting.h"
#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)

#include <vector>
#include <map>
#include "XMChipmunk/chipmunk.h"
#include "../../platform/CCPlatformMacros.h"

NS_CC_BEGIN

class PhysicsShape;

class PhysicsShapeInfo
{
	public :

		KDvoid					add ( cpShape* pShape );
		KDvoid					remove ( cpShape* pShape );
		KDvoid					removeAll ( KDvoid );
		KDvoid					setGroup ( cpGroup uGroup );
		KDvoid					setBody ( cpBody* pBody);
    
	public :

		PhysicsShape*			getShape  ( KDvoid ) const { return m_pShape; }
		std::vector<cpShape*>&	getShapes ( KDvoid ) { return m_aShapes; }
		cpBody*					getBody   ( KDvoid ) const { return m_pBody; }
		cpGroup					getGourp  ( KDvoid ) const { return m_uGroup; }

		static std::map<cpShape*, PhysicsShapeInfo*>&	getMap ( KDvoid ) { return m_aMap; }

		static cpBody*			getSharedBody ( KDvoid ) { return m_pSharedBody; }
    
	private :

		 PhysicsShapeInfo ( PhysicsShape* pShape );
		~PhysicsShapeInfo ( KDvoid );
    
	private :

		std::vector<cpShape*>							m_aShapes;
		PhysicsShape*									m_pShape;
		cpBody*											m_pBody;
		cpGroup											m_uGroup;
		static std::map<cpShape*, PhysicsShapeInfo*>	m_aMap;
		static cpBody*									m_pSharedBody;
    
		friend class PhysicsShape;
};

NS_CC_END

#endif	// CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK
#endif	// __CCPhysicsShapeInfo_chipmunk_h__
