/* -----------------------------------------------------------------------------------
 *
 *      File            CCArray.h
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

#ifndef __CCArray_h__
#define __CCArray_h__

#include "../support/data_support/ccCArray.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

/** @def CCARRAY_FOREACH
 *	A convenience macro to iterate over a CCArray using. It is faster than the "fast enumeration" interface.
 *	@since v0.99.4
 */

/*
 *	In cocos2d-iphone 1.0.0, This macro have been update to like this:
 *
 *	#define CCARRAY_FOREACH(__array__, __object__)																	\
 *	if (__array__ && __array__->data->num > 0)																		\
 *	for(id *__arr__ = __array__->data->arr, *end = __array__->data->arr + __array__->data->num-1;					\
 *	__arr__ <= end && ((__object__ = *__arr__) != nil || true);														\
 *	__arr__++)
 *
 *	I found that it's not work in C++. So it keep what it's look like in version 1.0.0-rc3. ---By Bin
 */
#define CCARRAY_FOREACH(__array__, __object__)																					\
    if ((__array__) && (__array__)->m_pData->num > 0)																			\
    for(CCObject** __arr__ = (__array__)->m_pData->arr, **__end__ = (__array__)->m_pData->arr + (__array__)->m_pData->num-1;	\
    __arr__ <= __end__ && (((__object__) = *__arr__) != KD_NULL/* || true*/);													\
    __arr__++)

#define CCARRAY_FOREACH_REVERSE(__array__, __object__)																			\
    if ((__array__) && (__array__)->m_pData->num > 0)																			\
    for(CCObject** __arr__ = (__array__)->m_pData->arr + (__array__)->m_pData->num-1, **__end__ = (__array__)->m_pData->arr;	\
    __arr__ >= __end__ && (((__object__) = *__arr__) != KD_NULL/* || true*/);													\
    __arr__--)

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
#define CCARRAY_VERIFY_TYPE(__array__, __type__)																				\
    do {																														\
        if ((__array__) && (__array__)->m_pData->num > 0)																		\
            for(CCObject** __arr__ = (__array__)->m_pData->arr,																	\
                **__end__ = (__array__)->m_pData->arr + (__array__)->m_pData->num-1; __arr__ <= __end__; __arr__++)				\
                CCAssert(dynamic_cast<__type__>(*__arr__), "element type is wrong!");											\
    } while(0)
#else
#define CCARRAY_VERIFY_TYPE(__array__, __type__) void(0)
#endif

#define arrayMakeObjectsPerformSelector( _pArray, _func, _elementType )							\
do																								\
{																								\
    if ( _pArray && _pArray->count ( ) > 0 )					                                \
    {																							\
        CCObject*  pChild;																		\
        CCARRAY_FOREACH ( _pArray, pChild )														\
        {																						\
            _elementType  pNode = (_elementType) pChild;										\
            if ( pNode )																		\
            {																					\
                pNode->_func ( );																\
            }																					\
        }																						\
    }																							\
} while ( 0 )

#define arrayMakeObjectsPerformSelectorWithObject( _pArray, _func, _pObject, _elementType )		\
do																								\
{																								\
    if ( _pArray && _pArray->count ( ) > 0 )													\
    {																							\
        CCObject*  pChild = KD_NULL;															\
        CCARRAY_FOREACH ( _pArray, pChild )														\
        {																						\
            _elementType pNode = (_elementType) pChild;											\
            if ( pNode )																		\
            {																					\
                pNode->_func ( _pObject );														\
            }																					\
        }																						\
    }																							\
} while ( 0 )	

class CCArray : public CCObject
{
	public :
				 CCArray ( KDvoid );
				 CCArray ( KDuint uCapacity );
		virtual ~CCArray ( KDvoid );

		/** Create an array */
		static CCArray*			create ( KDvoid );

		/** Create an array with some objects */
		static CCArray*			create ( CCObject* pObject, ... );

		/** Create an array with one object */
		static CCArray*			createWithObject ( CCObject* pObject );

		/** Create an array with capacity */
		static CCArray*			createWithCapacity ( KDuint uCapacity );

