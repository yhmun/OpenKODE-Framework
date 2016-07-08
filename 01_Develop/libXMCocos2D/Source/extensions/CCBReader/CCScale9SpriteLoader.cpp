/* --------------------------------------------------------------------------
 *
 *      File            CCScale9SpriteLoader.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org   
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCBReader/CCScale9SpriteLoader.h"

NS_CC_BEGIN

#define PROPERTY_CONTENTSIZE	"contentSize"
#define PROPERTY_SPRITEFRAME	"spriteFrame"
#define PROPERTY_COLOR			"color"
#define PROPERTY_OPACITY		"opacity"
#define PROPERTY_BLENDFUNC		"blendFunc"
#define PROPERTY_PREFEREDSIZE	"preferedSize" // TODO Should be "preferredSize". This is a typo in cocos2d-iphone, cocos2d-x and CocosBuilder!
#define PROPERTY_INSETLEFT		"insetLeft"
#define PROPERTY_INSETTOP		"insetTop"
#define PROPERTY_INSETRIGHT		"insetRight"
#define PROPERTY_INSETBOTTOM	"insetBottom"

KDvoid CCScale9SpriteLoader::onHandlePropTypeSpriteFrame ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSpriteFrame* pCCSpriteFrame, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_SPRITEFRAME ) == 0 ) 
	{
        ( (CCScale9Sprite*) pNode )->setSpriteFrame ( pCCSpriteFrame );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeSpriteFrame ( pNode, pParent, pPropertyName, pCCSpriteFrame, pCCBReader );
    }
}

KDvoid CCScale9SpriteLoader::onHandlePropTypeColor3 ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B pCCColor3B, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_COLOR ) == 0 )
	{
        ( (CCScale9Sprite*) pNode )->setColor ( pCCColor3B );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeColor3 ( pNode, pParent, pPropertyName, pCCColor3B, pCCBReader );
    }
}

KDvoid CCScale9SpriteLoader::onHandlePropTypeByte ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte pByte, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_OPACITY ) == 0 ) 
	{
        ( (CCScale9Sprite*) pNode )->setOpacity ( pByte );
    } 
	else 
	{
        CCNodeLoader::onHandlePropTypeByte ( pNode, pParent, pPropertyName, pByte, pCCBReader );
    }
}

KDvoid CCScale9SpriteLoader::onHandlePropTypeBlendFunc ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_BLENDFUNC ) == 0 )
	{
        // TODO Not exported by CocosBuilder yet!
        // ( (CCScale9Sprite*) pNode )->setBlendFunc ( pCCBlendFunc );
    } 
	else 
	{
        CCNodeLoader::onHandlePropTypeBlendFunc ( pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader );
    }
}

KDvoid CCScale9SpriteLoader::onHandlePropTypeSize ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSize pSize, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_CONTENTSIZE ) == 0 )
	{
        //( (CCScale9Sprite*) pNode )->setContentSize ( pSize );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_PREFEREDSIZE ) == 0 )
	{
        ( (CCScale9Sprite*) pNode )->setPreferredSize ( pSize );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeSize ( pNode, pParent, pPropertyName, pSize, pCCBReader );
    }
}

KDvoid CCScale9SpriteLoader::onHandlePropTypeFloat ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat pFloat, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_INSETLEFT ) == 0 )
	{
        ( (CCScale9Sprite*) pNode )->setInsetLeft ( pFloat );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_INSETTOP ) == 0 ) 
	{
        ( (CCScale9Sprite*) pNode )->setInsetTop ( pFloat );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_INSETRIGHT ) == 0 ) 
	{
        ( (CCScale9Sprite*) pNode )->setInsetRight ( pFloat );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_INSETBOTTOM ) == 0 ) 
	{
        ( (CCScale9Sprite*) pNode )->setInsetBottom ( pFloat );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeFloat ( pNode, pParent, pPropertyName, pFloat, pCCBReader );
    }
}

NS_CC_END