/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextureCache.cpp 
 *      Author          Young-Hwan Mun
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *      Copyright (c)           Microsoft Open Technologies, Inc.
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
#include "textures/CCTextureCache.h"
#include "support/CCDirector.h"
#include "support/CCImage.h"
#include "support/CCFileUtils.h"
#include "support/CCScheduler.h"
#include "support/ccUtils.h"

#include <list>
#include <queue>

NS_CC_BEGIN
    
typedef struct _AsyncStruct
{
    std::string			sFilename;
    CCObject*			pTarget;
    SEL_CallFuncO		pSelector;
} AsyncStruct;

typedef struct _ImageInfo
{
    AsyncStruct*		pAsyncStruct;
    CCImage*			pImage;
} ImageInfo;

static KDThread*					l_pLoadingThread		 = KD_NULL;

static KDThreadMutex*				l_pSleepMutex			 = KD_NULL;
static KDThreadCond*				l_pSleepCondition		 = KD_NULL;

static KDThreadMutex*				l_pAsyncStructQueueMutex = KD_NULL;
static KDThreadMutex*				l_pImageInfoMutex		 = KD_NULL;

static KDuint						l_uAsyncRefCount		 = 0;
static KDbool						l_bNeedQuit				 = KD_FALSE;

static std::queue<AsyncStruct*>*	l_pAsyncStructQueue		 = KD_NULL;
static std::queue<ImageInfo*>*		l_pImageQueue			 = KD_NULL;

static KDvoid loadImageData ( AsyncStruct* pAsyncStruct )
{
	const KDchar*	szFilename = pAsyncStruct->sFilename.c_str ( );

	// generate image            
	CCImage*	pImage = CCImage::create ( );
	if ( pImage && !pImage->initWithFile ( szFilename, CCTexture2D::defaultAlphaPixelFormat ( ) ) )
	{
		CC_SAFE_RELEASE ( pImage );
		CCLOG ( "can not load %s", szFilename );
		return;
	}

	// generate image info
	ImageInfo*  pImageInfo   = new ImageInfo ( );
	pImageInfo->pAsyncStruct = pAsyncStruct;
	pImageInfo->pImage		 = pImage;

	// put the image info into the queue
	kdThreadMutexLock ( l_pImageInfoMutex );
	l_pImageQueue->push ( pImageInfo );
	kdThreadMutexUnlock ( l_pImageInfoMutex );    
}

static KDvoid*  loadImage ( KDvoid* pData )
{
    AsyncStruct*  pAsyncStruct = KD_NULL;

    while ( 1 )
    {
        std::queue<AsyncStruct*>*  pQueue = l_pAsyncStructQueue;

		kdThreadMutexLock ( l_pAsyncStructQueueMutex ); // get async struct from queue
        if ( pQueue->empty ( ) )
        {
            kdThreadMutexUnlock ( l_pAsyncStructQueueMutex );
            if ( l_bNeedQuit )
			{
                break;
			}
            else
			{
				kdThreadCondWait ( l_pSleepCondition, l_pSleepMutex );
                continue;
			}
        }
        else
        {
            pAsyncStruct = pQueue->front ( );
            pQueue->pop ( );
            kdThreadMutexUnlock ( l_pAsyncStructQueueMutex );
			loadImageData ( pAsyncStruct );
        }          
    }
    
	if ( l_pAsyncStructQueue != KD_NULL )
	{
		CC_SAFE_DELETE ( l_pAsyncStructQueue );
		CC_SAFE_DELETE ( l_pImageQueue );

		kdThreadMutexFree ( l_pAsyncStructQueueMutex );
		kdThreadMutexFree ( l_pImageInfoMutex );
		kdThreadMutexFree ( l_pSleepMutex );
		kdThreadCondFree  ( l_pSleepCondition );
	}
    
    return 0;
}

//
// implementation CCTextureCache
//

static CCTextureCache*	l_pSharedTextureCache = KD_NULL;

