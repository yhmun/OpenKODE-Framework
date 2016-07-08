/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsBody.cpp
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

#include "physics/CCPhysicsBody.h"
#ifdef CC_USE_PHYSICS

#include <climits>
#include <algorithm>

#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
#include "XMChipmunk/chipmunk.h"
#elif (CC_PHYSICS_ENGINE == CCPHYSICS_BOX2D)
#include "XMBox2D/Box2D.h"
#endif

#include "physics/CCPhysicsShape.h"
#include "physics/CCPhysicsJoint.h"
#include "physics/CCPhysicsWorld.h"

#include "physics/chipmunk/CCPhysicsBodyInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsBodyInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsJointInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsJointInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsWorldInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsWorldInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsShapeInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsShapeInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsHelper_chipmunk.h"
#include "physics/box2d/CCPhysicsHelper_box2d.h"

NS_CC_BEGIN


#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)

namespace
{
    static const float MASS_DEFAULT = 1.0;
    static const float MOMENT_DEFAULT = 200;
}

PhysicsBody::PhysicsBody()
: m_pNode(nullptr)
, m_pShapes(nullptr)
, m_pWorld(nullptr)
, m_pInfo(nullptr)
, m_bDynamic(true)
, m_bEnable(true)
, m_bRotationEnable(true)
, m_bGravityEnable(true)
, m_bMassDefault(true)
, m_bMomentDefault(true)
, m_fMass(MASS_DEFAULT)
, m_fArea(0.0f)
, m_fDensity(0.0f)
, m_fMoment(MOMENT_DEFAULT)
, m_fLinearDamping(0.0f)
, m_fAngularDamping(0.0f)
, m_nTag(0)
, m_nCategoryBitmask(UINT_MAX)
, m_nCollisionBitmask(UINT_MAX)
, m_nContactTestBitmask(UINT_MAX)
, m_nGroup(0)
{
}

PhysicsBody::~PhysicsBody()
{
    for (auto it = m_aJoints.begin(); it != m_aJoints.end(); ++it)
    {
        PhysicsJoint* joint = *it;
        
        PhysicsBody* other = joint->getBodyA() == this ? joint->getBodyB() : joint->getBodyA();
        other->removeJoint(joint);
        delete joint;
    }
    
    CC_SAFE_DELETE(m_pInfo);
}

PhysicsBody* PhysicsBody::create()
{
    PhysicsBody* body = new PhysicsBody();
    if (body && body->init())
    {
        body->autorelease();
        return body;
    }
    
    CC_SAFE_DELETE(body);
    return nullptr;
}

PhysicsBody* PhysicsBody::create(float mass)
{
    PhysicsBody* body = new PhysicsBody();
    if (body)
    {
        body->m_fMass = mass;
        body->m_bMassDefault = false;
        if (body->init())
        {
            body->autorelease();
            return body;
        }
    }
    
    CC_SAFE_DELETE(body);
    return nullptr;
}

PhysicsBody* PhysicsBody::create(float mass, float moment)
{
    PhysicsBody* body = new PhysicsBody();
    if (body)
    {
        body->m_fMass = mass;
        body->m_bMassDefault = false;
        body->m_fMoment = moment;
        body->m_bMomentDefault = false;
        if (body->init())
        {
            body->autorelease();
            return body;
        }
    }
    
    CC_SAFE_DELETE(body);
    return nullptr;
    
}

PhysicsBody* PhysicsBody::createCircle(float radius, const PhysicsMaterial& material, const Point& offset)
{
    PhysicsBody* body = new PhysicsBody();
    if (body && body->init())
    {
        body->addShape(PhysicsShapeCircle::create(radius, material, offset));
        body->autorelease();
        return body;
    }
    
    CC_SAFE_DELETE(body);
    return nullptr;
}

PhysicsBody* PhysicsBody::createBox(const Size& size, const PhysicsMaterial& material, const Point& offset)
{
    PhysicsBody* body = new PhysicsBody();
    if (body && body->init())
    {
        body->addShape(PhysicsShapeBox::create(size, material, offset));
        body->autorelease();
        return body;
    }
    
    CC_SAFE_DELETE(body);
    return nullptr;
}

