/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsJointInfo_chipmunk.cpp
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

#include "physics/chipmunk/CCPhysicsJointInfo_chipmunk.h"
#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
#include <algorithm>
NS_CC_BEGIN

std::map<cpConstraint*, PhysicsJointInfo*> PhysicsJointInfo::m_aMap;

PhysicsJointInfo::PhysicsJointInfo(PhysicsJoint* joint)
: m_pJoint(joint)
{
}

PhysicsJointInfo::~PhysicsJointInfo()
{
    for (cpConstraint* joint : m_aJoints)
    {
        cpConstraintFree(joint);
    }
}

void PhysicsJointInfo::add(cpConstraint* joint)
{
    if (joint == nullptr) return;

    m_aJoints.push_back(joint);
    m_aMap.insert(std::pair<cpConstraint*, PhysicsJointInfo*>(joint, this));
}

void PhysicsJointInfo::remove(cpConstraint* joint)
{
    if (joint == nullptr) return;
    
    auto it = std::find(m_aJoints.begin(), m_aJoints.end(), joint);
    if (it != m_aJoints.end())
    {
        m_aJoints.erase(it);
        
        auto mit = m_aMap.find(joint);
        if (mit != m_aMap.end()) m_aMap.erase(mit);
        
        cpConstraintFree(joint);
    }
}

void PhysicsJointInfo::removeAll()
{
    for (cpConstraint* joint : m_aJoints)
    {
        auto mit = m_aMap.find(joint);
        if (mit != m_aMap.end()) m_aMap.erase(mit);
        cpConstraintFree(joint);
    }
    
    m_aJoints.clear();
}

NS_CC_END
#endif // CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK
