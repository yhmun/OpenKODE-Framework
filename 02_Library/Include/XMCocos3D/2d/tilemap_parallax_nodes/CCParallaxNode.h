/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCParallaxNode_h__
#define __CCParallaxNode_h__

#include "../base_nodes/CCNode.h"

NS_CC_BEGIN

struct _ccArray;

/**
 *	@addtogroup tilemap_parallax_nodes
 *	@{
 */

/** 
 *	@brief ParallaxNode: A node that simulates a parallax scroller
 *
 *	The children will be moved faster / slower than the parent according the the parallax ratio.
 *
 */
class CC_DLL ParallaxNode : public Node 
{
	public :

		// Create a Parallax node
		static ParallaxNode*	create ( KDvoid );
    
		/** 
		 *	Adds a child to the container with a z-order, a parallax ratio and a position offset
		 *	It returns self, so you can chain several addChilds.
		 *	@since v0.8
		 *	@js ctor
		 */
		ParallaxNode ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParallaxNode ( KDvoid );

		// prevents compiler warning: "Included function hides overloaded virtual functions"
		using Node::addChild;

		KDvoid					addChild ( Node* pChild, KDint z, const Point& tParallaxRatio, const Point& tPositionOffset );

		/** Sets an array of layers for the Parallax node */
		KDvoid					setParallaxArray ( struct _ccArray* pParallaxArray ) { m_pParallaxArray = pParallaxArray; }

		/** Returns the array of layers of the Parallax node */
		struct _ccArray*		getParallaxArray ( KDvoid ) { return m_pParallaxArray; }
		const struct _ccArray*	getParallaxArray ( KDvoid ) const { return m_pParallaxArray; }

		//
		// Overrides
		//
		virtual KDvoid			addChild ( Node* pChild, KDint zOrder, KDint nTag ) override;
		virtual KDvoid			removeChild ( Node* pChild, KDbool bCleanup ) override;
		virtual KDvoid			removeAllChildrenWithCleanup ( KDbool bCleanup ) override;
		virtual KDvoid			visit ( KDvoid ) override;

	protected :

		Point					absolutePosition ( KDvoid );

		Point					m_tLastPosition;
		struct _ccArray*		m_pParallaxArray;
};

//	end of tilemap_parallax_nodes group
/// @}

NS_CC_END

#endif	// __CCParallaxNode_h__


