/* --------------------------------------------------------------------------
 *
 *      File            Morphology.cpp
 *      Description     Erosion and Dilation sample code
 *                      Advanced morphology Transformations sample code
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

KDvoid Morphology ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	Mat		tElement;
	KDint	nType;
	KDint	nSize;	
	KDint	nOperation;
		
	// Load the source image
	tSrc = imread ( nIdx < 5 ? "/res/image/cat.jpg" : "/res/image/baboon.jpg" );

	//nType = MORPH_RECT;
	nType = MORPH_CROSS;
	//nType = MORPH_ELLIPSE;

	switch ( nIdx )
	{
		case 0  : nOperation = MORPH_ERODE;		break;
		case 1  : nOperation = MORPH_DILATE;	break;
		case 2  : nOperation = MORPH_OPEN;		break;
		case 3  : nOperation = MORPH_CLOSE;		break;
		case 4  : nOperation = MORPH_GRADIENT;	break;
		case 5  : nOperation = MORPH_TOPHAT;	break;
		case 6  : nOperation = MORPH_BLACKHAT;	break;
		default : nOperation = 0;
	}

	nSize = 5;
	
	tElement = getStructuringElement ( nType, Size ( 2 * nSize + 1, 2 * nSize + 1 ), Point ( nSize, nSize ) ); 
	
	switch ( nOperation )
	{
		case MORPH_ERODE	:	erode  ( tSrc, tDst, tElement );					break;	// Apply the erosion operation
		case MORPH_DILATE	:	dilate ( tSrc, tDst, tElement );					break;	// Apply the dilation operation
		default				:	morphologyEx ( tSrc, tDst, nOperation, tElement );	break;	// Apply the specified morphology operation
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}