/* --------------------------------------------------------------------------
 *
 *      File            CCLabelTTFLoader.cpp
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
#include "extensions/CCBReader/CCLabelTTFLoader.h"

NS_CC_BEGIN

#define PROPERTY_COLOR					"color"
#define PROPERTY_OPACITY				"opacity"
#define PROPERTY_BLENDFUNC				"blendFunc"
#define PROPERTY_FONTNAME				"fontName"
#define PROPERTY_FONTSIZE				"fontSize"
#define PROPERTY_HORIZONTALALIGNMENT	"horizontalAlignment"
#define PROPERTY_VERTICALALIGNMENT		"verticalAlignment"
#define PROPERTY_STRING					"string"
#define PROPERTY_DIMENSIONS				"dimensions"

KDvoid CCLabelTTFLoader::onHandlePropTypeColor3 ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B pCCColor3B, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_COLOR ) == 0 )
	{
        ( (CCLabelTTF*) pNode )->setColor ( pCCColor3B );
    } 
	else
	{
        CCNodeLoader::onHandlePropTypeColor3 ( pNode, pParent, pPropertyName, pCCColor3B, pCCBReader );
    }
}

KDvoid CCLabelTTFLoader::onHandlePropTypeByte ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte pByte, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_OPACITY ) == 0 )
	{
        ( (CCLabelTTF*) pNode )->setOpacity ( pByte );
    } 
	else
	{
        CCNodeLoader::onHandlePropTypeByte ( pNode, pParent, pPropertyName, pByte, pCCBReader );
    }
}

KDvoid CCLabelTTFLoader::onHandlePropTypeBlendFunc ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_BLENDFUNC ) == 0 )
	{
        ( (CCLabelTTF*) pNode )->setBlendFunc ( pCCBlendFunc );
    } 
	else 
	{
        CCNodeLoader::onHandlePropTypeBlendFunc ( pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader );
    }
}

KDvoid CCLabelTTFLoader::onHandlePropTypeFontTTF ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pFontTTF, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_FONTNAME ) == 0 )
	{
        ( (CCLabelTTF*) pNode )->setFontName ( pFontTTF );
    }
	else
	{
        CCNodeLoader::onHandlePropTypeFontTTF ( pNode, pParent, pPropertyName, pFontTTF, pCCBReader );
    }
}

KDvoid CCLabelTTFLoader::onHandlePropTypeText ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pText, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_STRING ) == 0 )
	{
        ( (CCLabelTTF*) pNode )->setString ( pText );
    } 
	else
	{
        CCNodeLoader::onHandlePropTypeText ( pNode, pParent, pPropertyName, pText, pCCBReader );
    }
}

KDvoid CCLabelTTFLoader::onHandlePropTypeFloatScale ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat pFloatScale, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_FONTSIZE ) == 0 ) 
	{
        ( (CCLabelTTF*) pNode )->setFontSize ( pFloatScale );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeFloatScale ( pNode, pParent, pPropertyName, pFloatScale, pCCBReader );
    }
}

KDvoid CCLabelTTFLoader::onHandlePropTypeIntegerLabeled ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDint pIntegerLabeled, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_HORIZONTALALIGNMENT ) == 0 ) 
	{
        ( (CCLabelTTF*) pNode )->setHorizontalAlignment ( CCTextAlignment ( pIntegerLabeled ) );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_VERTICALALIGNMENT ) == 0 ) 
	{
        ( (CCLabelTTF*) pNode )->setVerticalAlignment ( CCVerticalTextAlignment ( pIntegerLabeled ) );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeFloatScale ( pNode, pParent, pPropertyName, (KDfloat) pIntegerLabeled, pCCBReader );
    }
}

KDvoid CCLabelTTFLoader::onHandlePropTypeSize ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSize pSize, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_DIMENSIONS ) == 0 )
	{
        ( (CCLabelTTF*) pNode )->setDimensions ( pSize );
    } 
	else 
	{
        CCNodeLoader::onHandlePropTypeSize ( pNode, pParent, pPropertyName, pSize, pCCBReader );
    }
}

NS_CC_END