/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextFieldTTF.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#include "2d/text_input_node/CCTextFieldTTF.h"

#include "2d/CCDirector.h"
#include "platform/CCEGLView.h"

NS_CC_BEGIN

static int _calcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);

        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}

//////////////////////////////////////////////////////////////////////////
// constructor and destructor
//////////////////////////////////////////////////////////////////////////

TextFieldTTF::TextFieldTTF()
: m_pDelegate(0)
, m_nCharCount(0)
, m_sInputText("")
, m_sPlaceHolder("")   // prevent LabelTTF initWithString assertion
, m_bSecureTextEntry(false)
{
    m_tColorSpaceHolder.r = m_tColorSpaceHolder.g = m_tColorSpaceHolder.b = 127;
}

TextFieldTTF::~TextFieldTTF()
{
}

//////////////////////////////////////////////////////////////////////////
// static constructor
//////////////////////////////////////////////////////////////////////////

TextFieldTTF * TextFieldTTF::textFieldWithPlaceHolder(const std::string& placeholder, const Size& dimensions, TextHAlignment alignment, const std::string& fontName, float fontSize)
{
    TextFieldTTF *ret = new TextFieldTTF();
    if(ret && ret->initWithPlaceHolder("", dimensions, alignment, fontName, fontSize))
    {
        ret->autorelease();
        if (placeholder.size()>0)
        {
            ret->setPlaceHolder(placeholder);
        }
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

TextFieldTTF * TextFieldTTF::textFieldWithPlaceHolder(const std::string& placeholder, const std::string& fontName, float fontSize)
{
    TextFieldTTF *ret = new TextFieldTTF();
    if(ret && ret->initWithString("", fontName, fontSize))
    {
        ret->autorelease();
        if (placeholder.size()>0)
        {
            ret->setPlaceHolder(placeholder);
        }
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
// initialize
//////////////////////////////////////////////////////////////////////////

bool TextFieldTTF::initWithPlaceHolder(const std::string& placeholder, const Size& dimensions, TextHAlignment alignment, const std::string& fontName, float fontSize)
{
    m_sPlaceHolder = placeholder;
    return LabelTTF::initWithString(m_sPlaceHolder, fontName, fontSize, dimensions, alignment);
}
bool TextFieldTTF::initWithPlaceHolder(const std::string& placeholder, const std::string& fontName, float fontSize)
{
    m_sPlaceHolder = std::string(placeholder);
    return LabelTTF::initWithString(m_sPlaceHolder, fontName, fontSize);
}

//////////////////////////////////////////////////////////////////////////
// IMEDelegate
//////////////////////////////////////////////////////////////////////////

KDbool TextFieldTTF::attachWithIME ( KDvoid )
{
    KDbool	bRet = IMEDelegate::attachWithIME ( );
    if ( bRet )
    {
        // open keyboard
        EGLView*	pGlView = Director::getInstance ( )->getOpenGLView ( );
        if ( pGlView )
        {
			pGlView->setIMEKeyboardState ( true );
        }
    }
    return bRet;
}

KDbool TextFieldTTF::detachWithIME ( KDvoid )
{
    KDbool	bRet = IMEDelegate::detachWithIME ( );
    if ( bRet )
    {
        // close keyboard
        EGLView*	pGlView = Director::getInstance ( )->getOpenGLView ( );
        if ( pGlView )
        {
			pGlView->setIMEKeyboardState ( false );
        }
    }
    return bRet;
}

bool TextFieldTTF::canAttachWithIME()
{
    return (m_pDelegate) ? (! m_pDelegate->onTextFieldAttachWithIME(this)) : true;
}

bool TextFieldTTF::canDetachWithIME()
{
    return (m_pDelegate) ? (! m_pDelegate->onTextFieldDetachWithIME(this)) : true;
}

void TextFieldTTF::insertText(const char * text, int len)
{
    std::string sInsert(text, len);

    // insert \n means input end
    int nPos = sInsert.find('\n');
    if ((int)sInsert.npos != nPos)
    {
        len = nPos;
        sInsert.erase(nPos);
    }

    if (len > 0)
    {
        if (m_pDelegate && m_pDelegate->onTextFieldInsertText(this, sInsert.c_str(), len))
        {
            // delegate doesn't want to insert text
            return;
        }

        m_nCharCount += _calcCharCount(sInsert.c_str());
        std::string sText(m_sInputText);
        sText.append(sInsert);
        setString(sText);
    }

    if ((int)sInsert.npos == nPos) {
        return;
    }

    // '\n' inserted, let delegate process first
    if (m_pDelegate && m_pDelegate->onTextFieldInsertText(this, "\n", 1))
    {
        return;
    }

    // if delegate hasn't processed, detach from IME by default
    detachWithIME();
}

void TextFieldTTF::deleteBackward()
{
    int nStrLen = m_sInputText.length();
    if (! nStrLen)
    {
        // there is no string
        return;
    }

    // get the delete byte number
    int nDeleteLen = 1;    // default, erase 1 byte

    while(0x80 == (0xC0 & m_sInputText.at(nStrLen - nDeleteLen)))
    {
        ++nDeleteLen;
    }

    if (m_pDelegate && m_pDelegate->onTextFieldDeleteBackward(this, m_sInputText.c_str() + nStrLen - nDeleteLen, nDeleteLen))
    {
        // delegate doesn't wan't to delete backwards
        return;
    }

    // if all text deleted, show placeholder string
    if (nStrLen <= nDeleteLen)
    {
        m_sInputText = "";
        m_nCharCount = 0;
        LabelTTF::setString(m_sPlaceHolder);
        return;
    }

    // set new input text
    std::string sText(m_sInputText.c_str(), nStrLen - nDeleteLen);
    setString(sText);
}

const char * TextFieldTTF::getContentText()
{
    return m_sInputText.c_str();
}

void TextFieldTTF::draw()
{
    if (m_pDelegate && m_pDelegate->onDraw(this))
    {
        return;
    }
    if (m_sInputText.length())
    {
        LabelTTF::draw();
        return;
    }

    // draw placeholder
    Color3B color = getColor();
    setColor(m_tColorSpaceHolder);
    LabelTTF::draw();
    setColor(color);
}

const Color3B& TextFieldTTF::getColorSpaceHolder()
{
    return m_tColorSpaceHolder;
}

void TextFieldTTF::setColorSpaceHolder(const Color3B& color)
{
    m_tColorSpaceHolder = color;
}

//////////////////////////////////////////////////////////////////////////
// properties
//////////////////////////////////////////////////////////////////////////

// input text property
void TextFieldTTF::setString(const std::string &text)
{
    static char bulletString[] = {(char)0xe2, (char)0x80, (char)0xa2, (char)0x00};
    std::string displayText;
    int length;

    if (text.length()>0)
    {
        m_sInputText = text;
        displayText = m_sInputText;
        if (m_bSecureTextEntry)
        {
            displayText = "";
            length = m_sInputText.length();
            while (length)
            {
                displayText.append(bulletString);
                --length;
            }
        }
    }
    else
    {
        m_sInputText = "";
    }

    // if there is no input text, display placeholder instead
    if (! m_sInputText.length())
    {
        LabelTTF::setString(m_sPlaceHolder);
    }
    else
    {
        LabelTTF::setString(displayText);
    }
    m_nCharCount = _calcCharCount(m_sInputText.c_str());
}

const std::string& TextFieldTTF::getString() const
{
    return m_sInputText;
}

// place holder text property
void TextFieldTTF::setPlaceHolder(const std::string& text)
{
    m_sPlaceHolder = text;
    if (! m_sInputText.length())
    {
        LabelTTF::setString(m_sPlaceHolder);
    }
}

const std::string& TextFieldTTF::getPlaceHolder() const
{
    return m_sPlaceHolder;
}

// secureTextEntry
void TextFieldTTF::setSecureTextEntry(bool value)
{
    if (m_bSecureTextEntry != value)
    {
        m_bSecureTextEntry = value;
        setString(getString());
    }
}

bool TextFieldTTF::isSecureTextEntry()
{
    return m_bSecureTextEntry;
}

NS_CC_END