/// TextureCache - Alloc, Init & Dealloc
CCTextureCache* CCTextureCache::sharedTextureCache ( KDvoid )
{
	if ( !l_pSharedTextureCache )
	{
		l_pSharedTextureCache = new CCTextureCache ( );
	}
    
	return l_pSharedTextureCache;
}

CCTextureCache::CCTextureCache ( KDvoid )
{
	CCAssert ( l_pSharedTextureCache == KD_NULL, "Attempted to allocate a second instance of a singleton." );
	
	m_pTextures = new CCDictionary ( );
	l_pSleepCondition = KD_NULL;
}

CCTextureCache::~CCTextureCache ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCTextureCache = %p", this );

	l_bNeedQuit = KD_TRUE;

	if ( l_pSleepCondition )
	{
		kdThreadCondFree ( l_pSleepCondition );
		l_pSleepCondition = KD_NULL;
	}
    
	CC_SAFE_RELEASE ( m_pTextures );
}

KDvoid CCTextureCache::purgeSharedTextureCache ( KDvoid )
{
	CC_SAFE_RELEASE ( l_pSharedTextureCache );
}

const KDchar* CCTextureCache::description ( KDvoid )
{
	return CCString::createWithFormat ( "<CCTextureCache | Number of textures = %u>", m_pTextures->count ( ) )->getCString ( );
}

CCDictionary* CCTextureCache::snapshotTextures ( KDvoid )
{ 
	CCDictionary*   pRet = CCDictionary::create ( );
    CCDictElement*  pElement = KD_NULL;
    CCDICT_FOREACH ( m_pTextures, pElement )
    {
        pRet->setObject ( pElement->getObject ( ), pElement->getStrKey ( ) );
    }
    return pRet;
}

KDvoid CCTextureCache::addImageAsync ( const KDchar* szPath, CCObject* pTarget, SEL_CallFuncO pSelector ) 
{
#ifdef EMSCRIPTEN
	CCLOGWARN ( "Cannot load image %s asynchronously in Emscripten builds.", szPath );
	return;
#endif // EMSCRIPTEN

    CCAssert ( szPath != KD_NULL, "TextureCache: fileimage MUST not be NULL" );    

    CCTexture2D*  pTexture = KD_NULL;

    // optimization

    std::string  sKey = szPath;

    sKey = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( sKey.c_str ( ) );
    pTexture = (CCTexture2D*) m_pTextures->objectForKey ( sKey.c_str ( ) );

    if ( pTexture != KD_NULL )
    {
        if ( pTarget && pSelector )
        {
            ( pTarget->*pSelector ) ( pTexture );
        }
        
        return;
    }

    // lazy init
    if ( l_pAsyncStructQueue == KD_NULL )
    {             
		l_pAsyncStructQueue = new std::queue<AsyncStruct*> ( );
		l_pImageQueue       = new std::queue<ImageInfo*>   ( );        
        
		l_pAsyncStructQueueMutex = kdThreadMutexCreate ( KD_NULL );
		l_pImageInfoMutex		 = kdThreadMutexCreate ( KD_NULL );
		l_pSleepMutex			 = kdThreadMutexCreate ( KD_NULL );
		l_pSleepCondition		 = kdThreadCondCreate  ( KD_NULL );

#if ( CC_TARGET_PLATFORM != CC_PLATFORM_WINRT ) && ( CC_TARGET_PLATFORM != CC_PLATFORM_WP8 )
		kdSleep ( 1000000 );
		l_pLoadingThread = kdThreadCreate ( KD_NULL, loadImage, KD_NULL );		
#endif	
		
        l_bNeedQuit = KD_FALSE;
    }

    if ( 0 == l_uAsyncRefCount )
    {
        CCDirector::sharedDirector ( )->getScheduler ( )->scheduleSelector ( schedule_selector ( CCTextureCache::addImageAsyncCallBack ), this, 0, KD_FALSE );
    }

    ++l_uAsyncRefCount;

    if ( pTarget )
    {
        pTarget->retain ( );
    }

    // generate async struct
    AsyncStruct*  pData = new AsyncStruct ( );
    pData->sFilename	 = sKey;
    pData->pTarget	 = pTarget;
    pData->pSelector = pSelector;

#if ( CC_TARGET_PLATFORM != CC_PLATFORM_WINRT ) && ( CC_TARGET_PLATFORM != CC_PLATFORM_WP8 )
    // add async struct into queue
    kdThreadMutexLock ( l_pAsyncStructQueueMutex );
    l_pAsyncStructQueue->push ( pData );
    kdThreadMutexUnlock ( l_pAsyncStructQueueMutex );

	kdThreadCondSignal ( l_pSleepCondition );
#else
	// WinRT uses an Async Task to load the image since the ThreadPool has a limited number of threads
	// std::replace( data->filename.begin(), data->filename.end(), '/', '\\'); 
	create_task([this, data] {
		loadImageData(data);
	});
#endif
}

