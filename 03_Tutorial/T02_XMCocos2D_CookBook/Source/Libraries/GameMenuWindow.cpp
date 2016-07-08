/* --------------------------------------------------------------------------
 *
 *      File            GameMenuWindow.cpp
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
#include "GameMenuWindow.h"

static KDfloat	l_fTitleBarHeight	= 20.0f;
static KDfloat	l_fBorderSize		= 2.0f;

enum 
{
	TS_NONE	,
	TS_TAP	,
	TS_HOLD	,
	TS_DRAG	,
};

GameMenuWindow* GameMenuWindow::create ( const KDchar* szTitle, const CCSize& tSize )
{
	GameMenuWindow*		pRet = new GameMenuWindow ( );

	if ( pRet && pRet->initWithTitle ( szTitle, tSize ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool GameMenuWindow::initWithTitle ( const KDchar* szTitle, const CCSize& tSize )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	
	m_sTitle = szTitle;
	m_tSize  = tSize;
	
	m_pBg = CCSprite::create ( "blank.png" );
	m_pBg->setTextureRect ( CCRect ( 0, 0, tSize.cx, tSize.cy ) );
	m_pBg->setColor ( ccc3 ( 0, 0, 0 ) );
	m_pBg->setPosition ( ccp ( 0, 0 ) );
	m_pBg->setOpacity ( 100 );
	this->addChild ( m_pBg );

	CCSize		tContentBgSize = ccs ( tSize.cx - l_fBorderSize * 2, tSize.cy - ( l_fBorderSize * 3 + l_fTitleBarHeight ) );
	
	m_pContent = CCSprite::create ( "blank.png" );
	m_pContent->setTextureRect ( CCRect ( 0, 0, tContentBgSize.cx, tContentBgSize.cy ) );
	m_pContent->setColor ( ccc3 ( 255, 255, 255 ) );
	m_pContent->setPosition ( ccp ( 0, -l_fTitleBarHeight / 2 - l_fBorderSize / 2 ) );
	this->addChild ( m_pContent );

	m_pTitleBar = CCSprite::create ( "blank.png" );
	m_pTitleBar->setTextureRect ( CCRect ( 0, 0, tSize.cx - l_fBorderSize * 2, l_fTitleBarHeight ) );
	m_pTitleBar->setColor ( ccc3 ( 200, 200, 255 ) );
	m_pTitleBar->setPosition ( ccp ( 0, tSize.cy / 2 - l_fBorderSize - l_fTitleBarHeight / 2 ) );
	m_pTitleBar->setOpacity ( 255 );
	this->addChild ( m_pTitleBar );

	// Title Item
	CCMenuItemFont::setFontSize ( 20 );
	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );

	CCMenuItemFont*		pTitleItem = CCMenuItemFont::create ( szTitle );
	pTitleItem->setPosition ( ccp ( -238, -162 ) );
	pTitleItem->setAnchorPoint ( ccp ( 0, 0 ) );
	pTitleItem->setDisabledColor ( ccc3 ( 0, 0, 0 ) );
	pTitleItem->setEnabled ( KD_FALSE );

	CCMenu*				pMenu1 = CCMenu::create ( pTitleItem, KD_NULL );
	pMenu1->setPosition ( ccp ( 240, 160 ) );
	m_pTitleBar->addChild ( pMenu1 );

	// Minimize Button
	CCMenuItemFont::setFontSize ( 40 );
	CCMenuItemFont::setFontName ( "Arial.ttf" );

	CCMenuItemFont*		pMinMinusFont = CCMenuItemFont::create ( "-" );
	pMinMinusFont->setColor ( ccc3 ( 0, 0, 0 ) );

	CCMenuItemFont::setFontSize ( 30 );

	CCMenuItemFont*		pMinPlusFont = CCMenuItemFont::create ( "+" );
	pMinPlusFont->setColor ( ccc3 ( 0, 0, 0 ) );

	CCMenuItemToggle*	pItemToggle = CCMenuItemToggle::createWithTarget ( this, menu_selector ( GameMenuWindow::minimize ), pMinMinusFont, pMinPlusFont, KD_NULL );
	
	CCMenu*				pItemToggleMenu = CCMenu::create ( pItemToggle, KD_NULL );
	pItemToggleMenu->setPosition ( ccp ( tSize.cx - 20.0f, 10.0f ) );
	m_pTitleBar->addChild ( pItemToggleMenu );
	
	m_bIsOpen = KD_TRUE;

	return KD_TRUE;
}

KDvoid GameMenuWindow::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	m_nTouchHash = pTouch->getID ( );
	m_bIsTouched = KD_TRUE;
	m_tTouchedPoint = tPoint;

	this->bringToFront ( );
}

KDvoid GameMenuWindow::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( !m_bIsTouched )
	{
		return;
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	if ( m_nTouchHash == pTouch->getID ( ) )
	{
		// If we moved on this sprite
		CCPoint		tTouchedDiff = ccp ( tPoint.x - m_tTouchedPoint.x, tPoint.y - m_tTouchedPoint.y );
		CCPoint		tPosition = this->getPosition ( );
		this->setPosition ( ccp ( tPosition.x + tTouchedDiff.x, tPosition.y + tTouchedDiff.y ) );
		m_tTouchedPoint = tPoint;		
	}
}

KDvoid GameMenuWindow::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( !m_bIsTouched )
	{
		return;
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	if ( m_nTouchHash == pTouch->getID ( ) )
	{
		m_bIsTouched = KD_FALSE;
	}
}

KDvoid GameMenuWindow::minimize ( CCObject* pSender )
{
	m_bIsOpen = !m_bIsOpen;
	
	if ( m_bIsOpen )
	{
		m_pContent->setVisible ( KD_TRUE );
		m_pBg->setVisible ( KD_TRUE );
	}
	else
	{
		m_pContent->setVisible ( KD_FALSE );
		m_pBg->setVisible ( KD_FALSE );
	}
	
	this->bringToFront ( );
}

CCRect GameMenuWindow::titleBarRect ( KDvoid )
{
	KDfloat		fScaleMod = 1.0f;
	
	CCSize		tSize;
	tSize.cx = m_tSize.cx * fScaleMod;
	tSize.cy = l_fTitleBarHeight * fScaleMod;
	
	CCPoint		tPoint;
	tPoint.x = m_pTitleBar->getPosition ( ).x + this->getPosition ( ).x - m_tSize.cx / 2;
	tPoint.y = m_pTitleBar->getPosition ( ).y + this->getPosition ( ).y - l_fTitleBarHeight / 2;
	
	return CCRect ( tPoint.x, tPoint.y, tSize.cx, tSize.cy ); 
}

CCRect GameMenuWindow::rect ( KDvoid )
{
	if ( !m_bIsOpen )
	{
		return this->titleBarRect ( );
	}

	KDfloat		fScaleMod = 1.0f;
	
	CCSize		tSize = this->getContentSize ( );
	tSize.cx = m_tSize.cx * fScaleMod;
	tSize.cy = m_tSize.cy * fScaleMod;
	
	CCPoint		tPoint = this->getPosition ( );
	tPoint.x = tPoint.x - tSize.cx / 2;
	tPoint.y = tPoint.y - tSize.cy / 2;
	
	return CCRect ( tPoint.x, tPoint.y, tSize.cx, tSize.cy ); 
}

KDvoid GameMenuWindow::bringToFront ( KDvoid )
{
	KDint		nCount = 1;

	CCObject*	pObject;
	CCARRAY_FOREACH ( this->getParent ( )->getChildren ( ), pObject )
	{
		GameMenuWindow*		pChild = (GameMenuWindow*) pObject;

		this->getParent ( )->reorderChild ( pChild, nCount );

		nCount++;

		pChild->getTitleBar ( )->setColor ( ccc3 ( 200, 200, 255 ) );
	}

	this->getParent ( )->reorderChild ( this, nCount + 1 );

	m_pTitleBar->setColor ( ccc3 ( 100, 100, 255 ) );
}

KDbool GameMenuWindow::isOpen ( KDvoid )
{
	return m_bIsOpen;
}

KDvoid GameMenuWindow::setOpen ( KDbool bIsOpen )
{
	m_bIsOpen = bIsOpen;
}

const CCSize& GameMenuWindow::getSize ( KDvoid )
{
	return m_tSize;
}

KDvoid GameMenuWindow::setSize ( const CCSize& tSize )
{
	m_tSize = tSize;
}

const KDchar* GameMenuWindow::getTitle ( KDvoid )
{
	return m_sTitle.c_str ( );
}

KDvoid GameMenuWindow::setTitle ( const KDchar* szTitle )
{
	m_sTitle = szTitle;
}

CCSprite* GameMenuWindow::getContent ( KDvoid )
{
	return m_pContent;
}

KDvoid GameMenuWindow::setContent ( CCSprite* pContent )
{
	m_pContent = pContent;
}

CCSprite* GameMenuWindow::getBg ( KDvoid )
{
	return m_pBg;
}

KDvoid GameMenuWindow::setBg ( CCSprite* pBg )
{
	m_pBg = pBg;
}

CCSprite* GameMenuWindow::getTitleBar ( KDvoid )
{
	return m_pTitleBar;
}

KDvoid GameMenuWindow::setTitleBar ( CCSprite* pTitleBar )
{
	m_pTitleBar = pTitleBar;
}

KDint GameMenuWindow::getTouchHash ( KDvoid )
{
	return m_nTouchHash;
}

KDvoid GameMenuWindow::setTouchHash ( KDint nTouchHash )
{
	m_nTouchHash = nTouchHash;
}

KDbool GameMenuWindow::isTouched ( KDvoid )
{
	return m_bIsTouched;
}

KDvoid GameMenuWindow::setTouched ( KDbool bIsTouched )
{
	m_bIsTouched = bIsTouched;
}

