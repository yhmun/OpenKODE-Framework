/* --------------------------------------------------------------------------
 *
 *      File            Application.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012 Jean-yves mengant on 2/6/12 week 6. All rights reserved.
 *
 *         http://www.raywenderlich.com/11338/cocos2d-x-for-ios-and-android-space-game
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
#include "GameScene.h"
#include "platform.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_ACCELEROMETER | XM_SYS_QUIT )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( 480, 320, kResolutionExactFit );
	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );
 
	CCScene*  pScene = CCScene::create ( );
    pScene->addChild ( GameScene::create ( ) );
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
