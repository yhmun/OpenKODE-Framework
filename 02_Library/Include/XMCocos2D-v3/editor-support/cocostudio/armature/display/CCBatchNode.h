/* -----------------------------------------------------------------------------------
 *
 *      File            CCBatchNode.h
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

#ifndef __CCBatchNode_h__
#define __CCBatchNode_h__

#include "../utils/CCArmatureDefine.h"
#include "../../../../2d/base_nodes/CCNode.h"
#include "../../../../2d/textures/CCTextureAtlas.h"

namespace cocostudio {

class BatchNode : public cocos2d::Node
{
	public :

		static BatchNode*				create ( KDvoid );

	public :

		/**
		 *	@js ctor
		 */
		BatchNode ( KDvoid );
		~BatchNode ( KDvoid );

		virtual KDbool					init ( KDvoid ) override;

		virtual KDvoid					addChild ( cocos2d::Node* pChild ) override;
		virtual KDvoid					addChild ( cocos2d::Node* pChild, KDint zOrder ) override;
		virtual KDvoid					addChild ( cocos2d::Node* pChild, KDint zOrder, KDint nTag ) override;

		virtual KDvoid					removeChild ( cocos2d::Node* pChild, KDbool bCleanup ) override;

		virtual KDvoid					visit ( KDvoid ) override;
		KDvoid							draw ( KDvoid ) override;

		virtual cocos2d::TextureAtlas*	getTexureAtlasWithTexture ( cocos2d::Texture2D* pTexture ) const;

	protected :

		cocos2d::TextureAtlas*			m_pAtlas;
		cocos2d::Dictionary*			m_pTextureAtlasDic;
};

}

#endif	// __CCBatchNode_h__
