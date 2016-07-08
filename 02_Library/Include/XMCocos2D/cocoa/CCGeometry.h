/* -----------------------------------------------------------------------------------
 *
 *      File            CCGeometry.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCGeometry_h__
#define __CCGeometry_h__

#include "CCObject.h"

typedef KDfloat       cpFloat;
typedef struct cpVect cpVect;

NS_CC_BEGIN
    
/**
 * @addtogroup data_structures
 * @{
 */

 // for CCPoint assignement operator and copy constructor
class CCSize;

class CCPoint 
{
	public :

		KDfloat		x;
		KDfloat		y;

	public :

		CCPoint ( KDvoid );		
		CCPoint ( KDfloat fX, KDfloat fY );
		CCPoint ( const CCPoint& tOther );
		CCPoint ( const CCSize& tSize );
		CCPoint ( const cpVect& tVect );
		
	public :

		CCPoint&		operator= ( const CCPoint& tOther );
		CCPoint&		operator= ( const CCSize& tSize );
		CCPoint&		operator= ( const cpVect& tVect );

		CCPoint			operator+ ( const CCPoint& tRight ) const;
		CCPoint			operator- ( const CCPoint& tRight ) const;

		CCPoint			operator- ( KDvoid ) const;

		CCPoint			operator* ( KDfloat fA ) const;
		CCPoint			operator/ ( KDfloat fA ) const;
		
		KDvoid			setPoint ( KDfloat fX, KDfloat fY );

		KDbool			equals ( const CCPoint& tTarget ) const;

		/**
		 *	@returns if points have fuzzy equality which means equal with some degree of variance.
		 *	@since v2.1.4
		 *	@lua NA
		 */
		KDbool			fuzzyEquals ( const CCPoint& tTarget, KDfloat fVariance ) const;

		/** 
		 *	Calculates distance between point an origin
		 *	@return float
		 *	@since v2.1.4
		 *	@lua NA
		 */
		inline KDfloat	getLength ( KDvoid ) const 
		{
			return kdSqrtf ( x * x + y * y );
		};

		/** 
		 *	Calculates the square length of a CCPoint (not calling sqrt() )
		 *	@return float
		 *	@since v2.1.4
		 *	@lua NA
		 */
		inline KDfloat	getLengthSq ( KDvoid ) const 
		{
			return dot ( *this );		// x * x + y * y;
		};

		/** 
		 *	Calculates the square distance between two points (not calling sqrt() )
		 *	@return float
		 *	@since v2.1.4
		 */
		inline KDfloat	getDistanceSq ( const CCPoint& tOther ) const 
		{
			return ( *this - tOther ).getLengthSq ( );
		};

		/** 
		 *	Calculates the distance between two points
		 *	@return float
		 *	@since v2.1.4
		 */
		inline KDfloat	getDistance ( const CCPoint& tOther ) const 
		{
			return ( *this - tOther ).getLength ( );
		};

		/** 
		 *	@returns the angle in radians between this vector and the x axis
		 *	@since v2.1.4
		 */
		inline KDfloat	getAngle ( KDvoid ) const 
		{
			return kdAtan2f ( y, x );
		};

		/** 
		 *	@returns the angle in radians between two vector directions
		 *	@since v2.1.4
		 */
		KDfloat			getAngle ( const CCPoint& tOther ) const;

		/** 
		 *	Calculates dot product of two points.
		 *	@return float
		 *	@since v2.1.4
		 */
		inline KDfloat	dot ( const CCPoint& tOther ) const 
		{
			return x * tOther.x + y * tOther.y;
		};

		/**
		 *	Calculates cross product of two points.
		 *	@return float
		 *	@since v2.1.4
		 */
		inline KDfloat	cross ( const CCPoint& tOther ) const 
		{
			return x * tOther.y - y * tOther.x;
		};

		/** 
		 *	Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
		 *	@return CCPoint
		 *	@since v2.1.4
		 */
		inline CCPoint	getPerp ( KDvoid ) const 
		{
			return CCPoint ( -y, x );
		};

		/**
		 *	Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
		 *	@return CCPoint
		 *	@since v2.1.4
		 */
		inline CCPoint	getRPerp ( KDvoid ) const 
		{
			return CCPoint ( y, -x );
		};

		/**
		 *	Calculates the projection of this over other.
		 *	@return CCPoint
		 *	@since v2.1.4
		 */
		inline CCPoint	project ( const CCPoint& tOther ) const 
		{
			return tOther * ( dot ( tOther ) / tOther.dot ( tOther ) );
		};

		/** 
		 *	Complex multiplication of two points ("rotates" two points).
		 *	@return CCPoint vector with an angle of this.getAngle() + other.getAngle(),
		 *	and a length of this.getLength() * other.getLength().
		 *	@since v2.1.4
		 */
		inline CCPoint	rotate ( const CCPoint& tOther ) const 
		{
			return CCPoint ( x * tOther.x - y * tOther.y, x * tOther.y + y * tOther.x );
		};

		/**
		 *	Unrotates two points.
		 *	@return CCPoint vector with an angle of this.getAngle() - other.getAngle(),
		 *	and a length of this.getLength() * other.getLength().
		 *	@since v2.1.4
		 */
		inline CCPoint	unrotate ( const CCPoint& tOther ) const 
		{
			return CCPoint ( x * tOther.x + y * tOther.y, y * tOther.x - x * tOther.y );
		};

