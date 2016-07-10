/* -----------------------------------------------------------------------------------
 *
 *      File            JBaseScene.h
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

#ifndef __JBaseScene_h__
#define __JBaseScene_h__

class cCamera;
class cDraw;
class cButton;

class JBaseScene : public Scene
{
	public :
				 JBaseScene ( KDvoid );
		virtual ~JBaseScene ( KDvoid );
		
	public :

		virtual KDbool			init ( KDvoid );

		// draw
		virtual KDvoid			draw ( KDvoid );

		// update
		virtual KDvoid			update ( KDfloat fDelta );
			
		virtual KDvoid			onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid			onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid			onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

		KDvoid					setIsTouch ( KDbool bIsTouch );

		// time
		KDdouble				getLeftTime ( KDvoid );

		// camera
		cCamera*				getCamera_ ( KDvoid );

		// draw
		KDvoid					addDrawObject ( cDraw* pDraw );

		// button
		KDvoid					addBtnObject ( cButton* pBtn );

		// set z order
		KDvoid					setDrawObjZorder ( cDraw* pDraw, KDint z );

		// draw & button
//		KDvoid					clear_draws_A_btns;

	private :

		// time
		KDdouble				m_dTickCount;
		KDdouble				m_dBeforeTickCount;
		KDdouble				m_dLeftTickCount;

		// camera
		cCamera*				m_pCamera;

		// button objects
		std::vector<cButton*>	m_aButtons;

		// draw objects
		std::vector<cDraw*>		m_aDraws;
};

#endif	// __JBaseScene_h__
