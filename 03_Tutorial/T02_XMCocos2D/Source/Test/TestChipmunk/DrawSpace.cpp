/* --------------------------------------------------------------------------
 *
 *      File            DrawSpace.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2007      Scott Lembcke. All rights reserved. 
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

#include "Precompiled.h"
#include "../TestChipmunk2.h"
#include "XMChipmunk/chipmunk_private.h"


/*
	IMPORTANT - READ ME!
	
	This file sets up a simple interface that the individual demos can use to get
	a Chipmunk space running and draw what's in it. In order to keep the Chipmunk
	examples clean and simple, they contain no graphics code. All drawing is done
	by accessing the Chipmunk structures at a very low level. It is NOT
	recommended to write a game or application this way as it does not scale
	beyond simple shape drawing and is very dependent on implementation details
	about Chipmunk which may change with little to no warning.
*/

#define LINE_COLOR                 0.0f, 0.0f, 0.0f, 1.0f

static KDvoid glColor_from_hash ( cpHashValue hash )
{
	unsigned long val = (unsigned long) hash;
	
	// hash the pointer up nicely
	val = ( val + 0x7ed55d16 ) + ( val << 12 );
	val = ( val ^ 0xc761c23c ) ^ ( val >> 19 );
	val = ( val + 0x165667b1 ) + ( val <<  5 );
	val = ( val + 0xd3a2646c ) ^ ( val <<  9 );
	val = ( val + 0xfd7046c5 ) + ( val <<  3 );
	val = ( val ^ 0xb55a4f09 ) ^ ( val >> 16 );

	GLubyte r = ( val >>  0 ) & 0xFF;
	GLubyte g = ( val >>  8 ) & 0xFF;
	GLubyte b = ( val >> 16 ) & 0xFF;
	
	GLubyte max = r > g ? ( r > b ? r : b ) : ( g > b ? g : b );
	
	const KDint mult = 255;
	const KDint add  = 0;
	r = ( r * mult ) / max + add;
	g = ( g * mult ) / max + add;
	b = ( b * mult ) / max + add;
	
	ccDrawColor4B ( r, g, b, 196 );
}

static KDvoid glColor_for_shape ( cpShape* shape, cpSpace* space )
{
	cpBody* body = shape->body;
	if ( body )
	{
		if ( cpBodyIsSleeping ( body ) )
		{
			GLfloat v = 0.2f;
			ccDrawColor4F ( v, v, v, 1.0f );
			return;
		} 
		else if ( body->node.idleTime > space->sleepTimeThreshold ) 
		{
			GLfloat v = 0.66f;
			ccDrawColor4F ( v, v, v, 1.0f );
			return;
		}
	}
	
	glColor_from_hash ( shape->hashid );
}

static const GLfloat circleVAR[] =
{
	 0.0000f,  1.0000f,
	 0.2588f,  0.9659f,
	 0.5000f,  0.8660f,
	 0.7071f,  0.7071f,
	 0.8660f,  0.5000f,
	 0.9659f,  0.2588f,
	 1.0000f,  0.0000f,
	 0.9659f, -0.2588f,
	 0.8660f, -0.5000f,
	 0.7071f, -0.7071f,
	 0.5000f, -0.8660f,
	 0.2588f, -0.9659f,
	 0.0000f, -1.0000f,
	-0.2588f, -0.9659f,
	-0.5000f, -0.8660f,
	-0.7071f, -0.7071f,
	-0.8660f, -0.5000f,
	-0.9659f, -0.2588f,
	-1.0000f, -0.0000f,
	-0.9659f,  0.2588f,
	-0.8660f,  0.5000f,
	-0.7071f,  0.7071f,
	-0.5000f,  0.8660f,
	-0.2588f,  0.9659f,
	 0.0000f,  1.0000f,
	 0.0f, 0.0f, // For an extra line to see the rotation.
};
static const KDint circleVAR_count = sizeof ( circleVAR ) / sizeof ( GLfloat ) / 2;

