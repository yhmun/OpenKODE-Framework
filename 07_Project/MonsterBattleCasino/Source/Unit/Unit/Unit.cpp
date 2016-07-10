/* -----------------------------------------------------------------------------------
 *
 *      File            Unit.cpp
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
#include "Unit.h"
#include "Unit/Create/UnitCreateInfos.h"
#include "MyData/MyData.h"

cUnit::cUnit ( Node* pNode, const cUnitCreateInfo* pCreateInfo, eCreateInfoFileType eCreateInfoType, KDint z, KDint nUnitID, KDint nCreateIndex, KDint nSeatIndex )
: m_pNode					( pNode					)
, m_pAnis					( nullptr				)
, m_pSprShadow				( nullptr				)
, m_bIsShdow				( false					)
, m_eCreateInfoFileType		( eCreateInfoType		)
, m_eUnitType				( eUnitType_End			)
, m_eUniqueType				( eUniqueType_common	)
, m_nCapablePoint			( 0						)
, m_nCreateIndex			( nCreateIndex			)
, m_nSeatIndex				( nSeatIndex			)
, m_nSlotIndex				( 0						)
, m_pSprNameA				( nullptr				)
, m_pSprNameB				( nullptr				)
, m_pSprPortrait			( nullptr				)
, m_nUnitID					( nUnitID				)
, m_nLevel					( 1						)
, m_fExp					( 0						)
, m_fClassExp				( 0						)
, m_fFriendShipExp			( 0						)
, m_nKillCount				( 0						)
, m_nHp						( 0						)
, m_nAtk					( 0						)
, m_nDef					( 0						)
, m_nAgl					( 0						)
, m_nCrt					( 0						)
, m_nHpMax					( 0						)
, m_nAtkMax					( 0						)
, m_nDefMax					( 0						)
, m_nAglMax					( 0						)
, m_nCrtMax					( 0						)
, m_tSpecial				( Point::ZERO			)
, m_nUseHp					( 0						)
, m_nActionTotalChance		( 0						)
, m_bIsColorChange			( false					)
, m_nImgRed					( 0						)
, m_nImgGreen				( 0						)
, m_nImgBlue				( 0						)
, m_nImgAlpha				( 0						)
, m_bIsDie					( false					)
, m_nDieStep				( 0						)
, m_dDieLeftDelay			( 0						)
, m_nKilledMeUnitSeatIndex	( -1					)
, m_ePositionType			( ePositionType_Front	)
, m_nMobLineIndex			( 0						)
, m_bIsFlicker				( false					)
, m_dFlickerLeftTime1		( 0						)
, m_dFlickerLeftTime2		( 0						)
, m_nBetMoney				( 0						)
, m_fDividendMagnification	( 0						)
, m_bIsAutoChangeNor		( true					)
, m_nSellPrice				( 0						)
, m_nCasinoAdvantage		( 0						)
, m_tNorFirstSize			( Size::ZERO			)
, m_bIsArcadeEasyClear		( false					)
, m_bIsArcadeNorClear		( false					)
, m_bIsArcadeHardClear		( false					)
{
	// animations
	m_pAnis = new cSprAnis ( m_pNode, pCreateInfo->sSpriteFileDir + pCreateInfo->sSpriteFileName, z, Point::ZERO );
	m_pAnis->setVisible ( false );

	// shadow
	switch ( m_nSeatIndex )
	{
		case 0	:	m_pSprShadow = new cSprite ( m_pNode, "Game/shdow.png", LAYER_GAMEVIEW_MOB_SHDOW, Point ( 0.5f, 0.5f ), Point (  50, 70 ) );	break;
		case 1	:	m_pSprShadow = new cSprite ( m_pNode, "Game/shdow.png", LAYER_GAMEVIEW_MOB_SHDOW, Point ( 0.5f, 0.5f ), Point ( 145, 70 ) );	break;
		case 2	:	m_pSprShadow = new cSprite ( m_pNode, "Game/shdow.png", LAYER_GAMEVIEW_MOB_SHDOW, Point ( 0.5f, 0.5f ), Point ( 240, 70 ) );	break;
		case 3	:	m_pSprShadow = new cSprite ( m_pNode, "Game/shdow.png", LAYER_GAMEVIEW_MOB_SHDOW, Point ( 0.5f, 0.5f ), Point ( 334, 70 ) );	break;
		case 4	:	m_pSprShadow = new cSprite ( m_pNode, "Game/shdow.png", LAYER_GAMEVIEW_MOB_SHDOW, Point ( 0.5f, 0.5f ), Point ( 429, 70 ) );	break;
		default	:	m_pSprShadow = new cSprite ( m_pNode, "Game/shdow.png", LAYER_GAMEVIEW_MOB_SHDOW, Point ( 0.5f, 0.5f ), Point (  50, 70 ) );	break;
	}
	
	m_pSprShadow->get ( )->setVisible ( false );

	// name
	m_sName	= pCreateInfo->sName;

	// unique type
	m_eUniqueType = (eUniqueType) pCreateInfo->nCasinoAdvantageORuniqueType;

	// parameters
	m_nHp		= pCreateInfo->aHp  [ 0 ] + kdRand ( ) % ( pCreateInfo->aHp  [ 1 ] - pCreateInfo->aHp  [ 0 ] + 1 );
	m_nUseHp	= m_nHp;
	m_nAtk		= pCreateInfo->aAtk [ 0 ] + kdRand ( ) % ( pCreateInfo->aAtk [ 1 ] - pCreateInfo->aAtk [ 0 ] + 1 );
	m_nDef		= pCreateInfo->aDef [ 0 ] + kdRand ( ) % ( pCreateInfo->aDef [ 1 ] - pCreateInfo->aDef [ 0 ] + 1 );
	m_nAgl		= pCreateInfo->aAgl [ 0 ] + kdRand ( ) % ( pCreateInfo->aAgl [ 1 ] - pCreateInfo->aAgl [ 0 ] + 1 );
	m_nCrt		= pCreateInfo->aCrt [ 0 ] + kdRand ( ) % ( pCreateInfo->aCrt [ 1 ] - pCreateInfo->aCrt [ 0 ] + 1 );

	m_nHpMax	= pCreateInfo->aLvUpHp  [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpHp  [ 1 ] - pCreateInfo->aLvUpHp  [ 0 ] + 1 );
	m_nAtkMax	= pCreateInfo->aLvUpAtk [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpAtk [ 1 ] - pCreateInfo->aLvUpAtk [ 0 ] + 1 );
	m_nDefMax	= pCreateInfo->aLvUpDef [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpDef [ 1 ] - pCreateInfo->aLvUpDef [ 0 ] + 1 );
	m_nAglMax	= pCreateInfo->aLvUpAgl [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpAgl [ 1 ] - pCreateInfo->aLvUpAgl [ 0 ] + 1 );
	m_nCrtMax	= pCreateInfo->aLvUpCrt [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpCrt [ 1 ] - pCreateInfo->aLvUpCrt [ 0 ] + 1 );

	m_aLvUpHp  [ 0 ] = pCreateInfo->aLvUpHp  [ 0 ];
	m_aLvUpHp  [ 1 ] = pCreateInfo->aLvUpHp  [ 1 ];
	m_aLvUpAtk [ 0 ] = pCreateInfo->aLvUpAtk [ 0 ];
	m_aLvUpAtk [ 1 ] = pCreateInfo->aLvUpAtk [ 1 ];
	m_aLvUpDef [ 0 ] = pCreateInfo->aLvUpDef [ 0 ];
	m_aLvUpDef [ 1 ] = pCreateInfo->aLvUpDef [ 1 ];
	m_aLvUpAgl [ 0 ] = pCreateInfo->aLvUpAgl [ 0 ];
	m_aLvUpAgl [ 1 ] = pCreateInfo->aLvUpAgl [ 1 ];
	m_aLvUpCrt [ 0 ] = pCreateInfo->aLvUpCrt [ 0 ];
	m_aLvUpCrt [ 1 ] = pCreateInfo->aLvUpCrt [ 1 ];

	// special point
	m_tSpecial.x = pCreateInfo->tPointSpecialX;
	m_tSpecial.y = pCreateInfo->tPointSpecialY;

	// detail
	m_sDetail = pCreateInfo->sStrDetail;

	// casino advantage
	m_nCasinoAdvantage = pCreateInfo->nCasinoAdvantageORuniqueType;
							
	// actions
	for ( auto pSrcAction : pCreateInfo->aActions )
	{	
		// action
		cUnitAction*	pDestAction = new cUnitAction ( );

		// chance
		KDint			nChance = 10;
					
		// scripts
		for ( auto pSrcScript : pSrcAction->aScripts )
		{
			// script
			cUnitScript*	pDestScript = new cUnitScript ( );

			// copy
			pDestScript->copy ( *pSrcScript );

			// chance
			if ( pDestScript->eType == eScriptType_Chance )
			{
				nChance = pDestScript->fSub;
			}

			// add
			pDestAction->aScripts.push_back ( pDestScript );			
		}

		// total chance
		m_nActionTotalChance += nChance;

		// add
		m_aActions.push_back ( pDestAction );		
	}

	// skill info
	addSkill ( pCreateInfo->nSkillIcon1 );
	addSkill ( pCreateInfo->nSkillIcon2 );
	addSkill ( pCreateInfo->nSkillIcon3 );
	addSkill ( pCreateInfo->nSkillIcon4 );

	m_aSkillStrs.push_back ( pCreateInfo->sSkillStr1 );
	m_aSkillStrs.push_back ( pCreateInfo->sSkillStr2 );
	m_aSkillStrs.push_back ( pCreateInfo->sSkillStr3 );
	m_aSkillStrs.push_back ( pCreateInfo->sSkillStr4 );

	// changeAni
	m_pAnis->changeAni ( eUnitAniType_Normal );
	m_pAnis->setVisible ( false );

	// portrait sprite file name
	m_pAnis->getAniFileNames ( eUnitAniType_Portrait, m_sPortraitSprFileName );

	// name, portrait
	m_pSprNameA	   = m_pAnis->getAni ( eUnitAniType_Name_a	 );
	m_pSprNameB	   = m_pAnis->getAni ( eUnitAniType_Name_b	 );
	m_pSprPortrait = m_pAnis->getAni ( eUnitAniType_Portrait );

	m_pAnis->setPointAutoSwitch ( eUnitAniType_Name_a  , false );
	m_pAnis->setPointAutoSwitch ( eUnitAniType_Name_b  , false );
	m_pAnis->setPointAutoSwitch ( eUnitAniType_Portrait, false );

	// animation loop
	m_pAnis->setLoop ( eUnitAniType_Normal , true  );
	m_pAnis->setLoop ( eUnitAniType_Attack , false );
	m_pAnis->setLoop ( eUnitAniType_Special, false );
	m_pAnis->setLoop ( eUnitAniType_Damage , false );
				
	// normal animation first frame size
	KDbool		bIsSucces = false;
	cFrame		tFrame;
	m_pAnis->getAni ( eUnitAniType_Normal )->getFrameData ( 0, bIsSucces, tFrame );
	if ( bIsSucces == true )
	{
		m_tNorFirstSize = tFrame.tRectSrc.size;
	}

	// capable point
	updateCapablePoint ( );
}

cUnit::~cUnit ( KDvoid )
{
	// animations
	CC_SAFE_RELEASE ( m_pAnis );

	//shadow
	CC_SAFE_RELEASE ( m_pSprShadow );
	
	// name, portrait
	m_pSprNameA	   = nullptr;
	m_pSprNameB	   = nullptr;
	m_pSprPortrait = nullptr;

	// skill info
	m_aSkills   .clear ( );
	m_aSkillStrs.clear ( );

	// actions
	for ( auto pAction : m_aActions )
	{
		CC_SAFE_RELEASE ( pAction );
	}
	m_aActions.clear ( );
}

KDvoid cUnit::load ( KDint nSeatIndex, ePositionType ePosType, KDint nLevel, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt, const std::vector<KDint>& rSkills )
{
	m_nSeatIndex	= nSeatIndex;
	m_ePositionType	= ePosType;

	m_nLevel		= nLevel;
	m_nHp			= nHp;
	m_nUseHp		= nHp;
	m_nAtk			= nAtk;
	m_nDef			= nDef;
	m_nAgl			= nAgl;
	m_nCrt			= nCrt;

	m_aSkills.clear ( );
	for ( auto& nSkill : rSkills )
	{
		addSkill ( nSkill );
	}

	// capable point
	updateCapablePoint ( );
}

KDvoid cUnit::load ( eCreateInfoFileType eCreateInfoType, KDint nSlotIndex, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt, KDint nHpMax, KDint nAtkMax, KDint nDefMax, KDint nAglMax, KDint nCrtMax, KDfloat fExp, KDfloat fClassExp, KDfloat fFriendShipExp, KDint nKillCount )
{
	m_eCreateInfoFileType	= eCreateInfoType;
	m_nSlotIndex			= nSlotIndex;
	m_nHp					= nHp;
	m_nUseHp				= nHp;
	m_nAtk					= nAtk;
	m_nDef					= nDef;
	m_nAgl					= nAgl;
	m_nCrt					= nCrt;
	m_nHpMax				= nHpMax;
	m_nAtkMax				= nAtkMax;
	m_nDefMax				= nDefMax;
	m_nAglMax				= nAglMax;
	m_nCrtMax				= nCrtMax;
	m_fExp					= fExp;
	m_fClassExp				= fClassExp;
	m_fFriendShipExp		= fFriendShipExp;
	m_nKillCount			= nKillCount;

	// capable point
	updateCapablePoint ( );
}

KDvoid cUnit::load ( KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt )
{
	m_nHp	 = nHp;
	m_nUseHp = nHp;
	m_nAtk	 = nAtk;
	m_nDef	 = nDef;
	m_nAgl	 = nAgl;
	m_nCrt	 = nCrt;

	// capable point
	updateCapablePoint ( );
}

KDvoid cUnit::changeNode ( Node* pNode, KDint z )
{
	m_pNode = pNode;

	m_pAnis->changeNode	( m_pNode, z );
	m_pSprShadow->changeNode ( m_pNode, LAYER_GAMEVIEW_MOB_SHDOW );
}

KDvoid cUnit::texLoad ( KDint nAniIndex )
{
	m_pAnis->texLoad ( nAniIndex );
}

KDvoid cUnit::texLoad ( KDint nAniIndex, KDint z )
{
	m_pAnis->texLoad ( nAniIndex, z );
}

KDvoid cUnit::texRelease ( KDint nAniIndex )
{
	m_pAnis->texRelease ( nAniIndex );
}

KDvoid cUnit::update ( KDdouble dLeftTickCount )
{
	m_pSprNameA	  ->update ( dLeftTickCount );
	m_pSprNameB	  ->update ( dLeftTickCount );
	m_pSprPortrait->update ( dLeftTickCount );

	if ( m_pAnis->update ( dLeftTickCount ) == true )
	{
		if ( m_bIsAutoChangeNor == true && m_pAnis->getSelectnAniIndex ( ) != eUnitAniType_Normal )
		{
			m_pAnis->changeAni ( eUnitAniType_Normal );
			m_pAnis->start ( );
		}
	}

	// shadow
	if ( m_bIsShdow == true )
	{
		if ( m_bIsDie == true )	m_pSprShadow->get ( )->setVisible ( false );
		else					m_pSprShadow->get ( )->setVisible ( true );
	}
	else
	{
		m_pSprShadow->get ( )->setVisible ( false );
	}

	// die delay check
	if ( m_bIsDie == true && m_nDieStep == 0 )
	{
		m_dDieLeftDelay -= dLeftTickCount;

		if ( m_dDieLeftDelay < 0 )
		{
			m_bIsColorChange = true;
			m_nImgRed		 = 255;
			m_nImgGreen		 = 50;
			m_nImgBlue		 = 50;
			m_nImgAlpha		 = 255;
			m_nDieStep		 = 1;
		}
	}

	// color change
	if ( m_bIsColorChange == true && m_nImgAlpha > 0 )
	{
		m_nImgAlpha -= (KDint) ( dLeftTickCount * 400 );
		m_nImgGreen += (KDint) ( dLeftTickCount * 400 );
		m_nImgBlue  += (KDint) ( dLeftTickCount * 400 );

		if ( m_nImgAlpha <   0 )	m_nImgAlpha	= 0;
		if ( m_nImgGreen > 255 )	m_nImgGreen	= 255;
		if ( m_nImgBlue  > 255 )	m_nImgBlue	= 255;

		m_pAnis->setAlpha ( eUnitAniType_Normal , (KDubyte) m_nImgAlpha );
		m_pAnis->setAlpha ( eUnitAniType_Attack , (KDubyte) m_nImgAlpha );
		m_pAnis->setAlpha ( eUnitAniType_Special, (KDubyte) m_nImgAlpha );
		m_pAnis->setAlpha ( eUnitAniType_Damage , (KDubyte) m_nImgAlpha );

		m_pAnis->setColor ( eUnitAniType_Normal , m_nImgRed, m_nImgGreen, m_nImgBlue );
		m_pAnis->setColor ( eUnitAniType_Attack , m_nImgRed, m_nImgGreen, m_nImgBlue );
		m_pAnis->setColor ( eUnitAniType_Special, m_nImgRed, m_nImgGreen, m_nImgBlue );
		m_pAnis->setColor ( eUnitAniType_Damage , m_nImgRed, m_nImgGreen, m_nImgBlue );
	}

	// flicker
	if ( m_bIsDie == false && m_bIsFlicker == true )
	{
		m_dFlickerLeftTime1 -= dLeftTickCount;
		m_dFlickerLeftTime2 -= dLeftTickCount;

		if ( m_dFlickerLeftTime1 < 0 && m_pAnis->getVisible ( ) == true )
		{
			m_bIsFlicker = false;
		}
		else
		{
			if ( m_dFlickerLeftTime2 < 0 )
			{
				if ( m_pAnis->getVisible ( ) == true )	m_pAnis->setVisible ( false );
				else									m_pAnis->setVisible ( true  );

				m_dFlickerLeftTime2 = 0.06f;
			}
		}
	}
}

KDbool cUnit::updateExp ( KDfloat fExp )
{
	// set
	m_fExp = fExp;

	if ( m_fExp >= 100 )
	{
		m_fExp -= 100;
		return true;
	}

	return false;
}

KDvoid cUnit::turn ( KDdouble dLeftTickCount, std::vector<sTurnResult>& aResults )
{
	// time parameters
	std::vector<cParaAdd>::iterator		pNode = m_aUseAdds.begin ( );
	while ( pNode != m_aUseAdds.end ( ) )
	{
		// init
		cParaAdd&	rParaAdd = *pNode;

		//left time
		rParaAdd.dLeftTime -= dLeftTickCount;
		
		// left time check
		if ( rParaAdd.dLeftTime > 0 )
		{
			++pNode;
			continue;
		}

		// left time & left count init
		rParaAdd.dLeftTime = rParaAdd.dTime;
		--rParaAdd.nLeftCount;

		// hp
		if ( rParaAdd.nHp != 0 )
		{
			sTurnResult		tResult;

			KDint			nSaveHP = m_nUseHp;
			setUseHp ( m_nUseHp + rParaAdd.nHp );
			tResult.nHp = m_nUseHp - nSaveHP;

			if ( tResult.nHp == 0 )
			{
				tResult.nHp = -100000;
			}

			if ( m_nUseHp == 0 )
			{
				m_nKilledMeUnitSeatIndex = rParaAdd.nAttackUnitSeatIndex;
			}

			aResults.push_back ( tResult );
		}

		// next
		if ( rParaAdd.nLeftCount <= 0 )
		{
			pNode = m_aUseAdds.erase ( pNode );
		}
		else
		{
			++pNode;
		}
	}

	if ( m_nUseHp == 0 )
	{
		die ( 0.2f );
	}
}

KDvoid cUnit::setVisible ( KDbool bIsVisible )
{
	m_pAnis->setVisible ( bIsVisible );
}

KDvoid cUnit::setPoint ( const Point& tPoint )
{
	m_pAnis->setPoint ( tPoint );
}

KDvoid cUnit::setPointTime ( const Point& tPoint, KDdouble dLeftTime, KDbool bIsForciblyChange )
{
	m_pAnis->setPointTime ( tPoint, dLeftTime, bIsForciblyChange );
}

KDbool cUnit::getVisible ( KDvoid )
{
	return m_pAnis->getVisible ( );
}

Point cUnit::getPoint ( KDvoid )
{
	return m_pAnis->getPoint ( );
}

KDvoid cUnit::setVisibleNameA ( KDbool bIsVisible )
{
	m_pSprNameA->setVisible ( bIsVisible );
}

KDvoid cUnit::setVisibleNameB ( KDbool bIsVisible )
{
	m_pSprNameB->setVisible ( bIsVisible);
}

KDvoid cUnit::setVisiblePortrait ( KDbool bIsVisible )
{
	m_pSprPortrait->setVisible ( bIsVisible );
}

KDvoid cUnit::setPointNameA ( const Point& tPoint )
{
	m_pSprNameA->setPoint ( tPoint );
}

KDvoid cUnit::setPointNameB ( const Point& tPoint )
{
	m_pSprNameB->setPoint ( tPoint );
}

KDvoid cUnit::setPointPortrait ( const Point& tPoint )
{
	m_pSprPortrait->setPoint ( tPoint );
}

KDvoid cUnit::setPointTimeNameA ( const Point& tPoint, KDdouble dLeftTime )
{
	m_pSprNameA->setPointTime ( tPoint, dLeftTime );
}

KDvoid cUnit::setPonitTimeNameB ( const Point& tPoint, KDdouble dLeftTime )
{
	m_pSprNameB->setPointTime ( tPoint, dLeftTime );
}

KDvoid cUnit::setPointTimePortrait ( const Point& tPoint, KDdouble dLeftTime )
{
	m_pSprPortrait->setPointTime ( tPoint, dLeftTime );
}

KDvoid cUnit::setPortraitAlpha ( KDubyte cAlpha )
{
	m_pSprPortrait->setAlpha ( cAlpha );
}

KDvoid cUnit::setPortraitColor ( KDubyte r, KDubyte g, KDubyte b )
{
	m_pSprPortrait->setColor ( r, g, b );
}

KDvoid cUnit::setHP ( KDint nHp ) 
{
	m_nHp = nHp;	
	updateCapablePoint ( );
}

KDvoid cUnit::setATK ( KDint nAtk )
{
	m_nAtk = nAtk;	
	updateCapablePoint ( );
}

KDvoid cUnit::setDEF ( KDint nDef )
{
	m_nDef = nDef;
	updateCapablePoint ( );
}

KDvoid cUnit::setAGL ( KDint nAgl )	
{
	m_nAgl = nAgl;
	updateCapablePoint ( );
}

KDvoid cUnit::setCRT ( KDint nCrt )	
{
	m_nCrt = nCrt;
	updateCapablePoint ( );
}

KDvoid cUnit::paraUp ( KDint& rHpAdd, KDint& rAtkAdd, KDint& rDefAdd, KDint& rAglAdd, KDint& rCrtAdd )
{
	if ( m_nHp	 + rHpAdd > m_nHpMax  )	rHpAdd	= m_nHpMax	- m_nHp;
	if ( m_nAtk + rAtkAdd > m_nAtkMax )	rAtkAdd = m_nAtkMax - m_nAtk;
	if ( m_nDef + rDefAdd > m_nDefMax )	rDefAdd = m_nDefMax - m_nDef;
	if ( m_nAgl + rAglAdd > m_nAglMax )	rAglAdd = m_nAglMax - m_nAgl;
	if ( m_nCrt + rCrtAdd > m_nCrtMax )	rCrtAdd = m_nCrtMax - m_nCrt;

	m_nHp	 += rHpAdd;
	m_nUseHp  = m_nHp;
	m_nAtk	 += rAtkAdd;
	m_nDef	 += rDefAdd;
	m_nAgl	 += rAglAdd;
	m_nCrt	 += rCrtAdd;

	// capable point;
	updateCapablePoint ( );
}

KDbool cUnit::getIsParaMax ( KDvoid )
{
	if ( m_nHp < m_nHpMax || m_nAtk < m_nAtkMax || m_nDef < m_nDefMax || m_nAgl < m_nAglMax || m_nCrt < m_nCrtMax )
	{
		return false;
	}

	return true;
}

KDvoid cUnit::resetParaAdds ( KDvoid )
{
	m_aUseAdds.clear ( );
}

KDvoid cUnit::setParaAdds ( const std::vector<cParaAdd>& rParaAdds )
{
	m_aUseAdds.clear ( );

	for ( KDuint i = 0; i < rParaAdds.size ( ); i++ )
	{
		m_aUseAdds.push_back ( rParaAdds [ i ] );
	}
}

KDvoid cUnit::setUseHp ( KDint nHp )
{
	if ( nHp <  0	  )	nHp = 0;
	if ( nHp >= m_nHp )	nHp = m_nHp;

	m_nUseHp = nHp;
}

KDvoid cUnit::setUseHpAdd ( KDdouble dTime, KDint nCount, KDint nHp, KDint nAttackUnitSeatIndex )
{
	cParaAdd		tAdd;

	tAdd.dTime					= dTime;
	tAdd.dLeftTime				= dTime;
	tAdd.nLeftCount				= nCount;
	tAdd.nHp					= nHp;
	tAdd.nAttackUnitSeatIndex	= nAttackUnitSeatIndex;

	m_aUseAdds.push_back ( tAdd );
}

KDvoid cUnit::setUseAtkAdd ( KDdouble dTime, KDint nAtk )
{
	cParaAdd		tAdd;

	tAdd.dTime		= dTime;
	tAdd.dLeftTime	= dTime;
	tAdd.nLeftCount	= 1;
	tAdd.nAtk		= nAtk;

	m_aUseAdds.push_back ( tAdd );
}

KDvoid cUnit::setUseDefAdd ( KDdouble dTime, KDint nDef )
{
	cParaAdd		tAdd;

	tAdd.dTime		= dTime;
	tAdd.dLeftTime  = dTime;
	tAdd.nLeftCount = 1;
	tAdd.nDef		= nDef;

	m_aUseAdds.push_back ( tAdd );
}

KDvoid cUnit::setUseAglAdd ( KDdouble dTime, KDint nAgl )
{
	cParaAdd		tAdd;

	tAdd.dTime		= dTime;
	tAdd.dLeftTime  = dTime;
	tAdd.nLeftCount = 1;
	tAdd.nAgl		= nAgl;

	m_aUseAdds.push_back ( tAdd );
}

KDvoid cUnit::setUseCrtAdd ( KDdouble dTime, KDint nCrt )
{
	cParaAdd		tAdd;

	tAdd.dTime		= dTime;
	tAdd.dLeftTime  = dTime;
	tAdd.nLeftCount = 1;
	tAdd.nCrt		= nCrt;

	m_aUseAdds.push_back ( tAdd );
}

KDint cUnit::getUseHp ( KDvoid )
{
	return m_nUseHp;
}

KDint cUnit::getUseAtk ( KDvoid )
{
	KDint	nSub = m_nAtk;

	for ( auto& rUseAdd : m_aUseAdds )
	{
		nSub += rUseAdd.nAtk;
	}

	if ( nSub <  0 )	nSub = 0;
	if ( nSub > 99 )	nSub = 99;

	return nSub;
}

KDint cUnit::getUseDef ( KDvoid )
{
	KDint	nSub = m_nDef;

	for ( auto& rUseAdd : m_aUseAdds )
	{
		nSub += rUseAdd.nDef;
	}

	if ( nSub <  0 )	nSub = 0;
	if ( nSub > 99 )	nSub = 99;

	return nSub;
}

KDint cUnit::getUseAgl ( KDvoid )
{
	KDint	nSub = m_nAgl;

	for ( auto& rUseAdd : m_aUseAdds )
	{
		nSub += rUseAdd.nAgl;
	}

	if ( nSub <  1 )	nSub = 1;
	if ( nSub > 99 )	nSub = 99;

	return nSub;
}

KDint cUnit::getUseCrt ( KDvoid )
{
	KDint	nSub = m_nCrt;

	for ( auto& rUseAdd : m_aUseAdds )
	{
		nSub += rUseAdd.nCrt;
	}

	if ( nSub <  0 )	nSub = 0;
	if ( nSub > 99 )	nSub = 99;

	return nSub;
}

KDvoid cUnit::addSkill ( KDint nType )
{
	sSkill		tSkill;

	tSkill.nType	 = nType;
	tSkill.fWaitTime = 0;
	tSkill.ePosition = ePositionType_End;
	tSkill.eTarget	 = eTargetType_End;
	tSkill.eRange	 = eRangeType_End;

	if ( tSkill.nType >= 0 && tSkill.nType < (KDint) m_aActions.size ( ) )
	{
		for ( auto pScript : m_aActions [ tSkill.nType ]->aScripts )
		{
			if ( pScript->eType == eScriptType_Skill_delay )
			{
				tSkill.fWaitTime = pScript->fSub / 1000;
			}
			else if ( pScript->eType == eScriptType_Skill_positonType )
			{
				if ( (KDint) pScript->fSub == ePositionType_Front )			tSkill.ePosition = ePositionType_Front;
				else														tSkill.ePosition = ePositionType_Back;
			}
			else if ( pScript->eType == eScriptType_Skill_targetType )
			{
					 if ( (KDint) pScript->fSub == eTargetType_Ally_One	 )	tSkill.eTarget = eTargetType_Ally_One;
				else if ( (KDint) pScript->fSub == eTargetType_Ally_All	 )	tSkill.eTarget = eTargetType_Ally_All;
				else if ( (KDint) pScript->fSub == eTargetType_Enemy_One )	tSkill.eTarget = eTargetType_Enemy_One;	
				else														tSkill.eTarget = eTargetType_Enemy_All;	
			}
			else if ( pScript->eType == eScriptType_Skill_rangeType )
			{
					 if ( (KDint) pScript->fSub == eRangeType_Short )		tSkill.eRange = eRangeType_Short;
				else if ( (KDint) pScript->fSub == eRangeType_Long  )		tSkill.eRange = eRangeType_Long;	
				else if ( (KDint) pScript->fSub == eRangeType_Back  )		tSkill.eRange = eRangeType_Back;	
				else														tSkill.eRange = eRangeType_All;
			}
		}
	}

	m_aSkills.push_back ( tSkill );
}

sSkill cUnit::getSkill ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aSkills.size ( ) )
	{
		sSkill		tSkill;

		tSkill.nType		= -1;
		tSkill.fWaitTime	= 0;
		tSkill.ePosition	= ePositionType_End;
		tSkill.eTarget		= eTargetType_End;
		tSkill.eRange		= eRangeType_End;

		return tSkill;
	}

	return m_aSkills [ nIndex ];
}

KDint cUnit::getSkillType ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aSkills.size ( ) )
	{
		return -1;
	}

	return m_aSkills [ nIndex ].nType;
}

std::string	cUnit::getSkillStr ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aSkillStrs.size ( ) )
	{
		return "";
	}

	return m_aSkillStrs [ nIndex ];
}

Size cUnit::getNorFirstSize ( KDvoid )
{
	return m_tNorFirstSize;
}

KDvoid cUnit::die ( KDdouble dDelay )
{
	if ( m_bIsDie == true )
	{
		return;
	}

	if ( cMyData::getObj ( )->m_bIsOptionSound != 0 )
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "monster_dead.mp3" );
	}

	m_pAnis->setVisible ( true );

	m_bIsDie		= true;
	m_dDieLeftDelay	= dDelay;
	m_bIsFlicker	= false;

	if ( dDelay == 0 )
	{
		m_nDieStep		 = 1;
		m_bIsColorChange = true;
		m_nImgRed		 = 255;
		m_nImgGreen		 = 50;
		m_nImgBlue		 = 50;
		m_nImgAlpha		 = 255;
	}
	else
	{
		m_nDieStep = 0;
	}

	m_aUseAdds.clear();
}

KDvoid cUnit::dieNoEffect ( KDvoid )
{
	m_bIsDie		 = true;
	m_dDieLeftDelay	 = 0;
	m_bIsFlicker	 = false;
	m_nDieStep		 = 1;
	m_bIsColorChange = true;

	setColor ( 0, 255, 255, 255 );
}

KDint cUnit::getActionRandIndex ( KDvoid )
{
	KDint	nCheckChance = kdRand ( ) % m_nActionTotalChance;
	KDint	nChanceSum	 = 0;

	for ( KDuint i = 0; i < m_aActions.size ( ); i++ )
	{
		auto		pAction = m_aActions [ i ];
		KDint		nChance = 10;

		for ( auto pScript : pAction->aScripts )
		{		
			if ( pScript->eType == eScriptType_Chance ) 
			{
				nChance = pScript->fSub;
				break;
			}
		}

		nChanceSum += nChance;

		if ( nCheckChance < nChanceSum )
		{
			return i;
		}
	}
		   
   return -1;
}

KDint cUnit::getActionRandIndex ( KDbool bRangeShort, KDbool bRangeBack )
{
	std::vector<cUnitAction*>	aActions;
	std::vector<KDint>			aActionIndexs;
	KDint						nTotalCance = 0;

	// actions
	for ( KDuint i = 0; i < m_aActions.size ( ); i++ )
	{
		// chance
		KDint		nChance			= 10;
		KDbool		bLoopRangeShort = false;
		KDbool		bLoopRangeBack	= false;

		// scripts
		for ( auto pScript : m_aActions [ i ]->aScripts )
		{
			// chance
			if ( pScript->eType == eScriptType_Chance )
			{
				nChance = pScript->fSub;
			}

			// rangeType
			if ( pScript->eType == eScriptType_Skill_rangeType )
			{
					 if ( (KDint) pScript->fSub == eRangeType_Short )	bLoopRangeShort	= true;
				else if ( (KDint) pScript->fSub == eRangeType_Back  )	bLoopRangeBack	= true;	
			}
		}

		// total chance
		if ( ( bRangeShort == false && bLoopRangeShort == true ) || ( bRangeBack == false && bLoopRangeBack == true ) )
		{

		}
		else
		{
			aActions.push_back ( m_aActions [ i ] );
			aActionIndexs.push_back ( i );
			nTotalCance += nChance;
		}
	}

	KDint	nCheckChance = kdRand ( ) % nTotalCance;
	KDint	nChanceSum = 0;

	for ( KDuint i = 0; i < aActions.size ( ); i++ )
	{
		cUnitAction*	pAction = aActions [ i ];
		KDint			nChance  = 10;

		for ( auto pScript : pAction->aScripts )
		{
			if ( pScript->eType == eScriptType_Chance )
			{
				nChance = pScript->fSub;
				break;
			}
		}

		nChanceSum += nChance;

		if ( nCheckChance < nChanceSum )
		{
			return aActionIndexs [ i ];
		}
	}

	return -1;
}

cUnitScript* cUnit::getScript ( KDint nActionIndex, KDint nScriptIndex )
{
	// check
	if ( nActionIndex < 0 || nActionIndex >= (KDint) m_aActions.size ( ) ||
		 nScriptIndex < 0 || nScriptIndex >= (KDint) m_aActions [ nActionIndex ]->aScripts.size ( ) )
	{
		return nullptr;
	}

	return m_aActions [ nActionIndex ]->aScripts [ nScriptIndex ];
}

KDvoid cUnit::changeAni ( eUnitAniType eAniType )
{
	m_pAnis->changeAni ( eAniType);
	m_pAnis->start ( );
}

KDvoid cUnit::aniStart ( KDvoid )
{
	m_pAnis->start ( );
}

KDvoid cUnit::aniStop ( KDvoid )
{
	m_pAnis->stop ( );
}

KDvoid cUnit::setAniNowFrame ( KDint nFrame )
{
	m_pAnis->setSelectAniNowFrame ( nFrame );
}

eUnitAniType cUnit::getAniType ( KDvoid )
{
	return (eUnitAniType) m_pAnis->getSelectnAniIndex ( );
}

KDint cUnit::getAniNowFrame ( KDvoid )
{
	return m_pAnis->getSelectAniNowFrame ( );
}

std::string	cUnit::getAniFileName ( KDvoid )
{
	return m_pAnis->getFileName ( );
}

KDvoid cUnit::setFlicker ( KDdouble dLeftTime )
{
	m_bIsFlicker = true;
	m_dFlickerLeftTime1	= dLeftTime;
	m_dFlickerLeftTime2 = 0.00;
}

KDvoid cUnit::setBetMoney ( KDint64 nMoney )
{
	m_nBetMoney = nMoney;
}

KDvoid cUnit::setDividendMagnification ( KDfloat fMagnification )
{
	KDfloat		a = ( ( fMagnification * 10 - (KDint) ( fMagnification * 10 ) ) / 10 ) - 0.001f;

	if ( a >= 0.05f )
	{
		fMagnification = fMagnification + 0.1f;
	}

	fMagnification -= a;

	m_fDividendMagnification = fMagnification;
}

KDvoid cUnit::setIsColorChange ( KDbool bIsChange )
{
	m_bIsColorChange = bIsChange;
}

KDvoid cUnit::setColor ( KDubyte a, KDubyte r, KDubyte g, KDubyte b )
{
	m_nImgAlpha	= a;
	m_nImgRed	= r;
	m_nImgGreen	= g;
	m_nImgBlue	= b;

	m_pAnis->setAlpha ( eUnitAniType_Normal , (KDubyte) m_nImgAlpha );
	m_pAnis->setAlpha ( eUnitAniType_Attack , (KDubyte) m_nImgAlpha );
	m_pAnis->setAlpha ( eUnitAniType_Special, (KDubyte) m_nImgAlpha );
	m_pAnis->setAlpha ( eUnitAniType_Damage , (KDubyte) m_nImgAlpha );

	m_pAnis->setColor ( eUnitAniType_Normal , m_nImgRed, m_nImgGreen, m_nImgBlue );
	m_pAnis->setColor ( eUnitAniType_Attack , m_nImgRed, m_nImgGreen, m_nImgBlue );
	m_pAnis->setColor ( eUnitAniType_Special, m_nImgRed, m_nImgGreen, m_nImgBlue );
	m_pAnis->setColor ( eUnitAniType_Damage , m_nImgRed, m_nImgGreen, m_nImgBlue );
}

KDvoid cUnit::setIsAutoChangeNor ( KDbool bIsAuto )
{
	m_bIsAutoChangeNor = bIsAuto;
}

KDbool cUnit::isRectInPoint ( Point tPoint )
{
	KDbool		bIsSucces = false;
	cFrame		tFrame;

	m_pAnis->getAni ( eUnitAniType_Normal )->getFrameData ( 0, bIsSucces, tFrame );
	
	if ( bIsSucces == false )
	{
		return false;
	}

	Point		tUnitPoint	= Point ( m_pAnis->getPoint ( ).x - tFrame.tCenter.x, m_pAnis->getPoint ( ).y );
	Rect		tUnitRect	= Rect ( tUnitPoint.x, tUnitPoint.y, tFrame.tRectSrc.size.width, tFrame.tRectSrc.size.height );

	return RectInPoint ( tUnitRect, tPoint );
}

KDvoid cUnit::updateCapablePoint ( KDvoid )
{
	m_nCapablePoint = 0;

	m_nCapablePoint += m_nHp  * _CAPABLE_POINT_HP_MULT;
	m_nCapablePoint += m_nAtk * _CAPABLE_POINT_ATK_MULT;
	m_nCapablePoint += m_nDef * _CAPABLE_POINT_DEF_MULT;
	m_nCapablePoint += m_nAgl * _CAPABLE_POINT_AGL_MULT;
	m_nCapablePoint += m_nCrt * _CAPABLE_POINT_CRT_MULT;
}

