/* -----------------------------------------------------------------------------------
 *
 *      File            CCArray.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCArray_h__
#define __CCArray_h__

#define CC_USE_ARRAY_VECTOR 0

#if CC_USE_ARRAY_VECTOR
#include <vector>
#include <algorithm>
#include "CCObject.h"
#include "ccMacros.h"
#else
#include "../support/data_support/ccCArray.h"
#endif


#if CC_USE_ARRAY_VECTOR
/**
 * A reference counting-managed pointer for classes derived from RCBase which can
 * be used as C pointer
 * Original code: http://www.codeproject.com/Articles/64111/Building-a-Quick-and-Handy-Reference-Counting-Clas
 * License: http://www.codeproject.com/info/cpol10.aspx
 */
template < class T >
class RCPtr
{
	public :

		//Construct using a C pointer
		//e.g. RCPtr< T > x = new T();
		RCPtr(T* ptr = nullptr)
		: _ptr(ptr)
		{
			if(ptr != nullptr) {ptr->retain();}
		}

		//Copy constructor
		RCPtr(const RCPtr &ptr)
		: _ptr(ptr._ptr)
		{
	//        printf("Array: copy constructor: %p\n", this);
			if(_ptr != NULL) {_ptr->retain();}
		}

		//Move constructor
		RCPtr(RCPtr &&ptr)
		: _ptr(ptr._ptr)
		{
	//        printf("Array: Move Constructor: %p\n", this);
			ptr._ptr = nullptr;
		}

		~RCPtr()
		{
	//        printf("Array: Destructor: %p\n", this);
			if(_ptr != nullptr) {_ptr->release();}
		}

		//Assign a pointer
		//e.g. x = new T();
		RCPtr &operator=(T* ptr)
		{
	//        printf("Array: operator= T*: %p\n", this);

			//The following grab and release operations have to be performed
			//in that order to handle the case where ptr == _ptr
			//(See comment below by David Garlisch)
			if(ptr != nullptr) {ptr->retain();}
			if(_ptr != nullptr) {_ptr->release();}
			_ptr = ptr;
			return (*this);
		}

		//Assign another RCPtr
		RCPtr &operator=(const RCPtr &ptr)
		{
	//        printf("Array: operator= const&: %p\n", this);
			return (*this) = ptr._ptr;
		}

		//Retrieve actual pointer
		T* get() const
		{
			return _ptr;
		}

		//Some overloaded operators to facilitate dealing with an RCPtr
		//as a conventional C pointer.
		//Without these operators, one can still use the less transparent
		//get() method to access the pointer.
		T* operator->() const {return _ptr;}		//x->member
		T &operator*() const {return *_ptr;}		//*x, (*x).member
		explicit operator T*() const {return _ptr;}		//T* y = x;
		explicit operator bool() const {return _ptr != nullptr;}	//if(x) {/*x is not NULL*/}
		bool operator==(const RCPtr &ptr) {return _ptr == ptr._ptr;}
		bool operator==(const T *ptr) {return _ptr == ptr;}

	private :

		T*			_ptr;	// Actual pointer
};
#endif // CC_USE_ARRAY_VECTOR


/**
 * @addtogroup data_structures
 * @{
 */

/** @def CCARRAY_FOREACH
A convenience macro to iterate over a Array using. It is faster than the "fast enumeration" interface.
@since v0.99.4
*/

/*
In cocos2d-iphone 1.0.0, This macro have been update to like this:

#define CCARRAY_FOREACH(__array__, __object__)																			\
if (__array__ && __array__->data->num > 0)																				\
for(id *__arr__ = __array__->data->arr, *end = __array__->data->arr + __array__->data->num-1;							\
__arr__ <= end && ((__object__ = *__arr__) != nil || true);																\
__arr__++)

I found that it's not work in C++. So it keep what it's look like in version 1.0.0-rc3. ---By Bin
*/

