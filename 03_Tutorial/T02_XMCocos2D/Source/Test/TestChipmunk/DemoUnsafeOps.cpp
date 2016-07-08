/* --------------------------------------------------------------------------
 *
 *      File            DemoUnsafeOps.cpp
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
#include "XMChipmunk/chipmunk_unsafe.h"

static cpSpace* space;

#define NUM_CIRCLES 30
cpShape* circles[NUM_CIRCLES];
cpFloat  circleRadius = 30.0f;

static KDvoid update ( KDint ticks )
{
	if ( arrowDirection.y )
	{
		circleRadius = cpfmax ( 10.0f, circleRadius + arrowDirection.y );
		
		for ( KDint i = 0; i < NUM_CIRCLES; i++ )
		{
			circles[i]->body->m = cpMomentForCircle ( 1.0f, 0.0f, circleRadius, cpvzero );
			cpCircleShapeSetRadius ( circles[i], circleRadius );
		}
	}
	
	KDint steps = 1;
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
	}
}

static cpSpace* init ( KDvoid )
{	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->iterations = 5;
	space->gravity = cpv ( 0, -100 );
	
	cpBody *body, *staticBody = space->staticBody;
	cpShape* shape;
	
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( -320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( 320, -240 ), cpv ( 320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( 320, -240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	
	for ( KDint i = 0; i < NUM_CIRCLES; i++ )
	{
		body = cpSpaceAddBody ( space, cpBodyNew ( 1.0f, cpMomentForCircle ( 1.0f, 0.0f, circleRadius, cpvzero ) ) );
		body->p = cpvmult ( cpv ( frand ( ) * 2.0f - 1.0f, frand ( ) * 2.0f - 1.0f ), circleRadius * 5.0f );
		
		circles[i] = shape = cpSpaceAddShape ( space, cpCircleShapeNew ( body, circleRadius, cpvzero ) );
		shape->e = 0.0f; shape->u = 1.0f;
	}
	
	kdStrcat ( messageString,
		"chipmunk_unsafe.h Contains functions for changing shapes, but they can cause severe stability problems if used incorrectly.\n"
		"Shape changes occur as instantaneous changes to position without an accompanying velocity change. USE WITH CAUTION!" );
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo UnsafeOps =
{
	"Unsafe Operations",
	KD_NULL,
	init,
	update,
	destroy,
};
