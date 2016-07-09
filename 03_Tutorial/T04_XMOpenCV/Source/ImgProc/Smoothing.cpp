/* --------------------------------------------------------------------------
 *
 *      File            Smoothing.cpp
 *      Description     Sample code for simple filters
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

KDvoid Smoothing ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	KDint	MAX_KERNEL_LENGTH = 9;	// 31;
	KDint	i;

	// Load the source image
	tSrc = imread ( "/res/image/lena.jpg", 1 ); 
	tDst = tSrc.clone ( );

	for ( i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
	{
		switch ( nIdx )
		{
			case 0 : blur ( tSrc, tDst, Size ( i, i ), Point ( -1, -1 ) );	break; // Applying Homogeneous blur 
			case 1 : GaussianBlur ( tSrc, tDst, Size ( i, i ), 0, 0 );		break; // Applying Gaussian blur 
			case 2 : medianBlur ( tSrc, tDst, i );							break; // Applying Median blur
			case 3 : bilateralFilter ( tSrc, tDst, i, i * 2, i / 2 );		break; // Applying Bilateral Filter
		}
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
