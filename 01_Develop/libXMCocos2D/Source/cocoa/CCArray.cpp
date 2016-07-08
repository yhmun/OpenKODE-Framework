/* -----------------------------------------------------------------------------------
 *
 *      File            CCArray.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      ForzeField Studios S.L. 
 *
 *         http://www.cocos2d-x.org      
 *         http://forzefield.com  
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
#include "cocoa/CCArray.h"
#include "cocoa/CCString.h"
#include "support/CCFileUtils.h"

NS_CC_BEGIN
    
CCArray::CCArray ( KDvoid )
{
	m_pData = KD_NULL;

	init ( );
}

CCArray::CCArray ( KDuint uCapacity )
{
	m_pData = KD_NULL;

	initWithCapacity ( uCapacity );
}

CCArray::~CCArray ( KDvoid )
{
	ccArrayFree ( m_pData );
}

CCArray* CCArray::create ( KDvoid )
{
    CCArray*  pArray = new CCArray ( );

    if ( pArray && pArray->init ( ) )
    {
        pArray->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pArray );
    }
    
    return pArray;
}

CCArray* CCArray::createWithObject ( CCObject* pObject )
{
    CCArray*  pArray = new CCArray ( );

    if ( pArray && pArray->initWithObject ( pObject ) )
    {
        pArray->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pArray );
    }
    
    return pArray;
}

CCArray* CCArray::create ( CCObject* pObject, ... )
{
    KDVaListKHR  pArgs;
    KD_VA_START_KHR ( pArgs, pObject );
    
    CCArray*  pArray = create ( );
    if ( pArray && pObject )
    {
        pArray->addObject ( pObject );

        CCObject*  pObject2 = KD_VA_ARG ( pArgs, CCObject* );
        while ( pObject2 ) 
        {
            pArray->addObject ( pObject2 );
            pObject2 = KD_VA_ARG ( pArgs, CCObject* );
        }
    }
    else
    {
        CC_SAFE_DELETE ( pArray );
    }

    KD_VA_END_KHR ( pArgs );
    
    return pArray;
}

CCArray* CCArray::createWithCapacity ( KDuint uCapacity )
{
    CCArray*  pArray = new CCArray ( );

    if ( pArray && pArray->initWithCapacity ( uCapacity ) )
    {
        pArray->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pArray );
    }

    return pArray;
}

CCArray* CCArray::createWithArray ( CCArray* pOtherArray )
{
	CCArray*  pArray = (CCArray*) pOtherArray->copy ( );

    if ( pArray )
    {
        pArray->autorelease ( );
    }

    return pArray;
}

CCArray* CCArray::createWithContentsOfFile ( const KDchar* szFileName )
{
    CCArray*  pRet = createWithContentsOfFileThreadSafe ( szFileName );
    
	if ( pRet )
    {
        pRet->autorelease();
    }

    return pRet;
}

CCArray* CCArray::createWithContentsOfFileThreadSafe ( const KDchar* szFileName )
{
    return CCFileUtils::sharedFileUtils ( )->createCCArrayWithContentsOfFile ( szFileName );
}

KDbool CCArray::init ( KDvoid )
{
    return initWithCapacity ( 1 );
}

KDbool CCArray::initWithObject ( CCObject* pObject )
{
    ccArrayFree ( m_pData );

    KDbool bRet = initWithCapacity ( 1 );
    
	if ( bRet )
    {
        addObject ( pObject );
    }

    return bRet;
}

/** Initializes an array with some objects */
KDbool CCArray::initWithObjects ( CCObject* pObject, ... )
{
    ccArrayFree ( m_pData );

    KDbool  bRet = KD_FALSE;
    do 
    {
        CC_BREAK_IF ( pObject == KD_NULL );

        KDVaListKHR  pArgs;
        KD_VA_START_KHR ( pArgs, pObject );

        if ( pObject )
        {
            this->addObject ( pObject );

            CCObject*  pObject2 = KD_VA_ARG ( pArgs, CCObject* );
            while ( pObject2 ) 
            {
                this->addObject ( pObject2 );
                pObject2 = KD_VA_ARG ( pArgs, CCObject* );
            }

            bRet = KD_TRUE;
        }

        KD_VA_END_KHR ( pArgs );

    } while ( 0 );

    return bRet;
}

KDbool CCArray::initWithCapacity ( KDuint uCapacity )
{
	ccArrayFree ( m_pData );
    m_pData = ccArrayNew ( uCapacity );
    return KD_TRUE;
}

KDbool CCArray::initWithArray ( CCArray* pOtherArray )
{
	ccArrayFree ( m_pData );

    KDbool  bRet = KD_FALSE;
    do 
    {
        CC_BREAK_IF ( !initWithCapacity ( pOtherArray->m_pData->num ) );

        addObjectsFromArray ( pOtherArray );

        bRet = KD_TRUE;

    } while ( 0 );
    
    return bRet;
}

KDuint CCArray::count ( KDvoid ) const
{
    return m_pData->num;
}

KDuint CCArray::capacity ( KDvoid ) const
{
    return m_pData->max;
}

KDuint CCArray::indexOfObject ( CCObject* pObject ) const
{
    return ccArrayGetIndexOfObject ( m_pData, pObject );
}

CCObject* CCArray::objectAtIndex ( KDuint uIndex )
{
	CCAssert ( uIndex < m_pData->num, "index out of range in objectAtIndex ( )" );

	return m_pData->arr [ uIndex ];
}

