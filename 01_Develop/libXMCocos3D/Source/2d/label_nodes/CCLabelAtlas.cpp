/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelAtlas.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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

#include "2d/label_nodes/CCLabelAtlas.h"
#include "2d/textures/CCTextureAtlas.h"
#include "2d/textures/CCTextureCache.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include "ccConfig.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "2d/CCDirector.h"
#include "support/TransformUtils.h"
#include "base/CCInteger.h"
#include "platform/CCFileUtils.h"
// external
#include "XMKazmath/GL/matrix.h"

NS_CC_BEGIN

//CCLabelAtlas - Creation & Init

LabelAtlas* LabelAtlas::create(const std::string& string, const std::string& charMapFile, KDint32 itemWidth, KDint32 itemHeight, KDint32 startCharMap)
{
    LabelAtlas *pRet = new LabelAtlas();
    if(pRet && pRet->initWithString(string, charMapFile, itemWidth, itemHeight, startCharMap))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool LabelAtlas::initWithString(const std::string& string, const std::string& charMapFile, KDint32 itemWidth, KDint32 itemHeight, KDint32 startCharMap)
{
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(charMapFile);
	return initWithString(string, texture, itemWidth, itemHeight, startCharMap);
}

bool LabelAtlas::initWithString(const std::string& string, Texture2D* texture, KDint32 itemWidth, KDint32 itemHeight, KDint32 startCharMap)
{
    if (AtlasNode::initWithTexture(texture, itemWidth, itemHeight, string.size()))
    {
        m_nMapStartChar = startCharMap;
        this->setString(string);
        return true;
    }
    return false;
}

LabelAtlas* LabelAtlas::create(const std::string& string, const std::string& fntFile)
{    
    LabelAtlas *ret = new LabelAtlas();
    if (ret)
    {
        if (ret->initWithString(string, fntFile))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE(ret);
        }
    }
    
    return ret;
}

bool LabelAtlas::initWithString(const std::string& theString, const std::string& fntFile)
{
    std::string pathStr = FileUtils::getInstance()->fullPathForFilename(fntFile);
    std::string relPathStr = pathStr.substr(0, pathStr.find_last_of("/"))+"/";
    Dictionary *dict = Dictionary::createWithContentsOfFile(pathStr.c_str());

    CCASSERT(((String*)dict->objectForKey("version"))->intValue() == 1, "Unsupported version. Upgrade cocos2d version");

    std::string texturePathStr = relPathStr + ((String*)dict->objectForKey("textureFilename"))->getCString();
    String *textureFilename = String::create(texturePathStr);
    unsigned int width = ((String*)dict->objectForKey("itemWidth"))->intValue() / CC_CONTENT_SCALE_FACTOR();
    unsigned int height = ((String*)dict->objectForKey("itemHeight"))->intValue() / CC_CONTENT_SCALE_FACTOR();
    unsigned int startChar = ((String*)dict->objectForKey("firstChar"))->intValue();


    this->initWithString(theString, textureFilename->getCString(), width, height, startChar);

    return true;
}

//CCLabelAtlas - Atlas generation
void LabelAtlas::updateAtlasValues()
{
    size_t n = m_sString.length();

    const unsigned char *s = (unsigned char*)m_sString.c_str();

    Texture2D *texture = m_pTextureAtlas->getTexture();
    float textureWide = (float) texture->getPixelsWide();
    float textureHigh = (float) texture->getPixelsHigh();
    float itemWidthInPixels = m_nItemWidth * CC_CONTENT_SCALE_FACTOR();
    float itemHeightInPixels = m_nItemHeight * CC_CONTENT_SCALE_FACTOR();
    if (m_bIgnoreContentScaleFactor)
    {
        itemWidthInPixels = m_nItemWidth;
        itemHeightInPixels = m_nItemHeight;
    }

    CCASSERT( static_cast<long>(n) <= m_pTextureAtlas->getCapacity(), "updateAtlasValues: Invalid String length");
    V3F_C4B_T2F_Quad* quads = m_pTextureAtlas->getQuads();
    for(long i = 0; i < static_cast<long>(n); i++) {

        unsigned char a = s[i] - m_nMapStartChar;
        float row = (float) (a % m_nItemsPerRow);
        float col = (float) (a / m_nItemsPerRow);

#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        // Issue #938. Don't use texStepX & texStepY
        float left        = (2 * row * itemWidthInPixels + 1) / (2 * textureWide);
        float right        = left + (itemWidthInPixels * 2 - 2) / (2 * textureWide);
        float top        = (2 * col * itemHeightInPixels + 1) / (2 * textureHigh);
        float bottom    = top + (itemHeightInPixels * 2 - 2) / (2 * textureHigh);
#else
        float left        = row * itemWidthInPixels / textureWide;
        float right        = left + itemWidthInPixels / textureWide;
        float top        = col * itemHeightInPixels / textureHigh;
        float bottom    = top + itemHeightInPixels / textureHigh;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL

        quads[i].tl.texCoords.u = left;
        quads[i].tl.texCoords.v = top;
        quads[i].tr.texCoords.u = right;
        quads[i].tr.texCoords.v = top;
        quads[i].bl.texCoords.u = left;
        quads[i].bl.texCoords.v = bottom;
        quads[i].br.texCoords.u = right;
        quads[i].br.texCoords.v = bottom;

        quads[i].bl.vertices.x = (float) (i * m_nItemWidth);
        quads[i].bl.vertices.y = 0;
        quads[i].bl.vertices.z = 0.0f;
        quads[i].br.vertices.x = (float)(i * m_nItemWidth + m_nItemWidth);
        quads[i].br.vertices.y = 0;
        quads[i].br.vertices.z = 0.0f;
        quads[i].tl.vertices.x = (float)(i * m_nItemWidth);
        quads[i].tl.vertices.y = (float)(m_nItemHeight);
        quads[i].tl.vertices.z = 0.0f;
        quads[i].tr.vertices.x = (float)(i * m_nItemWidth + m_nItemWidth);
        quads[i].tr.vertices.y = (float)(m_nItemHeight);
        quads[i].tr.vertices.z = 0.0f;
        Color4B c(m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity);
        quads[i].tl.colors = c;
        quads[i].tr.colors = c;
        quads[i].bl.colors = c;
        quads[i].br.colors = c;
    }
    if (n > 0 ){
        m_pTextureAtlas->setDirty(true);
        long totalQuads = m_pTextureAtlas->getTotalQuads();
        if (static_cast<long>(n) > totalQuads) {
            m_pTextureAtlas->increaseTotalQuadsWith(n - totalQuads);
        }
    }
}

//CCLabelAtlas - LabelProtocol
void LabelAtlas::setString(const std::string &label)
{
    size_t len = label.size();
    if (static_cast<long>(len) > m_pTextureAtlas->getTotalQuads())
    {
        m_pTextureAtlas->resizeCapacity(len);
    }
    m_sString.clear();
    m_sString = label;
    this->updateAtlasValues();

    Size s = Size(len * m_nItemWidth, m_nItemHeight);

    this->setContentSize(s);

    m_nQuadsToDraw = len;
}

const std::string& LabelAtlas::getString(void) const
{
    return m_sString;
}

//CCLabelAtlas - draw

#if CC_LABELATLAS_DEBUG_DRAW    
void LabelAtlas::draw()
{
    AtlasNode::draw();

    const Size& s = this->getContentSize();
    Point vertices[4]={
        Point(0,0),Point(s.width,0),
        Point(s.width,s.height),Point(0,s.height),
    };
    ccDrawPoly(vertices, 4, true);
}
#endif

NS_CC_END
