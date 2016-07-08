/* -----------------------------------------------------------------------------------
 *
 *      File            CCDictionary.cpp
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

#include "base/CCDictionary.h"
#include "base/CCString.h"
#include "base/CCInteger.h"
#include "platform/CCFileUtils.h"

using namespace std;

NS_CC_BEGIN

// -----------------------------------------------------------------------
// DictElement

DictElement::DictElement(const char* pszKey, Object* pObject)
{
    CCASSERT(pszKey && strlen(pszKey) > 0, "Invalid key value.");
    m_nIntKey = 0;
    const char* pStart = pszKey;
    
    size_t len = strlen(pszKey);
    if (len > MAX_KEY_LEN )
    {
        char* pEnd = (char*)&pszKey[len-1];
        pStart = pEnd - (MAX_KEY_LEN-1);
    }
    
    strcpy(m_pStrKey, pStart);
    
    m_pObject = pObject;
    kdMemset(&hh, 0, sizeof(hh));
}

DictElement::DictElement(KDintptr iKey, Object* pObject)
{
    m_pStrKey[0] = '\0';
    m_nIntKey = iKey;
    m_pObject = pObject;
    kdMemset(&hh, 0, sizeof(hh));
}

DictElement::~DictElement()
{
    CCLOGINFO("deallocing DictElement: %p", this);
}

// -----------------------------------------------------------------------
// Dictionary

Dictionary::Dictionary()
: m_pElements(NULL)
, m_eDictType(kDictUnknown)
{

}

Dictionary::~Dictionary()
{
    CCLOGINFO("deallocing Dictionary: %p", this);
    removeAllObjects();
}

unsigned int Dictionary::count()
{
    return HASH_COUNT(m_pElements);
}

Array* Dictionary::allKeys()
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;

    Array* pArray = Array::createWithCapacity(iKeyCount);

    DictElement *pElement, *tmp;
    if (m_eDictType == kDictStr)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            String* pOneKey = new String(pElement->m_pStrKey);
            pArray->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    else if (m_eDictType == kDictInt)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            Integer* pOneKey = new Integer(pElement->m_nIntKey);
            pArray->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    
    return pArray;
}

Array* Dictionary::allKeysForObject(Object* object)
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;
    Array* pArray = Array::create();

    DictElement *pElement, *tmp;

    if (m_eDictType == kDictStr)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
                String* pOneKey = new String(pElement->m_pStrKey);
                pArray->addObject(pOneKey);
                CC_SAFE_RELEASE(pOneKey);
            }
        }
    }
    else if (m_eDictType == kDictInt)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
                Integer* pOneKey = new Integer(pElement->m_nIntKey);
                pArray->addObject(pOneKey);
                CC_SAFE_RELEASE(pOneKey);
            }
        }
    }
    return pArray;
}

Object* Dictionary::objectForKey(const std::string& key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kDictUnknown) return NULL;
    // Dictionary only supports one kind of key, string or integer.
    // This method uses string as key, therefore we should make sure that the key type of this Dictionary is string.
    CCASSERT(m_eDictType == kDictStr, "this dictionary does not use string as key.");

    Object* pRetObject = NULL;
    DictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

Object* Dictionary::objectForKey ( KDintptr key )
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kDictUnknown) return NULL;
    // Dictionary only supports one kind of key, string or integer.
    // This method uses integer as key, therefore we should make sure that the key type of this Dictionary is integer.
    CCASSERT(m_eDictType == kDictInt, "this dictionary does not use integer as key.");

    Object* pRetObject = NULL;
    DictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

const String* Dictionary::valueForKey(const std::string& key)
{
    String* pStr = dynamic_cast<String*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = String::create("");
    }
    return pStr;
}

const String* Dictionary::valueForKey(KDintptr key)
{
    String* pStr = dynamic_cast<String*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = String::create("");
    }
    return pStr;
}

void Dictionary::setObject(Object* pObject, const std::string& key)
{
    CCASSERT(key.length() > 0 && pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kDictUnknown)
    {
        m_eDictType = kDictStr;
    }

    CCASSERT(m_eDictType == kDictStr, "this dictionary doesn't use string as key.");

    DictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        Object* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }
}

void Dictionary::setObject(Object* pObject, KDintptr key)
{
    CCASSERT(pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kDictUnknown)
    {
        m_eDictType = kDictInt;
    }

    CCASSERT(m_eDictType == kDictInt, "this dictionary doesn't use integer as key.");

    DictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        Object* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }

}

void Dictionary::removeObjectForKey(const std::string& key)
{
    if (m_eDictType == kDictUnknown)
    {
        return;
    }
    
    CCASSERT(m_eDictType == kDictStr, "this dictionary doesn't use string as its key");
    CCASSERT(key.length() > 0, "Invalid Argument!");
    DictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    removeObjectForElememt(pElement);
}

void Dictionary::removeObjectForKey(KDintptr key)
{
    if (m_eDictType == kDictUnknown)
    {
        return;
    }
    
    CCASSERT(m_eDictType == kDictInt, "this dictionary doesn't use integer as its key");
    DictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    removeObjectForElememt(pElement);
}

void Dictionary::setObjectUnSafe(Object* pObject, const std::string& key)
{
    pObject->retain();
    DictElement* pElement = new DictElement(key.c_str(), pObject);
    HASH_ADD_STR(m_pElements, m_pStrKey, pElement);
}

void Dictionary::setObjectUnSafe(Object* pObject, const KDintptr key)
{
    pObject->retain();
    DictElement* pElement = new DictElement(key, pObject);
    HASH_ADD_PTR(m_pElements, m_nIntKey, pElement);
}

void Dictionary::removeObjectsForKeys(Array* pKeyArray)
{
    Object* pObj = NULL;
    CCARRAY_FOREACH(pKeyArray, pObj)
    {
        String* pStr = static_cast<String*>(pObj);
        removeObjectForKey(pStr->getCString());
    }
}

void Dictionary::removeObjectForElememt(DictElement* pElement)
{
    if (pElement != NULL)
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);
    }
}

void Dictionary::removeAllObjects()
{
    DictElement *pElement, *tmp;
    HASH_ITER(hh, m_pElements, pElement, tmp) 
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);

    }
}

Object* Dictionary::randomObject()
{
    if (m_eDictType == kDictUnknown)
    {
        return NULL;
    }
    
    Object* key = allKeys()->getRandomObject();
    
    if (m_eDictType == kDictInt)
    {
        return objectForKey( static_cast<Integer*>(key)->getValue());
    }
    else if (m_eDictType == kDictStr)
    {
        return objectForKey( static_cast<String*>(key)->getCString());
    }
    else
    {
        return NULL;
    }
}

Dictionary* Dictionary::create()
{
    Dictionary* ret = new Dictionary();
    if (ret && ret->init() )
    {
        ret->autorelease();
    }
    return ret;
}

bool Dictionary::init()
{
    return true;
}

Dictionary* Dictionary::createWithDictionary(Dictionary* srcDict)
{
    return srcDict->clone();
}

Dictionary* Dictionary::createWithContentsOfFileThreadSafe(const char *pFileName)
{
    return FileUtils::getInstance()->createDictionaryWithContentsOfFile(pFileName);
}

void Dictionary::acceptVisitor(DataVisitor &visitor)
{
    return visitor.visit(this);
}

Dictionary* Dictionary::createWithContentsOfFile(const char *pFileName)
{
    auto ret = createWithContentsOfFileThreadSafe(pFileName);
    if (ret != nullptr)
    {
        ret->autorelease();
    }
    return ret;
}

bool Dictionary::writeToFile(const char *fullPath)
{
    return FileUtils::getInstance()->writeToFile(this, fullPath);
}

Dictionary* Dictionary::clone() const
{
    Dictionary* newDict = Dictionary::create();
    
    DictElement* element = NULL;
    Object* tmpObj = NULL;
    Clonable* obj = NULL;
    if (m_eDictType == kDictInt)
    {
        CCDICT_FOREACH(this, element)
        {
            obj = dynamic_cast<Clonable*>(element->getObject());
            if (obj)
            {
                tmpObj = dynamic_cast<Object*>(obj->clone());
                if (tmpObj)
                {
                    newDict->setObject(tmpObj, element->getIntKey());
                }
            }
            else
            {
                CCLOGWARN("%s isn't clonable.", typeid(*element->getObject()).name());
            }
        }
    }
    else if (m_eDictType == kDictStr)
    {
        CCDICT_FOREACH(this, element)
        {
            obj = dynamic_cast<Clonable*>(element->getObject());
            if (obj)
            {
                tmpObj = dynamic_cast<Object*>(obj->clone());
                if (tmpObj)
                {
                    newDict->setObject(tmpObj, element->getStrKey());
                }
            }
            else
            {
                CCLOGWARN("%s isn't clonable.", typeid(*element->getObject()).name());
            }
        }
    }
    
    return newDict;
}

NS_CC_END
