/* --------------------------------------------------------------------------
 *
 *      File            CreditScene.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/06/30
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
#include "CreditScene.h"

KDbool CreditScene::init ( KDvoid )
{
	if ( !KWScene::init ( ) )
	{
		return KD_FALSE;
	}
 
	KWSprite*		pBG = KWSprite::create ( "image/credit/credit_background-hd.png" );
	pBG->setPosition ( ccpMid ( m_tLyrSize ) );

	CCLayerColor*	pLayer = CCLayerColor::create ( ccc4 ( 128, 128, 128, 200 ), m_tLyrSize ); 
	
	KWSprite*		pCredit = KWAnimation::spriteWithFiles
	(
		CCArray::create
		(
			CCString::create ( "image/credit/credit0-hd.png" ),
			CCString::create ( "image/credit/credit1-hd.png" ),
			KD_NULL
		), 
		15.0 / FPS 
	);
    pCredit->setPosition ( ccpMid ( m_tLyrSize ) );

	CCMenu*			pMenu = CCMenu::create
	(
		CCMenuItemImage::create 
		(
			"image/credit/credit_button-hd.png", "image/credit/credit_button_selected-hd.png",
			this, menu_selector ( CreditScene::pressBackButton ) 
		),
		KD_NULL
	);

    pMenu->setPosition ( ccp ( m_tLyrSize.cx - 184, 68 ) );
    this->addChild ( pBG );
    this->addChild ( pLayer );
    this->addChild ( pCredit );
    this->addChild ( pMenu );

	return KD_TRUE;
}

KDvoid CreditScene::pressBackButton  ( CCObject* pSender )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/pico.wav" );	
	CCDirector::sharedDirector ( )->popScene ( );
}