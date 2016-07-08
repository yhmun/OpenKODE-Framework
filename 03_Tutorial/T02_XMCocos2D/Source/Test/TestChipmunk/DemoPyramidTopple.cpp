/* --------------------------------------------------------------------------
 *
 *      File            DemoPyramidTopple.cpp
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
	cpFloat dt = 1.0f / 60.0f / (cpFloat) steps;
	
	for ( KDint i = 0; i < steps; i++ )
	{
		cpSpaceStep ( space, dt );
	}
}

#define WIDTH    4.0f
#define HEIGHT  30.0f

static KDvoid add_domino ( cpSpace* space, cpVect pos, cpBool flipped )
{
	cpFloat mass = 1.0f;
	cpFloat moment = cpMomentForBox ( mass, WIDTH, HEIGHT );
	
	cpBody* body = cpSpaceAddBody ( space, cpBodyNew ( mass, moment ) );
	body->p = pos;

	cpShape* shape = ( flipped ? cpBoxShapeNew ( body, HEIGHT, WIDTH ) : cpBoxShapeNew ( body, WIDTH, HEIGHT ) );
	cpSpaceAddShape ( space, shape );
	shape->e = 0.0f; shape->u = 0.6f;
}

static cpSpace* init ( KDvoid )
{	
	cpResetShapeIdCounter ( );
	
	space = cpSpaceNew ( );
	space->iterations = 30;
	space->gravity = cpv ( 0, -300 );
	space->sleepTimeThreshold = 0.5f;
	space->collisionSlop = 0.5f;
	
	cpShape* shape = cpSpaceAddShape ( space, cpSegmentShapeNew ( space->staticBody, cpv ( -600, -240 ), cpv ( 600, -240 ), 0.0f ) );
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
	
	// Add the dominoes.
	KDint n = 12;
	for ( KDint i = 0; i < n; i++ )
	{
		for ( KDint j = 0; j < ( n - i ); j++ )
		{
			cpVect offset = cpv ( ( j - ( n - 1 - i ) * 0.5f ) * 1.5f * HEIGHT, ( i + 0.5f ) * ( HEIGHT + 2 * WIDTH ) - WIDTH - 240 );
			add_domino ( space, offset, cpFalse );
			add_domino ( space, cpvadd ( offset, cpv ( 0, ( HEIGHT + WIDTH ) / 2.0f ) ), cpTrue );
			
			if ( j == 0 )
			{
				add_domino ( space, cpvadd ( offset, cpv ( 0.5f * ( WIDTH - HEIGHT ), HEIGHT + WIDTH ) ), cpFalse );
			}
			
			if ( j != n - i - 1 )
			{
				add_domino ( space, cpvadd ( offset, cpv ( HEIGHT * 0.75f, ( HEIGHT + 3 * WIDTH ) / 2.0f ) ), cpTrue );
			}
			else 
			{
				add_domino ( space, cpvadd ( offset, cpv ( 0.5f * ( HEIGHT - WIDTH ), HEIGHT + WIDTH ) ), cpFalse );
			}
		}
	}
	
	return space;
}

static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo PyramidTopple = 
{
	"Pyramid Topple",
	KD_NULL,
	init,
	update,
	destroy,
};
