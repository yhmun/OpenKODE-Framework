/* --------------------------------------------------------------------------
 *
 *      File            GameOver.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors   
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
#include "GameOver.h"
#include "GameLayer.h"

KDbool GameOver::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pLoading = CCSprite::create ( PATH_IMAGE_LOADING );
	pLoading->setAnchorPoint ( CCPointZero );
	this->addChild ( pLoading );

	CCSprite*  pLogo = CCSprite::create ( PATH_IMAGE_GAMEOVER );
	pLogo->setAnchorPoint ( CCPointZero );
	pLogo->setPosition ( ccp ( 0, 300 ) );
	this->addChild ( pLogo, 10 );

	CCMenuItemSprite*  pPlayAgain = CCMenuItemSprite::create
	(
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 378, 0, 126, 33 ) ),
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 378, 33, 126, 33 ) ),
		CCSprite::create ( PATH_IMAGE_MENU, CCRect ( 378, 33 * 2, 126, 33 ) ),
		this, menu_selector ( GameOver::onPlayAgain )
	);
	pPlayAgain->setPosition ( ccp ( tWinSize.cx / 2, 220 ) );
	this->addChild ( CCMenu::create ( pPlayAgain, KD_NULL ), 1 );

	CCLabelTTF*  pScore = CCLabelTTF::create
	(
		CCString::createWithFormat ( "Your Score : %d", Global::nScore )->getCString ( ), PATH_FONT_DEFAULT, 16 
	);
	pScore->setPosition ( ccp ( 160, 280 ) );
	pScore->setColor ( ccc3 ( 250, 179, 0 ) );
	this->addChild ( pScore, 10 );

	CCMenuItemFont::setFontName ( PATH_FONT_DEFAULT );
	CCMenuItemFont::setFontSize ( 8 );
	CCMenuItemFont*  pTitle1 = CCMenuItemFont::create ( "http://www.cocos2d-x.org/projects/cocos2d-x/wiki/Cocos2d-html5" );
	pTitle1->setEnabled ( KD_FALSE );

	CCMenuItemFont*  pTitle2 = CCMenuItemFont::create ( "https://github.com/ShengxiangChen/MoonWarriors" );
	pTitle2->setEnabled ( KD_FALSE );

	CCMenu*  pMenu = CCMenu::create ( pTitle1, pTitle2, KD_NULL );
	pMenu->alignItemsVerticallyWithPadding ( 10 );
	pMenu->setPosition ( ccp ( 160, 80 ) );
	this->addChild ( pMenu );

	Global::playBGM ( PATH_MUSIC_MAIN );

	return KD_TRUE;
}

KDvoid GameOver::onPlayAgain ( CCObject* pSender )
{
	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( GameLayer::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.2f, pScene ) );
}