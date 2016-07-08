/* -----------------------------------------------------------------------------------
 *
 *      File            CCSet.h
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


#ifndef __CCSet_h__
#define __CCSet_h__

#include "CCObject.h"

#include <set>

NS_CC_BEGIN
    
class CCArray;

/**
 * @addtogroup data_structures
 * @{
 */

typedef std::set<CCObject*>::iterator  CCSetIterator;

class CCSet : public CCObject
{
	public :
		         CCSet ( KDvoid );
		         CCSet ( const CCSet& tSetObject );		
		virtual ~CCSet ( KDvoid );

		static   CCSet*		create ( KDvoid );

	public :
		
		/**
		 *	@brief	Return a copy of the CCSet, it will copy all the elements.
		 */
		CCSet*				copy ( KDvoid );

		/**
		 *	@brief	It is the same as copy().
		 */
		CCSet*				mutableCopy ( KDvoid );

		/**
		 *	@brief	Return the number of elements the CCSet contains.
		 */
		KDint				count ( KDvoid );

		/**
		 *	@brief	Add a element into CCSet, it will retain the element.
		 */
		KDvoid				addObject ( CCObject* pObject );

		/**
		 *	@brief	Remove the given element, nothing todo if no element equals pObject.
		 */
		KDvoid				removeObject ( CCObject* pObject );

		/**
		 *	@brief  Remove all elements of the set
		 */
		KDvoid				removeAllObjects ( KDvoid );

		/**
		 *	@brief	Check if CCSet contains a element equals pObject.
		 */
		KDbool				containsObject ( CCObject* pObject );

		/**
		 *	@brief	Return the iterator that points to the first element.
		 */
		CCSetIterator		begin ( KDvoid );

		/**
		 *	@brief	Return the iterator that points to the position after the last element.
		 */
		CCSetIterator		end ( KDvoid );

		/**
		 *	@brief	Return the first element if it contains elements, or null if it doesn't contain any element.
		 */
		CCObject*			anyObject ( KDvoid );

		CCArray*			allObjects ( KDvoid );

		virtual KDvoid		acceptVisitor ( CCDataVisitor& tVisitor );

	private :
		
		std::set<CCObject*>*		m_pSet;
};

// end of data_structure group
/// @}

NS_CC_END

#endif // __CCSet_h__ 