		/**
		 *	Returns point multiplied to a length of 1.
		 *	If the point is 0, it returns (1, 0)
		 *	@return CCPoint
		 *	@since v2.1.4
		 */
		inline CCPoint	normalize ( KDvoid ) const 
		{
			KDfloat		fLength = getLength ( );
			if ( fLength == 0.0f ) 
			{
				return CCPoint ( 1.f, 0 );
			}
			return *this / getLength ( );
		};

		/**
		 *	Linear Interpolation between two points a and b
		 *	@returns
		 *		alpha == 0 ? a
		 *		alpha == 1 ? b
		 *		otherwise a value between a..b
		 *	@since v2.1.4
		 */
		inline CCPoint	lerp ( const CCPoint& tOther, KDfloat fAlpha ) const 
		{
			return *this * ( 1.f - fAlpha ) + tOther * fAlpha;
		};

		/**
		 *	Rotates a point counter clockwise by the angle around a pivot
		 *	@param pivot is the pivot, naturally
		 *	@param angle is the angle of rotation ccw in radians
		 *	@returns the rotated point
		 *	@since v2.1.4
		 */
		CCPoint			rotateByAngle ( const CCPoint& tPivot, KDfloat fAngle ) const;

		static inline CCPoint	forAngle ( const KDfloat fAngle )
		{
    		return CCPoint ( kdCosf ( fAngle ), kdSinf ( fAngle ) );
		}

		KDbool			containsPolygon ( const CCPoint* pPoints, KDuint uPoints ) const;
};

class CCSize 
{
	public :

		union 
		{
			KDfloat     cx;
			KDfloat     width;			
		};

		union
		{
			KDfloat     cy;
			KDfloat     height;
		};

	public :

		CCSize ( KDvoid );
		CCSize ( KDfloat fWidth, KDfloat fHeight );
		CCSize ( const CCSize& tOther );
		CCSize ( const CCPoint& tPoint );

		CCSize&			operator= ( const CCSize& tOther );
		CCSize&			operator= ( const CCPoint& tPoint );

		CCSize			operator+ ( const CCSize& tRight ) const;
		CCSize			operator- ( const CCSize& tRight ) const;

		CCSize			operator- ( KDvoid ) const;

		CCSize			operator* ( KDfloat fA ) const;
		CCSize			operator/ ( KDfloat fA ) const;

		KDvoid			setSize ( KDfloat fWidth, KDfloat fHeight );

		KDbool			equals ( const CCSize& tTarget ) const;	
};

class CCRect 
{
	public :

		CCPoint			origin;
		CCSize			size;		

	public :

		CCRect ( KDvoid );	
		CCRect ( KDfloat fX, KDfloat fY, KDfloat fWidth, KDfloat fHeight );
		CCRect ( const CCPoint& tPoint, const CCSize& tSize );
		CCRect ( const CCRect&  tRect );
		
		CCRect&			operator= ( const CCRect& tRect );

		KDvoid			setRect ( KDfloat fX, KDfloat fY, KDfloat fWidth, KDfloat fHeight );
				
		KDfloat			getMinX ( KDvoid ) const;	/// return the leftmost   x-value of current rect
		KDfloat			getMidX ( KDvoid ) const;	/// return the midpoint   x-value of current rect
		KDfloat			getMaxX ( KDvoid ) const;	/// return the rightmost  x-value of current rect
		KDfloat			getMinY ( KDvoid ) const;	/// return the bottommost y-value of current rect
		KDfloat			getMidY ( KDvoid ) const;	/// return the midpoint   y-value of current rect
		KDfloat			getMaxY ( KDvoid ) const;	/// return the topmost    y-value of current rect

		KDbool			equals ( const CCRect& tRect ) const;   
		
		KDbool			containsPoint ( const CCPoint& tPoint ) const;

		KDbool			intersectsRect ( const CCRect& tRect ) const;

		KDbool			overlaps ( const CCRect& tRect ) const;

		CCRect&			unions ( const CCRect& tRect );
};


#define CCPointMake( _X, _Y )               CCPoint ( (KDfloat) (_X), (KDfloat) (_Y) )
#define CCSizeMake( _W, _H )                CCSize  ( (KDfloat) (_W), (KDfloat) (_H) )
#define CCRectMake( _X, _Y, _W, _H )        CCRect  ( (KDfloat) (_X), (KDfloat) (_Y), (KDfloat) (_W), (KDfloat) (_H) )

const CCPoint CCPointZero = CCPoint ( 0, 0 );

/* The "zero" size -- equivalent to CCSizeMake(0, 0). */ 
const CCSize  CCSizeZero  = CCSize ( 0, 0 );         

/* The "zero" rectangle -- equivalent to CCRectMake(0, 0, 0, 0). */ 
const CCRect  CCRectZero  = CCRect ( 0, 0, 0, 0 );  

// end of data_structure group
/// @}

NS_CC_END

cpVect cpv ( const cocos2d::CCPoint& tPoint );

#endif // __CCGeometry_h__