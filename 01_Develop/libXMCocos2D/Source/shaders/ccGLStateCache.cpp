/* -----------------------------------------------------------------------------------
 *
 *      File            ccGLStateCache.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCGLProgram.h"
#include "cocoa/CCString.h"
#include "kazmath/GL/matrix.h"
#include "kazmath/kazmath.h"

NS_CC_BEGIN

static GLuint	l_uCurrentProjectionMatrix	= -1;
static KDbool	l_bVertexAttribPosition		= KD_FALSE;
static KDbool	l_bVertexAttribColor		= KD_FALSE;
static KDbool	l_bVertexAttribTexCoords	= KD_FALSE;


#if CC_ENABLE_GL_STATE_CACHE

#define kCCMaxActiveTexture 16

static GLuint    l_uCurrentBoundTexture [ kCCMaxActiveTexture ] =
{
	(GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, 
	(GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, (GLuint) -1, 
};

static GLuint    l_uCurrentShaderProgram  = -1;
static GLenum    l_eBlendingSource		  = -1;
static GLenum    l_eBlendingDest		  = -1;
static KDint     l_eGLServerState		  = 0;
#if CC_TEXTURE_ATLAS_USE_VAO
static GLuint    l_uVAO					  = 0;
#endif
#endif // CC_ENABLE_GL_STATE_CACHE

// GL State Cache functions

KDvoid ccGLInvalidateStateCache ( KDvoid )
{
    l_uCurrentProjectionMatrix	= -1;
    l_bVertexAttribPosition		= KD_FALSE;
    l_bVertexAttribColor		= KD_FALSE;
    l_bVertexAttribTexCoords	= KD_FALSE;

#if CC_ENABLE_GL_STATE_CACHE
    l_uCurrentShaderProgram		= -1;
    for ( KDint i = 0; i < kCCMaxActiveTexture; i++ )
    {
        l_uCurrentBoundTexture [ i ] = -1;
    }
    l_eBlendingSource			= -1;
    l_eBlendingDest				= -1;
    l_eGLServerState			= 0;
#if CC_TEXTURE_ATLAS_USE_VAO
	l_uVAO						= 0;
#endif	// CC_TEXTURE_ATLAS_USE_VAO

#endif	// CC_ENABLE_GL_STATE_CACHE
}

KDvoid  ccGLDeleteProgram ( GLuint uProgram )
{
#if defined ( USE_OPEN_GLES2 )

#if CC_ENABLE_GL_STATE_CACHE
    if ( uProgram == l_uCurrentShaderProgram )
	{
        l_uCurrentShaderProgram = -1;
	}
#endif // CC_ENABLE_GL_STATE_CACHE

    glDeleteProgram ( uProgram );

#endif
}

KDvoid ccGLUseProgram ( GLuint uProgram )
{
#if defined ( USE_OPEN_GLES2 )

#if CC_ENABLE_GL_STATE_CACHE
    if ( uProgram != l_uCurrentShaderProgram )
	{
        l_uCurrentShaderProgram = uProgram;
        glUseProgram ( uProgram );
    }
#else
    glUseProgram ( uProgram );
#endif // CC_ENABLE_GL_STATE_CACHE

#endif
}

static KDvoid SetBlending ( GLenum uSrcFactor, GLenum uDstFactor )
{
	if ( uSrcFactor == GL_ONE && uDstFactor == GL_ZERO )
    {
		glDisable ( GL_BLEND );
	}
    else
    {
		glEnable ( GL_BLEND );
		glBlendFunc ( uSrcFactor, uDstFactor );
	}
}

KDvoid ccGLBlendFunc ( GLenum uSrcFactor, GLenum uDstFactor )
{
#if CC_ENABLE_GL_STATE_CACHE
    if ( uSrcFactor != l_eBlendingSource || uDstFactor != l_eBlendingDest ) 
	{
        l_eBlendingSource = uSrcFactor;
        l_eBlendingDest   = uDstFactor;
        SetBlending ( uSrcFactor, uDstFactor );
    }
#else
    SetBlending ( uSrcFactor, uDstFactor );
#endif // CC_ENABLE_GL_STATE_CACHE
}

KDvoid ccGLBlendResetToCache ( KDvoid )
{
#if defined ( USE_OPEN_GLES2 )
	glBlendEquation ( GL_FUNC_ADD );
#endif

#if CC_ENABLE_GL_STATE_CACHE
	SetBlending ( l_eBlendingSource, l_eBlendingDest );
#else
	SetBlending ( CC_BLEND_SRC, CC_BLEND_DST );
#endif // CC_ENABLE_GL_STATE_CACHE
}

KDvoid ccGLBindTexture2D ( GLuint uTextureId )
{
    ccGLBindTexture2DN ( 0, uTextureId );
}

KDvoid ccGLBindTexture2DN ( GLuint uTextureUnit, GLuint uTextureId )
{
#if CC_ENABLE_GL_STATE_CACHE
    CCAssert ( uTextureUnit < kCCMaxActiveTexture, "textureUnit is too big" );
    if ( l_uCurrentBoundTexture [ uTextureUnit ] != uTextureId )
    {
        l_uCurrentBoundTexture [ uTextureUnit ] = uTextureId;
		glActiveTexture ( GL_TEXTURE0 + uTextureUnit );
        glBindTexture ( GL_TEXTURE_2D, uTextureId );
    }
#else
    glActiveTexture ( GL_TEXTURE0 + uTextureUnit );
    glBindTexture ( GL_TEXTURE_2D, uTextureId );
#endif
}

KDvoid ccGLDeleteTexture ( GLuint uTextureId )
{
    ccGLDeleteTextureN ( 0, uTextureId );
}

KDvoid ccGLDeleteTextureN ( GLuint uTextureUnit, GLuint uTextureId )
{
#if CC_ENABLE_GL_STATE_CACHE
    if ( l_uCurrentBoundTexture [ uTextureUnit ] == uTextureId )
    {
		l_uCurrentBoundTexture [ uTextureUnit ] = -1;
    }
#endif

    glDeleteTextures ( 1, &uTextureId );
}

KDvoid ccGLBindVAO ( GLuint uVAOId )
{
#if CC_TEXTURE_ATLAS_USE_VAO  
    
#if CC_ENABLE_GL_STATE_CACHE
	if ( l_uVAO != uVAOId )
	{
		l_uVAO = uVAOId;
		glBindVertexArray ( uVAOId );
	}
#else
	glBindVertexArray ( uVAOId );
#endif // CC_ENABLE_GL_STATE_CACHE
    
#endif
}

KDvoid ccGLEnable ( ccGLServerState uFlags )
{
#if CC_ENABLE_GL_STATE_CACHE

    KDint  nEnabled = 0;

    // GL_BLEND 
    if( ( nEnabled = ( uFlags & CC_GL_BLEND ) ) != ( l_eGLServerState & CC_GL_BLEND ) )
	{
        if ( nEnabled )
		{
            glEnable ( GL_BLEND );
            l_eGLServerState |= CC_GL_BLEND;
        } 
		else
		{
            glDisable ( GL_BLEND );
            l_eGLServerState &= ~CC_GL_BLEND;
        }
    }

#else
    
	if ( uFlags & CC_GL_BLEND )
	{
        glEnable ( GL_BLEND );
	}
    else
	{
        glDisable( GL_BLEND );
	}

#endif
}

//#pragma mark - GL Vertex Attrib functions

KDvoid ccGLEnableVertexAttribs ( KDuint uFlags )
{               
	ccGLBindVAO ( 0 );

    // Position 
    KDbool  bEnablePosition = uFlags & kCCVertexAttribFlag_Position;

    if ( bEnablePosition != l_bVertexAttribPosition )
	{
		( bEnablePosition ) ?
#if defined ( USE_OPEN_GLES2 )		
			glEnableVertexAttribArray  ( kCCVertexAttrib_Position ) : 
			glDisableVertexAttribArray ( kCCVertexAttrib_Position ) ;
#else
			glEnableClientState  ( GL_VERTEX_ARRAY ) :
			glDisableClientState ( GL_VERTEX_ARRAY ) ;
#endif

        l_bVertexAttribPosition = bEnablePosition;
    }

    // Color 
    KDbool  bEnableColor = ( uFlags & kCCVertexAttribFlag_Color ) != 0 ? KD_TRUE : KD_FALSE;

    if ( bEnableColor != l_bVertexAttribColor )
	{
		( bEnableColor ) ?
#if defined ( USE_OPEN_GLES2 )		
			glEnableVertexAttribArray  ( kCCVertexAttrib_Color ) : 
			glDisableVertexAttribArray ( kCCVertexAttrib_Color ) ;
#else
			glEnableClientState  ( GL_COLOR_ARRAY ) :
			glDisableClientState ( GL_COLOR_ARRAY ) ;
#endif

        l_bVertexAttribColor = bEnableColor;
    }

    // Tex Coords 
    KDbool  bEnableTexCoords = ( uFlags & kCCVertexAttribFlag_TexCoords ) != 0 ? KD_TRUE : KD_FALSE;

    if ( bEnableTexCoords != l_bVertexAttribTexCoords )
	{
		if ( bEnableTexCoords )
		{
#if defined ( USE_OPEN_GLES2 )	
			glEnableVertexAttribArray  ( kCCVertexAttrib_TexCoords );
#else
			glEnable ( GL_TEXTURE_2D ); 
			glEnableClientState  ( GL_TEXTURE_COORD_ARRAY );
#endif

		}
		else
		{
#if defined ( USE_OPEN_GLES2 )	
			glDisableVertexAttribArray ( kCCVertexAttrib_TexCoords );
#else
			glDisable ( GL_TEXTURE_2D ); 
			glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
#endif
		}

        l_bVertexAttribTexCoords = bEnableTexCoords;
    }
}

//#pragma mark - GL Uniforms functions

KDvoid ccSetProjectionMatrixDirty ( KDvoid )
{
    l_uCurrentProjectionMatrix = -1;
}

KDvoid  ccGLVertexAttribPointer ( GLuint uAttrib, GLint nSize, GLenum uType, GLboolean bNormalized, GLsizei nStride, const GLvoid* pPtr )
{
#if defined ( USE_OPEN_GLES2 )	
	glVertexAttribPointer ( uAttrib, nSize, uType, bNormalized, nStride, pPtr );
#else
	switch ( uAttrib )
	{
		case kCCVertexAttrib_Position	:	glVertexPointer   ( nSize, uType, nStride, pPtr );	break;
		case kCCVertexAttrib_TexCoords	:	glTexCoordPointer ( nSize, uType, nStride, pPtr );	break;
		case kCCVertexAttrib_Color		:	glColorPointer	  ( nSize, uType, nStride, pPtr );	break;
	}
#endif
}

NS_CC_END
