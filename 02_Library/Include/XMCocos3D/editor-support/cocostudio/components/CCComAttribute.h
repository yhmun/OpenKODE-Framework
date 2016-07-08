/* -----------------------------------------------------------------------------------
 *
 *      File            CCComAttribute.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __CCComAttribute_h__
#define __CCComAttribute_h__

#include <string>
#include "../json/CSContentJsonDictionary.h"
#include "../../../support/component/CCComponent.h"

namespace cocostudio {

class ComAttribute : public cocos2d::Component
{
	protected :

		/**
		 *	@js ctor
		 */
		ComAttribute ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ComAttribute ( KDvoid );
    
	public :

	   virtual KDbool			init ( KDvoid );

	   static ComAttribute*		create ( KDvoid );
   
	   KDvoid					setInt		( const KDchar* pKey, KDint			nValue );
	   KDvoid					setFloat	( const KDchar* pKey, KDfloat		fValue );
	   KDvoid					setBool		( const KDchar* pKey, KDbool		bValue );
	   KDvoid					setCString	( const KDchar* pKey, const KDchar* pValue );
   
	   KDint					getInt		( const KDchar* pKey ) const;
	   KDfloat					getFloat	( const KDchar* pKey ) const;
	   KDbool					getBool		( const KDchar* pKey ) const;
	   const KDchar*			getCString	( const KDchar* pKey ) const;
   
	   JsonDictionary*			getDict ( KDvoid ) const;
   
	private :

	   JsonDictionary*			m_pJsonDict;   
};

}

#endif  // __CCComAttribute_h__
