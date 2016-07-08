/* -----------------------------------------------------------------------------------
 *
 *      File            ccCArray.h 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

/**
 *	@file
 *	based on Chipmunk cpArray.
 *	ccArray is a faster alternative to NSMutableArray, it does pretty much the
 *	same thing (stores NSObjects and retains/releases them appropriately). It's
 *	faster because:
 *		- it uses a plain C interface so it doesn't incur Objective-c messaging overhead
 *		- it assumes you know what you're doing, so it doesn't spend time on safety checks
 *		(index out of bounds, required capacity etc.)
 *		- comparisons are done using pointer equality instead of isEqual
 *
 *	There are 2 kind of functions:
 *		- ccArray functions that manipulates objective-c objects (retain and release are performed)
 *		- ccCArray functions that manipulates values like if they were standard C structures (no retain/release is performed)
 */

#ifndef __ccCArray_h__
#define __ccCArray_h__

#include "../../ccMacros.h"
#include "../../base/CCObject.h"

NS_CC_BEGIN

extern const KDint32	CC_INVALID_INDEX;

// Easy integration
#define CCARRAYDATA_FOREACH(__array__, __object__)																\
__object__=__array__->arr[0]; for(KDint32 i=0, num=__array__->num; i<num; i++, __object__=__array__->arr[i])	\


typedef struct _ccArray 
{
	KDint32		num, max;
	Object**	arr;
} ccArray;

/** Allocates and initializes a new array with specified capacity */
ccArray*	ccArrayNew ( KDint32 capacity );

/** Frees array after removing all remaining objects. Silently ignores nil arr. */
KDvoid		ccArrayFree ( ccArray*& arr );

/** Doubles array capacity */
KDvoid		ccArrayDoubleCapacity ( ccArray* arr );

/** Increases array capacity such that max >= num + extra. */
KDvoid		ccArrayEnsureExtraCapacity ( ccArray* arr, KDint32 extra );

/** shrinks the array so the memory footprint corresponds with the number of items */
KDvoid		ccArrayShrink(ccArray* arr);

/** Returns index of first occurrence of object, NSNotFound if object not found. */
KDint32		ccArrayGetIndexOfObject(ccArray* arr, Object* object );

/** Returns a Boolean value that indicates whether object is present in array. */
KDbool		ccArrayContainsObject(ccArray* arr, Object* object );

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
KDvoid		ccArrayAppendObject(ccArray* arr, Object* object );

/** Appends an object. Capacity of arr is increased if needed. */
KDvoid		ccArrayAppendObjectWithResize ( ccArray* arr, Object* object );

/** 
 *	Appends objects from plusArr to arr. 
 *	Behavior undefined if arr doesn't have enough capacity. 
 */
KDvoid		ccArrayAppendArray ( ccArray* arr, ccArray* plusArr );

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
KDvoid		ccArrayAppendArrayWithResize ( ccArray* arr, ccArray* plusArr );

/** Inserts an object at index */
KDvoid		ccArrayInsertObjectAtIndex ( ccArray* arr, Object* object, KDint32 index );

/** Swaps two objects */
KDvoid		ccArraySwapObjectsAtIndexes ( ccArray* arr, KDint32 index1, KDint32 index2 );

/** Removes all objects from arr */
KDvoid		ccArrayRemoveAllObjects ( ccArray* arr );

/** 
 *	Removes object at specified index and pushes back all subsequent objects.
 *	Behavior undefined if index outside [0, num-1]. 
 */
KDvoid		ccArrayRemoveObjectAtIndex ( ccArray* arr, KDint32 index, KDbool bReleaseObj = true );

/** 
 *	Removes object at specified index and fills the gap with the last object,
 *	thereby avoiding the need to push back subsequent objects.
 *	Behavior undefined if index outside [0, num-1]. 
 */
KDvoid		ccArrayFastRemoveObjectAtIndex ( ccArray* arr, KDint32 index );

KDvoid		ccArrayFastRemoveObject(ccArray* arr, Object* object);

/** 
 *	Searches for the first occurrence of object and removes it. If object is not
 *	found the function has no effect. 
 */
