/* --------------------------------------------------------------------------
 *
 *      File            CCLayerGradientLoader.cpp
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
#include "extensions/CCBReader/CCLayerGradientLoader.h"

NS_CC_BEGIN

#define PROPERTY_STARTCOLOR			"startColor"
#define PROPERTY_ENDCOLOR			"endColor"
#define PROPERTY_STARTOPACITY		"startOpacity"
#define PROPERTY_ENDOPACITY			"endOpacity"
#define PROPERTY_VECTOR				"vector"
#define PROPERTY_BLENDFUNC			"blendFunc"

KDvoid CCLayerGradientLoader::onHandlePropTypeColor3 ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B pCCColor3B, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_STARTCOLOR ) == 0 ) 
	{
        ( (CCLayerGradient*) pNode )->setStartColor ( pCCColor3B );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ENDCOLOR ) == 0 ) 
	{
        ( (CCLayerGradient*) pNode )->setEndColor ( pCCColor3B );
    } 
	else 
	{
        CCLayerLoader::onHandlePropTypeColor3 ( pNode, pParent, pPropertyName, pCCColor3B, pCCBReader );
    }
}

KDvoid CCLayerGradientLoader::onHandlePropTypeByte ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte pByte, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_STARTOPACITY ) == 0 )
	{
        ( (CCLayerGradient*) pNode )->setStartOpacity ( pByte );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ENDOPACITY ) == 0 )
	{
        ( (CCLayerGradient*) pNode )->setEndOpacity ( pByte );
    } 
	else
	{
        CCLayerLoader::onHandlePropTypeByte ( pNode, pParent, pPropertyName, pByte, pCCBReader );
    }
}

KDvoid CCLayerGradientLoader::onHandlePropTypeBlendFunc ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_BLENDFUNC ) == 0 )
	{
        ( (CCLayerGradient*) pNode )->setBlendFunc ( pCCBlendFunc );
    } 
	else
	{
        CCLayerLoader::onHandlePropTypeBlendFunc ( pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader );
    }
}


KDvoid CCLayerGradientLoader::onHandlePropTypePoint ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint pPoint, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_VECTOR ) == 0 )
	{
        ( (CCLayerGradient*) pNode )->setVector ( pPoint );

        // TODO Not passed along the ccbi file.
        // ( (CCLayerGradient*) pNode )->setCompressedInterpolation(true);
    }
	else
	{
        CCLayerLoader::onHandlePropTypePoint ( pNode, pParent, pPropertyName, pPoint, pCCBReader );
    }
}

NS_CC_END