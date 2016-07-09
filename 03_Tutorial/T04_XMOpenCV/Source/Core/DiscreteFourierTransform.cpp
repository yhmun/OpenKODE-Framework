/* --------------------------------------------------------------------------
 *
 *      File            DiscreteFourierTransform.cpp
 *      Description     This program demonstrated the use of the discrete 
 *                      Fourier transform (DFT).
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

KDvoid DiscreteFourier ( KDint nIdx )
{
	Mat			tSrc;
	Mat			tDst;

	// Load the source image
	tSrc = imread ( "/res/image/imageTextR.png", CV_LOAD_IMAGE_GRAYSCALE ); 
    
	// expand input image to optimal size
	Mat		tPadded;								
	KDint	nM = getOptimalDFTSize ( tSrc.rows );
	KDint	nN = getOptimalDFTSize ( tSrc.cols );	

	// on the border add zero values
	copyMakeBorder ( tSrc, tPadded, 0, nM - tSrc.rows, 0, nN - tSrc.cols, BORDER_CONSTANT, Scalar::all ( 0 ) );

	Mat		tPlanes [ ] = { Mat_<KDfloat> ( tPadded ), Mat::zeros ( tPadded.size ( ), CV_32F ) };
	Mat		tComplex;

	// Add to the expanded another plane with zeros
	merge ( tPlanes, 2, tComplex );         

	// this way the result may fit in the source matrix
	dft ( tComplex, tComplex );    

	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))

	// planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	split ( tComplex, tPlanes );                   

	// planes[0] = magnitude  
	magnitude ( tPlanes [ 0 ], tPlanes [ 1 ], tPlanes [ 0 ] );
	tDst = tPlanes [ 0 ];

	// switch to logarithmic scale
	tDst += Scalar::all ( 1 );                    
	log ( tDst, tDst );

	// crop the spectrum, if it has an odd number of rows or columns
	tDst = tDst ( Rect ( 0, 0, tDst.cols & -2, tDst.rows & -2 ) );

	// rearrange the quadrants of Fourier image  so that the origin is at the image center        
	KDint	nCx = tDst.cols / 2;
	KDint	nCy = tDst.rows / 2;

	Mat		tQ0 ( tDst, Rect (   0,   0, nCx, nCy ) ); // Top-Left - Create a ROI per quadrant 
	Mat		tQ1 ( tDst, Rect ( nCx,   0, nCx, nCy ) ); // Top-Right
	Mat		tQ2 ( tDst, Rect (   0, nCy, nCx, nCy ) ); // Bottom-Left
	Mat		tQ3 ( tDst, Rect ( nCx, nCy, nCx, nCy ) ); // Bottom-Right
	
	Mat		tTemp;       

	// swap quadrants (Top-Left with Bottom-Right)
	  tQ0.copyTo ( tTemp );
	  tQ3.copyTo ( tQ0 );
	tTemp.copyTo ( tQ3 );

	// swap quadrant (Top-Right with Bottom-Left)
	  tQ1.copyTo ( tTemp );                    
	  tQ2.copyTo ( tQ1 );
	tTemp.copyTo ( tQ2 );

	// Transform the matrix with float values into a viewable image form (float between values 0 and 1).
	normalize ( tDst, tDst, 0, 1, CV_MINMAX ); 

	g_pController->setFrame ( 1, tSrc );
	g_pController->setFrame ( 2, tDst );
}
