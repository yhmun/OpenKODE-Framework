/* --------------------------------------------------------------------------
 *
 *      File            Application.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2010      Ray Wenderlich
 *
 *         http://www.cocos2d-x.org  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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
#include "GameLayer.h"
#include "platform.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_LOG )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( 400, 320, kResolutionShowAll );
	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );
 
	CCScene*  pScene = CCScene::create ( );
    pScene->addChild ( GameLayer::create ( ) );
    CCDirector::sharedDirector ( )->runWithScene ( pScene );


	// Background Node
	CCSprite*  pBack = CCSprite::create ( "Images/main_bg.png" );
	pBack->ignoreAnchorPointForPosition ( KD_TRUE );	
	pBack->runAction ( CCRepeatForever::create ( CCLiquid::create ( 10, ccs ( 20, 20 ), 5, 20 ) ) );
	CCDirector::sharedDirector ( )->setBackgroundNode ( pBack );	

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
