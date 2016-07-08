/* -----------------------------------------------------------------------------------
 *
 *      File            CCImage.cpp
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

#define __CC_PLATFORM_IMAGE_CPP__
#include "platform/CCImage.h"
#include "platform/CCFileUtils.h"
#include "support/zip_support/ZipUtils.h"
#include "2d/CCConfiguration.h"

NS_CC_BEGIN

//////////////////////////////////////////////////////////////////////////
// Implement Image
//////////////////////////////////////////////////////////////////////////

Image* Image::create ( KDvoid )
{
	Image*	pRet = new Image ( );

	if ( pRet )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Image::Image()
: m_pData(0)
, m_nDataLen(0)
, m_nWidth(0)
, m_nHeight(0)
, m_eFileType(Format::UNKNOWN)
, m_eRenderFormat(Texture2D::PixelFormat::NONE)
, m_nNumberOfMipmaps(0)
, m_bHasPremultipliedAlpha(true)
{
	m_pImageAtx = KD_NULL;
}

Image::~Image()
{
	if (m_pImageAtx)
	{
		kdFreeImageATX(m_pImageAtx);
	}
	else
	{
		CC_SAFE_DELETE_ARRAY(m_pData);
	}
}

bool Image::initWithImageFile(const char * strPath)
{
	bool bRet = false;
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(strPath);

#ifdef EMSCRIPTEN
	// Emscripten includes a re-implementation of SDL that uses HTML5 canvas
	// operations underneath. Consequently, loading images via IMG_Load (an SDL
	// API) will be a lot faster than running libpng et al as compiled with
	// Emscripten.
	SDL_Surface *iSurf = IMG_Load(fullPath.c_str());

	int size = 4 * (iSurf->w * iSurf->h);
	bRet = initWithRawData((const unsigned char*)iSurf->pixels, size, iSurf->w, iSurf->h, 8, true);

	unsigned int *tmp = (unsigned int *)m_pData;
	int nrPixels = iSurf->w * iSurf->h;
	for (int i = 0; i < nrPixels; i++)
	{
		unsigned char *p = m_pData + i * 4;
		tmp[i] = CC_RGB_PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
	}

	SDL_FreeSurface(iSurf);
#else
	KDint32 bufferLen = 0;
	unsigned char* buffer = FileUtils::getInstance()->getFileData(fullPath.c_str(), "rb", &bufferLen);

	if (buffer != nullptr && bufferLen > 0)
	{
		bRet = initWithImageData(buffer, bufferLen);
	}

	kdFree(buffer);
#endif // EMSCRIPTEN

	return bRet;
}

bool Image::initWithImageFileThreadSafe(const char *fullpath)
{
	bool ret = false;
	KDint32 dataLen = 0;

	unsigned char *buffer = FileUtils::getInstance()->getFileData(fullpath, "rb", &dataLen);
	if (buffer != NULL && dataLen > 0)
	{
		ret = initWithImageData(buffer, dataLen);
	}
	kdFree(buffer);
	return ret;
}

KDbool Image::initWithImageData ( const KDubyte* pData, KDint32 nDataLen )
{
	KDbool	bRet = false;

	do
	{
		CC_BREAK_IF ( !pData || nDataLen <= 0 );

		KDubyte*	pUnpackedData = nullptr;
		KDint		nUnpackedLen = 0;

		// detecgt and unzip the compress file
		if ( ZipUtils::isCCZBuffer ( pData, nDataLen ) )
		{
			nUnpackedLen = ZipUtils::inflateCCZBuffer ( pData, nDataLen, &pUnpackedData );
		}
		else if ( ZipUtils::isGZipBuffer ( pData, nDataLen ) )
		{
			nUnpackedLen = ZipUtils::inflateMemory ( const_cast<KDubyte*> ( pData ), nDataLen, &pUnpackedData );
		}
		else
		{
			pUnpackedData = const_cast<KDubyte*> ( pData );
			nUnpackedLen = nDataLen;
		}

		KDFile*		pFile   = kdFmemopen ( pUnpackedData, nUnpackedLen, "rb" );
		KDoff		uOffset = kdFtell ( pFile );
		KDImageATX	pImage  = KD_NULL;

		do 
		{
			Texture2D::PixelFormat  eFormat = Texture2D::getDefaultAlphaPixelFormat ( );
			
			if ( eFormat == Texture2D::PixelFormat::AUTO )
			{			
				pImage = kdGetImageInfoFromStreamATX ( pFile );
				CC_BREAK_IF ( !pImage );

				KDint  nFormat = kdGetImageIntATX ( pImage, KD_IMAGE_FORMAT_ATX );
				KDint  nBpp    = kdGetImageIntATX ( pImage, KD_IMAGE_BITSPERPIXEL_ATX );
				KDint  nAlpha  = kdGetImageIntATX ( pImage, KD_IMAGE_ALPHA_ATX );
				KDint  nType   = kdGetImageIntATX ( pImage, KD_IMAGE_TYPE );

				m_eFileType = (Format) nType;
		
				kdFreeImageATX ( pImage );
				kdFseek ( pFile, uOffset, KD_SEEK_SET );

				switch ( nFormat )
				{
					case KD_IMAGE_FORMAT_COMPRESSED_ATX :	
						if ( ( nType == KD_IMAGE_TYPE_PVR   && !Configuration::getInstance ( )->supportsPVRTC ( ) ) ||
							 ( nType == KD_IMAGE_TYPE_S3TC  && !Configuration::getInstance ( )->supportsS3TC  ( ) ) ||
							 ( nType == KD_IMAGE_TYPE_ATITC && !Configuration::getInstance ( )->supportsATITC ( ) ) ||
							 ( nType == KD_IMAGE_TYPE_ETC   && !Configuration::getInstance ( )->supportsETC   ( ) ) )
						{
							eFormat = Texture2D::PixelFormat::RGBA8888;
						}
						else
						{
							eFormat = Texture2D::PixelFormat::COMPRESSED;
						}
						break;

					case KD_IMAGE_FORMAT_ALPHA8_ATX:
						eFormat = Texture2D::PixelFormat::A8;
						break;

					case KD_IMAGE_FORMAT_LUMINANCE_ATX:
						eFormat = Texture2D::PixelFormat::I8;
						break;

					case KD_IMAGE_FORMAT_LUMALPHA_ATX:
						eFormat = Texture2D::PixelFormat::AI88;
						break;

					case KD_IMAGE_FORMAT_PALETTED_ATX:
						eFormat = Texture2D::PixelFormat::RGB5A1;
						break;

					case KD_IMAGE_FORMAT_RGB_ATX:
						if ( nBpp == 24 || nBpp == 48 )
						{
							eFormat = Texture2D::PixelFormat::RGB888;
						}
						else
						{
							eFormat = Texture2D::PixelFormat::RGB565;
						}						
						break;

					case KD_IMAGE_FORMAT_RGBA_ATX:
						if ( nBpp == 16 )
						{
							eFormat = ( nAlpha == 4 ) ? Texture2D::PixelFormat::RGBA4444 : Texture2D::PixelFormat::RGB5A1;
						}
						else
						{
							eFormat = Texture2D::PixelFormat::RGBA8888;
						}
						break;

					default:
						break; break;
				}
			}
	
			pImage = kdGetImageFromStreamATX ( pFile, (KDint) eFormat, KD_IMAGE_FLAG_PREMULTIPLIED_ALPHA );
			CC_BREAK_IF ( !pImage );
			
			m_eRenderFormat				= (Texture2D::PixelFormat) kdGetImageIntATX ( pImage, KD_IMAGE_FORMAT_ATX );
			m_nWidth					= kdGetImageIntATX ( pImage, KD_IMAGE_WIDTH_ATX );
			m_nHeight					= kdGetImageIntATX ( pImage, KD_IMAGE_HEIGHT_ATX );
			m_bHasPremultipliedAlpha	= kdGetImageIntATX ( pImage, KD_IMAGE_FLAG_PREMULTIPLIED_ALPHA ) ? KD_TRUE : KD_FALSE;
			m_nDataLen					= kdGetImageIntATX ( pImage, KD_IMAGE_DATASIZE_ATX );
			m_nNumberOfMipmaps			= kdGetImageIntATX ( pImage, KD_IMAGE_LEVELS_ATX );
			m_pData						= (KDubyte*) kdGetImagePointerATX ( pImage, KD_IMAGE_POINTER_BUFFER_ATX );
			m_pImageAtx					= pImage;

			for ( KDint i = 0; i < m_nNumberOfMipmaps; i++ )
			{
				m_aMipmaps [ i ].address = m_pData + kdGetImageLevelIntATX ( pImage, KD_IMAGE_BUFFEROFFSET_ATX, i );
				m_aMipmaps [ i ].len     = kdGetImageLevelIntATX ( pImage, KD_IMAGE_DATASIZE_ATX, i );
			}

			bRet = true;

		} while ( 0 );

		if ( pFile )
		{
			kdFclose ( pFile );
		}
	
		if ( pUnpackedData != pData )
		{
			kdFree ( pUnpackedData );
		}

	} while ( 0 );

	return bRet;
}

KDbool Image::initWithRawData ( const KDubyte* pData, KDint32 nDataLen, KDint32 nWidth, KDint32 nHeight, KDint32 nBitsPerComponent, KDbool bPreMulti )
{
	KDbool		bRet = false;
	do
	{
		CC_BREAK_IF ( 0 == nWidth || 0 == nHeight );

		m_nHeight		= nHeight;
		m_nWidth		= nWidth;
		m_bPreMulti		= bPreMulti;
		m_bHasPremultipliedAlpha = bPreMulti;

		switch ( nBitsPerComponent )
		{
			case  8 :	m_eRenderFormat = Texture2D::PixelFormat::I8;			break;
			case 16 :	m_eRenderFormat = Texture2D::PixelFormat::AI88;			break;
			case 24 :	m_eRenderFormat = Texture2D::PixelFormat::RGB888;		break;
			case 32 :	m_eRenderFormat = Texture2D::PixelFormat::RGBA8888;		break;

			default :
				CCAssert ( false, "Not Support BitsPerComponent" );
		}

		// only RGBA8888 supported
		KDint			bytesPerComponent = nBitsPerComponent >> 3;
		m_nDataLen		= nHeight * nWidth * bytesPerComponent;
		m_pData			= new KDubyte [ m_nDataLen ];
		CC_BREAK_IF ( !m_pData );
		kdMemcpy ( m_pData, pData, m_nDataLen );

		bRet = true;
	} while ( 0 );

	return bRet;
}

int Image::getBitPerPixel()
{
	return Texture2D::getPixelFormatInfoMap().at(m_eRenderFormat).bpp;
}

bool Image::hasAlpha()
{
	return Texture2D::getPixelFormatInfoMap().at(m_eRenderFormat).alpha;
}

bool Image::isCompressed()
{
	return Texture2D::getPixelFormatInfoMap().at(m_eRenderFormat).compressed;
}

bool Image::saveToFile(const char *pszFilePath, bool bIsToRGB)
{
	//only support for Texture2D::PixelFormat::RGB888 or Texture2D::PixelFormat::RGBA8888 uncompressed data
	if (isCompressed() || (m_eRenderFormat != Texture2D::PixelFormat::RGB888 && m_eRenderFormat != Texture2D::PixelFormat::RGBA8888))
	{
		CCLOG("cocos2d: Image: saveToFile is only support for Texture2D::PixelFormat::RGB888 or Texture2D::PixelFormat::RGBA8888 uncompressed data for now");
		return false;
	}

	bool bRet = false;

	do
	{
		CC_BREAK_IF(NULL == pszFilePath);

		std::string strFilePath(pszFilePath);
		CC_BREAK_IF(strFilePath.size() <= 4);

		std::string strLowerCasePath(strFilePath);
		for (unsigned int i = 0; i < strLowerCasePath.length(); ++i)
		{
			strLowerCasePath[i] = tolower(strFilePath[i]);
		}

		if (std::string::npos != strLowerCasePath.find(".png"))
		{
			CC_BREAK_IF(!saveImageToPNG(pszFilePath, bIsToRGB));
		}
		else if (std::string::npos != strLowerCasePath.find(".jpg"))
		{
			CC_BREAK_IF(!saveImageToJPG(pszFilePath));
		}
		else
		{
			break;
		}

		bRet = true;
	} while (0);

	return bRet;
}

bool Image::saveImageToPNG(const char * filePath, bool isToRGB)
{
	bool bRet = false;

	return bRet;
}
bool Image::saveImageToJPG(const char * filePath)
{
	bool bRet = false;

	return bRet;
}

KDbool Image::initWithString ( const KDchar* pText, KDint nWidth, KDint nHeight, TextAlign eAlignMask, const KDchar* szFontName, KDint nSize )
{
	return this->initWithStringShadowStroke ( pText, nWidth, nHeight, eAlignMask, szFontName, nSize );
}

KDbool Image::initWithStringShadowStroke (	const KDchar*	szText,
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
											KDfloat			fStrokeSize )
{
	KDbool		bRet = KD_FALSE;

    KDFile*		pFile = kdFopen ( FileUtils::getInstance ( )->fullPathForFilename ( szFontName ).c_str( ), "r" );
	if ( pFile )
	{
		kdFclose ( pFile );
		bRet = this->initWithFreeTypeFont ( szText, nWidth, nHeight, eAlignMask, szFontName, nSize );
	}
	#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	else
	{
		bRet = this->initWithPlatformFont
		(
			szText, nWidth, nHeight, eAlignMask, szFontName, nSize, 
			fTextTintR, fTextTintG, fTextTintB, 
			bShadow, fShadowOffsetX, fShadowOffsetY, fShadowOpacity, fShadowBlur,
			bStroke, fStrokeR, fStrokeG, fStrokeB, fStrokeSize
		);
	}
	#endif

	return bRet;
}

NS_CC_END
