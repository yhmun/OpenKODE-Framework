/* --------------------------------------------------------------------------
 *
 *      File            CCNodeLoader.h
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

#ifndef __CCNodeLoader_h__
#define __CCNodeLoader_h__

#include "CCBReader.h"
#include "CCBValue.h"
#include "../../sprite_nodes/CCAnimation.h"
#include "../../sprite_nodes/CCSpriteFrame.h"
#include "../CCControl/CCInvocation.h"

NS_CC_BEGIN

#define PROPERTY_POSITION										"position"
#define PROPERTY_CONTENTSIZE									"contentSize"
#define PROPERTY_SKEW											"skew"
#define PROPERTY_ANCHORPOINT									"anchorPoint"
#define PROPERTY_SCALE											"scale"
#define PROPERTY_ROTATION										"rotation"
#define PROPERTY_ROTATIONX										"rotationX"
#define PROPERTY_ROTATIONY										"rotationY"
#define PROPERTY_TAG											"tag"
#define PROPERTY_IGNOREANCHORPOINTFORPOSITION					"ignoreAnchorPointForPosition"
#define PROPERTY_VISIBLE										"visible"

#define ASSERT_FAIL_UNEXPECTED_PROPERTY( _PROPERTY )			CCAssert ( KD_FALSE, "Unexpected property: '%s'!", _PROPERTY );
#define ASSERT_FAIL_UNEXPECTED_PROPERTYTYPE( _PROPERTYTYPE )	CCAssert ( KD_FALSE, "Unexpected property type: '%d'!", _PROPERTYTYPE );

#define CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD( _TYPE )				\
virtual _TYPE* createCCNode ( CCNode* pParent, CCBReader* pCCBReader )			\
{																				\
    return _TYPE::create ( );													\
}

#define CCB_PURE_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD( _TYPE )			\
virtual _TYPE* createCCNode ( CCNode* pParent, CCBReader* pCCBReader ) = 0		\

struct BlockData
{
    SEL_MenuHandler			m_pSELMenuHandler;
    CCObject*				m_pTarget;
};

struct BlockCCControlData
{
    SEL_CCControlHandler	m_pSELCCControlHandler;
    CCObject*				m_pTarget;
    KDint					m_nControlEvents;
};

/* Forward declaration. */
class CCNodeLoader : public CCObject
{
    public :
				 CCNodeLoader ( KDvoid );
        virtual ~CCNodeLoader ( KDvoid );

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( CCNodeLoader, loader );

        virtual CCNode*				loadCCNode		( CCNode* pNode, CCBReader* pCCBReader );
        virtual KDvoid				parseProperties ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
		virtual CCDictionary*		getCustomProperties ( KDvoid );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( CCNode );

        virtual CCPoint				parsePropTypePosition			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName );
        virtual CCPoint				parsePropTypePoint				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual CCPoint				parsePropTypePointLock			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual CCSize				parsePropTypeSize				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDfloat*			parsePropTypeScaleLock			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName );
        virtual KDfloat				parsePropTypeFloat				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDfloat				parsePropTypeDegrees			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName );
        virtual KDfloat				parsePropTypeFloatScale			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDint				parsePropTypeInteger			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDint				parsePropTypeIntegerLabeled		( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDfloat*			parsePropTypeFloatVar			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDbool				parsePropTypeCheck				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName );
        virtual CCSpriteFrame*		parsePropTypeSpriteFrame		( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName );
        virtual CCAnimation*		parsePropTypeAnimation			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual CCTexture2D*		parsePropTypeTexture			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDubyte				parsePropTypeByte				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName );
        virtual ccColor3B			parsePropTypeColor3				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName );
        virtual ccColor4F*			parsePropTypeColor4FVar			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDbool*				parsePropTypeFlip				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual ccBlendFunc			parsePropTypeBlendFunc			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual std::string			parsePropTypeFntFile			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual std::string			parsePropTypeString				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual std::string			parsePropTypeText				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual std::string			parsePropTypeFontTTF			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual BlockData*			parsePropTypeBlock				( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual BlockCCControlData* parsePropTypeBlockCCControl		( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual CCNode*				parsePropTypeCCBFile			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );
        virtual KDfloat*			parsePropTypeFloatXY			( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader );

        virtual KDvoid				onHandlePropTypePosition		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint				pPosition			, CCBReader* pCCBReader );
        virtual KDvoid 				onHandlePropTypePoint			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint				pPoint				, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypePointLock		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint				pPointLock			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeSize			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSize				pSize				, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeScaleLock		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat*				pScaleLock			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeFloat			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat				pFloat				, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeDegrees			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat				pDegrees			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeFloatScale		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat				pFloatScale			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeInteger			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDint				pInteger			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeIntegerLabeled	( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDint				pIntegerLabeled		, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeFloatVar		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat*				pFoatVar			, CCBReader* pCCBReader );
        virtual KDvoid				onHandlePropTypeFloatXY			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat*				pFoatVar			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeCheck			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool				pCheck				, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeSpriteFrame		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSpriteFrame*		pCCSpriteFrame		, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeAnimation		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCAnimation*			pCCAnimation		, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeTexture			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCTexture2D*			pCCTexture2D		, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeByte			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte				pByte				, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeColor3			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B			pCCColor3B			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeColor4FVar		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor4F*			pCCColor4FVar		, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeFlip			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool*				pFlip				, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeBlendFunc		( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc			pCCBlendFunc		, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeFntFile			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar*		pFntFile			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeString			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar*		pString				, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeText			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar*		pText				, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeFontTTF			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar*		pFontTTF			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeBlock			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, BlockData*			pBlockData			, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeBlockCCControl	( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, BlockCCControlData*	pBlockCCControlData	, CCBReader* pCCBReader );
        virtual KDvoid  			onHandlePropTypeCCBFile			( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCNode*				pCCBFileNode		, CCBReader* pCCBReader );

	protected :

        CCDictionary*				m_pCustomProperties;
};

NS_CC_END

#endif	// __CCNodeLoader_h__
