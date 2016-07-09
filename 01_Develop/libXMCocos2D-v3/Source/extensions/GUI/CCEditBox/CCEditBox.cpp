/* -----------------------------------------------------------------------------------
 *
 *      File            CCEditBox.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2010-2012 cocos2d-x.org.
 *      Copyright (c) 2012      James Chen
 *
 *          http://www.cocos2d-x.org
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "extensions/GUI/CCEditBox/CCEditBox.h"
#include "CCEditBoxImpl.h"

NS_CC_EXT_BEGIN

EditBox::EditBox(void)
: m_pEditBoxImpl(NULL)
, m_pDelegate(NULL)
, m_eEditBoxInputMode(EditBox::InputMode::SINGLE_LINE)
, m_eEditBoxInputFlag(EditBox::InputFlag::INTIAL_CAPS_ALL_CHARACTERS)
, m_eKeyboardReturnType(KeyboardReturnType::DEFAULT)
, m_nFontSize(-1)
, m_nPlaceholderFontSize(-1)
, m_tColText(Color3B::WHITE)
, m_tColPlaceHolder(Color3B::GRAY)
, m_nMaxLength(0)
, m_fAdjustHeight(0.0f)
, m_nScriptEditBoxHandler(0)
{
}

EditBox::~EditBox(void)
{
    CC_SAFE_DELETE(m_pEditBoxImpl);
    unregisterScriptEditBoxHandler();
}


void EditBox::touchDownAction(Object *sender, Control::EventType controlEvent)
{
    m_pEditBoxImpl->openKeyboard();
}

EditBox* EditBox::create(const Size& size, Scale9Sprite* pNormal9SpriteBg, Scale9Sprite* pPressed9SpriteBg/* = NULL*/, Scale9Sprite* pDisabled9SpriteBg/* = NULL*/)
{
    EditBox* pRet = new EditBox();
    
    if (pRet != NULL && pRet->initWithSizeAndBackgroundSprite(size, pNormal9SpriteBg))
    {
        if (pPressed9SpriteBg != NULL)
        {
            pRet->setBackgroundSpriteForState(pPressed9SpriteBg, Control::State::HIGH_LIGHTED);
        }
        
        if (pDisabled9SpriteBg != NULL)
        {
            pRet->setBackgroundSpriteForState(pDisabled9SpriteBg, Control::State::DISABLED);
        }
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool EditBox::initWithSizeAndBackgroundSprite(const Size& size, Scale9Sprite* pPressed9SpriteBg)
{
    if (ControlButton::initWithBackgroundSprite(pPressed9SpriteBg))
    {
        m_pEditBoxImpl = __createSystemEditBox(this);
        m_pEditBoxImpl->initWithSize(size);
        
        this->setZoomOnTouchDown(false);
        this->setPreferredSize(size);
        this->setPosition(Point(0, 0));
        this->addTargetWithActionForControlEvent(this, cccontrol_selector(EditBox::touchDownAction), Control::EventType::TOUCH_UP_INSIDE);
        
        return true;
    }
    return false;
}

void EditBox::setDelegate(EditBoxDelegate* pDelegate)
{
    m_pDelegate = pDelegate;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setDelegate(pDelegate);
    }
}

EditBoxDelegate* EditBox::getDelegate()
{
    return m_pDelegate;
}

void EditBox::setText(const char* pText)
{
    if (pText != NULL)
    {
        m_sText = pText;
        if (m_pEditBoxImpl != NULL)
        {
            m_pEditBoxImpl->setText(pText);
        }
    }
}

const char* EditBox::getText(void)
{
    if (m_pEditBoxImpl != NULL)
    {
		const char* pText = m_pEditBoxImpl->getText();
		if(pText != NULL)
			return pText;
    }
    
    return "";
}

void EditBox::setFont(const char* pFontName, int fontSize)
{
    m_sFontName = pFontName;
    m_nFontSize = fontSize;
    if (pFontName != NULL)
    {
        if (m_pEditBoxImpl != NULL)
        {
            m_pEditBoxImpl->setFont(pFontName, fontSize);
        }
    }
}

void EditBox::setFontName(const char* pFontName)
{
    m_sFontName = pFontName;
    if (m_pEditBoxImpl != NULL && m_nFontSize != -1)
    {
        m_pEditBoxImpl->setFont(pFontName, m_nFontSize);
    }
}

void EditBox::setFontSize(int fontSize)
{
    m_nFontSize = fontSize;
    if (m_pEditBoxImpl != NULL && m_sFontName.length() > 0)
    {
        m_pEditBoxImpl->setFont(m_sFontName.c_str(), m_nFontSize);
    }
}

void EditBox::setFontColor(const Color3B& color)
{
    m_tColText = color;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setFontColor(color);
    }
}

