/* -----------------------------------------------------------------------------------
 *
 *      File            CCGeometry.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCGeometry_h__
#define __CCGeometry_h__

#include "CCObject.h"
#include "../ccMacros.h"
#include <functional>

NS_CC_BEGIN

/** 
 *	Clamp a value between from and to.
 *	@since v0.99.1
 */
inline KDfloat	clampf ( KDfloat fValue, KDfloat fMinInclusive, KDfloat fMaxInclusive )
{
    if ( fMinInclusive > fMaxInclusive ) 
	{
        CC_SWAP ( fMinInclusive, fMaxInclusive, KDfloat );
    }
    return fValue < fMinInclusive ? fMinInclusive : fValue < fMaxInclusive? fValue : fMaxInclusive;
}

/**
 *	@addtogroup data_structures
 *	@{
 */

// for Point assignement operator and copy constructor
class CC_DLL Size;
class CC_DLL Rect;

class CC_DLL Point
{
	public :

		KDfloat			x;
		KDfloat			y;

	public :

		/**
		 *	@js NA
		 */
		Point ( KDvoid );

		/**
		 *	@js NA
		 */
		Point ( KDfloat x, KDfloat y );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point ( const Point& rOther );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		explicit Point ( const Size& tSize );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point&					operator= ( const Point& rOther );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point&					operator= ( const Size& tSize );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point					operator+ ( const Point& tRight ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point&					operator+= ( const Point& tRight );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point					operator- ( const Point& tRight ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point&					operator-= ( const Point& tRight );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point					operator- ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool					operator== ( const Point& tRight );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool					operator!= ( const Point& tRight );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool					operator== ( const Point& tRight ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool					operator!= ( const Point& tRight ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point					operator* ( const Point& tRight ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point					operator* ( KDfloat a ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Point					operator/ ( KDfloat a ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					setPoint ( KDfloat x, KDfloat y );

		/**
		 *	@js NA
		 */
		KDbool					equals ( const Point& tTarget ) const;
    
		/** 
		 *	@returns if points have fuzzy equality which means equal with some degree of variance.
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		KDbool					fuzzyEquals ( const Point& tTarget, KDfloat fVariance ) const;


		/**
		 *	Calculates distance between point an origin
		 *	@return float
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline KDfloat			getLength ( KDvoid ) const 
		{
			return kdSqrtf ( x * x + y * y );
		};

		/**
		 *	Calculates the square length of a Point (not calling sqrt() )
		 *	@return float
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline KDfloat			getLengthSq ( KDvoid ) const 
		{
			return dot ( *this );	// x*x + y*y;
		};

		/**
		 *	Calculates the square distance between two points (not calling sqrt() )
		 *	@return float
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline KDfloat			getDistanceSq ( const Point& tOther ) const 
		{
			return ( *this - tOther ).getLengthSq ( );
		};

		/** 
		 *	Calculates the distance between two points
		 *	@return float
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline KDfloat			getDistance ( const Point& tOther ) const
		{
			return ( *this - tOther ).getLength ( );
		};

		/**
		 *	@returns the angle in radians between this vector and the x axis
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline KDfloat			getAngle ( KDvoid ) const 
		{
			return kdAtan2f ( y, x );
		};

		/** 
		 *	@returns the angle in radians between two vector directions
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		KDfloat					getAngle ( const Point& tOther ) const;

		/**
		 *	Calculates dot product of two points.
		 *	@return float
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline KDfloat			dot ( const Point& tOther ) const 
		{
			return x * tOther.x + y * tOther.y;
		};

		/**
		 *	Calculates cross product of two points.
		 *	@return float
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline KDfloat			cross ( const Point& tOther ) const 
		{
			return x * tOther.y - y * tOther.x;
		};

		/**
		 *	Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
		 *	@return Point
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			getPerp ( KDvoid ) const 
		{
			return Point ( -y, x );
		};
    
		/** 
		 *	Calculates midpoint between two points.
		 *	@return Point
		 *	@since v3.0
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			getMidpoint ( const Point& tOther ) const
		{
			return Point ( ( x + tOther.x ) / 2.0f, ( y + tOther.y ) / 2.0f );
		}
    
		/** 
		 *	Clamp a point between from and to.
		 *	@since v3.0
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			getClampPoint ( const Point& tMinInclusive, const Point& tMaxInclusive ) const
		{
			return Point ( clampf ( x, tMinInclusive.x, tMaxInclusive.x ), clampf ( y, tMinInclusive.y, tMaxInclusive.y ) );
		}
    
		/** 
		 *	Run a math operation function on each point component
		 *	absf, fllorf, ceilf, roundf
		 *	any function that has the signature: float func(float);
		 *	For example: let's try to take the floor of x,y
		 *	p.compOp(floorf);
		 *	@since v3.0
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			compOp ( std::function<KDfloat(KDfloat)> function ) const
		{
			return Point ( function ( x ), function ( y ) );
		}

		/** 
		 *	Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
		 *	@return Point
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			getRPerp ( KDvoid ) const 
		{
			return Point ( y, -x );
		};

		/** 
		 *	Calculates the projection of this over other.
		 *	@return Point
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			project ( const Point& tOther ) const 
		{
			return tOther * ( dot ( tOther ) / tOther.dot ( tOther ) );
		};

		/**
		 *	Complex multiplication of two points ("rotates" two points).
		 *	@return Point vector with an angle of this.getAngle() + other.getAngle(),
		 *	and a length of this.getLength() * other.getLength().
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			rotate ( const Point& tOther ) const 
		{
			return Point ( x * tOther.x - y * tOther.y, x * tOther.y + y * tOther.x );
		};

		/** 
		 *	Unrotates two points.
		 *	@return Point vector with an angle of this.getAngle() - other.getAngle(),
		 *	and a length of this.getLength() * other.getLength().
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			unrotate ( const Point& tOther ) const
		{
			return Point ( x * tOther.x + y * tOther.y, y * tOther.x - x * tOther.y );
		};

		/** 
		 *	Returns point multiplied to a length of 1.
		 *	If the point is 0, it returns (1, 0)
		 *	@return Point
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			normalize ( KDvoid ) const 
		{
			KDfloat		fLength = getLength ( );
			if ( fLength == 0.0f ) return Point ( 1.f, 0 );
			return *this / getLength ( );
		};

		/** 
		 *	Linear Interpolation between two points a and b
		 *	@returns
		 *		alpha == 0 ? a
		 *		alpha == 1 ? b
		 *		otherwise a value between a..b
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		inline Point			lerp ( const Point& tOther, KDfloat fAlpha ) const 
		{
			return *this * ( 1.f - fAlpha ) + tOther * fAlpha;
		};

		/**
		 *	Rotates a point counter clockwise by the angle around a pivot
		 *	@param pivot is the pivot, naturally
		 *	@param angle is the angle of rotation ccw in radians
		 *	@returns the rotated point
		 *	@since v2.1.4
		 *	@js NA
		 *	@lua NA
		 */
		Point					rotateByAngle ( const Point& tPivot, KDfloat fAngle ) const;

		//////////////////
		// Added By XMSoft
		KDbool					isInRect ( const Rect& tRect ) const;

		KDbool					isInCircle ( const Point& tOrigin, KDfloat fRadius ) const;

		static KDfloat			pointToRadians ( Point tPoint );

		static Point			radiansToPoint ( KDfloat fRadians );
		
		/////////////////

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static inline Point		forAngle ( const KDfloat a )
		{
    		return Point ( kdCosf ( a ), kdSinf ( a ) );
		}
    
		/**
		 *	A general line-line intersection test
		 *	@param A   the startpoint for the first line L1 = (A - B)
		 *	@param B   the endpoint for the first line L1 = (A - B)
		 *	@param C   the startpoint for the second line L2 = (C - D)
		 *	@param D   the endpoint for the second line L2 = (C - D)
		 *	@param S   the range for a hitpoint in L1 (p = A + S*(B - A))
		 *	@param T   the range for a hitpoint in L2 (p = C + T*(D - C))
		 *	@returns   whether these two lines interects.
		 *
		 *	Note that to truly test intersection for segments we have to make
		 *	sure that S & T lie within [0..1] and for rays, make sure S & T > 0
		 *	the hit point is        C + T * (D - C);
		 *	the hit point also is   A + S * (B - A);
		 *	@since 3.0
		 *	@js NA
		 *	@lua NA
		 */
		static KDbool			isLineIntersect ( const Point& A, const Point& B,
												  const Point& C, const Point& D,
												  KDfloat* S = nullptr, KDfloat* T = nullptr );
    
		/**
		 *	returns true if Line A-B overlap with segment C-D
		 *	@since v3.0
		 *	@js NA
		 *	@lua NA
		 */
		static KDbool			isLineOverlap ( const Point& A, const Point& B,
												const Point& C, const Point& D );
    
		/**
		 *	returns true if Line A-B parallel with segment C-D
		 *	@since v3.0
		 *	@js NA
		 *	@lua NA
		 */
		static KDbool			isLineParallel ( const Point& A, const Point& B,
												 const Point& C, const Point& D );
    
		/**
		 *	returns true if Segment A-B overlap with segment C-D
		 *	@since v3.0
		 *	@js NA
		 *	@lua NA
		 */
		static KDbool			isSegmentOverlap ( const Point& A, const Point& B,
												   const Point& C, const Point& D,
												   Point* S = nullptr, Point* E = nullptr );
    
		/**
		 *	returns true if Segment A-B intersects with segment C-D
		 *	@since v3.0
		 *	@js NA
		 *	@lua NA
		 */
		static KDbool			isSegmentIntersect ( const Point& A, const Point& B, const Point& C, const Point& D );
    
		/**
		 *	returns the intersection point of line A-B, C-D
		 *	@since v3.0
		 *	@js NA
		 *	@lua NA
		 */
		static Point			getIntersectPoint ( const Point& A, const Point& B, const Point& C, const Point& D );

		/** equals to Point(0,0) */
		static const Point		ZERO;
		/** equals to Point(0.5, 0.5) */
		static const Point		ANCHOR_MIDDLE;
		/** equals to Point(0, 0) */
		static const Point		ANCHOR_BOTTOM_LEFT;
		/** equals to Point(0, 1) */
		static const Point		ANCHOR_TOP_LEFT;
		/** equals to Point(1, 0) */
		static const Point		ANCHOR_BOTTOM_RIGHT;
		/** equals to Point(1, 1) */
		static const Point		ANCHOR_TOP_RIGHT;
		/** equals to Point(1, 0.5) */
		static const Point		ANCHOR_MIDDLE_RIGHT;
		/** equals to Point(0, 0.5) */
		static const Point		ANCHOR_MIDDLE_LEFT;
		/** equals to Point(0.5, 1) */
		static const Point		ANCHOR_MIDDLE_TOP;
		/** equals to Point(0.5, 0) */
		static const Point		ANCHOR_MIDDLE_BOTTOM;

	private :

		// returns true if segment A-B intersects with segment C-D. S->E is the ovderlap part
		static KDbool			isOneDemensionSegmentOverlap ( KDfloat A, KDfloat B, KDfloat C, KDfloat D, KDfloat* S, KDfloat* E );
    
		// cross procuct of 2 vector. A->B X C->D
		static KDfloat			crossProduct2Vector ( const Point& A, const Point& B, const Point& C, const Point& D )
		{
			return ( D.y - C.y ) * ( B.x - A.x ) - ( D.x - C.x ) * ( B.y - A.y );
		}
};

class CC_DLL Size
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

		/**
		 *	@js NA
		 */
		Size ( KDvoid );

		/**
		 *	@js NA
		 */
		Size ( KDfloat fWidth, KDfloat fHeight );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Size ( const Size& rOther );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		explicit Size ( const Point& tPoint );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Size&					operator= ( const Size& rOther );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Size&					operator= ( const Point& tPoint );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Size					operator+ ( const Size& tRight ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Size					operator- ( const Size& tRight ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Size					operator* ( KDfloat a ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Size					operator/ ( KDfloat a ) const;
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					setSize ( KDfloat fWidth, KDfloat fHeight );

		/**
		 *	@js NA
		 */
		KDbool					equals ( const Size& tTarget ) const;
    
		static const Size		ZERO;
};

class CC_DLL Rect
{
	public :

		Point		origin;
		Size		size;

	public :

		/**
		 *	@js NA
		 */
		Rect ( KDvoid );

		/**
		 *	@js NA
		 */
		Rect ( KDfloat x, KDfloat y, KDfloat fWidth, KDfloat fHeight );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Rect ( const Rect& rOther );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Rect&					operator= ( const Rect& rOther );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid					setRect ( KDfloat x, KDfloat y, KDfloat fWidth, KDfloat fHeight );

		/**
		 *	@js NA
		 */
		KDfloat  				getMinX ( KDvoid ) const;	/// return the leftmost x-value of current rect

		/**
		 *	@js NA
		 */
		KDfloat  				getMidX ( KDvoid ) const;	/// return the midpoint x-value of current rect

		/**
		 *	@js NA
		 */
		KDfloat  				getMaxX ( KDvoid ) const;	/// return the rightmost x-value of current rect

		/**
		 *	@js NA
		 */
		KDfloat  				getMinY ( KDvoid ) const;	/// return the bottommost y-value of current rect

		/**
		 *	@js NA
		 */
		KDfloat 				getMidY ( KDvoid ) const;	/// return the midpoint y-value of current rect

		/**
		 *	@js NA
		 */
		KDfloat					getMaxY ( KDvoid ) const;	/// return the topmost y-value of current rect

		/**
		 *	@js NA
		 */
		KDbool					equals ( const Rect& tRect ) const;
		
		/**
		 *	@js NA
		 */
		KDbool					containsPoint ( const Point& tPoint ) const;

		/**
		 *	@js NA
		 */
		KDbool					intersectsRect ( const Rect& tRect ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Rect					unionWithRect ( const Rect& tRect ) const;
    
		static const Rect		ZERO;
};

//	end of data_structure group
/// @}

NS_CC_END

#endif	// __CCGeometry_h__
