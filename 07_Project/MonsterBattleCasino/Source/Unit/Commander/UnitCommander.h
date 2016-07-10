/* -----------------------------------------------------------------------------------
 *
 *      File            UnitCommander.h
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

#ifndef __UnitCommander_h__
#define __UnitCommander_h__

#include "Object/Sprite/Sprite.h"
#include "Object/Sprite/SprAni.h"
#include "Object/Sprite/SprEffects.h"
#include "Object/Label/LabelAtlasCustom.h"
#include "Unit/Unit/Unit.h"

typedef struct _sActive
{
	eUnitType					eUnit;
	KDint						nUnitID;
	KDint						nSeatIndex;
	KDint						nActionIndex;
	KDint						nScriptIndex;
	KDint						nSelectHeroID;
	KDint						nSelectMobSeatIndex;
	eRangeType					eRange;
	KDdouble					dLeftTime;
	KDbool						bIsActionStart;
	std::vector<KDint>			aTargetHeroIDs;
	std::vector<KDint>			aTargetMobIndexs;
} sActive;

typedef struct _sLabelDamage
{
	KDbool						bIsMove;
	KDint						nDamageType;
	KDint						nDamage;
	KDint						nMoveType;
	Point						tPointBackup;
	Point						tPoint;
	KDfloat						fMoveY;
	KDfloat						fBottomY;
	KDdouble					dLastLeftTime;

	cSprite*					pSprCri;
	cLabelAtlasCustom*			pLabelBad;
	cLabelAtlasCustom*			pLabelCriBad;
	cLabelAtlasCustom*			pLabelGood;
} sLabelDamage;


typedef struct _sReservationLabelDamage
{
	KDint						nLabelIndex;
	KDdouble					dLeftTime;
	KDint						nDamageType;
	Point						tPoint;
	KDint						nDamage;
} sReservationLabelDamage;


typedef struct _sTimeEffect
{
	eUnitType					eUnit;
	KDint						nUnitID;
	KDint						nSeatIndex;
	KDint						nEffectIndex;
	std::string					sEffectName;
	Point						tPoint;
	KDdouble					dTime;
	KDdouble					dLeftTime;
	KDint						nLeftCount;
} sTimeEffect;


typedef struct _sReservationEffect
{
	KDint						nEffectIndex;
	std::string					sEffectName;
	Point						tPoint;
	KDdouble					dLeftTime;
} sReservationEffect;

typedef struct _sMobCursor
{
	KDbool						bIsView;
	KDint						nSeatIndex;
	cSprAni*					pAni;
} sMobCursor;


typedef struct _sStatusEffect
{
	KDbool						bIsView;
	KDdouble					dLeftTime;
	cSprAni*					pAni;
	Point						tPoint;
} sStatusEffect;


typedef struct _sReservationStatusEffect
{
	KDdouble					dWaitLeftTime;
	KDint						nIndex;
	eStatusEffect				eStatusType;
	Point						tPoint;
} sReservationStatusEffect;

class cUnitCommander : public Object
{
	public :

		// int & destroy
				 cUnitCommander ( Node* pNode, eModeType eMode, std::vector<cSprEffects*>& rListEffects, KDint nLabelDamageZ1, KDint nLabelDamageZ2, KDint nLabelDamageZ3, KDint nStatusEffectZ1, KDint nStatusEffectZ2, KDint nStatusEffectZ3 );
		virtual ~cUnitCommander ( KDvoid );

		KDvoid									initDamageLabel ( KDint nZ1, KDint nZ2, KDint nZ3 );

		// update
		virtual KDvoid							update ( KDdouble dLeftTickCount, KDbool& bIsCreate );

		// set
		KDvoid									setDamageLabelColor ( KDint r, KDint g, KDint b );
		KDvoid									setStatusEffectColor ( KDint r, KDint g, KDint b );
		inline KDvoid							setUnitCreateCount ( KDint nUnitCreateCount ) { m_nUnitCreateCount = nUnitCreateCount; }
		inline KDvoid							setUnitCreateLeftTime ( KDdouble dLeftTime ) { m_dUnitCreateLeftTime = dLeftTime; }

		//get
		inline std::vector<cUnit*>&				getMobs ( KDvoid ) { return m_aMobs; }

		KDvoid									setGameEndByForce  ( KDvoid );
		KDbool									getIsGameEnd ( KDvoid );
		KDint									getLivHeroCount ( KDvoid );
		inline KDint							getUnitCreateCount ( KDvoid ) { return m_nUnitCreateCount; }
		inline KDdouble							getUnitCreateLeftTime ( KDvoid ) { return m_dUnitCreateLeftTime; }

		// active
		KDvoid									setActives ( const std::vector<sActive>& rActives );
		KDint									getActiveMax ( KDvoid );
		sActive*								getActive ( KDint nIndex );

		// mob active gages
		KDvoid									setMobActiveGages ( const std::vector<KDfloat>& rGages );
		const std::vector<KDfloat>&				getMobActiveGages ( KDvoid );

		// effect
		KDvoid									setTimeEffects ( const std::vector<sTimeEffect>& rTimeEffects );
		const std::vector<sTimeEffect>&			getTimeEffects ( KDvoid ) { return m_aTimeEffects; }

	protected :

		// get unit
		cUnit*									getUnit ( KDint nUnitID );

		// active
		KDvoid									addActive ( eUnitType eUnit, KDint nUnitID, KDint nSeatIndex, KDint nActionIndex, KDint nSelectHeroID, KDint nSelectMobSeatIndex );
		KDvoid									delActive ( KDint nUnitID, KDint nSeatIndex );
		sActive*								getActiveFirst ( KDvoid );

		// line count
		KDvoid									getUnitLineCount ( KDint& rHeroFrontCount, KDint& rHeroBackCount, KDint& rMobLine1Count, KDint& rMobLine2Count, KDint& rMobLine3Count );

		// mob active gages
		KDvoid									initMobActiveGages ( KDint nSeatMax );
		KDvoid									setMobActiveGageZero ( KDint nIndex );
		KDfloat									getMobActiveGage ( KDint nIndex );

		// damage label effect
		KDvoid									viewDamageLabel ( KDint nLabelIndex, KDint nType, Point tPoint, KDint nDamage );
		KDvoid									addDamageLabelReservation ( KDint nLabelIndex, KDdouble dLeftTime, KDint nDamageType, Point tPoint, KDint nDamage );

		// effect
		KDvoid									viewEffect ( KDint nIndex, std::string sEffectName, Point tPoint );
		KDvoid									viewTimeEffect ( sTimeEffect& rTimeEffect );
		KDvoid									delTimeEffectAll ( KDvoid );
		KDvoid									delTimeEffect ( eUnitType eUnit, KDint nUnitID, KDint nSeatIndex );
		KDbool									isEffect ( KDvoid );

		// status effect
		KDvoid									viewStatusEffect			( KDint nIndex, eStatusEffect eStatusType, Point tPoint );
		KDvoid									viewStatusEffectReservation ( KDint nIndex, eStatusEffect eStatusType, Point tPoint, KDdouble dWaitTime );

		// mob die
		KDvoid									setHpZeroDie ( KDvoid );	// hp가 없는 몹은 다이 처리해준다.

		// exp up
		KDvoid									viewExpUpEffect ( Point tPoint );
		
		// 아케이드, 서바이벌 모드 시 유닛 생성.
		KDbool									unitCreate ( KDdouble dLeftTime );

		// update
		virtual KDbool							updateScriptRangeType		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;

		virtual KDbool							updateScriptAttackMotion	( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptSpecialMotion	( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;

		virtual KDbool							updateScriptDamage			( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;

		virtual KDbool							updateScriptWaitEvent		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptWaitTime		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;

		virtual KDbool							updateScriptTargetClear		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptTargetMy		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptTargetOne		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptTargetAll		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptTargetRand		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;

		virtual KDbool							updateScriptHP				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptATK				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptDEF				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptAGL				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptCRT				( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;

		virtual KDbool							updateScriptEffect			( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptEffectFlicker	( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;
		virtual KDbool							updateScriptTimeEffect		( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript ) = 0;

	protected :

		// scene
		Node*									m_pNode;

		// heros
		std::vector<cUnit*>						m_aHeros;
	
		//mobs
		std::vector<cUnit*>						m_aMobs;

		// game type
		eGameType								m_eGameType;

		// mode type
		eModeType								m_eModeType;

		// active gage
		std::vector<KDfloat>					m_aMobActiveGages;

		// unit create count
		KDint									m_nUnitCreateCount;
		KDint									m_nUnitCreateLastSeatIndex;
		KDdouble								m_dUnitCreateLeftTime;

		// unit create 연출
		KDint									m_nUnitCreateDirectSeatIndex;

	private :

		// update
		KDvoid									updateUnitTurn		( KDdouble dLeftTickCount );
		KDvoid									updateTimeEffect	( KDdouble dLeftTickCount );
		KDvoid									updateDamageLabel	( KDdouble dLeftTickCount );
		KDvoid									updateStatusEffect	( KDdouble dLeftTickCount );
		KDvoid									updateReservation	( KDdouble dLeftTickCount );
		KDvoid									updateExpEffect		( KDdouble dLeftTickCount );
		KDbool									updateUnitCreate	( KDdouble dLeftTickCount );

	private :

		// effects
		std::vector<cSprEffects*>				m_aListEffects;

		// active list
		std::vector<sActive>					m_aActives;					// 활동중인 유닛.

		// damage label effect
		std::vector<sLabelDamage>				m_aLabelDamages1;
		std::vector<sLabelDamage>				m_aLabelDamages2;
		std::vector<sLabelDamage>				m_aLabelDamages3;
		std::vector<sReservationLabelDamage>	m_aReservationlabelDamages;

		// status effects
		std::vector<sStatusEffect>				m_aStatusEffects1;
		std::vector<sStatusEffect>				m_aStatusEffects2;
		std::vector<sStatusEffect>				m_aStatusEffects3;
		std::list<sReservationStatusEffect>		m_aStatusEffectsReservation;

		// time effects
		std::vector<sTimeEffect>				m_aTimeEffects;

		// Reservation effects
		std::vector<sReservationEffect>			m_aReservationEffects;

		// exp plus
		cSprite*								m_pSprExpUp;

		// unit create 연출
		KDint									m_nUnitCreateDirectStep;
		KDdouble								m_dUnitCreateDirectTime;
	
		// game end
		KDbool									m_bIsGameEndByForce;
};

#endif	// __UnitCommander_h__
