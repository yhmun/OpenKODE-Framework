/* --------------------------------------------------------------------------
 *
 *      File            Ch4_CharacterMovement.cpp
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
#include "Ch4_CharacterMovement.h"

KDbool Ch4_CharacterMovement::init ( KDvoid )
{	
	if ( !SideScrollerRecipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Press 'A' to jump." );
	
	// Create buttons
	this->createButtonWithPosition ( ccp ( 420, 75 ), "a_button_up.png", "a_button_down.png", "A" );

	// Add some boxes
	for ( KDint x = 0; x < 480; x += 40 )
	{
		this->addBoxWithPosition ( ccp ( x, 40 ), "bricks.png", 5.0f );
	}

	for ( KDint i = 0; i < 12; i++ )
	{
		KDint		x = kdRand ( ) % 480;
		KDint		y = kdRand ( ) % 200 + 120;
		
		this->addBoxWithPosition ( ccp ( x, y ), "crate2.png", 1.0f );
	}

	return KD_TRUE;
}

KDvoid Ch4_CharacterMovement::step ( KDfloat fDelta )
{
	SideScrollerRecipe::step ( fDelta );

	// Process input for the A button
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "A" ) )
		{
			this->processJump ( );
		}
		else
		{
			m_fJumpCounter = -10.f;
		}
	}
}