#if CC_USE_ARRAY_VECTOR
#define CCARRAY_FOREACH(__array__, __object__)																			\
    if (__array__)																										\
    for( auto __it__ = (__array__)->data.begin();																		\
        __it__ != (__array__)->data.end() && ((__object__) = __it__->get()) != nullptr;                    				\
        ++__it__)


#define CCARRAY_FOREACH_REVERSE(__array__, __object__)																	\
    if (__array__)																										\
    for( auto __it__ = (__array__)->data.rbegin();																		\
    __it__ != (__array__)->data.rend() && ((__object__) = __it__->get()) != nullptr;                        			\
    ++__it__ )


#define CCARRAY_VERIFY_TYPE(__array__, __type__) void(0)

#else // ! CC_USE_ARRAY_VECTOR --------------------------

#define CCARRAY_FOREACH(__array__, __object__)																			\
    if ((__array__) && (__array__)->data->num > 0)																		\
    for(Object** __arr__ = (__array__)->data->arr, **__end__ = (__array__)->data->arr + (__array__)->data->num-1;		\
    __arr__ <= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);												\
    __arr__++)

#define CCARRAY_FOREACH_REVERSE(__array__, __object__)                                                                  \
    if ((__array__) && (__array__)->data->num > 0)                                                                      \
    for(Object** __arr__ = (__array__)->data->arr + (__array__)->data->num-1, **__end__ = (__array__)->data->arr;		\
    __arr__ >= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);                                              \
    __arr__--)

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
#define CCARRAY_VERIFY_TYPE(__array__, __type__)																		\
    do {																												\
        if ((__array__) && (__array__)->data->num > 0)																	\
            for(Object** __arr__ = (__array__)->data->arr,																\
                **__end__ = (__array__)->data->arr + (__array__)->data->num-1; __arr__ <= __end__; __arr__++)			\
                CCASSERT(dynamic_cast<__type__>(*__arr__), "element type is wrong!");									\
    } while(false)
#else
#define CCARRAY_VERIFY_TYPE(__array__, __type__) void(0)
#endif

#endif // ! CC_USE_ARRAY_VECTOR


// Common defines -----------------------------------------------------------------------------------------------

#define arrayMakeObjectsPerformSelector(pArray, func, elementType)														\
do {                                                                  													\
    if(pArray && pArray->count() > 0)                                 													\
    {                                                                 													\
        Object* child;                                                													\
        CCARRAY_FOREACH(pArray, child)                                													\
        {                                                             													\
            elementType pNode = static_cast<elementType>(child);      													\
            if(pNode)                                                 													\
            {                                                         													\
                pNode->func();                                        													\
            }                                                         													\
        }                                                            													\
    }                                                                 													\
}                                                                     													\
while(false)

#define arrayMakeObjectsPerformSelectorWithObject(pArray, func, object, elementType)									\
do {                                                                  													\
    if(pArray && pArray->count() > 0)                                 													\
    {                                                                 													\
        Object* child;                                                													\
        CCARRAY_FOREACH(pArray, child)                               													\
        {                                                            													\
            elementType pNode = static_cast<elementType>(child);      													\
            if(pNode)                                                 													\
            {                                                         													\
                pNode->func(object);                                 													\
            }                                                         													\
        }                                                             													\
    }                                                                 													\
}                                                                     													\
while(false)


NS_CC_BEGIN

class CC_DLL Array : public Object, public Clonable
{
	public :

		/** 
		 *	Creates an empty array. Default capacity is 10 
		 *	@js NA
		 *	@lua NA
		 */
		static Array*		create ( KDvoid );
		
		/** 
		 *	Create an array with objects 
		 *	@js NA
		 */
		static Array*		create ( Object* pObject, ... ) CC_REQUIRES_NULL_TERMINATION;
		
		/**
		 *	Create an array with one object 
		 *	@js NA
		 */
		static Array*		createWithObject ( Object* pObject );

		/**
		 *	Create an array with a default capacity 
		 *	@js NA
		 */
		static Array*		createWithCapacity ( KDint32 nCapacity );

		/**
		 *	Create an array with from an existing array 
		 *	@js NA
		 */
		static Array*		createWithArray ( Array* pOtherArray );

