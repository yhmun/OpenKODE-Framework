/* -----------------------------------------------------------------------------------
 *
 *      File            Labels.h
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

#ifndef __Labels_h__
#define __Labels_h__

typedef struct _sMove
{
	LabelTTF*					pLabel;
	Point						tMoveBefore;
	Point						tMove;
	KDdouble					dTimeMoveFull;
	KDdouble					dTimeMoveLeft;
} sMove;

class cLabels : public Object
{
	public :
				 cLabels ( Node* pNode );
		virtual ~cLabels ( KDvoid );

		KDvoid					clear ( KDvoid );
		KDvoid					changeNode ( Node* pNode );

		LabelTTF*				add ( const KDchar* pFontName, KDfloat fFontSize, TextHAlignment eAlignment, Rect tRect, const KDchar* pStr, KDint z );	
		KDvoid					del ( LabelTTF* pLabel );

		KDvoid					setVisible		( LabelTTF* pLabel, KDbool bIsVisible );
		KDvoid					setPoint		( LabelTTF* pLabel, Point tPoint );
		KDvoid					setAnchorPoint	( LabelTTF* pLabel, Point tPoint );
		KDvoid					setStr			( LabelTTF* pLabel, const KDchar* pStr );
		KDvoid					setAllColor		( KDubyte a, KDubyte r, KDubyte g, KDubyte b );
		KDvoid					setColor		( LabelTTF* pLabel, KDubyte r, KDubyte g, KDubyte b );
		KDvoid					setAlpha		( LabelTTF* pLabel, KDubyte cAlpha );
		KDvoid					setPointTime	( LabelTTF* pLabel, Point tPoint, KDdouble dLeftTime );

		Point					getPoint		( LabelTTF* pLabel );
		Size					getSize			( LabelTTF* pLabel );

		KDvoid					update ( KDdouble dLeftTime );

	private :

		Node*					m_pNode;

		std::vector<LabelTTF*>	m_aLabels;
		std::vector<sMove>		m_aMoves;
};

#endif	// __Labels_h__
