/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsWorld.cpp
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

#include "physics/CCPhysicsWorld.h"
#ifdef CC_USE_PHYSICS

#include <climits>

#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
#include "XMChipmunk/chipmunk.h"
#elif (CC_PHYSICS_ENGINE == CCPHYSICS_BOX2D)
#include "XMBox2D/Box2D.h"
#endif

#include "physics/CCPhysicsBody.h"
#include "physics/CCPhysicsShape.h"
#include "physics/CCPhysicsContact.h"
#include "physics/CCPhysicsJoint.h"
#include "physics/CCPhysicsContact.h"

#include "physics/chipmunk/CCPhysicsWorldInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsWorldInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsBodyInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsBodyInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsShapeInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsShapeInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsContactInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsContactInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsJointInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsJointInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsHelper_chipmunk.h"

#include "2d/draw_nodes/CCDrawNode.h"
#include "base/CCArray.h"
#include "2d/layers_scenes_transitions_nodes/CCScene.h"
#include "2d/CCDirector.h"
#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "2d/event_dispatcher/CCEventCustom.h"

#include <algorithm>

NS_CC_BEGIN

extern const char* PHYSICSCONTACT_EVENT_NAME;

#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)

const float PHYSICS_INFINITY = INFINITY;

namespace
{
    typedef struct RayCastCallbackInfo
    {
        PhysicsWorld* world;
        PhysicsRayCastCallbackFunc func;
        Point p1;
        Point p2;
        void* data;
    }RayCastCallbackInfo;
    
    typedef struct RectQueryCallbackInfo
    {
        PhysicsWorld* world;
        PhysicsRectQueryCallbackFunc func;
        void* data;
    }RectQueryCallbackInfo;
    
    typedef struct PointQueryCallbackInfo
    {
        PhysicsWorld* world;
        PhysicsPointQueryCallbackFunc func;
        void* data;
    }PointQueryCallbackInfo;
}

class PhysicsWorldCallback
{
public:
    static int collisionBeginCallbackFunc(cpArbiter *arb, struct cpSpace *space, PhysicsWorld *world);
    static int collisionPreSolveCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world);
    static void collisionPostSolveCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world);
    static void collisionSeparateCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world);
    static void rayCastCallbackFunc(cpShape *shape, cpFloat t, cpVect n, RayCastCallbackInfo *info);
    static void queryRectCallbackFunc(cpShape *shape, RectQueryCallbackInfo *info);
    static void queryPointFunc(cpShape *shape, cpFloat distance, cpVect point, PointQueryCallbackInfo *info);
    static void getShapesAtPointFunc(cpShape *shape, cpFloat distance, cpVect point, Array *arr);
    
public:
    static bool continues;
};

bool PhysicsWorldCallback::continues = true;

int PhysicsWorldCallback::collisionBeginCallbackFunc(cpArbiter *arb, struct cpSpace *space, PhysicsWorld *world)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);
    
    auto ita = PhysicsShapeInfo::getMap().find(a);
    auto itb = PhysicsShapeInfo::getMap().find(b);
    CC_ASSERT(ita != PhysicsShapeInfo::getMap().end() && itb != PhysicsShapeInfo::getMap().end());
    
    PhysicsContact* contact = PhysicsContact::construct(ita->second->getShape(), itb->second->getShape());
    arb->data = contact;
    contact->m_pContactInfo = arb;
    
    return world->collisionBeginCallback(*contact);
}

int PhysicsWorldCallback::collisionPreSolveCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world)
{
    return world->collisionPreSolveCallback(*static_cast<PhysicsContact*>(arb->data));
}

void PhysicsWorldCallback::collisionPostSolveCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world)
{
    world->collisionPostSolveCallback(*static_cast<PhysicsContact*>(arb->data));
}

void PhysicsWorldCallback::collisionSeparateCallbackFunc(cpArbiter *arb, cpSpace *space, PhysicsWorld *world)
{
    PhysicsContact* contact = static_cast<PhysicsContact*>(arb->data);
    
    world->collisionSeparateCallback(*contact);
    
    delete contact;
}

