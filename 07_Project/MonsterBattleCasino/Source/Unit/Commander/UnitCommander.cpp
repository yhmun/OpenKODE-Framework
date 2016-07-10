/* -----------------------------------------------------------------------------------
 *
 *      File            UnitCommander.cpp
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
#include "UnitCommander.h"
#include "MyData/MyData.h"

cUnitCommander::cUnitCommander ( Node* pNode, eModeType eMode, std::vector<cSprEffects*>& rListEffects, KDint nLabelDamageZ1, KDint nLabelDamageZ2, KDint nLabelDamageZ3, KDint nStatusEffectZ1, KDint nStatusEffectZ2, KDint nStatusEffectZ3 )
: m_pNode						( pNode			   )
, m_eGameType					( eGameType_Casino )
, m_eModeType					( eMode			   )
, m_nUnitCreateCount			( 0				   )
, m_nUnitCreateLastSeatIndex	( -1			   )
, m_dUnitCreateLeftTime			( 2.5f			   )
, m_nUnitCreateDirectSeatIndex	( -1			   )
, m_pSprExpUp					( nullptr		   )
, m_nUnitCreateDirectStep		( -1			   )
, m_dUnitCreateDirectTime		( 0				   )
, m_bIsGameEndByForce			( false			   )
{
	// effects
	for ( auto pNode : rListEffects )
	{
		pNode->retain ( );
		m_aListEffects.push_back ( pNode );
	}

	// label damages
	for ( KDint i = 0; i < 3; i++ )
	{
		KDint	z = 0;
		switch ( i )
		{
			case 0	:	z = nLabelDamageZ1;		break;
			case 1	:	z = nLabelDamageZ2;		break;
			case 2	:	z = nLabelDamageZ3;		break;
		}

		for ( KDint a = 0; a < 20; a++ )
		{
			sLabelDamage	tLabelDamage;

			tLabelDamage.bIsMove		= false;
			tLabelDamage.nDamageType	= 0;
			tLabelDamage.nDamage		= 0;
			tLabelDamage.nMoveType		= 0;
			tLabelDamage.tPoint			= Point::ZERO;
			tLabelDamage.fMoveY			= 0;
			tLabelDamage.fBottomY		= 0;
			tLabelDamage.dLastLeftTime	= 0;

			// sprite
			tLabelDamage.pSprCri = new cSprite ( m_pNode, "Game/game_damage_num_cri_text.png", z, Point ( 0, 0 ), Point ( 0, 0 ) );
			tLabelDamage.pSprCri->get ( )->setVisible ( false );

			// bad
			tLabelDamage.pLabelBad = new cLabelAtlasCustom ( m_pNode, "Game/game_damage_num_bad.png", 21, z + 1 );
			tLabelDamage.pLabelBad->addCharInfo ( '0', 22 );	tLabelDamage.pLabelBad->addCharInfo ( '1', 15 );	tLabelDamage.pLabelBad->addCharInfo ( '2', 22 );
			tLabelDamage.pLabelBad->addCharInfo ( '3', 22 );	tLabelDamage.pLabelBad->addCharInfo ( '4', 22 );	tLabelDamage.pLabelBad->addCharInfo ( '5', 22 );
			tLabelDamage.pLabelBad->addCharInfo ( '6', 22 );	tLabelDamage.pLabelBad->addCharInfo ( '7', 22 );	tLabelDamage.pLabelBad->addCharInfo ( '8', 22 );
			tLabelDamage.pLabelBad->addCharInfo ( '9', 22 );
			tLabelDamage.pLabelBad->setVisible ( false );

			// critical bad
			tLabelDamage.pLabelCriBad = new cLabelAtlasCustom ( m_pNode, "Game/game_damage_num_cri_bad.png", 24, z + 1 );
			tLabelDamage.pLabelCriBad->addCharInfo ( '0', 24 );	tLabelDamage.pLabelCriBad->addCharInfo ( '1', 16 );	tLabelDamage.pLabelCriBad->addCharInfo ( '2', 24 );
			tLabelDamage.pLabelCriBad->addCharInfo ( '3', 25 );	tLabelDamage.pLabelCriBad->addCharInfo ( '4', 25 );	tLabelDamage.pLabelCriBad->addCharInfo ( '5', 24 );
			tLabelDamage.pLabelCriBad->addCharInfo ( '6', 24 );	tLabelDamage.pLabelCriBad->addCharInfo ( '7', 23 );	tLabelDamage.pLabelCriBad->addCharInfo ( '8', 24 );
			tLabelDamage.pLabelCriBad->addCharInfo ( '9', 24 );
			tLabelDamage.pLabelCriBad->setVisible(false);

			// good
			tLabelDamage.pLabelGood = new cLabelAtlasCustom ( m_pNode, "Game/game_damage_num_good.png", 21, z + 1 );
			tLabelDamage.pLabelGood->addCharInfo ( '0', 22 );	tLabelDamage.pLabelGood->addCharInfo ( '1', 15 );	tLabelDamage.pLabelGood->addCharInfo ( '2', 22 );
			tLabelDamage.pLabelGood->addCharInfo ( '3', 22 );	tLabelDamage.pLabelGood->addCharInfo ( '4', 22 );	tLabelDamage.pLabelGood->addCharInfo ( '5', 22 );
			tLabelDamage.pLabelGood->addCharInfo ( '6', 22 );	tLabelDamage.pLabelGood->addCharInfo ( '7', 22 );	tLabelDamage.pLabelGood->addCharInfo ( '8', 22 );
			tLabelDamage.pLabelGood->addCharInfo ( '9', 22 );
			tLabelDamage.pLabelGood->setVisible ( false );

			// push
			switch ( i )
			{
				case 0	:	m_aLabelDamages1.push_back ( tLabelDamage );	break;
				case 1	:	m_aLabelDamages2.push_back ( tLabelDamage );	break;
				case 2	:	m_aLabelDamages3.push_back ( tLabelDamage );	break;
			}
		}

	}

	// status effects
	for ( KDint i = 0; i < 3; i++ )
	{
		KDint	z = 0;

		switch ( i )
		{
			case 0	:	z = nStatusEffectZ1;	break;
			case 1	:	z = nStatusEffectZ2;	break;
			case 2	:	z = nStatusEffectZ3;	break;
		}

		for ( KDint a = 0; a < 30; a++ )
		{
			sStatusEffect	tStatusEffect;

			tStatusEffect.bIsView	= false;
			tStatusEffect.dLeftTime	= 0;
			tStatusEffect.pAni		= new cSprAni ( m_pNode, "Game/status_effects.png", z, Point ( 0, 0 ) );
			tStatusEffect.tPoint	= Point::ZERO;

			tStatusEffect.pAni->addFrame ( Rect (   0,  0, 73, 30 ), 80, false, false, Point ( 36, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect (  73,  0, 73, 30 ), 80, false, false, Point ( 36, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect ( 146,  0, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect ( 213,  0, 73, 30 ), 80, false, false, Point ( 36, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect ( 286,  0, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect (   0, 30, 73, 30 ), 80, false, false, Point ( 36, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect (  73, 30, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect ( 140, 30, 73, 30 ), 80, false, false, Point ( 36, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect ( 213, 30, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
			tStatusEffect.pAni->addFrame ( Rect ( 280, 30, 73, 30 ), 80, false, false, Point ( 36, 30 ), Rect::ZERO, Rect::ZERO );

			tStatusEffect.pAni->setVisible ( false );
			tStatusEffect.pAni->setFrameInit ( 0 );
			tStatusEffect.pAni->setLoop ( false );
			tStatusEffect.pAni->stop ( );

			switch ( i )
			{
				case 0	:	m_aStatusEffects1.push_back ( tStatusEffect );	break;
				case 1	:	m_aStatusEffects2.push_back ( tStatusEffect );	break;
				case 2	:	m_aStatusEffects3.push_back ( tStatusEffect );	break;
			}
		}
	}

	// exp up effect
	m_pSprExpUp = new cSprite ( m_pNode, "Game/plus_exp.png", nStatusEffectZ3, Point::ZERO, Point ( 0, 0 ) );
	m_pSprExpUp->get ( )->setOpacity ( 0 );

}

cUnitCommander::~cUnitCommander ( KDvoid )
{
	// mobs
	SAFE_RELEASE_ARRAYS ( m_aMobs );

	// effects
	SAFE_RELEASE_ARRAYS ( m_aListEffects );

	// active gages
	m_aMobActiveGages.clear ( );

	// active
	m_aActives.clear ( );

	// damage label effect
	for ( auto& rNode : m_aLabelDamages1 )
	{
		CC_SAFE_RELEASE ( rNode.pLabelBad	 );
		CC_SAFE_RELEASE ( rNode.pLabelCriBad );
		CC_SAFE_RELEASE ( rNode.pLabelGood	 );
	}
	for ( auto& rNode : m_aLabelDamages2 )
	{
		CC_SAFE_RELEASE ( rNode.pLabelBad	 );
		CC_SAFE_RELEASE ( rNode.pLabelCriBad );
		CC_SAFE_RELEASE ( rNode.pLabelGood	 );
	}
	for ( auto& rNode : m_aLabelDamages3 )
	{
		CC_SAFE_RELEASE ( rNode.pLabelBad	 );
		CC_SAFE_RELEASE ( rNode.pLabelCriBad );
		CC_SAFE_RELEASE ( rNode.pLabelGood	 );
	}
	
	m_aLabelDamages1.clear ( );
	m_aLabelDamages2.clear ( );
	m_aLabelDamages3.clear ( );
	m_aReservationlabelDamages.clear ( );

	// status effects
	for ( auto& rNode : m_aStatusEffects1 )		
	{
		CC_SAFE_RELEASE ( rNode.pAni ); 
	}	
	for ( auto& rNode : m_aStatusEffects2 )	
	{
		CC_SAFE_RELEASE ( rNode.pAni );	
	}
	for ( auto& rNode : m_aStatusEffects3 )	
	{
		CC_SAFE_RELEASE ( rNode.pAni );
	}
	m_aStatusEffects1.clear ( );
	m_aStatusEffects2.clear ( );
	m_aStatusEffects3.clear ( );

	// time effects
	m_aTimeEffects.clear ( );
	m_aReservationEffects.clear ( );

	// Reservation effects
	m_aReservationEffects.clear ( );

	// exp up effect
	CC_SAFE_RELEASE ( m_pSprExpUp );
}

KDvoid cUnitCommander::update ( KDdouble dLeftTickCount, KDbool& rIsCreate )
{
	// hero
	for ( auto pNode : m_aHeros )
	{
		pNode->update ( dLeftTickCount );		
	}

	// mobs
	for ( auto pNode : m_aMobs )
	{
		if ( pNode )
		{
			pNode->update ( dLeftTickCount );
		}
	}

	// time effect
	updateTimeEffect	( dLeftTickCount );

	// unit turn
	updateUnitTurn		( dLeftTickCount );

	//DamageLabel
	updateDamageLabel	( dLeftTickCount );

	//reservation
	updateReservation	( dLeftTickCount );

	//status effect
	updateStatusEffect	( dLeftTickCount );

	//exp effect
	updateExpEffect		( dLeftTickCount );

	// unit create
	rIsCreate = updateUnitCreate ( dLeftTickCount );
}

KDvoid cUnitCommander::updateUnitTurn ( KDdouble dLeftTickCount )
{
	// hero
	for ( auto pHero : m_aHeros )
	{
		KDdouble					dLeftTime = 0;
		std::vector<sTurnResult>	aTurnResults;

		// turn
		pHero->turn ( dLeftTickCount, aTurnResults );

		// damage label
		for ( auto& rResultNode : aTurnResults )
		{
			if ( dLeftTime == 0 )
			{
					 if ( rResultNode.nHp == -100000 )	viewDamageLabel ( pHero->getMobLineIndex(), 3, pHero->getPoint ( ),  0 );
				else if ( rResultNode.nHp <		   0 )	viewDamageLabel ( pHero->getMobLineIndex(), 0, pHero->getPoint ( ), -rResultNode.nHp );
				else if ( rResultNode.nHp >		   0 )	viewDamageLabel ( pHero->getMobLineIndex(), 3, pHero->getPoint ( ),  rResultNode.nHp );
			}
			else
			{
				KDdouble	dLeftTime2	= dLeftTime;
				KDint		nDamageType	= 0;
				Point		tPoint		= pHero->getPoint ( );
				KDint		nDamage		= 0;

					 if ( rResultNode.nHp == -100000 )	{	nDamageType = 3;	nDamage =  0; }
				else if ( rResultNode.nHp <		   0 )	{	nDamageType = 0;	nDamage = -rResultNode.nHp;	}
				else if ( rResultNode.nHp >		   0 )	{	nDamageType = 3;	nDamage =  rResultNode.nHp;	}

				addDamageLabelReservation ( pHero->getMobLineIndex ( ), dLeftTime2, nDamageType, tPoint, nDamage );
			}

			dLeftTime += 0.2;
		}

		// clear
		aTurnResults.clear ( );

		// die check
		if ( pHero->getDie ( ) == true )
		{
			delTimeEffect ( eUnitType_Hero, pHero->getUnitID ( ), -1 );
		}
	}

	// mob
	for ( auto pMob : m_aMobs )
	{
		// init
		KDdouble					dLeftTime = 0;
		std::vector<sTurnResult>	aTurnResults;

		// check
		if ( pMob == nullptr )
		{
			continue;
		}

		// turn
		pMob->turn ( dLeftTickCount, aTurnResults );

		// damage label
		for ( auto& rResultNode : aTurnResults )
		{
			if ( dLeftTime == 0 )
			{
					 if ( rResultNode.nHp == -100000 )	viewDamageLabel ( pMob->getMobLineIndex ( ), 3, pMob->getPoint ( ),  0 );
				else if ( rResultNode.nHp <		   0 )	viewDamageLabel ( pMob->getMobLineIndex ( ), 0, pMob->getPoint ( ), -rResultNode.nHp );
				else if ( rResultNode.nHp >		   0 )	viewDamageLabel ( pMob->getMobLineIndex ( ), 3, pMob->getPoint ( ),  rResultNode.nHp );
			}
			else
			{
				KDdouble	dLeftTime2	= dLeftTime;
				KDint		nDamageType = 0;
				Point		tPoint		= pMob->getPoint ( );
				KDint		nDamage		= 0;

					 if ( rResultNode.nHp == -100000 )	{ nDamageType = 3; nDamage =  0; }
				else if ( rResultNode.nHp <		   0 )	{ nDamageType = 0; nDamage = -rResultNode.nHp; }
				else if ( rResultNode.nHp >		   0 )	{ nDamageType = 3; nDamage =  rResultNode.nHp; }

				addDamageLabelReservation ( pMob->getMobLineIndex ( ), dLeftTime2, nDamageType, tPoint, nDamage );
			}

			dLeftTime += 0.2;
		}

		// clear
		aTurnResults.clear ( );

		// die check
		if ( pMob->getDie ( ) == true )
		{
			// 해당 몬서터를 죽인 몬스터가 내가 키우는 몬스터라면 exp를 올려주고 exp effect를 보여줘야 한다.
			KDint		nKilledUnitSeatIndex = pMob->getKillMeUnitSeatIndex ( );
			if ( nKilledUnitSeatIndex >= 0 && nKilledUnitSeatIndex < (KDint) m_aMobs.size ( ) )
			{
				if ( m_aMobs [ nKilledUnitSeatIndex ]->getUnitID ( ) > 0 )
				{
					m_aMobs [ nKilledUnitSeatIndex ]->setKillCount ( m_aMobs [ nKilledUnitSeatIndex ]->getKillCount ( ) + 1 );
					++cMyData::getObj ( )->m_nGamingKillCount;

					cMyData::getObj ( )->gamingExpAdd ( m_aMobs [ nKilledUnitSeatIndex ]->getCapablePoint ( ), pMob->getCapablePoint ( ) );

					viewExpUpEffect ( m_aMobs [ nKilledUnitSeatIndex ]->getPoint ( ) + m_aMobs [ nKilledUnitSeatIndex ]->getPointSpecial ( ) + Point ( -27, -8 ) );

					pMob->setKillMeUnitSeatIndex ( -1 );
				}
			}

			m_aMobActiveGages [ pMob->getSeatIndex ( ) ] = 0;
			delTimeEffect ( eUnitType_Mob, -1, pMob->getSeatIndex ( ) );
		}
	}
}

KDvoid cUnitCommander::updateTimeEffect ( KDdouble dLeftTickCount )
{
	//init
	std::vector<sTimeEffect>::iterator	pNode = m_aTimeEffects.begin ( );

	// loop
	while ( pNode != m_aTimeEffects.end ( ) )
	{
		// init
		sTimeEffect&	rTimeEffect = *pNode;

		// left time
		rTimeEffect.dLeftTime -= dLeftTickCount;

		// left time check
		if ( rTimeEffect.dLeftTime > 0 )
		{
			++pNode;
			continue;
		}

		// time & count
		rTimeEffect.dLeftTime = rTimeEffect.dTime;
		--rTimeEffect.nLeftCount;

		// add effect
		if ( rTimeEffect.nEffectIndex <  0							     )	rTimeEffect.nEffectIndex = 0;
		if ( rTimeEffect.nEffectIndex <= (KDint) m_aListEffects.size ( ) )	rTimeEffect.nEffectIndex = m_aListEffects.size ( ) - 1;
		m_aListEffects [ rTimeEffect.nEffectIndex ]->addEffect ( rTimeEffect.sEffectName.c_str ( ), rTimeEffect.tPoint );

		// next node
		if ( rTimeEffect.nLeftCount <= 0 )	
		{
			pNode = m_aTimeEffects.erase ( pNode );
		}
		else					
		{
			++pNode;
		}
	}
}

KDvoid cUnitCommander::updateDamageLabel ( KDdouble dLeftTickCount )
{
	// check
	if ( dLeftTickCount == 0 )
	{
		return;
	}

	// update
	for ( KDint i = 0; i < 3; i++ )
	{
		std::vector<sLabelDamage>*		pLabelDamages = nullptr;

		switch ( i )
		{
			case 0	:	pLabelDamages = &m_aLabelDamages1;	break;
			case 1	:	pLabelDamages = &m_aLabelDamages2;	break;
			case 2	:	pLabelDamages = &m_aLabelDamages3;	break;
		}

		for ( auto& rLabelDamage : *pLabelDamages )
		{
			// check
			if ( rLabelDamage.bIsMove == false )
			{
				continue;
			}

			// set
			if ( rLabelDamage.nMoveType == 0 || rLabelDamage.nMoveType == 2 )
			{
				rLabelDamage.tPoint.y += rLabelDamage.fMoveY * dLeftTickCount;
				rLabelDamage.fMoveY -= dLeftTickCount * 1000;

				switch ( rLabelDamage.nDamageType )
				{
					case 0	:	rLabelDamage.pLabelBad		 ->setPosition ( rLabelDamage.tPoint );		break;
					case 1	:	rLabelDamage.pLabelCriBad	 ->setPosition ( rLabelDamage.tPoint );		break;
					case 2	:	rLabelDamage.pSprCri->get ( )->setPosition ( rLabelDamage.tPoint );		break;
					case 3	:	rLabelDamage.pLabelGood		 ->setPosition ( rLabelDamage.tPoint );		break;
				}

				if ( rLabelDamage.fMoveY < 3 )
				{
					if ( rLabelDamage.nMoveType == 0 && rLabelDamage.nDamageType == 2 )
					{
						viewDamageLabel ( i, 1, rLabelDamage.tPointBackup, rLabelDamage.nDamage );
					}

					rLabelDamage.fMoveY = 1;
					++rLabelDamage.nMoveType;
				}
			}
			else if ( rLabelDamage.nMoveType == 1 || rLabelDamage.nMoveType == 3 )
			{
				rLabelDamage.tPoint.y -= rLabelDamage.fMoveY * dLeftTickCount;
				rLabelDamage.fMoveY += dLeftTickCount * 1000;

				if ( rLabelDamage.tPoint.y < rLabelDamage.fBottomY )
				{
					if ( rLabelDamage.nMoveType == 3 )
					{
						rLabelDamage.pSprCri->get ( )->setVisible ( false );
					}

					rLabelDamage.tPoint.y = rLabelDamage.fBottomY;
					rLabelDamage.fMoveY = 90;
					++rLabelDamage.nMoveType;
				}

				switch ( rLabelDamage.nDamageType )
				{
					case 0	:	rLabelDamage.pLabelBad		 ->setPosition ( rLabelDamage.tPoint );		break;
					case 1	:	rLabelDamage.pLabelCriBad	 ->setPosition ( rLabelDamage.tPoint );		break;
					case 2	:	rLabelDamage.pSprCri->get ( )->setPosition ( rLabelDamage.tPoint );		break;
					case 3	:	rLabelDamage.pLabelGood		 ->setPosition ( rLabelDamage.tPoint );		break;
				}
			}
			else
			{
				rLabelDamage.dLastLeftTime -= dLeftTickCount;

				if ( rLabelDamage.dLastLeftTime < 0 )
				{
					rLabelDamage.bIsMove = false;

					rLabelDamage.pLabelBad		 ->setVisible ( false );
					rLabelDamage.pLabelCriBad	 ->setVisible ( false );
					rLabelDamage.pLabelGood		 ->setVisible ( false );
					rLabelDamage.pSprCri->get ( )->setVisible ( false );
				}
			}
		}
	}
}

KDvoid cUnitCommander::updateStatusEffect ( KDdouble dLeftTickCount )
{
	// check
	if ( dLeftTickCount == 0 )
	{
		return;
	}

	std::list<sReservationStatusEffect>::iterator	pReservationNode = m_aStatusEffectsReservation.begin ( );
	while ( pReservationNode != m_aStatusEffectsReservation.end ( ) )
	{
		(*pReservationNode).dWaitLeftTime -= dLeftTickCount;

		if ( (*pReservationNode).dWaitLeftTime < 0 )
		{
			viewStatusEffect ( (*pReservationNode).nIndex, (*pReservationNode).eStatusType, (*pReservationNode).tPoint );

			pReservationNode = m_aStatusEffectsReservation.erase ( pReservationNode );
		}
		else
		{
			++pReservationNode;
		}
	}

	for ( KDint i = 0; i < 3; i++ )
	{
		std::vector<sStatusEffect>*		pStatusEffects = nullptr;
		switch ( i )
		{
			case 0	:	pStatusEffects = &m_aStatusEffects1;		break;
			case 1	:	pStatusEffects = &m_aStatusEffects2;		break;
			case 2	:	pStatusEffects = &m_aStatusEffects3;		break;
		}

		for ( auto& rStatusEffect : *pStatusEffects )
		{
			if ( rStatusEffect.bIsView == false )
			{
				continue;
			}

			rStatusEffect.pAni->setVisible ( true );

			rStatusEffect.dLeftTime -= dLeftTickCount;

			if ( rStatusEffect.dLeftTime < 0 )
			{
				rStatusEffect.bIsView = false;
				rStatusEffect.pAni->setVisible ( false );
				continue;
			}

			rStatusEffect.pAni->setPoint ( Point ( rStatusEffect.tPoint.x, rStatusEffect.tPoint.y + ( 1 - rStatusEffect.dLeftTime ) * 55 ) );

			if ( rStatusEffect.dLeftTime < 0.5f )
			{
				KDint		nAlpha = 255 * ( rStatusEffect.dLeftTime * 2 );
				if ( nAlpha > 255 )
				{
					nAlpha = 255;
				}
				rStatusEffect.pAni->setAlpha ( (KDubyte) nAlpha );
			}
		}
	}
}

KDvoid cUnitCommander::updateReservation ( KDdouble dLeftTickCount )
{
	//label damage effect
	std::vector<sReservationLabelDamage>::iterator	pLabelDamageNode = m_aReservationlabelDamages.begin ( );
	while ( pLabelDamageNode != m_aReservationlabelDamages.end ( ) )
	{
		(*pLabelDamageNode).dLeftTime -= dLeftTickCount;

		if ( (*pLabelDamageNode).dLeftTime > 0 )
		{
			++pLabelDamageNode;		
			continue;	
		}

		viewDamageLabel ( (*pLabelDamageNode).nLabelIndex, (*pLabelDamageNode).nDamageType, (*pLabelDamageNode).tPoint, (*pLabelDamageNode).nDamage	);

		pLabelDamageNode = m_aReservationlabelDamages.erase ( pLabelDamageNode );
	}

	// effect
	std::vector<sReservationEffect>::iterator	pEffectNode = m_aReservationEffects.begin ( );
	while ( pEffectNode != m_aReservationEffects.end ( ) )
	{
		(*pEffectNode).dLeftTime -= dLeftTickCount;

		if ( (*pEffectNode).dLeftTime > 0 )
		{
			++pEffectNode;	
			continue;	
		}

		if ( (*pEffectNode).nEffectIndex <  0 )	
		{
			(*pEffectNode).nEffectIndex = 0;
		}
		if ( (*pEffectNode).nEffectIndex <= (KDint) m_aListEffects.size ( ) )
		{
			(* pEffectNode).nEffectIndex = (KDint) m_aListEffects.size ( ) - 1;
		}
		m_aListEffects [ (*pEffectNode).nEffectIndex ]->addEffect ( (*pEffectNode).sEffectName.c_str ( ), (*pEffectNode).tPoint );

		pEffectNode = m_aReservationEffects.erase ( pEffectNode );
	}
}

KDvoid cUnitCommander::updateExpEffect ( KDdouble dLeftTickCount )
{
	KDint	nAlpha = m_pSprExpUp->get ( )->getOpacity ( );
	nAlpha -= 2;
	if ( nAlpha < 0 )
	{
		nAlpha = 0;
	}
	m_pSprExpUp->get ( )->setOpacity ( nAlpha );

	m_pSprExpUp->update ( dLeftTickCount );
}

KDbool cUnitCommander::updateUnitCreate ( KDdouble dLeftTickCount )
{
	// init
	KDbool		bIsCreate = false;

	// unit down 연출
	if ( m_nUnitCreateDirectStep == 0 )
	{
		// time
		m_dUnitCreateDirectTime -= dLeftTickCount;

		// percent
		KDdouble	dPercent = ( 0.3f - m_dUnitCreateDirectTime ) / 0.3f;	// max == 1
		if ( dPercent > 1 )
		{
			dPercent = 1;
		}

		// get unit
		cUnit*		pUnit = m_aMobs [ m_nUnitCreateDirectSeatIndex ];

		// unit
		if ( pUnit != nullptr )
		{
			// color
			pUnit->setColor ( dPercent * 255, 0, 0, 0 );
		}

		// end check
		if ( m_dUnitCreateDirectTime < 0 )
		{
			// unit
			if ( pUnit != nullptr )
			{
				// alpha
				pUnit->setColor ( 255, 0, 0, 0 );

				// ani start
				pUnit->aniStart ( );
			}

			// time
			m_dUnitCreateDirectTime = 0.3f;

			// switch
			++m_nUnitCreateDirectStep;
		}
	}
	else if ( m_nUnitCreateDirectStep == 1 )
	{
		// time
		m_dUnitCreateDirectTime -= dLeftTickCount;

		// percent
		KDdouble	dPercent = ( 0.3f - m_dUnitCreateDirectTime ) / 0.3f;	// max == 1
		if ( dPercent > 1 )
		{
			dPercent = 1;
		}

		// get unit
		cUnit*		pUnit = m_aMobs [ m_nUnitCreateDirectSeatIndex ];

		// unit
		if ( pUnit != nullptr )
		{
			// color
			pUnit->setColor ( 255, dPercent * 255, dPercent * 255, dPercent * 255 );
		}

		// end check
		if ( m_dUnitCreateDirectTime < 0 )
		{
			// unit
			if ( pUnit != nullptr )
			{
				// color
				pUnit->setColor ( 255, 255, 255, 255 );

				// ani start
				pUnit->aniStart ( );
			}

			// switch
			m_nUnitCreateDirectStep = -1;
		}
	}

	// check
	if ( m_eModeType != eModeType_MyRoom_Arcade_Easy && m_eModeType != eModeType_MyRoom_Arcade_Nor && m_eModeType != eModeType_MyRoom_Arcade_Hard && m_eModeType != eModeType_MyRoom_Survival )	
	{
		return bIsCreate;
	}

	// 살아있는 몹을 체크.
	KDint		nLiveCount = 0;
	for ( KDint i = 1; i < (KDint) m_aMobs.size ( ); i++ )
	{
		if ( m_aMobs [ i ] != nullptr && m_aMobs [ i ]->getDie ( ) == false )
		{
			++nLiveCount;
		}
	}

	// 자신 이외에 모든 유닛이 죽었다면 생성해준다.
	if ( nLiveCount == 0 )
	{
		if ( m_dUnitCreateLeftTime > 1.5f )
		{
			m_dUnitCreateLeftTime = 1.5f;
		}
	}

	bIsCreate = unitCreate ( dLeftTickCount );
	
	return bIsCreate;
}

void cUnitCommander::setDamageLabelColor ( KDint r, KDint g, KDint b )
{
	Color3B		tColor;

	tColor.r = r;
	tColor.g = g;
	tColor.b = b;

	for ( auto& rNode : m_aLabelDamages1 )
	{
		rNode.pSprCri->get ( )->setColor ( tColor );
		rNode.pLabelBad		  ->setColor ( 255, r, g, b );
		rNode.pLabelCriBad	  ->setColor ( 255, r, g, b );
		rNode.pLabelGood	  ->setColor ( 255, r, g, b );
	}

	for ( auto& rNode : m_aLabelDamages2 )
	{
		rNode.pSprCri->get ( )->setColor ( tColor );
		rNode.pLabelBad		  ->setColor ( 255, r, g, b );
		rNode.pLabelCriBad	  ->setColor ( 255, r, g, b );
		rNode.pLabelGood	  ->setColor ( 255, r, g, b );
	}

	for ( auto& rNode : m_aLabelDamages3 )
	{
		rNode.pSprCri->get ( )->setColor ( tColor );
		rNode.pLabelBad		  ->setColor ( 255, r, g, b );
		rNode.pLabelCriBad	  ->setColor ( 255, r, g, b );
		rNode.pLabelGood	  ->setColor ( 255, r, g, b );
	}
}

KDvoid cUnitCommander::setStatusEffectColor ( KDint r, KDint g, KDint b )
{
	for ( auto& rNode : m_aStatusEffects1 )	rNode.pAni->setColor ( r, g, b );
	for ( auto& rNode : m_aStatusEffects2 )	rNode.pAni->setColor ( r, g, b );
	for ( auto& rNode : m_aStatusEffects3 )	rNode.pAni->setColor ( r, g, b );
}

KDvoid cUnitCommander::setGameEndByForce ( KDvoid )
{
	m_bIsGameEndByForce = true;
}

KDbool cUnitCommander::getIsGameEnd ( KDvoid )
{
	if ( m_bIsGameEndByForce == true )
	{
		return true;
	}
	
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		KDint		nLiveCount = 0;
		for ( auto pNode : m_aMobs )
		{
			if ( pNode != nullptr && pNode->getDie ( ) == false )	
			{
				++nLiveCount;
			}
		}

		if ( nLiveCount <= 1 )
		{
			return true;
		}
	}
	else if ( m_eModeType == eModeType_MyRoom_Arcade_Easy || m_eModeType == eModeType_MyRoom_Arcade_Nor || m_eModeType == eModeType_MyRoom_Arcade_Hard )
	{
		if ( m_aMobs [ 0 ]->getDie ( ) == true )
		{
			return true;
		}

		if ( m_nUnitCreateCount == 8 )
		{
			KDint	nLiveCount = 0;

			for ( KDuint i = 1; i < m_aMobs.size ( ); i++ )
			{
				if ( m_aMobs [ i ] != nullptr && m_aMobs [ i ]->getDie ( ) == false )
				{
					++nLiveCount;
				}
			}

			if ( nLiveCount == 0 )
			{
				return true;
			}
		}
	}
	else if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		if ( m_aMobs [ 0 ]->getDie ( ) == true )
		{
			return true;
		}
	}

	return false;
}

KDint cUnitCommander::getLivHeroCount ( KDvoid )
{
	KDint	nLiveCount = 0;

	for ( auto  pNode : m_aHeros )
	{
		if ( pNode->getDie ( ) == false )
		{
			++nLiveCount;
		}
	}

	return nLiveCount;
}

cUnit* cUnitCommander::getUnit ( KDint nUnitID )
{
	for ( auto  pNode : m_aHeros )
	{
		if ( pNode->getUnitID ( ) == nUnitID )	
		{
			return pNode;
		}
	}

	return nullptr;
}

KDvoid cUnitCommander::addActive ( eUnitType eUnit, KDint nUnitID, KDint nSeatIndex, KDint nActionIndex, KDint nSelectHeroID, KDint nSelectMobSeatIndex )
{
	if ( eUnit == eUnitType_Hero )
	{
		cUnit*		pUnit = getUnit ( nUnitID );
		if ( pUnit == nullptr || pUnit->getDie ( ) == true )
		{
			return;
		}
	}
	else if ( eUnit == eUnitType_Mob )
	{
		if ( nSeatIndex < 0 || nSeatIndex >= (KDint) m_aMobs.size() || m_aMobs [ nSeatIndex ] == nullptr || m_aMobs [ nSeatIndex ]->getDie ( ) == true )
		{
			return;
		}
	}

	if ( nActionIndex == -1 )
	{
		return;
	}

	sActive			tActive;

	tActive.eUnit				= eUnit;
	tActive.nUnitID				= nUnitID;
	tActive.nSeatIndex			= nSeatIndex;
	tActive.nActionIndex		= nActionIndex;
	tActive.eRange				= eRangeType_Long_Long;
	tActive.nScriptIndex		= 0;
	tActive.nSelectHeroID		= nSelectHeroID;
	tActive.nSelectMobSeatIndex = nSelectMobSeatIndex;
	tActive.dLeftTime			= -1;
	tActive.bIsActionStart		= false;

	m_aActives.push_back ( tActive );
}

KDvoid cUnitCommander::delActive ( KDint nUnitID, KDint nSeatIndex )
{
	for ( std::vector<sActive>::iterator pNode = m_aActives.begin ( ); pNode != m_aActives.end ( ); ++pNode )
	{
		if ( (*pNode).nUnitID == nUnitID && (*pNode).nSeatIndex == nSeatIndex )
		{
			m_aActives.erase ( pNode );
			return;
		}
	}
}

KDvoid cUnitCommander::setActives ( const std::vector<sActive>& rActives )
{
	// clear
	m_aActives.clear ( );

	//set loop
	for ( auto rActive : rActives )
	{
		// push back
		m_aActives.push_back ( rActive );
	}
}

KDint cUnitCommander::getActiveMax ( KDvoid )
{
	return (KDint) m_aActives.size ( );
}

sActive* cUnitCommander::getActive ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aActives.size ( ) )
	{
		return nullptr;
	}

	return &m_aActives [ nIndex ];
}

sActive* cUnitCommander::getActiveFirst ( KDvoid )
{
	// 생성된 유닛이 없는지 체크한다.
	KDbool	bIsCreate = false;
	for ( auto pMob : m_aMobs )
	{
		if ( pMob != nullptr )
		{
			bIsCreate = true;
			break;
		}
	}

	if ( bIsCreate == false )
	{
		return nullptr;
	}

	// 유닛 생성 중인지 체크한다.
	if ( m_nUnitCreateDirectStep != -1 )
	{
		return nullptr;
	}

	// 행동 데이타가 없는지 확인한다.
	if ( m_aActives.size ( ) == 0 )
	{
		return nullptr;
	}

	// return
	return &m_aActives [ 0 ];
}

KDvoid cUnitCommander::getUnitLineCount ( KDint& rHeroFrontCount, KDint& rHeroBackCount, KDint& rMobLine1Count, KDint& rMobLine2Count, KDint& rMobLine3Count )
{
	rHeroFrontCount	= 0;
	rHeroBackCount	= 0;
	rMobLine1Count	= 0;
	rMobLine2Count	= 0;
	rMobLine3Count	= 0;

	for ( auto pHero : m_aHeros )
	{
		if ( pHero->getDie ( ) == false && pHero->getPositionType ( ) == ePositionType_Front )	++rHeroFrontCount;
		if ( pHero->getDie ( ) == false && pHero->getPositionType ( ) == ePositionType_Back	 )	++rHeroBackCount;
	}

	for ( auto pHero : m_aHeros )
	{
		if ( pHero->getDie ( ) == false ) 
		{
				 if ( pHero->getMobLineIndex ( ) == 0 )	++rMobLine1Count;
			else if ( pHero->getMobLineIndex ( ) == 1 )	++rMobLine2Count;
			else										++rMobLine3Count;
		}
	}
}

KDvoid cUnitCommander::setHpZeroDie ( KDvoid )
{
	// heros
	for ( auto pHero : m_aHeros )
	{
		if ( pHero->getDie ( ) ==false && pHero->getUseHp ( ) == 0 )
		{
			pHero->die ( 0 );
			delTimeEffect ( eUnitType_Hero, pHero->getUnitID ( ), -1 );
		}
	}

	// mobs
	for ( auto pMob : m_aMobs )
	{
		if ( pMob != NULL && pMob->getDie() == false && pMob->getUseHp ( ) == 0 )
		{
			pMob->die ( 0 );

			m_aMobActiveGages [ pMob->getSeatIndex ( ) ] = 0;
			delTimeEffect ( eUnitType_Mob, -1, pMob->getSeatIndex ( ) );
		}
	}
}

KDvoid cUnitCommander::viewEffect ( KDint nIndex, std::string sEffectName, Point tPoint )
{
	if ( nIndex < 0									)	nIndex = 0;
	if ( nIndex >= (KDint) m_aListEffects.size ( )	)	nIndex = (KDint) m_aListEffects.size ( ) - 1;

	m_aListEffects [ nIndex ]->addEffect ( sEffectName.c_str ( ), tPoint );
}

KDvoid cUnitCommander::setTimeEffects ( const std::vector<sTimeEffect>& rTimeEffects )
{
	// clear
	m_aTimeEffects.clear ( );

	// set
	for ( auto& rTimeEffect : rTimeEffects )
	{
		// push
		m_aTimeEffects.push_back ( rTimeEffect );
	}
}

KDvoid cUnitCommander::viewTimeEffect ( sTimeEffect& rTimeEffect )
{
	m_aTimeEffects.push_back ( rTimeEffect );
}

KDvoid cUnitCommander::delTimeEffectAll ( KDvoid )
{
	m_aTimeEffects.clear ( );
}

KDvoid cUnitCommander::delTimeEffect ( eUnitType eUnit, KDint nUnitID, KDint nSeatIndex )
{
	std::vector<sTimeEffect>::iterator	pNode = m_aTimeEffects.begin ( );
	while ( pNode != m_aTimeEffects.end ( ) )
	{
		sTimeEffect&	rTimeEffect = *pNode;

		if ( rTimeEffect.eUnit == eUnit && rTimeEffect.nUnitID == nUnitID && rTimeEffect.nSeatIndex == nSeatIndex )
		{
			pNode = m_aTimeEffects.erase ( pNode );
		}
		else
		{
			++pNode;
		}
	}
}

KDbool cUnitCommander::isEffect ( KDvoid )
{
	for ( auto pNode : m_aListEffects )
	{
		if ( pNode->isEffect ( ) == true )
		{
			return true;
		}
	}

	return false;
}

KDvoid cUnitCommander::initMobActiveGages ( KDint nSeatMax )
{
	for ( KDint i = 0; i < nSeatMax; i++ )
	{
		m_aMobActiveGages.push_back ( 0 );
	}
}

KDvoid cUnitCommander::setMobActiveGageZero ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aMobActiveGages.size ( ) )
	{
		return;
	}

	m_aMobActiveGages [ nIndex ] = 0;
}

KDvoid cUnitCommander::setMobActiveGages ( const std::vector<KDfloat>& rGages )
{
	for ( KDuint i = 0; i < rGages.size() && i < m_aMobActiveGages.size ( ); i++ )
	{
		m_aMobActiveGages [ i ] = rGages [ i ];
	}
}

const std::vector<float>& cUnitCommander::getMobActiveGages ( KDvoid )
{
	return m_aMobActiveGages;
}

KDfloat cUnitCommander::getMobActiveGage ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aMobActiveGages.size ( ) )
	{
		return 0;
	}

	return m_aMobActiveGages [ nIndex ];
}

KDvoid cUnitCommander::viewDamageLabel ( KDint nLabelIndex, KDint nType, Point tPoint, KDint nDamage )
{
	std::vector<sLabelDamage>*	pLabelDamages = nullptr;

	switch ( nLabelIndex )
	{
		case 0	:	pLabelDamages = &m_aLabelDamages1;	break;
		case 1	:	pLabelDamages = &m_aLabelDamages2;	break;
		default	:	pLabelDamages = &m_aLabelDamages3;	break;
	}

	for ( auto& rLabelDamage : *pLabelDamages )
	{
		// check
		if ( rLabelDamage.bIsMove == true )
		{
			continue;
		}

		// set
		rLabelDamage.bIsMove		= true;
		rLabelDamage.nDamageType	= nType;
		rLabelDamage.nDamage		= nDamage;
		rLabelDamage.nMoveType		= 0;
		rLabelDamage.fMoveY			= 200;
		rLabelDamage.dLastLeftTime	= 0.3;

		// set string
		const KDchar*	pStr = ccszf ( "%d", nDamage );
		switch ( rLabelDamage.nDamageType )
		{
			case 0	:	rLabelDamage.pLabelBad   ->setString ( pStr );	rLabelDamage.pLabelBad   ->setVisible ( true );		break;
			case 1	:	rLabelDamage.pLabelCriBad->setString ( pStr );	rLabelDamage.pLabelCriBad->setVisible ( true );		break;
			case 3	:	rLabelDamage.pLabelGood  ->setString ( pStr );	rLabelDamage.pLabelGood  ->setVisible ( true );		break;
		}

		// set position
		rLabelDamage.fBottomY	  = tPoint.y;
		rLabelDamage.tPointBackup = tPoint;
		rLabelDamage.tPoint		  = tPoint;

		switch ( rLabelDamage.nDamageType )
		{
			case 0	:	rLabelDamage.tPoint.x -= rLabelDamage.pLabelBad   ->getSize ( ).width / 2;	rLabelDamage.pLabelBad   ->setPosition ( rLabelDamage.tPoint );		break;
			case 1	:	rLabelDamage.tPoint.x -= rLabelDamage.pLabelCriBad->getSize ( ).width / 2;	rLabelDamage.pLabelCriBad->setPosition ( rLabelDamage.tPoint );		break;
			case 3	:	rLabelDamage.tPoint.x -= rLabelDamage.pLabelGood  ->getSize ( ).width / 2;	rLabelDamage.pLabelGood  ->setPosition ( rLabelDamage.tPoint );		break;
		}

		// sprite
		if ( rLabelDamage.nDamageType == 2 )
		{
			rLabelDamage.tPoint.x -= 50;
			rLabelDamage.pSprCri->get ( )->setVisible  ( true );
			rLabelDamage.pSprCri->get ( )->setPosition ( rLabelDamage.tPoint );
		}

		break;
	}
}

KDvoid cUnitCommander::addDamageLabelReservation ( KDint nLabelIndex, KDdouble dLeftTime, KDint nDamageType, Point tPoint, KDint nDamage )
{
	sReservationLabelDamage		tReservationLabelDamage;

	tReservationLabelDamage.nLabelIndex = nLabelIndex;
	tReservationLabelDamage.dLeftTime	= dLeftTime;
	tReservationLabelDamage.nDamageType = nDamageType;
	tReservationLabelDamage.tPoint		= tPoint;
	tReservationLabelDamage.nDamage		= nDamage;

	m_aReservationlabelDamages.push_back ( tReservationLabelDamage );
}

KDvoid cUnitCommander::viewStatusEffect ( KDint nIndex, eStatusEffect eStatusType, Point tPoint )
{
	if ( eStatusType < 0 || eStatusType >= eStatusEffect_Max )
	{
		return;
	}

	// play sound effect
	if ( cMyData::getObj ( )->m_bIsOptionSound != false )
	{
		switch ( eStatusType )
		{
			case eStatusEffect_Regene	:	break;
			case eStatusEffect_poison	:	break;
			case eStatusEffect_AtkUp	:	SimpleAudioEngine::getInstance ( )->playEffect ( "statusup.mp3"   );	break;
			case eStatusEffect_AtkDown	:	SimpleAudioEngine::getInstance ( )->playEffect ( "statusdown.mp3" );	break;
			case eStatusEffect_DefUp	:	SimpleAudioEngine::getInstance ( )->playEffect ( "statusup.mp3"   );	break;
			case eStatusEffect_DefDown	:	SimpleAudioEngine::getInstance ( )->playEffect ( "statusdown.mp3" );	break;
			case eStatusEffect_AglUp	:	SimpleAudioEngine::getInstance ( )->playEffect ( "statusup.mp3"   );	break;
			case eStatusEffect_AglDown	:	SimpleAudioEngine::getInstance ( )->playEffect ( "statusdown.mp3" );	break;
			case eStatusEffect_CrtUp	:	SimpleAudioEngine::getInstance ( )->playEffect ( "statusup.mp3"   );	break;
			case eStatusEffect_CrtDown	:	SimpleAudioEngine::getInstance ( )->playEffect ( "statusdown.mp3" );	break;
			case eStatusEffect_Max		:	break;
		}
	}

	std::vector<sStatusEffect>*		pStatusEffects = nullptr;
	switch ( nIndex )
	{
		case 0	:	pStatusEffects = &m_aStatusEffects1;	break;
		case 1	:	pStatusEffects = &m_aStatusEffects2;	break;
		default	:	pStatusEffects = &m_aStatusEffects3;	break;
	}

	for ( auto& rStatusEffect : *pStatusEffects )
	{
		if ( rStatusEffect.bIsView == true )
		{
			continue;
		}

		rStatusEffect.bIsView   = true;
		rStatusEffect.dLeftTime = 1;
		rStatusEffect.tPoint	   = tPoint;

		rStatusEffect.pAni->setFrameInit ( (KDint) eStatusType );
		rStatusEffect.pAni->setAlpha ( 255 );
		rStatusEffect.pAni->setPoint ( rStatusEffect.tPoint	);

		break;
	}
}

KDvoid cUnitCommander::viewStatusEffectReservation ( KDint nIndex, eStatusEffect eStatusType, Point tPoint, KDdouble dWaitTime )
{
	sReservationStatusEffect statusEffect_reservation;

	statusEffect_reservation.dWaitLeftTime	= dWaitTime;
	statusEffect_reservation.nIndex			= nIndex;
	statusEffect_reservation.eStatusType	= eStatusType;
	statusEffect_reservation.tPoint			= tPoint;

	m_aStatusEffectsReservation.push_back ( statusEffect_reservation );
}

KDvoid cUnitCommander::viewExpUpEffect ( Point tPoint )
{
	m_pSprExpUp->get ( )->setPosition ( tPoint );
	m_pSprExpUp->get ( )->setOpacity  ( 255 );
	m_pSprExpUp->setPointTime ( Point ( tPoint.x, tPoint.y + 40 ), 2 );
}

KDbool cUnitCommander::unitCreate ( KDdouble dLeftTime )
{
	// check
	if ( m_eModeType != eModeType_MyRoom_Arcade_Easy && m_eModeType != eModeType_MyRoom_Arcade_Nor && m_eModeType != eModeType_MyRoom_Arcade_Hard && m_eModeType != eModeType_MyRoom_Survival )
	{
		return false;
	}

	if ( m_eModeType != eModeType_MyRoom_Survival && m_nUnitCreateCount == 8 )	
	{
		return false;
	}

	if ( getIsGameEnd ( ) == true )
	{
		return false;
	}

	// 모든 유닛이 살아있다면 유닛을 생성하지 않는다.
	KDint				nLiveCount = 0;
	std::vector<KDint>	aEmptySeatIndexs;
	for ( KDint i = 1; i < (KDint) m_aMobs.size(); i++ )
	{
		if ( m_aMobs [ i ] != nullptr && m_aMobs [ i ]->getDie ( ) == false )
		{
			++nLiveCount;
		}
		else
		{
			aEmptySeatIndexs.push_back ( i );
		}
	}
	if ( nLiveCount == 4 )
	{
		return false;
	}
	
	// 아케이드 모드에서 중간 보스, 마지막 보스는 모든 몬스터가 다 죽어야만 생성한다.
	if ( m_eModeType != eModeType_MyRoom_Survival && ( m_nUnitCreateCount == 3 || m_nUnitCreateCount == 4 || m_nUnitCreateCount == 7 ) )
	{
		if ( nLiveCount > 0 )
		{
			return false;
		}
	}
	
	// time check
	if ( m_dUnitCreateLeftTime > 0 )
	{
		m_dUnitCreateLeftTime -= dLeftTime;
		return false;
	}
	
	// 서바이벌 모드는 비어있는 공간 아무곳이나 생성이 되고, 아케이드 모드는 가급적 순차적으로 유닛이 나온다.
	if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		m_nUnitCreateLastSeatIndex = aEmptySeatIndexs [ kdRand ( ) % aEmptySeatIndexs.size ( ) ];
	}
	else
	{
		if ( m_nUnitCreateCount == 3 || m_nUnitCreateCount == 7 )
		{
			m_nUnitCreateLastSeatIndex = 4;
		}
		else
		{
			// seat index 를 찾는다.
			if ( m_nUnitCreateLastSeatIndex == -1 )
			{
				m_nUnitCreateLastSeatIndex = 1;
			}
			else
			{
				++m_nUnitCreateLastSeatIndex;
				
				if ( m_nUnitCreateLastSeatIndex > 4 )
				{
					m_nUnitCreateLastSeatIndex = 1;
				}
			}
			
			KDbool	bIsFind = false;
			for ( KDuint i = 0; i < aEmptySeatIndexs.size ( ); i++ )
			{
				if ( m_nUnitCreateLastSeatIndex == aEmptySeatIndexs [ i ] )
				{
					bIsFind = true;
					break;
				}
			}
			
			if ( bIsFind == false )
			{
				m_nUnitCreateLastSeatIndex = aEmptySeatIndexs [ kdRand ( ) % aEmptySeatIndexs.size ( ) ];
			}
		}
	}

	// create info
	cUnitCreateInfos*		pMobCreate = nullptr;
	eCreateInfoFileType		eCreateInfoType = eCreateInfoFileType_Coin_1;
	if ( m_eModeType == eModeType_MyRoom_Arcade_Easy )
	{
			 if(m_nUnitCreateCount == 3)	{	pMobCreate = new cUnitCreateInfos("arcade_easy_middle_boss.par");	eCreateInfoType = eCreateInfoFileType_ArcadeEasyMiddleBoss;	}
		else if(m_nUnitCreateCount == 7)	{	pMobCreate = new cUnitCreateInfos("arcade_easy_last_boss.par"  );	eCreateInfoType = eCreateInfoFileType_ArcadeEasyLastBoss;	}
		else							{	pMobCreate = new cUnitCreateInfos("arcade_easy.par"			  );	eCreateInfoType = eCreateInfoFileType_ArcadeEasy;			}
	}
	else if ( m_eModeType == eModeType_MyRoom_Arcade_Nor )
	{
			 if(m_nUnitCreateCount == 3)	{	pMobCreate = new cUnitCreateInfos("arcade_normal_middle_boss.par");	eCreateInfoType = eCreateInfoFileType_ArcadeNorMiddleBoss;	}
		else if(m_nUnitCreateCount == 7)	{	pMobCreate = new cUnitCreateInfos("arcade_normal_last_boss.par"	);	eCreateInfoType = eCreateInfoFileType_ArcadeNorLastBoss;		}
		else							{	pMobCreate = new cUnitCreateInfos("arcade_normal.par"			);	eCreateInfoType = eCreateInfoFileType_ArcadeNor;				}
	}
	else if ( m_eModeType == eModeType_MyRoom_Arcade_Hard )
	{
			 if ( m_nUnitCreateCount == 3 )	{	pMobCreate = new cUnitCreateInfos("arcade_hard_middle_boss.par");	eCreateInfoType = eCreateInfoFileType_ArcadeHardMiddleBoss;	}
		else if ( m_nUnitCreateCount == 7 )	{	pMobCreate = new cUnitCreateInfos("arcade_hard_last_boss.par"  );	eCreateInfoType = eCreateInfoFileType_ArcadeHardLastBoss;	}
		else								{	pMobCreate = new cUnitCreateInfos("arcade_hard.par"			  );	eCreateInfoType = eCreateInfoFileType_ArcadeHard;			}
	}
	else if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		pMobCreate = new cUnitCreateInfos("survival.par");
		eCreateInfoType = eCreateInfoFileType_Survival;
	}

	//mob create
	const cUnitCreateInfo*	pCreateInfo = pMobCreate->getUnitInfoRand ( );
	cUnit*					pMob = new cUnit ( m_pNode, pCreateInfo, eCreateInfoType, LAYER_GAMEVIEW_MOB, -1, pCreateInfo->nCreateIndex, m_nUnitCreateLastSeatIndex );

	pMob->texLoad ( eUnitAniType_Normal  );
	pMob->texLoad ( eUnitAniType_Attack  );
	pMob->texLoad ( eUnitAniType_Special );
	pMob->texLoad ( eUnitAniType_Damage  );

	// shadow
	pMob->setIsShadow ( true );
	
	// change animation
	pMob->changeAni ( eUnitAniType_Normal );
	pMob->aniStop ( );

	// visible
	pMob->setVisible ( true );

	// alpha & color
	pMob->setColor ( 0, 0, 0, 0 );

	// mob position
	switch ( pMob->getSeatIndex ( ) )
	{
		case 0	:	pMob->setPoint ( Point (  50, 70 ) );		break;
		case 1	:	pMob->setPoint ( Point ( 145, 70 ) );		break;
		case 2	:	pMob->setPoint ( Point ( 240, 70 ) );		break;
		case 3	:	pMob->setPoint ( Point ( 334, 70 ) );		break;
		case 4	:	pMob->setPoint ( Point ( 429, 70 ) );		break;
	}

	// add
	m_aMobs [ pMob->getSeatIndex ( ) ] = pMob;

	//unit down 연출
	m_nUnitCreateDirectStep		 = 0;
	m_nUnitCreateDirectSeatIndex = pMob->getSeatIndex ( );
	m_dUnitCreateDirectTime		 = 0.3f;

	// left time
		 if ( m_eModeType == eModeType_MyRoom_Arcade_Easy )	m_dUnitCreateLeftTime = 20;
	else if ( m_eModeType == eModeType_MyRoom_Arcade_Nor  )	m_dUnitCreateLeftTime = 18;
	else if ( m_eModeType == eModeType_MyRoom_Arcade_Hard )	m_dUnitCreateLeftTime = 15;
	else													m_dUnitCreateLeftTime = 12;
	
	// unit count
	++m_nUnitCreateCount;

	return true;
}
