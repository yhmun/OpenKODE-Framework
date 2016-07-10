/* --------------------------------------------------------------------------
 *
 *      File            XMReady.cpp
 *      Description     XMReady
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
#include "XMReady.h"

#include "Node/Scene/XMStageScene.h"

XMReady::XMReady ( KDvoid )
{

}

XMReady::~XMReady ( KDvoid )
{

}

KDbool init ( KDvoid );

KDbool XMReady::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return KD_FALSE;
	}


	Sprite*		pSpriteBgReady = Sprite::create ( "bg_ready.png" );
	pSpriteBgReady->setPosition ( Point ( m_tContentSize / 2 ) );
	this->addChild ( pSpriteBgReady );

	Sprite*		pBtnStart = Sprite::create ( "btn_start.png" );
	pBtnStart->setPosition ( Point ( m_tContentSize.width - 230, 70 ) );
	pBtnStart->setTag ( UI_BUTTON_START );
	pSpriteBgReady->addChild ( pBtnStart );
	m_aUiSprite.push_back ( pBtnStart );

	this->setTouchEnable ( );

	return KD_TRUE;
}

KDvoid XMReady::changeScene ( )
{
	CCLOG ( "game start..." );

	Director::getInstance ( )->replaceScene ( XMStageScene::create ( ) );
}


KDvoid XMReady::setTouchEnable ( )
{
	m_pEventListener = EventListenerTouchOneByOne::create ( );
	m_pEventListener->setSwallowTouches ( true );
	m_pEventListener->onTouchBegan		= CC_CALLBACK_2 ( XMReady::onTouchBegan, this );
	m_pEventListener->onTouchMoved		= CC_CALLBACK_2 ( XMReady::onTouchMoved, this );
	m_pEventListener->onTouchEnded		= CC_CALLBACK_2 ( XMReady::onTouchEnded, this );
	m_pEventListener->onTouchCancelled	= CC_CALLBACK_2 ( XMReady::onTouchCancelled, this );
	
	m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( m_pEventListener, this );
}

KDbool XMReady::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	return true;
}

KDvoid XMReady::onTouchMoved ( Touch* pTouch, Event* pEvent )
{
}

KDvoid XMReady::onTouchEnded ( Touch* pTouch, Event* pEvent )
{
	for ( std::vector<Sprite*>::iterator pIter = m_aUiSprite.begin ( ); pIter != m_aUiSprite.end ( ); pIter++ )
	{
		Sprite*		pUiSprite	= (Sprite*)*pIter;
		Rect		tRect		= pUiSprite->getBoundingBox ( );

		Point		tPoint		= pTouch->getLocation ( );
		KDbool		bContain	= tRect.containsPoint ( this->convertToNodeSpace ( tPoint ) );

		if ( bContain == true )
		{
			switch ( pUiSprite->getTag ( ) )
			{
				case UI_BUTTON_START :
					this->changeScene ( );
					break;
			}

		}

	}
}

KDvoid XMReady::onTouchCancelled	( Touch* pTouch, Event* pEvent )
{

}