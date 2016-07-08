/* --------------------------------------------------------------------------
 *
 *      File            DemoPlink.cpp
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

// Iterate over all of the bodies and reset the ones that have fallen offscreen.
static KDvoid eachBody ( cpBody* body, KDvoid* unused )
{
	if ( body->p.y < -260 || cpfabs ( body->p.x ) > 340 )
	{
		cpFloat x = frand ( ) / (cpFloat) KD_RAND_MAX * 640 - 320;
		body->p = cpv ( x, 260 );
	}
}

static KDvoid update ( KDint ticks )
{
	KDint steps = 1;
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
		cpSpaceEachBody ( space, &eachBody, KD_NULL );
	}
}

#define NUM_VERTS 5

static cpSpace* init ( KDvoid )
{	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->iterations = 5;
	space->gravity = cpv ( 0, -100 );
	
	cpBody *body, *staticBody = space->staticBody;
	cpShape* shape;
	
	// Create vertexes for a pentagon shape.
	cpVect verts[NUM_VERTS];
	for ( KDint i = 0; i < NUM_VERTS; i++ )
	{
		cpFloat angle = -2 * KD_PI_F * i / ( (cpFloat) NUM_VERTS );
		verts[i] = cpv ( 10 * cpfcos ( angle ), 10 * cpfsin ( angle ) );
	}
	
	// Vertexes for a triangle shape.
	cpVect tris[] =
	{
		cpv ( -15, -15 ),
		cpv (   0,  10 ),
		cpv (  15, -15 ),
	};

	// Create the static triangles.
	for ( KDint i = 0; i < 9; i++ )
	{
		for ( KDint j = 0; j < 6; j++ )
		{
			cpFloat stagger = ( j % 2 ) * 40.0f;
			cpVect offset = cpv ( i * 80 - 320.0f + stagger, j * 70 - 240.0f );
			shape = cpSpaceAddShape ( space, cpPolyShapeNew ( staticBody, 3, tris, offset ) );
			shape->e = 1.0f; shape->u = 1.0f;
			shape->layers = NOT_GRABABLE_MASK;
		}
	}
	
	// Add lots of pentagons.
	for ( KDint i = 0; i < 300; i++ )
	{
		body = cpSpaceAddBody ( space, cpBodyNew ( 1.0f, cpMomentForPoly ( 1.0f, NUM_VERTS, verts, cpvzero ) ) );
		cpFloat x = frand ( ) / (cpFloat) KD_RAND_MAX * 640 - 320;
		body->p = cpv ( x, 350 );
		
		shape = cpSpaceAddShape ( space, cpPolyShapeNew ( body, NUM_VERTS, verts, cpvzero ) );
		shape->e = 0.0f; shape->u = 0.4f;
	}
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Plink = 
{
	"Plink",
	KD_NULL,
	init,
	update,
	destroy,
};