KDvoid CCTextureCache::addImageAsyncCallBack ( KDfloat fTime )
{
    // the image is generated in loading thread
    std::queue<ImageInfo*>*  pImagesQueue = l_pImageQueue;

    kdThreadMutexLock ( l_pImageInfoMutex );
    if ( pImagesQueue->empty ( ) )
    {
        kdThreadMutexUnlock ( l_pImageInfoMutex );
    }
    else
    {
        ImageInfo*  pImageInfo = pImagesQueue->front ( );
        pImagesQueue->pop ( );
        kdThreadMutexUnlock ( l_pImageInfoMutex );

        AsyncStruct*  pAsyncStruct = pImageInfo->pAsyncStruct;
        CCImage*      pImage    = pImageInfo->pImage;
        CCObject*     pTarget   = pAsyncStruct->pTarget;
        SEL_CallFuncO pSelector = pAsyncStruct->pSelector;
        const KDchar* szPath    = pAsyncStruct->sFilename.c_str ( );

        // generate texture in render thread
        CCTexture2D*  pTexture = new CCTexture2D ( );

        pTexture->initWithImage ( pImage );

#if CC_ENABLE_CACHE_TEXTURE_DATA
       // cache the texture file name
       VolatileTexture::addImageTexture ( pTexture, szPath );
#endif

        // cache the texture
        m_pTextures->setObject ( pTexture, szPath );
        pTexture->autorelease ( );

        if ( pTarget && pSelector )
        {
            ( pTarget->*pSelector ) ( pTexture );
            pTarget->release ( );
        }        

        pImage->release ( );
        delete pAsyncStruct;
        delete pImageInfo;

        --l_uAsyncRefCount;
        if ( 0 == l_uAsyncRefCount )
        {
            CCDirector::sharedDirector ( )->getScheduler ( )->unscheduleSelector ( schedule_selector ( CCTextureCache::addImageAsyncCallBack ), this );
        }
    }
}

CCTexture2D* CCTextureCache::addImage ( const KDchar* szFilePath )
{
	CCAssert ( szFilePath != KD_NULL, "TextureCache : path must not be NULL" );
    
	std::string   sKey = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath );	
	if ( sKey.size ( ) == 0 )
	{
		return KD_NULL;
	}

	CCTexture2D*  pTexture = (CCTexture2D*) m_pTextures->objectForKey ( sKey.c_str ( ) );
    
	if ( !pTexture ) 
	{
		CCImage*  pImage = CCImage::create ( );

		if ( pImage->initWithFile ( sKey.c_str ( ), CCTexture2D::defaultAlphaPixelFormat ( ) ) )
		{
			pTexture = new CCTexture2D ( );
			if ( pTexture && pTexture->initWithImage ( pImage ) )
			{
			#if CC_ENABLE_CACHE_TEXTURE_DATA
				// cache the texture file name
				VolatileTexture::addImageTexture ( pTexture, sKey.c_str ( ) );
			#endif

				m_pTextures->setObject ( pTexture, sKey );
				pTexture->release ( );	
			}
			else
			{
				CC_SAFE_DELETE ( pTexture );						
			}
		}
	}

	if ( !pTexture && CCFileUtils::sharedFileUtils ( )->isPopupNotify ( ) )
	{
		CCLOG ( "XMCocos2D : Couldn't add image - %s in CCTextureCache", szFilePath );
	}
    
	return pTexture;
}

