/* --------------------------------------------------------------------------
 *
 *      File            Intro.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Sergey Tikhonov. All rights reserved. 
 *
 *      https://www.github.com/haqu/climbers
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
#include "Intro.h"
#include "Game.h"

KDbool Intro::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	KDfloat		sw = m_tContentSize.cx;
	KDfloat		sh = m_tContentSize.cy;

	CCSprite*	pWall = CCSprite::create ( "wall.png", CCRect ( 0, 0, sw, sh / 2 ) );
	pWall->setPosition ( ccp ( sw / 2, sh / 4 ) );
	this->addChild ( pWall );

	ccTexParams	tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	pWall->getTexture ( )->setTexParameters ( &tp );

	// sprite sheet
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "sprites.plist" );

	CCSpriteBatchNode*	pBatch = CCSpriteBatchNode::create ( "sprites.png", 50 );
	this->addChild ( pBatch );

	// sprites
	CCSprite*	pSprite;

	pSprite = CCSprite::createWithSpriteFrameName ( "bottom.png" ); 
	pSprite->setPosition ( ccp ( sw / 2, sh / 32 ) );
	pBatch->addChild ( pSprite );

	pSprite = CCSprite::createWithSpriteFrameName ( "top.png" ); 
	pSprite->setPosition ( ccp ( sw / 2, sh / 2 ) );
	pBatch->addChild ( pSprite );

	pSprite = CCSprite::createWithSpriteFrameName ( "gameTitle.png" ); 
	pSprite->setPosition ( ccp ( sw / 2, sh * 7 / 8 ) );
	pBatch->addChild ( pSprite );

	pSprite = CCSprite::createWithSpriteFrameName ( "moreGamesButton.png" ); 
	pSprite->setPosition ( ccp ( sw / 4, sh / 2 ) );
	pSprite->setOpacity ( 0 );
	pSprite->runAction ( CCSequence::createWithTwoActions ( CCDelayTime::create ( 0.2f ), CCFadeIn::create ( 0.4f ) ) );
	pBatch->addChild ( pSprite );
	m_pMoreGamesButton = pSprite;

	pSprite = CCSprite::createWithSpriteFrameName ( "playButton.png" ); 
	pSprite->setPosition ( ccp ( sw / 2, sh / 2 ) );
	pSprite->setOpacity ( 0 );
	pSprite->runAction ( CCFadeIn::create ( 0.2f ) );
	pBatch->addChild ( pSprite );
	m_pPlayButton = pSprite;

	pSprite = CCSprite::createWithSpriteFrameName ( "gameSourcesButton.png" ); 
	pSprite->setPosition ( ccp ( sw * 3 / 4, sh / 2 ) );
	pSprite->setOpacity ( 0 );
	pSprite->runAction ( CCSequence::createWithTwoActions ( CCDelayTime::create ( 0.2f ), CCFadeIn::create ( 0.4f ) ) );
	pBatch->addChild ( pSprite );
	m_pGameSourcesButton = pSprite;

	pSprite = CCSprite::createWithSpriteFrameName ( "gpcLogo.png" ); 
	pSprite->setPosition ( ccp ( sw * 5 / 48, sh * 11.7f / 256 ) );
	pSprite->setOpacity ( 0 );
	pSprite->runAction ( CCSequence::createWithTwoActions ( CCDelayTime::create ( 0.6f ), CCFadeIn::create ( 0.4f ) ) );
	pBatch->addChild ( pSprite );

	pSprite = CCSprite::createWithSpriteFrameName ( "cocos2dLogo.png" ); 
	pSprite->setPosition ( ccp ( sw * 43 / 48, sh * 14.2f / 256 ) );
	pSprite->setOpacity ( 0 );
	pSprite->runAction ( CCSequence::createWithTwoActions ( CCDelayTime::create ( 0.6f ), CCFadeIn::create ( 0.4f ) ) );
	pBatch->addChild ( pSprite );

	// labels
	CCLabelTTF*		pLabel;

	KDfloat			fFontSize = 12.0f;
	if ( IS_IPHONE ) 
	{
		fFontSize = 6.0f;
	}

	pLabel = CCLabelTTF::create ( "Created by @haqu for Game Prototype Challenge using Cocos2D.", "Verdana Bold.ttf", fFontSize );
	pLabel->setPosition ( ccp ( sw / 2, sh * 5 / 256 ) );
	pLabel->setOpacity ( 0 );
	pLabel->runAction ( CCSequence::createWithTwoActions ( CCDelayTime::create ( 0.6f ), CCFadeIn::create ( 0.4f ) ) );
	this->addChild ( pLabel );

	SimpleAudioEngine*	pSE = SimpleAudioEngine::sharedEngine ( );

	pSE->preloadEffect ( "click.mp3" );
	pSE->preloadEffect ( "grab.mp3" );

	pSE->stopBackgroundMusic ( );
	pSE->playBackgroundMusic ( "intro.mp3" );

	this->setTouchEnabled ( KD_TRUE );
	this->setTouchMode ( kCCTouchesOneByOne );

	return KD_TRUE;
}

KDbool Intro::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	SimpleAudioEngine*	pSE = SimpleAudioEngine::sharedEngine ( );

	CCPoint		tLocation = pTouch->getLocation ( );

	KDfloat		sw = m_tContentSize.cx;
	KDfloat		sh = m_tContentSize.cy;

	CCRect		tRect;
	
	// play button
	tRect = CCRect ( sw / 2 - 64, sh / 2 - 64, 128, 128 );
	if ( tRect.containsPoint ( tLocation ) )
	{
		m_pPlayButton->setScale ( 0.95f );
		pSE->playEffect ( "click.mp3" );
	}

	// more games button
	tRect = CCRect ( sw / 4 - 32, sh / 2 - 32, 64, 64 );
	if ( tRect.containsPoint ( tLocation ) )
	{
		m_pMoreGamesButton->setScale ( 0.95f );
		pSE->playEffect ( "click.mp3" );
	}

	// game sources button
	tRect = CCRect ( sw * 3 / 4 - 32, sh / 2 - 32, 64, 64);
	if ( tRect.containsPoint ( tLocation ) )
	{
		m_pGameSourcesButton->setScale ( 0.95f );
		pSE->playEffect ( "click.mp3" );
	}

	return KD_TRUE;
}

KDvoid Intro::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint		tLocation = pTouch->getLocation ( );

	KDfloat		sw = m_tContentSize.cx;
	KDfloat		sh = m_tContentSize.cy;

	CCRect		tRect;

	m_pPlayButton->setScale ( 1.0f );
	m_pMoreGamesButton->setScale ( 1.0f );
	m_pGameSourcesButton->setScale ( 1.0f );
	
	// play button
	tRect = CCRect ( sw / 2 - 64, sh / 2 - 64, 128, 128 );
	if ( tRect.containsPoint ( tLocation ) )
	{
		CCDirector::sharedDirector ( )->replaceScene ( Game::scene ( ) );
	}
	
	// more games button
	tRect = CCRect ( sw / 4 - 32, sh / 2 - 32, 64, 64 );
	if ( tRect.containsPoint ( tLocation ) )
	{
		xmOpenURL ( "itms-apps://itunes.com/apps/iplayfulinc" );	
	}
	
	// game sources button
	tRect = CCRect ( sw * 3 / 4 - 32, sh / 2 - 32, 64, 64 );
	if ( tRect.containsPoint ( tLocation ) )
	{
		xmOpenURL ( "https://github.com/haqu/climbers" );
        CCUserDefault::sharedUserDefault ( )->setIntegerForKey ( "currentLevel", 1 );
        CCUserDefault::sharedUserDefault ( )->flush ( );
	}
}
