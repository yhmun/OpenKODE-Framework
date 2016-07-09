/* --------------------------------------------------------------------------
 *
 *      File            BasicDrawing1.cpp
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

#define w	400

// Draw a fixed-size ellipse with different angles
KDvoid MyEllipse ( Mat tImg, KDdouble dAngle )
{
	KDint  nThickness = 2;
	KDint  nLineType  = 8;

	ellipse 
	( 
		tImg,
		Point ( w / 2, w /  2 ),
		Size  ( w / 4, w / 16 ),
		dAngle,
		0, 
		360,
		Scalar ( 255, 0, 0 ),
		nThickness,
		nLineType 
	);  
}

// Draw a fixed-size filled circle
KDvoid MyFilledCircle ( Mat tImg, Point tCenter )
{
	KDint  nThickness = -1;
	KDint  nLineType  = 8;

	circle
	(
		tImg, 
		tCenter,
		w / 32,
		Scalar ( 0, 0, 255 ),
		nThickness,
		nLineType 
	);
}

// Draw a simple concave polygon (rook)
KDvoid MyPolygon ( Mat tImg )
{
	KDint  nLineType  = 8;

	// Create some points 
	Point  aRookPoints [1][20];

	aRookPoints [0][ 0] = Point (      w /  4,  7 * w /  8 );
	aRookPoints [0][ 1] = Point (  3 * w /  4,  7 * w /  8 );
	aRookPoints [0][ 2] = Point (  3 * w /  4, 13 * w / 16 );
	aRookPoints [0][ 3] = Point ( 11 * w / 16, 13 * w / 16 );
	aRookPoints [0][ 4] = Point ( 19 * w / 32,  3 * w /  8 );
	aRookPoints [0][ 5] = Point (  3 * w /  4,  3 * w /  8 );
	aRookPoints [0][ 6] = Point (  3 * w /  4,      w /  8 );
	aRookPoints [0][ 7] = Point ( 26 * w / 40,      w /  8 );
	aRookPoints [0][ 8] = Point ( 26 * w / 40,      w /  4 );
	aRookPoints [0][ 9] = Point ( 22 * w / 40,      w /  4 );
	aRookPoints [0][10] = Point ( 22 * w / 40,      w /  8 );
	aRookPoints [0][11] = Point ( 18 * w / 40,      w /  8 );
	aRookPoints [0][12] = Point ( 18 * w / 40,      w /  4 );
	aRookPoints [0][13] = Point ( 14 * w / 40,      w /  4 );
	aRookPoints [0][14] = Point ( 14 * w / 40,      w /  8 );
	aRookPoints [0][15] = Point (      w /  4,      w /  8 );
	aRookPoints [0][16] = Point (      w /  4,  3 * w /  8 );
	aRookPoints [0][17] = Point ( 13 * w / 32,  3 * w /  8 );
	aRookPoints [0][18] = Point (  5 * w / 16, 13 * w / 16 );
	aRookPoints [0][19] = Point (      w /  4, 13 * w / 16 );

	const Point*  pPt[1] = { aRookPoints[0] };
	int	          nPt[ ] = { 20 };

	fillPoly
	(
		tImg,
		pPt,
		nPt,
		1,
		Scalar ( 255, 255, 255 ),
		nLineType 
	);			
}

// Draw a simple line
KDvoid MyLine ( Mat tImg, Point tStart, Point tEnd )
{
	KDint  nThickness = 2;
	KDint  nLineType  = 8;

	line
	(
		tImg, 
		tStart,
		tEnd,
		Scalar ( 0, 0, 0 ),
		nThickness,
		nLineType 
	);
}

KDvoid BasicDrawing1 ( KDint nIdx )
{
	Mat			tSrc;
	Mat			tDst;

	// Create black empty images 
	tSrc = Mat::zeros ( w, w, CV_8UC3 );
	tDst = Mat::zeros ( w, w, CV_8UC3 );

	// 1. Draw a simple atom:
	// -----------------------

	/// 1.a. Creating ellipses 
	MyEllipse ( tSrc,  90 );
	MyEllipse ( tSrc,   0 );
	MyEllipse ( tSrc,  45 );
	MyEllipse ( tSrc, -45 );

	// 1.b. Creating circles
	MyFilledCircle ( tSrc, Point ( w / 2, w / 2 ) );

	// 2. Draw a rook
	// ------------------

	// 2.a. Create a convex polygon
	MyPolygon ( tDst );

	// 2.b. Creating rectangles
	rectangle ( tDst, Point ( 0, 7 * w / 8 ), Point ( w, w ), Scalar ( 0, 255, 255 ), -1, 8 );

	// 2.c. Create a few lines 
	MyLine ( tDst, Point(         0, 15 * w / 16 ), Point (         w, 15 * w / 16 ) );
	MyLine ( tDst, Point(     w / 4,  7 * w /  8 ), Point (     w / 4,           w ) );
	MyLine ( tDst, Point(     w / 2,  7 * w /  8 ), Point (     w / 2,           w ) );
	MyLine ( tDst, Point( 3 * w / 4,  7 * w /  8 ), Point ( 3 * w / 4,           w ) );

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}