CCTexture2D* CCTextureCache::addCCImage ( CCImage* pImage, const KDchar* szKey )
{
    CCAssert ( pImage != KD_NULL, "TextureCache: image MUST not be nill");

    CCTexture2D*  pTexture = KD_NULL;

    // textureForKey() use full path,so the key should be full path 
    // Don't have to lock here, because addImageAsync() will not 
    // invoke opengl function in loading thread.
    do 
    {
		std::string   sForKey;

		// If szKey is nil, then create a new texture each time
		if ( szKey )
		{			
			sForKey  = szKey;
			pTexture = (CCTexture2D*) m_pTextures->objectForKey ( sForKey.c_str ( ) );
			if ( pTexture )
			{
				break;
			}
		}

        // prevents overloading the autorelease pool
        pTexture = new CCTexture2D ( );
		if ( pTexture && pTexture->initWithImage ( pImage ) )
		{
		#if CC_ENABLE_CACHE_TEXTURE_DATA
			VolatileTexture::addCCImage ( pTexture, pImage );
		#endif

			if ( szKey )
			{
				m_pTextures->setObject ( pTexture, sForKey.c_str ( ) );
				pTexture->autorelease ( );
			}
		}
		else
		{
			CC_SAFE_DELETE ( pTexture );
			CCLOG ( "XMCocos2D : Couldn't add CCImage in CCTextureCache" );
		}

    } while ( 0 );

    return pTexture;
}

/// TextureCache - Remove

KDvoid CCTextureCache::removeAllTextures ( KDvoid )
{
	m_pTextures->removeAllObjects ( );
}

KDvoid CCTextureCache::removeUnusedTextures ( KDvoid )
{
    // Inter engineer zhuoshi sun finds that this way will get better performance    
    if ( m_pTextures->count ( ) )
    {   
        // find elements to be removed
        CCDictElement*  pElement = KD_NULL;
		std::list<CCDictElement*>  pElementToRemove;
        CCDICT_FOREACH ( m_pTextures, pElement )
        {
//			CCLOG ( "XMCocos2D : CCTextureCache: texture: %s", pElement->getStrKey ( ) );
            CCTexture2D*  pValue = (CCTexture2D*) pElement->getObject ( );
            if ( pValue->retainCount ( ) == 1 )
            {
                pElementToRemove.push_back ( pElement );
            }
        }
        
        // remove elements
		for ( std::list<CCDictElement*>::iterator iter = pElementToRemove.begin ( ); iter != pElementToRemove.end ( ); ++iter )
        {
//			CCLOG ( "XMCocos2D : CCTextureCache: removing unused texture: %s", ( *iter )->getStrKey ( ) );
            m_pTextures->removeObjectForElememt ( *iter );
        }
    }
}

KDvoid CCTextureCache::removeTexture ( CCTexture2D* pTexture )
{
	if ( !pTexture )
	{
		return;
	}

    CCArray*  pKeys = m_pTextures->allKeysForObject ( pTexture );
    m_pTextures->removeObjectsForKeys ( pKeys );
}

KDvoid CCTextureCache::removeTextureForKey ( const KDchar* szKey )
{
	if ( szKey == KD_NULL )
	{
		return;
	}

	std::string  sForKey = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szKey );
	m_pTextures->removeObjectForKey ( sForKey );
}

CCTexture2D* CCTextureCache::textureForKey ( const KDchar* szKey )
{
	std::string  sForKey = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szKey );
	return (CCTexture2D*) m_pTextures->objectForKey ( sForKey );
}

KDvoid CCTextureCache::reloadAllTextures ( )
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::reloadAllTextures ( );
#endif
}

