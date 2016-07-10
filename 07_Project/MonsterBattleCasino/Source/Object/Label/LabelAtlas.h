/* -----------------------------------------------------------------------------------
 *
 *      File            LabelAtlas.h
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

#ifndef __LabelAtlas_h__
#define __LabelAtlas_h__

class cLabelAtlas : public Object
{
	public :
				 cLabelAtlas ( Node* pNode, const KDchar* pStr, const KDchar* pCharMapFile, KDint nWidth, KDint nHeight, KDchar cStartChar, KDint z, Point tPoint );
		virtual ~cLabelAtlas ( KDvoid );

		KDvoid					setVisible ( KDbool bIsVisible );
		KDvoid					setStr ( const KDchar* pStr );
		KDvoid					setPoint ( Point tPoint );
		KDvoid					setPointTime ( Point tPoint, KDdouble dLeftTime );

		KDvoid					update ( KDdouble dLeftTime );

		inline Size				getSize ( KDvoid ) { return m_pLabel->getContentSize ( ); }

	private :

		Node*					m_pNode;
		LabelAtlas*				m_pLabel;

		KDbool					m_isMove;
		Point					m_ptMoveBefore;
		Point					m_ptMove;
		KDdouble				m_timeMoveFull;
		KDdouble				m_timeMoveLeft;
};

#endif	// __LabelAtlas_h__
