/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxScrollNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
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
#include "../../sprite_nodes/CCSprite.h"
#include "../../sprite_nodes/CCSpriteBatchNode.h"

NS_CC_BEGIN


class CCParallaxScrollNode : public CCLayer
{
	public :
				 CCParallaxScrollNode ( KDvoid );
		virtual ~CCParallaxScrollNode ( KDvoid );

		static CCParallaxScrollNode*	create ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );
	    
		KDvoid				addChild ( CCSprite* pNode, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tOffset );
		KDvoid				addChild ( CCSprite* pNode, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tOffset, const CCPoint& tVelocity );
	    
		KDvoid				removeChild ( CCSprite* pNode, KDbool bCleanup );

		KDvoid				updateWithVelocity ( const CCPoint& tVelocity, KDfloat fDelta );
		KDvoid				updateWithYPosition ( KDfloat fPosY, KDfloat fDelta );
	    
		KDvoid				addInfiniteScrollWithZ  ( KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tDir, CCSprite* pObject, ... );
		KDvoid				addInfiniteScrollXWithZ ( KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, CCSprite* pObject, ... );
		KDvoid				addInfiniteScrollYWithZ ( KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, CCSprite* pObject, ... );
	    

		KDvoid				addInfiniteScrollWithObjects ( CCArray* pObjects, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tDir );
		KDvoid				addInfiniteScrollWithObjects ( CCArray* pObjects, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tDir, const CCPoint& tVelocity );
		KDvoid				addInfiniteScrollWithObjects ( CCArray* pObjects, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tDir, const CCPoint& tVelocity, const CCPoint& tPadding );
	    
	protected :

		CCArray*			m_pScrollOffsets;
};

NS_CC_END

#endif	// __CCParallaxScrollNode_h__
