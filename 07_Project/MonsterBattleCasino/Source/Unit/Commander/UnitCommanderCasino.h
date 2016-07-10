/* -----------------------------------------------------------------------------------
 *
 *      File            UnitCommanderCasino.h
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

#ifndef __UnitCommanderCasino_h__
#define __UnitCommanderCasino_h__

#include "UnitCommander.h"
#include "Unit/UI/MobCasinoStatus.h"

class cUnitCommanderCasino : public cUnitCommander
{
	public :

		// init & destroy
				 cUnitCommanderCasino ( Node* pNode, std::vector<cSprEffects*>& rListEffects, const std::vector<cUnit*>& rMobs, eModeType eMode );
		virtual ~cUnitCommanderCasino ( KDvoid );

		// update
		KDvoid							update ( KDdouble dLeftTickCount );

		// set
		KDvoid							setModeStatus ( KDvoid );
		KDvoid							setModeMagnification ( KDvoid );
			
		KDvoid							setTimeStatus ( KDint nSeatIndex, KDdouble nTime, KDint nCount, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt );
		KDvoid							setDamage ( KDint nSeatIndex, KDint nDamage );

		// get
		cUnit*							getMyMob ( KDvoid );
		inline KDint					getMyMobIndex ( KDvoid ) { return m_nMyMobIndex;	}

	private :

		KDvoid							updateActive ( KDdouble dLeftTickCount );
		KDvoid							updateAction ( KDdouble dLeftTickCount );

		// update script
		KDbool							updateScriptRangeType		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );

		KDbool							updateScriptAttackMotion	( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptSpecialMotion	( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );

		KDbool							updateScriptDamage			( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );

		KDbool							updateScriptWaitEvent		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptWaitTime		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );

		KDbool							updateScriptTargetClear		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptTargetMy		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptTargetOne		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptTargetAll		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptTargetRand		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );

		KDbool							updateScriptHP				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptATK				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptDEF				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptAGL				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptCRT				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );

		KDbool							updateScriptEffect			( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptEffectFlicker	( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );
		KDbool							updateScriptTimeEffect		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript );

	private :

		// casino status ui
		cMobCasinoStatus*				m_pCasinoStatus;

		// bet mob index
		KDint							m_nMyMobIndex;

		//active sign icon
		std::vector<cSprite*>			m_aActiveMobSigns;

		//get coin
		std::vector<cSprAnis*>			m_aAniGetCoins;
		KDint64							m_nGetLeftCoin;
		KDdouble						m_dGetCoinCheckTime;
		KDdouble						m_dGetCoinLeftTime;
};

#endif	// __UnitCommanderCasino_h__
