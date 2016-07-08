/* --------------------------------------------------------------------------
 *
 *      File            Ch2_Buttons.cpp
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
#include "Ch2_Buttons.h"
#include "Libraries/GameButton.h"

KDbool Ch2_Buttons::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}
	
	m_pMessage->setString ( "Buttons pressed:" );

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "dpad_buttons.plist" );
		
	m_pButtons = CCArray::create ( );
	m_pButtons->retain ( );
		
	this->createButtonWithPosition ( ccp ( 350,  50 ), "b_button_up.png", "b_button_down.png", "B" );
	this->createButtonWithPosition ( ccp ( 400, 100 ), "a_button_up.png", "a_button_down.png", "A" );
	this->createButtonWithPosition ( ccp ( 350, 150 ), "x_button_up.png", "x_button_down.png", "X" );
	this->createButtonWithPosition ( ccp ( 300, 100 ), "y_button_up.png", "y_button_down.png", "Y" );

	this->schedule ( schedule_selector ( Ch2_Buttons::step ) );

	return KD_TRUE;
}

KDvoid Ch2_Buttons::onExit ( KDvoid )
{
	m_pButtons->release ( );

	Recipe::onExit ( );
}

KDvoid Ch2_Buttons::step ( KDfloat fDelta )
{
	m_pMessage->setString ( "Buttons pressed:" );

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		if ( pButton->isPressed ( ) )
		{
			m_pMessage->setString ( ccszf ( "%s %s", m_pMessage->getString ( ), pButton->getName ( ) ) );
		}
	}
}

KDvoid Ch2_Buttons::createButtonWithPosition ( const CCPoint& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	GameButton*		pButton = GameButton::create ( );
	pButton->setPosition ( tPosition );
	pButton->setUpSpriteFrame ( szUpFrame );
	pButton->setDownSpriteFrame ( szDownFrame );
	pButton->setDisplayFrame ( pCache->spriteFrameByName ( pButton->getUpSpriteFrame ( ) ) );
	pButton->setName ( szName );
	this->addChild ( pButton );

	m_pButtons->addObject ( pButton );
}

KDvoid Ch2_Buttons::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesBegan ( pTouches, pEvent );	
	}
}

KDvoid Ch2_Buttons::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesMoved ( pTouches, pEvent );
	}
}

KDvoid Ch2_Buttons::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesEnded ( pTouches, pEvent );	
	}
}
