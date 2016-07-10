/* -----------------------------------------------------------------------------------
 *
 *      File            Char.h
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

#ifndef __Char_h__
#define __Char_h__

#include "Scene/JBase/JBaseScene.h"

class cCommander;
class cAnimations;
class cSprite;

class cChar : public Object
{
	public :

		enum eType
		{
			eType_nurungee
		};
	
		enum eAniType
		{
			eAniType_nor	,
			eAniType_step_1	,
			eAniType_step_2	,
			eAniType_step_3	,
			eAniType_step_4	,
			eAniType_finish	,
			eAniType_miss	,
		};
	
	public :

		// init
		cChar ( JBaseScene* pScene, cCommander* pCommander, eType eType, eLookAtType eLookAt, KDint nPlayeriD, Point tPoint );
		virtual ~cChar ( KDvoid );
	
		// power
		KDvoid					setPowerGetting	( KDbool isGetting );
		KDvoid					setPowerZero ( KDvoid );
		inline KDvoid			setPowerSpeed ( KDint nSpeed ) { m_fPowerGetSpeed = nSpeed; }
		inline KDint			getPowerLv ( KDvoid ) { return m_nPowerLevel; }
		inline KDbool			getIsMaxPower ( KDvoid ) { return m_bIsMaxPower; }
		inline KDbool			getIsPowerGetting ( KDvoid ) { return m_bIsPowerGetting; }
	
		// animation
		KDvoid					setAniNor ( KDvoid );
		KDvoid					setAniFinish ( KDvoid );
		KDvoid					setAniMiss ( KDvoid );
		KDvoid					setNextAniNor ( KDvoid );
	
		// effect
		KDvoid					addPerfectEffect ( KDvoid );
		KDvoid					addMaxPowerEffect ( KDvoid );

		// angry
		KDvoid					setIsAngry ( KDbool bIsAngry );
	
		// update
		KDvoid					update ( KDdouble dLeftTime );
		KDvoid					updateChar ( KDdouble dLeftTime );
		KDvoid					updateEffect ( KDdouble dLeftTime );

	private :

		// scene
		JBaseScene*				m_pScene;
	
		// commander
		cCommander*				m_pCommander;
	
		// id
		KDint					m_nPlayerID;
	
		// animation
		cAnimations*			m_pAni;
		KDint					m_nNextAni;
	
		// max power
		cSprite*				m_pSprMaxPowerGaugeBack;
		cSprite*				m_pSprMaxPowerGauge;
		cAnimations*			m_pAniMaxPower;

		// effect
		cSprite*				m_pSprPerfectEffect;
		std::vector<cSprite*>	m_aPerfectEffects;
	
		// point
		Point					m_tPoint;
	
		// power
		KDbool					m_bIsPowerGetting;
		KDbool					m_bIsMaxPower;
		KDint					m_nPowerLevel;
		KDfloat					m_fPower;
		KDfloat					m_fPowerGetSpeed;
		const KDfloat			m_fPowerLv1CutOff;
		const KDfloat			m_fPowerLv2CutOff;
		const KDfloat			m_fPowerLv3CutOff;
	
		// max power
		KDfloat					m_fMaxPowerGauge;
		KDbool					m_bIsMaxPowerGaugeAdd;
	
		// scale
		KDbool					m_bIsScaleAdd;
		KDfloat					m_fScaleY;
		const KDdouble			m_dScaleAddMaxLv2;
		const KDdouble			m_dScaleAddMaxLv3;
	
		// angry
		KDbool					m_bIsAngry;
		KDfloat					m_fAngryColor;
		KDbool					m_bIsAngryColorAdd;
};

#endif	// __Char_h__