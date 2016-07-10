/* -----------------------------------------------------------------------------------
 *
 *      File            IntroScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
#include "IntroScene.h"
#include "Scene/Logo/LogoScene.h"

KDbool IntroScene::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	auto  pBg = Sprite::create ( "Default2.png" );
	pBg->setPosition ( Point ( m_tContentSize ) / 2 );
	this->addChild ( pBg );

	this->scheduleOnce ( schedule_selector ( IntroScene::nextScene ), 1.f );

	return true;
}

KDvoid IntroScene::nextScene ( KDfloat fDelta )
{
	SimpleAudioEngine*		pSAE = SimpleAudioEngine::getInstance ( );

	pSAE->preloadEffect ( "axe_1_small.wav"	 );
	pSAE->preloadEffect ( "axe_2_medium.wav" );
	pSAE->preloadEffect ( "axe_3_large.wav"	 );
	pSAE->preloadEffect ( "axe_miss.wav"	 );
	pSAE->preloadEffect ( "button.wav"		 );
	pSAE->preloadEffect ( "mode_berserk.wav" );
	pSAE->preloadEffect ( "mode_time.wav"	 );
	pSAE->preloadEffect ( "punch.wav"		 );
	pSAE->preloadEffect ( "result_clear.wav" );
	pSAE->preloadEffect ( "result_over.wav"	 );
	pSAE->preloadEffect ( "throw.wav"		 );

	Director::getInstance ( )->replaceScene ( TransitionCrossFade::create ( 0.5f, LogoScene::create ( ) ) );
}
