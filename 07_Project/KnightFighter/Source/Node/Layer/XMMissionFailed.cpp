/* --------------------------------------------------------------------------
 *
 *      File            XMMissionFailed.cpp
 *      Description     XMMissionFailed
 *      Author          Pill-Gwan Jeon
 *      Contact         jpg3087@gmail.com
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
#include "XMMissionFailed.h"
#include "Node/Scene/XMReady.h"
#include "Node/Scene/XMMain.h"
#include "Node/Scene/XMStageScene.h"

XMMissionFailed::XMMissionFailed ( )
{

}

XMMissionFailed::~XMMissionFailed ( )
{
	
}


KDbool XMMissionFailed::init ( )
{
	Size			tWinSize = Director::getInstance ( )->getWinSize ( );

	if ( !LayerColor::initWithColor ( Color4B ( 0, 0, 0, 180 ), tWinSize.width, tWinSize.height ) )
	{
		return false;
	}

	m_pUiBg = Sprite::create ( "Faild.jpg" );
	m_pUiBg->setPosition ( Point ( tWinSize.width / 2, 0 ) );
	m_pUiBg->setAnchorPoint ( Point ( 0.5, 0 ) );
	this->addChild ( m_pUiBg );

	Sprite*			pUiBtnRestart = Sprite::create ( "button_restart.png" );
	pUiBtnRestart->setPosition ( Point (  ( m_pUiBg->getContentSize ( ).width / 2 ) - 200, 80 ) );
	pUiBtnRestart->setTag ( UI_BUTTON_RESTART );
	m_pUiBg->addChild ( pUiBtnRestart );
	m_aUiSprite.push_back ( pUiBtnRestart );

	Sprite*			pUiBtnNext = Sprite::create ( "button_previous.png" );
	pUiBtnNext->setPosition ( Point ( ( m_pUiBg->getContentSize ( ).width / 2 ), 80 ) );
	pUiBtnNext->setTag ( UI_BUTTON_NEXT );
	m_pUiBg->addChild ( pUiBtnNext );
	m_aUiSprite.push_back ( pUiBtnNext );

	Sprite*			pUiBtnConfirm = Sprite::create ( "button_confirm.png" );
	pUiBtnConfirm->setPosition ( Point ( ( m_pUiBg->getContentSize ( ).width / 2 ) + 200, 80 ) );
	pUiBtnConfirm->setTag ( UI_BUTTON_CONFIRM );
	m_pUiBg->addChild ( pUiBtnConfirm );
	m_aUiSprite.push_back ( pUiBtnConfirm );

	m_pEventDispatcher->removeAllEventListeners ( );

	this->setTouchEnable ( );


	return KD_TRUE;
}


KDvoid XMMissionFailed::makeRestart ( KDvoid )
{
	Director::getInstance ( )->replaceScene ( XMStageScene::create ( ) );
}

KDvoid XMMissionFailed::makeConfirm ( KDvoid )
{
	Director::getInstance ( )->replaceScene ( XMMain::create ( ) );
}

KDvoid XMMissionFailed::makeGoPrivStage ( KDvoid )
{
	Director::getInstance ( )->replaceScene ( XMReady::create ( ) );
}


KDvoid XMMissionFailed::setTouchEnable ( )
{
	m_pEventListener = EventListenerTouchOneByOne::create ( );
	m_pEventListener->setSwallowTouches ( true );
	m_pEventListener->onTouchBegan		= CC_CALLBACK_2 ( XMMissionFailed::onTouchBegan, this );
	m_pEventListener->onTouchMoved		= CC_CALLBACK_2 ( XMMissionFailed::onTouchMoved, this );
	m_pEventListener->onTouchEnded		= CC_CALLBACK_2 ( XMMissionFailed::onTouchEnded, this );
	m_pEventListener->onTouchCancelled	= CC_CALLBACK_2 ( XMMissionFailed::onTouchCancelled, this );
	
	m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( m_pEventListener, this );
}

KDbool XMMissionFailed::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	return true;
}

KDvoid XMMissionFailed::onTouchMoved ( Touch* pTouch, Event* pEvent )
{

}

KDvoid XMMissionFailed::onTouchEnded ( Touch* pTouch, Event* pEvent )
{

	for ( std::vector<Sprite*>::iterator pIter = m_aUiSprite.begin ( ); pIter != m_aUiSprite.end ( ); pIter++ )
	{
		Sprite*		pUiSprite	= (Sprite*)*pIter;
		Rect		tRect		= pUiSprite->getBoundingBox ( );

		Point		tPoint		= pTouch->getLocation ( );
		
		Rect		tUibgRect   = m_pUiBg->getBoundingBox ( );
		KDbool		bContain	= tRect.containsPoint ( m_pUiBg->convertToNodeSpace ( tPoint ) );

		if ( bContain == true )
		{
			switch ( pUiSprite->getTag ( ) )
			{
				case UI_BUTTON_RESTART :
					CCLOG ( "UI_BUTTON_RESTART" );
					this->makeRestart ( );
					break;
				case UI_BUTTON_CONFIRM :
					CCLOG ( "UI_BUTTON_CONFIRM" );
					this->makeConfirm ( );
					break;
				case UI_BUTTON_NEXT :
					CCLOG ( "UI_BUTTON_NEXT" );
					this->makeGoPrivStage ( );
					break;
			}

		}
	}
}

KDvoid XMMissionFailed::onTouchCancelled	( Touch* pTouch, Event* pEvent )
{

}