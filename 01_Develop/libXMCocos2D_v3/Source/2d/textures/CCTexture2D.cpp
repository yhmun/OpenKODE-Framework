/* -----------------------------------------------------------------------------------
 *
 *      File            CCTexture2D.cpp
 *      Author          Young-Hwan Mun
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

/*
* Support for RGBA_4_4_4_4 and RGBA_5_5_5_1 was copied from:
* https://devforums.apple.com/message/37855#37855 by a1studmuffin
*/

#include "2d/textures/CCTexture2D.h"
#include "ccConfig.h"
#include "2d/CCConfiguration.h"
#include "platform/CCImage.h"
#include "support/ccUtils.h"
#include "2d/CCDirector.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCShaderCache.h"

#if CC_ENABLE_CACHE_TEXTURE_DATA
    #include "2d/textures/CCTextureCache.h"
#endif

NS_CC_BEGIN

namespace 
{
    typedef Texture2D::PixelFormatInfoMap::value_type PixelFormatInfoMapValue;
    static const PixelFormatInfoMapValue TexturePixelFormatInfoTablesValue [ ] =
    {
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::BGRA8888				, Texture2D::PixelFormatInfo ( GL_RGBA								, GL_BGRA			, GL_UNSIGNED_BYTE			, 32, false, true	) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::RGBA8888				, Texture2D::PixelFormatInfo ( GL_RGBA								, GL_RGBA			, GL_UNSIGNED_BYTE			, 32, false, true	) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::RGBA4444				, Texture2D::PixelFormatInfo ( GL_RGBA								, GL_RGBA			, GL_UNSIGNED_SHORT_4_4_4_4	, 16, false, true	) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::RGB5A1				, Texture2D::PixelFormatInfo ( GL_RGBA								, GL_RGBA			, GL_UNSIGNED_SHORT_5_5_5_1	, 16, false, true	) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::RGB565				, Texture2D::PixelFormatInfo ( GL_RGB								, GL_RGB			, GL_UNSIGNED_SHORT_5_6_5	, 16, false, false	) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::RGB888				, Texture2D::PixelFormatInfo ( GL_RGB								, GL_RGB			, GL_UNSIGNED_BYTE			, 24, false, false	) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::A8					, Texture2D::PixelFormatInfo ( GL_ALPHA								, GL_ALPHA			, GL_UNSIGNED_BYTE			,  8, false, false	) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::I8					, Texture2D::PixelFormatInfo ( GL_LUMINANCE							, GL_LUMINANCE		, GL_UNSIGNED_BYTE			,  8, false, false	) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::AI88					, Texture2D::PixelFormatInfo ( GL_LUMINANCE_ALPHA					, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE			, 16, false, true	) ),
		
#ifdef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::PVRTC2				, Texture2D::PixelFormatInfo ( GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG	, 0xFFFFFFFF		, 0xFFFFFFFF				,  2, true , false ) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::PVRTC2A				, Texture2D::PixelFormatInfo ( GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG	, 0xFFFFFFFF		, 0xFFFFFFFF				,  2, true , true  ) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::PVRTC4				, Texture2D::PixelFormatInfo ( GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG	, 0xFFFFFFFF		, 0xFFFFFFFF				,  4, true , false ) ),
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::PVRTC4A				, Texture2D::PixelFormatInfo ( GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG	, 0xFFFFFFFF		, 0xFFFFFFFF				,  4, true , true  ) ),
#endif
        
#ifdef GL_ETC1_RGB8_OES
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::ETC					, Texture2D::PixelFormatInfo ( GL_ETC1_RGB8_OES						, 0xFFFFFFFF		, 0xFFFFFFFF				, 24, true , false ) ),
#endif
        
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::S3TC_DXT1				, Texture2D::PixelFormatInfo ( GL_COMPRESSED_RGBA_S3TC_DXT1_EXT		, 0xFFFFFFFF		, 0xFFFFFFFF				,  4, true , false ) ),
#endif
        
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::S3TC_DXT3				, Texture2D::PixelFormatInfo ( GL_COMPRESSED_RGBA_S3TC_DXT3_EXT		, 0xFFFFFFFF		, 0xFFFFFFFF				,  8, true , false ) ),
#endif
        
#ifdef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::S3TC_DXT5				, Texture2D::PixelFormatInfo ( GL_COMPRESSED_RGBA_S3TC_DXT5_EXT		, 0xFFFFFFFF		, 0xFFFFFFFF				,  8, true , false ) ),
#endif
        
#ifdef GL_ATC_RGB_AMD
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::ATC_RGB				, Texture2D::PixelFormatInfo ( GL_ATC_RGB_AMD						, 0xFFFFFFFF		, 0xFFFFFFFF				,  4, true , false ) ),
#endif
        
#ifdef GL_ATC_RGBA_EXPLICIT_ALPHA_AMD
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::ATC_EXPLICIT_ALPHA	, Texture2D::PixelFormatInfo ( GL_ATC_RGBA_EXPLICIT_ALPHA_AMD		, 0xFFFFFFFF		, 0xFFFFFFFF				,  8, true , false ) ),
#endif
        
#ifdef GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD
        PixelFormatInfoMapValue ( Texture2D::PixelFormat::ATC_INTERPOLATED_ALPHA, Texture2D::PixelFormatInfo ( GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD	, 0xFFFFFFFF		, 0xFFFFFFFF				,  8, true , false ) ),
#endif
    };
}

//CLASS IMPLEMENTATIONS:

