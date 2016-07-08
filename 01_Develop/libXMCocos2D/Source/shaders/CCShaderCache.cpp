/* -----------------------------------------------------------------------------------
 *
 *      File            CCShaderCache.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      Ricardo Quesada
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccShaders.h"

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WINRT )
#include "shaders/precompiled/winrt/ccShaders_winrt.h"
#include "CCWinRTUtils.h"
#endif

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
#include "shaders/precompiled/wp8/ccShaders_wp8.h"
#endif

NS_CC_BEGIN

enum 
{
    kCCShaderType_PositionTextureColor			,
    kCCShaderType_PositionTextureColorAlphaTest	,
    kCCShaderType_PositionColor					,
    kCCShaderType_PositionTexture				,
    kCCShaderType_PositionTexture_uColor		,
    kCCShaderType_PositionTextureA8Color		,
    kCCShaderType_Position_uColor				,
	kCCShaderType_PositionLengthTexureColor		,
    
    kCCShaderType_MAX							,
};

static CCShaderCache*   l_pSharedShaderCache = KD_NULL;

CCShaderCache* CCShaderCache::sharedShaderCache ( KDvoid )
{
    if ( !l_pSharedShaderCache )
	{
        l_pSharedShaderCache = new CCShaderCache ( );
        if ( !l_pSharedShaderCache->init ( ) )
        {
            CC_SAFE_DELETE ( l_pSharedShaderCache );
        }
    }

    return l_pSharedShaderCache;
}

KDvoid CCShaderCache::purgeSharedShaderCache ( KDvoid )
{
    CC_SAFE_RELEASE ( l_pSharedShaderCache );
}

CCShaderCache::CCShaderCache ( KDvoid )
{
	m_pPrograms = 0;
}

CCShaderCache::~CCShaderCache ( KDvoid )
{
    CCLOGINFO ( "XMCocos2D deallocing CCShaderCache = %p", this );
    m_pPrograms->release ( );
}

KDbool CCShaderCache::init ( KDvoid )
{
    m_pPrograms = new CCDictionary ( );
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WINRT ) || ( CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
	GLboolean	bHasCompiler = KD_TRUE;
	glGetBooleanv ( GL_SHADER_COMPILER, &bHasCompiler );
	if ( bHasCompiler )
	{
		loadDefaultShaders ( );
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WINRT ) && defined ( CC_PLATFORM_WINRT_SAVE_SHADERS )
		savePrecompiledShaders ( m_pPrograms );
#endif
	}
	else
	{
		loadDefaultPrecompiledShaders ( );
	}
#else
	loadDefaultShaders ( );
#endif

	return KD_TRUE;
}

KDvoid CCShaderCache::loadDefaultShaders ( KDvoid )
{
    // Position Texture Color shader
    CCGLProgram*  pProgram = new CCGLProgram ( );
    loadDefaultShader ( pProgram, kCCShaderType_PositionTextureColor );

    m_pPrograms->setObject ( pProgram, kCCShader_PositionTextureColor );
    pProgram->release ( );

    // Position Texture Color alpha test
    pProgram = new CCGLProgram ( );
    loadDefaultShader ( pProgram, kCCShaderType_PositionTextureColorAlphaTest );

    m_pPrograms->setObject ( pProgram, kCCShader_PositionTextureColorAlphaTest );
    pProgram->release ( );

    //
    // Position, Color shader
    //
    pProgram = new CCGLProgram ( );
    loadDefaultShader ( pProgram, kCCShaderType_PositionColor );

    m_pPrograms->setObject ( pProgram, kCCShader_PositionColor );
    pProgram->release ( );

    //
    // Position Texture shader
    //
    pProgram = new CCGLProgram ( );
    loadDefaultShader ( pProgram, kCCShaderType_PositionTexture );

    m_pPrograms->setObject ( pProgram, kCCShader_PositionTexture );
    pProgram->release ( );

    //
    // Position, Texture attribs, 1 Color as uniform shader
    //
    pProgram = new CCGLProgram ( );
    loadDefaultShader ( pProgram, kCCShaderType_PositionTexture_uColor );

    m_pPrograms->setObject ( pProgram, kCCShader_PositionTexture_uColor );
    pProgram->release ( );

    //
    // Position Texture A8 Color shader
    //
    pProgram = new CCGLProgram ( );
    loadDefaultShader ( pProgram, kCCShaderType_PositionTextureA8Color );
    
    m_pPrograms->setObject ( pProgram, kCCShader_PositionTextureA8Color );
    pProgram->release ( );

    //
    // Position and 1 color passed as a uniform (to simulate glColor4ub )
    //
    pProgram = new CCGLProgram ( );
    loadDefaultShader ( pProgram, kCCShaderType_Position_uColor );
    
    m_pPrograms->setObject ( pProgram, kCCShader_Position_uColor );
    pProgram->release ( );    

	//
	// Position, Legth(TexCoords, Color (used by Draw Node basically )
	//
    pProgram = new CCGLProgram ( );
    loadDefaultShader ( pProgram, kCCShaderType_PositionLengthTexureColor );
    
    m_pPrograms->setObject ( pProgram, kCCShader_PositionLengthTexureColor );
    pProgram->release ( );
}

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WINRT ) || ( CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
KDvoid CCShaderCache::loadDefaultPrecompiledShaders ( KDvoid )
{
	// Position Texture Color shader
	CCGLProgram*	pProgram = new CCGLProgram ( );
	loadDefaultPrecompiledShader ( pProgram, kCCShaderType_PositionTextureColor );
	m_pPrograms->setObject ( pProgram, kCCShader_PositionTextureColor );
	pProgram->release ( );

	// Position Texture Color alpha test
	pProgram = new CCGLProgram ( );
	loadDefaultPrecompiledShader ( pProgram, kCCShaderType_PositionTextureColorAlphaTest );

	m_pPrograms->setObject ( pProgram, kCCShader_PositionTextureColorAlphaTest );
	pProgram->release ( );

	//
	// Position, Color shader
	//
	pProgram = new CCGLProgram ( );
	loadDefaultPrecompiledShader ( pProgram, kCCShaderType_PositionColor );

	m_pPrograms->setObject ( pProgram, kCCShader_PositionColor );
	pProgram->release();

	//
	// Position Texture shader
	//
	pProgram = new CCGLProgram ( );
	loadDefaultPrecompiledShader ( pProgram, kCCShaderType_PositionTexture );

	m_pPrograms->setObject ( pProgram, kCCShader_PositionTexture );
	pProgram->release ( );

	//
	// Position, Texture attribs, 1 Color as uniform shader
	//
	pProgram = new CCGLProgram ( );
	loadDefaultPrecompiledShader ( pProgram, kCCShaderType_PositionTexture_uColor );

	m_pPrograms->setObject ( pProgram, kCCShader_PositionTexture_uColor );
	pProgram->release ( );

	//
	// Position Texture A8 Color shader
	//
	pProgram = new CCGLProgram ( );
	loadDefaultPrecompiledShader ( pProgram, kCCShaderType_PositionTextureA8Color );

	m_pPrograms->setObject ( pProgram, kCCShader_PositionTextureA8Color );
	pProgram->release ( );

	//
	// Position and 1 color passed as a uniform (to simulate glColor4ub )
	//
	pProgram = new CCGLProgram ( );
	loadDefaultPrecompiledShader ( pProgram, kCCShaderType_Position_uColor );

	m_pPrograms->setObject ( pProgram, kCCShader_Position_uColor );
	pProgram->release();

	//
	// Position, Length(TexCoords, Color (used by Draw Node basically )
	//
	pProgram = new CCGLProgram ( );
	loadDefaultPrecompiledShader ( pProgram, kCCShaderType_PositionLengthTexureColor );

	m_pPrograms->setObject ( pProgram, kCCShader_PositionLengthTexureColor );
	pProgram->release ( );
}
#endif

KDvoid CCShaderCache::reloadDefaultShaders ( KDvoid )
{
    // reset all programs and reload them
    
    // Position Texture Color shader
    CCGLProgram*  pProgram = programForKey ( kCCShader_PositionTextureColor );    
    pProgram->reset ( );   
    loadDefaultShader ( pProgram, kCCShaderType_PositionTextureColor );

    // Position Texture Color alpha test
    pProgram = programForKey ( kCCShader_PositionTextureColorAlphaTest );
    pProgram->reset ( );    
    loadDefaultShader ( pProgram, kCCShaderType_PositionTextureColorAlphaTest );
    
    //
    // Position, Color shader
    //
    pProgram = programForKey ( kCCShader_PositionColor );
    pProgram->reset ( );   
    loadDefaultShader ( pProgram, kCCShaderType_PositionColor );
    
    //
    // Position Texture shader
    //
    pProgram = programForKey ( kCCShader_PositionTexture );
    pProgram->reset ( );   
    loadDefaultShader ( pProgram, kCCShaderType_PositionTexture );
    
    //
    // Position, Texture attribs, 1 Color as uniform shader
    //
    pProgram = programForKey ( kCCShader_PositionTexture_uColor );
    pProgram->reset ( );   
    loadDefaultShader ( pProgram, kCCShaderType_PositionTexture_uColor );
    
    //
    // Position Texture A8 Color shader
    //
    pProgram = programForKey ( kCCShader_PositionTextureA8Color );
    pProgram->reset ( );   
    loadDefaultShader ( pProgram, kCCShaderType_PositionTextureA8Color );
    
    //
    // Position and 1 color passed as a uniform (to simulate glColor4ub )
    //
    pProgram = programForKey ( kCCShader_Position_uColor );
    pProgram->reset ( );   
    loadDefaultShader ( pProgram, kCCShaderType_Position_uColor );  

	//
	// Position, Legth(TexCoords, Color (used by Draw Node basically )
	//
    pProgram = programForKey ( kCCShader_PositionLengthTexureColor );
    pProgram->reset ( );
    loadDefaultShader ( pProgram, kCCShaderType_PositionLengthTexureColor );
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
KDvoid CCShaderCache::loadDefaultPrecompiledShader ( CCGLProgram* pProgram, KDint nType )
{
	switch ( nType )
	{
		case kCCShaderType_PositionTextureColor :
			pProgram->initWithPrecompiledProgramByteArray((const GLchar*)ShaderPositionTextureColor, sizeof(ShaderPositionTextureColor));
			pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			break;
		case kCCShaderType_PositionTextureColorAlphaTest:
			pProgram->initWithPrecompiledProgramByteArray((const GLchar*)ShaderPositionTextureColorAlphaTest, sizeof(ShaderPositionTextureColorAlphaTest));
			pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			break;
		case kCCShaderType_PositionColor:  
			pProgram->initWithPrecompiledProgramByteArray((const GLchar*)ShaderPositionColor, sizeof(ShaderPositionColor));
			pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			break;
		case kCCShaderType_PositionTexture:
			pProgram->initWithPrecompiledProgramByteArray((const GLchar*)ShaderPositionTexture, sizeof(ShaderPositionTexture));
			pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			break;
		case kCCShaderType_PositionTexture_uColor:
			pProgram->initWithPrecompiledProgramByteArray((const GLchar*)ShaderPositionTexture_uColor, sizeof(ShaderPositionTexture_uColor));
			pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			break;
		case kCCShaderType_PositionTextureA8Color:
			pProgram->initWithPrecompiledProgramByteArray((const GLchar*)ShaderPositionTextureA8Color, sizeof(ShaderPositionTextureA8Color));
			pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			break;
		case kCCShaderType_Position_uColor:
			pProgram->initWithPrecompiledProgramByteArray((const GLchar*)ShaderPosition_uColor, sizeof(ShaderPosition_uColor));
			pProgram->addAttribute("aVertex", kCCVertexAttrib_Position);    
			break;
		case kCCShaderType_PositionLengthTexureColor:
			pProgram->initWithPrecompiledProgramByteArray((const GLchar*)ShaderPositionLengthTextureColor, sizeof(ShaderPositionLengthTextureColor));
			pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			break;
		default:
			CCLOG ( "cocos2d: %s:%d, error shader type", __FUNCTION__, __LINE__ );
			return;
	}

	pProgram->updateUniforms();

	CHECK_GL_ERROR_DEBUG();
}

KDvoid CCShaderCache::reloadDefaultPrecompiledShaders ( KDvoid )
{
	// reset all programs and reload them

	// Position Texture Color shader
	CCGLProgram*	pProgram = programForKey(kCCShader_PositionTextureColor);    
	pProgram->reset();
	loadDefaultPrecompiledShader(pProgram, kCCShaderType_PositionTextureColor);

	// Position Texture Color alpha test
	pProgram = programForKey(kCCShader_PositionTextureColorAlphaTest);
	pProgram->reset();    
	loadDefaultPrecompiledShader(pProgram, kCCShaderType_PositionTextureColorAlphaTest);

	//
	// Position, Color shader
	//
	pProgram = programForKey(kCCShader_PositionColor);
	pProgram->reset();
	loadDefaultPrecompiledShader(pProgram, kCCShaderType_PositionColor);

	//
	// Position Texture shader
	//
	pProgram = programForKey(kCCShader_PositionTexture);
	pProgram->reset();
	loadDefaultPrecompiledShader(pProgram, kCCShaderType_PositionTexture);

	//
	// Position, Texture attribs, 1 Color as uniform shader
	//
	pProgram = programForKey(kCCShader_PositionTexture_uColor);
	pProgram->reset();
	loadDefaultPrecompiledShader(pProgram, kCCShaderType_PositionTexture_uColor);

	//
	// Position Texture A8 Color shader
	//
	pProgram = programForKey(kCCShader_PositionTextureA8Color);
	pProgram->reset();
	loadDefaultPrecompiledShader(pProgram, kCCShaderType_PositionTextureA8Color);

	//
	// Position and 1 color passed as a uniform (to simulate glColor4ub )
	//
	pProgram = programForKey(kCCShader_Position_uColor);
	pProgram->reset();
	loadDefaultPrecompiledShader(pProgram, kCCShaderType_Position_uColor);

	//
	// Position, Legth(TexCoords, Color (used by Draw Node basically )
	//
	pProgram = programForKey(kCCShader_PositionLengthTexureColor);
	pProgram->reset();
	loadDefaultPrecompiledShader(pProgram, kCCShaderType_PositionLengthTexureColor);
}
#endif

KDvoid CCShaderCache::loadDefaultShader ( CCGLProgram* pProgram, KDint nType )
{
    switch ( nType )
	{
        case kCCShaderType_PositionTextureColor :
            
			pProgram->initWithVertexShaderByteArray ( ccPositionTextureColor_vert, ccPositionTextureColor_frag );
            
            pProgram->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position  );
            pProgram->addAttribute ( kCCAttributeNameColor   , kCCVertexAttrib_Color     );
            pProgram->addAttribute ( kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords );
            
            break;

        case kCCShaderType_PositionTextureColorAlphaTest :
            
			pProgram->initWithVertexShaderByteArray ( ccPositionTextureColor_vert, ccPositionTextureColorAlphaTest_frag );
            
            pProgram->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position  );
            pProgram->addAttribute ( kCCAttributeNameColor   , kCCVertexAttrib_Color     );
            pProgram->addAttribute ( kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords );

            break;

        case kCCShaderType_PositionColor :

            pProgram->initWithVertexShaderByteArray ( ccPositionColor_vert ,ccPositionColor_frag );
            
            pProgram->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position );
            pProgram->addAttribute ( kCCAttributeNameColor   , kCCVertexAttrib_Color    );

            break;

        case kCCShaderType_PositionTexture :
            pProgram->initWithVertexShaderByteArray ( ccPositionTexture_vert ,ccPositionTexture_frag );
            
            pProgram->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position  );
            pProgram->addAttribute ( kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords );

            break;

        case kCCShaderType_PositionTexture_uColor :

            pProgram->initWithVertexShaderByteArray ( ccPositionTexture_uColor_vert, ccPositionTexture_uColor_frag );
            
            pProgram->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position  );
            pProgram->addAttribute ( kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords );

            break;

        case kCCShaderType_PositionTextureA8Color :

            pProgram->initWithVertexShaderByteArray ( ccPositionTextureA8Color_vert, ccPositionTextureA8Color_frag );
            
            pProgram->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position  );
            pProgram->addAttribute ( kCCAttributeNameColor   , kCCVertexAttrib_Color     );
            pProgram->addAttribute ( kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords );

            break;

        case kCCShaderType_Position_uColor :

            pProgram->initWithVertexShaderByteArray ( ccPosition_uColor_vert, ccPosition_uColor_frag );    
            
            pProgram->addAttribute ( "aVertex", kCCVertexAttrib_Position );    
            
            break;

		case kCCShaderType_PositionLengthTexureColor :

            pProgram->initWithVertexShaderByteArray ( ccPositionColorLengthTexture_vert, ccPositionColorLengthTexture_frag );
            
            pProgram->addAttribute ( kCCAttributeNamePosition, kCCVertexAttrib_Position  );
            pProgram->addAttribute ( kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords );
            pProgram->addAttribute ( kCCAttributeNameColor   , kCCVertexAttrib_Color     );
            
            break;

        default :

            CCLOG ( "XMCocos2D : %s:%d, error shader type", __FUNCTION__, __LINE__ );
            
			return;
    }
    
    pProgram->link ( );
    pProgram->updateUniforms ( );
    
    CHECK_GL_ERROR_DEBUG ( );
}

CCGLProgram* CCShaderCache::programForKey ( const KDchar* szKey )
{
    return (CCGLProgram*) m_pPrograms->objectForKey ( szKey );
}

KDvoid CCShaderCache::addProgram ( CCGLProgram* pProgram, const KDchar* szKey )
{
    m_pPrograms->setObject ( pProgram, szKey );
}

NS_CC_END
