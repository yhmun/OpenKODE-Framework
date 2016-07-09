/* --------------------------------------------------------------------------
 *
 *      File            Threshold.cpp
 *      Description     Sample code that shows how to use the diverse
 *                      threshold options offered by OpenCV
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

KDvoid Threshold ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;
	Mat     tGray;
	KDint   nType;
	KDint   nValue;
	KDint	nMaxBinary;
	
	// Load the source image
	tSrc = imread ( "/res/image/chicky_512.png", 1 ); 

	// Convert the image to Gray
	cvtColor ( tSrc, tGray, CV_RGB2GRAY );

	nValue = 128;
	nMaxBinary = 255;

	switch ( nIdx )
	{
		case 0 : nType = THRESH_BINARY;		break;	// Binary
		case 1 : nType = THRESH_BINARY_INV;	break;	// Binary Inverted
		case 2 : nType = THRESH_TRUNC;		break;	// Threshold Truncated
		case 3 : nType = THRESH_TOZERO;		break;	// Threshold to Zero
		case 4 : nType = THRESH_TOZERO_INV;	break;	// Threshold to Zero Inverted
	}

	threshold ( tGray, tDst, nValue, nMaxBinary, nType );

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