//The PixpelFormat corresponding information
const Texture2D::PixelFormatInfoMap Texture2D::m_aPixelFormatInfoTables(TexturePixelFormatInfoTablesValue,
                                                                     TexturePixelFormatInfoTablesValue + sizeof(TexturePixelFormatInfoTablesValue) / sizeof(TexturePixelFormatInfoTablesValue[0]));

// If the image has alpha, you can create RGBA8 (32-bit) or RGBA4 (16-bit) or RGB5A1 (16-bit)
// Default is: RGBA8888 (32-bit textures)
static Texture2D::PixelFormat	g_defaultAlphaPixelFormat = Texture2D::PixelFormat::AUTO;

static bool _PVRHaveAlphaPremultiplied = false;

//////////////////////////////////////////////////////////////////////////
//conventer function

// IIIIIIII -> RRRRRRRRGGGGGGGGGBBBBBBBB
void Texture2D::convertI8ToRGB888(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i=0; i < dataLen; ++i)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBB
void Texture2D::convertAI88ToRGB888(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
    }
}

// IIIIIIII -> RRRRRRRRGGGGGGGGGBBBBBBBBAAAAAAAA
void Texture2D::convertI8ToRGBA8888(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0; i < dataLen; ++i)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
        *outData++ = 0xFF;        //A
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA
void Texture2D::convertAI88ToRGBA8888(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i];     //R
        *outData++ = data[i];     //G
        *outData++ = data[i];     //B
        *outData++ = data[i + 1]; //A
    }
}

// IIIIIIII -> RRRRRGGGGGGBBBBB
void Texture2D::convertI8ToRGB565(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i] & 0x00FC) << 3         //G
            | (data[i] & 0x00F8) >> 3;        //B
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRGGGGGGBBBBB
void Texture2D::convertAI88ToRGB565(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i] & 0x00FC) << 3         //G
            | (data[i] & 0x00F8) >> 3;        //B
    }
}

// IIIIIIII -> RRRRGGGGBBBBAAAA
void Texture2D::convertI8ToRGBA4444(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i] & 0x00F0) << 4             //G
        | (data[i] & 0x00F0)                  //B
        | 0x000F;                             //A
    }
}

// IIIIIIIIAAAAAAAA -> RRRRGGGGBBBBAAAA
void Texture2D::convertAI88ToRGBA4444(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i] & 0x00F0) << 4             //G
        | (data[i] & 0x00F0)                  //B
        | (data[i+1] & 0x00F0) >> 4;          //A
    }
}

// IIIIIIII -> RRRRRGGGGGBBBBBA
void Texture2D::convertI8ToRGB5A1(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0; i < dataLen; ++i)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i] & 0x00F8) << 3         //G
            | (data[i] & 0x00F8) >> 2         //B
            | 0x0001;                         //A
    }
}

// IIIIIIIIAAAAAAAA -> RRRRRGGGGGBBBBBA
void Texture2D::convertAI88ToRGB5A1(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i] & 0x00F8) << 3         //G
            | (data[i] & 0x00F8) >> 2         //B
            | (data[i + 1] & 0x0080) >> 7;    //A
    }
}

// IIIIIIII -> IIIIIIIIAAAAAAAA
void Texture2D::convertI8ToAI88(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0; i < dataLen; ++i)
    {
        *out16++ = 0xFF00     //A
        | data[i];            //I
    }
}

// IIIIIIIIAAAAAAAA -> AAAAAAAA
void Texture2D::convertAI88ToA8(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 1; i < dataLen; i += 2)
    {
        *outData++ = data[i]; //A
    }
}

