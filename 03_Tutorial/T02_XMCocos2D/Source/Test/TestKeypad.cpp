/* --------------------------------------------------------------------------
 *
 *      File            TestKeypad.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestKeypad.h"

KDbool TestKeypad::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	m_pLabel = CCLabelTTF::create ( "", "fonts/Marker Felt.ttf", 32 );	
	this->addChild ( m_pLabel );
	m_pLabel->setPositionWithParent ( kCCAlignmentCenter );
	
	this->setKeypadEnabled ( KD_TRUE );

	return KD_TRUE;
} 

const KDchar* TestKeypad::subtitle ( KDvoid )
{
	return "Please press any key...";
}

KDvoid TestKeypad::keyReleased ( KDint nID )
{
	m_pLabel->setString ( ccszf ( "keyReleased\n%s", xmGetKeyName ( nID ) ) );
}

KDvoid TestKeypad::keyPressed ( KDint nID )
{
	m_pLabel->setString ( ccszf ( "keyPressed\n%s", xmGetKeyName ( nID ) ) );
}

KDvoid TestKeypad::keyBackClicked ( KDvoid )
{
	m_pLabel->setString ( "keyBackClicked" );
}

KDvoid TestKeypad::keyMenuClicked ( KDvoid )
{
	m_pLabel->setString ( "keyMenuClicked" );
}

