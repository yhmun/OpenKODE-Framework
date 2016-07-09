/* --------------------------------------------------------------------------
 *
 *      File            CalcHist.cpp
 *      Description     Demo code to use the function calcHist
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

KDvoid CalcHist ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	// Load the source image
	tSrc = imread ( "/res/image/bg2.png" ); 

	// Separate the image in 3 places ( R, G and B )
	vector<Mat>		aRgbPlanes;
	split ( tSrc, aRgbPlanes ); 

	// Establish the number of bins 
	int				nHistSize = 255;

	// Set the ranges ( for R,G,B) )
	float			fRange [ ] = { 0, 255 };
	const float* 	 fHistRange = { fRange };

	bool			bUniform = true;
	bool			bAccumulate = false;

	Mat				tHistR;
	Mat				tHistG;
	Mat				tHistB;

	// Compute the histograms:
	calcHist ( &aRgbPlanes [0], 1, 0, Mat ( ), tHistR, 1, &nHistSize, &fHistRange, bUniform, bAccumulate );
	calcHist ( &aRgbPlanes [1], 1, 0, Mat ( ), tHistG, 1, &nHistSize, &fHistRange, bUniform, bAccumulate );
	calcHist ( &aRgbPlanes [2], 1, 0, Mat ( ), tHistB, 1, &nHistSize, &fHistRange, bUniform, bAccumulate );

	// Draw the histograms for R, G and B
	KDint	nHistW = 400;
	KDint	nHistH = 400;
	KDint	nBinW  = cvRound ( (KDdouble) nHistW / nHistSize );

	tDst = Mat::zeros ( nHistW, nHistH, CV_8UC3 );

	// Normalize the result to [ 0, histImage.rows ]
	normalize ( tHistR, tHistR, 0, tDst.rows, NORM_MINMAX, -1, Mat ( ) );
	normalize ( tHistG, tHistG, 0, tDst.rows, NORM_MINMAX, -1, Mat ( ) );
	normalize ( tHistB, tHistB, 0, tDst.rows, NORM_MINMAX, -1, Mat ( ) );

	// Draw for each channel 
	for ( KDint i = 1; i < nHistSize; i++ )
	{ 
		line ( tDst, Point ( nBinW * ( i - 1 ), nHistH - cvRound ( tHistR.at<KDfloat> ( i - 1 ) ) ) , 
					 Point ( nBinW * ( i	 ), nHistH - cvRound ( tHistR.at<KDfloat> ( i	  ) ) ), 
					 Scalar ( 0, 0, 255 ), 2, 8, 0 ); 

		line ( tDst, Point ( nBinW * ( i - 1 ), nHistH - cvRound ( tHistG.at<KDfloat> ( i - 1 ) ) ) , 
					 Point ( nBinW * ( i	 ), nHistH - cvRound ( tHistG.at<KDfloat> ( i	  ) ) ), 
					 Scalar ( 0, 255, 0 ), 2, 8, 0 ); 

		line ( tDst, Point ( nBinW * ( i - 1 ), nHistH - cvRound ( tHistB.at<KDfloat> ( i - 1 ) ) ) , 
					 Point ( nBinW * ( i	 ), nHistH - cvRound ( tHistB.at<KDfloat> ( i	  ) ) ), 
					 Scalar ( 255, 0, 0 ), 2, 8, 0 ); 
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
