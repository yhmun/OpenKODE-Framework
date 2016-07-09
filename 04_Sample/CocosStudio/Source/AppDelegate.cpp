/* -----------------------------------------------------------------------------------
 *
 *      File            AppDelegate.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#include "Precompiled.h"

#define XM_EMULATOR_CX  640
#define XM_EMULATOR_CY  960

#include "platform.h"

#include "DemoBag/EquipScene.h"
#include "DemoGame/GameScene.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE );

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
    // initialize director
    auto	pDirector   = Director::getInstance ( );

    pDirector->setDisplayStats ( true );
    pDirector->setAnimationInterval ( 1.0 / 60 );

	auto	pEGLView    = EGLView::getInstance ( );
    auto	tScreenSize = pEGLView->getFrameSize ( );
    auto	tDesignSize = Size ( 480, 320 );

	pEGLView->setDesignResolutionSize ( tDesignSize.width, tDesignSize.height, ResolutionPolicy::EXACT_FIT );

    auto	pFileUtils  = FileUtils::getInstance ( );
	std::vector<std::string>	aSearchPaths;
    
	aSearchPaths.push_back ( "DemoBag" );
	aSearchPaths.push_back ( "DemoGame" );

	pFileUtils->setSearchPaths(aSearchPaths);
    
//    auto	pScene = Scene::create ( );
//    auto	pLayer = ;
//    pScene->addChild ( pLayer );
    pDirector->runWithScene ( GameScene::create ( ) );
	
    return true;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{

}

KDvoid AppDelegate::applicationDidEnterBackground ( KDvoid )
{

}

KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{

}
