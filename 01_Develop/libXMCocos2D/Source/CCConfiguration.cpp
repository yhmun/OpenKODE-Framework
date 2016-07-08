/* -----------------------------------------------------------------------------------
 *
 *      File            CCConfiguration.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Ricardo Quesada
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
#include "support/CCConfiguration.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCInteger.h"
#include "cocoa/CCBool.h"
#include "cocoa/CCDouble.h"
#include "support/CCFileUtils.h"
#include "support/CCDirector.h"
#include "ccConfig.h"
#include "CCApplication.h"

NS_CC_BEGIN
    
static CCConfiguration*  l_pSharedConfiguration = KD_NULL;

CCConfiguration::CCConfiguration ( KDvoid )
{
    m_nMaxTextureSize               = 0;
    m_nMaxModelviewStackDepth       = 0;
    m_bSupportsPVRTC                = KD_FALSE;
    m_bSupportsNPOT                 = KD_FALSE;
    m_bSupportsBGRA8888             = KD_FALSE;
    m_bSupportsDiscardFramebuffer   = KD_FALSE;
	m_bSupportsShareableVAO			= KD_FALSE;
    m_nMaxSamplesAllowed            = 0;
	m_nMaxTextureUnits				= 0;
    m_pGlExtensions                 = KD_NULL;
	m_pValueDict					= KD_NULL;
}

CCConfiguration::~CCConfiguration ( KDvoid )
{
	m_pValueDict->release ( );
}

KDbool CCConfiguration::init ( KDvoid )
{
	m_pValueDict = CCDictionary::create ( );
	m_pValueDict->retain ( );

	m_pValueDict->setObject ( CCString::create ( CCApplication::sharedApplication ( )->getVersion ( ) ), "cocos2d.x.version" );

#if CC_ENABLE_PROFILERS
	m_pValueDict->setObject ( CCBool::create ( KD_TRUE ), "cocos2d.x.compiled_with_profiler" );
#else
	m_pValueDict->setObject ( CCBool::create ( KD_FALSE ), "cocos2d.x.compiled_with_profiler" );
#endif

#if CC_ENABLE_GL_STATE_CACHE == 0
	m_pValueDict->setObject ( CCBool::create ( KD_FALSE ), "cocos2d.x.compiled_with_gl_state_cache" );
#else
	m_pValueDict->setObject ( CCBool::create ( KD_TRUE ), "cocos2d.x.compiled_with_gl_state_cache" );
#endif

	this->gatherGPUInfo ( );
	this->dumpInfo ( );

	return KD_TRUE;
}

KDvoid CCConfiguration::dumpInfo ( KDvoid ) const
{
	// Dump
	CCPrettyPrinter		tVisitor ( 0 );
	m_pValueDict->acceptVisitor ( tVisitor );

	CCLOG ( "%s", tVisitor.getResult ( ).c_str ( ) );

	// And Dump some warnings as well
#if CC_ENABLE_PROFILERS
	CCLOG ( "cocos2d: **** WARNING **** CC_ENABLE_PROFILERS is defined. Disable it when you finish profiling (from ccConfig.h)" );
#endif

#if CC_ENABLE_GL_STATE_CACHE == 0
	CCLOG ( "" );
	CCLOG ( "cocos2d: **** WARNING **** CC_ENABLE_GL_STATE_CACHE is disabled. To improve performance, enable it (from ccConfig.h)" );
#endif
}

KDvoid CCConfiguration::gatherGPUInfo ( KDvoid )
{
	m_pValueDict->setObject ( CCString::create ( (const KDchar*) glGetString ( GL_VENDOR   ) ), "gl.vendor" );
	m_pValueDict->setObject ( CCString::create ( (const KDchar*) glGetString ( GL_RENDERER ) ), "gl.renderer" );
	m_pValueDict->setObject ( CCString::create ( (const KDchar*) glGetString ( GL_VERSION  ) ), "gl.version" );

	m_pGlExtensions = (KDchar*) glGetString ( GL_EXTENSIONS );

	glGetIntegerv ( GL_MAX_TEXTURE_SIZE, &m_nMaxTextureSize );
	m_pValueDict->setObject ( CCInteger::create ( (KDint) m_nMaxTextureSize ), "gl.max_texture_size" );

#if defined ( USE_OPEN_GLES2 )
	glGetIntegerv ( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_nMaxTextureUnits );
	m_pValueDict->setObject ( CCInteger::create ( (KDint) m_nMaxTextureUnits ), "gl.max_texture_units" );
#endif

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS ) && defined ( USE_OPEN_GLES2 )
	glGetIntegerv ( GL_MAX_SAMPLES_APPLE, &m_nMaxSamplesAllowed );
	m_pValueDict->setObject ( CCInteger::create ( (KDint) m_nMaxSamplesAllowed ), "gl.max_samples_allowed" );
#endif

	m_bSupportsPVRTC = checkForGLExtension ( "GL_IMG_texture_compression_pvrtc" );
	m_pValueDict->setObject ( CCBool::create ( m_bSupportsPVRTC ), "gl.supports_PVRTC" );

#if defined ( USE_OPEN_GLES2 )
	m_bSupportsNPOT = KD_TRUE;
#else
	glGetIntegerv ( GL_MAX_MODELVIEW_STACK_DEPTH, &m_nMaxModelviewStackDepth );
	m_bSupportsNPOT = checkForGLExtension ( "GL_APPLE_texture_2D_limited_npot" );
#endif
	m_pValueDict->setObject ( CCBool::create ( m_bSupportsNPOT ), "gl.supports_NPOT" );

	m_bSupportsBGRA8888 = checkForGLExtension ( "GL_IMG_texture_format_BGRA888" );
	m_pValueDict->setObject ( CCBool::create ( m_bSupportsBGRA8888 ), "gl.supports_BGRA8888" );

	m_bSupportsDiscardFramebuffer = checkForGLExtension ( "GL_EXT_discard_framebuffer" );
	m_pValueDict->setObject ( CCBool::create ( m_bSupportsDiscardFramebuffer ), "gl.supports_discard_framebuffer" );

	m_bSupportsShareableVAO = checkForGLExtension ( "vertex_array_object" );
	m_pValueDict->setObject ( CCBool::create ( m_bSupportsShareableVAO ), "gl.supports_vertex_array_object" );

	CHECK_GL_ERROR_DEBUG ( );
}

CCConfiguration* CCConfiguration::sharedConfiguration ( KDvoid )
{
	if ( !l_pSharedConfiguration )
	{
		l_pSharedConfiguration = new CCConfiguration ( );
		l_pSharedConfiguration->init ( );
	}

	return l_pSharedConfiguration;
}

KDvoid CCConfiguration::purgeConfiguration ( KDvoid )
{
	CC_SAFE_RELEASE ( l_pSharedConfiguration );
}

KDbool CCConfiguration::checkForGLExtension ( const KDchar* szSearchName ) const
{
	return ( m_pGlExtensions && kdStrstr ( m_pGlExtensions, szSearchName ) );
}

//
// getters for specific variables.
// Mantained for backward compatiblity reasons only.
//
KDint CCConfiguration::getMaxTextureSize ( KDvoid ) const
{
	return m_nMaxTextureSize;
}

KDint CCConfiguration::getMaxModelviewStackDepth ( KDvoid ) const
{
	return m_nMaxModelviewStackDepth;
}

KDint CCConfiguration::getMaxTextureUnits ( KDvoid ) const
{
	return m_nMaxTextureUnits;
}

KDbool CCConfiguration::supportsNPOT ( KDvoid ) const
{
	return m_bSupportsNPOT;
}

KDbool CCConfiguration::supportsPVRTC ( KDvoid ) const
{
	return m_bSupportsPVRTC;
}

KDbool CCConfiguration::supportsBGRA8888 ( KDvoid ) const
{
	return m_bSupportsBGRA8888;
}

KDbool CCConfiguration::supportsDiscardFramebuffer ( KDvoid ) const
{
	return m_bSupportsDiscardFramebuffer;
}

KDbool CCConfiguration::supportsShareableVAO ( KDvoid ) const
{
	return m_bSupportsShareableVAO;
}

//
// generic getters for properties
//
const KDchar* CCConfiguration::getCString ( const KDchar* szKey, const KDchar* szDefaultValue ) const
{
	CCObject*	pRet = m_pValueDict->objectForKey ( szKey );

	if ( pRet ) 
	{
		if ( CCString*	pStr = dynamic_cast<CCString*> ( pRet ) )
		{
			return pStr->getCString ( );
		}

		CCAssert ( KD_FALSE, "Key found, but from different type" );
	}

	// XXX: Should it throw an exception ?
	return szDefaultValue;
}

/** returns the value of a given key as a boolean */
KDbool CCConfiguration::getBool ( const KDchar* szKey, KDbool bDefaultValue ) const
{
	CCObject*	pRet = m_pValueDict->objectForKey ( szKey );

	if ( pRet ) 
	{
		if ( CCBool*	pBoolObj = dynamic_cast<CCBool*> ( pRet ) )
		{
			return pBoolObj->getValue ( );
		}

		if ( CCString*	pStrObj = dynamic_cast<CCString*> ( pRet ) )
		{
			return pStrObj->boolValue ( );
		}

		CCAssert ( KD_FALSE, "Key found, but from different type" );
	}

	// XXX: Should it throw an exception ?
	return bDefaultValue;
}

