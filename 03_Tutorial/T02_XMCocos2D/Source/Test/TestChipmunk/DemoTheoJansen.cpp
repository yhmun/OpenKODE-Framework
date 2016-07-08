/* --------------------------------------------------------------------------
 *
 *      File            DemoTheoJansen.cpp
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

static cpSpace*       space;
static cpConstraint*  motor;

static KDvoid update ( KDint ticks )
{
	cpFloat coef = ( 2.0f + arrowDirection.y ) / 3.0f;
	cpFloat rate = arrowDirection.x * 10.0f * coef;
	cpSimpleMotorSetRate ( motor, rate );
	motor->maxForce = ( rate ) ? 100000.0f : 0.0f;
	
	KDint steps = 3;
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
	}
}

static cpFloat seg_radius = 3.0f;

static KDvoid make_leg ( cpFloat side, cpFloat offset, cpBody* chassis, cpBody* crank, cpVect anchor )
{
	cpVect a, b;
	cpShape* shape;
	
	cpFloat leg_mass = 1.0f;

	// make leg
	a = cpvzero, b = cpv ( 0.0f, side );
	cpBody* upper_leg = cpBodyNew ( leg_mass, cpMomentForSegment ( leg_mass, a, b ) );
	upper_leg->p = cpv ( offset, 0.0f );
	cpSpaceAddBody ( space, upper_leg );
	cpSpaceAddShape ( space, cpSegmentShapeNew ( upper_leg, a, b, seg_radius ) );
	cpSpaceAddConstraint ( space, cpPivotJointNew2 ( chassis, upper_leg, cpv ( offset, 0.0f ), cpvzero ) );
	
	// lower leg
	a = cpvzero, b = cpv ( 0.0f, -1.0f * side );
	cpBody* lower_leg = cpBodyNew ( leg_mass, cpMomentForSegment ( leg_mass, a, b ) );
	lower_leg->p = cpv ( offset, -side );
	cpSpaceAddBody ( space, lower_leg );
	shape = cpSegmentShapeNew ( lower_leg, a, b, seg_radius );
	shape->group = 1;
	cpSpaceAddShape ( space, shape );
	shape = cpCircleShapeNew ( lower_leg, seg_radius * 2.0f, b );
	shape->group = 1;
	shape->e = 0.0f; shape->u = 1.0f;
	cpSpaceAddShape ( space, shape );
	cpSpaceAddConstraint ( space, cpPinJointNew ( chassis, lower_leg, cpv ( offset, 0.0f ), cpvzero ) );
	
	cpSpaceAddConstraint ( space, cpGearJointNew ( upper_leg, lower_leg, 0.0f, 1.0f ) );
	
	cpConstraint* constraint;
	cpFloat diag = cpfsqrt ( side * side + offset * offset );
	
	constraint = cpPinJointNew ( crank, upper_leg, anchor, cpv ( 0.0f, side ) );
	cpPinJointSetDist ( constraint, diag );
	cpSpaceAddConstraint ( space, constraint );
	constraint = cpPinJointNew ( crank, lower_leg, anchor, cpvzero );
	cpPinJointSetDist ( constraint, diag );
	cpSpaceAddConstraint ( space, constraint );
}

static cpSpace* init ( KDvoid )
{
	space = cpSpaceNew ( );	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->iterations = 20;
	space->gravity = cpv ( 0,-500 );
	
	cpBody*  staticBody = space->staticBody;
	cpShape* shape;
	cpVect a, b;
	
	// Create segments around the edge of the screen.
	shape = cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( -320, 240 ), 0.0f );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	cpSpaceAddShape ( space, shape );

	shape = cpSegmentShapeNew ( staticBody, cpv ( 320, -240 ), cpv ( 320, 240 ), 0.0f );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	cpSpaceAddShape ( space, shape );

	shape = cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( 320, -240 ), 0.0f );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	cpSpaceAddShape ( space, shape );
	
	cpFloat offset = 30.0f;

	// make chassis
	cpFloat chassis_mass = 2.0f;
	a = cpv ( -offset, 0.0f ), b = cpv ( offset, 0.0f );
	cpBody* chassis = cpBodyNew ( chassis_mass, cpMomentForSegment ( chassis_mass, a, b ) );
	cpSpaceAddBody ( space, chassis );
	shape = cpSegmentShapeNew ( chassis, a, b, seg_radius );
	shape->group = 1;
	cpSpaceAddShape ( space, shape );
	
	// make crank
	cpFloat crank_mass = 1.0f;
	cpFloat crank_radius = 13.0f;
	cpBody* crank = cpBodyNew ( crank_mass, cpMomentForCircle ( crank_mass, crank_radius, 0.0f, cpvzero ) );
	cpSpaceAddBody ( space, crank );
	shape = cpCircleShapeNew ( crank, crank_radius, cpvzero );
	shape->group = 1;
	cpSpaceAddShape ( space, shape );
	cpSpaceAddConstraint ( space, cpPivotJointNew2 ( chassis, crank, cpvzero, cpvzero ) );
	
	cpFloat side = 30.0f;
	
	KDint num_legs = 2;
	for ( KDint i = 0; i < num_legs; i++ )
	{
		make_leg ( side,  offset, chassis, crank, cpvmult ( cpvforangle (  ( 2 * i + 0 ) / num_legs * KD_PI_F ), crank_radius ) );
		make_leg ( side, -offset, chassis, crank, cpvmult ( cpvforangle (  ( 2 * i + 1 ) / num_legs * KD_PI_F ), crank_radius ) );
	}
	
	motor = cpSimpleMotorNew ( chassis, crank, 6.0f );
	cpSpaceAddConstraint ( space, motor );

	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo TheoJansen = 
{
	"Theo Jansen Machine",
	KD_NULL,
	init,
	update,
	destroy,
};
