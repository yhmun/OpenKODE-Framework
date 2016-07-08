/* -----------------------------------------------------------------------------------
 *
 *      File            CCDrawingPrimitives.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "actions/CCActionCatmullRom.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"

NS_CC_BEGIN
    
static KDbool		l_bInitialized		 = KD_FALSE;
static CCGLProgram*	l_pShader			 = KD_NULL;
static ccColor4F	l_tColor			 = { 1, 1, 1, 1 };
static GLfloat		l_fPointSize		 = 1;

#if defined ( USE_OPEN_GLES2 )
static KDint		l_nColorLocation	 = -1;
static KDint		l_nPointSizeLocation = -1;
#endif

#ifdef EMSCRIPTEN
static GLuint		l_uBufferObject = 0;
static GLuint		l_uBufferSize = 0;

static KDvoid setGLBufferData ( KDvoid* pBuf, GLuint uBufSize )
{
	if ( l_uBufferSize < uBufSize )
	{
		if ( l_uBufferObject )
		{
			glDeleteBuffers ( 1, &l_uBufferObject );
		}
		glGenBuffers ( 1, &l_uBufferObject );
		l_uBufferSize = uBufSize;

		glBindBuffer ( GL_ARRAY_BUFFER, l_uBufferObject );
		glBufferData ( GL_ARRAY_BUFFER, uBufSize, pBuf, GL_DYNAMIC_DRAW );
	}
	else
	{
		glBindBuffer    ( GL_ARRAY_BUFFER, l_uBufferObject );
		glBufferSubData ( GL_ARRAY_BUFFER, 0, uBufSize, pBuf );
	}
}

#endif // EMSCRIPTEN

static KDvoid draw_setup ( KDbool bUsePointSize = KD_FALSE )
{
    l_pShader->use ( );
    l_pShader->setUniformsForBuiltins ( );

	#if defined ( USE_OPEN_GLES2 )

		l_pShader->setUniformLocationWith4fv ( l_nColorLocation, ( GLfloat* ) &l_tColor.r, 1 );

		if ( bUsePointSize )
		{
			l_pShader->setUniformLocationWith1f  ( l_nPointSizeLocation, l_fPointSize );
		}

	#endif
}

static KDvoid lazy_init ( KDvoid )
{
    if ( !l_bInitialized ) 
	{
        //
        // Position and 1 color passed as a uniform ( to simulate glColor4ub )
        //
        l_pShader = CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_Position_uColor );
		l_pShader->retain ( );

	#if defined ( USE_OPEN_GLES2 )

		l_nColorLocation = glGetUniformLocation ( l_pShader->getProgram ( ), "u_color" );
		CHECK_GL_ERROR_DEBUG ( );
		
		l_nPointSizeLocation = glGetUniformLocation ( l_pShader->getProgram ( ), "u_pointSize" );
		CHECK_GL_ERROR_DEBUG ( );

	#endif

		l_bInitialized = KD_TRUE;
    }
}

// When switching from background to foreground on android, we want the params to be initialized again
KDvoid ccDrawInit ( KDbool bUsePointSize )
{
    lazy_init  ( );
	draw_setup ( bUsePointSize );
}

KDvoid ccDrawFree ( KDvoid )
{
	CC_SAFE_RELEASE ( l_pShader );
	l_bInitialized = KD_FALSE;
}

KDvoid ccDrawPoint ( const CCPoint& tPoint )
{
    ccVertex2F  tP;
    tP.x = tPoint.x;
    tP.y = tPoint.y;
	
    ccDrawInit ( KD_TRUE );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

#ifdef EMSCRIPTEN
	setGLBufferData ( &tP, 8 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &tP );
#endif // EMSCRIPTEN

	glDrawArrays ( GL_POINTS, 0, 1 );

	CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid ccDrawPoints ( const CCPoint* pPoints, KDuint uNumberOfPoints )
{
    ccDrawInit ( KD_TRUE );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

    // XXX: Mac OpenGL error. arrays can't go out of scope before draw is executed
    ccVertex2F* pNewPoints = new ccVertex2F [ uNumberOfPoints ];

    // iPhone and 32-bit machines optimization
    if ( sizeof ( CCPoint ) == sizeof ( ccVertex2F ) )
    {
	#ifdef EMSCRIPTEN
		setGLBufferData ( (KDvoid*) pPoints, uNumberOfPoints * sizeof ( CCPoint ) );
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	#else
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pPoints );
	#endif // EMSCRIPTEN        
    }
    else
    {
        // Mac on 64-bit
        for ( KDuint i = 0; i < uNumberOfPoints;i++ )
		{
            pNewPoints [ i ].x = pPoints [ i ].x;
            pNewPoints [ i ].y = pPoints [ i ].y;
        }

	#ifdef EMSCRIPTEN
		// Suspect Emscripten won't be emitting 64-bit code for a while yet,
		// but want to make sure this continues to work even if they do.
		setGLBufferData ( pNewPoints, uNumberOfPoints * sizeof ( ccVertex2F ) );
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	#else
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pNewPoints );
	#endif // EMSCRIPTEN
    }

    glDrawArrays ( GL_POINTS, 0, (GLsizei) uNumberOfPoints );

    CC_SAFE_DELETE_ARRAY ( pNewPoints );

    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid ccDrawLine ( const CCPoint& tOrigin, const CCPoint& tDestination )
{
	ccVertex2F  tVertices [ 2 ] = 
    {
        {      tOrigin.x,      tOrigin.y },
        { tDestination.x, tDestination.y },
    };

	ccDrawInit ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

#ifdef EMSCRIPTEN
	setGLBufferData ( tVertices, 16 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, tVertices );
#endif // EMSCRIPTEN

    glDrawArrays ( GL_LINES, 0, 2 );

    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid ccDrawRect ( const CCPoint& tOrigin, const CCPoint& tDestination )
{
    ccDrawLine ( ccp ( tOrigin.x	 , tOrigin.y	  ), ccp ( tDestination.x, tOrigin.y	  ) );
    ccDrawLine ( ccp ( tDestination.x, tOrigin.y	  ), ccp ( tDestination.x, tDestination.y ) );
    ccDrawLine ( ccp ( tDestination.x, tDestination.y ), ccp ( tOrigin.x	 , tDestination.y ) );
    ccDrawLine ( ccp ( tOrigin.x	 , tDestination.y ), ccp ( tOrigin.x	 , tOrigin.y	  ) );
}

KDvoid ccDrawSolidRect ( const CCPoint& tOrigin, const CCPoint& tDestination, ccColor4F tColor )
{
	ccColor4F  tRestore = l_tColor;
	ccDrawColor4F ( tColor.r, tColor.g, tColor.b, tColor.a );

	ccDrawSolidRect ( tOrigin, tDestination );

	ccDrawColor4F ( tRestore.r, tRestore.g, tRestore.b, tRestore.a );
}

KDvoid ccDrawSolidRect ( const CCPoint& tOrigin, const CCPoint& tDestination )
{
    CCPoint  aVertices [ ] = 
	{
        tOrigin,
        ccp ( tDestination.x, tOrigin.y ),
        tDestination,
        ccp ( tOrigin.x, tDestination.y )
    };

    ccDrawSolidPoly ( aVertices, 4 );
}

KDvoid ccDrawPoly ( const CCPoint* pPoints, KDuint uNumberOfPoints, KDbool bClosePolygon )
{    
	ccDrawInit ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

    // iPhone and 32-bit machines optimization
    if ( sizeof ( CCPoint ) == sizeof ( ccVertex2F ) )
	{
	#ifdef EMSCRIPTEN
		setGLBufferData ( (KDvoid*) pPoints, uNumberOfPoints * sizeof ( CCPoint ) );
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	#else
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pPoints );
	#endif // EMSCRIPTEN

		if ( bClosePolygon )
		{
			glDrawArrays ( GL_LINE_LOOP, 0, (GLsizei) uNumberOfPoints );
		}
		else
		{
			glDrawArrays ( GL_LINE_STRIP, 0, (GLsizei) uNumberOfPoints );
		}
	}
    else
    {
		// XXX: Mac OpenGL error. arrays can't go out of scope before draw is executed
		ccVertex2F*  pNewPoints = new ccVertex2F [ uNumberOfPoints ];

        // Mac on 64-bit
        for ( KDuint i = 0; i < uNumberOfPoints;i++ ) 
		{
            pNewPoints[i].x = pPoints[i].x;
            pNewPoints[i].y = pPoints[i].y;
        }

	#ifdef EMSCRIPTEN
		setGLBufferData ( (KDvoid*) pNewPoints, uNumberOfPoints * sizeof ( ccVertex2F ) );
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	#else
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pNewPoints );
	#endif // EMSCRIPTEN

		if ( bClosePolygon )
		{
			glDrawArrays ( GL_LINE_LOOP, 0, (GLsizei) uNumberOfPoints );
		}
		else
		{
			glDrawArrays ( GL_LINE_STRIP, 0, (GLsizei) uNumberOfPoints );
		}

		CC_SAFE_DELETE_ARRAY ( pNewPoints );
    }

    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid ccDrawSolidPoly ( const CCPoint* pPoints, KDuint uNumberOfPoints, ccColor4F tColor )
{
	ccColor4F  tRestore = l_tColor;
	ccDrawColor4F ( tColor.r, tColor.g, tColor.b, tColor.a );

	ccDrawSolidPoly ( pPoints, uNumberOfPoints );

	ccDrawColor4F ( tRestore.r, tRestore.g, tRestore.b, tRestore.a );
}

KDvoid ccDrawSolidPoly ( const CCPoint* pPoints, KDuint uNumberOfPoints )
{
    ccDrawInit ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

    // XXX: Mac OpenGL error. arrays can't go out of scope before draw is executed
    ccVertex2F*  pNewPoints = new ccVertex2F [ uNumberOfPoints ];

    // iPhone and 32-bit machines optimization
    if ( sizeof ( CCPoint ) == sizeof ( ccVertex2F ) )
    {
#ifdef EMSCRIPTEN
		setGLBufferData ( (KDvoid*) pPoints, uNumberOfPoints * sizeof ( CCPoint ) );
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pPoints );
#endif // EMSCRIPTEN
    }
    else
    {
        // Mac on 64-bit
        for ( KDuint i = 0; i < uNumberOfPoints; i++ )
        {
            pNewPoints[i] = vertex2 ( pPoints[i].x, pPoints[i].y );
        }

#ifdef EMSCRIPTEN
		setGLBufferData ( (KDvoid*) pNewPoints, uNumberOfPoints * sizeof ( ccVertex2F ) );
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
		ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pNewPoints );
#endif // EMSCRIPTEN
    }    

    glDrawArrays ( GL_TRIANGLE_FAN, 0, (GLsizei) uNumberOfPoints );

    CC_SAFE_DELETE_ARRAY ( pNewPoints );
    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid ccDrawCircle ( const CCPoint& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter, KDfloat fScaleX, KDfloat fScaleY )
{
	KDint  nAdditionalSegment = 1;

	if ( bDrawLineToCenter )
	{
		++nAdditionalSegment;
	}

	const KDfloat	fCoef = 2.0f * KD_PI_F / uSegments;

	KDfloat*		pVertices = (KDfloat*) kdCalloc ( 2 * ( uSegments + 2 ), sizeof ( KDfloat ) );
	if ( !pVertices )
	{
		return;
	}
	
	for ( KDuint i = 0; i <= uSegments; i++ )
	{
		KDfloat		fRads = i * fCoef;
		
		pVertices [ i * 2 + 0 ] = fRadius * kdCosf ( fRads + fAngle ) * fScaleX + tCenter.x;
		pVertices [ i * 2 + 1 ] = fRadius * kdSinf ( fRads + fAngle ) * fScaleY + tCenter.y;
	}
	pVertices [ ( uSegments + 1 ) * 2 + 0 ] = tCenter.x;
	pVertices [ ( uSegments + 1 ) * 2 + 1 ] = tCenter.y;

    ccDrawInit ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

#ifdef EMSCRIPTEN
	setGLBufferData ( (KDvoid*) pVertices, sizeof ( GLfloat ) * 2 * ( uSegments + 2 ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pVertices );
#endif // EMSCRIPTEN

	glDrawArrays ( GL_LINE_STRIP, 0, (GLsizei) uSegments + nAdditionalSegment );

    kdFree ( pVertices ); 
}

KDvoid ccDrawCircle ( const CCPoint& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter )
{
	ccDrawCircle ( tCenter, fRadius, fAngle, uSegments, bDrawLineToCenter, 1.0f, 1.0f );
}

KDvoid ccDrawSolidCircle ( const CCPoint& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter, KDfloat fScaleX, KDfloat fScaleY )
{  
	KDint  nAdditionalSegment = 1;

	if ( bDrawLineToCenter )
	{
		++nAdditionalSegment;
	}

	const KDfloat	fCoef = 2.0f * KD_PI_F / uSegments;

	KDfloat*		pVertices = (KDfloat*) kdCalloc ( 2 * ( uSegments + 2 ), sizeof ( KDfloat ) );
	if ( ! pVertices )
	{
		return;
	}
	
	for ( KDuint i = 0; i <= uSegments; i++ )
	{
		KDfloat  fRads = i * fCoef;
		
		pVertices [ i * 2 + 0 ] = fRadius * kdCosf ( fRads + fAngle ) * fScaleX + tCenter.x;
		pVertices [ i * 2 + 1 ] = fRadius * kdSinf ( fRads + fAngle ) * fScaleY + tCenter.y;
	}
	pVertices[ ( uSegments + 1 ) * 2 + 0 ] = tCenter.x;
	pVertices[ ( uSegments + 1 ) * 2 + 1 ] = tCenter.y;

    ccDrawInit ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

#ifdef EMSCRIPTEN
	setGLBufferData ( (KDvoid*) pVertices, sizeof ( GLfloat ) * 2 * ( uSegments + 2 ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pVertices );
#endif // EMSCRIPTEN

	glDrawArrays ( GL_TRIANGLE_FAN, 0, (GLsizei) uSegments + nAdditionalSegment );

    kdFree ( pVertices ); 
}

KDvoid ccDrawSolidCircle ( const CCPoint& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter )
{
	ccDrawSolidCircle ( tCenter, fRadius, fAngle, uSegments, bDrawLineToCenter, 1.0f, 1.0f );
}

KDvoid ccDrawQuadBezier ( const CCPoint& tOrigin, const CCPoint& tControl, const CCPoint& tDestination, KDuint uSegments )
{ 
	ccVertex2F*  pVertices = new ccVertex2F [ uSegments + 1 ];
	
	KDfloat  t = 0.0f;
	for ( KDuint i = 0; i < uSegments; i++ )
	{
		pVertices [ i ].x = kdPowf ( 1.0f - t, 2.0f ) * tOrigin.x + 2.0f * ( 1.0f - t ) * t * tControl.x + t * t * tDestination.x;
        pVertices [ i ].y = kdPowf ( 1.0f - t, 2.0f ) * tOrigin.y + 2.0f * ( 1.0f - t ) * t * tControl.y + t * t * tDestination.y;
        
		t += 1.0f / uSegments;
	}

	pVertices [ uSegments ].x = tDestination.x;
	pVertices [ uSegments ].y = tDestination.y;

    ccDrawInit ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

#ifdef EMSCRIPTEN
	setGLBufferData ( (KDvoid*) pVertices, ( uSegments + 1 ) * sizeof ( ccVertex2F ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pVertices );
#endif // EMSCRIPTEN

    glDrawArrays ( GL_LINE_STRIP, 0, (GLsizei) uSegments + 1 );
    CC_SAFE_DELETE_ARRAY ( pVertices );

    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid ccDrawCubicBezier ( const CCPoint& tOrigin, const CCPoint& tControl1, const CCPoint& tControl2, const CCPoint& tDestination, KDuint uSegments )
{
	ccVertex2F*  pVertices = new ccVertex2F [ uSegments + 1 ];
	
	KDfloat  t = 0.0f;
	for ( KDuint i = 0; i < uSegments; ++i )
	{
		pVertices [ i ].x = kdPowf ( 1.0f - t, 3.0f ) * tOrigin.x + 3.0f * kdPowf ( 1.0f - t, 2.0f ) * t * tControl1.x + 3.0f * ( 1.0f - t ) * t * t * tControl2.x + t * t * t * tDestination.x;
		pVertices [ i ].y = kdPowf ( 1.0f - t, 3.0f ) * tOrigin.y + 3.0f * kdPowf ( 1.0f - t, 2.0f ) * t * tControl1.y + 3.0f * ( 1.0f - t ) * t * t * tControl2.y + t * t * t * tDestination.y;
		t += 1.0f / uSegments;
	}

	pVertices [ uSegments ].x = tDestination.x;
	pVertices [ uSegments ].y = tDestination.y;

    ccDrawInit ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

#ifdef EMSCRIPTEN
	setGLBufferData ( (KDvoid*) pVertices, ( uSegments + 1 ) * sizeof ( ccVertex2F ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pVertices );
#endif // EMSCRIPTEN

    glDrawArrays ( GL_LINE_STRIP, 0, (GLsizei) uSegments + 1 );
    CC_SAFE_DELETE_ARRAY ( pVertices );

    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid ccDrawCatmullRom ( CCPointArray* pArrayOfControlPoints, KDuint uSegments )
{
    ccDrawCardinalSpline ( pArrayOfControlPoints, 0.5f, uSegments );
}

KDvoid ccDrawCardinalSpline ( CCPointArray* pConfig, KDfloat fTension, KDuint uSegments )
{
    ccVertex2F*  pVertices = new ccVertex2F [ uSegments + 1 ];

    KDuint   p;
    KDfloat  lt;
    KDfloat  deltaT = 1.0f / pConfig->count ( );

    for ( KDuint i = 0; i < uSegments + 1; i++ ) 
	{
        KDfloat  dt = (KDfloat) i / uSegments;

        // border
        if ( dt == 1 ) 
		{
             p = pConfig->count ( ) - 1;
            lt = 1;
        }
		else
		{
             p = (KDuint) ( dt / deltaT );
            lt = ( dt - deltaT * (KDfloat) p ) / deltaT;
        }

        // Interpolate
        CCPoint  tP0 = pConfig->getControlPointAtIndex ( p - 1 );
        CCPoint  tP1 = pConfig->getControlPointAtIndex ( p + 0 );
        CCPoint  tP2 = pConfig->getControlPointAtIndex ( p + 1 );
        CCPoint  tP3 = pConfig->getControlPointAtIndex ( p + 2 );

        CCPoint  tNewPos = ccCardinalSplineAt ( tP0, tP1, tP2, tP3, fTension, lt );
        pVertices [ i ].x = tNewPos.x;
        pVertices [ i ].y = tNewPos.y;
    }

	ccDrawInit ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

#ifdef EMSCRIPTEN
	setGLBufferData ( (KDvoid*) pVertices, ( uSegments + 1 ) * sizeof ( ccVertex2F ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, pVertices );
#endif // EMSCRIPTEN

    glDrawArrays ( GL_LINE_STRIP, 0, (GLsizei) uSegments + 1 );

    CC_SAFE_DELETE_ARRAY ( pVertices );
    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid ccDrawColor4F ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    l_tColor.r = r;
    l_tColor.g = g;
    l_tColor.b = b;
    l_tColor.a = a;

#if !defined ( USE_OPEN_GLES2 )
	glColor4f ( l_tColor.r, l_tColor.g, l_tColor.b, l_tColor.a );
#endif
}

KDvoid ccPointSize ( GLfloat fPointSize )
{
    l_fPointSize = fPointSize * CC_CONTENT_SCALE_FACTOR ( );

#if !defined ( USE_OPEN_GLES2 )
	glPointSize ( l_fPointSize );
#endif
}

KDvoid ccDrawColor4B ( GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
    l_tColor.r = r / 255.0f;
    l_tColor.g = g / 255.0f;
    l_tColor.b = b / 255.0f;
    l_tColor.a = a / 255.0f;

#if !defined ( USE_OPEN_GLES2 )
	glColor4f ( l_tColor.r, l_tColor.g, l_tColor.b, l_tColor.a );
#endif
}

KDvoid ccLineWidth ( GLfloat fLineWidth )
{
#if !defined ( USE_OPEN_GLES2 )    
	if ( fLineWidth == 1.0f )
	{
		glEnable ( GL_LINE_SMOOTH );
	}
	else
	{
		glDisable ( GL_LINE_SMOOTH );
	}
#endif

	glLineWidth ( fLineWidth );
}

NS_CC_END