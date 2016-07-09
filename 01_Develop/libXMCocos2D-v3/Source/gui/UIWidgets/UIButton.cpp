/* -----------------------------------------------------------------------------------
 *
 *      File            UIButton.cpp
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

#include "gui/UIWidgets/UIButton.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#include "2d/actions/CCActionInterval.h"

namespace gui {

#define NORMALRENDERERZ (0)
#define PRESSEDRENDERERZ (0)
#define DISABLEDRENDERERZ (0)
#define TITLERENDERERZ (1)
    
UIButton::UIButton():
m_pButtonNormalRenderer(nullptr),
m_pButtonClickedRenderer(nullptr),
m_pButtonDisableRenderer(nullptr),
m_pTitleRenderer(nullptr),
m_sNormalFileName(""),
m_sClickedFileName(""),
m_sDisabledFileName(""),
m_bPrevIgnoreSize(true),
m_bScale9Enabled(false),
m_tCapInsetsNormal(cocos2d::Rect::ZERO),
m_tCapInsetsPressed(cocos2d::Rect::ZERO),
m_tCapInsetsDisabled(cocos2d::Rect::ZERO),
m_eNormalTexType(UI_TEX_TYPE_LOCAL),
m_ePressedTexType(UI_TEX_TYPE_LOCAL),
m_eDisabledTexType(UI_TEX_TYPE_LOCAL),
m_tNormalTextureSize(m_tSize),
m_tPressedTextureSize(m_tSize),
m_tDisabledTextureSize(m_tSize),
m_bPressedActionEnabled(false),
m_tTitleColor(cocos2d::Color3B::WHITE)
{
    
}

UIButton::~UIButton()
{
}

UIButton* UIButton::create()
{
    UIButton* widget = new UIButton();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool UIButton::init()
{
    if (UIWidget::init())
    {
        return true;
    }
    return false;
}

void UIButton::initRenderer()
{
    UIWidget::initRenderer();
    m_pButtonNormalRenderer = cocos2d::Sprite::create();
    m_pButtonClickedRenderer = cocos2d::Sprite::create();
    m_pButtonDisableRenderer = cocos2d::Sprite::create();
    m_pTitleRenderer = cocos2d::LabelTTF::create();
    m_pRenderer->addChild(m_pButtonNormalRenderer,NORMALRENDERERZ);
    m_pRenderer->addChild(m_pButtonClickedRenderer,PRESSEDRENDERERZ);
    m_pRenderer->addChild(m_pButtonDisableRenderer,DISABLEDRENDERERZ);
    m_pRenderer->addChild(m_pTitleRenderer,TITLERENDERERZ);
}

void UIButton::setScale9Enabled(bool able)
{
    if (m_bScale9Enabled == able)
    {
        return;
    }
    m_eBrightStyle = BRIGHT_NONE;
    m_bScale9Enabled = able;

    
    m_pRenderer->removeChild(m_pButtonNormalRenderer, true);
    m_pRenderer->removeChild(m_pButtonClickedRenderer, true);
    m_pRenderer->removeChild(m_pButtonDisableRenderer, true);
    
    m_pButtonNormalRenderer = nullptr;
    m_pButtonClickedRenderer = nullptr;
    m_pButtonDisableRenderer = nullptr;
    if (m_bScale9Enabled)
    {
        m_pButtonNormalRenderer = cocos2d::extension::Scale9Sprite::create();
        m_pButtonClickedRenderer = cocos2d::extension::Scale9Sprite::create();
        m_pButtonDisableRenderer = cocos2d::extension::Scale9Sprite::create();
    }
    else
    {
        m_pButtonNormalRenderer = cocos2d::Sprite::create();
        m_pButtonClickedRenderer = cocos2d::Sprite::create();
        m_pButtonDisableRenderer = cocos2d::Sprite::create();
    }

    loadTextureNormal(m_sNormalFileName.c_str(), m_eNormalTexType);
    loadTexturePressed(m_sClickedFileName.c_str(), m_ePressedTexType);
    loadTextureDisabled(m_sDisabledFileName.c_str(), m_eDisabledTexType);
    m_pRenderer->addChild(m_pButtonNormalRenderer,NORMALRENDERERZ);
    m_pRenderer->addChild(m_pButtonClickedRenderer,PRESSEDRENDERERZ);
    m_pRenderer->addChild(m_pButtonDisableRenderer,DISABLEDRENDERERZ);
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
    setCapInsetsNormalRenderer(m_tCapInsetsNormal);
    setCapInsetsPressedRenderer(m_tCapInsetsPressed);
    setCapInsetsDisabledRenderer(m_tCapInsetsDisabled);
    setBright(m_bBright);
}

void UIButton::ignoreContentAdaptWithSize(bool ignore)
{
    if (!m_bScale9Enabled || (m_bScale9Enabled && !ignore))
    {
        UIWidget::ignoreContentAdaptWithSize(ignore);
        m_bPrevIgnoreSize = ignore;
    }
}

void UIButton::loadTextures(const char* normal,const char* selected,const char* disabled,TextureResType texType)
{
    loadTextureNormal(normal,texType);
    loadTexturePressed(selected,texType);
    loadTextureDisabled(disabled,texType);
}

void UIButton::loadTextureNormal(const char* normal,TextureResType texType)
{
    if (!normal || strcmp(normal, "") == 0)
    {
        return;
    }
    m_sNormalFileName = normal;
    m_eNormalTexType = texType;
    if (m_bScale9Enabled)
    {
        switch (m_eNormalTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonNormalRenderer)->initWithFile(normal);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonNormalRenderer)->initWithSpriteFrameName(normal);
                break;
            default:
                break;
        }
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonNormalRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonNormalRenderer)->setOpacity(getOpacity());
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonNormalRenderer)->setCapInsets(m_tCapInsetsNormal);
    }
    else
    {
        switch (m_eNormalTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<cocos2d::Sprite*>(m_pButtonNormalRenderer)->initWithFile(normal);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<cocos2d::Sprite*>(m_pButtonNormalRenderer)->initWithSpriteFrameName(normal);
                break;
            default:
                break;
        }
        dynamic_cast<cocos2d::Sprite*>(m_pButtonNormalRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::Sprite*>(m_pButtonNormalRenderer)->setOpacity(getOpacity());
    }
    m_tNormalTextureSize = m_pButtonNormalRenderer->getContentSize();
    updateAnchorPoint();
    normalTextureScaleChangedWithSize();
}

void UIButton::loadTexturePressed(const char* selected,TextureResType texType)
{
    if (!selected || strcmp(selected, "") == 0)
    {
        return;
    }
    m_sClickedFileName = selected;
    m_ePressedTexType = texType;
    if (m_bScale9Enabled)
    {
        switch (m_ePressedTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonClickedRenderer)->initWithFile(selected);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonClickedRenderer)->initWithSpriteFrameName(selected);
                break;
            default:
                break;
        }
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonClickedRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonClickedRenderer)->setOpacity(getOpacity());
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonClickedRenderer)->setCapInsets(m_tCapInsetsPressed);
    }
    else
    {
        switch (m_ePressedTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<cocos2d::Sprite*>(m_pButtonClickedRenderer)->initWithFile(selected);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<cocos2d::Sprite*>(m_pButtonClickedRenderer)->initWithSpriteFrameName(selected);
                break;
            default:
                break;
        }
        dynamic_cast<cocos2d::Sprite*>(m_pButtonClickedRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::Sprite*>(m_pButtonClickedRenderer)->setOpacity(getOpacity());
    }
    m_tPressedTextureSize = m_pButtonClickedRenderer->getContentSize();
    updateAnchorPoint();
    pressedTextureScaleChangedWithSize();
}

void UIButton::loadTextureDisabled(const char* disabled,TextureResType texType)
{
    if (!disabled || strcmp(disabled, "") == 0)
    {
        return;
    }
    m_sDisabledFileName = disabled;
    m_eDisabledTexType = texType;
    if (m_bScale9Enabled)
    {
        switch (m_eDisabledTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonDisableRenderer)->initWithFile(disabled);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonDisableRenderer)->initWithSpriteFrameName(disabled);
                break;
            default:
                break;
        }
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonDisableRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonDisableRenderer)->setOpacity(getOpacity());
        dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonDisableRenderer)->setCapInsets(m_tCapInsetsDisabled);
    }
    else
    {
        switch (m_eDisabledTexType)
        {
            case UI_TEX_TYPE_LOCAL:
                dynamic_cast<cocos2d::Sprite*>(m_pButtonDisableRenderer)->initWithFile(disabled);
                break;
            case UI_TEX_TYPE_PLIST:
                dynamic_cast<cocos2d::Sprite*>(m_pButtonDisableRenderer)->initWithSpriteFrameName(disabled);
                break;
            default:
                break;
        }
        dynamic_cast<cocos2d::Sprite*>(m_pButtonDisableRenderer)->setColor(getColor());
        dynamic_cast<cocos2d::Sprite*>(m_pButtonDisableRenderer)->setOpacity(getOpacity());
    }
    m_tDisabledTextureSize = m_pButtonDisableRenderer->getContentSize();
    updateAnchorPoint();
    disabledTextureScaleChangedWithSize();
}

void UIButton::setCapInsets(const cocos2d::Rect &capInsets)
{
    setCapInsetsNormalRenderer(capInsets);
    setCapInsetsPressedRenderer(capInsets);
    setCapInsetsDisabledRenderer(capInsets);
}

void UIButton::setCapInsetsNormalRenderer(const cocos2d::Rect &capInsets)
{
    m_tCapInsetsNormal = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonNormalRenderer)->setCapInsets(capInsets);
}

void UIButton::setCapInsetsPressedRenderer(const cocos2d::Rect &capInsets)
{
    m_tCapInsetsPressed = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonClickedRenderer)->setCapInsets(capInsets);
}

void UIButton::setCapInsetsDisabledRenderer(const cocos2d::Rect &capInsets)
{
    m_tCapInsetsDisabled = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonDisableRenderer)->setCapInsets(capInsets);
}

void UIButton::onPressStateChangedToNormal()
{
    m_pButtonNormalRenderer->setVisible(true);
    m_pButtonClickedRenderer->setVisible(false);
    m_pButtonDisableRenderer->setVisible(false);
    if (m_bPressedActionEnabled)
    {
        m_pButtonNormalRenderer->stopAllActions();
        m_pButtonClickedRenderer->stopAllActions();
        m_pButtonDisableRenderer->stopAllActions();
        cocos2d::Action *zoomAction = cocos2d::ScaleTo::create(0.05f, 1.0f);
        cocos2d::Action *zoomAction1 = cocos2d::ScaleTo::create(0.05f, 1.0f);
        cocos2d::Action *zoomAction2 = cocos2d::ScaleTo::create(0.05f, 1.0f);
        m_pButtonNormalRenderer->runAction(zoomAction);
        m_pButtonClickedRenderer->runAction(zoomAction1);
        m_pButtonDisableRenderer->runAction(zoomAction2);
    }
}

void UIButton::onPressStateChangedToPressed()
{
    m_pButtonNormalRenderer->setVisible(false);
    m_pButtonClickedRenderer->setVisible(true);
    m_pButtonDisableRenderer->setVisible(false);
    if (m_bPressedActionEnabled)
    {
        m_pButtonNormalRenderer->stopAllActions();
        m_pButtonClickedRenderer->stopAllActions();
        m_pButtonDisableRenderer->stopAllActions();
        cocos2d::Action *zoomAction = cocos2d::ScaleTo::create(0.05f, 1.1f);
        cocos2d::Action *zoomAction1 = cocos2d::ScaleTo::create(0.05f, 1.1f);
        cocos2d::Action *zoomAction2 = cocos2d::ScaleTo::create(0.05f, 1.1f);
        m_pButtonNormalRenderer->runAction(zoomAction);
        m_pButtonClickedRenderer->runAction(zoomAction1);
        m_pButtonDisableRenderer->runAction(zoomAction2);
    }
}

void UIButton::onPressStateChangedToDisabled()
{
    m_pButtonNormalRenderer->setVisible(false);
    m_pButtonClickedRenderer->setVisible(false);
    m_pButtonDisableRenderer->setVisible(true);
}

void UIButton::setFlipX(bool flipX)
{
    m_pTitleRenderer->setFlippedX(flipX);
    if (m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<cocos2d::Sprite*>(m_pButtonNormalRenderer)->setFlippedX(flipX);
    dynamic_cast<cocos2d::Sprite*>(m_pButtonClickedRenderer)->setFlippedX(flipX);
    dynamic_cast<cocos2d::Sprite*>(m_pButtonDisableRenderer)->setFlippedX(flipX);
}

void UIButton::setFlipY(bool flipY)
{
    m_pTitleRenderer->setFlippedY(flipY);
    if (m_bScale9Enabled)
    {
        return;
    }
    dynamic_cast<cocos2d::Sprite*>(m_pButtonNormalRenderer)->setFlippedY(flipY);
    dynamic_cast<cocos2d::Sprite*>(m_pButtonClickedRenderer)->setFlippedY(flipY);
    dynamic_cast<cocos2d::Sprite*>(m_pButtonDisableRenderer)->setFlippedY(flipY);
}

bool UIButton::isFlipX()
{
    if (m_bScale9Enabled)
    {
        return false;
    }
    return dynamic_cast<cocos2d::Sprite*>(m_pButtonNormalRenderer)->isFlippedX();
}

bool UIButton::isFlipY()
{
    if (m_bScale9Enabled)
    {
        return false;
    }
    return dynamic_cast<cocos2d::Sprite*>(m_pButtonNormalRenderer)->isFlippedY();
}

void UIButton::setAnchorPoint(const cocos2d::Point &pt)
{
    UIWidget::setAnchorPoint(pt);
    m_pButtonNormalRenderer->setAnchorPoint(pt);
    m_pButtonClickedRenderer->setAnchorPoint(pt);
    m_pButtonDisableRenderer->setAnchorPoint(pt);
    m_pTitleRenderer->setPosition(cocos2d::Point(m_tSize.width*(0.5f-m_tAnchorPoint.x), m_tSize.height*(0.5f-m_tAnchorPoint.y)));
}

void UIButton::onSizeChanged()
{
    normalTextureScaleChangedWithSize();
    pressedTextureScaleChangedWithSize();
    disabledTextureScaleChangedWithSize();
}

const cocos2d::Size& UIButton::getContentSize() const
{
    return m_tNormalTextureSize;
}

cocos2d::Node* UIButton::getVirtualRenderer()
{
    if (m_bBright)
    {
        switch (m_eBrightStyle)
        {
            case BRIGHT_NORMAL:
                return m_pButtonNormalRenderer;
            case BRIGHT_HIGHLIGHT:
                return m_pButtonClickedRenderer;
            default:
                return nullptr;
        }
    }
    else
    {
        return m_pButtonDisableRenderer;
    }
}

void UIButton::normalTextureScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        if (!m_bScale9Enabled)
        {
            m_pButtonNormalRenderer->setScale(1.0f);
            m_tSize = m_tNormalTextureSize;
        }
    }
    else
    {
        if (m_bScale9Enabled)
        {
            dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonNormalRenderer)->setPreferredSize(m_tSize);
        }
        else
        {
            cocos2d::Size textureSize = m_tNormalTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                m_pButtonNormalRenderer->setScale(1.0f);
                return;
            }
            float scaleX = m_tSize.width / textureSize.width;
            float scaleY = m_tSize.height / textureSize.height;
            m_pButtonNormalRenderer->setScaleX(scaleX);
            m_pButtonNormalRenderer->setScaleY(scaleY);
        }
    }
}

void UIButton::pressedTextureScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        if (!m_bScale9Enabled)
        {
            m_pButtonClickedRenderer->setScale(1.0f);
        }
    }
    else
    {
        if (m_bScale9Enabled)
        {
            dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonClickedRenderer)->setPreferredSize(m_tSize);
        }
        else
        {
            cocos2d::Size textureSize = m_tPressedTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                m_pButtonClickedRenderer->setScale(1.0f);
                return;
            }
            float scaleX = m_tSize.width / m_tPressedTextureSize.width;
            float scaleY = m_tSize.height / m_tPressedTextureSize.height;
            m_pButtonClickedRenderer->setScaleX(scaleX);
            m_pButtonClickedRenderer->setScaleY(scaleY);
        }
    }
}

void UIButton::disabledTextureScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        if (!m_bScale9Enabled)
        {
            m_pButtonDisableRenderer->setScale(1.0f);
        }
    }
    else
    {
        if (m_bScale9Enabled)
        {
            dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pButtonDisableRenderer)->setPreferredSize(m_tSize);
        }
        else
        {
            cocos2d::Size textureSize = m_tDisabledTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                m_pButtonDisableRenderer->setScale(1.0f);
                return;
            }
            float scaleX = m_tSize.width / m_tDisabledTextureSize.width;
            float scaleY = m_tSize.height / m_tDisabledTextureSize.height;
            m_pButtonDisableRenderer->setScaleX(scaleX);
            m_pButtonDisableRenderer->setScaleY(scaleY);
        }
    }
}

void UIButton::setPressedActionEnabled(bool enabled)
{
    m_bPressedActionEnabled = enabled;
}

void UIButton::setTitleText(const std::string& text)
{
    m_pTitleRenderer->setString(text);
}

const std::string& UIButton::getTitleText() const
{
    return m_pTitleRenderer->getString();
}

void UIButton::setTitleColor(const cocos2d::Color3B& color)
{
    m_tTitleColor = color;
    m_pTitleRenderer->setColor(color);
}

const cocos2d::Color3B& UIButton::getTitleColor() const
{
    return m_pTitleRenderer->getColor();
}

void UIButton::setTitleFontSize(float size)
{
    m_pTitleRenderer->setFontSize(size);
}

float UIButton::getTitleFontSize() const
{
    return m_pTitleRenderer->getFontSize();
}

void UIButton::setTitleFontName(const char* fontName)
{
    m_pTitleRenderer->setFontName(fontName);
}

const char* UIButton::getTitleFontName() const
{
    return m_pTitleRenderer->getFontName().c_str();
}

void UIButton::setColor(const cocos2d::Color3B &color)
{
    UIWidget::setColor(color);
    setTitleColor(m_tTitleColor);
}

const char* UIButton::getDescription() const
{
    return "Button";
}

UIWidget* UIButton::createCloneInstance()
{
    return UIButton::create();
}

void UIButton::copySpecialProperties(UIWidget *widget)
{
    UIButton* button = dynamic_cast<UIButton*>(widget);
    if (button)
    {   
        m_bPrevIgnoreSize = button->m_bPrevIgnoreSize;
        setScale9Enabled(button->m_bScale9Enabled);
        loadTextureNormal(button->m_sNormalFileName.c_str(), button->m_eNormalTexType);
        loadTexturePressed(button->m_sClickedFileName.c_str(), button->m_ePressedTexType);
        loadTextureDisabled(button->m_sDisabledFileName.c_str(), button->m_eDisabledTexType);
        setCapInsetsNormalRenderer(button->m_tCapInsetsNormal);
        setCapInsetsPressedRenderer(button->m_tCapInsetsPressed);
        setCapInsetsDisabledRenderer(button->m_tCapInsetsDisabled);
        setTitleText(button->getTitleText());
        setTitleFontName(button->getTitleFontName());
        setTitleFontSize(button->getTitleFontSize());
        setTitleColor(button->getTitleColor());
        setPressedActionEnabled(button->m_bPressedActionEnabled);
    }
}

}