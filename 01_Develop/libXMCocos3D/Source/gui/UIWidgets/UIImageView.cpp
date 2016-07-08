/* -----------------------------------------------------------------------------------
 *
 *      File            UIImageView.cpp
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

#include "gui/UIWidgets/UIImageView.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#include "2d/sprite_nodes/CCSprite.h"

namespace gui {


#define DYNAMIC_CAST_CCSPRITE		dynamic_cast<cocos2d::Sprite*> ( m_pImageRenderer )
#define DYNAMIC_CAST_SCALE9SPRITE	dynamic_cast<cocos2d::extension::Scale9Sprite*> ( m_pImageRenderer )

UIImageView::UIImageView():
m_nClickCount(0),
m_fClickTimeInterval(0.0),
m_bStartCheckDoubleClick(false),
m_bTouchRelease(false),
m_bDoubleClickEnabled(false),
m_bScale9Enabled(false),
m_bPrevIgnoreSize(true),
m_tCapInsets(cocos2d::Rect::ZERO),
m_pImageRenderer(nullptr),
m_sTextureFile(""),
m_eImageTexType(UI_TEX_TYPE_LOCAL),
m_tImageTextureSize(m_tSize)
{

}

UIImageView::~UIImageView()
{
    
}

UIImageView* UIImageView::create()
{
    UIImageView* widget = new UIImageView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

void UIImageView::initRenderer()
{
    UIWidget::initRenderer();
    m_pImageRenderer = cocos2d::Sprite::create();
    m_pRenderer->addChild(m_pImageRenderer);
}

void UIImageView::loadTexture(const char *fileName, TextureResType texType)
{
    if (!fileName || strcmp(fileName, "") == 0)
    {
        return;
    }
    m_sTextureFile = fileName;
    m_eImageTexType = texType;
    switch (m_eImageTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            if (m_bScale9Enabled)
            {
                DYNAMIC_CAST_SCALE9SPRITE->initWithFile(fileName);
                DYNAMIC_CAST_SCALE9SPRITE->setColor(getColor());
                DYNAMIC_CAST_SCALE9SPRITE->setOpacity(getOpacity());
                DYNAMIC_CAST_SCALE9SPRITE->setCapInsets(m_tCapInsets);
            }
            else
            {
                DYNAMIC_CAST_CCSPRITE->initWithFile(fileName);
                DYNAMIC_CAST_CCSPRITE->setColor(getColor());
                DYNAMIC_CAST_CCSPRITE->setOpacity(getOpacity());
            }
            break;
        case UI_TEX_TYPE_PLIST:
            if (m_bScale9Enabled)
            {
                DYNAMIC_CAST_SCALE9SPRITE->initWithSpriteFrameName(fileName);
                DYNAMIC_CAST_SCALE9SPRITE->setColor(getColor());
                DYNAMIC_CAST_SCALE9SPRITE->setOpacity(getOpacity());
                DYNAMIC_CAST_SCALE9SPRITE->setCapInsets(m_tCapInsets);
            }
            else
            {
                DYNAMIC_CAST_CCSPRITE->initWithSpriteFrameName(fileName);
                DYNAMIC_CAST_CCSPRITE->setColor(getColor());
                DYNAMIC_CAST_CCSPRITE->setOpacity(getOpacity());
            }
            break;
        default:
            break;
    }
    m_tImageTextureSize = m_pImageRenderer->getContentSize();
    updateAnchorPoint();
    imageTextureScaleChangedWithSize();
}

void UIImageView::setTextureRect(const cocos2d::Rect &rect)
{
    if (m_bScale9Enabled)
    {
    }
    else
    {
        DYNAMIC_CAST_CCSPRITE->setTextureRect(rect);
    }
}

bool UIImageView::onTouchBegan(const cocos2d::Point &touchPoint)
{
    setFocused(true);
    m_tTouchStartPos.x = touchPoint.x;
    m_tTouchStartPos.y = touchPoint.y;
    m_pWidgetParent->checkChildInfo(0,this,touchPoint);
    pushDownEvent();
    
    if (m_bDoubleClickEnabled)
    {
        m_fClickTimeInterval = 0;
        m_bStartCheckDoubleClick = true;
        m_nClickCount++;
        m_bTouchRelease = false;
    }
    return m_bTouchPassedEnabled;
}

void UIImageView::onTouchEnded(const cocos2d::Point &touchPoint)
{
    if (m_bDoubleClickEnabled)
    {
        if (m_nClickCount >= 2)
        {
            doubleClickEvent();
            m_nClickCount = 0;
            m_bStartCheckDoubleClick = false;
        }
        else
        {
            m_bTouchRelease = true;
        }
    }
    else
    {
        UIWidget::onTouchEnded(touchPoint);
    }
}

void UIImageView::doubleClickEvent()
{
    
}

void UIImageView::checkDoubleClick(float dt)
{
    if (m_bStartCheckDoubleClick)
    {
        m_fClickTimeInterval += dt;
        if (m_fClickTimeInterval >= 200 && m_nClickCount > 0)
        {
            m_fClickTimeInterval = 0;
            m_nClickCount--;
            m_bStartCheckDoubleClick = false;
        }
    }
    else
    {
        if (m_nClickCount <= 1)
        {
            if (m_bTouchRelease)
            {
                releaseUpEvent();
                m_fClickTimeInterval = 0;
                m_nClickCount = 0;
                m_bTouchRelease = false;
            }
        }
    }
}

void UIImageView::setDoubleClickEnabled(bool able)
{
    if (able == m_bDoubleClickEnabled)
    {
        return;
    }
    m_bDoubleClickEnabled = able;
    if (able)
    {
//        COCOUISYSTEM->getUIInputManager()->addCheckedDoubleClickWidget(this);
    }
    else
    {
        
    }
}

void UIImageView::setFlipX(bool flipX)
{
    if (m_bScale9Enabled)
    {
    }
    else
    {
        DYNAMIC_CAST_CCSPRITE->setFlippedX(flipX);
    }
}

void UIImageView::setFlipY(bool flipY)
{
    if (m_bScale9Enabled)
    {
    }
    else
    {
        DYNAMIC_CAST_CCSPRITE->setFlippedY(flipY);
    }
}

bool UIImageView::isFlipX()
{
    if (m_bScale9Enabled)
    {
        return false;
    }
    else
    {
        return DYNAMIC_CAST_CCSPRITE->isFlippedX();
    }
}

bool UIImageView::isFlipY()
{
    if (m_bScale9Enabled)
    {
        return false;
    }
    else
    {
        return DYNAMIC_CAST_CCSPRITE->isFlippedY();
    }
}

void UIImageView::setScale9Enabled(bool able)
{
    if (m_bScale9Enabled == able)
    {
        return;
    }
    
    
    m_bScale9Enabled = able;
    m_pRenderer->removeChild(m_pImageRenderer, true);
    m_pImageRenderer = nullptr;
    if (m_bScale9Enabled)
    {
        m_pImageRenderer = cocos2d::extension::Scale9Sprite::create();
    }
    else
    {
        m_pImageRenderer = cocos2d::Sprite::create();
    }
    loadTexture(m_sTextureFile.c_str(),m_eImageTexType);
    m_pRenderer->addChild(m_pImageRenderer);
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

void UIImageView::ignoreContentAdaptWithSize(bool ignore)
{
    if (!m_bScale9Enabled || (m_bScale9Enabled && !ignore))
    {
        UIWidget::ignoreContentAdaptWithSize(ignore);
        m_bPrevIgnoreSize = ignore;
    }
}

void UIImageView::setCapInsets(const cocos2d::Rect &capInsets)
{
    m_tCapInsets = capInsets;
    if (!m_bScale9Enabled)
    {
        return;
    }
    DYNAMIC_CAST_SCALE9SPRITE->setCapInsets(capInsets);
}

void UIImageView::setAnchorPoint(const cocos2d::Point &pt)
{
    UIWidget::setAnchorPoint(pt);
    m_pImageRenderer->setAnchorPoint(pt);
}

void UIImageView::onSizeChanged()
{
    imageTextureScaleChangedWithSize();
}

const cocos2d::Size& UIImageView::getContentSize() const
{
    return m_tImageTextureSize;
}

cocos2d::Node* UIImageView::getVirtualRenderer()
{
    return m_pImageRenderer;
}

void UIImageView::imageTextureScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        if (!m_bScale9Enabled)
        {
            m_pImageRenderer->setScale(1.0f);
            m_tSize = m_tImageTextureSize;
        }
    }
    else
    {
        if (m_bScale9Enabled)
        {
            dynamic_cast<cocos2d::extension::Scale9Sprite*>(m_pImageRenderer)->setPreferredSize(m_tSize);
        }
        else
        {
            cocos2d::Size textureSize = m_pImageRenderer->getContentSize();
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                m_pImageRenderer->setScale(1.0f);
                return;
            }
            float scaleX = m_tSize.width / textureSize.width;
            float scaleY = m_tSize.height / textureSize.height;
            m_pImageRenderer->setScaleX(scaleX);
            m_pImageRenderer->setScaleY(scaleY);
        }
    }
}

const char* UIImageView::getDescription() const
{
    return "ImageView";
}

UIWidget* UIImageView::createCloneInstance()
{
    return UIImageView::create();
}

void UIImageView::copySpecialProperties(UIWidget *widget)
{
    UIImageView* imageView = dynamic_cast<UIImageView*>(widget);
    if (imageView)
    {
        m_bPrevIgnoreSize = imageView->m_bPrevIgnoreSize;
        setScale9Enabled(imageView->m_bScale9Enabled);
        loadTexture(imageView->m_sTextureFile.c_str(), imageView->m_eImageTexType);
        setCapInsets(imageView->m_tCapInsets);
    }
}

}