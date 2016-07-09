/* --------------------------------------------------------------------------
 *
 *      File            Main.cpp
 *      Description     Application Entrance.
 *      Author          Y.H Mun
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
#include "Controller.h"

KDvoid DisplayImage				( KDint nIdx );
KDvoid MaskOperations			( KDint nIdx );
KDvoid AddingImages				( KDint nIdx );
KDvoid BasicLinearTransforms	( KDint nIdx );
KDvoid BasicDrawing1			( KDint nIdx );
KDvoid BasicDrawing2			( KDint nIdx );
KDvoid DiscreteFourier			( KDint nIdx );
KDvoid FileInputOutput			( KDint nIdx );
KDvoid Interoperability			( KDint nIdx );
KDvoid Smoothing				( KDint nIdx );
KDvoid Morphology				( KDint nIdx );
KDvoid Pyramids					( KDint nIdx );
KDvoid Threshold				( KDint nIdx );
KDvoid Filter2D					( KDint nIdx );
KDvoid CopyMakeBorder			( KDint nIdx );
KDvoid AddingImages				( KDint nIdx );
KDvoid Sobel					( KDint nIdx );
KDvoid Laplace					( KDint nIdx );
KDvoid CannyDetector			( KDint nIdx );
KDvoid HoughLines				( KDint nIdx );
KDvoid HoughCircle				( KDint nIdx );
KDvoid Remap					( KDint nIdx );
KDvoid GeometricTransforms		( KDint nIdx );
KDvoid EqualizeHist				( KDint nIdx );
KDvoid CalcHist					( KDint nIdx );
KDvoid CompareHist				( KDint nIdx );
KDvoid CalcBackProject1			( KDint nIdx );
KDvoid CalcBackProject2			( KDint nIdx );
KDvoid MatchTemplate			( KDint nIdx );
KDvoid FindContours				( KDint nIdx );
KDvoid Hull						( KDint nIdx );
KDvoid GeneralContours			( KDint nIdx );
KDvoid ImageMoments				( KDint nIdx );
KDvoid PointPolygonTest			( KDint nIdx );
KDvoid CornerHarris				( KDint nIdx );
KDvoid GoodFeaturesToTrack		( KDint nIdx );
KDvoid CornerDetector			( KDint nIdx );
KDvoid CornerSubPix				( KDint nIdx );
KDvoid SURF_Detector			( KDint nIdx );
KDvoid SURF_Descriptor			( KDint nIdx );
KDvoid SURF_FlannMatcher		( KDint nIdx );
KDvoid SURF_Homography			( KDint nIdx );
KDvoid SBMSample				( KDint nIdx );
KDvoid IntroductionToSVM		( KDint nIdx );
KDvoid NonLinearSVMs			( KDint nIdx );
KDvoid ObjectDetection			( KDint nIdx );

struct Example  
{
	PFNExample     func;
	KDint          index;
	const KDchar*  title;
	const KDchar*  subtitle;
};

static struct Example  l_aExamples [] =
{	
	{ DisplayImage			, 0, "Introduction"				, "Load, Write and Display"				},
	{ MaskOperations		, 0, "Mask Operations"			, "Sharpen Using Custom"				},
	{ MaskOperations		, 1, "Mask Operations"			, "Sharpen Using Built in Filter2D"		},
	{ AddingImages			, 0, "Adding two images"		, "Add two images using addWeighted"	},
	{ BasicLinearTransforms	, 0, "Basic Linear Transforms"	, "Simple contrast control"				},
	{ BasicDrawing1			, 0, "Basic Drawing"			, "Atom and Rook"						},
	{ BasicDrawing2			, 0, "Basic Drawing"			, "Random Drawing"						},
	{ DiscreteFourier		, 0, "Discrete Fourier"			, "Spectrum magnitude"					},
	{ FileInputOutput		, 0, "File Input Output"		, "XML and YAML"						},
	{ Interoperability		, 0, "Interoperability"			, "Using YUV"							},
	{ Smoothing				, 0, "Smoothing"				, "Homogeneous blur"					},
	{ Smoothing				, 1, "Smoothing"				, "Gaussian blur"						},
	{ Smoothing				, 2, "Smoothing"				, "Median blur"							},
	{ Smoothing				, 3, "Smoothing"				, "Bilateral Filter"					},
	{ Morphology			, 0, "Morphology"				, "Erosion"								},
	{ Morphology			, 1, "Morphology"				, "Dilation"							},
	{ Morphology			, 2, "Morphology"				, "Opening"								},
	{ Morphology			, 3, "Morphology"				, "Closing"								},
	{ Morphology			, 4, "Morphology"				, "Gradient"							},
	{ Morphology			, 5, "Morphology"				, "Top Hat"								},
	{ Morphology			, 6, "Morphology"				, "Black Hat"							},
	{ Pyramids				, 0, "Pyramids"					, "Zoom In"								},
	{ Pyramids				, 1, "Pyramids"					, "Zoom Out"							},
	{ Threshold				, 0, "Threshold"				, "Binary"								},
	{ Threshold				, 1, "Threshold"				, "Binary Inverted"						},
	{ Threshold				, 2, "Threshold"				, "Threshold Truncated"					},
	{ Threshold				, 3, "Threshold"				, "Threshold to Zero"					},
	{ Threshold				, 4, "Threshold"				, "Threshold to Zero Inverted"			},
	{ Filter2D				, 0, "Filter2D"					, "Custom linear"						},
	{ CopyMakeBorder		, 0, "Copy Make Border"			, "Constant"							},
	{ CopyMakeBorder		, 1, "Copy Make Border"			, "Replicate"							},
	{ Sobel					, 0, "Sobel"					, "Simple Edge Detector"				},
	{ Laplace				, 0, "Laplace"					, "Laplace Operator"					},
	{ CannyDetector			, 0, "Canny Detector"			, "Edge Map"							},
	{ HoughLines			, 0, "Hough Lines"				, "Standard"							},
	{ HoughLines			, 1, "Hough Lines"				, "Probabilistic"						},
	{ HoughCircle			, 0, "Hough Circle"				, "Transform"							},
	{ Remap					, 0, "Remap"					, "Resize & Centering"					},
	{ Remap					, 1, "Remap"					, "Reflecting in Y direction"			},
	{ Remap					, 2, "Remap"					, "Reflecting in X direction"			},
	{ Remap					, 3, "Remap"					, "Reflecting in Both direction"		},
	{ GeometricTransforms	, 0, "Geometric Transforms"		, "Warp"								},
	{ GeometricTransforms	, 1, "Geometric Transforms"		, "Warp + Rotate"						},
	{ EqualizeHist			, 0, "Histogram"				, "Equalized Image"						},
	{ CalcHist				, 0, "Histogram"				, "Calc Histogram"						},	
	{ CompareHist			, 0, "Histogram"				, "Compare Histogram"					},	
	{ CalcBackProject1		, 0, "Back Project"				, "Mix Channel"							},	
	{ CalcBackProject2		, 0, "Back Project"				, "Flood Fill"							},	
	{ MatchTemplate			, 0, "Match Template"			, "TM_SQDIFF"							},	
	{ MatchTemplate			, 1, "Match Template"			, "TM_SQDIFF_NORMED"					},	
	{ MatchTemplate			, 2, "Match Template"			, "TM_CCORR"							},	
	{ MatchTemplate			, 3, "Match Template"			, "TM_CCORR_NORMED"						},	
	{ MatchTemplate			, 4, "Match Template"			, "TM_CCOEFF"							},	
	{ MatchTemplate			, 5, "Match Template"			, "TM_CCOEFF_NORMED"					},	
	{ FindContours			, 0, "Find Contours"			, "findContours + drawContours"			},	
	{ Hull					, 0, "Convex Hull"				, "convexHull"							},	
	{ GeneralContours		, 0, "General Contours"			, "boundingRect + minEnclosingCircle"	},	
	{ GeneralContours		, 1, "General Contours"			, "minAreaRect + fitEllipse"			},	
	{ ImageMoments			, 0, "Image Moments"			, "moments + contourArea + arcLength"	},	
	{ PointPolygonTest		, 0, "Point Polygon Test"		, "pointPolygonTest"					},
	{ CornerHarris			, 0, "Corners detected"			, "Harris corner detector"				},
	{ GoodFeaturesToTrack	, 0, "Corners detected"			, "Shi-Tomasi corner detector"			},
	{ CornerDetector		, 0, "Corners detected"			, "Custom corner detector"				},
	{ CornerSubPix			, 0, "Corners detected"			, "Corners location in subpixeles"		},
	{ SURF_Detector			, 0, "SURF Detector"			, "Keypoint Detection & Drawing"		},
	{ SURF_Descriptor		, 0, "SURF Detector"			, "BruteForce Matcher"					},
	{ SURF_FlannMatcher		, 0, "SURF Detector"			, "Flann Matcher"						},
	{ SURF_Homography		, 0, "SURF Detector"			, "Find Homography"						},
	{ SBMSample				, 0, "SBM Sample"				, "Get a disparity map of two images"	},
	{ IntroductionToSVM		, 0, "Support Vector Machines"	, "Simple Example"						},
//	{ NonLinearSVMs			, 0, "Support Vector Machines"	, "SVM for Non-Linear Training Data"	},
	{ ObjectDetection		, 0, "Object Detection"			, "Face Detect"							},
};

Controller*   g_pController = KD_NULL;

KDbool Controller::init ( KDvoid )
{
	g_pController = this;

	CCLayer::init ( );

	const CCSize&  tSize = CCDirector::sharedDirector ( )->getWinSize ( );	

	CCMenu*  pMenu = CCMenu::create
	(
		CCMenuItemImage::create 
        (
            "image/btn_prev_0.png"	 , 
            "image/btn_prev_1.png"	 ,
            this, menu_selector ( Controller::onPrev ) 
        ),
		CCMenuItemImage::create 
        ( 
            "image/btn_restart_0.png", 
            "image/btn_restart_1.png", 
            this, menu_selector ( Controller::onRestart )
        ),
		CCMenuItemImage::create 
        (
            "image/btn_next_0.png"	 , 
            "image/btn_next_1.png"	 , 
            this, menu_selector ( Controller::onNext ) 
        ),				
		KD_NULL
	);
	do
	{
		CC_BREAK_IF ( !pMenu );

		pMenu->setPosition ( ccp ( tSize.cx / 2, 35 ) );
		pMenu->alignItemsHorizontally ( 50 );

		this->addChild ( pMenu );

	} while ( 0 );	

	m_pTitle = CCLabelTTF::create ( "", "font/Schwarzwald Regular.ttf", 34, CCSize ( tSize.cx - 20.f, 0 ), kCCTextAlignmentCenter );
	do
	{
		CC_BREAK_IF ( !m_pTitle );

		m_pTitle->setPosition ( ccp ( tSize.cx / 2, tSize.cy - 50 ) );
		m_pTitle->setColor ( ccGREEN );

		this->addChild ( m_pTitle );
		
	} while ( 0 );	

	m_pSubTitle = CCLabelTTF::create ( "", "font/arial.ttf", 26, CCSize ( tSize.cx - 20.f, 0 ), kCCTextAlignmentCenter );
	do
	{
		CC_BREAK_IF ( !m_pSubTitle );

		m_pSubTitle->setPosition ( ccp ( tSize.cx / 2, tSize.cy - 100 ) );
		m_pSubTitle->setColor ( ccYELLOW );

		this->addChild ( m_pSubTitle );

	} while ( 0 );	

	m_pMessage = CCLabelTTF::create ( "", "font/arial.ttf", 14, CCSize ( tSize.cx - 20.f, 190 ), kCCTextAlignmentLeft );
	do
	{
		CC_BREAK_IF ( !m_pMessage );

		m_pMessage->setAnchorPoint ( ccp ( 0, 0 ) );
		m_pMessage->setPosition ( ccp ( 10.f, 60.f ) );
		m_pMessage->setColor ( ccRED );

		this->addChild ( m_pMessage );
		
	} while ( 0 );	

    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTexture ) );
    
	glGenTextures     ( 1, &m_uTexture );
	ccGLBindTexture2D ( m_uTexture );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );	

	m_nIndex    = 0;
	m_pDrawFunc = KD_NULL;
	m_pExitFunc = KD_NULL;

	setExample ( 0 );

	return KD_TRUE;
}

KDvoid Controller::onExit ( KDvoid )
{
	if ( m_pExitFunc )
	{
		m_pExitFunc ( );
	}

	CCLayer::onExit ( );
}

KDvoid Controller::draw ( KDvoid )
{
    CC_NODE_DRAW_SETUP ( );
    
	GLfloat  fVertices [ ] = 
	{
		   0,  512, 
		1024,  512, 
		   0,    0, 
		1024,    0, 
	};

	GLfloat  fCoords [ ] =
	{
		   0,  0,
		   1,  0,
		   0,  1, 
		   1,  1, 
	};

	ccGLBindTexture2D ( m_uTexture );
    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );

	if ( m_pDrawFunc )
	{
		m_pDrawFunc ( KD_GET_UST2MSEC );
	}

	ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT, GL_FALSE, 0, fVertices );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, fCoords );	

	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

	ccGLBindTexture2D ( 0 );
}

KDvoid Controller::onRestart ( CCObject* pSender )
{
	setExample ( m_nIndex );
}

KDvoid Controller::onPrev ( CCObject* pSender )
{
	KDint  nMax = sizeof ( l_aExamples ) / sizeof ( struct Example );

	m_nIndex--;
	if ( m_nIndex < 0 )
	{
		m_nIndex = nMax - 1;
	}

	setExample ( m_nIndex );
}

KDvoid Controller::onNext ( CCObject* pSender )
{
	KDint  nMax = sizeof ( l_aExamples ) / sizeof ( struct Example );

	m_nIndex++;
	if ( m_nIndex == nMax )
	{
		m_nIndex = 0;
	}

	setExample ( m_nIndex );
}

KDvoid Controller::setExample ( KDint nIndex )
{
	KDubyte*  pPixels = (KDubyte*) kdCalloc ( 1024 * 512 * 3, 1 );

	ccGLBindTexture2D ( m_uTexture );
	glTexImage2D  ( GL_TEXTURE_2D, 0, GL_RGB, 1024, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, pPixels );

	kdFree ( pPixels );

	if ( m_pExitFunc )
	{
		m_pExitFunc ( );
	}

	m_nIndex    = nIndex;
	m_pDrawFunc = KD_NULL;
	m_pExitFunc = KD_NULL;

	m_pTitle->setString ( l_aExamples [ nIndex ].title );
	m_pSubTitle->setString ( l_aExamples [ nIndex ].subtitle );
	m_pMessage->setString ( "" );

	l_aExamples [ nIndex ].func ( l_aExamples [ nIndex ].index );
}

KDvoid Controller::setFrame ( KDint nLocation, Mat& tImage )
{
	if ( tImage.empty ( ) )
	{
		return;
	}

	KDint  nCols = KD_MIN ( nLocation == 0 ? 1024 : 512, tImage.cols );
	KDint  nRows = KD_MIN ( 512, tImage.rows );
	
	KDint  nStartX = nLocation == 0 ? ( 1024 - nCols ) / 2 : nLocation == 1 ? ( 512 - nCols ) / 2 : ( 512 - nCols ) / 2 + 512;
	KDint  nStartY = ( 512 - nRows ) / 2;

	KDubyte*  pDst = (KDubyte*) kdMalloc ( nRows * nCols * 3 );
	uchar*    pSrc = KD_NULL;
	KDint     nOff = 0;

	if ( tImage.channels ( ) == 1 )
	{
		if ( tImage.depth ( ) == 5 )
		{
			for ( KDint  y = 0; y < nRows; ++y )
			{
				for ( KDint  x = 0; x < nCols; ++x )
				{
					pSrc = tImage.ptr<uchar> ( y, x );
					nOff = ( y * nCols + x ) * 3;

					pDst [ nOff + 2 ] = pDst [ nOff + 1 ] = pDst [ nOff + 0 ] = (KDubyte) ( *((KDfloat*) pSrc) * 255 );
				}
			}	
		}
		else
		{
			for ( KDint  y = 0; y < nRows; ++y )
			{
				for ( KDint  x = 0; x < nCols; ++x )
				{
					pSrc = tImage.ptr<uchar> ( y, x );
					nOff = ( y * nCols + x ) * 3;

					pDst [ nOff + 2 ] = pDst [ nOff + 1 ] = pDst [ nOff + 0 ] = pSrc [ 0 ];
				}
			}	
		}
	}
	else
	{
		for ( KDint  y = 0; y < nRows;  ++y )
		{
			for ( KDint  x = 0; x < nCols; ++x )
			{
				pSrc = tImage.ptr<uchar> ( y, x );
				nOff = ( y * nCols + x ) * 3;

				pDst [ nOff + 2 ] = pSrc [ 0 ];
				pDst [ nOff + 1 ] = pSrc [ 1 ];
				pDst [ nOff + 0 ] = pSrc [ 2 ];
			}
		}	
	}

	ccGLBindTexture2D ( m_uTexture );
	glTexSubImage2D ( GL_TEXTURE_2D, 0, nStartX, nStartY, nCols, nRows, GL_RGB, GL_UNSIGNED_BYTE, pDst );

	kdFree ( pDst );
}

KDvoid Controller::setMessage ( const KDchar* szMsg )
{
	m_pMessage->setString ( szMsg );
	kdLogMessage ( szMsg );
}

KDvoid Controller::setDrawFunc ( PFNDraw func )
{
	m_pDrawFunc = func;
}

KDvoid Controller::setExitFunc ( PFNExit func )
{
	m_pExitFunc = func;
}