// IIIIIIIIAAAAAAAA -> IIIIIIII
void Texture2D::convertAI88ToI8(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 1; i < l; i += 2)
    {
        *outData++ = data[i]; //R
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA
void Texture2D::convertRGB888ToRGBA8888(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = data[i];         //R
        *outData++ = data[i + 1];     //G
        *outData++ = data[i + 2];     //B
        *outData++ = 0xFF;            //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRRRRRGGGGGGGGBBBBBBBB
void Texture2D::convertRGBA8888ToRGB888(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = data[i];         //R
        *outData++ = data[i + 1];     //G
        *outData++ = data[i + 2];     //B
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGGBBBBB
void Texture2D::convertRGB888ToRGB565(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i + 1] & 0x00FC) << 3     //G
            | (data[i + 2] & 0x00F8) >> 3;    //B
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRRGGGGGGBBBBB
void Texture2D::convertRGBA8888ToRGB565(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i + 1] & 0x00FC) << 3     //G
            | (data[i + 2] & 0x00F8) >> 3;    //B
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> IIIIIIII
void Texture2D::convertRGB888ToI8(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> IIIIIIII
void Texture2D::convertRGBA8888ToI8(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> AAAAAAAA
void Texture2D::convertRGBA8888ToA8(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen -3; i < l; i += 4)
    {
        *outData++ = data[i + 3]; //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> IIIIIIIIAAAAAAAA
void Texture2D::convertRGB888ToAI88(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
        *outData++ = 0xFF;
    }
}


// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> IIIIIIIIAAAAAAAA
void Texture2D::convertRGBA8888ToAI88(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    for (int i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *outData++ = (data[i] * 299 + data[i + 1] * 587 + data[i + 2] * 114 + 500) / 1000;  //I =  (R*299 + G*587 + B*114 + 500) / 1000
        *outData++ = data[i + 3];
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRGGGGBBBBAAAA
void Texture2D::convertRGB888ToRGBA4444(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = ((data[i] & 0x00F0) << 8           //R
                    | (data[i + 1] & 0x00F0) << 4     //G
                    | (data[i + 2] & 0xF0)            //B
                    |  0x0F);                         //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA -> RRRRGGGGBBBBAAAA
void Texture2D::convertRGBA8888ToRGBA4444(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (int i = 0, l = dataLen - 3; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F0) << 8    //R
        | (data[i + 1] & 0x00F0) << 4         //G
        | (data[i + 2] & 0xF0)                //B
        |  (data[i + 3] & 0xF0) >> 4;         //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGBBBBBA
void Texture2D::convertRGB888ToRGB5A1(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (long i = 0, l = dataLen - 2; i < l; i += 3)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i + 1] & 0x00F8) << 3     //G
            | (data[i + 2] & 0x00F8) >> 2     //B
            |  0x01;                          //A
    }
}

// RRRRRRRRGGGGGGGGBBBBBBBB -> RRRRRGGGGGBBBBBA
void Texture2D::convertRGBA8888ToRGB5A1(const unsigned char* data, KDint32 dataLen, unsigned char* outData)
{
    unsigned short* out16 = (unsigned short*)outData;
    for (long i = 0, l = dataLen - 2; i < l; i += 4)
    {
        *out16++ = (data[i] & 0x00F8) << 8    //R
            | (data[i + 1] & 0x00F8) << 3     //G
            | (data[i + 2] & 0x00F8) >> 2     //B
            |  (data[i + 3] & 0x0080) >> 7;   //A
    }
}
// conventer function end
//////////////////////////////////////////////////////////////////////////

Texture2D* Texture2D::createWithImage ( Image* pImage )
{
	Texture2D*	pRet = new Texture2D ( );

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

Texture2D::Texture2D()
: m_ePixelFormat(Texture2D::PixelFormat::DEFAULT)
, m_nPixelsWide(0)
, m_nPixelsHigh(0)
, m_uName(0)
, m_fMaxS(0.0)
, m_fMaxT(0.0)
, m_bHasPremultipliedAlpha(false)
, m_bHasMipmaps(false)
, m_pShaderProgram(NULL)
{
}

Texture2D::~Texture2D()
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTextureMgr::removeTexture(this);
#endif

    CCLOGINFO("deallocing Texture2D: %p - id=%u", this, m_uName);
    CC_SAFE_RELEASE(m_pShaderProgram);

    if(m_uName)
    {
        GL::deleteTexture(m_uName);
    }
}

Texture2D::PixelFormat Texture2D::getPixelFormat() const
{
    return m_ePixelFormat;
}

KDint32 Texture2D::getPixelsWide() const
{
    return m_nPixelsWide;
}

KDint32 Texture2D::getPixelsHigh() const
{
    return m_nPixelsHigh;
}

GLuint Texture2D::getName() const
{
    return m_uName;
}

Size Texture2D::getContentSize() const
{
    Size ret;
    ret.width = m_tContentSize.width / CC_CONTENT_SCALE_FACTOR();
    ret.height = m_tContentSize.height / CC_CONTENT_SCALE_FACTOR();
    
    return ret;
}

const Size& Texture2D::getContentSizeInPixels()
{
    return m_tContentSize;
}

GLfloat Texture2D::getMaxS() const
{
    return m_fMaxS;
}

void Texture2D::setMaxS(GLfloat maxS)
{
    m_fMaxS = maxS;
}

GLfloat Texture2D::getMaxT() const
{
    return m_fMaxT;
}

void Texture2D::setMaxT(GLfloat maxT)
{
    m_fMaxT = maxT;
}

GLProgram* Texture2D::getShaderProgram() const
{
    return m_pShaderProgram;
}

void Texture2D::setShaderProgram(GLProgram* pShaderProgram)
{
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
}

void Texture2D::releaseData(void *data)
{
    kdFree(data);
}

void* Texture2D::keepData(void *data, unsigned int length)
{
    CC_UNUSED_PARAM(length);
    //The texture data mustn't be saved because it isn't a mutable texture.
    return data;
}

bool Texture2D::hasPremultipliedAlpha() const
{
    return m_bHasPremultipliedAlpha;
}

KDbool Texture2D::initWithData ( const KDvoid* pData, KDint32 nDataLen, Texture2D::PixelFormat ePixelFormat, KDint32 nPixelsWide, KDint32 nPixelsHigh, const Size& tContentSize )
{
    CCASSERT ( nDataLen > 0 && nPixelsWide > 0 && nPixelsHigh > 0, "Invalid size" );

    //if data has no mipmaps, we will consider it has only one mipmap
    MipmapInfo	tMipmap;
    tMipmap.address = (KDubyte*) pData;
    tMipmap.len = nDataLen;
    return initWithMipmaps ( &tMipmap, 1, ePixelFormat, nPixelsWide, nPixelsHigh );

    //update information
    m_tContentSize = tContentSize;
    m_fMaxS = tContentSize.cx / nPixelsWide;
    m_fMaxT = tContentSize.cy / nPixelsHigh;
}

KDbool Texture2D::initWithMipmaps ( MipmapInfo* pMipmaps, KDint nMipmapsNum, PixelFormat ePixelFormat, KDint32 nPixelsWide, KDint32 nPixelsHigh )
{
    //the pixelFormat must be a certain value 
    CCASSERT ( ePixelFormat != PixelFormat::NONE && ePixelFormat != PixelFormat::AUTO, "the \"pixelFormat\" param must be a certain value!" );
    CCASSERT ( nPixelsWide > 0 && nPixelsHigh > 0, "Invalid size" );

    if ( nMipmapsNum <= 0 )
    {
        CCLOG ( "cocos2d: WARNING: mipmap number is less than 1" );
        return false;
    }
    
    if ( m_aPixelFormatInfoTables.find ( ePixelFormat ) == m_aPixelFormatInfoTables.end ( ) )
    {
        CCLOG ( "cocos2d: WARNING: unsupported pixelformat: %x", (KDint) ePixelFormat );
        return false;
    }

    const PixelFormatInfo&	tInfo = m_aPixelFormatInfoTables.at ( ePixelFormat );

    if ( tInfo.compressed && !Configuration::getInstance ( )->supportsPVRTC ( )
                          && !Configuration::getInstance ( )->supportsETC   ( )
                          && !Configuration::getInstance ( )->supportsS3TC  ( )
                          && !Configuration::getInstance ( )->supportsATITC ( ) )
    {
        CCLOG ( "cocos2d: WARNING: PVRTC/ETC images are not supported" );
        return false;
    }

    // Set the row align only when mipmapsNum == 1 and the data is uncompressed
    if ( nMipmapsNum == 1 && !tInfo.compressed )
    {
        KDuint	uBytesPerRow = nPixelsWide * tInfo.bpp / 8;

        if ( uBytesPerRow % 8 == 0 )
        {
            glPixelStorei ( GL_UNPACK_ALIGNMENT, 8 );
        }
        else if ( uBytesPerRow % 4 == 0 )
        {
            glPixelStorei ( GL_UNPACK_ALIGNMENT, 4 );
        }
        else if ( uBytesPerRow % 2 == 0 )
        {
            glPixelStorei ( GL_UNPACK_ALIGNMENT, 2 );
        }
        else
        {
            glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
        }
    }
	else
    {
        glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
    }
    
    glGenTextures ( 1, &m_uName );
    GL::bindTexture2D ( m_uName );

    if ( nMipmapsNum == 1 )
    {
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
	else
    {
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    }
    
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    CHECK_GL_ERROR_DEBUG ( ); // clean possible GL error
    
    // Specify OpenGL texture image
    KDint32		nWidth = nPixelsWide;
    KDint32		nHeight = nPixelsHigh;
    
    for ( KDint i = 0; i < nMipmapsNum; ++i )
    {
        KDubyte*	pData    = pMipmaps [ i ].address;
        GLsizei		nDatalen = pMipmaps [ i ].len;

        if ( tInfo.compressed )
        {
            glCompressedTexImage2D ( GL_TEXTURE_2D, i, tInfo.internalFormat, (GLsizei) nWidth, (GLsizei) nHeight, 0, nDatalen, pData );
        }
        else
        {
			glTexImage2D(GL_TEXTURE_2D, i, tInfo.internalFormat, (GLsizei) nWidth, (GLsizei) nHeight, 0, tInfo.format, tInfo.type, pData );
        }

        if ( i > 0 && ( nWidth != nHeight || ccNextPOT ( nWidth ) != nWidth ) )
        {
            CCLOG ( "cocos2d: Texture2D. WARNING. Mipmap level %u is not squared. Texture won't render correctly. width=%d != height=%d", i, nWidth, nHeight );
        }

        GLenum		nErr = glGetError ( );
        if ( nErr != GL_NO_ERROR )
        {
            CCLOG ( "cocos2d: Texture2D: Error uploading compressed texture level: %u . glError: 0x%04X", i, nErr );
            return false;
        }

        nWidth  = KD_MAX ( nWidth  >> 1, 1 );
        nHeight = KD_MAX ( nHeight >> 1, 1 );
    }

    m_tContentSize	= Size ( (KDfloat) nPixelsWide, (KDfloat) nPixelsHigh );
    m_nPixelsWide	= nPixelsWide;
    m_nPixelsHigh	= nPixelsHigh;
    m_ePixelFormat	= ePixelFormat;
    m_fMaxS			= 1;
    m_fMaxT			= 1;

    m_bHasPremultipliedAlpha = false;
    m_bHasMipmaps = nMipmapsNum > 1;

    setShaderProgram ( ShaderCache::getInstance ( )->getProgram ( GLProgram::SHADER_NAME_POSITION_TEXTURE ) );

    return true;
}

const char* Texture2D::description(void) const
{
    return String::createWithFormat("<Texture2D | Name = %u | Dimensions = %ld x %ld | Coordinates = (%.2f, %.2f)>", m_uName, (long)m_nPixelsWide, (long)m_nPixelsHigh, m_fMaxS, m_fMaxT)->getCString();
}

// implementation Texture2D (Image)
KDbool Texture2D::initWithImage ( Image* pImage )
{
    return initWithImage ( pImage, PixelFormat::NONE );
}

KDbool Texture2D::initWithImage ( Image* pImage, PixelFormat eFormat )
{
    if ( pImage == NULL )
    {
        CCLOG ( "cocos2d: Texture2D. Can't create Texture. UIImage is nil" );
        return false;
    }

    KDint			nImageWidth  = pImage->getWidth  ( );
    KDint			nImageHeight = pImage->getHeight ( );

    Configuration*	pConf = Configuration::getInstance ( );

    KDint			nMaxTextureSize = pConf->getMaxTextureSize ( );
    if ( nImageWidth > nMaxTextureSize || nImageHeight > nMaxTextureSize ) 
    {
        CCLOG ( "cocos2d: WARNING: Image (%u x %u) is bigger than the supported %u x %u", nImageWidth, nImageHeight, nMaxTextureSize, nMaxTextureSize );
        return false;
    }

    KDubyte*		pTempData     = pImage->getData ( );
    Size			tImageSize    = Size ( (KDfloat) nImageWidth, (KDfloat) nImageHeight );
    PixelFormat		ePixelFormat  = PixelFormat::NONE;
    PixelFormat		eRenderFormat = pImage->getRenderFormat ( );
    KDsize			uTempDataLen  = pImage->getDataLen ( );

    if ( pImage->getNumberOfMipmaps ( ) > 1 )
    {
        if ( eFormat != PixelFormat::NONE )
        {
            CCLOG ( "cocos2d: WARNING: This image has more than 1 mipmaps and we will not convert the data format" );
        }

        initWithMipmaps ( pImage->getMipmaps ( ), pImage->getNumberOfMipmaps ( ), pImage->getRenderFormat ( ), nImageWidth, nImageHeight );
        
        return true;
    }
    else if ( pImage->isCompressed ( ) )
    {
        if ( eFormat != PixelFormat::NONE )
        {
            CCLOG ( "cocos2d: WARNING: This image is compressed and we cann't convert it for now" );
        }

        initWithData ( pTempData, uTempDataLen, pImage->getRenderFormat ( ), nImageWidth, nImageHeight, tImageSize );
        return true;
    }
    else
    {
        // compute pixel format
        if ( eFormat != PixelFormat::NONE )
        {
			KDubyte*	pOutTempData = KD_NULL;
			KDint		nOutTempDataLen = 0;

			ePixelFormat = convertDataToFormat ( pTempData, uTempDataLen, eRenderFormat, eFormat, &pOutTempData, &nOutTempDataLen );
			initWithData ( pOutTempData, nOutTempDataLen, ePixelFormat, nImageWidth, nImageHeight, tImageSize );

			if ( pOutTempData != NULL && pOutTempData != pTempData )
			{
				delete [] pOutTempData;
			}
        }		        
		else
		{
			initWithData ( pTempData, uTempDataLen, eRenderFormat, nImageWidth, nImageHeight, tImageSize );
		}

        // set the premultiplied tag
		m_bHasPremultipliedAlpha = pImage->hasPremultipliedAlpha ( );
	
        return true;
    }
}

Texture2D::PixelFormat Texture2D::convertI8ToFormat(const unsigned char* data, KDint32 dataLen, PixelFormat format, unsigned char** outData, int* outDataLen)
{
    switch (format)
    {
    case PixelFormat::RGBA8888:
        *outDataLen = dataLen*4;
        *outData = new unsigned char[*outDataLen];
        convertI8ToRGBA8888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB888:
        *outDataLen = dataLen*3;
        *outData = new unsigned char[*outDataLen];
        convertI8ToRGB888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB565:
        *outDataLen = dataLen*2;
        *outData = new unsigned char[*outDataLen];
        convertI8ToRGB565(data, dataLen, *outData);
        break;
    case PixelFormat::AI88:
        *outDataLen = dataLen*2;
        *outData = new unsigned char[*outDataLen];
        convertI8ToAI88(data, dataLen, *outData);
        break;
    case PixelFormat::RGBA4444:
        *outDataLen = dataLen*2;
        *outData = new unsigned char[*outDataLen];
        convertI8ToRGBA4444(data, dataLen, *outData);
        break;
    case PixelFormat::RGB5A1:
        *outDataLen = dataLen*2;
        *outData = new unsigned char[*outDataLen];
        convertI8ToRGB5A1(data, dataLen, *outData);
        break;
    default:
        // unsupport convertion or don't need to convert
        if (format != PixelFormat::AUTO && format != PixelFormat::I8)
        {
            CCLOG("Can not convert image format PixelFormat::I8 to format ID:%d, we will use it's origin format PixelFormat::I8", format);
        }

        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return PixelFormat::I8;
    }

    return format;
}

Texture2D::PixelFormat Texture2D::convertAI88ToFormat(const unsigned char* data, KDint32 dataLen, PixelFormat format, unsigned char** outData, int* outDataLen)
{
    switch (format)
    {
    case PixelFormat::RGBA8888:
        *outDataLen = dataLen*2;
        *outData = new unsigned char[*outDataLen];
        convertAI88ToRGBA8888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB888:
        *outDataLen = dataLen/2*3;
        *outData = new unsigned char[*outDataLen];
        convertAI88ToRGB888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB565:
        *outDataLen = dataLen;
        *outData = new unsigned char[*outDataLen];
        convertAI88ToRGB565(data, dataLen, *outData);
        break;
    case PixelFormat::A8:
        *outDataLen = dataLen/2;
        *outData = new unsigned char[*outDataLen];
        convertAI88ToA8(data, dataLen, *outData);
        break;
    case PixelFormat::I8:
        *outDataLen = dataLen/2;
        *outData = new unsigned char[*outDataLen];
        convertAI88ToI8(data, dataLen, *outData);
        break;
    case PixelFormat::RGBA4444:
        *outDataLen = dataLen;
        *outData = new unsigned char[*outDataLen];
        convertAI88ToRGBA4444(data, dataLen, *outData);
        break;
    case PixelFormat::RGB5A1:
        *outDataLen = dataLen;
        *outData = new unsigned char[*outDataLen];
        convertAI88ToRGB5A1(data, dataLen, *outData);
        break;
    default:
        // unsupport convertion or don't need to convert
        if (format != PixelFormat::AUTO && format != PixelFormat::AI88)
        {
            CCLOG("Can not convert image format PixelFormat::AI88 to format ID:%d, we will use it's origin format PixelFormat::AI88", format);
        }

        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return PixelFormat::AI88;
        break;
    }

    return format;
}

Texture2D::PixelFormat Texture2D::convertRGB888ToFormat(const unsigned char* data, KDint32 dataLen, PixelFormat format, unsigned char** outData, int* outDataLen)
{
    switch (format)
    {
    case PixelFormat::RGBA8888:
        *outDataLen = dataLen/3*4;
        *outData = new unsigned char[*outDataLen];
        convertRGB888ToRGBA8888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB565:
        *outDataLen = dataLen/3*2;
        *outData = new unsigned char[*outDataLen];
        convertRGB888ToRGB565(data, dataLen, *outData);
        break;
    case PixelFormat::I8:
        *outDataLen = dataLen/3;
        *outData = new unsigned char[*outDataLen];
        convertRGB888ToI8(data, dataLen, *outData);
        break;
    case PixelFormat::AI88:
        *outDataLen = dataLen/3*2;
        *outData = new unsigned char[*outDataLen];
        convertRGB888ToAI88(data, dataLen, *outData);
        break;
    case PixelFormat::RGBA4444:
        *outDataLen = dataLen/3*2;
        *outData = new unsigned char[*outDataLen];
        convertRGB888ToRGBA4444(data, dataLen, *outData);
        break;
    case PixelFormat::RGB5A1:
        *outDataLen = dataLen;
        *outData = new unsigned char[*outDataLen];
        convertRGB888ToRGB5A1(data, dataLen, *outData);
        break;
    default:
        // unsupport convertion or don't need to convert
        if (format != PixelFormat::AUTO && format != PixelFormat::RGB888)
        {
            CCLOG("Can not convert image format PixelFormat::RGB888 to format ID:%d, we will use it's origin format PixelFormat::RGB888", format);
        }

        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return PixelFormat::RGB888;
    }
    return format;
}

Texture2D::PixelFormat Texture2D::convertRGBA8888ToFormat(const unsigned char* data, KDint32 dataLen, PixelFormat format, unsigned char** outData, int* outDataLen)
{

    switch (format)
    {
    case PixelFormat::RGB888:
        *outDataLen = dataLen/4*3;
        *outData = new unsigned char[*outDataLen];
        convertRGBA8888ToRGB888(data, dataLen, *outData);
        break;
    case PixelFormat::RGB565:
        *outDataLen = dataLen/2;
        *outData = new unsigned char[*outDataLen];
        convertRGBA8888ToRGB565(data, dataLen, *outData);
        break;
    case PixelFormat::A8:
        *outDataLen = dataLen/4;
        *outData = new unsigned char[*outDataLen];
        convertRGBA8888ToA8(data, dataLen, *outData);
        break;
    case PixelFormat::I8:
        *outDataLen = dataLen/4;
        *outData = new unsigned char[*outDataLen];
        convertRGBA8888ToI8(data, dataLen, *outData);
        break;
    case PixelFormat::AI88:
        *outDataLen = dataLen/2;
        *outData = new unsigned char[*outDataLen];
        convertRGBA8888ToAI88(data, dataLen, *outData);
        break;
    case PixelFormat::RGBA4444:
        *outDataLen = dataLen/2;
        *outData = new unsigned char[*outDataLen];
        convertRGBA8888ToRGBA4444(data, dataLen, *outData);
        break;
    case PixelFormat::RGB5A1:
        *outDataLen = dataLen/2;
        *outData = new unsigned char[*outDataLen];
        convertRGBA8888ToRGB5A1(data, dataLen, *outData);
        break;
    default:
        // unsupport convertion or don't need to convert
        if (format != PixelFormat::AUTO && format != PixelFormat::RGBA8888)
        {
            CCLOG("Can not convert image format PixelFormat::RGBA8888 to format ID:%d, we will use it's origin format PixelFormat::RGBA8888", format);
        }

        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return PixelFormat::RGBA8888;
    }

    return format;
}

/*
convert map:
1.PixelFormat::RGBA8888
2.PixelFormat::RGB888
3.PixelFormat::RGB565
4.PixelFormat::A8
5.PixelFormat::I8
6.PixelFormat::AI88
7.PixelFormat::RGBA4444
8.PixelFormat::RGB5A1

gray(5) -> 1235678
gray alpha(6) -> 12345678
rgb(2) -> 1235678
rgba(1) -> 12345678

*/
Texture2D::PixelFormat Texture2D::convertDataToFormat(const unsigned char* data, KDint32 dataLen, PixelFormat originFormat, PixelFormat format, unsigned char** outData, int* outDataLen)
{
    switch (originFormat)
    {
    case PixelFormat::I8:
        return convertI8ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::AI88:
        return convertAI88ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::RGB888:
        return convertRGB888ToFormat(data, dataLen, format, outData, outDataLen);
    case PixelFormat::RGBA8888:
        return convertRGBA8888ToFormat(data, dataLen, format, outData, outDataLen);
    default:
        CCLOG("unsupport convert for format %d to format %d", originFormat, format);
        *outData = (unsigned char*)data;
        *outDataLen = dataLen;
        return originFormat;
    }
}

// implementation Texture2D (Text)
bool Texture2D::initWithString(const char *text, const char *fontName, float fontSize, const Size& dimensions/* = Size(0, 0)*/, TextHAlignment hAlignment/* =  TextHAlignment::CENTER */, TextVAlignment vAlignment/* =  TextVAlignment::TOP */)
{
    FontDefinition tempDef;
    
    tempDef.m_tShadow.m_bShadowEnabled = false;
    tempDef.m_tStroke.m_bStrokeEnabled = false;
   
    
    tempDef.m_sFontName      = std::string(fontName);
    tempDef.m_nFontSize      = fontSize;
    tempDef.m_tDimensions    = dimensions;
    tempDef.m_eAlignment     = hAlignment;
    tempDef.m_eVertAlignment = vAlignment;
    tempDef.m_tFontFillColor = Color3B::WHITE;

    return initWithString(text, tempDef);
}

bool Texture2D::initWithString(const char *text, const FontDefinition& textDefinition)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // cache the texture data
    VolatileTextureMgr::addStringTexture(this, text, textDefinition);
#endif

    bool bRet = false;
    Image::TextAlign eAlign;
    
    if (TextVAlignment::TOP == textDefinition.m_eVertAlignment)
    {
        eAlign = (TextHAlignment::CENTER == textDefinition.m_eAlignment) ? Image::TextAlign::TOP
        : (TextHAlignment::LEFT == textDefinition.m_eAlignment) ? Image::TextAlign::TOP_LEFT : Image::TextAlign::TOP_RIGHT;
    }
    else if (TextVAlignment::CENTER == textDefinition.m_eVertAlignment)
    {
        eAlign = (TextHAlignment::CENTER == textDefinition.m_eAlignment) ? Image::TextAlign::CENTER
        : (TextHAlignment::LEFT == textDefinition.m_eAlignment) ? Image::TextAlign::LEFT : Image::TextAlign::RIGHT;
    }
    else if (TextVAlignment::BOTTOM == textDefinition.m_eVertAlignment)
    {
        eAlign = (TextHAlignment::CENTER == textDefinition.m_eAlignment) ? Image::TextAlign::BOTTOM
        : (TextHAlignment::LEFT == textDefinition.m_eAlignment) ? Image::TextAlign::BOTTOM_LEFT : Image::TextAlign::BOTTOM_RIGHT;
    }
    else
    {
        CCASSERT(false, "Not supported alignment format!");
        return false;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    // handle shadow parameters
    bool  shadowEnabled = false;
    float shadowDX      = 0.0f;
    float shadowDY      = 0.0f;
    float shadowBlur    = 0.0f;
    float shadowOpacity = 0.0f;
    
    if ( textDefinition.m_tShadow.m_bShadowEnabled )
    {
        shadowEnabled =  true;
        shadowDX      = textDefinition.m_tShadow.m_tShadowOffset.width;
        shadowDY      = textDefinition.m_tShadow.m_tShadowOffset.height;
        shadowBlur    = textDefinition.m_tShadow.m_fShadowBlur;
        shadowOpacity = textDefinition.m_tShadow.m_fShadowOpacity;
    }
    
    // handle stroke parameters
    bool strokeEnabled = false;
    float strokeColorR = 0.0f;
    float strokeColorG = 0.0f;
    float strokeColorB = 0.0f;
    float strokeSize   = 0.0f;
    
    if ( textDefinition.m_tStroke.m_bStrokeEnabled )
    {
        strokeEnabled = true;
        strokeColorR = textDefinition.m_tStroke.m_tStrokeColor.r / 255.0f;
        strokeColorG = textDefinition.m_tStroke.m_tStrokeColor.g / 255.0f;
        strokeColorB = textDefinition.m_tStroke.m_tStrokeColor.b / 255.0f;
        strokeSize   = textDefinition.m_tStroke.m_fStrokeSize;
    }
    
    Image* pImage = new Image();
    do
    {
        CC_BREAK_IF(NULL == pImage);
        
        bRet = pImage->initWithStringShadowStroke(text,
                                                  (int)textDefinition.m_tDimensions.width,
                                                  (int)textDefinition.m_tDimensions.height,
                                                  eAlign,
                                                  textDefinition.m_sFontName.c_str(),
                                                  textDefinition.m_nFontSize,
                                                  textDefinition.m_tFontFillColor.r / 255.0f,
                                                  textDefinition.m_tFontFillColor.g / 255.0f,
                                                  textDefinition.m_tFontFillColor.b / 255.0f,
                                                  shadowEnabled,
                                                  shadowDX,
                                                  shadowDY,
                                                  shadowOpacity,
                                                  shadowBlur,
                                                  strokeEnabled,
                                                  strokeColorR,
                                                  strokeColorG,
                                                  strokeColorB,
                                                  strokeSize);
        
        
        CC_BREAK_IF(!bRet);
        bRet = initWithImage(pImage);
        
    } while (0);
    
    CC_SAFE_RELEASE(pImage);
    
    return bRet;

#else
    bool requestUnsupported = textDefinition.m_tShadow.m_bShadowEnabled || textDefinition.m_tStroke.m_bStrokeEnabled;

    CCASSERT(requestUnsupported == false, "Currently shadow and stroke only supported on iOS and Android!");

    Image* pImage = new Image();
    do
    {
        CC_BREAK_IF(NULL == pImage);
        bRet = pImage->initWithString(text, (int)textDefinition.m_tDimensions.width, (int)textDefinition.m_tDimensions.height, eAlign, textDefinition.m_sFontName.c_str(), (int)textDefinition.m_nFontSize);
        CC_BREAK_IF(!bRet);
        bRet = initWithImage(pImage);
    } while (0);
    
    CC_SAFE_RELEASE(pImage);

    return bRet;    
#endif
}


// implementation Texture2D (Drawing)

void Texture2D::drawAtPoint(const Point& point)
{
    GLfloat    coordinates[] = {
        0.0f,    m_fMaxT,
        m_fMaxS,m_fMaxT,
        0.0f,    0.0f,
        m_fMaxS,0.0f };

    GLfloat    width = (GLfloat)m_nPixelsWide * m_fMaxS,
        height = (GLfloat)m_nPixelsHigh * m_fMaxT;

    GLfloat        vertices[] = {    
        point.x,            point.y,
        width + point.x,    point.y,
        point.x,            height  + point.y,
        width + point.x,    height  + point.y };

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS );
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();

    GL::bindTexture2D( m_uName );


#ifdef EMSCRIPTEN
    setGLBufferData(vertices, 8 * sizeof(GLfloat), 0);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLBufferData(coordinates, 8 * sizeof(GLfloat), 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
#endif // EMSCRIPTEN

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Texture2D::drawInRect(const Rect& rect)
{
    GLfloat    coordinates[] = {    
        0.0f,    m_fMaxT,
        m_fMaxS,m_fMaxT,
        0.0f,    0.0f,
        m_fMaxS,0.0f };

    GLfloat    vertices[] = {    rect.origin.x,        rect.origin.y,                            /*0.0f,*/
        rect.origin.x + rect.size.width,        rect.origin.y,                            /*0.0f,*/
        rect.origin.x,                            rect.origin.y + rect.size.height,        /*0.0f,*/
        rect.origin.x + rect.size.width,        rect.origin.y + rect.size.height,        /*0.0f*/ };

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS );
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();

    GL::bindTexture2D( m_uName );

#ifdef EMSCRIPTEN
    setGLBufferData(vertices, 8 * sizeof(GLfloat), 0);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLBufferData(coordinates, 8 * sizeof(GLfloat), 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
#endif // EMSCRIPTEN
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Texture2D::PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied)
{
    _PVRHaveAlphaPremultiplied = haveAlphaPremultiplied;
}

    
//
// Use to apply MIN/MAG filter
//
// implementation Texture2D (GLFilter)

void Texture2D::generateMipmap()
{
    CCASSERT( static_cast<unsigned long>(m_nPixelsWide) == ccNextPOT(m_nPixelsWide) && static_cast<unsigned long>(m_nPixelsHigh) == ccNextPOT(m_nPixelsHigh), "Mipmap texture only works in POT textures");
    GL::bindTexture2D( m_uName );
    glGenerateMipmap(GL_TEXTURE_2D);
    m_bHasMipmaps = true;
}

bool Texture2D::hasMipmaps() const
{
    return m_bHasMipmaps;
}

void Texture2D::setTexParameters(const TexParams &texParams)
{
    CCASSERT( (static_cast<unsigned long>(m_nPixelsWide) == ccNextPOT(m_nPixelsWide) || texParams.wrapS == GL_CLAMP_TO_EDGE) &&
        (static_cast<unsigned long>(m_nPixelsHigh) == ccNextPOT(m_nPixelsHigh) || texParams.wrapT == GL_CLAMP_TO_EDGE),
        "GL_CLAMP_TO_EDGE should be used in NPOT dimensions");

    GL::bindTexture2D( m_uName );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParams.minFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParams.magFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParams.wrapS );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParams.wrapT );

#if CC_ENABLE_CACHE_TEXTURE_DATA
    VolatileTextureMgr::setTexParameters(this, texParams);
#endif
}

void Texture2D::setAliasTexParameters()
{
    GL::bindTexture2D( m_uName );

    if( ! m_bHasMipmaps )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    }
    else
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
    }

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
#if CC_ENABLE_CACHE_TEXTURE_DATA
    TexParams texParams = {(GLuint)(m_bHasMipmaps?GL_NEAREST_MIPMAP_NEAREST:GL_NEAREST),GL_NEAREST,GL_NONE,GL_NONE};
    VolatileTextureMgr::setTexParameters(this, texParams);
#endif
}

void Texture2D::setAntiAliasTexParameters()
{
    GL::bindTexture2D( m_uName );

    if ( !m_bHasMipmaps )
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
    else
    {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    }

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
#if CC_ENABLE_CACHE_TEXTURE_DATA
    TexParams texParams = {(GLuint)(m_bHasMipmaps?GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR),GL_LINEAR,GL_NONE,GL_NONE};
    VolatileTextureMgr::setTexParameters(this, texParams);
#endif
}

const char* Texture2D::getStringForFormat() const
{
	switch (m_ePixelFormat) 
	{
		case Texture2D::PixelFormat::RGBA8888:
			return  "RGBA8888";

		case Texture2D::PixelFormat::RGB888:
			return  "RGB888";

		case Texture2D::PixelFormat::RGB565:
			return  "RGB565";

		case Texture2D::PixelFormat::RGBA4444:
			return  "RGBA4444";

		case Texture2D::PixelFormat::RGB5A1:
			return  "RGB5A1";

		case Texture2D::PixelFormat::AI88:
			return  "AI88";

		case Texture2D::PixelFormat::A8:
			return  "A8";

		case Texture2D::PixelFormat::I8:
			return  "I8";

		case Texture2D::PixelFormat::PVRTC4:
			return  "PVRTC4";

		case Texture2D::PixelFormat::PVRTC2:
			return  "PVRTC2";

		default:
			CCASSERT(false , "unrecognized pixel format");
			CCLOG("stringForFormat: %ld, cannot give useful result", (long)m_ePixelFormat);
			break;
	}

	return  NULL;
}

//
// Texture options for images that contains alpha
//
// implementation Texture2D (PixelFormat)

void Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat format)
{
    g_defaultAlphaPixelFormat = format;
}

Texture2D::PixelFormat Texture2D::getDefaultAlphaPixelFormat()
{
    return g_defaultAlphaPixelFormat;
}

unsigned int Texture2D::getBitsPerPixelForFormat(Texture2D::PixelFormat format) const
{
    if (format == PixelFormat::NONE)
    {
        return 0;
    }
    
	return m_aPixelFormatInfoTables.at(format).bpp;
}

unsigned int Texture2D::getBitsPerPixelForFormat() const
{
	return this->getBitsPerPixelForFormat(m_ePixelFormat);
}

const Texture2D::PixelFormatInfoMap& Texture2D::getPixelFormatInfoMap()
{
    return m_aPixelFormatInfoTables;
}


NS_CC_END
