/* --------------------------------------------------------------------------
 *
 *      File            Mole.cpp
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
#include "Mole.h"

Mole* Mole::createWithSpriteFrameName ( const KDchar* szSpriteFrameName )
{
	Mole*		pRet = new Mole ( );

	if ( pRet && pRet->init ( ) && pRet->initWithSpriteFrameName ( szSpriteFrameName ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Mole::Mole ( KDvoid )
{
	m_pNormalAnim	= KD_NULL;
	m_pHitAnim		= KD_NULL;
	m_pDelegate		= KD_NULL;
}

Mole::~Mole ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pNormalAnim );
	CC_SAFE_RELEASE ( m_pHitAnim );
}

KDbool Mole::init ( KDvoid )
{
	if ( !TouchableSprite::init ( ) )
	{
		return KD_FALSE;
	}

	m_nState = MOLE_DOWN;
	
	CC_SAFE_RELEASE ( m_pNormalAnim );
	CC_SAFE_RELEASE ( m_pHitAnim );

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	
	m_pNormalAnim = CCAnimation::create ( );
	m_pNormalAnim->setDelayPerUnit ( 1.0f ); 
	m_pNormalAnim->addSpriteFrame ( pCache->spriteFrameByName ( "mole_normal.png" ) );
	m_pNormalAnim->retain ( );
	
	m_pHitAnim = CCAnimation::create ( );
	m_pHitAnim->setDelayPerUnit ( 1.0f ); 
	m_pHitAnim->addSpriteFrame ( pCache->spriteFrameByName ( "mole_hit.png" ) );
	m_pHitAnim->retain ( );

	this->runAction ( CCRepeatForever::create ( CCAnimate::create ( m_pNormalAnim ) ) );

	return KD_TRUE;
}

KDvoid Mole::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	if ( pointIsInRect ( tPoint, this->rect ( ) ) )
	{		
		m_nTouchHash = pTouch->getID ( );
		this->processTouch ( tPoint );
	}
}

CCRect Mole::rect ( KDvoid )
{
	// We set our scale mod to make sprite easier to press.
	// This also lets us press 2 sprites with 1 touch if they are sufficiently close.

	KDfloat		fScaleMod = 1.5f;
	KDfloat		fScale = this->getScale ( );
	
	CCSize		tSize = this->getContentSize ( );
	tSize.cx = tSize.cx * fScale * fScaleMod;
	tSize.cy = tSize.cy * fScale * fScaleMod;
	
	CCPoint		tPoint = this->getPosition ( );
	tPoint.x = tPoint.x - tSize.cx / 2;
	tPoint.y = tPoint.y - tSize.cy / 2;
	
	return CCRect ( tPoint.x, tPoint.y + 50, tSize.cx, tSize.cy / 2 ); 
}

KDvoid Mole::processTouch ( const CCPoint& tPoint )
{
	if ( m_nState == MOLE_UP )
	{
		this->gotHit ( );
	}
}

KDvoid Mole::processRelease ( KDvoid )
{

}

KDvoid Mole::setDownPosition ( const CCPoint& tPoint )
{
	m_tDownPosition = tPoint;
	this->setPosition ( tPoint );
}

KDvoid Mole::startPopUp ( KDvoid )
{
	this->setColor ( ccc3 ( 255, 255, 255 ) );

	m_nState = MOLE_MOVING_UP;
	
	this->stopAllActions ( );

	this->runAction
	(
		CCRepeatForever::create 
		(
			CCAnimate::create ( m_pNormalAnim ) 
		)	
	);

	this->runAction
	(
		CCSequence::create 
		(
			CCMoveTo::create ( 0.25f, ccp ( m_tDownPosition.x, m_tDownPosition.y + 20 ) ),
			CCCallFunc::create ( this, callfunc_selector ( Mole::finishPopUp ) ),
			KD_NULL
		)
	);
}

KDvoid Mole::finishPopUp ( KDvoid )
{
	m_nState = MOLE_UP;
}

KDvoid Mole::startHideDown ( KDvoid )
{
	m_nState = MOLE_MOVING_DOWN;

	this->runAction
	(
		CCSequence::create 
		(
			CCMoveTo::create ( 0.25f, ccp ( m_tDownPosition.x, m_tDownPosition.y ) ),
			CCCallFunc::create ( this, callfunc_selector ( Mole::finishHideDown ) ),
			KD_NULL
		)
	);
}

KDvoid Mole::finishHideDown ( KDvoid )
{
	m_nState = MOLE_DOWN;

	this->runAction
	(
		CCRepeatForever::create 
		(
			CCAnimate::create ( m_pNormalAnim ) 
		)	
	);

	this->setColor ( ccc3 ( 255, 255, 255 ) );
}

KDvoid Mole::gotHit ( KDvoid )
{
	m_pDelegate->processMoleHit ( );

	this->setColor ( ccc3 ( 255, 100, 100 ) );

	m_nState = MOLE_HIT;

	this->stopAllActions ( );

	this->runAction
	(
		CCRepeatForever::create 
		(
			CCAnimate::create ( m_pHitAnim ) 
		)	
	);

	this->runAction
	(
		CCSequence::create 
		(
			CCEaseInOut::create ( CCMoveTo::create ( 2, ccp ( m_tDownPosition.x, m_tDownPosition.y ) ), 2 ),
			CCCallFunc::create ( this, callfunc_selector ( Mole::finishHideDown ) ),
			KD_NULL
		)
	);
}

KDint Mole::getState ( KDvoid )
{
	return m_nState;
}

KDvoid Mole::setDelegate ( MoleDelgate* pDelegate )
{
	m_pDelegate = pDelegate;
}