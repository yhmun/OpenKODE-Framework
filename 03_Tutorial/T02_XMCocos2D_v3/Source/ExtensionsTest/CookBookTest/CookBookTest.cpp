/* --------------------------------------------------------------------------
 *
 *      File            CookBookTest.cpp
 *      Ported By       Young-Hwan Mun, Jin-Su Park
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#include "CookBookTest.h"
#include "testResource.h"

USING_NS_CC_EXT;

static std::function<Layer*()>	createFunctions[] = 
{
    CL ( GameDPadTest ),
	CL ( GameAPadTest ),
	CL ( GameButtonTest ),
};

static KDint		l_nSceneIdx = -1;
#define MAX_LAYER	( sizeof ( createFunctions) / sizeof ( createFunctions [ 0 ] ) )

static Layer* nextAction ( KDvoid )
{
    l_nSceneIdx++;
    l_nSceneIdx = l_nSceneIdx % MAX_LAYER;
    
    auto	pLayer = ( createFunctions [ l_nSceneIdx ] ) ( );
    pLayer->init ( );
    pLayer->autorelease ( );
    
    return pLayer;
}

static Layer* backAction ( KDvoid )
{
    l_nSceneIdx--;
    KDint	nTotal = MAX_LAYER;
    if ( l_nSceneIdx < 0 )
	{
        l_nSceneIdx += nTotal;
	}
    
    auto	pLayer = ( createFunctions [ l_nSceneIdx ] ) ( );
    pLayer->init ( );
    pLayer->autorelease ( );
    
    return pLayer;
}

static Layer* restartAction ( KDvoid )
{
    auto	pLayer = ( createFunctions [ l_nSceneIdx ] ) ( );
    pLayer->init ( );
    pLayer->autorelease ( );
    
    return pLayer;
}

KDvoid CookBookTestScene::runThisTest ( KDvoid )
{
    l_nSceneIdx = -1;
    addChild ( nextAction ( ) );
    
    Director::getInstance ( )->replaceScene ( this );
}

//------------------------------------------------------------------
//
// CookBookTestDemo
//
//------------------------------------------------------------------

KDvoid CookBookTestDemo::onEnter ( KDvoid )
{
    BaseTest::onEnter ( );

	EventListenerTouchAllAtOnce*	pListner = EventListenerTouchAllAtOnce::create ( );

	pListner->onTouchesBegan = CC_CALLBACK_2 ( CookBookTestDemo::onTouchesBegan, this );
	pListner->onTouchesMoved = CC_CALLBACK_2 ( CookBookTestDemo::onTouchesMoved, this );
	pListner->onTouchesEnded = CC_CALLBACK_2 ( CookBookTestDemo::onTouchesEnded, this );

	m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pListner, this );
}

KDvoid CookBookTestDemo::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{

}

KDvoid CookBookTestDemo::onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent )
{

}

KDvoid CookBookTestDemo::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{

}

KDvoid CookBookTestDemo::restartCallback ( Object* pSender )
{
    auto	s = new CookBookTestScene();
    s->addChild ( restartAction() );
    Director::getInstance ( )->replaceScene ( s );
    s->release ( );
}

KDvoid CookBookTestDemo::nextCallback ( Object* pSender )
{
    auto	s = new CookBookTestScene ( );
    s->addChild ( nextAction ( ) );
    Director::getInstance ( )->replaceScene ( s );
    s->release ( );
}

KDvoid CookBookTestDemo::backCallback ( Object* pSender )
{
    auto	s = new CookBookTestScene ( );
    s->addChild ( backAction ( ) );
    Director::getInstance ( )->replaceScene ( s );
    s->release ( );
}

// GameDPadTest

KDvoid GameDPadTest::onEnter ( KDvoid )
{
    CookBookTestDemo::onEnter ( );

	SpriteFrameCache::getInstance ( )->addSpriteFramesWithFile ( "supports/dpad_buttons.plist" );

	m_pGameDPad = GameDPad::create ( "d_pad_normal.png", "d_pad_horizontal.png", "d_pad_diagonal.png" );
	m_pGameDPad->setPosition ( Point ( 100, 100 ) );
	m_pGameDPad->setOpacity ( 100 );
	m_pGameDPad->setScale ( 2 );
	this->addChild ( m_pGameDPad );
}

std::string GameDPadTest::title ( KDvoid )
{
    return "CookBook Test";
}

std::string GameDPadTest::subtitle ( KDvoid )
{
    return "GameDPad";
}

KDvoid GameDPadTest::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_pGameDPad->onTouchesBegan ( aTouches, pEvent );
}

KDvoid GameDPadTest::onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_pGameDPad->onTouchesMoved ( aTouches, pEvent );
}

KDvoid GameDPadTest::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_pGameDPad->onTouchesEnded ( aTouches, pEvent );
}

// GameAPadTest

KDvoid GameAPadTest::onEnter ( KDvoid )
{
    CookBookTestDemo::onEnter ( );

	SpriteFrameCache::getInstance ( )->addSpriteFramesWithFile ( "supports/analog_stick.plist" );

	m_pGameAPad = GameAPad::create ( "analog_pad.png", "analog_bar.png", "analog_nub.png" );
	m_pGameAPad->setPosition ( Point ( 100, 100 ) );
	m_pGameAPad->setOpacity ( 100 );
//	m_pGameAPad->getBar ( )->setVisible ( KD_FALSE );
//	m_pGameAPad->setTouchColor ( Color3B ( 200, 255, 255 ) );
	this->addChild ( m_pGameAPad );	
}

std::string GameAPadTest::title ( KDvoid )
{
    return "CookBook Test";
}

std::string GameAPadTest::subtitle ( KDvoid )
{
    return "GameDPad";
}

KDvoid GameAPadTest::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_pGameAPad->onTouchesBegan ( aTouches, pEvent );
}

KDvoid GameAPadTest::onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_pGameAPad->onTouchesMoved ( aTouches, pEvent );
}

KDvoid GameAPadTest::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_pGameAPad->onTouchesEnded ( aTouches, pEvent );
}

// GameButtonTest

KDvoid GameButtonTest::onEnter ( KDvoid )
{
    CookBookTestDemo::onEnter ( );

	SpriteFrameCache::getInstance ( )->addSpriteFramesWithFile ( "supports/dpad_buttons.plist" );

	this->createButtonWithPosition ( Point ( 350,  50 ), "b_button_up.png", "b_button_down.png", "B" );
	this->createButtonWithPosition ( Point ( 400, 100 ), "a_button_up.png", "a_button_down.png", "A" );
	this->createButtonWithPosition ( Point ( 350, 150 ), "x_button_up.png", "x_button_down.png", "X" );
	this->createButtonWithPosition ( Point ( 300, 100 ), "y_button_up.png", "y_button_down.png", "Y" );
}

KDvoid GameButtonTest::createButtonWithPosition ( const Point& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName )
{
	GameButton*		pButton = GameButton::create ( szUpFrame, szDownFrame );

	pButton->setPosition ( tPosition );
	pButton->setOpacity ( 100 );
	pButton->setName ( szName );
	pButton->setScale ( 2 );
	this->addChild ( pButton );

	m_aGameButtons.push_back ( pButton );
}

std::string GameButtonTest::title ( KDvoid )
{
    return "CookBook Test";
}

std::string GameButtonTest::subtitle ( KDvoid )
{
    return "GameButtons";
}

KDvoid GameButtonTest::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	for ( KDuint i = 0; i < m_aGameButtons.size ( ); i++ )
	{
		m_aGameButtons [ i ]->onTouchesBegan ( aTouches, pEvent );
	}
}

KDvoid GameButtonTest::onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	for ( KDuint i = 0; i < m_aGameButtons.size ( ); i++ )
	{
		m_aGameButtons [ i ]->onTouchesMoved ( aTouches, pEvent );
	}
}

KDvoid GameButtonTest::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	for ( KDuint i = 0; i < m_aGameButtons.size ( ); i++ )
	{
		m_aGameButtons [ i ]->onTouchesEnded ( aTouches, pEvent );
	}
}
