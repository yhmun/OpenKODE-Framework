/* -----------------------------------------------------------------------------------
 *
 *      File            CCColliderDetector.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#include "editor-support/cocostudio/armature/physics/CCColliderDetector.h"
#include "editor-support/cocostudio/armature/CCBone.h"
#include "editor-support/cocostudio/armature/utils/CCTransformHelp.h"

#include "XMBox2D/Box2D.h"
#include "XMChipmunk/chipmunk.h"

using namespace cocos2d;

namespace cocostudio {


ColliderFilter::ColliderFilter ( KDvoid )
{
    m_uCategoryBits		= 0x0001;
    m_uMaskBits			= 0xFFFF;
    m_nGroupIndex		= 0;
    m_uCollisionType	= 0;
    m_uGroup			= 0;
}

ColliderFilter::ColliderFilter(unsigned short categoryBits, unsigned short maskBits, signed short groupIndex)
    : m_uCategoryBits(categoryBits)
    , m_uMaskBits(maskBits)
    , m_nGroupIndex(groupIndex)
{
}

void ColliderFilter::updateB2Shape(b2Fixture *fixture)
{
        b2Filter filter;
        filter.categoryBits = m_uCategoryBits;
        filter.groupIndex = m_nGroupIndex;
        filter.maskBits = m_uMaskBits;
        
        fixture->SetFilterData(filter);
}

ColliderFilter::ColliderFilter(KDuintptr collisionType, KDuintptr group)
    : m_uCollisionType(collisionType)
    , m_uGroup(group)
{
}
void ColliderFilter::updateCPShape(cpShape *shape)
{
        shape->collision_type = m_uCollisionType;
        shape->group = m_uGroup;
}

ColliderBody::ColliderBody ( ContourData *pContourData )
{
	m_pFixture = nullptr;
	m_pShape   = nullptr;
	m_pContourData = pContourData;

    CC_SAFE_RETAIN ( m_pContourData );
    m_pFilter = new ColliderFilter ( );

#if ENABLE_PHYSICS_SAVE_CALCULATED_VERTEX
    m_pCalculatedVertexList = Array::create ( );
    CC_SAFE_RETAIN ( m_pCalculatedVertexList );
#endif
}

ColliderBody::~ColliderBody()
{
    CC_SAFE_RELEASE(m_pContourData);
    CC_SAFE_DELETE(m_pFilter);

#if ENABLE_PHYSICS_SAVE_CALCULATED_VERTEX
    CC_SAFE_RELEASE(m_pCalculatedVertexList);
#endif
}

void ColliderBody::setColliderFilter(ColliderFilter *filter)
{
    *m_pFilter = *filter;
}
ColliderFilter *ColliderBody::getColliderFilter()
{
    return m_pFilter;
}



ColliderDetector *ColliderDetector::create()
{
    ColliderDetector *pColliderDetector = new ColliderDetector();
    if (pColliderDetector && pColliderDetector->init())
    {
        pColliderDetector->autorelease();
        return pColliderDetector;
    }
    CC_SAFE_DELETE(pColliderDetector);
    return nullptr;
}

ColliderDetector *ColliderDetector::create(Bone *bone)
{
    ColliderDetector *pColliderDetector = new ColliderDetector();
    if (pColliderDetector && pColliderDetector->init(bone))
    {
        pColliderDetector->autorelease();
        return pColliderDetector;
    }
    CC_SAFE_DELETE(pColliderDetector);
    return nullptr;
}

ColliderDetector::ColliderDetector()
    : m_pColliderBodyList(nullptr)
    , m_pFilter(nullptr)
    , m_bActive(false)
{
    m_pB2Body = nullptr;
	m_pCPBody = nullptr;
}

ColliderDetector::~ColliderDetector()
{
    m_pColliderBodyList->removeAllObjects();
    CC_SAFE_DELETE(m_pColliderBodyList);
    CC_SAFE_DELETE(m_pFilter);
}

bool ColliderDetector::init()
{
    m_pColliderBodyList = Array::create();
    CCASSERT(m_pColliderBodyList, "create _colliderBodyList failed!");
    m_pColliderBodyList->retain();

    m_pFilter = new ColliderFilter();

    return true;
}

bool ColliderDetector::init(Bone *bone)
{
    init();
    setBone(bone);

    return true;
}

void ColliderDetector::addContourData(ContourData *contourData)
{
    ColliderBody *colliderBody = new ColliderBody(contourData);
    m_pColliderBodyList->addObject(colliderBody);
    colliderBody->release();


#if ENABLE_PHYSICS_SAVE_CALCULATED_VERTEX
    CCArray *calculatedVertexList = colliderBody->getCalculatedVertexList();
    
    int num = contourData->m_tVertexList.count();
    for (int i = 0; i < num; i++)
    {
        ContourVertex2 *newVertex = new ContourVertex2(0, 0);
        calculatedVertexList->addObject(newVertex);
        newVertex->release();
    }
#endif
}

void ColliderDetector::addContourDataList(Array *contourDataList)
{
    for(auto object : *contourDataList)
    {
        addContourData((ContourData *)object);
    }
}

void ColliderDetector::removeContourData(ContourData *contourData)
{
	for(auto object : *m_pColliderBodyList)
	{
		ColliderBody *body = (ColliderBody*)object;
		if (body && body->getContourData() == contourData)
		{
			m_pColliderBodyList->removeObject(body);
		}
	}
}

void ColliderDetector::removeAll()
{
    m_pColliderBodyList->removeAllObjects();
}


void ColliderDetector::setActive(bool active)
{
    if (m_bActive == active)
    {
        return;
    }

    m_bActive = active;

    if (m_pB2Body)
    {
        if (active)
        {
            setB2Body(m_pB2Body);
        }
        else
        {
            for(auto object : *m_pColliderBodyList)
            {
                ColliderBody *colliderBody = (ColliderBody *)object;
                b2Fixture *fixture = colliderBody->getB2Fixture();

                m_pB2Body->DestroyFixture(fixture);
                colliderBody->setB2Fixture(nullptr);
            }
        }
    }
    else if (m_pCPBody)
    {
        if (m_bActive)
        {
            for(auto object : *m_pColliderBodyList)
            {
                ColliderBody *colliderBody = (ColliderBody *)object;
                cpShape *shape = colliderBody->getShape();
                if(shape->space == nullptr)
                {
                    cpSpaceAddShape(m_pCPBody->space, shape);
                }
            }
        }
        else
        {
            for(auto object : *m_pColliderBodyList)
            {
                ColliderBody *colliderBody = (ColliderBody *)object;
                cpShape *shape = colliderBody->getShape();
                if (shape->space != nullptr)
                {
                    cpSpaceRemoveShape(m_pCPBody->space, shape);
                }
            }
        }
    }
}

bool ColliderDetector::getActive()
{
    return m_bActive;
}

Array *ColliderDetector::getColliderBodyList()
{
    return m_pColliderBodyList;
}

void ColliderDetector::setColliderFilter(ColliderFilter *filter)
{
    *m_pFilter = *filter;
    
    for(auto object : *m_pColliderBodyList)
    {
        ColliderBody *colliderBody = (ColliderBody *)object;
        colliderBody->setColliderFilter(filter);


        if (colliderBody->getB2Fixture())
        {
            colliderBody->getColliderFilter()->updateB2Shape(colliderBody->getB2Fixture());
        }

        if (colliderBody->getShape())
        {
            colliderBody->getColliderFilter()->updateCPShape(colliderBody->getShape());
        }
    }
}
ColliderFilter *ColliderDetector::getColliderFilter()
{
    return m_pFilter;
}


Point helpPoint;

void ColliderDetector::updateTransform(AffineTransform &t)
{
    if (!m_bActive)
    {
        return;
    }

    for(auto object : *m_pColliderBodyList)
    {
        ColliderBody *colliderBody = (ColliderBody *)object;
        ContourData *contourData = colliderBody->getContourData();

        b2PolygonShape* b2shape = nullptr;
        if (m_pB2Body != nullptr)
        {
            b2shape = (b2PolygonShape *)colliderBody->getB2Fixture()->GetShape();
        }

        cpPolyShape* cpshape = nullptr;
        if (m_pCPBody != nullptr)
        {
            cpshape = (cpPolyShape *)colliderBody->getShape();
        }

        int num = contourData->m_tVertexList.count();
        ContourVertex2 **vs = (ContourVertex2 **)contourData->m_tVertexList.data->arr;

#if ENABLE_PHYSICS_SAVE_CALCULATED_VERTEX
        ContourVertex2 **cvs = (ContourVertex2 **)colliderBody->getCalculatedVertexList()->data->arr;
#endif

        for (int i = 0; i < num; i++)
        {
            helpPoint.setPoint( vs[i]->x,  vs[i]->y);
            helpPoint = PointApplyAffineTransform(helpPoint, t);


#if ENABLE_PHYSICS_SAVE_CALCULATED_VERTEX
            cvs[i]->x = helpPoint.x;
            cvs[i]->y = helpPoint.y;
#endif

            if ( b2shape != nullptr )
            {
                b2Vec2 &bv = b2shape->m_vertices[i];
                bv.Set(helpPoint.x / PT_RATIO, helpPoint.y / PT_RATIO);
            }

            if ( cpshape != nullptr )
            {
                cpVect v ;
                v.x = helpPoint.x;
                v.y = helpPoint.y;
                cpshape->verts[i] = v;
            }
        }

		if ( cpshape != nullptr )
		{
			cpConvexHull(num, cpshape->verts, nullptr, nullptr, 0);
			for (int i = 0; i < num; i++)
			{
				cpVect b = cpshape->verts[(i + 1) % cpshape->numVerts];
				cpVect n = cpvnormalize(cpvperp(cpvsub(b, cpshape->verts[i])));

				cpshape->planes[i].n = n;
				cpshape->planes[i].d = cpvdot(n, cpshape->verts[i]);
			}
		}
    }
}

void ColliderDetector::setB2Body(b2Body *pBody)
{
    m_pB2Body = pBody;

    for(auto object : *m_pColliderBodyList)
    {
        ColliderBody *colliderBody = (ColliderBody *)object;

        ContourData *contourData = colliderBody->getContourData();

		ContourVertex2 **vs = (ContourVertex2 **)contourData->m_tVertexList.data->arr;
		int num = contourData->m_tVertexList.count();

        b2Vec2 *b2bv = new b2Vec2[num];
        for (int i = 0; i < num; i++)
        {
			b2bv[i].Set(vs[i]->x / PT_RATIO, vs[i]->y / PT_RATIO);
        }

        b2PolygonShape polygon;
        polygon.Set(b2bv, contourData->m_tVertexList.count());

        CC_SAFE_DELETE(b2bv);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &polygon;
        fixtureDef.isSensor = true;

        b2Fixture *fixture = m_pB2Body->CreateFixture(&fixtureDef);
        fixture->SetUserData(m_pBone);

        if (colliderBody->getB2Fixture() != nullptr)
        {
            m_pB2Body->DestroyFixture(colliderBody->getB2Fixture());
        }
        colliderBody->setB2Fixture(fixture);

        colliderBody->getColliderFilter()->updateB2Shape(fixture);
    }
}

b2Body *ColliderDetector::getB2Body() const
{
    return m_pB2Body;
}

KDvoid ColliderDetector::setCPBody ( cpBody* pBody )
{
    m_pCPBody = pBody;

    for(auto object : *m_pColliderBodyList)
    {
        ColliderBody *colliderBody = (ColliderBody *)object;

        ContourData *contourData = colliderBody->getContourData();

        int num = contourData->m_tVertexList.count();
        ContourVertex2 **vs = (ContourVertex2 **)contourData->m_tVertexList.data->arr;
        cpVect *verts = new cpVect[num];
        for (int i = 0; i < num; i++)
        {
            verts[num - 1 - i].x = vs[i]->x;
            verts[num - 1 - i].y = vs[i]->y;
        }

        cpShape *shape = cpPolyShapeNew(m_pCPBody, num, verts, cpvzero);

        shape->sensor = true;
        shape->data = m_pBone;

        if ( m_bActive )
        {
            cpSpaceAddShape(m_pCPBody->space, shape);
        }

        colliderBody->setShape(shape);
        colliderBody->getColliderFilter()->updateCPShape(shape);

        delete [] verts;
    }
}

cpBody* ColliderDetector::getCPBody ( KDvoid ) const
{
    return m_pCPBody;
}

}
