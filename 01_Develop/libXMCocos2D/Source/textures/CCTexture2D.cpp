/* -----------------------------------------------------------------------------------
 *
 *      File            CCTexture2D.cpp
 *      Author          Young-Hwan Mun
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008      Apple Inc. All Rights Reserved.
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
#include "textures/CCTexture2D.h"
#include "support/CCConfiguration.h"
#include "support/CCImage.h"
#include "support/CCFileUtils.h"
#include "support/CCDirector.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCShaderCache.h"
#include "support/ccUtils.h"

#if CC_ENABLE_CACHE_TEXTURE_DATA
#include "textures/CCTextureCache.h"
#endif

NS_CC_BEGIN

CCTexture2D* CCTexture2D::createWithImage ( CCImage* pImage )
{
	CCTexture2D*	pRet = new CCTexture2D ( );

	if ( pRet && pRet->initWithImage ( pImage ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_RELEASE ( pRet );
	}

	return pRet;
}

//CLASS IMPLEMENTATIONS:

// If the image has alpha, you can create RGBA8 (32-bit) or RGBA4 (16-bit) or RGB5A1 (16-bit)
// Default is: RGBA8888 (32-bit textures)
static CCTexture2DPixelFormat   l_eDefaultAlphaPixelFormat = kCCTexture2DPixelFormat_Default;

CCTexture2D::CCTexture2D ( KDvoid )
{
    m_uPixelsWide            = 0;
    m_uPixelsHigh            = 0;
    m_uName                  = 0;
    m_fMaxS                  = 0.0f;
    m_fMaxT                  = 0.0f;
    m_tContentSize           = CCSizeZero;
    m_bHasPremultipliedAlpha = KD_FALSE;
	m_bHasMipmaps			 = KD_FALSE;
	m_pShaderProgram		 = KD_NULL;
    m_ePixelFormat           = kCCTexture2DPixelFormat_Default;
}

CCTexture2D::~CCTexture2D ( KDvoid )
{
    CCLOGINFO ( "XMCocos2D : deallocing CCTexture2D = %p", this );
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::removeTexture ( this );
#endif

    CC_SAFE_RELEASE ( m_pShaderProgram );

	if ( m_uName )
	{
		ccGLDeleteTexture ( m_uName );
	}
}

CCTexture2DPixelFormat CCTexture2D::getPixelFormat ( KDvoid )
{
	return m_ePixelFormat;
}

KDuint CCTexture2D::getPixelsWide ( KDvoid )
{
	return m_uPixelsWide;
}

KDuint CCTexture2D::getPixelsHigh ( KDvoid )
{
	return m_uPixelsHigh;
}

GLuint CCTexture2D::getName ( KDvoid )
{
	return m_uName;
}

CCSize CCTexture2D::getContentSize ( KDvoid )
{
	KDfloat  fScale = CC_CONTENT_SCALE_FACTOR ( );
	return CCSize ( m_tContentSize.cx / fScale, m_tContentSize.cy / fScale );
}

const CCSize& CCTexture2D::getContentSizeInPixels ( KDvoid )
{
	return m_tContentSize;
}

GLfloat CCTexture2D::getMaxS ( KDvoid )
{
	return m_fMaxS;
}

KDvoid CCTexture2D::setMaxS ( GLfloat fMaxS )
{
	m_fMaxS = fMaxS;
}

GLfloat CCTexture2D::getMaxT ( KDvoid )
{
	return m_fMaxT;
}

KDvoid CCTexture2D::setMaxT ( GLfloat fMaxT )
{
	m_fMaxT = fMaxT;
}

CCGLProgram* CCTexture2D::getShaderProgram ( KDvoid )
{
    return m_pShaderProgram;
}

KDvoid CCTexture2D::setShaderProgram ( CCGLProgram* pShaderProgram )
{
    CC_SAFE_RETAIN ( pShaderProgram );
    CC_SAFE_RELEASE ( m_pShaderProgram );
    m_pShaderProgram = pShaderProgram;
}

KDvoid CCTexture2D::releaseData ( KDvoid* pData )
{
	kdFree ( pData );
}

KDvoid* CCTexture2D::keepData ( KDvoid* pData, KDuint uLength )
{
	CC_UNUSED_PARAM ( uLength );

	//The texture data mustn't be saved because it isn't a mutable texture.
	return pData;
}

KDbool CCTexture2D::hasPremultipliedAlpha ( KDvoid )
{
	return m_bHasPremultipliedAlpha;
}

KDbool CCTexture2D::initWithData ( const KDvoid* pData, CCTexture2DPixelFormat ePixelFormat, const CCSize& tContentSize, KDuint uMipmaps )
{
	CCConfiguration*  pConfig = CCConfiguration::sharedConfiguration ( );

	KDuint  uTexMaxSize = pConfig->getMaxTextureSize ( );
	KDuint  uPixelsWide = (KDuint) tContentSize.cx;
	KDuint	uPixelsHigh = (KDuint) tContentSize.cy;
	
    if ( uPixelsWide > uTexMaxSize || uPixelsHigh > uTexMaxSize ) 
    {
        CCLOG ( "XMCocos2D : WARNING: Image (%u x %u) is bigger than the supported %u x %u", uPixelsWide, uPixelsHigh, uTexMaxSize, uTexMaxSize );
        return KD_FALSE;
    }

    // XXX: 32 bits or POT textures uses UNPACK of 4 (is this correct ??? )
    if ( ePixelFormat == kCCTexture2DPixelFormat_RGBA8888 || ( ccNextPOT ( uPixelsWide ) == uPixelsWide && ccNextPOT ( uPixelsHigh )== uPixelsHigh ) )
    {
        glPixelStorei ( GL_UNPACK_ALIGNMENT, 4 );
    }
    else
    {
        glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
    }

    glGenTextures ( 1, &m_uName );
    ccGLBindTexture2D ( m_uName );

    // Specify OpenGL texture image
	GLenum	nTexType   = 0;
	GLenum	nTexFormat = 0;
	GLint   nTexBytes  = 0;

	switch ( ePixelFormat )
	{
		case kCCTexture2DPixelFormat_RGBA8888 : nTexBytes = 4; nTexFormat = GL_RGBA;            nTexType = GL_UNSIGNED_BYTE;          break;
		case kCCTexture2DPixelFormat_RGB888   : nTexBytes = 3; nTexFormat = GL_RGB;             nTexType = GL_UNSIGNED_BYTE;          break;
		case kCCTexture2DPixelFormat_RGBA4444 : nTexBytes = 2; nTexFormat = GL_RGBA;            nTexType = GL_UNSIGNED_SHORT_4_4_4_4; break;
		case kCCTexture2DPixelFormat_RGB5A1   : nTexBytes = 2; nTexFormat = GL_RGBA;            nTexType = GL_UNSIGNED_SHORT_5_5_5_1; break;
		case kCCTexture2DPixelFormat_RGB565   : nTexBytes = 2; nTexFormat = GL_RGB;             nTexType = GL_UNSIGNED_SHORT_5_6_5;   break;
		case kCCTexture2DPixelFormat_A8       : nTexBytes = 1; nTexFormat = GL_ALPHA;           nTexType = GL_UNSIGNED_BYTE;          break;
		case kCCTexture2DPixelFormat_I8       : nTexBytes = 1; nTexFormat = GL_LUMINANCE;       nTexType = GL_UNSIGNED_BYTE;          break;
		case kCCTexture2DPixelFormat_AI88     : nTexBytes = 2; nTexFormat = GL_LUMINANCE_ALPHA; nTexType = GL_UNSIGNED_BYTE;          break;		

		default :

			CCAssert ( 0, "NSInternalInconsistencyException" );           
	}

	if ( uMipmaps > 1 )
	{
		KDubyte*	pBuffer  = (KDubyte*) pData;
		KDuint		uPOTWide = ccNextPOT ( uPixelsWide );
		KDuint		uPOTHigh = ccNextPOT ( uPixelsHigh );
		KDuint		uOffset  = 0;

		for ( KDuint i = 0; i < uMipmaps; i++ )
		{
			glTexImage2D ( GL_TEXTURE_2D, i, nTexFormat, uPOTWide, uPOTHigh, 0, nTexFormat, nTexType, &pBuffer [ uOffset ] );

			uOffset += ( uPOTWide < 2 ? 2 : uPOTWide ) * ( uPOTHigh < 2 ? 2 : uPOTHigh ) * nTexBytes;

			uPOTWide = KD_MAX ( uPOTWide >> 1, 1 );
			uPOTHigh = KD_MAX ( uPOTHigh >> 1, 1 );
		}
	}
	else if ( pConfig->supportsNPOT ( ) )
	{
		glTexImage2D ( GL_TEXTURE_2D, 0, nTexFormat, uPixelsWide, uPixelsHigh, 0, nTexFormat, nTexType, pData );
	}
	else
	{		
		KDuint   uPOTWide = ccNextPOT ( uPixelsWide );
		KDuint   uPOTHigh = ccNextPOT ( uPixelsHigh );
		KDvoid*  pTemp    = KD_NULL;

		if ( uPOTWide == uPixelsWide && uPOTHigh == uPixelsHigh )
		{
			if ( !pData )
			{
				pTemp = kdCalloc ( 1, uPOTWide * uPOTHigh * nTexBytes );
				pData = pTemp;
			}

			glTexImage2D ( GL_TEXTURE_2D, 0, nTexFormat, uPixelsWide, uPixelsHigh, 0, nTexFormat, nTexType, pData );
		}
		else
		{
			pTemp = kdCalloc ( 1, uPOTWide * uPOTHigh * nTexBytes );
			glTexImage2D ( GL_TEXTURE_2D, 0, nTexFormat, uPOTWide, uPOTHigh, 0, nTexFormat, nTexType, pTemp );

			if ( pData )
			{
				glTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, uPixelsWide, uPixelsHigh, nTexFormat, nTexType, pData );
			}
			
			uPixelsWide = uPOTWide;
			uPixelsHigh = uPOTHigh;
		}

		CC_SAFE_FREE ( pTemp );
	}

    m_tContentSize	= tContentSize;
    m_uPixelsWide	= uPixelsWide;
    m_uPixelsHigh	= uPixelsHigh;
    m_ePixelFormat	= ePixelFormat;
    m_fMaxS			= tContentSize.cx / (KDfloat) ( uPixelsWide );
    m_fMaxT			= tContentSize.cy / (KDfloat) ( uPixelsHigh );
	m_bHasMipmaps	= uMipmaps > 1 ? KD_TRUE : KD_FALSE;

    
	ccTexParams  texParams = { GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
    if ( m_bHasMipmaps )
    {
        texParams.minFilter = GL_LINEAR_MIPMAP_NEAREST;
    }
    this->setTexParameters ( texParams );

    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTexture ) );

    return KD_TRUE;
}

const KDchar* CCTexture2D::description ( KDvoid )
{
    return CCString::createWithFormat 
	(
		"<CCTexture2D | Name = %u | Dimensions = %u x %u | Coordinates = (%.2f, %.2f)>",
		m_uName, m_uPixelsWide, m_uPixelsHigh, m_fMaxS, m_fMaxT
	)->getCString ( );
}

// implementation CCTexture2D (Image)

KDbool CCTexture2D::initWithImage ( CCImage* pImage )
{
    if ( pImage == KD_NULL )
    {
        CCLOG ( "XMCocos2D : CCTexture2D. Can't create Texture. UIImage is nil" );
        this->release ( );
        return KD_FALSE;
    }
    
    KDuint    uCX     = pImage->getWidth  ( );
    KDuint    uCY     = pImage->getHeight ( );
	KDubyte*  pData   = pImage->getData   ( );
	CCTexture2DPixelFormat  eFormat = pImage->getFormat ( );

	m_bHasPremultipliedAlpha = pImage->isPremultipliedAlpha ( );
	m_bHasFlipY = pImage->isFlipY ( );

	if ( eFormat == kCCTexture2DPixelFormat_PVRTC )
	{		
		return initWithPVRTCData ( pData, pImage->getLevel ( ), pImage->getBitsPerComponent ( ), pImage->hasAlpha ( ), uCX, uCY );
	}
	else
	{
		return initWithData ( pData, eFormat, CCSizeMake ( uCX, uCY ), pImage->getLevel ( ) );
	}
}

// implementation CCTexture2D (Text)
KDbool CCTexture2D::initWithString ( const KDchar* szText, const KDchar* szFontPath, KDfloat fFontSize )
{
	return initWithString ( szText, szFontPath, fFontSize, CCSize ( 0, 0 ), kCCAlignmentTop );
}

KDbool CCTexture2D::initWithString ( const KDchar* szText, const KDchar* szFontPath, KDfloat fFontSize, const CCSize& tDimension, CCTextAlignment eAlignH, CCVerticalTextAlignment eAlignV )
{
	ccFontDefinition	tTempDef;

	tTempDef.m_tShadow.m_bShadowEnabled = KD_FALSE;
	tTempDef.m_tStroke.m_bStrokeEnabled = KD_FALSE;

	tTempDef.m_sFontName				= szFontPath;
	tTempDef.m_nFontSize				= (KDint) fFontSize;
	tTempDef.m_tDimensions				= tDimension;
	tTempDef.m_eAlignment				= eAlignH;
	tTempDef.m_eVertAlignment			= eAlignV;
	tTempDef.m_tFontFillColor			= ccWHITE;

	return initWithString ( szText, &tTempDef );
}

KDbool CCTexture2D::initWithString ( const KDchar* szText, const KDchar* szFontPath, KDfloat fFontSize, const CCSize& tDimension, CCAlignment eAlign )
{
	return initWithString ( szText, szFontPath, fFontSize, tDimension, (CCTextAlignment) ( eAlign & 0x0F ), (CCVerticalTextAlignment) ( eAlign & 0xF0 ) );
}

KDbool CCTexture2D::initWithString ( const KDchar* szText, ccFontDefinition* pTextDefinition )
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
	// cache the texture data
	VolatileTexture::addStringTexture ( this, szText, pTextDefinition->m_tDimensions, (CCAlignment) ( pTextDefinition->m_eAlignment | pTextDefinition->m_eVertAlignment ), pTextDefinition->m_sFontName.c_str ( ), pTextDefinition->m_nFontSize );
#endif

	KDbool		bRet = KD_FALSE;

	// handle shadow parameters
	KDbool		bShadowEnabled = KD_FALSE;
	KDfloat		fShadowDX      = 0.0f;
	KDfloat		fShadowDY      = 0.0f;
	KDfloat		fShadowBlur    = 0.0f;
	KDfloat		fShadowOpacity = 0.0f;

	if ( pTextDefinition->m_tShadow.m_bShadowEnabled )
	{
		bShadowEnabled = KD_TRUE;
		fShadowDX      = pTextDefinition->m_tShadow.m_tShadowOffset.cx;
		fShadowDY      = pTextDefinition->m_tShadow.m_tShadowOffset.cy;
		fShadowBlur    = pTextDefinition->m_tShadow.m_fShadowBlur;
		fShadowOpacity = pTextDefinition->m_tShadow.m_fShadowOpacity;
	}

	// handle stroke parameters
	KDbool		bStrokeEnabled = KD_FALSE;
	KDfloat		fStrokeColorR  = 0.0f;
	KDfloat		fStrokeColorG  = 0.0f;
	KDfloat		fStrokeColorB  = 0.0f;
	KDfloat		fStrokeSize    = 0.0f;

	if ( pTextDefinition->m_tStroke.m_bStrokeEnabled )
	{
		bStrokeEnabled = KD_TRUE;
		fStrokeColorR  = pTextDefinition->m_tStroke.m_tStrokeColor.r / 255.0f;
		fStrokeColorG  = pTextDefinition->m_tStroke.m_tStrokeColor.g / 255.0f;
		fStrokeColorB  = pTextDefinition->m_tStroke.m_tStrokeColor.b / 255.0f;
		fStrokeSize    = pTextDefinition->m_tStroke.m_fStrokeSize;
	}

	CCImage*	pImage = CCImage::create ( );
	do
	{
		CC_BREAK_IF ( KD_NULL == pImage );

		bRet = pImage->initWithStringShadowStroke
		(
			szText,
			(KDint) pTextDefinition->m_tDimensions.width,
			(KDint) pTextDefinition->m_tDimensions.height,
			(CCAlignment) ( pTextDefinition->m_eAlignment | pTextDefinition->m_eVertAlignment ),
			pTextDefinition->m_sFontName.c_str ( ),
			pTextDefinition->m_nFontSize,
			pTextDefinition->m_tFontFillColor.r / 255.0f,
			pTextDefinition->m_tFontFillColor.g / 255.0f,
			pTextDefinition->m_tFontFillColor.b / 255.0f,
			bShadowEnabled,
			fShadowDX,
			fShadowDY,
			fShadowOpacity,
			fShadowBlur,
			bStrokeEnabled,
			fStrokeColorR,
			fStrokeColorG,
			fStrokeColorB,
			fStrokeSize 
		);

		CC_BREAK_IF ( !bRet );
		bRet = this->initWithImage ( pImage );

	} while ( 0 );

	return bRet;
}

// implementation CCTexture2D (Drawing)
KDvoid CCTexture2D::drawAtPoint ( const CCPoint& tPoint )
{  
   	GLfloat  pCoordinates [ ] = 
	{	
           0.0f, m_fMaxT,
		m_fMaxS, m_fMaxT,
           0.0f,    0.0f,
		m_fMaxS,    0.0f, 
	};

	GLfloat  fCX = (GLfloat) m_uPixelsWide * m_fMaxS;
    GLfloat  fCY = (GLfloat) m_uPixelsHigh * m_fMaxT;

	GLfloat  pVertices [ ] = 
	{	
		tPoint.x   	  , tPoint.y      ,  
		tPoint.x + fCX, tPoint.y	  ,  
		tPoint.x	  , tPoint.y + fCY,  
		tPoint.x + fCX, tPoint.y + fCY,  
	};

    m_pShaderProgram->use ( );
    m_pShaderProgram->setUniformsForBuiltins ( );

    ccGLBindTexture2D ( m_uName );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );

#ifdef EMSCRIPTEN
	setGLBufferData ( pVertices, 8 * sizeof ( GLfloat ), 0 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT, GL_FALSE, 0, 0 );

	setGLBufferData ( pCoordinates, 8 * sizeof ( GLfloat ), 1 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else
    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT, GL_FALSE, 0, pVertices );
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, pCoordinates );
#endif // EMSCRIPTEN

    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
}

KDvoid CCTexture2D::drawInRect ( const CCRect& tRect )
{
  	GLfloat  pCoordinates [ ] = 
	{	
           0.0f, m_fMaxT,
		m_fMaxS, m_fMaxT,
           0.0f,    0.0f,
		m_fMaxS,    0.0f, 
	};    

	GLfloat  pVertices [ ] = 
	{
		tRect.origin.x					,  tRect.origin.y				 ,
		tRect.origin.x + tRect.size.cx	,  tRect.origin.y				 ,						
		tRect.origin.x					,  tRect.origin.y + tRect.size.cy,		
		tRect.origin.x + tRect.size.cx	,  tRect.origin.y + tRect.size.cy,		
	};
    
    m_pShaderProgram->use ( );
    m_pShaderProgram->setUniformsForBuiltins ( );

    ccGLBindTexture2D ( m_uName );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );

#ifdef EMSCRIPTEN
	setGLBufferData ( pVertices, 8 * sizeof ( GLfloat ), 0 );
	glVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	setGLBufferData ( pCoordinates, 8 * sizeof ( GLfloat ), 1);
	glVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0 );
#else	
    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT, GL_FALSE, 0, pVertices );
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, pCoordinates );
#endif	// EMSCRIPTEN

    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
}

KDbool CCTexture2D::initWithPVRTCData ( const KDvoid* pData, KDint nLevel, KDint nBpp, KDbool bHasAlpha, KDuint uPOTWide, KDuint uPOTHigh )
{
	if( !( CCConfiguration::sharedConfiguration ( )->supportsPVRTC ( ) ) )
    {
        CCLOG ( "XMCocos2D : WARNING: PVRTC images is not supported." );
        this->release ( );
        return KD_FALSE;
    }

	GLubyte*	pBuffer = (GLubyte*) pData;
	GLenum		nFormat = 0;
	GLint		nOffset = 0;    
	GLsizei		nSize	= 0; 

	KDuint		uBlockSize;
	KDuint		uBlockWide;
	KDuint		uBlockHigh;

	glGenTextures ( 1, &m_uName );
    ccGLBindTexture2D ( m_uName );

	switch ( nBpp )
	{
		case 2 :
			nFormat = bHasAlpha ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
			uBlockSize = 8 * 4;
			break;

		case 4 :
			nFormat = bHasAlpha ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
			uBlockSize = 4 * 4;
			break;

		default :
			uBlockSize = 1;
			break;
	}

    m_tContentSize = CCSizeMake ( uPOTWide, uPOTHigh );
    m_uPixelsWide  = uPOTWide;
    m_uPixelsHigh  = uPOTHigh;
    m_fMaxS        = 1.0f;
    m_fMaxT        = 1.0f;
    m_ePixelFormat = kCCTexture2DPixelFormat_PVRTC;
	m_bHasMipmaps  = nLevel > 1 ? KD_TRUE : KD_FALSE;

	for ( KDint i = 0; i < nLevel; i++ )
	{
		switch ( nBpp )
		{
			case  2 :	uBlockWide = uPOTWide / 8;	uBlockHigh = uPOTHigh / 4;	break;
			case  4 :	uBlockWide = uPOTWide / 4;	uBlockHigh = uPOTHigh / 4;	break;
			default :	uBlockWide = uPOTWide;		uBlockHigh = uPOTHigh;
		}
		
		if ( uBlockWide < 2 )	uBlockWide = 2;
		if ( uBlockHigh < 2 )	uBlockHigh = 2;

		nSize = uBlockWide * uBlockHigh * ( ( uBlockSize * nBpp ) / 8 );
		glCompressedTexImage2D ( GL_TEXTURE_2D, i, nFormat, uPOTWide, uPOTHigh, 0, nSize, &pBuffer [ nOffset ] );

		nOffset += nSize;
        uPOTWide = KD_MAX ( uPOTWide / 2, 1 );
        uPOTHigh = KD_MAX ( uPOTHigh / 2, 1 );
	}

   	ccTexParams  texParams = { GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
    if ( m_bHasMipmaps )
    {
        texParams.minFilter = GL_LINEAR_MIPMAP_NEAREST;
    }
    this->setTexParameters ( texParams );

	return KD_TRUE;
}

KDvoid CCTexture2D::generateMipmap ( KDvoid )
{
	CCAssert ( m_uPixelsWide == ccNextPOT ( m_uPixelsWide ) && m_uPixelsHigh == ccNextPOT ( m_uPixelsHigh ), "Mipmap texture only works in POT textures" );
    
	ccGLBindTexture2D  ( this->m_uName );
	ccGLGenerateMipmap ( GL_TEXTURE_2D );
	m_bHasMipmaps = KD_TRUE;
}

KDbool CCTexture2D::hasMipmaps ( KDvoid )
{
    return m_bHasMipmaps;
}

KDbool CCTexture2D::hasFlipY ( KDvoid )
{
	return m_bHasFlipY;
}

KDvoid CCTexture2D::setTexParameters ( const ccTexParams& tTexParams )
{
	/*
	CCAssert ( ( m_uPixelsWide == ccNextPOT ( m_uPixelsWide ) && m_uPixelsHigh == ccNextPOT ( m_uPixelsHigh ) )
		|| ( tTexParams.wrapS == GL_CLAMP_TO_EDGE && tTexParams.wrapT == GL_CLAMP_TO_EDGE ),
        "GL_CLAMP_TO_EDGE should be used in NPOT textures" );
	*/

	ccGLBindTexture2D ( this->m_uName );

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tTexParams.minFilter );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tTexParams.magFilter );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tTexParams.wrapS );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tTexParams.wrapT );

