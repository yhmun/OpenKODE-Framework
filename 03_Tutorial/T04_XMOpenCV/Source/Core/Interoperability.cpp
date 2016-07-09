/* --------------------------------------------------------------------------
 *
 *      File            Interoperability.cpp
 *      Description     Shows how to use cv::Mat and IplImages together 
 *                      (converting back and forth).
 *                      Also contains example for image read, spliting the 
 *                      planes, merging back and color conversion, plus iterating 
 *                      through pixels.      
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

KDvoid Interoperability ( KDint nIdx )
{
	Mat			tSrc;
	Mat			tDst;

	// the newer cvLoadImage alternative, MATLAB-style function
	tSrc = imread ( "/res/image/fruits.jpg" );     

	// convert image to YUV color space. The output image will be created automatically. 
	Mat		tYUV;
	cvtColor ( tSrc, tYUV, CV_BGR2YCrCb ); 

	// Use the STL's vector structure to store multiple Mat objects 
	vector<Mat>		aPlanes;    

	// split the image into separate color planes (Y U V)
	split ( tYUV, aPlanes );  

    // Mat scanning
    // Method 1. process Y plane using an iterator
    MatIterator_<uchar>  it = aPlanes[0].begin<uchar> ( );
	MatIterator_<uchar>  it_end = aPlanes[0].end<uchar> ( );
    for ( ; it != it_end; ++it )
    {
        KDdouble  v = *it * 1.7 + kdRand ( ) % 21 - 10;
        *it = saturate_cast<uchar> ( v * v / 255 );
    }
    
    for ( KDint  y = 0; y < tYUV.rows; y++ )
    {
        // Method 2. process the first chroma plane using pre-stored row pointer.
        uchar*  pPtr = aPlanes [ 1 ].ptr<uchar> ( y );
        for ( KDint  x = 0; x < tYUV.cols; x++ )
        {
            pPtr [ x ] = saturate_cast<uchar> ( ( pPtr [ x ] - 128 ) / 2 + 128 );
            
            // Method 3. process the second chroma plane using individual element access
            uchar&  vXY = aPlanes [ 2 ].at<uchar> ( y, x );
            vXY = saturate_cast<uchar> ( ( vXY - 128 ) / 2 + 128 );
        }
    }

	// now merge the results back
    merge ( aPlanes, tYUV );     

	// and produce the output RGB image
    cvtColor ( tYUV, tDst, CV_YCrCb2BGR );

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