KDvoid CCTextureCache::dumpCachedTextureInfo ( KDvoid )
{
    KDuint  uCount = 0;
    KDuint  uTotalBytes = 0;

    CCDictElement*  pElement = KD_NULL;
    CCDICT_FOREACH ( m_pTextures, pElement )
    {
        CCTexture2D*  pTexture = (CCTexture2D*) pElement->getObject();
        KDuint  uBpp = pTexture->bitsPerPixelForFormat();
        // Each texture takes up width * height * bytesPerPixel bytes.
        KDuint  uBytes = pTexture->getPixelsWide ( ) * pTexture->getPixelsHigh ( ) * uBpp / 8;
        uTotalBytes += uBytes;
        uCount++;
        CCLOG ( "XMCocos2D : \"%s\" rc=%lu id=%lu %lu x %lu @ %ld bpp => %lu KB",
                pElement->getStrKey ( ),
                pTexture->retainCount ( ),
                pTexture->getName ( ),
                pTexture->getPixelsWide ( ),
                pTexture->getPixelsHigh ( ),
                uBpp,
                uBytes / 1024 );
    }

    CCLOG ( "XMCocos2D : CCTextureCache dumpDebugInfo: %ld textures, for %lu KB (%.2f MB)", uCount, uTotalBytes / 1024, uTotalBytes / ( 1024.0f * 1024.0f ) );
}

#if CC_ENABLE_CACHE_TEXTURE_DATA

static std::list<VolatileTexture*>	l_pTextures;
static KDbool						l_bIsReloading = KD_FALSE;

VolatileTexture::VolatileTexture ( CCTexture2D* pTexture )
{
    m_pTexture          = pTexture;
	m_pImage		    = KD_NULL;
    m_eCashedImageType  = kInvalid;

	m_pTextureData      = KD_NULL;
    m_tTextureSize		= CCSizeZero;
    m_ePixelFormat      = kCCTexture2DPixelFormat_RGBA8888;

    m_sFilePath			= "";

    m_tDimension		= CCSizeZero;
    m_eAlign			= kCCAlignmentCenter;
    m_fFontSize         = 0.0f;
    m_sText				= "";

    m_tTexParams.minFilter = GL_LINEAR;
    m_tTexParams.magFilter = GL_LINEAR;
    m_tTexParams.wrapS     = GL_CLAMP_TO_EDGE;
    m_tTexParams.wrapT     = GL_CLAMP_TO_EDGE;

    l_pTextures.push_back ( this );
}

VolatileTexture::~VolatileTexture ( KDvoid )
{
    l_pTextures.remove ( this );
	CC_SAFE_FREE ( m_pImage );
}

KDvoid VolatileTexture::addImageTexture ( CCTexture2D* pTexture, const KDchar* szFilePath )
{
    if ( l_bIsReloading )
    {
        return;
    }

    VolatileTexture*  pVT = findVolotileTexture ( pTexture );

    pVT->m_eCashedImageType = kImageFile;
    pVT->m_sFilePath		= szFilePath;
    pVT->m_ePixelFormat		= pTexture->getPixelFormat ( );
}

KDvoid VolatileTexture::addCCImage ( CCTexture2D* pTexture, CCImage* pImage )
{
    if ( l_bIsReloading )
    {
        return;
    }

    VolatileTexture*  pVT = findVolotileTexture ( pTexture );

    pImage->retain ( );

    pVT->m_pImage = pImage;
    pVT->m_eCashedImageType = kImage;
}

VolatileTexture* VolatileTexture::findVolotileTexture ( CCTexture2D* pTexture )
{
    VolatileTexture*  pVT = KD_NULL;
    std::list<VolatileTexture *>::iterator  iter = l_pTextures.begin ( );
    while ( iter != l_pTextures.end ( ) )
    {
        VolatileTexture*  pV = *iter++;
        if ( pV->m_pTexture == pTexture ) 
        {
            pVT = pV;
            break;
        }
    }
    
    if ( !pVT )
    {
        pVT = new VolatileTexture ( pTexture );
    }
    
    return pVT;
}