void PhysicsWorldCallback::rayCastCallbackFunc(cpShape *shape, cpFloat t, cpVect n, RayCastCallbackInfo *info)
{
    if (!PhysicsWorldCallback::continues)
    {
        return;
    }
    
    auto it = PhysicsShapeInfo::getMap().find(shape);
    CC_ASSERT(it != PhysicsShapeInfo::getMap().end());
    
    PhysicsRayCastInfo callbackInfo =
    {
        it->second->getShape(),
        info->p1,
        info->p2,
        Point(info->p1.x+(info->p2.x-info->p1.x)*t, info->p1.y+(info->p2.y-info->p1.y)*t),
        Point(n.x, n.y),
        (float)t,
    };
    
    PhysicsWorldCallback::continues = info->func(*info->world, callbackInfo, info->data);
}

void PhysicsWorldCallback::queryRectCallbackFunc(cpShape *shape, RectQueryCallbackInfo *info)
{
    auto it = PhysicsShapeInfo::getMap().find(shape);
    
    CC_ASSERT(it != PhysicsShapeInfo::getMap().end());
    
    if (!PhysicsWorldCallback::continues)
    {
        return;
    }
    
    PhysicsWorldCallback::continues = info->func(*info->world, *it->second->getShape(), info->data);
}

void PhysicsWorldCallback::getShapesAtPointFunc(cpShape *shape, cpFloat distance, cpVect point, Array *arr)
{
    auto it = PhysicsShapeInfo::getMap().find(shape);
    
    CC_ASSERT(it != PhysicsShapeInfo::getMap().end());
    
    arr->addObject(it->second->getShape());
}

void PhysicsWorldCallback::queryPointFunc(cpShape *shape, cpFloat distance, cpVect point, PointQueryCallbackInfo *info)
{
    auto it = PhysicsShapeInfo::getMap().find(shape);
    
    CC_ASSERT(it != PhysicsShapeInfo::getMap().end());
    
    PhysicsWorldCallback::continues = info->func(*info->world, *it->second->getShape(), info->data);
}

bool PhysicsWorld::init(Scene& scene)
{
    do
    {
        m_pDelayAddBodies = Array::create();
        m_pDelayRemoveBodies = Array::create();
        CC_BREAK_IF(m_pDelayAddBodies == nullptr || m_pDelayRemoveBodies == nullptr);
        m_pDelayAddBodies->retain();
        m_pDelayRemoveBodies->retain();
        
        m_pInfo = new PhysicsWorldInfo();
        CC_BREAK_IF(m_pInfo == nullptr);
        m_pBodies = Array::create();
        CC_BREAK_IF(m_pBodies == nullptr);
        m_pBodies->retain();

        m_pScene = &scene;
        
        cpSpaceSetGravity(m_pInfo->getSpace(), PhysicsHelper::point2cpv(m_tGravity));
        
        cpSpaceSetDefaultCollisionHandler(m_pInfo->getSpace(),
                                          (cpCollisionBeginFunc)PhysicsWorldCallback::collisionBeginCallbackFunc,
                                          (cpCollisionPreSolveFunc)PhysicsWorldCallback::collisionPreSolveCallbackFunc,
                                          (cpCollisionPostSolveFunc)PhysicsWorldCallback::collisionPostSolveCallbackFunc,
                                          (cpCollisionSeparateFunc)PhysicsWorldCallback::collisionSeparateCallbackFunc,
                                          this);
        
        return true;
    } while (false);
    
    return false;
}

void PhysicsWorld::addBodyOrDelay(PhysicsBody* body)
{
    if (m_pDelayRemoveBodies->getIndexOfObject(body) != CC_INVALID_INDEX)
    {
        m_pDelayRemoveBodies->removeObject(body);
        return;
    }
    
    if (m_pInfo->getSpace()->locked)
    {
        if (m_pDelayAddBodies->getIndexOfObject(body) == CC_INVALID_INDEX)
        {
            m_pDelayAddBodies->addObject(body);
            m_bDelayDirty = true;
        }
    }else
    {
        doAddBody(body);
    }
}

void PhysicsWorld::removeBodyOrDelay(PhysicsBody* body)
{
    if (m_pDelayAddBodies->getIndexOfObject(body) != CC_INVALID_INDEX)
    {
        m_pDelayAddBodies->removeObject(body);
        return;
    }
    
    if (m_pInfo->getSpace()->locked)
    {
        if (m_pDelayRemoveBodies->getIndexOfObject(body) == CC_INVALID_INDEX)
        {
            m_pDelayRemoveBodies->addObject(body);
            m_bDelayDirty = true;
        }
    }else
    {
        doRemoveBody(body);
    }
}

