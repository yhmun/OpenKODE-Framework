/* --------------------------------------------------------------------------
 *
 *      File            CCBReader.h
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

#ifndef __CCBReader_h__
#define __CCBReader_h__

#include "../../base_nodes/CCNode.h"
#include "../../cocoa/CCDictionary.h"
#include "CCBSequence.h"

#include <vector>
#include <set>

NS_CC_BEGIN

#define CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD( _TYPE, _METHOD )					\
static _TYPE*  _METHOD ( KDvoid )													\
{																					\
    _TYPE*  pRet = new _TYPE ( );													\
																					\
	if ( pRet )																		\
	{																				\
        pRet->autorelease ( );														\
    }																				\
																					\
    return pRet;																	\
}

#define CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD( _TYPE, _METHOD )		\
static _TYPE*  _METHOD ( KDvoid )													\
{																					\
    _TYPE*  pRet = new _TYPE ( );													\
																					\
	if ( pRet && pRet->init ( ) )													\
	{																				\
        pRet->autorelease ( );														\
    }																				\
	else																			\
	{																				\
		CC_SAFE_DELETE ( pRet );													\
	}																				\
																					\
    return pRet;																	\
}

#define kCCBVersion						5

enum 
{
    kCCBPropTypePosition			= 0	,
    kCCBPropTypeSize					,
    kCCBPropTypePoint					,
    kCCBPropTypePointLock				,
    kCCBPropTypeScaleLock				,
    kCCBPropTypeDegrees					,
    kCCBPropTypeInteger					,
    kCCBPropTypeFloat					,
    kCCBPropTypeFloatVar				,
    kCCBPropTypeCheck					,
    kCCBPropTypeSpriteFrame				,
    kCCBPropTypeTexture					,
    kCCBPropTypeByte					,
    kCCBPropTypeColor3					,
    kCCBPropTypeColor4FVar				,
    kCCBPropTypeFlip					,
    kCCBPropTypeBlendMode				,
    kCCBPropTypeFntFile					,
    kCCBPropTypeText					,
    kCCBPropTypeFontTTF					,
    kCCBPropTypeIntegerLabeled			,
    kCCBPropTypeBlock					,
	kCCBPropTypeAnimation				,
    kCCBPropTypeCCBFile					,
    kCCBPropTypeString					,
    kCCBPropTypeBlockCCControl			,
    kCCBPropTypeFloatScale				,
	kCCBPropTypeFloatXY					,
};

enum 
{
    kCCBFloat0						= 0	,
    kCCBFloat1							,
    kCCBFloatMinus1						,
    kCCBFloat05							,
    kCCBFloatInteger					,
    kCCBFloatFull						,
};

enum 
{
    kCCBPlatformAll					= 0	,
    kCCBPlatformIOS						,
    kCCBPlatformMac						,
};

enum
{
    kCCBTargetTypeNone				= 0	,
    kCCBTargetTypeDocumentRoot		= 1	,
    kCCBTargetTypeOwner				= 2	,
};

enum
{
    kCCBKeyframeEasingInstant			,
    
    kCCBKeyframeEasingLinear			,
    
    kCCBKeyframeEasingCubicIn			,
    kCCBKeyframeEasingCubicOut			,
    kCCBKeyframeEasingCubicInOut		,
    
    kCCBKeyframeEasingElasticIn			,
    kCCBKeyframeEasingElasticOut		,
    kCCBKeyframeEasingElasticInOut		,
    
    kCCBKeyframeEasingBounceIn			,
    kCCBKeyframeEasingBounceOut			,
    kCCBKeyframeEasingBounceInOut		,
    
    kCCBKeyframeEasingBackIn			,
    kCCBKeyframeEasingBackOut			,
    kCCBKeyframeEasingBackInOut			,
};

enum
{
    kCCBPositionTypeRelativeBottomLeft	,
    kCCBPositionTypeRelativeTopLeft		,
    kCCBPositionTypeRelativeTopRight	,
    kCCBPositionTypeRelativeBottomRight	,
    kCCBPositionTypePercent				,
    kCCBPositionTypeMultiplyResolution	,
};

enum
{
    kCCBSizeTypeAbsolute				,
    kCCBSizeTypePercent					,
    kCCBSizeTypeRelativeContainer		,
    kCCBSizeTypeHorizontalPercent		,
    kCCBSizeTypeVerticalPercent			,
    kCCBSizeTypeMultiplyResolution		,
};

enum
{
    kCCBScaleTypeAbsolute				,
    kCCBScaleTypeMultiplyResolution		,
};

/**
 * @addtogroup cocosbuilder
 * @{
 */

class CCBFile : public CCNode
{	    
	public :

		CCBFile ( KDvoid );
	    
		static CCBFile*		create ( KDvoid );
	    
	public :

		CCNode*				getCCBFileNode ( KDvoid );
		KDvoid				setCCBFileNode ( CCNode* pNode );		// retain

	private :

		CCNode*				m_pCCBFileNode;
};

/* Forward declaration. */
class CCNodeLoader;
class CCNodeLoaderLibrary;
class CCNodeLoaderListener;
class CCBMemberVariableAssigner;
class CCBSelectorResolver;
class CCBAnimationManager;
class CCData;
class CCBKeyframe;

/**
 * @brief Parse CCBI file which is generated by CocosBuilder
 */
class CCBReader : public CCObject
{
    public :
				 CCBReader ( CCNodeLoaderLibrary* pCCNodeLoaderLibrary, CCBMemberVariableAssigner* pCCBMemberVariableAssigner = KD_NULL, CCBSelectorResolver* pCCBSelectorResolver = KD_NULL, CCNodeLoaderListener* pCCNodeLoaderListener = KD_NULL );
				 CCBReader ( CCBReader* pCCBReader );
				 CCBReader ( KDvoid );
        virtual ~CCBReader ( KDvoid );
    
