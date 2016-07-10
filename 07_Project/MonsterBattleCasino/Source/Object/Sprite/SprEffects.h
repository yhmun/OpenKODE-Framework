/* -----------------------------------------------------------------------------------
 *
 *      File            SprEffects.h
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

#ifndef __SprEffects_h__
#define __SprEffects_h__

#include "SprAni.h"

typedef struct _sReservationEffect2
{
	KDdouble			dLeftTime;
	Point				tPoint;
	std::string			sSoundEffect;
} sReservationEffect2;


// effect------------------------------------------------------------------
class cFrame;

class cSprEffect : public Object
{
	public :
				 cSprEffect ( Node* pNode, const KDchar* pTexPath, std::string sName, KDint z, KDint nAniMax );
		virtual ~cSprEffect ( KDvoid );

		KDvoid					addFrame ( Rect tRectSrc, KDfloat fDelay, Point tCenter );
		KDvoid					view ( Point tPoint );
		KDvoid					viewReservation ( Point tPoint, KDdouble dLeftTime, std::string sSoundEffect = "" );

		inline std::string		getName ( KDvoid )	{ return m_sName; }
		KDbool					isView ( KDvoid );

		KDvoid					update ( KDdouble dLeftTickCount );

		KDvoid					setScale ( KDfloat fScaleX, KDfloat fScaleY );
		KDvoid					setAnchorPoint ( Point tPoint );
		KDvoid					setAlpha ( KDubyte cAlpha );
		KDvoid					setColor ( KDint r, KDint g, KDint b );

	private :

		Node*								m_pNode;
		std::string							m_sName;
		std::vector<cSprAni*>				m_aAnis;
		std::vector<sReservationEffect2>	m_aReservations;
};


// effect------------------------------------------------------------------

class cSprEffectFinish : public Object
{
	public :
				 cSprEffectFinish ( Node* pNode, const KDchar* pTexPath, KDint z );
		virtual ~cSprEffectFinish ( KDvoid );

		KDvoid					addFrame ( Rect tRectSrc, KDfloat fDelay, Point tCenter );
		KDbool					view ( Point tPoint, KDdouble dWaitTime );

		KDbool					isView ( KDvoid );

		KDvoid					update ( KDdouble dLeftTickCount );

	private :

		Node*								m_pNode;
		cSprAni*							m_pAni;

		KDint								m_nStep;
		KDdouble							m_dLeftTime;
		Point								m_tPoint;
};

// effects-----------------------------------------------------------------

class cSprEffects : public Object
{
	public :
				 cSprEffects ( Node* pNode, KDint nZ1, KDint nZ2 );
		virtual ~cSprEffects ( KDvoid );

		KDvoid					update ( KDdouble dLeftTickCount );

		KDvoid					addEffect ( std::string sName, Point tPoint );

		KDbool					isEffect ( KDvoid );

		KDvoid					setColor ( KDint r, KDint g, KDint b );

	private :

		KDvoid					addEffectNail	 ( Point tPoint );
		KDvoid					addEffectCut	 ( Point tPoint );
		KDvoid					addEffectBite	 ( Point tPoint );
		KDvoid					addEffectHit	 ( Point tPoint );
		KDvoid					addEffectNailAll ( Point tPoint );
		KDvoid					addEffectCutAll	 ( Point tPoint );
		KDvoid					addEffectBiteAll ( Point tPoint );
		KDvoid					addEffectHitAll	 ( Point tPoint );
		KDvoid					addEffectPoison	 ( Point tPoint );
		KDvoid					addEffectRegen	 ( Point tPoint );
		KDvoid					addEffectFinish	 ( Point tPoint, KDfloat fWaitTime );

		Node*								m_pNode;
		std::vector<cSprEffect*>			m_aEffects;
		std::vector<cSprEffectFinish*>		m_aEffectFinishs;
};

#endif	// __SprEffects_h__
