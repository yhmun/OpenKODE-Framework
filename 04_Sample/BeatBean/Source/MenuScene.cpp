/* --------------------------------------------------------------------------
 *
 *      File            MenuScene.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      t2cn on 2009/10/08
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "MenuScene.h"
#include "GameScene.h"

KDbool MenuScene::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCMenuItemFont::setFontSize ( 32 );
	CCMenuItemFont::setFontName ( "Arial.ttf" );

	CCLayerColor*	pLayer = CCLayerColor::create ( ccc4 ( 32, 32, 32, 255 ) );
	this->addChild ( pLayer, 1 ); 

	CCMenuItemFont*	pStart = CCMenuItemFont::create ( "START", this, menu_selector ( MenuScene::onStart ) );
	pStart->setColor ( ccc3 ( 0x00, 0xCC, 0x00 ) );

	CCMenuItemFont*	pContinue = CCMenuItemFont::create ( "CONTINUE", this, menu_selector ( MenuScene::onContinue ) );
	pContinue->setColor ( ccc3 ( 0xFF, 0x99, 0x00 ) );

	CCMenuItemFont*	pOption = CCMenuItemFont::create ( "OPTION", this, menu_selector ( MenuScene::onOption ) );
	pOption->setColor ( ccc3 ( 0x00, 0x66, 0xFF ) );

	CCMenuItemFont*	pHelp = CCMenuItemFont::create ( "HELP", this, menu_selector ( MenuScene::onHelp ) );
	pHelp->setColor ( ccc3 ( 0xFF, 0x66, 0x66 ) );

	CCMenu*			pMenu = CCMenu::create ( pStart, pContinue, pOption, pHelp, KD_NULL );	
	pMenu->setPosition ( ccpMid ( m_tContentSize ) );
	pMenu->alignItemsVerticallyWithPadding ( 20.0f );
	
	// elastic effect
	KDint			i = 0;
	CCObject*		pObject;
	CCARRAY_FOREACH ( pMenu->getChildren ( ), pObject )
	{
		CCNode*		pChild  = (CCNode*) pObject;
		CCPoint		tDstPos = pChild->getPosition ( );
		KDfloat		fOffset = m_tContentSize.cx / 2 + 50;
		
		if ( i % 2 == 0 )
		{
			fOffset = -fOffset;
		}

		pChild->setPositionX ( tDstPos.x + fOffset );
		pChild->runAction ( CCEaseElasticOut::create ( CCMoveBy::create ( 2, ccp ( tDstPos.x - fOffset, 0 ) ), 0.35f ) );

		i++;
	}

	this->addChild ( pMenu, 2 );

	return KD_TRUE;
}	

KDvoid MenuScene::onStart ( CCObject* pSender )
{
	CCDirector::sharedDirector ( )->pushScene ( CCTransitionSlideInL::create ( 1, GameScene::scene ( ) ) );
}

KDvoid MenuScene::onContinue ( CCObject* pSender )
{

}

KDvoid MenuScene::onOption ( CCObject* pSender )
{

}

KDvoid MenuScene::onHelp ( CCObject* pSender )
{

}



