/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsJoint.cpp
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

#include "physics/CCPhysicsJoint.h"
#ifdef CC_USE_PHYSICS

#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
#include "XMChipmunk/chipmunk.h"
#elif (CC_PHYSICS_ENGINE == CCPHYSICS_BOX2D)
#include "XMBox2D/Box2D.h"
#endif

#include "physics/CCPhysicsBody.h"
#include "physics/CCPhysicsWorld.h"

#include "physics/chipmunk/CCPhysicsJointInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsJointInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsBodyInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsBodyInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsShapeInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsShapeInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsHelper_chipmunk.h"
#include "physics/box2d/CCPhysicsHelper_box2d.h"
#include "2d/base_nodes/CCNode.h"

NS_CC_BEGIN

PhysicsJoint::PhysicsJoint()
: m_pBodyA(nullptr)
, m_pBodyB(nullptr)
, m_pWorld(nullptr)
, m_pInfo(nullptr)
, m_bEnable(false)
, m_bCollisionEnable(true)
, m_bDestoryMark(false)
, m_nTag(0)
{
    
}

PhysicsJoint::~PhysicsJoint()
{
    // reset the shapes collision group
    setCollisionEnable(true);
    
    CC_SAFE_DELETE(m_pInfo);
}

bool PhysicsJoint::init(cocos2d::PhysicsBody *a, cocos2d::PhysicsBody *b)
{
    do
    {
        CC_BREAK_IF(!(m_pInfo = new PhysicsJointInfo(this)));
        
        if (a != nullptr)
        {
            m_pBodyA = a;
            m_pBodyA->m_aJoints.push_back(this);
        }
        
        if (b != nullptr)
        {
            m_pBodyB = b;
            m_pBodyB->m_aJoints.push_back(this);
        }
        
        return true;
    } while (false);
    
    return false;
}

void PhysicsJoint::setEnable(bool enable)
{
    if (m_bEnable != enable)
    {
        m_bEnable = enable;
        
        if (m_pWorld != nullptr)
        {
            if (enable)
            {
                m_pWorld->addJointOrDelay(this);
            }else
            {
                m_pWorld->removeJointOrDelay(this);
            }
        }
    }
}

PhysicsJointPin::PhysicsJointPin()
{
    
}

PhysicsJointPin::~PhysicsJointPin()
{
    
}

PhysicsJointFixed::PhysicsJointFixed()
{
    
}

PhysicsJointFixed::~PhysicsJointFixed()
{
    
}

PhysicsJointLimit::PhysicsJointLimit()
{
    
}

PhysicsJointLimit::~PhysicsJointLimit()
{
    
}

PhysicsJointDistance::PhysicsJointDistance()
{
    
}

PhysicsJointDistance::~PhysicsJointDistance()
{
    
}

#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
PhysicsBodyInfo* PhysicsJoint::getBodyInfo(PhysicsBody* body) const
{
    return body->m_pInfo;
}

Node* PhysicsJoint::getBodyNode(PhysicsBody* body) const
{
    return body->m_pNode;
}


void PhysicsJoint::setCollisionEnable(bool enable)
{
    if (m_bCollisionEnable != enable)
    {
        m_bCollisionEnable = enable;
    }
}

void PhysicsJoint::removeFormWorld()
{
    if (m_pWorld)
    {
        m_pWorld->removeJoint(this, false);
    }
}

void PhysicsJoint::destroy(PhysicsJoint* joint)
{
    if (joint!= nullptr)
    {
        // remove the joint and delete it.
        if (joint->m_pWorld != nullptr)
        {
            joint->m_pWorld->removeJoint(joint, true);
        }
        else
        {
            if (joint->m_pBodyA != nullptr)
            {
                joint->m_pBodyA->removeJoint(joint);
            }
            
            if (joint->m_pBodyB != nullptr)
            {
                joint->m_pBodyB->removeJoint(joint);
            }
            
            delete joint;
        }
    }
}

PhysicsJointFixed* PhysicsJointFixed::construct(PhysicsBody* a, PhysicsBody* b, const Point& anchr)
{
    PhysicsJointFixed* joint = new PhysicsJointFixed();
    
    if (joint && joint->init(a, b, anchr))
    {
        return joint;
    }
    
    CC_SAFE_DELETE(joint);
    return nullptr;
}

bool PhysicsJointFixed::init(PhysicsBody* a, PhysicsBody* b, const Point& anchr)
{
    do
    {
        CC_BREAK_IF(!PhysicsJoint::init(a, b));
        
        getBodyNode(a)->setPosition(anchr);
        getBodyNode(b)->setPosition(anchr);
        
        // add a pivot joint to fixed two body together
        cpConstraint* joint = cpPivotJointNew(getBodyInfo(a)->getBody(), getBodyInfo(b)->getBody(),
                                        PhysicsHelper::point2cpv(anchr));
        CC_BREAK_IF(joint == nullptr);
        m_pInfo->add(joint);
        
        // add a gear joint to make two body have the same rotation.
        joint = cpGearJointNew(getBodyInfo(a)->getBody(), getBodyInfo(b)->getBody(), 0, 1);
        CC_BREAK_IF(joint == nullptr);
        m_pInfo->add(joint);
        
        setCollisionEnable(false);
        
        return true;
    } while (false);
    
    return false;
}

