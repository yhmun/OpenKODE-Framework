/* --------------------------------------------------------------------------
 *
 *      File            ColorTouchSprite.cpp
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
#include "ColorTouchSprite.h"

ColorTouchSprite* ColorTouchSprite::create ( const KDchar* szFilePath )
{
	ColorTouchSprite*	pRet = new ColorTouchSprite ( );

	if ( pRet && pRet->init ( ) && pRet->initWithFile ( szFilePath ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool ColorTouchSprite::init ( KDvoid )
{
	if ( !CCSprite::init ( ) )
	{
		KD_FALSE;
	}

	m_nHoldTime			= 0;
	m_nTouchedState		= TS_NONE;
	m_bIsTouched		= KD_FALSE;
	m_nLastMoved		= 0;
	m_tLastTouchedPoint	= ccpz;
	
	this->schedule ( schedule_selector ( ColorTouchSprite::step ) );

	return KD_TRUE;
}

KDvoid ColorTouchSprite::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
	
	m_bIsTouched		= KD_TRUE;
	m_nHoldTime			= 0;
	m_nTouchedState		= TS_NONE;	
	m_tLastTouchedPoint = tPoint;
}

KDvoid ColorTouchSprite::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( !m_bIsTouched )
	{
		return;
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );
		
	// We have a drag threshold of 3 pixels.
	if ( m_nTouchedState == TS_DRAG || distanceBetweenPoints ( m_tLastTouchedPoint, tPoint ) > 3 )
	{
		m_nTouchedState = TS_DRAG;
		this->setPosition ( tPoint );
		m_nLastMoved = 0;
	}
	m_tLastTouchedPoint = tPoint;
}

KDvoid ColorTouchSprite::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( !m_bIsTouched )
	{
		return;
	}

	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	// A short hold time after a touch ended means a tap.
	if ( m_nHoldTime < 10 )
	{
		m_nTouchedState = TS_TAP;
	}

	m_nHoldTime			= 0;
	m_bIsTouched		= KD_FALSE;	
	m_tLastTouchedPoint = tPoint;
}

KDvoid ColorTouchSprite::step ( KDfloat fDelta )
{
	// We use holdTime to determine the difference between a tap and a hold
	if ( m_bIsTouched )
	{
		m_nHoldTime  += 1;
		m_nLastMoved += 1;
	}
	else
	{
		m_nHoldTime += 1;
		if ( m_nHoldTime > 60 )
		{
			m_nTouchedState = TS_NONE;
		}
	}
	
	// If you are holding and you haven't moved in a while change the state
	if ( m_nHoldTime > 10 && m_bIsTouched && m_nLastMoved > 30 )
	{
		m_nTouchedState = TS_HOLD;
	}
}

CCRect ColorTouchSprite::rect ( KDvoid )
{
	KDfloat		fScaleMod = 1.0f;
	KDfloat		fScale = this->getScale ( );
	
	CCSize		tSize = this->getContentSize ( );
	tSize.cx = tSize.cx * fScale * fScaleMod;
	tSize.cy = tSize.cy * fScale * fScaleMod;
	
	CCPoint		tPoint = this->getPosition ( );
	tPoint.x = tPoint.x - tSize.cx / 2;
	tPoint.y = tPoint.y - tSize.cy / 2;
	
	return CCRect ( tPoint.x, tPoint.y, tSize.cx, tSize.cy ); 
}

KDint ColorTouchSprite::getTouchedState ( KDvoid )
{
	return m_nTouchedState;
}

KDvoid ColorTouchSprite::setTouchedState ( KDint nState )
{
	m_nTouchedState = nState;
}
