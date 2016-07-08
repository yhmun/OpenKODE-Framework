/* -----------------------------------------------------------------------------------
 *
 *      File            CCAtlasNode.cpp
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
#include "base_nodes/CCAtlasNode.h"
#include "textures/CCTextureAtlas.h"
#include "textures/CCTextureCache.h"
#include "support/CCDirector.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN
    
//
// implementation CCAtlasNode
//

/// CCAtlasNode - Creation & Init
CCAtlasNode::CCAtlasNode ( KDvoid )
{
    m_uItemsPerRow				= 0;
    m_uItemsPerColumn			= 0;
    m_uItemWidth				= 0;
    m_uItemHeight				= 0;
    m_pTextureAtlas				= KD_NULL;
    m_bIsOpacityModifyRGB		= KD_FALSE;
    m_uQuadsToDraw				= 0;
	m_nUniformColor				= 0;
	m_bIgnoreContentScaleFactor = KD_FALSE;
}

CCAtlasNode::~CCAtlasNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTextureAtlas );
}

CCAtlasNode* CCAtlasNode::create ( const KDchar* szFilePath, KDuint uTileWidth, KDuint uTileHeight, KDuint uItemsToRender )
{
	CCAtlasNode*  pRet = new CCAtlasNode ( );

	if ( pRet && pRet->initWithTileFile ( szFilePath, uTileWidth, uTileHeight, uItemsToRender ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCAtlasNode::initWithTileFile ( const KDchar* szFilePath, KDuint uTileWidth, KDuint uTileHeight, KDuint uItemsToRender )
{
    CCAssert ( szFilePath != KD_NULL, "title should not be null" );
    CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilePath );
	return initWithTexture ( pTexture, uTileWidth, uTileHeight, uItemsToRender );
}

KDbool CCAtlasNode::initWithTexture ( CCTexture2D* pTexture, KDuint uTileWidth, KDuint uTileHeight, KDuint uItemsToRender )
{
	m_uItemWidth  = uTileWidth;
	m_uItemHeight = uTileHeight;

	m_tColorUnmodified = ccWHITE;
	m_bIsOpacityModifyRGB = KD_TRUE;

	m_tBlendFunc.src = CC_BLEND_SRC;
	m_tBlendFunc.dst = CC_BLEND_DST;

    m_pTextureAtlas = new CCTextureAtlas ( );
    m_pTextureAtlas->initWithTexture ( pTexture, uItemsToRender );

	if ( !m_pTextureAtlas )
	{
		CCLOG ( "XCocos2D : Could not initialize CCAtlasNode. Invalid Texture.");		
		return KD_FALSE;
	}

	this->updateBlendFunc ( );
	this->updateOpacityModifyRGB ( );

	this->calculateMaxItems ( );

	m_uQuadsToDraw = uItemsToRender;

    // shader stuff
    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTexture_uColor ) );
#if defined ( USE_OPEN_GLES2 )
    m_nUniformColor = glGetUniformLocation ( getShaderProgram ( )->getProgram ( ), "u_color" );
#endif

	return KD_TRUE;
}

/// CCAtlasNode - Atlas generation
KDvoid CCAtlasNode::calculateMaxItems ( KDvoid )
{
	CCSize		tSize = m_pTextureAtlas->getTexture ( )->getContentSize ( );

	if ( m_bIgnoreContentScaleFactor )
	{
		tSize = m_pTextureAtlas->getTexture ( )->getContentSizeInPixels ( );
	}

	m_uItemsPerColumn = (KDuint) ( tSize.cy / m_uItemHeight );
	m_uItemsPerRow    = (KDuint) ( tSize.cx / m_uItemWidth  );
}

KDvoid CCAtlasNode::updateAtlasValues ( KDvoid )
{
	CCAssert ( KD_FALSE, "CCAtlasNode : Abstract updateAtlasValue not overridden" );
}

/// CCAtlasNode - draw
KDvoid CCAtlasNode::draw ( KDvoid )
{
    CC_NODE_DRAW_SETUP ( );

    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

    GLfloat  aColors [ 4 ] = 
	{
		m_tDisplayedColor.r / 255.0f,
		m_tDisplayedColor.g / 255.0f,
		m_tDisplayedColor.b / 255.0f,
		m_cDisplayedOpacity / 255.0f
	};

#if defined ( USE_OPEN_GLES2 )
    getShaderProgram ( )->setUniformLocationWith4fv ( m_nUniformColor, aColors, 1 );
#else
	glColor4f ( aColors [ 0 ], aColors [ 1 ], aColors [ 2 ], aColors [ 3 ] );
#endif

    m_pTextureAtlas->drawNumberOfQuads ( m_uQuadsToDraw, 0 );
}

/// CCAtlasNode - RGBA protocol
const ccColor3B& CCAtlasNode:: getColor ( KDvoid )
{
	if ( m_bIsOpacityModifyRGB )
	{
		return m_tColorUnmodified;
	}

	return CCNodeRGBA::getColor ( );
}

KDvoid CCAtlasNode::setColor ( const ccColor3B& tColor )
{
    ccColor3B  tTemp = tColor;
    m_tColorUnmodified = tColor;

    if ( m_bIsOpacityModifyRGB )
    {
        tTemp.r = tTemp.r * m_cDisplayedOpacity / 255;
        tTemp.g = tTemp.g * m_cDisplayedOpacity / 255;
        tTemp.b = tTemp.b * m_cDisplayedOpacity / 255;
    }
    CCNodeRGBA::setColor ( tTemp );
}

KDvoid CCAtlasNode::setOpacity ( GLubyte cOpacity )
{
	CCNodeRGBA::setOpacity ( cOpacity );

	// special opacity for premultiplied textures
	if ( m_bIsOpacityModifyRGB )
	{
		this->setColor ( m_tColorUnmodified );
	}
}

KDvoid CCAtlasNode::setOpacityModifyRGB ( KDbool bOpacityModifyRGB )
{
	ccColor3B  tOldColor  = this->getColor ( );
	m_bIsOpacityModifyRGB = bOpacityModifyRGB;
	this->setColor ( tOldColor );
}

KDbool CCAtlasNode::isOpacityModifyRGB ( KDvoid )
{
	return m_bIsOpacityModifyRGB;
}

KDvoid CCAtlasNode::updateOpacityModifyRGB ( KDvoid )
{
	m_bIsOpacityModifyRGB = m_pTextureAtlas->getTexture ( )->hasPremultipliedAlpha ( );
}

KDvoid CCAtlasNode::setIgnoreContentScaleFactor ( KDbool bIgnoreContentScaleFactor )
{
	m_bIgnoreContentScaleFactor = bIgnoreContentScaleFactor;
}

/// CCAtlasNode - CocosNodeTexture protocol
ccBlendFunc CCAtlasNode::getBlendFunc ( KDvoid )
{
	return m_tBlendFunc;
}

KDvoid CCAtlasNode::setBlendFunc ( ccBlendFunc tBlendFunc )
{
	m_tBlendFunc = tBlendFunc;
}

KDvoid CCAtlasNode::updateBlendFunc ( KDvoid )
{
	if ( !m_pTextureAtlas->getTexture ( )->hasPremultipliedAlpha ( ) )
	{
		m_tBlendFunc.src = GL_SRC_ALPHA;
		m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	}
}

KDvoid CCAtlasNode::setTexture ( CCTexture2D* pTexture )
{
	m_pTextureAtlas->setTexture ( pTexture );

	this->updateBlendFunc ( );
	this->updateOpacityModifyRGB ( );
}

CCTexture2D* CCAtlasNode::getTexture ( KDvoid )
{
	return m_pTextureAtlas->getTexture ( );
}

KDvoid CCAtlasNode::setTextureAtlas ( CCTextureAtlas* pTextureAtlas )
{
	CC_SAFE_RETAIN ( pTextureAtlas );
	CC_SAFE_RELEASE ( m_pTextureAtlas );

	m_pTextureAtlas = pTextureAtlas;
}

CCTextureAtlas* CCAtlasNode::getTextureAtlas ( KDvoid )
{
	return m_pTextureAtlas;
}

KDuint CCAtlasNode::getQuadsToDraw ( KDvoid )
{
	return m_uQuadsToDraw;
}

KDvoid CCAtlasNode::setQuadsToDraw ( KDuint uQuadsToDraw )
{
	m_uQuadsToDraw = uQuadsToDraw;
}


NS_CC_END