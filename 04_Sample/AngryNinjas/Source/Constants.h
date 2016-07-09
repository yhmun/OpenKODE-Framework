/* --------------------------------------------------------------------------
 *
 *      File            Constants.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
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

#ifndef __Constants_h__
#define __Constants_h__

#define PTM_RATIO							32

#define MAX_LEVEL							30

// depth levels		
#define DepthClouds							-300
#define DepthParticles						-299
#define DepthHills							-298
#define DepthFloor							-290
#define DepthScore							-285
		
#define DepthWhiteDots						-205

#define DepthStrapBack						-202
#define DepthPlatform						-201
#define DepthNinjas							-200
#define DepthStack							-199
				
#define DepthSlingShotFront					-179
#define DepthStrapFront						-178
		
#define DepthVisualFx						50
#define DepthPointScore						100

// tags	
#define TagForWhiteDotsEvenNumberedTurn		1000
#define TagForWhiteDotsOddNumberedTurn		2000

// creation methods
enum CreationMethod 
{
	ShapeOfSourceImage						= 0, 		// use for illustrations that are squares/ and rectangles and cropped right up to the edge. Will work with any size, vector points are in all four corners
	
	Triangle								= 1, 		// vector points are in the bottom left, bottom right, and top center of the image, (for example image triangleTall.png, triangleLarge.png, triangleMedium.png, triangleSmall.png )
	TriangleRightAngle						= 2, 		// vector points are in the top right corner, top left corner, and bottom left corner of the source image, (for example triangleRightAngle.png ) 
	
	Trapezoid								= 3, 		// vector points are in the bottom left corner, bottom right corner, and 1/4 and 3/4's across the top of the image, (for example trapezoid.png ) 
	
	DiameterOfImageForCircle				= 4,  		// use for illustrations that are perfect circles and cropped right up to the edge of the circle on all sides. Will work with any size
	
	Hexagon									= 5, 		// any size image with a hexagon can work, scale up or down the hexagonShape.png as a template to get the location of each edge.
	Octagon									= 6, 		// any size image with a octagon can work, scale up or down the octagonShape.png as a template to get the location of each edge.
	Pentagon								= 7,		// any size image with a pentagon can work, scale up or down the pentagon.png as a template to get the location of each edge.
		
	Parallelogram							= 8,		// vector points are in the bottom left corner, and 3/4's across the bottom of the image, top right corner, and 1/4 across the top (for example parallelogram.png ) 
	
	ShapeOfSourceImageButSlightlySmaller	= 9,		// defines vector shapes about 80% the size of the source (still a square or rectangle just slightly smaller) . Good for Enemies. 

	CustomCoordinates1 						= 101,		// use your own custom coordinates from a program like Vertex Helper Pro 
	CustomCoordinates2 						= 102,		// use your own custom coordinates from a program like Vertex Helper Pro
	CustomCoordinates3 						= 103,		// use your own custom coordinates from a program like Vertex Helper Pro

	// remember when defining custom coordinates, the vec points much be defined in counter clockwise order, 8 points at most, and make a convex shape (i.e. every point must be able to touch every other point)
};

// Break Effects
enum BreakEffect
{
	None									= 0,
	SmokePuffs								= 1,
	Explosion								= 2,
};

// Ambient Sounds
enum AmbientFXSounds
{
	Frogs									= 1,
	Insects									= 2,
};

#endif	// __Constants_h__