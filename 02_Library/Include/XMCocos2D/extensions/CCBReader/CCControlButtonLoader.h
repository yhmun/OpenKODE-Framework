/* --------------------------------------------------------------------------
 *
 *      File            CCControlButtonLoader.h
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

#ifndef __CCControlButtonLoader_h__
#define __CCControlButtonLoader_h__

#include "CCControlLoader.h"
#include "../CCControl/CCControlButton.h"

NS_CC_BEGIN

/* Forward declaration. */
class CCControlButtonLoader : public CCControlLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( CCControlButtonLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( CCControlButton );

        virtual KDvoid		onHandlePropTypeCheck		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool			pCheck		  , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeString		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar*	pString		  , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeFontTTF		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar*	pFontTTF	  , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeFloatScale	( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat			pFloatScale	  , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypePoint		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint			pPoint		  , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeSize		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSize			pSize		  , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeSpriteFrame ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSpriteFrame*	pCCSpriteFrame, CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeColor3		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B		pCCColor3B	  , CCBReader* pCCBReader );
};

NS_CC_END

#endif // __CCControlButtonLoader_h__
