/* --------------------------------------------------------------------------
 *
 *      File            HoughCircle.cpp
 *      Description     Demo code for Hough Transform
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

KDvoid HoughCircle ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;
	Mat		tGray;
	KDsize  i;

	vector<Vec3f>  aCircles;

	// Load the source image
	tSrc = imread ( "/res/image/cycle.jpg" ); 
	tDst = tSrc.clone ( );

	// Convert it to gray 
	cvtColor ( tSrc, tGray, CV_BGR2GRAY );

	// Reduce the noise so we avoid false circle detection
	GaussianBlur ( tGray, tGray, Size ( 9, 9 ), 2, 2 );

	// Apply the Hough Transform to find the circles
	HoughCircles ( tGray, aCircles, CV_HOUGH_GRADIENT, 1, tGray.rows / 8, 200, 100, 0, 0 );

	// Draw the circles detected
	for( i = 0; i < aCircles.size ( ); i++ )
	{
		 Point  tCenter ( cvRound ( aCircles [i][0] ), cvRound ( aCircles [i][1] ) );
		 KDint  nRadius = cvRound ( aCircles [i][2] );

		 // circle center
		 circle ( tDst, tCenter, 3, Scalar ( 0, 255, 0 ), -1, 8, 0 );

		 // circle outline
		 circle ( tDst, tCenter, nRadius, Scalar ( 0, 0, 255 ), 3, 8, 0 );
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}

