/* --------------------------------------------------------------------------
 *
 *      File            CCLabelBMFontLoader.h
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

#ifndef __CCLabelBMFontLoader_h__
#define __CCLabelBMFontLoader_h__

#include "CCNodeLoader.h"
#include "../../label_nodes/CCLabelBMFont.h"

NS_CC_BEGIN

/* Forward declaration. */
class CCLabelBMFontLoader : public CCNodeLoader 
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( CCLabelBMFontLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( CCLabelBMFont );

        virtual KDvoid		onHandlePropTypeColor3		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B	 pCCColor3B	 , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeByte		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte		 pByte		 , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeBlendFunc	( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc	 pCCBlendFunc, CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeFntFile		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pFntFile	 , CCBReader* pCCBReader );
        virtual KDvoid		onHandlePropTypeText		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pText		 , CCBReader* pCCBReader );
};

NS_CC_END	// __CCLabelBMFontLoader_h__

#endif	// __CCLabelBMFontLoader_h__