void PhysicsWorld::addJointOrDelay(PhysicsJoint* joint)
{
    auto it = std::find(m_aDelayRemoveJoints.begin(), m_aDelayRemoveJoints.end(), joint);
    if (it != m_aDelayRemoveJoints.end())
    {
        m_aDelayRemoveJoints.erase(it);
        return;
    }
    
    if (m_pInfo->getSpace()->locked)
    {
        if (std::find(m_aDelayAddJoints.begin(), m_aDelayAddJoints.end(), joint) == m_aDelayAddJoints.end())
        {
            m_aDelayAddJoints.push_back(joint);
            m_bDelayDirty = true;
        }
    }else
    {
        doAddJoint(joint);
    }
}

void PhysicsWorld::removeJointOrDelay(PhysicsJoint* joint)
{
    auto it = std::find(m_aDelayAddJoints.begin(), m_aDelayAddJoints.end(), joint);
    if (it != m_aDelayAddJoints.end())
    {
        m_aDelayAddJoints.erase(it);
        return;
    }
    
    if (m_pInfo->getSpace()->locked)
    {
        if (std::find(m_aDelayRemoveJoints.begin(), m_aDelayRemoveJoints.end(), joint) == m_aDelayRemoveJoints.end())
        {
            m_aDelayRemoveJoints.push_back(joint);
            m_bDelayDirty = true;
        }
    }else
    {
        doRemoveJoint(joint);
    }
}

void PhysicsWorld::addJoint(PhysicsJoint* joint)
{
    if (joint->getWorld() != nullptr && joint->getWorld() != this)
    {
        joint->removeFormWorld();
    }
    
    addJointOrDelay(joint);
    m_aJoints.push_back(joint);
    joint->m_pWorld = this;
}

void PhysicsWorld::removeJoint(PhysicsJoint* joint, bool destroy)
{
    if (joint->getWorld() != this)
    {
        if (destroy)
        {
            CCLOG("physics warnning: the joint is not in this world, it won't be destoried utill the body it conntect is destoried");
        }
        return;
    }
    
    removeJointOrDelay(joint);
    
    m_aJoints.remove(joint);
    joint->m_pWorld = nullptr;
    
    // clean the connection to this joint
    if (destroy)
    {
        if (joint->getBodyA() != nullptr)
        {
            joint->getBodyA()->removeJoint(joint);
        }
        
        if (joint->getBodyB() != nullptr)
        {
            joint->getBodyB()->removeJoint(joint);
        }
        
        // test the distraction is delaied or not
        if (m_aDelayRemoveJoints.size() > 0 && m_aDelayRemoveJoints.back() == joint)
        {
            joint->m_bDestoryMark = true;
        }
        else
        {
            delete joint;
        }
    }
}

void PhysicsWorld::removeAllJoints(bool destroy)
{
    for (auto joint : m_aJoints)
    {
        removeJointOrDelay(joint);
        joint->m_pWorld = nullptr;
        
        // clean the connection to this joint
        if (destroy)
        {
            if (joint->getBodyA() != nullptr)
            {
                joint->getBodyA()->removeJoint(joint);
            }
            
            if (joint->getBodyB() != nullptr)
            {
                joint->getBodyB()->removeJoint(joint);
            }
            
            // test the distraction is delaied or not
            if (m_aDelayRemoveJoints.size() > 0 && m_aDelayRemoveJoints.back() == joint)
            {
                joint->m_bDestoryMark = true;
            }
            else
            {
                delete joint;
            }
        }
    }
    
    m_aJoints.clear();
}

void PhysicsWorld::addShape(PhysicsShape* shape)
{
    for (auto cps : shape->m_pInfo->getShapes())
    {
        m_pInfo->addShape(cps);
    }
    
    return;
}

void PhysicsWorld::doAddJoint(PhysicsJoint *joint)
{
    for (auto subjoint : joint->m_pInfo->getJoints())
    {
        m_pInfo->addJoint(subjoint);
    }
}

