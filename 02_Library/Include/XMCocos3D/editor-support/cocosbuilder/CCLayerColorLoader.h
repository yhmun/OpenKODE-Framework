/* -----------------------------------------------------------------------------------
 *
 *      File            CCLayerColorLoader.h
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

#ifndef __CCLayerColorLoader_h__
#define __CCLayerColorLoader_h__

#include "CCLayerLoader.h"

namespace cocosbuilder {

/* Forward declaration. */
class CCBReader;

class LayerColorLoader : public LayerLoader 
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~LayerColorLoader ( KDvoid ) { }

		CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( LayerColorLoader, loader );

	protected :

		CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( cocos2d::LayerColor );

		virtual KDvoid		onHandlePropTypeColor3		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Color3B	pColor3B  , CCBReader* ccbReader );
		virtual KDvoid		onHandlePropTypeByte		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDubyte			pByte	  , CCBReader* ccbReader );
		virtual KDvoid		onHandlePropTypeBlendFunc	( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::BlendFunc pBlendFunc, CCBReader* ccbReader );
};

}

#endif	// __CCLayerColorLoader_h__
