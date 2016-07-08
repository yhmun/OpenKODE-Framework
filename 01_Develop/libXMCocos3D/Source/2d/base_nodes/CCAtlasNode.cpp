/* -----------------------------------------------------------------------------------
 *
 *      File            CCAtlasNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#include "2d/base_nodes/CCAtlasNode.h"
#include "2d/textures/CCTextureAtlas.h"
#include "2d/textures/CCTextureCache.h"
#include "2d/CCDirector.h"
#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "support/TransformUtils.h"

// external
#include "XMKazmath/GL/matrix.h"

NS_CC_BEGIN

// implementation AtlasNode

// AtlasNode - Creation & Init

AtlasNode::AtlasNode()
: m_nItemsPerRow(0)
, m_nItemsPerColumn(0)
, m_nItemWidth(0)
, m_nItemHeight(0)
, m_pTextureAtlas(NULL)
, m_bIsOpacityModifyRGB(false)
, m_nQuadsToDraw(0)
, m_nUniformColor(0)
, m_bIgnoreContentScaleFactor(false)
{
}

AtlasNode::~AtlasNode()
{
    CC_SAFE_RELEASE(m_pTextureAtlas);
}

AtlasNode* AtlasNode::create ( const std::string& sTile, KDint32 nTileWidth, KDint32 nTileHeight, KDint32 nItemsToRender )
{
	AtlasNode*	pRet = new AtlasNode ( );
	if ( pRet->initWithTileFile ( sTile, nTileWidth, nTileHeight, nItemsToRender ) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool AtlasNode::initWithTileFile ( const std::string& sTile, KDint32 nTileWidth, KDint32 nTileHeight, KDint32 nItemsToRender )
{
    CCASSERT ( sTile.size ( ) > 0, "file size should not be empty" );
    Texture2D*	pTexture = Director::getInstance ( )->getTextureCache ( )->addImage ( sTile );
	return initWithTexture ( pTexture, nTileWidth, nTileHeight, nItemsToRender );
}

bool AtlasNode::initWithTexture ( Texture2D* pTexture, KDint32 nTileWidth, KDint32 nTileHeight, KDint32 nItemsToRender )
{
    m_nItemWidth  = nTileWidth;
    m_nItemHeight = nTileHeight;

    m_tColorUnmodified = Color3B::WHITE;
    m_bIsOpacityModifyRGB = true;

    m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

    m_pTextureAtlas = new TextureAtlas();
    m_pTextureAtlas->initWithTexture(pTexture, nItemsToRender);

    if (! m_pTextureAtlas)
    {
        CCLOG("cocos2d: Could not initialize AtlasNode. Invalid Texture.");
        return false;
    }

    this->updateBlendFunc();
    this->updateOpacityModifyRGB();

    this->calculateMaxItems();

    m_nQuadsToDraw = nItemsToRender;

    // shader stuff
    setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR));
    m_nUniformColor = glGetUniformLocation( getShaderProgram()->getProgram(), "u_color");

    return true;
}


// AtlasNode - Atlas generation

void AtlasNode::calculateMaxItems()
{
    Size s = m_pTextureAtlas->getTexture()->getContentSize();
    
    if (m_bIgnoreContentScaleFactor)
    {
        s = m_pTextureAtlas->getTexture()->getContentSizeInPixels();
    }
    
    m_nItemsPerColumn = (int)(s.height / m_nItemHeight);
    m_nItemsPerRow = (int)(s.width / m_nItemWidth);
}

void AtlasNode::updateAtlasValues()
{
    CCASSERT(false, "CCAtlasNode:Abstract updateAtlasValue not overridden");
}

// AtlasNode - draw
void AtlasNode::draw(void)
{
    CC_NODE_DRAW_SETUP();

    GL::blendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );

    GLfloat colors[4] = {m_tDisplayedColor.r / 255.0f, m_tDisplayedColor.g / 255.0f, m_tDisplayedColor.b / 255.0f, m_cDisplayedOpacity / 255.0f};
    getShaderProgram()->setUniformLocationWith4fv(m_nUniformColor, colors, 1);

    m_pTextureAtlas->drawNumberOfQuads(m_nQuadsToDraw, 0);
}

// AtlasNode - RGBA protocol

const Color3B& AtlasNode::getColor() const
{
    if(m_bIsOpacityModifyRGB)
    {
        return m_tColorUnmodified;
    }
    return NodeRGBA::getColor();
}

void AtlasNode::setColor(const Color3B& color3)
{
    Color3B tmp = color3;
    m_tColorUnmodified = color3;

    if( m_bIsOpacityModifyRGB )
    {
        tmp.r = tmp.r * m_cDisplayedOpacity/255;
        tmp.g = tmp.g * m_cDisplayedOpacity/255;
        tmp.b = tmp.b * m_cDisplayedOpacity/255;
    }
    NodeRGBA::setColor(tmp);
}

void AtlasNode::setOpacity(GLubyte opacity)
{
    NodeRGBA::setOpacity(opacity);

    // special opacity for premultiplied textures
    if( m_bIsOpacityModifyRGB )
        this->setColor(m_tColorUnmodified);
}

void AtlasNode::setOpacityModifyRGB(bool bValue)
{
    Color3B oldColor = this->getColor();
    m_bIsOpacityModifyRGB = bValue;
    this->setColor(oldColor);
}

bool AtlasNode::isOpacityModifyRGB() const
{
    return m_bIsOpacityModifyRGB;
}

void AtlasNode::updateOpacityModifyRGB()
{
    m_bIsOpacityModifyRGB = m_pTextureAtlas->getTexture()->hasPremultipliedAlpha();
}

void AtlasNode::setIgnoreContentScaleFactor(bool bIgnoreContentScaleFactor)
{
    m_bIgnoreContentScaleFactor = bIgnoreContentScaleFactor;
}

// AtlasNode - CocosNodeTexture protocol

const BlendFunc& AtlasNode::getBlendFunc() const
{
    return m_tBlendFunc;
}

void AtlasNode::setBlendFunc(const BlendFunc &blendFunc)
{
    m_tBlendFunc = blendFunc;
}

void AtlasNode::updateBlendFunc()
{
    if( ! m_pTextureAtlas->getTexture()->hasPremultipliedAlpha() )
        m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
}

void AtlasNode::setTexture(Texture2D *texture)
{
    m_pTextureAtlas->setTexture(texture);
    this->updateBlendFunc();
    this->updateOpacityModifyRGB();
}

Texture2D * AtlasNode::getTexture() const
{
    return m_pTextureAtlas->getTexture();
}

void AtlasNode::setTextureAtlas(TextureAtlas* textureAtlas)
{
    CC_SAFE_RETAIN(textureAtlas);
    CC_SAFE_RELEASE(m_pTextureAtlas);
    m_pTextureAtlas = textureAtlas;
}

TextureAtlas * AtlasNode::getTextureAtlas() const
{
    return m_pTextureAtlas;
}

KDint32 AtlasNode::getQuadsToDraw ( KDvoid ) const
{
    return m_nQuadsToDraw;
}

void AtlasNode::setQuadsToDraw ( KDint32 nQuadsToDraw )
{
    m_nQuadsToDraw = nQuadsToDraw;
}

NS_CC_END
