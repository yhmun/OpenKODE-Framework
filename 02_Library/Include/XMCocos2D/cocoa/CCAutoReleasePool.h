/* -----------------------------------------------------------------------------------
 *
 *      File            CCAutoReleasePool.h
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

#ifndef __CCAutoReleasePool_h__
#define __CCAutoReleasePool_h__

#include "CCObject.h"
#include "CCArray.h"

NS_CC_BEGIN
    
/**
 * @addtogroup base_nodes
 * @{
 */

class CCAutoReleasePool : public CCObject
{
	public :
				 CCAutoReleasePool ( KDvoid );
		virtual ~CCAutoReleasePool ( KDvoid );
		
	public :

		KDvoid					addObject ( CCObject* pObject );

		KDvoid					removeObject ( CCObject* pObject );

		KDvoid					clear ( KDvoid );

	protected :

		CCArray*				m_pManagedObjectArray;	
};

class CCPoolManager
{
	friend class CCAutoReleasePool;

	protected :
				 CCPoolManager ( KDvoid );
		virtual ~CCPoolManager ( KDvoid );

	public :

		static CCPoolManager*	sharedPoolManager ( KDvoid );
		static KDvoid            purgePoolManager ( KDvoid );

	public :

		KDvoid					finalize ( KDvoid );

		KDvoid					push ( KDvoid );

		KDvoid					pop ( KDvoid );

		KDvoid					removeObject ( CCObject* pObject );

		KDvoid					addObject ( CCObject* pObject );

	protected :

		CCAutoReleasePool*		getCurReleasePool ( KDvoid );

	protected :

		CCArray*				m_pReleasePoolStack;	
		CCAutoReleasePool*		m_pCurReleasePool;
};

// end of base_nodes group
/// @}

NS_CC_END

#endif // __CCAutoReleasePool_h__
