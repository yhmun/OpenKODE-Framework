/* --------------------------------------------------------------------------
 *
 *      File            NonLinearSVMs.cpp
 *      Description     This program shows Support Vector Machines for 
 *                      Non-Linearly Separable Data
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

#define	NTRAINING_SAMPLES	100			// Number of training samples per class
#define FRAC_LINEAR_SEP		0.9f	    // Fraction of samples which compose the linear separable part

KDvoid NonLinearSVMs ( KDint nIdx )
{
	// Data for visual representation
	const KDint  WIDTH = 512, HEIGHT = 512;

	Mat  tImage = Mat::zeros ( HEIGHT, WIDTH, CV_8UC3 );

	//--------------------- 1. Set up training data randomly ---------------------------------------
	Mat  tTrainData ( 2 * NTRAINING_SAMPLES, 2, CV_32FC1 );
	Mat  tLabels    ( 2 * NTRAINING_SAMPLES, 1, CV_32FC1 );

	RNG  tRng ( 100 );  // Random value generation class

	// Set up the linearly separable part of the training data
	KDint  nLinearSamples = (KDint) ( FRAC_LINEAR_SEP * NTRAINING_SAMPLES );

	// Generate random points for the class 1
	Mat  tTrainClass = tTrainData.rowRange ( 0, nLinearSamples );
	// The x coordinate of the points is in [0, 0.4)
	Mat  tC = tTrainClass.colRange ( 0, 1 );
	tRng.fill ( tC, RNG::UNIFORM, Scalar ( 1 ), Scalar ( 0.4 * WIDTH ) );
	// The y coordinate of the points is in [0, 1)
	tC = tTrainClass.colRange ( 1, 2 );
	tRng.fill ( tC, RNG::UNIFORM, Scalar ( 1 ), Scalar ( HEIGHT ) );

	// Generate random points for the class 2
	tTrainClass = tTrainData.rowRange ( 2 * NTRAINING_SAMPLES - nLinearSamples, 2 * NTRAINING_SAMPLES );
	// The x coordinate of the points is in [0.6, 1]
	tC = tTrainClass.colRange ( 0, 1 ); 
	tRng.fill ( tC, RNG::UNIFORM, Scalar ( 0.6 * WIDTH ), Scalar ( WIDTH ) );
	// The y coordinate of the points is in [0, 1)
	tC = tTrainClass.colRange ( 1, 2 );
	tRng.fill ( tC, RNG::UNIFORM, Scalar ( 1 ), Scalar ( HEIGHT ) );

	//------------------ Set up the non-linearly separable part of the training data ---------------

	// Generate random points for the classes 1 and 2
	tTrainClass = tTrainData.rowRange ( nLinearSamples, 2 * NTRAINING_SAMPLES - nLinearSamples );
	// The x coordinate of the points is in [0.4, 0.6)
	tC = tTrainClass.colRange ( 0, 1 );
	tRng.fill ( tC, RNG::UNIFORM, Scalar ( 0.4 * WIDTH ), Scalar ( 0.6 * WIDTH ) ); 
	// The y coordinate of the points is in [0, 1)
	tC = tTrainClass.colRange ( 1, 2 );
	tRng.fill ( tC, RNG::UNIFORM, Scalar ( 1 ), Scalar ( HEIGHT ) );

	//------------------------- Set up the labels for the classes ---------------------------------
	tLabels.rowRange (                 0,     NTRAINING_SAMPLES ).setTo ( 1 );  // Class 1
	tLabels.rowRange ( NTRAINING_SAMPLES, 2 * NTRAINING_SAMPLES ).setTo ( 2 );  // Class 2

	//------------------------ 2. Set up the support vector machines parameters --------------------
	CvSVMParams  tParams;
	tParams.svm_type    = SVM::C_SVC;
	tParams.C 		    = 0.1;
	tParams.kernel_type = SVM::LINEAR;
	tParams.term_crit   = TermCriteria ( CV_TERMCRIT_ITER, (KDint) 1e7, 1e-6 );

	//------------------------ 3. Train the svm ----------------------------------------------------
	CvSVM  tSvm;
	tSvm.train ( tTrainData, tLabels, Mat ( ), Mat ( ), tParams );

	//------------------------ 4. Show the decision regions ----------------------------------------
	Vec3b  tGreen ( 0, 100, 0 );
	Vec3b  tBlue  ( 100, 0, 0 );
	for ( KDint i = 0; i < tImage.rows; ++i )
	{
		for ( KDint j = 0; j < tImage.cols; ++j )
		{
			Mat     tSampleMat = (Mat_<KDfloat> ( 1, 2 ) << i, j );
			KDfloat  fResponse = tSvm.predict ( tSampleMat );

			if      ( fResponse == 1 )    tImage.at<Vec3b> ( j, i )  = tGreen;
			else if ( fResponse == 2 )    tImage.at<Vec3b> ( j, i )  = tBlue;
		}
	}

	//----------------------- 5. Show the training data --------------------------------------------
	KDint  nThick = -1;
	KDint  nLineType = 8;
	KDfloat  fPx, fPy;

	// Class 1
	for ( KDint i = 0; i < NTRAINING_SAMPLES; ++i )
	{
		fPx = tTrainData.at<KDfloat> ( i, 0 );
		fPy = tTrainData.at<KDfloat> ( i, 1 );
		circle ( tImage, Point ( (KDint) fPx,  (KDint) fPy ), 3, Scalar ( 0, 255, 0 ), nThick, nLineType );
	}

	// Class 2
	for ( KDint i = NTRAINING_SAMPLES; i < 2 * NTRAINING_SAMPLES; ++i )
	{
		fPx = tTrainData.at<KDfloat> ( i, 0 );
		fPy = tTrainData.at<KDfloat> ( i, 1 );
		circle ( tImage, Point ( (KDint) fPx, (KDint) fPy ), 3, Scalar ( 255, 0, 0 ), nThick, nLineType );
	}

	//------------------------- 6. Show support vectors --------------------------------------------
	nThick    = 2;
	nLineType = 8;
	KDint  nX = tSvm.get_support_vector_count ( );

	for ( KDint i = 0; i < nX; ++i )
	{
		const KDfloat*  v = tSvm.get_support_vector ( i );
		circle ( tImage,  Point ( (KDint) v[0], (KDint) v[1] ), 6, Scalar ( 128, 128, 128 ), nThick, nLineType );
	}
}
