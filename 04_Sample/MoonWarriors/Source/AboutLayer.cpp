/* --------------------------------------------------------------------------
 *
 *      File            AboutLayer.cpp
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
#include "AboutLayer.h"
#include "SysMenu.h"

KDbool AboutLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCSprite*  pLoading = CCSprite::create ( PATH_IMAGE_LOADING );
	pLoading->setAnchorPoint ( CCPointZero );
	this->addChild ( pLoading );

	CCSprite*  pTitle = CCSprite::create ( PATH_IMAGE_MENU_TITLE, CCRect ( 0, 34, 100, 34 ) );
	pTitle->setPosition ( ccp ( tWinSize.cx / 2, tWinSize.cy - 120 ) );
	this->addChild ( pTitle );

	CCLabelTTF*  pAbout = CCLabelTTF::create
	(
		"\n \n This showcase utilizes many features from Cocos2D engine, including: Parallax background, tilemap, actions, ease, frame animation, schedule, Labels, keyboard Dispatcher, Scene Transition. Art and audio is copyrighted by Enigmata Genus Revenge, you may not use any copyrigted material without permission. This showcase is licensed under GPL",
		PATH_FONT_DEFAULT, 12, CCSize ( tWinSize.cx * 0.85f, 100 ), kCCTextAlignmentLeft
	);
	pAbout->setPosition ( ccp ( tWinSize.cx / 2, tWinSize.cy / 2 + 20 ) );
	this->addChild ( pAbout );

	CCLabelTTF*  pAuthor = CCLabelTTF::create
	(
		"Programmer : Shengxiang Chen\n"
		"                     Dingping Lv",
		PATH_FONT_DEFAULT, 12 , CCSize ( tWinSize.cx * 0.85f, 100 ), kCCTextAlignmentLeft
	);
	pAuthor->setPosition ( ccp ( tWinSize.cx / 2, 100 ) );
	this->addChild ( pAuthor );

	pAuthor = CCLabelTTF::create
	(
		"Effects animation : Hao Wu",
		PATH_FONT_DEFAULT, 12, CCSize ( tWinSize.cx * 0.85f, 100 ), kCCTextAlignmentLeft
	);
	pAuthor->setPosition ( ccp ( tWinSize.cx / 2, 60 ) );
	this->addChild ( pAuthor );

	pAuthor = CCLabelTTF::create
	(
		"Quality Assurance : Sean Lin",
		PATH_FONT_DEFAULT, 12, CCSize ( tWinSize.cx * 0.85f, 100 ), kCCTextAlignmentLeft
	);
	pAuthor->setPosition ( ccp ( tWinSize.cx / 2, 40 ) );
	this->addChild ( pAuthor );

	CCMenuItemLabel*  pBack = CCMenuItemLabel::create 
	(
		CCLabelTTF::create ( "Go back", PATH_FONT_DEFAULT, 14 ),
		this, menu_selector ( AboutLayer::onBack )
	);
	pBack->setPosition ( ccp ( tWinSize.cx / 2, 40 ) );
	this->addChild ( CCMenu::createWithItem ( pBack ) );

	return KD_TRUE;
}

KDvoid AboutLayer::onBack ( CCObject* pSender )
{
	Global::playEffect ( PATH_SOUND_BUTTON );

	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( SysMenu::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.2f, pScene ) );
}
