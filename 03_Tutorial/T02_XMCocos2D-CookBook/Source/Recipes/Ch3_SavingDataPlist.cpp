/* --------------------------------------------------------------------------
 *
 *      File            Ch3_SavingDataPlist.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "Ch3_SavingDataPlist.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch3_SavingDataPlist::init ( KDvoid )
{	
	if ( !SimpleTimedGameRecipe::init ( ) )
	{
		return KD_FALSE;
	}

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "mole.plist" );

	m_nTagCount = 0;
	m_pHiScores = KD_NULL;

	m_pMessage->setString ( "Welcome to Whack-A-Mole" );

	m_tMalletPosition = ccp ( 400, 400 );
	m_pMallet = CCSprite::createWithSpriteFrameName ( "mole_mallet.png" );
	m_pMallet->setPosition ( m_tMalletPosition );
	m_pMallet->setRotation ( 90 );
	m_pMallet->setScale ( 0.5f );
	this->addChild ( m_pMallet, 10, m_nTagCount );
	m_nTagCount++;

	this->initBackground ( );
	
	//Init moles
	m_pMoles = CCArray::create ( );
	m_pMoles->retain ( );

	// Add moles
	this->createMoleAtPosition ( ccp (  50, 205 ), 0 );
	this->createMoleAtPosition ( ccp ( 100, 205 ), 0 );
	this->createMoleAtPosition ( ccp ( 150, 205 ), 0 );
	this->createMoleAtPosition ( ccp ( 200, 205 ), 0 );
	this->createMoleAtPosition ( ccp ( 250, 205 ), 0 );

	this->createMoleAtPosition ( ccp (  75, 155 ), 1 );
	this->createMoleAtPosition ( ccp ( 125, 155 ), 1 );
	this->createMoleAtPosition ( ccp ( 175, 155 ), 1 );
	this->createMoleAtPosition ( ccp ( 225, 155 ), 1 );
	
	this->createMoleAtPosition ( ccp ( 100, 105 ), 2 );
	this->createMoleAtPosition ( ccp ( 150, 105 ), 2 );
	this->createMoleAtPosition ( ccp ( 200, 105 ), 2 );
	
	this->createMoleAtPosition ( ccp ( 125,  55 ), 3 );
	this->createMoleAtPosition ( ccp ( 175,  55 ), 3 );
	
	this->createMoleAtPosition ( ccp ( 150,   5 ), 4 );
		
	m_nMoleCount = m_pMoles->count ( );

	return KD_TRUE;
}

KDvoid Ch3_SavingDataPlist::onExit ( KDvoid )
{
	m_pMoles->release ( );

	CC_SAFE_RELEASE ( m_pHiScores );

	SimpleTimedGameRecipe::onExit ( );
}

KDvoid Ch3_SavingDataPlist::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_nGameState == GAME_OVER )
	{
		return;
	}
	
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	
	m_pMallet->stopAllActions ( );

	m_pMallet->runAction
	(
		CCSequence::create 
		(
			CCSpawn::create 
			(
				CCEaseSineInOut::create ( CCMoveTo::create ( 0.05f, tPoint ) ),
				CCEaseSineInOut::create ( CCRotateTo::create ( 0.05f, 0 ) ),
				KD_NULL 
			),
			CCSpawn::create 
			(
				CCEaseSineInOut::create ( CCMoveTo::create ( 0.25f, m_tMalletPosition ) ),
				CCEaseSineInOut::create ( CCRotateTo::create ( 0.25f, 90 ) ),
				KD_NULL 
			),
			KD_NULL
		)
	);
	
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pMoles, pObject )
	{
		Mole*	pMole = (Mole*) pObject;
		pMole->ccTouchesBegan ( pTouches, pEvent );
	}
}

KDvoid Ch3_SavingDataPlist::initBackground ( KDvoid )
{
	ccTexParams		tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
	
	CCPoint			aVertexArr [ ] = { ccp ( 0, 0 ), ccp ( 480, 0 ), ccp ( 480, 320 ), ccp ( 0, 320 ) };
	KDint			nNumVerts = 4;
	CCArray*		pVertices = CCArray::create ( );
	for ( KDint i = 0; i < nNumVerts; i++ )
	{
		pVertices->addObject ( CCPointValue::create ( ccp ( aVertexArr [ i ].x * 1, aVertexArr [ i ].y * 1 ) ) );
	}
	
	TexturedPolygon*	pTexturedPoly = TexturedPolygon::create ( "mole_bg.png", pVertices );
	pTexturedPoly->getTexture ( )->setTexParameters ( &tParams );
	pTexturedPoly->setPosition ( ccp ( 0, 0 ) );

	this->addChild ( pTexturedPoly, 0, m_nTagCount ); 
}

KDvoid Ch3_SavingDataPlist::createMoleAtPosition ( const CCPoint& tPoint, KDfloat fZOrder )
{	
	KDint			nZOrder = (KDint) fZOrder;

	CCSprite*		pBack = CCSprite::createWithSpriteFrameName ( "mole_back.png" );
	pBack->setPosition ( tPoint );
	this->addChild ( pBack, nZOrder, m_nTagCount );
	m_nTagCount++;
	
	Mole*			pMole = Mole::createWithSpriteFrameName ( "mole_normal.png" );
	pMole->setDownPosition ( ccp ( tPoint.x, tPoint.y - 30 ) );
	pMole->setDelegate ( this );
	this->addChild ( pMole, nZOrder, m_nTagCount );
	m_pMoles->addObject ( pMole );
	m_nTagCount++;

	CCSprite*		pFront = CCSprite::createWithSpriteFrameName ( "mole_front.png" );
	pFront->setPosition ( tPoint );
	this->addChild ( pFront, nZOrder, m_nTagCount );
	m_nTagCount++;
}

KDvoid Ch3_SavingDataPlist::processMoleHit ( KDvoid )
{
	m_nCurrentScore++;
	this->setCurrentScore ( );
}

KDvoid Ch3_SavingDataPlist::addHiScoresToMenu ( KDvoid )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pHiScores, pObject )
	{	
		CCDictionary*	pScore = (CCDictionary*) pObject;
		const KDchar*	szScore = ccszf ( "%s %s", ( (CCString*) pScore->objectForKey ( "name" ) )->getCString ( ), 
												   ( (CCString*) pScore->objectForKey ( "score" ) )->getCString ( ) );

		m_pHiScoresMenu->addChild ( CCMenuItemFont::create ( szScore ) );
	}
}

KDvoid Ch3_SavingDataPlist::loadHiScores ( KDvoid )
{
	const KDchar*	szFilename = "/data/whackamole.plist";

	// Our dictionary
	CCDictionary*	pFileDict;

	pFileDict = CCDictionary::createWithContentsOfFile ( szFilename );
	if ( !pFileDict )
	{
		// If it does we load it in the dict
		pFileDict = CCDictionary::createWithContentsOfFile ( "whackamole_template_xml.plist" );
	}

	// Load hi scores into our dictionary
	CC_SAFE_RELEASE ( m_pHiScores );

	CCArray*		m_pHiScores = (CCArray*) pFileDict->objectForKey ( "hiscores" );
	m_pHiScores->retain ( );
	
	// Set the 'hiScore' variable (the highest score)
	CCObject*		pObject;
	CCARRAY_FOREACH ( m_pHiScores, pObject )
	{
		CCDictionary*	pScore = (CCDictionary*) pObject;
		KDint			nScoreNum = ( (CCString*) pScore->objectForKey ( "score" ) )->intValue ( );

		if ( m_nHiScore < nScoreNum )
		{
			m_nHiScore = nScoreNum;
		}
	}

	// Write dict to file
//	pFileDict->writeToFile ( szFilename );
}

KDvoid Ch3_SavingDataPlist::addHiScore ( KDvoid )
{
	const KDchar*	szFilename = "/data/whackamole.plist";

	// Our dictionary
	CCDictionary*	pFileDict;

	pFileDict = CCDictionary::createWithContentsOfFile ( szFilename );
	if ( !pFileDict )
	{
		// If it does we load it in the dict
		pFileDict = CCDictionary::createWithContentsOfFile ( "whackamole_template_xml.plist" );
	}

	// Load hi scores into our dictionary
	CC_SAFE_RELEASE ( m_pHiScores );

	CCArray*		m_pHiScores = (CCArray*) pFileDict->objectForKey ( "hiscores" );
	m_pHiScores->retain ( );

	// Add hi score
	KDbool			bScoreRecorded = KD_FALSE;

	// Add score if player's name already exists
	CCObject*		pObject;
	CCARRAY_FOREACH ( m_pHiScores, pObject )
	{
		CCDictionary*	pScoreDict = (CCDictionary*) pObject;

		const KDchar*	szName = ( (CCString*) pScoreDict->objectForKey ( "name" ) )->getCString ( );
		if ( !kdStrcmp ( szName, m_sCurrentPlayerName.c_str ( ) ) )
		{
			KDint		nScore = ( (CCString*) pScoreDict->objectForKey ( "score" ) )->intValue ( );
			if ( nScore < m_nCurrentScore ) 
			{
				pScoreDict->setObject ( CCString::createWithFormat ( "%d", m_nCurrentScore ), "score" );
			}
			bScoreRecorded = KD_TRUE;
		}
	}

	// Add new score if player's name doesn't exist
	if ( !bScoreRecorded ) 
	{
		CCDictionary*	pNewScore = CCDictionary::create ( );
		pNewScore->setObject ( CCString::create ( m_sCurrentPlayerName ), "name" );
		pNewScore->setObject ( CCString::createWithFormat ( "%d", m_nCurrentScore ), "score" );
		m_pHiScores->addObject ( pNewScore );
	}
	
	// Write dict to file
//	pFileDict->writeToFile ( szFilename );
	
}

KDvoid Ch3_SavingDataPlist::deleteHiScores ( KDvoid )
{
	// Delete our file
	kdRemove ( "/data/whackamole.plist" );

	m_pMessage->setString ( "Hi scores deleted!" );
	
	m_nHiScore = 0;
	this->loadHiScores ( );
}

KDvoid Ch3_SavingDataPlist::startNewGame ( CCObject* pSender )
{
	SimpleTimedGameRecipe::startNewGame ( );
	
	m_pMessage->setString ( "WHACK A MOLE!" );
}

KDvoid Ch3_SavingDataPlist::gameOver ( KDvoid )
{
	SimpleTimedGameRecipe::gameOver ( );

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pMoles, pObject )
	{
		Mole*	pMole = (Mole*) pObject;
		pMole->startHideDown ( );
	}
}

KDvoid Ch3_SavingDataPlist::step ( KDfloat fDelta )
{
	SimpleTimedGameRecipe::step ( fDelta );
	
	if ( m_nGameState == MID_GAME )
	{
		KDint	nRandMod = (KDint) ( ( m_fGameTimer - 5 ) * 25 );
		if ( nRandMod < m_nMoleCount )
		{
			nRandMod = m_nMoleCount; 
		}
	
		KDint	nNum = kdRand ( ) % nRandMod;
		if ( nNum < m_nMoleCount )
		{
			Mole*	pMole = (Mole*) m_pMoles->objectAtIndex ( nNum );
			KDint	nState = pMole->getState ( );
			if ( nState == MOLE_DOWN )
			{
				pMole->startPopUp ( );
			}
			else if ( nState == MOLE_UP )
			{
				pMole->startHideDown ( );
			}
		}
	}
}