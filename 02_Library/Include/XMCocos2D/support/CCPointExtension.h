/* -----------------------------------------------------------------------------------
 *
 *      File            CCPointExtension.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2007 Scott Lembcke
 *      Copyright (c) 2010 Lam Pham      
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

#ifndef __CCPointExtension_h__
#define __CCPointExtension_h__

#include "../cocoa/CCGeometry.h"

NS_CC_BEGIN
    
/**
 * @addtogroup data_structures
 * @{
 */

/**
 *	@file
 *	 CCPoint extensions based on Chipmunk's cpVect file.
 *	 These extensions work both with CCPoint and cpVect.
 *	 
 *	 The "ccp" prefix means: "XMCG Point ( Cocos2D )"
 *	 
 *	 Examples:
 *	  - ccpAdd( ccp(1, 1), ccp(2, 2) );               // preferred cocos2d way
 *	  - ccpAdd( CCPointMake(1,1), CCPointMake(2,2) ); // also ok but more verbose
 *	  
 *	  - cpvadd( cpv(1,1), cpv(2,2) );                 // way of the chipmunk
 *	  - ccpAdd( cpv(1,1), cpv(2,2) );                 // mixing chipmunk and cocos2d (avoid)
 *	  - cpvadd( CCPointMake(1,1), CCPointMake(2,2) ); // mixing chipmunk and XMCG (avoid)
 */


/**
 *	Helper macro that creates a CCPoint
 *	@return CCPoint
 *	@since	v0.7.2
 */

#define ccp( _x, _y )				CCPointMake ( (KDfloat) (_x), (KDfloat) (_y) )
#define ccs( _w, _h )				CCSizeMake  ( (KDfloat) (_w), (KDfloat) (_h) )
#define ccr( _x, _y, _w, _h )		CCRectMake  ( (KDfloat) (_x), (KDfloat) (_y), (KDfloat) (_w), (KDfloat) (_h) )

#define ccpz						CCPointZero
#define ccsz						CCSizeZero
#define ccrz						CCRectZero

static inline CCPoint  ccpMid ( const CCPoint& v )
{
	return v / 2;
}

static inline CCPoint  ccpMid ( const CCSize& s )
{
	return s / 2;
}

/** 
 *	Returns opposite of point.
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpNeg ( const CCPoint& v )
{
	return -v;
}

/** 
 *	Calculates sum of two points.
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpAdd ( const CCPoint& v1, const CCPoint& v2 )
{
	return v1 + v2;
}

/** 
 *	Calculates difference of two points.
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpSub ( const CCPoint& v1, const CCPoint& v2 )
{
	return v1 - v2;
}

static inline CCPoint  ccpMul ( const CCPoint& v1, const CCPoint& v2 )
{
	return ccp ( v1.x * v2.x, v1.y * v2.y );
}

static inline CCPoint  ccpDiv ( const CCPoint& v1, const CCPoint& v2 )
{
	return ccp ( v1.x / v2.x, v1.y / v2.y );
}

/** 
 *	Returns point multiplied by given factor.
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpMult ( const CCPoint& v, const KDfloat s )
{
	return v * s;
}

/** 
 *	Calculates midpoint between two points.
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpMidpoint ( const CCPoint& v1, const CCPoint& v2 )
{
	return ( v1 + v2 ) / 2.f;
}

/** 
 *	Calculates dot product of two points.
 *	@return KDfloat
 *	@since	v0.7.2
 */
static inline KDfloat  ccpDot ( const CCPoint& v1, const CCPoint& v2 )
{
	return v1.dot ( v2 );
}

/** 
 *	Calculates cross product of two points.
 *	@return KDfloat
 *	@since	v0.7.2
 */
static inline KDfloat  ccpCross ( const CCPoint& v1, const CCPoint& v2 )
{
	return v1.cross ( v2 );
}

/** 
 *	Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpPerp ( const CCPoint& v )
{
	return v.getPerp ( );
}

/** 
 *	Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpRPerp ( const CCPoint& v )
{
	return v.getRPerp ( );
}

/** 
 *	Calculates the projection of v1 over v2.
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpProject ( const CCPoint& v1, const CCPoint& v2 )
{
	return v1.project ( v2 );
}

/** 
 *	Rotates two points.
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpRotate ( const CCPoint& v1, const CCPoint& v2 )
{
	return v1.rotate ( v2 );
}

/** 
 *	Unrotates two points.
 *	@return CCPoint
 *	@since	v0.7.2
 */
static inline CCPoint  ccpUnrotate ( const CCPoint& v1, const CCPoint& v2 )
{
	return v1.unrotate ( v2 );
}

/** 
 *	Calculates the square length of a CCPoint (not calling sqrt() )
 *	@return KDfloat
 *	@since	v0.7.2
 */
static inline KDfloat  ccpLengthSQ ( const CCPoint& v )
{
	return v.getLengthSq ( );
}

/**
 *	Calculates the square distance between two points (not calling sqrt() )
 *	@return KDfloat
 *	@since v1.1
 */
static inline KDfloat  ccpDistanceSQ ( const CCPoint& v1, const CCPoint& v2 )
{
    return ( v1 - v2 ).getLengthSq ( ); 
}

