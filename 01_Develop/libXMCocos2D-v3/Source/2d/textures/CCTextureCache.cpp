/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextureCache.cpp 
 *      Author          Young-Hwan Mun
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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
 * --------------------------------------------------------------------------------- */ 

#include <errno.h>
#include <stack>
#include <cctype>
#include <list>

#include "2d/textures/CCTextureCache.h"
#include "2d/textures/CCTexture2D.h"
#include "2d/CCDirector.h"
#include "platform/CCFileUtils.h"
#include "platform/CCThread.h"
#include "support/ccUtils.h"
#include "2d/CCScheduler.h"
#include "base/CCString.h"

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#include "platform/emscripten/CCTextureCacheEmscripten.h"
#endif // EMSCRIPTEN

using namespace std;

NS_CC_BEGIN

// implementation TextureCache

TextureCache::TextureCache()
: m_pLoadingThread(nullptr)
, m_pAsyncStructQueue(nullptr)
, m_pImageInfoQueue(nullptr)
, m_bNeedQuit(false)
, m_nAsyncRefCount(0)
{
}

TextureCache::~TextureCache()
{
    CCLOGINFO("deallocing TextureCache: %p", this);

    for( auto it=m_aTextures.begin(); it!=m_aTextures.end(); ++it)
        (it->second)->release();

    CC_SAFE_DELETE(m_pLoadingThread);
}

const char* TextureCache::description() const
{
    return String::createWithFormat("<TextureCache | Number of textures = %u>", m_aTextures.size() )->getCString();
}

//Dictionary* TextureCache::snapshotTextures()
//{ 
//    Dictionary* pRet = new Dictionary();
//    DictElement* pElement = NULL;
//    CCDICT_FOREACH(_textures, pElement)
//    {
//        pRet->setObject(pElement->getObject(), pElement->getStrKey());
//    }
//    pRet->autorelease();
//    return pRet;
//}

void TextureCache::addImageAsync(const std::string &path, Object *target, SEL_CallFuncO selector)
{
    Texture2D *texture = NULL;

    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(path.c_str());

    auto it = m_aTextures.find(fullpath);
    if( it != m_aTextures.end() )
        texture = it->second;

    if (texture != NULL && target && selector)
    {
        (target->*selector)(texture);
        return;
    }

    // lazy init
    if (m_pAsyncStructQueue == NULL)
    {             
        m_pAsyncStructQueue = new queue<AsyncStruct*>();
        m_pImageInfoQueue   = new queue<ImageInfo*>();        

        // create a new thread to load images
        m_pLoadingThread = new std::thread(&TextureCache::loadImage, this);

        m_bNeedQuit = false;
    }

    if (0 == m_nAsyncRefCount)
    {
        Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(TextureCache::addImageAsyncCallBack), this, 0, false);
    }

    ++m_nAsyncRefCount;

    if (target)
    {
        target->retain();
    }

    // generate async struct
    AsyncStruct *data = new AsyncStruct(fullpath, target, selector);

    // add async struct into queue
    m_tAsyncStructQueueMutex.lock();
    m_pAsyncStructQueue->push(data);
    m_tAsyncStructQueueMutex.unlock();

    m_tSleepCondition.notify_one();
}

void TextureCache::loadImage()
{
    AsyncStruct *asyncStruct = nullptr;

    while (true)
    {
        // create autorelease pool for iOS
        Thread thread;
        thread.createAutoreleasePool();

        std::queue<AsyncStruct*> *pQueue = m_pAsyncStructQueue;
        m_tAsyncStructQueueMutex.lock();
        if (pQueue->empty())
        {
            m_tAsyncStructQueueMutex.unlock();
            if (m_bNeedQuit) {
                break;
            }
            else {
                std::unique_lock<std::mutex> lk(m_tSleepMutex);
                m_tSleepCondition.wait(lk);
                continue;
            }
        }
        else
        {
            asyncStruct = pQueue->front();
            pQueue->pop();
            m_tAsyncStructQueueMutex.unlock();
        }        

        const char *filename = asyncStruct->sFilename.c_str();
        
        // generate image            
        Image *image = new Image();
        if (image && !image->initWithImageFileThreadSafe(filename))
        {
            CC_SAFE_RELEASE(image);
            CCLOG("can not load %s", filename);
            continue;
        }

        // generate image info
        ImageInfo *imageInfo = new ImageInfo();
        imageInfo->pAsyncStruct = asyncStruct;
        imageInfo->pImage = image;

        // put the image info into the queue
        m_tImageInfoMutex.lock();
        m_pImageInfoQueue->push(imageInfo);
        m_tImageInfoMutex.unlock();
    }
    
	if(m_pAsyncStructQueue != nullptr)
    {
        delete m_pAsyncStructQueue;
	    m_pAsyncStructQueue = nullptr;
        delete m_pImageInfoQueue;
	    m_pImageInfoQueue = nullptr;
    }
}

