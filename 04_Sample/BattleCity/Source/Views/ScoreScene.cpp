/* --------------------------------------------------------------------------
 *
 *      File            ScoreScene.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/10). All rights reserved.
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
#include "ScoreScene.h"
#include "MainScene.h"
#include "WinScene.h"

CCScene* ScoreScene::scene ( KDint nSlow, KDint nQuike, KDint nStrong, KDint nStrongY, KDint nStrongG, KDint nLevel, KDint nKind, KDint nLife )
{
	CCScene*	pScene = CCScene::create ( );
	ScoreScene*	pLayer = new ScoreScene ( );

	if ( pScene && pLayer && pLayer->initWithNumber ( nSlow, nQuike, nStrong, nStrongY, nStrongG, nLevel, nKind, nLife ) )
	{
		pLayer->autorelease ( );	
		pScene->addChild ( pLayer );
	}
	else
	{
		CC_SAFE_DELETE ( pScene );
		CC_SAFE_DELETE ( pLayer );
	}

	return pScene;
}

KDbool ScoreScene::initWithNumber ( KDint nSlow, KDint nQuike, KDint nStrong, KDint nStrongY, KDint nStrongG, KDint nLevel, KDint nKind, KDint nLife )
{
	if ( !CCLayerColor::initWithColor ( ccc4 ( 0, 0, 0, 255 ) ) )
	{
		return KD_FALSE;
	}

	this->setVisibleViewport ( );	

	m_nSlow		= nSlow;
	m_nQuike	= nQuike;
	m_nStrong	= nStrong;
	m_nStrongYe	= nStrongY;
	m_nStrongG	= nStrongG;
	m_nLevel	= nLevel;
	m_nLife		= nLife;
	m_nKind		= nKind;
	m_tLyrSize	= this->getContentSize ( );

	CCLabelTTF*		pStageLabel = CCLabelTTF::create ( ccszf ( "STAGE %d", nLevel ), "Font/CourierBold.ttf", 25 );
	pStageLabel->setPosition ( ccp ( m_tLyrSize.cx / 2, m_tLyrSize.cy / 2 + 120 ) );
	this->addChild ( pStageLabel );      

	this->score1 ( );
	this->score2 ( );
	this->score3 ( );
	this->score4 ( );
	this->score5 ( );

	this->scoreSum ( );
    
	CCMenuItemFont::setFontName ( "Font/HelveticaBoldOblique.ttf" );
	CCMenuItemFont::setFontSize ( 25 );

	CCMenuItemFont*		pButton = CCMenuItemFont::create ( "NEXT", this, menu_selector ( ScoreScene::onButtonClicked ) );
	pButton->setPosition ( ccp ( m_tLyrSize.cx - 70, 30 ) );
	this->addChild ( CCMenu::createWithItem ( pButton ) );

	return KD_TRUE;
}

KDvoid ScoreScene::score1 ( KDvoid )
{
	CCSprite*		pTankSprite = CCSprite::createWithSpriteFrameName ( "en1.png" );
	pTankSprite->setScale ( 0.7f );
	pTankSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 100, m_tLyrSize.cy / 2 + 80 ) );
	this->addChild ( pTankSprite );
	
	CCSprite*		pSprite = CCSprite::create ( "Images/jiantou.png" );
	pSprite->setScale ( 1.5f );
	pSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 50, m_tLyrSize.cy / 2 + 80 ) );
	this->addChild ( pSprite );
    
	CCLabelTTF*		pScoreLabel = CCLabelTTF::create ( ccszf ( "%d", m_nSlow * 100 ), "Font/CourierBold.ttf", 20 );
	pScoreLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 10, m_tLyrSize.cy / 2 + 80 ) );
	this->addChild ( pScoreLabel );
    
    CCLabelTTF*		pNumLabel = CCLabelTTF::create ( ccszf ( "%dpts", m_nSlow ), "Font/CourierBold.ttf", 20 );
	pNumLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 90, m_tLyrSize.cy / 2 + 80 ) );
	this->addChild ( pNumLabel );  
}

KDvoid ScoreScene::score2 ( KDvoid )
{
	CCSprite*		pTankSprite = CCSprite::createWithSpriteFrameName ( "en2.png" );
	pTankSprite->setScale ( 0.7f );
	pTankSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 100, m_tLyrSize.cy / 2 + 50 ) );
	this->addChild ( pTankSprite );
	
	CCSprite*		pSprite = CCSprite::create ( "Images/jiantou.png" );
	pSprite->setScale ( 1.5f );
	pSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 50, m_tLyrSize.cy / 2 + 50 ) );
	this->addChild ( pSprite );
    
	CCLabelTTF*		pScoreLabel = CCLabelTTF::create ( ccszf ( "%d", m_nQuike * 200 ), "Font/CourierBold.ttf", 20 );
	pScoreLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 10, m_tLyrSize.cy / 2 + 50 ) );
	this->addChild ( pScoreLabel );
    
    CCLabelTTF*		pNumLabel = CCLabelTTF::create ( ccszf ( "%dpts", m_nQuike ), "Font/CourierBold.ttf", 20 );
	pNumLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 90, m_tLyrSize.cy / 2 + 50 ) );
	this->addChild ( pNumLabel );  
}

KDvoid ScoreScene::score3 ( KDvoid )
{
	CCSprite*		pTankSprite = CCSprite::createWithSpriteFrameName ( "en6.png" );
	pTankSprite->setScale ( 0.7f );
	pTankSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 100, m_tLyrSize.cy / 2 + 20 ) );
	this->addChild ( pTankSprite );
	
	CCSprite*		pSprite = CCSprite::create ( "Images/jiantou.png" );
	pSprite->setScale ( 1.5f );
	pSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 50, m_tLyrSize.cy / 2 + 20 ) );
	this->addChild ( pSprite );
    
	CCLabelTTF*		pScoreLabel = CCLabelTTF::create ( ccszf ( "%d", m_nStrong * 500 ), "Font/CourierBold.ttf", 20 );
	pScoreLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 10, m_tLyrSize.cy / 2 + 20 ) );
	this->addChild ( pScoreLabel );
    
    CCLabelTTF*		pNumLabel = CCLabelTTF::create ( ccszf ( "%dpts", m_nStrong ), "Font/CourierBold.ttf", 20 );
	pNumLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 90, m_tLyrSize.cy / 2 + 20 ) );
	this->addChild ( pNumLabel );  
}

KDvoid ScoreScene::score4 ( KDvoid )
{
	CCSprite*		pTankSprite = CCSprite::createWithSpriteFrameName ( "en5.png" );
	pTankSprite->setScale ( 0.7f );
	pTankSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 100, m_tLyrSize.cy / 2 - 10 ) );
	this->addChild ( pTankSprite );
	
	CCSprite*		pSprite = CCSprite::create ( "Images/jiantou.png" );
	pSprite->setScale ( 1.5f );
	pSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 50, m_tLyrSize.cy / 2 - 10 ) );
	this->addChild ( pSprite );
    
	CCLabelTTF*		pScoreLabel = CCLabelTTF::create ( ccszf ( "%d", m_nStrongYe * 800 ), "Font/CourierBold.ttf", 20 );
	pScoreLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 10, m_tLyrSize.cy / 2 - 10 ) );
	this->addChild ( pScoreLabel );
    
    CCLabelTTF*		pNumLabel = CCLabelTTF::create ( ccszf ( "%dpts", m_nStrongYe ), "Font/CourierBold.ttf", 20 );
	pNumLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 90, m_tLyrSize.cy / 2 - 10 ) );
	this->addChild ( pNumLabel );  
}

KDvoid ScoreScene::score5 ( KDvoid )
{
	CCSprite*		pTankSprite = CCSprite::createWithSpriteFrameName ( "en3.png" );
	pTankSprite->setScale ( 0.7f );
	pTankSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 100, m_tLyrSize.cy / 2 - 40 ) );
	this->addChild ( pTankSprite );
	
	CCSprite*		pSprite = CCSprite::create ( "Images/jiantou.png" );
	pSprite->setScale ( 1.5f );
	pSprite->setPosition ( ccp ( m_tLyrSize.cx / 2 - 50, m_tLyrSize.cy / 2 - 40 ) );
	this->addChild ( pSprite );
    
	CCLabelTTF*		pScoreLabel = CCLabelTTF::create ( ccszf ( "%d", m_nStrongG * 1000 ), "Font/CourierBold.ttf", 20 );
	pScoreLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 10, m_tLyrSize.cy / 2 - 40 ) );
	this->addChild ( pScoreLabel );
    
    CCLabelTTF*		pNumLabel = CCLabelTTF::create ( ccszf ( "%dpts", m_nStrongG ), "Font/CourierBold.ttf", 20 );
	pNumLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 90, m_tLyrSize.cy / 2 - 40 ) );
	this->addChild ( pNumLabel );  
}

KDvoid ScoreScene::scoreSum ( KDvoid )
{
	KDint	nTotalScore = m_nSlow * 100 + m_nQuike * 200 + m_nStrong * 500 + m_nStrongYe * 800 + m_nStrongG * 1000;
    KDint	nTotalNum = m_nSlow + m_nQuike + m_nStrong + m_nStrongYe + m_nStrongG;
    
	CCLabelTTF*		pTotalLabel = CCLabelTTF::create ( "TOTAL", "Font/CourierBold.ttf", 25 );
	pTotalLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 - 120, m_tLyrSize.cy / 2 - 80 ) );
	this->addChild ( pTotalLabel );

	CCLabelTTF*		pScoreLabel = CCLabelTTF::create ( ccszf ( "%d", nTotalScore ), "Font/CourierBold.ttf", 25 );
	pScoreLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 10, m_tLyrSize.cy / 2 - 80 ) );
	this->addChild ( pScoreLabel );
    
    CCLabelTTF*		pNumLabel = CCLabelTTF::create ( ccszf ( "%dpts", nTotalNum ), "Font/CourierBold.ttf", 25 );
	pNumLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 + 120, m_tLyrSize.cy / 2 - 80 ) );
	this->addChild ( pNumLabel );  
}

KDvoid ScoreScene::onButtonClicked ( CCObject* pSender )
{
    if ( m_nLevel + 1 > 20 )
	{
		CCDirector::sharedDirector ( )->replaceScene ( WinScene::scene ( ) );
    }
	else 
	{
		CCDirector::sharedDirector ( )->replaceScene ( MainScene::scene ( m_nLevel + 1, m_nKind, 3 ) );
    }
}
