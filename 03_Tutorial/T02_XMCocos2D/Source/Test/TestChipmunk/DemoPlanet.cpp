/* --------------------------------------------------------------------------
 *
 *      File            DemoPlanet.cpp
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
static cpBody*  planetBody;
static cpFloat  gravityStrength = 5.0e6f;

static KDvoid update ( KDint ticks )
{
	KDint steps = 1;
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
		
		// Update the static body spin so that it looks like it's rotating.
		cpBodyUpdatePosition ( planetBody, dt );
	}
}

static KDvoid planetGravityVelocityFunc ( cpBody* body, cpVect gravity, cpFloat damping, cpFloat dt )
{
	// Gravitational acceleration is proportional to the inverse square of
	// distance, and directed toward the origin. The central planet is assumed
	// to be massive enough that it affects the satellites but not vice versa.
	cpVect  p = body->p;
	cpFloat sqdist = cpvlengthsq ( p );
	cpVect  g = cpvmult ( p, -gravityStrength / ( sqdist * cpfsqrt ( sqdist ) ) );
	
	cpBodyUpdateVelocity ( body, g, damping, dt );
}

static cpVect rand_pos ( cpFloat radius )
{
	cpVect v;
	do 
	{
		v = cpv ( frand ( ) * ( 640 - 2 * radius ) - ( 320 - radius ), frand ( ) * ( 480 - 2 * radius ) - ( 240 - radius ) );
	} while ( cpvlength ( v ) < 85.0f );
	
	return v;
}

static KDvoid add_box ( KDvoid )
{
	const cpFloat size = 10.0f;
	const cpFloat mass = 1.0f;
	
	cpVect verts[] = 
	{
		cpv ( -size, -size ),
		cpv ( -size,  size ),
		cpv (  size,  size ),
		cpv (  size, -size ),
	};
	
	cpFloat radius = cpvlength ( cpv ( size, size ) );

	cpBody* body = cpSpaceAddBody ( space, cpBodyNew ( mass, cpMomentForPoly ( mass, 4, verts, cpvzero ) ) );
	body->velocity_func = planetGravityVelocityFunc;
	body->p = rand_pos ( radius );

	// Set the box's velocity to put it into a circular orbit from its
	// starting position.
	cpFloat r = cpvlength ( body->p );
	cpFloat v = cpfsqrt ( gravityStrength / r ) / r;
	body->v = cpvmult ( cpvperp ( body->p ), v );

	// Set the box's angular velocity to match its orbital period and
	// align its initial angle with its position.
	body->w = v;
	cpBodySetAngle ( body, cpfatan2 ( body->p.y, body->p.x ) );

	cpShape* shape = cpSpaceAddShape ( space, cpPolyShapeNew ( body, 4, verts, cpvzero ) );
	shape->e = 0.0f; shape->u = 0.7f;
}

static cpSpace* init ( KDvoid )
{
	planetBody = cpBodyNew ( KD_INFINITY, KD_INFINITY );
	planetBody->w = 0.2f;
	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->iterations = 20;
	
	for ( KDint i = 0; i < 30; i++ )
	{
		add_box ( );
	}
	
	cpShape* shape = cpSpaceAddShape ( space, cpCircleShapeNew ( planetBody, 70.0f, cpvzero ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	cpBodyFree ( planetBody );
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Planet =
{
	"Planet",
	KD_NULL,
	init,
	update,
	destroy,
};