void TextureCache::addImageAsyncCallBack(float dt)
{
    // the image is generated in loading thread
    std::queue<ImageInfo*> *imagesQueue = m_pImageInfoQueue;

    m_tImageInfoMutex.lock();
    if (imagesQueue->empty())
    {
        m_tImageInfoMutex.unlock();
    }
    else
    {
        ImageInfo *imageInfo = imagesQueue->front();
        imagesQueue->pop();
        m_tImageInfoMutex.unlock();

        AsyncStruct *asyncStruct = imageInfo->pAsyncStruct;
        Image *image = imageInfo->pImage;

        Object *target = asyncStruct->pTarget;
        SEL_CallFuncO selector = asyncStruct->pSelector;
        const char* filename = asyncStruct->sFilename.c_str();

        // generate texture in render thread
        Texture2D *texture = new Texture2D();

        texture->initWithImage(image);

#if CC_ENABLE_CACHE_TEXTURE_DATA
       // cache the texture file name
       VolatileTextureMgr::addImageTexture(texture, filename);
#endif
        // cache the texture. retain it, since it is added in the map
        m_aTextures.insert( std::make_pair(filename, texture) );
        texture->retain();

        texture->autorelease();

        if (target && selector)
        {
            (target->*selector)(texture);
            target->release();
        }        

        image->release();
        delete asyncStruct;
        delete imageInfo;

        --m_nAsyncRefCount;
        if (0 == m_nAsyncRefCount)
        {
            Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(TextureCache::addImageAsyncCallBack), this);
        }
    }
}

Texture2D * TextureCache::addImage(const std::string &path)
{
    Texture2D * texture = NULL;
    Image* image = NULL;
    // Split up directory and filename
    // MUTEX:
    // Needed since addImageAsync calls this method from a different thread

    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(path.c_str());
    if (fullpath.size() == 0)
    {
        return NULL;
    }
    auto it = m_aTextures.find(fullpath);
    if( it != m_aTextures.end() )
        texture = it->second;

    if (! texture)
    {
        // all images are handled by UIImage except PVR extension that is handled by our own handler
        do 
        {
            image = new Image();
            CC_BREAK_IF(NULL == image);

            bool bRet = image->initWithImageFile(fullpath.c_str());
            CC_BREAK_IF(!bRet);

            texture = new Texture2D();

            if( texture && texture->initWithImage(image) )
            {
#if CC_ENABLE_CACHE_TEXTURE_DATA
                // cache the texture file name
                VolatileTextureMgr::addImageTexture(texture, fullpath.c_str());
#endif
                // texture already retained, no need to re-retain it
                m_aTextures.insert( std::make_pair(fullpath, texture) );
            }
            else
            {
                CCLOG("cocos2d: Couldn't create texture for file:%s in TextureCache", path.c_str());
            }
        } while (0);
    }

    CC_SAFE_RELEASE(image);

    return texture;
}

Texture2D* TextureCache::addImage(Image *image, const std::string &key)
{
    CCASSERT(image != NULL, "TextureCache: image MUST not be nil");

    Texture2D * texture = NULL;

    do
    {
        auto it = m_aTextures.find(key);
        if( it != m_aTextures.end() ) {
            texture = it->second;
            break;
        }

        // prevents overloading the autorelease pool
        texture = new Texture2D();
        texture->initWithImage(image);

        if(texture)
        {
            m_aTextures.insert( std::make_pair(key, texture) );
            texture->retain();

            texture->autorelease();
        }
        else
        {
            CCLOG("cocos2d: Couldn't add UIImage in TextureCache");
        }

    } while (0);
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTextureMgr::addImage(texture, image);
#endif
    
    return texture;
}

// TextureCache - Remove

