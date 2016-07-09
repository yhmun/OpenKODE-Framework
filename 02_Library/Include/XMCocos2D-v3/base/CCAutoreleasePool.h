/* -----------------------------------------------------------------------------------
 *
 *      File            CCAutoReleasePool.h
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

#ifndef __CCAutoReleasePool_h__
#define __CCAutoReleasePool_h__

#include "CCObject.h"
#include "CCArray.h"

NS_CC_BEGIN

/**
 *	@addtogroup base_nodes
 *	@{
 */

class CC_DLL AutoreleasePool : public Object
{
		/**
		 *	The underlying array of object managed by the pool.
		 *
		 *	Although Array retains the object once when an object is added, proper
		 *	Object::release() is called outside the array to make sure that the pool
		 *	does not affect the managed object's reference count. So an object can
		 *	be destructed properly by calling Object::release() even if the object
		 *	is in the pool.
		 */
		Array*			m_pManagedObjectArray;

	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		AutoreleasePool ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~AutoreleasePool ( KDvoid );

		/**
		 *	Add a given object to this pool.
		 *
		 *	The same object may be added several times to the same pool; When the
		 *	pool is destructed, the object's Object::release() method will be called
		 *	for each time it was added.
		 *
		 *	@param object    The object to add to the pool.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		addObject ( Object* pObject );

		/**
		 *	Remove a given object from this pool.
		 *
		 *	@param object    The object to be removed from the pool.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		removeObject ( Object* pObject );

		/**
		 *	Clear the autorelease pool.
		 *
		 *	Object::release() will be called for each time the managed object is
		 *	added to the pool.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		clear ( KDvoid );
	};

	class CC_DLL PoolManager
	{
		Array*					m_pReleasePoolStack;
		AutoreleasePool*		m_pCurReleasePool;

		AutoreleasePool*		getCurReleasePool ( KDvoid );

	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static PoolManager*		sharedPoolManager (  KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static KDvoid			purgePoolManager ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		PoolManager ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~PoolManager ( KDvoid );

		/**
		 *	Clear all the AutoreleasePool on the pool stack.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		finalize ( KDvoid );

		/**
		 *	Push a new AutoreleasePool to the pool stack.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		push ( KDvoid );

		/**
		 *	Pop one AutoreleasePool from the pool stack.
		 *
		 *	This method will ensure that there is at least one AutoreleasePool on
		 *	the stack.
		 *
		 *	The AutoreleasePool being poped is destructed.
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		pop ( KDvoid );

		/**
		 *	Remove a given object from the current autorelease pool.
		 *
		 *	@param object    The object to be removed.
		 *
		 *	@see AutoreleasePool::removeObject
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		removeObject ( Object* pObject );

		/**
		 *	Add a given object to the current autorelease pool.
		 *
		 *	@param object    The object to add.
		 *
		 *	@see AutoreleasePool::addObject
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		addObject ( Object* pObject );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		friend class AutoreleasePool;
};

// end of base_nodes group
/// @}

NS_CC_END

#endif	// __CCAutoReleasePool_h__
