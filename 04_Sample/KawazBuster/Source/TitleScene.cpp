/* --------------------------------------------------------------------------
 *
 *      File            TitleScene.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/05/27 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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
#include "TitleScene.h"
#include "MainScene.h"
#include "HowtoScene.h"
#include "CreditScene.h"

KDbool TitleScene::init ( KDvoid )
{
	if ( !KWScene::init ( ) )
	{
		return KD_FALSE;
	}

	m_bPlayed = KD_FALSE;

	KWSprite*	pBackground = KWSprite::create ( "image/title/title_background-hd.png" );
	pBackground->setPosition ( ccpMid ( m_tLyrSize ) );
	

	KWSprite*	pLogo = KWSprite::create ( "image/title/logo-hd.png" );
	pLogo->setPosition ( ccp ( m_tLyrSize.cx / 2, m_tLyrSize.cy / 2 + 20 ) );	
	
	CCMenu*		pMenu = CCMenu::create
	(
		CCMenuItemImage::create ( "image/title/howto-hd.png" , "image/title/howto_selected-hd.png" , this, menu_selector ( TitleScene::pressHowtoButton  ) ),
		CCMenuItemImage::create ( "image/title/start-hd.png" , "image/title/start_selected-hd.png" , this, menu_selector ( TitleScene::pressStartButton  ) ),
		CCMenuItemImage::create ( "image/title/credit-hd.png", "image/title/credit_selected-hd.png", this, menu_selector ( TitleScene::pressCreditButton ) ),		
		KD_NULL
	);
	pMenu->alignItemsHorizontally ( );
	pMenu->setPosition ( ccp ( m_tLyrSize.cx / 2, 80 ) );

	this->addChild ( pBackground );
	this->addChild ( pLogo );
	this->addChild ( pMenu );

	return KD_TRUE;
}		

KDvoid TitleScene::onEnterTransitionDidFinish ( KDvoid )
{
	if ( !m_bPlayed )
	{
		KWMusicManager::sharedManager ( )->playMusic ( "music/title.mp3", KD_FALSE );
		m_bPlayed = KD_TRUE;
	}
}

KDvoid TitleScene::pressStartButton  ( CCObject* pSender )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/pico.wav" );

	CCTransitionPageTurn*	pTransition = CCTransitionPageTurn::create ( 0.5f, MainScene::scene ( ), KD_FALSE );
	CCDirector::sharedDirector ( )->replaceScene ( pTransition );

	KWMusicManager::sharedManager ( )->fadeout ( 0.5f );
}

KDvoid TitleScene::pressCreditButton ( CCObject* pSender )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/pico.wav" );

	CCTransitionCrossFade*	pTransition = CCTransitionCrossFade::create ( 0.5f, CreditScene::scene ( ) );
	CCDirector::sharedDirector ( )->pushScene ( pTransition );
}

KDvoid TitleScene::pressHowtoButton  ( CCObject* pSender )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/pico.wav" );

	CCTransitionCrossFade*	pTransition = CCTransitionCrossFade::create ( 0.5f, HowtoScene::scene ( ) );
	CCDirector::sharedDirector ( )->pushScene ( pTransition );
}