PhysicsJointPin* PhysicsJointPin::construct(PhysicsBody* a, PhysicsBody* b, const Point& anchr)
{
    PhysicsJointPin* joint = new PhysicsJointPin();
    
    if (joint && joint->init(a, b, anchr))
    {
        return joint;
    }
    
    CC_SAFE_DELETE(joint);
    return nullptr;
}

bool PhysicsJointPin::init(PhysicsBody *a, PhysicsBody *b, const Point& anchr)
{
    do
    {
        CC_BREAK_IF(!PhysicsJoint::init(a, b));
        cpConstraint* joint = cpPivotJointNew(getBodyInfo(a)->getBody(), getBodyInfo(b)->getBody(),
                                       PhysicsHelper::point2cpv(anchr));
        
        CC_BREAK_IF(joint == nullptr);
        
        m_pInfo->add(joint);
        
        return true;
    } while (false);
    
    return false;
}

void PhysicsJointPin::setMaxForce(float force)
{
    m_pInfo->getJoints().front()->maxForce = PhysicsHelper::float2cpfloat(force);
}

float PhysicsJointPin::getMaxForce() const
{
    return PhysicsHelper::cpfloat2float(m_pInfo->getJoints().front()->maxForce);
}

PhysicsJointLimit* PhysicsJointLimit::construct(PhysicsBody* a, PhysicsBody* b, const Point& anchr1, const Point& anchr2)
{
    PhysicsJointLimit* joint = new PhysicsJointLimit();
    
    if (joint && joint->init(a, b, anchr1, anchr2))
    {
        return joint;
    }
    
    CC_SAFE_DELETE(joint);
    return nullptr;
}

bool PhysicsJointLimit::init(PhysicsBody* a, PhysicsBody* b, const Point& anchr1, const Point& anchr2)
{
    do
    {
        CC_BREAK_IF(!PhysicsJoint::init(a, b));
        
        cpConstraint* joint = cpSlideJointNew(getBodyInfo(a)->getBody(), getBodyInfo(b)->getBody(),
                                       PhysicsHelper::point2cpv(anchr1),
                                       PhysicsHelper::point2cpv(anchr2),
                                       0,
                                       PhysicsHelper::float2cpfloat(m_pBodyA->local2World(anchr1).getDistance(m_pBodyB->local2World(anchr2))));
        
        CC_BREAK_IF(joint == nullptr);
        
        m_pInfo->add(joint);
        
        return true;
    } while (false);
    
    return false;
}

float PhysicsJointLimit::getMin() const
{
    return PhysicsHelper::cpfloat2float(cpSlideJointGetMin(m_pInfo->getJoints().front()));
}

void PhysicsJointLimit::setMin(float min)
{
    cpSlideJointSetMin(m_pInfo->getJoints().front(), PhysicsHelper::float2cpfloat(min));
}

float PhysicsJointLimit::getMax() const
{
    return PhysicsHelper::cpfloat2float(cpSlideJointGetMax(m_pInfo->getJoints().front()));
}

void PhysicsJointLimit::setMax(float max)
{
    cpSlideJointSetMax(m_pInfo->getJoints().front(), PhysicsHelper::float2cpfloat(max));
}

PhysicsJointDistance* PhysicsJointDistance::construct(PhysicsBody* a, PhysicsBody* b, const Point& anchr1, const Point& anchr2)
{
    PhysicsJointDistance* joint = new PhysicsJointDistance();
    
    if (joint && joint->init(a, b, anchr1, anchr2))
    {
        return joint;
    }
    
    CC_SAFE_DELETE(joint);
    return nullptr;
}

bool PhysicsJointDistance::init(PhysicsBody* a, PhysicsBody* b, const Point& anchr1, const Point& anchr2)
{
    do
    {
        CC_BREAK_IF(!PhysicsJoint::init(a, b));
        
        cpConstraint* joint = cpPinJointNew(getBodyInfo(a)->getBody(),
                                            getBodyInfo(b)->getBody(),
                                            PhysicsHelper::point2cpv(anchr1),
                                            PhysicsHelper::point2cpv(anchr2));
        
        CC_BREAK_IF(joint == nullptr);
        
        m_pInfo->add(joint);
        
        return true;
    } while (false);
    
    return false;
}

#elif (CC_PHYSICS_ENGINE == CC_PHYSICS_BOX2D)

#endif

NS_CC_END
#endif // CC_USE_PHYSICS
