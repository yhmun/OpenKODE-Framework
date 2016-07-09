/* --------------------------------------------------------------------------
 *
 *      File            HudLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 13/05/2011
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "HudLayer.h"

KDbool HudLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	KDint		nMargin = 10;

	m_pLevelLabel = CCLabelTTF::create ( "init", "Verdana Bold.ttf", 18, ccs ( 100, 20 ), kCCAlignmentLeft );
	m_pLevelLabel->setPosition ( ccp ( nMargin + m_pLevelLabel->getContentSize ( ).cx / 2, m_pLevelLabel->getContentSize ( ).cy / 2 + nMargin ) );
	m_pLevelLabel->setColor ( ccc3 ( 255, 255, 255 ) );
	this->addChild ( m_pLevelLabel );

	m_pLivesLabel = CCLabelTTF::create ( "init", "Verdana Bold.ttf", 18, ccs ( 100, 20 ), kCCAlignmentRight );
	m_pLivesLabel->setPosition ( ccp ( m_tContentSize.cx - nMargin - m_pLivesLabel->getContentSize ( ).cx / 2, m_pLivesLabel->getContentSize ( ).cy / 2 + nMargin ) );
	m_pLivesLabel->setColor ( ccc3 ( 255, 255, 255 ) );
	this->addChild ( m_pLivesLabel );

	this->updateLives ( GameMgr->getPlayerLives ( ) );
	this->updateLevel ( GameMgr->getLevel ( ) );

	m_pSpeedUpSprite = CCSprite::create ( "SpeedUpIcon.png" );
	m_pSpeedUpSprite->setPosition ( ccp ( m_tContentSize.cx / 2 - m_pSpeedUpSprite->getContentSize ( ).cx / 2, m_pSpeedUpSprite->getContentSize ( ).cy / 2 + nMargin ) );
	m_pSpeedUpSprite->setVisible ( KD_FALSE );
	this->addChild ( m_pSpeedUpSprite );

	m_pTripleShotsSprite = CCSprite::create ( "TripleShotsIcon.png" );
	m_pTripleShotsSprite->setPosition ( ccp ( m_tContentSize.cx / 2 + m_pTripleShotsSprite->getContentSize ( ).cx / 2, m_pTripleShotsSprite->getContentSize ( ).cy / 2 + nMargin ) );
	m_pTripleShotsSprite->setVisible ( KD_FALSE );
	this->addChild ( m_pTripleShotsSprite );

	return KD_TRUE;
}

KDvoid HudLayer::updateLives ( KDint nLives )
{
	m_pLivesLabel->setString ( ccszf ( "Lives: %d", nLives ) );
}

KDvoid HudLayer::updateLevel ( KDint nLevel )
{
	m_pLevelLabel->setString ( ccszf ( "Level: %d", nLevel ) );
}

KDvoid HudLayer::showSpeedUpSprite ( KDbool bShow )
{
	m_pSpeedUpSprite->setVisible ( bShow );
}

KDvoid HudLayer::showTripleShotsSprite ( KDbool bShow )
{
	m_pTripleShotsSprite->setVisible ( bShow );
}

KDvoid HudLayer::gameOver ( KDvoid )
{
	m_pGameOverLabel = CCLabelTTF::create ( "Game Over", "GrusskartenGotisch.ttf", 72, ccs ( m_tContentSize.cx, 80 ), kCCAlignmentCenter );
	m_pGameOverLabel->setPosition ( ccp ( m_tContentSize.cx / 2, m_tContentSize.cy / 2 + m_pGameOverLabel->getContentSize ( ).cy / 2 ) );
	
	CCLabelTTF*		pTapLabel = CCLabelTTF::create ( "Tap to continue", "GrusskartenGotisch.ttf", 32, ccs ( m_tContentSize.cx, 40 ), kCCAlignmentCenter );
	pTapLabel->setPosition ( ccp ( m_tContentSize.cx / 2, m_tContentSize.cy / 2 - pTapLabel->getContentSize ( ).cy / 2 ) );
	this->addChild ( m_pGameOverLabel );
	this->addChild ( pTapLabel );

	CCDirector::sharedDirector ( )->pause ( );
}

KDvoid HudLayer::restartGame ( KDvoid )
{
	this->updateLives ( 3 );
	this->removeAllChildrenWithCleanup ( KD_TRUE );
}
