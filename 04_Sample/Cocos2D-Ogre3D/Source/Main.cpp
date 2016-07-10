/* ------------------------------------------------------------------------------------
 *
 *      File            Main.cpp
 *      Author          Y.H Mun
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * ------------------------------------------------------------------------------------
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ------------------------------------------------------------------------------------ */

#if defined ( Linux )
#include "kode.h"
#include "platform.h"
#endif

#include "Precompiled.h"
#include "platform.h"
#include "UIControl.h"

COCOS2D_ENGINE_LAUNCH ( XM_SYS_LANDSCAPE )

KDbool AppDelegate::applicationDidFinishLaunching ( KDvoid )
{
	CCEGLView::sharedOpenGLView ( )->setDesignResolutionSize ( 800, 480, kResolutionExactFit );
	CCDirector::sharedDirector ( )->setDisplayStats ( KD_TRUE );
  
	CCScene*  pScene = CCScene::create ( );    
    pScene->addChild ( UIControl::create ( ) );
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