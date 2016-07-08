/* --------------------------------------------------------------------------
 *
 *      File            CCControlLoader.cpp
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
#include "extensions/CCBReader/CCControlLoader.h"

NS_CC_BEGIN

#define PROPERTY_ENABLED		"enabled"
#define PROPERTY_SELECTED		"selected"
#define PROPERTY_CCCONTROL		"ccControl"

KDvoid CCControlLoader::onHandlePropTypeCheck ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool pCheck, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_ENABLED ) == 0 ) 
	{
        ( (CCControl*) pNode )->setEnabled ( pCheck );
    }
	else if ( kdStrcmp ( pPropertyName, PROPERTY_SELECTED ) == 0 )
	{
        ( (CCControl*) pNode )->setSelected ( pCheck );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeCheck ( pNode, pParent, pPropertyName, pCheck, pCCBReader );
    }
}

KDvoid CCControlLoader::onHandlePropTypeBlockCCControl ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, BlockCCControlData* pBlockCCControlData, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_CCCONTROL ) == 0 )
	{
        ( (CCControl*) pNode )->addTargetWithActionForControlEvents ( pBlockCCControlData->m_pTarget, pBlockCCControlData->m_pSELCCControlHandler, pBlockCCControlData->m_nControlEvents );
    }
	else 
	{
        CCNodeLoader::onHandlePropTypeBlockCCControl ( pNode, pParent, pPropertyName, pBlockCCControlData, pCCBReader );
    }
}

NS_CC_END