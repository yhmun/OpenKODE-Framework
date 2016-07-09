/* -----------------------------------------------------------------------------------
 *
 *      File            AppDelegate.cpp
 *      Author          J.S. Park
 *      Contact         pparkppark84@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"

#define XM_EMULATOR_CX  640
#define XM_EMULATOR_CY  960

#include "platform.h"
#include "SampleLayer.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE | XM_SYS_QUIT );

bool AppDelegate::applicationDidFinishLaunching()
{
	// As an example, load config file
	// XXX: This should be loaded before the Director is initialized,
	// XXX: but at this point, the director is already initialized

    // initialize director
    auto director = Director::getInstance();
    director->setOpenGLView(EGLView::getInstance());

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);

   	std::vector<std::string> searchPaths;
    searchPaths.push_back("Assets");
	FileUtils::getInstance ( )->setSearchPaths(searchPaths);
    
    auto screenSize = EGLView::getInstance()->getFrameSize();

    auto designSize = Size(480, 320);

    EGLView::getInstance()->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);

    auto scene = Scene::create();
	auto layer = SampleLayer::create ( );

    scene->addChild(layer);
    director->runWithScene(scene);

    return true;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{

}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
KDvoid AppDelegate::applicationDidEnterBackground ( KDvoid )
{
    Director::getInstance()->stopAnimation();
}

// this function will be called when the app is active again
KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{
    Director::getInstance()->startAnimation();
}
