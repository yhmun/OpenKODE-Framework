/* --------------------------------------------------------------------------
 *
 *      File            SBMSample.cpp
 *      Description     Get a disparity map of two images
 *      Author          A. Huaman, Y.H Mun
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

KDvoid SBMSample ( KDint nIdx )
{
	Mat		tImg1;
	Mat		tImg2;

	// 1. Read the images
	tImg1 = imread ( "/res/image/tsukuba_l.png", CV_LOAD_IMAGE_GRAYSCALE );
	tImg2 = imread ( "/res/image/tsukuba_r.png", CV_LOAD_IMAGE_GRAYSCALE );

	// And create the image in which we will save our disparities
	Mat	  tDisparity16S = Mat ( tImg1.rows, tImg1.cols, CV_16S );
	Mat   tDisparity8U  = Mat ( tImg1.rows, tImg1.cols, CV_8UC1 );

	// 2. Call the constructor for StereoBM
	KDint  ndisparities = 16 * 5;   // Range of disparity 
	KDint  nSADWindowSize = 21;		// Size of the block window. Must be odd 

	StereoBM  tSBM ( StereoBM::BASIC_PRESET, ndisparities, nSADWindowSize );

	// 3. Calculate the disparity image
	tSBM ( tImg1, tImg2, tDisparity16S, CV_16S );

	// Check its extreme values
	KDdouble  dMinVal;
	KDdouble  dMaxVal;

	minMaxLoc ( tDisparity16S, &dMinVal, &dMaxVal );

	// 4. Display it as a CV_8UC1 image
	tDisparity16S.convertTo ( tDisparity8U, CV_8UC1, 255 / ( dMaxVal - dMinVal ) );

	// 5. Save the image
	imwrite ( "/data/SBMSample.png", tDisparity16S );

	g_pController->setFrame ( 1, tDisparity8U );
	g_pController->setFrame ( 2, tDisparity16S );
}