/** returns the value of a given key as a double */
KDdouble CCConfiguration::getNumber ( const KDchar* szKey, KDdouble dDefaultValue ) const
{
	CCObject*	pRet = m_pValueDict->objectForKey ( szKey );

	if ( pRet ) 
	{
		if ( CCDouble*	pObj = dynamic_cast<CCDouble*> ( pRet ) )
		{
			return pObj->getValue ( );
		}

		if ( CCInteger*	pObj = dynamic_cast<CCInteger*> ( pRet ) )
		{
			return pObj->getValue ( );
		}

		if ( CCString*	pStrObj = dynamic_cast<CCString*> ( pRet ) )
		{
			return pStrObj->doubleValue ( );
		}

		CCAssert ( KD_FALSE, "Key found, but from different type" );
	}

	// XXX: Should it throw an exception ?
	return dDefaultValue;
}

CCObject* CCConfiguration::getObject ( const KDchar* szKey ) const
{
	return m_pValueDict->objectForKey ( szKey );
}

KDvoid CCConfiguration::setObject ( const KDchar* szKey, CCObject* pValue )
{
	m_pValueDict->setObject ( pValue, szKey );
}

//
// load file
//
KDvoid CCConfiguration::loadConfigFile ( const KDchar* szFilename )
{
	CCDictionary*	pDict = CCDictionary::createWithContentsOfFile ( szFilename );
	CCAssert ( pDict, "cannot create dictionary" );

	// search for metadata
	KDbool		bMetaDataOk = false;
	CCObject*	pMetaData = pDict->objectForKey ( "metadata" );
	if ( pMetaData && dynamic_cast<CCDictionary*> ( pMetaData ) )
	{
		CCObject*	pFormatO = static_cast<CCDictionary*> ( pMetaData )->objectForKey ( "format" );

		// XXX: cocos2d-x returns CCStrings when importing from .plist. This bug will be addressed in cocos2d-x v3.x
		if ( pFormatO && dynamic_cast<CCString*> ( pFormatO ) ) 
		{
			KDint	nFormat = static_cast<CCString*> ( pFormatO )->intValue ( );

			// Support format: 1
			if ( nFormat == 1 )
			{
				bMetaDataOk = KD_TRUE;
			}
		}
	}

	if ( !bMetaDataOk ) 
	{
		CCLOG ( "Invalid config format for file: %s", szFilename );
		return;
	}

	CCObject*	pData = pDict->objectForKey ( "data" );
	if ( !pData || !dynamic_cast<CCDictionary*> ( pData ) ) 
	{
		CCLOG ( "Expected 'data' dict, but not found. Config file: %s", szFilename );
		return;
	}

	// Add all keys in the existing dictionary
	CCDictionary*	pDataDict = static_cast<CCDictionary*> ( pData );
	CCDictElement*	pElement;
	CCDICT_FOREACH ( pDataDict, pElement )
	{
		if ( !m_pValueDict->objectForKey ( pElement->getStrKey ( ) ) )
		{
			m_pValueDict->setObject ( pElement->getObject ( ), pElement->getStrKey ( ) );
		}
		else
		{
			CCLOG ( "Key already present. Ignoring '%s'", pElement->getStrKey ( ) );
		}
	}

	CCDirector::sharedDirector ( )->setDefaultValues ( );
}

NS_CC_END

