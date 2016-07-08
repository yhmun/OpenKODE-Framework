/* --------------------------------------------------------------------------
 *
 *      File            CCMenuItemLoader.h
 *      Author          Young-Hwan Mun
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

#ifndef __CCMenuItemLoader_h__
#define __CCMenuItemLoader_h__

#include "CCMenuLoader.h"

NS_CC_BEGIN

/* Forward declaration. */
class CCMenuItemLoader : public CCNodeLoader 
{
    protected :

        CCB_PURE_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( CCMenuItem );

        virtual KDvoid		onHandlePropTypeBlock ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, BlockData* pBlockData	, CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeCheck ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool		pCheck		, CCBReader* pCCBReader );
};

NS_CC_END

#endif	// __CCMenuItemLoader_h__
