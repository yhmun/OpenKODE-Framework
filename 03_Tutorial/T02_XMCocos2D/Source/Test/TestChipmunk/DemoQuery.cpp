/* --------------------------------------------------------------------------
 *
 *      File            DemoQuery.cpp
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
static cpVect   mousePoint;
       cpShape* querySeg = KD_NULL;

static KDvoid update ( KDint ticks )
{
	messageString[0] = '\0';
	
	cpVect start = cpvzero;
	cpVect end = /*cpv ( 0, 85 );//*/mousePoint;
	cpVect lineEnd = end;
	
	{
		KDchar infoString[1024];
		kdSprintfKHR ( infoString, "Query: Dist ( %f ) Point%s, ", cpvdist ( start, end ), cpvstr ( end ) );
		kdStrcat ( messageString, infoString );
	}
	
	cpSegmentQueryInfo info = { };
	if ( cpSpaceSegmentQueryFirst ( space, start, end, CP_ALL_LAYERS, CP_NO_GROUP, &info ) )
	{
		cpVect point = cpSegmentQueryHitPoint ( start, end, info );
		lineEnd = cpvadd ( point, cpvzero );//cpvmult ( info.n, 4.0f ) );
		
		KDchar infoString[1024];
		kdSprintfKHR ( infoString, "Segment Query: Dist ( %f ) Normal%s", cpSegmentQueryHitDist ( start, end, info ), cpvstr ( info.n ) );
		kdStrcat ( messageString, infoString );
	} else 
	{
		kdStrcat ( messageString, "Segment Query ( None )" );
	}
	
	cpSegmentShapeSetEndpoints ( querySeg, cpvzero, lineEnd );
	// force it to update it's collision detection data so it will draw
	cpShapeUpdate ( querySeg, cpvzero, cpv ( 1.0f, 0.0f ) );
	
	// normal other stuff.
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
	
	cpBody* staticBody = space->staticBody;	
	cpShape* shape;
	
	// add a non-collidable segment as a quick and dirty way to draw the query line
	shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpvzero, cpv ( 100.0f, 0.0f ), 4.0f ) );
	shape->layers = 0;
	querySeg = shape;
	
	{ // add a fat segment
		cpFloat mass = 1.0f;
		cpFloat length = 100.0f;
		cpVect a = cpv ( -length / 2.0f, 0.0f ), b = cpv ( length / 2.0f, 0.0f );
		
		cpBody *body = cpSpaceAddBody ( space, cpBodyNew ( mass, cpMomentForSegment ( mass, a, b ) ) );
		body->p = cpv ( 0.0f, 100.0f );
		
		cpSpaceAddShape ( space, cpSegmentShapeNew ( body, a, b, 20.0f ) );
	}
	
	{ // add a static segment
		cpSpaceAddShape ( space, cpSegmentShapeNew ( staticBody, cpv ( 0, 300 ), cpv ( 300, 0 ), 0.0f ) );
	}
	
	{ // add a pentagon
		cpFloat mass = 1.0f;
		const KDint NUM_VERTS = 5;
		
		cpVect verts[NUM_VERTS];
		for ( KDint i=0; i<NUM_VERTS; i++ )
		{
			cpFloat angle = -2 * KD_PI_F * i / (  NUM_VERTS );
			verts[i] = cpv ( 30.f * cpfcos ( angle ), 30.f * cpfsin ( angle ) );
		}
		
		cpBody* body = cpSpaceAddBody ( space, cpBodyNew ( mass, cpMomentForPoly ( mass, NUM_VERTS, verts, cpvzero ) ) );
		body->p = cpv ( 50.0f, 50.0f );
		
		cpSpaceAddShape ( space, cpPolyShapeNew ( body, NUM_VERTS, verts, cpvzero ) );
	}
	
	{ // add a circle
		cpFloat mass = 1.0f;
		cpFloat r = 20.0f;
		
		cpBody *body = cpSpaceAddBody ( space, cpBodyNew ( mass, cpMomentForCircle ( mass, 0.0f, r, cpvzero ) ) );
		body->p = cpv ( 100.0f, 100.0f );
		
		cpSpaceAddShape ( space, cpCircleShapeNew ( body, r, cpvzero ) );
	}
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Query =
{
	"Segment Query",
	KD_NULL,
	init,
	update,
	destroy,
};