#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTexture::setTexParameters ( this, tTexParams );
#endif
}

KDvoid CCTexture2D::setTexParameters ( const ccTexParams* pTexParams )
{
	this->setTexParameters ( *pTexParams );
}

KDvoid CCTexture2D::setAliasTexParameters ( KDvoid )
{
    ccGLBindTexture2D ( m_uName );

    if ( !m_bHasMipmaps )
    {
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    }
    else
    {
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
    }

    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

#if CC_ENABLE_CACHE_TEXTURE_DATA
    ccTexParams  tTexParams = { m_bHasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST, GL_NEAREST, 0, 0 };
    VolatileTexture::setTexParameters ( this, tTexParams );
#endif
}

KDvoid CCTexture2D::setAntiAliasTexParameters ( KDvoid )
{
    ccGLBindTexture2D ( m_uName );

    if ( !m_bHasMipmaps )
    {
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    }

    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

#if CC_ENABLE_CACHE_TEXTURE_DATA
    ccTexParams  tTexParams = { m_bHasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR, GL_LINEAR, 0 , 0 };
    VolatileTexture::setTexParameters ( this, tTexParams );
#endif
}

const KDchar* CCTexture2D::stringForFormat ( KDvoid )
{
	switch ( m_ePixelFormat ) 
	{
		case kCCTexture2DPixelFormat_RGBA8888	:	return  "RGBA8888";
		case kCCTexture2DPixelFormat_RGB888		:	return  "RGB888";
		case kCCTexture2DPixelFormat_RGB565		:	return  "RGB565";
		case kCCTexture2DPixelFormat_RGBA4444	:	return  "RGBA4444";
		case kCCTexture2DPixelFormat_RGB5A1		:	return  "RGB5A1";
		case kCCTexture2DPixelFormat_AI88		:	return  "AI88";
		case kCCTexture2DPixelFormat_A8			:	return  "A8";
		case kCCTexture2DPixelFormat_I8			:	return  "I8";
		case kCCTexture2DPixelFormat_PVRTC		:	return  "PVRTC";
		default :
			CCAssert ( KD_FALSE , "unrecognized pixel format" );
			CCLOG ( "stringForFormat: %ld, cannot give useful result", (KDint32) m_ePixelFormat );
			break;
	}

	return KD_NULL;
}

