/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlButtonLoader.h
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

#ifndef __CCControlButtonLoader_h__
#define __CCControlButtonLoader_h__

#include "CCControlLoader.h"
#include "../../extensions/GUI/CCControlExtension/CCControlButton.h"

namespace cocosbuilder {

/* Forward declaration. */
class CCBReader;

class ControlButtonLoader : public ControlLoader 
{
    public :

        /**
         *	@js NA
         *	@lua NA
         */
        virtual ~ControlButtonLoader ( KDvoid ) { }

        /**
         *	@js NA
         *	@lua NA
         */
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( ControlButtonLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( cocos2d::extension::ControlButton );

        virtual KDvoid		onHandlePropTypeCheck		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDbool					pCheck		, CCBReader* ccbReader );
        virtual KDvoid		onHandlePropTypeString		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, const KDchar*			pString		, CCBReader* ccbReader );
        virtual KDvoid		onHandlePropTypeFontTTF		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, const KDchar*			pFontTTF	, CCBReader* ccbReader );
        virtual KDvoid		onHandlePropTypeFloatScale	( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDfloat				pFloatScale	, CCBReader* ccbReader );
        virtual KDvoid		onHandlePropTypePoint		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Point			pPoint		, CCBReader* ccbReader );
        virtual KDvoid		onHandlePropTypeSize		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Size			pSize		, CCBReader* ccbReader );
        virtual KDvoid		onHandlePropTypeSpriteFrame ( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::SpriteFrame*	pSpriteFrame, CCBReader* ccbReader );
        virtual KDvoid		onHandlePropTypeColor3		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Color3B		pColor3B	, CCBReader* ccbReader );
};

}

#endif	// __CCControlButtonLoader_h__
