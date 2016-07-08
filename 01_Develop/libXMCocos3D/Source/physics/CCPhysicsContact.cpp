/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsContact.cpp
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

#include "physics/CCPhysicsContact.h"
#ifdef CC_USE_PHYSICS

#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
#include "XMChipmunk/chipmunk.h"
#elif (CC_PHYSICS_ENGINE == CCPHYSICS_BOX2D)
#include "XMBox2D/Box2D.h"
#endif

#include "physics/CCPhysicsBody.h"

#include "physics/chipmunk/CCPhysicsContactInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsContactInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsHelper_chipmunk.h"
#include "physics/box2d/CCPhysicsHelper_box2d.h"

#include "2d/event_dispatcher/CCEventCustom.h"

NS_CC_BEGIN

const char* PHYSICSCONTACT_EVENT_NAME = "PhysicsContactEvent";

PhysicsContact::PhysicsContact()
: Event(Event::Type::CUSTOM)
, m_pWorld(nullptr)
, m_pShapeA(nullptr)
, m_pShapeB(nullptr)
, m_eEventCode(EventCode::NONE)
, m_pInfo(nullptr)
, m_bNotificationEnable(true)
, m_bBegin(false)
, m_bResult(true)
, m_pData(nullptr)
, m_pContactInfo(nullptr)
, m_pContactData(nullptr)
{
    
}

PhysicsContact::~PhysicsContact()
{
    CC_SAFE_DELETE(m_pInfo);
    CC_SAFE_DELETE(m_pContactData);
}

PhysicsContact* PhysicsContact::construct(PhysicsShape* a, PhysicsShape* b)
{
    PhysicsContact * contact = new PhysicsContact();
    if(contact && contact->init(a, b))
    {
        return contact;
    }
    
    CC_SAFE_DELETE(contact);
    return nullptr;
}

bool PhysicsContact::init(PhysicsShape* a, PhysicsShape* b)
{
    do
    {
        CC_BREAK_IF(a == nullptr || b == nullptr);
        
        CC_BREAK_IF(!(m_pInfo = new PhysicsContactInfo(this)));
        
        m_pShapeA = a;
        m_pShapeB = b;
        
        return true;
    } while(false);
    
    return false;
}

void PhysicsContact::generateContactData()
{
    if (m_pContactInfo == nullptr)
    {
        return;
    }
    
    cpArbiter* arb = static_cast<cpArbiter*>(m_pContactInfo);
    m_pContactData = new PhysicsContactData();
    m_pContactData->count = cpArbiterGetCount(arb);
    for (int i=0; i<m_pContactData->count; ++i)
    {
        m_pContactData->points[i] = PhysicsHelper::cpv2point(cpArbiterGetPoint(arb, i));
    }
    
    m_pContactData->normal = m_pContactData->count > 0 ? PhysicsHelper::cpv2point(cpArbiterGetNormal(arb, 0)) : Point::ZERO;
}

// PhysicsContactPreSolve implementation
PhysicsContactPreSolve::PhysicsContactPreSolve(PhysicsContactData* data, void* contactInfo)
: m_pPreContactData(data)
, m_pContactInfo(contactInfo)
{
}

PhysicsContactPreSolve::~PhysicsContactPreSolve()
{
    CC_SAFE_DELETE(m_pPreContactData);
}

float PhysicsContactPreSolve::getElasticity() const
{
    return static_cast<cpArbiter*>(m_pContactInfo)->e;
}

float PhysicsContactPreSolve::getFriction() const
{
    return static_cast<cpArbiter*>(m_pContactInfo)->u;
}

Point PhysicsContactPreSolve::getSurfaceVelocity() const
{
    return PhysicsHelper::cpv2point(static_cast<cpArbiter*>(m_pContactInfo)->surface_vr);
}

void PhysicsContactPreSolve::setElasticity(float elasticity)
{
    static_cast<cpArbiter*>(m_pContactInfo)->e = elasticity;
}

void PhysicsContactPreSolve::setFriction(float friction)
{
    static_cast<cpArbiter*>(m_pContactInfo)->u = friction;
}

void PhysicsContactPreSolve::setSurfaceVelocity(const Vect& velocity)
{
    static_cast<cpArbiter*>(m_pContactInfo)->surface_vr = PhysicsHelper::point2cpv(velocity);
}

void PhysicsContactPreSolve::ignore()
{
    cpArbiterIgnore(static_cast<cpArbiter*>(m_pContactInfo));
}

// PhysicsContactPostSolve implementation
PhysicsContactPostSolve::PhysicsContactPostSolve(void* contactInfo)
: m_pContactInfo(contactInfo)
{
    
}

