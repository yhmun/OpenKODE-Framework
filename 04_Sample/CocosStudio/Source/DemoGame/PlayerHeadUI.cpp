/* -----------------------------------------------------------------------------------
 *
 *      File            PlayerHeadUI.cpp
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
#include "PlayerHeadUI.h"

PlayerHeadUI::PlayerHeadUI ( KDvoid ) :
m_pUiLayer ( KD_NULL ), m_pPlayer ( KD_NULL ),
m_nPlayerLifeRecorder ( 0 ), m_nPlayerHPRecorder ( 0 )
{
	m_pUiLayer = UILayer::create ( );

	initWidget ( );

	this->addChild ( m_pUiLayer );

	scheduleUpdate ( );
}

KDvoid PlayerHeadUI::initWidget ( KDvoid )
{
	auto	pWidget = GUIReader::shareReader ( )->widgetFromJsonFile ( "uifiles/ui_1.ExportJson" );
	pWidget->setScale ( 0.5f );
	pWidget->setPosition ( Point ( 0, 160 ) );
	m_pUiLayer->addWidget ( pWidget );
}

KDvoid PlayerHeadUI::update ( KDfloat fDelta )
{
	if ( m_pPlayer == NULL )
	{
		return;
	}

	processPlayerLife ( );

	processPlayerHP ( );
}

KDvoid PlayerHeadUI::processPlayerLife ( KDvoid )
{
	if ( m_nPlayerLifeRecorder == m_pPlayer->getLifeNum ( ) )
	{
		return;
	}
	else 
	{
		m_nPlayerLifeRecorder = m_pPlayer->getLifeNum ( );
	}

	auto	pLifeOne = m_pUiLayer->getWidgetByName ( "lifeOne" );
	auto	pLifeTwo = m_pUiLayer->getWidgetByName ( "lifeTwo" );
	auto	pDeadOne = m_pUiLayer->getWidgetByName ( "deadOne" );
	auto	pDeadTwo = m_pUiLayer->getWidgetByName ( "deadTwo" );
	
	if ( m_pPlayer->getLifeNum ( ) == 0 )
	{
		pLifeOne->setVisible ( false );
		pLifeTwo->setVisible ( false );
		pDeadOne->setVisible ( true );
		pDeadTwo->setVisible ( true );
	}
	if ( m_pPlayer->getLifeNum ( ) == 1 )
	{
		pLifeOne->setVisible ( true );
		pLifeTwo->setVisible ( false );
		pDeadOne->setVisible ( false );
		pDeadTwo->setVisible ( true );
	}
	if ( m_pPlayer->getLifeNum ( ) == 2 )
	{
		pLifeOne->setVisible ( true );
		pLifeTwo->setVisible ( true );
		pDeadOne->setVisible ( false );
		pDeadTwo->setVisible ( false );
	}
}

KDvoid PlayerHeadUI::processPlayerHP ( KDvoid )
{
	if ( m_nPlayerHPRecorder == m_pPlayer->getHPPercent ( ) )
	{
		return;
	}
	else if ( m_nPlayerHPRecorder > m_pPlayer->getHPPercent ( ) )
	{
		m_nPlayerHPRecorder -= 2;
		setHPBarPercent ( m_nPlayerHPRecorder );
	}
	else
	{
		m_nPlayerHPRecorder += 10;
		setHPBarPercent ( m_nPlayerHPRecorder );
	}
}

KDvoid PlayerHeadUI::setHPBarPercent ( KDint nPercent )
{
	UILoadingBar*	pHpBar = dynamic_cast<UILoadingBar*> ( m_pUiLayer->getWidgetByName ( "redBar" ) );
	pHpBar->setPercent ( nPercent );
}

PlayerHeadUI::~PlayerHeadUI ( KDvoid )
{
	unscheduleUpdate ( );
}