KDvoid CCTexture2D::setDefaultAlphaPixelFormat ( CCTexture2DPixelFormat ePixelFormat )
{
	l_eDefaultAlphaPixelFormat = ePixelFormat;
}

CCTexture2DPixelFormat CCTexture2D::defaultAlphaPixelFormat ( KDvoid )
{
	return l_eDefaultAlphaPixelFormat;
}

KDuint CCTexture2D::bitsPerPixelForFormat ( CCTexture2DPixelFormat eFormat )
{
	KDuint  uRet = 0;
    
	switch ( eFormat ) 
	{
		case kCCTexture2DPixelFormat_RGBA8888 :	uRet = 32; break;
		case kCCTexture2DPixelFormat_RGB888   : uRet = 24; break;
		case kCCTexture2DPixelFormat_RGB565   :	uRet = 16; break;
		case kCCTexture2DPixelFormat_RGBA4444 : uRet = 16; break;
		case kCCTexture2DPixelFormat_RGB5A1   :	uRet = 16; break;
		case kCCTexture2DPixelFormat_A8       :	uRet =  8; break;
		case kCCTexture2DPixelFormat_I8       :	uRet =  8; break;
		case kCCTexture2DPixelFormat_AI88     :	uRet = 16; break;
		case kCCTexture2DPixelFormat_PVRTC    : uRet =  0; break;	
            
        default :
            uRet = -1;
            CCAssert ( KD_FALSE, "unrecognized pixel format" );
            CCLOG ( "bitsPerPixelForFormat: %d, cannot give useful result", m_ePixelFormat );
            break;
	}
    
	return uRet;
}

KDuint CCTexture2D::bitsPerPixelForFormat ( KDvoid )
{
	return this->bitsPerPixelForFormat ( m_ePixelFormat );
}

NS_CC_END