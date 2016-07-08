/* --------------------------------------------------------------------------
 *
 *      File            CCImage.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010      cocos2d-x.org
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
 *      This m_pLibrary is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This m_pLibrary is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this m_pLibrary in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "support/CCImage.h"
#include "support/CCFileUtils.h"
#include "support/zip_support/ZipUtils.h"
#include "support/CCConfiguration.h"
#include "cocoa/CCString.h"
#include "ccConfig.h"

NS_CC_BEGIN

//////////////////////////////////////////////////////////////////////////
// Impliment CCImage
//////////////////////////////////////////////////////////////////////////

CCImage* CCImage::create ( KDvoid )
{
	CCImage*	pRet = new CCImage;

	if ( pRet )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_RELEASE ( pRet );
	}

	return pRet;
}

CCImage* CCImage::create ( const KDchar* szPath, CCTexture2DPixelFormat eFormat )
{
	CCImage*	pRet = new CCImage;

	if ( pRet && pRet->initWithFile ( szPath, eFormat ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_RELEASE ( pRet );
	}

	return pRet;
}

CCImage::CCImage ( KDvoid )
{
	m_eFormat    = kCCTexture2DPixelFormat_Automatic;
	m_uWidth	 = 0;
	m_uHeight	 = 0;
	m_pAtx		 = KD_NULL;
	m_uBpp		 = 0;
	m_pData		 = 0;
	m_uDataLen	 = 0;
	m_uLevel     = 0;
	m_bHasAlpha	 = KD_FALSE;
	m_bPreMulti	 = KD_FALSE;
	m_bKeepData  = KD_FALSE;
	m_bFlipY	 = KD_FALSE;
}

CCImage::~CCImage ( KDvoid )
{
	if ( m_pAtx )
	{
		kdFreeImageATX ( m_pAtx );
	}

	if ( m_bKeepData )
	{
		CC_SAFE_FREE ( m_pData );
	}
}

KDbool CCImage::initWithFile ( const KDchar* szPath, CCTexture2DPixelFormat eFormat )
{
	std::string  sPath  = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szPath ).c_str ( );
	KDbool		 bRet   = KD_FALSE;
	KDFile*      pFile  = KD_NULL;
	KDubyte*     pBuff  = KD_NULL;
	KDint		 nLen   = 0;
	
	if ( ( nLen = ZipUtils::ccInflateCCZFile ( sPath.c_str ( ), &pBuff ) ) >= 0 )
	{
		pFile = kdFmemopen ( pBuff, nLen,  "rb" );
	}
	else if ( ( nLen = ZipUtils::ccInflateGZipFile ( sPath.c_str ( ), &pBuff ) ) >= 0 )
	{
		pFile = kdFmemopen ( pBuff, nLen, "rb" );
	}
	else
	{
		pFile = kdFopen ( sPath.c_str ( ), "rb" );
	}

	if ( pFile )
	{
		bRet = initWithStream ( pFile, eFormat );
		kdFclose ( pFile );
	}

	CC_SAFE_FREE ( pBuff );

	return bRet;	
}

KDbool CCImage::initWithData ( KDvoid* pData, KDuint uDataLen, CCTexture2DPixelFormat eFormat )
{
	KDbool   bRet  = KD_FALSE;
	KDFile*  pFile = kdFmemopen ( pData, uDataLen, "rb" );

	if ( pFile )
	{
		bRet = initWithStream ( pFile, eFormat );

		kdFclose ( pFile );
	}

	return bRet;	
}

KDbool CCImage::initWithStream ( KDFile* pStream, CCTexture2DPixelFormat eFormat )
{
	CCAssert ( eFormat != kCCTexture2DPixelFormat_PVRTC, "" );
	
	KDoff		uOffset = kdFtell ( pStream );
	KDImageATX	pImage  = KD_NULL;
    
	if ( eFormat == kCCTexture2DPixelFormat_Automatic )
	{
		pImage = kdGetImageInfoFromStreamATX ( pStream );
		if ( !pImage )
		{
			return KD_FALSE;
		}

		KDint  nFormat = kdGetImageIntATX ( pImage, KD_IMAGE_FORMAT_ATX		  );
		KDint  nBpp    = kdGetImageIntATX ( pImage, KD_IMAGE_BITSPERPIXEL_ATX );
		KDint  nAlpha  = kdGetImageIntATX ( pImage, KD_IMAGE_ALPHA_ATX        );			

		kdFreeImageATX ( pImage );

		kdFseek ( pStream, uOffset, KD_SEEK_SET );

		switch ( nFormat )
		{
			case KD_IMAGE_FORMAT_COMPRESSED_ATX :
	            
				if ( !CCConfiguration::sharedConfiguration ( )->supportsPVRTC ( ) )
				{
					eFormat = kCCTexture2DPixelFormat_RGBA8888;
				}
				else
				{
					eFormat = kCCTexture2DPixelFormat_PVRTC;
				}				
	            
				break;
	            
			case KD_IMAGE_FORMAT_ALPHA8_ATX :

				eFormat = kCCTexture2DPixelFormat_A8;

				break;

			case KD_IMAGE_FORMAT_LUMINANCE_ATX :
	            
				eFormat = kCCTexture2DPixelFormat_I8;
	            
				break;
	            
			case KD_IMAGE_FORMAT_LUMALPHA_ATX :
	            
				eFormat = kCCTexture2DPixelFormat_AI88;
	            
				break;
	            
			case KD_IMAGE_FORMAT_PALETTED_ATX :
	            
				eFormat = kCCTexture2DPixelFormat_RGB5A1;
	            
				break;
	            
			case KD_IMAGE_FORMAT_RGB_ATX  : 
	            
				eFormat = ( nBpp == 24 ) ? kCCTexture2DPixelFormat_RGB888 : kCCTexture2DPixelFormat_RGB565;
	            
				break;
	            
			case KD_IMAGE_FORMAT_RGBA_ATX :
	            
				if ( nBpp == 16 )
				{
					eFormat = ( nAlpha == 4 ) ? kCCTexture2DPixelFormat_RGBA4444 : kCCTexture2DPixelFormat_RGB5A1;					
				}
				else
				{	
					eFormat = kCCTexture2DPixelFormat_RGBA8888;	
				}
	                	                
				break;		
	            
			default :
	            
				return KD_FALSE;
		}
	}
	
	pImage = kdGetImageFromStreamATX ( pStream, eFormat, KD_IMAGE_FLAG_PREMULTIPLIED_ALPHA ); 
	if ( !pImage )
	{
		return KD_FALSE;
	}

	m_eFormat   = eFormat;
	m_pAtx		= pImage;
	m_uWidth	= kdGetImageIntATX ( pImage, KD_IMAGE_WIDTH_ATX  );
	m_uHeight	= kdGetImageIntATX ( pImage, KD_IMAGE_HEIGHT_ATX );
	m_bFlipY	= kdGetImageIntATX ( pImage, KD_IMAGE_FLAG_FLIP_Y_ATX ) ? KD_TRUE : KD_FALSE;
	m_bPreMulti = kdGetImageIntATX ( pImage, KD_IMAGE_FLAG_PREMULTIPLIED_ALPHA ) ? KD_TRUE : KD_FALSE; 
	m_uBpp		= kdGetImageIntATX ( pImage, KD_IMAGE_BITSPERPIXEL_ATX );
	m_bHasAlpha = kdGetImageIntATX ( pImage, KD_IMAGE_ALPHA_ATX ) > 0 ? KD_TRUE : KD_FALSE;
	m_uDataLen  = kdGetImageIntATX ( pImage, KD_IMAGE_DATASIZE_ATX );
	m_uLevel    = kdGetImageIntATX ( pImage, KD_IMAGE_LEVELS_ATX );	
	
	m_pData     = (KDubyte*) kdGetImagePointerATX ( pImage, KD_IMAGE_POINTER_BUFFER_ATX );

	return KD_TRUE;
}

KDbool CCImage::initWithRawData ( KDvoid* pData, KDuint uDataLen, KDuint uWidth, KDuint uHeight, KDuint uBpp, KDbool bKeepData )
{
    KDbool  bRet = KD_FALSE;
	
    do 
    {
		CC_BREAK_IF ( !pData || uDataLen == 0 );
        CC_BREAK_IF ( 0 == uWidth || 0 == uHeight );

        m_uBpp		= uBpp;
        m_uWidth	= uWidth;
        m_uHeight	= uHeight;
        m_bHasAlpha	= KD_FALSE;
		m_uDataLen  = uDataLen;

		if ( m_uBpp == 32 )
		{
			m_eFormat = kCCTexture2DPixelFormat_RGBA8888;
		}
		else if ( m_uBpp == 16 )
		{
			m_eFormat = kCCTexture2DPixelFormat_RGB888;
		}
		else
		{
			return KD_FALSE;
		}

		if ( bKeepData )
		{
			m_pData = (KDubyte*) kdCalloc ( 1, uDataLen );

			CC_BREAK_IF ( !m_pData );
			kdMemcpy ( m_pData, pData, uDataLen );		

			m_bKeepData = KD_TRUE;
		}
		else
		{
			m_pData = (KDubyte*) pData;
			m_bKeepData = KD_FALSE;
		}

        bRet = KD_TRUE;

    } while ( 0 );
	
    return bRet;
}

KDbool CCImage::saveToFile ( const KDchar* szFilePath, KDbool bIsToRGB )
{
	if ( !szFilePath )
	{
		return KD_FALSE;
	}

	return kdSaveImageATX ( szFilePath, m_uWidth, m_uHeight, m_eFormat, m_pData ) == 0 ? KD_TRUE : KD_FALSE;
}

KDbool CCImage::initWithStringShadowStroke ( const KDchar*	szText			, 
											 KDuint			uWidth			, 
											 KDuint			uHeight			,
											 CCAlignment	eAlignMask		,
											 const KDchar*	szFontName		,
											 KDuint			uSize			,
											 KDfloat        fTextTintR		,
											 KDfloat        fTextTintG		,
											 KDfloat        fTextTintB		,
											 KDbool			bShadow			,
											 KDfloat		fShadowOffsetX	,
											 KDfloat		fShadowOffsetY	,
											 KDfloat		fShadowOpacity	,
											 KDfloat		fShadowBlur		,
											 KDbool			bStroke			,
											 KDfloat		fStrokeR		,
											 KDfloat		fStrokeG		,
											 KDfloat		fStrokeB		,
											 KDfloat		fStrokeSize		)
{
	KDFile*  pFile = kdFopen ( CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFontName ).c_str ( ), "r" );
	KDbool   bRet  = KD_FALSE;

	if ( pFile )
	{
		kdFclose ( pFile );
		bRet = CCImage::initWithFontFile ( szText, uWidth, uHeight, eAlignMask, szFontName, uSize );
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	else
	{
		bRet = CCImage::initWithFontName ( szText, uWidth, uHeight, eAlignMask, szFontName, uSize );
	}
#endif

	return bRet;
}

NS_CC_END
