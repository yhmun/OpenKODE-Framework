/* --------------------------------------------------------------------------
 *
 *      File            Ch1_2DEasingActions.cpp
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
#include "Ch1_2DEasingActions.h"
#include "Libraries/CCMoveByCustom.h"

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

const KDint		l_nActionCount = sizeof ( l_aActionMethods ) / sizeof ( l_aActionMethods [ 0 ] );

KDbool Ch1_2DEasingActions::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pActionToEases [ 0 ] = CCMoveByCustom::create ( 2, ccp ( 200, 0 ) );
	m_pActionToEases [ 0 ]->retain ( );

	m_pActionToEases [ 1 ] = CCMoveByCustom::create ( 2, ccp ( 0, 200 ) );
	m_pActionToEases [ 1 ]->retain ( );

	CCSprite*	pSprite = CCSprite::create ( "colorable_sprite.png" );
	pSprite->setPosition ( ccp ( 150, 50 ) );
	pSprite->setScale ( 0.5f );
	this->addChild ( pSprite, 1, TAG_SPRITE_TO_MOVE );	

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

	this->schedule ( schedule_selector ( Ch1_2DEasingActions::step ) );

	this->initSubMenus ( l_nActionCount * l_nActionCount );

	CCMenuItemFont*		pReload = CCMenuItemFont::create ( "<RANDOM>", this, menu_selector ( Ch1_2DEasingActions::randomActionCallback ) );	
    pReload->setPosition ( ccp ( 0, 25 ) );
	this->getChildByTag ( TAG_RECIPE_SUB )->addChild ( pReload, 4 );	

	return KD_TRUE;
}

KDvoid Ch1_2DEasingActions::onExit ( KDvoid )
{
	m_pActionToEases [ 0 ]->release ( );
	m_pActionToEases [ 1 ]->release ( );

	Recipe::onExit ( );
}

const KDchar* Ch1_2DEasingActions::runSubAction ( KDint nIndex )
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
	
	KDint		aAction [ 2 ] = 
	{
		nIndex % l_nActionCount,
		nIndex / l_nActionCount 		
	};

	for ( KDint i = 0; i < 2; i++ )
	{		
		CCActionInterval*	pActionToEase = m_pActionToEases [ i ];
		CCActionInterval*	pEaseAction = KD_NULL;	

		switch ( aAction [ i ] )
		{
			case  0 :	pEaseAction = CCEaseIn					::create ( pActionToEase, 2 );		break;
			case  1 :	pEaseAction = CCEaseOut					::create ( pActionToEase, 2 );		break;
			case  2 :	pEaseAction = CCEaseInOut				::create ( pActionToEase, 2 );		break;
			case  3 :	pEaseAction = CCEaseExponentialIn		::create ( pActionToEase );			break;
			case  4 :	pEaseAction = CCEaseExponentialOut		::create ( pActionToEase );			break;
			case  5 :	pEaseAction = CCEaseExponentialInOut	::create ( pActionToEase );			break;
			case  6 :	pEaseAction = CCEaseSineIn				::create ( pActionToEase );			break;
			case  7 :	pEaseAction = CCEaseSineOut				::create ( pActionToEase );			break;
			case  8 :	pEaseAction = CCEaseSineInOut			::create ( pActionToEase );			break;
			case  9 :	pEaseAction = CCEaseElasticIn			::create ( pActionToEase, 0.3f );	break;
			case 10 :	pEaseAction = CCEaseElasticOut			::create ( pActionToEase, 0.3f );	break;
			case 11 :	pEaseAction = CCEaseElasticInOut		::create ( pActionToEase, 0.3f );	break;
			case 12 :	pEaseAction = CCEaseBounceIn			::create ( pActionToEase );			break;
			case 13 :	pEaseAction = CCEaseBounceOut			::create ( pActionToEase );			break;
			case 14 :	pEaseAction = CCEaseBounceInOut			::create ( pActionToEase );			break;
			case 15 :	pEaseAction = CCEaseBackIn				::create ( pActionToEase );			break;
			case 16 :	pEaseAction = CCEaseBackOut				::create ( pActionToEase );			break;
			case 17 :	pEaseAction = CCEaseBackInOut			::create ( pActionToEase );			break;
		}

		pNode->runAction ( pEaseAction );
	}

	return ccszf ( "%s %s", l_aActionMethods [ aAction [ 0 ] ], l_aActionMethods [ aAction [ 1 ] ] );
}

KDvoid Ch1_2DEasingActions::step ( KDfloat fDelta )
{
	CCNode*		pBreadcrumbs = this->getChildByTag ( TAG_BREADCRUMBS );
	CCNode*		pNode = this->getChildByTag ( TAG_SPRITE_TO_MOVE );
	
	CCSprite*	pCrumb = CCSprite::create ( "blank.png" );
	pCrumb->setTextureRect ( CCRect ( 0, 0, 2, 2 ) );
	pCrumb->setColor ( ccc3 ( 255, 255, 0 ) );
	pCrumb->setPosition ( pNode->getPosition ( ) );
	pBreadcrumbs->addChild ( pCrumb );
}

KDvoid Ch1_2DEasingActions::randomActionCallback ( CCObject* pSender )
{
	m_nSubIndex = kdRand ( ) % m_nSubCount;

	Recipe::reloadSubCallback ( pSender );
}