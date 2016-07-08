/* -----------------------------------------------------------------------------------
 *
 *      File            CCString.cpp
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

#include "base/CCString.h"
#include "platform/CCFileUtils.h"
#include "ccMacros.h"
#include "base/CCArray.h"

NS_CC_BEGIN

#define kMaxStringLen (1024*100)

String::String()
    :m_sString("")
{}

String::String(const char * str)
    :m_sString(str)
{}

String::String(const std::string& str)
    :m_sString(str)
{}

String::String(const String& str)
    :m_sString(str.getCString())
{}

String::~String()
{
    CCLOGINFO("deallocing String: %p", this);

    m_sString.clear();
}

String& String::operator= (const String& other)
{
    m_sString = other.m_sString;
    return *this;
}

bool String::initWithFormatAndValist(const char* format, va_list ap)
{
    bool bRet = false;
    char* pBuf = (char*) kdMalloc(kMaxStringLen);
    if (pBuf != NULL)
    {
        vsnprintf(pBuf, kMaxStringLen, format, ap);
        m_sString = pBuf;
        kdFree(pBuf);
        bRet = true;
    }
    return bRet;
}

bool String::initWithFormat(const char* format, ...)
{
    bool bRet = false;
    m_sString.clear();

    va_list ap;
    va_start(ap, format);

    bRet = initWithFormatAndValist(format, ap);

    va_end(ap);

    return bRet;
}

int String::intValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return kdAtoi(m_sString.c_str());
}

unsigned int String::uintValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return (unsigned int) kdAtoi(m_sString.c_str());
}

float String::floatValue() const
{
    if (length() == 0)
    {
        return 0.0f;
    }
    return kdAtof(m_sString.c_str());
}

double String::doubleValue() const
{
    if (length() == 0)
    {
        return 0.0;
    }
    return kdAtod(m_sString.c_str());
}

bool String::boolValue() const
{
    if (length() == 0)
    {
        return false;
    }

    if (0 == kdStrcmp(m_sString.c_str(), "0") || 0 == kdStrcmp(m_sString.c_str(), "false"))
    {
        return false;
    }
    return true;
}

const char* String::getCString() const
{
    return m_sString.c_str();
}

unsigned int String::length() const
{
    return m_sString.length();
}

int String::compare(const char * pStr) const
{
    return kdStrcmp(getCString(), pStr);
}

void String::append(const std::string& str)
{
    m_sString.append(str);
}

void String::appendWithFormat(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    
    char* pBuf = (char*) kdMalloc(kMaxStringLen);
    if (pBuf != NULL)
    {
        vsnprintf(pBuf, kMaxStringLen, format, ap);
        m_sString.append(pBuf);
        kdFree(pBuf);
    }
    
    va_end(ap);
    
}

Array* String::componentsSeparatedByString(const char *delimiter)
{
    Array* result = Array::create();
    
    int cutAt;
    while( (cutAt = m_sString.find_first_of(delimiter)) != m_sString.npos )
    {
        if(cutAt > 0)
        {
            result->addObject(String::create(m_sString.substr(0, cutAt)));
        }
        m_sString = m_sString.substr(cutAt + 1);
    }
    
    if(m_sString.length() > 0)
    {
        result->addObject(String::create(m_sString));
    }
    
    return result;
}

bool String::isEqual(const Object* pObject)
{
    bool bRet = false;
    const String* pStr = dynamic_cast<const String*>(pObject);
    if (pStr != NULL)
    {
        if (0 == m_sString.compare(pStr->m_sString))
        {
            bRet = true;
        }
    }
    return bRet;
}

String* String::create(const std::string& str)
{
    String* ret = new String(str);
    ret->autorelease();
    return ret;
}

String* String::createWithData(const unsigned char* data, unsigned long nLen)
{
    String* ret = NULL;
    if (data != NULL)
    {
        char* pStr = (char*) kdMalloc(nLen+1);
        if (pStr != NULL)
        {
            pStr[nLen] = '\0';
            if (nLen > 0)
            {
                memcpy(pStr, data, nLen);
            }
            
            ret = String::create(pStr);
            kdFree(pStr);
        }
    }
    return ret;
}

String* String::createWithFormat(const char* format, ...)
{
    String* ret = String::create("");
    va_list ap;
    va_start(ap, format);
    ret->initWithFormatAndValist(format, ap);
    va_end(ap);

    return ret;
}

String* String::createWithContentsOfFile(const char* filename)
{
    KDint32 size = 0;
    unsigned char* data = 0;
    String* ret = NULL;
    data = FileUtils::getInstance()->getFileData(filename, "rb", &size);
    ret = String::createWithData(data, size);
    kdFree(data);
    return ret;
}

void String::acceptVisitor(DataVisitor &visitor)
{
    visitor.visit(this);
}

String* String::clone() const
{
    return String::create(m_sString);
}

NS_CC_END
