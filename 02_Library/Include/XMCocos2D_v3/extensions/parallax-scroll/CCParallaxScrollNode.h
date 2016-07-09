/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxScrollNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      Created by Thomas Broquist on 3/26/12. 
 *      Copyright (c) 2012      Little Wins LLC. All rights reserved.
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

#ifndef __CCParallaxScrollNode_h__
#define __CCParallaxScrollNode_h__

#include "CCParallaxScrollOffset.h"
#include "../../2d/sprite_nodes/CCSprite.h"
#include "../../2d/sprite_nodes/CCSpriteBatchNode.h"

NS_CC_EXT_BEGIN


class ParallaxScrollNode : public Layer
{
	public :
				 ParallaxScrollNode ( KDvoid );
		virtual ~ParallaxScrollNode ( KDvoid );

		static ParallaxScrollNode*	create ( KDvoid );

	public :

		virtual KDbool				init ( KDvoid );
	    
		KDvoid						addChild ( Sprite* pNode, KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tOffset );
		KDvoid						addChild ( Sprite* pNode, KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tOffset, const Point& tVelocity );
	    
		KDvoid						removeChild ( Sprite* pNode, KDbool bCleanup );

		KDvoid						updateWithVelocity ( const Point& tVelocity, KDfloat fDelta );
		KDvoid						updateWithYPosition ( KDfloat fPosY, KDfloat fDelta );
	    
		KDvoid						addInfiniteScrollWithZ  ( KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tDir, Sprite* pObject, ... );
		KDvoid						addInfiniteScrollXWithZ ( KDint nZOrder, const Point& tRatio, const Point& tPosition, Sprite* pObject, ... );
		KDvoid						addInfiniteScrollYWithZ ( KDint nZOrder, const Point& tRatio, const Point& tPosition, Sprite* pObject, ... );
	    

		KDvoid						addInfiniteScrollWithObjects ( Array* pObjects, KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tDir );
		KDvoid						addInfiniteScrollWithObjects ( Array* pObjects, KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tDir, const Point& tVelocity );
		KDvoid						addInfiniteScrollWithObjects ( Array* pObjects, KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tDir, const Point& tVelocity, const Point& tPadding );
	    
	protected :

		Array*						m_pScrollOffsets;
};

NS_CC_EXT_END

#endif	// __CCParallaxScrollNode_h__
