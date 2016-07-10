/* -----------------------------------------------------------------------------------
 *
 *      File            UnitCreateInfos.cpp
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
#include "UnitCreateInfos.h"
#include "MyData/MyData.h"

#define _UNIT_NAME_MAX		30
#define _MAX_PATH			260

static const KDchar*		l_aCharacterDir [ ]
{
	"Cha1_Warrior"			,
	"Cha2_Magician"			,
	"Cha3_Barbarian"		,
	"Cha4_Priest"			,
	"Cha5_SwordSoldier"		,
	"Cha6_FemaleGunner"		,
	"Cha7_GunGirl"			,
	"Cha8_Wizard"			,
	"Cha9_SpaceSoldier"		,
	"Cha10_MobileKnight"	,
	"Cha11_BigFighter"		,
	"Cha12_SpaceWarrior"	,
	"Cha13_Protector"		,
	"Cha14_SaintWarrior"	,
	"Cha15_MagicMusician"	,
	"Cha16_Knightmare"		,
	"Cha17_Doly"			,
	"Cha18_BardGirl"		,
	"Cha19_Rink"			,
	"Cha20_Spidahmane"		,
	"Cha21_Zerocult5050"	,
	"Cha22_Kswizze"			,
	"Cha23_FantasyDreams"	,
	"Cha24_Molverine"		,
	"Cha25_HumanSpider"		,	
	"Cha26_Crusader"		,
	"Cha27_MPTK"			,
	"Cha28_MagicGunGirl"	,
	"Cha29_Hulkman"			,
	"Cha30_Omlet"			,
	"Cha31_Xianfeng1981"	,
	"Cha32_Fukuizumi"		,
	"Cha33_BatGuy"			,
	"Cha34_Louis0728"		,
};

cUnitCreateInfos::cUnitCreateInfos ( std::string sFileName )
: m_nAllChance ( 0 )
{
	KDubyte*	pData = nullptr;
	KDint		nSize = 0;
	KDint		nSeek = 0;

	pData = FileUtils::getInstance ( )->getFileData ( sFileName.c_str ( ), "rb", &nSize );

	// monsters
	KDint		nMonsterCount = 0;
	cMyData::readToData ( pData, nSize, nSeek, &nMonsterCount, sizeof ( KDint ) );
	for ( KDint i = 0; i < nMonsterCount; i++ )
	{
		// monster
		cUnitCreateInfo*	pUnit = new cUnitCreateInfo ( i );

		// name
		KDint				nUnitNameSize = 0;
		KDchar				szUnitName [ _UNIT_NAME_MAX + 1 ] = { 0, };
		cMyData::readToData ( pData, nSize, nSeek, &nUnitNameSize, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szUnitName	 , sizeof ( KDchar ) * nUnitNameSize );
		pUnit->sName = szUnitName;

		// chance
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->nChance, sizeof ( KDint ) );

		// sprite file name
		KDint				nSpriteFileNameLen = 0;
		KDchar				szSpriteFileName [ _MAX_PATH + 1 ] = { 0, };
		cMyData::readToData ( pData, nSize, nSeek, &nSpriteFileNameLen, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szSpriteFileName	  , sizeof ( KDchar ) * nSpriteFileNameLen );
		pUnit->sSpriteFileName = szSpriteFileName;

		std::string			sDir = szSpriteFileName;
		KDint				nDirNum = 0;
		KDchar				szDirName [256];
		sDir = sDir.substr ( 0, sDir.size ( ) - 4 );
		sDir = sDir.substr ( sDir.rfind ( "_" ) + 1 );
		nDirNum = kdAtoi ( sDir.c_str ( ) );

		if ( !kdStrncmp ( "monster", szSpriteFileName, 7 ) )
		{			
			kdSprintf ( szDirName, "Mobs/%d/", nDirNum );
			
		}
		else
		{
			kdSprintf ( szDirName, "Heros/%s/", l_aCharacterDir [ nDirNum - 1 ] );
		}
		pUnit->sSpriteFileDir = szDirName; 
		CCLOG ( "%s, %s%s", szUnitName, szDirName, szSpriteFileName );

		// detail
		KDint				nDetailLen = 0;
		KDchar				szStrDetail [ _MAX_PATH + 1 ] = { 0, };
		cMyData::readToData ( pData, nSize, nSeek, &nDetailLen, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szStrDetail, sizeof ( KDchar ) * nDetailLen );
		pUnit->sStrDetail = szStrDetail;

		// skill explan
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->nSkillIcon1, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->nSkillIcon2, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->nSkillIcon3, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->nSkillIcon4, sizeof ( KDint ) );

		KDint				nSkillStr1Len = 0;
		KDchar				szStrSkill1 [ _MAX_PATH + 1 ] = { 0, };
		cMyData::readToData ( pData, nSize, nSeek, &nSkillStr1Len, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szStrSkill1	 , sizeof ( KDchar ) * nSkillStr1Len );
		pUnit->sSkillStr1 = szStrSkill1;

		KDint				nSkillStr2Len = 0;
		KDchar				szStrSkill2 [ _MAX_PATH + 1 ] = { 0, };
		cMyData::readToData ( pData, nSize, nSeek, &nSkillStr2Len, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szStrSkill2	 , sizeof ( KDchar ) * nSkillStr2Len );
		pUnit->sSkillStr2 = szStrSkill2;

		KDint				nSkillStr3Len = 0;
		KDchar				szStrSkill3[_MAX_PATH+1] = {0, };
		cMyData::readToData ( pData, nSize, nSeek, &nSkillStr3Len, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szStrSkill3	 , sizeof ( KDchar ) * nSkillStr3Len );
		pUnit->sSkillStr3 = szStrSkill3;
		
		KDint				nSkillStr4Len = 0;
		KDchar				szStrSkill4[_MAX_PATH+1] = {0, };
		cMyData::readToData ( pData, nSize, nSeek, &nSkillStr4Len, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, szStrSkill4	 , sizeof ( KDchar ) * nSkillStr4Len );
		pUnit->sSkillStr4 = szStrSkill4;

		// parameters
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aHp  [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aHp  [ 1 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aAtk [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aAtk [ 1 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aDef [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aDef [ 1 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aAgl [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aAgl [ 1 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aCrt [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aCrt [ 1 ], sizeof ( KDint ) );

		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpHp  [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpHp  [ 1 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpAtk [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpAtk [ 1 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpDef [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpDef [ 1 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpAgl [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpAgl [ 1 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpCrt [ 0 ], sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->aLvUpCrt [ 1 ], sizeof ( KDint ) );

		// special point
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->tPointSpecialX, sizeof ( KDint ) );
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->tPointSpecialY, sizeof ( KDint ) );

		// casino advantage
		cMyData::readToData ( pData, nSize, nSeek, &pUnit->nCasinoAdvantageORuniqueType, sizeof ( KDint ) );

		// actions
		KDint				nActionCount = 0;
		cMyData::readToData ( pData, nSize, nSeek, &nActionCount, sizeof ( KDint ) );
		for ( KDint a = 0; a < nActionCount; a++ )
		{
			// action
			cUnitAction*	pAction = new cUnitAction ( );

			// aScripts
			KDint			nScriptCount = 0;
			cMyData::readToData ( pData, nSize, nSeek, &nScriptCount, sizeof ( KDint ) );
			for ( KDint z = 0; z < nScriptCount; z++ )
			{
				// script
				cUnitScript*	pScript = new cUnitScript ( );

				// type
				cMyData::readToData ( pData, nSize, nSeek, &pScript->eType, sizeof ( KDint ) );

				// sub
				cMyData::readToData ( pData, nSize, nSeek, &pScript->fSub, sizeof ( KDfloat ) );

				// str
				KDint		nStrLen = 0;
				KDchar		szStr [ _MAX_PATH + 1 ] = { 0, };
				cMyData::readToData ( pData, nSize, nSeek, &nStrLen, sizeof ( KDint ) );
				cMyData::readToData ( pData, nSize, nSeek, szStr   , sizeof ( KDchar ) * nStrLen );
				pScript->sStr = szStr;

				// script push
				pAction->aScripts.push_back ( pScript );
			}

			// action push
			pUnit->aActions.push_back ( pAction );
		}

		// monster push
		m_aUnits.push_back ( pUnit );
	}

	// data delete
	CC_SAFE_FREE ( pData );

	// all chance
	for ( auto pUnit : m_aUnits )
	{
		m_nAllChance += pUnit->nChance;
	}
}

cUnitCreateInfos::~cUnitCreateInfos ( KDvoid )
{
	for ( auto pUnit : m_aUnits )
	{
		CC_SAFE_RELEASE ( pUnit );
	}
	m_aUnits.clear ( );
}

const cUnitCreateInfo* cUnitCreateInfos::getUnitInfo ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aUnits.size ( ) )
	{
		return nullptr;
	}

	return m_aUnits [ nIndex ];
}

const cUnitCreateInfo* cUnitCreateInfos::getUnitInfo ( std::string sName )
{
	for ( auto pUnit : m_aUnits )
	{
		if ( pUnit->sName.compare ( sName ) == 0 )
		{
			return pUnit;
		}
	}

	return nullptr;
}

const cUnitCreateInfo* cUnitCreateInfos::getUnitInfoRand ( KDvoid )
{
	KDint		nChanceCount = 0;
	KDint		nRand = kdRand ( ) % m_nAllChance;

	for ( auto pUnit : m_aUnits )
	{
		if ( nRand >= nChanceCount && nRand < nChanceCount + pUnit->nChance )
		{
			return pUnit;
		}

		nChanceCount += pUnit->nChance;
	}

	cocos2d::log ( "cUnitCreateInfos overflow!!!" );

	return nullptr;
}