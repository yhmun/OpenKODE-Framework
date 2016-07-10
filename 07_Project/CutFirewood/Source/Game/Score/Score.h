/* -----------------------------------------------------------------------------------
 *
 *      File            Score.h
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

#ifndef __Score_h__
#define __Score_h__

#include "Scene/JBase/JBaseScene.h"

class cSprite;
class cLabelAtlasCostom;

class cScore : public Object
{	
	public :

		// init
		cScore ( JBaseScene* pScene );
		virtual ~cScore ( KDvoid );

		// set
		inline KDvoid			setScore ( KDint nScore	) { m_nScore  = nScore; }
		inline KDvoid			addScore ( KDint nScore	) { m_nScore += nScore; }
		inline KDvoid			isLookScoreStay	( KDbool bIsStay ) { m_bIsLookScoreStay = bIsStay; }

		// get
		inline KDint			getScore ( KDvoid )	{ return m_nScore; }
		KDint					getLookScore ( KDvoid );
	
		// update
		KDvoid					update ( KDdouble dLeftTime );

	private :

		// sprite
		cSprite*				m_pBack;
	
		//label
		cLabelAtlasCostom*		m_pLabel;
	
		// score
		KDint					m_nScore;
	
		// switch
		KDbool					m_bIsLookScoreStay;
};

#endif	// __Score_h__