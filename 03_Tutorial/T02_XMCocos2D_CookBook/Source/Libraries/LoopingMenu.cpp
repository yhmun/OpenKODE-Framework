/* --------------------------------------------------------------------------
 *
 *      File            LoopingMenu.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      João Caxaria on 5/29/09
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2009      Imaginary Factory. All rights reserved.
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
#include "LoopingMenu.h"
#include "InputController.h"

LoopingMenu* LoopingMenu::create ( KDvoid )
{
	LoopingMenu*	pRet = new LoopingMenu ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDvoid LoopingMenu::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	
//	m_nTouchPriority = KDINT_MIN + 1;

    pDirector->getTouchDispatcher ( )->addTargetedDelegate ( this, m_nTouchPriority, KD_FALSE );
}

KDbool LoopingMenu::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_bTouchDown = KD_TRUE;
	m_bMoving = KD_FALSE;
	
	m_pSelectedItem = this->itemForTouch ( pTouch ); 
	if ( m_pSelectedItem )
	{
		m_pSelectedItem->selected ( );
	}
	
	m_eState = kCCMenuStateTrackingTouch;

	return KD_TRUE;
}

KDvoid LoopingMenu::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCSet*		pTouches = CCSet::create ( );
	pTouches->addObject ( pTouch );

	CCPoint		tDistance = icDistance ( 1, pTouches, pEvent );

	if ( icWasSwipeLeft ( pTouches, pEvent ) && tDistance.y < tDistance.x )
	{
		m_bMoving = KD_TRUE;
		this->moveItemsLeftBy ( -tDistance.x );
	} 
	else if ( icWasSwipeRight ( pTouches, pEvent )  && tDistance.y < tDistance.x )
	{
		m_bMoving = KD_TRUE;
		this->moveItemsRightBy ( tDistance.x );
	}
	else if ( !m_bMoving && m_eState == kCCMenuStateTrackingTouch )
	{
		CCMenu::ccTouchMoved ( pTouch, pEvent );
	}
}

KDvoid LoopingMenu::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{	
	if ( !m_bMoving && m_eState == kCCMenuStateTrackingTouch )
	{
		CCMenu::ccTouchEnded ( pTouch, pEvent );
	}
	else if ( m_eState == kCCMenuStateTrackingTouch )
	{
		if ( m_pSelectedItem )
		{
			m_pSelectedItem->unselected ( );
		}

		m_eState = kCCMenuStateWaiting;
	}

	m_bMoving = KD_FALSE;
	m_bTouchDown = KD_FALSE;
}

KDvoid LoopingMenu::alignItemsHorizontallyWithPadding ( KDfloat fPadding )
{
	m_fAccelerometerVelocity = 0;
	
	m_fPaddingH = fPadding;
	m_fLowerBound = ( (CCMenuItem*) this->getChildren ( )->objectAtIndex ( 0 ) )->getContentSize ( ).cy / 2.f;

	CCMenu::alignItemsHorizontallyWithPadding ( fPadding );

	this->updateAnimation ( );
}

KDvoid LoopingMenu::updateAnimation ( KDvoid )
{
	static KDfloat	fQuadraticCoefficient = -1.0f / 90000.0f; //1/300^
	
	KDfloat			fHalfSize = this->getContentSize ( ).cx / 2;

	CCObject*		pObject;
	CCARRAY_FOREACH ( this->getChildren ( ), pObject )
	{
		CCMenuItem*		pItem = (CCMenuItem*) pObject;
		KDfloat			fDistance = kdFabsf ( pItem->getPosition ( ).x - fHalfSize + this->getPosition ( ).x );
		
		if ( fDistance > fHalfSize )
		{
			fDistance = fHalfSize;
		}
		else if ( fDistance < 0.0f )
		{
			fDistance = 0.0f;
		}
		
		KDfloat		fRatio = fQuadraticCoefficient * ( fDistance * fDistance ) + 1;
		
		pItem->setScale ( fRatio );
		pItem->setOpacity ( (KDubyte) ( fRatio * 255 ) );
		pItem->setPosition ( ccp ( pItem->getPosition ( ).x, m_fOffsetY - ( m_fLowerBound - pItem->getContentSize ( ).cy * fRatio / 2.0f ) ) );
	}
}

KDvoid LoopingMenu::moveItemsLeftBy  ( KDfloat fOffset )
{
	if ( m_pSelectedItem )
	{
		m_pSelectedItem->unselected ( );
	}
	
	CCObject*		pObject;
	CCArray*		pChildren = this->getChildren ( );
	CCARRAY_FOREACH ( pChildren, pObject )
	{
		CCMenuItem*		pItem = (CCMenuItem*) pObject;
		pItem->setPosition ( ccpAdd ( pItem->getPosition ( ), ccp ( fOffset, 0 ) ) );		
	}
	
	CCMenuItem*		pLeftItem = (CCMenuItem*) pChildren->objectAtIndex ( 0 );
	if ( pLeftItem->getPosition ( ).x + this->getPosition ( ).x + pLeftItem->getContentSize ( ).cx / 2.0f < 0)
	{
		pLeftItem->retain ( );
		pChildren->removeObjectAtIndex ( 0 );

		CCMenuItem*		pLastItem = (CCMenuItem*) pChildren->lastObject ( );
		pLeftItem->setPosition ( ccpAdd ( pLastItem->getPosition ( ), ccp ( pLastItem->getContentSize ( ).cx / 2.0f + pLeftItem->getContentSize ( ).cx / 2.0f + m_fPaddingH, 0 ) ) );
		pChildren->addObject ( pLeftItem );
		pLeftItem->autorelease ( );
	}

	this->updateAnimation ( );
}

KDvoid LoopingMenu::moveItemsRightBy ( KDfloat fOffset )
{
	if ( m_pSelectedItem )
	{
		m_pSelectedItem->unselected ( );
	}
	
	CCObject*		pObject;
	CCArray*		pChildren = this->getChildren ( );
	CCARRAY_FOREACH ( pChildren, pObject )
	{
		CCMenuItem*		pItem = (CCMenuItem*) pObject;
		pItem->setPosition ( ccpAdd ( pItem->getPosition ( ), ccp ( fOffset, 0 ) ) );		
	}

	CCMenuItem*		pLastItem = (CCMenuItem*) pChildren->lastObject ( );
	if ( pLastItem->getPosition ( ).x + this->getPosition ( ).x - pLastItem->getContentSize ( ).cx / 2.0 > this->getContentSize ( ).cx )
	{		
		pLastItem->retain ( );
		pChildren->removeLastObject ( );

		CCMenuItem*		pFirstItem = (CCMenuItem*) pChildren->objectAtIndex ( 0 );
		pLastItem->setPosition ( ccpSub ( pFirstItem->getPosition ( ), ccp ( pFirstItem->getContentSize ( ).cx / 2.0f + pLastItem->getContentSize ( ).cx / 2.0f + m_fPaddingH, 0 ) ) );

		pChildren->insertObject ( pLastItem, 0 );
		pLastItem->autorelease ( );
	}

	this->updateAnimation ( );
}