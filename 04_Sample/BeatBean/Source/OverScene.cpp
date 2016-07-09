/* --------------------------------------------------------------------------
 *
 *      File            OverScene.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      t2cn on 2009/10/13
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
#include "OverScene.h"
#include "GameScene.h"

KDbool OverScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCMenuItemFont::setFontSize ( 32 );
	CCMenuItemFont::setFontName ( "Arial.ttf" );

	CCLayerColor*	pLayer = CCLayerColor::create ( ccc4 ( 32, 32, 32, 255 ) );
	this->addChild ( pLayer, 1 ); 

	m_pTopScore = CCLabelTTF::create ( "0", "Arial.ttf", 32 );
	this->addChild ( m_pTopScore, 3 );
	m_pTopScore->setColor ( ccc3 ( 255, 0, 0 ) );
	m_pTopScore->setPosition ( ccp ( 160, 370 - 50 ) );	

	m_pScore = CCLabelTTF::create ( "0", "Arial.ttf", 32 );
	this->addChild ( m_pScore, 3 );
	m_pScore->setColor ( ccc3 ( 0, 0, 255 ) );
	m_pScore->setPosition ( ccp ( 160, 320 - 40 ) );	

	CCMenuItemFont*		pReStart = CCMenuItemFont::create ( "RESTART", this, menu_selector ( OverScene::onReStart ) );
	pReStart->setColor ( ccc3 ( 0x00, 0xCC, 0x00 ) );

	CCMenu*				pMenu = CCMenu::create ( pReStart, KD_NULL );	
	pMenu->setPosition ( ccpMid ( m_tContentSize ) );
	pMenu->alignItemsVerticallyWithPadding ( 20.0f );
	this->addChild ( pMenu, 2 );

	return KD_TRUE;
}

KDvoid OverScene::initTop ( KDvoid )
{
	m_pTopScore->setString ( ccszf ( "Best:%d" , m_pGame->getTopScore ( ) ) );
	m_pScore   ->setString ( ccszf ( "Score:%d", m_pGame->getScore ( ) ) );
}

KDvoid OverScene::setGame ( GameScene* pGame )
{
	m_pGame = pGame;
}

KDvoid OverScene::onReStart ( CCObject* pSender )
{
	m_pGame->initBubble ( );

	CCDirector::sharedDirector ( )->popScene ( );
}

