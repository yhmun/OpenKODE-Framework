/* -----------------------------------------------------------------------------------
 *
 *      File            CCClippingNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Pierre-David Bélanger
 *
 *         http://www.cocos2d-iphone.org
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

#ifndef __CCClippingNode_h__
#define __CCClippingNode_h__

#include "../base_nodes/CCNode.h"
#include "../../platform/CCGL.h"

NS_CC_BEGIN

/**
 *	ClippingNode is a subclass of Node.
 *	It draws its content (childs) clipped using a stencil.
 *	The stencil is an other Node that will not be drawn.
 *	The clipping is done using the alpha part of the stencil (adjusted with an alphaThreshold).
 */
class CC_DLL ClippingNode : public Node
{
	public :

		/** 
		 *	Creates and initializes a clipping node without a stencil.
		 */
		static ClippingNode*	create ( KDvoid );
    
		/** 
		 *	Creates and initializes a clipping node with an other node as its stencil.
		 *	The stencil node will be retained.
		 */
		static ClippingNode*	create ( Node* pStencil );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ClippingNode ( KDvoid );
    
		/** 
		 *	Initializes a clipping node without a stencil.
		 */
		virtual KDbool			init ( KDvoid );
    
		/** 
		 *	Initializes a clipping node with an other node as its stencil.
		 *	The stencil node will be retained, and its parent will be set to this clipping node.
		 */
		virtual KDbool			init ( Node* pStencil );
    
		/**
		 *	The Node to use as a stencil to do the clipping.
		 *	The stencil node will be retained.
		 *	This default to nil.
		 */
		Node*					getStencil ( KDvoid ) const;
		KDvoid					setStencil ( Node* pStencil );
    
		/** 
		 *	The alpha threshold.
		 *	The content is drawn only where the stencil have pixel with alpha greater than the alphaThreshold.
		 *	Should be a float between 0 and 1.
		 *	This default to 1 (so alpha test is disabled).
		 */
		GLfloat					getAlphaThreshold ( KDvoid ) const;
		KDvoid					setAlphaThreshold ( GLfloat fAlphaThreshold );
    
		/** 
		 *	Inverted. If this is set to true,
		 *	the stencil is inverted, so the content is drawn where the stencil is NOT drawn.
		 *	This default to false.
		 */
		KDbool					 isInverted ( KDvoid ) const;
		KDvoid					setInverted ( KDbool bInverted );

		// Overrides

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			onEnter ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			onEnterTransitionDidFinish ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			onExitTransitionDidStart ( KDvoid ) override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			onExit ( KDvoid ) override;

		virtual KDvoid			visit ( KDvoid ) override;

	private :

		/**
		 *	draw fullscreen quad to clear stencil bits
		 */
		KDvoid					drawFullScreenQuadClearStencil ( KDvoid );

	protected :

		ClippingNode ( KDvoid );

	protected :

		Node*					m_pStencil;
		GLfloat					m_fAlphaThreshold;
		KDbool					m_bInverted;
};

NS_CC_END

#endif	// __CCClippingNode_h__