static KDvoid drawCircleShape ( cpBody *body, cpCircleShape *circle, cpSpace* space )
{
	if ( !circle->shape.sensor )
	{
		glColor_for_shape ( (cpShape*) circle, space );
		ccDrawSolidCircle  ( circle->tc,  circle->r, 0, circleVAR_count, KD_TRUE );
	}
	
	ccDrawColor4F ( LINE_COLOR );
	ccDrawCircle  ( circle->tc,  circle->r, 0, circleVAR_count, KD_TRUE );
}

static const GLfloat pillVAR[] =
{
	 0.0000f,  1.0000f, 1.0f,
	 0.2588f,  0.9659f, 1.0f,
	 0.5000f,  0.8660f, 1.0f,
	 0.7071f,  0.7071f, 1.0f,
	 0.8660f,  0.5000f, 1.0f,
	 0.9659f,  0.2588f, 1.0f,
	 1.0000f,  0.0000f, 1.0f,
	 0.9659f, -0.2588f, 1.0f,
	 0.8660f, -0.5000f, 1.0f,
	 0.7071f, -0.7071f, 1.0f,
	 0.5000f, -0.8660f, 1.0f,
	 0.2588f, -0.9659f, 1.0f,
	 0.0000f, -1.0000f, 1.0f,

	 0.0000f, -1.0000f, 0.0f,
	-0.2588f, -0.9659f, 0.0f,
	-0.5000f, -0.8660f, 0.0f,
	-0.7071f, -0.7071f, 0.0f,
	-0.8660f, -0.5000f, 0.0f,
	-0.9659f, -0.2588f, 0.0f,
	-1.0000f, -0.0000f, 0.0f,
	-0.9659f,  0.2588f, 0.0f,
	-0.8660f,  0.5000f, 0.0f,
	-0.7071f,  0.7071f, 0.0f,
	-0.5000f,  0.8660f, 0.0f,
	-0.2588f,  0.9659f, 0.0f,
	 0.0000f,  1.0000f, 0.0f,
};
static const KDint pillVAR_count = sizeof ( pillVAR ) / sizeof ( GLfloat ) / 3;

static KDvoid drawSegmentShape ( cpBody* body, cpSegmentShape* seg, cpSpace* space )
{
	cpVect a = seg->ta;
	cpVect b = seg->tb;
	
	if ( seg->r )
	{			
		KDint		nCount     = pillVAR_count;
		CCPoint*	pNewPoints = new CCPoint [ nCount ];

		cpVect		d = cpvsub ( b, a );
		cpVect		r = cpvmult ( d, seg->r / cpvlength ( d ) );

		kmMat4		matrix =
		{
			(GLfloat)  r.x, (GLfloat) r.y, 0.0f, 0.0f,
			(GLfloat) -r.y, (GLfloat) r.x, 0.0f, 0.0f,
			(GLfloat)  d.x, (GLfloat) d.y, 0.0f, 0.0f,
			(GLfloat)  a.x, (GLfloat) a.y, 0.0f, 1.0f,
		};
		
		for ( KDint i = 0; i < nCount; i++ )
		{
			KDint   offset = i * 3;				
			kmVec3  pill = { pillVAR [ offset + 0 ], pillVAR [ offset + 1 ], pillVAR [ offset + 2 ] };
			kmVec3  vect;

			kmVec3Transform ( &vect, &pill, &matrix );

			pNewPoints [ i ].x = vect.x;
			pNewPoints [ i ].y = vect.y;
		};
			
		if ( !seg->shape.sensor )
		{
			glColor_for_shape ( (cpShape *) seg, space );
			ccDrawSolidPoly ( pNewPoints, nCount );
		}
		
		ccDrawColor4F ( LINE_COLOR );
		ccDrawPoly ( pNewPoints, nCount, KD_TRUE );

		CC_SAFE_DELETE_ARRAY ( pNewPoints );
	} 
	else 
	{
		ccDrawColor4F ( LINE_COLOR );
		ccDrawLine ( ccp ( a.x, a.y ), ccp ( b.x, b.y ) );
	}
}

