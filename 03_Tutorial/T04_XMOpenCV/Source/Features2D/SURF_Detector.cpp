/* --------------------------------------------------------------------------
 *
 *      File            SURF_Detector.cpp
 *      Description     SURF keypoint detection + keypoint drawing with
 *                      OpenCV functions
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
#include "XMOpenCV2/nonfree/features2d.hpp"

KDvoid SURF_Detector ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;
	Mat		tImg1;
	Mat		tImg2;

	tImg1 = imread ( "/res/image/box.png", CV_LOAD_IMAGE_GRAYSCALE );
	tImg2 = imread ( "/res/image/box_in_scene.png", CV_LOAD_IMAGE_GRAYSCALE );

	// -- Step 1: Detect the keypoints using SURF Detector
	KDint  nMinHessian = 400;

	SurfFeatureDetector   tDetector ( nMinHessian );
	std::vector<KeyPoint> aKeypoints1, aKeypoints2;

	tDetector.detect ( tImg1, aKeypoints1 );
	tDetector.detect ( tImg2, aKeypoints2 );

	// -- Draw keypoints
	drawKeypoints ( tImg1, aKeypoints1, tSrc, Scalar::all ( -1 ), DrawMatchesFlags::DEFAULT ); 
	drawKeypoints ( tImg2, aKeypoints2, tDst, Scalar::all ( -1 ), DrawMatchesFlags::DEFAULT ); 

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );

}
