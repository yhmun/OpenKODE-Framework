/* --------------------------------------------------------------------------
 *
 *      File            DemoPump.cpp
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

static cpSpace*        space;
static cpConstraint*   motor;

#define numBalls       5
static cpBody* balls[numBalls];

static KDvoid update ( KDint ticks )
{
	cpFloat coef = ( 2.0f + arrowDirection.y ) / 3.0f;
	cpFloat rate = arrowDirection.x * 30.0f * coef;

	cpSimpleMotorSetRate ( motor, rate );
	motor->maxForce = ( rate ) ? 1000000.0f : 0.0f;

	KDint steps = 2;
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
		
		for ( KDint i = 0; i < numBalls; i++ )
		{
			cpBody* ball = balls[i];
			if ( ball->p.x > 320.0f )
			{
				ball->v = cpvzero;
				ball->p = cpv ( -224.0f, 200.0f );
			}
		}
	}
}

static cpBody* add_ball ( cpVect pos )
{
	cpBody* body = cpSpaceAddBody ( space, cpBodyNew ( 1.0f, cpMomentForCircle ( 1.0f, 30, 0, cpvzero ) ) );
	body->p = pos;
	
	cpShape* shape = cpSpaceAddShape ( space, cpCircleShapeNew ( body, 30, cpvzero ) );
	shape->e = 0.0f; shape->u = 0.5f;
	
	return body;
}

static cpSpace* init ( KDvoid )
{	
	space = cpSpaceNew ( );
	space->gravity = cpv ( 0, -600 );
	
	cpBody *staticBody = space->staticBody;
	cpShape* shape;
	
	// beveling all of the line segments helps prevent things from getting stuck on cracks
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -256, 16 ), cpv ( -256, 240 ), 2.0f ) );
	shape->e = 0.0f; shape->u = 0.5f; shape->layers = 1;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -256, 16 ), cpv ( -192, 0 ), 2.0f ) );
	shape->e = 0.0f; shape->u = 0.5f; shape->layers = 1;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -192, 0 ), cpv ( -192, -64 ), 2.0f ) );
	shape->e = 0.0f; shape->u = 0.5f; shape->layers = 1;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -128, -64 ), cpv ( -128, 144 ), 2.0f ) );
	shape->e = 0.0f; shape->u = 0.5f; shape->layers = 1;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -192, 80 ), cpv ( -192, 176 ), 2.0f ) );
	shape->e = 0.0f; shape->u = 0.5f; shape->layers = 1;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -192, 176 ), cpv ( -128, 240 ), 2.0f ) );
	shape->e = 0.0f; shape->u = 0.5f; shape->layers = 1;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -128, 144 ), cpv ( 192, 64 ), 2.0f ) );
	shape->e = 0.0f; shape->u = 0.5f; shape->layers = 1;
	shape->layers = NOT_GRABABLE_MASK;

	cpVect verts[] = 
	{
		cpv ( -30, -80 ),
		cpv ( -30,  80 ),
		cpv (  30,  64 ),
		cpv (  30, -80 ),
	};

	cpBody* plunger = cpSpaceAddBody ( space, cpBodyNew ( 1.0f, KD_INFINITY ) );
	plunger->p = cpv ( -160,-80 );
	
	shape = cpSpaceAddShape ( space, cpPolyShapeNew ( plunger, 4, verts, cpvzero ) );
	shape->e = 1.0f; shape->u = 0.5f; shape->layers = 1;
	
	// add balls to hopper
	for ( KDint i = 0; i < numBalls; i++ )
	{
		balls[i] = add_ball ( cpv ( -224, 80.f + 64 * i ) );
	}
	
	// add small gear
	cpBody* smallGear = cpSpaceAddBody ( space, cpBodyNew ( 10.0f, cpMomentForCircle ( 10.0f, 80, 0, cpvzero ) ) );
	smallGear->p = cpv ( -160, -160 );
	cpBodySetAngle ( smallGear, -KD_PI_2_F );

	shape = cpSpaceAddShape ( space, cpCircleShapeNew ( smallGear, 80.0f, cpvzero ) );
	shape->layers = 0;
	
	cpSpaceAddConstraint ( space, cpPivotJointNew2 ( staticBody, smallGear, cpv ( -160,-160 ), cpvzero ) );

	// add big gear
	cpBody* bigGear = cpSpaceAddBody ( space, cpBodyNew ( 40.0f, cpMomentForCircle ( 40.0f, 160, 0, cpvzero ) ) );
	bigGear->p = cpv ( 80,-160 );
	cpBodySetAngle ( bigGear, KD_PI_2_F );
	
	shape = cpSpaceAddShape ( space, cpCircleShapeNew ( bigGear, 160.0, cpvzero ) );
	shape->layers = 0;
	
	cpSpaceAddConstraint ( space, cpPivotJointNew2 ( staticBody, bigGear, cpv ( 80, -160 ), cpvzero ) );

	// connect the plunger to the small gear.
	cpSpaceAddConstraint ( space, cpPinJointNew ( smallGear, plunger, cpv ( 80,0 ), cpv ( 0, 0 ) ) );
	// connect the gears.
	cpSpaceAddConstraint ( space, cpGearJointNew ( smallGear, bigGear, -KD_PI_2_F, -2.0f ) );
	
	// feeder mechanism
	cpFloat bottom = -300.0f;
	cpFloat top = 32.0f;
	cpBody* feeder = cpSpaceAddBody ( space, cpBodyNew ( 1.0f, cpMomentForSegment ( 1.0f, cpv ( -224.0f, bottom ), cpv ( -224.0f, top ) ) ) );
	feeder->p = cpv ( -224, ( bottom + top ) / 2.0f );
	
	cpFloat len = top - bottom;
	cpSpaceAddShape ( space, cpSegmentShapeNew ( feeder, cpv ( 0.0f, len / 2.0f ), cpv ( 0.0f, -len / 2.0f ), 20.0f ) );
	
	cpSpaceAddConstraint ( space, cpPivotJointNew2 ( staticBody, feeder, cpv ( -224.0, bottom ), cpv ( 0.0f, -len / 2.0f ) ) );
	cpVect anchr = cpBodyWorld2Local ( feeder, cpv ( -224.0f, -160.0f ) );
	cpSpaceAddConstraint ( space, cpPinJointNew ( feeder, smallGear, anchr, cpv ( 0.0f, 80.0f ) ) );

	// motorize the second gear
	motor = cpSpaceAddConstraint ( space, cpSimpleMotorNew ( staticBody, bigGear, 3.0f ) );

	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Pump =
{
	"Pump",
	KD_NULL,
	init,
	update,
	destroy,
};
