/* -----------------------------------------------------------------------------------
 *
 *      File            LogoNode.cpp
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
#include "LogoNode.h"
#include "TitleNode/Comback/CombackNode.h"
#include "MyData/MyData.h"

KDbool LogoNode::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	auto  pBg = Sprite::create ( "Logo/logo.png" );
	pBg->setPosition ( Point ( m_tContentSize ) / 2 );
	this->addChild ( pBg, LAYER_BACK );

	this->scheduleOnce ( schedule_selector ( LogoNode::timeout ), 1.5f );

    auto	pTouchListener = EventListenerTouchAllAtOnce::create ( );
	pTouchListener->onTouchesEnded = CC_CALLBACK_2 ( LogoNode::onTouchesEnded, this );
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pTouchListener, this );

	return true;
}

KDvoid LogoNode::timeout ( KDfloat fDelta )
{
	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.5f, CombackNode::create ( ) ) );
}

KDvoid LogoNode::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	this->unschedule ( schedule_selector ( LogoNode::timeout ) );

	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.5f, CombackNode::create ( ) ) );
}
