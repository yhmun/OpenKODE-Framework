/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsDebugNode.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2013 cocos2d-x.org
 *      Copyright (c) 2012 Scott Lembcke and Howling Moon Software
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com	
 *
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
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

#include "Precompiled.h"
#include "extensions/CCPhysics/CCPhysicsDebugNode.h"

NS_CC_BEGIN

//
//	IMPORTANT - READ ME!
//
//	This file sets pokes around in the private API a lot to provide efficient
//	debug rendering given nothing more than reference to a Chipmunk space.
//	It is not recommended to write rendering code like this in your own games
//	as the private API may change with little or no warning.
//

static ccColor4F ColorForBody ( cpBody* pBody )
{
	if ( cpBodyIsRogue ( pBody ) || cpBodyIsSleeping ( pBody ) )
    {
		return ccc4f ( 0.5f, 0.5f, 0.5f, 0.5f );
	}
    else if ( pBody->CP_PRIVATE ( node ).idleTime > pBody->CP_PRIVATE ( space )->sleepTimeThreshold )
    {
		return ccc4f ( 0.33f, 0.33f, 0.33f, 0.5f );
	}
    else
    {
		return ccc4f ( 1.0f, 0.0f, 0.0f, 0.5f );
	}
}

static CCPoint cpVert2ccp ( const cpVect& tVert )
{
    return ccp ( tVert.x, tVert.y );
}

static CCPoint* cpVertArray2ccpArrayN ( const cpVect* pVertArray, KDuint uCount )
{
    if ( uCount == 0 ) 
	{
		return KD_NULL;
	}

    CCPoint*  pPoints = new CCPoint [ uCount ];
    
    for ( KDuint i = 0; i < uCount; ++i)
    {
        pPoints [ i ].x = pVertArray [ i ].x;
        pPoints [ i ].y = pVertArray [ i ].y;
    }

    return pPoints;
}

static KDvoid DrawShape ( cpShape* pShape, CCDrawNode* pRenderer )
{
	cpBody*    pBody  = pShape->body;
	ccColor4F  tColor = ColorForBody ( pBody );
    
	switch ( pShape->CP_PRIVATE ( klass )->type )
    {
		case CP_CIRCLE_SHAPE :
        {
            cpCircleShape*  pCircle = (cpCircleShape*) pShape;
            cpVect   tCenter = pCircle->tc;
            cpFloat  fRadius = pCircle->r;
            pRenderer->drawDot ( cpVert2ccp ( tCenter ), cpfmax ( fRadius, 1.0f ), tColor );
            pRenderer->drawSegment ( cpVert2ccp ( tCenter ), cpVert2ccp ( cpvadd ( tCenter, cpvmult ( pBody->rot, fRadius ) ) ), 1.0f, tColor );
        }	break;

		case CP_SEGMENT_SHAPE:
        {
            cpSegmentShape*  pSeg = (cpSegmentShape*) pShape;
            pRenderer->drawSegment ( cpVert2ccp ( pSeg->ta ), cpVert2ccp ( pSeg->tb ), cpfmax ( pSeg->r, 2.0f ), tColor );
        }	break;

		case CP_POLY_SHAPE:
        {
            cpPolyShape*  pPoly = (cpPolyShape*) pShape;
            ccColor4F     tLine = tColor;
            tLine.a = cpflerp ( tColor.a, 1.0f, 0.5f );
            CCPoint*    pPoints = cpVertArray2ccpArrayN ( pPoly->tVerts, pPoly->numVerts );
            pRenderer->drawPolygon ( pPoints, pPoly->numVerts, tColor, 1.0, tLine );
            CC_SAFE_DELETE_ARRAY ( pPoints );

		}	break;
                    
		default :
			cpAssertHard ( KD_FALSE, "Bad assertion in DrawShape()" );
	}
}

static ccColor4F  CONSTRAINT_COLOR = { 0, 1, 0, 0.5f };

