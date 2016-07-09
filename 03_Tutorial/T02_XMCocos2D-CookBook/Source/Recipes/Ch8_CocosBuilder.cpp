/* --------------------------------------------------------------------------
 *
 *      File            Ch8_CocosBuilder.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Ch8_CocosBuilder.h"

KDbool Ch8_CocosBuilder::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	this->showMessage ( "Press the button below to load a \nCocosBuilder created scene." );
	
	// Add button to push CocosBuilder scene
	CCMenuItemFont::setFontSize ( 32 );

	CCMenuItemFont*		pPushItem = CCMenuItemFont::create ( "Push CocosBuilder Scene", this, menu_selector ( Ch8_CocosBuilder::pushScene ) );
	CCMenu*				pPushMenu = CCMenu::create ( pPushItem, KD_NULL );
	pPushMenu->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pPushMenu );

	return KD_TRUE;
}

KDvoid Ch8_CocosBuilder::pushScene ( CCObject* pSender )
{
//	CCBReader	pCBR;
//	CCScene*	pScene = pCBR.createSceneWithNodeGraphFromFile ( "scene.ccb", this );
//	CCDirector::sharedDirector ( )->pushScene ( pScene );
}

KDvoid Ch8_CocosBuilder::back ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->popScene ( );
}