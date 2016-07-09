/* --------------------------------------------------------------------------
 *
 *      File            HowtoScene.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/23 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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
#include "HowtoScene.h"

KDbool HowtoScene::init ( KDvoid )
{
	if ( !KWScene::init ( ) )
	{
		return KD_FALSE;
	}

	KWSprite*		pBG = KWSprite::create ( "image/title/title_background-hd.png" );
	pBG->setPosition ( ccpMid ( m_tLyrSize ) );

	CCLayerColor*	pLayer = CCLayerColor::create ( ccc4 ( 128, 128, 128, 200 ), m_tLyrSize ); 
	
	KWSprite*		pHowto = KWAnimation::spriteWithFiles
	(
		CCArray::create
		(
			CCString::create ( "image/howto/howto0-hd.png" ),
			CCString::create ( "image/howto/howto1-hd.png" ),
			KD_NULL
		), 
		15.0 / FPS 
	);
    pHowto->setPosition ( ccpMid ( m_tLyrSize ) );

	CCMenu*			pMenu = CCMenu::create
	(
		CCMenuItemImage::create 
		(
			"image/howto/howto_button-hd.png", "image/howto/howto_button_selected-hd.png",
			this, menu_selector ( HowtoScene::pressBackButton ) 
		),
		KD_NULL
	);

    pMenu->setPosition ( ccp ( m_tLyrSize.cx - 184, 68 ) );
    this->addChild ( pBG );
    this->addChild ( pLayer );
    this->addChild ( pHowto );
    this->addChild ( pMenu );

	return KD_TRUE;
}

KDvoid HowtoScene::pressBackButton  ( CCObject* pSender )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/pico.wav" );	
	CCDirector::sharedDirector ( )->popScene ( );
}