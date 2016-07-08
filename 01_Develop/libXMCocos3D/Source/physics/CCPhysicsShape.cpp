/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsShape.cpp
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

#include "physics/CCPhysicsShape.h"
#ifdef CC_USE_PHYSICS

#include <climits>

#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
#include "XMChipmunk/chipmunk.h"
#elif (CC_PHYSICS_ENGINE == CCPHYSICS_BOX2D)
#include "XMBox2D/Box2D.h"
#endif

#include "physics/CCPhysicsBody.h"
#include "physics/CCPhysicsWorld.h"

#include "physics/chipmunk/CCPhysicsBodyInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsBodyInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsShapeInfo_chipmunk.h"
#include "physics/box2d/CCPhysicsShapeInfo_box2d.h"
#include "physics/chipmunk/CCPhysicsHelper_chipmunk.h"

NS_CC_BEGIN

PhysicsShape::PhysicsShape()
: m_pBody(nullptr)
, m_pInfo(nullptr)
, m_eType(Type::UNKNOWN)
, m_fArea(0)
, m_fMass(0)
, m_fMoment(0)
, m_nTag(0)
, m_nCategoryBitmask(UINT_MAX)
, m_nCollisionBitmask(UINT_MAX)
, m_nContactTestBitmask(UINT_MAX)
, m_nGroup(0)
{
    
}

PhysicsShape::~PhysicsShape()
{
    CC_SAFE_DELETE(m_pInfo);
}

bool PhysicsShape::init(Type type)
{
    m_pInfo = new PhysicsShapeInfo(this);
    if (m_pInfo == nullptr) return false;
    
    m_eType = type;
    
    return true;
}

void PhysicsShape::setMass(float mass)
{
    if (mass < 0)
    {
        return;
    }
    
    if (m_pBody)
    {
        m_pBody->addMass(-m_fMass);
        m_pBody->addMass(mass);
    };
    
    m_fMass = mass;
}

void PhysicsShape::setMoment(float moment)
{
    if (moment < 0)
    {
        return;
    }
    
    if (m_pBody)
    {
        m_pBody->addMoment(-m_fMoment);
        m_pBody->addMoment(moment);
    };
    
    m_fMoment = moment;
}

void PhysicsShape::setMaterial(const PhysicsMaterial& material)
{
    setDensity(material.density);
    setRestitution(material.restitution);
    setFriction(material.friction);
}

PhysicsBodyInfo* PhysicsShape::bodyInfo() const
{
    if (m_pBody != nullptr)
    {
        return m_pBody->m_pInfo;
    }else
    {
        return nullptr;
    }
}

PhysicsShapeCircle::PhysicsShapeCircle()
{
    
}

PhysicsShapeCircle::~PhysicsShapeCircle()
{
    
}

PhysicsShapeBox::PhysicsShapeBox()
{
    
}

PhysicsShapeBox::~PhysicsShapeBox()
{
    
}

PhysicsShapePolygon::PhysicsShapePolygon()
{
    
}

PhysicsShapePolygon::~PhysicsShapePolygon()
{
    
}

PhysicsShapeEdgeBox::PhysicsShapeEdgeBox()
{
    
}

PhysicsShapeEdgeBox::~PhysicsShapeEdgeBox()
{
    
}

PhysicsShapeEdgeChain::PhysicsShapeEdgeChain()
{
    
}

PhysicsShapeEdgeChain::~PhysicsShapeEdgeChain()
{
    
}

PhysicsShapeEdgePolygon::PhysicsShapeEdgePolygon()
{
    
}

PhysicsShapeEdgePolygon::~PhysicsShapeEdgePolygon()
{
    
}

PhysicsShapeEdgeSegment::PhysicsShapeEdgeSegment()
{
    
}

PhysicsShapeEdgeSegment::~PhysicsShapeEdgeSegment()
{
    
}

#if (CC_PHYSICS_ENGINE == CC_PHYSICS_CHIPMUNK)
void PhysicsShape::setDensity(float density)
{
    if (density < 0)
    {
        return;
    }
    
    m_eMaterial.density = density;
    
    if (m_eMaterial.density == PHYSICS_INFINITY)
    {
        setMass(PHYSICS_INFINITY);
    }else if (m_fArea > 0)
    {
        setMass(PhysicsHelper::float2cpfloat(m_eMaterial.density * m_fArea));
    }
}

