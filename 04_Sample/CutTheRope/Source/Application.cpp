/* --------------------------------------------------------------------------
 *
 *      File            Application.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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
#include "platform.h"
#include "LevelEditor/LevelSelectionScene.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_QUIT )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	CCDirector*		pDirector = CCDirector::sharedDirector ( );
	CCSize			tWinSize  = pDirector->getWinSizeInPixels ( );
	CCSize			tDesignSize = ccs ( 320, 480 );
	CCSize			tResourceSize = ccsz;

    CCFileUtils*	pFileUtils = CCFileUtils::sharedFileUtils ( );

    std::vector<std::string>	aSearchPaths;
    /*
    if ( tWinSize.cx > 320 )
    {
        aSearchPaths.push_back ( "hd" );
        aSearchPaths.push_back ( "sd" );
        tResourceSize = ccs ( 640, 960 );
        if ( tWinSize.cy > 960 ) 
		{
            tDesignSize.cy = 1136 / 2.0f;
        }
    }
    else
    {
	*/
        aSearchPaths.push_back ( "sd" );
        tResourceSize = ccs ( 320, 480 );
    //}
    
    pFileUtils->setSearchPaths ( aSearchPaths );
    pDirector->setContentScaleFactor ( tResourceSize.cy / tDesignSize.cy );

	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( tDesignSize, kResolutionExactFit );
	pDirector->setDisplayStats ( KD_TRUE );
 
	CCScene*  pScene = LevelSelectionScene::scene ( );
    pDirector->runWithScene ( pScene );

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
