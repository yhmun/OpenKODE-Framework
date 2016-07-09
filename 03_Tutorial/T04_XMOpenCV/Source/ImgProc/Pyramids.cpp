/* --------------------------------------------------------------------------
 *
 *      File            Pyramids.cpp
 *      Description     Sample code of image pyramids (pyrDown and pyrUp)
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

KDvoid Pyramids ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	// Load the source image
	tSrc = imread ( "/res/image/chicky_512.png", 1 ); 

	if ( nIdx == 0 )
	{
		pyrUp ( tSrc, tDst, Size ( tSrc.cols * 2, tSrc.rows * 2 ) );
	}
	else
	{
		pyrDown ( tSrc, tDst, Size ( tSrc.cols / 2, tSrc.rows / 2 ) );
	}

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
