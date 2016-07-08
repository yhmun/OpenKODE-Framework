/* --------------------------------------------------------------------------
 *
 *      File            DemoPlayer.cpp
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

static cpSpace* space;

typedef struct PlayerStruct
{
	cpFloat   u;
	cpShape*  shape;
} PlayerStruct;

PlayerStruct playerInstance;

static KDvoid playerUpdateVelocity ( cpBody* body, cpVect gravity, cpFloat damping, cpFloat dt )
{
	cpBodyUpdateVelocity ( body, gravity, damping, dt );
	body->v.y = cpfmax ( body->v.y, -700 );
	body->v.x = cpfclamp ( body->v.x, -400, 400 );
}

static KDvoid SelectPlayerGroundNormal ( cpBody* body, cpArbiter* arb, cpVect* groundNormal )
{
	cpVect n = cpArbiterGetNormal ( arb, 0 );
	
	if ( n.y > groundNormal->y )
	{
		*groundNormal = n;
	}
}

static KDvoid update ( KDint ticks )
{
	static KDint lastJumpState = 0;
	KDint jumpState = ( arrowDirection.y > 0.0f );
	
	cpBody* body = playerInstance.shape->body;

	cpVect groundNormal = cpvzero;
	cpBodyEachArbiter ( body, (cpBodyArbiterIteratorFunc) SelectPlayerGroundNormal, &groundNormal );
	if ( groundNormal.y > 0.0f )
	{
		playerInstance.shape->surface_v = cpv ( 400.0f * arrowDirection.x, 0.0f ); // cpvmult ( cpvperp ( groundNormal ), 400.0f * arrowDirection.x );
		if ( arrowDirection.x ) 
		{
			cpBodyActivate ( body );
		}
	}
	else
	{
		playerInstance.shape->surface_v = cpvzero;
	}
		
	// apply jump
	if ( jumpState && !lastJumpState && cpvlengthsq ( groundNormal ) )
	{
//		body->v = cpvmult ( cpvslerp ( groundNormal, cpv ( 0.0f, 1.0f ), 0.5f ), 500.0f );
		body->v = cpvadd ( body->v, cpvmult ( cpvslerp ( groundNormal, cpv ( 0.0f, 1.0f ), 0.75f ), 500.0f ) );
	}
	
	if ( cpvlengthsq ( groundNormal ) )
	{
		cpFloat air_accel = body->v.x + arrowDirection.x * ( 2000.0f );
		body->f.x = body->m * air_accel;
//		body->v.x = cpflerpconst ( body->v.x, 400.0f * arrowDirection.x, 2000.0f / 60.0f );
	}
	
	KDint steps = 3;
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
	}
	
	lastJumpState = jumpState;
}

static cpSpace* init ( KDvoid )
{	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->iterations = 10;
	space->gravity = cpv ( 0, -1500 );
	space->sleepTimeThreshold = 999999;
	
	cpBody *body, *staticBody = space->staticBody;
	cpShape* shape;
	
	// Create segments around the edge of the screen.
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( -320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	shape->collision_type = 2;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( 320, -240 ), cpv ( 320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	shape->collision_type = 2;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( 320, -240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	shape->collision_type = 2;
	
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, 240 ), cpv ( 320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	shape->collision_type = 2;
	
	// add some other segments to play with
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -220,-200 ), cpv ( -220, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	shape->collision_type = 2;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( 0, -240 ), cpv ( 320,-200 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	shape->collision_type = 2;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( 200, -240 ), cpv ( 320,-100 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	shape->collision_type = 2;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -220,-80 ), cpv ( 200,-80 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	shape->collision_type = 2;
	
	// Set up the player
	cpFloat radius = 15.0f;
	body = cpSpaceAddBody ( space, cpBodyNew ( 10.0f, KD_INFINITY ) );
	body->p = cpv ( 0, -220 );
	body->velocity_func = playerUpdateVelocity;

	shape = cpSpaceAddShape ( space, cpCircleShapeNew ( body, radius, cpvzero ) );
	shape->e = 0.0f; shape->u = 2.0f;
	shape->collision_type = 1;
	
	playerInstance.u = shape->u;
	playerInstance.shape = shape;
	shape->data = &playerInstance;
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Player = 
{
	"Player",
	KD_NULL,
	init,
	update,
	destroy,
};