void PhysicsWorld::doAddBody(PhysicsBody* body)
{
    if (body->isEnabled())
    {
        //is gravity enable
        if (!body->isGravityEnabled())
        {
            body->applyForce(-m_tGravity);
        }
        
        // add body to space
        if (body->isDynamic())
        {
            m_pInfo->addBody(body->m_pInfo->getBody());
        }
        
        // add shapes to space
        for (auto shape : *body->getShapes())
        {
            addShape(dynamic_cast<PhysicsShape*>(shape));
        }
    }
}

void PhysicsWorld::addBody(PhysicsBody* body)
{
    CCASSERT(body != nullptr, "the body can not be nullptr");
    
    if (body->getWorld() == this)
    {
        return;
    }
    
    if (body->getWorld() != nullptr)
    {
        body->removeFromWorld();
    }
    
    addBodyOrDelay(body);
    m_pBodies->addObject(body);
    body->m_pWorld = this;
}

void PhysicsWorld::removeBody(PhysicsBody* body)
{
    
    if (body->getWorld() != this)
    {
        CCLOG("Physics Warnning: this body doesn't belong to this world");
        return;
    }
    
    // destory the body's joints
    for (auto joint : body->m_aJoints)
    {
        removeJoint(joint, true);
    }
    
    removeBodyOrDelay(body);
    m_pBodies->removeObject(body);
    body->m_pWorld = nullptr;
}

void PhysicsWorld::removeBody(int tag)
{
    for (Object* obj : *m_pBodies)
    {
        PhysicsBody* body = dynamic_cast<PhysicsBody*>(obj);
        if (body->getTag() == tag)
        {
            removeBody(body);
            return;
        }
    }
}

void PhysicsWorld::doRemoveBody(PhysicsBody* body)
{
    CCASSERT(body != nullptr, "the body can not be nullptr");
    
    // reset the gravity
    if (!body->isGravityEnabled())
    {
        body->applyForce(-m_tGravity);
    }
    
    // remove shaps
    for (auto shape : *body->getShapes())
    {
        removeShape(dynamic_cast<PhysicsShape*>(shape));
    }
    
    // remove body
    m_pInfo->removeBody(body->m_pInfo->getBody());
}

void PhysicsWorld::doRemoveJoint(PhysicsJoint* joint)
{
    for (auto subjoint : joint->m_pInfo->getJoints())
    {
        m_pInfo->removeJoint(subjoint);
    }
}

void PhysicsWorld::removeAllBodies()
{
    for (Object* obj : *m_pBodies)
    {
        PhysicsBody* child = dynamic_cast<PhysicsBody*>(obj);
        removeBodyOrDelay(child);
        child->m_pWorld = nullptr;
    }

    m_pBodies->removeAllObjects();
    CC_SAFE_RELEASE(m_pBodies);
}

void PhysicsWorld::removeShape(PhysicsShape* shape)
{
    for (auto cps : shape->m_pInfo->getShapes())
    {
        if (cpSpaceContainsShape(m_pInfo->getSpace(), cps))
        {
            cpSpaceRemoveShape(m_pInfo->getSpace(), cps);
        }
    }
}

void PhysicsWorld::updateBodies()
{
    if (m_pInfo->getSpace()->locked)
    {
        return;
    }
    
    for (auto body : *m_pDelayAddBodies)
    {
        doAddBody(dynamic_cast<PhysicsBody*>(body));
    }
    
    for (auto body : *m_pDelayRemoveBodies)
    {
        doRemoveBody(dynamic_cast<PhysicsBody*>(body));
    }
    
    m_pDelayAddBodies->removeAllObjects();
    m_pDelayRemoveBodies->removeAllObjects();
}

void PhysicsWorld::updateJoints()
{
    if (m_pInfo->getSpace()->locked)
    {
        return;
    }
    
    for (auto joint : m_aDelayAddJoints)
    {
        doAddJoint(joint);
    }
    
    for (auto joint : m_aDelayRemoveJoints)
    {
        doRemoveJoint(joint);
        
        if (joint->m_bDestoryMark)
        {
            delete joint;
        }
    }
    
    m_aDelayAddJoints.clear();
    m_aDelayRemoveJoints.clear();
}

