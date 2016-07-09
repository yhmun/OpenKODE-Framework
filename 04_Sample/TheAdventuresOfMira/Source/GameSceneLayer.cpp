/* --------------------------------------------------------------------------
 *
 *      File            GameSceneLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
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
#include "GameSceneLayer.h"

CCScene* GameSceneLayer::scene ( KDvoid )
{
    // 'scene' is an autorelease object
    CCScene*	pScene = CCScene::create ( );
    
    // 'layer' is an autorelease object
    GameSceneLayer*		pLayer = GameSceneLayer::create ( );
    pLayer->setTag ( kGameManager_GameSceneLayer );
    
    GameManager::sharedGameManager ( )->setGameSceneLayer ( pLayer );
    
    // add layer as a child to scene
    pScene->addChild ( pLayer );
    
    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
KDbool GameSceneLayer::init ( KDvoid )
{
    // super init first
    if ( !CCLayer::init ( ) )
    {
        return KD_FALSE;
    }
    
    // create and add the background parallax scrolling layer
    this->addChild ( GameManager::sharedGameManager ( )->getBackgroundLayer ( ), 0 );
    
	SimpleAudioEngine*		pSound = SimpleAudioEngine::sharedEngine ( );

    pSound->preloadBackgroundMusic ( "song_intro.mp3" );
    pSound->preloadBackgroundMusic ( "song_1.mp3" );
    pSound->preloadBackgroundMusic ( "song_2.mp3" );
    pSound->preloadBackgroundMusic ( "song_3.mp3" );
    pSound->preloadBackgroundMusic ( "song_4.mp3" );
    
    pSound->preloadEffect ( "sfx_thud.mp3" );
    pSound->preloadEffect ( "sfx_laugh.mp3" );
    pSound->preloadEffect ( "sfx_click.mp3" );
    pSound->preloadEffect ( "sfx_collect.mp3" );
    pSound->preloadEffect ( "sfx_gameover.mp3" );
    
    return KD_TRUE;
}

KDvoid GameSceneLayer::keyBackClicked ( KDvoid )
{
    this->menuCloseCallback ( this );
}

KDvoid GameSceneLayer::menuCloseCallback ( CCObject* pSender )
{
    CCDirector::sharedDirector ( )->end ( );  
}