PhysicsBody* PhysicsBody::createPolygon(const Point* points, int count, const PhysicsMaterial& material, const Point& offset)
{
    PhysicsBody* body = new PhysicsBody();
    if (body && body->init())
    {
        body->addShape(PhysicsShapePolygon::create(points, count, material, offset));
        body->autorelease();
        return body;
    }
    
    CC_SAFE_DELETE(body);
    return nullptr;
}

PhysicsBody* PhysicsBody::createEdgeSegment(const Point& a, const Point& b, const PhysicsMaterial& material, float border/* = 1*/)
{
    PhysicsBody* body = new PhysicsBody();
    if (body && body->init())
    {
        body->addShape(PhysicsShapeEdgeSegment::create(a, b, material, border));
        body->m_bDynamic = false;
        body->autorelease();
        return body;
    }
    
    CC_SAFE_DELETE(body);
    return nullptr;
}

PhysicsBody* PhysicsBody::createEdgeBox(const Size& size, const PhysicsMaterial& material, float border/* = 1*/, const Point& offset)
{
    PhysicsBody* body = new PhysicsBody();
    if (body && body->init())
    {
        body->addShape(PhysicsShapeEdgeBox::create(size, material, border, offset));
        body->m_bDynamic = false;
        body->autorelease();
        return body;
    }
    
    CC_SAFE_DELETE(body);
    
    return nullptr;
}

PhysicsBody* PhysicsBody::createEdgePolygon(const Point* points, int count, const PhysicsMaterial& material, float border/* = 1*/)
{
    PhysicsBody* body = new PhysicsBody();
    if (body && body->init())
    {
        body->addShape(PhysicsShapeEdgePolygon::create(points, count, material, border));
        body->m_bDynamic = false;
        body->autorelease();
        return body;
    }
    
    CC_SAFE_DELETE(body);
    
    return nullptr;
}

PhysicsBody* PhysicsBody::createEdgeChain(const Point* points, int count, const PhysicsMaterial& material, float border/* = 1*/)
{
    PhysicsBody* body = new PhysicsBody();
    if (body && body->init())
    {
        body->addShape(PhysicsShapeEdgeChain::create(points, count, material, border));
        body->m_bDynamic = false;
        body->autorelease();
        return body;
    }
    
    CC_SAFE_DELETE(body);
    
    return nullptr;
}

bool PhysicsBody::init()
{
    do
    {
        m_pInfo = new PhysicsBodyInfo();
        CC_BREAK_IF(m_pInfo == nullptr);
        m_pShapes = Array::create();
        CC_BREAK_IF(m_pShapes == nullptr);
        m_pShapes->retain();
        
        m_pInfo->setBody(cpBodyNew(PhysicsHelper::float2cpfloat(m_fMass), PhysicsHelper::float2cpfloat(m_fMoment)));
        
        CC_BREAK_IF(m_pInfo->getBody() == nullptr);
        
        return true;
    } while (false);
    
    return false;
}

void PhysicsBody::removeJoint(PhysicsJoint* joint)
{
    auto it = std::find(m_aJoints.begin(), m_aJoints.end(), joint);
    
    if (it != m_aJoints.end())
    {
        m_aJoints.erase(it);
    }
}

void PhysicsBody::setDynamic(bool dynamic)
{
    if (dynamic != m_bDynamic)
    {
        m_bDynamic = dynamic;
        if (dynamic)
        {
            cpBodySetMass(m_pInfo->getBody(), m_fMass);
            
            if (m_pWorld != nullptr)
            {
                cpSpaceAddBody(m_pWorld->m_pInfo->getSpace(), m_pInfo->getBody());
            }
        }else
        {
            cpBodySetMass(m_pInfo->getBody(), PHYSICS_INFINITY);
            
            if (m_pWorld != nullptr)
            {
                cpSpaceRemoveBody(m_pWorld->m_pInfo->getSpace(), m_pInfo->getBody());
            }
        }
        
    }
}

void PhysicsBody::setRotationEnable(bool enable)
{
    if (m_bRotationEnable != enable)
    {
        cpBodySetMoment(m_pInfo->getBody(), enable ? m_fMoment : PHYSICS_INFINITY);
        m_bRotationEnable = enable;
    }
}

