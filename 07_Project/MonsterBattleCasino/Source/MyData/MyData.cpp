/* -----------------------------------------------------------------------------------
 *
 *      File            MyData.cpp
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
#include "MyData.h"
#include "Unit/Unit/Unit.h"

#define _SECRET_CODE_1_VERSION_1_0		1798428082
#define _SECRET_CODE_1_VERSION_1_1		1654681231
#define _SECRET_CODE_2					1489152434
#define _SECRET_CODE_3					1158948469
#define _SECRET_CODE_4					1486588498
#define _SECRET_CODE_5					1731115564
#define _SECRET_CODE_6					1189157357

#define _DUMMY_1_COUNT					37
#define _DUMMY_2_COUNT					12
#define _DUMMY_3_COUNT					14
#define _DUMMY_4_COUNT					3
#define _DUMMY_5_COUNT					22
#define _DUMMY_6_COUNT					25
#define _DUMMY_7_COUNT					6
#define _DUMMY_8_COUNT					32
#define _DUMMY_9_COUNT					1
#define _DUMMY_10_COUNT					7
#define _DUMMY_11_COUNT					19
#define _DUMMY_12_COUNT					15
#define _DUMMY_13_COUNT					26
#define _DUMMY_14_COUNT					9
#define _DUMMY_15_COUNT					14
#define _DUMMY_16_COUNT					10
#define _DUMMY_17_COUNT					8
#define _DUMMY_18_COUNT					5
#define _DUMMY_19_COUNT					19
#define _DUMMY_20_COUNT					25
#define _DUMMY_21_COUNT					4
#define _DUMMY_22_COUNT					1
#define _DUMMY_23_COUNT					9
#define _DUMMY_24_COUNT					17
#define _DUMMY_25_COUNT					13
#define _DUMMY_26_COUNT					24
#define _DUMMY_27_COUNT					29
#define _DUMMY_28_COUNT					21
#define _DUMMY_29_COUNT					8
#define _DUMMY_30_COUNT					28
#define _DUMMY_31_COUNT					12
#define _DUMMY_32_COUNT					5
#define _DUMMY_33_COUNT					18
#define _DUMMY_34_COUNT					4
#define _DUMMY_35_COUNT					12
#define _DUMMY_36_COUNT					17


static cMyData*			l_pMyData = nullptr;

inline static KDvoid	appendBytes ( std::vector<KDubyte>& rOut, KDvoid* pData, KDsize uSize )
{
	const KDubyte*	pBytes = (const KDubyte*) pData;
	for ( KDuint i = 0; i < uSize; i++ )
	{		
		rOut.push_back ( pBytes [ i ] );
	}
}

cMyData::cMyData ( KDvoid )
: m_nConstInitMoney			( 1000 )
, m_nMoney					( m_nConstInitMoney )
, m_nBettingMoney			( 0 )
, m_nComebackCoin			( 0 )
, m_nPotion					( 0 )
, m_bIsOptionSound			( 1 )
, m_nSlotCount				( 6 )
, m_bIsFirstIntoMyRoom		( 1 )
, m_nMyRoomPage				( 0 )
, m_nSuccessive				( 0 )
, m_nGameCount				( 0 )
, m_nLoseCount				( 0 )
, m_nOpenTime				( 0 )
, m_dMachine1BuyLeftTime	( 600 )
, m_nNewHeroID				( 0 )
, m_nNewMyRoomUnitID		( 0 )
{
	// static
	l_pMyData = this;

	// file load
	load ( );

	// offline coin
	if ( m_nOpenTime != 0 )
	{
		KDTimeval	tNowTime;
		kdGettimeofday ( &tNowTime, nullptr );
		
		KDint		nGapTime = kdAbs ( tNowTime.tv_sec - m_nOpenTime );

		// comeback coin
		m_nComebackCoin = nGapTime / 60;
		if ( m_nComebackCoin > 100 )
		{
			m_nComebackCoin = 100;
		}

		// machine 1 time
		m_dMachine1BuyLeftTime -= nGapTime;
		if ( m_dMachine1BuyLeftTime < 0 )
		{
			m_dMachine1BuyLeftTime = 0;
		}

		// my room unit data exp
		for ( auto& rMyRoomUnitSaveData : m_aMyRoomUnitSaveDatas )
		{
			// exp
				 if ( rMyRoomUnitSaveData.fFriendshipExp > FRIENDSHIP_LOVE )	rMyRoomUnitSaveData.fExp += nGapTime * _EXP_UP_OFFLINE_WORST;
			else if ( rMyRoomUnitSaveData.fFriendshipExp > FRIENDSHIP_BEST )	rMyRoomUnitSaveData.fExp += nGapTime * _EXP_UP_OFFLINE_BEST;
			else if ( rMyRoomUnitSaveData.fFriendshipExp > FRIENDSHIP_GOOD )	rMyRoomUnitSaveData.fExp += nGapTime * _EXP_UP_OFFLINE_GOOD;
			else if ( rMyRoomUnitSaveData.fFriendshipExp > FRIENDSHIP_BAD  )	rMyRoomUnitSaveData.fExp += nGapTime * _EXP_UP_OFFLINE_BAD;
			else																rMyRoomUnitSaveData.fExp += nGapTime * _EXP_UP_OFFLINE_WORST;

			// friendship
			rMyRoomUnitSaveData.fFriendshipExp -= nGapTime * _FRIENDSHIP_DOWN_OFFLINE_DOWN;
			if ( rMyRoomUnitSaveData.fFriendshipExp < 0 )	rMyRoomUnitSaveData.fFriendshipExp = 0;
		}
	}

	if ( m_nComebackCoin > 100 )	m_nComebackCoin = 100;
	m_nMoney += m_nComebackCoin;

	// save
	save ( );
}

cMyData::~cMyData ( KDvoid )
{
	// hero save data
	m_aHeroSaveDatas.clear ( );

	// my room unit save data
	m_aMyRoomUnitSaveDatas.clear ( );

	// bet mob
	clearBetMobData ( );
}

cMyData* cMyData::getObj ( KDvoid )
{
	if ( !l_pMyData )
	{
		l_pMyData = new cMyData ( );
		l_pMyData->preLoadSounds ( );
	}

	return l_pMyData;
}

KDvoid cMyData::init ( KDvoid )
{
	m_nMoney			= m_nConstInitMoney;
	m_nComebackCoin		= 0;
	m_nPotion			= 0;
	m_bIsOptionSound	= 1;
	m_nSuccessive		= 0;
	m_nGameCount		= 0;
	m_nLoseCount		= 0;
	m_nOpenTime			= 0;
	m_nNewHeroID		= 0;
	m_nNewMyRoomUnitID	= 0;

	clearBetMobData ( );
}

KDvoid cMyData::preLoadSounds ( KDvoid ) 
{
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "battlestart.mp3"			);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "cut.mp3"					);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "hit.mp3"					);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "bite.mp3"					);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "nail.mp3"					);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "monster_dead.mp3"			);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "bet_move.mp3"				);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "button_select.mp3"			);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "casino_result_coin_2.mp3"	);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "casino_result_win.mp3"		);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "casino_result_coin_1.mp3"	);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "button_select.mp3"			);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "soulBead_break.mp3"		);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "unitDelete.mp3"			);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "capsuleBomb.mp3"			);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "capsule_buy_not.mp3"		);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "capsule_rare_unique.mp3"	);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "machine_out_capsule.mp3"	);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "machine_capsule_shake.mp3"	);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "machine_handle_rotate.mp3"	);
	SimpleAudioEngine::getInstance ( )->preloadEffect ( "machine_into_coin.mp3"		);
}

KDvoid cMyData::addHeroSaveDatas ( cHeroSaveData& rSaveData )
{
	m_aHeroSaveDatas.push_back ( rSaveData );

	save ( );
}

KDvoid cMyData::updateHeroSaveDatas ( std::vector<cUnit*>& rHeros )
{
	for ( auto pHero : rHeros )
	{
		KDbool	bIsCopy = false;

		for ( auto& rHeroSaveData : m_aHeroSaveDatas )
		{
			if ( pHero->getUnitID ( ) == rHeroSaveData.nHeroID )
			{
				rHeroSaveData.nHeroID		= pHero->getUnitID			( );
				rHeroSaveData.nCreateIndex	= pHero->getCreateIndex		( );
				rHeroSaveData.nSeatIndex	= pHero->getSeatIndex		( );
				rHeroSaveData.nPositionType	= pHero->getPositionType	( );
				rHeroSaveData.nLevel		= pHero->getLevel			( );
				rHeroSaveData.nExp			= pHero->getExp				( );
				rHeroSaveData.nHp			= pHero->getHP				( );
				rHeroSaveData.nUseHp		= pHero->getUseHp			( );
				rHeroSaveData.nAtk			= pHero->getATK				( );
				rHeroSaveData.nDef			= pHero->getDEF				( );
				rHeroSaveData.nAgl			= pHero->getAGL				( );
				rHeroSaveData.nCrt			= pHero->getCRT				( );

				rHeroSaveData.aSkills.clear ( );
				for ( KDint z = 0; z < pHero->getSkillCount ( ); z++ )
				{
					rHeroSaveData.aSkills.push_back ( pHero->getSkillType ( z ) );
				}

				bIsCopy = true;
				break;
			}
		}

		if ( bIsCopy == false )
		{
			cHeroSaveData	tSaveData;

			tSaveData.nHeroID		= pHero->getUnitID		 ( );
			tSaveData.nCreateIndex	= pHero->getCreateIndex	 ( );
			tSaveData.nSeatIndex	= pHero->getSeatIndex	 ( );
			tSaveData.nPositionType	= pHero->getPositionType ( );
			tSaveData.nLevel		= pHero->getLevel		 ( );
			tSaveData.nExp			= pHero->getExp			 ( );
			tSaveData.nHp			= pHero->getHP			 ( );
			tSaveData.nUseHp		= pHero->getUseHp		 ( );
			tSaveData.nAtk			= pHero->getATK			 ( );
			tSaveData.nDef			= pHero->getDEF			 ( );
			tSaveData.nAgl			= pHero->getAGL			 ( );
			tSaveData.nCrt			= pHero->getCRT			 ( );

			tSaveData.aSkills.clear ( );
			for ( KDint a = 0; a < pHero->getSkillCount ( ); a++ )
			{
				tSaveData.aSkills.push_back ( pHero->getSkillType ( a ) );
			}

			m_aHeroSaveDatas.push_back ( tSaveData );
		}
	}

	save ( );
}

KDvoid cMyData::addMyRoomUnitSaveDatas ( cMyRoomUnitSaveData& rSaveData )
{
	m_aMyRoomUnitSaveDatas.push_back ( rSaveData );

	save ( );
}

KDvoid cMyData::delMyRoomUnitSaveDatas ( cUnit* pUnit )
{
	for ( std::vector<cMyRoomUnitSaveData>::iterator it = m_aMyRoomUnitSaveDatas.begin ( ); it != m_aMyRoomUnitSaveDatas.end ( ); ++it )
	{
		if ( ( *it ).nUnitID == pUnit->getUnitID ( ) )
		{
			m_aMyRoomUnitSaveDatas.erase ( it );
			break;
		}
	}

	save ( );
}

KDvoid cMyData::updateMyRoomUnitSaveDatas ( std::vector<cUnit*>& rUnits )
{
	for ( auto pUnit : rUnits )
	{
		if ( pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_1    && pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_2	&&
			 pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_3	   && pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_4	&&
			 pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_5	   && pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_6	&&
			 pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_7	   && pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_8	&& 
			 pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_9	   && pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_10	&&
			 pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_11   && pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_12	&&
			 pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Coin_13   && pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Premium_1	&&
			 pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Premium_2 && pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Premium_3	&&
			 pUnit->getCreateInfoFileType ( ) != eCreateInfoFileType_Premium_4 )	
		{
			continue;
		}

		KDbool		bIsCopy = false;

		for ( auto& rMyRoomUnitSaveData : m_aMyRoomUnitSaveDatas )
		{
			if ( pUnit->getUnitID ( ) == rMyRoomUnitSaveData.nUnitID )
			{
				rMyRoomUnitSaveData.nCreateInfoFileType = pUnit->getCreateInfoFileType	( );
				rMyRoomUnitSaveData.nUnitID				= pUnit->getUnitID				( );
				rMyRoomUnitSaveData.nCreateIndex		= pUnit->getCreateIndex			( );
				rMyRoomUnitSaveData.nSlotIndex			= pUnit->getSlotIndex			( );
				rMyRoomUnitSaveData.aHp  [ 0 ]			= pUnit->getHP					( );
				rMyRoomUnitSaveData.aHp  [ 1 ]			= pUnit->getHPMax				( );
				rMyRoomUnitSaveData.aAtk [ 0 ]			= pUnit->getATK					( );
				rMyRoomUnitSaveData.aAtk [ 1 ]			= pUnit->getATKMax				( );
				rMyRoomUnitSaveData.aDef [ 0 ]			= pUnit->getDEF					( );
				rMyRoomUnitSaveData.aDef [ 1 ]			= pUnit->getDEFMax				( );
				rMyRoomUnitSaveData.aAgl [ 0 ]			= pUnit->getAGL					( );
				rMyRoomUnitSaveData.aAgl [ 1 ]			= pUnit->getAGLMax				( );
				rMyRoomUnitSaveData.aCrt [ 0 ]			= pUnit->getCRT					( );
				rMyRoomUnitSaveData.aCrt [ 1 ]			= pUnit->getCRTMax				( );
				rMyRoomUnitSaveData.fClassExp			= pUnit->getClassExp			( );
				rMyRoomUnitSaveData.fFriendshipExp		= pUnit->getFriendShipExp		( );
				rMyRoomUnitSaveData.nKillCount			= pUnit->getKillCount			( );
				rMyRoomUnitSaveData.fExp				= pUnit->getExp					( );
				rMyRoomUnitSaveData.nIsArcadeEasyClear	= pUnit->getIsArcadeEasyClear	( );
				rMyRoomUnitSaveData.nIsArcadeNorClear	= pUnit->getIsArcadeNorClear	( );
				rMyRoomUnitSaveData.nIsArcadeHardClear	= pUnit->getIsArcadeHardClear	( );

				bIsCopy = true;
				break;
			}
		}

		if ( bIsCopy == false )
		{
			cMyRoomUnitSaveData			tSaveData;

			tSaveData.nCreateInfoFileType	= pUnit->getCreateInfoFileType	( );
			tSaveData.nUnitID				= pUnit->getUnitID				( );
			tSaveData.nCreateIndex			= pUnit->getCreateIndex			( );
			tSaveData.nSlotIndex			= pUnit->getSlotIndex			( );
			tSaveData.aHp  [ 0 ]			= pUnit->getHP					( );
			tSaveData.aHp  [ 1 ]			= pUnit->getHPMax				( );
			tSaveData.aAtk [ 0 ]			= pUnit->getATK					( );
			tSaveData.aAtk [ 1 ]			= pUnit->getATKMax				( );
			tSaveData.aDef [ 0 ]			= pUnit->getDEF					( );
			tSaveData.aDef [ 1 ]			= pUnit->getDEFMax				( );
			tSaveData.aAgl [ 0 ]			= pUnit->getAGL					( );
			tSaveData.aAgl [ 1 ]			= pUnit->getAGLMax				( );
			tSaveData.aCrt [ 0 ]			= pUnit->getCRT					( );
			tSaveData.aCrt [ 1 ]			= pUnit->getCRTMax				( );
			tSaveData.fClassExp				= pUnit->getClassExp			( );
			tSaveData.fFriendshipExp		= pUnit->getFriendShipExp		( );
			tSaveData.nKillCount			= pUnit->getKillCount			( );
			tSaveData.fExp					= pUnit->getExp					( );
			tSaveData.nIsArcadeEasyClear	= pUnit->getIsArcadeEasyClear	( );
			tSaveData.nIsArcadeNorClear		= pUnit->getIsArcadeNorClear	( );
			tSaveData.nIsArcadeHardClear	= pUnit->getIsArcadeHardClear	( );

			m_aMyRoomUnitSaveDatas.push_back ( tSaveData );
		}
	}

	save ( );
}

KDvoid cMyData::addBetMobSaveData ( KDint nCreateIndex, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt )
{
	cBetMobData*	pBetMobSaveData = new cBetMobData ( );

	pBetMobSaveData->nCreateIndex	= nCreateIndex;
	pBetMobSaveData->nHp			= nHp;
	pBetMobSaveData->nAtk			= nAtk;
	pBetMobSaveData->nDef			= nDef;
	pBetMobSaveData->nAgl			= nAgl;
	pBetMobSaveData->nCrt			= nCrt;

	m_aBetMobDatas.push_back ( pBetMobSaveData );
}

KDvoid cMyData::clearBetMobData ( KDvoid )
{
	for ( auto pBetMobData : m_aBetMobDatas )
	{
		CC_SAFE_DELETE ( pBetMobData );
	}

	m_aBetMobDatas.clear ( );
}

const cBetMobData* cMyData::getBetMobData ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aBetMobDatas.size ( ) )
	{
		return nullptr;
	}

	return m_aBetMobDatas [ nIndex ];
}

KDvoid cMyData::save ( KDvoid )
{
	// NSData---------------------------------------------------------------------
	// init
	std::vector<KDubyte>	aDatas;
		
	// dummy 1
	for ( KDint i = 0; i < _DUMMY_1_COUNT; i++ )
	{
		KDint	nRand = kdRand ( ) % 2000000000;
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	// secret code 1
	KDint	nSecretCode1 = _SECRET_CODE_1_VERSION_1_1;
	appendBytes ( aDatas, &nSecretCode1, sizeof ( KDint ) );

	// dummy 2
	for ( KDint i = 0; i < _DUMMY_2_COUNT; i++ )	
	{
		KDint	nRand = kdRand ( ) % 2000000000;	
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	// bet mob data count
	KDint	nBetMobCount = (KDint) m_aBetMobDatas.size ( );
	appendBytes ( aDatas, &nBetMobCount, sizeof ( KDint ) );

	//dummy 3
	for ( KDint i = 0; i < _DUMMY_3_COUNT; i++ )	
	{
		KDint	nRand = kdRand ( ) % 2000000000;	
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	//bet mob data loop
	for ( KDint i = 0; i < nBetMobCount; i++ )
	{
		// create index
		appendBytes ( aDatas, &m_aBetMobDatas [ i ]->nCreateIndex, sizeof ( KDint ) );

		// dummy 4
		for ( KDint a = 0; a < _DUMMY_4_COUNT; a++ )
		{			
			KDint	nRand = kdRand ( ) % 100;
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// hp
		appendBytes ( aDatas, &m_aBetMobDatas [ i ]->nHp, sizeof ( KDint ) );

		// dummy 5
		for ( KDint a = 0; a < _DUMMY_5_COUNT; a++ )
		{
			KDint	nRand = kdRand ( ) % 100;
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// atk
		appendBytes ( aDatas, &m_aBetMobDatas [ i ]->nAtk, sizeof ( KDint ) );

		// secret code 2
		KDint	nSecretCode2 = _SECRET_CODE_2;
		appendBytes ( aDatas, &nSecretCode2, sizeof ( KDint ) );

		// dummy 6
		for ( KDint a = 0; a < _DUMMY_6_COUNT; a++ )
		{
			KDint	nRand = kdRand ( ) % 100;	
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// def
		appendBytes ( aDatas, &m_aBetMobDatas [ i ]->nDef, sizeof ( KDint ) );

		// dummy 7
		for ( KDint a = 0; a < _DUMMY_7_COUNT; a++ )
		{
			KDint	nRand = kdRand ( ) % 100;	
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// agl
		appendBytes ( aDatas, &m_aBetMobDatas [ i ]->nAgl, sizeof ( KDint ) );

		// dummy 8
		for ( KDint a = 0; a < _DUMMY_8_COUNT; a++ )	
		{
			KDint	nRand = kdRand ( ) % 100;	
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// crt
		appendBytes ( aDatas, &m_aBetMobDatas [ i ]->nCrt, sizeof ( KDint ) );
	}

	// dummy 9
	for ( KDint a = 0; a < _DUMMY_9_COUNT; a++ )
	{
		KDint	nRand = kdRand ( ) % 2000000000;	
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	// secret code 3
	KDint	nSecretCode3 = _SECRET_CODE_3;
	appendBytes ( aDatas, &nSecretCode3, sizeof ( KDint ) );

	// money
	appendBytes ( aDatas, &m_nMoney, sizeof ( KDint64 ) );

	// dummy 10
	for ( KDint a = 0; a < _DUMMY_10_COUNT; a++ )
	{
		KDint	nRand = kdRand ( ) % 2000000000;	
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	// bet money
	appendBytes ( aDatas, &m_nBettingMoney, sizeof ( KDint64 ) );

	// dummy 11
	for ( KDint a = 0; a < _DUMMY_11_COUNT; a++ )	
	{
		KDint	nRand = kdRand ( ) % 2000000000;	
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	// item
	appendBytes ( aDatas, &m_nPotion, sizeof ( KDint ) );

	// dummy 12
	for ( KDint a = 0; a < _DUMMY_12_COUNT; a++ )
	{
		KDint	nRand = kdRand ( ) % 2000000000;	
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	// time
	KDTimeval	tNowTime;
	kdGettimeofday ( &tNowTime, nullptr );

	appendBytes ( aDatas, &tNowTime.tv_sec, sizeof ( KDint ) );
	
	// dummy 13
	for ( KDint a = 0; a < _DUMMY_13_COUNT; a++ )	
	{
		KDint	nRand = kdRand ( ) % 2000000000;
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	// option sound
	appendBytes ( aDatas, &m_bIsOptionSound, sizeof ( KDint ) );

	// dummy 14
	for ( KDint a = 0; a < _DUMMY_14_COUNT; a++ )
	{
		KDint	nRand = kdRand ( ) % 2000000000;	
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );
	}

	// game count
	appendBytes ( aDatas, &m_nGameCount, sizeof ( KDint ) );

	// dummy 15
	for ( KDint a = 0; a < _DUMMY_15_COUNT; a++ )
	{	
		KDint	nRand = kdRand ( ) % 2000000000;	
		appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
	}

	// lose count
	appendBytes ( aDatas, &m_nLoseCount, sizeof ( KDint ) );

	// version 1.10, 구분짖기 위해서 {	} 사용.
	{
		// dummy 16
		for ( KDint a = 0; a < _DUMMY_16_COUNT; a++ )	
		{	
			KDint	nRand = kdRand ( ) % 2000000000;
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// my room unit id
		appendBytes ( aDatas, &m_nNewMyRoomUnitID, sizeof ( KDint ) );

		//secret code 4
		KDint	nSecretCode4 = _SECRET_CODE_4;
		appendBytes ( aDatas, &nSecretCode4, sizeof ( KDint ) );

		// dummy 17
		for ( KDint a = 0; a < _DUMMY_17_COUNT; a++ )
		{
			KDint	nRand = kdRand ( ) % 2000000000;	
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// slot count
		appendBytes ( aDatas, &m_nSlotCount, sizeof ( KDint ) );

		// dummy 18
		for ( KDint a = 0; a < _DUMMY_18_COUNT; a++ )
		{
			KDint	nRand = kdRand ( ) % 2000000000;
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// is first into my room
		appendBytes ( aDatas, &m_bIsFirstIntoMyRoom, sizeof ( KDint ) );
		appendBytes ( aDatas, &m_nMyRoomPage, sizeof ( KDint ) );

		// dummy 19
		for ( KDint a = 0; a < _DUMMY_19_COUNT; a++ )	
		{
			KDint	nRand = kdRand ( ) % 2000000000;	
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// machine 1 buy left time
		appendBytes ( aDatas, &m_dMachine1BuyLeftTime, sizeof ( KDdouble ) );

		// dummy 20
		for ( KDint a = 0; a < _DUMMY_20_COUNT; a++ )	
		{
			KDint	nRand = kdRand ( ) % 2000000000;
			appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
		}

		// secret code 5
		KDint	nSecretCode5 = _SECRET_CODE_5;
		appendBytes ( aDatas, &nSecretCode5, sizeof ( KDint ) );

		// my room unit data count
		KDint	nMyRoomUnitCount = (KDint) m_aMyRoomUnitSaveDatas.size ( );
		appendBytes ( aDatas, &nMyRoomUnitCount, sizeof ( KDint ) );

		// my room unit data loop
		for ( KDint i = 0; i < nMyRoomUnitCount; i++ )
		{
			// dummy 21
			for ( KDint a = 0; a < _DUMMY_21_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// create info file type
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].nCreateInfoFileType, sizeof ( KDint ) );

			// dummy 22
			for ( KDint a = 0; a < _DUMMY_22_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// unit id
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].nUnitID, sizeof ( KDint ) );

			// dummy 23
			for ( KDint a = 0; a < _DUMMY_23_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// create index
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].nCreateIndex, sizeof ( KDint ) );

			// dummy 24
			for ( KDint a = 0; a < _DUMMY_24_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// slot index
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].nSlotIndex, sizeof ( KDint ) );

			// dummy 25
			for ( KDint a = 0; a < _DUMMY_25_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// hp
			appendBytes ( aDatas, m_aMyRoomUnitSaveDatas [ i ].aHp, sizeof ( KDint ) * 2 );

			// dummy 26
			for ( KDint a = 0; a < _DUMMY_26_COUNT; a++ )
			{
				KDint nRand = kdRand ( ) % 2000000000;	appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// secret code 6
			KDint	nSecretCode6 = _SECRET_CODE_6;
			appendBytes ( aDatas, &nSecretCode6, sizeof ( KDint ) );

			// atk
			appendBytes ( aDatas, m_aMyRoomUnitSaveDatas [ i ].aAtk, sizeof ( KDint ) * 2 );

			// dummy 27
			for ( KDint a = 0; a < _DUMMY_27_COUNT; a++ )	
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// def
			appendBytes ( aDatas, m_aMyRoomUnitSaveDatas [ i ].aDef, sizeof ( KDint ) * 2 );

			// dummy 28
			for ( KDint a = 0; a < _DUMMY_28_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// agl
			appendBytes ( aDatas, m_aMyRoomUnitSaveDatas [ i ].aAgl, sizeof ( KDint ) * 2 );

			// dummy 29
			for ( KDint a = 0; a < _DUMMY_29_COUNT; a++)	
			{
				KDint	nRand = kdRand ( ) % 2000000000;
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// crt
			appendBytes ( aDatas, m_aMyRoomUnitSaveDatas [ i ].aCrt, sizeof ( KDint ) * 2 );

			// dummy 30
			for ( KDint a = 0; a < _DUMMY_30_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// exp
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].fExp, sizeof ( KDfloat ) );

			// dummy 31
			for ( KDint a = 0; a < _DUMMY_31_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// class exp
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].fClassExp, sizeof ( KDfloat ) );

			// dummy 32
			for ( KDint a = 0; a < _DUMMY_32_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// friendship exp
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].fFriendshipExp, sizeof ( KDfloat ) );

			// dummy 33
			for ( KDint a = 0; a < _DUMMY_33_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// kill count
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].nKillCount, sizeof ( KDint ) );

			// dummy 34
			for ( KDint a = 0; a < _DUMMY_34_COUNT; a++ )	
			{
				KDint	nRand = kdRand ( ) % 2000000000;
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// is arcade easy clear count
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].nIsArcadeEasyClear, sizeof ( KDint ) );

			// dummy 35
			for ( KDint a = 0; a < _DUMMY_35_COUNT; a++ )	
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// is arcade normal clear count
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].nIsArcadeNorClear, sizeof ( KDint ) );

			// dummy 36
			for ( KDint a = 0; a < _DUMMY_36_COUNT; a++ )
			{
				KDint	nRand = kdRand ( ) % 2000000000;	
				appendBytes ( aDatas, &nRand, sizeof ( KDint ) );	
			}

			// is arcade Hard clear count
			appendBytes ( aDatas, &m_aMyRoomUnitSaveDatas [ i ].nIsArcadeHardClear, sizeof ( KDint ) );
		}
	}

	// ShiftData------------------------------------------------------------------
	KDchar*		pWriteData = new KDchar [ aDatas.size ( ) ];
	kdMemcpy ( pWriteData, aDatas.data ( ), aDatas.size ( ) );	
	ShiftData ( pWriteData, aDatas.size ( ), true );

	// file write-----------------------------------------------------------------
	// path

	// open
	KDFile*		pFile = kdFopen ( _FILE_NAME_MY_DATA, "wb" );
	if ( pFile != nullptr )
	{
		// write
		kdFwrite ( pWriteData, aDatas.size ( ), 1, pFile );

		// close
		kdFclose ( pFile );
	}

	// release--------------------------------------------------------------------
	delete pWriteData;
	aDatas.clear ( );
}

KDvoid cMyData::load ( KDvoid )
{
	// init read data-------------------------------------------------------------
	KDubyte*	pData	  = nullptr;
	KDint		nDataSize = 0;
	KDint		nVersion  = 0;	//0 == 1.0, 10 == 1.10

	// file load------------------------------------------------------------------
	// path

	// open
	KDFile*		pFile = kdFopen ( _FILE_NAME_MY_DATA, "rb" );
	if ( pFile != nullptr )
	{
		// file size
		kdFseek ( pFile, 0, KD_SEEK_END );
		nDataSize = kdFtell ( pFile );
		kdFseek ( pFile , 0, KD_SEEK_SET );

		// new read data
		pData = new KDubyte [ nDataSize ];

		// read
		kdFread ( pData, sizeof ( KDubyte ), nDataSize, pFile );

		// close
		kdFclose ( pFile );
	}

	// check----------------------------------------------------------------------
	if ( pData == nullptr )
	{
		return;
	}

	// ShiftData------------------------------------------------------------------
	ShiftData ( (KDchar*) pData, nDataSize, false );

	// set data-------------------------------------------------------------------
	KDint	nSeek = 0;

	// dummy 1
	nSeek += _DUMMY_1_COUNT * sizeof ( KDint );

	// secret code 1
	KDint		nSecretCode1 = 0;
	if ( readToData ( pData, nDataSize, nSeek, &nSecretCode1, sizeof ( KDint ) ) == false )	
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );		
	}

	if ( nSecretCode1 != _SECRET_CODE_1_VERSION_1_0 && nSecretCode1 != _SECRET_CODE_1_VERSION_1_1 )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );		
	}

		 if ( nSecretCode1 == _SECRET_CODE_1_VERSION_1_0 )	nVersion = 0;
	else if ( nSecretCode1 == _SECRET_CODE_1_VERSION_1_1 )	nVersion = 10;

	// dummy 2
	nSeek += _DUMMY_2_COUNT * sizeof ( KDint );

	// bet mob data count
	KDint	nBetMobCount = 0;
	if ( readToData ( pData, nDataSize, nSeek, &nBetMobCount, sizeof ( KDint ) ) == false )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );			
	}

	// dummy 3
	nSeek += _DUMMY_3_COUNT * sizeof ( KDint );

	// bet mob data loop
	for ( KDint i = 0; i < nBetMobCount; i++ )
	{
		// init
		cBetMobData*	pBetMobSaveData = new cBetMobData ( );

		// create index
		if ( readToData ( pData, nDataSize, nSeek, &pBetMobSaveData->nCreateIndex, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );		
		}

		// dummy 4
		nSeek += _DUMMY_4_COUNT * sizeof ( KDint );

		// hp
		if ( readToData( pData, nDataSize, nSeek, &pBetMobSaveData->nHp, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );			
		}

		//dummy 5
		nSeek += _DUMMY_5_COUNT * sizeof ( KDint );

		// atk
		if ( readToData ( pData, nDataSize, nSeek, &pBetMobSaveData->nAtk, sizeof ( KDint ) ) == false )	
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );		
		}

		// secret code 2
		KDint	nSecretCode2 = 0;
		if ( readToData ( pData, nDataSize, nSeek, &nSecretCode2, sizeof ( KDint ) ) == false )	
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );			
		}

		if ( nSecretCode2 != _SECRET_CODE_2 )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );	
		}

		// dummy 6
		nSeek += _DUMMY_6_COUNT * sizeof ( KDint );

		// def
		if ( readToData ( pData, nDataSize, nSeek, &pBetMobSaveData->nDef, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );		
		}

		// dummy 7
		nSeek += _DUMMY_7_COUNT * sizeof ( KDint );

		// agl
		if ( readToData ( pData, nDataSize, nSeek, &pBetMobSaveData->nAgl, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );		
		}

		// dummy 8
		nSeek += _DUMMY_8_COUNT * sizeof ( KDint );

		// crt
		if ( readToData ( pData, nDataSize, nSeek, &pBetMobSaveData->nCrt, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );		
		}

		// push
		m_aBetMobDatas.push_back ( pBetMobSaveData );
	}

	// dummy 9
	nSeek += _DUMMY_9_COUNT * sizeof ( KDint );

	// secret code 3
	KDint	nSecretCode3 = 0;
	if ( readToData ( pData, nDataSize, nSeek, &nSecretCode3, sizeof ( KDint ) ) == false )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );		
	}

	if ( nSecretCode3 != _SECRET_CODE_3 )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );	
	}

	// money
	if ( readToData ( pData, nDataSize, nSeek, &m_nMoney, sizeof ( KDint64 ) ) == false )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );			
	}

	// dummy 10
	nSeek += _DUMMY_10_COUNT * sizeof ( KDint );

	// betting money
	if ( readToData ( pData, nDataSize, nSeek, &m_nBettingMoney, sizeof ( KDint64 ) ) == false )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );		
	}

	// dummy 11
	nSeek += _DUMMY_11_COUNT * sizeof ( KDint );

	// item
	if ( readToData ( pData, nDataSize, nSeek, &m_nPotion, sizeof ( KDint ) ) == false )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );	
	}

	// dummy 12
	nSeek += _DUMMY_12_COUNT * sizeof ( KDint );

	// time
	if ( readToData ( pData, nDataSize, nSeek, &m_nOpenTime, sizeof ( KDint ) ) == false )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );		
	}

	// dummy 13
	nSeek += _DUMMY_13_COUNT * sizeof ( KDint );

	// option sound
	if ( readToData ( pData, nDataSize, nSeek, &m_bIsOptionSound, sizeof ( KDint ) ) == false )
	{	
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );		
	}

	// dummy 14
	nSeek += _DUMMY_14_COUNT * sizeof ( KDint );

	// game count
	if ( readToData ( pData, nDataSize, nSeek, &m_nGameCount, sizeof ( KDint ) ) == false )	
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );			
	}

	// dummy 15
	nSeek += _DUMMY_15_COUNT * sizeof ( KDint );

	// lose count
	if ( readToData ( pData, nDataSize, nSeek, &m_nLoseCount, sizeof ( KDint ) ) == false )
	{
		kdUnlink ( _FILE_NAME_MY_DATA );		
		init ( );			
	}

	// version
	if ( nVersion >= 10 )		// version 1.10
	{
		// dummy 16
		nSeek += _DUMMY_16_COUNT * sizeof ( KDint );

		// my room unit id
		if ( readToData ( pData, nDataSize, nSeek, &m_nNewMyRoomUnitID, sizeof ( KDint ) ) == false )	
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );			
		}

		// secret code 4
		KDint	nSecretCode4 = 0;
		if ( readToData ( pData, nDataSize, nSeek, &nSecretCode4, sizeof ( KDint ) ) == false )	
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );				
		}

		if ( nSecretCode4 != _SECRET_CODE_4 )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );	
		}

		// dummy 17
		nSeek += _DUMMY_17_COUNT * sizeof ( KDint );

		// slot count
		if ( readToData ( pData, nDataSize, nSeek, &m_nSlotCount, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );			
		}

		// dummy 18
		nSeek += _DUMMY_18_COUNT * sizeof ( KDint );

		// is first into my room
		if ( readToData ( pData, nDataSize, nSeek, &m_bIsFirstIntoMyRoom, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );			
		}
		if ( readToData ( pData, nDataSize, nSeek, &m_nMyRoomPage, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );				
		}

		// dummy 19
		nSeek += _DUMMY_19_COUNT * sizeof ( KDint );

		// machine 1 buy left time
		if ( readToData ( pData, nDataSize, nSeek, &m_dMachine1BuyLeftTime, sizeof ( KDdouble ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );	
		}

		// dummy 20
		nSeek += _DUMMY_20_COUNT * sizeof ( KDint );

		// secret code 5
		KDint	nSecretCode5 = 0;
		if ( readToData ( pData, nDataSize, nSeek, &nSecretCode5, sizeof ( KDint ) ) == false )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );		
		}

		if ( nSecretCode5 != _SECRET_CODE_5 )
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );
		}

		// my room unit count
		KDint	nMyRoomUnitCount = 0;
		if ( readToData ( pData, nDataSize, nSeek, &nMyRoomUnitCount, sizeof ( KDint ) ) == false )	
		{
			kdUnlink ( _FILE_NAME_MY_DATA );		
			init ( );	
		}

		// my room unit data loop
		for ( KDint i = 0; i < nMyRoomUnitCount; i++ )
		{
			// init
			cMyRoomUnitSaveData		tSaveData;

			// dummy 21
			nSeek += _DUMMY_21_COUNT * sizeof ( KDint );

			// create info file type
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.nCreateInfoFileType, sizeof ( KDint ) ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );		
			}

			// dummy 22
			nSeek += _DUMMY_22_COUNT * sizeof ( KDint );

			// unit id
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.nUnitID, sizeof ( KDint ) ) == false )
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );		
			}

			// dummy 23
			nSeek += _DUMMY_23_COUNT * sizeof ( KDint );

			// create index
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.nCreateIndex, sizeof ( KDint ) ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );		
			}

			// dummy 24
			nSeek += _DUMMY_24_COUNT * sizeof ( KDint );

			//slot index
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.nSlotIndex, sizeof ( KDint ) ) == false )
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );			
			}

			// dummy 25
			nSeek += _DUMMY_25_COUNT * sizeof ( KDint );

			// hp
			if ( readToData ( pData, nDataSize, nSeek, tSaveData.aHp, sizeof ( KDint ) * 2 ) == false )
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );				
			}

			// dummy 26
			nSeek += _DUMMY_26_COUNT * sizeof ( KDint );

			// secret code 6
			KDint		nSecretCode6 = 0;
			if ( readToData ( pData, nDataSize, nSeek, &nSecretCode6, sizeof ( KDint ) ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );		
			}

			if ( nSecretCode6 != _SECRET_CODE_6 )
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );	
			}

			// atk
			if ( readToData ( pData, nDataSize, nSeek, tSaveData.aAtk, sizeof ( KDint ) * 2 ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );		
			}

			// dummy 27
			nSeek += _DUMMY_27_COUNT * sizeof ( KDint );

			// def
			if ( readToData ( pData, nDataSize, nSeek, tSaveData.aDef, sizeof ( KDint ) * 2 ) == false )
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );			
			}

			// dummy 28
			nSeek += _DUMMY_28_COUNT * sizeof ( KDint );

			// agl
			if ( readToData ( pData, nDataSize, nSeek, tSaveData.aAgl, sizeof ( KDint ) * 2 ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );				
			}

			// dummy 29
			nSeek += _DUMMY_29_COUNT * sizeof ( KDint );

			// crt
			if ( readToData ( pData, nDataSize, nSeek, tSaveData.aCrt, sizeof ( KDint ) * 2 ) == false )
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );	
			}

			// dummy 30
			nSeek += _DUMMY_30_COUNT * sizeof ( KDint );

			// exp
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.fExp, sizeof ( KDfloat ) ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );		
			}

			// dummy 31
			nSeek += _DUMMY_31_COUNT * sizeof ( KDint );

			// exp
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.fClassExp, sizeof ( KDfloat ) ) == false )
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );				
			}

			// dummy 32
			nSeek += _DUMMY_32_COUNT * sizeof ( KDint );

			// friendship exp
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.fFriendshipExp, sizeof ( KDfloat ) ) == false )
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );			
			}

			// dummy 33
			nSeek += _DUMMY_33_COUNT * sizeof ( KDint );

			// kill count
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.nKillCount, sizeof ( KDint ) ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );		
			}

			// dummy 34
			nSeek += _DUMMY_34_COUNT * sizeof ( KDint );

			// is arcade easy clear count
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.nIsArcadeEasyClear, sizeof ( KDint ) ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );		
			}

			// dummy 35
			nSeek += _DUMMY_35_COUNT * sizeof ( KDint );

			// is arcade easy clear count
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.nIsArcadeNorClear, sizeof ( KDint ) ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );			
			}

			// dummy 36
			nSeek += _DUMMY_36_COUNT * sizeof ( KDint );

			// is arcade easy clear count
			if ( readToData ( pData, nDataSize, nSeek, &tSaveData.nIsArcadeHardClear, sizeof ( KDint ) ) == false )	
			{
				kdUnlink ( _FILE_NAME_MY_DATA );		
				init ( );			
			}

			// push back
			m_aMyRoomUnitSaveDatas.push_back ( tSaveData );
		}
	}

	// release--------------------------------------------------------------------
	delete pData;
}

KDbool cMyData::readToData ( const KDubyte* pSrcData, const KDint nSrcDataSize, KDint& nSeek, KDvoid* pDestData, const KDint nDestDataSize )
{
	if ( nSeek + nDestDataSize > nSrcDataSize )
	{
		return false;
	}

	kdMemcpy ( pDestData, &pSrcData [ nSeek ], nDestDataSize );

	nSeek += nDestDataSize;

	return true;
}

KDvoid cMyData::ShiftData ( KDchar* pSource, KDint nLength, KDbool bForward )
{
	KDchar*		pData = nullptr;
	KDchar*		pTemp = nullptr;

	pData = new KDchar [ nLength ];
	pTemp = new KDchar [ nLength ];
	kdMemset ( pData, 0, sizeof ( KDchar ) * nLength );
	kdMemset ( pTemp, 0, sizeof ( KDchar ) * nLength );

	kdMemcpy ( pData, pSource, sizeof ( KDchar ) * nLength );

	KDint		nPos = 0;			//	입력 할 위치
	KDulong		uOrigin = 0;		//	변환 할 정보
	KDulong		uConvert = 0;		//	변환 된 정보

	while ( nPos < ( nLength - 3 ) ) 
	{
		uOrigin = *(KDulong*) ( pData + nPos );
		uConvert = shifter ( uOrigin, bForward );
		*(KDulong*) ( pTemp + nPos ) = uConvert;
		nPos += 4;
	}	

	if ( nLength % 4 )
	{
		kdMemcpy ( pTemp, &pData + sizeof ( KDchar ) * nPos, sizeof ( KDchar ) * ( nLength % 4 ) );
	}

	kdMemcpy ( pSource, pTemp, sizeof ( KDchar ) * nLength );	

	if ( pData ) delete pData;
	if ( pTemp ) delete pTemp;
}

KDulong cMyData::shifter ( KDulong i, KDbool bForward )
{
	KDint		aShiftTable   [ 32 ] = { 26, 31, 17, 10, 30, 16, 24,  2, 29,  8, 20, 15, 28, 11, 13,  4, 19, 23,  0, 12, 14, 27,  6, 18, 21,  3,  9,  7, 22, 1, 25, 5 };
	KDint		aReverseTable [ 32 ] = { 18, 29, 07, 25, 15, 31, 22, 27,  9, 26,  3, 13, 19, 14, 20, 11,  5,  2, 23, 16, 10, 24, 28, 17,  6, 30,  0, 21, 12, 8,  4, 1 };

	KDulong		uResult = 0;
	KDint		nPos = 0;
	KDint		nValue;
	while ( i > 0 ) 
	{
		nValue = i - ( ( i >> 1 ) << 1 );
		i = i >> 1;
		if ( nValue )  
		{
			if ( bForward )		uResult += nValue << aShiftTable   [ nPos ];
			else 				uResult += nValue << aReverseTable [ nPos ];
		}
		nPos++;
	}

	return uResult;
}

KDint cMyData::getNewHeroID ( KDvoid )
{
	++m_nNewHeroID;
	return m_nNewHeroID;
}

KDint cMyData::getNewMyRoomUnitID ( KDvoid )
{
	++m_nNewMyRoomUnitID;
	return m_nNewMyRoomUnitID;
}

KDvoid cMyData::gamingExpAdd ( KDint nAttUnitCapablePoint, KDint nDefUnitCapablePoint )
{
	KDint	nSubPoint = nAttUnitCapablePoint - nDefUnitCapablePoint;

		 if ( nSubPoint < -50 )	m_fGamingExp += 30;
	else if ( nSubPoint < -45 )	m_fGamingExp += 28;
	else if ( nSubPoint < -40 )	m_fGamingExp += 27;
	else if ( nSubPoint < -35 )	m_fGamingExp += 26;
	else if ( nSubPoint < -30 )	m_fGamingExp += 25;
	else if ( nSubPoint < -25 )	m_fGamingExp += 24;
	else if ( nSubPoint < -20 )	m_fGamingExp += 23;
	else if ( nSubPoint < -15 )	m_fGamingExp += 22;
	else if ( nSubPoint < -10 )	m_fGamingExp += 21;
	else if ( nSubPoint < - 5 )	m_fGamingExp += 20;
	else if ( nSubPoint <   0 )	m_fGamingExp += 18;
	else if ( nSubPoint <  10 )	m_fGamingExp += 16;
	else if ( nSubPoint <  20 )	m_fGamingExp += 14;
	else if ( nSubPoint <  30 )	m_fGamingExp += 12;
	else if ( nSubPoint <  40 )	m_fGamingExp += 10;
	else if ( nSubPoint <  50 )	m_fGamingExp += 9;
	else if ( nSubPoint <  60 )	m_fGamingExp += 8;
	else if ( nSubPoint <  70 )	m_fGamingExp += 7;
	else if ( nSubPoint <  80 )	m_fGamingExp += 6;
	else						m_fGamingExp += 5;
}

KDvoid cMyData::update ( KDdouble dLeftTime )
{
	m_dMachine1BuyLeftTime -= dLeftTime;
	if ( dLeftTime > 1.0f )
	{
		dLeftTime = 1.0f;
	}

	for ( auto& rMyRoomUnitSaveData : m_aMyRoomUnitSaveDatas )
	{
		// exp
			 if ( rMyRoomUnitSaveData.fFriendshipExp > FRIENDSHIP_LOVE )	rMyRoomUnitSaveData.fExp += dLeftTime * _EXP_UP_ONLINE_WORST;
		else if ( rMyRoomUnitSaveData.fFriendshipExp > FRIENDSHIP_BEST )	rMyRoomUnitSaveData.fExp += dLeftTime * _EXP_UP_ONLINE_BEST;
		else if ( rMyRoomUnitSaveData.fFriendshipExp > FRIENDSHIP_GOOD )	rMyRoomUnitSaveData.fExp += dLeftTime * _EXP_UP_ONLINE_GOOD;
		else if ( rMyRoomUnitSaveData.fFriendshipExp > FRIENDSHIP_BAD  )	rMyRoomUnitSaveData.fExp += dLeftTime * _EXP_UP_ONLINE_BAD;
		else																rMyRoomUnitSaveData.fExp += dLeftTime * _EXP_UP_ONLINE_WORST;

		// friendship
		rMyRoomUnitSaveData.fFriendshipExp -= dLeftTime * _FRIENDSHIP_DOWN_OFFLINE_DOWN;
		if ( rMyRoomUnitSaveData.fFriendshipExp < 0 )
		{
			rMyRoomUnitSaveData.fFriendshipExp = 0;
		}
	}
}
