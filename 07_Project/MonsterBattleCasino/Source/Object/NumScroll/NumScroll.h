/* -----------------------------------------------------------------------------------
 *
 *      File            NumScroll.h
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

#ifndef __NumScroll_h__
#define __NumScroll_h__

#include "Object/Sprite/Sprite.h"

typedef struct _sTouchMove
{
	Point			tMove;
	KDdouble		dTimeStamp;
} sTouchMove;

class cNumScroll : public Object
{
	public :
				 cNumScroll ( Node* pNode, const KDchar* pFileName, KDint z, Point tPoint, Size tUnitImgSize, Rect tRectTouch, Rect tRectPut );
		virtual ~cNumScroll ( KDvoid );

		KDvoid					update ( KDdouble dLeftTime );

		KDbool					touchBegan ( KDint nTid, Point tPoint );
		KDvoid					touchMoved ( KDint nTid, Point tPoint );
		KDvoid					touchEnded ( KDint nTid, Point tPoint );

		KDvoid					setVisible ( KDbool bIsVisible );
		KDvoid					setNum ( KDint nNum );
		KDvoid					setMax ( KDint nNum );
		KDvoid					setEffect ( KDvoid );

		inline KDint			getNum ( KDvoid ) { return m_nNum; }

	private :

		KDvoid					setNum2 ( KDint nNum );
		KDvoid					setPoint ( Point tPoint, KDfloat fMove );

		Node*					m_pNode;

		std::vector<cSprite*>	m_aSprites;

		Point					m_tPoint;
		Size					m_tUnitImgSize;
		Rect					m_tRectTouch;
		Rect					m_tRectPut;

		KDint					m_nNum;
		KDint					m_nMaxNum;

		KDbool					m_bIsMoving;
		KDfloat					m_fMove;
		KDfloat					m_fMove2;

		KDint					m_nTouchID;
		Point					m_tTouch;
		KDdouble				m_dTouchTime;
		sTouchMove				m_aTouchMoves [ 5 ];
		KDint					m_nTouchMovesCount;
		KDint					m_tTouchMovesSize;
};

#endif	// __NumScroll_h__