static KDvoid DrawConstraint ( cpConstraint* pConstraint, CCDrawNode* pRenderer )
{
	cpBody*  pBodyA = pConstraint->a;
	cpBody*  pBodyB = pConstraint->b;
    
	const cpConstraintClass*  pKlass = pConstraint->CP_PRIVATE ( klass );
	if ( pKlass == cpPinJointGetClass ( ) )
    {
		cpPinJoint*  pJoint = (cpPinJoint*) pConstraint;
		
		cpVect  a = cpBodyLocal2World ( pBodyA, pJoint->anchr1 );
		cpVect  b = cpBodyLocal2World ( pBodyB, pJoint->anchr2 );
		
        pRenderer->drawDot ( cpVert2ccp ( a ), 3.0f, CONSTRAINT_COLOR );
        pRenderer->drawDot ( cpVert2ccp ( b ), 3.0f, CONSTRAINT_COLOR );
        pRenderer->drawSegment ( cpVert2ccp ( a ), cpVert2ccp ( b ), 1.0f, CONSTRAINT_COLOR );
	}
    else if ( pKlass == cpSlideJointGetClass ( ) )
    {
		cpSlideJoint*  pJoint = (cpSlideJoint*) pConstraint;
        
		cpVect  a = cpBodyLocal2World ( pBodyA, pJoint->anchr1 );
		cpVect  b = cpBodyLocal2World ( pBodyB, pJoint->anchr2 );
        
        pRenderer->drawDot ( cpVert2ccp ( a ), 3.0f, CONSTRAINT_COLOR );
        pRenderer->drawDot ( cpVert2ccp ( b ), 3.0f, CONSTRAINT_COLOR );
        pRenderer->drawSegment ( cpVert2ccp ( a ), cpVert2ccp ( b ), 1.0f, CONSTRAINT_COLOR );
	}
    else if ( pKlass == cpPivotJointGetClass ( ) )
    {
		cpPivotJoint*  pJoint = (cpPivotJoint*) pConstraint;
        
		cpVect  a = cpBodyLocal2World ( pBodyA, pJoint->anchr1 );
		cpVect  b = cpBodyLocal2World ( pBodyB, pJoint->anchr2 );
        
        pRenderer->drawDot ( cpVert2ccp ( a ), 3.0f, CONSTRAINT_COLOR );
        pRenderer->drawDot ( cpVert2ccp ( b ), 3.0f, CONSTRAINT_COLOR );
	}
    else if ( pKlass == cpGrooveJointGetClass ( ) )
    {
		cpGrooveJoint*  pJoint = (cpGrooveJoint*) pConstraint;
        
		cpVect  a = cpBodyLocal2World ( pBodyA, pJoint->grv_a );
		cpVect  b = cpBodyLocal2World ( pBodyA, pJoint->grv_b );
		cpVect  c = cpBodyLocal2World ( pBodyB, pJoint->anchr2 );
        
        pRenderer->drawDot ( cpVert2ccp ( c ), 3.0f, CONSTRAINT_COLOR );
        pRenderer->drawSegment ( cpVert2ccp ( a ), cpVert2ccp ( b ), 1.0f, CONSTRAINT_COLOR);
	}
    else if ( pKlass == cpDampedSpringGetClass ( ) )
    {
		// TODO
	}
}

//
// implementation of CCPhysicsDebugNode
//

KDvoid CCPhysicsDebugNode::draw ( KDvoid )
{
    if ( !m_pSpacePtr )
    {
        return;
    }
    
    cpSpaceEachShape ( m_pSpacePtr, (cpSpaceShapeIteratorFunc) DrawShape, this );
	cpSpaceEachConstraint ( m_pSpacePtr, (cpSpaceConstraintIteratorFunc) DrawConstraint, this );
    
    CCDrawNode::draw  ( );
    CCDrawNode::clear ( );
}

CCPhysicsDebugNode::CCPhysicsDebugNode ( KDvoid )
{
	m_pSpacePtr = KD_NULL;
}

CCPhysicsDebugNode* CCPhysicsDebugNode::create ( cpSpace* pSpace )
{
    CCPhysicsDebugNode*  pNode = new CCPhysicsDebugNode ( );

    if ( pNode && pNode->init ( ) )
    {                
        pNode->m_pSpacePtr = pSpace;        
        pNode->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pNode );
    }
    
    return pNode;
}

CCPhysicsDebugNode::~CCPhysicsDebugNode ( KDvoid )
{

}

cpSpace* CCPhysicsDebugNode::getSpace ( KDvoid ) const
{
    return m_pSpacePtr;
}

KDvoid CCPhysicsDebugNode::setSpace ( cpSpace* pSpace )
{
    m_pSpacePtr = pSpace;
}

NS_CC_END
