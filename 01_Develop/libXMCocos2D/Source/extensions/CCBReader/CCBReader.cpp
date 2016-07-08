/* --------------------------------------------------------------------------
 *
 *      File            CCBReader.cpp
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
#include "extensions/CCBReader/CCNodeLoaderLibrary.h"
#include "extensions/CCBReader/CCNodeLoader.h"
#include "extensions/CCBReader/CCNodeLoaderListener.h"
#include "extensions/CCBReader/CCBMemberVariableAssigner.h"
#include "extensions/CCBReader/CCBSelectorResolver.h"
#include "extensions/CCBReader/CCBAnimationManager.h"
#include "cocoa/CCData.h"
#include "layers_scenes_transitions_nodes/CCScene.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "textures/CCTextureCache.h"
#include "support/CCDirector.h"
#include "support/CCEGLView.h"
#include "support/CCFileUtils.h"

#include <algorithm>
#include <ctype.h>

NS_CC_BEGIN

//
// Implementation of CCBFile
// 

CCBFile::CCBFile ( KDvoid )
{
	m_pCCBFileNode = KD_NULL;
}

CCBFile* CCBFile::create ( KDvoid )
{
    CCBFile*  pRet = new CCBFile ( );
    
    if ( pRet )
    {
        pRet->autorelease ( );
    }
    
    return pRet;
}

CCNode* CCBFile::getCCBFileNode ( KDvoid )
{
    return m_pCCBFileNode;
}

KDvoid CCBFile::setCCBFileNode ( CCNode* pNode )
{
    CC_SAFE_RELEASE ( m_pCCBFileNode );
    m_pCCBFileNode = pNode;
    CC_SAFE_RETAIN  ( m_pCCBFileNode );
}

//
// Implementation of CCBReader
// 

CCBReader::CCBReader ( CCNodeLoaderLibrary* pCCNodeLoaderLibrary, CCBMemberVariableAssigner* pCCBMemberVariableAssigner, CCBSelectorResolver* pCCBSelectorResolver, CCNodeLoaderListener* pCCNodeLoaderListener )
{
	m_pData							= KD_NULL;
	m_pBytes						= KD_NULL;
	m_nCurrentByte					= -1;
	m_nCurrentBit					= -1;
	m_pOwner						= KD_NULL;
	m_pActionManager				= KD_NULL;
	m_pActionManagers				= KD_NULL;
	m_pAnimatedProps				= KD_NULL;
	m_pOwnerOutletNodes				= KD_NULL;
	m_pNodesWithAnimationManagers	= KD_NULL;
	m_pAnimationManagersForNodes	= KD_NULL;
	m_pOwnerCallbackNodes			= KD_NULL;
	m_bHasScriptingOwner			= KD_FALSE;

    m_pCCNodeLoaderLibrary			= pCCNodeLoaderLibrary;
	m_pCCNodeLoaderLibrary->retain ( );
	m_pCCBMemberVariableAssigner	= pCCBMemberVariableAssigner;
    m_pCCBSelectorResolver			= pCCBSelectorResolver;
    m_pCCNodeLoaderListener			= pCCNodeLoaderListener;
    
	init ( );
}

CCBReader::CCBReader ( CCBReader* pCCBReader )
{
	m_pData							= KD_NULL;
	m_pBytes						= KD_NULL;
	m_nCurrentByte					= -1;
	m_nCurrentBit					= -1;
	m_pOwner						= KD_NULL;
	m_pActionManager				= KD_NULL;
	m_pActionManagers				= KD_NULL;
	m_pAnimatedProps				= KD_NULL;
	m_pOwnerOutletNodes				= KD_NULL;
	m_pNodesWithAnimationManagers	= KD_NULL;
	m_pAnimationManagersForNodes	= KD_NULL;
	m_pOwnerCallbackNodes			= KD_NULL;
	m_bHasScriptingOwner			= KD_FALSE;

    m_aLoadedSpriteSheets			= pCCBReader->m_aLoadedSpriteSheets;
    m_pCCNodeLoaderLibrary			= pCCBReader->m_pCCNodeLoaderLibrary;
	m_pCCNodeLoaderLibrary->retain ( );
    
    m_pCCBMemberVariableAssigner	= pCCBReader->m_pCCBMemberVariableAssigner;
    m_pCCBSelectorResolver			= pCCBReader->m_pCCBSelectorResolver;
	m_pCCNodeLoaderListener			= pCCBReader->m_pCCNodeLoaderListener;

    m_aOwnerCallbackNames			= pCCBReader->m_aOwnerCallbackNames;
    m_pOwnerCallbackNodes			= pCCBReader->m_pOwnerCallbackNodes;
    m_pOwnerCallbackNodes->retain ( );
    m_aOwnerOutletNames				= pCCBReader->m_aOwnerOutletNames;
    m_pOwnerOutletNodes				= pCCBReader->m_pOwnerOutletNodes;
    m_pOwnerOutletNodes->retain ( );	

	init ( );
}

CCBReader::CCBReader ( KDvoid )
{
	m_pData							= KD_NULL;
	m_pBytes						= KD_NULL;
	m_nCurrentByte					= -1;
	m_nCurrentBit					= -1;
	m_pOwner						= KD_NULL;
	m_pActionManager				= KD_NULL;
	m_pActionManagers				= KD_NULL;
    m_pCCNodeLoaderLibrary			= KD_NULL;
	m_pCCNodeLoaderListener			= KD_NULL;
    m_pCCBMemberVariableAssigner	= KD_NULL;
    m_pCCBSelectorResolver			= KD_NULL;
	m_pNodesWithAnimationManagers	= KD_NULL;
	m_pAnimationManagersForNodes	= KD_NULL;
	m_bHasScriptingOwner			= KD_FALSE;

	init ( );
}

CCBReader::~CCBReader ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pOwner );
    CC_SAFE_RELEASE ( m_pData );

    this->m_pCCNodeLoaderLibrary->release ( );

	CC_SAFE_RELEASE ( m_pOwnerOutletNodes );
    m_aOwnerOutletNames.clear ( );
    CC_SAFE_RELEASE ( m_pOwnerCallbackNodes );
    m_aOwnerCallbackNames.clear ( );

    // Clear string cache. 
    this->m_aStringCache.clear ( );
	CC_SAFE_RELEASE ( m_pNodesWithAnimationManagers );
    CC_SAFE_RELEASE ( m_pAnimationManagersForNodes );
    
	setAnimationManager ( KD_NULL );
}

KDvoid CCBReader::setCCBRootPath ( const KDchar* szCCBRootPath )
{
    CCAssert ( szCCBRootPath != KD_NULL, "" );
    m_sCCBRootPath = szCCBRootPath;
}

const std::string& CCBReader::getCCBRootPath ( KDvoid ) const
{
    return m_sCCBRootPath;
}

KDbool CCBReader::init ( KDvoid )
{
    // Setup action manager
    CCBAnimationManager*  pActionManager = new CCBAnimationManager ( );
    setAnimationManager ( pActionManager );
    pActionManager->release ( );
    
    // Setup resolution scale and container size
    m_pActionManager->setRootContainerSize ( CCDirector::sharedDirector ( )->getWinSize ( ) );
    
    return KD_TRUE;
}

CCBAnimationManager* CCBReader::getAnimationManager ( KDvoid )
{
    return m_pActionManager;
}

KDvoid CCBReader::setAnimationManager ( CCBAnimationManager* pAnimationManager )
{
    CC_SAFE_RELEASE ( m_pActionManager );
    m_pActionManager = pAnimationManager;
    CC_SAFE_RETAIN  ( m_pActionManager );
}

CCDictionary* CCBReader::getAnimationManagers ( KDvoid )
{
    return m_pActionManagers;
}

KDvoid CCBReader::setAnimationManagers ( CCDictionary* pDict )
{
    m_pActionManagers = pDict;
}

CCBMemberVariableAssigner* CCBReader::getCCBMemberVariableAssigner ( KDvoid ) 
{
    return m_pCCBMemberVariableAssigner;
}

CCBSelectorResolver * CCBReader::getCCBSelectorResolver ( KDvoid )
{
    return m_pCCBSelectorResolver;
}

std::set<std::string>* CCBReader::getAnimatedProperties ( KDvoid )
{
    return m_pAnimatedProps;
}

std::set<std::string>& CCBReader::getLoadedSpriteSheet ( KDvoid )
{
    return m_aLoadedSpriteSheets;
}

CCObject* CCBReader::getOwner ( KDvoid )
{
    return m_pOwner;
}

CCNode* CCBReader::readNodeGraphFromFile ( const KDchar* szCCBFileName )
{
    return this->readNodeGraphFromFile ( szCCBFileName, KD_NULL );
}

CCNode* CCBReader::readNodeGraphFromFile ( const KDchar* szCCBFileName, CCObject* pOwner ) 
{
    return this->readNodeGraphFromFile ( szCCBFileName, pOwner, CCDirector::sharedDirector ( )->getWinSize ( ) );
}

CCNode* CCBReader::readNodeGraphFromFile ( const KDchar* szCCBFileName, CCObject* pOwner, const CCSize& tParentSize ) 
{
    if ( KD_NULL == szCCBFileName || kdStrlen ( szCCBFileName ) == 0 )
    {
        return KD_NULL;
    }

    std::string  sCCBFileName ( szCCBFileName );
    std::string  sSuffix ( ".ccbi" );
    
	// Add ccbi suffix
    if ( !CCBReader::endsWith ( sCCBFileName.c_str ( ), sSuffix.c_str ( ) ) )
    {
        sCCBFileName += sSuffix;
    }

	std::string  sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( sCCBFileName.c_str ( ) );
    KDsize    uSize  = 0;
    KDubyte*  pBytes = CCFileUtils::sharedFileUtils ( )->getFileData ( sPath.c_str ( ), "rb", &uSize );
    CCData*   pData  = new CCData ( pBytes, uSize );
    CC_SAFE_DELETE_ARRAY ( pBytes );

    CCNode*   pRet = this->readNodeGraphFromData ( pData, pOwner, tParentSize );
    pData->release ( );
    
    return pRet;
}

CCNode* CCBReader::readNodeGraphFromData ( CCData* pData, CCObject* pOwner, const CCSize& tParentSize )
{
    m_pData = pData;
    CC_SAFE_RETAIN ( m_pData );
    m_pBytes = m_pData->getBytes ( );
    m_nCurrentByte = 0;
    m_nCurrentBit  = 0;
    m_pOwner = pOwner;
    CC_SAFE_RETAIN ( m_pOwner );

    m_pActionManager->setRootContainerSize ( tParentSize );
	m_pActionManager->m_pOwner = m_pOwner;
    
    m_pOwnerOutletNodes   = new CCArray ( );
    m_pOwnerCallbackNodes = new CCArray ( );
    
    CCDictionary*  pAnimationManagers = CCDictionary::create ( );
    CCNode*  pNodeGraph = readFileWithCleanUp ( KD_TRUE, pAnimationManagers );
    
    if ( pNodeGraph && m_pActionManager->getAutoPlaySequenceId ( ) != -1 && !m_bJSControlled )
    {
        // Auto play animations
        m_pActionManager->runAnimationsForSequenceIdTweenDuration ( m_pActionManager->getAutoPlaySequenceId ( ), 0 );
    }

    // Assign actionManagers to userObject
    if ( m_bJSControlled )
	{
        m_pNodesWithAnimationManagers = new CCArray ( );
        m_pAnimationManagersForNodes  = new CCArray ( );
    }
    
    CCDictElement*  pElement = KD_NULL;
    CCDICT_FOREACH ( pAnimationManagers, pElement )
    {
        CCNode*  pNode = (CCNode*) pElement->getIntKey ( );
        CCBAnimationManager*  pManager = (CCBAnimationManager*) pAnimationManagers->objectForKey ( (KDintptr) pNode );
        pNode->setUserObject ( pManager );

        if ( m_bJSControlled )
        {
            m_pNodesWithAnimationManagers->addObject ( pNode );
            m_pAnimationManagersForNodes->addObject ( pManager );
        }
    }
    
    return pNodeGraph;
}

CCScene* CCBReader::createSceneWithNodeGraphFromFile ( const KDchar* szCCBFileName )
{
    return createSceneWithNodeGraphFromFile ( szCCBFileName, KD_NULL );
}

CCScene* CCBReader::createSceneWithNodeGraphFromFile ( const KDchar* szCCBFileName, CCObject* pOwner )
{
    return createSceneWithNodeGraphFromFile ( szCCBFileName, pOwner, CCDirector::sharedDirector ( )->getWinSize ( ) );
}

CCScene* CCBReader::createSceneWithNodeGraphFromFile ( const KDchar* szCCBFileName, CCObject* pOwner, const CCSize& tParentSize )
{
    CCNode*  pNode = readNodeGraphFromFile ( szCCBFileName, pOwner, tParentSize );

    CCScene*  pScene = CCScene::create ( );
    pScene->addChild ( pNode );
    
    return pScene;
}
  
KDvoid CCBReader::cleanUpNodeGraph ( CCNode* pNode )
{
    pNode->setUserObject ( KD_NULL );
    
    CCObject*  pChild = KD_NULL;
    CCARRAY_FOREACH ( pNode->getChildren ( ), pChild )
    {
        cleanUpNodeGraph ( (CCNode*) pChild );
    }
}

CCNode* CCBReader::readFileWithCleanUp ( KDbool bCleanUp, CCDictionary* pDict )
{
    if ( !readHeader ( ) )
    {
        return KD_NULL;
    }
    
    if ( !readStringCache ( ) )
    {
        return KD_NULL;
    }
    
    if ( !readSequences ( ) )
    {
        return KD_NULL;
    }
    
	setAnimationManagers ( pDict );

    CCNode*  pNode = readNodeGraph ( KD_NULL );
	
	m_pActionManagers->setObject ( m_pActionManager, (KDintptr) pNode );
    
    if ( bCleanUp )
    {
        cleanUpNodeGraph ( pNode );
    }
    
    return pNode;
}

KDbool CCBReader::readStringCache ( KDvoid )
{
    KDint  nNumStrings = this->readInt ( KD_FALSE );

    for ( KDint i = 0; i < nNumStrings; i++ )
	{
		this->m_aStringCache.push_back ( this->readUTF8 ( ) );
    }

    return KD_TRUE;
}

KDbool CCBReader::readHeader ( KDvoid )
{
    // If no bytes loaded, don't crash about it. 
    if ( this->m_pBytes == KD_NULL )
	{
        return KD_FALSE;
    }

    // Read magic bytes 
    KDint  nMagicBytes = *((KDint*) ( this->m_pBytes + this->m_nCurrentByte ) );
    this->m_nCurrentByte += 4;

    if ( CC_SWAP_INT32_LITTLE_TO_HOST ( nMagicBytes ) != 'ccbi' )
	{
        return KD_FALSE; 
    }

    // Read version. 
    KDint  nVersion = this->readInt ( KD_FALSE );
    if ( nVersion != kCCBVersion )
	{
        CCLOG ( "WARNING! Incompatible ccbi file version (file: %d reader: %d)", nVersion, kCCBVersion );
        return KD_FALSE;
    }

    // Read JS check
    m_bJSControlled = this->readBool ( );
	m_pActionManager->m_bJSControlled = m_bJSControlled;

    return KD_TRUE;
}

KDubyte CCBReader::readByte ( KDvoid ) 
{
    KDubyte  cByte = this->m_pBytes [ this->m_nCurrentByte ];
    this->m_nCurrentByte++;
    return cByte;
}

KDbool CCBReader::readBool ( KDvoid ) 
{
    return 0 == this->readByte ( ) ? KD_FALSE : KD_TRUE;
}

std::string CCBReader::readUTF8 ( KDvoid )
{
    std::string  sRet;

    KDint  b0 = this->readByte();
    KDint  b1 = this->readByte();

    KDint  nNumBytes = b0 << 8 | b1;

    KDchar*  szStr = (KDchar*) kdMalloc ( nNumBytes + 1 );
    kdMemcpy ( szStr, m_pBytes + m_nCurrentByte, nNumBytes );
    szStr [ nNumBytes ] = '\0';
    sRet = szStr;
    kdFree ( szStr );

    m_nCurrentByte += nNumBytes;

    return sRet;
}

KDbool CCBReader::getBit ( KDvoid ) 
{
    KDbool   bBit;
    KDubyte  cByte = *( this->m_pBytes + this->m_nCurrentByte );
    if ( cByte & ( 1 << this->m_nCurrentBit ) )
	{
        bBit = KD_TRUE;
    } 
	else
	{
        bBit = KD_FALSE;
    }
            
    this->m_nCurrentBit++;

    if ( this->m_nCurrentBit >= 8 )
	{
        this->m_nCurrentBit = 0;
        this->m_nCurrentByte++;
    }
    
    return bBit;
}

KDvoid CCBReader::alignBits ( KDvoid )
{
    if ( this->m_nCurrentBit )
	{
        this->m_nCurrentBit = 0;
        this->m_nCurrentByte++;
    }
}

KDint CCBReader::readInt ( KDbool pSigned )
{
	// Read encoded int
    KDint  nNumBits = 0;
    while ( !this->getBit ( ) )
	{
        nNumBits++;
    }
    
    KDint  nCurrent = 0;
    for ( KDint a = nNumBits - 1; a >= 0; a-- )
	{
        if ( this->getBit ( ) )
		{
            nCurrent |= 1LL << a;
        }
    }
    nCurrent |= 1LL << nNumBits;
    
    KDint  nNum;
    if ( pSigned )
	{
        KDint  s = nCurrent % 2;
        if ( s )
		{
            nNum = (KDint) ( nCurrent * 0.5f );
        } 
		else 
		{
            nNum = (KDint) ( -nCurrent * 0.5f );
        }
    } 
	else
	{
        nNum = nCurrent - 1;
    }
    
    this->alignBits ( );
    
    return nNum;
}


KDfloat CCBReader::readFloat ( KDvoid ) 
{
    KDubyte  cType = this->readByte ( );
    
    switch ( cType )
	{
        case kCCBFloat0			:	return 0;   
        case kCCBFloat1			:	return 1;
        case kCCBFloatMinus1	:	return -1;
        case kCCBFloat05		:	return 0.5f;
        case kCCBFloatInteger	:	return (KDfloat) this->readInt ( KD_TRUE );

        default :
            // using a memcpy since the compiler isn't
            // doing the float ptr math correctly on device.
            // TODO still applies in C++ ? 
            KDfloat*  pF = (KDfloat*) ( this->m_pBytes + this->m_nCurrentByte );
            KDfloat    f = 0;

            // N.B - in order to avoid an unaligned memory access crash on 'memcpy()' the the (void*) casts of the source and
            // destination pointers are EXTREMELY important for the ARM compiler.
            //
            // Without a (void*) cast, the ARM compiler makes the assumption that the float* pointer is naturally aligned
            // according to it's type size (aligned along 4 byte boundaries) and thus tries to call a more optimized
            // version of memcpy() which makes this alignment assumption also. When reading back from a file of course our pointers
            // may not be aligned, hence we need to avoid the compiler making this assumption. The (void*) cast serves this purpose,
            // and causes the ARM compiler to choose the slower, more generalized (unaligned) version of memcpy()
            //
            // For more about this compiler behavior, see:
            // http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka3934.html
            kdMemcpy ( (KDvoid*) &f, (const KDvoid*) pF, sizeof ( KDfloat ) );
            this->m_nCurrentByte += sizeof ( KDfloat );               
            return f;
    }
}

std::string CCBReader::readCachedString ( KDvoid )
{
    KDint  n = this->readInt ( KD_FALSE );
    return this->m_aStringCache [ n ];
}

CCNode* CCBReader::readNodeGraph ( CCNode* pParent )
{
    // Read class name. 
    std::string  sClassName = this->readCachedString ( );

	std::string  sJSControlledName;
    
    if ( m_bJSControlled )
	{
        sJSControlledName = this->readCachedString ( );
    }

    // Read assignment type and name
    KDint        nMemberVarAssignmentType = this->readInt ( KD_FALSE );
    std::string  sMemberVarAssignmentName;

    if ( nMemberVarAssignmentType != kCCBTargetTypeNone ) 
	{
        sMemberVarAssignmentName = this->readCachedString ( );
    }

	CCNodeLoader*  pCCNodeLoader = this->m_pCCNodeLoaderLibrary->getCCNodeLoader ( sClassName.c_str ( ) );
    if ( !pCCNodeLoader )
    {
		CCLOG ( "no corresponding node loader for %s", sClassName.c_str ( ) );
        return KD_NULL;
    }

    CCNode*  pNode = pCCNodeLoader->loadCCNode ( pParent, this );

    // Set root node
    if ( !m_pActionManager->getRootNode ( ) )
    {
        m_pActionManager->setRootNode ( pNode );
    }
    
	// Assign controller
    if ( m_bJSControlled && pNode == m_pActionManager->getRootNode ( ) )
    {
        m_pActionManager->setDocumentControllerName ( sJSControlledName );
    }

    // Read animated properties
    CCDictionary*  pSeqs = CCDictionary::create ( );
	m_pAnimatedProps = new std::set<std::string> ( );
    
    KDint  nNumSequence = readInt ( KD_FALSE );
    for ( KDint i = 0; i < nNumSequence; ++i )
    {
        KDint  nSeqId = readInt ( KD_FALSE );
        CCDictionary*  pSeqNodeProps = CCDictionary::create ( );
        
        KDint  nNumProps = readInt ( KD_FALSE );
        
        for ( KDint j = 0; j < nNumProps; ++j )
        {
            CCBSequenceProperty*  pSeqProp = new CCBSequenceProperty ( );
            pSeqProp->autorelease ( );
            
			pSeqProp->setName ( readCachedString ( ).c_str ( ) );
            pSeqProp->setType ( readInt ( KD_FALSE ) );
            m_pAnimatedProps->insert ( pSeqProp->getName ( ) );
            
            KDint  nNumKeyframes = readInt ( KD_FALSE );
            
            for ( KDint k = 0; k < nNumKeyframes; ++k )
            {
                CCBKeyframe*  pKeyframe = readKeyframe ( pSeqProp->getType ( ) );
                
                pSeqProp->getKeyframes ( )->addObject ( pKeyframe );
            }
            
            pSeqNodeProps->setObject ( pSeqProp, pSeqProp->getName ( ) );
        }
        
        pSeqs->setObject ( pSeqNodeProps, nSeqId );
    }
    
    if ( pSeqs->count ( ) > 0 )
    {
        m_pActionManager->addNode ( pNode, pSeqs );
    }
    
    // Read properties
    pCCNodeLoader->parseProperties ( pNode, pParent, this );
    
	KDbool  bIsCCBFileNode = ( dynamic_cast<CCBFile*> ( pNode ) ) ? KD_TRUE : KD_FALSE;
    // Handle sub ccb files (remove middle node)
    if ( bIsCCBFileNode )
    {
        CCBFile*  pCCBFileNode  = (CCBFile*) pNode;        
        CCNode*   pEmbeddedNode = pCCBFileNode->getCCBFileNode ( );
        pEmbeddedNode->setPosition ( pCCBFileNode->getPosition ( ) );
        pEmbeddedNode->setRotation ( pCCBFileNode->getRotation ( ) );
		pEmbeddedNode->setScaleX ( pCCBFileNode->getScaleX ( ) );
        pEmbeddedNode->setScaleY ( pCCBFileNode->getScaleY ( ) );
        pEmbeddedNode->setTag ( pCCBFileNode->getTag ( ) );
        pEmbeddedNode->setVisible ( KD_TRUE );
        pEmbeddedNode->ignoreAnchorPointForPosition ( pCCBFileNode->isIgnoreAnchorPointForPosition ( ) );
        
		m_pActionManager->moveAnimationsFromNode ( pCCBFileNode, pEmbeddedNode );

        pCCBFileNode->setCCBFileNode ( KD_NULL );
        
        pNode = pEmbeddedNode;
    }

    if ( nMemberVarAssignmentType != kCCBTargetTypeNone )
	{
		if ( !m_bJSControlled ) 
		{
			CCObject*  pTarget = KD_NULL;
			if ( nMemberVarAssignmentType == kCCBTargetTypeDocumentRoot ) 
			{
				pTarget = m_pActionManager->getRootNode ( );
			} 
			else if ( nMemberVarAssignmentType == kCCBTargetTypeOwner ) 
			{
				pTarget = this->m_pOwner;
			}

			if ( pTarget != KD_NULL )
			{
				KDbool  bAssigned = KD_FALSE;

				CCBMemberVariableAssigner*  pTargetAsCCBMemberVariableAssigner = dynamic_cast<CCBMemberVariableAssigner*> ( pTarget );

				if ( nMemberVarAssignmentType != kCCBTargetTypeNone )
				{
					if ( pTargetAsCCBMemberVariableAssigner != KD_NULL )
					{
						bAssigned = pTargetAsCCBMemberVariableAssigner->onAssignCCBMemberVariable ( pTarget, sMemberVarAssignmentName.c_str ( ), pNode );
					}

					if ( !bAssigned && this->m_pCCBMemberVariableAssigner != KD_NULL )
					{
						bAssigned = this->m_pCCBMemberVariableAssigner->onAssignCCBMemberVariable ( pTarget, sMemberVarAssignmentName.c_str ( ), pNode );
					}
				}
			}
			else
			{
				if ( nMemberVarAssignmentType == kCCBTargetTypeDocumentRoot )
				{
					m_pActionManager->addDocumentOutletName ( sMemberVarAssignmentName );
					m_pActionManager->addDocumentOutletNode ( pNode );
				}
				else
				{
					m_aOwnerOutletNames.push_back ( sMemberVarAssignmentName );
					m_pOwnerOutletNodes->addObject ( pNode );
				}
			}
		}
    }
    
    // Assign custom properties.
    if ( pCCNodeLoader->getCustomProperties ( )->count ( ) > 0 )
	{            
        KDbool  bCustomAssigned = KD_FALSE;
        
        if ( !m_bJSControlled )
        {
            CCObject*  pTarget = pNode;
            if ( pTarget != KD_NULL )
            {
                CCBMemberVariableAssigner*  pTargetAsCCBMemberVariableAssigner = dynamic_cast<CCBMemberVariableAssigner*> ( pTarget );
                if ( pTargetAsCCBMemberVariableAssigner != KD_NULL )
				{                    
                    CCDictionary*   pCustomPropeties = pCCNodeLoader->getCustomProperties ( );
                    CCDictElement*  pElement;
                    CCDICT_FOREACH ( pCustomPropeties, pElement )
                    {
                        bCustomAssigned = pTargetAsCCBMemberVariableAssigner->onAssignCCBCustomProperty ( pTarget, pElement->getStrKey ( ), (CCBValue*) pElement->getObject ( ) );

                        if ( !bCustomAssigned && this->m_pCCBMemberVariableAssigner != KD_NULL )
                        {
                            bCustomAssigned = this->m_pCCBMemberVariableAssigner->onAssignCCBCustomProperty ( pTarget, pElement->getStrKey ( ), (CCBValue*) pElement->getObject ( ) );
                        }
                    }
                }
            }
        }
    }

	CC_SAFE_DELETE ( m_pAnimatedProps );

    // Read and add children. 
    KDint  nNumChildren = this->readInt ( KD_FALSE );
    for ( KDint i = 0; i < nNumChildren; i++ ) 
	{
        CCNode*  pChild = this->readNodeGraph ( pNode );
        pNode->addChild ( pChild );
    }

    // FIX ISSUE #1860: "onNodeLoaded will be called twice if ccb was added as a CCBFile".
    // If it's a sub-ccb node, skip notification to CCNodeLoaderListener since it will be
    // notified at LINE #734: CCNode * child = this->readNodeGraph(node);
    if ( !bIsCCBFileNode ) 
	{
        // Call onNodeLoaded
        CCNodeLoaderListener*  pNodeAsCCNodeLoaderListener = dynamic_cast<CCNodeLoaderListener *> ( pNode );
        if ( pNodeAsCCNodeLoaderListener != KD_NULL )
		{
            pNodeAsCCNodeLoaderListener->onNodeLoaded ( pNode, pCCNodeLoader );
        }
		else if ( this->m_pCCNodeLoaderListener != KD_NULL )
		{
            this->m_pCCNodeLoaderListener->onNodeLoaded ( pNode, pCCNodeLoader );
        }
    }

    return pNode;
}

CCBKeyframe* CCBReader::readKeyframe ( KDint nType )
{
    CCBKeyframe*  pKeyframe = new CCBKeyframe ( );
    pKeyframe->autorelease ( );
    
    pKeyframe->setTime ( readFloat ( ) );
    
    KDint      nEasingType = readInt ( KD_FALSE );
    KDfloat    fEasingOpt  = 0;
    CCObject*  pValue      = KD_NULL;
    
    if ( nEasingType == kCCBKeyframeEasingCubicIn
      || nEasingType == kCCBKeyframeEasingCubicOut
      || nEasingType == kCCBKeyframeEasingCubicInOut
      || nEasingType == kCCBKeyframeEasingElasticIn
      || nEasingType == kCCBKeyframeEasingElasticOut
      || nEasingType == kCCBKeyframeEasingElasticInOut )
    {
        fEasingOpt = readFloat ( );
    }
    pKeyframe->setEasingType ( nEasingType );
    pKeyframe->setEasingOpt  ( fEasingOpt  );
    
	switch ( nType )
	{
		case kCCBPropTypeCheck :
			pValue = CCBValue::create ( readBool ( ) );
			break;

		case kCCBPropTypeByte :
			pValue = CCBValue::create ( readByte ( ) );
			break;

		case kCCBPropTypeColor3 :
			pValue = ccColor3BWapper::create ( ccc3 ( readByte ( ), readByte ( ), readByte ( ) ) );
			break;

		case kCCBPropTypeDegrees :
			pValue = CCBValue::create ( readFloat ( ) );
			break;

		case kCCBPropTypeScaleLock :
		case kCCBPropTypePosition :
		case kCCBPropTypeFloatXY :
			pValue = CCArray::create ( CCBValue::create ( readFloat ( ) ), CCBValue::create ( readFloat ( ) ), KD_NULL );
			break;

		case kCCBPropTypeSpriteFrame :
		{
			std::string   sSpriteSheet = readCachedString ( );
			std::string   sSpriteFile  = readCachedString ( );
	        
			CCSpriteFrame*  pSpriteFrame;

			if ( sSpriteSheet.length ( ) == 0 )
			{
				sSpriteFile = m_sCCBRootPath + sSpriteFile;

				CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( sSpriteFile.c_str ( ) );
				CCRect  tBounds ( CCPointZero, pTexture->getContentSize ( ) );
				pSpriteFrame = CCSpriteFrame::createWithTexture ( pTexture, tBounds );
			}
			else
			{
				sSpriteSheet = m_sCCBRootPath + sSpriteSheet;

				CCSpriteFrameCache*  pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	            
				// Load the sprite sheet only if it is not loaded            
				if ( m_aLoadedSpriteSheets.find ( sSpriteSheet ) == m_aLoadedSpriteSheets.end ( ) )
				{
					pFrameCache->addSpriteFramesWithFile ( sSpriteSheet.c_str ( ) );
					m_aLoadedSpriteSheets.insert ( sSpriteSheet );
				}
	            
				pSpriteFrame = pFrameCache->spriteFrameByName ( sSpriteFile.c_str ( ) );
			}

			pValue = pSpriteFrame;
				
		}	break;

		default : 

			;			
	}

    pKeyframe->setValue ( pValue );
    
    return pKeyframe;
}

KDbool CCBReader::readCallbackKeyframesForSeq ( CCBSequence* pSeq ) 
{
    KDint  nNumKeyframes = readInt ( KD_FALSE );
    if ( !nNumKeyframes ) 
	{
		return KD_TRUE;
	}
    
    CCBSequenceProperty*  pChannel = new CCBSequenceProperty ( );
    pChannel->autorelease ( );

    for ( KDint i = 0; i < nNumKeyframes; ++i )
	{      
        KDfloat  fTime = readFloat ( );
        std::string  sCallbackName = readCachedString ( );
      
        KDint  nCallbackType = readInt ( KD_FALSE );
      
        CCArray*  pValue = CCArray::create ( );
        pValue->addObject ( CCString::create ( sCallbackName ) );
        pValue->addObject ( CCString::createWithFormat ( "%d", nCallbackType ) );
        
        CCBKeyframe*  pKeyframe = new CCBKeyframe ( );
        pKeyframe->autorelease ( );
        
        pKeyframe->setTime  ( fTime );
        pKeyframe->setValue ( pValue );
        
        if ( m_bJSControlled )
		{
            m_pActionManager->getKeyframeCallbacks ( )->addObject ( CCString::createWithFormat ( "%d:%s", nCallbackType, sCallbackName.c_str ( ) ) );
        }
    
        pChannel->getKeyframes ( )->addObject ( pKeyframe );
    }
    
    pSeq->setCallbackChannel ( pChannel );
    
    return KD_TRUE;
}

KDbool CCBReader::readSoundKeyframesForSeq ( CCBSequence* pSeq )
{
    KDint numKeyframes = readInt ( KD_FALSE );
    if ( !numKeyframes ) 
	{
		return KD_TRUE;
	}
    
    CCBSequenceProperty*  channel = new CCBSequenceProperty ( );
	channel->autorelease ( );
    
    for ( KDint i = 0; i < numKeyframes; ++i )
	{        
        KDfloat   fTime  = readFloat ( );
        std::string  sSoundFile = readCachedString ( );
        KDfloat   fPitch = readFloat ( );
        KDfloat   fPan   = readFloat ( );
        KDfloat   fGain  = readFloat ( );
                
        CCArray*  pValue = CCArray::create ( );
        
        pValue->addObject ( CCString::create ( sSoundFile ) );
        pValue->addObject ( CCString::createWithFormat ( "%f", fPitch ) );
        pValue->addObject ( CCString::createWithFormat ( "%f", fPan   ) );
        pValue->addObject ( CCString::createWithFormat ( "%f", fGain  ) );
        
        CCBKeyframe*  pKeyframe = new CCBKeyframe ( );
        pKeyframe->setTime ( fTime );
        pKeyframe->setValue ( pValue );
        
        channel->getKeyframes ( )->addObject ( pKeyframe );
		pKeyframe->release ( );
    }
    
    pSeq->setSoundChannel ( channel );
    
    return KD_TRUE;
}

CCNode* CCBReader::readNodeGraph ( KDvoid )
{
    return this->readNodeGraph ( KD_NULL );
}

KDbool CCBReader::readSequences ( KDvoid )
{
    CCArray*  pSequences = m_pActionManager->getSequences ( );
    
    KDint  nNumSeqs = readInt ( KD_FALSE );
    
    for ( KDint i = 0; i < nNumSeqs; i++ )
    {
        CCBSequence*  pSeq = new CCBSequence ( );
        pSeq->autorelease ( );
        
        pSeq->setDuration ( readFloat ( ) );
		pSeq->setName ( readCachedString ( ).c_str ( ) );
        pSeq->setSequenceId ( readInt ( KD_FALSE ) );
        pSeq->setChainedSequenceId ( readInt ( KD_TRUE ) );
        
        if ( !readCallbackKeyframesForSeq ( pSeq ) )
		{
			return KD_FALSE;
		}

        if ( !readSoundKeyframesForSeq ( pSeq ) )
		{
			return KD_FALSE;
		}
   
        pSequences->addObject ( pSeq );
    }
    
    m_pActionManager->setAutoPlaySequenceId ( readInt ( KD_TRUE ) );

    return KD_TRUE;
}

std::string CCBReader::lastPathComponent ( const KDchar* pPath )
{
    std::string  sPath ( pPath );
    KDsize  uSlashPos = sPath.find_last_of ( "/" );
    if ( uSlashPos != std::string::npos )
	{
        return sPath.substr ( uSlashPos + 1, sPath.length ( ) - uSlashPos );
    }
    return sPath;
}

std::string CCBReader::deletePathExtension ( const KDchar* pPath )
{
    std::string  sPath ( pPath );
    KDsize  uDotPos = sPath.find_last_of ( "." );
    if ( uDotPos != std::string::npos ) 
	{
        return sPath.substr ( 0, uDotPos );
    }

    return sPath;
}

std::string CCBReader::toLowerCase ( const KDchar* pString )
{
    std::string  sCopy ( pString );
    std::transform ( sCopy.begin ( ), sCopy.end ( ), sCopy.begin ( ), tolower );
    return sCopy;
}

KDbool CCBReader::endsWith ( const KDchar* pString, const KDchar* pEnding )
{
    std::string  sString ( pString );
    std::string  sEnding ( pEnding );

    if ( sString.length ( ) >= sEnding.length ( ) )
	{
        return ( sString.compare ( sString.length ( ) - sEnding.length ( ), sEnding.length ( ), sEnding ) == 0 );
    } 
	else 
	{
        return KD_FALSE;
    }
}

KDbool CCBReader::isJSControlled ( KDvoid )
{
    return m_bJSControlled;
}

KDvoid CCBReader::addOwnerCallbackName ( std::string sName )
{
    m_aOwnerCallbackNames.push_back ( sName );
}

KDvoid CCBReader::addOwnerCallbackNode ( CCNode* pNode )
{
    m_pOwnerCallbackNodes->addObject ( pNode );
}

KDvoid CCBReader::addDocumentCallbackName ( std::string sName )
{
    m_pActionManager->addDocumentCallbackName ( sName );
}

KDvoid CCBReader::addDocumentCallbackNode ( CCNode* pNode )
{
    m_pActionManager->addDocumentCallbackNode ( pNode );
}

CCArray* CCBReader::getOwnerCallbackNames ( KDvoid )
{
    CCArray*  pRet = CCArray::createWithCapacity ( m_aOwnerCallbackNames.size ( ) );
    std::vector<std::string>::iterator it = m_aOwnerCallbackNames.begin ( );
    for ( ; it != m_aOwnerCallbackNames.end ( ); ++it )
    {
        pRet->addObject ( CCString::create ( *it ) );
    }
    
    return pRet;
}

CCArray* CCBReader::getOwnerCallbackNodes ( KDvoid )
{
    return m_pOwnerCallbackNodes;
}

CCArray* CCBReader::getOwnerOutletNames ( KDvoid )
{
    CCArray*  pRet = CCArray::createWithCapacity ( m_aOwnerOutletNames.size ( ) );
    std::vector<std::string>::iterator  it = m_aOwnerOutletNames.begin ( );
    for ( ; it != m_aOwnerOutletNames.end ( ); ++it )
    {
        pRet->addObject ( CCString::create ( *it ) );
    }

    return pRet;
}

CCArray* CCBReader::getOwnerOutletNodes ( KDvoid )
{
    return m_pOwnerOutletNodes;
}

CCArray* CCBReader::getNodesWithAnimationManagers ( KDvoid )
{
    return m_pNodesWithAnimationManagers;
}

CCArray* CCBReader::getAnimationManagersForNodes ( KDvoid )
{
    return m_pAnimationManagersForNodes;
}

//
// Static functions
//

static KDfloat  l_fCCBResolutionScale = 1.0f;

KDfloat CCBReader::getResolutionScale ( KDvoid )
{	
	return 1;
}

KDvoid CCBReader::setResolutionScale ( KDfloat fScale )
{
    l_fCCBResolutionScale = fScale;
}

NS_CC_END