void PhysicsShape::setRestitution(float restitution)
{
    m_eMaterial.restitution = restitution;
    
    for (cpShape* shape : m_pInfo->getShapes())
    {
        cpShapeSetElasticity(shape, PhysicsHelper::float2cpfloat(restitution));
    }
}

void PhysicsShape::setFriction(float friction)
{
    m_eMaterial.friction = friction;
    
    for (cpShape* shape : m_pInfo->getShapes())
    {
        cpShapeSetFriction(shape, PhysicsHelper::float2cpfloat(friction));
    }
}


Point* PhysicsShape::recenterPoints(Point* points, int count, const Point& center)
{
    cpVect* cpvs = new cpVect[count];
    cpRecenterPoly(count, PhysicsHelper::points2cpvs(points, cpvs, count));
    PhysicsHelper::cpvs2points(cpvs, points, count);
    delete[] cpvs;
    
    if (center != Point::ZERO)
    {
        for (int i = 0; i < count; ++i)
        {
            points[i] += center;
        }
    }
    
    return points;
}

Point PhysicsShape::getPolyonCenter(const Point* points, int count)
{
    cpVect* cpvs = new cpVect[count];
    cpVect center = cpCentroidForPoly(count, PhysicsHelper::points2cpvs(points, cpvs, count));
    delete[] cpvs;
    
    return PhysicsHelper::cpv2point(center);
}

void PhysicsShape::setBody(PhysicsBody *body)
{
    // already added
    if (body != nullptr && m_pBody == body)
    {
        return;
    }
    
    if (m_pBody != nullptr)
    {
        m_pBody->removeShape(this);
    }
    
    if (body == nullptr)
    {
        m_pInfo->setBody(nullptr);
        m_pBody = nullptr;
    }else
    {
        m_pInfo->setBody(body->m_pInfo->getBody());
        m_pBody = body;
    }
}

