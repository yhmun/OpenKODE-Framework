/* -----------------------------------------------------------------------------------
 *
 *      File            UITextField.cpp
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

#include "gui/UIWidgets/UITextField.h"

namespace gui {

UICCTextField::UICCTextField()
: m_bMaxLengthEnabled(false)
, m_nMaxLength(0)
, m_bPasswordEnabled(false)
, m_sPasswordStyleText("*")
, m_bAttachWithIME(false)
, m_bDetachWithIME(false)
, m_bInsertText(false)
, m_bDeleteBackward(false)
{
}

UICCTextField::~UICCTextField()
{
}

UICCTextField * UICCTextField::create(const char *placeholder, const char *fontName, float fontSize)
{
    UICCTextField *pRet = new UICCTextField();
    
    if(pRet && pRet->initWithString("", fontName, fontSize))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    
    return nullptr;
}

void UICCTextField::onEnter()
{
    cocos2d::TextFieldTTF::setDelegate(this);
}


bool UICCTextField::onTextFieldAttachWithIME(cocos2d::TextFieldTTF *pSender)
{
    setAttachWithIME(true);
    return false;
}

bool UICCTextField::onTextFieldInsertText(cocos2d::TextFieldTTF *pSender, const char *text, int nLen)
{
    if (nLen == 1 && strcmp(text, "\n") == 0)
    {
        return false;
    }
    setInsertText(true);
    if (m_bMaxLengthEnabled)
    {
        if (cocos2d::TextFieldTTF::getCharCount() >= m_nMaxLength)
        {
            return true;
        }
    }
    
    return false;
}

bool UICCTextField::onTextFieldDeleteBackward(cocos2d::TextFieldTTF *pSender, const char *delText, int nLen)
{
    setDeleteBackward(true);
    return false;
}

bool UICCTextField::onTextFieldDetachWithIME(cocos2d::TextFieldTTF *pSender)
{
    setDetachWithIME(true);
    return false;
}

void UICCTextField::insertText(const char * text, int len)
{
    std::string str_text = text;
    int str_len = cocos2d::TextFieldTTF::getString().size();
    
    if (strcmp(text, "\n") != 0)
    {
        if (m_bMaxLengthEnabled)
        {
            int multiple = 1;
            char value = text[0];
            if (value < 0 || value > 127)
            {
                multiple = 3;
            }
            
            if (str_len + len > m_nMaxLength * multiple)
            {
                str_text = str_text.substr(0, m_nMaxLength * multiple);
                len = m_nMaxLength * multiple;
                /*
                 int mod = str_len % 3;
                 int offset = (mod == 0) ? 0 : (3 - mod);
                 int amount = str_len + offset;
                 str_text = str_text.substr(0, _maxLength - amount);
                 //                CCLOG("str_test = %s", str_text.c_str());
                 */
            }
        }
    }
    cocos2d::TextFieldTTF::insertText(str_text.c_str(), len);
    
    // password
    if (m_bPasswordEnabled)
    {
        if (cocos2d::TextFieldTTF::getCharCount() > 0)
        {
            setPasswordText(m_sInputText.c_str());
        }
    }
}

void UICCTextField::deleteBackward()
{
    cocos2d::TextFieldTTF::deleteBackward();
    
    if (cocos2d::TextFieldTTF::getCharCount() > 0)
    {
        // password
        if (m_bPasswordEnabled)
        {
            setPasswordText(m_sInputText.c_str());
        }
    }
}

void UICCTextField::openIME()
{
    cocos2d::TextFieldTTF::attachWithIME();
}

void UICCTextField::closeIME()
{
    cocos2d::TextFieldTTF::detachWithIME();
}

void UICCTextField::setMaxLengthEnabled(bool enable)
{
    m_bMaxLengthEnabled = enable;
}

bool UICCTextField::isMaxLengthEnabled()
{
    return m_bMaxLengthEnabled;
}

void UICCTextField::setMaxLength(int length)
{
    m_nMaxLength = length;
}

int UICCTextField::getMaxLength()
{
    return m_nMaxLength;
}

int UICCTextField::getCharCount()
{
    return cocos2d::TextFieldTTF::getCharCount();
}

void UICCTextField::setPasswordEnabled(bool enable)
{
    m_bPasswordEnabled = enable;
}

bool UICCTextField::isPasswordEnabled()
{
    return m_bPasswordEnabled;
}

void UICCTextField::setPasswordStyleText(const char* styleText)
{
    if (strlen(styleText) > 1)
    {
        return;
    }
    char value = styleText[0];
    if (value < 33 || value > 126)
    {
        return;
    }
    m_sPasswordStyleText = styleText;
}

void UICCTextField::setPasswordText(const char *text)
{
    std::string tempStr;
    for (size_t i = 0; i < strlen(text); ++i)
    {
        tempStr.append(m_sPasswordStyleText);
    }
    cocos2d::LabelTTF::setString(tempStr.c_str());
}

