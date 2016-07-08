/* -----------------------------------------------------------------------------------
 *
 *      File            ccCArray.h 
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

#ifndef __ccCArray_h__
#define __ccCArray_h__

#include "../../cocoa/CCObject.h"

NS_CC_BEGIN
    
/** 
 *	@file
 *	based on Chipmunk cpArray.
 *	ccArray is a faster alternative to NSMutableArray, it does pretty much the
 *	same thing (stores NSObjects and retains/releases them appropriately). It's
 *	faster because:
 *		- it uses a plain C interface so it doesn't incur Objective-c messaging overhead 
 *		- it assumes you know what you're doing, so it doesn't spend time on safety checks
 *		  (index out of bounds, required capacity etc.)
 *		- comparisons are done using pointer equality instead of isEqual
 *
 *	There are 2 kind of functions:
 *		- ccArray functions that manipulates objective-c objects (retain and release are performanced)
 *		- ccCArray functions that manipulates values like if they were standard C structures (no retain/release is performed)
 */

 #define CC_INVALID_INDEX	0xffffffff

/// Easy integration	
#define CCARRAYDATA_FOREACH( __array__, __object__ )		\
__object__=__array__->arr[0]; for ( KDuint i=0, num=__array__->num; i<num; i++, __object__=__array__->arr[i])	


typedef struct _ccArray 
{
	KDuint        num;
	KDuint        max;
	CCObject**    arr; 
} ccArray;

/** Allocates and initializes a new array with specified capacity */
ccArray*  ccArrayNew ( KDuint uCapacity );

/** Frees array after removing all remaining objects. Silently ignores nil arr. */
KDvoid  ccArrayFree ( ccArray*& pArray );

/** Doubles array capacity */
KDvoid  ccArrayDoubleCapacity ( ccArray* pArray );

/** Increases array capacity such that max >= num + extra. */
KDvoid  ccArrayEnsureExtraCapacity ( ccArray* pArray, KDuint uExtra );

/** shrinks the array so the memory footprint corresponds with the number of items */
KDvoid  ccArrayShrink ( ccArray* pArray );

/** Returns index of first occurrence of object, UXNotFound if object not found. */
KDuint  ccArrayGetIndexOfObject ( ccArray* pArray, CCObject* pObject );

/** Returns a Boolean value that indicates whether object is present in array. */
KDbool  ccArrayContainsObject ( ccArray* pArray, CCObject* pObject );

/** Appends an object. Bahaviour undefined if array doesn't have enough capacity. */
KDvoid  ccArrayAppendObject ( ccArray* pArray, CCObject* pObject );

/** Appends an object. Capacity of arr is increased if needed. */
KDvoid  ccArrayAppendObjectWithResize ( ccArray* pArray, CCObject* pObject );

/** Appends objects from plusArr to arr. Behavior undefined if arr doesn't have enough capacity. */
KDvoid  ccArrayAppendArray ( ccArray* pArray, ccArray* pPlusArray );

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
KDvoid  ccArrayAppendArrayWithResize ( ccArray* pArray, ccArray* pPlusArray );

/** Inserts an object at index */
KDvoid  ccArrayInsertObjectAtIndex ( ccArray* pArray, CCObject* pObject, KDuint uIndex );

/** Swaps two objects */
KDvoid  ccArraySwapObjectsAtIndexes ( ccArray* pArray, KDuint uIndex1, KDuint uIndex2 );

/** Removes all objects from arr */
KDvoid  ccArrayRemoveAllObjects ( ccArray* pArray );

/** 
 *	Removes object at specified index and pushes back all subsequent objects.
 *	Behavior undefined if index outside [0, num-1]. 
 */
KDvoid  ccArrayRemoveObjectAtIndex ( ccArray* pArray, KDuint uIndex, KDbool bReleaseObj = KD_TRUE );

/**
 *	Removes object at specified index and fills the gap with the last object,
 *	thereby avoiding the need to push back subsequent objects.
 *	Behavior undefined if index outside [0, num-1]. 
 */
KDvoid  ccArrayFastRemoveObjectAtIndex ( ccArray* pArray, KDuint uIndex );

KDvoid  ccArrayFastRemoveObject ( ccArray* pArray, CCObject* pObject );

