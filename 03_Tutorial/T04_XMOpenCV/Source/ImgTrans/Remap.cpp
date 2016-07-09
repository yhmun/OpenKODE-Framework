/* --------------------------------------------------------------------------
 *
 *      File            Remap.cpp
 *      Description     Demo code for Remap
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

KDvoid Remap ( KDint nIdx )
{
	Mat		tSrc;
	Mat		tDst;

	Mat		tMapX;
	Mat		tMapY;
	KDint   i, j;

	// Load the image
	tSrc = imread ( "/res/image/lena.jpg" ); 

	// Create dst, map_x and map_y with the same size as src:
	 tDst.create ( tSrc.size ( ), tSrc.type ( ) );
	tMapX.create ( tSrc.size ( ), CV_32FC1 );
	tMapY.create ( tSrc.size ( ), CV_32FC1 );

	for ( j = 0; j < tSrc.rows; j++ )
	{ 
		for ( i = 0; i < tSrc.cols; i++ )
		{
			switch( nIdx )
			{   
				case 0 :
					if ( i > tSrc.cols * 0.25 && i < tSrc.cols * 0.75 && j > tSrc.rows * 0.25 && j < tSrc.rows * 0.75 )
					{
						tMapX.at<KDfloat> ( j, i ) = 2 * ( i - tSrc.cols * 0.25f ) + 0.5f;
						tMapY.at<KDfloat> ( j, i ) = 2 * ( j - tSrc.rows * 0.25f ) + 0.5f;
					}
					else
					{
						tMapX.at<KDfloat> ( j, i ) = 0;
						tMapY.at<KDfloat> ( j, i ) = 0;
					}
					break;

				case 1 :
					tMapX.at<KDfloat> ( j, i ) = i * 1.f;
					tMapY.at<KDfloat> ( j, i ) = tSrc.rows - j * 1.f;
					break;

				case 2 :
					tMapX.at<KDfloat> ( j, i ) = tSrc.cols - i * 1.f;
					tMapY.at<KDfloat> ( j, i ) = j * 1.f;
					break;

				case 3 :
					tMapX.at<KDfloat> ( j, i ) = tSrc.cols - i * 1.f;
					tMapY.at<KDfloat> ( j, i ) = tSrc.rows - j * 1.f;
					break;
			} 
		}
	}

	remap ( tSrc, tDst, tMapX, tMapY, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar ( 0, 0, 0 ) ); 

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
