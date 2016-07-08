/* --------------------------------------------------------------------------
 *
 *      File            DemoSimple.cpp
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

static cpSpace*  space;

// Init is called by the demo code to set up the demo.
static cpSpace* init ( KDvoid )
{	
	// Create a space, a space is a simulation world. It simulates the motions of rigid bodies,
	// handles collisions between them, and simulates the joints between them.
	space = cpSpaceNew ( );
	
	// Lets set some parameters of the space:
	// More iterations make the simulation more accurate but slower
	space->iterations = 1;

	// Give it some gravity
	space->gravity = cpv ( 0, -100 );
	
	// Create A ground segment along the bottom of the screen
	cpShape* ground = cpSegmentShapeNew ( space->staticBody, cpv ( -320, -240 ), cpv ( 320, -240 ), 0.0f );
	// Set some parameters of the shape.
	// For more info: http://code.google.com/p/chipmunk-physics/wiki/cpShape
	ground->e = 1.0f; ground->u = 1.0f;
	ground->layers = NOT_GRABABLE_MASK; // Used by the Demo mouse grabbing code
	// Add the shape to the space as a static shape
	// If a shape never changes position, add it as static so Chipmunk knows it only needs to
	// calculate collision information for it once when it is added.
	// Do not change the postion of a static shape after adding it.
	cpSpaceAddShape ( space, ground );
	
	for ( KDint i = -5; i <= 5; i++ )
	{
		cpFloat radius = 25.0f;
		cpFloat mass = 1.0f;
//		cpBody* ballBody = cpBodyNew ( KD_INFINITY, cpMomentForCircle ( mass, 0.0f, radius, cpvzero ) );
		cpBody* ballBody = cpBodyNew ( mass, KD_INFINITY );
		ballBody->p = cpv ( ( 5 + 2 * radius ) * i, 200 );
		cpSpaceAddBody ( space, ballBody );
		
		cpShape* ballShape = cpSpaceAddShape ( space, cpCircleShapeNew ( ballBody, radius, cpvzero ) );
		ballShape->e = 0.0f; ballShape->u = 0.9f;
		
		cpFloat stiffness = 30;
		cpFloat damping   = 2.0f * cpfsqrt ( stiffness * ballBody->m ) * cpfpow ( 1.1f, (cpFloat) i );
//		cpFloat stiffness = 30;
//		cpFloat damping   = 0.0;
		cpSpaceAddConstraint ( space, cpDampedSpringNew ( space->staticBody, ballBody, cpv ( ( 5 + 2 * radius ) * i, 0 ), cpvzero, 0, stiffness, damping ) );
	}
	
	return space;
}

// Update is called by the demo code each frame.
static KDvoid update ( KDint ticks )
{
	// Chipmunk allows you to use a different timestep each frame, but it works much better when you use a fixed timestep.
	// An excellent article on why fixed timesteps for game logic can be found here: http://gafferongames.com/game-physics/fix-your-timestep/
	cpSpaceStep ( space, 1.0f / 60.0f );
}

// destroy is called by the demo code to free all the memory we've allocated
static KDvoid destroy ( KDvoid )
{
	ChipmunkDemoFreeSpaceChildren ( space );
	cpSpaceFree ( space );
}

chipmunkDemo Simple =
{
	"Simple",
	KD_NULL,
	init,
	update,
	destroy,
};
