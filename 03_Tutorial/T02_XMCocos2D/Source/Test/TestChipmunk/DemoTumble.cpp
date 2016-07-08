/* --------------------------------------------------------------------------
 *
 *      File            DemoTumble.cpp
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
static cpBody*  staticBody;

static KDvoid update ( KDint ticks )
{
	KDint steps = 3;
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
		
		// Manually update the position of the static shape so that
		// the box rotates.
		cpBodyUpdatePosition ( staticBody, dt );
		
		// Because the box was added as a static shape and we moved it
		// we need to manually rehash the static spatial hash.
		cpSpaceReindexStatic ( space );
	}
}

static cpSpace* init ( KDvoid )
{
	staticBody = cpBodyNew ( KD_INFINITY, KD_INFINITY );
	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->gravity = cpv ( 0, -600 );
	
	cpBody*  body;
	cpShape* shape;
	
	// Vertexes for the bricks
	KDint num = 4;
	cpVect verts[] = 
	{
		cpv ( -30, -15 ),
		cpv ( -30,  15 ),
		cpv (  30,  15 ),
		cpv (  30, -15 ),
	};
	
	// Set up the static box.
	cpVect a = cpv ( -200, -200 );
	cpVect b = cpv ( -200,  200 );
	cpVect c = cpv (  200,  200 );
	cpVect d = cpv (  200, -200 );
	
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, a, b, 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, b, c, 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, c, d, 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;

	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, d, a, 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	
	// Give the box a little spin.
	// Because staticBody is never added to the space, we will need to
	// update it ourselves. ( see above ).
	// NOTE: Normally you would want to add the segments as normal and not static shapes.
	// I'm just doing it to demonstrate the cpSpaceRehashStatic ( )function.
	staticBody->w = 0.4f;
	
	// Add the bricks.
	for ( KDint i = 0; i < 3; i++ )
	{
		for ( KDint j = 0; j < 7; j++ )
		{
			body = cpSpaceAddBody ( space, cpBodyNew ( 1.0f, cpMomentForPoly ( 1.0f, num, verts, cpvzero ) ) );
			body->p = cpv ( i * 60 - 150.0f, j * 30 - 150.0f );
			
			shape = cpSpaceAddShape ( space, cpPolyShapeNew ( body, num, verts, cpvzero ) );
			shape->e = 0.0f; shape->u = 0.7f;
		}
	}
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	cpBodyFree ( staticBody );
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Tumble = 
{
	"Tumble",
	KD_NULL,
	init,
	update,
	destroy,
};
