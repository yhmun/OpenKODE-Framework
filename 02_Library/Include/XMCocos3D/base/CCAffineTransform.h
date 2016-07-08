/* -----------------------------------------------------------------------------------
 *
 *      File            CCAffineTransform.h
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

#ifndef __CCAffineTransform_h__
#define __CCAffineTransform_h__

#include "CCGeometry.h"
#include "../platform/CCPlatformMacros.h"

NS_CC_BEGIN

struct AffineTransform 
{
    KDfloat		a, b, c, d;
    KDfloat		tx, ty;

    static const AffineTransform IDENTITY;
};

CC_DLL AffineTransform	AffineTransformMake ( KDfloat a, KDfloat b, KDfloat c, KDfloat d, KDfloat tx, KDfloat ty );

CC_DLL Point			PointApplyAffineTransform ( const Point& tPoint, const AffineTransform& t );

CC_DLL Size				SizeApplyAffineTransform ( const Size& tSize, const AffineTransform& t );

CC_DLL AffineTransform	AffineTransformMakeIdentity ( KDvoid );

CC_DLL Rect				RectApplyAffineTransform ( const Rect& tRect, const AffineTransform& tAnAffineTransform );

CC_DLL AffineTransform	AffineTransformTranslate ( const AffineTransform& t, KDfloat tx, KDfloat ty );

CC_DLL AffineTransform	AffineTransformRotate ( const AffineTransform& aTransform, KDfloat anAngle );

CC_DLL AffineTransform	AffineTransformScale ( const AffineTransform& t, KDfloat sx, KDfloat sy );

CC_DLL AffineTransform	AffineTransformConcat ( const AffineTransform& t1, const AffineTransform& t2 );

CC_DLL KDbool			AffineTransformEqualToTransform ( const AffineTransform& t1, const AffineTransform& t2 );

CC_DLL AffineTransform	AffineTransformInvert ( const AffineTransform& t );

extern CC_DLL const AffineTransform		AffineTransformIdentity;

NS_CC_END

#endif	// __CCAffineTransform_h__
