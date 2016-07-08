/* --------------------------------------------------------------------------
 *
 *      File            CCNodeLoader.cpp
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "extensions/CCBReader/CCNodeLoader.h"
#include "extensions/CCBReader/CCBSelectorResolver.h"
#include "extensions/CCBReader/CCBMemberVariableAssigner.h"
#include "extensions/CCBReader/CCBAnimationManager.h"
#include "extensions/CCBReader/CCBRelativePositioning.h"
#include "cocoa/CCData.h"
#include "sprite_nodes/CCAnimationCache.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "textures/CCTextureCache.h"
#include "support/CCFileUtils.h"

NS_CC_BEGIN

CCNodeLoader::CCNodeLoader ( KDvoid )
{
    m_pCustomProperties = new CCDictionary ( );
}

CCNodeLoader::~CCNodeLoader ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pCustomProperties );
}

CCDictionary* CCNodeLoader::getCustomProperties ( KDvoid )
{
    return m_pCustomProperties;
}

CCNode* CCNodeLoader::loadCCNode ( CCNode* pParent, CCBReader* pCCBReader )
{
    CCNode*  pNode = this->createCCNode ( pParent, pCCBReader );

    return pNode;
}

KDvoid CCNodeLoader::parseProperties ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
	KDint  nNumRegularProps = pCCBReader->readInt ( KD_FALSE );
    KDint  nNumExturaProps  = pCCBReader->readInt ( KD_FALSE );
    KDint  nPropertyCount   = nNumRegularProps + nNumExturaProps;

    for ( KDint i = 0; i < nPropertyCount; i++ )
	{
		KDbool		bIsExtraProp = ( i >= nNumRegularProps );
        KDint		nType = pCCBReader->readInt ( KD_FALSE );
        std::string sPropertyName = pCCBReader->readCachedString ( );

        // Check if the property can be set for this platform
        KDbool     bSetProp  = KD_FALSE;        
        KDint      nPlatform = pCCBReader->readByte ( );

        // Cocos2d-x is using touch event callback for all platforms,
        // it's different from cocos2d-iphone which uses mouse event for Mac port.
        // So we just need to touch event by using kCCBPlatformIOS.
        if ( nPlatform == kCCBPlatformAll || nPlatform == kCCBPlatformIOS ) 
		{
            bSetProp = KD_TRUE;
        }

		// Forward properties for sub ccb files
        if ( dynamic_cast<CCBFile*> ( pNode ) != KD_NULL )
        {
            CCBFile*  pCCBNode = (CCBFile*) pNode;
            if ( pCCBNode->getCCBFileNode ( ) && bIsExtraProp )
            {
                pNode = pCCBNode->getCCBFileNode ( );
                
                // Skip properties that doesn't have a value to override
                CCArray*  pExtraPropsNames = (CCArray*) pNode->getUserObject ( );
                CCObject* pObj   = KD_NULL;
                KDbool    bFound = KD_FALSE;
                CCARRAY_FOREACH ( pExtraPropsNames, pObj )
                {
                    CCString*  pStr = (CCString*) pObj;
                    if ( 0 == pStr->compare ( sPropertyName.c_str ( ) ) )
                    {
                        bFound = KD_TRUE;
                        break;
                    }
                }

                bSetProp &= bFound;
            }
        }
        else if ( bIsExtraProp && pNode == pCCBReader->getAnimationManager ( )->getRootNode ( ) )
        {
            CCArray*  pExtraPropsNames = (CCArray*) pNode->getUserObject ( );
            if ( !pExtraPropsNames )
            {
                pExtraPropsNames = CCArray::create ( );
                pNode->setUserObject ( pExtraPropsNames );
            }
            
            pExtraPropsNames->addObject ( CCString::create ( sPropertyName ) );
        }

        switch ( nType ) 
		{
            case kCCBPropTypePosition :
			{
				CCPoint  tPosition = this->parsePropTypePosition ( pNode, pParent, pCCBReader, sPropertyName.c_str ( ) );
                if ( bSetProp ) 
				{
					this->onHandlePropTypePosition ( pNode, pParent, sPropertyName.c_str ( ), tPosition, pCCBReader );
                }
                
            }	break;

            case kCCBPropTypePoint :
			{
                CCPoint  tPoint = this->parsePropTypePoint ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypePoint ( pNode, pParent, sPropertyName.c_str ( ), tPoint, pCCBReader );
                }
                
            }	break;

            case kCCBPropTypePointLock :
			{
                CCPoint  tPointLock = this->parsePropTypePointLock ( pNode, pParent, pCCBReader );
                if ( bSetProp )
				{
                    this->onHandlePropTypePointLock ( pNode, pParent, sPropertyName.c_str ( ), tPointLock, pCCBReader );
                }
                
            }	break;

            case kCCBPropTypeSize :
			{
                CCSize   tSize = this->parsePropTypeSize ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeSize ( pNode, pParent, sPropertyName.c_str ( ), tSize, pCCBReader );
                }
                
            }	break;

            case kCCBPropTypeScaleLock :
			{
                KDfloat*  pScaleLock = this->parsePropTypeScaleLock ( pNode, pParent, pCCBReader, sPropertyName.c_str ( ) );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeScaleLock ( pNode, pParent, sPropertyName.c_str ( ), pScaleLock, pCCBReader );
                }
                CC_SAFE_DELETE_ARRAY ( pScaleLock );
                
            }	break;

            case kCCBPropTypeFloat :
			{
                KDfloat  fValue = this->parsePropTypeFloat ( pNode, pParent, pCCBReader );
                if ( bSetProp )
				{
                    this->onHandlePropTypeFloat ( pNode, pParent, sPropertyName.c_str ( ), fValue, pCCBReader );
                }
            }	break;

			case kCCBPropTypeFloatXY:
			{
				KDfloat*  xy =  this->parsePropTypeFloatXY ( pNode, pParent, pCCBReader );
				if ( bSetProp )
				{
					this->onHandlePropTypeFloatXY ( pNode, pParent, sPropertyName.c_str ( ), xy, pCCBReader );
				}
				CC_SAFE_DELETE_ARRAY ( xy );
				
			}	break;

            case kCCBPropTypeDegrees :
			{
                KDfloat  fDegrees = this->parsePropTypeDegrees ( pNode, pParent, pCCBReader, sPropertyName.c_str ( ) );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeDegrees ( pNode, pParent, sPropertyName.c_str ( ), fDegrees, pCCBReader );
                }
            }	break;

            case kCCBPropTypeFloatScale :
			{
                KDfloat  fFloatScale = this->parsePropTypeFloatScale ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeFloatScale ( pNode, pParent, sPropertyName.c_str ( ), fFloatScale, pCCBReader );
                }
            }	break;

            case kCCBPropTypeInteger :
			{
                KDint  nInteger = this->parsePropTypeInteger ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeInteger ( pNode, pParent, sPropertyName.c_str ( ), nInteger, pCCBReader );
                }                
            }	break;

            case kCCBPropTypeIntegerLabeled :
			{
                KDint  nIntegerLabeled = this->parsePropTypeIntegerLabeled ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeIntegerLabeled ( pNode, pParent, sPropertyName.c_str ( ), nIntegerLabeled, pCCBReader );
                }                
            }	break;

            case kCCBPropTypeFloatVar :
			{
                KDfloat*  pFloatVar = this->parsePropTypeFloatVar ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeFloatVar ( pNode, pParent, sPropertyName.c_str ( ), pFloatVar, pCCBReader );
                }
                CC_SAFE_DELETE_ARRAY ( pFloatVar );                
            }	break;

            case kCCBPropTypeCheck :
			{
                KDbool  bCheck = this->parsePropTypeCheck ( pNode, pParent, pCCBReader, sPropertyName.c_str ( ) );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeCheck ( pNode, pParent, sPropertyName.c_str ( ), bCheck, pCCBReader );
                }
            }	break;

            case kCCBPropTypeSpriteFrame :
			{
                CCSpriteFrame*  pCCSpriteFrame = this->parsePropTypeSpriteFrame ( pNode, pParent, pCCBReader, sPropertyName.c_str ( ) );
                if ( bSetProp )
				{
                    this->onHandlePropTypeSpriteFrame ( pNode, pParent, sPropertyName.c_str ( ), pCCSpriteFrame, pCCBReader );
                }                
            }	break;

            case kCCBPropTypeAnimation :
			{
                CCAnimation*  pCCAnimation = this->parsePropTypeAnimation ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeAnimation ( pNode, pParent, sPropertyName.c_str ( ), pCCAnimation, pCCBReader );
                }
            }	break;

            case kCCBPropTypeTexture :
			{
                CCTexture2D *  pCCTexture2D = this->parsePropTypeTexture ( pNode, pParent, pCCBReader );
                if ( bSetProp )
				{
                    this->onHandlePropTypeTexture ( pNode, pParent, sPropertyName.c_str ( ), pCCTexture2D, pCCBReader );
                }                
            }	break;

            case kCCBPropTypeByte :
			{
                KDubyte  cByte = this->parsePropTypeByte ( pNode, pParent, pCCBReader, sPropertyName.c_str ( ) );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeByte ( pNode, pParent, sPropertyName.c_str ( ), cByte, pCCBReader );
                }
            }	break;

            case kCCBPropTypeColor3 :
			{
                ccColor3B  tColor3B = this->parsePropTypeColor3 ( pNode, pParent, pCCBReader, sPropertyName.c_str ( ) );
                if ( bSetProp )
				{
                    this->onHandlePropTypeColor3 ( pNode, pParent, sPropertyName.c_str ( ), tColor3B, pCCBReader );
                }                
            }	break;

            case kCCBPropTypeColor4FVar : 
			{
                ccColor4F*  pColor4FVar = this->parsePropTypeColor4FVar ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeColor4FVar ( pNode, pParent, sPropertyName.c_str ( ), pColor4FVar, pCCBReader );
                }
                CC_SAFE_DELETE_ARRAY ( pColor4FVar );                
            }	break;

            case kCCBPropTypeFlip :
			{
                KDbool*  pFlip = this->parsePropTypeFlip ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeFlip ( pNode, pParent, sPropertyName.c_str ( ), pFlip, pCCBReader );
                }
                CC_SAFE_DELETE_ARRAY ( pFlip );
            }	break;

            case kCCBPropTypeBlendMode :
			{
                ccBlendFunc  tBlendFunc = this->parsePropTypeBlendFunc ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeBlendFunc ( pNode, pParent, sPropertyName.c_str ( ), tBlendFunc, pCCBReader );
                }                
            }	break;

            case kCCBPropTypeFntFile :
			{
                std::string  sFntFile = pCCBReader->getCCBRootPath ( ) + this->parsePropTypeFntFile ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
					this->onHandlePropTypeFntFile ( pNode, pParent, sPropertyName.c_str ( ), sFntFile.c_str ( ), pCCBReader );
                }                
            }	break;

            case kCCBPropTypeFontTTF :
			{
                std::string  sFontTTF = this->parsePropTypeFontTTF ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
					this->onHandlePropTypeFontTTF ( pNode, pParent, sPropertyName.c_str ( ), sFontTTF.c_str ( ), pCCBReader );
                }                
            }	break;

            case kCCBPropTypeString :
			{
                std::string  sString = this->parsePropTypeString ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
					this->onHandlePropTypeString ( pNode, pParent, sPropertyName.c_str ( ), sString.c_str ( ), pCCBReader );
                }                
            }	break;

            case kCCBPropTypeText :
			{
                std::string  sText = this->parsePropTypeText ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
					this->onHandlePropTypeText ( pNode, pParent, sPropertyName.c_str ( ), sText.c_str ( ), pCCBReader );
                }                
            }	break;

            case kCCBPropTypeBlock :
			{
                BlockData*  pBlockData = this->parsePropTypeBlock ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeBlock ( pNode, pParent, sPropertyName.c_str ( ), pBlockData, pCCBReader );
                }
                CC_SAFE_DELETE ( pBlockData );                
            }	break;

            case kCCBPropTypeBlockCCControl :
			{
                BlockCCControlData*  pBlockCCControlData = this->parsePropTypeBlockCCControl ( pNode, pParent, pCCBReader );
                if ( bSetProp && pBlockCCControlData != KD_NULL ) 
				{
                    this->onHandlePropTypeBlockCCControl ( pNode, pParent, sPropertyName.c_str ( ), pBlockCCControlData, pCCBReader );
                }
                CC_SAFE_DELETE ( pBlockCCControlData );                
            }	break;

            case kCCBPropTypeCCBFile :
			{
                CCNode*  pCCBFileNode = this->parsePropTypeCCBFile ( pNode, pParent, pCCBReader );
                if ( bSetProp ) 
				{
                    this->onHandlePropTypeCCBFile ( pNode, pParent, sPropertyName.c_str ( ), pCCBFileNode, pCCBReader );
                }                
            }	break;

            default :
                ASSERT_FAIL_UNEXPECTED_PROPERTYTYPE ( nType );
                break;
        }
    }
}


CCPoint CCNodeLoader::parsePropTypePosition ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName )
{
    KDfloat  x = pCCBReader->readFloat ( );
    KDfloat  y = pCCBReader->readFloat ( );
    
    KDint    nType = pCCBReader->readInt ( KD_FALSE );
    
    CCSize   tContainerSize = pCCBReader->getAnimationManager ( )->getContainerSize ( pParent );
    
    CCPoint  tPosition = getAbsolutePosition ( ccp ( x, y ), nType, tContainerSize, szPropertyName );
    pNode->setPosition ( tPosition );
    
    if ( pCCBReader->getAnimatedProperties ( )->find ( szPropertyName ) != pCCBReader->getAnimatedProperties ( )->end ( ) )
    {
        CCArray*  pBaseValue = CCArray::create ( CCBValue::create ( x ), CCBValue::create ( y ), CCBValue::create ( nType ), KD_NULL );
        pCCBReader->getAnimationManager ( )->setBaseValue ( pBaseValue, pNode, szPropertyName );
    }

    return tPosition;
}

CCPoint CCNodeLoader::parsePropTypePoint ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    KDfloat  x = pCCBReader->readFloat ( );
    KDfloat  y = pCCBReader->readFloat ( );

    return CCPoint ( x, y );
}

CCPoint CCNodeLoader::parsePropTypePointLock ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    KDfloat  x = pCCBReader->readFloat ( );
    KDfloat  y = pCCBReader->readFloat ( );

    return CCPoint ( x, y );
}

CCSize CCNodeLoader::parsePropTypeSize ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    KDfloat  cx = pCCBReader->readFloat ( );
    KDfloat  cy = pCCBReader->readFloat ( );

    KDint    nType = pCCBReader->readInt ( KD_FALSE );

    CCSize   tContainerSize = pCCBReader->getAnimationManager ( )->getContainerSize ( pParent );

    switch ( nType ) 
	{
        case kCCBSizeTypeAbsolute :		
            // Nothing.             
        	break;

        case kCCBSizeTypeRelativeContainer :		
            cx = tContainerSize.cx - cx;
            cy = tContainerSize.cy - cy;            
        	break;

        case kCCBSizeTypePercent :		
            cx = tContainerSize.cx * cx / 100.0f;
            cy = tContainerSize.cy * cy / 100.0f;            
        	break;

        case kCCBSizeTypeHorizontalPercent :		
            cx = tContainerSize.cx * cx / 100.0f;
            break;        

        case kCCBSizeTypeVerticalPercent :		
            cy = tContainerSize.cy * cy / 100.0f;
            break;
        
		case kCCBSizeTypeMultiplyResolution :
        {
            KDfloat  fResolutionScale = CCBReader::getResolutionScale ( );
            
            cx *= fResolutionScale;
            cy *= fResolutionScale;
        }	break;

        default :
        {
            CCLOG ( "Unknown CCB type." );
        }	break;
    }
    
    return CCSize ( cx, cy );
}

KDfloat* CCNodeLoader::parsePropTypeFloatXY ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    KDfloat  x = pCCBReader->readFloat ( );
    KDfloat  y = pCCBReader->readFloat ( );

    KDfloat*  floatXY = new KDfloat [ 2 ];

    floatXY [ 0 ] = x;
    floatXY [ 1 ] = y;

    return floatXY;
}

KDfloat* CCNodeLoader::parsePropTypeScaleLock ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName ) 
{
    KDfloat  x = pCCBReader->readFloat ( );
    KDfloat  y = pCCBReader->readFloat ( );
    
    KDint    nType = pCCBReader->readInt ( KD_FALSE );
    
	setRelativeScale ( pNode, x, y, nType, szPropertyName );
    
    if ( pCCBReader->getAnimatedProperties ( )->find ( szPropertyName ) != pCCBReader->getAnimatedProperties ( )->end ( ) )
    {
        CCArray*  pBaseValue = CCArray::create ( CCBValue::create ( x ), CCBValue::create ( y ), CCBValue::create ( nType ), KD_NULL );
        pCCBReader->getAnimationManager ( )->setBaseValue ( pBaseValue, pNode, szPropertyName );
    }

    if ( nType == kCCBScaleTypeMultiplyResolution )
	{
        x *= pCCBReader->getResolutionScale ( );
        y *= pCCBReader->getResolutionScale ( );
    }
    
    KDfloat*  pScaleLock = new KDfloat[2];

    pScaleLock[0] = x;
    pScaleLock[1] = y;

    return pScaleLock;
}

KDfloat CCNodeLoader::parsePropTypeFloat ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader ) 
{
    return pCCBReader->readFloat ( );
}

KDfloat CCNodeLoader::parsePropTypeDegrees ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName )
{
	KDfloat  fRet = pCCBReader->readFloat ( );
    if ( pCCBReader->getAnimatedProperties ( )->find ( szPropertyName ) != pCCBReader->getAnimatedProperties ( )->end ( ) )
    {
        CCBValue*  pValue = CCBValue::create ( fRet );
        pCCBReader->getAnimationManager ( )->setBaseValue ( pValue, pNode, szPropertyName );
    }
    
    return fRet;
}

KDfloat CCNodeLoader::parsePropTypeFloatScale ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    KDfloat  fRet = pCCBReader->readFloat ( );

    KDint    nType = pCCBReader->readInt ( KD_FALSE );
    
    if ( nType == kCCBScaleTypeMultiplyResolution )
	{
        fRet *= pCCBReader->getResolutionScale ( );
    }

    return fRet;
}

KDint CCNodeLoader::parsePropTypeInteger ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader ) 
{
    return pCCBReader->readInt ( KD_TRUE );
}

KDint CCNodeLoader::parsePropTypeIntegerLabeled ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    return pCCBReader->readInt ( KD_TRUE );
}

KDfloat* CCNodeLoader::parsePropTypeFloatVar ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader ) 
{
    KDfloat   fFloat = pCCBReader->readFloat ( );
    KDfloat   fVar   = pCCBReader->readFloat ( );
    
    KDfloat*  pArr = new KDfloat[2];

    pArr[0] =  fFloat;
    pArr[1] =  fVar;
    
    return pArr;
}

KDbool CCNodeLoader::parsePropTypeCheck ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName ) 
{
	KDbool  bRet = pCCBReader->readBool ( );
    
    if ( pCCBReader->getAnimatedProperties ( )->find ( szPropertyName ) != pCCBReader->getAnimatedProperties ( )->end ( ) )
    {
        CCBValue*  pValue = CCBValue::create ( bRet );
        pCCBReader->getAnimationManager ( )->setBaseValue ( pValue, pNode, szPropertyName );
    }
    
    return bRet;
}


CCSpriteFrame* CCNodeLoader::parsePropTypeSpriteFrame ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName )
{
    std::string  sSpriteSheet = pCCBReader->readCachedString ( );
    std::string  sSpriteFile  = pCCBReader->readCachedString ( );
    
    CCSpriteFrame*  pSpriteFrame = KD_NULL;
    if ( sSpriteFile.length ( ) != 0 )
    {
        if ( sSpriteSheet.length ( ) == 0 )
        {
			sSpriteFile = pCCBReader->getCCBRootPath ( ) + sSpriteFile;

			CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( sSpriteFile.c_str ( ) );
            CCRect  tBounds = CCRect ( CCPointZero, pTexture->getContentSize ( ) );
            pSpriteFrame = CCSpriteFrame::createWithTexture ( pTexture, tBounds );
        }
        else 
        {
            CCSpriteFrameCache*  pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
			sSpriteSheet = pCCBReader->getCCBRootPath ( ) + sSpriteSheet; 
            
            // Load the sprite sheet only if it is not loaded
			if ( pCCBReader->getLoadedSpriteSheet ( ).find ( sSpriteSheet.c_str ( ) ) == pCCBReader->getLoadedSpriteSheet ( ).end ( ) )
            {
				pFrameCache->addSpriteFramesWithFile ( sSpriteSheet.c_str ( ) );
				pCCBReader->getLoadedSpriteSheet ( ).insert ( sSpriteSheet.c_str ( ) );
            }
            
			pSpriteFrame = pFrameCache->spriteFrameByName ( sSpriteFile.c_str ( ) );
        }
        
        if ( pCCBReader->getAnimatedProperties ( )->find ( szPropertyName ) != pCCBReader->getAnimatedProperties ( )->end ( ) )
        {
            pCCBReader->getAnimationManager ( )->setBaseValue ( pSpriteFrame, pNode, szPropertyName );
        }
    }
    
    return pSpriteFrame;
}

CCAnimation* CCNodeLoader::parsePropTypeAnimation ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    std::string  sAnimationFile = pCCBReader->getCCBRootPath ( ) + pCCBReader->readCachedString ( );
    std::string  sAnimation = pCCBReader->readCachedString ( );
    
    CCAnimation*  pCCAnimation = KD_NULL;
    
    // Support for stripping relative file paths, since ios doesn't currently
    // know what to do with them, since its pulling from bundle.
    // Eventually this should be handled by a client side asset manager
    // interface which figured out what resources to load.
    // TODO Does this problem exist in C++?
	sAnimation = CCBReader::lastPathComponent ( sAnimation.c_str ( ) );
	sAnimationFile = CCBReader::lastPathComponent ( sAnimationFile.c_str ( ) );
    
	if ( sAnimation.length ( ) > 0 )
	{
        CCAnimationCache*  pAnimationCache = CCAnimationCache::sharedAnimationCache ( );
		pAnimationCache->addAnimationsWithFile ( sAnimationFile.c_str ( ) );
        
		pCCAnimation = pAnimationCache->animationByName ( sAnimation.c_str ( ) );
    }

    return pCCAnimation;
}

CCTexture2D* CCNodeLoader::parsePropTypeTexture ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    std::string  sSpriteFile = pCCBReader->getCCBRootPath ( ) + pCCBReader->readCachedString ( );

	if ( sSpriteFile.length ( ) > 0 )
    {
		return CCTextureCache::sharedTextureCache ( )->addImage ( sSpriteFile.c_str ( ) );
    }
    else 
    {
        return KD_NULL;
    }
}

KDubyte CCNodeLoader::parsePropTypeByte ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName )
{
    KDubyte  cRet = pCCBReader->readByte ( );
    
    if ( pCCBReader->getAnimatedProperties ( )->find ( szPropertyName ) != pCCBReader->getAnimatedProperties ( )->end ( ) )
    {
        pCCBReader->getAnimationManager ( )->setBaseValue ( CCBValue::create ( cRet ), pNode, szPropertyName );
    }
    
    return cRet;
}

ccColor3B CCNodeLoader::parsePropTypeColor3 ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader, const KDchar* szPropertyName ) 
{    
    ccColor3B  tColor = 
	{
		pCCBReader->readByte ( ), 
		pCCBReader->readByte ( ), 
		pCCBReader->readByte ( )
	};

	if ( pCCBReader->getAnimatedProperties ( )->find ( szPropertyName ) != pCCBReader->getAnimatedProperties ( )->end ( ) )
    {
        ccColor3BWapper*  pValue = ccColor3BWapper::create ( tColor );
        pCCBReader->getAnimationManager ( )->setBaseValue ( pValue, pNode, szPropertyName );
    }

    return tColor;
}

ccColor4F* CCNodeLoader::parsePropTypeColor4FVar ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{   
    ccColor4F*  pColors = new ccColor4F [ 2 ];

    pColors[0].r = pCCBReader->readFloat ( );
    pColors[0].g = pCCBReader->readFloat ( );
    pColors[0].b = pCCBReader->readFloat ( );
    pColors[0].a = pCCBReader->readFloat ( );
    
    pColors[1].r = pCCBReader->readFloat ( );
    pColors[1].g = pCCBReader->readFloat ( );
    pColors[1].b = pCCBReader->readFloat ( );
    pColors[1].a = pCCBReader->readFloat ( );
    
    return pColors;
}

KDbool* CCNodeLoader::parsePropTypeFlip ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader ) 
{
    KDbool*  pArr = new KDbool[2];

    pArr[0] = pCCBReader->readBool ( );
    pArr[1] = pCCBReader->readBool ( );

    return pArr;
}

ccBlendFunc CCNodeLoader::parsePropTypeBlendFunc ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader ) 
{    
    ccBlendFunc  tBlendFunc;

    tBlendFunc.src = pCCBReader->readInt ( KD_FALSE );
    tBlendFunc.dst = pCCBReader->readInt ( KD_FALSE );
    
    return tBlendFunc;
}

std::string CCNodeLoader::parsePropTypeFntFile ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    return pCCBReader->readCachedString ( );
}

std::string CCNodeLoader::parsePropTypeString ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    return pCCBReader->readCachedString ( );
}

std::string CCNodeLoader::parsePropTypeText ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader ) 
{
    return pCCBReader->readCachedString ( );
}

std::string CCNodeLoader::parsePropTypeFontTTF ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
	std::string  sFontTTF = pCCBReader->readCachedString ( );
    return sFontTTF;
}

BlockData* CCNodeLoader::parsePropTypeBlock ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    std::string  sSelectorName   = pCCBReader->readCachedString ( );
    KDint		 nSelectorTarget = pCCBReader->readInt ( KD_FALSE );

    if ( nSelectorTarget != kCCBTargetTypeNone )
	{
        CCObject*  pTarget = KD_NULL;
        if ( !pCCBReader->isJSControlled ( ) )
		{            
            if ( nSelectorTarget == kCCBTargetTypeDocumentRoot )
			{
                pTarget = pCCBReader->getAnimationManager ( )->getRootNode ( );
            } 
			else if ( nSelectorTarget == kCCBTargetTypeOwner ) 
			{
                pTarget = pCCBReader->getOwner ( );
            }
            
            if ( pTarget != KD_NULL )
			{
                if ( sSelectorName.length ( ) > 0 )
				{
                    SEL_MenuHandler  pSelMenuHandler = 0;
                    
                    CCBSelectorResolver*  pTargetAsCCBSelectorResolver = dynamic_cast<CCBSelectorResolver*> ( pTarget );
                    
                    if ( pTargetAsCCBSelectorResolver != KD_NULL )
					{
                        pSelMenuHandler = pTargetAsCCBSelectorResolver->onResolveCCBCCMenuItemSelector ( pTarget, sSelectorName.c_str ( ) );
                    }

                    if ( pSelMenuHandler == 0 )
					{
                        CCBSelectorResolver*  pCCBSelectorResolver = pCCBReader->getCCBSelectorResolver ( );
                        if ( pCCBSelectorResolver != KD_NULL )
						{
                            pSelMenuHandler = pCCBSelectorResolver->onResolveCCBCCMenuItemSelector ( pTarget, sSelectorName.c_str ( ) );
                        }
                    }
                    
                    if ( pSelMenuHandler == 0 )
					{
                        CCLOG ( "Skipping selector '%s' since no CCBSelectorResolver is present.", sSelectorName.c_str ( ) );
                    } 
					else
					{
                        BlockData*  pBlockData = new BlockData ( );
                        pBlockData->m_pSELMenuHandler = pSelMenuHandler;                        
                        pBlockData->m_pTarget = pTarget;
                        
                        return pBlockData;
                    }
                } 
				else
				{
                    CCLOG ( "Unexpected empty selector." );
                }
            } 
			else
			{
                CCLOG ( "Unexpected NULL target for selector." );
            }
        }
		else
		{
            if ( nSelectorTarget == kCCBTargetTypeDocumentRoot )
			{
                pCCBReader->addDocumentCallbackNode ( pNode );
                pCCBReader->addDocumentCallbackName ( sSelectorName );
                
            } 
			else 
			{
                pCCBReader->addOwnerCallbackNode ( pNode );
                pCCBReader->addOwnerCallbackName ( sSelectorName );
            }
        }
    }

    return KD_NULL;
}

BlockCCControlData* CCNodeLoader::parsePropTypeBlockCCControl ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader )
{
    std::string  sSelectorName = pCCBReader->readCachedString ( );
    KDint  nSelectorTarget = pCCBReader->readInt ( KD_FALSE );
    KDint  nControlEvents  = pCCBReader->readInt ( KD_FALSE );

    if ( nSelectorTarget != kCCBTargetTypeNone )
	{        
        if ( !pCCBReader->isJSControlled ( ) ) 
		{
            CCObject*  pTarget = KD_NULL;
            if ( nSelectorTarget == kCCBTargetTypeDocumentRoot )
			{
                pTarget = pCCBReader->getAnimationManager ( )->getRootNode ( );
            } 
			else if ( nSelectorTarget == kCCBTargetTypeOwner )
			{
                pTarget = pCCBReader->getOwner ( );
            }
            
            if ( pTarget != KD_NULL )
			{
                if ( sSelectorName.length ( ) > 0 )
				{
                    SEL_CCControlHandler  pSelCCControlHandler = 0;
                    
                    CCBSelectorResolver*  pTargetAsCCBSelectorResolver = dynamic_cast<CCBSelectorResolver*> ( pTarget );
                    
                    if ( pTargetAsCCBSelectorResolver != KD_NULL )
					{
                        pSelCCControlHandler = pTargetAsCCBSelectorResolver->onResolveCCBCCControlSelector ( pTarget, sSelectorName.c_str ( ) );
                    }

                    if ( pSelCCControlHandler == 0 )
					{
                        CCBSelectorResolver*  pCCBSelectorResolver = pCCBReader->getCCBSelectorResolver ( );
                        if ( pCCBSelectorResolver != NULL )
						{
                            pSelCCControlHandler = pCCBSelectorResolver->onResolveCCBCCControlSelector ( pTarget, sSelectorName.c_str ( ) );
                        }
                    }
                    
                    if ( pSelCCControlHandler == 0 )
					{
                        CCLOG ( "Skipping selector '%s' since no CCBSelectorResolver is present.", sSelectorName.c_str ( ) );
                    }
					else 
					{
                        BlockCCControlData*  pBlockCCControlData = new BlockCCControlData ( );
                        pBlockCCControlData->m_pSELCCControlHandler = pSelCCControlHandler;                        
                        pBlockCCControlData->m_pTarget = pTarget;
                        pBlockCCControlData->m_nControlEvents = nControlEvents;
                        
                        return pBlockCCControlData;
                    }
                }
				else
				{
                    CCLOG ( "Unexpected empty selector." );
                }
            } 
			else
			{
                CCLOG ( "Unexpected NULL target for selector." );
            }
        } 
		else 
		{
            if ( nSelectorTarget == kCCBTargetTypeDocumentRoot )
			{
                pCCBReader->addDocumentCallbackNode ( pNode );
                pCCBReader->addDocumentCallbackName ( sSelectorName );
                
            } 
			else 
			{
                pCCBReader->addOwnerCallbackNode ( pNode );
                pCCBReader->addOwnerCallbackName ( sSelectorName );
            }
        }
    }

    return KD_NULL;
}

CCNode* CCNodeLoader::parsePropTypeCCBFile ( CCNode* pNode, CCNode* pParent, CCBReader* pCCBReader ) 
{
    std::string  sCCBFileName = pCCBReader->getCCBRootPath ( ) + pCCBReader->readCachedString ( );

    // Change path extension to .ccbi. 
    std::string  sCCBFileWithoutPathExtension = CCBReader::deletePathExtension ( sCCBFileName.c_str ( ) );
    sCCBFileName = sCCBFileWithoutPathExtension + ".ccbi";
    
    // Load sub file
	std::string  sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( sCCBFileName.c_str ( ) );
    KDsize   uSize  = 0;
	KDubyte* pBytes = CCFileUtils::sharedFileUtils ( )->getFileData ( sPath.c_str ( ), "rb", &uSize );

    CCBReader*  pCCBReader2 = new CCBReader ( pCCBReader );
    pCCBReader2->autorelease ( );
	pCCBReader2->getAnimationManager ( )->setRootContainerSize ( pParent->getContentSize ( ) );
    
    CCData*  pData = new CCData ( pBytes, uSize );
    CC_SAFE_DELETE_ARRAY ( pBytes );

    pData->retain ( );
    pCCBReader2->m_pData		= pData;
    pCCBReader2->m_pBytes		= pData->getBytes ( );
    pCCBReader2->m_nCurrentByte = 0;
    pCCBReader2->m_nCurrentBit	= 0;
    CC_SAFE_RETAIN ( pCCBReader2->m_pOwner );
    pCCBReader2->m_pOwner = pCCBReader->m_pOwner;

	pCCBReader2->getAnimationManager ( )->m_pOwner = pCCBReader->m_pOwner;

//    pCCBReader2->m_aOwnerOutletNames = pCCBReader->m_aOwnerOutletNames;
//    pCCBReader2->m_pOwnerOutletNodes = pCCBReader->m_pOwnerOutletNodes;
//    pCCBReader2->m_pOwnerOutletNodes->retain ( );
//    pCCBReader2->m_aOwnerCallbackNames = pCCBReader->m_aOwnerCallbackNames;
//    pCCBReader2->m_pOwnerCallbackNodes = pCCBReader->m_pOwnerCallbackNodes;
//    pCCBReader2->m_pOwnerCallbackNodes->retain ( );

    pData->release ( );
    
    CCNode*  pCCBFileNode = pCCBReader2->readFileWithCleanUp ( KD_FALSE, pCCBReader->getAnimationManagers ( ) );
    
    if ( pCCBFileNode && pCCBReader2->getAnimationManager ( )->getAutoPlaySequenceId ( ) != -1 )
    {
        // Auto play animations
        pCCBReader2->getAnimationManager ( )->runAnimationsForSequenceIdTweenDuration ( pCCBReader2->getAnimationManager ( )->getAutoPlaySequenceId ( ), 0 );
    }

    return pCCBFileNode;
}

KDvoid CCNodeLoader::onHandlePropTypePosition ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint pPosition, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_POSITION ) == 0 ) 
	{
        pNode->setPosition ( pPosition );
    }
	else
	{
        ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
    }
}

KDvoid CCNodeLoader::onHandlePropTypePoint ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint pPoint, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_ANCHORPOINT ) == 0 )
	{
        pNode->setAnchorPoint ( pPoint );
    }
	else
	{
        ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
    }
}

KDvoid CCNodeLoader::onHandlePropTypePointLock ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCPoint pPointLock, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeSize ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSize pSize, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_CONTENTSIZE ) == 0 ) 
	{
        pNode->setContentSize ( pSize );
    }
	else
	{
        ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
    }
}

KDvoid CCNodeLoader::onHandlePropTypeFloatXY ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat* pFloat, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_SKEW ) == 0 ) 
	{
        pNode->setSkewX ( pFloat[0] );
        pNode->setSkewY ( pFloat[1] );
    } 
	else 
	{
        ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
    }
}

KDvoid CCNodeLoader::onHandlePropTypeScaleLock ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat* pScaleLock, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_SCALE ) == 0 )
	{
        pNode->setScaleX ( pScaleLock[0] );
        pNode->setScaleY ( pScaleLock[1] );
    }
	else
	{
        ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
    }
}

KDvoid CCNodeLoader::onHandlePropTypeFloat ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat pFloat, CCBReader* pCCBReader )
{
//    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );

    // It may be a custom property, add it to custom property dictionary.
    m_pCustomProperties->setObject ( CCBValue::create ( pFloat ), pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeDegrees ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat pDegrees, CCBReader* pCCBReader )
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_ROTATION ) == 0 ) 
	{
        pNode->setRotation ( pDegrees );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ROTATIONX ) == 0 )
	{
        pNode->setRotationX ( pDegrees );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_ROTATIONY ) == 0 ) 
	{
        pNode->setRotationY ( pDegrees );
    }
	else 
	{
        ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
    }
}

KDvoid CCNodeLoader::onHandlePropTypeFloatScale ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat pFloatScale, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeInteger ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDint pInteger, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_TAG ) == 0 )
	{
        pNode->setTag ( pInteger );
    }
	else 
	{
//        ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
        
		// It may be a custom property, add it to custom property dictionary.
        m_pCustomProperties->setObject ( CCBValue::create ( pInteger ), pPropertyName );
    }
}

KDvoid CCNodeLoader::onHandlePropTypeIntegerLabeled ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDint pIntegerLabeled, CCBReader* pCCBReader ) 
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeFloatVar ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDfloat* pFloatVar, CCBReader* pCCBReader ) 
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeCheck ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool pCheck, CCBReader* pCCBReader ) 
{
    if ( kdStrcmp ( pPropertyName, PROPERTY_VISIBLE ) == 0 ) 
	{
        pNode->setVisible ( pCheck );
    } 
	else if ( kdStrcmp ( pPropertyName, PROPERTY_IGNOREANCHORPOINTFORPOSITION ) == 0 ) 
	{
        pNode->ignoreAnchorPointForPosition ( pCheck );
    }
	else 
	{
//        ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );

        // It may be a custom property, add it to custom property dictionary.
        m_pCustomProperties->setObject ( CCBValue::create ( pCheck ), pPropertyName );

    }
}

KDvoid CCNodeLoader::onHandlePropTypeSpriteFrame ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCSpriteFrame* pCCSpriteFrame, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeAnimation ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCAnimation* pCCAnimation, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeTexture ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCTexture2D* pCCTexture2D, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeByte ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDubyte pByte, CCBReader* pCCBReader ) 
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeColor3 ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor3B pCCColor3B, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeColor4FVar ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccColor4F* pCCColor4FVar, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeFlip ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, KDbool* pFlip, CCBReader* pCCBReader ) 
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeBlendFunc ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeFntFile ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pFntFile, CCBReader* pCCBReader ) 
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeString ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pString, CCBReader* pCCBReader ) 
{
//    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );

    // It may be a custom property, add it to custom property dictionary.
    m_pCustomProperties->setObject ( CCBValue::create ( pString ), pPropertyName );

}

KDvoid CCNodeLoader::onHandlePropTypeText ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pText, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeFontTTF ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, const KDchar* pFontTTF, CCBReader* pCCBReader ) 
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeBlock ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, BlockData* pBlockData, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeBlockCCControl ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, BlockCCControlData* pBlockCCControlData, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

KDvoid CCNodeLoader::onHandlePropTypeCCBFile ( CCNode* pNode, CCNode* pParent, const KDchar* pPropertyName, CCNode* pCCBFileNode, CCBReader* pCCBReader )
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY ( pPropertyName );
}

NS_CC_END