/* --------------------------------------------------------------------------
 *
 *      File            SURF_Homography.cpp
 *      Description     SURF detector + descriptor + FLANN Matcher + FindHomography
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

KDvoid SURF_Homography ( KDint nIdx )
{
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

	// -- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor  tExtractor;
	Mat  tDescriptors1, tDescriptors2;

	tExtractor.compute ( tImg1, aKeypoints1, tDescriptors1 );
	tExtractor.compute ( tImg2, aKeypoints2, tDescriptors2 );

	// -- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher		tMatcher;
	std::vector< DMatch >   aMatches;

	tMatcher.match ( tDescriptors1, tDescriptors2, aMatches );

	KDdouble  dMaxDist = 0;
	KDdouble  dMinDist = 100;

	// -- Quick calculation of max and min distances between keypoints
	for ( KDint  i = 0; i < tDescriptors1.rows; i++ )
	{	
		KDdouble  dDist = aMatches [ i ].distance;

		if ( dDist < dMinDist ) dMinDist = dDist;
		if ( dDist > dMaxDist ) dMaxDist = dDist;
	}

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
	//-- PS.- radiusMatch can also be used here.
	std::vector< DMatch >  aGoodMatches;

	for ( KDint i = 0; i < tDescriptors1.rows; i++ )
	{ 
		if ( aMatches [ i ].distance < 2 * dMinDist )
		{
			aGoodMatches.push_back ( aMatches [ i ] );
		}
	}  

	//-- Draw only "good" matches
	drawMatches ( tImg1, aKeypoints1, tImg2, aKeypoints2, 
				aGoodMatches, tDst, Scalar::all ( -1 ), Scalar::all ( -1 ), 
				vector<KDchar> ( ), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS ); 

	// -- Localize the object from img_1 in img_2 
	std::vector<Point2f>  aObject;
	std::vector<Point2f>  aScene;

	for ( KDuint i = 0; i < aGoodMatches.size ( ); i++ )
	{
		//-- Get the keypoints from the good matches
		aObject.push_back ( aKeypoints1 [ aGoodMatches [ i ].queryIdx ].pt );
		 aScene.push_back ( aKeypoints2 [ aGoodMatches [ i ].trainIdx ].pt ); 
	}

	Mat  tHomography = findHomography ( aObject, aScene, CV_RANSAC );

	// -- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f>  tObjCorners ( 4 );
	tObjCorners [ 0 ] = cvPoint ( 0, 0 ); 
	tObjCorners [ 1 ] = cvPoint ( tImg1.cols, 0 );
	tObjCorners [ 2 ] = cvPoint ( tImg1.cols, tImg1.rows );
	tObjCorners [ 3 ] = cvPoint ( 0, tImg1.rows );

	std::vector<Point2f>  tSceneCorners ( 4 );
	perspectiveTransform ( tObjCorners, tSceneCorners, tHomography );

	// -- Draw lines between the corners (the mapped object in the scene - image_2 )
	line ( tDst, tSceneCorners [0] + Point2f ( (KDfloat) tImg1.cols, 0 ), tSceneCorners [1] + Point2f ( (KDfloat) tImg1.cols, 0 ), Scalar ( 0, 255, 0 ), 4 );
	line ( tDst, tSceneCorners [1] + Point2f ( (KDfloat) tImg1.cols, 0 ), tSceneCorners [2] + Point2f ( (KDfloat) tImg1.cols, 0 ), Scalar ( 0, 255, 0 ), 4 );
	line ( tDst, tSceneCorners [2] + Point2f ( (KDfloat) tImg1.cols, 0 ), tSceneCorners [3] + Point2f ( (KDfloat) tImg1.cols, 0 ), Scalar ( 0, 255, 0 ), 4 );
	line ( tDst, tSceneCorners [3] + Point2f ( (KDfloat) tImg1.cols, 0 ), tSceneCorners [0] + Point2f ( (KDfloat) tImg1.cols, 0 ), Scalar ( 0, 255, 0 ), 4 );

	g_pController->setFrame ( 0, tDst );
}