void PhysicsBody::setGravityEnable(bool enable)
{
    if (m_bGravityEnable != enable)
    {
        m_bGravityEnable = enable;
        
        if (m_pWorld != nullptr)
        {
            if (enable)
            {
                applyForce(m_pWorld->getGravity() * m_fMass);
            }else
            {
                applyForce(-m_pWorld->getGravity() * m_fMass);
            }
        }
    }
}

void PhysicsBody::setPosition(Point position)
{
    cpBodySetPos(m_pInfo->getBody(), PhysicsHelper::point2cpv(position));
}

void PhysicsBody::setRotation(float rotation)
{
    cpBodySetAngle(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(rotation));
}

Point PhysicsBody::getPosition() const
{
    cpVect vec = cpBodyGetPos(m_pInfo->getBody());
    return PhysicsHelper::cpv2point(vec);
}

float PhysicsBody::getRotation() const
{
    return -PhysicsHelper::cpfloat2float(cpBodyGetAngle(m_pInfo->getBody()) / 3.14f * 180.0f);
}

PhysicsShape* PhysicsBody::addShape(PhysicsShape* shape, bool addMassAndMoment/* = true*/)
{
    if (shape == nullptr) return nullptr;
    
    // add shape to body
    if (m_pShapes->getIndexOfObject(shape) == CC_INVALID_INDEX)
    {
        shape->setBody(this);
        
        // calculate the area, mass, and desity
        // area must update before mass, because the density changes depend on it.
        if (addMassAndMoment)
        {
            m_fArea += shape->getArea();
            addMass(shape->getMass());
            addMoment(shape->getMoment());
        }
        
        if (m_pWorld != nullptr)
        {
            m_pWorld->addShape(shape);
        }
        
        m_pShapes->addObject(shape);
        
        if (m_nGroup != CP_NO_GROUP && shape->getGroup() == CP_NO_GROUP)
        {
            shape->setGroup(m_nGroup);
        }
    }
    
    return shape;
}

void PhysicsBody::applyForce(const Vect& force)
{
    applyForce(force, Point::ZERO);
}

void PhysicsBody::applyForce(const Vect& force, const Point& offset)
{
    cpBodyApplyForce(m_pInfo->getBody(), PhysicsHelper::point2cpv(force), PhysicsHelper::point2cpv(offset));
}

void PhysicsBody::resetForces()
{
    cpBodyResetForces(m_pInfo->getBody());
    
    // if _gravityEnable is false, add a reverse of gravity force to body
    if (m_pWorld != nullptr && !m_bGravityEnable)
    {
        applyForce(-m_pWorld->getGravity() * m_fMass);
    }
}

void PhysicsBody::applyImpulse(const Vect& impulse)
{
    applyImpulse(impulse, Point());
}

void PhysicsBody::applyImpulse(const Vect& impulse, const Point& offset)
{
    cpBodyApplyImpulse(m_pInfo->getBody(), PhysicsHelper::point2cpv(impulse), PhysicsHelper::point2cpv(offset));
}

void PhysicsBody::applyTorque(float torque)
{
    cpBodySetTorque(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(torque));
}

void PhysicsBody::setMass(float mass)
{
    if (mass <= 0)
    {
        return;
    }
    
    m_fMass = mass;
    m_bMassDefault = false;
    
    // update density
    if (m_fMass == PHYSICS_INFINITY)
    {
        m_fDensity = PHYSICS_INFINITY;
    }
    else
    {
        if (m_fArea > 0)
        {
            m_fDensity = m_fMass / m_fArea;
        }else
        {
            m_fDensity = 0;
        }
    }
    
    cpBodySetMass(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(m_fMass));
}

void PhysicsBody::addMass(float mass)
{
    if (mass == PHYSICS_INFINITY)
    {
        m_fMass = PHYSICS_INFINITY;
        m_bMassDefault = false;
        m_fDensity = PHYSICS_INFINITY;
    }
    else if (mass == -PHYSICS_INFINITY)
    {
        return;
    }
    else if (m_fMass != PHYSICS_INFINITY)
    {
        if (m_bMassDefault)
        {
            m_fMass = 0;
            m_bMassDefault = false;
        }
        
        if (m_fMass + mass > 0)
        {
            m_fMass +=  mass;
        }else
        {
            m_fMass = MASS_DEFAULT;
            m_bMassDefault = true;
        }
        
        if (m_fArea > 0)
        {
            m_fDensity = m_fMass / m_fArea;
        }
        else
        {
            m_fDensity = 0;
        }
    }
    
    cpBodySetMass(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(m_fMass));
}