void TextureCache::removeAllTextures()
{
    for( auto it=m_aTextures.begin(); it!=m_aTextures.end(); ++it ) {
        (it->second)->release();
    }
    m_aTextures.clear();
}

void TextureCache::removeUnusedTextures()
{
    for( auto it=m_aTextures.cbegin(); it!=m_aTextures.cend(); /* nothing */) {
        Texture2D *tex = it->second;
        if( tex->retainCount() == 1 ) {
            CCLOG("cocos2d: TextureCache: removing unused texture: %s", it->first.c_str());

            tex->release();
            m_aTextures.erase(it++);
        } else {
            ++it;
        }

    }
}

void TextureCache::removeTexture(Texture2D* texture)
{
    if( ! texture )
    {
        return;
    }

    for( auto it=m_aTextures.cbegin(); it!=m_aTextures.cend(); /* nothing */ ) {
        if( it->second == texture ) {
            texture->release();
            m_aTextures.erase(it++);
            break;
        } else
            ++it;
    }
}

void TextureCache::removeTextureForKey(const std::string &textureKeyName)
{
    auto it = m_aTextures.find(textureKeyName);
    if( it != m_aTextures.end() ) {
        (it->second)->release();
        m_aTextures.erase(it);
    }
}

Texture2D* TextureCache::getTextureForKey(const std::string &key) const
{
    auto it = m_aTextures.find(key);
    if( it != m_aTextures.end() )
        return it->second;
    return NULL;
}

void TextureCache::waitForQuit()
{
    // notify sub thread to quick
    m_bNeedQuit = true;
    m_tSleepCondition.notify_one();
    if (m_pLoadingThread) m_pLoadingThread->join();
}

void TextureCache::dumpCachedTextureInfo() const
{
    unsigned int count = 0;
    unsigned int totalBytes = 0;

    for( auto it = m_aTextures.begin(); it != m_aTextures.end(); ++it ) {

        Texture2D* tex = it->second;
        unsigned int bpp = tex->getBitsPerPixelForFormat();
        // Each texture takes up width * height * bytesPerPixel bytes.
        KDint32 bytes = tex->getPixelsWide() * tex->getPixelsHigh() * bpp / 8;
        totalBytes += bytes;
        count++;
        log("cocos2d: \"%s\" rc=%d id=%d %d x %d @ %d bpp => %d KB",
               it->first.c_str(),
               tex->retainCount(),
               tex->getName(),
               tex->getPixelsWide(),
               tex->getPixelsHigh(),
               bpp,
               bytes / 1024);
    }

    log("cocos2d: TextureCache dumpDebugInfo: %d textures, for %d KB (%.2f MB)", count, totalBytes / 1024, totalBytes / (1024.0f*1024.0f));
}

#if CC_ENABLE_CACHE_TEXTURE_DATA

std::list<VolatileTexture*> VolatileTextureMgr::m_aTextures;
bool VolatileTextureMgr::m_bIsReloading = false;

VolatileTexture::VolatileTexture(Texture2D *t)
: m_pTexture(t)
, m_eCashedImageType(kInvalid)
, m_pTextureData(NULL)
, m_ePixelFormat(Texture2D::PixelFormat::RGBA8888)
, _fileName("")
, m_sText("")
, m_pUiImage(NULL)
{
    m_eTexParams.minFilter = GL_LINEAR;
    m_eTexParams.magFilter = GL_LINEAR;
    m_eTexParams.wrapS = GL_CLAMP_TO_EDGE;
    m_eTexParams.wrapT = GL_CLAMP_TO_EDGE;
}

VolatileTexture::~VolatileTexture()
{
    CC_SAFE_RELEASE(m_pUiImage);
}

void VolatileTextureMgr::addImageTexture(Texture2D *tt, const char* imageFileName)
{
    if (m_bIsReloading)
    {
        return;
    }

    VolatileTexture *vt = findVolotileTexture(tt);

    vt->m_eCashedImageType = VolatileTexture::kImageFile;
    vt->_fileName = imageFileName;
    vt->m_ePixelFormat = tt->getPixelFormat();
}

void VolatileTextureMgr::addImage(Texture2D *tt, Image *image)
{
    VolatileTexture *vt = findVolotileTexture(tt);
    image->retain();
    vt->m_pUiImage = image;
    vt->m_eCashedImageType = VolatileTexture::kImage;
}

