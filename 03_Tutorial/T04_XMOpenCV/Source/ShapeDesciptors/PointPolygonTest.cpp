/* --------------------------------------------------------------------------
 *
 *      File            PointPolygonTest.cpp
 *      Description     Demo code to use the pointPolygonTest function...fairly easy
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

KDvoid  PointPolygonTest ( KDint nIdx )
{
	KDint			i, j;

	// Create an image
	const KDint		r = 100;

	Mat		tSrc;
	Mat		tDst;

	tSrc = Mat::zeros ( Size ( 4 * r, 4 * r ), CV_8UC1 );

	// Create a sequence of points to make a contour:
	vector<Point2f>		aVert ( 6 );

	aVert [0] = Point2f ( 1.5f * r, 1.340f * r );
	aVert [1] = Point2f ( 1.0f * r, 2.000f * r );
	aVert [2] = Point2f ( 1.5f * r, 2.866f * r );
	aVert [3] = Point2f ( 2.5f * r, 2.866f * r );
	aVert [4] = Point2f ( 3.0f * r, 2.000f * r );
	aVert [5] = Point2f ( 2.5f * r, 1.340f * r );

	// Draw it in src
	for ( j = 0; j < 6; j++ )
	{ 
		line ( tSrc, aVert [ j ],  aVert [ ( j + 1 ) % 6 ], Scalar ( 255 ), 3, 8 ); 
	} 

	// Get the contours
	vector<vector<Point> >	aContours;
	vector<Vec4i>			aHierarchy;

    Mat     tCopy = tSrc.clone ( );
	findContours ( tCopy, aContours, aHierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );  

	// Calculate the distances to the contour
	Mat		tRawDist ( tSrc.size ( ), CV_32FC1 );
	for ( j = 0; j < tSrc.rows; j++ )
	{
		for ( i = 0; i < tSrc.cols; i++ )
		{
			tRawDist.at<KDfloat> ( j, i ) = (KDfloat) pointPolygonTest ( aContours [ 0 ], Point2f ( (KDfloat) i, (KDfloat) j ), true );
		}
	}

	KDdouble	dMinVal;
	KDdouble	dMaxVal;

	minMaxLoc ( tRawDist, &dMinVal, &dMaxVal, 0, 0, Mat ( ) );
	dMinVal = kdFabsKHR ( dMinVal ); dMaxVal = kdFabsKHR ( dMaxVal );

	// Depicting the  distances graphically
	tDst = Mat::zeros ( tSrc.size ( ), CV_8UC3 );
	for ( j = 0; j < tSrc.rows; j++ )
	{
		for( i = 0; i < tSrc.cols; i++ )
		{ 
			if ( tRawDist.at<KDfloat> ( j, i ) < 0 )
			{
				tDst.at<Vec3b> ( j, i )[0] = 255 - (KDint) ( kdFabsf ( tRawDist.at<KDfloat> ( j, i ) ) * 255 / dMinVal );
			}
			else if ( tRawDist.at<KDfloat> ( j, i ) > 0 )
			{
				tDst.at<Vec3b> ( j, i )[2] = 255 - (KDint) ( tRawDist.at<KDfloat> ( j, i ) * 255 / dMaxVal ); 
			}    
			else
			{
				tDst.at<Vec3b> ( j, i )[0] = 255; 
				tDst.at<Vec3b> ( j, i )[1] = 255;
				tDst.at<Vec3b> ( j, i )[2] = 255; 
			}     
		}
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