void PhysicsBody::addMoment(float moment)
{
    if (moment == PHYSICS_INFINITY)
    {
        // if moment is INFINITY, the moment of the body will become INFINITY
        m_fMoment = PHYSICS_INFINITY;
        m_bMomentDefault = false;
    }
    else if (moment == -PHYSICS_INFINITY)
    {
        // if moment is -INFINITY, it won't change
        return;
    }
    else
    {
        // if moment of the body is INFINITY is has no effect
        if (m_fMoment != PHYSICS_INFINITY)
        {
            if (m_bMomentDefault)
            {
                m_fMoment = 0;
                m_bMomentDefault = false;
            }
            
            if (m_fMoment + moment > 0)
            {
                m_fMoment += moment;
            }
            else
            {
                m_fMoment = MOMENT_DEFAULT;
                m_bMomentDefault = true;
            }
        }
    }
    
    if (m_bRotationEnable)
    {
        cpBodySetMoment(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(m_fMoment));
    }
}

void PhysicsBody::setVelocity(const Point& velocity)
{
    cpBodySetVel(m_pInfo->getBody(), PhysicsHelper::point2cpv(velocity));
}

Point PhysicsBody::getVelocity()
{
    return PhysicsHelper::cpv2point(cpBodyGetVel(m_pInfo->getBody()));
}

Point PhysicsBody::getVelocityAtLocalPoint(const Point& point)
{
    return PhysicsHelper::cpv2point(cpBodyGetVelAtLocalPoint(m_pInfo->getBody(), PhysicsHelper::point2cpv(point)));
}

Point PhysicsBody::getVelocityAtWorldPoint(const Point& point)
{
    return PhysicsHelper::cpv2point(cpBodyGetVelAtWorldPoint(m_pInfo->getBody(), PhysicsHelper::point2cpv(point)));
}

void PhysicsBody::setAngularVelocity(float velocity)
{
    cpBodySetAngVel(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(velocity));
}

float PhysicsBody::getAngularVelocity()
{
    return PhysicsHelper::cpfloat2float(cpBodyGetAngVel(m_pInfo->getBody()));
}

void PhysicsBody::setVelocityLimit(float limit)
{
    cpBodySetVelLimit(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(limit));
}

float PhysicsBody::getVelocityLimit()
{
    return PhysicsHelper::cpfloat2float(cpBodyGetVelLimit(m_pInfo->getBody()));
}

void PhysicsBody::setAngularVelocityLimit(float limit)
{
    cpBodySetVelLimit(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(limit));
}

float PhysicsBody::getAngularVelocityLimit()
{
    return PhysicsHelper::cpfloat2float(cpBodyGetAngVelLimit(m_pInfo->getBody()));
}

void PhysicsBody::setMoment(float moment)
{
    m_fMoment = moment;
    m_bMomentDefault = false;
    
    if (m_bRotationEnable)
    {
        cpBodySetMoment(m_pInfo->getBody(), PhysicsHelper::float2cpfloat(m_fMoment));
    }
}

PhysicsShape* PhysicsBody::getShape(int tag) const
{
    for (auto child : *m_pShapes)
    {
        PhysicsShape* shape = dynamic_cast<PhysicsShape*>(child);
        if (shape->getTag() == tag)
        {
            return shape;
        }
    }
    
    return nullptr;
}

void PhysicsBody::removeShape(int tag, bool reduceMassAndMoment/* = true*/)
{
    for (auto child : *m_pShapes)
    {
        PhysicsShape* shape = dynamic_cast<PhysicsShape*>(child);
        if (shape->getTag() == tag)
        {
            removeShape(shape, reduceMassAndMoment);
            return;
        }
    }
}

