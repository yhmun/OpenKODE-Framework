/* -----------------------------------------------------------------------------------
 *
 *      File            CCPointExtension.cpp
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

#include "Precompiled.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN
    
#define kCCPointEpsilon  KD_FLT_EPSILON

KDfloat ccpLength ( const CCPoint& v )
{
	return v.getLength ( );
}

KDfloat ccpDistance ( const CCPoint& v1, const CCPoint& v2 )
{
	return ( v1 - v2 ).getLength ( );
}

CCPoint ccpNormalize ( const CCPoint& v )
{
	return v.normalize ( );
}

CCPoint ccpForAngle ( const KDfloat a )
{
	return CCPoint::forAngle ( a );
}

KDfloat ccpToAngle ( const CCPoint& v )
{
	return v.getAngle ( );
}

CCPoint ccpLerp ( const CCPoint& a, const CCPoint& b, KDfloat alpha )
{
	return a.lerp ( b, alpha );
}

KDfloat clampf ( KDfloat value, KDfloat min_inclusive, KDfloat max_inclusive )
{
	if ( min_inclusive > max_inclusive ) 
	{
		CC_SWAP ( min_inclusive, max_inclusive, KDfloat );
	}

	return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

CCPoint ccpClamp ( const CCPoint& p, const CCPoint& min_inclusive, const CCPoint& max_inclusive )
{
	return ccp ( clampf ( p.x, min_inclusive.x, max_inclusive.x ), clampf ( p.y, min_inclusive.y, max_inclusive.y ) );
}

CCPoint ccpFromSize ( const CCSize& s )
{
	return CCPoint ( s );
}

CCPoint ccpCompOp ( const CCPoint& p, KDfloat (*opFunc) ( KDfloat ) )
{
	return ccp ( opFunc ( p.x ), opFunc ( p.y ) );
}

KDbool ccpFuzzyEqual ( const CCPoint& a, const CCPoint& b, KDfloat var )
{
	return a.fuzzyEquals ( b, var );
}

CCPoint ccpCompMult ( const CCPoint& a, const CCPoint& b )
{
	return ccp ( a.x * b.x, a.y * b.y );
}

KDfloat ccpAngleSigned ( CCPoint& a, CCPoint& b )
{
	return a.getAngle ( b );
}

CCPoint ccpRotateByAngle ( const CCPoint& v, const CCPoint& pivot, KDfloat angle ) 
{
	return v.rotateByAngle (pivot, angle );
}

KDbool ccpSegmentIntersect ( const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D )
{
    KDfloat S, T;

    if ( ccpLineIntersect ( A, B, C, D, &S, &T ) && ( S >= 0.0f && S <= 1.0f && T >= 0.0f && T <= 1.0f ) )
	{
        return KD_TRUE;
	}

    return KD_FALSE;
}

CCPoint ccpIntersectPoint ( const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D )
{
    KDfloat S, T;

    if ( ccpLineIntersect ( A, B, C, D, &S, &T ) )
    {
        // Point of intersection
        CCPoint P;
        P.x = A.x + S * ( B.x - A.x );
        P.y = A.y + S * ( B.y - A.y );
        return P;
    }

    return CCPointZero;
}

KDbool ccpLineIntersect ( const CCPoint& A, const CCPoint& B, const CCPoint& C, const CCPoint& D, KDfloat* S, KDfloat* T )
{
    // FAIL: Line undefined
    if ( ( A.x == B.x && A.y == B.y ) || ( C.x == D.x && C.y == D.y ) )
    {
        return KD_FALSE;
    }

    const KDfloat BAx = B.x - A.x;
    const KDfloat BAy = B.y - A.y;
    const KDfloat DCx = D.x - C.x;
    const KDfloat DCy = D.y - C.y;
    const KDfloat ACx = A.x - C.x;
    const KDfloat ACy = A.y - C.y;

    const KDfloat denom = DCy * BAx - DCx * BAy;

    *S = DCx*ACy - DCy*ACx;
    *T = BAx*ACy - BAy*ACx;

    if ( denom == 0 )
    {
        if ( *S == 0 || *T == 0 )
        { 
            // Lines incident
            return KD_TRUE;   
        }
        // Lines parallel and not incident
        return KD_FALSE;
    }

    *S = *S / denom;
    *T = *T / denom;

    // Point of intersection
    // CCPoint P;
    // P.x = A.x + *S * ( B.x - A.x );
    // P.y = A.y + *S * ( B.y - A.y );

    return KD_TRUE;
}

KDfloat ccpAngle ( const CCPoint& a, const CCPoint& b )
{
	KDfloat angle = kdAcosf ( ccpDot ( ccpNormalize ( a ), ccpNormalize ( b ) ) );

	if ( kdFabsf ( angle ) < kCCPointEpsilon )
	{
		return 0.0f;
	}

	return angle;
}


NS_CC_END