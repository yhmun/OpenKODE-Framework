/* -----------------------------------------------------------------------------------
 *
 *      File            UnitCommanderTower.cpp
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
#include "UnitCommanderTower.h"
//#include "unitCreateInfos.h"

#if 0

cUnitCommanderTower::cUnitCommanderTower(Node* node, std::vector<cSprEffects*>& listEffects, const std::vector<cUnit*>& heros, const std::vector<cUnit*>& mobs)
: cUnitCommander(node, eModeType_End, listEffects, LAYER_TOWER_BATTLE_DAMAGE_1_1, LAYER_TOWER_BATTLE_DAMAGE_2_1, LAYER_TOWER_BATTLE_DAMAGE_3_1, LAYER_TOWER_BATTLE_STATUS_EFFECT_1, LAYER_TOWER_BATTLE_STATUS_EFFECT_2, LAYER_TOWER_BATTLE_STATUS_EFFECT_3)
, m_nSelectMob(-1)
{
	//game type
	m_eGameType = eGameType_Tower;

	//active gage
	initMobActiveGages( mobs.size() );

	//units
	for(int i = 0; i < (int)heros.size(); i++)	m_aHeros.push_back( heros[i] );
	for(int i = 0; i < (int)mobs .size(); i++)	m_aMobs .push_back( mobs [i] );

	//active sign icon
	for(int i = 0; i < (int)m_aHeros.size(); i++)
	{
		cSprite* pSprActiveSign = new cSprite( m_pNode, "active_mob_sign.png", LAYER_TOWER_BATTLE_UNIT_CUR, Point (0.5f, 0.5f), m_aHeros[i]->getPt() );
		
		pSprActiveSign->get ( )->setVisible ( false );
		
		m_aActiveHeroSigns.push_back(pSprActiveSign);
	}

	for(int i = 0; i < (int)m_aMobs.size(); i++)
	{
		cSprite* pSprActiveSign = new cSprite( m_pNode, "active_mob_sign.png", LAYER_TOWER_BATTLE_UNIT_CUR, Point (0.5f, 0.5f), m_aMobs[i]->getPt() );

		pSprActiveSign->get ( )->setVisible ( false );

		m_aActiveMobSigns.push_back(pSprActiveSign);
	}
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
cUnitCommanderTower::~cUnitCommanderTower()
{
	//active sign icon
	for(std::vector<cSprite*>::iterator node = m_aActiveHeroSigns.begin(); node != m_aActiveHeroSigns.end(); ++node)	*node = NULL;
	for(std::vector<cSprite*>::iterator node = m_aActiveMobSigns .begin(); node != m_aActiveMobSigns .end(); ++node)	*node = NULL;
	m_aActiveHeroSigns.clear();
	m_aActiveMobSigns .clear();
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
void cUnitCommanderTower::setSelectMob(int select)
{
	m_nSelectMob = select;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
void cUnitCommanderTower::exerciseSkill(KDint nUnitID, int skillType, int selectHeroID, int selectMobSeatIndex)
{
	addActive(eUnitType_Hero, unitID, -1, skillType, selectHeroID, selectMobSeatIndex);
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
void cUnitCommanderTower::setSignIconColor(int r, int g, int b)
{
	ccColor3B color;
	color.r = r;	color.g = g;	color.b = b;

	for(int i = 0; i < (int)m_aActiveHeroSigns.size(); i++)	m_aActiveHeroSigns[i]->get ( ).color = color;
	for(int i = 0; i < (int)m_aActiveMobSigns .size(); i++)	m_aActiveMobSigns [i]->get ( ).color = color;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::getIsActiveSkill(KDint nUnitID)
{
	for(int i = 0; i < getActiveMax(); i++)
	{
		sActive* pActive = getActive(i);

		if(pActive->eUnit == eUnitType_Hero && pActive->nUnitID == unitID)
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
void cUnitCommanderTower::update( KDdouble dLeftTickCount)
{
	//init
	bool isCreate = false;

	//active
	updateActive(dLeftTickCount);

	//super
	cUnitCommander::update(dLeftTickCount, isCreate);

	//active hero sign img
	for(int i = 0; i < (int)m_aActiveHeroSigns.size(); i++)
	{
		if( getIsActiveSkill( m_aHeros[i]->getUnitID() ) == false )
		{
			m_aActiveHeroSigns[i]->get ( ).opacity = 0;
			m_aActiveHeroSigns[i]->get ( )->setVisible ( false );
		}
		else
		{
			int opacity = m_aActiveHeroSigns[i]->get ( ).opacity + 15;
			if(opacity > 255)	opacity = 255;

			m_aActiveHeroSigns[i]->get ( )->setVisible ( true );
			m_aActiveHeroSigns[i]->get ( ).opacity = opacity;
		}
	}
	
	//active mob sign img
	for(int i = 0; i < (int)m_aActiveMobSigns.size(); i++)
	{
		if(getMobActiveGage(i) < 100)
		{
			m_aActiveMobSigns[i]->get ( ).opacity = 0;
			m_aActiveMobSigns[i]->get ( )->setVisible ( false );
		}
		else
		{
			int opacity = m_aActiveMobSigns[i]->get ( ).opacity + 15;
			if(opacity > 255)	opacity = 255;

			m_aActiveMobSigns[i]->get ( )->setVisible ( true );
			m_aActiveMobSigns[i]->get ( ).opacity = opacity;
		}
	}

	//action
	updateAction(dLeftTickCount);
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
void cUnitCommanderTower::updateActive( KDdouble dLeftTickCount)
{
	if(getIsGameEnd() == true)	return;

	//mobs------------------------------
	for(int i = 0; i < (int)m_aMobs.size(); i++)
	{
		//mob
		cUnit* pMob = m_aMobs[i];

		//check
		if(pMob->getDie() == true || m_aMobActiveGages[i] == 100)	continue;

		//gage
		double base = 7 * dLeftTickCount;
		double plus = pMob->get_use_agl() * (dLeftTickCount/6);
		m_aMobActiveGages[i] += base + plus;

		//max
		if(m_aMobActiveGages[i] >= 100)
		{
			m_aMobActiveGages[i] = 100;

			//add skill
			int heroFrontCount	= 0;
			int heroBackCount	= 0;
			int mobLine_1_Count = 0;
			int mobLine_2_Count = 0;
			int mobLine_3_Count = 0;
			getUnitLineCount(heroFrontCount, heroBackCount, mobLine_1_Count, mobLine_2_Count, mobLine_3_Count);

			bool isShort = true;
			bool isBack  = true;
			if(pMob->getMobLineIndex() == 1 && mobLine_1_Count > 0)
			{
				isShort = false;
				isBack  = false;
			}
			if( pMob->getMobLineIndex() == 2 && (mobLine_1_Count > 0 || mobLine_2_Count > 0) )
			{
				isShort = false;
				isBack  = false;
			}

			addActive( eUnitType_Mob, -1, i, pMob->getActionRandIndex(isShort, isBack), -1, -1 );
		}
	}
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
void cUnitCommanderTower::updateAction( KDdouble dLeftTickCount)
{
	//mobs------------------------------
	for(int i = 0; i < getActiveMax(); i++)
	{
		sActive* pActive = getActive(i);

		//get active unit
		cUnit* pUnit = NULL;

		if(pActive->eUnit == eUnitType_Hero)	pUnit = getUnit(pActive->nUnitID);
		else									pUnit = m_aMobs[pActive->nSeatIndex];

		//die check
		if(pUnit == NULL || pUnit->getDie() == true)
		{
			delActive(pActive->nUnitID, pActive->nSeatIndex);

			return;
		}

		//switch
		bool isEraseActive = false;

		//action
		while(true)
		{
			//new script
			cUnitScript* pScript = pUnit->getScript(pActive->nActionIndex, pActive->nScriptIndex);

			//모든 스크립트가 끝났다면 현재 액티브를 다은 몹에 넘겨준다.
			if(pScript == NULL)
			{
				//게이지를 초기화 한다.
				if(pActive->eUnit == eUnitType_Mob)	setMobActiveGageZero(pActive->nSeatIndex);

				//죽는것을 처리해준다.
				setHpZeroDie();

				//switch
				isEraseActive = true;

				break;
			}

			//첫번째 턴이라면 몹의 턴처리를 해준다.
			if(pActive->nScriptIndex == 0 && pActive->bIsActionStart == false)
			{
				//switch
				pActive->bIsActionStart = true;
			}

			//is exit init
			bool isExit = false;

			//processing
			switch( (int)pScript->type )
			{
			case eScriptType_Skill_rangeType	:	isExit = updateScriptRangeType	(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_AttackMotion		:	isExit = updateScriptAttackMotion (dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_SpecialMotion		:	isExit = updateScriptSpecialMotion(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_Damage				:	isExit = updateScriptDamage		(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_WaitEvent			:	isExit = updateScriptWaitEvent	(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_WaitTime			:	isExit = updateScriptWaitTime		(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_TargetClear		:	isExit = updateScriptTargetClear	(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_TargetMy			:	isExit = updateScriptTargetMy		(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_TargetOne			:	isExit = updateScriptTargetOne	(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_TargetAll			:	isExit = updateScriptTargetAll	(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_TargetRand			:	isExit = updateScriptTargetRand	(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_HP					:	isExit = updateScriptHP			(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_ATK				:	isExit = updateScriptATK			(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_DEF				:	isExit = updateScriptDEF			(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_AGL				:	isExit = updateScriptAGL			(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_CRT				:	isExit = updateScriptCRT			(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_Effect				:	isExit = updateScriptEffect		(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_EffectFlicker		:	isExit = updateScriptEffectFlicker(dLeftTickCount, pUnit, pActive, pScript );	break;
			case eScriptType_Time_Effect		:	isExit = updateScriptTimeEffect	(dLeftTickCount, pUnit, pActive, pScript );	break;
			}

			//check exit
			if(isExit == true)	break;

			//next script index
			++pActive->nScriptIndex;
		}

		//erase node
		if(isEraseActive == true)
		{
			delActive(pActive->nUnitID, pActive->nSeatIndex);
		}
	}
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptRangeType( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
		 if( (eRangeType)pScript->sub == eRangeType_Short )	pActive->eRange = eRangeType_Short;
	else if( (eRangeType)pScript->sub == eRangeType_Long  )	pActive->eRange = eRangeType_Long;	
	else if( (eRangeType)pScript->sub == eRangeType_Back  )	pActive->eRange = eRangeType_Back;	
	else													pActive->eRange = eRangeType_All;

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptAttackMotion( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	pUnit->changeAni(eUnitAniType_Attack);

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptSpecialMotion( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	pUnit->changeAni(eUnitAniType_Special);

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptDamage( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//heros
	for(std::vector<int>::iterator node = pActive->aTargetHeroIDs.begin(); node != pActive->aTargetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );
		if(pTargetHero == NULL)	continue;

		if(pTargetHero->getDie() == true)	continue;

		//para
		int atk	= pUnit		 ->get_use_atk();
		int crt	= pUnit		 ->get_use_crt();
		int def	= pTargetHero->get_use_def();

		//damage
		int damage	= atk - def;
		if(damage < 1)	damage = 1;

			 if(damage <  7)	damage += (random() % 3) - 1;
		else if(damage < 14)	damage += (random() % 5) - 2;
		else if(damage < 21)	damage += (random() % 7) - 3;
		else					damage += (random() % 9) - 4;

		if(damage < 1)	damage = 1;

		//critical
		bool isCri = false;
		if(random() % 100 <= crt)
		{
			isCri = true;
			damage *= 2;
		}

		//보정
		if( damage > pTargetHero->get_use_hp() )	damage = pTargetHero->get_use_hp();

		//damage label effect
		if(isCri == false)
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewDamageLabel(0, 0, pTargetHero->getPt(), damage);
			else														viewDamageLabel(1, 0, pTargetHero->getPt(), damage);
		}
		else
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewDamageLabel(0, 2, pTargetHero->getPt(), damage);
			else														viewDamageLabel(1, 2, pTargetHero->getPt(), damage);
		}

		//set hp
		pTargetHero->set_use_hp( pTargetHero->get_use_hp() - damage );

		//set ani
		pTargetHero->changeAni(eUnitAniType_Damage);
	}

	//mobs
	for(std::vector<int>::iterator node = pActive->aTargetMobIndexs.begin(); node != pActive->aTargetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob = m_aMobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		//para
		int atk	= pUnit		->get_use_atk();
		int crt	= pUnit		->get_use_crt();
		int def	= pTargetMob->get_use_def();

		//damage
		int damage	= atk - def;
		if(damage < 1)	damage = 1;

			 if(damage <  7)	damage += (random() % 3) - 1;
		else if(damage < 14)	damage += (random() % 5) - 2;
		else if(damage < 21)	damage += (random() % 7) - 3;
		else					damage += (random() % 9) - 4;

		if(damage < 1)	damage = 1;

		//critical
		bool isCri = false;
		if(random() % 100 <= crt)
		{
			isCri = true;
			damage *= 2;
		}

		//보정
		if( damage > pTargetMob->get_use_hp() )	damage = pTargetMob->get_use_hp();

		//damage label effect
		if(isCri == false)	viewDamageLabel(pTargetMob->getMobLineIndex(), 0, pTargetMob->getPt(), damage);
		else				viewDamageLabel(pTargetMob->getMobLineIndex(), 2, pTargetMob->getPt(), damage);

		//set hp
		pTargetMob->set_use_hp( pTargetMob->get_use_hp() - damage );

		//set ani
		pTargetMob->changeAni(eUnitAniType_Damage);
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptWaitEvent( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptWaitTime( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	if(pActive->dLeftTime == -1)
	{
		pActive->dLeftTime = pScript->sub / 1000;
	}
	else
	{
		pActive->dLeftTime -= dLeftTickCount;

		if(pActive->dLeftTime < 0)
		{
			pActive->dLeftTime = -1;

			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptTargetClear( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	pActive->aTargetHeroIDs	.clear();
	pActive->aTargetMobIndexs.clear();

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptTargetMy( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	if(pUnit->getUnitType() == eUnitType_Hero)	pActive->aTargetHeroIDs	.push_back( pUnit->getUnitID()	  );
	else										pActive->aTargetMobIndexs.push_back( pUnit->getSeatIndex() );

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptTargetOne( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//hero
	if(pUnit->getUnitType() == eUnitType_Hero)
	{
		if(pScript->sub == 0)	pActive->aTargetHeroIDs	.push_back(pActive->nSelectHeroID	  );
		else					pActive->aTargetMobIndexs.push_back(pActive->nSelectMobSeatIndex);
	}
	//mob
	else
	{
		//아군.
		if(pScript->sub == 0)
		{
			std::vector<int> randIndexs;
			for(std::vector<cUnit*>::iterator node = m_aMobs.begin(); node != m_aMobs.end(); ++node)
			{
				if( (* node)->getDie() == false && (* node)->getSeatIndex() != pUnit->getSeatIndex() )
				{
					randIndexs.push_back( (* node)->getSeatIndex() );
				}
			}

			if(randIndexs.size() > 0)
			{
				pActive->aTargetMobIndexs.push_back( randIndexs[ random() % randIndexs.size() ] );
			}
		}
		//적군.
		else
		{
			int heroFrontCount	= 0;
			int heroBackCount	= 0;
			int mobLine_1_Count = 0;
			int mobLine_2_Count = 0;
			int mobLine_3_Count = 0;
			getUnitLineCount(heroFrontCount, heroBackCount, mobLine_1_Count, mobLine_2_Count, mobLine_3_Count);

			std::vector<int> randIDs;
			for(std::vector<cUnit*>::iterator node = m_aHeros.begin(); node != m_aHeros.end(); ++node)
			{
				cUnit* pLoopUnit = *node;

				if( pLoopUnit->getDie() == true )	continue;

				if( pActive->eRange == eRangeType_Short || (pActive->eRange == eRangeType_Long && pUnit->getMobLineIndex() != 0) )
				{
					if(	 pLoopUnit->getPositionType() == ePositionType_Front || (pLoopUnit->getPositionType() == ePositionType_Back && heroFrontCount == 0 ) )
					{
						randIDs.push_back( (* node)->getUnitID() );
					}
				}
				else
				{
					randIDs.push_back( (* node)->getUnitID() );
				}
			}

			if(randIDs.size() > 0)
			{
				pActive->aTargetHeroIDs.push_back( randIDs[ random() % randIDs.size() ] );
			}
		}
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptTargetAll( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//hero
	if(pUnit->getUnitType() == eUnitType_Hero)
	{
		//아군.
		if(pScript->sub == 0)
		{
			for(std::vector<cUnit*>::iterator node = m_aHeros.begin(); node != m_aHeros.end(); ++node)
			{
				if( (* node)->getDie() == false && (* node)->getUnitID() != pUnit->getUnitID() )
				{
					pActive->aTargetHeroIDs.push_back( (* node)->getUnitID() );
				}
			}
		}
		//적군.
		else
		{
			for(std::vector<cUnit*>::iterator node = m_aMobs.begin(); node != m_aMobs.end(); ++node)
			{
				if( (* node)->getDie() == false )	pActive->aTargetMobIndexs.push_back( (* node)->getSeatIndex() );
			}
		}
	}
	//mob
	else
	{
		//아군.
		if(pScript->sub == 0)
		{
			for(std::vector<cUnit*>::iterator node = m_aMobs.begin(); node != m_aMobs.end(); ++node)
			{
				if( (* node)->getDie() == false && (* node)->getSeatIndex() != pUnit->getSeatIndex() )
				{
					pActive->aTargetMobIndexs.push_back( (* node)->getSeatIndex() );
				}
			}
		}
		//적군.
		else
		{
			for(std::vector<cUnit*>::iterator node = m_aHeros.begin(); node != m_aHeros.end(); ++node)
			{
				if( (* node)->getDie() == false )	pActive->aTargetHeroIDs.push_back( (* node)->getUnitID() );
			}
		}
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptTargetRand( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//hero
	if(pUnit->getUnitType() == eUnitType_Hero)
	{
	}
	//mob
	else
	{
		//아군.
		if(pScript->sub == 0)
		{
			std::vector<int> randIndexs;
			for(std::vector<cUnit*>::iterator node = m_aMobs.begin(); node != m_aMobs.end(); ++node)
			{
				if( (* node)->getDie() == false )
				{
					randIndexs.push_back( (* node)->getSeatIndex() );
				}
			}

			if(randIndexs.size() > 0)
			{
				pActive->aTargetMobIndexs.push_back( randIndexs[ random() % randIndexs.size() ] );
			}
		}
		//적군.
		else
		{
			int heroFrontCount	= 0;
			int heroBackCount	= 0;
			int mobLine_1_Count = 0;
			int mobLine_2_Count = 0;
			int mobLine_3_Count = 0;
			getUnitLineCount(heroFrontCount, heroBackCount, mobLine_1_Count, mobLine_2_Count, mobLine_3_Count);

			std::vector<int> randIDs;
			for(std::vector<cUnit*>::iterator node = m_aHeros.begin(); node != m_aHeros.end(); ++node)
			{
				cUnit* pLoopUnit = *node;

				if( pLoopUnit->getDie() == true )	continue;

				if( pActive->eRange == eRangeType_Short || (pActive->eRange == eRangeType_Long && pUnit->getMobLineIndex() != 0) )
				{
					if(	 pLoopUnit->getPositionType() == ePositionType_Front || (pLoopUnit->getPositionType() == ePositionType_Back && heroFrontCount == 0 ) )
					{
						randIDs.push_back( (* node)->getUnitID() );
					}
				}
				else
				{
					randIDs.push_back( (* node)->getUnitID() );
				}
			}

			if(randIDs.size() > 0)
			{
				pActive->aTargetHeroIDs.push_back( randIDs[ random() % randIDs.size() ] );
			}
		}
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptHP( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//heros
	for(std::vector<int>::iterator node = pActive->aTargetHeroIDs.begin(); node != pActive->aTargetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );

		if(pTargetHero == NULL || pTargetHero->getDie() == true)	continue;

		//hp
		double	time  = (float)( (int)pScript->sub % 10000 ) / 1000;
		int		count = (int)pScript->sub / 10000;
		int		hp	  = atoi( pScript->str.c_str() );

		pTargetHero->set_use_hp_add( time, count, hp, pUnit->getSeatIndex() );

		//status effect
		if(hp > 0)
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_Regene, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_Regene, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
		else
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_poison, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_poison, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
	}

	//mobs
	for(std::vector<int>::iterator node = pActive->aTargetMobIndexs.begin(); node != pActive->aTargetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob = m_aMobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		//hp
		double	time  = (float)( (int)pScript->sub % 10000 ) / 1000;
		int		count = (int)pScript->sub / 10000;
		int		hp	  = atoi( pScript->str.c_str() );

		pTargetMob->set_use_hp_add( time, count, hp, pUnit->getSeatIndex() );

		//status effect
		if(hp > 0)	viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_Regene, cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
		else		viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_poison, cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptATK( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//heros
	for(std::vector<int>::iterator node = pActive->aTargetHeroIDs.begin(); node != pActive->aTargetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );

		if(pTargetHero == NULL || pTargetHero->getDie() == true)	continue;

		//atk
		double	time = pScript->sub / 1000;
		int		atk	 = atoi( pScript->str.c_str() );

		pTargetHero->set_use_atk_add(time, atk);

		//status effect
		if(atk > 0)
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_AtkUp, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_AtkUp, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
		else
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_AtkDown, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_AtkDown, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
	}

	//mobs
	for(std::vector<int>::iterator node = pActive->aTargetMobIndexs.begin(); node != pActive->aTargetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob = m_aMobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		//atk
		double	time = pScript->sub / 1000;
		int		atk	 = atoi( pScript->str.c_str() );

		pTargetMob->set_use_atk_add(time, atk);

		//status effect
		if(atk > 0)	viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_AtkUp  , cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
		else		viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_AtkDown, cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptDEF( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//heros
	for(std::vector<int>::iterator node = pActive->aTargetHeroIDs.begin(); node != pActive->aTargetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );
		if(pTargetHero == NULL)	continue;

		if(pTargetHero == NULL || pTargetHero->getDie() == true)	continue;

		//def
		double	time = pScript->sub / 1000;
		int		def	 = atoi( pScript->str.c_str() );

		pTargetHero->set_use_def_add(time, def);

		//status effect
		if(def > 0)
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_DefUp, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_DefUp, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
		else
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_DefDown, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_DefDown, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
	}

	//mobs
	for(std::vector<int>::iterator node = pActive->aTargetMobIndexs.begin(); node != pActive->aTargetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob = m_aMobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		//def
		double	time = pScript->sub / 1000;
		int		def	 = atoi( pScript->str.c_str() );

		pTargetMob->set_use_def_add(time, def);

		//status effect
		if(def > 0)	viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_DefUp  , cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
		else		viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_DefDown, cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptAGL( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//heros
	for(std::vector<int>::iterator node = pActive->aTargetHeroIDs.begin(); node != pActive->aTargetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );

		if(pTargetHero == NULL || pTargetHero->getDie() == true)	continue;

		//agl
		double	time = pScript->sub / 1000;
		int		agl	 = atoi( pScript->str.c_str() );

		pTargetHero->set_use_agl_add(time, agl);

		//status effect
		if(agl > 0)
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_AglUp, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_AglUp, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
		else
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_AglDown, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_AglDown, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
	}

	//mobs
	for(std::vector<int>::iterator node = pActive->aTargetMobIndexs.begin(); node != pActive->aTargetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob = m_aMobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		//agl
		double	time = pScript->sub / 1000;
		int		agl	 = atoi( pScript->str.c_str() );

		pTargetMob->set_use_agl_add(time, agl);

		//status effect
		if(agl > 0)	viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_AglUp  , cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
		else		viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_AglDown, cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptCRT( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//heros
	for(std::vector<int>::iterator node = pActive->aTargetHeroIDs.begin(); node != pActive->aTargetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );

		if(pTargetHero == NULL || pTargetHero->getDie() == true)	continue;

		//crt
		double	time = pScript->sub / 1000;
		int		crt	 = atoi( pScript->str.c_str() );

		pTargetHero->set_use_crt_add(time, crt);

		//status effect
		if(crt > 0)
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_CrtUp, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_CrtUp, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
		else
		{
			if(pTargetHero->getPositionType() == ePositionType_Front)	viewStatusEffect( 0, eStatusEffect_CrtDown, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
			else														viewStatusEffect( 1, eStatusEffect_CrtDown, cpvadd( pTargetHero->getPt(), pTargetHero->getPtSpecial() ) );
		}
	}

	//mobs
	for(std::vector<int>::iterator node = pActive->aTargetMobIndexs.begin(); node != pActive->aTargetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob = m_aMobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		//crt
		double	time = pScript->sub / 1000;
		int		crt	 = atoi( pScript->str.c_str() );

		pTargetMob->set_use_crt_add(time, crt);

		//status effect
		if(crt > 0)	viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_CrtUp  , cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
		else		viewStatusEffect( pTargetMob->getMobLineIndex(), eStatusEffect_CrtDown, cpvadd( pTargetMob->getPt(), pTargetMob->getPtSpecial() ) );
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptEffect( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//heros
	for(std::vector<int>::iterator node = pActive->aTargetHeroIDs.begin(); node != pActive->aTargetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );

		if(pTargetHero == NULL || pTargetHero->getDie() == true)	continue;

		//point
		Point addPt = Point::ZERO;

		if(pScript->sub == 0)	addPt = Point ( 0, (pTargetHero->getNorFirstSize().height/2) );
		else					addPt = pTargetHero->getPtSpecial();

		//effect
		if(pTargetHero->getPositionType() == ePositionType_Front)	viewEffect(0, pScript->str.c_str(), cpvadd( pTargetHero->getPt(), addPt) );
		else														viewEffect(1, pScript->str.c_str(), cpvadd( pTargetHero->getPt(), addPt) );
	}

	//mobs
	for(std::vector<int>::iterator node = pActive->aTargetMobIndexs.begin(); node != pActive->aTargetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob	= m_aMobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		//point
		Point addPt = Point::ZERO;

		if(pScript->sub == 0)	addPt = Point ( 0, (pTargetMob->getNorFirstSize().height/2) );
		else					addPt = pTargetMob->getPtSpecial();

		//effect
		viewEffect(pTargetMob->getMobLineIndex(), pScript->str.c_str(), cpvadd( pTargetMob->getPt(), addPt) );
	}

	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptEffectFlicker( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
/*
	//heros
	for(std::vector<int>::iterator node = pActive->targetHeroIDs.begin(); node != pActive->targetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );

		if(pTargetHero == NULL || pTargetHero->getDie() == true)	continue;

		pTargetHero->setFlicker(pScript->sub / 1000);
	}

	//mob
	for(std::vector<int>::iterator node = pActive->targetMobIndexs.begin(); node != pActive->targetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob	= m_mobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		pTargetMob->setFlicker(pScript->sub / 1000);
	}
*/
	return false;
}

