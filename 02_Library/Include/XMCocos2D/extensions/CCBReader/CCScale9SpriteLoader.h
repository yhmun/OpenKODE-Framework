/* --------------------------------------------------------------------------
 *
 *      File            CCScale9SpriteLoader.h
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

#ifndef __CCScale9SpriteLoader_h__
#define __CCScale9SpriteLoader_h__

#include "CCNodeLoader.h"
#include "../CCControl/CCScale9Sprite.h"

NS_CC_BEGIN

/* Forward declaration. */
class CCScale9SpriteLoader : public CCNodeLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( CCScale9SpriteLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( CCScale9Sprite );

        virtual KDvoid		onHandlePropTypeColor3		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B		pCCColor3B		, CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeByte		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte			pByte			, CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeBlendFunc	( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc		pCCBlendFunc	, CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeSpriteFrame ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSpriteFrame*	pCCSpriteFrame	, CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeSize		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSize			pSize			, CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeFloat		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat			pFloat			, CCBReader* pCCBReader );
};

NS_CC_END

#endif	// __CCScale9SpriteLoader_h__
