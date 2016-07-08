/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextureCache.h 
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

#ifndef __CCTextureCache_h__
#define __CCTextureCache_h__

#include "CCTexture2D.h"
#include "../cocoa/CCDictionary.h"
#include "../ccConfig.h"

#include <string>

NS_CC_BEGIN

class CCImage;

/**
 * @addtogroup textures
 * @{
 */

/**
 *	@brief	Singleton that handles the loading of textures
 *	Once the texture is loaded, the next time it will return
 *	a reference of the previously loaded texture reducing GPU & CPU memory
 */
class CCTextureCache : public CCObject
{
	protected :
		         CCTextureCache ( KDvoid );
		virtual ~CCTextureCache ( KDvoid );

	public :

		/** Returns the shared instance of the cache */
		static  CCTextureCache*		sharedTextureCache ( KDvoid );

		/**
		 *	purges the cache. It releases the retained instance.
		 *	@since  v0.99.0
		 */
		static  KDvoid				purgeSharedTextureCache ( KDvoid );  

		/** 
		 *	Reload all textures
		 *	It's only useful when the value of CC_ENABLE_CACHE_TEXTURE_DATA is 1
		 */
		static  KDvoid				reloadAllTextures ( KDvoid );

    public :

		const KDchar*				description ( KDvoid );

		CCDictionary*				snapshotTextures ( KDvoid );

		/**
		 *	Returns a Texture2D object given an file image
		 *	If the file image was not previously loaded, it will create a new CCTexture2D
		 *  object and it will return it. It will use the filename as a key.
		 *	Otherwise it will return a reference of a previously loaded image.
		 *	Supported image extensions: .png, .bmp, .jpeg, .pvr, .tga
		 */
        CCTexture2D*				addImage ( const KDchar* szFilePath );

		/*  
		 *	Returns a Texture2D object given a file image
		 *  If the file image was not previously loaded, it will create a new CCTexture2D object and it will return it.
		 *  Otherwise it will load a texture in a new thread, and when the image is loaded, the callback will be called with the Texture2D as a parameter.
		 *  The callback will be called from the main thread, so it is safe to create any cocos2d object from the callback.
		 *  Supported image extensions: .png, .jpg
		 *  @since v0.8
		 */
		KDvoid						addImageAsync ( const KDchar* szPath, CCObject* pTarget, SEL_CallFuncO pSelector );

		/** 
		 *	Returns a Texture2D object given an UIImage image
		 *	If the image was not previously loaded, it will create a new CCTexture2D object and it will return it.
		 *	Otherwise it will return a reference of a previously loaded image
		 *	The "key" parameter will be used as the "key" for the cache.
		 *	If "key" is nil, then a new texture will be created each time.
		 */
		CCTexture2D*				addCCImage ( CCImage* pImage, const KDchar* szKey );

		/** 
		 *	Returns an already created texture. Returns nil if the texture doesn't exist.
		 *	@since	v0.99.5
		 */
		CCTexture2D*				textureForKey ( const KDchar* szKey );

		/**
		 *	Purges the dictionary of loaded textures.
		 *	Call this method if you receive the "Memory Warning"
		 *	In the short term: it will free some resources preventing your app from being killed
		 *	In the medium term: it will allocate more resources
		 *	In the long term: it will be the same
		 */
		KDvoid						removeAllTextures ( KDvoid );

		/** 
		 *	Removes unused textures
		 *	Textures that have a retain count of 1 will be deleted
		 *	It is convenient to call this method after when starting a new Scene
		 *	@since	v0.8
		 */
		KDvoid						removeUnusedTextures ( KDvoid );

		/** Deletes a texture from the cache given a texture */
		KDvoid						removeTexture ( CCTexture2D* pTexture );

		/** 
		 *	Deletes a texture from the cache given a its key name
		 *	@since	v0.99.4
		 */
		KDvoid						removeTextureForKey ( const KDchar* szKey );

		/** 
		 *	Output to CCLOG the current contents of this CCTextureCache
		 *	This will attempt to calculate the size of each texture, and the total texture memory in use
		 *
		 *	@since	v1.0
		 */
		KDvoid						dumpCachedTextureInfo ( KDvoid );

	private :

		KDvoid						addImageAsyncCallBack ( KDfloat fTime );
	
	protected :

		CCDictionary*				m_pTextures;
};

#if CC_ENABLE_CACHE_TEXTURE_DATA

class VolatileTexture
{
	typedef enum 
	{
        kInvalid	= 0 ,
		kImageFile		,
		kImageData		,
		kImage			,
		kString			,
	} ccCachedImageType;

	public :
				 VolatileTexture ( CCTexture2D* pTexture );
		virtual ~VolatileTexture ( KDvoid );

	public :

		static KDvoid				addImageTexture  ( CCTexture2D* pTexture, const KDchar* szFilePath );
		static KDvoid				addStringTexture ( CCTexture2D* pTexture, const KDchar* szText, const CCSize& tDimension, CCAlignment eAlign, const KDchar* szFontPath, KDfloat fFontSize );
		static KDvoid				addDataTexture	 ( CCTexture2D* pTexture, KDvoid* pData, CCTexture2DPixelFormat ePixelFormat, const CCSize& tContentSize );
		static KDvoid				addCCImage		 ( CCTexture2D* pTexture, CCImage* pImage );
		
		static KDvoid				setTexParameters ( CCTexture2D* pTexture, const ccTexParams& tTexParams );

		static KDvoid				removeTexture ( CCTexture2D* pTexture );
    
		static KDvoid				reloadAllTextures ( KDvoid );

	private :

		// find VolatileTexture by CCTexture2D*
		// if not found, create a new one
		static VolatileTexture*		findVolotileTexture ( CCTexture2D* pTexture );

	protected :

		ccTexParams					m_tTexParams;
		CCTexture2D*				m_pTexture;
		CCImage*					m_pImage;
		ccCachedImageType			m_eCashedImageType;

		KDvoid*						m_pTextureData;
		CCSize						m_tTextureSize;
		CCTexture2DPixelFormat		m_ePixelFormat;

		std::string					m_sFilePath;

		CCSize						m_tDimension;
		CCAlignment					m_eAlign;
		KDfloat						m_fFontSize;
		std::string					m_sText;
};

#endif // CC_ENABLE_CACHE_TEXTURE_DATA

// end of textures group
/// @}

NS_CC_END

#endif	// __CCTextureCache_h__

