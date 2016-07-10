/* -----------------------------------------------------------------------------------
 *
 *      File            Combo.h
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

#ifndef __Combo_h__
#define __Combo_h__

#include "Scene/JBase/JBaseScene.h"

class cSprite;
class cLabelAtlasCostom;

class cCombo : public Object
{
	public :

		// init
		cCombo ( JBaseScene* pScene );
		virtual ~cCombo ( KDvoid );
	
		// set
		KDvoid					setAdd ( KDvoid );
		KDvoid					setZero ( KDvoid );
	
		// get
		inline KDint			getComboCount ( KDvoid ) { return m_nComboCount; }
	
		// update
	 	KDvoid					update ( KDdouble dLeftTime );

	private :

		// scene
		JBaseScene*				m_pScene;

		//sprite
		cSprite*				m_pSprite;
	
		//label
		cLabelAtlasCostom*		m_pLabel;
	
		// combo
		KDint					m_nComboCount;
	
		// scale
		KDbool					m_bIsScale;
		KDbool					m_bIsScaleAdd;
		KDfloat					m_fScale;
};

#endif	// __Combo_h__
