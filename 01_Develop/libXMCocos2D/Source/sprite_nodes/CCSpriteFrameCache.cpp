/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteFrameCache.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2009      Jason Booth
 *      Copyright (c) 2009      Robert J Payne
 *      Copyright (c) 2011      Zynga Inc.
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "sprite_nodes/CCSprite.h"
#include "cocoa/CCNS.h"
#include "cocoa/CCString.h"
#include "cocoa/CCArray.h"
#include "textures/CCTextureCache.h"
#include "support/TransformUtils.h"
#include "support/CCFileUtils.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN
    
static CCSpriteFrameCache*  l_pSharedSpriteFrameCache = KD_NULL;

CCSpriteFrameCache* CCSpriteFrameCache::sharedSpriteFrameCache ( KDvoid )
{
	if ( !l_pSharedSpriteFrameCache )
	{
		l_pSharedSpriteFrameCache = new CCSpriteFrameCache ( );
		l_pSharedSpriteFrameCache->init ( );
	}

	return l_pSharedSpriteFrameCache;
}

KDvoid CCSpriteFrameCache::purgeSharedSpriteFrameCache ( KDvoid )
{
	CC_SAFE_RELEASE ( l_pSharedSpriteFrameCache );
}

KDbool CCSpriteFrameCache::init ( KDvoid )
{
	m_pSpriteFrames			= new CCDictionary ( );
	m_pSpriteFramesAliases	= new CCDictionary ( );
	m_pLoadedFileNames		= new std::set<std::string> ( );
	return KD_TRUE;
}

CCSpriteFrameCache::CCSpriteFrameCache ( KDvoid )
{
	m_pSpriteFrames			= KD_NULL;
	m_pSpriteFramesAliases	= KD_NULL;
	m_pLoadedFileNames		= KD_NULL;
}

CCSpriteFrameCache::~CCSpriteFrameCache ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCSpriteFrameCache = %p", this );

	CC_SAFE_RELEASE ( m_pSpriteFrames );
	CC_SAFE_RELEASE ( m_pSpriteFramesAliases );
	CC_SAFE_DELETE  ( m_pLoadedFileNames );
}

