/* --------------------------------------------------------------------------
 *
 *      File            CCScrollViewLoader.cpp
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
#include "extensions/CCBReader/CCScrollViewLoader.h"

NS_CC_BEGIN

#define PROPERTY_CONTAINER			"container"
#define PROPERTY_DIRECTION			"direction"
#define PROPERTY_CLIPSTOBOUNDS		"clipsToBounds"
#define PROPERTY_BOUNCES			"bounces"
#define PROPERTY_SCALE				"scale"

KDvoid CCScrollViewLoader::onHandlePropTypeSize ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSize pSize, CCBReader* pCCBReader )
{
	if ( kdStrcmp ( pPropertyName, PROPERTY_CONTENTSIZE ) == 0 ) 
	{
		( (CCScrollView*) pNode )->setViewSize ( pSize );
	} 
	else 
	{
		CCNodeLoader::onHandlePropTypeSize ( pNode, pParent, pPropertyName, pSize, pCCBReader );
	}
}

KDvoid CCScrollViewLoader::onHandlePropTypeCheck ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool pCheck, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_CLIPSTOBOUNDS ) == 0 )
	{
        ( (CCScrollView*) pNode )->setClippingToBounds ( pCheck );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_BOUNCES ) == 0 )
	{
        ( (CCScrollView*) pNode )->setBounceable ( pCheck );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeCheck ( pNode, pParent, pPropertyName, pCheck, pCCBReader );
    }
}

KDvoid CCScrollViewLoader::onHandlePropTypeCCBFile ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCNode* pCCBFileNode, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_CONTAINER ) == 0 )
	{
        ( (CCScrollView*) pNode )->setContainer ( pCCBFileNode );
		( (CCScrollView*) pNode )->updateInset ( );
    } 
	else 
	{
        CCNodeLoader::onHandlePropTypeCCBFile ( pNode, pParent, pPropertyName, pCCBFileNode, pCCBReader );
    }
}

KDvoid CCScrollViewLoader::onHandlePropTypeFloat ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat pFloat, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_SCALE ) == 0 )
	{
        ( (CCScrollView*) pNode )->setScale ( pFloat );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeFloat ( pNode, pParent, pPropertyName, pFloat, pCCBReader );
    }
}

KDvoid CCScrollViewLoader::onHandlePropTypeIntegerLabeled ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDint pIntegerLabeled, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_DIRECTION ) == 0 )
	{
        ( (CCScrollView*) pNode )->setDirection ( CCScrollViewDirection(pIntegerLabeled ) );
    } 
	else
	{
        CCNodeLoader::onHandlePropTypeFloatScale ( pNode, pParent, pPropertyName, (KDfloat) pIntegerLabeled, pCCBReader );
    }
}

NS_CC_END