KDvoid		ccArrayRemoveObject ( ccArray* arr, Object* object, KDbool bReleaseObj = true );

/** 
 *	Removes from arr all objects in minusArr. For each object in minusArr, the
 *	first matching instance in arr will be removed. 
 */
KDvoid		ccArrayRemoveArray ( ccArray* arr, ccArray* minusArr );

/**
 *	Removes from arr all objects in minusArr. For each object in minusArr, all
 *	matching instances in arr will be removed. 
 */
KDvoid		ccArrayFullRemoveArray ( ccArray* arr, ccArray* minusArr );

// #pragma mark -
// #pragma mark ccCArray for Values (c structures)

typedef struct _ccCArray
{
    KDint32		num, max;
    KDvoid**	arr;
} ccCArray;

/** Allocates and initializes a new C array with specified capacity */
ccCArray*	ccCArrayNew ( KDint32 capacity );

/** Frees C array after removing all remaining values. Silently ignores nil arr. */
KDvoid		ccCArrayFree ( ccCArray* arr );

/** Doubles C array capacity */
KDvoid		ccCArrayDoubleCapacity ( ccCArray* arr );

/** Increases array capacity such that max >= num + extra. */
KDvoid		ccCArrayEnsureExtraCapacity ( ccCArray* arr, KDint32 extra );

/** Returns index of first occurrence of value, NSNotFound if value not found. */
KDint32		ccCArrayGetIndexOfValue ( ccCArray* arr, KDvoid* value );

/** Returns a Boolean value that indicates whether value is present in the C array. */
KDbool		ccCArrayContainsValue ( ccCArray* arr, KDvoid* value );

/** Inserts a value at a certain position. Behavior undefined if array doesn't have enough capacity */
KDvoid		ccCArrayInsertValueAtIndex ( ccCArray* arr, KDvoid* value, KDint32 index );

/** Appends an value. Behavior undefined if array doesn't have enough capacity. */
KDvoid		ccCArrayAppendValue ( ccCArray* arr, KDvoid* value );

/** Appends an value. Capacity of arr is increased if needed. */
KDvoid		ccCArrayAppendValueWithResize ( ccCArray* arr, KDvoid* value );

/** 
 *	Appends values from plusArr to arr. Behavior undefined if arr doesn't have
 *	enough capacity. 
 */
KDvoid		ccCArrayAppendArray ( ccCArray* arr, ccCArray* plusArr );

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
KDvoid		ccCArrayAppendArrayWithResize ( ccCArray* arr, ccCArray* plusArr );

/** Removes all values from arr */
KDvoid		ccCArrayRemoveAllValues ( ccCArray* arr );

/** 
 *	Removes value at specified index and pushes back all subsequent values.
 *	Behavior undefined if index outside [0, num-1].
 *	@since v0.99.4
 */
KDvoid		ccCArrayRemoveValueAtIndex ( ccCArray* arr, KDint32 index );

/** 
 *	Removes value at specified index and fills the gap with the last value,
 *	thereby avoiding the need to push back subsequent values.
 *	Behavior undefined if index outside [0, num-1].
 *	@since v0.99.4
 */
KDvoid		ccCArrayFastRemoveValueAtIndex ( ccCArray* arr, KDint32 index );

/** 
 *	Searches for the first occurrence of value and removes it. If value is not found the function has no effect.
 *	@since v0.99.4
 */
KDvoid		ccCArrayRemoveValue ( ccCArray* arr, KDvoid* value );

/** 
 *	Removes from arr all values in minusArr. For each Value in minusArr, the first matching instance in arr will be removed.
 *	@since v0.99.4
 */
KDvoid		ccCArrayRemoveArray ( ccCArray* arr, ccCArray* minusArr );

/**
 *	Removes from arr all values in minusArr. For each value in minusArr, all matching instances in arr will be removed.
 *	@since v0.99.4
 */
KDvoid		ccCArrayFullRemoveArray ( ccCArray* arr, ccCArray* minusArr );

NS_CC_END
	
#endif	// __ccCArray_h__