// PhysicsShapeCircle
PhysicsShapeCircle* PhysicsShapeCircle::create(float radius, const PhysicsMaterial& material/* = MaterialDefault*/, const Point& offset/* = Point(0, 0)*/)
{
    PhysicsShapeCircle* shape = new PhysicsShapeCircle();
    if (shape && shape->init(radius, material, offset))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeCircle::init(float radius, const PhysicsMaterial& material/* = MaterialDefault*/, const Point& offset /*= Point(0, 0)*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::CIRCLE));
        
        cpShape* shape = cpCircleShapeNew(m_pInfo->getSharedBody(), radius, PhysicsHelper::point2cpv(offset));
        
        CC_BREAK_IF(shape == nullptr);
        
        m_pInfo->add(shape);
        
        m_fArea = calculateDefaultArea();
        m_fMass = material.density == PHYSICS_INFINITY ? PHYSICS_INFINITY : material.density * m_fArea;
        m_fMoment = calculateDefaultMoment();
        
        setMaterial(material);
        return true;
    } while (false);
    
    return false;
}

float PhysicsShapeCircle::calculateArea(float radius)
{
    return PhysicsHelper::cpfloat2float(cpAreaForCircle(0, radius));
}

float PhysicsShapeCircle::calculateMoment(float mass, float radius, const Point& offset)
{
    return mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForCircle(PhysicsHelper::float2cpfloat(mass),
                                                     0,
                                                     PhysicsHelper::float2cpfloat(radius),
                                                     PhysicsHelper::point2cpv(offset)));
}

float PhysicsShapeCircle::calculateDefaultArea()
{
    return PhysicsHelper::cpfloat2float(cpAreaForCircle(0, cpCircleShapeGetRadius(m_pInfo->getShapes().front())));
}

float PhysicsShapeCircle::calculateDefaultMoment()
{
    cpShape* shape = m_pInfo->getShapes().front();
    
    return m_fMass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForCircle(PhysicsHelper::float2cpfloat(m_fMass),
                                                     0,
                                                     cpCircleShapeGetRadius(shape),
                                                     cpCircleShapeGetOffset(shape)));
}

float PhysicsShapeCircle::getRadius() const
{
    return PhysicsHelper::cpfloat2float(cpCircleShapeGetRadius(m_pInfo->getShapes().front()));
}

Point PhysicsShapeCircle::getOffset()
{
    return PhysicsHelper::cpv2point(cpCircleShapeGetOffset(m_pInfo->getShapes().front()));
}

// PhysicsShapeEdgeSegment
PhysicsShapeEdgeSegment* PhysicsShapeEdgeSegment::create(const Point& a, const Point& b, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    PhysicsShapeEdgeSegment* shape = new PhysicsShapeEdgeSegment();
    if (shape && shape->init(a, b, material, border))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeEdgeSegment::init(const Point& a, const Point& b, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGESEGMENT));
        
        cpShape* shape = cpSegmentShapeNew(m_pInfo->getSharedBody(),
                                           PhysicsHelper::point2cpv(a),
                                           PhysicsHelper::point2cpv(b),
                                           PhysicsHelper::float2cpfloat(border));
        
        CC_BREAK_IF(shape == nullptr);
        
        m_pInfo->add(shape);
        
        m_fMass = PHYSICS_INFINITY;
        m_fMoment = PHYSICS_INFINITY;
        m_tCenter = a.getMidpoint(b);
        
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    return false;
}

Point PhysicsShapeEdgeSegment::getPointA() const
{
    return PhysicsHelper::cpv2point(((cpSegmentShape*)(m_pInfo->getShapes().front()))->ta);
}

Point PhysicsShapeEdgeSegment::getPointB() const
{
    return PhysicsHelper::cpv2point(((cpSegmentShape*)(m_pInfo->getShapes().front()))->tb);
}

Point PhysicsShapeEdgeSegment::getCenter()
{
    return m_tCenter;
}

// PhysicsShapeBox
PhysicsShapeBox* PhysicsShapeBox::create(const Size& size, const PhysicsMaterial& material/* = MaterialDefault*/, const Point& offset/* = Point(0, 0)*/)
{
    PhysicsShapeBox* shape = new PhysicsShapeBox();
    if (shape && shape->init(size, material, offset))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeBox::init(const Size& size, const PhysicsMaterial& material/* = MaterialDefault*/, const Point& offset /*= Point(0, 0)*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::BOX));
        
        cpVect wh = PhysicsHelper::size2cpv(size);
        cpVect vec[4] =
        {
            {-wh.x/2.0f, -wh.y/2.0f}, {-wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, -wh.y/2.0f}
        };
        
        cpShape* shape = cpPolyShapeNew(m_pInfo->getSharedBody(), 4, vec, PhysicsHelper::point2cpv(offset));
        
        CC_BREAK_IF(shape == nullptr);
        
        m_pInfo->add(shape);
        
        m_tOffset = offset;
        m_fArea = calculateDefaultArea();
        m_fMass = material.density == PHYSICS_INFINITY ? PHYSICS_INFINITY : material.density * m_fArea;
        m_fMoment = calculateDefaultMoment();
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    return false;
}

float PhysicsShapeBox::calculateArea(const Size& size)
{
    cpVect wh = PhysicsHelper::size2cpv(size);
    cpVect vec[4] =
    {
        {-wh.x/2.0f, -wh.y/2.0f}, {-wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, -wh.y/2.0f}
    };
    return PhysicsHelper::cpfloat2float(cpAreaForPoly(4, vec));
}

float PhysicsShapeBox::calculateMoment(float mass, const Size& size, const Point& offset)
{
    cpVect wh = PhysicsHelper::size2cpv(size);
    cpVect vec[4] =
    {
        {-wh.x/2.0f, -wh.y/2.0f}, {-wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, -wh.y/2.0f}
    };
    
    return mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForPoly(PhysicsHelper::float2cpfloat(mass),
                                                   4,
                                                   vec,
                                                   PhysicsHelper::point2cpv(offset)));
}

float PhysicsShapeBox::calculateDefaultArea()
{
    cpShape* shape = m_pInfo->getShapes().front();
    return PhysicsHelper::cpfloat2float(cpAreaForPoly(((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts));
}

float PhysicsShapeBox::calculateDefaultMoment()
{
    cpShape* shape = m_pInfo->getShapes().front();
    return m_fMass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForPoly(m_fMass, ((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts, cpvzero));
}

void PhysicsShapeBox::getPoints(Point* points) const
{
    cpShape* shape = m_pInfo->getShapes().front();
    PhysicsHelper::cpvs2points(((cpPolyShape*)shape)->verts, points, ((cpPolyShape*)shape)->numVerts);
}

Size PhysicsShapeBox::getSize() const
{
    cpShape* shape = m_pInfo->getShapes().front();
    return PhysicsHelper::cpv2size(cpv(cpvdist(cpPolyShapeGetVert(shape, 0), cpPolyShapeGetVert(shape, 1)),
                                       cpvdist(cpPolyShapeGetVert(shape, 1), cpPolyShapeGetVert(shape, 2))));
}

// PhysicsShapePolygon
PhysicsShapePolygon* PhysicsShapePolygon::create(const Point* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, const Point& offset/* = Point(0, 0)*/)
{
    PhysicsShapePolygon* shape = new PhysicsShapePolygon();
    if (shape && shape->init(points, count, material, offset))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapePolygon::init(const Point* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, const Point& offset/* = Point(0, 0)*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::POLYGEN));
        
        cpVect* vecs = new cpVect[count];
        PhysicsHelper::points2cpvs(points, vecs, count);
        cpShape* shape = cpPolyShapeNew(m_pInfo->getSharedBody(), count, vecs, PhysicsHelper::point2cpv(offset));
        CC_SAFE_DELETE_ARRAY(vecs);
        
        CC_BREAK_IF(shape == nullptr);
        
        m_pInfo->add(shape);
        
        m_fArea = calculateDefaultArea();
        m_fMass = material.density == PHYSICS_INFINITY ? PHYSICS_INFINITY : material.density * m_fArea;
        m_fMoment = calculateDefaultMoment();
        m_tCenter = PhysicsHelper::cpv2point(cpCentroidForPoly(((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts));
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    return false;
}

float PhysicsShapePolygon::calculateArea(const Point* points, int count)
{
    cpVect* vecs = new cpVect[count];
    PhysicsHelper::points2cpvs(points, vecs, count);
    float area = PhysicsHelper::cpfloat2float(cpAreaForPoly(count, vecs));
    CC_SAFE_DELETE_ARRAY(vecs);
    
    return area;
}

float PhysicsShapePolygon::calculateMoment(float mass, const Point* points, int count, const Point& offset)
{
    cpVect* vecs = new cpVect[count];
    PhysicsHelper::points2cpvs(points, vecs, count);
    float moment = mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForPoly(mass, count, vecs, PhysicsHelper::point2cpv(offset)));
    CC_SAFE_DELETE_ARRAY(vecs);
    
    return moment;
}

float PhysicsShapePolygon::calculateDefaultArea()
{
    cpShape* shape = m_pInfo->getShapes().front();
    return PhysicsHelper::cpfloat2float(cpAreaForPoly(((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts));
}

float PhysicsShapePolygon::calculateDefaultMoment()
{
    cpShape* shape = m_pInfo->getShapes().front();
    return m_fMass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForPoly(m_fMass, ((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts, cpvzero));
}

Point PhysicsShapePolygon::getPoint(int i) const
{
    return PhysicsHelper::cpv2point(cpPolyShapeGetVert(m_pInfo->getShapes().front(), i));
}

void PhysicsShapePolygon::getPoints(Point* outPoints) const
{
    cpShape* shape = m_pInfo->getShapes().front();
    PhysicsHelper::cpvs2points(((cpPolyShape*)shape)->verts, outPoints, ((cpPolyShape*)shape)->numVerts);
}

KDint32 PhysicsShapePolygon::getPointsCount() const
{
    return ((cpPolyShape*)m_pInfo->getShapes().front())->numVerts;
}

Point PhysicsShapePolygon::getCenter()
{
    return m_tCenter;
}

// PhysicsShapeEdgeBox
PhysicsShapeEdgeBox* PhysicsShapeEdgeBox::create(const Size& size, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/, const Point& offset/* = Point(0, 0)*/)
{
    PhysicsShapeEdgeBox* shape = new PhysicsShapeEdgeBox();
    if (shape && shape->init(size, material, border, offset))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeEdgeBox::init(const Size& size, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/, const Point& offset/*= Point(0, 0)*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGEBOX));
        
        cpVect vec[4] = {};
        vec[0] = PhysicsHelper::point2cpv(Point(-size.width/2+offset.x, -size.height/2+offset.y));
        vec[1] = PhysicsHelper::point2cpv(Point(+size.width/2+offset.x, -size.height/2+offset.y));
        vec[2] = PhysicsHelper::point2cpv(Point(+size.width/2+offset.x, +size.height/2+offset.y));
        vec[3] = PhysicsHelper::point2cpv(Point(-size.width/2+offset.x, +size.height/2+offset.y));
        
        int i = 0;
        for (; i < 4; ++i)
        {
            cpShape* shape = cpSegmentShapeNew(m_pInfo->getSharedBody(), vec[i], vec[(i+1)%4],
                                               PhysicsHelper::float2cpfloat(border));
            CC_BREAK_IF(shape == nullptr);
            m_pInfo->add(shape);
        }
        CC_BREAK_IF(i < 4);
        
        m_tOffset = offset;
        m_fMass = PHYSICS_INFINITY;
        m_fMoment = PHYSICS_INFINITY;
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    return false;
}

// PhysicsShapeEdgeBox
PhysicsShapeEdgePolygon* PhysicsShapeEdgePolygon::create(const Point* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    PhysicsShapeEdgePolygon* shape = new PhysicsShapeEdgePolygon();
    if (shape && shape->init(points, count, material, border))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeEdgePolygon::init(const Point* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    cpVect* vec = nullptr;
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGEPOLYGEN));
        
        vec = new cpVect[count];
        PhysicsHelper::points2cpvs(points, vec, count);
        m_tCenter = PhysicsHelper::cpv2point(cpCentroidForPoly(count, vec));
        
        int i = 0;
        for (; i < count; ++i)
        {
            cpShape* shape = cpSegmentShapeNew(m_pInfo->getSharedBody(), vec[i], vec[(i+1)%count],
                                               PhysicsHelper::float2cpfloat(border));
            CC_BREAK_IF(shape == nullptr);
			cpShapeSetElasticity(shape, 1.0f);
			cpShapeSetFriction(shape, 1.0f);
            m_pInfo->add(shape);
        }
        CC_SAFE_DELETE_ARRAY(vec);
        
        CC_BREAK_IF(i < count);
        
        m_fMass = PHYSICS_INFINITY;
        m_fMoment = PHYSICS_INFINITY;
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    CC_SAFE_DELETE_ARRAY(vec);
    
    return false;
}

Point PhysicsShapeEdgePolygon::getCenter()
{
    return m_tCenter;
}

KDint32 PhysicsShapeEdgePolygon::getPointsCount() const
{
    return m_pInfo->getShapes().size() + 1;
}

// PhysicsShapeEdgeChain
PhysicsShapeEdgeChain* PhysicsShapeEdgeChain::create(const Point* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    PhysicsShapeEdgeChain* shape = new PhysicsShapeEdgeChain();
    if (shape && shape->init(points, count, material, border))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeEdgeChain::init(const Point* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    cpVect* vec = nullptr;
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGECHAIN));
        
        vec = new cpVect[count];
        PhysicsHelper::points2cpvs(points, vec, count);
        m_tCenter = PhysicsHelper::cpv2point(cpCentroidForPoly(count, vec));
        
        int i = 0;
        for (; i < count - 1; ++i)
        {
            cpShape* shape = cpSegmentShapeNew(m_pInfo->getSharedBody(), vec[i], vec[i+1],
                                               PhysicsHelper::float2cpfloat(border));
            CC_BREAK_IF(shape == nullptr);
			cpShapeSetElasticity(shape, 1.0f);
			cpShapeSetFriction(shape, 1.0f);
            m_pInfo->add(shape);
        }
        CC_SAFE_DELETE_ARRAY(vec);
        CC_BREAK_IF(i < count);
        
        m_fMass = PHYSICS_INFINITY;
        m_fMoment = PHYSICS_INFINITY;
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    CC_SAFE_DELETE_ARRAY(vec);
    
    return false;
}

Point PhysicsShapeEdgeChain::getCenter()
{
    return m_tCenter;
}

KDint32 PhysicsShapeEdgeChain::getPointsCount() const
{
    return m_pInfo->getShapes().size() + 1;
}

void PhysicsShape::setGroup(int group)
{
    if (group < 0)
    {
        for (auto shape : m_pInfo->getShapes())
        {
            cpShapeSetGroup(shape, (cpGroup)group);
        }
    }
    
    m_nGroup = group;
}

bool PhysicsShape::containsPoint(const Point& point) const
{
    for (auto shape : m_pInfo->getShapes())
    {
        if (cpShapePointQuery(shape, PhysicsHelper::point2cpv(point)))
        {
            return true;
        }
    }
    
    return false;
}

#elif (CC_PHYSICS_ENGINE == CC_PHYSICS_BOX2D)

#endif

NS_CC_END

#endif // CC_USE_PHYSICS
