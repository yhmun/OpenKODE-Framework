/* --------------------------------------------------------------------------
 *
 *      File            IntroductionToSVM.cpp
 *      Description     Get a disparity map of two images
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

KDvoid IntroductionToSVM ( KDint nIdx )
{
	// Data for visual representation
	KDint  nCX = 512;
	KDint  nCY = 512;

	Mat  tImage = Mat::zeros ( nCY, nCX, CV_8UC3 );

	// Set up training data
	KDfloat  aLabels [4] = { 1.0, -1.0, -1.0, -1.0 };
	Mat  tLabelsMat ( 3, 1, CV_32FC1, aLabels );

	KDfloat  aTrainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
	Mat  tTrainingDataMat ( 3, 2, CV_32FC1, aTrainingData );

	// Set up SVM's parameters
	CvSVMParams  tParams;
	tParams.svm_type    = CvSVM::C_SVC;
	tParams.kernel_type = CvSVM::LINEAR;
	tParams.term_crit   = cvTermCriteria ( CV_TERMCRIT_ITER, 100, 1e-6 );

	// Train the SVM
	CvSVM  tSVM;
	tSVM.train ( tTrainingDataMat, tLabelsMat, Mat ( ), Mat ( ), tParams );

	Vec3b  tGreen ( 0, 255, 0 );
	Vec3b  tBlue  ( 255, 0, 0 );

	// Show the decision regions given by the SVM
	for ( KDint i = 0; i < tImage.rows; ++i )
	{
		for ( KDint j = 0; j < tImage.cols; ++j )
		{
			Mat  tSampleMat = ( Mat_<KDfloat> ( 1, 2 ) << i, j );

			KDfloat  fResponse = tSVM.predict ( tSampleMat );

			if ( fResponse == 1 )
			{
				tImage.at<Vec3b> ( j, i ) = tGreen;
			}
			else if ( fResponse == -1 ) 
			{
				 tImage.at<Vec3b> ( j, i ) = tBlue;
			}
		}
	}

	// Show the training data
	KDint  nThickness = -1;
	KDint  nLineType = 8;

	circle ( tImage, Point ( 501,  10 ), 5, Scalar (   0,   0,   0 ), nThickness, nLineType );
	circle ( tImage, Point ( 255,  10 ), 5, Scalar ( 255, 255, 255 ), nThickness, nLineType );
	circle ( tImage, Point ( 501, 255 ), 5, Scalar ( 255, 255, 255 ), nThickness, nLineType );
	circle ( tImage, Point (  10, 501 ), 5, Scalar ( 255, 255, 255 ), nThickness, nLineType );

	// Show support vectors
	nThickness = 2;
	nLineType  = 8;
	KDint  c   = tSVM.get_support_vector_count ( );

	for ( KDint i = 0; i < c; ++i )
	{
		const KDfloat*  v = tSVM.get_support_vector ( i );

		circle ( tImage, Point ( (KDint) v[0], (KDint) v[1] ), 6,  Scalar ( 128, 128, 128 ), nThickness, nLineType );
	}

	g_pController->setFrame ( 0, tImage );
}
