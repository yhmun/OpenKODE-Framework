/* -----------------------------------------------------------------------------------
 *
 *      File            CCPointer.h
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


#ifndef __CCPointer_h__
#define __CCPointer_h__

#include "CCObject.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

class CCPointer : public CCObject
{
	public :

		CCPointer ( KDvoid* pPtr )
		{
			m_pPtr = pPtr;
		}
		
		static CCPointer*	create ( KDvoid* pPtr )
		{
			CCPointer*  pRet = new CCPointer ( pPtr );
			if ( pRet )
			{
				pRet->autorelease ( );
			}
			return pRet;
		}

	public :

		inline KDvoid*		getPointer ( KDvoid ) const
		{
			return m_pPtr; 
		}

		inline KDvoid		setPointer ( KDvoid* pPtr ) 
		{
			m_pPtr = pPtr; 
		}

	protected :

		KDvoid*				m_pPtr;
};

// end of data_structure group
/// @}

NS_CC_END

#endif // __CCPointer_h__