static KDvoid drawPolyShape ( cpBody* body, cpPolyShape* poly, cpSpace* space )
{
	KDint     nCount     = poly->numVerts;
	CCPoint*  pNewPoints = new CCPoint [ nCount ];

	for ( KDint  i = 0; i < nCount; i++ )
	{
		pNewPoints [ i ].x = poly->tVerts [ i ].x;
		pNewPoints [ i ].y = poly->tVerts [ i ].y;
	}

	if ( !poly->shape.sensor )
	{
		glColor_for_shape ( (cpShape*) poly, space );
		ccDrawSolidPoly   ( pNewPoints, nCount );
	}
	
	ccDrawColor4F ( LINE_COLOR );
	ccDrawPoly    ( pNewPoints, nCount, KD_TRUE );

	CC_SAFE_DELETE_ARRAY ( pNewPoints );
}

static KDvoid drawObject ( cpShape* shape, cpSpace* space )
{
	cpBody*   body = shape->body;
	
	switch ( shape->klass->type )
	{
		case CP_CIRCLE_SHAPE  :	drawCircleShape  ( body, (cpCircleShape*)  shape, space ); break;
		case CP_SEGMENT_SHAPE :	drawSegmentShape ( body, (cpSegmentShape*) shape, space ); break;
		case CP_POLY_SHAPE    :	drawPolyShape    ( body, (cpPolyShape*)    shape, space ); break;
		default : ;
	}
}

static const GLfloat springVAR[] =
{
	0.00f, 0.0f,
	0.20f, 0.0f,
	0.25f, 3.0f,
	0.30f,-6.0f,
	0.35f, 6.0f,
	0.40f,-6.0f,
	0.45f, 6.0f,
	0.50f,-6.0f,
	0.55f, 6.0f,
	0.60f,-6.0f,
	0.65f, 6.0f,
	0.70f,-3.0f,
	0.75f, 6.0f,
	0.80f, 0.0f,
	1.00f, 0.0f,
};

static const KDint springVAR_count = sizeof ( springVAR ) / sizeof ( GLfloat ) / 2;

static KDvoid drawSpring ( cpDampedSpring* spring, cpBody* body_a, cpBody* body_b )
{
	cpVect	a = cpvadd ( body_a->p, cpvrotate ( spring->anchr1, body_a->rot ) );
	cpVect	b = cpvadd ( body_b->p, cpvrotate ( spring->anchr2, body_b->rot ) );

	ccPointSize ( 5.0f );
	ccDrawLine  (  ccp ( a.x, a.y ), ccp ( b.x, b.y ) );

	cpVect		delta = cpvsub ( b, a );
	
	KDint		nCount     = springVAR_count;
	CCPoint*	pNewPoints = new CCPoint [ nCount ];

	GLfloat		x = a.x;
	GLfloat		y = a.y;
	GLfloat		cos = delta.x;
	GLfloat		sin = delta.y;
	GLfloat		s = 1.0f / cpvlength ( delta );

	kmMat4		matrix = 
	{
		     cos,     sin, 0.0f, 0.0f,
		-sin * s, cos * s, 0.0f, 0.0f,
		    0.0f,    0.0f, 1.0f, 1.0f,
		       x,       y, 0.0f, 1.0f,
	};
	
	for ( KDint i = 0; i < nCount; i++ )
	{
		KDint   offset = i * 2;				
		kmVec3  spring = { springVAR [ offset + 0 ], springVAR [ offset + 1 ], 0 };
		kmVec3  vect;

		kmVec3Transform ( &vect, &spring, &matrix );

		pNewPoints [ i ].x = vect.x;
		pNewPoints [ i ].y = vect.y;
	};

	ccDrawPoly ( pNewPoints, nCount, KD_FALSE );

	CC_SAFE_DELETE_ARRAY ( pNewPoints );
}

