/* --------------------------------------------------------------------------
 *
 *      File            CornerDetector.cpp
 *      Description     Demo code for detecting corners using OpenCV built-in functions
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

KDvoid CornerDetector ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	Mat		tOrg;
	Mat		tGray;
	RNG		tRng ( 12345 );
	KDint	i, j;

	// Load source image and convert it to gray
	tOrg = imread ( "/res/image/building2.png", 1 );
	cvtColor ( tOrg, tGray, CV_BGR2GRAY );

	tSrc = tOrg.clone ( );
	tDst = tOrg.clone ( );

	// Set some parameters
	KDint		nBlockSize		 = 3;
	KDint		nApertureSize	 = 3;
	KDdouble	dMinVal			 = 0;
	KDdouble	dMaxVal			 = 0;
	KDint		nQualityLevel	 = 5;
	KDint		nMaxQualityLevel = 100;

	//
	// My Harris matrix -- Using cornerEigenValsAndVecs
	//
	Mat			tMyHarris;
	Mat			tMc;

	tMyHarris = Mat::zeros ( tGray.size ( ), CV_32FC ( 6 ) );
	tMc = Mat::zeros ( tGray.size ( ), CV_32FC1 );

	cornerEigenValsAndVecs ( tGray, tMyHarris, nBlockSize, nApertureSize, BORDER_DEFAULT );

	// calculate Mc  
	for ( j = 0; j < tGray.rows; j++ )
	{
		for ( i = 0; i < tGray.cols; i++ )
		{
			Vec2f  vLambda = tMyHarris.at<Vec2f> ( j, i );

			tMc.at<KDfloat> ( j, i ) = vLambda[0] * vLambda[1] - 0.04f * kdPowf ( vLambda[0] + vLambda[1], 2 );
		}
	}

	minMaxLoc ( tMc, &dMinVal, &dMaxVal, 0, 0, Mat ( ) );

	for ( j = 0; j < tGray.rows; j++ )
	{
		for ( i = 0; i < tGray.cols; i++ )
		{
			if ( tMc.at<KDfloat> ( j, i ) > dMinVal + ( dMaxVal - dMinVal ) * nQualityLevel / nMaxQualityLevel )
			{
//				circle ( tSrc, Point ( i, j ), 4, Scalar ( tRng.uniform ( 0, 255 ), tRng.uniform ( 0, 255 ), tRng.uniform ( 0, 255 ) ), -1, 8, 0 );
			}
		}
	}

	//
	// My Shi-Tomasi -- Using cornerMinEigenVal
	//
	Mat			tMyShiTomasi;

	tMyShiTomasi = Mat::zeros ( tGray.size ( ), CV_32FC1 );  
	cornerMinEigenVal ( tGray, tMyShiTomasi, nBlockSize, nApertureSize, BORDER_DEFAULT );

	minMaxLoc ( tMyShiTomasi, &dMinVal, &dMaxVal, 0, 0, Mat ( ) );

	for( j = 0; j < tGray.rows; j++ )
	{
		for ( i = 0; i < tGray.cols; i++ )
		{
			if ( tMyShiTomasi.at<KDfloat> ( j, i ) > dMinVal + ( dMaxVal - dMinVal ) * nQualityLevel / nMaxQualityLevel )
			{
				circle ( tDst, Point ( i, j ), 4, Scalar ( tRng.uniform ( 0, 255 ), tRng.uniform ( 0, 255 ), tRng.uniform ( 0, 255 ) ), -1, 8, 0 );
			}
		}
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