/** 
 *	Calculates distance between point an origin
 *	@return KDfloat
 *	@since	v0.7.2
 */
KDfloat  ccpLength ( const CCPoint& v );

/** 
 *	Calculates the distance between two points
 *	@return KDfloat
 *	@since	v0.7.2
 */
KDfloat  ccpDistance ( const CCPoint& v1, const CCPoint& v2 );

/** 
 *	Returns point multiplied to a length of 1.
 *	@return CCPoint
 *	@since	v0.7.2
 */
CCPoint  ccpNormalize ( const CCPoint& v );

/** 
 *	Converts radians to a normalized vector.
 *	@return CCPoint
 *	@since	v0.7.2
 */
CCPoint  ccpForAngle ( const KDfloat a );

/** 
 *	Converts a vector to radians.
 *	@return KDfloat
 *	@since	v0.7.2
 */
KDfloat  ccpToAngle ( const CCPoint& v );

/** 
 *	Clamp a value between from and to.
 *	@since	v0.99.1
 */
KDfloat  clampf ( KDfloat value, KDfloat min_inclusive, KDfloat max_inclusive );

/** 
 *	Clamp a point between from and to.
 *	@since	v0.99.1
 */
CCPoint  ccpClamp ( const CCPoint& p, const CCPoint& from, const CCPoint& to );

/** 
 *	Quickly convert CCSize to a CCPoint
 *	@since	v0.99.1
 */
CCPoint  ccpFromSize ( const CCSize& s );

/** 
 *	Run a math operation function on each point component
 *	absf, fllorf, ceilf, roundf
 *	any function that has the signature: float func(float);
 *	For example: let's try to take the floor of x,y
 *	ccpCompOp(p,floorf);
 *	@since	v0.99.1
 */
CCPoint  ccpCompOp ( const CCPoint& p, KDfloat (*opFunc) ( KDfloat ) );

/** 
 *	Linear Interpolation between two points a and b
 *	@returns
 *		alpha == 0 ? a
 *		alpha == 1 ? b
 *		otherwise a value between a..b
 *	@since	v0.99.1
 */
CCPoint  ccpLerp ( const CCPoint& a, const CCPoint& b, KDfloat alpha );

/** 
 *	@returns if points have fuzzy equality which means equal with some degree of variance.
 *	@since	v0.99.1
 */
KDbool   ccpFuzzyEqual ( const CCPoint& a, const CCPoint& b, KDfloat variance );

/**
 *	Multiplies a and b components, a.x*b.x, a.y*b.y
 *	@returns a component-wise multiplication
 *	@since	 v0.99.1
 */
CCPoint  ccpCompMult ( const CCPoint& a, const CCPoint& b );

/** 
 *	@returns the signed angle in radians between two vector directions
 *	@since	v0.99.1
 */
KDfloat  ccpAngleSigned ( const CCPoint& a, const CCPoint& b );

/** 
 *	@returns the angle in radians between two vector directions
 *	@since	v0.99.1
*/
KDfloat  ccpAngle ( const CCPoint& a, const CCPoint& b );

/** 
 *	Rotates a point counter clockwise by the angle around a pivot
 *	@param	 v is the point to rotate
 *	@param	 pivot is the pivot, naturally
 *	@param	 angle is the angle of rotation cw in radians
 *	@returns the rotated point
 *	@since	 v0.99.1
 */
CCPoint  ccpRotateByAngle ( const CCPoint& v, const CCPoint& pivot, KDfloat angle );

/**
 *	A general line-line intersection test
 *	@param p1 
 *		is the startpoint for the first line P1 = (p1 - p2)
 *	@param p2 
 *		is the endpoint for the first line P1 = (p1 - p2)
 *	 @param p3 
 *		is the startpoint for the second line P2 = (p3 - p4)
 *	 @param p4 
 *		is the endpoint for the second line P2 = (p3 - p4)
 *	 @param s 
 *		is the range for a hitpoint in P1 (pa = p1 + s*(p2 - p1))
 *	 @param t
 *		is the range for a hitpoint in P3 (pa = p2 + t*(p4 - p3))
 *	 @return bool 
 *		indicating successful intersection of a line
 *		note that to truly test intersection for segments we have to make 
 *		sure that s & t lie within [0..1] and for rays, make sure s & t > 0
 *		the hit point is		p3 + t * (p4 - p3);
 *		the hit point also is	p1 + s * (p2 - p1);
 *	 @since v0.99.1
 */
KDbool   ccpLineIntersect ( const CCPoint& p1, const CCPoint& p2, const CCPoint& p3, const CCPoint& p4, KDfloat* s, KDfloat* t );

/*
 *	ccpSegmentIntersect returns YES if Segment A-B intersects with segment C-D
 *	@since	v1.0.0
 */
KDbool   ccpSegmentIntersect ( const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D );

/*
 *	ccpIntersectPoint returns the intersection point of line A-B, C-D
 *	@since	v1.0.0
 */
CCPoint  ccpIntersectPoint ( const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D );

// end of data_structures group
/// @}

NS_CC_END

#endif // __CCPointExtension_h__