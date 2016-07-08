/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsDebugNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org
 *      Copyright (c) 2012      Scott Lembcke and Howling Moon Software
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

#include "extensions/physics-nodes/CCPhysicsDebugNode.h"
#include "ccTypes.h"
#include "base/CCGeometry.h"

NS_CC_EXT_BEGIN

/*
 IMPORTANT - READ ME!
 
 This file sets pokes around in the private API a lot to provide efficient
 debug rendering given nothing more than reference to a Chipmunk space.
 It is not recommended to write rendering code like this in your own games
 as the private API may change with little or no warning.
 */

static Color4F ColorForBody(cpBody *body)
{
	if (cpBodyIsRogue(body) || cpBodyIsSleeping(body))
    {
		return Color4F(0.5f, 0.5f, 0.5f ,0.5f);
	}
    else if (body->CP_PRIVATE(node).idleTime > body->CP_PRIVATE(space)->sleepTimeThreshold)
    {
		return Color4F(0.33f, 0.33f, 0.33f, 0.5f);
	}
    else
    {
		return Color4F(1.0f, 0.0f, 0.0f, 0.5f);
	}
}

static Point cpVert2Point(const cpVect &vert)
{
    return Point(vert.x, vert.y);
}

static Point* cpVertArray2ccpArrayN(const cpVect* cpVertArray, unsigned int count)
{
    if (count == 0) return NULL;
    Point* pPoints = new Point[count];
    
    for (unsigned int i = 0; i < count; ++i)
    {
        pPoints[i].x = cpVertArray[i].x;
        pPoints[i].y = cpVertArray[i].y;
    }
    return pPoints;
}

static void DrawShape(cpShape *shape, DrawNode *renderer)
{
	cpBody *body = shape->body;
	Color4F color = ColorForBody(body);
    
	switch (shape->CP_PRIVATE(klass)->type)
    {
		case CP_CIRCLE_SHAPE:
        {
            cpCircleShape *circle = (cpCircleShape *)shape;
            cpVect center = circle->tc;
            cpFloat radius = circle->r;
            renderer->drawDot(cpVert2Point(center), cpfmax(radius, 1.0), color);
            renderer->drawSegment(cpVert2Point(center), cpVert2Point(cpvadd(center, cpvmult(body->rot, radius))), 1.0, color);
        }
             break;
		case CP_SEGMENT_SHAPE:
        {
            cpSegmentShape *seg = (cpSegmentShape *)shape;
            renderer->drawSegment(cpVert2Point(seg->ta), cpVert2Point(seg->tb), cpfmax(seg->r, 2.0), color);
        }
            break;
		case CP_POLY_SHAPE:
        {
            cpPolyShape *poly = (cpPolyShape *)shape;
            Color4F line = color;
            line.a = cpflerp(color.a, 1.0, 0.5);
            Point* pPoints = cpVertArray2ccpArrayN(poly->tVerts, poly->numVerts);
            renderer->drawPolygon(pPoints, poly->numVerts, color, 1.0, line);
            CC_SAFE_DELETE_ARRAY(pPoints);
        }
            break;
		default:
			cpAssertHard(false, "Bad assertion in DrawShape()");
	}
}

static Color4F CONSTRAINT_COLOR(0, 1, 0, 0.5);

static void DrawConstraint(cpConstraint *constraint, DrawNode *renderer)
{
	cpBody *body_a = constraint->a;
	cpBody *body_b = constraint->b;
    
	const cpConstraintClass *klass = constraint->CP_PRIVATE(klass);
	if (klass == cpPinJointGetClass())
    {
		cpPinJoint *joint = (cpPinJoint *)constraint;
		
		cpVect a = cpBodyLocal2World(body_a, joint->anchr1);
		cpVect b = cpBodyLocal2World(body_b, joint->anchr2);
		
        renderer->drawDot(cpVert2Point(a), 3.0, CONSTRAINT_COLOR);
        renderer->drawDot(cpVert2Point(b), 3.0, CONSTRAINT_COLOR);
        renderer->drawSegment(cpVert2Point(a), cpVert2Point(b), 1.0, CONSTRAINT_COLOR);
	}
    else if (klass == cpSlideJointGetClass())
    {
		cpSlideJoint *joint = (cpSlideJoint *)constraint;
        
		cpVect a = cpBodyLocal2World(body_a, joint->anchr1);
		cpVect b = cpBodyLocal2World(body_b, joint->anchr2);
        
        renderer->drawDot(cpVert2Point(a), 3.0, CONSTRAINT_COLOR);
        renderer->drawDot(cpVert2Point(b), 3.0, CONSTRAINT_COLOR);
        renderer->drawSegment(cpVert2Point(a), cpVert2Point(b), 1.0, CONSTRAINT_COLOR);
	}
    else if (klass == cpPivotJointGetClass())
    {
		cpPivotJoint *joint = (cpPivotJoint *)constraint;
        
		cpVect a = cpBodyLocal2World(body_a, joint->anchr1);
		cpVect b = cpBodyLocal2World(body_b, joint->anchr2);
        
        renderer->drawDot(cpVert2Point(a), 3.0, CONSTRAINT_COLOR);
        renderer->drawDot(cpVert2Point(b), 3.0, CONSTRAINT_COLOR);
	}
    else if (klass == cpGrooveJointGetClass())
    {
		cpGrooveJoint *joint = (cpGrooveJoint *)constraint;
        
		cpVect a = cpBodyLocal2World(body_a, joint->grv_a);
		cpVect b = cpBodyLocal2World(body_a, joint->grv_b);
		cpVect c = cpBodyLocal2World(body_b, joint->anchr2);
        
        renderer->drawDot(cpVert2Point(c), 3.0, CONSTRAINT_COLOR);
        renderer->drawSegment(cpVert2Point(a), cpVert2Point(b), 1.0, CONSTRAINT_COLOR);
	}
    else if (klass == cpDampedSpringGetClass())
    {
		// TODO
	}
    else
    {
        //		printf("Cannot draw constraint\n");
	}
}

// implementation of PhysicsDebugNode

void PhysicsDebugNode::draw()
{
    if (! m_pSpacePtr)
    {
        return;
    }
    
    cpSpaceEachShape(m_pSpacePtr, (cpSpaceShapeIteratorFunc)DrawShape, this);
	cpSpaceEachConstraint(m_pSpacePtr, (cpSpaceConstraintIteratorFunc)DrawConstraint, this);
    
    DrawNode::draw();
    DrawNode::clear();
}

PhysicsDebugNode::PhysicsDebugNode()
: m_pSpacePtr(NULL)
{}

PhysicsDebugNode* PhysicsDebugNode::create(cpSpace *space)
{
    PhysicsDebugNode *node = new PhysicsDebugNode();
    if (node)
    {
        node->init();
        
        node->m_pSpacePtr = space;
        
        node->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(node);
    }
    
    return node;
}

PhysicsDebugNode::~PhysicsDebugNode()
{
}

cpSpace* PhysicsDebugNode::getSpace() const
{
    return m_pSpacePtr;
}

void PhysicsDebugNode::setSpace(cpSpace *space)
{
    m_pSpacePtr = space;
}

NS_CC_EXT_END
