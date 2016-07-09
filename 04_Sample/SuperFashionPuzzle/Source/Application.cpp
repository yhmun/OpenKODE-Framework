/* --------------------------------------------------------------------------
 *
 *      File            Application.cpp
 *      Description     Application Entrance.
 *
 *      Created By      Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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

#define XM_EMULATOR_CX		640
#define XM_EMULATOR_CY		960

#include "platform.h"
#include "CompanyScene.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	// Initialize Resolution
	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( ccs ( 480, 320 ), kResolutionExactFit );

	// Initalize Frame rate and debug display
	CCDirector::sharedDirector ( )->setAnimationInterval ( 1 / 60.0 );
//	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );

    std::vector<std::string>	aPaths;
	aPaths.push_back ( "game" );
	aPaths.push_back ( "game/girls" );
	aPaths.push_back ( "in_game_menu" );
	aPaths.push_back ( "loading" );
	aPaths.push_back ( "logo" );
	aPaths.push_back ( "main_menu" );
	aPaths.push_back ( "shared" );
	aPaths.push_back ( "sound" );
	aPaths.push_back ( "font" );
	
	CCFileUtils::sharedFileUtils ( )->setSearchPaths ( aPaths );

	// Initialize Scene
	CCDirector::sharedDirector ( )->runWithScene ( CompanyScene::create ( ) );

	return KD_TRUE;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{

}

KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{

}

KDvoid AppDelegate::applicationDidEnterBackground ( KDvoid )
{

}
