/* -----------------------------------------------------------------------------------
 *
 *      File            CCDrawingPrimitives.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

/**
 *
 *	IMPORTANT       IMPORTANT        IMPORTANT        IMPORTANT
 *
 *
 *	LEGACY FUNCTIONS
 *
 *	USE DrawNode instead
 *
 */


#ifndef __CCDrawingPrimitives_h__
#define __CCDrawingPrimitives_h__
    
#include "../../ccTypes.h"
#include "../../base/CCGeometry.h"    // for Point

/**
 *	@file
 *	Drawing OpenGL ES primitives.
 *		- drawPoint, drawPoints
 *		- drawLine
 *		- drawRect, drawSolidRect
 *		- drawPoly, drawSolidPoly
 *		- drawCircle
 *		- drawQuadBezier
 *		- drawCubicBezier
 *		- drawCatmullRom
 *		- drawCardinalSpline
 *
 *	You can change the color, point size, width by calling:
 *		- drawColor4B(), drawColor4F()
 *		- ccPointSize()
 *		- glLineWidth()
 * 
 *	@warning These functions draws the Line, Point, Polygon, immediately. They aren't batched. If you are going to make a game that depends on these primitives, I suggest creating a batch. Instead you should use DrawNode
 *
 */

NS_CC_BEGIN

/**
 *	@addtogroup global
 *	@{
 */

class PointArray;

namespace DrawPrimitives
{
    /** Initializes the drawing primitives */
    KDvoid		init ( KDvoid );

    /** Frees allocated resources by the drawing primitives */
    KDvoid		free ( KDvoid );

    /** draws a point given x and y coordinate measured in points */
    KDvoid		drawPoint( const Point& tPoint );

    /** 
	 *	draws an array of points.
     *	@since v0.7.2
     */
    KDvoid		drawPoints ( const Point* pPoints, KDuint uNumberOfPoints );

    /** draws a line given the origin and destination point measured in points */
    KDvoid		drawLine ( const Point& tOrigin, const Point& tDestination );

    /** draws a rectangle given the origin and destination point measured in points. */
    KDvoid		drawRect ( Point tOrigin, Point tDestination );

    /** 
	 *	draws a solid rectangle given the origin and destination point measured in points.
     *	@since 1.1
     */
    KDvoid		drawSolidRect ( const Point& tOrigin, const Point& tDestination, const Color4F& tColor );

	KDvoid		drawSolidRect ( const Point& tOrigin, const Point& tDestination );

    /** 
	 *	draws a polygon given a pointer to Point coordinates and the number of vertices measured in points.
     *	The polygon can be closed or open
     */
    KDvoid		drawPoly ( const Point* pVertices, KDuint uNumberOfPoints, KDbool bClosePolygon );

    /**
	 *	draws a solid polygon given a pointer to CGPoint coordinates, the number of vertices measured in points, and a color.
     */
    KDvoid		drawSolidPoly ( const Point* pPoli, KDuint uNumberOfPoints, Color4F tColor );

	KDvoid		drawSolidPoly ( const Point* pPoli, KDuint uNumberOfPoints );

    /** draws a circle given the center, radius and number of segments. */
    KDvoid		drawCircle ( const Point& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter, KDfloat fScaleX, KDfloat fScaleY );
    KDvoid		drawCircle ( const Point& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter );

    /** draws a solid circle given the center, radius and number of segments. */
    KDvoid		drawSolidCircle ( const Point& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDfloat fScaleX, KDfloat fScaleY );
    KDvoid		drawSolidCircle ( const Point& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments );

    /** 
	 *	draws a quad bezier path
     *	@warning This function could be pretty slow. Use it only for debugging purposes.
     *	@since v0.8
     */
    KDvoid		drawQuadBezier ( const Point& tOrigin, const Point& tControl, const Point& tDestination, KDuint uSegments );

    /**
	 *	draws a cubic bezier path
     *	@warning This function could be pretty slow. Use it only for debugging purposes.
     *	@since v0.8
     */
    KDvoid		drawCubicBezier ( const Point& tOrigin, const Point& tControl1, const Point& tControl2, const Point& tDestination, KDuint uSegments );

    /** 
	 *	draws a Catmull Rom path.
     *	@warning This function could be pretty slow. Use it only for debugging purposes.
     *	@since v2.0
     */
    KDvoid		drawCatmullRom ( PointArray* pArrayOfControlPoints, KDuint uSegments );

    /**
	 *	draws a Cardinal Spline path.
     *	@warning This function could be pretty slow. Use it only for debugging purposes.
     *	@since v2.0
     */
    KDvoid		drawCardinalSpline ( PointArray* pConfig, KDfloat fTension,  KDuint uSegments );

    /**
	 *	set the drawing color with 4 unsigned bytes
     *	@since v2.0
     */
    KDvoid		setDrawColor4B ( GLubyte r, GLubyte g, GLubyte b, GLubyte a );

    /**
	 *	set the drawing color with 4 floats
     *	@since v2.0
     */
    KDvoid		setDrawColor4F ( GLfloat r, GLfloat g, GLfloat b, GLfloat a );

    /** 
	 *	set the point size in points. Default 1.
     *	@since v2.0
     */
    KDvoid		setPointSize ( GLfloat fPointSize );

};

//	end of global group
/// @}

NS_CC_END

#endif	// __CCDrawingPrimitives_h__
