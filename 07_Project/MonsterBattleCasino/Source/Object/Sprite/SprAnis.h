/* -----------------------------------------------------------------------------------
 *
 *      File            SprAnis.h
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

#ifndef __SprAnis_h__
#define __SprAnis_h__

#include "SprAni.h"

class cSprAnis : public Object
{
	public :

				 cSprAnis ( Node* pNode, std::string sFileName, KDint z, Point tPoint );		// *.spr사용
		virtual ~cSprAnis ( KDvoid );

		KDvoid						changeNode ( Node* pNode, KDint z );

		KDvoid						texLoad    ( KDint nnAniIndex );
		KDvoid						texLoad    ( KDint nnAniIndex, KDint z );
		KDvoid						texRelease ( KDint nnAniIndex );

		KDbool						update ( KDdouble dLeftTickCount );

		KDvoid						start ( KDvoid );
		KDvoid						stop  ( KDvoid );

		KDvoid						changeAni ( KDint nSelectionIndex );

		KDvoid						setVisible ( KDbool bIsVisible );
		KDvoid						setAlpha ( KDubyte cAlpha );
		KDvoid						setAlpha ( KDint nnAniIndex, KDubyte cAlpha );
		KDvoid						setColor ( KDubyte r, KDubyte g, KDubyte b );
		KDvoid						setColor ( KDint nnAniIndex, KDubyte r, KDubyte g, KDubyte b);
		KDvoid						setPoint ( const Point& tPoint );
		KDvoid						setPointTime ( const Point& tPoint, KDdouble leftTime, KDbool bIsForciblyChange = true );
		KDvoid						setPointAutoSwitch ( KDint nIndex, KDbool bIsAuto );
		KDvoid						setLoop ( KDint nnAniIndex, KDbool bIsLoop );
		KDvoid						setScale ( KDfloat fScaleX, KDfloat fScaleY );		// 1.0f이 기본 사이즈.
		KDvoid						setSelectAniNowFrame ( KDint nFrame );

		inline std::string			getFileName ( KDvoid ) { return m_sFileName; }
		inline KDint				getSelectnAniIndex ( KDvoid ) { return m_nAniSelectionIndex; }
		KDvoid						getAniFileNames ( KDint nIndex, std::string& sAniFileName );
		KDint						getSelectAniNowFrame ( KDvoid );
		Point						getPoint ( KDvoid );
		cSprAni*					getAni ( KDint nIndex );							// 가능하면 쓰지말자.
		KDint						getFrameSize ( KDint nnAniIndex );
		KDbool						getVisible ( KDvoid );
		KDubyte 					getAlpha ( KDvoid );

	private :

		Node*						m_pNode;

		std::string					m_sFileName;

		std::vector<std::string>	m_sAniFileNames;
		std::vector<cSprAni*>		m_aAnis;
		std::vector<KDbool>			m_aAnisAutoPointSet;

		KDint						m_nAniSelectionIndex;
		Point						m_tPoint;
};

#endif	// __SprAnis_h__
