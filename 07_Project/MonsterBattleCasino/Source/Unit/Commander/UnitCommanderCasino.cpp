/* -----------------------------------------------------------------------------------
 *
 *      File            UnitCommanderCasino.cpp
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

#include "Precompiled.h"
#include "UnitCommanderCasino.h"
#include "MyData/MyData.h"

cUnitCommanderCasino::cUnitCommanderCasino ( Node* pNode, std::vector<cSprEffects*>& rListEffects, const std::vector<cUnit*>& rMobs, eModeType eMode )
: cUnitCommander ( pNode, eMode, rListEffects, LAYER_GAMEVIEW_DAMAGE_1, LAYER_GAMEVIEW_DAMAGE_1, LAYER_GAMEVIEW_DAMAGE_1, LAYER_GAMEVIEW_EFFECT_2, LAYER_GAMEVIEW_EFFECT_2, LAYER_GAMEVIEW_EFFECT_2 )
, m_pCasinoStatus		( nullptr	)
, m_nMyMobIndex			( 0			)
, m_nGetLeftCoin		( 0			)
, m_dGetCoinCheckTime	( 0			)
, m_dGetCoinLeftTime	( 0			)
{
	// game type
	m_eGameType = eGameType_Casino;

	// mobs
	for ( KDuint i = 0; i < rMobs.size ( ); i++ )
	{
		auto	pMob = rMobs [ i ];

		// check
		if ( pMob == nullptr )
		{
			continue;
		}

		// texLoad
		pMob->texLoad ( eUnitAniType_Attack  );
		pMob->texLoad ( eUnitAniType_Special );
		
		// shadow
		pMob->setIsShadow ( true );
		
		// visible
		pMob->setVisibleNameA	 ( false );
		pMob->setVisibleNameB	 ( false );
		pMob->setVisiblePortrait ( false );
		pMob->setVisible		 ( true  );

		// position
		switch ( i )
		{
			case 0	:	pMob->setPoint ( Point (  50, 70 ) );	break;
			case 1	:	pMob->setPoint ( Point ( 145, 70 ) );	break;
			case 2	:	pMob->setPoint ( Point ( 240, 70 ) );	break;
			case 3	:	pMob->setPoint ( Point ( 334, 70 ) );	break;
			case 4	:	pMob->setPoint ( Point ( 429, 70 ) );	break;
		}

		// animation
		pMob->aniStart ( );
		
		// check bet mob
		if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
		{
			if ( pMob->getBetMoney ( ) > 0 )
			{
				m_nMyMobIndex = i;
			}
		}			

		// push back
		pMob->retain ( );
		m_aMobs.push_back ( pMob );
	}

	// 몬스터 수가 부족하면 공간만 만들어 놓는다.
	while ( m_aMobs.size ( ) < 5 )
	{
		m_aMobs.push_back ( nullptr );
	}

	// active sign icon & shdow icon
	for ( KDuint i = 0; i < rMobs.size ( ); i++ )
	{
		cSprite*	pSprActiveSign = nullptr;

		switch ( i )
		{
			case 0	:	pSprActiveSign	= new cSprite ( m_pNode, "Game/active_mob_sign.png", LAYER_GAMEVIEW_MOB_CUR, Point ( 0.5f, 0.5f ), Point (  50, 70 ) );		break;
			case 1	:	pSprActiveSign	= new cSprite ( m_pNode, "Game/active_mob_sign.png", LAYER_GAMEVIEW_MOB_CUR, Point ( 0.5f, 0.5f ), Point ( 145, 70 ) );		break;
			case 2	:	pSprActiveSign	= new cSprite ( m_pNode, "Game/active_mob_sign.png", LAYER_GAMEVIEW_MOB_CUR, Point ( 0.5f, 0.5f ), Point ( 240, 70 ) );		break;
			case 3	:	pSprActiveSign	= new cSprite ( m_pNode, "Game/active_mob_sign.png", LAYER_GAMEVIEW_MOB_CUR, Point ( 0.5f, 0.5f ), Point ( 334, 70 ) );		break;
			case 4	:	pSprActiveSign	= new cSprite ( m_pNode, "Game/active_mob_sign.png", LAYER_GAMEVIEW_MOB_CUR, Point ( 0.5f, 0.5f ), Point ( 429, 70 ) );		break;
		}

		pSprActiveSign->get ( )->setVisible ( false );

		m_aActiveMobSigns.push_back ( pSprActiveSign );
	}

	// active gage
	initMobActiveGages ( m_aMobs.size ( ) );

	// casino status ui
	m_pCasinoStatus = new cMobCasinoStatus ( m_pNode, m_aMobs, m_eModeType, m_nMyMobIndex );

	// bet info setting
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		m_aMobs [ m_nMyMobIndex ]->setVisiblePortrait ( true );
		m_aMobs [ m_nMyMobIndex ]->setPointPortrait ( Point ( 27, 50 ) );
	}

	// get coin
	for ( KDuint i = 0; i < m_aMobs.size ( ); i++)
	{
		cSprAnis*	pAni = nullptr;

		switch ( i )
		{
			case 0	:	pAni = new cSprAnis ( m_pNode, "Game/get_coin.spr", LAYER_GAMEVIEW_DAMAGE_2, Point (  50, 70 ) );	break;
			case 1	:	pAni = new cSprAnis ( m_pNode, "Game/get_coin.spr", LAYER_GAMEVIEW_DAMAGE_2, Point ( 145, 70 ) );	break;
			case 2	:	pAni = new cSprAnis ( m_pNode, "Game/get_coin.spr", LAYER_GAMEVIEW_DAMAGE_2, Point ( 240, 70 ) );	break;
			case 3	:	pAni = new cSprAnis ( m_pNode, "Game/get_coin.spr", LAYER_GAMEVIEW_DAMAGE_2, Point ( 334, 70 ) );	break;
			case 4	:	pAni = new cSprAnis ( m_pNode, "Game/get_coin.spr", LAYER_GAMEVIEW_DAMAGE_2, Point ( 429, 70 ) );	break;
		}

		pAni->texLoad ( 0 );
		pAni->changeAni ( 0 );
		pAni->stop ( );
		pAni->setVisible ( false );

		m_aAniGetCoins.push_back ( pAni );
	}
}

cUnitCommanderCasino::~cUnitCommanderCasino ( KDvoid )
{
	// casino status ui
	CC_SAFE_RELEASE ( m_pCasinoStatus );

	// active sign icon
	SAFE_RELEASE_ARRAYS ( m_aActiveMobSigns );

	// get coin
	SAFE_RELEASE_ARRAYS ( m_aAniGetCoins );
}

KDvoid cUnitCommanderCasino::update ( KDdouble dLeftTickCount )
{
	// init
	KDbool		bIsCreate = false;

	// active
	updateActive ( dLeftTickCount );

	// super
	cUnitCommander::update ( dLeftTickCount, bIsCreate );

	// status parameter init
	if ( bIsCreate != false )
	{
		m_pCasinoStatus->setParaSub ( m_aMobs [ m_nUnitCreateDirectSeatIndex ] );
	}

	// casino status
	m_pCasinoStatus->update ( m_aMobs, getMobActiveGages ( ) );

	// active sign img
	sActive*	pActive = getActiveFirst ( );

	if ( pActive == nullptr)
	{
		for ( auto pNode : m_aActiveMobSigns )
		{
			pNode->get ( )->setOpacity ( 0 );
			pNode->get ( )->setVisible ( false );
		}
	}
	else
	{
		for ( KDint i = 0; i < (KDint) m_aActiveMobSigns.size ( ); i++ )
		{
			if ( i != pActive->nSeatIndex )
			{
				m_aActiveMobSigns [ i ]->get ( )->setOpacity ( 0 );
				m_aActiveMobSigns [ i ]->get ( )->setVisible ( false );
			}
			else
			{
				KDint		nOpacity = m_aActiveMobSigns [ i ]->get ( )->getOpacity ( ) + 15;
				if ( nOpacity > 255 )
				{
					nOpacity = 255;
				}

				m_aActiveMobSigns [ i ]->get ( )->setVisible ( true );
				m_aActiveMobSigns [ i ]->get ( )->setOpacity ( nOpacity );
			}
		}
	}

	// action
	updateAction ( dLeftTickCount );

	// reset papa adds & clear turn effect
	if ( getIsGameEnd ( ) == true )
	{
		for ( auto pNode : m_aMobs )
		{
			if ( pNode == nullptr )
			{
				continue;
			}

			pNode->resetParaAdds ( );
		}

		delTimeEffectAll ( );
	}

	// get coin
	if ( m_nGetLeftCoin > 0 && m_dGetCoinCheckTime > 0 )
	{
		m_dGetCoinLeftTime -= dLeftTickCount;

		while ( m_nGetLeftCoin > 0 && m_dGetCoinLeftTime < 0 )
		{
			--m_nGetLeftCoin;
			++cMyData::getObj ( )->m_nMoney;

			m_dGetCoinLeftTime += m_dGetCoinCheckTime;
		}
	}		

	for ( auto pNode : m_aAniGetCoins )
	{
		if ( pNode->getVisible ( ) == true && pNode->update ( dLeftTickCount ) == true )
		{
			pNode->stop ( );
			pNode->setVisible ( false );
		}
	}
}

KDvoid cUnitCommanderCasino::updateActive ( KDdouble dLeftTickCount)
{
	if ( getIsGameEnd ( ) == true )
	{
		return;
	}

	// mobs------------------------------
	for ( KDuint i = 0; i < m_aMobs.size ( ); i++ )
	{
		// mob
		cUnit*		pMob = m_aMobs [ i ];

		// check
		if ( pMob == nullptr || pMob->getDie ( ) == true || m_aMobActiveGages [ i ] == 100 )
		{
			continue;
		}

		// gage
		KDdouble	dBase = 10.5 * dLeftTickCount;
		KDdouble	dPlus = pMob->getUseAgl ( ) * ( dLeftTickCount / 3 );
		m_aMobActiveGages [ i ] += dBase + dPlus;

		// max
		if ( m_aMobActiveGages [ i ] >= 100 )
		{
			m_aMobActiveGages [ i ] = 100;
			addActive ( eUnitType_Mob, -1, i, pMob->getActionRandIndex ( ), -1, -1 );
		}
	}
}

KDvoid cUnitCommanderCasino::updateAction ( KDdouble dLeftTickCount )
{
	// mobs------------------------------
	// get now active
	sActive*	pActive = getActiveFirst ( );
	if ( pActive == nullptr )
	{
		return;
	}

	// get active mob
	cUnit*		pMob = m_aMobs [ pActive->nSeatIndex ];

	// die check
	if ( pMob == nullptr || pMob->getDie ( ) == true )
	{
		delActive ( pActive->nUnitID, pActive->nSeatIndex );
		return;
	}

	// switch
	KDbool		bIsEraseActive = false;

	// action
	while ( true )
	{
		// new script
		cUnitScript*	pScript = pMob->getScript ( pActive->nActionIndex, pActive->nScriptIndex );

		// 모든 스크립트가 끝났다면 현재 액티브를 다은 몹에 넘겨준다.
		if ( pScript == nullptr )
		{
			// 게이지를 초기화 한다.
			setMobActiveGageZero ( pActive->nSeatIndex );

			// 죽는것을 처리해준다.
			setHpZeroDie ( );

			//switch
			bIsEraseActive = true;

			break;
		}

		// 첫번째 턴이라면 몹의 턴처리를 해준다.
		if ( pActive->nScriptIndex == 0 && pActive->bIsActionStart == false )
		{
			// switch
			pActive->bIsActionStart = true;
		}

		// is exit init
		KDbool		bIsExit = false;

		// processing
		switch ( (KDint) pScript->eType )
		{
			case eScriptType_Skill_rangeType	:	bIsExit = updateScriptRangeType		( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_AttackMotion		:	bIsExit = updateScriptAttackMotion	( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_SpecialMotion		:	bIsExit = updateScriptSpecialMotion	( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_Damage				:	bIsExit = updateScriptDamage		( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_WaitEvent			:	bIsExit = updateScriptWaitEvent		( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_WaitTime			:	bIsExit = updateScriptWaitTime		( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_TargetClear		:	bIsExit = updateScriptTargetClear	( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_TargetMy			:	bIsExit = updateScriptTargetMy		( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_TargetOne			:	bIsExit = updateScriptTargetOne		( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_TargetAll			:	bIsExit = updateScriptTargetAll		( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_TargetRand			:	bIsExit = updateScriptTargetRand	( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_HP					:	bIsExit = updateScriptHP			( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_ATK				:	bIsExit = updateScriptATK			( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_DEF				:	bIsExit = updateScriptDEF			( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_AGL				:	bIsExit = updateScriptAGL			( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_CRT				:	bIsExit = updateScriptCRT			( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_Effect				:	bIsExit = updateScriptEffect		( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_EffectFlicker		:	bIsExit = updateScriptEffectFlicker	( dLeftTickCount, pMob, pActive, pScript );		break;
			case eScriptType_Time_Effect		:	bIsExit = updateScriptTimeEffect	( dLeftTickCount, pMob, pActive, pScript );		break;
		}

		// check exit
		if ( bIsExit == true )
		{
			break;
		}

		// next script index
		++pActive->nScriptIndex;
	}

	// erase node
	if ( bIsEraseActive == true )
	{
		delActive ( pActive->nUnitID, pActive->nSeatIndex );
	}
}

KDbool cUnitCommanderCasino::updateScriptRangeType ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	return false;
}

KDbool cUnitCommanderCasino::updateScriptAttackMotion ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	pUnit->changeAni ( eUnitAniType_Attack );

	return false;
}

KDbool cUnitCommanderCasino::updateScriptSpecialMotion ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	pUnit->changeAni ( eUnitAniType_Special );

	return false;
}

KDbool cUnitCommanderCasino::updateScriptDamage ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	for ( auto nNode : pActive->aTargetMobIndexs )
	{
		cUnit*		pTargetMob = m_aMobs [ nNode ];

		if ( pTargetMob == NULL || pTargetMob->getDie ( ) == true )	
		{
			continue;
		}

		// para
		KDint		nAtk = pUnit	 ->getUseAtk ( );
		KDint		nCrt = pUnit	 ->getUseCrt ( );
		KDint		nDef = pTargetMob->getUseDef ( );

		// damage
		KDint		nDamage	= nAtk - nDef;
		if ( nDamage < 1 )
		{
			nDamage = 1;
		}

			 if ( nDamage <  7 )	nDamage += ( kdRand ( ) % 3 ) - 1;
		else if ( nDamage < 14 )	nDamage += ( kdRand ( ) % 5 ) - 2;
		else if ( nDamage < 21 )	nDamage += ( kdRand ( ) % 7 ) - 3;
		else						nDamage += ( kdRand ( ) % 9 ) - 4;

		if ( nDamage < 1 )
		{
			nDamage = 1;
		}

		// critical
		KDbool		bIsCri = false;
		if ( kdRand ( ) % 100 <= nCrt )
		{
			bIsCri = true;
			nDamage *= 2;
		}

		// 보정
		if ( nDamage > pTargetMob->getUseHp ( ) )
		{
			nDamage = pTargetMob->getUseHp ( );
		}

		// damage label effect
		if ( bIsCri == false )	viewDamageLabel ( 0, 0, pTargetMob->getPoint ( ), nDamage );
		else					viewDamageLabel ( 0, 2, pTargetMob->getPoint ( ), nDamage );

		// set hp
		pTargetMob->setUseHp ( pTargetMob->getUseHp ( ) - nDamage );

		// set ani
		pTargetMob->changeAni ( eUnitAniType_Damage );

		// is coin up
		KDbool		bIsCoinUp = false;

		// 킬수를 올려줘야 한다.
		if ( pUnit->getUnitID ( ) > 0 && pTargetMob->getUseHp ( ) == 0 )
		{
			// kill count
			pUnit->setKillCount ( pUnit->getKillCount ( ) + 1 );
			++cMyData::getObj ( )->m_nGamingKillCount;

			// coin up
			bIsCoinUp = true;

			// exp를 올려줘야 한다.
			cMyData::getObj ( )->gamingExpAdd ( pUnit->getCapablePoint ( ), pTargetMob->getCapablePoint ( ) );

			// exp 연출
			viewExpUpEffect ( pUnit->getPoint ( ) + pUnit->getPointSpecial ( ) + Point ( -27, -8 ) );
		}
		// 공격한 유닛이 자신의 유닛이고 타겟 유닛이 죽었다면 해당 데미지 많큼 돈을 올려주고 코인 연출을 해준다.
		else if ( pUnit->getSeatIndex ( ) == getMyMob ( )->getSeatIndex ( ) && pTargetMob->getUseHp ( ) == 0 )
		{
			// coin up
			bIsCoinUp = true;
		}

		// 코인을 올려준다.
		if ( bIsCoinUp == true )
		{
			m_aAniGetCoins [ pTargetMob->getSeatIndex ( ) ]->setVisible ( true );
			m_aAniGetCoins [ pTargetMob->getSeatIndex ( ) ]->start ( );

			m_nGetLeftCoin += nDamage;

			m_dGetCoinCheckTime = 0.15;
			m_dGetCoinLeftTime  = m_dGetCoinCheckTime;

			if ( cMyData::getObj()->m_bIsOptionSound != 0 )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "casino_result_coin_2.mp3" );
			}
		}
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptWaitEvent ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	// 기본동작이 아닌 몹이 있다면 기다려준다.
	for ( auto pMob : m_aMobs )
	{
		if ( pMob != nullptr && pMob->getAniType ( ) != eUnitAniType_Normal )
		{
			return true;
		}
	}

	// 이펙트가 터지는 중이라면 기다려준다.
	return isEffect ( );
}

KDbool cUnitCommanderCasino::updateScriptWaitTime ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	if ( pActive->dLeftTime == -1 )
	{
		pActive->dLeftTime = pScript->fSub / 1000;
	}
	else
	{
		pActive->dLeftTime -= dLeftTickCount;

		if ( pActive->dLeftTime < 0 )
		{
			pActive->dLeftTime = -1;
			return false;
		}
	}

	return true;
}

KDbool cUnitCommanderCasino::updateScriptTargetClear ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	pActive->aTargetMobIndexs.clear ( );

	return false;
}

KDbool cUnitCommanderCasino::updateScriptTargetMy ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	pActive->aTargetMobIndexs.push_back ( pUnit->getSeatIndex ( ) );

	return false;
}

KDbool cUnitCommanderCasino::updateScriptTargetOne ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	if ( m_eModeType == eModeType_MyRoom_Arcade_Easy || m_eModeType == eModeType_MyRoom_Arcade_Nor || m_eModeType == eModeType_MyRoom_Arcade_Hard || m_eModeType == eModeType_MyRoom_Survival )
	{
		if ( pUnit->getSeatIndex ( ) == 0 )
		{
			std::vector<KDint>		aRandIndexs;
			
			for ( auto pNode : m_aMobs )
			{
				if ( pNode != nullptr && pNode->getDie() == false && pNode->getSeatIndex ( ) != pUnit->getSeatIndex ( ) )
				{
					aRandIndexs.push_back ( pNode->getSeatIndex ( ) );
				}
			}
			
			if ( aRandIndexs.size ( ) > 0 )
			{
				pActive->aTargetMobIndexs.push_back( aRandIndexs [ kdRand ( ) % aRandIndexs.size ( ) ] );
			}
		}
		else
		{
			pActive->aTargetMobIndexs.push_back ( 0 );
		}
	}
	else
	{
		std::vector<KDint>		aRandIndexs;
		
		for ( auto pNode : m_aMobs )
		{
			if ( pNode != nullptr && pNode->getDie ( ) == false && pNode->getSeatIndex ( ) != pUnit->getSeatIndex ( ) )
			{
				aRandIndexs.push_back ( pNode->getSeatIndex ( ) );
			}
		}
		
		if ( aRandIndexs.size ( ) > 0 )
		{
			pActive->aTargetMobIndexs.push_back ( aRandIndexs [ kdRand ( ) % aRandIndexs.size ( ) ] );
		}
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptTargetAll ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	if ( m_eModeType == eModeType_MyRoom_Arcade_Easy || m_eModeType == eModeType_MyRoom_Arcade_Nor || m_eModeType == eModeType_MyRoom_Arcade_Hard || m_eModeType == eModeType_MyRoom_Survival )
	{
		if ( pUnit->getSeatIndex ( ) == 0 )
		{
			for ( auto pNode : m_aMobs )
			{
				if ( pNode != nullptr && pNode->getDie ( ) == false && pNode->getSeatIndex ( ) != pUnit->getSeatIndex ( ) )
				{
					pActive->aTargetMobIndexs.push_back ( pNode->getSeatIndex ( ) );
				}
			}
		}
		else
		{
			pActive->aTargetMobIndexs.push_back ( 0 );
		}
	}
	else
	{
		for ( auto pNode : m_aMobs )
		{
			if ( pNode != nullptr && pNode->getDie ( ) == false && pNode->getSeatIndex ( ) != pUnit->getSeatIndex ( ) )
			{
				pActive->aTargetMobIndexs.push_back ( pNode->getSeatIndex ( ) );
			}
		}
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptTargetRand ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	std::vector<KDint>		aRandIndexs;

	for ( auto pNode : m_aMobs )
	{
		if ( pNode != nullptr && pNode->getDie ( ) == false )
		{
			aRandIndexs.push_back ( pUnit->getSeatIndex ( ) );
		}
	}

	if ( aRandIndexs.size ( ) > 0 )
	{
		pActive->aTargetMobIndexs.push_back ( aRandIndexs [ kdRand ( ) % aRandIndexs.size ( ) ] );
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptHP ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	for ( auto nIndex : pActive->aTargetMobIndexs )
	{
		cUnit*		pTargetMob = m_aMobs [ nIndex ];

		if ( pTargetMob == nullptr || pTargetMob->getDie ( ) == true )	
		{
			continue;
		}

		// hp
		KDdouble	dTime  = (KDfloat) ( (KDint) pScript->fSub % 10000 ) / 1000;
		KDint		nCount = (KDint) pScript->fSub / 10000;
		KDint		nHp	  = atoi( pScript->sStr.c_str ( ) );

		pTargetMob->setUseHpAdd ( dTime, nCount, nHp, pUnit->getSeatIndex ( ) );

		// status effect
			 if ( nHp > 0			   )	viewStatusEffect ( 0, eStatusEffect_Regene, pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
		else if ( nHp < 0 && dTime > 0 )	viewStatusEffect ( 0, eStatusEffect_poison, pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptATK ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	for ( auto nIndex : pActive->aTargetMobIndexs )
	{
		cUnit*		pTargetMob = m_aMobs [ nIndex ];

		if ( pTargetMob == nullptr || pTargetMob->getDie ( ) == true )	
		{
			continue;
		}

		// atk
		KDdouble	dTime = pScript->fSub / 1000;
		KDint		nAtk  = kdAtoi ( pScript->sStr.c_str ( ) );

		pTargetMob->setUseAtkAdd ( dTime, nAtk );

		// status effect
		if ( nAtk > 0 )	viewStatusEffect ( 0, eStatusEffect_AtkUp  , pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
		else			viewStatusEffect ( 0, eStatusEffect_AtkDown, pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptDEF ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	for ( auto nIndex : pActive->aTargetMobIndexs )
	{
		cUnit*		pTargetMob = m_aMobs [ nIndex ];

		if ( pTargetMob == nullptr || pTargetMob->getDie ( ) == true )	
		{
			continue;
		}

		// def
		KDdouble	dTime = pScript->fSub / 1000;
		KDint		nDef  = kdAtoi ( pScript->sStr.c_str ( ) );

		pTargetMob->setUseDefAdd ( dTime, nDef );

		// status effect
		if ( nDef > 0 )	viewStatusEffect( 0, eStatusEffect_DefUp  , pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
		else			viewStatusEffect( 0, eStatusEffect_DefDown, pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptAGL( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	for ( auto nIndex : pActive->aTargetMobIndexs )
	{
		cUnit*		pTargetMob = m_aMobs [ nIndex ];

		if ( pTargetMob == nullptr || pTargetMob->getDie ( ) == true )	
		{
			continue;
		}

		// agl
		KDdouble	dTime = pScript->fSub / 1000;
		KDint		nAgl  = kdAtoi ( pScript->sStr.c_str ( ) );

		pTargetMob->setUseAglAdd ( dTime, nAgl );

		// status effect
		if ( nAgl > 0 )	viewStatusEffect( 0, eStatusEffect_AglUp  , pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
		else			viewStatusEffect( 0, eStatusEffect_AglDown, pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptCRT ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	for ( auto nIndex : pActive->aTargetMobIndexs )
	{
		cUnit*		pTargetMob = m_aMobs [ nIndex ];

		if ( pTargetMob == nullptr || pTargetMob->getDie ( ) == true )	
		{
			continue;
		}

		// crt
		KDdouble	dTime = pScript->fSub / 1000;
		KDint		nCrt  = kdAtoi ( pScript->sStr.c_str ( ) );

		pTargetMob->setUseCrtAdd ( dTime, nCrt );

		// status effect
		if ( nCrt > 0 )	viewStatusEffect( 0, eStatusEffect_CrtUp  , pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
		else			viewStatusEffect( 0, eStatusEffect_CrtDown, pTargetMob->getPoint ( ) + pTargetMob->getPointSpecial ( ) );
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptEffect ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	for ( auto nTargetIndex : pActive->aTargetMobIndexs )
	{
		cUnit*		pTargetMob	= m_aMobs [ nTargetIndex ];

		if ( pTargetMob == nullptr || pTargetMob->getDie ( ) == true )	
		{
			continue;
		}

		// point
		Point		tAddPoint = Point::ZERO;

		if ( pScript->fSub == 0 )	tAddPoint = Point ( 0, ( pTargetMob->getNorFirstSize ( ).height / 2 ) );
		else						tAddPoint = pTargetMob->getPointSpecial ( );

		// effect
		switch ( nTargetIndex )
		{
			case 0	:	viewEffect ( 0, pScript->sStr.c_str ( ), Point (  50 + tAddPoint.x, 70 + tAddPoint.y ) );	break;
			case 1	:	viewEffect ( 0, pScript->sStr.c_str ( ), Point ( 145 + tAddPoint.x, 70 + tAddPoint.y ) );	break;
			case 2	:	viewEffect ( 0, pScript->sStr.c_str ( ), Point ( 240 + tAddPoint.x, 70 + tAddPoint.y ) );	break;
			case 3	:	viewEffect ( 0, pScript->sStr.c_str ( ), Point ( 334 + tAddPoint.x, 70 + tAddPoint.y ) );	break;
			case 4	:	viewEffect ( 0, pScript->sStr.c_str ( ), Point ( 429 + tAddPoint.x, 70 + tAddPoint.y ) );	break;
		}
	}

	return false;
}

KDbool cUnitCommanderCasino::updateScriptEffectFlicker ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	return false;
}

KDbool cUnitCommanderCasino::updateScriptTimeEffect ( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	for ( auto nTargetIndex : pActive->aTargetMobIndexs )
	{
		cUnit*		pTargetMob	= m_aMobs [ nTargetIndex ];

		if ( pTargetMob == nullptr || pTargetMob->getDie ( ) == true )	
		{
			continue;
		}

		sTimeEffect		tTimeEffect;

		tTimeEffect.eUnit			= eUnitType_Mob;
		tTimeEffect.nUnitID			= -1;
		tTimeEffect.nSeatIndex		= pTargetMob->getSeatIndex ( );
		tTimeEffect.nEffectIndex	= 0;
		tTimeEffect.sEffectName		= pScript->sStr;
		tTimeEffect.nLeftCount		= (KDint) pScript->fSub / 10000;
		tTimeEffect.dTime			= (KDfloat) ( (KDint) pScript->fSub % 10000 ) / 1000;
		tTimeEffect.dLeftTime		= tTimeEffect.dTime;

		// point
		Point			tAddPoint = Point ( 0, ( pTargetMob->getNorFirstSize ( ).height / 2 ) );

		// effect
		switch ( nTargetIndex )
		{
			case 0	:	tTimeEffect.tPoint = Point (  50 + tAddPoint.x, 70 + tAddPoint.y );		break;
			case 1	:	tTimeEffect.tPoint = Point ( 145 + tAddPoint.x, 70 + tAddPoint.y );		break;
			case 2	:	tTimeEffect.tPoint = Point ( 240 + tAddPoint.x, 70 + tAddPoint.y );		break;
			case 3	:	tTimeEffect.tPoint = Point ( 334 + tAddPoint.x, 70 + tAddPoint.y );		break;
			case 4	:	tTimeEffect.tPoint = Point ( 429 + tAddPoint.x, 70 + tAddPoint.y );		break;
		}

		// add
		viewTimeEffect ( tTimeEffect );
	}

	return false;
}

KDvoid cUnitCommanderCasino::setModeStatus ( KDvoid )
{
	m_pCasinoStatus->setModeStatus ( );
}

KDvoid cUnitCommanderCasino::setModeMagnification ( KDvoid )
{
	m_pCasinoStatus->setModeMagnification();
}

KDvoid cUnitCommanderCasino::setTimeStatus ( KDint nSeatIndex, KDdouble nTime, KDint nCount, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt )
{
	auto	pMob = m_aMobs [ nSeatIndex ];

	// die check
	if ( pMob == nullptr || pMob->getDie ( ) == true )
	{
		return;
	}

	// init
	KDdouble	fWaitTime = 0;

	//hp
	if ( nHp != 0 )
	{
		// hp
		pMob->setUseHpAdd ( nTime, nCount, nHp, -1 );

		// timeEffect
		sTimeEffect		tTimeEffect;

		tTimeEffect.eUnit		 = eUnitType_Mob;
		tTimeEffect.nUnitID		 = -1;
		tTimeEffect.nSeatIndex	 = pMob->getSeatIndex ( );
		tTimeEffect.nEffectIndex = 0;
		tTimeEffect.nLeftCount	 = nCount;
		tTimeEffect.dTime		 = nTime;
		tTimeEffect.dLeftTime	 = tTimeEffect.dTime;

		if ( nHp < 0 )	tTimeEffect.sEffectName = _EFFECT_STR_POISON;
		else			tTimeEffect.sEffectName = _EFFECT_STR_REGEN;

		Point			tAddPoint = Point ( 0, ( pMob->getNorFirstSize ( ).height / 2 ) );

		switch ( nSeatIndex )
		{
			case 0	:	tTimeEffect.tPoint = Point (  50 + tAddPoint.x, 70 + tAddPoint.y );		break;
			case 1	:	tTimeEffect.tPoint = Point ( 145 + tAddPoint.x, 70 + tAddPoint.y );		break;
			case 2	:	tTimeEffect.tPoint = Point ( 240 + tAddPoint.x, 70 + tAddPoint.y );		break;
			case 3	:	tTimeEffect.tPoint = Point ( 334 + tAddPoint.x, 70 + tAddPoint.y );		break;
			case 4	:	tTimeEffect.tPoint = Point ( 429 + tAddPoint.x, 70 + tAddPoint.y );		break;
		}

		viewTimeEffect ( tTimeEffect );

		// status effect
		if ( nHp > 0 )	viewStatusEffectReservation ( 0, eStatusEffect_Regene, pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );
		else			viewStatusEffectReservation ( 0, eStatusEffect_poison, pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );

		// effect
		if ( nHp < 0 )	viewEffect ( 0, _EFFECT_STR_POISON, tTimeEffect.tPoint );

		fWaitTime += 0.3f;
	}

	// atk
	if ( nAtk != 0 )
	{
		// add
		pMob->setUseAtkAdd ( nTime, nAtk );

		//status effect
		if ( nAtk > 0 )	viewStatusEffectReservation ( 0, eStatusEffect_AtkUp  , pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );
		else			viewStatusEffectReservation ( 0, eStatusEffect_AtkDown, pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );

		fWaitTime += 0.3f;
	}
	
	// def
	if ( nDef != 0 )
	{
		// add
		pMob->setUseDefAdd ( nTime, nDef );
		
		// status effect
		if ( nDef > 0 )	viewStatusEffectReservation ( 0, eStatusEffect_DefUp  , pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );
		else			viewStatusEffectReservation ( 0, eStatusEffect_DefDown, pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );
		
		fWaitTime += 0.3f;
	}
	
	// agl
	if ( nAgl != 0 )
	{
		// add
		pMob->setUseAglAdd ( nTime, nAgl );
		
		// status effect
		if ( nAgl > 0 )	viewStatusEffectReservation ( 0, eStatusEffect_AglUp  , pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );
		else			viewStatusEffectReservation ( 0, eStatusEffect_AglDown, pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );
		
		fWaitTime += 0.3f;
	}

	// crt
	if ( nCrt != 0 )
	{
		// add
		pMob->setUseCrtAdd ( nTime, nCrt );
		
		// status effect
		if ( nCrt > 0 )	viewStatusEffectReservation ( 0, eStatusEffect_CrtUp  , pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );
		else			viewStatusEffectReservation ( 0, eStatusEffect_CrtDown, pMob->getPoint ( ) + pMob->getPointSpecial ( ), fWaitTime );
		
		fWaitTime += 0.3f;
	}
}

KDvoid cUnitCommanderCasino::setDamage ( KDint nSeatIndex, KDint nDamage )
{
	auto	pMob = m_aMobs [ nSeatIndex ];

	// check
	if ( pMob->getDie ( ) == true || nDamage <= 0 )
	{
		return;
	}

	// critical
	KDbool		bIsCri = false;
	if ( kdRand ( ) % 100 < 10 )
	{
		bIsCri = true;
		nDamage *= 2;
	}

	// 보정
	if ( nDamage > pMob->getUseHp ( ) )
	{
		nDamage = pMob->getUseHp ( );
	}

	// damage label effect
	if ( bIsCri == false )	viewDamageLabel ( 0, 0, pMob->getPoint ( ), nDamage );
	else					viewDamageLabel ( 0, 2, pMob->getPoint ( ), nDamage );

	// set hp
	pMob->setUseHp ( pMob->getUseHp ( ) - nDamage );

	// set ani
	if ( pMob->getAniType ( ) != eUnitAniType_Attack && pMob->getAniType ( ) != eUnitAniType_Special )
	{
		pMob->changeAni ( eUnitAniType_Damage );
	}
}

cUnit* cUnitCommanderCasino::getMyMob ( KDvoid )
{
	return m_aMobs [ m_nMyMobIndex ];
}

