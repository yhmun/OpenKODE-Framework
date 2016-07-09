/* --------------------------------------------------------------------------
 *
 *      File            StartScene.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/10). All rights reserved.
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
#include "StartScene.h"
#include "MainScene.h"

KDbool StartScene::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&		tLyrSize = this->getContentSize ( );

	CCMenuItemFont::setFontName ( "Font/HelveticaBoldOblique.ttf" );
	CCMenuItemFont::setFontSize ( 25 );
    
	CCMenuItemFont*		pButton = CCMenuItemFont::create ( "Start Game", this, menu_selector ( StartScene::onButtonClicked ) );
	pButton->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 - 50 ) );
	this->addChild ( CCMenu::createWithItem ( pButton ) );


	CCSprite*			pTitle = CCSprite::create ( "Images/BattleCity.png" );
	pTitle->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 + 50 ) );
	this->addChild ( pTitle );

	return KD_TRUE;
}

KDvoid StartScene::onButtonClicked ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->replaceScene ( MainScene::scene ( 1, 1, 3 ) );
}
