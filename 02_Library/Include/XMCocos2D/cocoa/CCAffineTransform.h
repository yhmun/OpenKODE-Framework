/* -----------------------------------------------------------------------------------
 *
 *      File            CCAffineTransform.h
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

#ifndef __CCAffineTransform_h__
#define __CCAffineTransform_h__

#include "CCGeometry.h"

NS_CC_BEGIN
    
struct CCAffineTransform 
{
	KDfloat     a, b, c, d;
	KDfloat     tx, ty;
};

CCAffineTransform	CCAffineTransformMake ( KDfloat a, KDfloat b, KDfloat c, KDfloat d, KDfloat tx, KDfloat ty );
	
CCPoint				CCPointApplyAffineTransform ( const CCPoint& tPoint, const CCAffineTransform& tTransform );

CCSize				CCSizeApplyAffineTransform ( const CCSize& tSize, const CCAffineTransform& tTransform );

CCAffineTransform	CCAffineTransformMakeIdentity ( KDvoid );

CCRect				CCRectApplyAffineTransform ( const CCRect& tRect, const CCAffineTransform& tTransform );

CCAffineTransform	CCAffineTransformTranslate ( const CCAffineTransform& tTransform, KDfloat fTranslateX, KDfloat fTranslateY );

CCAffineTransform	CCAffineTransformRotate ( const CCAffineTransform& tTransform, KDfloat fAngle );

CCAffineTransform	CCAffineTransformScale ( const CCAffineTransform& tTransform, KDfloat fScaleX, KDfloat fScaleY );

CCAffineTransform	CCAffineTransformConcat ( const CCAffineTransform& tTransform1, const CCAffineTransform& tTransform2 );

KDbool				CCAffineTransformEqualToTransform ( const CCAffineTransform& tTransform1, const CCAffineTransform& tTransform2 );

CCAffineTransform	CCAffineTransformInvert ( const CCAffineTransform& tTransform );

extern const CCAffineTransform     CCAffineTransformIdentity;

NS_CC_END

#endif // __CCAffineTransform_h__
