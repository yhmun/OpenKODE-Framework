/* --------------------------------------------------------------------------
 *
 *      File            Application.cpp
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

#define XM_EMULATOR_CX                    320
#define XM_EMULATOR_CY                    480

#include "platform.h"
#include "IntroLayer.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_LOG | XM_SYS_QUIT )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	// 원하는 해상도로 지정
	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( 480, 320, kResolutionShowAll );

	// 디버그 정보 표시 (좌 하단)
	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );

	// 처음 화면 표시
    CCScene    *pScene = CCScene::create ( );
    CCLayer    *pLayer = IntroLayer::create ( );
    pScene->addChild ( pLayer );
	CCDirector::sharedDirector ( )->runWithScene ( pScene );
	
	return KD_TRUE;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{
    
}

KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{
    
}

KDvoid AppDelegate::applicationDidEnterBackground  ( KDvoid )
{
    
}