/**
 *	Searches for the first occurrence of object and removes it. If object is not
 *	found the function has no effect. 
 */
KDvoid  ccArrayRemoveObject ( ccArray* pArray, CCObject* pObject, KDbool bReleaseObj = KD_TRUE );

/** 
 *	Removes from arr all objects in minusArr. For each object in minusArr, the
 *	first matching instance in arr will be removed. 
 */
KDvoid  ccArrayRemoveArray ( ccArray* pArray, ccArray* pMinusArray );

/** 
 *	Removes from arr all objects in minusArr. For each object in minusArr, all
 *	matching instances in arr will be removed. 
 */
KDvoid  ccArrayFullRemoveArray ( ccArray* pArray, ccArray* pMinusArray );

typedef struct _ccCArray 
{
	KDuint      num;
	KDuint      max;
	KDvoid**    arr; 
} ccCArray;

/** Allocates and initializes a new C array with specified capacity */
ccCArray*  ccCArrayNew ( KDuint uCapacity );

/** Frees C array after removing all remaining values. Silently ignores nil arr. */
KDvoid  ccCArrayFree ( ccCArray* pArray );

/** Doubles C array capacity */
KDvoid  ccCArrayDoubleCapacity ( ccCArray* pArray );

/** Increases array capacity such that max >= num + extra. */
KDvoid  ccCArrayEnsureExtraCapacity ( ccCArray* pArray, KDuint uExtra );

/** Returns index of first occurence of value, NSNotFound if value not found. */
KDuint  ccCArrayGetIndexOfValue ( ccCArray* pArray, KDvoid* pValue );

/** Returns a Boolean value that indicates whether value is present in the C array. */
KDbool  ccCArrayContainsValue ( ccCArray* pArray, KDvoid* pValue );

/** Inserts a value at a certain position. The valid index is [0, num] */
KDvoid  ccCArrayInsertValueAtIndex ( ccCArray* pArray, KDvoid* value, KDuint uIndex );

/** Appends an value */
KDvoid  ccCArrayAppendValue ( ccCArray* pArray, KDvoid* pValue );

/** Appends an value. Capacity of arr is increased if needed. */
KDvoid  ccCArrayAppendValueWithResize ( ccCArray* pArray, KDvoid* pValue );

/** Appends values from plusArr to arr. Behavior undefined if arr doesn't have enough capacity. */
KDvoid  ccCArrayAppendArray ( ccCArray* pArray, ccCArray* pPlusArr );

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
KDvoid  ccCArrayAppendArrayWithResize ( ccCArray* pArray, ccCArray* pPlusArr );

/** Removes all values from arr */
KDvoid  ccCArrayRemoveAllValues ( ccCArray* pArray );

/** 
 *	Removes value at specified index and pushes back all subsequent values.
 *	Behavior undefined if index outside [0, num-1]. 
 *	@since v0.99.4
 */
KDvoid  ccCArrayRemoveValueAtIndex ( ccCArray* pArray, KDuint uIndex );

/** 
 *	Removes value at specified index and fills the gap with the last value,
 *	thereby avoiding the need to push back subsequent values.
 *	Behavior undefined if index outside [0, num-1]. 
 *	@since v0.99.4
 */
KDvoid  ccCArrayFastRemoveValueAtIndex ( ccCArray* pArray, KDuint uIndex );

/** 
 *	Searches for the first occurrence of value and removes it. If value is not
 *	found the function has no effect. 
 *	@since v0.99.4
 */
KDvoid  ccCArrayRemoveValue ( ccCArray* pArray, KDvoid* pValue );

/** 
 *	Removes from arr all values in minusArr. For each Value in minusArr, the
 *	first matching instance in arr will be removed. 
 *	@since v0.99.4
 */
KDvoid  ccCArrayRemoveArray ( ccCArray* pArray, ccCArray* pMinusArr );

/**
 *	Removes from arr all values in minusArr. For each value in minusArr, all
 *	matching instances in arr will be removed. 
 *	@since v0.99.4
 */
KDvoid  ccCArrayFullRemoveArray ( ccCArray* pArray, ccCArray* pMinusArr );

NS_CC_END

#endif // __ccCArray_h__
