/* --------------------------------------------------------------------------
 *
 *      File            Moments.cpp
 *      Description     Demo code to calculate moments
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

KDvoid ImageMoments ( KDint nIdx )
{
	std::string  sMsg;
	KDchar       szStr [256];

	Mat		tSrc;
	Mat		tDst;
	Mat		tGray;
	Mat		tCanny;
	KDint	nThresh;
	KDuint  i;
	RNG		tRng ( 12345 );

	std::vector <vector <Point> >  aContours;
	std::vector <Vec4i>	 aHierarchy;
	

	nThresh = 50;

	// Load source image
	tSrc = imread ( "/res/image/character.png" );

	// Convert image to gray and blur it
	cvtColor ( tSrc, tGray, CV_BGR2GRAY );
	blur ( tGray, tGray, Size ( 3, 3 ) );

	// Detect edges using canny
	Canny( tGray, tCanny, nThresh, nThresh * 2, 3 );

	// Find contours  
	findContours ( tCanny, aContours, aHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point ( 0, 0 ) );

	// Get the moments
	vector<Moments>		aMu ( aContours.size ( ) );
	for ( i = 0; i < aContours.size ( ); i++ )
	{
		aMu [ i ] = moments ( aContours [ i ], false ); 
	}

	// Get the mass centers:
	vector<Point2f>		aMc ( aContours.size ( ) );
	for ( i = 0; i < aContours.size ( ); i++ )
	{ 
		aMc [ i ] = Point2f ( (KDfloat) ( aMu [i].m10 / aMu [i].m00 ), (KDfloat) ( aMu [i].m01 / aMu [i].m00 ) );
	}
	
	// Draw contours
	tDst = Mat::zeros ( tCanny.size(), CV_8UC3 );
	for ( KDuint i = 0; i< aContours.size ( ); i++ )
	{ 
		kdSprintfKHR ( szStr, " * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, aMu [ i ].m00, contourArea ( aContours [ i ] ), arcLength ( aContours [ i ], true ) );  
		sMsg += szStr;

   		Scalar  tColor = Scalar ( tRng.uniform ( 0, 255 ), tRng.uniform ( 0, 255 ), tRng.uniform ( 0, 255 ) );
		drawContours ( tDst, aContours, i, tColor, 2, 8, aHierarchy, 0, Point ( ) ); 
		circle ( tDst, aMc [ i ], 4, tColor, -1, 8, 0 );
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
	g_pController->setMessage ( sMsg.c_str ( ) );
}
