/* --------------------------------------------------------------------------
 *
 *      File            HoughLines.cpp
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

KDvoid HoughLines ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	Mat		tGray;
	Mat		tEdge;

	KDint   nThreshold;
	KDuint  i;

	// Load the source image
	tSrc = imread ( "/res/image/building2.png" ); 

	// Pass the image to gray
	cvtColor ( tSrc, tGray, CV_RGB2GRAY );

	// Apply Canny edge detector
	Canny ( tGray, tEdge, 50, 200, 3 );

	cvtColor ( tEdge, tDst, CV_GRAY2BGR );

	if ( nIdx == 0 )
	{
		vector<Vec2f>  vLines;

		nThreshold = 150;

		// 1. Use Standard Hough Transform
		HoughLines ( tEdge, vLines, 1, CV_PI / 180, nThreshold, 0, 0 ); 

		// Show the result
		for( i = 0; i < vLines.size ( ); i++ )
		{
			KDfloat		r = vLines [i][0];
			KDfloat		t = vLines [i][1];
			KDdouble	cos_t = kdCosKHR ( t );
			KDdouble	sin_t = kdSinKHR  ( t );
			KDdouble	x0 = r * cos_t;
			KDdouble	y0 = r * sin_t;
			KDdouble	alpha = 1000;

			Point	pt1 ( cvRound ( x0 + alpha * ( -sin_t ) ), cvRound ( y0 + alpha * cos_t ) );
			Point	pt2 ( cvRound ( x0 - alpha * ( -sin_t ) ), cvRound ( y0 - alpha * cos_t ) );

			line ( tDst, pt1, pt2, Scalar ( 255, 0, 0 ), 3, CV_AA ); 
		} 
	}
	else
	{
		vector<Vec4i>  vLines;
	
		nThreshold = 70;

		// 2. Use Probabilistic Hough Transform
		HoughLinesP ( tEdge, vLines, 1, CV_PI / 180, nThreshold, 30, 10 );

		// Show the result
		for ( i = 0; i < vLines.size ( ); i++ )
		{
			Vec4i  tLine = vLines [ i ];
			line ( tDst, Point ( tLine[0], tLine[1] ), Point ( tLine[2], tLine[3] ), Scalar ( 255, 0, 0 ), 3, CV_AA );
		}
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
