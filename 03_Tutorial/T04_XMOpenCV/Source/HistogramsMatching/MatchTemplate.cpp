/* --------------------------------------------------------------------------
 *
 *      File            MatchTemplate.cpp
 *      Description     Sample code to use the function MatchTemplate
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

KDvoid MatchTemplate ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	Mat		tTempl;
	KDint   nMethod;

	// Load the source image
	tSrc = imread ( "/res/image/couple.jpg" ); 
	tTempl = imread ( "/res/image/couple_face.jpg" ); 

	// Create the result matrix
	KDint	nCols = tSrc.cols - tTempl.cols + 1;
	KDint	nRows = tSrc.rows - tTempl.rows + 1;   

	tDst.create ( nCols, nRows, CV_32FC1 );

	switch ( nIdx )
	{
		case  0 : nMethod = TM_SQDIFF;			break;
		case  1 : nMethod = TM_SQDIFF_NORMED;	break;
		case  2 : nMethod = TM_CCORR;			break;
		case  3 : nMethod = TM_CCORR_NORMED;	break;
		case  4 : nMethod = TM_CCOEFF;			break;
		case  5 : nMethod = TM_CCOEFF_NORMED;	break;
		default : nMethod = 0;
	}

	// Do the Matching and Normalize
	matchTemplate ( tSrc, tTempl, tDst, nMethod );
	normalize ( tDst, tDst, 0, 1, NORM_MINMAX, -1, Mat ( ) );

	// Localizing the best match with minMaxLoc
	KDdouble	dMinVal;
	KDdouble	dMaxVal; 
	Point		tMinLoc;
	Point		tMaxLoc;
	Point		tMatchLoc;

	minMaxLoc ( tDst, &dMinVal, &dMaxVal, &tMinLoc, &tMaxLoc, Mat ( ) );

	// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if ( nMethod  == CV_TM_SQDIFF || nMethod == CV_TM_SQDIFF_NORMED )
	{ 
		tMatchLoc = tMinLoc; 
	}
	else  
	{
		tMatchLoc = tMaxLoc;
	}

	// Show me what you got
	rectangle ( tSrc, tMatchLoc, Point ( tMatchLoc.x + tTempl.cols , tMatchLoc.y + tTempl.rows ), Scalar ( 255, 0, 0 ), 2, 8, 0 ); 

	tMatchLoc.x -= tTempl.cols / 2;
	tMatchLoc.y -= tTempl.rows / 2;
	rectangle ( tDst, tMatchLoc, Point ( tMatchLoc.x + tTempl.cols , tMatchLoc.y + tTempl.rows ), Scalar::all ( 0 ), 2, 8, 0 ); 

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
