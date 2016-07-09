/* --------------------------------------------------------------------------
 *
 *      File            MaskOperations.cpp
 *      Description     This program shows how to filter images with mask: 
 *                      the write it yourself and the filter2d way.
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

KDvoid MaskOperations ( KDint nIdx )
{
	Mat		tSrc, tDst;

	// Load the source image
	tSrc = imread ( "/res/image/fruits.jpg" ); 

	if ( nIdx == 0 )
	{
		const KDint  nChannels = tSrc.channels ( );
		tDst.create ( tSrc.size ( ), tSrc.type ( ) );
	    
		
		for ( KDint j = 1 ; j < tSrc.rows - 1; ++j )
		{
			const uchar*  pPrev = tSrc.ptr<uchar> ( j - 1 );
			const uchar*  pCurr = tSrc.ptr<uchar> ( j     );
			const uchar*  pNext = tSrc.ptr<uchar> ( j + 1 );

			uchar*  pOutput = tDst.ptr<uchar> ( j );

			for ( KDint i = nChannels; i < nChannels * ( tSrc.cols - 1 ); ++i )
			{
				*pOutput++ = saturate_cast<uchar> ( 5 * pCurr [ i ] -pCurr [ i - nChannels ] - pCurr [ i + nChannels ] - pPrev [ i ] - pNext [ i ] );
			}
		}

		tDst.row ( 0             ).setTo ( Scalar ( 0 ) );
		tDst.row ( tDst.rows - 1 ).setTo ( Scalar ( 0 ) );
		tDst.col ( 0             ).setTo ( Scalar ( 0 ) );
		tDst.col ( tDst.cols - 1 ).setTo ( Scalar ( 0 ) );
	}
	else
	{
		Mat  tKern = ( Mat_<char>(3,3) <<  0, -1,  0, 
					 				      -1,  5, -1,
										   0, -1,  0 );

		filter2D ( tSrc, tDst, tSrc.depth ( ), tKern ); 
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
