/* --------------------------------------------------------------------------
 *
 *      File            DemoSensors.cpp
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

enum CollisionTypes 
{
	BALL_TYPE,
	BLOCKING_SENSOR_TYPE,
	CATCH_SENSOR_TYPE,
};

typedef struct Emitter 
{
	KDint   queue;
	KDint   blocked;
	cpVect  position;
} Emitter;

static Emitter emitterInstance;

static cpBool blockerBegin ( cpArbiter* arb, cpSpace* space, KDvoid* unused )
{
	CP_ARBITER_GET_SHAPES ( arb, a, b );
	Emitter* emitter = ( Emitter* ) ( a->data );
	
	emitter->blocked++;
	
	return cpFalse; // Return values from sensors callbacks are ignored,
}

static KDvoid blockerSeparate ( cpArbiter* arb, cpSpace* space, KDvoid* unused )
{
	CP_ARBITER_GET_SHAPES ( arb, a, b );
	Emitter* emitter = ( Emitter* ) ( a->data );
	
	emitter->blocked--;
}

static KDvoid postStepRemove ( cpSpace* space, cpShape* shape, KDvoid* unused )
{
	cpSpaceRemoveBody ( space, shape->body );
	cpSpaceRemoveShape ( space, shape );
	
	cpBodyFree ( shape->body );
	cpShapeFree ( shape );
}

static cpBool catcherBarBegin ( cpArbiter* arb, cpSpace* space, KDvoid* unused )
{
	cpShape* a, *b; cpArbiterGetShapes ( arb, &a, &b );
	Emitter* emitter = ( Emitter* ) ( a->data );
	
	emitter->queue++;
	cpSpaceAddPostStepCallback ( space, ( cpPostStepFunc ) postStepRemove, b, KD_NULL );
	
	return cpFalse;
}

static cpFloat frand_unit ( ) { return 2.0f * ( (cpFloat) kdRand ( ) / (cpFloat) KD_RAND_MAX ) - 1.0f; }

static KDvoid update ( KDint ticks )
{
	KDint steps = 1;
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	if ( !emitterInstance.blocked && emitterInstance.queue )
	{
		emitterInstance.queue--;
		
		cpBody* body = cpSpaceAddBody ( space, cpBodyNew ( 1.0f, cpMomentForCircle ( 1.0f, 15.0f, 0.0f, cpvzero ) ) );
		body->p = emitterInstance.position;
		body->v = cpvmult ( cpv ( frand_unit ( ), frand_unit ( ) ), 100.0f );
		
		cpShape* shape = cpSpaceAddShape ( space, cpCircleShapeNew ( body, 15.0f, cpvzero ) );
		shape->collision_type = BALL_TYPE;
	}
	
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
	
	cpBody* staticBody = space->staticBody;
	cpShape* shape;
	
	// Data structure for our ball emitter
	// We'll use two sensors for it, one to see if the emitter is blocked
	// a second to catch the balls and add them back to the emitter
	emitterInstance.queue = 5;
	emitterInstance.blocked = 0;
	emitterInstance.position = cpv ( 0, 150 );
	
	// Create our blocking sensor, so we know when the emitter is clear to emit another ball
	shape = cpSpaceAddShape ( space, cpCircleShapeNew ( staticBody, 15.0f, emitterInstance.position ) );
	shape->sensor = 1;
	shape->collision_type = BLOCKING_SENSOR_TYPE;
	shape->data = &emitterInstance;
	
	// Create our catch sensor to requeue the balls when they reach the bottom of the screen
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -2000, -200 ), cpv ( 2000, -200 ), 15.0f ) );
	shape->sensor = 1;
	shape->collision_type = CATCH_SENSOR_TYPE;
	shape->data = &emitterInstance;
	
	cpSpaceAddCollisionHandler ( space, BLOCKING_SENSOR_TYPE, BALL_TYPE, blockerBegin, KD_NULL, KD_NULL, blockerSeparate, KD_NULL );
	cpSpaceAddCollisionHandler ( space, CATCH_SENSOR_TYPE, BALL_TYPE, catcherBarBegin, KD_NULL, KD_NULL, KD_NULL, KD_NULL );
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Sensors =
{
	"Sensors",
	KD_NULL,
	init,
	update,
	destroy,
};