void PhysicsBody::removeShape(PhysicsShape* shape, bool reduceMassAndMoment/* = true*/)
{
    if (m_pShapes->getIndexOfObject(shape) != CC_INVALID_INDEX)
    {
        // deduce the area, mass and moment
        // area must update before mass, because the density changes depend on it.
        if (reduceMassAndMoment)
        {
            m_fArea -= shape->getArea();
            addMass(-shape->getMass());
            addMoment(-shape->getMoment());
        }
        
        //remove
        if (m_pWorld)
        {
            m_pWorld->removeShape(shape);
        }
        
        // set shape->_body = nullptr make the shape->setBody will not trigger the _body->removeShape function call.
        shape->m_pBody = nullptr;
        shape->setBody(nullptr);
        m_pShapes->removeObject(shape);
    }
}

void PhysicsBody::removeAllShapes(bool reduceMassAndMoment/* = true*/)
{
    for (auto child : *m_pShapes)
    {
        PhysicsShape* shape = dynamic_cast<PhysicsShape*>(child);
        
        // deduce the area, mass and moment
        // area must update before mass, because the density changes depend on it.
        if (reduceMassAndMoment)
        {
            m_fArea -= shape->getArea();
            addMass(-shape->getMass());
            addMoment(-shape->getMoment());
        }
        
        if (m_pWorld)
        {
            m_pWorld->removeShape(shape);
        }
        
        // set shape->_body = nullptr make the shape->setBody will not trigger the _body->removeShape function call.
        shape->m_pBody = nullptr;
        shape->setBody(nullptr);
    }
    
    m_pShapes->removeAllObjects();
}

void PhysicsBody::removeFromWorld()
{
    if (m_pWorld)
    {
        m_pWorld->removeBody(this);
    }
}

void PhysicsBody::setEnable(bool enable)
{
    if (m_bEnable != enable)
    {
        m_bEnable = enable;
        
        if (m_pWorld)
        {
            if (enable)
            {
                m_pWorld->addBodyOrDelay(this);
            }else
            {
                m_pWorld->removeBodyOrDelay(this);
            }
        }
    }
}

bool PhysicsBody::isResting() const
{
    return cpBodyIsSleeping(m_pInfo->getBody()) == cpTrue;
}

void PhysicsBody::update(float delta)
{
    // damping compute
    if (m_bDynamic)
    {
        m_pInfo->getBody()->v.x *= cpfclamp(1.0f - delta * m_fLinearDamping, 0.0f, 1.0f);
        m_pInfo->getBody()->v.y *= cpfclamp(1.0f - delta * m_fLinearDamping, 0.0f, 1.0f);
        m_pInfo->getBody()->w *= cpfclamp(1.0f - delta * m_fAngularDamping, 0.0f, 1.0f);
    }
}

void PhysicsBody::setCategoryBitmask(int bitmask)
{
    m_nCategoryBitmask = bitmask;
    
    for (auto shape : *m_pShapes)
    {
        ((PhysicsShape*)shape)->setCategoryBitmask(bitmask);
    }
}

void PhysicsBody::setContactTestBitmask(int bitmask)
{
    m_nContactTestBitmask = bitmask;
    
    for (auto shape : *m_pShapes)
    {
        ((PhysicsShape*)shape)->setContactTestBitmask(bitmask);
    }
}

void PhysicsBody::setCollisionBitmask(int bitmask)
{
    m_nCollisionBitmask = bitmask;
    
    for (auto shape : *m_pShapes)
    {
        ((PhysicsShape*)shape)->setCollisionBitmask(bitmask);
    }
}

void PhysicsBody::setGroup(int group)
{
    for (auto shape : *m_pShapes)
    {
        ((PhysicsShape*)shape)->setGroup(group);
    }
}

Point PhysicsBody::world2Local(const Point& point)
{
    return PhysicsHelper::cpv2point(cpBodyWorld2Local(m_pInfo->getBody(), PhysicsHelper::point2cpv(point)));
}

Point PhysicsBody::local2World(const Point& point)
{
    return PhysicsHelper::cpv2point(cpBodyLocal2World(m_pInfo->getBody(), PhysicsHelper::point2cpv(point)));
}

#elif (CC_PHYSICS_ENGINE == CC_PHYSICS_BOX2D)


#endif

NS_CC_END

#endif // CC_USE_PHYSICS
