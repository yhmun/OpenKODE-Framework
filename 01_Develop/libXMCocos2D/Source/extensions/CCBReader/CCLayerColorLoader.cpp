/* --------------------------------------------------------------------------
 *
 *      File            CCLayerColorLoader.cpp
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
#include "extensions/CCBReader/CCLayerColorLoader.h"

NS_CC_BEGIN

#define PROPERTY_COLOR			"color"
#define PROPERTY_OPACITY		"opacity"
#define PROPERTY_BLENDFUNC		"blendFunc"

KDvoid CCLayerColorLoader::onHandlePropTypeColor3 ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B pCCColor3B, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_COLOR ) == 0 )
	{
        ( (CCLayerColor*) pNode )->setColor ( pCCColor3B );
    } 
	else
	{
        CCLayerLoader::onHandlePropTypeColor3 ( pNode, pParent, pPropertyName, pCCColor3B, pCCBReader );
    }
}

KDvoid CCLayerColorLoader::onHandlePropTypeByte ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte pByte, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_OPACITY ) == 0 )
	{
        ( (CCLayerColor*) pNode )->setOpacity ( pByte );
    } 
	else 
	{
        CCLayerLoader::onHandlePropTypeByte ( pNode, pParent, pPropertyName, pByte, pCCBReader );
    }
}

KDvoid CCLayerColorLoader::onHandlePropTypeBlendFunc ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_BLENDFUNC ) == 0 )
	{
        ( (CCLayerColor*) pNode )->setBlendFunc ( pCCBlendFunc );
    } 
	else
	{
        CCLayerLoader::onHandlePropTypeBlendFunc ( pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader );
    }
}

NS_CC_END