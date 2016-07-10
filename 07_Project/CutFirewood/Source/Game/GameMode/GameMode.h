/* -----------------------------------------------------------------------------------
 *
 *      File            GameMode.h
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

#ifndef __GameMode_h__
#define __GameMode_h__

#include "Scene/JBase/JBaseScene.h"

class cAnimations;
class cColorLayer;
class cSprite;
class cCommander;

class cGameMode : public Object
{	
	public :

		// init
		cGameMode ( JBaseScene* pScene, cCommander* pCommander );
		virtual ~cGameMode ( KDvoid );

		// set
		KDvoid				setGaugeZero ( KDvoid );
		KDvoid				setAddTimeGauge ( KDbool bIsMiss, KDbool bIsComboAdd );
		KDvoid				setAddBerserkerGauge ( KDbool bIsMiss, KDint nComboCount, KDbool bIsMaxPower );
	
		// get
		inline KDbool		getIsTimeMode ( KDvoid ) { return m_bIsTimeMode; }
		inline KDbool		getIsBerserkerMode ( KDvoid ) {	return m_bIsBerserkerMode; }
	
		// update
		KDvoid				update ( KDdouble dLeftTime );

	private :

		// update
		KDvoid				updateGauge ( KDdouble dLeftTime );
	
		//commander
		cCommander*			m_pCommander;
	
		// sprite
		cSprite*			m_pBack;
		cSprite*			m_pSprGaugeTime;
		cSprite*			m_pSprGaugeTime2;
		cSprite*			m_pSprGaugeTime3;
		cSprite*			m_pSprGaugeBerserker;
		cSprite*			m_pSprGaugeBerserker2;
		cSprite*			m_pSprGaugeBerserker3;
		cSprite*			m_pSprCutin1;
		cSprite*			m_pSprCutin2;

		// color layer
		cColorLayer*		m_pColorLayerIntro;
	
		// size
		Size				m_tTimeGauge;
		Size				m_tBerserkerGauge;
	
		// switch
		KDbool				m_bIsTimeMode;
		KDbool				m_bTimeModeGaugeOpacityAdd;
		KDbool				m_bIsBerserkerMode;
		KDbool				m_bBerserkerModeGaugeOpacityAdd;
	
		// time mode intro
		KDbool				m_bIsTimeModeIntro;
		KDint				m_nTimeModeIntroStep;
		KDdouble			m_dTimeModeIntroTime;
		KDint				m_nTimeModeIntroCutinType;

		// berserker mode intro
		KDbool				m_bIsBerserkerModeIntro;
		KDint				m_nBerserkerModeIntroStep;
		KDdouble			m_dBerserkerModeIntroTime;
		KDint				m_nBerserkerModeIntroCutinType;
	
		// gauge
		KDfloat				m_fGaugeTime;
		KDfloat				m_fGaugeBerserker;
	
		// view
		KDfloat				m_fSeenGaugeTime;
		KDfloat				m_fSeenGaugeBerserker;
		KDfloat				m_fSeenGaugeTimeOpacity;
		KDfloat				m_fSeenGaugeBerserkerOpacity;
};

#endif	// __GameMode_h__