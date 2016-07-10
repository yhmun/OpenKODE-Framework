/* --------------------------------------------------------------------------
 *
 *      File            Main.cpp
 *      Description     Application Entrance.
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "Controller.h"
#include "platform.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_QUIT )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( 640, 480, kResolutionShowAll );
	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );
        
	CCScene*  pScene = CCScene::create ( );    
    pScene->addChild ( Controller::create ( ) );
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
