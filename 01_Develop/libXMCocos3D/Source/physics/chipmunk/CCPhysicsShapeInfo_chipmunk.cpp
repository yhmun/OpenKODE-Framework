/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsShapeInfo_chipmunk.cpp
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

#include "physics/chipmunk/CCPhysicsShapeInfo_chipmunk.h"
#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
#include <algorithm>
NS_CC_BEGIN

std::map<cpShape*, PhysicsShapeInfo*> PhysicsShapeInfo::m_aMap;
cpBody* PhysicsShapeInfo::m_pSharedBody = nullptr;

PhysicsShapeInfo::PhysicsShapeInfo(PhysicsShape* shape)
: m_pShape(shape)
, m_uGroup(CP_NO_GROUP)
{
    if (m_pSharedBody == nullptr)
    {
        m_pSharedBody = cpBodyNewStatic();
    }
    
    m_pBody = m_pSharedBody;
}

PhysicsShapeInfo::~PhysicsShapeInfo()
{
    for (auto shape : m_aShapes)
    {
        auto it = m_aMap.find(shape);
        if (it != m_aMap.end()) m_aMap.erase(shape);
        
        cpShapeFree(shape);
    }
}

void PhysicsShapeInfo::setGroup(cpGroup group)
{
    this->m_uGroup = group;
    
    for (cpShape* shape : m_aShapes)
    {
        cpShapeSetGroup(shape, group);
    }
}

void PhysicsShapeInfo::setBody(cpBody* body)
{
    if (this->m_pBody != body)
    {
        this->m_pBody = body;
        for (cpShape* shape : m_aShapes)
        {
            cpShapeSetBody(shape, body == nullptr ? m_pSharedBody : body);
        }
    }
}

void PhysicsShapeInfo::add(cpShape* shape)
{
    if (shape == nullptr) return;
    
    cpShapeSetGroup(shape, m_uGroup);
    m_aShapes.push_back(shape);
    m_aMap.insert(std::pair<cpShape*, PhysicsShapeInfo*>(shape, this));
}

void PhysicsShapeInfo::remove(cpShape* shape)
{
    if (shape == nullptr) return;
    
    auto it = std::find(m_aShapes.begin(), m_aShapes.end(), shape);
    if (it != m_aShapes.end())
    {
        m_aShapes.erase(it);
        
        auto mit = m_aMap.find(shape);
        if (mit != m_aMap.end()) m_aMap.erase(mit);
        
        cpShapeFree(shape);
    }
}

void PhysicsShapeInfo::removeAll()
{
    for (cpShape* shape : m_aShapes)
    {
        auto mit = m_aMap.find(shape);
        if (mit != m_aMap.end()) m_aMap.erase(mit);
        cpShapeFree(shape);
    }
    
    m_aShapes.clear();
}

NS_CC_END
#endif // CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK
