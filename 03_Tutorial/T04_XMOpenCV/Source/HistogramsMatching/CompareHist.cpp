/* --------------------------------------------------------------------------
 *
 *      File            CompareHist.cpp
 *      Description     Sample code to use the function compareHist
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

KDvoid CompareHist ( KDint nIdx )
{
	KDchar       szStr [ 256 ];
	std::string  sMsg;
	
	Mat		tSrcBase , tHsvBase;
	Mat		tSrcTest1, tHsvTest1;
	Mat		tSrcTest2, tHsvTest2;
	Mat		tHsvHalfDown;

	tSrcBase  = imread ( "/res/image/hand_sample1.jpg", 1 );
	tSrcTest1 = imread ( "/res/image/hand_sample2.jpg", 1 );
	tSrcTest2 = imread ( "/res/image/hand_sample3.jpg", 1 );

	// Convert to HSV
	cvtColor ( tSrcBase , tHsvBase , CV_BGR2HSV );
	cvtColor ( tSrcTest1, tHsvTest1, CV_BGR2HSV );
	cvtColor ( tSrcTest2, tHsvTest2, CV_BGR2HSV );

	tHsvHalfDown = tHsvBase ( Range ( tHsvBase.rows / 2, tHsvBase.rows - 1 ), Range ( 0, tHsvBase.cols - 1 ) );  

	// Using 30 bins for hue and 32 for saturation
	int	  nBinsH = 50;
	int	  nBinsS = 60;
	int	  nHistSize [ ] = { nBinsH, nBinsS };

	// hue varies from 0 to 256, saturation from 0 to 180
	float		fRangeH [ ] = { 0, 256 };
	float		fRangeS [ ] = { 0, 180 };
	const float* pRanges [ ] = { fRangeH, fRangeS };

	// Use the o-th and 1-st nChannels
	int		nChannels [ ] = { 0, 1 };

	// Histograms
	MatND	tHistBase;
	MatND	tHistHalfDown;
	MatND	tHistTest1;
	MatND	tHistTest2;

	// Calculate the histograms for the HSV images
	calcHist  ( &tHsvBase, 1, nChannels, Mat(), tHistBase, 2, nHistSize, pRanges, true, false );
	normalize ( tHistBase, tHistBase, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist  ( &tHsvHalfDown, 1, nChannels, Mat(), tHistHalfDown, 2, nHistSize, pRanges, true, false );
	normalize ( tHistHalfDown, tHistHalfDown, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist  ( &tHsvTest1, 1, nChannels, Mat(), tHistTest1, 2, nHistSize, pRanges, true, false );
	normalize ( tHistTest1, tHistTest1, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist  ( &tHsvTest2, 1, nChannels, Mat(), tHistTest2, 2, nHistSize, pRanges, true, false );
	normalize ( tHistTest2, tHistTest2, 0, 1, NORM_MINMAX, -1, Mat() );

	// Apply the histogram comparison methods
	for( KDint i = 0; i < 4; i++ )
	{
		KDint		nCompareMethod = i; 
		KDdouble	dBaseBase  = compareHist ( tHistBase, tHistBase		, nCompareMethod );
		KDdouble	dBaseHalf  = compareHist ( tHistBase, tHistHalfDown	, nCompareMethod );
		KDdouble	dBaseTest1 = compareHist ( tHistBase, tHistTest1	, nCompareMethod );
		KDdouble	dBaseTest2 = compareHist ( tHistBase, tHistTest2	, nCompareMethod );
	 
		kdSprintfKHR ( szStr, " Method [%d] Perfect, Base-Half, Base-Test(1), Base-Test(2) : %f, %f, %f, %f \n", i, dBaseBase, dBaseHalf, dBaseTest1, dBaseTest2 );
		sMsg += szStr;
	} 
	
	g_pController->setFrame ( 1, tSrcBase );
	g_pController->setFrame ( 2, tSrcTest2 );
	g_pController->setMessage ( sMsg.c_str ( ) );
}
