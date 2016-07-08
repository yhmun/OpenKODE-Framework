/* -----------------------------------------------------------------------------------
 *
 *      File            CCDictionary.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCInteger.h"
#include "support/CCFileUtils.h"

NS_CC_BEGIN

// -----------------------------------------------------------------------
// CCDictElement
// -----------------------------------------------------------------------

CCDictElement::CCDictElement ( const KDchar* szKey, CCObject* pObject )
{
	CCAssert ( szKey && kdStrlen ( szKey ) > 0, "Invalid key value." );

	m_nKey	  = 0;
	m_pObject = pObject;

	const KDchar*  pStart = szKey; 
	KDint  nLen = kdStrlen ( szKey );

	if ( nLen > MAX_KEY_LEN )
	{
		KDchar*  pEnd = (KDchar*) &szKey [ nLen - 1 ];
		pStart = pEnd - ( MAX_KEY_LEN - 1 );
	}

	kdStrcpy ( m_szKey, pStart );
	kdMemset ( &hh , 0, sizeof ( hh ) );
}

CCDictElement::CCDictElement ( KDintptr nKey, CCObject* pObject )
{
	m_nKey	  = nKey;
	m_pObject = pObject;
	kdMemset ( m_szKey, 0, sizeof ( m_szKey ) );
	kdMemset ( &hh , 0, sizeof ( hh ) );
}

CCDictElement::~CCDictElement ( KDvoid )
{

}

// -----------------------------------------------------------------------
// CCDictionary
// -----------------------------------------------------------------------

CCDictionary::CCDictionary ( KDvoid )
{
	m_pElements	= KD_NULL;
	m_eDictType	= kCCDictUnknown;
}

CCDictionary::~CCDictionary ( KDvoid )
{
    removeAllObjects ( );
}

KDuint CCDictionary::count ( KDvoid )
{
    return HASH_COUNT ( m_pElements );
}

CCArray* CCDictionary::allKeys ( KDvoid )
{
    KDint  nKeyCount = this->count ( );
    if ( nKeyCount <= 0 ) 
	{
		return KD_NULL;
	}

    CCArray*  pArray = CCArray::createWithCapacity ( nKeyCount );

    CCDictElement*  pElement;
	CCDictElement*  pTemp;

    if ( m_eDictType == kCCDictStr )
    {
        HASH_ITER ( hh, m_pElements, pElement, pTemp ) 
        {
            CCString*  pOneKey = new CCString ( pElement->m_szKey );
            pArray->addObject ( pOneKey );
			CC_SAFE_RELEASE ( pOneKey );
        }
    }
    else if ( m_eDictType == kCCDictInt )
    {
        HASH_ITER ( hh, m_pElements, pElement, pTemp ) 
        {
            CCInteger*  pOneKey = new CCInteger ( pElement->m_nKey );
            pArray->addObject ( pOneKey );
			CC_SAFE_RELEASE ( pOneKey );
        }
    }
    
    return pArray;
}

CCArray* CCDictionary::allKeysForObject ( CCObject* pObject )
{
    KDint  nKeyCount = this->count ( );
    if ( nKeyCount <= 0 )
	{
		return KD_NULL;
	}

    CCArray*  pArray = CCArray::create ( );

    CCDictElement*  pElement;
	CCDictElement*  pTemp;

    if ( m_eDictType == kCCDictStr )
    {
        HASH_ITER ( hh, m_pElements, pElement, pTemp ) 
        {
            if ( pObject == pElement->m_pObject )
            {
                CCString*  pOneKey = new CCString ( pElement->m_szKey );                
                pArray->addObject ( pOneKey );
				CC_SAFE_RELEASE ( pOneKey );
            }
        }
    }
    else if ( m_eDictType == kCCDictInt )
    {
        HASH_ITER ( hh, m_pElements, pElement, pTemp ) 
        {
            if ( pObject == pElement->m_pObject )
            {
                CCInteger*  pOneKey = new CCInteger ( pElement->m_nKey );                
                pArray->addObject ( pOneKey );
				CC_SAFE_RELEASE ( pOneKey );
            }
        }
    }

    return pArray;
}

CCArray* CCDictionary::allValues ( KDvoid )
{
	KDint  nKeyCount = this->count ( );
	if ( nKeyCount <= 0 ) 
	{
		return KD_NULL;
	}

	CCArray*  pArray = CCArray::createWithCapacity ( nKeyCount );

	CCDictElement*  pElement;
	CCDictElement*  pTemp;

	HASH_ITER ( hh, m_pElements, pElement, pTemp ) 
	{
		pArray->addObject ( pElement->m_pObject );
	}

	return pArray;
}

CCObject* CCDictionary::objectForKey ( const std::string& sKey )
{
	// if dictionary wasn't initialized, return NULL directly.
    if ( m_eDictType == kCCDictUnknown ) 
	{
		return KD_NULL;
	}
    // CCDictionary only supports one kind of key, string or integer.
    // This method uses string as key, therefore we should make sure that the key type of this CCDictionary is string.
    CCAssert ( m_eDictType == kCCDictStr, "this dictionary does not use string as key." );

    CCObject*       pRetObject = KD_NULL;
    CCDictElement*  pElement   = KD_NULL;
    HASH_FIND_STR ( m_pElements, sKey.c_str ( ), pElement );
    if ( pElement != KD_NULL )
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

CCObject* CCDictionary::objectForKey ( KDintptr nKey )
{
    // if dictionary wasn't initialized, return NULL directly.
    if ( m_eDictType == kCCDictUnknown )
	{
		return KD_NULL;
	}
    // CCDictionary only supports one kind of key, string or integer.
    // This method uses integer as key, therefore we should make sure that the key type of this CCDictionary is integer.
   CCAssert ( m_eDictType == kCCDictInt, "this dictionary does not use integer as key." );

    CCObject*       pRetObject = KD_NULL;
    CCDictElement*  pElement   = KD_NULL;
    HASH_FIND_PTR ( m_pElements, &nKey, pElement );
    if ( pElement != KD_NULL )
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

const CCString* CCDictionary::valueForKey ( const std::string& sKey )
{
    CCString*  pStr = dynamic_cast<CCString*> ( objectForKey ( sKey ) );
    if ( pStr == KD_NULL )
    {
        pStr = CCString::create ( "" );
    }
    return pStr;
}

const CCString* CCDictionary::valueForKey ( KDintptr nKey )
{
    CCString*  pStr = dynamic_cast<CCString*> ( objectForKey ( nKey ) );
    if ( pStr == KD_NULL )
    {
        pStr = CCString::create ( "" );
    }
    return pStr;
}

KDvoid CCDictionary::setObject ( CCObject* pObject, const std::string& sKey )
{
    CCAssert ( sKey.length ( ) > 0 && pObject != NULL, "Invalid Argument!" );
    if ( m_eDictType == kCCDictUnknown )
    {
        m_eDictType = kCCDictStr;
    }
    CCAssert ( m_eDictType == kCCDictStr, "this dictionary doesn't use string as key." );

    CCDictElement*  pElement = KD_NULL;
    HASH_FIND_STR ( m_pElements, sKey.c_str ( ), pElement );
    if ( pElement == KD_NULL )
    {
        setObjectUnSafe ( pObject, sKey );
    }
    else if ( pElement->m_pObject != pObject )
    {
        CCObject*  pTempObj = pElement->m_pObject;
        pTempObj->retain ( );
        removeObjectForElememt ( pElement );
        setObjectUnSafe ( pObject, sKey );
        pTempObj->release ( );
    }
}

KDvoid CCDictionary::setObject ( CCObject* pObject, KDintptr nKey )
{
    CCAssert ( pObject != KD_NULL, "Invalid Argument!" );
    if ( m_eDictType == kCCDictUnknown )
    {
        m_eDictType = kCCDictInt;
    }

    CCAssert ( m_eDictType == kCCDictInt, "this dictionary doesn't use integer as key." );

    CCDictElement*  pElement = KD_NULL;
    HASH_FIND_PTR ( m_pElements, &nKey, pElement );
    if ( pElement == KD_NULL )
    {
        setObjectUnSafe ( pObject, nKey );
    }
    else if ( pElement->m_pObject != pObject )
    {
        CCObject*  pTempObj = pElement->m_pObject;
        pTempObj->retain ( );
        removeObjectForElememt ( pElement );
        setObjectUnSafe ( pObject, nKey );
        pTempObj->release ( );
    }
}

KDvoid CCDictionary::removeObjectForKey ( const std::string& sKey )
{
    if ( m_eDictType == kCCDictUnknown ) 
    {
        return;
    }

    CCAssert ( m_eDictType == kCCDictStr, "this dictionary doesn't use string as its key" );
    CCAssert ( sKey.length ( ) > 0, "Invalid Argument!" );
    CCDictElement*  pElement = KD_NULL;
    HASH_FIND_STR ( m_pElements, sKey.c_str ( ), pElement );
    removeObjectForElememt ( pElement );
}

KDvoid CCDictionary::removeObjectForKey ( KDintptr nKey )
{
    if ( m_eDictType == kCCDictUnknown ) 
    {
        return;
    }

    CCAssert ( m_eDictType == kCCDictInt, "this dictionary doesn't use integer as its key" );
    CCDictElement*  pElement = KD_NULL;
    HASH_FIND_PTR ( m_pElements, &nKey, pElement );
    removeObjectForElememt ( pElement );
}

KDvoid CCDictionary::setObjectUnSafe ( CCObject* pObject, const std::string& sKey )
{
    pObject->retain ( );
    CCDictElement*  pElement = new CCDictElement ( sKey.c_str ( ), pObject );
    HASH_ADD_STR ( m_pElements, m_szKey, pElement );
}

KDvoid CCDictionary::setObjectUnSafe ( CCObject* pObject, const KDintptr nKey )
{
    pObject->retain ( );
    CCDictElement*  pElement = new CCDictElement ( nKey, pObject );
    HASH_ADD_PTR ( m_pElements, m_nKey, pElement );
}

KDvoid CCDictionary::removeObjectsForKeys ( CCArray* pKeyArray )
{
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( pKeyArray, pObject )
    {
        CCString*  pStr = (CCString*) pObject;
        removeObjectForKey ( pStr->getCString ( ) );
    }
}

KDvoid CCDictionary::removeObjectForElememt ( CCDictElement* pElement )
{
    if ( pElement != KD_NULL )
    {
        HASH_DEL ( m_pElements, pElement );
        pElement->m_pObject->release ( );
        CC_SAFE_DELETE ( pElement );
    }
}

KDvoid CCDictionary::removeAllObjects ( KDvoid )
{
    CCDictElement*	pElement;
	CCDictElement*  pTemp;

    HASH_ITER ( hh, m_pElements, pElement, pTemp ) 
    {
        HASH_DEL ( m_pElements, pElement );
        pElement->m_pObject->release ( );
        CC_SAFE_DELETE ( pElement );
    }
}

CCObject* CCDictionary::copyWithZone ( CCZone* pZone )
{
    CCAssert ( pZone == KD_NULL, "CCDictionary should not be inherited." );

    CCDictionary*   pNewDict = new CCDictionary ( );
    CCDictElement*  pElement = KD_NULL;
	CCObject*       pTempObj = KD_NULL;

    if ( m_eDictType == kCCDictInt )
    {
        CCDICT_FOREACH ( this, pElement )
        {
            pTempObj = pElement->getObject ( )->copy ( );
            pNewDict->setObject ( pTempObj, pElement->getIntKey ( ) );
            pTempObj->release ( );
        }
    }
    else if ( m_eDictType == kCCDictStr )
    {
        CCDICT_FOREACH ( this, pElement )
        {
            pTempObj = pElement->getObject ( )->copy ( );
            pNewDict->setObject ( pTempObj, pElement->getStrKey ( ) );
            pTempObj->release ( );
        }
    }

    return pNewDict;
}

CCObject* CCDictionary::randomObject ( KDvoid )
{
    if ( m_eDictType == kCCDictUnknown )
    {
        return KD_NULL;
    }
    
    CCObject*  pKey = allKeys()->randomObject();
    
    if ( m_eDictType == kCCDictInt )
    {
        return objectForKey ( ( (CCInteger*) pKey )->getValue ( ) );
    }
    else if ( m_eDictType == kCCDictStr )
    {
        return objectForKey ( ( (CCString*) pKey )->getCString ( ) );
    }
    else
    {
        return KD_NULL;
    }
}

CCDictionary* CCDictionary::create ( KDvoid )
{
    CCDictionary*  pRet = new CCDictionary ( );

    if ( pRet )
    {
        pRet->autorelease ( );
    }

    return pRet;
}

CCDictionary* CCDictionary::createWithDictionary ( CCDictionary* pSrcDict )
{
    CCDictionary*  pNewDict = (CCDictionary*) pSrcDict->copy ( );
	
	if ( pNewDict )
	{
		pNewDict->autorelease ( );
	}

    return pNewDict;
}

CCDictionary* CCDictionary::createWithContentsOfFileThreadSafe ( const KDchar* szFileName )
{
    return CCFileUtils::sharedFileUtils ( )->createCCDictionaryWithContentsOfFile ( szFileName );
}

KDvoid CCDictionary::acceptVisitor ( CCDataVisitor& tVisitor )
{
	return tVisitor.visit ( this );
}

CCDictionary* CCDictionary::createWithContentsOfFile ( const KDchar* szFileName )
{
    CCDictionary*  pRet = createWithContentsOfFileThreadSafe ( szFileName );
	
	if ( pRet )
	{
		pRet->autorelease ( );
	}

    return pRet;
}

KDbool CCDictionary::writeToFile ( const KDchar* szFullPath )
{
	return CCFileUtils::sharedFileUtils ( )->writeToFile ( this, szFullPath );
}

NS_CC_END
