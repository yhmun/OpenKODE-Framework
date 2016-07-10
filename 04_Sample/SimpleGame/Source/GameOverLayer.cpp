/* --------------------------------------------------------------------------
 *
 *      File            GameOverLayer.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2010      Ray Wenderlich
 *
 *         http://www.cocos2d-x.org  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "GameOverLayer.h"
#include "GameLayer.h"

GameOverLayer::GameOverLayer ( KDvoid )
{
	m_pLabel = KD_NULL;
}

KDbool GameOverLayer::init ( KDvoid )
{
	if ( !CCLayerColor::initWithColor ( ccc4 ( 32, 32, 32, 128 ) ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	m_pLabel = CCLabelTTF::create ( "", "Fonts/timesbd.ttf", 32 );

	m_pLabel->setColor ( ccORANGE );
	m_pLabel->setPosition ( ccpMid ( tWinSize ) );
	
	this->addChild ( m_pLabel );
	this->runAction ( CCSequence::createWithTwoActions 
	(
		CCDelayTime::create ( 3 ),
		CCCallFunc ::create ( this, callfunc_selector ( GameOverLayer::onGameOverDone ) )
	) );
		
	return KD_TRUE;
}

KDvoid GameOverLayer::onGameOverDone ( )
{
	CCScene*  pScene = CCScene::create ( );
    pScene->addChild ( GameLayer::create ( ) );
	CCDirector::sharedDirector()->replaceScene ( pScene );
}
