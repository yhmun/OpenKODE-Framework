/* --------------------------------------------------------------------------
 *
 *      File            CCLabelBMFontLoader.cpp
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
#include "extensions/CCBReader/CCLabelBMFontLoader.h"

NS_CC_BEGIN

#define PROPERTY_COLOR				"color"
#define PROPERTY_OPACITY			"opacity"
#define PROPERTY_BLENDFUNC			"blendFunc"
#define PROPERTY_FNTFILE			"fntFile"
#define PROPERTY_STRING				"string"

KDvoid CCLabelBMFontLoader::onHandlePropTypeColor3 ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B pCCColor3B, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_COLOR ) == 0 )
	{
        ( (CCLabelBMFont*) pNode )->setColor ( pCCColor3B );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeColor3 ( pNode, pParent, pPropertyName, pCCColor3B, pCCBReader );
    }
}

KDvoid CCLabelBMFontLoader::onHandlePropTypeByte ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte pByte, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_OPACITY ) == 0 )
	{
        ( (CCLabelBMFont*) pNode )->setOpacity ( pByte );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeByte ( pNode, pParent, pPropertyName, pByte, pCCBReader );
    }
}

KDvoid CCLabelBMFontLoader::onHandlePropTypeBlendFunc ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_BLENDFUNC ) == 0 )
	{
        ( (CCLabelBMFont*) pNode )->setBlendFunc ( pCCBlendFunc );
    } 
	else 
	{
        CCNodeLoader::onHandlePropTypeBlendFunc ( pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader );
    }
}

KDvoid CCLabelBMFontLoader::onHandlePropTypeFntFile ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pFntFile, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_FNTFILE ) == 0 )
	{
        ( (CCLabelBMFont*) pNode )->setFntFile ( pFntFile );
    } 
	else 
	{
        CCNodeLoader::onHandlePropTypeFntFile ( pNode, pParent, pPropertyName, pFntFile, pCCBReader );
    }
}

KDvoid CCLabelBMFontLoader::onHandlePropTypeText ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pText, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_STRING ) == 0 )
	{
        ( (CCLabelBMFont*) pNode )->setString ( pText );
    }
	else
	{
        CCNodeLoader::onHandlePropTypeText ( pNode, pParent, pPropertyName, pText, pCCBReader );
    }
}

NS_CC_END