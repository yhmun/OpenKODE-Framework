/* -----------------------------------------------------------------------------------
 *
 *      File            MyData.h
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

#ifndef __MyData_h__
#define __MyData_h__

class cHeroSaveData
{
	public :

		cHeroSaveData ( KDvoid )
		: nHeroID		(  0 )
		, nCreateIndex	( -1 )
		, nSeatIndex	( -1 )
		, nPositionType	( ePositionType_Front)
		, nLevel		( 1 )
		, nExp			( 0 )
		, nHp			( 0 )
		, nUseHp		( 0 )
		, nAtk			( 0 )
		, nDef			( 0 )
		, nAgl			( 0 )
		, nCrt			( 0 )
		{
		}
		~cHeroSaveData ( KDvoid )
		{
		}

		KDint				nHeroID;
		KDint				nCreateIndex;
		KDint				nSeatIndex;
		KDint				nPositionType;

		KDint				nLevel;
		KDint				nExp;

		KDint				nHp;
		KDint				nUseHp;
		KDint				nAtk;
		KDint				nDef;
		KDint				nAgl;
		KDint				nCrt;

		std::vector<KDint>	aSkills;
};


class cMyRoomUnitSaveData
{
	public :

		cMyRoomUnitSaveData ( KDvoid )
		: nCreateInfoFileType	( eCreateInfoFileType_Coin_1 )
		, nUnitID				( 0 )
		, nCreateIndex			( 0 )
		, nSlotIndex			( 0 )
		, fExp					( 0 )
		, fClassExp				( 0 )
		, fFriendshipExp		( 0 )
		, nKillCount			( 0 )
		, nIsArcadeEasyClear	( 0 )
		, nIsArcadeNorClear		( 0 )
		, nIsArcadeHardClear	( 0 )
		{
			aHp  [ 0 ] = 0;
			aHp  [ 1 ] = 0;
			aAtk [ 0 ] = 0;
			aAtk [ 1 ] = 0;
			aDef [ 0 ] = 0;
			aDef [ 1 ] = 0;
			aAgl [ 0 ] = 0;
			aAgl [ 1 ] = 0;
			aCrt [ 0 ] = 0;
			aCrt [ 1 ] = 0;
		}

		KDvoid		operator= ( const cMyRoomUnitSaveData& rSrc )
		{
			nCreateInfoFileType	= rSrc.nCreateInfoFileType;
			nUnitID				= rSrc.nUnitID;
			nCreateIndex		= rSrc.nCreateIndex;
			nSlotIndex			= rSrc.nSlotIndex;

			aHp  [ 0 ]			= rSrc.aHp  [ 0 ];
			aHp  [ 1 ]			= rSrc.aHp  [ 1 ];
			aAtk [ 0 ]			= rSrc.aAtk [ 0 ];
			aAtk [ 1 ]			= rSrc.aAtk [ 1 ];
			aDef [ 0 ]			= rSrc.aDef [ 0 ];
			aDef [ 1 ]			= rSrc.aDef [ 1 ];
			aAgl [ 0 ]			= rSrc.aAgl [ 0 ];
			aAgl [ 1 ]			= rSrc.aAgl [ 1 ];
			aCrt [ 0 ]			= rSrc.aCrt [ 0 ];
			aCrt [ 1 ]			= rSrc.aCrt [ 1 ];
			fExp				= rSrc.fExp;
			fClassExp			= rSrc.fClassExp;
			fFriendshipExp		= rSrc.fFriendshipExp;
			nKillCount			= rSrc.nKillCount;
			nIsArcadeEasyClear	= rSrc.nIsArcadeEasyClear;
			nIsArcadeNorClear	= rSrc.nIsArcadeNorClear;
			nIsArcadeHardClear	= rSrc.nIsArcadeHardClear;
		}

		KDint				nCreateInfoFileType;
		KDint				nUnitID;
		KDint				nCreateIndex;
		KDint				nSlotIndex;

		KDint				aHp  [ 2 ];				// [0] == now, [1] == max
		KDint				aAtk [ 2 ];
		KDint				aDef [ 2 ];
		KDint				aAgl [ 2 ];
		KDint				aCrt [ 2 ];

		KDfloat				fExp;

		KDfloat				fClassExp;
		KDfloat				fFriendshipExp;
		KDint				nKillCount;

		KDint				nIsArcadeEasyClear;
		KDint				nIsArcadeNorClear;
		KDint				nIsArcadeHardClear;
};


class cBetMobData
{
	public :

		cBetMobData ( KDvoid )
		: nCreateIndex ( 0 )
		, nHp		   ( 0 )
		, nAtk		   ( 0 )
		, nDef		   ( 0 )
		, nAgl		   ( 0 )
		, nCrt		   ( 0 )
		{

		}

		~cBetMobData ( KDvoid )
		{

		}

		KDint				nCreateIndex;

		KDint				nHp;
		KDint				nAtk;
		KDint				nDef;
		KDint				nAgl;
		KDint				nCrt;
};


class cUnit;

class cMyData
{
	public :
				 cMyData ( KDvoid );
		virtual ~cMyData ( KDvoid );

		static cMyData*		getObj ( KDvoid );

	public :

		KDvoid				init ( KDvoid );

		KDvoid				addHeroSaveDatas ( cHeroSaveData& rSaveData );
		KDvoid				updateHeroSaveDatas ( std::vector<cUnit*>& rHeros );

		KDvoid				addMyRoomUnitSaveDatas ( cMyRoomUnitSaveData& aSaveData );
		KDvoid				delMyRoomUnitSaveDatas ( cUnit* pUnit );
		KDvoid				updateMyRoomUnitSaveDatas ( std::vector<cUnit*>& aUnits );

		KDvoid				addBetMobSaveData ( KDint nCreateIndex, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt );
		KDvoid				clearBetMobData ( KDvoid );
		const cBetMobData*	getBetMobData ( KDint nIndex );

		KDvoid				save ( KDvoid );
		KDvoid				load ( KDvoid );

		static KDbool		readToData ( const KDubyte* pSrcData, const KDint nSrcDataSize, KDint& rSeek, KDvoid* pDestData, const KDint nDestDataSize );
		KDvoid				ShiftData ( KDchar* pSource, KDint nLength, KDbool bForward );
		KDulong				shifter ( KDulong i, KDbool bForward );

		KDint				getNewHeroID ( KDvoid );
		KDint				getNewMyRoomUnitID ( KDvoid );

		KDvoid				gamingExpAdd ( KDint nAttUnitCapablePoint, KDint nDefUnitCapablePoint );

		KDvoid				update ( KDdouble dLeftTime );

		KDvoid				preLoadSounds ( KDvoid );

	private :

		const KDint64						m_nConstInitMoney;
		std::vector<cBetMobData*>			m_aBetMobDatas;

	public :

		std::vector<cHeroSaveData>			m_aHeroSaveDatas;
		std::vector<cMyRoomUnitSaveData>	m_aMyRoomUnitSaveDatas;

		KDint64								m_nMoney;
		KDint64								m_nBettingMoney;
		KDint								m_nComebackCoin;

		KDint								m_nPotion;
		KDbool								m_bIsOptionSound;

		// my room
		KDint								m_nSlotCount;
		KDbool								m_bIsFirstIntoMyRoom;
		KDint								m_nMyRoomPage;

		// ¿¬½Â.
		KDint								m_nSuccessive;

		// ½Î¿î ¼ö.
		KDint								m_nGameCount;

		// ÆÐ¹èÇÑ ¼ö.
		KDint								m_nLoseCount;

		// open time
		KDint								m_nOpenTime;

		// first machine buy left time
		KDdouble							m_dMachine1BuyLeftTime;

		// gameing exp
		KDfloat								m_fGamingExp;
		KDint								m_nGamingKillCount;

	private :

		KDint								m_nNewHeroID;
		KDint								m_nNewMyRoomUnitID;
};

#endif	// __MyData_h__
