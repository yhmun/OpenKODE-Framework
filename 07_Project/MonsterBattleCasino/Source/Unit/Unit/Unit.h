/* -----------------------------------------------------------------------------------
 *
 *      File            Unit.h
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

#ifndef __Unit_h__
#define __Unit_h__

#include "Object/Sprite/Sprite.h"
#include "Object/Sprite/SprAni.h"
#include "Object/Sprite/SprAnis.h"
#include "Unit/Create/UnitCreateInfos.h"

class cParaAdd
{
	public :
		cParaAdd ( KDvoid )
		: dTime					( 0 )
		, dLeftTime				( 0 )
		, nLeftCount			( 0 )
		, nHp					( 0 )
		, nAtk					( 0 )
		, nDef					( 0 )
		, nAgl					( 0 )
		, nCrt					( 0 )
		, nAttackUnitSeatIndex	( -1 )
		{
		}

		KDdouble		dTime;
		KDdouble		dLeftTime;
		KDint			nLeftCount;
		KDint			nHp;
		KDint			nAtk;
		KDint			nDef;
		KDint			nAgl;
		KDint			nCrt;
		KDint			nAttackUnitSeatIndex;
};

class sTurnResult
{
	public :
	
		sTurnResult ( KDvoid )
		: nHp  ( 0 )
		, nAtk ( 0 )
		, nDef ( 0 )
		, nAgl ( 0 )
		, nCrt ( 0 )
		{

		}

		KDint			nHp;
		KDint			nAtk;
		KDint			nDef;
		KDint			nAgl;
		KDint			nCrt;
};

class cUnit : public Object
{
	public :

		// init & destroy
				 cUnit ( Node* pNode, const cUnitCreateInfo* pCreateInfo, eCreateInfoFileType eCreateInfoType, KDint z, KDint nUnitID, KDint nCreateIndex, KDint nSeatIndex );
		virtual ~cUnit ( KDvoid );

		// load
		KDvoid							load ( KDint nSeatIndex, ePositionType ePosType, KDint nLevel, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt, const std::vector<KDint>& rSkills );
		KDvoid							load ( eCreateInfoFileType eCreateInfoType, KDint nSlotIndex, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt, KDint nHpMax, KDint nAtkMax, KDint nDefMax, KDint nAglMax, KDint nCrtMax, KDfloat fExp, KDfloat fClassExp, KDfloat fFriendShipExp, KDint nKillCount );
		KDvoid							load ( KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt );

		// update
		KDvoid							update ( KDdouble dLeftTickCount );
		KDbool							updateExp ( KDfloat fExp );

		// turn
		KDvoid							turn ( KDdouble dLeftTickCount, std::vector<sTurnResult>& aResults );

		// node
		KDvoid							changeNode ( Node* pNode, KDint z );

		// texture load & release
		KDvoid							texLoad ( KDint nnAniIndex );
		KDvoid							texLoad ( KDint nnAniIndex, KDint z );
		KDvoid							texRelease ( KDint nnAniIndex );

		// portrait sprite file name
		inline std::string				getPortraitSprFileName ( KDvoid ) {	return m_sPortraitSprFileName; }

		// create info unique type
		inline eCreateInfoFileType		getCreateInfoFileType ( KDvoid ) { return m_eCreateInfoFileType; }

		// unit type
		inline KDvoid					setUnitType ( eUnitType eType )	{ m_eUnitType = eType; }
		inline eUnitType				getUnitType ( KDvoid ) { return m_eUnitType; }

		// unique type
		inline KDvoid					setUniqueType ( eUniqueType eUnique ) {	m_eUniqueType = eUnique; }
		inline eUniqueType				getUniqueType ( KDvoid ) { return m_eUniqueType; }

		// capable point
		inline KDint					getCapablePoint ( KDvoid ) { return m_nCapablePoint; }

		// achievement
		inline KDvoid					setClassExp	( KDfloat fClassExp ) { m_fClassExp = fClassExp; }
		inline KDvoid					setFriendShipExp ( KDfloat fFriendShipExp ) { m_fFriendShipExp = fFriendShipExp; }
		inline KDvoid					setKillCount ( KDint nKillCount ) { m_nKillCount = nKillCount; }
		inline KDfloat					getClassExp	( KDvoid ) { return m_fClassExp; }
		inline KDfloat					getFriendShipExp ( KDvoid )	{ return m_fFriendShipExp; }
		inline KDint					getKillCount ( KDvoid )	{ return m_nKillCount; }

		// Indices
		inline KDvoid					setSeatIndex ( KDint nIndex ) { m_nSeatIndex = nIndex; }
		inline KDvoid					setSlotIndex ( KDint nIndex ) { m_nSlotIndex = nIndex; }
		inline KDint					getCreateIndex ( KDvoid ) { return m_nCreateIndex; }
		inline KDint					getSeatIndex ( KDvoid ) { return m_nSeatIndex; }
		inline KDint					getSlotIndex ( KDvoid ) { return m_nSlotIndex; }

		//name
		inline std::string				getName ( KDvoid ) { return m_sName; }

		// visible & position
		KDvoid							setVisible ( KDbool bIsVisible );		// 기본 동작
		KDvoid							setPoint ( const Point& tPoint );
		KDvoid							setPointTime ( const Point& tPoint, KDdouble dLeftTime, KDbool bIsForciblyChange = true );
		Point							getPoint ( KDvoid );
		KDbool							getVisible ( KDvoid );

 		// image name_b & portrait visible & position
		KDvoid							setVisibleNameA ( KDbool bIsVisible );
		KDvoid							setVisibleNameB ( KDbool bIsVisible );
		KDvoid							setVisiblePortrait ( KDbool bIsVisible );
		KDvoid							setPointNameA ( const Point& tPoint );
		KDvoid							setPointNameB ( const Point& tPoint );
		KDvoid							setPointPortrait ( const Point& tPoint );
		KDvoid							setPointTimeNameA ( const Point& tPoint, KDdouble dLeftTime );
		KDvoid							setPonitTimeNameB ( const Point& tPoint, KDdouble dLeftTime );
		KDvoid							setPointTimePortrait ( const Point& tPoint, KDdouble dLeftTime );
		KDvoid							setPortraitAlpha ( KDubyte cAlpha );
		KDvoid							setPortraitColor ( KDubyte r, KDubyte g, KDubyte b );

		// original parameter
		// 가능하면 쓰지 말자.
		KDvoid							setHP  ( KDint nHp  );
		KDvoid							setATK ( KDint nAtk );
		KDvoid							setDEF ( KDint nDef );
		KDvoid							setAGL ( KDint nAgl );
		KDvoid							setCRT ( KDint nCrt  );
		inline KDvoid					setHPMax  ( KDint nHpMax  )	{ m_nHpMax  = nHpMax;  }
		inline KDvoid					setATKMax ( KDint nAtkMax )	{ m_nAtkMax = nAtkMax; }
		inline KDvoid					setDEFMax ( KDint nDefMax )	{ m_nDefMax = nDefMax; }
		inline KDvoid					setAGLMax ( KDint nAglMax )	{ m_nAglMax = nAglMax; }
		inline KDvoid					setCRTMax ( KDint nCrtMax )	{ m_nCrtMax = nCrtMax; }

		inline KDint					getUnitID ( KDvoid ) { return m_nUnitID; }
		inline KDint					getLevel  ( KDvoid ) { return m_nLevel;	 }
		inline KDint					getHP	  ( KDvoid ) { return m_nHp;	 }
		inline KDint					getATK	  ( KDvoid ) { return m_nAtk;	 }
		inline KDint					getDEF	  ( KDvoid ) { return m_nDef;	 }
		inline KDint					getAGL	  ( KDvoid ) { return m_nAgl;	 }
		inline KDint					getCRT    ( KDvoid ) { return m_nCrt;	 }
		inline KDint					getHPMax  ( KDvoid ) { return m_nHpMax;	 }
		inline KDint					getATKMax ( KDvoid ) { return m_nAtkMax; }
		inline KDint					getDEFMax ( KDvoid ) { return m_nDefMax; }
		inline KDint					getAGLMax ( KDvoid ) { return m_nAglMax; }
		inline KDint					getCRTMax ( KDvoid ) { return m_nCrtMax; }

		KDvoid							paraUp ( KDint& rHpAdd, KDint& rAtkAdd, KDint& rDefAdd, KDint& rAglAdd, KDint& rCrtAdd );
		KDbool							getIsParaMax ( KDvoid );

		// exp
		inline KDvoid					setExp ( KDfloat fExp )	{ m_fExp = fExp; }
		inline KDfloat					getExp ( KDvoid ) {	return m_fExp; }

		// special point
		inline Point					getPointSpecial ( KDvoid )	{ return m_tSpecial; }

		// use parameter
		KDvoid							resetParaAdds ( KDvoid );									// hp는 제외.
		KDvoid							setParaAdds ( const std::vector<cParaAdd>& rParaAdds );
		KDvoid							setUseHp ( KDint nHp );
		KDvoid							setUseHpAdd  ( KDdouble dTime, KDint nCount, KDint nHp, KDint nAttackUnitSeatIndex );
		KDvoid							setUseAtkAdd ( KDdouble dTime, KDint nAtk );
		KDvoid							setUseDefAdd ( KDdouble dTime, KDint nDef );
		KDvoid							setUseAglAdd ( KDdouble dTime, KDint nAgl );
		KDvoid							setUseCrtAdd ( KDdouble dTime, KDint nCrt );
		KDint							getUseHp  ( KDvoid );
		KDint							getUseAtk ( KDvoid );
		KDint							getUseDef ( KDvoid );
		KDint							getUseAgl ( KDvoid );
		KDint							getUseCrt ( KDvoid );
		const std::vector<cParaAdd>&	getUseParaAdd ( KDvoid ) { return m_aUseAdds; }

		// detail
		inline const std::string		getStrDetail ( KDvoid )	{ return m_sDetail; }

		// skill info
		KDvoid							addSkill ( KDint nType );
		sSkill							getSkill ( KDint nIndex );
		KDint							getSkillType ( KDint nIndex );
		std::string						getSkillStr ( KDint nIndex );
		inline KDint					getSkillCount ( KDvoid ) { return (KDint) m_aSkills.size ( ); }

		// normal animation first frame size
		Size							getNorFirstSize ( KDvoid );

		// shdow
		inline KDvoid					setIsShadow ( KDbool bIsShdow ) { m_bIsShdow = bIsShdow; }
	
		//die
		KDvoid							die ( KDdouble dDelay );
		KDvoid							dieNoEffect ( KDvoid );
		inline KDbool					getDie ( KDvoid ) {	return m_bIsDie; }
		inline KDvoid					setKillMeUnitSeatIndex ( KDint nSeatIndex )	{ m_nKilledMeUnitSeatIndex = nSeatIndex; }
		inline KDint					getKillMeUnitSeatIndex ( KDvoid ) {	return m_nKilledMeUnitSeatIndex; }

		//positionType
		inline void						setPositionType ( ePositionType ePosType ) { m_ePositionType = ePosType; }
		inline void						setMobLineIndex ( KDint nIndex ) { m_nMobLineIndex = nIndex; }
		inline ePositionType			getPositionType ( KDvoid ) { return m_ePositionType; }
		inline int						getMobLineIndex ( KDvoid ) { return m_nMobLineIndex; }

		// action & script
		KDint							getActionRandIndex ( KDvoid );
		KDint							getActionRandIndex ( KDbool bRangeShort, KDbool bRangeBack );
		cUnitScript*					getScript ( KDint nActionIndex, KDint nScriptIndex );

		// animation
		KDvoid							changeAni ( eUnitAniType eAniType );
		KDvoid							aniStart ( KDvoid );
		KDvoid							aniStop ( KDvoid );
		KDvoid							setAniNowFrame ( KDint nFrame );
		eUnitAniType					getAniType ( KDvoid );
		KDint							getAniNowFrame ( KDvoid );
		std::string						getAniFileName ( KDvoid );

		// flicker
		KDvoid							setFlicker ( KDdouble dLeftTime );

		// bet info
		KDvoid							setBetMoney ( KDint64 nMoney );
		KDvoid							setDividendMagnification ( KDfloat fMagnification );
		inline KDint64					getBetMoney ( KDvoid ) { return m_nBetMoney; }
		inline KDfloat					getDividendMagnification ( KDvoid ) { return m_fDividendMagnification; }

		// color
		KDvoid							setIsColorChange ( KDbool bIsChange );
		KDvoid							setColor ( KDubyte a, KDubyte r, KDubyte g, KDubyte b );

		// switch
		KDvoid							setIsAutoChangeNor ( KDbool bIsAuto );

		// sell
		inline KDvoid					setSellPrice ( KDint64 nPrice )	{ m_nSellPrice = nPrice; }
		inline KDint64					getSellPrice ( KDvoid ) { return m_nSellPrice; }

		// casino advantage
		inline KDint					getCasinoAdvantage ( KDvoid ) { return m_nCasinoAdvantage; }

		// rect in pt check
		KDbool							isRectInPoint ( Point tPoint );

		// arcade clear
		inline KDvoid					setIsArcadeEasyClear ( KDbool bIsClear ) { m_bIsArcadeEasyClear = bIsClear; }
		inline KDvoid					setIsArcadeNorClear	 ( KDbool bIsClear ) { m_bIsArcadeNorClear  = bIsClear; }
		inline KDvoid					setIsArcadeHardClear ( KDbool bIsClear ) { m_bIsArcadeHardClear = bIsClear; }
		inline KDbool					getIsArcadeEasyClear ( KDvoid )	{ return m_bIsArcadeEasyClear; }
		inline KDbool					getIsArcadeNorClear	 ( KDvoid )	{ return m_bIsArcadeNorClear;  }
		inline KDbool					getIsArcadeHardClear ( KDvoid )	{ return m_bIsArcadeHardClear; }

	private :

		KDvoid							updateCapablePoint ( KDvoid );

	private :

		Node*							m_pNode;

		// animations
		cSprAnis*						m_pAnis;

		// shdow
		cSprite*						m_pSprShadow;
		KDbool							m_bIsShdow;

		// portrait sprite file name
		std::string						m_sPortraitSprFileName;

		// create info unique type
		eCreateInfoFileType				m_eCreateInfoFileType;

		// unit type
		eUnitType						m_eUnitType;

		// unique type
		eUniqueType						m_eUniqueType;

		// capable point
		KDint							m_nCapablePoint;

		// Indices
		KDint							m_nCreateIndex;
		KDint							m_nSeatIndex;
		KDint							m_nSlotIndex;

		// name
		std::string						m_sName;

		//image name, portrait
		cSprAni*						m_pSprNameA;
		cSprAni*						m_pSprNameB;
		cSprAni*						m_pSprPortrait;

		// level
		KDint							m_nUnitID;
		KDint							m_nLevel;
		KDfloat							m_fExp;

		// achievement
		KDfloat							m_fClassExp;
		KDfloat							m_fFriendShipExp;
		KDint							m_nKillCount;

		// original parameters
		KDint							m_nHp;
		KDint							m_nAtk;
		KDint							m_nDef;
		KDint							m_nAgl;
		KDint							m_nCrt;
		KDint							m_nHpMax;
		KDint							m_nAtkMax;
		KDint							m_nDefMax;
		KDint							m_nAglMax;
		KDint							m_nCrtMax;
		KDint							m_aLvUpHp  [ 2 ];
		KDint							m_aLvUpAtk [ 2 ];
		KDint							m_aLvUpDef [ 2 ];
		KDint							m_aLvUpAgl [ 2 ];
		KDint							m_aLvUpCrt [ 2 ];

		// special point
		Point							m_tSpecial;

		//use parameters
		KDint							m_nUseHp;
		std::vector<cParaAdd>			m_aUseAdds;

		// detail
		std::string						m_sDetail;

		// skill info
		std::vector<sSkill>				m_aSkills;
		std::vector<std::string>		m_aSkillStrs;

		// actions
		std::vector<cUnitAction*>		m_aActions;
		KDint							m_nActionTotalChance;

		// rgba
		KDbool							m_bIsColorChange;
		KDint							m_nImgRed;
		KDint							m_nImgGreen;
		KDint							m_nImgBlue;
		KDint							m_nImgAlpha;

		// die
		KDbool							m_bIsDie;
		KDint							m_nDieStep;
		KDdouble						m_dDieLeftDelay;
		KDint							m_nKilledMeUnitSeatIndex;

		// positionType
		ePositionType					m_ePositionType;
		KDint							m_nMobLineIndex;

		// flicker
		KDbool							m_bIsFlicker;
		KDdouble						m_dFlickerLeftTime1;
		KDdouble						m_dFlickerLeftTime2;

		// bet info
		KDint64							m_nBetMoney;
		KDfloat							m_fDividendMagnification;

		// switch
		KDbool							m_bIsAutoChangeNor;

		// sell
		KDint64							m_nSellPrice;

		// casino advantage
		KDint							m_nCasinoAdvantage;

		// normal animation first frame size
		Size							m_tNorFirstSize;

		// arcade clear
		KDbool							m_bIsArcadeEasyClear;
		KDbool							m_bIsArcadeNorClear;
		KDbool							m_bIsArcadeHardClear;
};

#endif	// __Unit_h__
