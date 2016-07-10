/* --------------------------------------------------------------------------
 *
 *      File            XMChapter.cpp
 *      Description     XMChapter
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
#include "XMChapter.h"
#include "XMReady.h"

XMChapter::XMChapter ( KDvoid )
{

}

XMChapter::~XMChapter ( KDvoid )
{

}

KDbool init ( KDvoid );

KDbool XMChapter::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return KD_FALSE;
	}

	Sprite*		pSpriteChapterogo = Sprite::create ( "Chapter.jpg" );
	pSpriteChapterogo->setPosition ( Point ( m_tContentSize / 2 ) );
	this->addChild ( pSpriteChapterogo );
/*
	m_pChapterSel = XMChapterSelect::create ( 0 );
	m_pChapterSel->setPosition ( Point ( 0, 0 ) );
	m_pChapterSel->setAnchorPoint ( Point ( 0, 0 ) );
	this->addChild ( m_pChapterSel );
*/
	this->setTouchEnable ( );

	return KD_TRUE;
}

KDvoid XMChapter::changeScene ( )
{
	CCLOG ( "changeScene..." );

	Director::getInstance ( )->replaceScene ( XMReady::create ( ) );

}
KDvoid XMChapter::setTouchEnable ( )
{
	m_pEventListener = EventListenerTouchOneByOne::create ( );
	m_pEventListener->setSwallowTouches ( true );
	m_pEventListener->onTouchBegan		= CC_CALLBACK_2 ( XMChapter::onTouchBegan, this );
	m_pEventListener->onTouchMoved		= CC_CALLBACK_2 ( XMChapter::onTouchMoved, this );
	m_pEventListener->onTouchEnded		= CC_CALLBACK_2 ( XMChapter::onTouchEnded, this );
	m_pEventListener->onTouchCancelled	= CC_CALLBACK_2 ( XMChapter::onTouchCancelled, this );
	
	m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( m_pEventListener, this );
}

KDbool XMChapter::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	return true;
}

KDvoid XMChapter::onTouchMoved ( Touch* pTouch, Event* pEvent )
{
}

KDvoid XMChapter::onTouchEnded ( Touch* pTouch, Event* pEvent )
{
	this->changeScene();
	return;
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
				case UI_BUTTON_SCENARIO :
					this->changeScene ( );
					break;
			}

		}

	}
}

KDvoid XMChapter::onTouchCancelled	( Touch* pTouch, Event* pEvent )
{

}