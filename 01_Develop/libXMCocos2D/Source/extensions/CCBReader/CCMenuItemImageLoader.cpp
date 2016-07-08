/* --------------------------------------------------------------------------
 *
 *      File            CCMenuItemImageLoader.cpp
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
#include "extensions/CCBReader/CCMenuItemImageLoader.h"

NS_CC_BEGIN

#define PROPERTY_NORMALDISPLAYFRAME			"normalSpriteFrame"
#define PROPERTY_SELECTEDDISPLAYFRAME		"selectedSpriteFrame"
#define PROPERTY_DISABLEDDISPLAYFRAME		"disabledSpriteFrame"

KDvoid CCMenuItemImageLoader::onHandlePropTypeSpriteFrame ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSpriteFrame* pCCSpriteFrame, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_NORMALDISPLAYFRAME ) == 0 )
	{
        if ( pCCSpriteFrame != KD_NULL ) 
		{
            ( (CCMenuItemImage*) pNode )->setNormalSpriteFrame ( pCCSpriteFrame );
        }
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_SELECTEDDISPLAYFRAME ) == 0 )
	{
        if ( pCCSpriteFrame != KD_NULL )
		{
            ( (CCMenuItemImage*) pNode )->setSelectedSpriteFrame ( pCCSpriteFrame );
        }
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_DISABLEDDISPLAYFRAME ) == 0 )
	{
        if ( pCCSpriteFrame != KD_NULL )
		{
            ( (CCMenuItemImage*) pNode )->setDisabledSpriteFrame ( pCCSpriteFrame );
        }
    }
	else 
	{
        CCMenuItemLoader::onHandlePropTypeSpriteFrame ( pNode, pParent, pPropertyName, pCCSpriteFrame, pCCBReader );
    }
}

NS_CC_END