KDvoid CCSpriteFrameCache::addSpriteFramesWithDictionary ( CCDictionary* pDictionary, CCTexture2D* pTexture )
{
	/*
	Supported Zwoptex Formats :

	ZWTCoordinatesFormatOptionXMLLegacy = 0, // Flash Version
	ZWTCoordinatesFormatOptionXML1_0 = 1, // Desktop Version 0.0 - 0.4b
	ZWTCoordinatesFormatOptionXML1_1 = 2, // Desktop Version 1.0.0 - 1.0.1
	ZWTCoordinatesFormatOptionXML1_2 = 3, // Desktop Version 1.0.2+
	*/

	CCDictionary*  pMetadataDict = (CCDictionary*) pDictionary->objectForKey ( "metadata" );
	CCDictionary*    pFramesDict = (CCDictionary*) pDictionary->objectForKey ( "frames" );
	KDint  nFormat = 0;

	// get the format
	if ( pMetadataDict != KD_NULL ) 
	{
		nFormat = pMetadataDict->valueForKey ( "format" )->intValue ( );
	}

	// check the format
	CCAssert ( nFormat >=0 && nFormat <= 3, "format is not supported for CCSpriteFrameCache addSpriteFramesWithDictionary:textureFilename:" );

	CCDictElement*  pElement = KD_NULL;
    CCDICT_FOREACH ( pFramesDict, pElement )
	{
		CCDictionary*     pFrameDict = (CCDictionary*) pElement->getObject ( );
		std::string sSpriteFrameName = pElement->getStrKey ( );
		CCSpriteFrame*  pSpriteFrame = (CCSpriteFrame*) m_pSpriteFrames->objectForKey ( sSpriteFrameName );
		if ( pSpriteFrame )
		{
			continue;
		}
		
		if ( nFormat == 0 ) 
		{
			KDfloat   x = pFrameDict->valueForKey ( "x" )->floatValue ( );
			KDfloat   y = pFrameDict->valueForKey ( "y" )->floatValue ( );
			KDfloat   w = pFrameDict->valueForKey ( "width"  )->floatValue ( );
			KDfloat   h = pFrameDict->valueForKey ( "height" )->floatValue ( );
			KDfloat  ox = pFrameDict->valueForKey ( "offsetX" )->floatValue ( );
			KDfloat  oy = pFrameDict->valueForKey ( "offsetY" )->floatValue ( );
			KDint    ow = pFrameDict->valueForKey ( "originalWidth"  )->intValue ( );
			KDint    oh = pFrameDict->valueForKey ( "originalHeight" )->intValue ( );
            
			// check ow/oh
			if ( !ow || !oh )
			{
				CCLOG ( "XMCocos2D : WARNING - originalWidth/Height not found on the CCSpriteFrame. AnchorPoint won't work as expected. Regenrate the .plist");
			}
            
			// abs ow/oh
			ow = kdAbs ( ow );
			oh = kdAbs ( oh );
            
			// create frame
			pSpriteFrame = new CCSpriteFrame ( );
			pSpriteFrame->initWithTexture ( pTexture, CCRect ( x, y, w, h ), KD_FALSE, ccp ( ox, oy ), CCSizeMake( ow, oh ) );
		} 
		else if ( nFormat == 1 || nFormat == 2 ) 
		{
			CCRect   tFrame = CCRectFromString ( pFrameDict->valueForKey ( "frame" )->getCString ( ) );
			KDbool bRotated = KD_FALSE;

			// rotation
			if ( nFormat == 2 )
			{
				bRotated = pFrameDict->valueForKey ( "rotated" )->boolValue ( );
			}

			CCPoint     tOffset = CCPointFromString ( pFrameDict->valueForKey ( "offset"     )->getCString ( ) );
			CCSize  tSourceSize = CCSizeFromString  ( pFrameDict->valueForKey ( "sourceSize" )->getCString ( ) );

			// create frame
			pSpriteFrame = new CCSpriteFrame ( );
			pSpriteFrame->initWithTexture ( pTexture, tFrame, bRotated, tOffset, tSourceSize );
		} 
		else if ( nFormat == 3 )
		{
			// get values
			CCSize   tSpriteSize       = CCSizeFromString  ( pFrameDict->valueForKey ( "spriteSize"      )->getCString ( ) );
			CCPoint  tSpriteOffset     = CCPointFromString ( pFrameDict->valueForKey ( "spriteOffset"    )->getCString ( ) );
			CCSize   tSpriteSourceSize = CCSizeFromString  ( pFrameDict->valueForKey ( "spriteSourceSize")->getCString ( ) );
			CCRect   tTextureRect      = CCRectFromString  ( pFrameDict->valueForKey ( "textureRect"     )->getCString ( ) );
            KDbool   bTextureRotated   = pFrameDict->valueForKey ( "textureRotated" )->boolValue ( );

			// get aliases
			CCArray*   pAliases  = (CCArray*) ( pFrameDict->objectForKey ( "aliases" ) );
            CCString*  pFrameKey = new CCString ( sSpriteFrameName );

            CCObject*  pObject = KD_NULL;
            CCARRAY_FOREACH ( pAliases, pObject )
            {
                std::string  sOneAlias = ( (CCString*) pObject )->getCString ( );
                if ( m_pSpriteFramesAliases->objectForKey ( sOneAlias.c_str ( ) ) )
                {
                    CCLOG ( "XMCocos2d : WARNING - an alias with name %s already exists", sOneAlias.c_str ( ) );
                }

                m_pSpriteFramesAliases->setObject ( pFrameKey, sOneAlias.c_str ( ) );
            }
            pFrameKey->release ( );
            
            // create frame
            pSpriteFrame = new CCSpriteFrame ( );
            pSpriteFrame->initWithTexture ( pTexture, CCRect ( tTextureRect.origin, tSpriteSize ), bTextureRotated, tSpriteOffset, tSpriteSourceSize );
		}

		// add sprite frame
		m_pSpriteFrames->setObject ( pSpriteFrame, sSpriteFrameName );
		pSpriteFrame->release ( );
	}
}