void EditBox::setPlaceholderFont(const char* pFontName, int fontSize)
{
    m_sPlaceholderFontName = pFontName;
    m_nPlaceholderFontSize = fontSize;
    if (pFontName != NULL)
    {
        if (m_pEditBoxImpl != NULL)
        {
            m_pEditBoxImpl->setPlaceholderFont(pFontName, fontSize);
        }
    }
}

void EditBox::setPlaceholderFontName(const char* pFontName)
{
    m_sPlaceholderFontName = pFontName;
    if (m_pEditBoxImpl != NULL && m_nPlaceholderFontSize != -1)
    {
        m_pEditBoxImpl->setPlaceholderFont(pFontName, m_nFontSize);
    }
}

void EditBox::setPlaceholderFontSize(int fontSize)
{
    m_nPlaceholderFontSize = fontSize;
    if (m_pEditBoxImpl != NULL && m_sPlaceholderFontName.length() > 0)
    {
        m_pEditBoxImpl->setPlaceholderFont(m_sPlaceholderFontName.c_str(), m_nFontSize);
    }
}

void EditBox::setPlaceholderFontColor(const Color3B& color)
{
    m_tColText = color;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setPlaceholderFontColor(color);
    }
}

void EditBox::setPlaceHolder(const char* pText)
{
    if (pText != NULL)
    {
        m_sPlaceHolder = pText;
        if (m_pEditBoxImpl != NULL)
        {
            m_pEditBoxImpl->setPlaceHolder(pText);
        }
    }
}

const char* EditBox::getPlaceHolder(void)
{
    return m_sPlaceHolder.c_str();
}

void EditBox::setInputMode(EditBox::InputMode inputMode)
{
    m_eEditBoxInputMode = inputMode;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setInputMode(inputMode);
    }
}

void EditBox::setMaxLength(int maxLength)
{
    m_nMaxLength = maxLength;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setMaxLength(maxLength);
    }
}


int EditBox::getMaxLength()
{
    return m_nMaxLength;
}

void EditBox::setInputFlag(EditBox::InputFlag inputFlag)
{
    m_eEditBoxInputFlag = inputFlag;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setInputFlag(inputFlag);
    }
}

void EditBox::setReturnType(EditBox::KeyboardReturnType returnType)
{
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setReturnType(returnType);
    }
}

/* override function */
void EditBox::setPosition(const Point& pos)
{
    ControlButton::setPosition(pos);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setPosition(pos);
    }
}

void EditBox::setVisible(bool visible)
{
    ControlButton::setVisible(visible);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setVisible(visible);
    }
}

void EditBox::setContentSize(const Size& size)
{
    ControlButton::setContentSize(size);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setContentSize(size);
    }
}

void EditBox::setAnchorPoint(const Point& anchorPoint)
{
    ControlButton::setAnchorPoint(anchorPoint);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setAnchorPoint(anchorPoint);
    }
}

void EditBox::visit(void)
{
    ControlButton::visit();
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->visit();
    }
}

void EditBox::onEnter(void)
{
    ControlButton::onEnter();
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->onEnter();
    }
}

void EditBox::onExit(void)
{
    ControlButton::onExit();
    if (m_pEditBoxImpl != NULL)
    {
        // remove system edit control
        m_pEditBoxImpl->closeKeyboard();
    }
}

static Rect getRect(Node * pNode)
{
	Size contentSize = pNode->getContentSize();
	Rect rect = Rect(0, 0, contentSize.width, contentSize.height);
	return RectApplyAffineTransform(rect, pNode->getNodeToWorldTransform());
}

void EditBox::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
    // CCLOG("CCEditBox::keyboardWillShow");
    Rect rectTracked = getRect(this);
	// some adjustment for margin between the keyboard and the edit box.
	rectTracked.origin.y -= 4;

    // if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
    if (!rectTracked.intersectsRect(info.end))
    {
        CCLOG("needn't to adjust view layout.");
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    m_fAdjustHeight = info.end.getMaxY() - rectTracked.getMinY();
    // CCLOG("CCEditBox:needAdjustVerticalPosition(%f)", _adjustHeight);
    
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->doAnimationWhenKeyboardMove(info.duration, m_fAdjustHeight);
    }
}

void EditBox::keyboardDidShow(IMEKeyboardNotificationInfo& info)
{
	
}

void EditBox::keyboardWillHide(IMEKeyboardNotificationInfo& info)
{
    // CCLOG("CCEditBox::keyboardWillHide");
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->doAnimationWhenKeyboardMove(info.duration, -m_fAdjustHeight);
    }
}

void EditBox::keyboardDidHide(IMEKeyboardNotificationInfo& info)
{
	
}

void EditBox::registerScriptEditBoxHandler(int handler)
{
    unregisterScriptEditBoxHandler();
    m_nScriptEditBoxHandler = handler;
}

void EditBox::unregisterScriptEditBoxHandler(void)
{
    if (0 != m_nScriptEditBoxHandler)
    {
        ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(m_nScriptEditBoxHandler);
        m_nScriptEditBoxHandler = 0;
    }
}


NS_CC_EXT_END
