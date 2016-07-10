/* -----------------------------------------------------------------------------------
 *
 *      File            House.h
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

#ifndef __House_h__
#define __House_h__

#include "Scene/JBase/JBaseGameScene.h"

class cSprite;
class cAnimations;
class cCommander;
class cLabelAtlasCostom;

class cHouse : public Object
{
	public :

		// init
		cHouse ( JBaseGameScene* pScene, KDint nStage, KDint nMaxLevel, Point tPoint );
		virtual ~cHouse ( KDvoid );

		// view
		KDvoid				view ( KDint nLevel, KDbool isEffect);

		// set
		KDvoid				setGray ( KDbool bIsGray );
	
		// get
		KDint				getMaxLevel ( KDvoid );
		inline KDint		getLevel ( KDvoid )	{ return m_nLevel; }

		// throbbing effect
		inline KDbool		getThrobbingEffect ( KDvoid ) { return m_bIsThrobbingEffect; }
	
		// update
		KDvoid				update ( KDdouble dLeftTime );
	
	private :

		// feet tall
		KDvoid				setAddFeetTall ( KDvoid );
	
		// scene
		JBaseGameScene*				m_pScene;
	
		//sprite
		std::vector<cSprite*>		m_aSprHouses;
		std::vector<cSprite*>		m_aSprHousesGray;
		std::vector<cAnimations*>	m_aAniMoles;
		cSprite*					m_pSprMoleBackup;

		// label
		cLabelAtlasCostom*			m_pLabelFeetTall;
	
		// stage
		KDint						m_nStage;
	
		// level
		KDint						m_nLevel;
	
		// gray
		KDbool						m_bIsGray;
	
		// throbbing effect
		KDbool						m_bIsThrobbingEffect;
		KDint						m_nIsThrobbingScaleStep;
		KDfloat						m_fThrobbingScale;
		KDdouble					m_dThrobbingLeftTime;
	
		// feet tall
		KDint						m_nFeetTall;
};

#endif	// __House_h__