		/**
		 *	@brief   Generate a Array pointer by file
		 *	@param   pFileName  The file name of *.plist file
		 *	@return  The Array pointer generated from the file
		 *	@js NA
		 */
		static Array*		createWithContentsOfFile ( const KDchar* pFileName );
    
		/**
		 *	@brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
		 *	invoker should call release().
		 *	@js NA
		 *	@lua NA
		 */
		static Array*		createWithContentsOfFileThreadSafe ( const KDchar* pFileName );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		~Array ( KDvoid );

		/** 
		 *	Initializes an array 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				init ( KDvoid );

		/**
		 *	Initializes an array with one object 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				initWithObject ( Object* pObject );

		/** 
		 *	Initializes an array with some objects 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				initWithObjects ( Object* pObject, ... ) CC_REQUIRES_NULL_TERMINATION;
		
		/** 
		 *	Initializes an array with capacity 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				initWithCapacity ( KDint32 nCapacity );

		/**
		 *	Initializes an array with an existing array 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				initWithArray ( Array* pOtherArray );

		// Querying an Array

		/**
		 *	Returns element count of the array 
		 *	@js NA
		 */
		KDint32				count ( KDvoid ) const
		{
	#if CC_USE_ARRAY_VECTOR
			return data.size ( );
	#else
			return data->num;
	#endif
		}

		/** 
		 *	Returns capacity of the array 
		 *	@js NA
		 */
		KDint32				capacity ( KDvoid ) const
		{
	#if CC_USE_ARRAY_VECTOR
			return data.capacity ( );
	#else
			return data->max;
	#endif
		}

		/** 
		 *	Returns index of a certain object, return UINT_MAX if doesn't contain the object 
		 *	@js NA
		 *	@lua NA
		 */
		KDint32				getIndexOfObject ( Object* pObject ) const;

		/** 
		 *	Returns an element with a certain index 
		 *	@js NA
		 *	@lua NA
		 */
		Object*				getObjectAtIndex ( KDint32 nIndex )
		{
			CCASSERT ( nIndex >= 0 && nIndex < count ( ), "index out of range in getObjectAtIndex()" );
	#if CC_USE_ARRAY_VECTOR
			return data [ nIndex ].get ( );
	#else
			return data->arr [ nIndex ];
	#endif
		}

		/**
		 *	Returns the last element of the array 
		 *	@js NA
		 */
		Object*				getLastObject ( KDvoid )
		{
	#if CC_USE_ARRAY_VECTOR
			return data.back ( ).get ( );
	#else
			if ( data->num > 0 )
			{
				return data->arr [ data->num - 1 ];
			}
        
			return nullptr;
	#endif
		}

		/**
		 *	Returns a random element 
		 *	@js NA
		 *	@lua NA
		 */
		Object*				getRandomObject ( KDvoid );

		/** 
		 *	Returns a Boolean value that indicates whether object is present in array. 
		 *	@js NA
		 */
		KDbool				containsObject ( Object* pObject ) const;

		/**
		 *	@since 1.1 
		 *	@js NA
		 */
		KDbool				isEqualToArray ( Array* pOtherArray );

		// Adding Objects

		/**
		 *	Add a certain object 
		 *	@js NA
		 */
		KDvoid				addObject ( Object* pObject );

		/**
		 *	@js NA
		 */

		/** 
		 *	Add all elements of an existing array 
		 *	@js NA
		 */
		KDvoid				addObjectsFromArray ( Array* pOtherArray );

		/**
		 *	Insert a certain object at a certain index 
		 *	@js NA
		 */
		KDvoid				insertObject ( Object* object, KDint32 nIndex );