    public :

		virtual	 KDbool				init ( KDvoid );

		KDvoid						setCCBRootPath ( const KDchar* szCCBRootPath );
	    const std::string&			getCCBRootPath ( KDvoid ) const;

		CCNode*						readNodeGraphFromFile ( const KDchar* szCCBFileName );
		CCNode*						readNodeGraphFromFile ( const KDchar* szCCBFileName, CCObject* pOwner);
		CCNode*						readNodeGraphFromFile ( const KDchar* szCCBFileName, CCObject* pOwner, const CCSize& tParentSize );

		CCNode*						readNodeGraphFromData ( CCData* pData, CCObject* pOwner, const CCSize& tParentSize );

		CCScene*					createSceneWithNodeGraphFromFile ( const KDchar* szCCBFileName );
		CCScene*					createSceneWithNodeGraphFromFile ( const KDchar* szCCBFileName, CCObject* pOwner );
		CCScene*					createSceneWithNodeGraphFromFile ( const KDchar* szCCBFileName, CCObject* pOwner, const CCSize& tParentSize );

        CCBMemberVariableAssigner*  getCCBMemberVariableAssigner ( KDvoid );
        CCBSelectorResolver*	    getCCBSelectorResolver ( KDvoid );

		CCBAnimationManager*		getAnimationManager ( KDvoid );
		KDvoid						setAnimationManager ( CCBAnimationManager* pAnimationManager );
    
		// Used in CCNodeLoader::parseProperties()
		std::set<std::string>*		getAnimatedProperties ( KDvoid );
		std::set<std::string>&		getLoadedSpriteSheet  ( KDvoid );
		CCObject*					getOwner ( KDvoid );

        /* Utility methods. */
        static std::string			lastPathComponent	( const KDchar* pString );
        static std::string			deletePathExtension ( const KDchar* pString );
        static std::string			toLowerCase			( const KDchar* pCCString );
        static KDbool				endsWith			( const KDchar* pString, const KDchar* pEnding );
    
        /* Parse methods. */
        KDint						readInt	( KDbool pSigned);
        KDubyte						readByte	( KDvoid );
        KDbool						readBool	( KDvoid );
		std::string					readUTF8	( KDvoid );
        KDfloat						readFloat	( KDvoid );
        std::string					readCachedString ( KDvoid );
		KDbool						isJSControlled ( KDvoid );

		KDbool						readCallbackKeyframesForSeq ( CCBSequence* pSeq );
		KDbool						readSoundKeyframesForSeq    ( CCBSequence* pSeq );

		CCArray*					getOwnerCallbackNames();
		CCArray*					getOwnerCallbackNodes();
		CCArray*					getOwnerOutletNames();
		CCArray*					getOwnerOutletNodes();
		CCArray*					getNodesWithAnimationManagers();
		CCArray*					getAnimationManagersForNodes();

		CCDictionary*				getAnimationManagers ( KDvoid );
		KDvoid						setAnimationManagers ( CCDictionary* pDict );	// weak reference

		KDvoid						addOwnerCallbackName ( std::string sName );
		KDvoid						addOwnerCallbackNode ( CCNode* pNode );

		KDvoid						addDocumentCallbackName ( std::string sName );
		KDvoid						addDocumentCallbackNode ( CCNode* pNode );

		static KDfloat  			getResolutionScale ( KDvoid );
		static KDvoid				setResolutionScale ( KDfloat fScale );
    
		CCNode*						readFileWithCleanUp ( KDbool bCleanUp, CCDictionary* pDict );
		
    private :

		KDvoid						cleanUpNodeGraph ( CCNode* pNode );
		
		KDbool						readSequences ( KDvoid );

		CCBKeyframe*				readKeyframe ( KDint nType );

        KDbool						readHeader ( KDvoid );
        
		KDbool						readStringCache ( KDvoid );

        CCNode*						readNodeGraph ( KDvoid );
        CCNode*						readNodeGraph ( CCNode* pParent );

        KDbool						getBit ( KDvoid );
        KDvoid						alignBits ( KDvoid );

    public :
 
		KDbool						m_bJSControlled;

    private :


		CCData*						m_pData;
		KDubyte*					m_pBytes;
		KDint						m_nCurrentByte;
		KDint						m_nCurrentBit;
	    
		std::vector<std::string>	m_aStringCache;
		std::set<std::string>		m_aLoadedSpriteSheets;
	    
		CCObject*					m_pOwner;
	    
		CCBAnimationManager*		m_pActionManager;		// retain
		CCDictionary*				m_pActionManagers;

		std::set<std::string>*		m_pAnimatedProps;

		CCNodeLoaderLibrary*		m_pCCNodeLoaderLibrary;
		CCNodeLoaderListener*		m_pCCNodeLoaderListener;
		CCBMemberVariableAssigner*	m_pCCBMemberVariableAssigner;
		CCBSelectorResolver*		m_pCCBSelectorResolver; 

		std::vector<std::string>	m_aOwnerOutletNames;
		CCArray*					m_pOwnerOutletNodes;
		CCArray*					m_pNodesWithAnimationManagers;
		CCArray*					m_pAnimationManagersForNodes;
    
		std::vector<std::string>	m_aOwnerCallbackNames;
		CCArray*					m_pOwnerCallbackNodes;
		std::string					m_sCCBRootPath;
		KDbool						m_bHasScriptingOwner;  

		friend class CCNodeLoader;
};

// end of effects group
/// @}

NS_CC_END

#endif // __CCBReader_h__