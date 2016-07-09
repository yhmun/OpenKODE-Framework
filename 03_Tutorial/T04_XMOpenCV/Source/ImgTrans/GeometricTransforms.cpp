/* --------------------------------------------------------------------------
 *
 *      File            GeometricTransforms.cpp
 *      Description     Demo code for Geometric Transforms
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

KDvoid GeometricTransforms ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	Point2f	tSrcTri [3];
	Point2f	tDstTri [3];

	Mat		tMatrix ( 2, 3, CV_32FC1 );
	Mat		tWrap;

	// Load the source image
	tSrc = imread ( "/res/image/bg.png" ); 

	// Set the dst image the same type and size as src  
	tWrap = Mat::zeros ( tSrc.rows, tSrc.cols, tSrc.type ( ) );

	// Set your 3 points to calculate the  Affine Transform
	tSrcTri [0] = Point2f ( 0, 0 );
	tSrcTri [1] = Point2f ( tSrc.cols - 1.f, 0 );
	tSrcTri [2] = Point2f ( 0, tSrc.rows - 1.f );

	tDstTri [0] = Point2f ( tSrc.cols * 0.0f , tSrc.rows * 0.33f );
	tDstTri [1] = Point2f ( tSrc.cols * 0.85f, tSrc.rows * 0.25f );
	tDstTri [2] = Point2f ( tSrc.cols * 0.15f, tSrc.rows * 0.7f );

	// Get the Affine Transform
	tMatrix = getAffineTransform ( tSrcTri, tDstTri );

	// Apply the Affine Transform just found to the src image
	warpAffine ( tSrc, tWrap, tMatrix, tWrap.size ( ) );

	if ( nIdx == 0 )
	{
		tDst = tWrap;
	}
	else
	{
		// Rotating the image after Warp 
		Mat			tRotate ( 2, 3, CV_32FC1 );

		// Compute a rotation matrix with respect to the center of the image
		Point		tCenter = Point ( tWrap.cols / 2, tWrap.rows / 2 );
		KDdouble	dAngle  = -50.0;
		KDdouble	dScale  = 0.6;

		// Get the rotation matrix with the specifications above
		tRotate = getRotationMatrix2D ( tCenter, dAngle, dScale );

		// Rotate the warped image
		warpAffine ( tWrap, tDst, tRotate, tWrap.size ( ) );
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
