/* -----------------------------------------------------------------------------------
 *
 *      File            CCAffineTransform.cpp
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

#include "Precompiled.h"
#include "cocoa/CCAffineTransform.h"

NS_CC_BEGIN
    
CCAffineTransform CCAffineTransformMake ( KDfloat a, KDfloat b, KDfloat c, KDfloat d, KDfloat tx, KDfloat ty )
{
	CCAffineTransform  tT;

	tT.a = a; 
    tT.b = b;
    tT.c = c; 
    tT.d = d; 
    
    tT.tx = tx; 
    tT.ty = ty;

	return tT;
}

CCPoint CCPointApplyAffineTransform ( const CCPoint& tP, const CCAffineTransform& tT )
{	
	return CCPointMake ( (KDdouble) tT.a * tP.x + (KDdouble) tT.c * tP.y + tT.tx,
					     (KDdouble) tT.b * tP.x + (KDdouble) tT.d * tP.y + tT.ty );
}

CCSize CCSizeApplyAffineTransform ( const CCSize& tS, const CCAffineTransform& tT )
{	
	return CCSizeMake ( (KDdouble) tT.a * tS.cx + (KDdouble) tT.c * tS.cy,
					    (KDdouble) tT.b * tS.cx + (KDdouble) tT.d * tS.cy );
}

CCAffineTransform CCAffineTransformMakeIdentity ( KDvoid )
{
	return CCAffineTransformMake ( 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f );
}

extern const CCAffineTransform CCAffineTransformIdentity = CCAffineTransformMakeIdentity ( );

CCRect CCRectApplyAffineTransform ( const CCRect& tRect, const CCAffineTransform& tT )
{
	KDfloat  fTop    = tRect.getMinY ( );
    KDfloat  fLeft   = tRect.getMinX ( );
	KDfloat  fRight  = tRect.getMaxX ( );
	KDfloat  fBottom = tRect.getMaxY ( );
	
	CCPoint  fTopLeft     = CCPointApplyAffineTransform ( CCPoint ( fLeft , fTop    ), tT );
    CCPoint  fTopRight    = CCPointApplyAffineTransform ( CCPoint ( fRight, fTop    ), tT );
    CCPoint  fBottomLeft  = CCPointApplyAffineTransform ( CCPoint ( fLeft , fBottom ), tT );
    CCPoint  fBottomRight = CCPointApplyAffineTransform ( CCPoint ( fRight, fBottom ), tT );

    KDfloat  fMinX = KD_MIN ( KD_MIN ( fTopLeft.x, fTopRight.x ), KD_MIN ( fBottomLeft.x, fBottomRight.x ) );
    KDfloat  fMaxX = KD_MAX ( KD_MAX ( fTopLeft.x, fTopRight.x ), KD_MAX ( fBottomLeft.x, fBottomRight.x ) );
    KDfloat  fMinY = KD_MIN ( KD_MIN ( fTopLeft.y, fTopRight.y ), KD_MIN ( fBottomLeft.y, fBottomRight.y ) );
    KDfloat  fMaxY = KD_MAX ( KD_MAX ( fTopLeft.y, fTopRight.y ), KD_MAX ( fBottomLeft.y, fBottomRight.y ) );
        
    return CCRect ( fMinX, fMinY, fMaxX - fMinX, fMaxY - fMinY );
}

CCAffineTransform CCAffineTransformTranslate ( const CCAffineTransform& tT, KDfloat fTX, KDfloat fTY )
{
	return CCAffineTransformMake ( tT.a, tT.b, tT.c, tT.d, 
                                   tT.tx + tT.a * fTX + tT.c * fTY, 
                                   tT.ty + tT.b * fTX + tT.d * fTY );
}

CCAffineTransform CCAffineTransformScale ( const CCAffineTransform& tT, KDfloat fSX, KDfloat fSY )
{
	return CCAffineTransformMake ( tT.a * fSX, tT.b * fSX, tT.c * fSY, tT.d * fSY, tT.tx, tT.ty );
}

CCAffineTransform CCAffineTransformRotate ( const CCAffineTransform& tT, KDfloat fAngle )
{
    KDfloat  fSin = kdSinf ( fAngle );
    KDfloat  fCos = kdCosf ( fAngle );

	return CCAffineTransformMake ( tT.a * fCos + tT.c * fSin,
								   tT.b * fCos + tT.d * fSin,
								   tT.c * fCos - tT.a * fSin,
								   tT.d * fCos - tT.b * fSin,
								   tT.tx, tT.ty );
}

// Concatenate `t2' to `t1' and return the result : t' = t1 * t2 
CCAffineTransform CCAffineTransformConcat ( const CCAffineTransform& tT1, const CCAffineTransform& tT2 )
{
	return CCAffineTransformMake ( tT1.a  * tT2.a + tT1.b  * tT2.c,
                                   tT1.a  * tT2.b + tT1.b  * tT2.d,  
								   tT1.c  * tT2.a + tT1.d  * tT2.c, 
                                   tT1.c  * tT2.b + tT1.d  * tT2.d, 
								   tT1.tx * tT2.a + tT1.ty * tT2.c + tT2.tx,				    
								   tT1.tx * tT2.b + tT1.ty * tT2.d + tT2.ty );				
}

// Return true if `t1' and `t2' are equal, false otherwise. 
KDbool CCAffineTransformEqualToTransform ( const CCAffineTransform& tT1, const CCAffineTransform& tT2 )
{
	return ( tT1.a == tT2.a  && tT1.b == tT2.b  && tT1.c == tT2.c  && tT1.d == tT2.d  && tT1.tx == tT2.tx && tT1.ty == tT2.ty ) ? KD_TRUE : KD_FALSE;
}

CCAffineTransform CCAffineTransformInvert ( const CCAffineTransform& tT )
{
    KDfloat  fDeterminant = 1.0f / ( tT.a * tT.d - tT.b * tT.c );

    return CCAffineTransformMake ( fDeterminant * tT.d, 
                                  -fDeterminant * tT.b, 
                                  -fDeterminant * tT.c, 
                                   fDeterminant * tT.a,
							       fDeterminant * ( tT.c * tT.ty - tT.d * tT.tx ),
                                   fDeterminant * ( tT.b * tT.tx - tT.a * tT.ty ) );
}

NS_CC_END