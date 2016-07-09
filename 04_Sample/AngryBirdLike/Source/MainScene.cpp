/* --------------------------------------------------------------------------
 *
 *      File            MainScene.cpp
 *      Ported By       Hyeon-Ho Jeong
 *      Contact         skdldnjs@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      
 *
 *      http://www.usmanacademy.com/2012/07/rage-of-war-game-using-cocos2d-x-and.html
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
#include "Level1Scene.h" 
//#include "Level2.h" 

KDbool MainScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCMenuItemImage*	pLevel1 = CCMenuItemImage::create 
	(
		"SpriteImage/Leve1Button.png",
		"SpriteImage/Leve1Button.png",
		this,
		menu_selector ( MainScene::onLevel1 ) 
	);   
	pLevel1->setPosition ( ccp ( 680, 200 ) );
	this->addChild ( CCMenu::createWithItem ( pLevel1 ), 1 );
		

	CCMenuItemImage*	pLevel2 = CCMenuItemImage::create 
	(
		"SpriteImage/Leve2Button.png",
		"SpriteImage/Leve2Button.png",
		this,
		menu_selector( MainScene::onLevel2 ) 
	);
	pLevel2->setPosition ( ccp ( 820,200 ) );
	this->addChild ( CCMenu::createWithItem ( pLevel2 ), 1 );


	CCSprite*	pSprite = CCSprite::create ( "BackGround/bgHome.png" );
	pSprite->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	this->addChild ( pSprite, 0 );


	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "Sound/CircusMusic.mp3" ) ;
 
	return KD_TRUE;
}

KDvoid MainScene::onMenuClose ( CCObject* pSender )
{
    CCDirector::sharedDirector ( )->end ( );
}

KDvoid MainScene::onLevel1 ( CCObject* pSender )
{
	CCScene*	pScene = CCScene::create ( );
	pScene->addChild ( Level1Scene::create ( ) );
	CCDirector::sharedDirector()->replaceScene ( pScene );
}

KDvoid MainScene::onLevel2 ( CCObject* pSender )
{

}
