/* --------------------------------------------------------------------------
 *
 *      File            Laplace.cpp
 *      Description     Sample code showing how to detect edges using
 *                      the Laplace operator
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

KDvoid Laplace ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	Mat		tGray;
	Mat		tTemp;

	KDint	nSize;
	KDint	nScale;
	KDint	nDelta;
	KDint	nDepth;

	// Load the source image
	tSrc = imread ( "/res/image/lena.jpg" ); 

	nSize  = 3;
	nScale = 1;
	nDelta = 0;
	nDepth = CV_16S;

	// Reduce noise
	GaussianBlur ( tSrc, tSrc, Size ( 3, 3 ), 0, 0, BORDER_DEFAULT );

	// Convert it to gray
	cvtColor ( tSrc, tGray, CV_RGB2GRAY );

	// Apply Laplace function
	Laplacian ( tGray, tTemp, nDepth, nSize, nScale, nDelta, BORDER_DEFAULT );
	convertScaleAbs ( tTemp, tDst );

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