//-----------------------------------------------------------
// Name : JINE(조진희)
// Dest : 
//-----------------------------------------------------------
bool cUnitCommanderTower::updateScriptTimeEffect( KDdouble dLeftTickCount, cUnit* pUnit, sActive* pActive, const cUnitScript* pScript )
{
	//heros
	for(std::vector<int>::iterator node = pActive->aTargetHeroIDs.begin(); node != pActive->aTargetHeroIDs.end(); ++node)
	{
		cUnit* pTargetHero = getUnit( *node );
		
		if(pTargetHero == NULL || pTargetHero->getDie() == true)	continue;

		sTimeEffect timeEffect;

		timeEffect.eUnit		= eUnitType_Hero;
		timeEffect.nUnitID		= pTargetHero->getUnitID();
		timeEffect.nSeatIndex	= -1;
		timeEffect.sEffectName	= pScript->str;
		timeEffect.nLeftCount	= (int)pScript->sub / 10000;
		timeEffect.dTime			= (float)( (int)pScript->sub % 10000 ) / 1000;
		timeEffect.dLeftTime		= timeEffect.dTime;

		//effect index
		if(pTargetHero->getPositionType() == ePositionType_Front)	timeEffect.nEffectIndex = 0;
		else														timeEffect.nEffectIndex = 1;

		//point
		Point addPt = Point ( 0, (pTargetHero->getNorFirstSize().height/2) );
		timeEffect.tPoint = cpvadd( pTargetHero->getPt(), addPt);
		
		//add
		viewTimeEffect(timeEffect);
	}
	
	//mobs
	for(std::vector<int>::iterator node = pActive->aTargetMobIndexs.begin(); node != pActive->aTargetMobIndexs.end(); ++node)
	{
		cUnit* pTargetMob	= m_aMobs[ *node ];

		if(pTargetMob->getDie() == true)	continue;

		sTimeEffect timeEffect;

		timeEffect.eUnit		= eUnitType_Mob;
		timeEffect.nUnitID		= -1;
		timeEffect.nSeatIndex	= pTargetMob->getSeatIndex();
		timeEffect.nEffectIndex	= pTargetMob->getMobLineIndex();
		timeEffect.sEffectName	= pScript->str;
		timeEffect.nLeftCount	= (int)pScript->sub / 10000;
		timeEffect.dTime			= (float)( (int)pScript->sub % 10000 ) / 1000;
		timeEffect.dLeftTime		= timeEffect.dTime;

		//point
		Point addPt = Point ( 0, (pTargetMob->getNorFirstSize().height/2) );
		timeEffect.tPoint = cpvadd( pTargetMob->getPt(), addPt);

		//add
		viewTimeEffect(timeEffect);
	}

	return false;
}

#endif	// 0