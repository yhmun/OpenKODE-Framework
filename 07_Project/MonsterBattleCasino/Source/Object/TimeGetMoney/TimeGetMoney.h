/* -----------------------------------------------------------------------------------
 *
 *      File            TimeGetMoney.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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

#ifndef __TimeGetMoney_h__
#define __TimeGetMoney_h__

#include "Object/Label/Labels.h"

class cTimeGetMoney : public Object
{
	public :
				 cTimeGetMoney ( KDvoid );
		virtual ~cTimeGetMoney ( KDvoid );

		static cTimeGetMoney*	getObj ( KDvoid );

		KDvoid			clear ( KDvoid );

		KDvoid			changeNode ( Node* pNode, KDint z, Rect tRectDest );

		KDbool			update ( KDdouble dLeftTime, KDint64& rMoney );

		KDvoid			setPoint ( Point tPoint );

	private :

		cLabels*		m_pLabels;
		LabelTTF*		m_pLabel;
		KDdouble		m_dLeftTime;
};

#endif	// __TimeGetMoney_h__