		/**
		 *	sets a certain object at a certain index 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				setObject ( Object* pObject, KDint32 nIndex );

		/** 
		 *	sets a certain object at a certain index without retaining. Use it with caution 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				fastSetObject ( Object* pObject, KDint32 nIndex )
		{
	#if CC_USE_ARRAY_VECTOR
			setObject ( pObject, nIndex );
	#else
			// no retain
			data->arr [ nIndex ] = pObject;
	#endif
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				swap ( KDint32 nIndexOne, KDint32 nIndexTwo )
		{
			CCASSERT ( nIndexOne >=0 && nIndexOne < count ( ) && nIndexTwo >= 0 && nIndexTwo < count ( ), "Invalid indices" );
	#if CC_USE_ARRAY_VECTOR
			std::swap ( data [ nIndexOne ], data[nIndexTwo] );
	#else
			std::swap ( data->arr [ nIndexOne ], data->arr [ nIndexTwo ] );
	#endif
		}

		// Removing Objects

		/**
		 *	Remove last object 
		 *	@js NA
		 */
		KDvoid				removeLastObject ( KDbool bReleaseObj = true );

		/**
		 *	Remove a certain object 
		 *	@js NA
		 */
		KDvoid				removeObject ( Object* pObject, KDbool bReleaseObj = true );

		/**
		 *	Remove an element with a certain index 
		 *	@js NA
		 */
		KDvoid				removeObjectAtIndex ( KDint32 nIndex, KDbool bReleaseObj = true );

		/**
		 *	Remove all elements 
		 *	@js NA
		 */
		KDvoid				removeObjectsInArray ( Array* pOtherArray );

		/**
		 *	Remove all objects 
		 *	@js NA
		 */
		KDvoid				removeAllObjects();

		/**
		 *	Fast way to remove a certain object 
		 *	@js NA
		 */
		KDvoid				fastRemoveObject ( Object* pObject );

		/**
		 *	Fast way to remove an element with a certain index 
		 *	@js NA
		 */
		KDvoid				fastRemoveObjectAtIndex ( KDint32 nIndex );

		// Rearranging Content

		/** 
		 *	Swap two elements 
		 *	@js NA
		 */
		KDvoid				exchangeObject ( Object* pObject1, Object* pObject2 );

		/**
		 *	Swap two elements with certain indexes 
		 *	@js NA
		 */
		KDvoid				exchangeObjectAtIndex ( KDint32 nIndex1, KDint32 nIndex2 );

		/** 
		 *	Replace object at index with another object. 
		 *	@js NA
		 */
		KDvoid				replaceObjectAtIndex ( KDint32 index, Object* object, KDbool releaseObject = true );

		/** 
		 *	Revers the array 
		 *	@js NA
		 */
		KDvoid				reverseObjects ( KDvoid );

		/** 
		 *	Shrinks the array so the memory footprint corresponds with the number of items 
		 *	@js NA
		 */
		KDvoid				reduceMemoryFootprint ( KDvoid );
  
		/** 
		 *	override functions 
		 *	@js NA
		 */
		virtual KDvoid		acceptVisitor ( DataVisitor &tVisitor );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual Array*		clone ( KDvoid ) const;

		// ------------------------------------------
		// Iterators
		// ------------------------------------------
	#if CC_USE_ARRAY_VECTOR

		typedef std::vector<RCPtr<Object>>::iterator		iterator;
		typedef std::vector<RCPtr<Object>>::const_iterator	const_iterator;
		/**
		 *	@js NA
		 *	@lua NA
		 */
		iterator			begin ( KDvoid ) { return data.begin ( ); }
		/**
		 *	@js NA
		 *	@lua NA
		 */
		iterator			end ( KDvoid ) { return data.end ( ); }

		const_iterator		cbegin ( KDvoid ) { return data.cbegin ( ); }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		const_iterator		cend ( KDvoid ) { return data.cend ( KDvoid ); }

		std::vector<RCPtr<Object>> data;

	#else
		/**
		 *	@js NA
		 *	@lua NA
		 */
		Object**			begin ( KDvoid ) { return &data->arr [ 0 ]; }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Object**			end ( KDvoid ) { return &data->arr [ data->num ]; }

		ccArray*			data;

	#endif

//	protected :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		Array ( KDvoid );
};

// end of data_structure group
/// @}

NS_CC_END

#endif	// __CCArray_h__
