/* --------------------------------------------------------------------------
 *
 *      File            Ch5_MenuWindows.cpp
 *      Ported By       Young-Hwan Mun
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

#include "Precompiled.h"
#include "Ch5_MenuWindows.h"
#include "Libraries/GameMenuWindow.h"

KDbool Ch5_MenuWindows::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Initialization
	m_pWindowContainer = CCNode::create ( );

	// Create three menu windows with randomized positions 
	GameMenuWindow*		pWindow1 = GameMenuWindow::create ( "Window 1", ccs ( kdRand ( ) % 200 + 120, kdRand ( ) % 100 + 50 ) );
	pWindow1->setPosition ( ccp ( kdRand ( ) % 100 + 150, kdRand ( ) % 140 + 100 ) );
	m_pWindowContainer->addChild ( pWindow1, 1 );

	GameMenuWindow*		pWindow2 = GameMenuWindow::create ( "Window 2", ccs ( kdRand ( ) % 200 + 120, kdRand ( ) % 100 + 50 ) );
	pWindow2->setPosition ( ccp ( kdRand ( ) % 100 + 150, kdRand ( ) % 140 + 100 ) );
	m_pWindowContainer->addChild ( pWindow2, 2 );

	GameMenuWindow*		pWindow3 = GameMenuWindow::create ( "Window 3", ccs ( kdRand ( ) % 200 + 120, kdRand ( ) % 100 + 50 ) );
	pWindow3->setPosition ( ccp ( kdRand ( ) % 100 + 150, kdRand ( ) % 140 + 100 ) );
	m_pWindowContainer->addChild ( pWindow3, 3 );

	// Add window container node
	this->addChild ( m_pWindowContainer );

	return KD_TRUE;
}

KDvoid Ch5_MenuWindows::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	// Grab the window by touching the top bar. Otherwise, merely bring the window to the front
	CCObject*	pObject;
	CCARRAY_FOREACH_REVERSE ( m_pWindowContainer->getChildren ( ), pObject )
	{
		GameMenuWindow*		pWindow = (GameMenuWindow*) pObject;

		if ( pointIsInRect ( tPoint, pWindow->titleBarRect ( ) ) )
		{
			pWindow->ccTouchesBegan ( pTouches, pEvent );
			return;
		}
		else if ( pointIsInRect ( tPoint, pWindow->rect ( ) ) )
		{
			pWindow->bringToFront ( );
			return;
		}
	}
}

KDvoid Ch5_MenuWindows::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	// If we touched a window them we can drag it

	CCObject*	pObject;
	CCARRAY_FOREACH_REVERSE ( m_pWindowContainer->getChildren ( ), pObject )
	{
		GameMenuWindow*		pWindow = (GameMenuWindow*) pObject;

		if ( pWindow->isTouched ( ) )
		{
			pWindow->ccTouchesMoved ( pTouches, pEvent );
		}
	}
}

KDvoid Ch5_MenuWindows::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// End a touch if neccessary

	CCObject*	pObject;
	CCARRAY_FOREACH_REVERSE ( m_pWindowContainer->getChildren ( ), pObject )
	{
		GameMenuWindow*		pWindow = (GameMenuWindow*) pObject;

		if ( pWindow->isTouched ( ) )
		{
			pWindow->ccTouchesEnded ( pTouches, pEvent );
		}
	}
}