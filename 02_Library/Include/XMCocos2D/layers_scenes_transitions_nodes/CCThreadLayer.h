/* -----------------------------------------------------------------------------------
 *
 *      File            CCThreadLayer.h
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

#ifndef __CCThreadLayer_h__
#define __CCThreadLayer_h__

#include "CCLayer.h"

NS_CC_BEGIN
    
enum  
{
	kCCThreadLayerDone = 1,
	kCCThreadLayerUser    ,
};

class CCThreadLayer : public CCLayer
{
	protected :	
				 CCThreadLayer ( KDvoid );
		virtual ~CCThreadLayer ( KDvoid );
	
	public :

		virtual KDvoid		visit ( KDvoid );
	
		virtual KDvoid		execute ( KDvoid );		
		virtual KDvoid		receive ( KDint nType, KDvoid* pData );
	
		KDvoid				post ( KDint nType, KDvoid* pData = KD_NULL );
	
		KDvoid				resume ( KDvoid );

		KDvoid				setIsLock ( KDbool bOn );
	
	protected :
	
		static KDvoid*		thread ( KDvoid* pArg );	
	
		KDThread*           m_pMain;
		KDThread*           m_pThread;
		KDThreadSem*        m_pLock;		
};

NS_CC_END

#endif // __CCThreadLayer_h__
