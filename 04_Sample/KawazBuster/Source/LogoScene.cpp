/* --------------------------------------------------------------------------
 *
 *      File            LogoScene.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/05/27 
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
#include "LogoScene.h"
#include "TitleScene.h"

KDbool LogoScene::init ( KDvoid )
{
	m_tBackgroundColor = ccc4 ( 255, 255, 255, 255 ); 

	if ( !KWScene::init ( ) )
	{
		return KD_FALSE;
	}

	KWSprite*	pLogo = KWSprite::create ( "image/title/kawaz-hd.png" ); 
    pLogo->setPosition ( ccpMid ( m_tLyrSize ) );
    pLogo->setOpacity ( 0 ); 
	pLogo->getTexture ( )->setAliasTexParameters ( );

	pLogo->runAction
	(
		CCSequence::create
		(
			CCFadeIn	::create ( 2 ),
			CCDelayTime	::create ( 2 ),
			CCFadeOut	::create ( 2 ),
			CCCallFunc	::create ( this, callfunc_selector ( LogoScene::goToTitle ) ),
			KD_NULL
		)
	);

	this->addChild ( pLogo );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}		

KDbool LogoScene::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	return KD_TRUE;
}

KDvoid LogoScene::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	this->goToTitle ( );
}

KDvoid LogoScene::goToTitle ( KDvoid )
{
	CCTransitionFade*	pTransition = CCTransitionFade::create ( 0.5f, TitleScene::scene ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( pTransition );
}