void PhysicsWorld::update(float delta)
{
    if (m_bDelayDirty)
    {
        // the updateJoints must run before the updateBodies.
        updateJoints();
        updateBodies();
        m_bDelayDirty = !(m_pDelayAddBodies->count() == 0 && m_pDelayRemoveBodies->count() == 0 && m_aDelayAddJoints.size() == 0 && m_aDelayRemoveJoints.size() == 0);
    }
    
    for (auto body : *m_pBodies)
    {
        body->update(delta);
    }
    
    cpSpaceStep(m_pInfo->getSpace(), delta);
    
    if (m_nDebugDrawMask != DEBUGDRAW_NONE)
    {
        debugDraw();
    }
}

void PhysicsWorld::debugDraw()
{
    if (m_pDebugDraw == nullptr)
    {
        m_pDebugDraw = new PhysicsDebugDraw(*this);
    }
    
    if (m_pDebugDraw && m_pBodies != nullptr)
    {
        if (m_pDebugDraw->begin())
        {
            if (m_nDebugDrawMask & DEBUGDRAW_SHAPE)
            {
                for (Object* obj : *m_pBodies)
                {
                    PhysicsBody* body = dynamic_cast<PhysicsBody*>(obj);
                    
                    for (auto shape : *body->getShapes())
                    {
                        m_pDebugDraw->drawShape(*dynamic_cast<PhysicsShape*>(shape));
                    }
                }
            }
            
            if (m_nDebugDrawMask & DEBUGDRAW_JOINT)
            {
                for (auto joint : m_aJoints)
                {
                    m_pDebugDraw->drawJoint(*joint);
                }
            }
            
            m_pDebugDraw->end();
        }
    }
}

void PhysicsWorld::setDebugDrawMask(int mask)
{
    if (mask == DEBUGDRAW_NONE)
    {
        CC_SAFE_DELETE(m_pDebugDraw);
    }
    
    m_nDebugDrawMask = mask;
}

int PhysicsWorld::collisionBeginCallback(PhysicsContact& contact)
{
    bool ret = true;
    
    PhysicsShape* shapeA = contact.getShapeA();
    PhysicsShape* shapeB = contact.getShapeB();
    PhysicsBody* bodyA = shapeA->getBody();
    PhysicsBody* bodyB = shapeB->getBody();
    std::vector<PhysicsJoint*> jointsA = bodyA->getJoints();
    
    // check the joint is collision enable or not
    for (PhysicsJoint* joint : jointsA)
    {
        if (std::find(m_aJoints.begin(), m_aJoints.end(), joint) == m_aJoints.end())
        {
            continue;
        }
        
        if (!joint->isCollisionEnabled())
        {
            PhysicsBody* body = joint->getBodyA() == bodyA ? joint->getBodyB() : joint->getBodyA();
            
            if (body == bodyB)
            {
                contact.setNotificationEnable(false);
                return false;
            }
        }
    }
    
    // bitmask check
    if ((shapeA->getCategoryBitmask() & shapeB->getContactTestBitmask()) == 0
        || (shapeB->getContactTestBitmask() & shapeA->getCategoryBitmask()) == 0)
    {
        contact.setNotificationEnable(false);
    }
    
    if (shapeA->getGroup() != 0 && shapeA->getGroup() == shapeB->getGroup())
    {
        ret = shapeA->getGroup() > 0;
    }
    else
    {
        if ((shapeA->getCategoryBitmask() & shapeB->getCollisionBitmask()) == 0
            || (shapeB->getCategoryBitmask() & shapeA->getCollisionBitmask()) == 0)
        {
            ret = false;
        }
    }
    
    contact.setEventCode(PhysicsContact::EventCode::BEGIN);
    contact.setWorld(this);
    EventCustom event(PHYSICSCONTACT_EVENT_NAME);
    event.setUserData(&contact);
    m_pScene->getEventDispatcher()->dispatchEvent(&event);
    
    return ret ? contact.resetResult() : false;
}

int PhysicsWorld::collisionPreSolveCallback(PhysicsContact& contact)
{
    if (!contact.isNotificationEnabled())
    {
        cpArbiterIgnore(static_cast<cpArbiter*>(contact.m_pContactInfo));
        return true;
    }
    
    contact.setEventCode(PhysicsContact::EventCode::PRESOLVE);
    contact.setWorld(this);
    EventCustom event(PHYSICSCONTACT_EVENT_NAME);
    event.setUserData(&contact);
    m_pScene->getEventDispatcher()->dispatchEvent(&event);
    
    return contact.resetResult();
}

