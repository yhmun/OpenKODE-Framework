/* --------------------------------------------------------------------------
 *
 *      File            Ch5_HorizScrollMenu.cpp
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
#include "Ch5_HorizScrollMenu.h"
#include "Libraries/LoopingMenu.h"

KDbool Ch5_HorizScrollMenu::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Initialize and add LoopingMenu
	LoopingMenu*	pMenu = LoopingMenu::create ( );

	// Create default sprites and 'selected' sprites 
	for ( KDint i = 1; i < 6; i++ )
	{
		const KDchar*	szFile = ccszf ( "book%d.jpg", i );
		ccBlendFunc		tBlend = { GL_ONE, GL_ONE };
		CCSprite*		pBookNor = CCSprite::create ( szFile );
		CCSprite*		pBookSel = CCSprite::create ( szFile );

		pBookSel->setColor ( ccc3 ( 128, 128, 180 ) );
		pBookSel->setBlendFunc ( tBlend );

		// Create CCMenuItemSprites 
		CCMenuItemSprite*	pItem = CCMenuItemSprite::create ( pBookNor, pBookSel, this, menu_selector ( Ch5_HorizScrollMenu::bookClicked ) );
		pMenu->addChild ( pItem, i, i );
	}

	pMenu->setPosition ( ccp ( 240, 150 ) );
	pMenu->alignItemsHorizontallyWithPadding ( 0 );
	this->addChild ( pMenu );

	return KD_TRUE;
}

// Book clicked callback
KDvoid Ch5_HorizScrollMenu::bookClicked ( CCObject* pSender )
{
	CCMenuItemSprite*	pSprite = (CCMenuItemSprite*) pSender;
	this->showMessage ( ccszf ( "Book clicked: %d", pSprite->getTag ( ) ) );
}