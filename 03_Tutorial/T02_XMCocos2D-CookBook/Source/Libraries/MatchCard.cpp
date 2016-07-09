/* --------------------------------------------------------------------------
 *
 *      File            MatchCard.cpp
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
#include "MatchCard.h"

MatchCard* MatchCard::createWithSpriteFrameName ( const KDchar* szSpriteFrameName )
{
	MatchCard*		pRet = new MatchCard ( );

	if ( pRet && pRet->initWithSpriteFrameName ( szSpriteFrameName ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

MatchCard::MatchCard ( KDvoid )
{
	m_pCardDown = KD_NULL;
	m_pCardUp   = KD_NULL;
}

MatchCard::~MatchCard ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pCardDown );
	CC_SAFE_RELEASE ( m_pCardUp );
}

KDbool MatchCard::initWithSpriteFrameName ( const KDchar* szSpriteFrameName )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_bCardIsUp = KD_TRUE;
	m_bAnimating = KD_FALSE;
	m_bHasBeenFlipped = KD_FALSE;	

	CCSprite*	pCardDown = CCSprite::createWithSpriteFrameName ( "match_card_back.png" );
	pCardDown->setOpacity ( 0 );
	pCardDown->setScale ( 0.5f );
	this->setCardDown ( pCardDown );

	CCSprite*	pCardUp = CCSprite::createWithSpriteFrameName ( szSpriteFrameName );
	pCardUp->setOpacity ( 255 );
	pCardUp->setScale ( 0.5f );
	this->setCardUp ( pCardUp );

	m_sCardType = szSpriteFrameName;

	return KD_TRUE;
}

KDvoid MatchCard::setAllPositions ( const CCPoint& tPosition )
{
	m_pCardDown	->setPosition ( tPosition );
	m_pCardUp	->setPosition ( tPosition );
	this		->setPosition ( tPosition );
}

KDvoid MatchCard::flipCard ( KDvoid )
{

	if ( m_bAnimating )
	{
		return;
	}
	
	m_bHasBeenFlipped = KD_TRUE;
	
	const CCPoint&		tPosition = this->getPosition ( );

	m_pCardDown	->setPosition ( tPosition );
	m_pCardUp	->setPosition ( tPosition );

	m_pCardDown	->setOpacity ( 0 );
	m_pCardUp	->setOpacity ( 0 );
	
	CCActionInterval*	pFlipCardIn = CCSequence::create
	(
		CCSpawn::create 
		(
			CCSequence::create 
			(
				CCScaleTo::create ( 0.25f, 0.75f ),
				CCScaleTo::create ( 0.25f, 0.5f ),
				KD_NULL 
			),			
			CCFlipX3D::create ( 0.5f ),
			CCSequence::create 
			(
				CCDelayTime::create ( 0.325f ),
				CCFadeIn::create ( 0 ),
				KD_NULL 
			),	
			CCMoveTo::create ( 0.5f, ccp ( 480 - tPosition.x, tPosition.y ) ),
			KD_NULL
		),
		CCCallFunc::create ( this, callfunc_selector ( MatchCard::doneAnimating ) ),
		KD_NULL
	);

	CCActionInterval*	pFlipCardOut = CCSequence::create
	(
		CCFadeIn::create ( 0 ),
		CCSpawn::create 
		(
			CCSequence::create 
			(
				CCScaleTo::create ( 0.25f, 0.75f ),
				CCScaleTo::create ( 0.25f, 0.5f ),
				KD_NULL 
			),		
			CCFlipX3D::create ( 0.5f ),
			CCSequence::create 
			(
				CCDelayTime::create ( 0.325f ),
				CCFadeOut::create ( 0 ),
				KD_NULL 
			),	
			CCMoveTo::create ( 0.5f, ccp ( 480 - tPosition.x, tPosition.y ) ),
			KD_NULL
		),
		CCCallFunc::create ( this, callfunc_selector ( MatchCard::doneAnimating ) ),
		KD_NULL
	);

	m_pCardUp	->getParent ( )->reorderChild ( m_pCardUp	, 1 ); 
	m_pCardDown	->getParent ( )->reorderChild ( m_pCardDown	, 1 ); 

	if ( !m_bCardIsUp )
	{
		m_pCardDown	->runAction ( pFlipCardOut );
		m_pCardUp	->runAction ( pFlipCardIn  );
		m_bCardIsUp = KD_TRUE;
	}
	else
	{
		m_pCardDown	->runAction ( pFlipCardIn  );
		m_pCardUp	->runAction ( pFlipCardOut );
		m_bCardIsUp = KD_FALSE;
	}

	m_bAnimating = KD_TRUE;
}

KDvoid MatchCard::flipCardNoAnim ( KDvoid )
{
	if ( m_bAnimating )
	{
		return;
	}
	
	if ( !m_bCardIsUp )
	{
		m_pCardDown->setOpacity ( 0 );
		m_pCardUp->setOpacity ( 255 );
		m_bCardIsUp = KD_TRUE;
	}
	else
	{
		m_pCardDown->setOpacity ( 255 );
		m_pCardUp->setOpacity ( 0 );
		m_bCardIsUp = KD_FALSE;
	}
}

KDvoid MatchCard::processTouch ( const CCPoint& tPoint )
{
	this->flipCard ( );
}

KDvoid MatchCard::doneAnimating ( KDvoid )
{
	m_bAnimating = KD_FALSE;

	m_pCardUp	->getParent ( )->reorderChild ( m_pCardUp	, 0 ); 
	m_pCardDown	->getParent ( )->reorderChild ( m_pCardDown	, 0 ); 
}

CCRect MatchCard::rect ( KDvoid )
{
	KDfloat		fScaleMod = 1.0f;
	KDfloat		fScale = m_pCardUp->getScale ( );
	
	CCSize		tSize = m_pCardUp->getContentSize ( );
	tSize.cx = tSize.cx * fScale * fScaleMod;
	tSize.cy = tSize.cy * fScale * fScaleMod;
	
	CCPoint		tPoint = this->getPosition ( );
	tPoint.x = tPoint.x - tSize.cx / 2;
	tPoint.y = tPoint.y - tSize.cy / 2;
	
	return CCRect ( tPoint.x, tPoint.y, tSize.cx, tSize.cy ); 
}

KDbool MatchCard::isCardUp ( KDvoid )
{
	return m_bCardIsUp;
}

KDvoid MatchCard::setCardUp ( KDbool bIsUp )
{
	m_bCardIsUp = bIsUp;
}

CCSprite* MatchCard::getCardUp ( KDvoid )
{
	return m_pCardUp;
}

KDvoid MatchCard::setCardUp ( CCSprite* pCardUp )
{
	CC_SAFE_RELEASE ( m_pCardUp );
	m_pCardUp = pCardUp;
	CC_SAFE_RETAIN ( m_pCardUp );
}

CCSprite* MatchCard::getCardDown ( KDvoid )
{
	return m_pCardDown;
}

KDvoid MatchCard::setCardDown ( CCSprite* pCardDown )
{
	CC_SAFE_RELEASE ( m_pCardDown );
	m_pCardDown = pCardDown;
	CC_SAFE_RETAIN ( m_pCardDown );
}

const KDchar* MatchCard::getCardType ( KDvoid )
{
	return m_sCardType.c_str ( );
}

KDvoid MatchCard::setCardType ( const KDchar* szCardType )
{
	m_sCardType = szCardType;
}
