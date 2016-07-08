/* -----------------------------------------------------------------------------------
 *
 *      File            UISlider.cpp
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

#include "gui/UIWidgets/UISlider.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"

namespace gui {

UISlider::UISlider():
m_pBarRenderer(nullptr),
m_pProgressBarRenderer(nullptr),
m_tProgressBarTextureSize(cocos2d::Size::ZERO),
m_pSlidBallNormalRenderer(nullptr),
m_pSlidBallPressedRenderer(nullptr),
m_pSlidBallDisabledRenderer(nullptr),
m_pSlidBallRenderer(nullptr),
m_fBarLength(0.0),
m_nPercent(0),
m_bScale9Enabled(false),
m_bPrevIgnoreSize(true),
m_sTextureFile(""),
m_sProgressBarTextureFile(""),
m_sSlidBallNormalTextureFile(""),
m_sSlidBallPressedTextureFile(""),
m_sSlidBallDisabledTextureFile(""),
m_tCapInsetsBarRenderer(cocos2d::Rect::ZERO),
m_tCapInsetsProgressBarRenderer(cocos2d::Rect::ZERO),
m_pSliderEventListener(nullptr),
m_pSliderEventSelector(nullptr),
m_eBarTexType(UI_TEX_TYPE_LOCAL),
m_eProgressBarTexType(UI_TEX_TYPE_LOCAL),
m_eBallNTexType(UI_TEX_TYPE_LOCAL),
m_eBallPTexType(UI_TEX_TYPE_LOCAL),
m_eBallDTexType(UI_TEX_TYPE_LOCAL)
{
}

UISlider::~UISlider()
{
    m_pSliderEventListener = nullptr;
    m_pSliderEventSelector = nullptr;
}

UISlider* UISlider::create()
{
    UISlider* widget = new UISlider();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

void UISlider::initRenderer()
{
    UIWidget::initRenderer();
    m_pBarRenderer = cocos2d::Sprite::create();
    m_pProgressBarRenderer = cocos2d::Sprite::create();
    m_pProgressBarRenderer->setAnchorPoint(cocos2d::Point(0.0f, 0.5f));
    m_pRenderer->addChild(m_pBarRenderer, -1);
    m_pRenderer->addChild(m_pProgressBarRenderer, -1);
    m_pSlidBallNormalRenderer = cocos2d::Sprite::create();
    m_pSlidBallPressedRenderer = cocos2d::Sprite::create();
    m_pSlidBallPressedRenderer->setVisible(false);
    m_pSlidBallDisabledRenderer = cocos2d::Sprite::create();
    m_pSlidBallDisabledRenderer->setVisible(false);
    m_pSlidBallRenderer = cocos2d::Node::create();
    m_pSlidBallRenderer->addChild(m_pSlidBallNormalRenderer);
    m_pSlidBallRenderer->addChild(m_pSlidBallPressedRenderer);
    m_pSlidBallRenderer->addChild(m_pSlidBallDisabledRenderer);
    m_pRenderer->addChild(m_pSlidBallRenderer);
}

void UISlider::loadBarTexture(const char* fileName, TextureResType texType)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    m_sTextureFile = fileName;
    m_eBarTexType = texType;
    switch (m_eBarTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            if (m_bScale9Enabled)
            {
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->initWithFile(fileName);
            }
            else
            {
                dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->initWithFile(fileName);
            }
            break;
        case UI_TEX_TYPE_PLIST:
            if (m_bScale9Enabled)
            {
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->initWithSpriteFrameName(fileName);
            }
            else
            {
                dynamic_cast<cocos2d::Sprite*>(m_pBarRenderer)->initWithSpriteFrameName(fileName);
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
    barRendererScaleChangedWithSize();
}

void UISlider::loadProgressBarTexture(const char *fileName, TextureResType texType)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    m_sProgressBarTextureFile = fileName;
    m_eProgressBarTexType = texType;
    switch (m_eProgressBarTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            if (m_bScale9Enabled)
            {
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pProgressBarRenderer)->initWithFile(fileName);
            }
            else
            {
                dynamic_cast<cocos2d::Sprite*>(m_pProgressBarRenderer)->initWithFile(fileName);
            }
            break;
        case UI_TEX_TYPE_PLIST:
            if (m_bScale9Enabled)
            {
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pProgressBarRenderer)->initWithSpriteFrameName(fileName);
            }
            else
            {
                dynamic_cast<cocos2d::Sprite*>(m_pProgressBarRenderer)->initWithSpriteFrameName(fileName);
            }
            break;
        default:
            break;
    }
    if (m_bScale9Enabled)
    {
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pProgressBarRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pProgressBarRenderer)->setOpacity(getOpacity());
    }
    else
    {
        dynamic_cast<cocos2d::Sprite*>(m_pProgressBarRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::Sprite*>(m_pProgressBarRenderer)->setOpacity(getOpacity());
    }
    m_pProgressBarRenderer->setAnchorPoint(cocos2d::Point(0.0f, 0.5f));
    m_tProgressBarTextureSize = m_pProgressBarRenderer->getContentSize();
    progressBarRendererScaleChangedWithSize();
}

void UISlider::setScale9Enabled(bool able)
{
    if (m_bScale9Enabled == able)
    {
        return;
    }
    
    m_bScale9Enabled = able;
    m_pRenderer->removeChild(m_pBarRenderer, true);
    m_pRenderer->removeChild(m_pProgressBarRenderer, true);
    m_pBarRenderer = nullptr;
    m_pProgressBarRenderer = nullptr;
    if (m_bScale9Enabled)
    {
        m_pBarRenderer = cocos2d::extension::Scale9Sprite::create();
        m_pProgressBarRenderer = cocos2d::extension::Scale9Sprite::create();
    }
    else
    {
        m_pBarRenderer = cocos2d::Sprite::create();
        m_pProgressBarRenderer = cocos2d::Sprite::create();
    }
    loadBarTexture(m_sTextureFile.c_str(), m_eBarTexType);
    loadProgressBarTexture(m_sProgressBarTextureFile.c_str(), m_eProgressBarTexType);
    m_pRenderer->addChild(m_pBarRenderer, -1);
    m_pRenderer->addChild(m_pProgressBarRenderer, -1);
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
    setCapInsetsBarRenderer(m_tCapInsetsBarRenderer);
    setCapInsetProgressBarRebderer(m_tCapInsetsProgressBarRenderer);
}

void UISlider::ignoreContentAdaptWithSize(bool ignore)
{
    if (!m_bScale9Enabled || (m_bScale9Enabled && !ignore))
    {
        UIWidget::ignoreContentAdaptWithSize(ignore);
        m_bPrevIgnoreSize = ignore;
    }
}

void UISlider::setCapInsets(const cocos2d::Rect &capInsets)
{
    setCapInsetsBarRenderer(capInsets);
    setCapInsetProgressBarRebderer(capInsets);
}

void UISlider::setCapInsetsBarRenderer(const cocos2d::Rect &capInsets)
{
    m_tCapInsetsBarRenderer = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->setCapInsets(capInsets);
}

void UISlider::setCapInsetProgressBarRebderer(const cocos2d::Rect &capInsets)
{
    m_tCapInsetsProgressBarRenderer = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pProgressBarRenderer)->setCapInsets(capInsets);
}

void UISlider::loadSlidBallTextures(const char* normal,const char* pressed,const char* disabled,TextureResType texType)
{
    loadSlidBallTextureNormal(normal, texType);
    loadSlidBallTexturePressed(pressed,texType);
    loadSlidBallTextureDisabled(disabled,texType);
}

void UISlider::loadSlidBallTextureNormal(const char* normal,TextureResType texType)
{
    if (!normal || strcmp(normal, "") == 0)
    {
        return;
    }
    m_sSlidBallNormalTextureFile = normal;
    m_eBallNTexType = texType;
    switch (m_eBallNTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            m_pSlidBallNormalRenderer->initWithFile(normal);
            break;
        case UI_TEX_TYPE_PLIST:
            m_pSlidBallNormalRenderer->initWithSpriteFrameName(normal);
            break;
        default:
            break;
    }
    m_pSlidBallNormalRenderer->setColor(getColor());
    m_pSlidBallNormalRenderer->setOpacity(getOpacity());
}

void UISlider::loadSlidBallTexturePressed(const char* pressed,TextureResType texType)
{
    if (!pressed || strcmp(pressed, "") == 0)
    {
        return;
    }
    m_sSlidBallPressedTextureFile = pressed;
    m_eBallPTexType = texType;
    switch (m_eBallPTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            m_pSlidBallPressedRenderer->initWithFile(pressed);
            break;
        case UI_TEX_TYPE_PLIST:
            m_pSlidBallPressedRenderer->initWithSpriteFrameName(pressed);
            break;
        default:
            break;
    }
    m_pSlidBallPressedRenderer->setColor(getColor());
    m_pSlidBallPressedRenderer->setOpacity(getOpacity());
}

void UISlider::loadSlidBallTextureDisabled(const char* disabled,TextureResType texType)
{
    if (!disabled || strcmp(disabled, "") == 0)
    {
        return;
    }
    m_sSlidBallDisabledTextureFile = disabled;
    m_eBallDTexType = texType;
    switch (m_eBallDTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            m_pSlidBallDisabledRenderer->initWithFile(disabled);
            break;
        case UI_TEX_TYPE_PLIST:
            m_pSlidBallDisabledRenderer->initWithSpriteFrameName(disabled);
            break;
        default:
            break;
    }
    m_pSlidBallDisabledRenderer->setColor(getColor());
    m_pSlidBallDisabledRenderer->setOpacity(getOpacity());
}

void UISlider::setPercent(int percent)
{
    if (percent > 100)
    {
        percent = 100;
    }
    if (percent < 0)
    {
        percent = 0;
    }
    m_nPercent = percent;
    float dis = m_fBarLength*(percent/100.0f);
    m_pSlidBallRenderer->setPosition(cocos2d::Point(-m_fBarLength/2.0f + dis, 0.0f));
    if (m_bScale9Enabled)
    {
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pProgressBarRenderer)->setPreferredSize(cocos2d::Size(dis,m_tProgressBarTextureSize.height));
    }
    else
    {
        int x = 0, y = 0;
        switch (m_eProgressBarTexType)
        {
            case UI_TEX_TYPE_PLIST:
            {
                cocos2d::Sprite* barNode = dynamic_cast<cocos2d::Sprite*>(m_pProgressBarRenderer);
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
        dynamic_cast<cocos2d::Sprite*>(m_pProgressBarRenderer)->setTextureRect(cocos2d::Rect(x, y, m_tProgressBarTextureSize.width * (percent/100.0f), m_tProgressBarTextureSize.height));
    }
}

bool UISlider::onTouchBegan(const cocos2d::Point &touchPoint)
{
    bool pass = UIWidget::onTouchBegan(touchPoint);
    cocos2d::Point nsp = m_pRenderer->convertToNodeSpace(touchPoint);
    setPercent(getPercentWithBallPos(nsp.x));
    percentChangedEvent();
    return pass;
}

void UISlider::onTouchMoved(const cocos2d::Point &touchPoint)
{
    cocos2d::Point nsp = m_pRenderer->convertToNodeSpace(touchPoint);
    m_pSlidBallRenderer->setPosition(cocos2d::Point(nsp.x,0));
    setPercent(getPercentWithBallPos(nsp.x));
    percentChangedEvent();
}

void UISlider::onTouchEnded(const cocos2d::Point &touchPoint)
{
    UIWidget::onTouchEnded(touchPoint);
}

void UISlider::onTouchCancelled(const cocos2d::Point &touchPoint)
{
    UIWidget::onTouchCancelled(touchPoint);
}

float UISlider::getPercentWithBallPos(float px)
{
    return (((px-(-m_fBarLength/2.0f))/m_fBarLength)*100.0f);
}

void UISlider::addEventListenerSlider(cocos2d::Object *target, SEL_SlidPercentChangedEvent selector)
{
    m_pSliderEventListener = target;
    m_pSliderEventSelector = selector;
}

void UISlider::percentChangedEvent()
{
    if (m_pSliderEventListener && m_pSliderEventSelector)
    {
        (m_pSliderEventListener->*m_pSliderEventSelector)(this,SLIDER_PERCENTCHANGED);
    }
}

int UISlider::getPercent()
{
    return m_nPercent;
}

void UISlider::onSizeChanged()
{
    barRendererScaleChangedWithSize();
    progressBarRendererScaleChangedWithSize();
}

const cocos2d::Size& UISlider::getContentSize() const
{
    return m_pBarRenderer->getContentSize();
}

cocos2d::Node* UISlider::getVirtualRenderer()
{
    return m_pBarRenderer;
}

void UISlider::barRendererScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        
        m_pBarRenderer->setScale(1.0f);
        m_tSize = m_pBarRenderer->getContentSize();
        m_fBarLength = m_tSize.width;
    }
    else
    {
        m_fBarLength = m_tSize.width;
        if (m_bScale9Enabled)
        {
            dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pBarRenderer)->setPreferredSize(m_tSize);
        }
        else
        {
            cocos2d::Size btextureSize = m_pBarRenderer->getContentSize();
            if (btextureSize.width <= 0.0f || btextureSize.height <= 0.0f)
            {
                m_pBarRenderer->setScale(1.0f);
                return;
            }
            float bscaleX = m_tSize.width / btextureSize.width;
            float bscaleY = m_tSize.height / btextureSize.height;
            m_pBarRenderer->setScaleX(bscaleX);
            m_pBarRenderer->setScaleY(bscaleY);
        }
    }
    setPercent(m_nPercent);
}

void UISlider::progressBarRendererScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        if (!m_bScale9Enabled)
        {
            cocos2d::Size ptextureSize = m_tProgressBarTextureSize;
            float pscaleX = m_tSize.width / ptextureSize.width;
            float pscaleY = m_tSize.height / ptextureSize.height;
            m_pProgressBarRenderer->setScaleX(pscaleX);
            m_pProgressBarRenderer->setScaleY(pscaleY);
        }
    }
    else
    {
        if (m_bScale9Enabled)
        {
            dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pProgressBarRenderer)->setPreferredSize(m_tSize);
        }
        else
        {
            cocos2d::Size ptextureSize = m_tProgressBarTextureSize;
            if (ptextureSize.width <= 0.0f || ptextureSize.height <= 0.0f)
            {
                m_pProgressBarRenderer->setScale(1.0f);
                return;
            }
            float pscaleX = m_tSize.width / ptextureSize.width;
            float pscaleY = m_tSize.height / ptextureSize.height;
            m_pProgressBarRenderer->setScaleX(pscaleX);
            m_pProgressBarRenderer->setScaleY(pscaleY);
        }
    }
    m_pProgressBarRenderer->setPosition(cocos2d::Point(-m_fBarLength * 0.5f, 0.0f));
    setPercent(m_nPercent);
}

void UISlider::onPressStateChangedToNormal()
{
    m_pSlidBallNormalRenderer->setVisible(true);
    m_pSlidBallPressedRenderer->setVisible(false);
    m_pSlidBallDisabledRenderer->setVisible(false);
}

void UISlider::onPressStateChangedToPressed()
{
    m_pSlidBallNormalRenderer->setVisible(false);
    m_pSlidBallPressedRenderer->setVisible(true);
    m_pSlidBallDisabledRenderer->setVisible(false);
}

void UISlider::onPressStateChangedToDisabled()
{
    m_pSlidBallNormalRenderer->setVisible(false);
    m_pSlidBallPressedRenderer->setVisible(false);
    m_pSlidBallDisabledRenderer->setVisible(true);
}

const char* UISlider::getDescription() const
{
    return "Slider";
}

UIWidget* UISlider::createCloneInstance()
{
    return UISlider::create();
}

void UISlider::copySpecialProperties(UIWidget *widget)
{
    UISlider* slider = dynamic_cast<UISlider*>(widget);
    if (slider)
    {
        m_bPrevIgnoreSize = slider->m_bPrevIgnoreSize;
        setScale9Enabled(slider->m_bScale9Enabled);
        loadBarTexture(slider->m_sTextureFile.c_str(), slider->m_eBarTexType);
        loadProgressBarTexture(slider->m_sProgressBarTextureFile.c_str(), slider->m_eProgressBarTexType);
        loadSlidBallTextureNormal(slider->m_sSlidBallNormalTextureFile.c_str(), slider->m_eBallNTexType);
        loadSlidBallTexturePressed(slider->m_sSlidBallPressedTextureFile.c_str(), slider->m_eBallPTexType);
        loadSlidBallTextureDisabled(slider->m_sSlidBallDisabledTextureFile.c_str(), slider->m_eBallDTexType);
        setPercent(slider->getPercent());
    }
}

}