CCObject* CCArray::lastObject ( KDvoid )
{
    if ( m_pData->num > 0 )
	{
        return m_pData->arr [ m_pData->num - 1 ];
	}

    return KD_NULL;
}

CCObject* CCArray::randomObject ( KDvoid )
{
    if ( m_pData->num == 0 )
	{
		return KD_NULL;
	}

	KDfloat  r = CCRANDOM_0_1 ( );
    
    if ( r == 1 ) // to prevent from accessing data-arr[data->num], out of range.
    {
        r = 0;
    }
    
    return m_pData->arr [ (KDint) ( m_pData->num * r ) ];
}

KDbool CCArray::containsObject ( CCObject* pObject ) const
{
    return ccArrayContainsObject ( m_pData, pObject );
}

KDbool CCArray::isEqualToArray ( CCArray* pOtherArray )
{
    for ( KDuint  i = 0; i < this->count ( ); i++ )
    {
        if ( !this->objectAtIndex ( i )->isEqual ( pOtherArray->objectAtIndex ( i ) ) )
        {
            return KD_FALSE;
        }
    }

    return KD_TRUE;
}

KDvoid CCArray::addObject ( CCObject* pObject )
{
    ccArrayAppendObjectWithResize ( m_pData, pObject );
}

KDvoid CCArray::addObjectsFromArray ( CCArray* pOtherArray )
{
    ccArrayAppendArrayWithResize ( m_pData, pOtherArray->m_pData );
}

KDvoid CCArray::insertObject ( CCObject* pObject, KDuint uIndex )
{
    ccArrayInsertObjectAtIndex ( m_pData, pObject, uIndex );
}

KDvoid CCArray::removeLastObject ( KDbool bReleaseObj )
{
    CCAssert ( m_pData->num, "no objects added" );
    ccArrayRemoveObjectAtIndex ( m_pData, m_pData->num - 1, bReleaseObj );
}

KDvoid CCArray::removeObject ( CCObject* pObject, KDbool bReleaseObj )
{
    ccArrayRemoveObject ( m_pData, pObject, bReleaseObj );
}

KDvoid CCArray::removeObjectAtIndex ( KDuint uIndex, KDbool bReleaseObj )
{
    ccArrayRemoveObjectAtIndex ( m_pData, uIndex, bReleaseObj );
}

KDvoid CCArray::removeObjectsInArray ( CCArray* pOtherArray )
{
    ccArrayRemoveArray ( m_pData, pOtherArray->m_pData );
}

KDvoid CCArray::removeAllObjects ( KDvoid )
{
    ccArrayRemoveAllObjects ( m_pData );
}

KDvoid CCArray::fastRemoveObjectAtIndex ( KDuint uIndex )
{
    ccArrayFastRemoveObjectAtIndex ( m_pData, uIndex );
}

KDvoid CCArray::fastRemoveObject ( CCObject* pObject )
{
    ccArrayFastRemoveObject ( m_pData, pObject );
}

KDvoid CCArray::exchangeObject ( CCObject* pObject1, CCObject* pObject2 )
{
    KDuint  uIndex1 = ccArrayGetIndexOfObject ( m_pData, pObject1 );
    
    if ( uIndex1 == KDUINT_MAX )
    {
        return;
    }

    KDuint  uIndex2 = ccArrayGetIndexOfObject ( m_pData, pObject2 );
    
    if ( uIndex2 == KDUINT_MAX )
    {
        return;
    }

    ccArraySwapObjectsAtIndexes ( m_pData, uIndex1, uIndex2 );
}

KDvoid CCArray::exchangeObjectAtIndex ( KDuint uIndex1, KDuint uIndex2 )
{
    ccArraySwapObjectsAtIndexes ( m_pData, uIndex1, uIndex2 );
}

KDvoid CCArray::replaceObjectAtIndex ( KDuint uIndex, CCObject* pObject, KDbool bReleaseObject )
{
    ccArrayInsertObjectAtIndex ( m_pData, pObject, uIndex );
    ccArrayRemoveObjectAtIndex ( m_pData, uIndex + 1 );
}

KDvoid CCArray::reverseObjects ( KDvoid )
{
    if ( m_pData->num > 1 )
    {
        // floorf(), since in the case of an even number, the number of swaps stays the same
        KDint   nCount = (KDint) kdFloorf ( m_pData->num / 2.f ); 
        KDuint  uMaxIndex = m_pData->num - 1;

        for ( KDint i = 0; i < nCount ; i++ )
        {
            ccArraySwapObjectsAtIndexes ( m_pData, i, uMaxIndex );
            uMaxIndex--;
        }
    }
}

KDvoid CCArray::reduceMemoryFootprint ( KDvoid )
{
    ccArrayShrink ( m_pData );
}

CCObject* CCArray::copyWithZone ( CCZone* pZone )
{
    CCAssert ( pZone == KD_NULL, "CCArray should not be inherited." );

    CCArray*   pArray = new CCArray ( );
    pArray->initWithCapacity ( this->m_pData->num > 0 ? this->m_pData->num : 1 );

    CCObject*  pObject  = KD_NULL;
	CCObject*  pTempObj = KD_NULL;
    CCARRAY_FOREACH ( this, pObject )
    {
		pTempObj = pObject->copy ( );
        pArray->addObject ( pTempObj );
        pTempObj->release ( );
    }

    return pArray;
}

KDvoid CCArray::acceptVisitor ( CCDataVisitor& tVisitor )
{
	tVisitor.visit ( this );
}

NS_CC_END