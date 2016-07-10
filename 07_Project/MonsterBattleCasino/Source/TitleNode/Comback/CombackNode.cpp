/* -----------------------------------------------------------------------------------
 *
 *      File            CombackNode.cpp
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
#include "CombackNode.h"
#include "TitleNode/Main/MainNode.h"
#include "MyData/MyData.h"

CombackNode::CombackNode ( KDvoid )
{
	m_pLabelCoin = nullptr;
}

CombackNode::~CombackNode ( KDvoid )
{
	CC_SAFE_DELETE ( m_pLabelCoin );
}

KDbool CombackNode::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	// back
	auto	pBg = Sprite::create ( "Logo/welcomeback.png" );
	pBg->setPosition ( Point ( m_tContentSize ) / 2 );
	this->addChild ( pBg );

	// label coin
	m_pLabelCoin = new cLabelAtlasCustom ( this, "CasinoResult/casino_result_num.png", 25, LAYER_MENE );
	m_pLabelCoin->addCharInfo ( '-', 17 );	m_pLabelCoin->addCharInfo ( '+', 21 );	m_pLabelCoin->addCharInfo ( '0', 26 );
	m_pLabelCoin->addCharInfo ( '1', 26 );	m_pLabelCoin->addCharInfo ( '2', 26 );	m_pLabelCoin->addCharInfo ( '3', 26 );
	m_pLabelCoin->addCharInfo ( '4', 26 );	m_pLabelCoin->addCharInfo ( '5', 26 );	m_pLabelCoin->addCharInfo ( '6', 26 );
	m_pLabelCoin->addCharInfo ( '7', 26 );	m_pLabelCoin->addCharInfo ( '8', 26 );	m_pLabelCoin->addCharInfo ( '9', 26 );

	m_pLabelCoin->setColor ( 255, 255, 255, 0 );

	m_pLabelCoin->setString ( ccszf ( "%d", cMyData::getObj ( )->m_nComebackCoin ) );
	m_pLabelCoin->setPosition ( Point ( 360 - (KDint) ( m_pLabelCoin->getSize ( ).width / 2 ), 233 ) ); 

    auto	pTouchListener = EventListenerTouchAllAtOnce::create ( );
	pTouchListener->onTouchesEnded = CC_CALLBACK_2 ( CombackNode::onTouchesEnded, this );
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pTouchListener, this );

	return true;
}

KDvoid CombackNode::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.5f, MainNode::create ( false ) ) );
}
