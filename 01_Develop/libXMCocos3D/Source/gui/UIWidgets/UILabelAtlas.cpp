/* -----------------------------------------------------------------------------------
 *
 *      File            UILabelAtlas.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#include "gui/UIWidgets/UILabelAtlas.h"

namespace gui {


UICCLabelAtlas::UICCLabelAtlas()
{
    
}

UICCLabelAtlas::~UICCLabelAtlas()
{
    
}

UICCLabelAtlas* UICCLabelAtlas::create()
{
    UICCLabelAtlas *pRet = new UICCLabelAtlas();
    if(pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    
    return nullptr;
}

void UICCLabelAtlas::setProperty(const std::string& string, const std::string& charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap)
{
    initWithString(string, charMapFile, itemWidth, itemHeight, startCharMap);
}

void UICCLabelAtlas::setProperty(const std::string& string, cocos2d::Texture2D *texture, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap)
{
    initWithString(string, texture, itemWidth, itemHeight, startCharMap);
}

void UICCLabelAtlas::draw()
{
    if (!m_pTextureAtlas)
    {
        return;
    }
    
    cocos2d::AtlasNode::draw();
}

void UICCLabelAtlas::updateDisplayedOpacity(GLubyte opacity)
{
    cocos2d::AtlasNode::setOpacity(opacity);
}




UILabelAtlas::UILabelAtlas():
m_pLabelAtlasRenderer(nullptr),
m_sStringValue(""),
m_sCharMapFileName(""),
m_nItemWidth(0),
m_nItemHeight(0),
m_sStartCharMap("")
{
    
}

UILabelAtlas::~UILabelAtlas()
{
    
}

UILabelAtlas* UILabelAtlas::create()
{
    UILabelAtlas* widget = new UILabelAtlas();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

void UILabelAtlas::initRenderer()
{
    UIWidget::initRenderer();
    m_pLabelAtlasRenderer = UICCLabelAtlas::create();
    m_pRenderer->addChild(m_pLabelAtlasRenderer);
}

void UILabelAtlas::setProperty(const std::string& stringValue, const std::string& charMapFile, int itemWidth, int itemHeight, const std::string& startCharMap)
{
    m_sStringValue = stringValue;
    m_sCharMapFileName = charMapFile;
    m_nItemWidth = itemWidth;
    m_nItemHeight = itemHeight;
    m_sStartCharMap = startCharMap;
    m_pLabelAtlasRenderer->setProperty(stringValue, charMapFile, itemWidth, itemHeight, (int)(startCharMap[0]));
    updateAnchorPoint();
    labelAtlasScaleChangedWithSize();
}

void UILabelAtlas::setStringValue(const std::string& value)
{
    m_sStringValue = value;
    m_pLabelAtlasRenderer->setString(value);
    labelAtlasScaleChangedWithSize();
}

const std::string& UILabelAtlas::getStringValue() const
{
    return m_pLabelAtlasRenderer->getString();
}

void UILabelAtlas::setAnchorPoint(const cocos2d::Point &pt)
{
    UIWidget::setAnchorPoint(pt);
    m_pLabelAtlasRenderer->setAnchorPoint(cocos2d::Point(pt.x, pt.y));
}

void UILabelAtlas::onSizeChanged()
{
    labelAtlasScaleChangedWithSize();
}

const cocos2d::Size& UILabelAtlas::getContentSize() const
{
    return m_pLabelAtlasRenderer->getContentSize();
}

cocos2d::Node* UILabelAtlas::getVirtualRenderer()
{
    return m_pLabelAtlasRenderer;
}

void UILabelAtlas::labelAtlasScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        m_pLabelAtlasRenderer->setScale(1.0f);
        m_tSize = m_pLabelAtlasRenderer->getContentSize();
    }
    else
    {
        cocos2d::Size textureSize = m_pLabelAtlasRenderer->getContentSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            m_pLabelAtlasRenderer->setScale(1.0f);
            return;
        }
        float scaleX = m_tSize.width / textureSize.width;
        float scaleY = m_tSize.height / textureSize.height;
        m_pLabelAtlasRenderer->setScaleX(scaleX);
        m_pLabelAtlasRenderer->setScaleY(scaleY);
    }
}

const char* UILabelAtlas::getDescription() const
{
    return "LabelAtlas";
}

UIWidget* UILabelAtlas::createCloneInstance()
{
    return UILabelAtlas::create();
}

void UILabelAtlas::copySpecialProperties(UIWidget *widget)
{
    UILabelAtlas* labelAtlas = dynamic_cast<UILabelAtlas*>(widget);
    if (labelAtlas)
    {
        setProperty(labelAtlas->m_sStringValue, labelAtlas->m_sCharMapFileName, labelAtlas->m_nItemWidth, labelAtlas->m_nItemHeight, labelAtlas->m_sStartCharMap);
    }
}
    
}