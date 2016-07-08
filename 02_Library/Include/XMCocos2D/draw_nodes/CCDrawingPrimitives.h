/* -----------------------------------------------------------------------------------
 *
 *      File            CCDrawingPrimitives.h
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

#ifndef __CCDrawingPrimitives_h__
#define __CCDrawingPrimitives_h__

#include "../ccTypes.h"

NS_CC_BEGIN
   
/**
 * @addtogroup global
 * @{
 */

/**
 *	@file
 *	Drawing OpenGL ES primitives.
 *		- ccDrawPoint, ccDrawPoints
 *		- ccDrawLine
 *		- ccDrawRect, ccDrawSolidRect
 *		- ccDrawPoly, ccDrawSolidPoly
 *		- ccDrawCircle
 *		- ccDrawQuadBezier
 *		- ccDrawCubicBezier
 *		- ccDrawCatmullRom
 *		- ccDrawCardinalSpline
 * 
 *	You can change the color, point size, width by calling:
 *		- ccDrawColor4B(), ccDrawColor4F()
 *		- ccPointSize()
 *		- glLineWidth()
 *
 *	@warning These functions draws the Line, Point, Polygon, immediately. They aren't batched. If you are going to make a game that depends on these primitives, I suggest creating a batch. Instead you should use CCDrawNode
 */

class CCPointArray;

/** Initializes the drawing primitives */
KDvoid	ccDrawInit ( KDbool bUsePointSize = KD_FALSE );

/** Frees allocated resources by the drawing primitives */
KDvoid	ccDrawFree ( KDvoid );

/** draws a point given x and y coordinate measured in points */
KDvoid	ccDrawPoint ( const CCPoint& tPoint );

/**
 *	draws an array of points.
 *	@since	v0.7.2
 */
KDvoid	ccDrawPoints ( const CCPoint* pPoints, KDuint uNumberOfPoints );

/** draws a line given the origin and destination point measured in points */
KDvoid	ccDrawLine ( const CCPoint& tOrigin, const CCPoint& tDestination );

/** draws a rectangle given the origin and destination point measured in points. */
KDvoid	ccDrawRect ( const CCPoint& tOrigin, const CCPoint& tDestination );

/**
 *	draws a solid rectangle given the origin and destination point measured in points.
 *	@since 1.1
 */
KDvoid	ccDrawSolidRect ( const CCPoint& tOrigin, const CCPoint& tDestination, ccColor4F tColor );

KDvoid	ccDrawSolidRect ( const CCPoint& tOrigin, const CCPoint& tDestination );

/** 
 *	draws a polygon given a pointer to CCPoint coordinates and the number of vertices measured in points.
 *	The polygon can be closed or open
 */
KDvoid	ccDrawPoly ( const CCPoint* pPoints, KDuint uNumberOfPoints, KDbool bClosePolygon );

/**
 *	draws a solid polygon given a pointer to CGPoint coordinates, the number of vertices measured in points, and a color.
 */
KDvoid	ccDrawSolidPoly ( const CCPoint* pPoints, KDuint uNumberOfPoints, ccColor4F tColor );

KDvoid	ccDrawSolidPoly ( const CCPoint* pPoints, KDuint uNumberOfPoints );

/** draws a circle given the center, radius and number of segments. */
KDvoid	ccDrawCircle ( const CCPoint& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter, KDfloat fScaleX, KDfloat fScaleY );
KDvoid	ccDrawCircle ( const CCPoint& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter );

KDvoid  ccDrawSolidCircle ( const CCPoint& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter, KDfloat fScaleX, KDfloat fScaleY );
KDvoid  ccDrawSolidCircle ( const CCPoint& tCenter, KDfloat fRadius, KDfloat fAngle, KDuint uSegments, KDbool bDrawLineToCenter );

/**
 *	draws a quad bezier path
 *	@warning This function could be pretty slow. Use it only for debugging purposes.
 *	@since	v0.8
 */
KDvoid	ccDrawQuadBezier ( const CCPoint& tOrigin, const CCPoint& tControl, const CCPoint& tDestination, KDuint uSegments );

/**
 *	draws a cubic bezier path
 *	@warning This function could be pretty slow. Use it only for debugging purposes.
 *	@since v0.8
 */
KDvoid	ccDrawCubicBezier ( const CCPoint& tOrigin, const CCPoint& tControl1, const CCPoint& tControl2, const CCPoint& tDestination, KDuint uSegments );

/** 
 *	draws a Catmull Rom path.
 *	@warning This function could be pretty slow. Use it only for debugging purposes.
 *	@since v2.0
 */
KDvoid	ccDrawCatmullRom ( CCPointArray* pArrayOfControlPoints, KDuint uSegments );

/** 
 *	draws a Cardinal Spline path.
 *	@warning This function could be pretty slow. Use it only for debugging purposes.
 *	@since v2.0
 */
KDvoid	ccDrawCardinalSpline ( CCPointArray* pConfig, KDfloat fTension, KDuint uSegments );

/** 
 *	set the drawing color with 4 unsigned bytes
 *	@since v2.0
 */
KDvoid	ccDrawColor4B ( GLubyte r, GLubyte g, GLubyte b, GLubyte a );

/**
 *	set the drawing color with 4 floats
 *	@since v2.0
 */
KDvoid	ccDrawColor4F ( GLfloat r, GLfloat g, GLfloat b, GLfloat a );

/** 
 *	set the point size in points. Default 1.
 *	@since v2.0
 */
KDvoid	ccPointSize ( GLfloat fPointSize );

KDvoid	ccLineWidth ( GLfloat fLineWidth );

// end of global group
/// @}

NS_CC_END

#endif // __CCDrawingPrimitives_h__