void PhysicsWorld::collisionPostSolveCallback(PhysicsContact& contact)
{
    if (!contact.isNotificationEnabled())
    {
        return;
    }
    
    contact.setEventCode(PhysicsContact::EventCode::POSTSOLVE);
    contact.setWorld(this);
    EventCustom event(PHYSICSCONTACT_EVENT_NAME);
    event.setUserData(&contact);
    m_pScene->getEventDispatcher()->dispatchEvent(&event);
}

void PhysicsWorld::collisionSeparateCallback(PhysicsContact& contact)
{
    if (!contact.isNotificationEnabled())
    {
        return;
    }
    
    contact.setEventCode(PhysicsContact::EventCode::SEPERATE);
    contact.setWorld(this);
    EventCustom event(PHYSICSCONTACT_EVENT_NAME);
    event.setUserData(&contact);
    m_pScene->getEventDispatcher()->dispatchEvent(&event);
}

void PhysicsWorld::setGravity(const Vect& gravity)
{
    if (m_pBodies != nullptr)
    {
        for (auto child : *m_pBodies)
        {
            PhysicsBody* body = dynamic_cast<PhysicsBody*>(child);
            
            // reset gravity for body
            if (!body->isGravityEnabled())
            {
                body->applyForce(-m_tGravity);
                body->applyForce(gravity);
            }
        }
    }
    
    m_tGravity = gravity;
    cpSpaceSetGravity(m_pInfo->getSpace(), PhysicsHelper::point2cpv(gravity));
}


void PhysicsWorld::rayCast(PhysicsRayCastCallbackFunc func, const Point& point1, const Point& point2, void* data)
{
    CCASSERT(func != nullptr, "func shouldn't be nullptr");
    
    if (func != nullptr)
    {
        RayCastCallbackInfo info = { this, func, point1, point2, data };
        
        PhysicsWorldCallback::continues = true;
        cpSpaceSegmentQuery(this->m_pInfo->getSpace(),
                            PhysicsHelper::point2cpv(point1),
                            PhysicsHelper::point2cpv(point2),
                            CP_ALL_LAYERS,
                            CP_NO_GROUP,
                            (cpSpaceSegmentQueryFunc)PhysicsWorldCallback::rayCastCallbackFunc,
                            &info);
    }
}


void PhysicsWorld::queryRect(PhysicsRectQueryCallbackFunc func, const Rect& rect, void* data)
{
    CCASSERT(func != nullptr, "func shouldn't be nullptr");
    
    if (func != nullptr)
    {
        RectQueryCallbackInfo info = {this, func, data};
        
        PhysicsWorldCallback::continues = true;
        cpSpaceBBQuery(this->m_pInfo->getSpace(),
                       PhysicsHelper::rect2cpbb(rect),
                       CP_ALL_LAYERS,
                       CP_NO_GROUP,
                       (cpSpaceBBQueryFunc)PhysicsWorldCallback::queryRectCallbackFunc,
                       &info);
    }
}

void PhysicsWorld::queryPoint(PhysicsPointQueryCallbackFunc func, const Point& point, void* data)
{
    CCASSERT(func != nullptr, "func shouldn't be nullptr");
    
    if (func != nullptr)
    {
        PointQueryCallbackInfo info = {this, func, data};
        
        PhysicsWorldCallback::continues = true;
        cpSpaceNearestPointQuery(this->m_pInfo->getSpace(),
                                 PhysicsHelper::point2cpv(point),
                                 0,
                                 CP_ALL_LAYERS,
                                 CP_NO_GROUP,
                                 (cpSpaceNearestPointQueryFunc)PhysicsWorldCallback::queryPointFunc,
                                 &info);
    }
}

Array* PhysicsWorld::getShapes(const Point& point) const
{
    Array* arr = Array::create();
    cpSpaceNearestPointQuery(this->m_pInfo->getSpace(),
                             PhysicsHelper::point2cpv(point),
                             0,
                             CP_ALL_LAYERS,
                             CP_NO_GROUP,
                             (cpSpaceNearestPointQueryFunc)PhysicsWorldCallback::getShapesAtPointFunc,
                             arr);
    
    return arr;
}

