/* --------------------------------------------------------------------------
 *
 *      File            CCControlButtonLoader.cpp
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
#include "extensions/CCBReader/CCControlButtonLoader.h"

NS_CC_BEGIN

#define PROPERTY_ZOOMONTOUCHDOWN					"zoomOnTouchDown"
#define PROPERTY_TITLE_NORMAL						"title|1"
#define PROPERTY_TITLE_HIGHLIGHTED					"title|2"
#define PROPERTY_TITLE_DISABLED						"title|3"
#define PROPERTY_TITLECOLOR_NORMAL					"titleColor|1"
#define PROPERTY_TITLECOLOR_HIGHLIGHTED				"titleColor|2"
#define PROPERTY_TITLECOLOR_DISABLED				"titleColor|3"
#define PROPERTY_TITLETTF_NORMAL					"titleTTF|1"
#define PROPERTY_TITLETTF_HIGHLIGHTED				"titleTTF|2"
#define PROPERTY_TITLETTF_DISABLED					"titleTTF|3"
#define PROPERTY_TITLETTFSIZE_NORMAL				"titleTTFSize|1"
#define PROPERTY_TITLETTFSIZE_HIGHLIGHTED			"titleTTFSize|2"
#define PROPERTY_TITLETTFSIZE_DISABLED				"titleTTFSize|3"
#define PROPERTY_LABELANCHORPOINT					"labelAnchorPoint"
#define PROPERTY_PREFEREDSIZE						"preferedSize" // TODO Should be "preferredSize". This is a typo in cocos2d-iphone, cocos2d-x and CocosBuilder!
#define PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL		"backgroundSpriteFrame|1"
#define PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED	"backgroundSpriteFrame|2"
#define PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED		"backgroundSpriteFrame|3"

KDvoid CCControlButtonLoader::onHandlePropTypeCheck ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool pCheck, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_ZOOMONTOUCHDOWN ) == 0 ) 
	{
        ( (CCControlButton*) pNode )->setZoomOnTouchDown ( pCheck );
    } 
	else
	{
        CCControlLoader::onHandlePropTypeCheck ( pNode, pParent, pPropertyName, pCheck, pCCBReader );
    }
}

KDvoid CCControlButtonLoader::onHandlePropTypeString ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pString, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_TITLE_NORMAL ) == 0 ) 
	{
        ( (CCControlButton*) pNode )->setTitleForState ( CCString::create ( pString ), kCCControlStateNormal );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TITLE_HIGHLIGHTED ) == 0 )
	{
        ( (CCControlButton*) pNode )->setTitleForState ( CCString::create ( pString ), kCCControlStateHighlighted );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TITLE_DISABLED ) == 0 ) 
	{
        ( (CCControlButton*) pNode )->setTitleForState ( CCString::create ( pString ), kCCControlStateDisabled );
    }
	else
	{
        CCControlLoader::onHandlePropTypeString ( pNode, pParent, pPropertyName, pString, pCCBReader );
    }
}

KDvoid CCControlButtonLoader::onHandlePropTypeFontTTF ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pFontTTF, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_TITLETTF_NORMAL ) == 0 )
	{
        ( (CCControlButton*) pNode )->setTitleTTFForState ( pFontTTF, kCCControlStateNormal );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TITLETTF_HIGHLIGHTED ) == 0 ) 
	{
        ( (CCControlButton*) pNode )->setTitleTTFForState ( pFontTTF, kCCControlStateHighlighted );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TITLETTF_DISABLED ) == 0 )
	{
        ( (CCControlButton*) pNode )->setTitleTTFForState ( pFontTTF, kCCControlStateDisabled );
    } 
	else
	{
        CCControlLoader::onHandlePropTypeFontTTF ( pNode, pParent, pPropertyName, pFontTTF, pCCBReader );
    }
}

KDvoid CCControlButtonLoader::onHandlePropTypeFloatScale ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat pFloatScale, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_TITLETTFSIZE_NORMAL ) == 0 ) 
	{
        ( (CCControlButton*) pNode )->setTitleTTFSizeForState ( pFloatScale, kCCControlStateNormal );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TITLETTFSIZE_HIGHLIGHTED ) == 0 )
	{
        ( (CCControlButton*) pNode )->setTitleTTFSizeForState ( pFloatScale, kCCControlStateHighlighted );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TITLETTFSIZE_DISABLED ) == 0 )
	{
        ( (CCControlButton*) pNode )->setTitleTTFSizeForState ( pFloatScale, kCCControlStateDisabled );
    }
	else
	{
        CCControlLoader::onHandlePropTypeFloatScale ( pNode, pParent, pPropertyName, pFloatScale, pCCBReader );
    }
}

KDvoid CCControlButtonLoader::onHandlePropTypePoint ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint pPoint, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_LABELANCHORPOINT ) == 0 )
	{
        ( (CCControlButton*) pNode )->setLabelAnchorPoint ( pPoint );
    } 
	else 
	{
        CCControlLoader::onHandlePropTypePoint ( pNode, pParent, pPropertyName, pPoint, pCCBReader );
    }
}

KDvoid CCControlButtonLoader::onHandlePropTypeSize ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSize pSize, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_PREFEREDSIZE ) == 0 )
	{
        ( (CCControlButton*) pNode )->setPreferredSize ( pSize );
    }
	else
	{
        CCControlLoader::onHandlePropTypeSize ( pNode, pParent, pPropertyName, pSize, pCCBReader );
    }
}

KDvoid CCControlButtonLoader::onHandlePropTypeSpriteFrame ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSpriteFrame* pCCSpriteFrame, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL ) == 0 )
	{
        if ( pCCSpriteFrame != KD_NULL )
		{
            ( (CCControlButton*) pNode )->setBackgroundSpriteFrameForState ( pCCSpriteFrame, kCCControlStateNormal );
        }
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED ) == 0 )
	{
        if ( pCCSpriteFrame != KD_NULL ) 
		{
            ( (CCControlButton*) pNode )->setBackgroundSpriteFrameForState ( pCCSpriteFrame, kCCControlStateHighlighted );
        }
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED ) == 0 )
	{
        if ( pCCSpriteFrame != KD_NULL )
		{
            ( (CCControlButton*) pNode )->setBackgroundSpriteFrameForState ( pCCSpriteFrame, kCCControlStateDisabled );
        }
    } 
	else
	{
        CCControlLoader::onHandlePropTypeSpriteFrame ( pNode, pParent, pPropertyName, pCCSpriteFrame, pCCBReader );
    }
}

KDvoid CCControlButtonLoader::onHandlePropTypeColor3 ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B pCCColor3B, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_TITLECOLOR_NORMAL ) == 0 ) 
	{
        ( (CCControlButton*) pNode )->setTitleColorForState ( pCCColor3B, kCCControlStateNormal );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TITLECOLOR_HIGHLIGHTED ) == 0 )
	{
        ( (CCControlButton*) pNode )->setTitleColorForState ( pCCColor3B, kCCControlStateHighlighted );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_TITLECOLOR_DISABLED ) == 0 ) 
	{
        ( (CCControlButton*) pNode )->setTitleColorForState ( pCCColor3B, kCCControlStateDisabled );
    }
	else
	{
        CCControlLoader::onHandlePropTypeColor3 ( pNode, pParent, pPropertyName, pCCColor3B, pCCBReader );
    }
}

NS_CC_END