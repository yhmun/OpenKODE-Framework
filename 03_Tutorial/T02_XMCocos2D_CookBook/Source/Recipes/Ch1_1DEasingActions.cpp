/* --------------------------------------------------------------------------
 *
 *      File            Ch1_1DEasingActions.cpp
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
#include "Ch1_1DEasingActions.h"

enum
{
	TAG_SPRITE_TO_MOVE	= 0,
	TAG_PREV_ACTION		= 1,
	TAG_RELOAD_ACTION	= 2,
	TAG_NEXT_ACTION		= 3,
	TAG_ACTION_NAME		= 4,
	TAG_BREADCRUMBS		= 5,
};

static const KDchar*	l_aActionMethods [ ] =
{
	"EaseIn"				,
	"EaseOut"				,
	"EaseInOut"				,
	"EaseExponentialIn"		,
	"EaseExponentialOut"	,
	"EaseExponentialInOut"	,
	"EaseSineIn"			,
	"EaseSineOut"			,
	"EaseSineInOut"			,
	"EaseElasticIn"			,
	"EaseElasticOut"		,
	"EaseElasticInOut"		,
	"EaseBounceIn"			,
	"EaseBounceOut"			,
	"EaseBounceInOut"		,
	"EaseBackIn"			,
	"EaseBackOut"			,
	"EaseBackInOut"			,
};

KDbool Ch1_1DEasingActions::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pActionToEase = CCMoveBy::create ( 2, ccp ( 200, 200 ) );
	m_pActionToEase->retain ( );

	CCSprite*	pSprite = CCSprite::create ( "colorable_sprite.png" );
	pSprite->setPosition ( ccp ( 150, 50 ) );
	pSprite->setScale ( 0.5f );
	this->addChild ( pSprite, 1, TAG_SPRITE_TO_MOVE );	
	
	// A dd Breadcrumbs
	CCSprite*	pBreadcrumbs = CCSprite::create ( "blank.png" );
	pBreadcrumbs->setPosition ( ccp ( 0, 0 ) );
	this->addChild ( pBreadcrumbs, 0, TAG_BREADCRUMBS );
		
	CCSprite*	pStart = CCSprite::create ( "blank.png" );
	pStart->setPosition ( ccp ( 150, 50 ) );
	pStart->setTextureRect ( CCRect ( 0, 0, 8, 8 ) );
	pStart->setColor ( ccc3 ( 0, 255, 0 ) );
	this->addChild ( pStart, 0 );
	
	CCSprite*	pEnd = CCSprite::create ( "blank.png" );
	pEnd->setPosition ( ccp ( 350, 250 ) );
	pEnd->setTextureRect ( CCRect ( 0, 0, 8, 8 ) );
	pEnd->setColor ( ccc3 ( 255, 0, 0 ) );
	this->addChild ( pEnd, 0 );

	this->schedule ( schedule_selector ( Ch1_1DEasingActions::step ) );

	this->initSubMenus ( sizeof ( l_aActionMethods ) / sizeof ( l_aActionMethods [ 0 ] ) );

	return KD_TRUE;
}

KDvoid Ch1_1DEasingActions::onExit ( KDvoid )
{
	m_pActionToEase->release ( );

	Recipe::onExit ( );
}

const KDchar* Ch1_1DEasingActions::runSubAction ( KDint nIndex )
{
	// Clean and re-add breadcrumbs
	CCNode*		pBreadcrumbs = this->getChildByTag ( TAG_BREADCRUMBS );
	this->removeChild ( pBreadcrumbs );

	pBreadcrumbs = CCSprite::create ( "blank.png" );
	pBreadcrumbs->setPosition ( ccp ( 0, 0 ) );
	this->addChild ( pBreadcrumbs, 0, TAG_BREADCRUMBS );

	CCNode*		pNode = this->getChildByTag ( TAG_SPRITE_TO_MOVE );
	pNode->stopAllActions ( );
	pNode->setPosition ( ccp ( 150, 50 ) );

	CCActionInterval*	pEaseAction = KD_NULL;

	switch ( nIndex )
	{
		case  0 :	pEaseAction = CCEaseIn					::create ( m_pActionToEase, 2 );	break;
		case  1 :	pEaseAction = CCEaseOut					::create ( m_pActionToEase, 2 );	break;
		case  2 :	pEaseAction = CCEaseInOut				::create ( m_pActionToEase, 2 );	break;
		case  3 :	pEaseAction = CCEaseExponentialIn		::create ( m_pActionToEase );		break;
		case  4 :	pEaseAction = CCEaseExponentialOut		::create ( m_pActionToEase );		break;
		case  5 :	pEaseAction = CCEaseExponentialInOut	::create ( m_pActionToEase );		break;
		case  6 :	pEaseAction = CCEaseSineIn				::create ( m_pActionToEase );		break;
		case  7 :	pEaseAction = CCEaseSineOut				::create ( m_pActionToEase );		break;
		case  8 :	pEaseAction = CCEaseSineInOut			::create ( m_pActionToEase );		break;
		case  9 :	pEaseAction = CCEaseElasticIn			::create ( m_pActionToEase, 0.3f );	break;
		case 10 :	pEaseAction = CCEaseElasticOut			::create ( m_pActionToEase, 0.3f );	break;
		case 11 :	pEaseAction = CCEaseElasticInOut		::create ( m_pActionToEase, 0.3f );	break;
		case 12 :	pEaseAction = CCEaseBounceIn			::create ( m_pActionToEase );		break;
		case 13 :	pEaseAction = CCEaseBounceOut			::create ( m_pActionToEase );		break;
		case 14 :	pEaseAction = CCEaseBounceInOut			::create ( m_pActionToEase );		break;
		case 15 :	pEaseAction = CCEaseBackIn				::create ( m_pActionToEase );		break;
		case 16 :	pEaseAction = CCEaseBackOut				::create ( m_pActionToEase );		break;
		case 17 :	pEaseAction = CCEaseBackInOut			::create ( m_pActionToEase );		break;
	}

	pNode->runAction ( pEaseAction );

	return l_aActionMethods [ nIndex ];
}

KDvoid Ch1_1DEasingActions::step ( KDfloat fDelta )
{
	CCNode*		pBreadcrumbs = this->getChildByTag ( TAG_BREADCRUMBS );
	CCNode*		pNode = this->getChildByTag ( TAG_SPRITE_TO_MOVE );
	
	CCSprite*	pCrumb = CCSprite::create ( "blank.png" );
	pCrumb->setTextureRect ( CCRect ( 0, 0, 2, 2 ) );
	pCrumb->setColor ( ccc3 ( 255, 255, 0 ) );
	pCrumb->setPosition ( pNode->getPosition ( ) );
	pBreadcrumbs->addChild ( pCrumb );
}