PhysicsShape* PhysicsWorld::getShape(const Point& point) const
{
    cpShape* shape = cpSpaceNearestPointQueryNearest(this->m_pInfo->getSpace(),
                                    PhysicsHelper::point2cpv(point),
                                    0,
                                    CP_ALL_LAYERS,
                                    CP_NO_GROUP,
                                    nullptr);
    
    return shape == nullptr ? nullptr : PhysicsShapeInfo::getMap().find(shape)->second->getShape();
}

Array* PhysicsWorld::getAllBodies() const
{
    return m_pBodies;
}

PhysicsBody* PhysicsWorld::getBody(int tag) const
{
    for (auto body : *m_pBodies)
    {
        if (((PhysicsBody*)body)->getTag() == tag)
        {
            return (PhysicsBody*)body;
        }
    }
    
    return nullptr;
}



void PhysicsDebugDraw::drawShape(PhysicsShape& shape)
{
    for (auto it = shape.m_pInfo->getShapes().begin(); it != shape.m_pInfo->getShapes().end(); ++it)
    {
        cpShape *subShape = *it;
        
        switch ((*it)->klass->type)
        {
            case CP_CIRCLE_SHAPE:
            {
                float radius = PhysicsHelper::cpfloat2float(cpCircleShapeGetRadius(subShape));
                Point centre = PhysicsHelper::cpv2point(cpBodyGetPos(cpShapeGetBody(subShape)))
                + PhysicsHelper::cpv2point(cpCircleShapeGetOffset(subShape));
                
                static const int CIRCLE_SEG_NUM = 12;
                Point seg[CIRCLE_SEG_NUM] = {};
                
                for (int i = 0; i < CIRCLE_SEG_NUM; ++i)
                {
                    float angle = (float)i * KD_PI_F / (float)CIRCLE_SEG_NUM * 2.0f;
                    Point d(radius * kdCosf(angle), radius * kdSinf(angle));
                    seg[i] = centre + d;
                }
                m_pDrawNode->drawPolygon(seg, CIRCLE_SEG_NUM, Color4F(1.0f, 0.0f, 0.0f, 0.3f), 1, Color4F(1, 0, 0, 1));
                break;
            }
            case CP_SEGMENT_SHAPE:
            {
                cpSegmentShape *seg = (cpSegmentShape *)subShape;
                m_pDrawNode->drawSegment(PhysicsHelper::cpv2point(seg->ta),
                                       PhysicsHelper::cpv2point(seg->tb),
                                       PhysicsHelper::cpfloat2float(seg->r==0 ? 1 : seg->r), Color4F(1, 0, 0, 1));
                break;
            }
            case CP_POLY_SHAPE:
            {
                cpPolyShape* poly = (cpPolyShape*)subShape;
                int num = poly->numVerts;
                Point* seg = new Point[num];
                
                PhysicsHelper::cpvs2points(poly->tVerts, seg, num);
                
                m_pDrawNode->drawPolygon(seg, num, Color4F(1.0f, 0.0f, 0.0f, 0.3f), 1.0f, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
                
                delete[] seg;
                break;
            }
            default:
                break;
        }
    }
}

void PhysicsDebugDraw::drawJoint(PhysicsJoint& joint)
{
    for (auto it = joint.m_pInfo->getJoints().begin(); it != joint.m_pInfo->getJoints().end(); ++it)
    {
        cpConstraint *constraint = *it;
        
        
        cpBody *body_a = constraint->a;
        cpBody *body_b = constraint->b;
        
		const cpConstraintClass *klass = constraint->klass;
        if(klass == cpPinJointGetClass())
        {
            cpPinJoint *subJoint = (cpPinJoint *)constraint;
            
            cpVect a = cpvadd(body_a->p, cpvrotate(subJoint->anchr1, body_a->rot));
            cpVect b = cpvadd(body_b->p, cpvrotate(subJoint->anchr2, body_b->rot));
            
            m_pDrawNode->drawSegment(PhysicsHelper::cpv2point(a), PhysicsHelper::cpv2point(b), 1, Color4F(0.0f, 0.0f, 1.0f, 1.0f));
            m_pDrawNode->drawDot(PhysicsHelper::cpv2point(a), 2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
            m_pDrawNode->drawDot(PhysicsHelper::cpv2point(b), 2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
        }
        else if(klass == cpSlideJointGetClass())
        {
            cpSlideJoint *subJoint = (cpSlideJoint *)constraint;
            
            cpVect a = cpvadd(body_a->p, cpvrotate(subJoint->anchr1, body_a->rot));
            cpVect b = cpvadd(body_b->p, cpvrotate(subJoint->anchr2, body_b->rot));
            
            m_pDrawNode->drawSegment(PhysicsHelper::cpv2point(a), PhysicsHelper::cpv2point(b), 1, Color4F(0.0f, 0.0f, 1.0f, 1.0f));
            m_pDrawNode->drawDot(PhysicsHelper::cpv2point(a), 2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
            m_pDrawNode->drawDot(PhysicsHelper::cpv2point(b), 2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
        }
        else if(klass == cpPivotJointGetClass())
        {
            cpPivotJoint *subJoint = (cpPivotJoint *)constraint;
            
            cpVect a = cpvadd(body_a->p, cpvrotate(subJoint->anchr1, body_a->rot));
            cpVect b = cpvadd(body_b->p, cpvrotate(subJoint->anchr2, body_b->rot));
            
            m_pDrawNode->drawDot(PhysicsHelper::cpv2point(a), 2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
            m_pDrawNode->drawDot(PhysicsHelper::cpv2point(b), 2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
        }
        else if(klass == cpGrooveJointGetClass())
        {
            cpGrooveJoint *subJoint = (cpGrooveJoint *)constraint;
            
            cpVect a = cpvadd(body_a->p, cpvrotate(subJoint->grv_a, body_a->rot));
            cpVect b = cpvadd(body_a->p, cpvrotate(subJoint->grv_b, body_a->rot));
            cpVect c = cpvadd(body_b->p, cpvrotate(subJoint->anchr2, body_b->rot));
            
            m_pDrawNode->drawSegment(PhysicsHelper::cpv2point(a), PhysicsHelper::cpv2point(b), 1, Color4F(0.0f, 0.0f, 1.0f, 1.0f));
            m_pDrawNode->drawDot(PhysicsHelper::cpv2point(c), 2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
        }
    }
}

void PhysicsDebugDraw::drawContact()
{
    
}

#elif (CC_PHYSICS_ENGINE == CC_PHYSICS_BOX2D)

#endif

PhysicsWorld* PhysicsWorld::construct(Scene& scene)
{
    PhysicsWorld * world = new PhysicsWorld();
    if(world && world->init(scene))
    {
        return world;
    }
    
    CC_SAFE_DELETE(world);
    return nullptr;
}

PhysicsWorld::PhysicsWorld()
: m_tGravity(Point(0.0f, -98.0f))
, m_fSpeed(1.0f)
, m_pInfo(nullptr)
, m_pBodies(nullptr)
, m_pScene(nullptr)
, m_bDelayDirty(false)
, m_pDebugDraw(nullptr)
, m_nDebugDrawMask(DEBUGDRAW_NONE)
, m_pDelayAddBodies(nullptr)
, m_pDelayRemoveBodies(nullptr)
{
    
}

PhysicsWorld::~PhysicsWorld()
{
    removeAllJoints(true);
    removeAllBodies();
    CC_SAFE_RELEASE(m_pDelayRemoveBodies);
    CC_SAFE_RELEASE(m_pDelayAddBodies);
    CC_SAFE_DELETE(m_pInfo);
    CC_SAFE_DELETE(m_pDebugDraw);
}



PhysicsDebugDraw::PhysicsDebugDraw(PhysicsWorld& world)
: m_pDrawNode(nullptr)
, m_tWorld(world)
{
}

PhysicsDebugDraw::~PhysicsDebugDraw()
{
    if (m_pDrawNode != nullptr)
    {
        m_pDrawNode->removeFromParent();
        m_pDrawNode = nullptr;
    }
}

bool PhysicsDebugDraw::begin()
{
    if (m_pDrawNode != nullptr)
    {
        m_pDrawNode->removeFromParent();
        m_pDrawNode = nullptr;
    }
    
    m_pDrawNode = DrawNode::create();
    
    if (m_pDrawNode == nullptr)
    {
        return false;
    }
    
    m_tWorld.getScene().addChild(m_pDrawNode);
    
    return true;
}

void PhysicsDebugDraw::end()
{
}

NS_CC_END

#endif // CC_USE_PHYSICS
