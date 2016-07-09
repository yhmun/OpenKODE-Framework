/* -----------------------------------------------------------------------------------
*
*      File            CCImage.h
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
* ----------------------------------------------------------------------------------- */

#ifndef __CCImage_h__
#define __CCImage_h__

#include "../base/CCObject.h"
#include "../2d/textures/CCTexture2D.h"

// premultiply alpha, or the effect will wrong when want to use other pixel format in Texture2D,
// such as RGB888, RGB5A1
#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 @brief Structure which can tell where mipmap begins and how long is it
 */
typedef struct _MipmapInfo
{
    KDubyte*		address;
    KDint			len;
} MipmapInfo;

class CC_DLL Image : public Object
{
	public :

		friend class TextureCache;
		
		/**
		 *	@js ctor
		 */
		Image ( KDvoid );

		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~Image ( KDvoid );

		// Added By XMSoft
		static Image*		create ( KDvoid );

	public :

		/** Supported formats for Image */
		enum class Format
		{
			//! JPEG
			JPG			= KD_IMAGE_TYPE_JPEG,
			//! PNG
			PNG			= KD_IMAGE_TYPE_PNG,
			//! TIFF
			TIFF		= KD_IMAGE_TYPE_TIFF,
			//! WebP
			WEBP		= KD_IMAGE_TYPE_WEBP,
			//! PVR
			PVR			= KD_IMAGE_TYPE_PVR,
			//! ETC
			ETC			= KD_IMAGE_TYPE_ETC,
			//! S3TC
			S3TC		= KD_IMAGE_TYPE_S3TC,
			//! ATITC
			ATITC		= KD_IMAGE_TYPE_ATITC,
			//! BITMAP
			BMP			= KD_IMAGE_TYPE_BMP,
			//! TARGA
			TGA			= KD_IMAGE_TYPE_TGA,
			//! Raw Data
			RAW_DATA,
			//! Unknown format
			UNKNOWN		= KD_IMAGE_TYPE_NULL,
		};

		enum class TextAlign
		{
			CENTER        = 0x33, ///< Horizontal center and vertical center.
			TOP           = 0x13, ///< Horizontal center and vertical top.
			TOP_RIGHT     = 0x12, ///< Horizontal right and vertical top.
			RIGHT         = 0x32, ///< Horizontal right and vertical center.
			BOTTOM_RIGHT  = 0x22, ///< Horizontal right and vertical bottom.
			BOTTOM        = 0x23, ///< Horizontal center and vertical bottom.
			BOTTOM_LEFT   = 0x21, ///< Horizontal left and vertical bottom.
			LEFT          = 0x31, ///< Horizontal left and vertical center.
			TOP_LEFT      = 0x11, ///< Horizontal left and vertical top.
		};
    
		/**
		 *	@brief Load the image from the specified path.
		 *	@param path   the absolute file path.
		 *	@return true if loaded correctly.
		 */
		KDbool							initWithImageFile ( const KDchar* pPath );

		/**
		 *	@brief Load image from stream buffer.
		 *	@param data  stream buffer which holds the image data.
		 *	@param dataLen  data length expressed in (number of) bytes.
		 *	@return true if loaded correctly.
		 *	@js NA
		 *	@lua NA
		 */
		KDbool							initWithImageData ( const KDubyte* pData, KDint32 nDataLen );

		// @warning kFmtRawData only support RGBA8888
		KDbool							initWithRawData ( const KDubyte* pData, KDint32 nDataLen, KDint32 nWidth, KDint32 nHeight, KDint32 nBitsPerComponent, KDbool bPreMulti = false );

		/**
		 *	@brief Create image with specified string.
		 *	@param text       the text the image will show (cannot be nil).
		 *	@param width      the image width, if 0, the width will match the text's width.
		 *	@param height     the image height, if 0, the height will match the text's height.
		 *	@param alignMask  the test Alignment
		 *	@param fontName   the name of the font used to draw the text. If nil, use the default system font.
		 *	@param size       the font size, if 0, use the system default size.
		 *	@js NA
		 *	@lua NA
		 */
		KDbool							initWithString				(	const KDchar*	pText,
																		KDint			nWidth			= 0,
																		KDint			nHeight			= 0,
																		TextAlign		eAlignMask		= TextAlign::CENTER,
																		const KDchar*	pFontName		= 0,
																		KDint			nSize			= 0	);
    
