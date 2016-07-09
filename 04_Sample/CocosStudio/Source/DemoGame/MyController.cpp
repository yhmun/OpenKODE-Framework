/* -----------------------------------------------------------------------------------
 *
 *      File            MyController.cpp
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
#include "MyController.h"

MyController* MyController::create ( )
{
	MyController*	pRet = new MyController ( );

    if ( pRet && pRet->init ( ) )
    {
        pRet->autorelease ( );
        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool MyController::init ( KDvoid )
{
	if ( !Layer::init ( ) )
	{
		return false;
	}

    do 
    {
		m_pPlayerStateMachine = KD_NULL;
		// create a joystick delegate for joystick
		m_pJoyStick = createJoyStick ( );
		this->addChild ( m_pJoyStick, 4 );

		UILayer*	pUiLayer = createButton ( );
		this->addChild ( pUiLayer, 4 );

		m_pHeadUI = new PlayerHeadUI ( );
		m_pHeadUI->bindPlayer ( m_pPlayer );
		this->addChild ( m_pHeadUI );
	} while ( 0 );

    return true;
}

JoyStick* MyController::createJoyStick ( KDvoid )
{
	Size		tSize = Director::getInstance ( )->getVisibleSize ( );

	JoyStick*	pJoyStick = JoyStick::create ( 20, 50, true, true, false, false );

	pJoyStick->setPosition ( Point ( 0.2f * tSize.width, 0.2f * tSize.height ) );
	pJoyStick->setBallTexture ( "ui/joystick.png" );
	pJoyStick->setStickTexture ( "ui/stick.png" );
	pJoyStick->setDockTexture ( "ui/joystickbg.png" );

	return pJoyStick;
}

UILayer* MyController::createButton ( KDvoid )
{
	Size		tSize = Director::getInstance ( )->getVisibleSize ( );

	UIButton*	pButton = UIButton::create();

	pButton->setTouchEnabled ( true );
	pButton->loadTextures ( "ui/buttonrelease.png", "ui/buttonpress.png", "" );
	pButton->setPosition ( Point ( 0.9f * tSize.width, 0.2f * tSize.height ) );
	pButton->setScale ( 0.5f );
	pButton->addTouchEventListener ( this, toucheventselector ( MyController::touchEvent ) );

	UILayer*	pUiLayer = UILayer::create ( );
    pUiLayer->addWidget ( pButton );

	return pUiLayer;
}

KDvoid MyController::bindPlayer ( Player* pPlayer )
{
	m_pPlayer = pPlayer;
	bindPlayerStateMachine ( );
	m_pHeadUI->bindPlayer ( pPlayer );
}

KDvoid MyController::bindPlayerStateMachine ( KDvoid )
{
	m_pPlayerStateMachine = m_pPlayer->getStateMachine ( );

	MyJoyStickDelegate*		pJoyStickDelegate = new MyJoyStickDelegate ( );
	pJoyStickDelegate->bindPlayerStateMachine ( m_pPlayerStateMachine );
	m_pJoyStick->setDelegate ( pJoyStickDelegate );
}

KDvoid MyController::touchEvent ( Object* pSender, TouchEventType eType )
{
	if ( eType == TOUCH_EVENT_BEGAN )
	{
		m_pPlayerStateMachine->updatePressButton ( true );
	}
	if ( eType == TOUCH_EVENT_MOVED )
	{
		m_pPlayerStateMachine->updatePressButton ( false );
	}
	if ( eType == TOUCH_EVENT_ENDED )
	{
		m_pPlayerStateMachine->updatePressButton ( false );
	}
}

KDvoid MyJoyStickDelegate::onJoyStickUpdate ( Node* pSender, KDfloat fAngle, Point tDirection, KDfloat fPower )
{
	m_pPlayerStateMachine->updateMoving ( tDirection );
}

KDvoid MyJoyStickDelegate::onJoyStickActivated ( Node* pSender )
{
    m_pPlayerStateMachine->updateActiveJoystick ( true );
}

KDvoid MyJoyStickDelegate::onJoyStickDeactivated ( Node* pSender )
{
    m_pPlayerStateMachine->updateActiveJoystick ( false );
	m_pPlayerStateMachine->updateMoving ( Point::ZERO );
}