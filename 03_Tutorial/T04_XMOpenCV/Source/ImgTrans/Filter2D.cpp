/* --------------------------------------------------------------------------
 *
 *      File            Filter2D.cpp
 *      Description     Sample code that shows how to implement your own linear
 *                      filters by using filter2D function
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

KDvoid Filter2D	( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	// Load the source image
	tSrc = imread ( "/res/image/chicky_512.png", 1 ); 

	Mat			tKernel;
	Point		tAnchor; 
	KDint		nSize; 
	KDint       nIndex;
	KDint		nDepth;
	KDdouble	dDelta;
	
	nIndex  = 3;
	tAnchor = Point( -1, -1 );
	dDelta  = 0;
	nDepth  = -1;
	nSize   = 3 + 2 * ( nIndex % 5 );
       
    tKernel = Mat::ones ( nSize, nSize, CV_32F ) / (KDfloat) ( nSize * nSize );

	filter2D ( tSrc, tDst, nDepth , tKernel, tAnchor, dDelta, BORDER_DEFAULT );

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}