PhysicsContactPostSolve::~PhysicsContactPostSolve()
{
    
}

float PhysicsContactPostSolve::getElasticity() const
{
    return static_cast<cpArbiter*>(m_pContactInfo)->e;
}

float PhysicsContactPostSolve::getFriction() const
{
    return static_cast<cpArbiter*>(m_pContactInfo)->u;
}

Point PhysicsContactPostSolve::getSurfaceVelocity() const
{
    return PhysicsHelper::cpv2point(static_cast<cpArbiter*>(m_pContactInfo)->surface_vr);
}

EventListenerPhysicsContact::EventListenerPhysicsContact()
: onContactBegin(nullptr)
, onContactPreSolve(nullptr)
, onContactPostSolve(nullptr)
, onContactSeperate(nullptr)
{
}

bool EventListenerPhysicsContact::init()
{
    auto func = [this](EventCustom* event) -> void
    {
        onEvent(event);
    };
    
    return EventListenerCustom::init(std::hash<std::string>()(PHYSICSCONTACT_EVENT_NAME), func);
}

void EventListenerPhysicsContact::onEvent(EventCustom* event)
{
    PhysicsContact& contact = *(PhysicsContact*)(event->getUserData());
    
    switch (contact.getEventCode())
    {
        case PhysicsContact::EventCode::BEGIN:
        {
            bool ret = true;
            
            if (onContactBegin != nullptr
                && hitTest(contact.getShapeA(), contact.getShapeB()))
            {
                contact.m_bBegin = true;
                contact.generateContactData();
                
                // the mask has high priority than _listener->onContactBegin.
                // so if the mask test is false, the two bodies won't have collision.
                if (ret)
                {
                    ret = onContactBegin(event, contact);
                }else
                {
                    onContactBegin(event, contact);
                }
            }
            
            contact.setResult(ret);
            break;
        }
        case PhysicsContact::EventCode::PRESOLVE:
        {
            bool ret = true;
            
            if (onContactPreSolve != nullptr
                && hitTest(contact.getShapeA(), contact.getShapeB()))
            {
                PhysicsContactPreSolve solve(contact.m_bBegin ? nullptr : contact.m_pContactData, contact.m_pContactInfo);
                contact.m_bBegin = false;
                contact.generateContactData();
                
                ret = onContactPreSolve(event, contact, solve);
            }
            
            contact.setResult(ret);
            break;
        }
        case PhysicsContact::EventCode::POSTSOLVE:
        {
            if (onContactPostSolve != nullptr
                && hitTest(contact.getShapeA(), contact.getShapeB()))
            {
                PhysicsContactPostSolve solve(contact.m_pContactInfo);
                onContactPostSolve(event, contact, solve);
            }
            break;
        }
        case PhysicsContact::EventCode::SEPERATE:
        {
            if (onContactSeperate != nullptr
                && hitTest(contact.getShapeA(), contact.getShapeB()))
            {
                onContactSeperate(event, contact);
            }
            break;
        }
        default:
            break;
    }
}

EventListenerPhysicsContact::~EventListenerPhysicsContact()
{
    
}

