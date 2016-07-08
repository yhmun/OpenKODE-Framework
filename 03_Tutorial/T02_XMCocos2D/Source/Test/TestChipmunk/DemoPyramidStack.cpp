/* --------------------------------------------------------------------------
 *
 *      File            DemoPyramidStack.cpp
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
	KDint steps = 1;
	cpFloat dt = 3.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
	}
}

static cpSpace* init ( KDvoid )
{	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->iterations = 30;
	space->gravity = cpv ( 0, -100 );
	space->sleepTimeThreshold = 0.5f;
	space->collisionSlop = 0.5f;

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
	
	// Add lots of boxes.
	for ( KDint i = 0; i < 14; i++ )
	{
		for ( KDint j = 0; j <= i; j++ )
		{
			body = cpSpaceAddBody ( space, cpBodyNew ( 1.0f, cpMomentForBox ( 1.0f, 30.0f, 30.0f ) ) );
			body->p = cpv ( j * 32.f - i * 16.f, 300.f - i * 32.f );
			
			shape = cpSpaceAddShape ( space, cpBoxShapeNew ( body, 30.0f, 30.0f ) );
			shape->e = 0.0f; shape->u = 0.8f;
		}
	}
	
	// Add a ball to make things more interesting
	cpFloat radius = 15.0f;
	body = cpSpaceAddBody ( space, cpBodyNew ( 10.0f, cpMomentForCircle ( 10.0f, 0.0f, radius, cpvzero ) ) );
	body->p = cpv ( 0, -240 + radius + 5 );

	shape = cpSpaceAddShape ( space, cpCircleShapeNew ( body, radius, cpvzero ) );
	shape->e = 0.0f; shape->u = 0.9f;
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo PyramidStack = 
{
	"Pyramid Stack",
	KD_NULL,
	init,
	update,
	destroy,
};