		KDbool							initWithStringShadowStroke	(	const KDchar*	pText,
																		KDint			nWidth			= 0,
																		KDint			nHeight			= 0,
																		TextAlign       eAlignMask		= TextAlign::CENTER,
																		const KDchar*	pFontName		= 0,
																		KDint			nSize			= 0,
																		KDfloat			fTextTintR		= 1,
																		KDfloat			fTextTintG		= 1,
																		KDfloat			fTextTintB		= 1,
																		KDbool			bShadow			= false,
																		KDfloat			fShadowOffsetX	= 0.0,
																		KDfloat			fShadowOffsetY	= 0.0,
																		KDfloat			fShadowOpacity	= 0.0,
																		KDfloat			fShadowBlur		= 0.0,
																		KDbool			bStroke			=  false,
																		KDfloat			fStrokeR		= 1,
																		KDfloat			fStrokeG		= 1,
																		KDfloat			fStrokeB		= 1,
																		KDfloat			fStrokeSize		= 1	);
    
    
		// Getters
		inline KDubyte*					getData					( KDvoid )	{ return m_pData; }
		inline KDint					getDataLen				( KDvoid )	{ return m_nDataLen; }
		inline Format					getFileType				( KDvoid )	{return m_eFileType; }
		inline Texture2D::PixelFormat	getRenderFormat			( KDvoid )	{ return m_eRenderFormat; }
		inline KDint					getWidth				( KDvoid )	{ return m_nWidth; }
		inline KDint					getHeight				( KDvoid )	{ return m_nHeight; }
		inline KDint					getNumberOfMipmaps		( KDvoid )	{ return m_nNumberOfMipmaps; }
		inline MipmapInfo*				getMipmaps				( KDvoid )	{ return m_aMipmaps; }
		inline KDbool					hasPremultipliedAlpha	( KDvoid )	{ return m_bHasPremultipliedAlpha; }

		KDint							getBitPerPixel			( KDvoid );
		KDbool							hasAlpha				( KDvoid );
		KDbool							isCompressed			( KDvoid );

		/**
		 *	@brief    Save Image data to the specified file, with specified format.
		 *	@param    filePath        the file's absolute path, including file suffix.
		 *	@param    isToRGB        whether the image is saved as RGB format.
		 */
		KDbool							saveToFile ( const KDchar* pFilePath, KDbool bIsToRGB = true );

	protected :

		KDbool							saveImageToPNG ( const KDchar* pFilePath, KDbool bIsToRGB = true );
		KDbool							saveImageToJPG ( const KDchar* pFilePath );
    
		KDbool							initWithFreeTypeFont (  const KDchar*	szText,
																KDint           nWidth,
																KDint           nHeight,
																TextAlign       eAlignMask,
																const KDchar*	szFontName,
																KDint			nSize );

		KDbool							initWithPlatformFont (	const KDchar*	szText,
																KDint           nWidth,
																KDint           nHeight,
																TextAlign       eAlignMask,
																const KDchar*	szFontName,
																KDint			nSize,
																KDfloat			fTextTintR,
																KDfloat			fTextTintG,
																KDfloat			fTextTintB,
																KDbool			bShadow,
																KDfloat			fShadowOffsetX,
																KDfloat			fShadowOffsetY,
																KDfloat			fShadowOpacity,
																KDfloat			fShadowBlur,
																KDbool			bStroke,
																KDfloat			fStrokeR,
																KDfloat			fStrokeG,
																KDfloat			fStrokeB,
																KDfloat			fStrokeSize );


	private :

		/**
		 *	@brief Determine how many mipmaps can we have.
		 *	Its same as define but it respects namespaces
		 */
		static const KDint				MIPMAP_MAX = 16;
		KDubyte*						m_pData;
		KDint							m_nDataLen;
		KDint							m_nWidth;
		KDint							m_nHeight;
		Format							m_eFileType;
		Texture2D::PixelFormat			m_eRenderFormat;
		KDbool							m_bPreMulti;
		MipmapInfo						m_aMipmaps [ MIPMAP_MAX ];		// pointer to mipmap images
		KDint							m_nNumberOfMipmaps;
		// false if we cann't auto detect the image is premultiplied or not.
		KDbool							m_bHasPremultipliedAlpha;

		KDImageATX						m_pImageAtx;

	private :

		// noncopyable
		Image ( const Image& rImg );
		Image&	operator= ( const Image& );
    
		/**
		 *	@brief The same result as with initWithImageFile, but thread safe. It is caused by
		 *	loadImage() in TextureCache.cpp.
		 *	@param fullpath  full path of the file.
		 *	@param imageType the type of image, currently only supporting two types.
		 *	@return  true if loaded correctly.
		 */
		KDbool							initWithImageFileThreadSafe ( const KDchar* pFullpath );
};

//	end of platform group
/// @}

NS_CC_END

#endif	// __CCImage_h__
