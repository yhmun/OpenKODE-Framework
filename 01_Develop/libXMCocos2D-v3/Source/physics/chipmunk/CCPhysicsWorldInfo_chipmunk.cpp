/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsWorldInfo_chipmunk.cpp
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

#include "physics/chipmunk/CCPhysicsWorldInfo_chipmunk.h"
#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
NS_CC_BEGIN

#define PHYSICS_WORLD_INFO_FUNCTION_IMPLEMENTS(name, type) \
void PhysicsWorldInfo::add##name(cp##type* data) \
{ \
    if (!cpSpaceContains##type(m_pSpace, data)) cpSpaceAdd##type(m_pSpace, data); \
} \
\
void PhysicsWorldInfo::remove##name(cp##type* data) \
{ \
    if (cpSpaceContains##type(m_pSpace, data)) cpSpaceRemove##type(m_pSpace, data); \
} \

PHYSICS_WORLD_INFO_FUNCTION_IMPLEMENTS(Shape, Shape)
PHYSICS_WORLD_INFO_FUNCTION_IMPLEMENTS(Body, Body)
PHYSICS_WORLD_INFO_FUNCTION_IMPLEMENTS(Joint, Constraint)

PhysicsWorldInfo::PhysicsWorldInfo()
{
    m_pSpace = cpSpaceNew();
}

PhysicsWorldInfo::~PhysicsWorldInfo()
{
    cpSpaceFree(m_pSpace);
}

NS_CC_END
#endif // CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK
