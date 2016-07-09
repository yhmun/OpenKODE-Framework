/* --------------------------------------------------------------------------
 *
 *      File            ObjectDetection.cpp
 *      Description     A simplified version of facedetect.cpp, show how to 
 *                      load a cascade classifier and how to find objects
 *                      (Face + eyes) in a video stream
 *      Author          A. Huaman, Y.H Mun
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

static CascadeClassifier*	l_pFaceCascade	= KD_NULL;
static CascadeClassifier*	l_pEyesCascade	= KD_NULL;
static KDuint               l_nMsec         = 0;
static VideoCapture			l_tCapture;
static std::vector<Rect>	l_aFaces;
static std::vector<Rect>	l_aEyes;

KDvoid ObjectDetectionExit ( KDvoid )
{
	if ( l_pFaceCascade )
	{
		delete l_pFaceCascade;
		l_pFaceCascade = KD_NULL;
	}

	if ( l_pEyesCascade )
	{
		delete l_pEyesCascade;
		l_pEyesCascade = KD_NULL;
	}

	if ( l_tCapture.isOpened ( ) )
	{
		l_tCapture.release ( );
	}
}

KDvoid ObjectDetectionDraw ( KDint nMsec )
{
	if ( !l_tCapture.isOpened ( ) )
	{
		return;
	}

	Mat		tFrame;

	// 3. Apply the classifier to the frame
	l_tCapture >> tFrame;
	if ( tFrame.empty ( ) )
	{
		return;
	}
   
    KDbool  bDetect = KD_FALSE;
    if ( nMsec - l_nMsec > 2000 )
    {
        bDetect = KD_TRUE;
    }

    // Detect faces
    Mat     tFrameGray;
    
    if ( bDetect )
    {
        cvtColor ( tFrame, tFrameGray, CV_BGR2GRAY );
        equalizeHist ( tFrameGray, tFrameGray );
        
        l_aFaces.clear ( );
        l_pFaceCascade->detectMultiScale ( tFrameGray, l_aFaces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size ( 30, 30 ) );
    }

    for ( KDuint i = 0; i < l_aFaces.size ( ); i++ )
    {
        Point   tFaceCenter ( l_aFaces [i].x + l_aFaces [i].width / 2, l_aFaces [i].y + l_aFaces [i].height / 2 );
        ellipse ( tFrame, tFaceCenter, Size ( l_aFaces [i].width / 2, l_aFaces [i].height / 2 ), 0, 0, 360, Scalar ( 255, 0, 255 ), 2, 8, 0 );

        // In each face, detect eyes
        if ( bDetect )
        {
             Mat     tFaceROI = tFrameGray ( l_aFaces [i] );
            
            l_aEyes.clear ( );
            l_pEyesCascade->detectMultiScale ( tFaceROI, l_aEyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size ( 30, 30 ) );
        }
        
        for( KDuint j = 0; j < l_aEyes.size ( ); j++ )
        {
            Point	tEyeCenter ( l_aFaces [i].x + l_aEyes [j].x + l_aEyes [j].width / 2, l_aFaces [i].y + l_aEyes [j].y + l_aEyes [j].height / 2 ); 
            KDint	nRadius = cvRound ( ( l_aEyes [j].width + l_aEyes [i].height ) * 0.25 );
            
            circle ( tFrame, tEyeCenter, nRadius, Scalar ( 255, 0, 0 ), 3, 8, 0 );
        }
    } 

	// Show what you got
	g_pController->setFrame ( 0, tFrame );

    if ( bDetect )
    {
        l_nMsec = KD_GET_UST2MSEC;
    }
}

KDvoid ObjectDetection ( KDint nIdx )
{
	// 1. Load the cascades
	l_pFaceCascade = new CascadeClassifier ( );
	if ( l_pFaceCascade && !l_pFaceCascade->load ( "/res/haarcascades/haarcascade_frontalface_alt.xml" ) )
	{
		kdAssertion ( "Error loading face cascade." );
	}

	l_pEyesCascade = new CascadeClassifier ( );
	if ( l_pEyesCascade && !l_pEyesCascade->load ( "/res/haarcascades/haarcascade_eye_tree_eyeglasses.xml" ) )
	{
		kdAssertion ( "Error loading eyes cascade." );
	}

	KDint  nDevice = -1;
#if defined ( ANDROID )
    nDevice = CV_CAP_ANDROID + 0;
#elif defined ( __APPLE__ )
    nDevice = CV_CAP_AVFOUNDATION + 0;
#endif
    
	// 2. Read the video stream
	l_tCapture = VideoCapture ( nDevice );
	l_tCapture.set ( CV_CAP_PROP_FRAME_WIDTH , 640 );
	l_tCapture.set ( CV_CAP_PROP_FRAME_HEIGHT, 480 );

	g_pController->setDrawFunc ( ObjectDetectionDraw );
	g_pController->setExitFunc ( ObjectDetectionExit );
}