KDvoid CCSpriteFrameCache::addSpriteFramesWithFile ( const KDchar* szPlist, CCTexture2D* pTexture )
{
	std::string    sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szPlist );
	CCDictionary*  pDict = CCDictionary::createWithContentsOfFileThreadSafe ( sPath.c_str ( ) );	

	addSpriteFramesWithDictionary ( pDict, pTexture );
	pDict->release ( );
}

KDvoid CCSpriteFrameCache::addSpriteFramesWithFile ( const KDchar* szPlist, const KDchar* szTextureFileName )
{
	CCAssert ( szPlist, "texture name should not be null" );
    
	CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szTextureFileName );
    
	if ( pTexture )
	{
		addSpriteFramesWithFile ( szPlist, pTexture );
	}
	else
	{
		CCLOGINFO ( "XMCocos2D : CCSpriteFrameCache - couldn't load texture file. File not found %s", szTextureFileName );
	}
}

KDvoid CCSpriteFrameCache::addSpriteFramesWithFile ( const KDchar* szPlist )
{
    CCAssert ( szPlist, "plist filename should not be NULL" );

    if ( m_pLoadedFileNames->find ( szPlist ) == m_pLoadedFileNames->end ( ) )
    {
        std::string    sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szPlist );
        CCDictionary*  pDict = CCDictionary::createWithContentsOfFileThreadSafe ( sPath.c_str ( ) );

		std::string    sTexturePath ( "" );
        CCDictionary*  pMetadataDict = (CCDictionary*) pDict->objectForKey ( "metadata" );
        if ( pMetadataDict )
        {
            // try to read  texture file name from meta data
            sTexturePath = pMetadataDict->valueForKey ( "textureFileName" )->getCString ( );
        }

        if ( !sTexturePath.empty ( ) )
        {
            // build texture path relative to plist file
            sTexturePath = CCFileUtils::sharedFileUtils ( )->fullPathFromRelativeFile ( sTexturePath.c_str ( ), szPlist );
        }
        else
        {
            // build texture path by replacing file extension
            sTexturePath = szPlist;

            // remove .xxx
            KDsize  uStartPos = sTexturePath.find_last_of ( "." ); 
            sTexturePath = sTexturePath.erase ( uStartPos );

            // append .png
            sTexturePath = sTexturePath.append ( ".png" );

            CCLOG ( "XMCocos2D : CCSpriteFrameCache: Trying to use file %s as texture", sTexturePath.c_str ( ) );
        }

        CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( sTexturePath.c_str ( ) );

        if ( pTexture )
        {
            addSpriteFramesWithDictionary ( pDict, pTexture );
            m_pLoadedFileNames->insert ( szPlist );
        }
        else
        {
            CCLOG ( "XMCocos2D : CCSpriteFrameCache: Couldn't load texture" );
        }

        pDict->release ( );
    }
}

KDvoid CCSpriteFrameCache::addSpriteFrame ( CCSpriteFrame* pFrame, const KDchar* szFrameName )
{
	m_pSpriteFrames->setObject ( pFrame, szFrameName );
}

KDvoid CCSpriteFrameCache::removeSpriteFrames ( KDvoid )
{
	m_pSpriteFrames->removeAllObjects ( );
	m_pSpriteFramesAliases->removeAllObjects ( );
	m_pLoadedFileNames->clear ( );
}

KDvoid CCSpriteFrameCache::removeUnusedSpriteFrames ( KDvoid )
{
	KDbool  bRemoved = KD_FALSE;
	CCDictElement*  pElement = KD_NULL;
	CCDICT_FOREACH ( m_pSpriteFrames, pElement )
	{
		CCSpriteFrame*  pSpriteFrame = (CCSpriteFrame*) pElement->getObject ( );
		if ( pSpriteFrame->retainCount ( ) == 1 ) 
		{
			CCLOGINFO ( "XMCocos2D : CCSpriteFrameCache: removing unused frame: %s", pElement->getStrKey ( ) );
			m_pSpriteFrames->removeObjectForElememt ( pElement );
			bRemoved = KD_TRUE;
		}
	}

	// XXX. Since we don't know the .plist file that originated the frame, we must remove all .plist from the cache
    if ( bRemoved )
    {
        m_pLoadedFileNames->clear ( );
    }
}

