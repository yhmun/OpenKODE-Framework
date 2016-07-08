/* --------------------------------------------------------------------------
 *
 *      File            DemoBounce.cpp
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

static KDvoid update ( KDint ticks )
{
	KDint steps = 3;
	cpFloat  dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
	}
}

static KDvoid add_box ( KDvoid )
{
	const cpFloat size = 10.0f;
	const cpFloat mass = 1.0f;
	
	cpVect verts[] = 
	{
		cpv ( -size,-size ),
		cpv ( -size, size ),
		cpv (  size, size ),
		cpv (  size,-size ),
	};
	
	cpFloat radius = cpvlength ( cpv ( size, size ) );

	cpBody* body = cpSpaceAddBody ( space, cpBodyNew ( mass, cpMomentForPoly ( mass, 4, verts, cpvzero ) ) );
	body->p = cpv ( frand ( ) * ( 640 - 2 * radius ) - ( 320 - radius ), frand ( ) * ( 480 - 2 * radius ) - ( 240 - radius ) );
	body->v = cpvmult ( cpv ( 2 * frand ( ) - 1, 2 * frand ( ) - 1 ), 200 );
	
	cpShape* shape = cpSpaceAddShape ( space, cpPolyShapeNew ( body, 4, verts, cpvzero ) );
	shape->e = 1.0f; shape->u = 0.0f;
}

static cpSpace* init ( KDvoid )
{	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->iterations = 10;
	
	cpBody *body, *staticBody = space->staticBody;
	cpShape *shape;
		
	// Create segments around the edge of the screen.
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, -240 ), cpv ( -320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( 320,-240 ), cpv ( 320,240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320,-240 ), cpv ( 320,-240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( -320, 240 ), cpv ( 320, 240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	
	for ( KDint i = 0; i < 10; i++ )
	{
		add_box ( );
	}
	
	body = cpSpaceAddBody ( space, cpBodyNew ( 100.0f, 10000.0f ) );

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( body, cpv ( -75, 0 ), cpv ( 75, 0 ), 5.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	
	cpSpaceAddConstraint ( space, cpPivotJointNew2 ( body, staticBody, cpvzero, cpvzero ) );
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Bounce =
{
	"Bounce",
	KD_NULL,
	init,
	update,
	destroy,
};
