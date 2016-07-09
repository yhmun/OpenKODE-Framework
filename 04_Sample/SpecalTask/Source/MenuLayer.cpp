/* --------------------------------------------------------------------------
 *
 *      File            MenuLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-10-25
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
#include "MenuLayer.h"
#include "Game.h"

KDbool MenuLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	m_nIndexText  = 0;

	CCSprite*		pBackground = CCSprite::create ( "Image 816 at frame 10.jpg" );
	pBackground->setScale ( IOSSCALE );
	pBackground->setPosition ( ccpMid ( m_tContentSize ) );
	this->addChild ( pBackground );

	CCMenuItemFont::setFontName ( "Arial Bold.ttf" );
	CCMenuItemFont::setFontSize ( 18 );

	CCMenuItem*		pItemAchievement = CCMenuItemFont::create ( "Achievement" );
	CCMenuItem*		pItemLeaderboard = CCMenuItemFont::create ( "Leaderboard" );
	CCMenuItem*		pItemGetProps = CCMenuItemFont::create ( "GetProps" );			
	CCMenuItem*		pItemPlay = CCMenuItemImage::create ( "Image 822 at frame 10.jpg", "Image 823 alpha channel at frame 10.png", this, menu_selector ( MenuLayer::playGame ) );

	CCMenu*			pMenu = CCMenu::create ( pItemAchievement, pItemLeaderboard, pItemGetProps, pItemPlay, KD_NULL );
	pMenu->alignItemsHorizontallyWithPadding ( 18 );
	pMenu->setPosition ( m_tContentSize.cx / 2, m_tContentSize.cy / 2 - 50 );
	this->addChild ( pMenu );

    m_pTelScope = TeleScope::create ( );
    m_pTelScope->setPosition ( ccp ( 80, 60 ) );
    m_pTelScope->setOpacity ( 0 );
    pBackground->addChild ( m_pTelScope, 1 );

	m_aTexts [ 0 ] = CCSprite::create ( "Image 817 at frame 10.jpg" );
	m_aTexts [ 1 ] = CCSprite::create ( "Image 818 at frame 10.jpg" );
	m_aTexts [ 2 ] = CCSprite::create ( "Image 819 at frame 10.jpg" );
	m_aTexts [ 3 ] = CCSprite::create ( "Image 820 at frame 10.jpg" );

	m_aTexts [ 0 ]->setPosition ( ccp ( 156 / 2      , 264 / 2 ) );
	m_aTexts [ 1 ]->setPosition ( ccp ( 156 / 2 +  40, 264 / 2 ) );
	m_aTexts [ 2 ]->setPosition ( ccp ( 156 / 2 +  80, 264 / 2 ) );
	m_aTexts [ 3 ]->setPosition ( ccp ( 156 / 2 + 120, 264 / 2 ) );

	m_aTexts [ 0 ]->setOpacity ( 0 );
	m_aTexts [ 1 ]->setOpacity ( 0 );
	m_aTexts [ 2 ]->setOpacity ( 0 );
	m_aTexts [ 3 ]->setOpacity ( 0 );

	pBackground->addChild ( m_aTexts [ 0 ] );
	pBackground->addChild ( m_aTexts [ 1 ] );
	pBackground->addChild ( m_aTexts [ 2 ] );
	pBackground->addChild ( m_aTexts [ 3 ] );

	m_aHoles [ 0 ] = CCSprite::create ( "Image 824 at frame 10.jpg" );
	m_aHoles [ 1 ] = CCSprite::create ( "Image 824 at frame 10.jpg" );
	m_aHoles [ 2 ] = CCSprite::create ( "Image 824 at frame 10.jpg" );
	m_aHoles [ 3 ] = CCSprite::create ( "Image 824 at frame 10.jpg" );

	m_aHoles [ 0 ]->setOpacity ( 0 );
	m_aHoles [ 1 ]->setOpacity ( 0 );
	m_aHoles [ 2 ]->setOpacity ( 0 );
	m_aHoles [ 3 ]->setOpacity ( 0 );

	m_aHoles [ 0 ]->setPosition ( ccp (  58, 142 ) );
	m_aHoles [ 1 ]->setPosition ( ccp (  88, 112 ) );
	m_aHoles [ 2 ]->setPosition ( ccp ( 226, 105 ) );
	m_aHoles [ 3 ]->setPosition ( ccp ( 178, 132 ) );

	pBackground->addChild ( m_aHoles [ 0 ] );
	pBackground->addChild ( m_aHoles [ 1 ] );
	pBackground->addChild ( m_aHoles [ 2 ] );
	pBackground->addChild ( m_aHoles [ 3 ] );
    
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "Sound 1 at frame 10.mp3" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "Sound 5 at frame 10.mp3" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "Sound 3 at frame 10.mp3" );

	return KD_TRUE;
}

KDvoid MenuLayer::onEnterTransitionDidFinish ( KDvoid )
{
	CCLayer::onEnterTransitionDidFinish ( );

	this->textAnimation ( );

	m_pTelScope->runAction
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCDelayTime	::create ( 5 ),
				CCMoveTo	::create ( 0.5f, ccp ( 220, 70 ) ), 
				CCRotateTo	::create ( 0.8f, 180 ),
				CCRotateTo	::create ( 0.9f, 360 ),
				CCDelayTime	::create ( 5 ),
				CCMoveTo	::create ( 0.5f, ccp ( 88, 150 ) ), 
				CCDelayTime	::create ( 5 ),
				CCRotateTo	::create ( 0.8f, 180 ),
				CCMoveTo	::create ( 0.5f, ccp ( 80, 60 ) ), 
				CCRotateTo	::create ( 0.7f, 0 ),
				CCDelayTime	::create ( 5 ),
				CCDelayTime	::create ( 5 ),				
				KD_NULL 
			)
		)
	);
}

KDvoid MenuLayer::textAnimation ( KDvoid )
{
	if ( m_nIndexText < 8 )
	{
		CCSequence*		pSeq = CCSequence::create 
		(
			CCFadeIn	::create ( 0.1f ), 
			CCScaleTo	::create ( 0.02f, 0.5f ), 
			CCCallFunc	::create ( this, callfunc_selector ( MenuLayer::textAnimation ) ),
			KD_NULL 
		);

		KDint			i = m_nIndexText % 4;

		if ( m_nIndexText < 4 )
		{
			m_aTexts [ i ]->runAction ( pSeq );

			SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound 5 at frame 10.mp3" );
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound 3 at frame 10.mp3" );
		}
		else 
		{
			m_aHoles [ i ]->runAction ( pSeq );

			SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound 1 at frame 10.mp3" );
		}
	}

    m_nIndexText++;
}

KDvoid MenuLayer::playGame ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.f, Game::scene ( ), ccBLACK ) );
}
