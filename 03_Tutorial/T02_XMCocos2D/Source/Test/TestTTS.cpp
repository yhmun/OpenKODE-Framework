/* --------------------------------------------------------------------------
 *
 *      File            TestTTS.cpp
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
#include "TestTTS.h"

KDvoid TestTTS::onEnter ( KDvoid )
{	
	TestBasic::onEnter ( );
    
	m_pLabel = CCLabelTTF::create ( "Touch and TTS test will be.", "fonts/Marker Felt.ttf", 32 );
	m_pLabel->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( m_pLabel );
    
    this->setTouchEnabled ( KD_TRUE );
	this->setEventEnabled ( KD_TRUE );

    xmTTSCreate ( );
}

KDvoid TestTTS::onExit ( KDvoid )
{
    xmTTSFree ( );
    
    TestBasic::onExit ( );
}

KDvoid TestTTS::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    const KDchar*  szText = "This is Text to speech test. are you hear my voice?";
    
    m_pLabel->setString ( szText );
    
    xmTTSSpeak ( szText, XM_TTS_QUEUE_FLUSH );
    xmTTSSetPitch ( xmTTSGetPitch ( ) + 0.1f );
}

KDvoid TestTTS::onEvent ( const KDEvent* pEvent )
{
	if ( pEvent->type == KD_EVENT_COMPLETED )
	{
		if ( pEvent->data.completed.type == KD_TTS_COMPLETED )
		{
			m_pLabel->setString ( "TTS completed." );
		}
	}
}
