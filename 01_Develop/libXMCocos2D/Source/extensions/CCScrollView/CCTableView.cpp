/* --------------------------------------------------------------------------
 *
 *      File            CCTableView.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Sangwoo Im
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
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
#include "extensions/CCScrollView/CCTableView.h"
#include "extensions/CCScrollView/CCTableViewCell.h"
#include "extensions/CCScrollView/CCSorting.h"
#include "menu_nodes/CCMenu.h"
#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCTableView* CCTableView::create ( CCTableViewDataSource* pDataSource, const CCSize& tSize )
{
    return CCTableView::create ( pDataSource, tSize, KD_NULL );
}

CCTableView* CCTableView::create ( CCTableViewDataSource* pDataSource, const CCSize& tSize, CCNode* pContainer )
{
    CCTableView*  pRet = new CCTableView ( );

	if ( pRet )
	{
		pRet->setDataSource ( pDataSource );

		if ( pRet->initWithViewSize ( tSize, pContainer ) )
		{
			pRet->autorelease ( );
		}
		else
		{
			CC_SAFE_DELETE ( pRet );
		}
	}

    return pRet;
}

KDbool CCTableView::initWithViewSize ( const CCSize& tSize, CCNode* pContainer )
{
    if ( CCScrollView::initWithViewSize ( tSize, pContainer ) )
    {
		CCScrollView::setDelegate ( this );

        m_pCellsUsed	= new CCArrayForObjectSorting ( );
        m_pCellsFreed	= new CCArrayForObjectSorting ( );
        m_pIndices		= new std::set<KDuint> ( );
        m_eVordering	= kCCTableViewFillTopDown;
	
		this->ignoreScroll ( KD_TRUE );
        this->setDirection ( kCCScrollViewDirectionVertical );
		this->_updateCellPositions ( );
		this->_updateContentSize ( );
		              
        return KD_TRUE;
    }

    return KD_FALSE;
}

KDvoid CCTableView::onEnter ( KDvoid )
{
	this->_updateContentSize ( );

	CCScrollView::onEnter ( );

    this->scrollViewDidScroll ( this );
}

CCTableView::CCTableView ( KDvoid )
{
	m_pTouchedCell		 = KD_NULL;
	m_pIndices			 = KD_NULL;
	m_pCellsUsed		 = KD_NULL;
	m_pCellsFreed		 = KD_NULL;
	m_pDataSource		 = KD_NULL;
	m_pTableViewDelegate = KD_NULL;
	m_uCellCount		 = 0;
	m_eOldDirection		 = kCCScrollViewDirectionNone;
}

CCTableView::~CCTableView ( KDvoid )
{
    CC_SAFE_DELETE  ( m_pIndices );
    CC_SAFE_RELEASE ( m_pCellsUsed );
    CC_SAFE_RELEASE ( m_pCellsFreed );
}

KDvoid CCTableView::setVerticalFillOrder ( CCTableViewVerticalFillOrder eFillOrder )
{
    if ( m_eVordering != eFillOrder )
	{
        m_eVordering = eFillOrder;
        if ( m_pCellsUsed->count ( ) > 0 )
		{
            this->reloadData ( );
        }
    }
}

CCTableViewVerticalFillOrder CCTableView::getVerticalFillOrder ( KDvoid )
{
    return m_eVordering;
}

KDvoid CCTableView::reloadData ( KDvoid )
{
	if ( !m_bDidEnter )
	{
		this->_updateContentSize ( );
		return;
	}

	m_eOldDirection = kCCScrollViewDirectionNone;

    CCObject*  pObj = KD_NULL;
    CCARRAY_FOREACH ( m_pCellsUsed, pObj )
    {
        CCTableViewCell*  pCell = (CCTableViewCell*) pObj;

        if ( m_pTableViewDelegate != KD_NULL )
		{
            m_pTableViewDelegate->tableCellWillRecycle ( this, pCell );
        }

        m_pCellsFreed->addObject ( pCell );
        pCell->reset ( );
        if ( pCell->getParent ( ) == this->getContainer ( ) )
        {
            this->getContainer ( )->removeChild ( pCell, KD_TRUE );
        }
    }

    m_pIndices->clear ( );
    m_pCellsUsed->release ( );
    m_pCellsUsed = new CCArrayForObjectSorting ( );
    
	this->_updateCellPositions ( );
	this->_updateContentSize ( );
	if ( m_pDataSource->numberOfCellsInTableView ( this ) > 0 )
	{
		this->scrollViewDidScroll ( this );
	}
}

CCTableViewCell* CCTableView::cellAtIndex ( KDuint uIndex )
{
    CCTableViewCell*  pFound = NULL;
    
    if ( m_pIndices->find ( uIndex ) != m_pIndices->end ( ) )
    {
        pFound = (CCTableViewCell*) m_pCellsUsed->objectWithObjectID ( uIndex );
    }
    
    return pFound;
}

KDvoid CCTableView::updateCellAtIndex ( KDuint uIndex )
{
    if ( uIndex == CC_INVALID_INDEX )
    {
        return;
    }

    KDuint  uCountOfItems = m_uCellCount;
    if ( 0 == uCountOfItems || uIndex > uCountOfItems - 1 )
    {
        return;
    }

    CCTableViewCell*  pCell = this->cellAtIndex ( uIndex );
    if ( pCell )
    {
        this->_moveCellOutOfSight ( pCell );
    } 

    pCell = m_pDataSource->tableCellAtIndex ( this, uIndex );
    this->_setIndexForCell ( uIndex, pCell );
    this->_addCellIfNecessary ( pCell );
}

KDvoid CCTableView::insertCellAtIndex ( KDuint uIndex )
{
    if ( uIndex == CC_INVALID_INDEX )
    {
        return;
    }

    KDuint  uCountOfItems = m_uCellCount;
    if ( 0 == uCountOfItems || uIndex > uCountOfItems - 1 )
    {
        return;
    }

    CCTableViewCell*  pCell = KD_NULL;
    KDint             nNewIdx = 0;
    
    pCell = (CCTableViewCell*) m_pCellsUsed->objectWithObjectID ( uIndex );
    if ( pCell ) 
    {
        nNewIdx = m_pCellsUsed->indexOfSortedObject ( pCell );
        for ( KDuint i = nNewIdx; i < m_pCellsUsed->count ( ); i++ )
        {
            pCell = (CCTableViewCell*) m_pCellsUsed->objectAtIndex ( i );
            this->_setIndexForCell ( pCell->getIdx ( ) + 1, pCell );
        }
    }
    
    // insert a new cell
    pCell = m_pDataSource->tableCellAtIndex ( this, uIndex );
    this->_setIndexForCell ( uIndex, pCell );
    this->_addCellIfNecessary ( pCell );    

	this->_updateCellPositions ( );
    this->_updateContentSize ( );
}

KDvoid CCTableView::removeCellAtIndex ( KDuint uIndex )
{
    if ( uIndex == CC_INVALID_INDEX )
    {
        return;
    }
    
    KDuint  uCountOfItems = m_uCellCount;
    if ( 0 == uCountOfItems || uIndex > uCountOfItems - 1 )
    {
        return;
    }

    CCTableViewCell*  pCell = this->cellAtIndex ( uIndex );
    if ( !pCell ) 
	{
        return;
    }
    
    KDuint  uNewIdx = m_pCellsUsed->indexOfSortedObject ( pCell );
    
    // remove first
    this->_moveCellOutOfSight ( pCell );
    
    m_pIndices->erase ( uIndex );
	this->_updateCellPositions ( );

    for ( KDuint i = m_pCellsUsed->count ( ) - 1; i > uNewIdx; i-- )
	{
        pCell = (CCTableViewCell*) m_pCellsUsed->objectAtIndex ( i );
        this->_setIndexForCell ( pCell->getIdx ( ) - 1, pCell );
    }
}

CCTableViewCell* CCTableView::dequeueCell ( KDvoid )
{
    CCTableViewCell*  pCell;
    
    if ( m_pCellsFreed->count ( ) == 0 )
	{
        pCell = KD_NULL;
    }
	else 
	{
        pCell = (CCTableViewCell*) m_pCellsFreed->objectAtIndex ( 0 );
        pCell->retain ( );
        m_pCellsFreed->removeObjectAtIndex ( 0 );
        pCell->autorelease ( );
    }

    return pCell;
}

KDvoid CCTableView::_addCellIfNecessary ( CCTableViewCell* pCell )
{
    if ( pCell->getParent ( ) != this->getContainer ( ) )
    {
        this->getContainer ( )->addChild ( pCell );
    }

    m_pCellsUsed->insertSortedObject ( pCell );
    m_pIndices->insert ( pCell->getIdx ( ) );
}

KDvoid CCTableView::_updateContentSize ( KDvoid )
{
	if ( m_bDidEnter )
	{
		m_uCellCount = m_pDataSource ? m_pDataSource->numberOfCellsInTableView ( this ) : 0;
	}

    CCSize   tSize;

	if ( m_uCellCount > 0 )
	{
		KDfloat  fMaxPosition = m_aCellsPositions [ m_uCellCount ];

        switch ( this->getDirection ( ) )
        {
            case kCCScrollViewDirectionHorizontal:
                tSize = CCSizeMake ( fMaxPosition, m_tViewSize.cy );
                break;

            default:
                tSize = CCSizeMake ( m_tViewSize.cx, fMaxPosition );
                break;
        }
	}

    this->setContentSize ( tSize );

	if ( m_eOldDirection != m_eDirection )
	{
		if ( m_eDirection == kCCScrollViewDirectionHorizontal )
		{
			this->setContentOffset ( ccp ( 0, 0 ) );
		}
		else
		{
			this->setContentOffset ( ccp ( 0, this->minContainerOffset ( ).y ) );
		}
		m_eOldDirection = m_eDirection;
	}
}

CCPoint CCTableView::_offsetFromIndex ( KDuint uIndex )
{
    CCPoint  tOffset = this->__offsetFromIndex ( uIndex );

	const CCSize  tCellSize = m_pDataSource->tableCellSizeForIndex ( this, uIndex );
    if ( this->getDirection ( ) != kCCScrollViewDirectionHorizontal && m_eVordering == kCCTableViewFillTopDown )
	{
        tOffset.y = this->getContainer ( )->getContentSize ( ).cy - tOffset.y - tCellSize.cy;
    }

    return tOffset;
}

CCPoint CCTableView::__offsetFromIndex ( KDuint uIndex )
{
    if ( this->getDirection ( ) == kCCScrollViewDirectionHorizontal )
	{
		return ccp ( m_aCellsPositions [ uIndex ], 0.0f );
	}
	else
	{
		return ccp ( 0.0f, m_aCellsPositions [ uIndex ] );
    }
}

KDuint CCTableView::_indexFromOffset ( CCPoint tOffset )
{
    KDint         nIndex = 0;
	const KDint   nMaxIdx = m_uCellCount > 0 ? m_uCellCount - 1 : 0;

    if ( m_eVordering == kCCTableViewFillTopDown )
	{
        tOffset.y = this->getContainer ( )->getContentSize ( ).cy - tOffset.y;
    }

    nIndex = this->__indexFromOffset ( tOffset );
	if ( nIndex != -1 )
	{
		nIndex = KD_MAX ( 0, nIndex );
        if ( nIndex > nMaxIdx )
        {
            nIndex = CC_INVALID_INDEX;
        }
	}

    return nIndex;
}

KDint CCTableView::__indexFromOffset ( CCPoint tOffset )
{
	KDint    nLow  = 0;
	KDint    nHigh = m_uCellCount - 1;
	KDfloat  fSearch;

    if ( this->getDirection ( ) == kCCScrollViewDirectionHorizontal )
	{
		fSearch = tOffset.x;
	}
	else
	{
		fSearch = tOffset.y;
    }

    while ( nHigh >= nLow )
    {
        KDint    nIndex = nLow + ( nHigh - nLow ) / 2;
        KDfloat  fCellStart = m_aCellsPositions [ nIndex ];
        KDfloat  fCellEnd   = m_aCellsPositions [ nIndex + 1 ];

        if ( fSearch >= fCellStart && fSearch <= fCellEnd )
        {
            return nIndex;
        }
        else if ( fSearch < fCellStart )
        {
            nHigh = nIndex - 1;
        }
        else
        {
            nLow = nIndex + 1;
        }
    }

    if ( nLow <= 0 ) 
	{
        return 0;
    }

    return -1;
}

KDvoid CCTableView::_moveCellOutOfSight ( CCTableViewCell* pCell )
{
    if ( m_pTableViewDelegate != KD_NULL )
	{
        m_pTableViewDelegate->tableCellWillRecycle ( this, pCell );
    }

    m_pCellsFreed->addObject ( pCell );
    m_pCellsUsed->removeSortedObject ( pCell );
    m_pIndices->erase ( pCell->getIdx ( ) );

    pCell->reset ( );
    if ( pCell->getParent ( ) == this->getContainer ( ) ) 
	{
        this->getContainer ( )->removeChild ( pCell, KD_TRUE );
    }
}

KDvoid CCTableView::_setIndexForCell ( KDuint uIndex, CCTableViewCell* pCell )
{
    pCell->setAnchorPoint ( ccp ( 0.0f, 0.0f ) );
    pCell->setPosition ( this->_offsetFromIndex ( uIndex ) );
    pCell->setIdx ( uIndex );
}

KDvoid CCTableView::_updateCellPositions ( KDvoid )
{
    KDint  nCellsCount = m_uCellCount = m_pDataSource ? m_pDataSource->numberOfCellsInTableView ( this ) : 0;
    m_aCellsPositions.resize ( nCellsCount + 1, 0.0 );

    if ( nCellsCount > 0 )
    {
        KDfloat  fCurrentPos = 0;
        CCSize   tCellSize;
        for ( KDint i = 0; i < nCellsCount; i++ )
        {
            m_aCellsPositions [ i ] = fCurrentPos;
            tCellSize = m_pDataSource->tableCellSizeForIndex ( this, i );
            switch ( this->getDirection ( ) )
            {
                case kCCScrollViewDirectionHorizontal:
                    fCurrentPos += tCellSize.cx;
                    break;

                default :
                    fCurrentPos += tCellSize.cy;
                    break;
            }
        }

        m_aCellsPositions [ nCellsCount ] = fCurrentPos;		// 1 extra value allows us to get right/bottom of the last cell
    }

}

KDvoid CCTableView::scrollViewDidScroll ( CCScrollView* pView )
{
    KDuint  uCountOfItems = m_uCellCount;
    if ( !m_bDidEnter || 0 == uCountOfItems )
    {
        return;
    }

    if ( m_pTableViewDelegate != KD_NULL )
	{
        m_pTableViewDelegate->scrollViewDidScroll ( this );
    }

    KDuint   uStartIdx = 0, uEndIdx = 0, uIdx = 0, uMaxIdx = 0;
    CCPoint  tOffset = ccpMult ( this->getContentOffset ( ), -1 );
    uMaxIdx = KD_MAX ( uCountOfItems - 1, 0 );
    
    if ( m_eVordering == kCCTableViewFillTopDown )
    {
        tOffset.y = tOffset.y + m_tViewSize.cy / this->getContainer ( )->getScaleY ( );
    }
    uStartIdx = this->_indexFromOffset ( tOffset );
	if ( uStartIdx == CC_INVALID_INDEX )
	{
		uStartIdx = uCountOfItems - 1;
	}

    if ( m_eVordering == kCCTableViewFillTopDown )
    {
        tOffset.y -= m_tViewSize.cy / this->getContainer ( )->getScaleY ( );
    }
    else 
    {
        tOffset.y += m_tViewSize.cy / this->getContainer ( )->getScaleY ( );
    }
    tOffset.x += m_tViewSize.cx / this->getContainer ( )->getScaleX ( );
    
    uEndIdx = this->_indexFromOffset ( tOffset );   
    if ( uEndIdx == CC_INVALID_INDEX )
	{
		uEndIdx = uCountOfItems - 1;
	}

    if ( m_pCellsUsed->count ( ) > 0 )
    {
        CCTableViewCell*  pCell = (CCTableViewCell*) m_pCellsUsed->objectAtIndex ( 0 );

        uIdx = pCell->getIdx ( );
        while ( uIdx < uStartIdx )
        {
            this->_moveCellOutOfSight ( pCell );
            if ( m_pCellsUsed->count ( ) > 0 )
            {
                pCell = (CCTableViewCell*) m_pCellsUsed->objectAtIndex ( 0 );
                uIdx = pCell->getIdx ( );    
            }
            else
            {
                break;
            }
        }
    }
    if ( m_pCellsUsed->count ( ) > 0 )
    {
        CCTableViewCell*  pCell = (CCTableViewCell*) m_pCellsUsed->lastObject ( );
        uIdx = pCell->getIdx ( );

        while ( uIdx <= uMaxIdx && uIdx > uEndIdx )
        {
            this->_moveCellOutOfSight ( pCell );
            if ( m_pCellsUsed->count ( ) > 0 )
            {
                pCell = (CCTableViewCell*) m_pCellsUsed->lastObject ( );
                uIdx = pCell->getIdx ( );
                
            }
            else
            {
                break;
            }
        }
    }
    
    for ( KDuint i = uStartIdx; i <= uEndIdx; i++ )
    {
        if ( m_pIndices->find ( i ) != m_pIndices->end ( ) )
        {
            continue;
        }
        this->updateCellAtIndex ( i );
    }
}

KDvoid CCTableView::scrollViewDidZoom ( CCScrollView* pView )
{

}

KDvoid CCTableView::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !this->isVisible ( ) )
	{
        return;
    }

	if ( m_pTouchedCell )
	{
		CCRect  bb = this->boundingBox ( );
		bb.origin = m_pParent->convertToWorldSpace ( bb.origin );

		if ( bb.containsPoint ( pTouch->getLocation ( ) ) && m_pTableViewDelegate != NULL )
        {
            m_pTableViewDelegate->tableCellUnhighlight ( this, m_pTouchedCell );
            m_pTableViewDelegate->tableCellTouched ( this, m_pTouchedCell );
        }
        
        m_pTouchedCell = KD_NULL;
	}

    CCScrollView::ccTouchEnded ( pTouch, pEvent );
}

KDbool CCTableView::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !this->isVisible ( ) )
	{
        return KD_FALSE;
    }

	KDbool  bTouchResult = CCScrollView::ccTouchBegan ( pTouch, pEvent );
	
	if ( m_pTouches->count ( ) == 1 )
	{
		KDuint		uIndex;
        CCPoint		tPoint;
        
        tPoint = this->getContainer ( )->convertTouchToNodeSpace ( pTouch );
        
        uIndex = this->_indexFromOffset ( tPoint );
		if ( uIndex == CC_INVALID_INDEX )
		{
			m_pTouchedCell = KD_NULL;
		}
        else
		{
			m_pTouchedCell = this->cellAtIndex ( uIndex );
		}

        if ( m_pTouchedCell && m_pTableViewDelegate != KD_NULL )
		{
            m_pTableViewDelegate->tableCellHighlight ( this, m_pTouchedCell );
        }
	}
    else if ( m_pTouchedCell ) 
	{
        if ( m_pTableViewDelegate != KD_NULL )
		{
            m_pTableViewDelegate->tableCellUnhighlight ( this, m_pTouchedCell );
        }
        
        m_pTouchedCell = KD_NULL;
    }

	return bTouchResult;
}

KDvoid CCTableView::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !this->isVisible ( ) )
	{
        return;
    }

	CCScrollView::ccTouchMoved ( pTouch, pEvent );

	CCTableViewCell*  pTouchedCell = m_pTouchedCell;
	if ( m_pTouches->containsObject ( pTouch ) && m_pTouches->count ( ) == 1 )
	{
		if ( !m_bDragging )
		{
			CCTableViewCell*  pCell = getHitCell ( pTouch );
			
			if ( pCell )
			{
				if ( pTouchedCell != pCell )
				{
					m_pTouchedCell = pCell;
					if ( pTouchedCell )
					{					
						m_pTableViewDelegate->tableCellUnhighlight ( this, pTouchedCell );					
					}
					m_pTableViewDelegate->tableCellHighlight ( this, pCell );					
				}												
			}
			else if ( pTouchedCell )
			{				
				m_pTouchedCell = KD_NULL;
				m_pTableViewDelegate->tableCellUnhighlight ( this, pTouchedCell );				
			}
		}
		else if ( this->isTouchMoved ( ) )
		{
			if ( pTouchedCell )
			{
				m_pTableViewDelegate->tableCellUnhighlight ( this, m_pTouchedCell );
				m_pTouchedCell = KD_NULL;	
			}
		}
	}
	else
	{
		if ( pTouchedCell )
		{
			m_pTouchedCell = KD_NULL;	
			m_pTableViewDelegate->tableCellUnhighlight ( this, pTouchedCell );			
		}
	}
}

CCTableViewCell* CCTableView::getHitCell ( CCTouch* pTouch )
{
	CCPoint  tPoint = this->getContainer ( )->convertTouchToNodeSpace ( pTouch );
	KDint	 nIndex = -1;

	CCTableViewCell*  pCell = KD_NULL;
	
	const CCSize&  tCellSize = m_pDataSource->cellSizeForTable ( this );

    if ( m_eVordering == kCCTableViewFillTopDown )
	{
        tPoint.y -= tCellSize.cy;
		tPoint.y  = this->getContainer ( )->getContentSize ( ).cy - tPoint.y - tCellSize.cy;
    }

    nIndex = this->__indexFromOffset ( tPoint );
	if ( 0 <= nIndex && nIndex < (KDint) m_uCellCount )
	{
		pCell = this->cellAtIndex ( nIndex );
	}

	return pCell;
}

NS_CC_END
