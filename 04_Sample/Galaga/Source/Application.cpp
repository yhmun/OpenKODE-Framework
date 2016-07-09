/* --------------------------------------------------------------------------
 *
 *      File            Application.h
 *      Description     The Entrance Point of Application
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

//#define XM_EMULATOR_CX	320
//#define XM_EMULATOR_CY	480

#include "platform.h"
#include "GameState.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_LOG | XM_SYS_QUIT )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	// ���ϴ� �ػ� ����
    CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( 640, 480, kResolutionNoBorder );

	// ����� ���� ǥ�� ( ���ϴ� )
	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );

	// ���α׷� ���۽�, ���� CCScene
	GSTAT->setScene ( eScrMainMenu );

	return KD_TRUE;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{
	// ���� �Ӽ� Ŭ������ �� �Ǿ� �ִٸ� ���켼��.
	GameState::purgeGameState ( );
}

KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{
    
}

KDvoid AppDelegate::applicationDidEnterBackground  ( KDvoid )
{
    
}
