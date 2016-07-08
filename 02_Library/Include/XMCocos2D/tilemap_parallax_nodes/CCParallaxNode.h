/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCParallaxNode_h__
#define __CCParallaxNode_h__

#include "../base_nodes/CCNode.h"

NS_CC_BEGIN
    
/**
 * @addtogroup tilemap_parallax_nodes
 * @{
 */

struct _ccArray;

/** 
 *	@brief CCParallaxNode: A node that simulates a parallax scroller
 *
 *	The children will be moved faster / slower than the parent according the the parallax ratio.
 *
 */
class CCParallaxNode : public CCNode 
{
	public :
		         CCParallaxNode ( KDvoid );
		virtual ~CCParallaxNode ( KDvoid );

		static   CCParallaxNode*	create ( KDvoid );

	public :

		/**
		 *	Adds a child to the container with a z-order, a parallax ratio and a position offset
		 *	It returns self, so you can chain several addChilds.
		 *	@since	v0.8
		 */
		virtual KDvoid				addChild ( CCNode* pChild, KDuint nZOrder, const CCPoint& tParallaxRatio, const CCPoint& tPositionOffset );

		// super methods
		virtual KDvoid				addChild ( CCNode* pChild, KDuint nZOrder, KDint nTag );
		virtual KDvoid				removeChild ( CCNode* pChild, KDbool bCleanup );
		virtual KDvoid				removeAllChildrenWithCleanup ( KDbool bCleanup );
		virtual KDvoid				visit ( KDvoid );

		/** array that holds the offset / ratio of the children */
		CC_SYNTHESIZE ( struct _ccArray*, m_pParallaxArray, ParallaxArray )
 
		// Extension API
		virtual	KDvoid				incrementOffset ( CCPoint tOffset, CCNode* pNode ); 

	protected :

		CCPoint						absolutePosition ( KDvoid );

	protected :

		CCPoint						m_tLastPosition;
};		

// end of tilemap_parallax_nodes group
/// @}

NS_CC_END

#endif // __CCParallaxNode_h__


