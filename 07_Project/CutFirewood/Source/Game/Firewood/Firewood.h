/* -----------------------------------------------------------------------------------
 *
 *      File            Firewood.h
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

#ifndef __Firewood_h__
#define __Firewood_h__

#include "Scene/JBase/JBaseScene.h"

class cSprite;
class cCommander;

//------------------------------------------cFirewoodStandBy-------------------------------------------//
class cFirewoodPreview : public Object
{
	public :

		// init
		cFirewoodPreview ( JBaseScene* pScene, Point pStart, Point tEnd );
		virtual ~cFirewoodPreview ( KDvoid );
	
		// throw
		KDvoid					throwing ( KDint nLevel );
	
		// standby
		KDvoid					standby ( KDint nLevel );
	
		// update
		KDvoid					update ( KDdouble dLeftTime, KDbool& bIsEnd );
	
		// set
		KDvoid					setOff ( KDvoid );
	
		// get
		inline KDint			getLevel ( KDvoid )	{ return m_nLevel; }
	
	private :

		// sprite
		std::vector<cSprite*>	m_aSprFulls;
	
		// level
		KDint					m_nLevel;
	
		//point
		Point					m_tStart;
		Point					m_tEnd;
	
		// throw
		KDbool					m_bIsThrowing;
		KDdouble				m_dThrowingTime;
		const KDdouble			m_dThrowTime;
};


//------------------------------------------cFirewoodStandBy-------------------------------------------//
class cFirewoodStandBy : public Object
{
	public :

		// init
		cFirewoodStandBy ( JBaseScene* pScene, Point tStart, Point tEnd, KDfloat fThrowHeight );
		virtual ~cFirewoodStandBy ( KDvoid );

		// throw
		KDvoid					throwing ( KDint nLevel );
	
		// set
		KDvoid					setOff ( KDvoid );
	
		// damage
		KDvoid					damage ( KDvoid );
		inline KDint			getDamage ( KDvoid ) { return m_nDamage; }
	
		// update
		KDvoid					update ( KDdouble dLeftTime );
	
		// get
		inline KDint			getLevel ( KDvoid ) { return m_nLevel; }
		inline KDbool			getIsStandBy ( KDvoid ) { return m_bIsStandBy; }
		inline KDbool			getIsTorhwing ( KDvoid ) { return m_bIsThrowing; }
	
	private :

		// sprite
		std::vector<cSprite*>	m_aSprFulls;
		std::vector<cSprite*>	m_aSprLefts;
		std::vector<cSprite*>	m_aSprRights;
	
		// level
		KDint					m_nLevel;
	
		// point
		Point					m_tStart;
		Point					m_tEnd;
	
		// height
		KDfloat					m_fThrowHeight;

		// standby
		KDbool					m_bIsStandBy;
	
		// throw
		KDbool					m_bIsThrowing;
		KDdouble				m_dThrowingTime;
		const KDdouble			m_dThrowTime;
	
		// damage
		KDint					m_nDamage;
};

//------------------------------------------cFirewoodProduct-------------------------------------------//
class cFirewoodProduct : public Object
{
	public :

		// init
		cFirewoodProduct ( JBaseScene* pScene, KDint nLevel, Point tStart, Point tEnd );
		virtual ~cFirewoodProduct ( KDvoid );

		// update
		KDvoid					update ( KDdouble dLeftTime, KDbool& bIsEnd );
	
		// level
		inline KDint			getLevel ( KDvoid ) { return m_nLevel; }
	
		// get
		inline Point			getPointLeftEnd  ( KDvoid )	{ return m_tLeftEnd; }
		inline Point			getPointRightEnd ( KDvoid )	{ return m_tRightEnd; }
	
	private :

		// scene
		JBaseScene*				m_pScene;

		// sprite
		cSprite*				m_pSprLeft;
		cSprite*				m_pSprRight;
	
		// level
		KDint					m_nLevel;
	
		// point
		Point					m_tLeftStart;
		Point					m_tLeftEnd;
		Point					m_tRightStart;
		Point					m_tRightEnd;
	
		// height
		KDfloat					m_fHeightLeft;
		KDfloat					m_fHeightRight;
	
		// throw
		KDdouble				m_dThrowingTime;
		const KDdouble			m_dThrowTime;
	
		// rotation
		KDfloat					m_fRotationLeft;
		KDfloat					m_fRotationRight;
		KDfloat					m_fRotationLeftSpeed;
		KDfloat					m_fRotationRightSpeed;
	
		// layer
		KDbool					m_bIsChangeZorder;
};

//---------------------------------------------cFirewoodMgr--------------------------------------------//

class cFirewoodMgr : public Object
{
	public :
	
		// init
		cFirewoodMgr ( JBaseScene* pScene, cCommander* pCommander, KDint nPlayerID, Point tIntro, Point tPreview, Point tStandBy, Point tProduct );
		virtual ~cFirewoodMgr ( KDvoid );

		// first setting
		KDvoid					firstSetting ( KDbool bIsNewPreview );
	
		// request
		KDvoid					request ( KDvoid );
	
		// splitting
		KDvoid					splitting ( KDint nPowerLv, KDbool& bIsSucces, KDint& nFirewoodLv );
	
		// get
		KDbool					getIsStandByThrowing ( KDvoid );
		KDbool					getIsStandBy ( KDvoid );
		KDint					getStandbyLevel ( KDvoid );

		// draw
		KDvoid					update ( KDdouble dLeftTime );
	
	private :

		// scene
		JBaseScene*							m_pScene;

		// commander
		cCommander*							m_pCommander;

		// id
		KDint								m_nPlayerID;
	
		// firewood
		cFirewoodPreview*					m_pFirewoodPreview;
		cFirewoodStandBy*					m_pFirewoodStandby;
		std::vector<cFirewoodProduct*>		m_aFirewoodProducts;
	
		// point
		Point								m_tIntro;
		Point								m_tPreview;
		Point								m_tStandBy;
		Point								m_tProduct;
};

#endif	// __Firewood_h__