void UICCTextField::setAttachWithIME(bool attach)
{
    m_bAttachWithIME = attach;	
}

bool UICCTextField::getAttachWithIME()
{
    return m_bAttachWithIME;
}

void UICCTextField::setDetachWithIME(bool detach)
{
    m_bDetachWithIME = detach;
}

bool UICCTextField::getDetachWithIME()
{
    return m_bDetachWithIME;
}

void UICCTextField::setInsertText(bool insert)
{
    m_bInsertText = insert;
}

bool UICCTextField::getInsertText()
{
    return m_bInsertText;
}

void UICCTextField::setDeleteBackward(bool deleteBackward)
{
    m_bDeleteBackward = deleteBackward;
}

bool UICCTextField::getDeleteBackward()
{
    return m_bDeleteBackward;
}



    
UITextField::UITextField():
m_pTextFieldRenderer(nullptr),
m_fTouchWidth(0.0f),
m_fTouchHeight(0.0f),
m_bUseTouchArea(false),
m_pTextFieldEventListener(nullptr),
m_pTextFieldEventSelector(nullptr),
m_sPasswordStyleText("")
{
}

UITextField::~UITextField()
{
    m_pTextFieldEventListener = nullptr;
    m_pTextFieldEventSelector = nullptr;
}

UITextField* UITextField::create()
{
    UITextField* widget = new UITextField();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool UITextField::init()
{
    if (UIWidget::init())
    {
        setUpdateEnabled(true);
        return true;
    }
    return false;
}

void UITextField::initRenderer()
{
    UIWidget::initRenderer();
    m_pTextFieldRenderer = UICCTextField::create("input words here", "Thonburi", 20);
    m_pRenderer->addChild(m_pTextFieldRenderer);
}

void UITextField::setTouchSize(const cocos2d::Size &size)
{
    m_bUseTouchArea = true;
    m_fTouchWidth = size.width;
    m_fTouchHeight = size.height;
}

void UITextField::setText(const std::string& text)
{
	if (text.size()==0)
		return;

    m_pTextFieldRenderer->setString(text);
    textfieldRendererScaleChangedWithSize();
}

void UITextField::setPlaceHolder(const std::string& value)
{
    m_pTextFieldRenderer->setPlaceHolder(value);
    textfieldRendererScaleChangedWithSize();
}

void UITextField::setFontSize(int size)
{
    m_pTextFieldRenderer->setFontSize(size);
    textfieldRendererScaleChangedWithSize();
}

void UITextField::setFontName(const std::string& name)
{
    m_pTextFieldRenderer->setFontName(name);
    textfieldRendererScaleChangedWithSize();
}

void UITextField::didNotSelectSelf()
{
    m_pTextFieldRenderer->detachWithIME();
}

const std::string& UITextField::getStringValue()
{
    return m_pTextFieldRenderer->getString();
}

bool UITextField::onTouchBegan(const cocos2d::Point &touchPoint)
{
    bool pass = UIWidget::onTouchBegan(touchPoint);

	m_pTextFieldRenderer->attachWithIME();
	
    return pass;
}

void UITextField::setMaxLengthEnabled(bool enable)
{
    m_pTextFieldRenderer->setMaxLengthEnabled(enable);
}

bool UITextField::isMaxLengthEnabled()
{
    return m_pTextFieldRenderer->isMaxLengthEnabled();
}

void UITextField::setMaxLength(int length)
{
    m_pTextFieldRenderer->setMaxLength(length);
}

int UITextField::getMaxLength()
{
    return m_pTextFieldRenderer->getMaxLength();
}

void UITextField::setPasswordEnabled(bool enable)
{
    m_pTextFieldRenderer->setPasswordEnabled(enable);
}

bool UITextField::isPasswordEnabled()
{
    return m_pTextFieldRenderer->isPasswordEnabled();
}

void UITextField::setPasswordStyleText(const char *styleText)
{
    m_pTextFieldRenderer->setPasswordStyleText(styleText);
    
    m_sPasswordStyleText = styleText;
}

void UITextField::update(float dt)
{
    if (getAttachWithIME())
    {
        attachWithIMEEvent();
        setAttachWithIME(false);
    }
    if (getDetachWithIME())
    {
        detachWithIMEEvent();
        setDetachWithIME(false);
    }
    if (getInsertText())
    {
        insertTextEvent();
        setInsertText(false);
        
        textfieldRendererScaleChangedWithSize();
    }
    if (getDeleteBackward())
    {
        deleteBackwardEvent();
        setDeleteBackward(false);
    }
}

bool UITextField::getAttachWithIME()
{
    return m_pTextFieldRenderer->getAttachWithIME();
}

void UITextField::setAttachWithIME(bool attach)
{
    m_pTextFieldRenderer->setAttachWithIME(attach);
}

bool UITextField::getDetachWithIME()
{
    return m_pTextFieldRenderer->getDetachWithIME();
}

void UITextField::setDetachWithIME(bool detach)
{
    m_pTextFieldRenderer->setDetachWithIME(detach);
}

bool UITextField::getInsertText()
{
    return m_pTextFieldRenderer->getInsertText();
}

void UITextField::setInsertText(bool insertText)
{
    m_pTextFieldRenderer->setInsertText(insertText);
}

bool UITextField::getDeleteBackward()
{
    return m_pTextFieldRenderer->getDeleteBackward();
}

void UITextField::setDeleteBackward(bool deleteBackward)
{
    m_pTextFieldRenderer->setDeleteBackward(deleteBackward);
}

void UITextField::attachWithIMEEvent()
{
    if (m_pTextFieldEventListener && m_pTextFieldEventSelector)
    {
        (m_pTextFieldEventListener->*m_pTextFieldEventSelector)(this, TEXTFIELD_EVENT_ATTACH_WITH_IME);
    }
}

void UITextField::detachWithIMEEvent()
{
    if (m_pTextFieldEventListener && m_pTextFieldEventSelector)
    {
        (m_pTextFieldEventListener->*m_pTextFieldEventSelector)(this, TEXTFIELD_EVENT_DETACH_WITH_IME);
    }
}

void UITextField::insertTextEvent()
{
    if (m_pTextFieldEventListener && m_pTextFieldEventSelector)
    {
        (m_pTextFieldEventListener->*m_pTextFieldEventSelector)(this, TEXTFIELD_EVENT_INSERT_TEXT);
    }
}

void UITextField::deleteBackwardEvent()
{
    if (m_pTextFieldEventListener && m_pTextFieldEventSelector)
    {
        (m_pTextFieldEventListener->*m_pTextFieldEventSelector)(this, TEXTFIELD_EVENT_DELETE_BACKWARD);
    }
}

void UITextField::addEventListenerTextField(cocos2d::Object *target, SEL_TextFieldEvent selecor)
{
    m_pTextFieldEventListener = target;
    m_pTextFieldEventSelector = selecor;
}

void UITextField::setAnchorPoint(const cocos2d::Point &pt)
{
    UIWidget::setAnchorPoint(pt);
    m_pTextFieldRenderer->setAnchorPoint(pt);
}

void UITextField::setColor(const cocos2d::Color3B &color)
{
    UIWidget::setColor(color);
    m_pTextFieldRenderer->setColor(color);
}

void UITextField::setOpacity(int opacity)
{
    UIWidget::setOpacity(opacity);
    m_pTextFieldRenderer->setOpacity(opacity);
}

void UITextField::onSizeChanged()
{
    textfieldRendererScaleChangedWithSize();
}

void UITextField::textfieldRendererScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        m_pTextFieldRenderer->setScale(1.0f);
        m_tSize = getContentSize();
    }
    else
    {
        cocos2d::Size textureSize = getContentSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            m_pTextFieldRenderer->setScale(1.0f);
            return;
        }
        float scaleX = m_tSize.width / textureSize.width;
        float scaleY = m_tSize.height / textureSize.height;
        m_pTextFieldRenderer->setScaleX(scaleX);
        m_pTextFieldRenderer->setScaleY(scaleY);
    }
}

