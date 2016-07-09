/* -----------------------------------------------------------------------------------
 *
 *      File            UILoadingBar.cpp
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

#include "gui/UIWidgets/UILoadingBar.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#include "2d/sprite_nodes/CCSprite.h"

namespace gui {


#define DYNAMIC_CAST_CCSPRITE dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)

UILoadingBar::UILoadingBar():
m_eBarType(LoadingBarTypeLeft),
m_nPercent(100),
m_fTotalLength(0),
m_pBarRenderer(nullptr),
m_eRenderBarTexType(UI_TEX_TYPE_LOCAL),
m_tBarRendererTextureSize(cocos2d::Size::ZERO),
m_bScale9Enabled(false),
m_bPrevIgnoreSize(true),
m_tCapInsets(cocos2d::Rect::ZERO),
m_sTextureFile("")
{
}

UILoadingBar::~UILoadingBar()
{
    
}

UILoadingBar* UILoadingBar::create()
{
    UILoadingBar* widget = new UILoadingBar();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

void UILoadingBar::initRenderer()
{
    UIWidget::initRenderer();
    m_pBarRenderer = cocos2d::Sprite::create();
    m_pRenderer->addChild(m_pBarRenderer);
    m_pBarRenderer->setAnchorPoint(cocos2d::Point(0.0,0.5));
}

void UILoadingBar::setDirection(LoadingBarType dir)
{
    if (m_eBarType == dir)
    {
        return;
    }
    m_eBarType = dir;

    switch (m_eBarType)
    {
        case LoadingBarTypeLeft:
            m_pBarRenderer->setAnchorPoint(cocos2d::Point(0.0f,0.5f));
            m_pBarRenderer->setPosition(cocos2d::Point(-m_fTotalLength*0.5f,0.0f));
            if (!m_bScale9Enabled)
            {
                dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->setFlippedX(false);
            }
            break;
        case LoadingBarTypeRight:
            m_pBarRenderer->setAnchorPoint(cocos2d::Point(1.0f,0.5f));
            m_pBarRenderer->setPosition(cocos2d::Point(m_fTotalLength*0.5f,0.0f));
            if (!m_bScale9Enabled)
            {
                dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->setFlippedX(true);
            }
            break;
    }
}

int UILoadingBar::getDirection()
{
    return m_eBarType;
}

void UILoadingBar::loadTexture(const char* texture,TextureResType texType)
{
    if (!texture || strcmp(texture, "") == 0)
    {
        return;
    }
    m_eRenderBarTexType = texType;
    m_sTextureFile = texture;
    switch (m_eRenderBarTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            if (m_bScale9Enabled)
            {
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->initWithFile(texture);
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->setCapInsets(m_tCapInsets);
            }
            else
            {
                dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->initWithFile(texture);
            }
            break;
        case UI_TEX_TYPE_PLIST:
            if (m_bScale9Enabled)
            {
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->initWithSpriteFrameName(texture);
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->setCapInsets(m_tCapInsets);
            }
            else
            {
                dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->initWithSpriteFrameName(texture);
            }
            break;
        default:
            break;
    }
    if (m_bScale9Enabled)
    {
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->setOpacity(getOpacity());
        
    }
    else
    {
        dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->setOpacity(getOpacity());
    }
    m_tBarRendererTextureSize = m_pBarRenderer->getContentSize();
    
    switch (m_eBarType)
    {
    case LoadingBarTypeLeft:
        m_pBarRenderer->setAnchorPoint(cocos2d::Point(0.0f,0.5f));
        if (!m_bScale9Enabled)
        {
            dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->setFlippedX(false);
        }
        break;
    case LoadingBarTypeRight:
        m_pBarRenderer->setAnchorPoint(cocos2d::Point(1.0f,0.5f));
        if (!m_bScale9Enabled)
        {
            dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->setFlippedX(true);
        }
        break;
    }
    barRendererScaleChangedWithSize();
}

void UILoadingBar::setScale9Enabled(bool enabled)
{
    if (m_bScale9Enabled == enabled)
    {
        return;
    }
    m_bScale9Enabled = enabled;
    m_pRenderer->removeChild(m_pBarRenderer, true);
    m_pBarRenderer = nullptr;
    if (m_bScale9Enabled)
    {
        m_pBarRenderer = cocos2d::extension::Scale9Sprite::create();
    }
    else
    {
        m_pBarRenderer = cocos2d::Sprite::create();
    }
    loadTexture(m_sTextureFile.c_str(),m_eRenderBarTexType);
    m_pRenderer->addChild(m_pBarRenderer);
    if (m_bScale9Enabled)
    {
        bool ignoreBefore = m_bIgnoreSize;
        ignoreContentAdaptWithSize(false);
        m_bPrevIgnoreSize = ignoreBefore;
    }
    else
    {
        ignoreContentAdaptWithSize(m_bPrevIgnoreSize);
    }
    setCapInsets(m_tCapInsets);
}

void UILoadingBar::setCapInsets(const cocos2d::Rect &capInsets)
{
    m_tCapInsets = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->setCapInsets(capInsets);
}

void UILoadingBar::setPercent(int percent)
{
    if ( percent < 0 || percent > 100)
    {
        return;
    }
    if (m_fTotalLength <= 0)
    {
        return;
    }
    m_nPercent = percent;
    float res = m_nPercent/100.0;
    
    int x = 0, y = 0;
    switch (m_eRenderBarTexType)
    {
        case UI_TEX_TYPE_PLIST:
        {
            cocos2d::Sprite* barNode = DYNAMIC_CAST_CCSPRITE;
            if (barNode)
            {
                cocos2d::Point to = barNode->getTextureRect().origin;
                x = to.x;
                y = to.y;
            }
            break;
        }
        default:
            break;
    }
    if (m_bScale9Enabled)
    {
        setScale9Scale();
    }
    else
    {
        dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->setTextureRect(cocos2d::Rect(x, y, m_tBarRendererTextureSize.width * res, m_tBarRendererTextureSize.height));
    }
}

int UILoadingBar::getPercent()
{
    return m_nPercent;
}

void UILoadingBar::onSizeChanged()
{
    barRendererScaleChangedWithSize();
}

void UILoadingBar::ignoreContentAdaptWithSize(bool ignore)
{
    if (!m_bScale9Enabled || (m_bScale9Enabled && !ignore))
    {
        UIWidget::ignoreContentAdaptWithSize(ignore);
        m_bPrevIgnoreSize = ignore;
    }
}

const cocos2d::Size& UILoadingBar::getContentSize() const
{
    return m_tBarRendererTextureSize;
}

cocos2d::Node* UILoadingBar::getVirtualRenderer()
{
    return m_pBarRenderer;
}

void UILoadingBar::barRendererScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        if (!m_bScale9Enabled)
        {
            m_fTotalLength = m_tBarRendererTextureSize.width;
            m_pBarRenderer->setScale(1.0f);
            m_tSize = m_tBarRendererTextureSize;
        }
    }
    else
    {
        m_fTotalLength = m_tSize.width;
        if (m_bScale9Enabled)
        {
            setScale9Scale();
        }
        else
        {
            
            cocos2d::Size textureSize = m_tBarRendererTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                m_pBarRenderer->setScale(1.0f);
                return;
            }
            float scaleX = m_tSize.width / textureSize.width;
            float scaleY = m_tSize.height / textureSize.height;
            m_pBarRenderer->setScaleX(scaleX);
            m_pBarRenderer->setScaleY(scaleY);
        }
    }
    switch (m_eBarType)
    {
        case LoadingBarTypeLeft:
            m_pBarRenderer->setPosition(cocos2d::Point(-m_fTotalLength * 0.5f, 0.0f));
            break;
        case LoadingBarTypeRight:
            m_pBarRenderer->setPosition(cocos2d::Point(m_fTotalLength * 0.5f, 0.0f));
            break;
        default:
            break;
    }
}

void UILoadingBar::setScale9Scale()
{
    float width = (float)(m_nPercent) / 100 * m_fTotalLength;
    dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->setPreferredSize(cocos2d::Size(width, m_tSize.height));
}

const char* UILoadingBar::getDescription() const
{
    return "LoadingBar";
}

UIWidget* UILoadingBar::createCloneInstance()
{
    return UILoadingBar::create();
}

void UILoadingBar::copySpecialProperties(UIWidget *widget)
{
    UILoadingBar* loadingBar = dynamic_cast<UILoadingBar*>(widget);
    if (loadingBar)
    {
        m_bPrevIgnoreSize = loadingBar->m_bPrevIgnoreSize;
        setScale9Enabled(loadingBar->m_bScale9Enabled);
        loadTexture(loadingBar->m_sTextureFile.c_str(), loadingBar->m_eRenderBarTexType);
        setCapInsets(loadingBar->m_tCapInsets);
        setPercent(loadingBar->m_nPercent);
    }
}

}