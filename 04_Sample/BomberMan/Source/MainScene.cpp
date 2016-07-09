/* --------------------------------------------------------------------------
 *
 *      File            MainScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "MainScene.h"
#include "GameLayer.h"
#include "GameController.h"

KDbool MainScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "img.plist" );

	CCSprite*		pBg = CCSprite::create ( "background.png" );
	pBg->setPosition ( ccpMid ( m_tContentSize ) );
	this->addChild ( pBg );
	
	GameLayer*		pGameLayer = GameLayer::create ( );
	this->addChild ( pGameLayer );

	GameController*	pGameController = GameController::create ( );
	this->addChild ( pGameController );

	pGameController->setHero ( pGameLayer->getHero ( ) );

	return KD_TRUE;
}