		/** Create an array with an existing array */
		static CCArray*			createWithArray ( CCArray* pOtherArray );

		/**
		 *	@brief   Generate a CCArray pointer by file
		 *	@param   pFileName  The file name of *.plist file
		 *	@return  The CCArray pointer generated from the file
		 */
		static CCArray*			createWithContentsOfFile ( const KDchar* szFileName );
	    
		/*
		 *	@brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
		 *	invoker should call release().
		 */
		static CCArray*			createWithContentsOfFileThreadSafe ( const KDchar* szFileName );

	public :

		/** Initializes an array */
		virtual KDbool			init ( KDvoid );

		/** Initializes an array with one object */
		virtual KDbool			initWithObject ( CCObject* pObject );

		/** Initializes an array with some objects */
		virtual KDbool			initWithObjects ( CCObject* pObject, ... );

		/** Initializes an array with capacity */
		virtual KDbool			initWithCapacity ( KDuint uCapacity );

		/** Initializes an array with an existing array */
		virtual KDbool			initWithArray ( CCArray* pOtherArray );

		// Querying an Array

		/** Returns element count of the array */
		KDuint					count ( KDvoid ) const;

		/** Returns capacity of the array */
		KDuint					capacity ( KDvoid ) const;

		/** Returns index of a certain object, return KDUINT_MAX if doesn't contain the object */
		KDuint					indexOfObject ( CCObject* pObject ) const;

		/** Returns an element with a certain index */
		CCObject*				objectAtIndex ( KDuint uIndex );

		/** Returns last element */
		CCObject*				lastObject ( KDvoid );

		/** Returns a random element */
		CCObject*				randomObject ( KDvoid );

		/** Returns a Boolean value that indicates whether object is present in array. */
		KDbool					containsObject ( CCObject* pObject ) const;

		/** @since 1.1 */
		KDbool					isEqualToArray ( CCArray* pOtherArray );

		// Adding Objects

		/** Add a certain object */
		KDvoid					addObject ( CCObject* pObject );

		/** Add all elements of an existing array */
		KDvoid					addObjectsFromArray ( CCArray* pOtherArray );

		/** Insert a certain object at a certain index */
		KDvoid					insertObject ( CCObject* pObject, KDuint uIndex );

		// Removing Objects

		/** Remove last object */
		KDvoid					removeLastObject ( KDbool bReleaseObj = KD_TRUE );

		/** Remove a certain object */
		KDvoid					removeObject ( CCObject* pObject, KDbool bReleaseObj = KD_TRUE );

		/** Remove an element with a certain index */
		KDvoid					removeObjectAtIndex ( KDuint uIndex, KDbool bReleaseObj = KD_TRUE );

		/** Remove all elements */
		KDvoid					removeObjectsInArray ( CCArray* pOtherArray );

		/** Remove all objects */
		KDvoid					removeAllObjects ( KDvoid );

		/** Fast way to remove a certain object */
		KDvoid					fastRemoveObject ( CCObject* pObject );

		/** Fast way to remove an element with a certain index */
		KDvoid					fastRemoveObjectAtIndex ( KDuint uIndex );

		// Rearranging Content

		/** Swap two elements */
		KDvoid					exchangeObject ( CCObject* pObject1, CCObject* pObject2 );

		/** Swap two elements with certain indexes */
		KDvoid					exchangeObjectAtIndex ( KDuint uIndex1, KDuint uIndex2 );

		/** Replace object at index with another object. */
		KDvoid					replaceObjectAtIndex ( KDuint uIndex, CCObject* pObject, KDbool bReleaseObject = KD_TRUE );

		/** Revers the array */
		KDvoid					reverseObjects ( KDvoid );

		/* Shrinks the array so the memory footprint corresponds with the number of items */
		KDvoid					reduceMemoryFootprint ( KDvoid );

		/* override functions */
		virtual CCObject*		copyWithZone ( CCZone* pZone );

		/* override functions */
		virtual KDvoid			acceptVisitor ( CCDataVisitor& tVisitor );

	public :
		
		ccArray*				m_pData;
};

// end of data_structure group
/// @}

NS_CC_END

#endif // __CCArray_h__
