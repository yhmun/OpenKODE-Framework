/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextureCache.h 
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

#ifndef __CCTextureCache_h__
#define __CCTextureCache_h__

#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <string>
#include <unordered_map>

#include "../../base/CCObject.h"
#include "../../platform/CCImage.h"
#include "CCTexture2D.h"

#if CC_ENABLE_CACHE_TEXTURE_DATA
    #include <list>
#endif

NS_CC_BEGIN

/**
 *	@addtogroup textures
 *	@{
 */

/*
 *	from version 3.0, TextureCache will never to treated as a singleton, it will be owned by director.
 *	all call by TextureCache::getInstance() should be replaced by Director::getInstance()->getTextureCache()
 */

/**
 *	@brief Singleton that handles the loading of textures
 *	Once the texture is loaded, the next time it will return
 *	a reference of the previously loaded texture reducing GPU & CPU memory
 */
class CC_DLL TextureCache : public Object
{
	public :

		/**
		 *	@js ctor
		 */
		TextureCache ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TextureCache ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*		description ( KDvoid ) const;

//		Dictionary*			snapshotTextures ( KDvoid );

		/**
		 *	Returns a Texture2D object given an filename.
		 *	If the filename was not previously loaded, it will create a new Texture2D
		 *  object and it will return it. It will use the filename as a key.
		 *	Otherwise it will return a reference of a previously loaded image.
		 *	Supported image extensions: .png, .bmp, .tiff, .jpeg, .pvr, .gif
		 */
		Texture2D*			addImage ( const std::string& sFilepath );

		/** 
		 *	Returns a Texture2D object given a file image
		 *	If the file image was not previously loaded, it will create a new Texture2D object and it will return it.
		 *	Otherwise it will load a texture in a new thread, and when the image is loaded, the callback will be called with the Texture2D as a parameter.
		 *	The callback will be called from the main thread, so it is safe to create any cocos2d object from the callback.
		 *	Supported image extensions: .png, .jpg
		 *	@since v0.8
		 */
		virtual KDvoid		addImageAsync ( const std::string& sFilepath, Object* pTarget, SEL_CallFuncO pSelector );

		/** 
		 *	Returns a Texture2D object given an Image.
		 *	If the image was not previously loaded, it will create a new Texture2D object and it will return it.
		 *	Otherwise it will return a reference of a previously loaded image.
		 *	The "key" parameter will be used as the "key" for the cache.
		 *	If "key" is nil, then a new texture will be created each time.
		 */
		Texture2D*			addImage ( Image* pImage, const std::string& sKey );

		/**
		 *	Returns an already created texture. Returns nil if the texture doesn't exist.
		 *	@since v0.99.5
		 */
		Texture2D*			getTextureForKey ( const std::string& sKey ) const;

		/** 
		 *	Purges the dictionary of loaded textures.
		 *	Call this method if you receive the "Memory Warning"
		 *	In the short term: it will free some resources preventing your app from being killed
		 *	In the medium term: it will allocate more resources
		 *	In the long term: it will be the same
		 */
		KDvoid				removeAllTextures ( KDvoid );

		/** 
		 *	Removes unused textures
		 *	Textures that have a retain count of 1 will be deleted
		 *	It is convenient to call this method after when starting a new Scene
		 *	@since v0.8
		 */
		KDvoid				removeUnusedTextures ( KDvoid );

		/**
		 *	Deletes a texture from the cache given a texture
		 */
		KDvoid				removeTexture ( Texture2D* pTexture );

		/**
		 *	Deletes a texture from the cache given a its key name
		 *	@since v0.99.4
		 */
		KDvoid				removeTextureForKey ( const std::string& sKey );

		/**
		 *	Output to CCLOG the current contents of this TextureCache
		 *	This will attempt to calculate the size of each texture, and the total texture memory in use
		 *
		 *	@since v1.0
		 */
		KDvoid				dumpCachedTextureInfo ( KDvoid ) const;

		// wait for texture cahe to quit befor destroy instance
		// called by director, please do not called outside
		KDvoid				waitForQuit ( KDvoid );

	private :

		KDvoid				addImageAsyncCallBack ( KDfloat fDelta );

		KDvoid				loadImage ( KDvoid );

	public :

		struct AsyncStruct
		{
			public :

				AsyncStruct ( const std::string& sFilename, Object* pTarget, SEL_CallFuncO pSelector ) 
				: sFilename ( sFilename ), pTarget ( pTarget ), pSelector ( pSelector ) 
				{

				}

				std::string			sFilename;
				Object*				pTarget;
				SEL_CallFuncO		pSelector;
		};

	protected :

		typedef struct _ImageInfo
		{
			AsyncStruct*			pAsyncStruct;
			Image*					pImage;
		} ImageInfo;
    
		std::thread*				m_pLoadingThread;

		std::queue<AsyncStruct*>*	m_pAsyncStructQueue;
		std::queue<ImageInfo*>*		m_pImageInfoQueue;

		std::mutex					m_tAsyncStructQueueMutex;
		std::mutex					m_tImageInfoMutex;

		std::mutex					m_tSleepMutex;
		std::condition_variable		m_tSleepCondition;

		KDbool						m_bNeedQuit;

		KDint						m_nAsyncRefCount;

		std::unordered_map<std::string, Texture2D*>		m_aTextures;
};

#if CC_ENABLE_CACHE_TEXTURE_DATA

class VolatileTexture
{
		typedef enum
		{
			kInvalid		= 0	,
			kImageFile			,
			kImageData			,
			kString				,
			kImage				,
		} ccCachedImageType;

	private :

		VolatileTexture ( Texture2D* t );

		/**
		 * @js NA
		 * @lua NA
		 */
		~VolatileTexture ( KDvoid );

	protected :

		friend class VolatileTextureMgr;

		Texture2D*					m_pTexture;
    
		Image*						m_pUiImage;

		ccCachedImageType			m_eCashedImageType;

		KDvoid*						m_pTextureData;
		KDint						m_nDataLen;
		Size						m_tTextureSize;
		Texture2D::PixelFormat		m_ePixelFormat;

		std::string _fileName;

		Texture2D::TexParams		m_eTexParams;
		std::string					m_sText;
		FontDefinition				m_tFontDefinition;
};

class VolatileTextureMgr
{
	public :

		static KDvoid 				addImageTexture ( Texture2D* tt, const KDchar* szImageFileName );
		
		static KDvoid 				addStringTexture ( Texture2D* tt, const KDchar* szText, const FontDefinition& tFontDefinition );
		
		static KDvoid 				addDataTexture ( Texture2D* tt, KDvoid* data, KDint nDataLen, Texture2D::PixelFormat ePixelFormat, const Size& tContentSize );

		static KDvoid 				addImage ( Texture2D* tt, Image* pImage );

		static KDvoid 				setTexParameters ( Texture2D* t, const Texture2D::TexParams& tTexParams );

		static KDvoid				removeTexture ( Texture2D* t );

		static KDvoid				reloadAllTextures ( KDvoid );

	public :

		static std::list<VolatileTexture*>	m_aTextures;
		static KDbool						m_bIsReloading;

	private :

		// find VolatileTexture by Texture2D*
		// if not found, create a new one
		static VolatileTexture*		findVolotileTexture ( Texture2D* tt );
};

#endif

// end of textures group
/// @}

NS_CC_END

#endif	// __CCTextureCache_h__