KDvoid VolatileTexture::addDataTexture ( CCTexture2D* pTexture, KDvoid* pData, CCTexture2DPixelFormat ePixelFormat, const CCSize& tContentSize )
{
    if ( l_bIsReloading )
    {
        return;
    }

    VolatileTexture*  pVT = findVolotileTexture ( pTexture );

    pVT->m_eCashedImageType = kImageData;
    pVT->m_pTextureData		= pData;
    pVT->m_ePixelFormat		= ePixelFormat;
    pVT->m_tTextureSize		= tContentSize;
}

KDvoid VolatileTexture::addStringTexture ( CCTexture2D* pTexture, const KDchar* szText, const CCSize& tDimension, CCAlignment eAlign, const KDchar* szFontPath, KDfloat fFontSize )
{
    if ( l_bIsReloading )
    {
        return;
    }

    VolatileTexture*  pVT = findVolotileTexture ( pTexture );

    pVT->m_eCashedImageType = kString;
    pVT->m_tDimension		= tDimension;
    pVT->m_sFilePath		= szFontPath;
    pVT->m_eAlign			= eAlign;
    pVT->m_fFontSize		= fFontSize;
    pVT->m_sText			= szText;
}

KDvoid VolatileTexture::setTexParameters ( CCTexture2D* pTexture, const ccTexParams& tTexParams ) 
{
    VolatileTexture*  pVT = findVolotileTexture ( pTexture );

    if ( tTexParams.minFilter != 0 )
	{
        pVT->m_tTexParams.minFilter = tTexParams.minFilter;
	}
    if ( tTexParams.magFilter != 0 )
	{
        pVT->m_tTexParams.magFilter = tTexParams.magFilter;
	}
    if ( tTexParams.wrapS != 0 )
	{
        pVT->m_tTexParams.wrapS = tTexParams.wrapS;
	}
    if ( tTexParams.wrapT != 0 )
	{
        pVT->m_tTexParams.wrapT = tTexParams.wrapT;
	}
}

KDvoid VolatileTexture::removeTexture ( CCTexture2D* pTexture )
{
    std::list<VolatileTexture *>::iterator  iter = l_pTextures.begin ( );
    while ( iter != l_pTextures.end ( ) )
    {
        VolatileTexture*  pVT = *iter++;
        if ( pVT->m_pTexture == pTexture ) 
		{
            delete pVT;
            break;
        }
    }
}

KDvoid VolatileTexture::reloadAllTextures ( KDvoid )
{
    l_bIsReloading = KD_TRUE;

    std::list<VolatileTexture *>::iterator  iter = l_pTextures.begin ( );

    while ( iter != l_pTextures.end ( ) )
    {
        VolatileTexture* pVT = *iter++;
		CCTexture2D*    pTex = pVT->m_pTexture;
        
        switch ( pVT->m_eCashedImageType )
        {
			case kImageFile:
				{
					CCImage  tImage;                   
					if ( tImage.initWithFile ( pVT->m_sFilePath.c_str ( ), CCTexture2D::defaultAlphaPixelFormat ( ) ) )
					{
						pTex->initWithImage ( &tImage );
					}					
				}
				break;			

			case kImageData :
                
				pTex->initWithData 
				(
					pVT->m_pTextureData,
					pVT->m_ePixelFormat,
					pVT->m_tTextureSize
				);

				break;

			case kString :

				pTex->initWithString 
				(
					pVT->m_sText.c_str ( ),
					pVT->m_sFilePath.c_str ( ),
					pVT->m_fFontSize,
					pVT->m_tDimension,
					pVT->m_eAlign										
				);

				break;

			case kImage :
                
				pTex->initWithImage 
				(
					pVT->m_pImage 
				);
				
				break;

			default :
				break;
        }

		pTex->setTexParameters ( pVT->m_tTexParams );
    }

    l_bIsReloading = KD_FALSE;
}

#endif // CC_ENABLE_CACHE_TEXTURE_DATA

NS_CC_END