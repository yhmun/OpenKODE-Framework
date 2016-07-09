/* --------------------------------------------------------------------------
 *
 *      File            WinScene.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/13). All rights reserved.
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
#include "WinScene.h"
#include "StartScene.h"

CCScene* WinScene::scene ( KDvoid )
{
	CCScene*	pScene = CCScene::create ( );
	WinScene*	pLayer = new WinScene ( );

	if ( pScene && pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );	
		pScene->addChild ( pLayer );
	}
	else
	{
		CC_SAFE_DELETE ( pScene );
		CC_SAFE_DELETE ( pLayer );
	}

	return pScene;
}

KDbool WinScene::init ( KDvoid )
{
	if ( !CCLayerColor::initWithColor ( ccc4 ( 0, 0, 0, 255 ) ) )
	{
		return KD_FALSE;
	}

	this->setVisibleViewport ( );	

	CCSprite*	pSprite = CCSprite::create ( "Images/gameover.png" );
	pSprite->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pSprite );

	this->scheduleOnce ( schedule_selector ( WinScene::gotoMainScene ), 3 );

	return KD_TRUE;
}

KDvoid WinScene::gotoMainScene ( KDfloat fDelta )
{
	CCDirector::sharedDirector ( )->replaceScene ( StartScene::scene ( ) );
}