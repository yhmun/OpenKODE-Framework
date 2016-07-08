/* -----------------------------------------------------------------------------------
 *
 *      File            CCFloat.h
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

#ifndef __CCFloat_h__
#define __CCFloat_h__

#include "CCObject.h"

NS_CC_BEGIN

/**
 *	@addtogroup data_structures
 *	@{
 */

class CC_DLL Float : public Object, public Clonable
{
	public :

		Float ( KDfloat v )
		{
			m_fValue = v;
		}

		static Float*		create ( KDfloat v )
		{
			Float*		pRet = new Float ( v );
			if ( pRet )
			{
				pRet->autorelease ( );
			}
			return pRet;
		}

		inline KDfloat		getValue ( KDvoid ) const
		{
			return m_fValue; 
		}

		inline KDvoid		setValue ( KDfloat fValue )
		{
			m_fValue = fValue; 
		}

		/* override functions */
		virtual KDvoid		acceptVisitor ( DataVisitor& tVisitor ) 
		{
			tVisitor.visit ( this );
		}
    
		Float*				clone ( KDvoid ) const
		{
			return Float::create ( m_fValue );
		}

	private :

		KDfloat				m_fValue;
};

//	end of data_structure group
/// @}

NS_CC_END

#endif	// __CCFloat_h__

