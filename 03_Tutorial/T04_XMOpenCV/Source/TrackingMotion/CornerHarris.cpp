/* --------------------------------------------------------------------------
 *
 *      File            CornerHarris.cpp
 *      Description     Demo code for detecting corners using Harris-Stephens method
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

KDvoid CornerHarris ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tGray;
	KDint   nThresh;

	nThresh = 205;

	// Load source image and convert it to gray
	tSrc = imread ( "/res/image/building.png" );
	cvtColor ( tSrc, tGray, CV_BGR2GRAY );

	//
	// Executes the corner detection and draw a circle around the possible corners
	//
	Mat			tCorner;
	Mat			tNorm;
	Mat			tNormScaled;

	// Detector parameters
	KDint		nBlockSize		= 2;
	KDint		nApertureSize	= 3;
	KDdouble	dK				= 0.04;

	tCorner = Mat::zeros ( tSrc.size ( ), CV_32FC1 );

	// Detecting corners
	cornerHarris ( tGray, tCorner, nBlockSize, nApertureSize, dK, BORDER_DEFAULT );

	// Normalizing
	normalize ( tCorner, tNorm, 0, 255, NORM_MINMAX, CV_32FC1, Mat ( ) );
	convertScaleAbs ( tNorm, tNormScaled ); 

	// Drawing a circle around corners
	for ( KDint j = 0; j < tNorm.rows ; j++ )
	{ 
		for ( KDint i = 0; i < tNorm.cols; i++ )
		{
			if( (KDint) tNorm.at<KDfloat> ( j, i ) > nThresh )
			{ 
				circle ( tNormScaled, Point ( i, j ), 5,  Scalar ( 0 ), 2, 8, 0 ); 
			}
		} 
	}    

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tNormScaled );
}
