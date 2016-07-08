/* -----------------------------------------------------------------------------------
 *
 *      File            ccCArray.cpp 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2007      Scott Lembcke
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
#include "support/data_support/ccCArray.h"
#include "cocoa/CCObject.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN

ccArray* ccArrayNew ( KDuint uCapacity ) 
{
	if ( uCapacity == 0 )
	{
		uCapacity = 1; 
	}
	
	ccArray*  pArray = (ccArray*) kdMalloc ( sizeof ( ccArray ) );
	pArray->num = 0;
	
	pArray->arr = (CCObject**) kdMalloc ( uCapacity * sizeof ( CCObject* ) );
	pArray->max = uCapacity;
	
	return pArray;
}

KDvoid ccArrayFree ( ccArray*& pArray )
{
	if ( pArray == KD_NULL ) 
	{
		return;
	}
	
	ccArrayRemoveAllObjects ( pArray );
	
	kdFree ( pArray->arr );
	kdFree ( pArray );
	pArray = KD_NULL;
}

KDvoid ccArrayDoubleCapacity ( ccArray* pArray )
{
	pArray->max *= 2;
	CCObject**  pNewArray = (CCObject**) kdRealloc ( pArray->arr, pArray->max * sizeof (CCObject *) );
    // will fail when there's not enough memory
    CCAssert ( pNewArray != 0, "ccArrayDoubleCapacity failed. Not enough memory" );
    pArray->arr = pNewArray;
}

KDvoid ccArrayEnsureExtraCapacity ( ccArray* pArray, KDuint uExtra )
{
	while ( pArray->max < pArray->num + uExtra )
	{
		ccArrayDoubleCapacity ( pArray ); 
	}
}

KDvoid ccArrayShrink ( ccArray* pArray )
{
    KDuint  uNewSize;

    // only resize when necessary
    if ( pArray->max > pArray->num && !( pArray->num == 0 && pArray->max == 1 ) )
    {
        if ( pArray->num != 0 ) 
        {
            uNewSize    = pArray->num;
            pArray->max = pArray->num; 
        }
        else 
        {
			// minimum capacity of 1, with 0 elements the array would be free'd by realloc
            uNewSize    = 1;
            pArray->max = 1;
        }

        pArray->arr = (CCObject**) kdRealloc ( pArray->arr, uNewSize * sizeof (CCObject*) );
        CCAssert ( pArray->arr != KD_NULL, "could not reallocate the memory" );
    }
} 

KDuint ccArrayGetIndexOfObject ( ccArray* pArray, CCObject* pObject )
{
	const KDuint	uArrNum = pArray->num;
	CCObject**		pPtr = pArray->arr;

	for ( KDuint i = 0; i < uArrNum; ++i, ++pPtr )
	{
		if ( *pPtr == pObject )
		{
			return i;
		}
	}

	return CC_INVALID_INDEX; 
}

KDbool ccArrayContainsObject ( ccArray* pArray, CCObject* pObject )
{
	return ccArrayGetIndexOfObject ( pArray, pObject ) != CC_INVALID_INDEX;
}

KDvoid ccArrayAppendObject ( ccArray* pArray, CCObject* pObject )
{
	CCAssert ( pObject != KD_NULL, "Invalid parameter!" );

	pObject->retain ( );
	pArray->arr[ pArray->num ] = pObject; 
	pArray->num++;
}

KDvoid ccArrayAppendObjectWithResize ( ccArray* pArray, CCObject* pObject )
{
	ccArrayEnsureExtraCapacity ( pArray, 1 );
	ccArrayAppendObject ( pArray, pObject );
}

KDvoid ccArrayAppendArray ( ccArray* pArray, ccArray* pPlusArray )
{
	for ( KDuint i = 0; i < pPlusArray->num; i++ )
	{
		ccArrayAppendObject ( pArray, pPlusArray->arr[ i ] ); 
	}
}

KDvoid ccArrayAppendArrayWithResize ( ccArray* pArray, ccArray* pPlusArray )
{
	ccArrayEnsureExtraCapacity ( pArray, pPlusArray->num ); 
	ccArrayAppendArray ( pArray, pPlusArray );
}

KDvoid ccArrayInsertObjectAtIndex ( ccArray* pArray, CCObject* pObject, KDuint uIndex )
{
    CCAssert ( uIndex <= pArray->num, "Invalid index. Out of bounds" );
	CCAssert ( pObject != KD_NULL, "Invalid parameter!" );

    ccArrayEnsureExtraCapacity ( pArray, 1 );

    KDint  uRemaining = pArray->num - uIndex;
    if ( uRemaining > 0 )
	{
        kdMemmove ( &pArray->arr [ uIndex + 1 ], &pArray->arr [ uIndex ], sizeof ( CCObject* ) * uRemaining );
	}

    pObject->retain ( );
    pArray->arr [ uIndex ] = pObject;
    pArray->num++;
}

KDvoid ccArraySwapObjectsAtIndexes ( ccArray* pArray, KDuint uIndex1, KDuint uIndex2 )
{
    CCAssert ( uIndex1 < pArray->num, "(1) Invalid index. Out of bounds" );
    CCAssert ( uIndex2 < pArray->num, "(2) Invalid index. Out of bounds" );

    CCObject*  pObject1 = pArray->arr [ uIndex1 ];

    pArray->arr [ uIndex1 ] = pArray->arr [ uIndex2 ];
    pArray->arr [ uIndex2 ] = pObject1;
}

KDvoid ccArrayRemoveAllObjects ( ccArray* pArray )
{
	while ( pArray->num > 0 )
	{
		pArray->arr[ --pArray->num ]->release ( );
	}
}

KDvoid ccArrayRemoveObjectAtIndex ( ccArray* pArray, KDuint uIndex, KDbool bReleaseObj )
{
	CCAssert ( pArray && pArray->num > 0 && uIndex < pArray->num, "Invalid index. Out of bounds" );
    if ( bReleaseObj )
    {
        CC_SAFE_RELEASE ( pArray->arr [ uIndex ] );
    }

	pArray->num--;

	KDuint  uRemaining = pArray->num - uIndex;
	if ( uRemaining > 0 )
	{
		kdMemmove ( &pArray->arr [ uIndex ], &pArray->arr [ uIndex + 1 ], uRemaining * sizeof (CCObject*) );
	}
}

KDvoid ccArrayFastRemoveObjectAtIndex ( ccArray* pArray, KDuint uIndex )
{
	CC_SAFE_RELEASE ( pArray->arr [ uIndex ] );

	KDuint  uLast = --pArray->num;
	pArray->arr [ uIndex ] = pArray->arr [ uLast ];
}

KDvoid ccArrayFastRemoveObject ( ccArray* pArray, CCObject* pObject )
{
    KDuint  uIndex = ccArrayGetIndexOfObject ( pArray, pObject );
    
    if ( uIndex != CC_INVALID_INDEX )
	{
        ccArrayFastRemoveObjectAtIndex ( pArray, uIndex );
	}
}

KDvoid ccArrayRemoveObject ( ccArray* pArray, CCObject* pObject, KDbool bReleaseObj )
{
	KDuint  uIndex = ccArrayGetIndexOfObject ( pArray, pObject );

	if ( uIndex != CC_INVALID_INDEX )
	{
		ccArrayRemoveObjectAtIndex ( pArray, uIndex, bReleaseObj );
	}
}

KDvoid ccArrayRemoveArray ( ccArray* pArray, ccArray* pMinusArray )
{
	for ( KDuint i = 0; i < pMinusArray->num; i++ )
	{
		ccArrayRemoveObject ( pArray, pMinusArray->arr [ i ] );
	}
}

KDvoid ccArrayFullRemoveArray ( ccArray* pArray, ccArray* pMinusArray )
{
	KDuint  uBack = 0;
	
	for ( KDuint i = 0; i < pArray->num; i++ ) 
	{
		if( ccArrayContainsObject ( pMinusArray, pArray->arr [ i ] ) ) 
		{
			CC_SAFE_RELEASE ( pArray->arr [ i ] );
			uBack++;
		}
		else
		{
			pArray->arr [ i - uBack ] = pArray->arr [ i ];
		}
	}
	
	pArray->num -= uBack;
}

ccCArray* ccCArrayNew ( KDuint uCapacity ) 
{
	if ( uCapacity == 0 )
	{
		uCapacity = 1; 
	}
	
	ccCArray*  pArray = (ccCArray *) kdMalloc ( sizeof ( ccCArray ) );
    
	pArray->num = 0;
	pArray->arr =  (KDvoid **) kdMalloc ( uCapacity * sizeof ( KDvoid* ) );
	pArray->max = uCapacity;
	
	return pArray;
}

KDvoid ccCArrayFree ( ccCArray* pArray )
{
	if ( pArray == KD_NULL ) 
	{
		return;
	}
	
	ccCArrayRemoveAllValues ( pArray );
	
	kdFree ( pArray->arr );
	kdFree ( pArray );
}

KDvoid ccCArrayDoubleCapacity ( ccCArray* pArray )
{
	ccArrayDoubleCapacity ( (ccArray*) pArray );
}

KDvoid ccCArrayEnsureExtraCapacity ( ccCArray* pArray, KDuint uExtra )
{
	ccArrayEnsureExtraCapacity ( (ccArray*) pArray, uExtra );
}

KDuint ccCArrayGetIndexOfValue ( ccCArray* pArray, KDvoid* pValue )
{
	for ( KDuint i = 0; i < pArray->num; i++ )
	{
		if ( pArray->arr [ i ] == pValue ) 
		{
			return i;
		}
	}

	return CC_INVALID_INDEX;
}

KDbool ccCArrayContainsValue ( ccCArray* pArray, KDvoid* pValue )
{
	return ccCArrayGetIndexOfValue ( pArray, pValue ) != CC_INVALID_INDEX;
}

KDvoid ccCArrayInsertValueAtIndex ( ccCArray* pArray, KDvoid* value, KDuint uIndex )
{
    CCAssert ( uIndex < pArray->max, "ccCArrayInsertValueAtIndex: invalid index" );
	KDuint  uRemaining = pArray->num - uIndex;

	// make sure it has enough capacity
	if ( pArray->num + 1 == pArray->max )
	{
        ccCArrayDoubleCapacity ( pArray );
	}
	
	// last Value doesn't need to be moved
	if ( uRemaining > 0 ) 
	{
		// tex coordinates
		kdMemmove ( &pArray->arr [ uIndex + 1 ], &pArray->arr [ uIndex ], sizeof ( KDvoid* ) * uRemaining );
	}
	
	pArray->num++;	
	pArray->arr [ uIndex ] = value;
}

KDvoid ccCArrayAppendValue ( ccCArray* pArray, KDvoid* pValue )
{
	pArray->arr [ pArray->num ] = pValue;
	pArray->num++;

	// double the capacity for the next append action
	// if the num >= max
	if ( pArray->num >= pArray->max )
	{
		ccCArrayDoubleCapacity ( pArray );
	}
}

KDvoid ccCArrayAppendValueWithResize ( ccCArray* pArray, KDvoid* pValue )
{
	ccCArrayEnsureExtraCapacity ( pArray, 1 );
	ccCArrayAppendValue ( pArray, pValue );
}

KDvoid ccCArrayAppendArray ( ccCArray* pArray, ccCArray* pPlusArr )
{
	for ( KDuint i = 0; i < pPlusArr->num; i++ )
	{
		ccCArrayAppendValue ( pArray, pPlusArr->arr[ i ] );
	}
}

KDvoid ccCArrayAppendArrayWithResize ( ccCArray* pArray, ccCArray* pPlusArr )
{
	ccCArrayEnsureExtraCapacity ( pArray, pPlusArr->num );
	ccCArrayAppendArray ( pArray, pPlusArr );
}

KDvoid ccCArrayRemoveAllValues ( ccCArray* pArray )
{
	pArray->num = 0;
}

KDvoid ccCArrayRemoveValueAtIndex ( ccCArray* pArray, KDuint uIndex )
{	
	for ( KDuint uLast = --pArray->num; uIndex < uLast; uIndex++ )
	{
		pArray->arr [ uIndex ] = pArray->arr [ uIndex + 1 ];
	}
}

KDvoid ccCArrayFastRemoveValueAtIndex ( ccCArray* pArray, KDuint uIndex )
{
	KDuint  uLast = --pArray->num;
	pArray->arr [ uIndex ] = pArray->arr [ uLast ];
}

KDvoid ccCArrayRemoveValue ( ccCArray* pArray, KDvoid* pValue )
{
	KDuint  uIndex = ccCArrayGetIndexOfValue ( pArray, pValue );
    
	if ( uIndex != CC_INVALID_INDEX )
	{
		ccCArrayRemoveValueAtIndex ( pArray, uIndex );
	}
}

KDvoid ccCArrayRemoveArray ( ccCArray* pArray, ccCArray* pMinusArr )
{
	for ( KDuint i = 0; i < pMinusArr->num; i++ )
	{
		ccCArrayRemoveValue ( pArray, pMinusArr->arr [ i ] );
	}
}

KDvoid ccCArrayFullRemoveArray ( ccCArray* pArray, ccCArray* pMinusArr )
{
	KDuint  uBack = 0;
	
	for ( KDuint i = 0; i < pArray->num; i++ ) 
	{
		if ( ccCArrayContainsValue ( pMinusArr, pArray->arr [ i ] ) ) 
		{
			uBack++;
		}
		else
		{
			pArray->arr [ i - uBack ] = pArray->arr [ i ];
		}
	}
	
	pArray->num -= uBack;
} 

NS_CC_END
