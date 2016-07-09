/* --------------------------------------------------------------------------
 *
 *      File            BasicDrawing2.cpp
 *      Description     Simple sample code
 *      Author          OpenCV team, Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#include "Precompiled.h"
#include "../Controller.h"

/// Global Variables
const int  NUMBER = 10;
const int  IMG_CX = 400;
const int  IMG_CY = 400;
      int      X1 = -IMG_CX     / 2;
      int      X2 =  IMG_CX * 3 / 2;
      int      Y1 = -IMG_CX     / 2;
      int      Y2 =  IMG_CX * 3 / 2;

// Produces a random color given a random object
static Scalar randomColor ( RNG& tRng )
{
	int	ncolor = (int) tRng;
	return Scalar ( ncolor & 255, ( ncolor >> 8 ) & 255, ( ncolor >> 16 ) & 255 );
}

KDvoid Drawing_Random_Lines ( Mat tImg, RNG tRng )
{
	Point	tP1, tP2;

	for ( KDint i = 0; i < NUMBER; i++ )
	{
		tP1.x = tRng.uniform ( X1, X2 );
		tP1.y = tRng.uniform ( Y1, Y2 );
		tP2.x = tRng.uniform ( X1, X2 );
		tP2.y = tRng.uniform ( Y1, Y2 );

		line ( tImg, tP1, tP2, randomColor ( tRng ), tRng.uniform ( 1, 10 ), 8 );
	}
}

KDvoid Drawing_Random_Rectangles ( Mat tImg, RNG tRng )
{
	KDint	nLineType = 8;
	KDint   nThickness = tRng.uniform ( -3, 10 );
	Point	tP1, tP2;

	for ( KDint i = 0; i < NUMBER; i++ )
	{
		tP1.x = tRng.uniform ( X1, X2 );
		tP1.y = tRng.uniform ( Y1, Y2 );
		tP2.x = tRng.uniform ( X1, X2 );
		tP2.y = tRng.uniform ( Y1, Y2 );

		rectangle ( tImg, tP1, tP2, randomColor ( tRng ), KD_MAX ( nThickness, -1 ), nLineType );
	}
}

KDvoid Drawing_Random_Ellipses ( Mat tImg, RNG tRng )
{
	KDint	nLineType = 8;

	for ( KDint i = 0; i < NUMBER; i++ )
	{
		Point  tCenter;
		tCenter.x = tRng.uniform ( X1, X2 );
		tCenter.y = tRng.uniform ( Y1, Y2 );

		Size   tAxes;
		tAxes.width  = tRng.uniform ( 0, 200 );
		tAxes.height = tRng.uniform ( 0, 200 );

		KDdouble  dAngle = tRng.uniform ( 0, 180 );

		ellipse ( tImg, tCenter, tAxes, dAngle, dAngle - 100, dAngle + 200,
				  randomColor ( tRng ), tRng.uniform ( -1, 9 ), nLineType );
	  
	}
}

KDvoid Drawing_Random_Polylines ( Mat tImg, RNG tRng )
{
	KDint	nLineType = 8;

	for ( KDint i = 0; i< NUMBER; i++ )
	{
		Point   aP [2][3];

		aP [0][0].x = tRng.uniform ( X1, X2 );
		aP [0][0].y = tRng.uniform ( Y1, Y2 );
		aP [0][1].x = tRng.uniform ( X1, X2 ); 
		aP [0][1].y = tRng.uniform ( Y1, Y2 ); 
		aP [0][2].x = tRng.uniform ( X1, X2 );
		aP [0][2].y = tRng.uniform ( Y1, Y2 );
		aP [1][0].x = tRng.uniform ( X1, X2 ); 
		aP [1][0].y = tRng.uniform ( Y1, Y2 );
		aP [1][1].x = tRng.uniform ( X1, X2 ); 
		aP [1][1].y = tRng.uniform ( Y1, Y2 );
		aP [1][2].x = tRng.uniform ( X1, X2 ); 
		aP [1][2].y = tRng.uniform ( Y1, Y2 );

		const Point* pP [2] = { aP [0], aP [1] };
		int          nP [ ] = { 3, 3 };
		    
		polylines ( tImg, pP, nP, 2, true, randomColor ( tRng ), tRng.uniform ( 1, 10 ), nLineType );
	}
}

KDvoid Drawing_Random_Filled_Polygons ( Mat tImg, RNG tRng )
{
	KDint	nLineType = 8;

	for ( KDint i = 0; i< NUMBER; i++ )
	{
		Point   aP [2][3];

		aP [0][0].x = tRng.uniform ( X1, X2 );
		aP [0][0].y = tRng.uniform ( Y1, Y2 );
		aP [0][1].x = tRng.uniform ( X1, X2 ); 
		aP [0][1].y = tRng.uniform ( Y1, Y2 ); 
		aP [0][2].x = tRng.uniform ( X1, X2 );
		aP [0][2].y = tRng.uniform ( Y1, Y2 );
		aP [1][0].x = tRng.uniform ( X1, X2 ); 
		aP [1][0].y = tRng.uniform ( Y1, Y2 );
		aP [1][1].x = tRng.uniform ( X1, X2 ); 
		aP [1][1].y = tRng.uniform ( Y1, Y2 );
		aP [1][2].x = tRng.uniform ( X1, X2 ); 
		aP [1][2].y = tRng.uniform ( Y1, Y2 );

		const Point* pP [2] = { aP [0], aP [1] };
		int          nP [ ] = { 3, 3 };
		    
		fillPoly ( tImg, pP, nP, 2, randomColor ( tRng ), nLineType );
	}
}

KDvoid Drawing_Random_Circles ( Mat tImg, RNG tRng )
{
	KDint	nLineType = 8;

	for ( KDint i = 0; i < NUMBER; i++ )
	{
		Point  tCenter;
		tCenter.x = tRng.uniform ( X1, X2 );
		tCenter.y = tRng.uniform ( Y1, Y2 );

		circle ( tImg, tCenter, tRng.uniform ( 0, 100 ), randomColor ( tRng ), tRng.uniform ( -1, 9 ), nLineType );
	 }
}

KDvoid Displaying_Random_Text ( Mat tImg, RNG tRng )
{
	KDint   nLineType = 8;

	for ( KDint i = 1; i < NUMBER; i++ )
	{
		Point  tP;
		tP.x = tRng.uniform ( X1, X2 );
		tP.y = tRng.uniform ( Y1, Y2 );

		putText ( tImg, "Testing text rendering", tP, tRng.uniform ( 0, 8 ),
				  tRng.uniform ( 0, 100 ) * 0.05 + 0.1, randomColor ( tRng ), tRng.uniform ( 1, 10 ), nLineType );
	    
	}

}
KDvoid Displaying_Big_End ( Mat tImg, RNG tRng )
{
	Size	tS = getTextSize ( "OpenCV forever!", CV_FONT_HERSHEY_COMPLEX, 3, 5, 0 );
	Point	tP ( ( IMG_CX - tS.width ) / 2, ( IMG_CY - tS.height ) / 2 );
	KDint	nLineType = 8;

	for ( KDint i = 0; i < 255; i += 2 )
	{
		putText ( tImg, "OpenCV forever!", tP, CV_FONT_HERSHEY_COMPLEX, 3,
				  Scalar ( i, i, 255 ), 5, nLineType );

	}
}

KDvoid BasicDrawing2 ( KDint nIdx )
{
	Mat		tDst;

	// Also create a random object (RNG)
	RNG		tRng ( 0xFFFFFFFF ); 

	// Initialize a matrix filled with zeros
	tDst = Mat::zeros ( 512, 1024, CV_8UC3 );

	// Now, let's draw some lines
	Drawing_Random_Lines ( tDst, tRng );

	// Go on drawing, this time nice rectangles
	Drawing_Random_Rectangles ( tDst, tRng );

	// Draw some ellipses
	Drawing_Random_Ellipses ( tDst, tRng );

	// Now some polylines
	Drawing_Random_Polylines ( tDst, tRng );

	// Draw filled polygons
	Drawing_Random_Filled_Polygons ( tDst, tRng );

	// Draw circles
	Drawing_Random_Circles ( tDst, tRng );

	// Display text in random positions
	Displaying_Random_Text( tDst, tRng );

	Displaying_Big_End ( tDst, tRng );

	g_pController->setFrame ( 0, tDst );
}