static KDvoid drawConstraint ( cpConstraint *constraint )
{
	cpBody* body_a = constraint->a;
	cpBody* body_b = constraint->b;

	const cpConstraintClass* klass = constraint->klass;
	if ( klass == cpPinJointGetClass ( ) )
	{
		cpPinJoint*  joint = (cpPinJoint*) constraint;
	
		cpVect a = cpvadd ( body_a->p, cpvrotate ( joint->anchr1, body_a->rot ) );
		cpVect b = cpvadd ( body_b->p, cpvrotate ( joint->anchr2, body_b->rot ) );

		ccPointSize ( 5.0f );
		ccDrawPoint (  ccp ( a.x, a.y ) );
		ccDrawPoint (  ccp ( b.x, b.y ) );

		ccDrawLine (  ccp ( a.x, a.y ), ccp ( b.x, b.y ) );
	}
	else if ( klass == cpSlideJointGetClass ( ) )
	{
		cpSlideJoint*  joint = (cpSlideJoint*) constraint;
	
		cpVect a = cpvadd ( body_a->p, cpvrotate ( joint->anchr1, body_a->rot ) );
		cpVect b = cpvadd ( body_b->p, cpvrotate ( joint->anchr2, body_b->rot ) );

		ccPointSize ( 5.0f );
		ccDrawPoint (  ccp ( a.x, a.y ) );
		ccDrawPoint (  ccp ( b.x, b.y ) );

		ccDrawLine (  ccp ( a.x, a.y ), ccp ( b.x, b.y ) );
	} 
	else if ( klass == cpPivotJointGetClass ( ) )
	{
		cpPivotJoint *joint = ( cpPivotJoint * )constraint;
	
		cpVect a = cpvadd ( body_a->p, cpvrotate ( joint->anchr1, body_a->rot ) );
		cpVect b = cpvadd ( body_b->p, cpvrotate ( joint->anchr2, body_b->rot ) );

		ccPointSize ( 10.0f );
		ccDrawPoint (  ccp ( a.x, a.y ) );
		ccDrawPoint (  ccp ( b.x, b.y ) );
	} 
	else if ( klass == cpGrooveJointGetClass ( ) )
	{
		cpGrooveJoint *joint = ( cpGrooveJoint * )constraint;
	
		cpVect a = cpvadd ( body_a->p, cpvrotate ( joint->grv_a, body_a->rot ) );
		cpVect b = cpvadd ( body_a->p, cpvrotate ( joint->grv_b, body_a->rot ) );
		cpVect c = cpvadd ( body_b->p, cpvrotate ( joint->anchr2, body_b->rot ) );

		ccPointSize ( 5.0f );
		ccDrawPoint (  ccp ( c.x, c.y ) );
		ccDrawLine  (  ccp ( a.x, a.y ), ccp ( b.x, b.y ) );
	} 
	else if ( klass == cpDampedSpringGetClass ( ) )
	{
		drawSpring ( (cpDampedSpring*) constraint, body_a, body_b );
	} 
	else 
	{
//		kdLogMessagefKHR ( "Cannot draw constraint" );
	}
}

static KDvoid drawBB ( KDvoid* ptr, KDvoid* unused )
{
	cpShape*  shape = (cpShape*) ptr;

	CCPoint vertices[] =
	{
		ccp ( shape->bb.l, shape->bb.b ),
		ccp ( shape->bb.l, shape->bb.t ),
		ccp ( shape->bb.r, shape->bb.t ),
		ccp ( shape->bb.r, shape->bb.b ),
	};

	ccDrawPoly ( vertices, 4, KD_FALSE );
}

KDvoid cpBBTreeRenderDebug ( cpSpatialIndex* index );
KDvoid cpSpaceHashRenderDebug ( cpSpatialIndex* index );

