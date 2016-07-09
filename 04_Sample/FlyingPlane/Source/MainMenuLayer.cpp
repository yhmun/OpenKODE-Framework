/* --------------------------------------------------------------------------
 *
 *      File            MainMenuLayer.cpp
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
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
#include "MainMenuLayer.h"
#include "GameLayer.h"

KDbool MainMenuLayer::init ( KDvoid )
{
    if ( !CCLayer::init ( ) )
    {
        return KD_FALSE;
    }
    
	const CCSize&          tLyrSize = this->getContentSize ( );
    
    // 타이틀
    CCLabelTTF      *pTitle         = CCLabelTTF::create ( "Flying Plane", "Font/arial.ttf", 40 );
    pTitle->setPosition ( ccp ( tLyrSize.width / 2 , tLyrSize.height * 0.8 ) );
    this->addChild ( pTitle );
    
    // 시작 메뉴 아이템 버튼
    CCLabelTTF      *pStart         = CCLabelTTF::create ( "Start Game", "Font/arial.ttf", 28 );
    CCMenuItemLabel *pStartMenuItem = CCMenuItemLabel::create ( pStart, this, menu_selector ( MainMenuLayer::doStartGame ) );
    
    // 설명 메뉴 아이템 버튼
    CCLabelTTF      *pAbout         = CCLabelTTF::create ( "About", "Font/arial.ttf", 28 );
    CCMenuItemLabel *pAboutMenuItem = CCMenuItemLabel::create ( pAbout, this, menu_selector ( MainMenuLayer::doStartGame ) );
    
    // 메뉴 버튼
    CCMenu          *pMenu          = CCMenu::create ( pStartMenuItem, pAboutMenuItem, KD_NULL );
    pMenu->alignItemsVerticallyWithPadding ( 20 );
    pMenu->setPosition ( ccp ( tLyrSize.width / 2, tLyrSize.height / 2 - 50 ) );
    this->addChild ( pMenu );
    
    return KD_TRUE;
}

KDvoid MainMenuLayer::doStartGame ( CCObject* pSender )
{
    CCLOG( "doStartGame" );
    
    // 게임 장면으로 이동
	CCScene   *pScene = CCScene::create ( );
    CCLayer   *pLayer = GameLayer::create ( );
    pScene->addChild ( pLayer );
    CCDirector::sharedDirector ( )->replaceScene ( pScene );
}

KDvoid MainMenuLayer::doAbout ( CCObject* pSender )
{
    CCLOG ( "doAbout" );
}
