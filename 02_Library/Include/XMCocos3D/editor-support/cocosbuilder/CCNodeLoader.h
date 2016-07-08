/* -----------------------------------------------------------------------------------
 *
 *      File            CCNodeLoader.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */

#ifndef __CCNodeLoader_h__
#define __CCNodeLoader_h__

#include "../../extensions/GUI/CCControlExtension/CCInvocation.h"
#include "CCBReader.h"
#include "CCBValue.h"
#include "../../extensions/GUI/CCControlExtension/CCControl.h"

namespace cocosbuilder {

#define PROPERTY_POSITION						"position"
#define PROPERTY_CONTENTSIZE					"contentSize"
#define PROPERTY_SKEW							"skew"
#define PROPERTY_ANCHORPOINT					"anchorPoint"
#define PROPERTY_SCALE							"scale"
#define PROPERTY_ROTATION						"rotation"
#define PROPERTY_ROTATIONX						"rotationX"
#define PROPERTY_ROTATIONY						"rotationY"
#define PROPERTY_TAG							"tag"
#define PROPERTY_IGNOREANCHORPOINTFORPOSITION	"ignoreAnchorPointForPosition"
#define PROPERTY_VISIBLE						"visible"

#define ASSERT_FAIL_UNEXPECTED_PROPERTY(PROPERTY)			cocos2d::log ( "Unexpected property: '%s'!\n", PROPERTY ); kdAssert ( false )
#define ASSERT_FAIL_UNEXPECTED_PROPERTYTYPE(PROPERTYTYPE)	cocos2d::log ( "Unexpected property type: '%d'!\n", PROPERTYTYPE ); kdAssert ( false )

#define CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(T)									\
virtual T* createNode ( cocos2d::Node* pParent, cocosbuilder::CCBReader* ccbReader ) {		\
    return T::create ( );																	\
}

#define CCB_PURE_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(T)								\
virtual T* createNode ( cocos2d::Node* pParent, cocosbuilder::CCBReader* ccbReader ) = 0

struct BlockData 
{
    cocos2d::SEL_MenuHandler				m_pSELMenuHandler;
    cocos2d::Object*						m_pTarget;
};

struct BlockControlData 
{
    cocos2d::extension::Control::Handler	m_pSELControlHandler;
    cocos2d::Object*						m_pTarget;
    cocos2d::extension::Control::EventType	m_eControlEvents;
};

/* Forward declaration. */
class CCBReader;

class NodeLoader : public cocos2d::Object
{
    public :

        /**
         *	@js NA
         *	@lua NA
         */
        NodeLoader ( KDvoid );

        /**
         *	@js NA
         *	@lua NA
         */
        virtual ~NodeLoader ( KDvoid );

        /**
         *	@js NA
         *	@lua NA
         */
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( NodeLoader, loader );

        /**
         * @js NA
         * @lua NA
         */
        virtual cocos2d::Node*			loadNode ( cocos2d::Node*, CCBReader* ccbReader );
        
		/**
         *	@js NA
         *	@lua NA
         */
        virtual KDvoid					parseProperties ( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        
		/**
         *	@js NA
         *	@lua NA
         */
        virtual cocos2d::Dictionary*	getCustomProperties ( KDvoid );
    
    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( cocos2d::Node );

        virtual cocos2d::Point			parsePropTypePosition			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader, const KDchar* pPropertyName );
        virtual cocos2d::Point			parsePropTypePoint				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual cocos2d::Point			parsePropTypePointLock			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual cocos2d::Size			parsePropTypeSize				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDfloat*				parsePropTypeScaleLock			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader, const KDchar* pPropertyName );
        virtual KDfloat					parsePropTypeFloat				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDfloat					parsePropTypeDegrees			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader, const KDchar* pPropertyName );
        virtual KDfloat					parsePropTypeFloatScale			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDint					parsePropTypeInteger			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDint					parsePropTypeIntegerLabeled		( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDfloat*				parsePropTypeFloatVar			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDbool					parsePropTypeCheck				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader, const KDchar* pPropertyName );
        virtual cocos2d::SpriteFrame*	parsePropTypeSpriteFrame		( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader, const KDchar* pPropertyName );
        virtual cocos2d::Animation*		parsePropTypeAnimation			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual cocos2d::Texture2D*		parsePropTypeTexture			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDubyte					parsePropTypeByte				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader, const KDchar* pPropertyName );
        virtual cocos2d::Color3B		parsePropTypeColor3				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader, const KDchar* pPropertyName );
        virtual cocos2d::Color4F*		parsePropTypeColor4FVar			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDbool*					parsePropTypeFlip				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual cocos2d::BlendFunc		parsePropTypeBlendFunc			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual std::string				parsePropTypeFntFile			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual std::string				parsePropTypeString				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual std::string				parsePropTypeText				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual std::string				parsePropTypeFontTTF			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual BlockData*				parsePropTypeBlock				( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual BlockControlData*		parsePropTypeBlockControl		( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual cocos2d::Node*			parsePropTypeCCBFile			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );
        virtual KDfloat*				parsePropTypeFloatXY			( cocos2d::Node* pNode, cocos2d::Node* pParent, CCBReader* ccbReader );

        virtual KDvoid					onHandlePropTypePosition		( cocos2d::Node* pNode,cocos2d:: Node* pParent, const KDchar* pPropertyName, cocos2d::Point			pPosition		 , CCBReader* ccbReader );
        virtual KDvoid					onHandlePropTypePoint			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Point			pPoint			 , CCBReader* ccbReader );
        virtual KDvoid					onHandlePropTypePointLock		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Point			pPointLock		 , CCBReader* ccbReader );
        virtual KDvoid					onHandlePropTypeSize			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Size			pSize			 , CCBReader* ccbReader );
        virtual KDvoid					onHandlePropTypeScaleLock		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDfloat*				pScaleLock		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeFloat			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDfloat				pFloat			 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeDegrees			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDfloat				pDegrees		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeFloatScale		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDfloat				pFloatScale		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeInteger			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDint					pInteger		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeIntegerLabeled	( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDint					pIntegerLabeled	 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeFloatVar		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDfloat*				pFoatVar		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeFloatXY			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDfloat*				pFoatVar		 , CCBReader* ccbReader );

        virtual KDvoid 					onHandlePropTypeCheck			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDbool					pCheck			 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeSpriteFrame		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::SpriteFrame*	pSpriteFrame	 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeAnimation		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Animation*	pAnimation		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeTexture			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Texture2D*	pTexture2D		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeByte			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDubyte				pByte			 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeColor3			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Color3B		pColor3B		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeColor4FVar		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Color4F*		pColor4FVar		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeFlip			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, KDbool*				pFlip			 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeBlendFunc		( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::BlendFunc		pBlendFunc		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeFntFile			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, const KDchar*			pFntFile		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeString			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, const KDchar*			pString			 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeText			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, const KDchar*			pText			 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeFontTTF			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, const KDchar*			pFontTTF		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeBlock			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, BlockData*				pBlockData		 , CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeBlockControl	( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, BlockControlData*		pBlockControlData, CCBReader* ccbReader );
        virtual KDvoid 					onHandlePropTypeCCBFile			( cocos2d::Node* pNode, cocos2d::Node* pParent, const KDchar* pPropertyName, cocos2d::Node*			pCCBFileNode	 , CCBReader* ccbReader );

	protected :

        cocos2d::Dictionary*			m_pCustomProperties;
};

}

#endif	// __CCNodeLoader_h__
