/* -----------------------------------------------------------------------------------
 *
 *      File            CCBValue.cpp
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

#include "editor-support/cocosbuilder/CCBValue.h"

using namespace cocos2d;

namespace cocosbuilder {

// Implementation of Color3BWapper

Color3BWapper* Color3BWapper::create(const Color3B& color)
{
    Color3BWapper *ret = new Color3BWapper();
    if (ret)
    {
        ret->m_tColor.r = color.r;
        ret->m_tColor.g = color.g;
        ret->m_tColor.b = color.b;

        ret->autorelease();
    }

    return ret;
}

const Color3B& Color3BWapper::getColor() const
{
    return m_tColor;
}

// Implementation of CCBValue

CCBValue* CCBValue::create(int nValue)
{
    CCBValue *ret = new CCBValue();
    if (ret)
    {
        ret->m_tValue.intValue = nValue;
        ret->m_eType = Type::INT;
        ret->autorelease();
    }

    return ret;
}

CCBValue* CCBValue::create(float fValue)
{
    CCBValue *ret = new CCBValue();
    if (ret)
    {
        ret->m_tValue.floatValue = fValue;
        ret->m_eType = Type::FLOAT;
        ret->autorelease();
    }

    return ret;
}

CCBValue* CCBValue::create(bool vValue)
{
    CCBValue *ret = new CCBValue();
    if (ret)
    {
        ret->m_tValue.intValue = vValue ? 1 : 0;
        ret->m_eType = Type::BOOL;
        ret->autorelease();
    }

    return ret;
}

CCBValue* CCBValue::create(unsigned char byte)
{
    CCBValue *ret = new CCBValue();
    if (ret)
    {
        ret->m_tValue.intValue = byte;
        ret->m_eType = Type::UNSIGNED_CHAR;
        ret->autorelease();
    }

    return ret;
}

CCBValue* CCBValue::create(const char *pStringValue)
{
    CCBValue *ret = new CCBValue();
    if (ret)
    {
        ret->m_sStrValue = pStringValue;
        ret->m_eType = Type::STRING;
        ret->autorelease();
    }

    return ret;
}


CCBValue* CCBValue::create(Array *pArrValue)
{
    CCBValue *ret = new CCBValue();
    if (ret)
    {
        ret->m_pArrValue = pArrValue;
        ret->m_eType = Type::ARRAY;
        ret->autorelease();
    }

    return ret;
}


int CCBValue::getIntValue()
{
    CCASSERT(m_eType == Type::INT, "The type of CCBValue isn't integer.");

    return m_tValue.intValue;
}

float CCBValue::getFloatValue()
{
    CCASSERT(m_eType == Type::FLOAT, "The type of CCBValue isn't float.");

    return m_tValue.floatValue;
}

bool CCBValue::getBoolValue()
{
    CCASSERT(m_eType == Type::BOOL, "The type of CCBValue isn't boolean.");

    return m_tValue.intValue == 1 ? true : false;
}

unsigned char CCBValue::getByteValue()
{
    CCASSERT(m_eType == Type::UNSIGNED_CHAR, "The type of CCBValue isn't unsigned char.");

    return (unsigned char)(m_tValue.intValue);
}

Array* CCBValue::getArrayValue()
{
    CCASSERT(m_eType == Type::ARRAY, "The type of CCBValue isn't array.");

    return m_pArrValue;
}


const char* CCBValue::getStringValue()
{
    CCASSERT(m_eType == Type::STRING, "The type of CCBValue isn't string.");

    return m_sStrValue.c_str();
}

CCBValue::Type CCBValue::getType()
{
    return m_eType;
}

}