KDvoid drawSpace ( cpSpace* space, drawSpaceOptions* options )
{	
	if ( options->drawHash )
	{

	}
	
	ccLineWidth ( options->lineThickness );
	if ( options->drawShapes )
	{
		cpSpatialIndexEach ( space->activeShapes, (cpSpatialIndexIteratorFunc) drawObject, space );
		cpSpatialIndexEach ( space->staticShapes, (cpSpatialIndexIteratorFunc) drawObject, space );
	}
	
	ccLineWidth ( 1.0f );
	if ( options->drawBBs )
	{
		ccDrawColor4F ( 0.3f, 0.5f, 0.3f, 1.0f );
		cpSpatialIndexEach ( space->activeShapes, (cpSpatialIndexIteratorFunc) drawBB, KD_NULL );
		cpSpatialIndexEach ( space->staticShapes, (cpSpatialIndexIteratorFunc) drawBB, KD_NULL );
	}

	cpArray*  constraints = space->constraints;

	ccDrawColor4F ( 0.0f, 0.0f, 0.5f, 1.0f );
	for ( KDint i=0, count = constraints->num; i < count; i++ )
	{
		drawConstraint ( (cpConstraint *) constraints->arr[i] );
	}
	
	if ( options->bodyPointSize ) 
	{
		ccPointSize ( options->bodyPointSize );

		ccDrawColor4F ( LINE_COLOR );
		cpArray*  bodies = space->bodies;

		for ( KDint i = 0, count = bodies->num; i < count; i++ )
		{
			cpBody* body = (cpBody *) bodies->arr[i];
			ccDrawPoint ( ccp ( body->p.x, body->p.y ) );
		}
	}

	if ( options->collisionPointSize )
	{
		cpArray*  arbiters = space->arbiters;
		
		ccDrawColor4F ( 0.0f, 1.0f, 0.0f, 1.0f );
		ccPointSize   ( 2.0f * options->collisionPointSize );
		
		for ( KDint i = 0; i < arbiters->num; i++ )
		{
			cpArbiter* arb = (cpArbiter*) arbiters->arr[i];
			if ( arb->state != cpArbiterStateFirstColl )
			{
				continue;
			}

			for ( KDint i = 0; i < arb->numContacts; i++ )
			{
				cpVect v = arb->contacts[i].p;
				ccDrawPoint ( ccp ( v.x, v.y ) );
			}			
		}

		ccDrawColor4F ( 1.0f, 0.0f, 0.0f, 1.0f );
		ccPointSize   ( options->collisionPointSize );
		for ( KDint i = 0; i < arbiters->num; i++ )
		{
			cpArbiter* arb = (cpArbiter*) arbiters->arr[i];
			if ( arb->state == cpArbiterStateFirstColl )
			{ 
				continue;
			}

			for ( KDint i=0; i < arb->numContacts; i++ )
			{
				cpVect v = arb->contacts[i].p;
				ccDrawPoint ( ccp ( v.x, v.y ) );
			}
		}
	}
}

static KDvoid shapeFreeWrap ( cpShape* ptr, KDvoid* unused )
{
	cpShapeFree ( ptr );
}

KDvoid ChipmunkDemoFreeSpaceChildren ( cpSpace* space )
{
	cpArray* components = space->sleepingComponents;
	while ( components->num ) cpBodyActivate(  (cpBody *) components->arr[0] );
	
	cpSpatialIndexEach ( space->staticShapes, (cpSpatialIndexIteratorFunc) &shapeFreeWrap, KD_NULL );
	cpSpatialIndexEach ( space->activeShapes, (cpSpatialIndexIteratorFunc) &shapeFreeWrap, KD_NULL );
	
  //  void cpArrayFreeEach(cpArray *arr, void (freeFunc)(void*));
    
	cpArrayFreeEach ( space->bodies, (void (*) (void*)) cpBodyFree );
	cpArrayFreeEach ( space->constraints, (void (*) (void*)) cpConstraintFree );
}