EventListenerPhysicsContact* EventListenerPhysicsContact::create()
{
    EventListenerPhysicsContact* obj = new EventListenerPhysicsContact();
    
    if (obj != nullptr && obj->init())
    {
        obj->autorelease();
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return nullptr;
}

bool EventListenerPhysicsContact::hitTest(PhysicsShape* shapeA, PhysicsShape* shapeB)
{
    CC_UNUSED_PARAM(shapeA);
    CC_UNUSED_PARAM(shapeB);
    return true;
}

bool EventListenerPhysicsContact::checkAvailable()
{
    if (onContactBegin == nullptr && onContactPreSolve == nullptr
        && onContactPostSolve == nullptr && onContactSeperate == nullptr)
    {
        CCASSERT(false, "Invalid PhysicsContactListener.");
        return false;
    }
    
    return true;
}

EventListenerPhysicsContact* EventListenerPhysicsContact::clone()
{
    EventListenerPhysicsContact* obj = EventListenerPhysicsContact::create();
    
    if (obj != nullptr)
    {
        obj->onContactBegin = onContactBegin;
        obj->onContactPreSolve = onContactPreSolve;
        obj->onContactPostSolve = onContactPostSolve;
        obj->onContactSeperate = onContactSeperate;
        
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return nullptr;
}

EventListenerPhysicsContactWithBodies* EventListenerPhysicsContactWithBodies::create(PhysicsBody* bodyA, PhysicsBody* bodyB)
{
    EventListenerPhysicsContactWithBodies* obj = new EventListenerPhysicsContactWithBodies();
    
    if (obj != nullptr && obj->init())
    {
        obj->m_pA = bodyA;
        obj->m_pB = bodyB;
        obj->autorelease();
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return nullptr;
}

EventListenerPhysicsContactWithBodies::EventListenerPhysicsContactWithBodies()
: m_pA(nullptr)
, m_pB(nullptr)
{
    
}

EventListenerPhysicsContactWithBodies::~EventListenerPhysicsContactWithBodies()
{
    
}


bool EventListenerPhysicsContactWithBodies::hitTest(PhysicsShape* shapeA, PhysicsShape* shapeB)
{
    if ((shapeA->getBody() == m_pA && shapeB->getBody() == m_pB)
        || (shapeA->getBody() == m_pB && shapeB->getBody() == m_pA))
    {
        return true;
    }
    
    return false;
}

EventListenerPhysicsContactWithBodies* EventListenerPhysicsContactWithBodies::clone()
{
    EventListenerPhysicsContactWithBodies* obj = EventListenerPhysicsContactWithBodies::create(m_pA, m_pB);
    
    if (obj != nullptr)
    {
        obj->onContactBegin = onContactBegin;
        obj->onContactPreSolve = onContactPreSolve;
        obj->onContactPostSolve = onContactPostSolve;
        obj->onContactSeperate = onContactSeperate;
        
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return nullptr;
}

EventListenerPhysicsContactWithShapes::EventListenerPhysicsContactWithShapes()
: m_pA(nullptr)
, m_pB(nullptr)
{
}

EventListenerPhysicsContactWithShapes::~EventListenerPhysicsContactWithShapes()
{
}

EventListenerPhysicsContactWithShapes* EventListenerPhysicsContactWithShapes::create(PhysicsShape* shapeA, PhysicsShape* shapeB)
{
    EventListenerPhysicsContactWithShapes* obj = new EventListenerPhysicsContactWithShapes();
    
    if (obj != nullptr && obj->init())
    {
        obj->m_pA = shapeA;
        obj->m_pB = shapeB;
        obj->autorelease();
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return nullptr;
}

bool EventListenerPhysicsContactWithShapes::hitTest(PhysicsShape* shapeA, PhysicsShape* shapeB)
{
    if ((shapeA == m_pA && shapeB == m_pB)
        || (shapeA == m_pB && shapeB == m_pA))
    {
        return true;
    }
    
    return false;
}

EventListenerPhysicsContactWithShapes* EventListenerPhysicsContactWithShapes::clone()
{
    EventListenerPhysicsContactWithShapes* obj = EventListenerPhysicsContactWithShapes::create(m_pA, m_pB);
    
    if (obj != nullptr)
    {
        obj->onContactBegin = onContactBegin;
        obj->onContactPreSolve = onContactPreSolve;
        obj->onContactPostSolve = onContactPostSolve;
        obj->onContactSeperate = onContactSeperate;
        
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return nullptr;
}

EventListenerPhysicsContactWithGroup::EventListenerPhysicsContactWithGroup()
: m_nGroup(CP_NO_GROUP)
{
}

EventListenerPhysicsContactWithGroup::~EventListenerPhysicsContactWithGroup()
{
}

EventListenerPhysicsContactWithGroup* EventListenerPhysicsContactWithGroup::create(int group)
{
    EventListenerPhysicsContactWithGroup* obj = new EventListenerPhysicsContactWithGroup();
    
    if (obj != nullptr && obj->init())
    {
        obj->m_nGroup = group;
        obj->autorelease();
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return nullptr;
}

bool EventListenerPhysicsContactWithGroup::hitTest(PhysicsShape* shapeA, PhysicsShape* shapeB)
{
    if (shapeA->getGroup() == m_nGroup || shapeB->getGroup() == m_nGroup)
    {
        return true;
    }
    
    return false;
}

EventListenerPhysicsContactWithGroup* EventListenerPhysicsContactWithGroup::clone()
{
    EventListenerPhysicsContactWithGroup* obj = EventListenerPhysicsContactWithGroup::create(m_nGroup);
    
    if (obj != nullptr)
    {
        obj->onContactBegin = onContactBegin;
        obj->onContactPreSolve = onContactPreSolve;
        obj->onContactPostSolve = onContactPostSolve;
        obj->onContactSeperate = onContactSeperate;
        
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return nullptr;
}

NS_CC_END
#endif // CC_USE_PHYSICS
