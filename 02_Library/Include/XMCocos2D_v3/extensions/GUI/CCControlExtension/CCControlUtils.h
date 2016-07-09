/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlUtils.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale.
 *      Copyright (c) 2011      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://creativewax.co.uk
 *          http://yannickloriot.com
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCControlUtils_h__
#define __CCControlUtils_h__

#include "../../../2d/sprite_nodes/CCSprite.h"
#include "../../ExtensionMacros.h"

NS_CC_EXT_BEGIN

typedef struct
{
    KDdouble	r;       // percent
    KDdouble	g;       // percent
    KDdouble	b;       // percent
    KDdouble	a;       // percent
} RGBA;

typedef struct
{
    KDdouble	h;       // angle in degrees
    KDdouble	s;       // percent
    KDdouble	v;       // percent
} HSV;

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

// helper class to store Color3B's in mutable arrays
class Color3bObject : public Object
{
	public :

		Color3B				m_tValue;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		Color3bObject ( Color3B tValue ) : m_tValue ( tValue ) { }
};

class ControlUtils
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static Sprite*		addSpriteToTargetWithPosAndAnchor ( const KDchar* pSpriteName, Node* pTarget, Point tPos, Point tAnchor );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static HSV			HSVfromRGB ( RGBA tValue );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static RGBA			RGBfromHSV ( HSV tValue );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static Rect			RectUnion ( const Rect& tSrc1, const Rect& tSrc2 );
};

//	end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControlUtils_h__