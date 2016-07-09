/* --------------------------------------------------------------------------
 *
 *      File            CalcBackProject.cpp
 *      Description     Sample code for backproject function usage
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

KDvoid CalcBackProject1 ( KDint nIdx )
{
	Mat		tOrg;
	Mat		tHsv;
	Mat		tHue;
	KDint	nBins;

	nBins = 12;

	// Load the source image
	tOrg = imread ( "/res/image/hand_sample2.jpg" ); 

	// Transform it to HSV
	cvtColor ( tOrg, tHsv, CV_BGR2HSV );

	// Use only the Hue value
	tHue.create ( tHsv.size ( ), tHsv.depth ( ) );

	const int		aCh[] = { 0, 0 };
	mixChannels ( &tHsv, 1, &tHue, 1, aCh, 1 );  

	MatND		tHist;
	int			nHistSize = KD_MAX ( nBins, 2 );
	float		aHueRange [ ] = { 0, 180 };

	const float*  pRanges = { aHueRange };

	// Get the Histogram and normalize it
	calcHist  ( &tHue, 1, 0, Mat ( ), tHist, 1, &nHistSize, &pRanges, true, false );
	normalize ( tHist, tHist, 0, 255, NORM_MINMAX, -1, Mat ( ) );
	
	// Get Backprojection
	MatND		tBackproj;
	calcBackProject ( &tHue, 1, 0, tHist, tBackproj, &pRanges, 1, true );

	// Draw the histogram
	KDint	nW = 400;
	KDint	nH = 400;
	KDint	nBinW = cvRound ( (KDdouble) nW / nHistSize ); 

	Mat		tDst = Mat::zeros ( nW, nH, CV_8UC3 );
	for ( KDint i = 0; i < nBins; i++ )  
	{ 
		rectangle ( tDst, Point ( i * nBinW, nH ), 
						  Point ( ( i + 1 ) * nBinW, nH - cvRound( tHist.at<KDfloat> ( i ) * nH / 255.0 ) ),
						  Scalar ( 0, 0, 255 ), -1 ); 
	}

	g_pController->setFrame ( 1, tBackproj );
	g_pController->setFrame ( 2, tDst );
}

KDvoid CalcBackProject2 ( KDint nIdx )
{
	Mat			tOrg;
	Mat			tHsv;
	Mat			tMask;
	KDint		nBins;

	nBins = 25;

	// Load the source image
	tOrg = imread ( "/res/image/hand_sample2.jpg" ); 

	// Transform it to HSV
	cvtColor ( tOrg, tHsv, CV_BGR2HSV );

	// Fill and get the mask
	KDint		nIo = 20;
	KDint		nUp = 20;
	KDint		nX  = 200;
	KDint		nY	= 200;

	Point		tSeed = Point ( nX, nY );

	KDint		nNewMaskVal = 255;
	Scalar		tNewVal = Scalar ( 120, 120, 120 );

	KDint		nConnectivity = 8;
	KDint		nFlags = nConnectivity + ( nNewMaskVal << 8 ) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;

	Mat			tMask2 = Mat::zeros ( tOrg.rows + 2, tOrg.cols + 2, CV_8UC1 );
	floodFill ( tOrg, tMask2, tSeed, tNewVal, 0, Scalar ( nIo, nIo, nIo ), Scalar ( nUp, nUp, nUp ), nFlags ); 
	tMask = tMask2 ( Range ( 1, tMask2.rows - 1 ), Range ( 1, tMask2.cols - 1 ) );

	MatND		tHist;
	int			nBinsH = 30;
	int			nBinsS = 32;
	int			nHistSize [ ] = { nBinsH, nBinsS };

	float		fRangeH [ ] = { 0, 179 };
	float		fRangeS [ ] = { 0, 255 };
	const float*  pRanges [ ] = { fRangeH, fRangeS };

	int		aChannels [ ] = { 0, 1 };

	// Get the Histogram and normalize it
	calcHist ( &tHsv, 1, aChannels, tMask, tHist, 2, nHistSize, pRanges, true, false );

	normalize ( tHist, tHist, 0, 255, NORM_MINMAX, -1, Mat ( ) );

	// Get Backprojection
	MatND		tBackproj;
	calcBackProject ( &tHsv, 1, aChannels, tHist, tBackproj, pRanges, 1, true );

	g_pController->setFrame ( 1, tMask );
	g_pController->setFrame ( 2, tBackproj );
}
