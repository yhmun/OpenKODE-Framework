/* -----------------------------------------------------------------------------------
 *
 *      File            CCMotionStreak.cpp 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      ForzeField Studios S.L.
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
#include "misc_nodes/CCMotionStreak.h"
#include "textures/CCTextureCache.h"
#include "support/CCPointExtension.h"
#include "support/CCVertex.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN
    
CCMotionStreak::CCMotionStreak ( KDvoid )
{
	m_bFastMode			= KD_FALSE;
	m_pTexture			= NULL;
	m_tPositionR		= ccpz;
	m_fStroke			= 0.0f;
	m_fFadeDelta		= 0.0f;
	m_fMinSeg			= 0.0f;
	m_uMaxPoints		= 0;
	m_uNuPoints			= 0;
	m_uPreviousNuPoints = 0;
	m_pPointVertexes	= KD_NULL;
	m_pPointState		= KD_NULL;
	m_pVertices			= KD_NULL;
	m_pColorPointer		= KD_NULL;
	m_pTexCoords		= KD_NULL;
    m_tBlendFunc.src	= GL_SRC_ALPHA;
    m_tBlendFunc.dst	= GL_ONE_MINUS_SRC_ALPHA;
	m_bStartingPositionInitialized	= KD_FALSE;
}

CCMotionStreak::~CCMotionStreak ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pTexture );
    CC_SAFE_FREE ( m_pPointState );
    CC_SAFE_FREE ( m_pPointVertexes );
    CC_SAFE_FREE ( m_pVertices );
    CC_SAFE_FREE ( m_pColorPointer );
    CC_SAFE_FREE ( m_pTexCoords );
}

CCMotionStreak* CCMotionStreak::create ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const ccColor3B& tColor, const KDchar* szPath )
{
	CCMotionStreak*  pRet = new CCMotionStreak ( );

	if ( pRet && pRet->initWithFade ( fFade, fMinSeg, fStroke, tColor, szPath ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCMotionStreak* CCMotionStreak::create ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const ccColor3B& tColor, CCTexture2D* pTexture )
{
	CCMotionStreak*  pRet = new CCMotionStreak ( );

	if ( pRet && pRet->initWithFade ( fFade, fMinSeg, fStroke, tColor, pTexture ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCMotionStreak::initWithFade ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const ccColor3B& tColor, const KDchar* szPath )
{
    CCAssert ( szPath != KD_NULL, "Invalid filename" );

    CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szPath );
    return initWithFade ( fFade, fMinSeg, fStroke, tColor, pTexture );
}

KDbool CCMotionStreak::initWithFade ( KDfloat fFade, KDfloat fMinSeg, KDfloat fStroke, const ccColor3B& tColor, CCTexture2D* pTexture )
{
    CCNode::setPosition ( CCPointZero );
    setAnchorPoint ( CCPointZero );
    ignoreAnchorPointForPosition ( KD_TRUE );
	m_bStartingPositionInitialized = KD_FALSE;

    m_tPositionR	 = CCPointZero;
    m_bFastMode		 = KD_TRUE;
    m_fMinSeg		 = ( fMinSeg == -1.0f ) ? fStroke / 5.0f : fMinSeg;
    m_fMinSeg		*= m_fMinSeg;

    m_fStroke		 = fStroke;
    m_fFadeDelta	 = 1.0f / fFade;

    m_uMaxPoints	 = (KDuint) ( fFade * 60.0f ) + 2;
    m_uNuPoints		 = 0;

    m_pPointState    = (KDfloat*) kdMalloc ( sizeof ( KDfloat ) * m_uMaxPoints );
    m_pPointVertexes = (CCPoint*) kdMalloc ( sizeof ( CCPoint ) * m_uMaxPoints );

    m_pVertices      = (ccVertex2F*) kdMalloc ( sizeof ( ccVertex2F ) * m_uMaxPoints * 2 );
    m_pTexCoords	 = (ccTex2F*) kdMalloc ( sizeof ( ccTex2F ) * m_uMaxPoints * 2 );
    m_pColorPointer  = (GLubyte*) kdMalloc ( sizeof ( GLubyte ) * m_uMaxPoints * 2 * 4 );

    // Set blend mode
    m_tBlendFunc.src = GL_SRC_ALPHA;
    m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;

    // shader program
    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );

    setTexture ( pTexture );
    setColor ( tColor );
    scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid CCMotionStreak::setPosition ( const CCPoint& tPosition )
{
	m_bStartingPositionInitialized = KD_TRUE;
    m_tPositionR = tPosition;
}

KDvoid CCMotionStreak::tintWithColor ( const ccColor3B& tColor )
{
    setColor ( tColor );

    // Fast assignation
    for ( KDuint i = 0; i < m_uNuPoints * 2; i++ ) 
    {
        *( (ccColor3B*) ( m_pColorPointer + i * 4 ) ) = tColor;
    }
}

CCTexture2D* CCMotionStreak::getTexture ( KDvoid )
{
    return m_pTexture;
}

KDvoid CCMotionStreak::setTexture ( CCTexture2D* pTexture )
{
    if ( m_pTexture != pTexture )
    {
        CC_SAFE_RETAIN ( pTexture );
        CC_SAFE_RELEASE ( m_pTexture );
        m_pTexture = pTexture;
    }
}

KDvoid CCMotionStreak::setBlendFunc ( ccBlendFunc tBlendFunc )
{
    m_tBlendFunc = tBlendFunc;
}

ccBlendFunc CCMotionStreak::getBlendFunc ( KDvoid )
{
    return m_tBlendFunc;
}

KDvoid CCMotionStreak::setOpacity ( GLubyte cOpacity )
{
    CCAssert ( KD_FALSE, "Set opacity no supported" );
}

GLubyte CCMotionStreak::getOpacity ( KDvoid )
{
    CCAssert ( KD_FALSE, "Opacity no supported" );
    return 0;
}

KDvoid CCMotionStreak::setOpacityModifyRGB ( KDbool bValue )
{
    CC_UNUSED_PARAM ( bValue );
}

KDbool CCMotionStreak::isOpacityModifyRGB ( KDvoid )
{
    return KD_FALSE;
}

KDvoid CCMotionStreak::update ( KDfloat fDelta )
{
	if ( !m_bStartingPositionInitialized )
    {
        return;
    }

    fDelta *= m_fFadeDelta;

    KDuint	uNewIdx, uNewIdx2, i, i2;
    KDuint	uMov = 0;

    // Update current points
    for ( i = 0; i < m_uNuPoints; i++ )
    {
        m_pPointState [ i ] -= fDelta;

        if ( m_pPointState [ i ] <= 0 )
		{
            uMov++;
		}
        else
        {
            uNewIdx = i - uMov;

            if ( uMov > 0 )
            {
                // Move data
                m_pPointState [ uNewIdx ] = m_pPointState [ i ];

                // Move point
                m_pPointVertexes [ uNewIdx ] = m_pPointVertexes [ i ];

                // Move vertices
                i2 = i * 2;
                uNewIdx2 = uNewIdx * 2;
                m_pVertices [ uNewIdx2 + 0 ] = m_pVertices [ i2 + 0 ];
                m_pVertices [ uNewIdx2 + 1 ] = m_pVertices [ i2 + 1 ];

                // Move color
                i2 *= 4;
                uNewIdx2 *= 4;
                m_pColorPointer [ uNewIdx2 + 0 ] = m_pColorPointer [ i2 + 0 ];
                m_pColorPointer [ uNewIdx2 + 1 ] = m_pColorPointer [ i2 + 1 ];
                m_pColorPointer [ uNewIdx2 + 2 ] = m_pColorPointer [ i2 + 2 ];
                m_pColorPointer [ uNewIdx2 + 4 ] = m_pColorPointer [ i2 + 4 ];
                m_pColorPointer [ uNewIdx2 + 5 ] = m_pColorPointer [ i2 + 5 ];
                m_pColorPointer [ uNewIdx2 + 6 ] = m_pColorPointer [ i2 + 6 ];
            }
			else
			{
                uNewIdx2 = uNewIdx * 8;
			}

            const GLubyte  cOpacity = (GLubyte) ( m_pPointState [ uNewIdx ] * 255.0f );
            m_pColorPointer [ uNewIdx2 + 3 ] = cOpacity;
            m_pColorPointer [ uNewIdx2 + 7 ] = cOpacity;
        }
    }
    m_uNuPoints -= uMov;

    // Append new point
    KDbool  bAppendNewPoint = KD_TRUE;
    if ( m_uNuPoints >= m_uMaxPoints )
	{
        bAppendNewPoint = KD_FALSE;
	}
    else if ( m_uNuPoints > 0 )
    {
        KDbool  a1 = ccpDistanceSQ ( m_pPointVertexes [ m_uNuPoints - 1 ], m_tPositionR ) < m_fMinSeg;
        KDbool  a2 = ( m_uNuPoints == 1 ) ? KD_FALSE : ( ccpDistanceSQ ( m_pPointVertexes [ m_uNuPoints - 2 ], m_tPositionR ) < ( m_fMinSeg * 2.0f ) );
        if ( a1 || a2 )
		{
            bAppendNewPoint = KD_FALSE;
		}
    }

    if ( bAppendNewPoint )
    {
        m_pPointVertexes [ m_uNuPoints ] = m_tPositionR;
        m_pPointState    [ m_uNuPoints ] = 1.0f;

        // Color assignment
        const KDuint  uOffset = m_uNuPoints * 8;
        *( (ccColor3B*) ( m_pColorPointer + uOffset + 0 ) ) = m_tDisplayedColor;
        *( (ccColor3B*) ( m_pColorPointer + uOffset + 4 ) ) = m_tDisplayedColor;

        // Opacity
        m_pColorPointer [ uOffset + 3 ] = 255;
        m_pColorPointer [ uOffset + 7 ] = 255;

        // Generate polygon
        if ( m_uNuPoints > 0 && m_bFastMode )
        {
            if ( m_uNuPoints > 1 )
			{
                ccVertexLineToPolygon ( m_pPointVertexes, m_fStroke, m_pVertices, m_uNuPoints, 1 );
			}
            else
			{
                ccVertexLineToPolygon ( m_pPointVertexes, m_fStroke, m_pVertices, 0, 2 );
			}
        }

        m_uNuPoints++;
    }

    if ( !m_bFastMode )
	{
        ccVertexLineToPolygon ( m_pPointVertexes, m_fStroke, m_pVertices, 0, m_uNuPoints );
	}

	// Updated Tex Coords only if they are different than previous step
    if ( m_uNuPoints > 0 && m_uPreviousNuPoints != m_uNuPoints )
	{
        KDfloat  fTexDelta = 1.0f / m_uNuPoints;
        for ( i = 0; i < m_uNuPoints; i++ ) 
		{
            m_pTexCoords [ i * 2 + 0 ] = tex2 ( 0, fTexDelta * i );
            m_pTexCoords [ i * 2 + 1 ] = tex2 ( 1, fTexDelta * i );
        }

        m_uPreviousNuPoints = m_uNuPoints;
    }
}

KDvoid CCMotionStreak::reset ( KDvoid )
{
    m_uNuPoints = 0;
}

KDvoid CCMotionStreak::draw ( KDvoid )
{
    if ( m_uNuPoints <= 1 )
	{
        return;
	}

    CC_NODE_DRAW_SETUP ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );
    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

    ccGLBindTexture2D ( m_pTexture->getName ( ) );

#ifdef EMSCRIPTEN
	// Size calculations from ::initWithFade
	setGLBufferData ( m_pVertices, ( sizeof ( ccVertex2F ) * m_uMaxPoints * 2 ), 0 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	setGLBufferData ( m_pTexCoords, ( sizeof ( ccTex2F ) * m_uMaxPoints * 2 ), 1 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	setGLBufferData ( m_pColorPointer, ( sizeof ( GLubyte ) * m_uMaxPoints * 2 * 4 ), 2 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0 );
#else
    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT		, GL_FALSE, 0, m_pVertices );
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT		, GL_FALSE, 0, m_pTexCoords );
    ccGLVertexAttribPointer ( kCCVertexAttrib_Color	   , 4, GL_UNSIGNED_BYTE, GL_TRUE , 0, m_pColorPointer );
#endif // EMSCRIPTEN
    glDrawArrays ( GL_TRIANGLE_STRIP, 0, (GLsizei) m_uNuPoints * 2 );

    CC_INCREMENT_GL_DRAWS ( 1 );
}

NS_CC_END
