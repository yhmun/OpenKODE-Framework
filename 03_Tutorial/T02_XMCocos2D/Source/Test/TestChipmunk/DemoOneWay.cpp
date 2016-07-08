/* --------------------------------------------------------------------------
 *
 *      File            DemoOneWay.cpp
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

static cpSpace* space;

typedef struct OneWayPlatform
{
	cpVect   n; // direction objects may pass through
	cpArray* passThruList; // list of objects passing through
} OneWayPlatform;

static OneWayPlatform platformInstance;

static int
preSolve ( cpArbiter* arb, cpSpace* space, KDvoid* ignore )
{
	CP_ARBITER_GET_SHAPES ( arb, a, b );
	OneWayPlatform *platform = ( OneWayPlatform* ) ( a->data );
	
	if ( cpvdot ( cpArbiterGetNormal ( arb, 0 ), platform->n ) < 0 )
	{
		cpArbiterIgnore ( arb );
		return cpFalse;
	}
	
	return cpTrue;
}

static KDvoid update ( KDint ticks )
{
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
	space->iterations = 10;
	space->gravity = cpv ( 0, -100 );
	
	cpBody *body, *staticBody = space->staticBody;
	cpShape* shape;
	
	// Create segments around the edge of the screen.
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( -320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( 320, -240 ), cpv ( 320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( 320, -240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	
	// Add our one way segment
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -160,-100 ), cpv ( 160,-100 ), 10.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->collision_type = 1;
	shape->layers = NOT_GRABABLE_MASK;
	
	// We'll use the data pointer for the OneWayPlatform struct
	platformInstance.n = cpv ( 0, 1 ); // let objects pass upwards
	shape->data = &platformInstance;
		
	// Add a ball to make things more interesting
	cpFloat radius = 15.0f;
	body = cpSpaceAddBody ( space, cpBodyNew ( 10.0f, cpMomentForCircle ( 10.0f, 0.0f, radius, cpvzero ) ) );
	body->p = cpv ( 0, -200 );
	body->v = cpv ( 0, 170 );

	shape = cpSpaceAddShape ( space, cpCircleShapeNew ( body, radius, cpvzero ) );
	shape->e = 0.0f; shape->u = 0.9f;
	shape->collision_type = 2;
	
	cpSpaceAddCollisionHandler ( space, 1, 2, KD_NULL, preSolve, KD_NULL, KD_NULL, KD_NULL );
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo OneWay = 
{
	"One Way Platforms",
	KD_NULL,
	init,
	update,
	destroy,
};
