/* -----------------------------------------------------------------------------------
 *
 *      File            AppDelegate.cpp
 *      Author          -
 *      Contact         -@gmail.com 
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

#define XM_EMULATOR_CX  720
#define XM_EMULATOR_CY  1280

#include "platform.h"
#include "Manager/XMGlobalManager.h"
#include "Node/Scene/XMCompanyLogo.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE );

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	auto director = Director::getInstance ( );
	director->setOpenGLView ( EGLView::getInstance ( ) );

//	director->setDisplayStats (  );
	director->setAnimationInterval ( 1.0 / 60 );

	auto screenSize = EGLView::getInstance ( )->getFrameSize ( );

	auto designSize = Size ( 1280, 720 );

	auto pFileUtils = FileUtils::getInstance ( );
	std::vector<std::string> searchPaths;

	searchPaths.push_back ( "Image/Background" );
	searchPaths.push_back ( "Image/Ui/Stage" );
	searchPaths.push_back ( "Image/Ui/Main" );
	searchPaths.push_back ( "Image/DPad" );
	searchPaths.push_back ( "Image/Character/Player" );
	searchPaths.push_back ( "Image/Character/Enemy" );
	searchPaths.push_back ( "Image/Bullet/Player" );
	searchPaths.push_back ( "Image/Bullet/Enemy" );
	searchPaths.push_back ( "Image/Effect" );
	searchPaths.push_back ( "Font" );

	pFileUtils->setSearchPaths ( searchPaths );

	EGLView::getInstance ( )->setDesignResolutionSize ( designSize.width, designSize.height, ResolutionPolicy::NO_BORDER );

	GLOBAL->create ( );
	
	Director::getInstance ( )->runWithScene ( XMCompanyLogo::create ( ) );

    return true;
}

KDvoid AppDelegate::applicationWillTerminate ( KDvoid )
{

}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
KDvoid AppDelegate::applicationDidEnterBackground ( KDvoid )
{
  
}

// this function will be called when the app is active again
KDvoid AppDelegate::applicationWillEnterForeground ( KDvoid )
{
 
}
