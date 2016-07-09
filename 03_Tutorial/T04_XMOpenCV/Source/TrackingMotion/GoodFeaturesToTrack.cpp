/* --------------------------------------------------------------------------
 *
 *      File            GoodFeaturesToTrack.cpp
 *      Description     Demo code for detecting corners using Shi-Tomasi method
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

KDvoid GoodFeaturesToTrack ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;
	Mat		tGray;
	KDint   nThresh;
	RNG		tRng ( 12345 );

	nThresh = 205;

	// Load source image and convert it to gray
	tSrc = imread ( "/res/image/building.png" );
	cvtColor ( tSrc, tGray, CV_BGR2GRAY );

	//
	// Apply Shi-Tomasi corner detector
	//

	// Parameters for Shi-Tomasi algorithm
	vector<Point2f>		aCorners;

	KDdouble	dQualityLevel		= 0.01;
	KDdouble	dMinDistance		= 10;
	KDint		nMaxCorners			= 40;
	KDint		nBlockSize			= 3;
	bool		bUseHarrisDetector	= false;
	KDdouble	dK					= 0.04;

	// Copy the source image
	tDst = tSrc.clone ( );

	// Apply corner detection
	goodFeaturesToTrack ( tGray, aCorners, nMaxCorners, dQualityLevel, dMinDistance, Mat ( ), nBlockSize, bUseHarrisDetector, dK ); 

	// Draw corners detected
	KDint		nR = 4;
	for ( KDuint i = 0; i < aCorners.size ( ); i++ )
	{
		circle ( tDst, aCorners [ i ], nR, Scalar ( tRng.uniform ( 0, 255 ), tRng.uniform ( 0, 255 ), tRng.uniform ( 0, 255 ) ), -1, 8, 0 );
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
