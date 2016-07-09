/* --------------------------------------------------------------------------
 *
 *      File            SGameLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-08
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
#include "SGameLayer.h"
#include "SLevelManager.h"

KDbool SGameLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*	pSprite = CCSprite::create ( "background.png" );
	pSprite->setPosition ( ccpMid ( m_tContentSize ) );
	this->addChild ( pSprite );

	m_pMapLayer = SMapLayer::create ( );
	m_pMapLayer->setDelegate ( this );
	m_pMapLayer->loadMapWithLevel ( SLevelManager::sharedLevelManager ( )->getLevel ( ) );
	m_pMapLayer->setPosition ( ccp ( 20, -15 ) );
	this->addChild ( m_pMapLayer, 1 );

	CCSprite*	pMenuBg = CCSprite::create ( "menu.png" );
	pMenuBg->setPosition ( ccp ( m_tContentSize.cx / 2, m_tContentSize.cy - 15 ) );
	this->addChild ( pMenuBg, 5 ); 

	m_pLevelLabel = CCLabelTTF::create ( "Level:0", "Marker Felt.ttf", 32 );
	m_pLevelLabel->setPosition ( ccp  ( 70, m_tContentSize.cy - 15 ) );
	m_pLevelLabel->setScale ( 0.6f );
	m_pLevelLabel->setColor ( ccYELLOW );
	this->addChild ( m_pLevelLabel, 5, 1 );
	this->setLevelCount ( SLevelManager::sharedLevelManager ( )->getCurrentLevel ( ) );

	m_pStepLabel = CCLabelTTF::create ( "000", "Marker Felt.ttf", 32 );
	m_pStepLabel->setScale ( 0.6f );
	m_pStepLabel->setColor ( ccYELLOW );
	m_pStepLabel->setPosition ( ccp  ( 230, m_tContentSize.cy - 15 ) );
	this->addChild ( m_pStepLabel, 5, 1 );
	this->setStepCount ( 0 );	

	CCMenuItemImage*	pPrevLevel = CCMenuItemImage::create ( "previous.png", "previous.png", this, menu_selector ( SGameLayer::onPrev ) );
	( (CCSprite*) pPrevLevel->getSelectedImage ( ) )->setOpacity ( 128 );
	pPrevLevel->setPosition ( ccp ( 0, 0 ) );

	CCMenuItemImage*	pNextLevel = CCMenuItemImage::create ( "next.png", "next.png", this, menu_selector ( SGameLayer::onNext ) );
	( (CCSprite*) pNextLevel->getSelectedImage ( ) )->setOpacity ( 128 );
	pNextLevel->setPosition ( ccp ( 40, 0 ) );

	CCMenuItemImage*	pResetLevel = CCMenuItemImage::create ( "reset.png", "reset.png", this, menu_selector ( SGameLayer::onReset ) );
	( (CCSprite*) pResetLevel->getSelectedImage ( ) )->setOpacity ( 128 );
	pResetLevel->setPosition ( ccp ( 80, 0 ) );
	
	CCMenu*		pMenu = CCMenu::create ( pPrevLevel, pNextLevel, pResetLevel, KD_NULL );
	pMenu->setPosition ( ccp  ( m_tContentSize.cx - 140, m_tContentSize.cy - 15 ) );
	this->addChild ( pMenu, 5, 1 ); 

	return KD_TRUE;
}

KDvoid SGameLayer::gameDidStart ( KDvoid )
{
    m_nStepCount = 0;    
}

KDvoid SGameLayer::gameDidFinish ( KDvoid )
{    
	this->scheduleOnce ( schedule_selector ( SGameLayer::nextLevel ), 1.0f );   
}

KDvoid SGameLayer::boxManDidMovedWithBox ( KDbool bWithBox )
{
	this->setStepCount ( m_nStepCount + 1 );

    if ( bWithBox )
	{
       
    } 
	else 
	{
     
    }
}

KDvoid SGameLayer::setLevelCount ( KDint nCount )
{
	m_pLevelLabel->setString ( ccszf ( "Level : %d", nCount + 1 ) );
    m_nLevelCount = nCount;
}

KDvoid SGameLayer::setStepCount ( KDint nCount )
{
	m_pStepLabel->setString ( ccszf ( "%03d", nCount ) );
	m_nStepCount = nCount;
}

KDvoid SGameLayer::nextLevel ( KDfloat fDelta )
{
	SLevelManager*		pManager = SLevelManager::sharedLevelManager ( );

	if ( pManager->hasNextLevel ( ) )
	{
		m_pMapLayer->loadMapWithLevel ( pManager->nextLevel ( ) );
		m_nLevelCount = pManager->getCurrentLevel ( );
	}
}

KDvoid SGameLayer::onPrev ( CCObject* pSender )
{
	SLevelManager*		pManager = SLevelManager::sharedLevelManager ( );

	if ( pManager->hasPrevLevel ( ) )
	{
		m_pMapLayer->loadMapWithLevel ( pManager->prevLevel ( ) );
		m_nLevelCount = pManager->getCurrentLevel ( );
	}
}

KDvoid SGameLayer::onNext ( CCObject* pSender )
{
	this->nextLevel ( KD_NULL );
}
		
KDvoid SGameLayer::onReset ( CCObject* pSender )
{
	m_pMapLayer->reloadMap ( );
}