VolatileTexture* VolatileTextureMgr::findVolotileTexture(Texture2D *tt)
{
    VolatileTexture *vt = 0;
    auto i = m_aTextures.begin();
    while (i != m_aTextures.end())
    {
        VolatileTexture *v = *i++;
        if (v->m_pTexture == tt)
        {
            vt = v;
            break;
        }
    }
    
    if (! vt)
    {
        vt = new VolatileTexture(tt);
        m_aTextures.push_back(vt);
    }
    
    return vt;
}

void VolatileTextureMgr::addDataTexture(Texture2D *tt, void* data, int dataLen, Texture2D::PixelFormat pixelFormat, const Size& contentSize)
{
    if (m_bIsReloading)
    {
        return;
    }

    VolatileTexture *vt = findVolotileTexture(tt);

    vt->m_eCashedImageType = VolatileTexture::kImageData;
    vt->m_pTextureData = data;
    vt->m_nDataLen = dataLen;
    vt->m_ePixelFormat = pixelFormat;
    vt->m_tTextureSize = contentSize;
}

void VolatileTextureMgr::addStringTexture(Texture2D *tt, const char* text, const FontDefinition& fontDefinition)
{
    if (m_bIsReloading)
    {
        return;
    }

    VolatileTexture *vt = findVolotileTexture(tt);

    vt->m_eCashedImageType = VolatileTexture::kString;
    vt->m_sText     = text;
    vt->m_tFontDefinition = fontDefinition;
}

void VolatileTextureMgr::setTexParameters(Texture2D *t, const Texture2D::TexParams &texParams)
{
    VolatileTexture *vt = findVolotileTexture(t);

    if (texParams.minFilter != GL_NONE)
        vt->m_eTexParams.minFilter = texParams.minFilter;
    if (texParams.magFilter != GL_NONE)
        vt->m_eTexParams.magFilter = texParams.magFilter;
    if (texParams.wrapS != GL_NONE)
        vt->m_eTexParams.wrapS = texParams.wrapS;
    if (texParams.wrapT != GL_NONE)
        vt->m_eTexParams.wrapT = texParams.wrapT;
}

void VolatileTextureMgr::removeTexture(Texture2D *t) 
{
    auto i = m_aTextures.begin();
    while (i != m_aTextures.end())
    {
        VolatileTexture *vt = *i++;
        if (vt->m_pTexture == t) 
        {
            m_aTextures.remove(vt);
            delete vt;
            break;
        }
    }
}

void VolatileTextureMgr::reloadAllTextures()
{
    m_bIsReloading = true;

    CCLOG("reload all texture");
    auto iter = m_aTextures.begin();

    while (iter != m_aTextures.end())
    {
        VolatileTexture *vt = *iter++;

        switch (vt->m_eCashedImageType)
        {
        case VolatileTexture::kImageFile:
            {
                Image* image = new Image();
                KDint32 size = 0;
                KDubyte* pBuffer = FileUtils::getInstance()->getFileData(vt->_fileName.c_str(), "rb", &size);
                
                if (image && image->initWithImageData(pBuffer, size))
                {
                    Texture2D::PixelFormat oldPixelFormat = Texture2D::getDefaultAlphaPixelFormat();
                    Texture2D::setDefaultAlphaPixelFormat(vt->m_ePixelFormat);
                    vt->m_pTexture->initWithImage(image);
                    Texture2D::setDefaultAlphaPixelFormat(oldPixelFormat);
                }
                
                free(pBuffer);
                CC_SAFE_RELEASE(image);
            }
            break;
        case VolatileTexture::kImageData:
            {
                vt->m_pTexture->initWithData(vt->m_pTextureData,
                                           vt->m_nDataLen,
                                          vt->m_ePixelFormat, 
                                          vt->m_tTextureSize.width, 
                                          vt->m_tTextureSize.height, 
                                          vt->m_tTextureSize);
            }
            break;
        case VolatileTexture::kString:
            {
                vt->m_pTexture->initWithString(vt->m_sText.c_str(), vt->m_tFontDefinition);
            }
            break;
        case VolatileTexture::kImage:
            {
                vt->m_pTexture->initWithImage(vt->m_pUiImage);
            }
            break;
        default:
            break;
        }
        vt->m_pTexture->setTexParameters(vt->m_eTexParams);
    }

    m_bIsReloading = false;
}

#endif // CC_ENABLE_CACHE_TEXTURE_DATA

NS_CC_END

