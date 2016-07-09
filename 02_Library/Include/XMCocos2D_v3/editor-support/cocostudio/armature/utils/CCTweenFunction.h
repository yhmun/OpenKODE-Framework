/* -----------------------------------------------------------------------------------
 *
 *      File            CCTweenFunction.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __CCTweenFunction_h__
#define __CCTweenFunction_h__

#include "CCArmatureDefine.h"

namespace cocostudio {

enum TweenType
{
    TWEEN_EASING_MIN = -1,

    Linear,

    Sine_EaseIn,
    Sine_EaseOut,
    Sine_EaseInOut,

    Quad_EaseIn,
    Quad_EaseOut,
    Quad_EaseInOut,

    Cubic_EaseIn,
    Cubic_EaseOut,
    Cubic_EaseInOut,

    Quart_EaseIn,
    Quart_EaseOut,
    Quart_EaseInOut,

    Quint_EaseIn,
    Quint_EaseOut,
    Quint_EaseInOut,

    Expo_EaseIn,
    Expo_EaseOut,
    Expo_EaseInOut,

    Circ_EaseIn,
    Circ_EaseOut,
    Circ_EaseInOut,

    Elastic_EaseIn,
    Elastic_EaseOut,
    Elastic_EaseInOut,

    Back_EaseIn,
    Back_EaseOut,
    Back_EaseInOut,

    Bounce_EaseIn,
    Bounce_EaseOut,
    Bounce_EaseInOut,

    TWEEN_EASING_MAX = 10000
};

class  TweenFunction
{
public:

    static KDfloat		tweenTo				( KDfloat from, KDfloat change, KDfloat time, KDfloat duration, TweenType tweenType );

    static KDfloat		linear				( KDfloat t, KDfloat b, KDfloat c, KDfloat d );

    static KDfloat		sineEaseIn			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		sineEaseOut			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		sineEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );

    static KDfloat		quadEaseIn			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		quadEaseOut			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		quadEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );

    static KDfloat		cubicEaseIn			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		cubicEaseOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		cubicEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );

    static KDfloat		quartEaseIn			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		quartEaseOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		quartEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );

    static KDfloat		quintEaseIn			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		quintEaseOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		quintEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );

    static KDfloat		expoEaseIn			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		expoEaseOut			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		expoEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );

    static KDfloat		circEaseIn			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		circEaseOut			( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		circEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );

    static KDfloat		elasticEaseIn		( KDfloat t, KDfloat b, KDfloat c, KDfloat d, KDfloat a = 0, KDfloat p = 0 );
    static KDfloat		elasticEaseOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d, KDfloat a = 0, KDfloat p = 0 );
    static KDfloat		elasticEaseInOut	( KDfloat t, KDfloat b, KDfloat c, KDfloat d, KDfloat a = 0, KDfloat p = 0 );

    static KDfloat		backEaseIn			( KDfloat t, KDfloat b, KDfloat c, KDfloat d, KDfloat s = 0 );
    static KDfloat		backEaseOut			( KDfloat t, KDfloat b, KDfloat c, KDfloat d, KDfloat s = 0 );
    static KDfloat		backEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d, KDfloat s = 0 );

    static KDfloat		bounceEaseIn		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		bounceEaseOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
    static KDfloat		bounceEaseInOut		( KDfloat t, KDfloat b, KDfloat c, KDfloat d );
};

}

#endif	// __CCTweenFunction_h__
