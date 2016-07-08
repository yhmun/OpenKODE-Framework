/* --------------------------------------------------------------------------
 *
 *      File            SlidingMenuGrid.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Brandon Reynolds on 1/9/11.
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
#include "SlidingMenuGrid.h"

SlidingMenuGrid* SlidingMenuGrid::create ( CCArray* pItems, KDint nCols, KDint nRows, const CCPoint& tPosition, const CCPoint& tPadding )
{
	SlidingMenuGrid*	pRet = new SlidingMenuGrid ( );

	if ( pRet && pRet->initWithArray ( pItems, nCols, nRows, tPosition, tPadding, KD_FALSE ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

SlidingMenuGrid* SlidingMenuGrid::create ( CCArray* pItems, KDint nCols, KDint nRows, const CCPoint& tPosition, const CCPoint& tPadding, KDbool bIsVertical )
{
	SlidingMenuGrid*	pRet = new SlidingMenuGrid ( );

	if ( pRet && pRet->initWithArray ( pItems, nCols, nRows, tPosition, tPadding, bIsVertical ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool SlidingMenuGrid::initWithArray ( CCArray* pItems, KDint nCols, KDint nRows, const CCPoint& tPosition, const CCPoint& tPadding, KDbool bIsVertical )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	this->setTouchEnabled ( KD_TRUE );
	
	KDint			nZOrder = 1;
	CCObject*		pObject;
	CCARRAY_FOREACH ( pItems, pObject )
	{
		CCNode*		pItem = (CCNode*) pObject;
		this->addChild ( pItem, nZOrder, nZOrder );
		nZOrder++;
	}

	m_tPadding			= tPadding;
	m_nCurrentPage		= 0;
	m_bMoving			= KD_FALSE;
	m_tMenuOrigin		= tPosition;
	m_fMoveDeadZone		= 10;
	m_bVerticalPaging	= bIsVertical;
	m_fAnimSpeed		= 1;
	m_eState			= kCCMenuStateWaiting;

	( bIsVertical ) ? this->buildGridVertical ( nCols, nRows ) : this->buildGrid ( nCols, nRows );

	this->setPosition ( tPosition );

	return KD_TRUE;
}

KDvoid SlidingMenuGrid::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	
//	m_nTouchPriority = KDINT_MIN + 1;

    pDirector->getTouchDispatcher ( )->addTargetedDelegate ( this, m_nTouchPriority, KD_FALSE );
}

KDbool SlidingMenuGrid::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	// Convert and store the location the touch began at.
	m_tTouchOrigin = pTouch->getLocation ( );
	
	// If we weren't in "waiting" state bail out.
	if ( m_eState != kCCMenuStateWaiting )
	{
		return KD_FALSE;
	}
	
	// Activate the menu item if we are touching one.
	if ( !m_bMoving )
	{
		m_pSelectedItem = this->getItemWithinTouch ( pTouch );
		if ( m_pSelectedItem )
		{
			m_pSelectedItem->selected ( );
		}
	}
		
	m_eState = kCCMenuStateTrackingTouch;

	return KD_TRUE;
}

KDvoid SlidingMenuGrid::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_pSelectedItem )
	{
		if ( this->getItemWithinTouch ( pTouch ) )
		{
			return;
		}
		else
		{
			// Touch Cancelled
			m_pSelectedItem->unselected ( );
			m_pSelectedItem = KD_NULL;
			m_eState = kCCMenuStateWaiting;	
		}
	}

	// Calculate the current touch point during the move.
	m_tTouchStop = pTouch->getLocation ( );

	// Distance between the origin of the touch and current touch point.
	m_fMoveDelta = ( m_bVerticalPaging ) ? ( m_tTouchStop.y - m_tTouchOrigin.y ) : ( m_tTouchStop.x - m_tTouchOrigin.x );

	// Set our position.
	this->setPosition ( this->getPositionOfCurrentPageWithOffset ( m_fMoveDelta ) );
	m_bMoving = KD_TRUE;
}

// Touch has ended. Process sliding of menu or press of menu item.
KDvoid SlidingMenuGrid::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	// User has been sliding the menu.
	if( m_bMoving )
	{
		m_bMoving = KD_FALSE;
		
		// Do we have multiple pages?
		if( m_nPageCount > 1 && ( m_fMoveDeadZone < kdFabsf ( m_fMoveDelta ) ) )
		{
			// Are we going forward or backward?
			KDbool		bForward = ( m_fMoveDelta < 0) ? KD_TRUE : KD_FALSE;
			
			// Do we have a page available?
			if ( bForward && ( m_nPageCount > m_nCurrentPage + 1 ) )
			{
				// Increment currently active page.
				m_nCurrentPage++;
			}
			else if ( !bForward && ( m_nCurrentPage > 0 ) )
			{
				// Decrement currently active page.
				m_nCurrentPage--;
			}
		}

		// Start sliding towards the current page.
		this->moveToCurrentPage ( );			
	}
	// User wasn't sliding menu and simply tapped the screen. Activate the menu item.
	else 
	{
		m_pSelectedItem->unselected ( );
		m_pSelectedItem->activate ( );
	}

	// Back to waiting state.
	m_eState = kCCMenuStateWaiting;
}

// Run the action necessary to slide the menu grid to the current page.
KDvoid SlidingMenuGrid::moveToCurrentPage ( KDvoid )
{	
	// Perform the action
	this->runAction
	(
		CCEaseBounce::create ( CCMoveTo::create ( m_fAnimSpeed * 0.3f, this->getPositionOfCurrentPage ( ) ) )
	);
}

KDvoid SlidingMenuGrid::buildGrid ( KDint nCols, KDint nRows )
{
	const CCSize&	tLyrSize  = this->getContentSize ( );
	const CCPoint&	tLyrPos   = this->getPosition ( );
	CCArray*		pChildren = this->getChildren ( );

	KDint			nCol = 0;
	KDint			nRow = 0;

	CCObject*		pObject;

	m_nPageCount = 0;
	CCARRAY_FOREACH ( pChildren, pObject )
	{
		CCMenuItem*		pItem = (CCMenuItem*) pObject;

		// Calculate the position of our menu item. 
		pItem->setPosition ( ccp ( tLyrPos.x + nCol * m_tPadding.x + ( m_nPageCount * tLyrSize.cx ), tLyrPos.y - nRow * m_tPadding.y ) );
		
		// Increment our positions for the next item(s).
		++nCol;
		if ( nCol == nCols )
		{
			nCol = 0;
			++nRow;
			
			if ( nRow == nRows )
			{
				m_nPageCount++;
				nCol = 0;
				nRow = 0;
			}
		}
	}
}

KDvoid SlidingMenuGrid::buildGridVertical ( KDint nCols, KDint nRows )
{
	const CCSize&	tLyrSize  = this->getContentSize ( );
	const CCPoint&	tLyrPos   = this->getPosition ( );
	CCArray*		pChildren = this->getChildren ( );

	KDint			nCol = 0;
	KDint			nRow = 0;

	CCObject*		pObject;

	m_nPageCount = 0;
	CCARRAY_FOREACH ( pChildren, pObject )
	{
		CCMenuItem*		pItem = (CCMenuItem*) pObject;

		// Calculate the position of our menu item. 
		pItem->setPosition ( ccp ( tLyrPos.x + nCol * m_tPadding.x, tLyrPos.y - nRow * m_tPadding.y + ( m_nPageCount * tLyrSize.cy ) ) );

		// Increment our positions for the next item(s).
		++nCol;
		if ( nCol == nCols )
		{
			nCol = 0;
			++nRow;
			
			if ( nRow == nRows )
			{
				m_nPageCount++;
				nCol = 0;
				nRow = 0;
			}
		}
	}
}

CCMenuItem* SlidingMenuGrid::getItemWithinTouch ( CCTouch* pTouch )
{	
	// Parse our menu items and see if our touch exists within one.
	CCArray*		pChildren = this->getChildren ( );
	CCObject*		pObject;
	CCARRAY_FOREACH ( pChildren, pObject )
	{
		CCMenuItem*		pItem  = (CCMenuItem*) pObject;
		CCPoint			tLocal = pItem->convertTouchToNodeSpace ( pTouch );
		CCRect			tRect  = pItem->rect ( );
		tRect.origin = ccpz;
		
		// If the touch was within this item. Return the item.
		if ( tRect.containsPoint ( tLocal ) )
		{
			return pItem;
		}
	}

	// Didn't touch an item.
	return KD_NULL;
}

CCPoint SlidingMenuGrid::getPositionOfCurrentPageWithOffset ( KDfloat fOffset )
{
	const CCSize&	tLyrSize  = this->getContentSize ( );

	return ( m_bVerticalPaging ) ?
		ccp ( m_tMenuOrigin.x, m_tMenuOrigin.y - ( m_nCurrentPage * tLyrSize.cy ) + fOffset ) :
		ccp ( ( m_tMenuOrigin.x - ( m_nCurrentPage * tLyrSize.cx ) + fOffset ), m_tMenuOrigin.y );
}

CCPoint SlidingMenuGrid::getPositionOfCurrentPage ( KDvoid )
{
	const CCSize&	tLyrSize  = this->getContentSize ( );
	
	return ( m_bVerticalPaging) ?
		ccp ( m_tMenuOrigin.x, m_tMenuOrigin.y - ( m_nCurrentPage * tLyrSize.cy ) ) :
		ccp ( ( m_tMenuOrigin.x - ( m_nCurrentPage * tLyrSize.cx ) ), m_tMenuOrigin.y );
}

// Returns the swiping dead zone. 
KDfloat SlidingMenuGrid::getSwipeDeadZone ( KDvoid )
{
	return m_fMoveDeadZone;
}

KDvoid SlidingMenuGrid::setSwipeDeadZone ( KDfloat fValue )
{
	m_fMoveDeadZone = fValue;
}

const CCPoint& SlidingMenuGrid::getPadding ( KDvoid )
{
	return m_tPadding;
}

KDvoid SlidingMenuGrid::setPadding ( const CCPoint& tPadding )
{
	m_tPadding = tPadding;
}

const CCPoint& SlidingMenuGrid::getMenuOrigin ( KDvoid )
{
	return m_tMenuOrigin;
}

KDvoid SlidingMenuGrid::setMenuOrigin ( const CCPoint& tMenuOrigin )
{
	m_tMenuOrigin = tMenuOrigin;
}

const CCPoint& SlidingMenuGrid::getTouchOrigin ( KDvoid )
{
	return m_tTouchOrigin;
}

KDvoid SlidingMenuGrid::setTouchOrigin ( const CCPoint& tTouchOrigin )
{
	m_tTouchOrigin = tTouchOrigin;
}

const CCPoint& SlidingMenuGrid::getTouchStop ( KDvoid )
{
	return m_tTouchStop;
}

KDvoid SlidingMenuGrid::setTouchStop ( const CCPoint& tTouchStop )
{
	m_tTouchStop = tTouchStop;
}

KDint SlidingMenuGrid::getPageCount ( KDvoid )
{
	return m_nPageCount;
}

KDvoid SlidingMenuGrid::setPageCount ( KDint nPageCount )
{
	m_nPageCount = nPageCount;
}

KDint SlidingMenuGrid::getCurrentPage ( KDvoid )
{
	return m_nCurrentPage;
}

KDvoid SlidingMenuGrid::setCurrentPage ( KDint nCurrentPage )
{
	m_nCurrentPage = nCurrentPage;
}	

KDbool SlidingMenuGrid::isMoving ( KDvoid )
{
	return m_bMoving;
}

KDvoid SlidingMenuGrid::setMoving ( KDbool bIsMoving )
{
	m_bMoving = bIsMoving;
}

// Returns wheather or not vertical paging is enabled.
KDbool SlidingMenuGrid::isVerticalPaging ( KDvoid )
{
	return m_bVerticalPaging;
}

// Sets the vertical paging flag.
KDvoid SlidingMenuGrid::setVerticalPaging ( KDbool bValue )
{
	m_bVerticalPaging = bValue;
//	this->buildGridVertical ( );
}

KDfloat SlidingMenuGrid::getMoveDelta ( KDvoid )
{
	return m_fMoveDelta;
}

KDvoid SlidingMenuGrid::setMoveDelta ( KDfloat fMoveDelta )
{
	m_fMoveDelta = fMoveDelta;
}	

KDfloat SlidingMenuGrid::getMoveDeadZone ( KDvoid )
{
	return m_fMoveDeadZone;
}

KDvoid SlidingMenuGrid::setMoveDeadZone ( KDfloat fMoveDeadZone )
{
	m_fMoveDeadZone = fMoveDeadZone;
}

KDfloat SlidingMenuGrid::getAnimSpeed ( KDvoid )
{
	return m_fAnimSpeed;
}

KDvoid SlidingMenuGrid::setAnimSpeed ( KDfloat fAnimSpeed )
{
	m_fAnimSpeed = fAnimSpeed;
}