KDvoid CCSpriteFrameCache::removeSpriteFrameByName ( const KDchar* szName )
{
	// explicit nil handling
	if ( !szName )
	{
		return;
	}

	// Is this an alias ?
	CCString*  pKey = (CCString*) m_pSpriteFramesAliases->objectForKey ( szName );

	if ( pKey )
	{
        m_pSpriteFrames->removeObjectForKey ( pKey->getCString ( ) );
		m_pSpriteFramesAliases->removeObjectForKey ( pKey->getCString ( ) );
	}
	else
	{
        m_pSpriteFrames->removeObjectForKey ( szName );
	}

	// XXX. Since we don't know the .plist file that originated the frame, we must remove all .plist from the cache
    m_pLoadedFileNames->clear ( );
}

KDvoid CCSpriteFrameCache::removeSpriteFramesFromFile ( const KDchar* szPlist )
{
	std::string    sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szPlist );
	CCDictionary*  pDict = CCDictionary::createWithContentsOfFileThreadSafe ( sPath.c_str ( ) );

	removeSpriteFramesFromDictionary ( (CCDictionary*) pDict );

	// remove it from the cache
	std::set<std::string>::iterator  ret = m_pLoadedFileNames->find ( szPlist );
    if ( ret != m_pLoadedFileNames->end ( ) )
    {
        m_pLoadedFileNames->erase ( ret );
    }

	pDict->release ( );
}

KDvoid CCSpriteFrameCache::removeSpriteFramesFromDictionary ( CCDictionary* pDictionary )
{
	CCDictionary*  pFramesDict = (CCDictionary*) pDictionary->objectForKey ( "frames" );
	CCArray*	   pKeysToRemove = CCArray::create ( );

	CCDictElement*  pElement = KD_NULL;
    CCDICT_FOREACH ( pFramesDict, pElement )
	{
		if ( m_pSpriteFrames->objectForKey ( pElement->getStrKey ( ) ) )
		{
			pKeysToRemove->addObject ( CCString::create ( pElement->getStrKey ( ) ) );
		}
	}
    
	m_pSpriteFrames->removeObjectsForKeys ( pKeysToRemove );
}

KDvoid CCSpriteFrameCache::removeSpriteFramesFromTexture ( CCTexture2D* pTexture )
{
	CCArray*  pKeysToRemove = CCArray::create ( );

	CCDictElement*  pElement = KD_NULL;
    CCDICT_FOREACH ( m_pSpriteFrames, pElement )
    {
        CCSpriteFrame*  pFrame = (CCSpriteFrame*) m_pSpriteFrames->objectForKey ( pElement->getStrKey ( ) );
        if ( pFrame && ( pFrame->getTexture ( ) == pTexture ) )
        {
            pKeysToRemove->addObject ( CCString::create ( pElement->getStrKey ( ) ) );
        }
    }

    m_pSpriteFrames->removeObjectsForKeys ( pKeysToRemove );
}

CCSpriteFrame* CCSpriteFrameCache::spriteFrameByName ( const KDchar* szName )
{
	CCSpriteFrame*  pFrame = (CCSpriteFrame*) m_pSpriteFrames->objectForKey ( szName );

	if ( !pFrame )
	{
		// try alias dictionary
		CCString*  pKey = (CCString*) m_pSpriteFramesAliases->objectForKey ( szName );  

		if ( pKey )
		{
			pFrame = (CCSpriteFrame*) m_pSpriteFrames->objectForKey ( pKey->getCString ( ) );
			if ( !pFrame )
			{
				CCLOGINFO ( "XMCocos2D : CCSpriteFrameCache - Frame '%s' not found", szName );
			}
		}
	}

	return pFrame;
}

NS_CC_END