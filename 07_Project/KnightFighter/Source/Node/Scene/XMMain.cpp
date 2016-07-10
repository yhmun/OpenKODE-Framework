/* --------------------------------------------------------------------------
 *
 *      File            XMMain.cpp
 *      Description     XMMain
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
#include "XMMain.h"
#include "XMChapter.h"


XMMain::XMMain ( KDvoid )
{

}

XMMain::~XMMain ( KDvoid )
{

}

KDbool init ( KDvoid );

KDbool XMMain::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return KD_FALSE;
	}


	Sprite*		pSpriteBgMain = Sprite::create ( "bg_main.png" );
	pSpriteBgMain->setPosition ( Point ( m_tContentSize / 2 ) );
	this->addChild ( pSpriteBgMain );

	Sprite*		pBtnStart = Sprite::create ( "btn_start.png" );
	pBtnStart->setPosition ( Point ( m_tContentSize.width - 230, 70 ) );
	pSpriteBgMain->addChild ( pBtnStart );
	pBtnStart->setTag ( UI_BUTTON_START );
	m_aUiSprite.push_back ( pBtnStart );

	this->setTouchEnable ( );

	return KD_TRUE;
}

KDvoid XMMain::changeScene ( )
{
	CCLOG ( "changeScene..." );

	Director::getInstance ( )->replaceScene ( XMChapter::create ( ) );
}


KDvoid XMMain::setTouchEnable ( )
{
	m_pEventListener = EventListenerTouchOneByOne::create ( );
	m_pEventListener->setSwallowTouches ( true );
	m_pEventListener->onTouchBegan		= CC_CALLBACK_2 ( XMMain::onTouchBegan, this );
	m_pEventListener->onTouchMoved		= CC_CALLBACK_2 ( XMMain::onTouchMoved, this );
	m_pEventListener->onTouchEnded		= CC_CALLBACK_2 ( XMMain::onTouchEnded, this );
	m_pEventListener->onTouchCancelled	= CC_CALLBACK_2 ( XMMain::onTouchCancelled, this );
	
	m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( m_pEventListener, this );
}

KDbool XMMain::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	return true;
}

KDvoid XMMain::onTouchMoved ( Touch* pTouch, Event* pEvent )
{
}

KDvoid XMMain::onTouchEnded ( Touch* pTouch, Event* pEvent )
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

KDvoid XMMain::onTouchCancelled	( Touch* pTouch, Event* pEvent )
{

}