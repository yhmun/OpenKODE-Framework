/* --------------------------------------------------------------------------
 *
 *      File            IntroLayer.cpp
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
#include "IntroLayer.h"
#include "MainMenuLayer.h"


KDbool IntroLayer::init ( KDvoid )
{
    if ( !CCLayer::init ( ) )
    {
        return KD_FALSE;
    }
    
    // 화면 크기
    const CCSize&   tLyrSize = this->getContentSize ( );
    
    // 소개 화면
	CCSprite    *pBackground = CCSprite::create ( "Images/Default.png" );
	pBackground->setRotation ( 90 );
    pBackground->setPosition ( ccp ( tLyrSize.width / 2, tLyrSize.height / 2 ) );
    this->addChild ( pBackground );
    
    // 1초 후에 화면 전환
    this->scheduleOnce ( schedule_selector ( IntroLayer::makeTransition ), 1 );
    
    return KD_TRUE;
}


KDvoid IntroLayer::makeTransition ( KDfloat fDelta )
{
   // 메뉴 화면으로 이동
   CCScene   *pScene = CCScene::create ( );
   CCLayer   *pLayer = MainMenuLayer::create ( );
   pScene->addChild ( pLayer );
   CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.0f, pScene, ccBLACK ) );
}
