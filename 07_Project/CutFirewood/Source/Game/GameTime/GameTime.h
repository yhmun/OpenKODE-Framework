/* -----------------------------------------------------------------------------------
 *
 *      File            GameTime.h
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

#ifndef __GameTime_h__
#define __GameTime_h__

#include "Scene/JBase/JBaseScene.h"

class cSprite;
class cLabelAtlasCostom;

class cGameTime : public Object
{
	public :

		// init
		cGameTime ( JBaseScene* scene, KDdouble dTime );
		virtual ~cGameTime ( KDvoid );

		// set
		KDvoid				setIceVersion ( KDbool bIsIce );
		KDvoid				setTime	( KDdouble dTime );
		KDvoid				setAddTime ( KDdouble dTime );
		inline KDvoid		setIsTimeStop ( KDbool bIsStop ) { m_bIsTimeStop = bIsStop; }
	
		//get
		inline KDdouble		getTime ( KDvoid ) { return m_dTime; }
	
		// update
		KDvoid				update		  ( KDdouble dLeftTime );
		KDvoid				updateAddTime ( KDdouble dLeftTime );

	private :

		// scene
		JBaseScene*							m_pScene;
	
		// sprite
		cSprite*							m_pSprBack;
		cSprite*							m_pSprBackGray;
		cSprite*							m_pSprIce;
		cSprite*							m_pSprNumAdd;

		// label
		cLabelAtlasCostom*					m_pLabel;
		std::vector<cLabelAtlasCostom*>		m_aLabelsAdd;
	
		// score
		KDdouble							m_dTime;
		KDint								m_nLookTime;
	
		// palpitate
		KDbool								m_bIsPalpitate;
		KDdouble							m_dPalpitateLeftTime;
	
		// time stop
		KDbool								m_bIsTimeStop;
};

#endif	// __GameTime_h__