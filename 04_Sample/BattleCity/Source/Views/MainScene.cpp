/* --------------------------------------------------------------------------
 *
 *      File            MainScene.cpp
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
#include "MainScene.h"
#include "Controllers/InputLayer.h"

enum
{
	eTagLife		= 1000	,
	eTagLevel				,
};

CCScene* MainScene::scene ( KDint nLevel, KDint nStatus, KDint nLife )
{
	CCScene*	pScene = CCScene::create ( );
	MainScene*	pLayer = new MainScene ( );

	if ( pScene && pLayer && pLayer->initWithMapInformation ( nLevel, nStatus, nLife ) )
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

MainScene::MainScene ( KDvoid )
{
	m_pIconArray = KD_NULL;
}

MainScene::~MainScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pIconArray );
}

KDbool MainScene::initWithMapInformation ( KDint nLevel, KDint nStatus, KDint nLife )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&		tLyrSize = this->getContentSize ( );

	SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/02 start.wav" );

	CCLayerColor*		pBackColor = CCLayerColor::create ( ccc4 ( 192, 192, 192, 255 ), tLyrSize );
	this->addChild ( pBackColor );

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "Images/images.plist" );

	m_pIconArray = CCArray::create ( );
	m_pIconArray->retain ( );

	this->iconTank ( );

	CCSprite*		pLife = CCSprite::createWithSpriteFrameName ( "IP.png" );
	pLife->setPosition ( ccp ( 30, tLyrSize.cy - 50 ) );
	this->addChild ( pLife );
    
    CCSprite*		pLifeIcon = CCSprite::createWithSpriteFrameName ( "p1.png" );
	pLifeIcon->setPosition ( ccp ( 20, tLyrSize.cy - 70 ) );
	pLifeIcon->setScale ( 0.5f );
	this->addChild ( pLifeIcon );
	this->showLife ( nLife );
    
	CCSprite*		pFlag = CCSprite::createWithSpriteFrameName ( "flag.png" );
    pFlag->setPosition ( ccp ( tLyrSize.cx - 50 , tLyrSize.cy - 200 ) );
	this->addChild ( pFlag );
	this->showLevel ( nLevel );

	MapLayer*		pMapLayer = MapLayer::create ( nLevel, nStatus, nLife );
	pMapLayer->setDelegate ( this );
	this->addChild ( pMapLayer );

	InputLayer*		pInputLayer = InputLayer::create ( );
	pInputLayer->setMapLayer ( pMapLayer );
	this->addChild ( pInputLayer );

	return KD_TRUE;
}

//
// PrivateMethods
//

KDvoid MainScene::iconTank ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	CCSpriteFrame*	pIconFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "enemy.png" );
    
	KDint	nWidth  = 55;
	KDint	nHeight = 15;  

    for ( KDint i = 0; i < 10; i++ ) 
	{    
        nHeight += 15;

		CCSprite*	pIconA = CCSprite::createWithSpriteFrame ( pIconFrame );
		pIconA->setPosition ( ccp ( tLyrSize.cx - nWidth, tLyrSize.cy - nHeight ) );
		this->addChild ( pIconA );
		m_pIconArray->addObject ( pIconA );        
        
		CCSprite*	pIconB = CCSprite::createWithSpriteFrame ( pIconFrame );
		pIconB->setPosition ( ccp ( tLyrSize.cx - nWidth + 18, tLyrSize.cy - nHeight ) );
		this->addChild ( pIconB );        
        m_pIconArray->addObject ( pIconB );
    }
}

KDvoid MainScene::showLife ( KDint nLife )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	this->removeChildByTag ( eTagLife );

	CCLabelTTF*		pLabel = CCLabelTTF::create ( ccszf ( "%d", nLife ), "Font/CourierBold.ttf", 20 );
	pLabel->setColor ( ccc3 ( 0, 0, 0 ) );
	pLabel->setPosition ( ccp ( 45, tLyrSize.cy - 70 ) );
	this->addChild ( pLabel, 0, eTagLife );
}

KDvoid MainScene::showLevel ( KDint nLevel )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	this->removeChildByTag ( eTagLevel );

	CCLabelTTF*		pLabel = CCLabelTTF::create ( ccszf ( "%d", nLevel ), "Font/CourierBold.ttf", 20 );
	pLabel->setColor ( ccc3 ( 0, 0, 0 ) );
	pLabel->setPosition ( ccp ( tLyrSize.cx - 40, tLyrSize.cy - 230 ) );
	this->addChild ( pLabel, 0, eTagLevel );
}

//
// MapLayerDelegate methods
//

KDvoid MainScene::gameOver ( KDvoid )
{

}

KDvoid MainScene::bornEnmey ( KDvoid )
{
	if ( m_pIconArray->count ( ) > 0 )
	{
		CCSprite*	pIcon = (CCSprite*) m_pIconArray->lastObject ( );
		pIcon->removeFromParent ( );
		m_pIconArray->removeLastObject ( );
	}
}

KDvoid MainScene::changeTankLife ( KDint nLife )
{
	this->showLife ( nLife );
}