const cocos2d::Size& UITextField::getContentSize() const
{
    return m_pTextFieldRenderer->getContentSize();
}

cocos2d::Node* UITextField::getVirtualRenderer()
{
    return m_pTextFieldRenderer;
}

const char* UITextField::getDescription() const
{
    return "TextField";
}

void UITextField::attachWithIME()
{
    m_pTextFieldRenderer->attachWithIME();
}

UIWidget* UITextField::createCloneInstance()
{
    return UITextField::create();
}

void UITextField::copySpecialProperties(UIWidget *widget)
{
    UITextField* textField = dynamic_cast<UITextField*>(widget);
    if (textField)
    {
        setText(textField->m_pTextFieldRenderer->getString());
        setPlaceHolder(textField->getStringValue());
        setFontSize(textField->m_pTextFieldRenderer->getFontSize());
        setFontName(textField->m_pTextFieldRenderer->getFontName().c_str());
        setMaxLengthEnabled(textField->isMaxLengthEnabled());
        setMaxLength(textField->getMaxLength());
        setPasswordEnabled(textField->isPasswordEnabled());
        setPasswordStyleText(textField->m_sPasswordStyleText.c_str());
        setAttachWithIME(textField->getAttachWithIME());
        setDetachWithIME(textField->getDetachWithIME());
        setInsertText(textField->getInsertText());
        setDeleteBackward(textField->getDeleteBackward());
    }
}

}