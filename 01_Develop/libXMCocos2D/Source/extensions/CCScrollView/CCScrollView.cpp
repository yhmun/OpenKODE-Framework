/* --------------------------------------------------------------------------
 *
 *      File            CCScrollView.cpp
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "extensions/CCScrollView/CCScrollView.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionTween.h"
#include "actions/CCActionInstant.h"
#include "effects/CCGrid.h"
#include "touch_dispatcher/CCTouch.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"
#include "support/CCEGLView.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

CCScrollView::CCScrollView ( KDvoid )
{
	m_fZoomScale		= 0.0f;
	m_fMinZoomScale		= 0.0f;
	m_fMaxZoomScale		= 0.0f;
	m_pDelegate			= KD_NULL;
	m_bDragging			= KD_FALSE;
	m_bBounceable		= KD_FALSE;
	m_eDirection		= kCCScrollViewDirectionBoth;
	m_bClippingToBounds	= KD_FALSE;
	m_pContainer		= KD_NULL;
	m_bTouchMoved		= KD_FALSE;
	m_fTouchLength		= 0.0f;
	m_pTouches			= KD_NULL;
	m_fMinScale			= 0.0f;
	m_fMaxScale			= 0.0f;

	m_fDeAccelRate		= 0.95f;
	m_fDeAccelDist	    = 1.00f;
	m_fBounceDuration	= 0.15f;
	m_fInsetRatio		= 0.20f;
	m_fDetectTouchMoved = 10.0f;
	m_bIgnoreScroll		= KD_FALSE;
}

CCScrollView::~CCScrollView ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTouches );
}

CCScrollView* CCScrollView::create ( const CCSize& tSize, CCNode* pContainer )
{
    CCScrollView*  pRet = new CCScrollView ( );

    if ( pRet && pRet->initWithViewSize ( tSize, pContainer ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

CCScrollView* CCScrollView::create ( KDvoid )
{
    CCScrollView*  pRet = new CCScrollView ( );

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


KDbool CCScrollView::initWithViewSize ( const CCSize& tSize, CCNode* pContainer )
{
    if ( CCLayer::init ( ) )
    {
        m_pContainer = pContainer;
        
        if ( !m_pContainer )
        {
            m_pContainer = CCLayer::create ( );
			m_pContainer->ignoreAnchorPointForPosition ( KD_FALSE );
            m_pContainer->setAnchorPoint ( CCPointZero );			
        }		        

        m_pTouches			= new CCArray ( );
        m_pDelegate			= KD_NULL;
        m_bBounceable		= KD_TRUE;
        m_bClippingToBounds = KD_TRUE;
        m_eDirection		= kCCScrollViewDirectionBoth;
        m_fTouchLength		= 0.0f;
        m_fMinScale			= m_fMaxScale = 1.0f;
		m_tViewSize			= tSize;
		 
		this->setTouchMode    ( kCCTouchesOneByOne );
		this->setTouchEnabled ( KD_TRUE );
		this->setContainer    ( m_pContainer );		

        return KD_TRUE;
    }

    return KD_FALSE;
}

KDbool CCScrollView::init ( KDvoid  )
{
	return this->initWithViewSize ( CCDirector::sharedDirector ( )->getWinSize ( ), KD_NULL );
}

KDbool CCScrollView::isNodeVisible ( CCNode* pNode )
{
    const CCPoint  tOffset = this->getContentOffset ( );
    const CCSize   tSize   = this->getViewSize ( );
    const KDfloat  fScale  = this->getZoomScale ( );
    
    CCRect  tViewRect;
    
    tViewRect = CCRectMake ( -tOffset.x / fScale, -tOffset.y / fScale, tSize.cx / fScale, tSize.cy / fScale ); 
    
    return tViewRect.intersectsRect ( pNode->boundingBox ( ) );
}

KDvoid CCScrollView::pause ( CCObject* pSender )
{
    m_pContainer->pauseSchedulerAndActions ( );

    CCObject*   pObject = KD_NULL;
    CCArray*  pChildren = m_pContainer->getChildren ( );
    CCARRAY_FOREACH ( pChildren, pObject )
    {
        CCNode*  pChild = (CCNode*) pObject;
        pChild->pauseSchedulerAndActions ( );
    }
}

KDvoid CCScrollView::resume ( CCObject* pSender )
{
    CCObject*   pObject = KD_NULL;
    CCArray*  pChildren = m_pContainer->getChildren ( );
    CCARRAY_FOREACH ( pChildren, pObject )
    {
        CCNode*  pChild = (CCNode*) pObject;
        pChild->resumeSchedulerAndActions ( );
    }

    m_pContainer->resumeSchedulerAndActions ( );
}

KDvoid CCScrollView::setTouchEnabled ( KDbool bEnable )
{
    CCLayer::setTouchEnabled ( bEnable );

    if ( !bEnable )
    {
        m_bDragging   = KD_FALSE;
        m_bTouchMoved = KD_FALSE;
        m_pTouches->removeAllObjects ( );
    }
}

KDvoid CCScrollView::setContentOffset ( CCPoint tOffset, KDbool bAnimated )
{
    if ( bAnimated )
    { 
		// animate scrolling
        this->setContentOffsetInDuration ( tOffset, 0.15f );
    } 
    else
    { 
		// set the container position directly
        if ( !m_bBounceable )
        {
            const CCPoint  tMinOffset = this->minContainerOffset ( );
            const CCPoint  tMaxOffset = this->maxContainerOffset ( );
            
            tOffset.x = KD_MAX ( tMinOffset.x, KD_MIN ( tMaxOffset.x, tOffset.x ) );
            tOffset.y = KD_MAX ( tMinOffset.y, KD_MIN ( tMaxOffset.y, tOffset.y ) );
        }

        m_pContainer->setPosition ( tOffset );

        if ( m_pDelegate )
        {
            m_pDelegate->scrollViewDidScroll ( this );   
        }
    }
}

KDvoid CCScrollView::setContentOffsetInDuration ( CCPoint tOffset, KDfloat fDelta )
{
    m_pContainer->runAction ( CCSequence::create 
	(
		CCMoveTo::create ( fDelta, tOffset ),
		CCCallFuncN::create ( this, callfuncN_selector ( CCScrollView::stoppedAnimatedScroll ) ),
		KD_NULL 
	) );
    this->schedule ( schedule_selector ( CCScrollView::performedAnimatedScroll ) );
}

CCPoint CCScrollView::getContentOffset ( KDvoid )
{
    return m_pContainer->getPosition ( );
}

KDvoid CCScrollView::setZoomScale ( KDfloat fScale )
{
    if ( m_pContainer->getScale ( ) != fScale )
    {
        CCPoint  tOldCenter, tNewCenter, tCenter;
        
        if ( m_fTouchLength == 0.0f ) 
        {
            tCenter = ccp ( m_tViewSize.cx * 0.5f, m_tViewSize.cy * 0.5f );
            tCenter = this->convertToWorldSpace ( tCenter );
        }
        else
        {
            tCenter = m_tTouchPoint;
        }
        
        tOldCenter = m_pContainer->convertToNodeSpace ( tCenter );
        m_pContainer->setScale ( KD_MAX ( m_fMinScale, KD_MIN ( m_fMaxScale, fScale ) ) );
        tNewCenter = m_pContainer->convertToWorldSpace ( tOldCenter );
        
        const CCPoint  tOffset = ccpSub ( tCenter, tNewCenter );
        if ( m_pDelegate != KD_NULL )
        {
            m_pDelegate->scrollViewDidZoom ( this );
        }
        this->setContentOffset ( ccpAdd ( m_pContainer->getPosition ( ), tOffset ) );
    }
}

KDfloat CCScrollView::getZoomScale ( KDvoid )
{
    return m_pContainer->getScale ( );
}

KDvoid CCScrollView::setZoomScale ( KDfloat fScale, KDbool bAnimated )
{
    if ( bAnimated )
    {
        this->setZoomScaleInDuration ( fScale, 0.15f );
    }
    else
    {
        this->setZoomScale ( fScale );
    }
}

KDvoid CCScrollView::setZoomScaleInDuration ( KDfloat fScale, KDfloat fDelta )
{
    if ( fDelta > 0 )
    {
        if ( m_pContainer->getScale ( ) != fScale )
        {
            this->runAction ( CCActionTween::create ( fDelta, "zoomScale", m_pContainer->getScale ( ), fScale ) );
        }
    }
    else
    {
        this->setZoomScale ( fScale );
    }
}

KDvoid CCScrollView::setViewSize ( const CCSize& tSize )
{
    m_tViewSize = tSize;

    CCLayer::setContentSize ( tSize );
}

CCNode* CCScrollView::getContainer ( KDvoid )
{
    return this->m_pContainer;
}

KDvoid CCScrollView::setContainer ( CCNode* pContainer )
{
    // Make sure that 'm_pContainer' has a non-NULL value since there are
    // lots of logic that use 'm_pContainer'.
    if ( KD_NULL == pContainer )
	{
        return;
	}

    this->removeAllChildrenWithCleanup ( KD_TRUE );
    this->m_pContainer = pContainer;

	const CCSize&  tSize = pContainer->getContentSize ( );
    pContainer->ignoreAnchorPointForPosition ( KD_FALSE );
    pContainer->setAnchorPoint ( ccp ( 0.0f, 0.0f ) );
	pContainer->setPosition ( ccp ( 0, this->minContainerOffset ( ).y ) );      
		
	this->setContentSize ( tSize );
    this->addChild ( m_pContainer );
	this->setViewSize ( m_tViewSize );
}

KDvoid CCScrollView::relocateContainer ( KDbool bAnimated )
{
    CCPoint  tOldPoint, tMin, tMax;
    KDfloat  fNewX, fNewY;
    
    tMin = this->minContainerOffset ( );
    tMax = this->maxContainerOffset ( );
    
    tOldPoint = m_pContainer->getPosition ( );

    fNewX = tOldPoint.x;
    fNewY = tOldPoint.y;

    if ( m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionHorizontal )
    {
		fNewX = KD_MAX ( fNewX, tMin.x );
        fNewX = KD_MIN ( fNewX, tMax.x );
    }

    if ( m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionVertical )
    {
        fNewY = KD_MIN ( fNewY, tMax.y );
        fNewY = KD_MAX ( fNewY, tMin.y );
    }

    if ( fNewY != tOldPoint.y || fNewX != tOldPoint.x )
    {
        this->setContentOffset ( ccp ( fNewX, fNewY ), bAnimated );
    }
}

CCPoint CCScrollView::maxContainerOffset ( KDvoid )
{
    return ccp ( 0.0f, 0.0f );
}

CCPoint CCScrollView::minContainerOffset ( KDvoid )
{
    return ccp ( m_tViewSize.cx - m_pContainer->getContentSize ( ).cx * m_pContainer->getScaleX ( ), 
                 m_tViewSize.cy - m_pContainer->getContentSize ( ).cy * m_pContainer->getScaleY ( ) );
}

KDvoid CCScrollView::deaccelerateScrolling ( KDfloat fDelta )
{
    if ( m_bDragging )
    {
        this->unschedule ( schedule_selector ( CCScrollView::deaccelerateScrolling ) );
        return;
    }
    
    KDfloat fNewX, fNewY;
    CCPoint tMaxInset, tMinInset;
    
    m_pContainer->setPosition ( ccpAdd ( m_pContainer->getPosition ( ), m_tScrollDistance ) );
    
    if ( m_bBounceable )
    {
        tMaxInset = m_fMaxInset;
        tMinInset = m_fMinInset;
    }
    else
    {
        tMaxInset = this->maxContainerOffset ( );
        tMinInset = this->minContainerOffset ( );
    }
    
    // check to see if offset lies within the inset bounds
    fNewX = KD_MIN ( m_pContainer->getPosition ( ).x, tMaxInset.x );
    fNewX = KD_MAX ( fNewX, tMinInset.x );
    fNewY = KD_MIN ( m_pContainer->getPosition ( ).y, tMaxInset.y );
    fNewY = KD_MAX ( fNewY, tMinInset.y );
    
    fNewX = m_pContainer->getPosition ( ).x;
    fNewY = m_pContainer->getPosition ( ).y;

    m_tScrollDistance = ccpSub  ( m_tScrollDistance, ccp ( fNewX - m_pContainer->getPosition ( ).x, fNewY - m_pContainer->getPosition ( ).y ) );
    m_tScrollDistance = ccpMult ( m_tScrollDistance, m_fDeAccelRate );
    this->setContentOffset ( ccp ( fNewX,fNewY ) );
    
    if ( ( kdFabsf ( m_tScrollDistance.x ) <= m_fDeAccelDist &&
           kdFabsf ( m_tScrollDistance.y ) <= m_fDeAccelDist ) ||
		   fNewY > tMaxInset.y || fNewY < tMinInset.y ||
		   fNewX > tMaxInset.x || fNewX < tMinInset.x ||
		   fNewX == tMaxInset.x || fNewX == tMinInset.x ||
		   fNewY == tMaxInset.y || fNewY == tMinInset.y )
    {
        this->unschedule ( schedule_selector ( CCScrollView::deaccelerateScrolling ) );
        this->relocateContainer ( KD_TRUE );
    }
}

KDvoid CCScrollView::stoppedAnimatedScroll ( CCNode* pNode )
{
    this->unschedule ( schedule_selector ( CCScrollView::performedAnimatedScroll ) );

    // After the animation stopped, "scrollViewDidScroll" should be invoked, this could fix the bug of lack of tableview cells.
    if ( m_pDelegate != KD_NULL )
    {
        m_pDelegate->scrollViewDidScroll ( this );
    }
}

KDvoid CCScrollView::performedAnimatedScroll ( KDfloat fDelta )
{
    if ( m_bDragging )
    {
        this->unschedule ( schedule_selector ( CCScrollView::performedAnimatedScroll ) );
        return;
    }

    if ( m_pDelegate != KD_NULL )
    {
        m_pDelegate->scrollViewDidScroll ( this );
    }
}

const CCSize& CCScrollView::getContentSize ( KDvoid )
{
    return m_pContainer->getContentSize ( );
}

KDvoid CCScrollView::setContentSize ( const CCSize& tSize )
{
    if ( this->getContainer ( ) != KD_NULL )
    {
        this->getContainer ( )->setContentSize ( tSize );
		this->updateInset  ( );
    }
}

KDvoid CCScrollView::updateInset ( KDvoid )
{
	if ( this->getContainer() != KD_NULL )
	{
		m_fMaxInset = this->maxContainerOffset ( );
		m_fMaxInset = ccp ( m_fMaxInset.x + m_tViewSize.cx * m_fInsetRatio, m_fMaxInset.y + m_tViewSize.cy * m_fInsetRatio );
		m_fMinInset = this->minContainerOffset ( );
		m_fMinInset = ccp ( m_fMinInset.x - m_tViewSize.cx * m_fInsetRatio, m_fMinInset.y - m_tViewSize.cy * m_fInsetRatio );
	}
}

//
// make sure all children go to the container
//
KDvoid CCScrollView::addChild ( CCNode* pChild, KDint nZOrder, KDint nTag )
{
    pChild->ignoreAnchorPointForPosition ( KD_FALSE );
    pChild->setAnchorPoint ( ccp ( 0.0f, 0.0f ) );

	if ( m_pContainer != pChild )
	{
        m_pContainer->addChild ( pChild, nZOrder, nTag );
    } 
	else
	{
        CCLayer::addChild ( pChild, nZOrder, nTag );
    }
}

KDvoid CCScrollView::addChild ( CCNode* pChild, KDint nZOrder )
{
    this->addChild ( pChild, nZOrder, pChild->getTag ( ) );
}

KDvoid CCScrollView::addChild ( CCNode* pChild )
{
    this->addChild ( pChild, pChild->getZOrder ( ), pChild->getTag ( ) );
}

//
// clip this view so that outside of the visible bounds can be hidden.
//
KDvoid CCScrollView::beforeDraw ( KDvoid )
{
    if ( m_bClippingToBounds ) 
    {
		m_bScissorRestored = KD_FALSE;

        CCRect  tFrame = getViewRect ( );

        if ( CCEGLView::sharedOpenGLView()->isScissorEnabled ( ) )
		{
            m_bScissorRestored = KD_TRUE;
            m_tParentScissorRect = CCEGLView::sharedOpenGLView ( )->getScissorRect ( );
            
			// set the intersection of m_tParentScissorRect and frame as the new scissor rect
            if ( tFrame.intersectsRect ( m_tParentScissorRect ) )
			{
                KDfloat   x = KD_MAX ( tFrame.origin.x, m_tParentScissorRect.origin.x );
                KDfloat   y = KD_MAX ( tFrame.origin.y, m_tParentScissorRect.origin.y );
                KDfloat  xx = KD_MIN ( tFrame.origin.x + tFrame.size.cx, m_tParentScissorRect.origin.x + m_tParentScissorRect.size.cx );
                KDfloat  yy = KD_MIN ( tFrame.origin.y + tFrame.size.cy, m_tParentScissorRect.origin.y + m_tParentScissorRect.size.cy );
                CCEGLView::sharedOpenGLView ( )->setScissorInPoints ( x, y, xx - x, yy - y );
            }
        }
        else 
		{
            glEnable ( GL_SCISSOR_TEST );
            CCEGLView::sharedOpenGLView ( )->setScissorInPoints ( tFrame.origin.x, tFrame.origin.y, tFrame.size.cx, tFrame.size.cy );
        }
    }
}

//
// retract what's done in beforeDraw so that there's no side effect to other nodes.
//
KDvoid CCScrollView::afterDraw ( KDvoid )
{
    if ( m_bClippingToBounds )
    {
        if ( m_bScissorRestored )
		{
			// restore the parent's scissor rect
            CCEGLView::sharedOpenGLView ( )->setScissorInPoints ( m_tParentScissorRect.origin.x, m_tParentScissorRect.origin.y, m_tParentScissorRect.size.cx, m_tParentScissorRect.size.cy );
        }
        else
		{
            glDisable ( GL_SCISSOR_TEST );
        }
    }
}

KDvoid CCScrollView::visit ( KDvoid )
{
	// quick return if not visible
	if ( !isVisible ( ) )
    {
		return;
    }

	kmGLPushMatrix ( );
	
    if ( m_pGrid && m_pGrid->isActive ( ) )
    {
        m_pGrid->beforeDraw ( );
        this->transformAncestors ( );
    }

	this->transform  ( );
    this->beforeDraw ( );

	if ( m_pChildren )
    {
		ccArray*  pArrayData = m_pChildren->m_pData;
		KDuint    i = 0;

		// draw children zOrder < 0
		for ( ; i < pArrayData->num; i++ )
        {
			CCNode*  pChild =  (CCNode*) pArrayData->arr [ i ];

			if ( pChild->getZOrder ( ) < 0 )
            {
				pChild->visit ( );
			}
            else
            {
				break;
            }
		}
		
		// this draw
		this->draw ( );
		
		// draw children zOrder >= 0
		for ( ; i < pArrayData->num; i++ )
        {
			CCNode*  pChild = (CCNode*) pArrayData->arr [ i ];
			pChild->visit ( );
		}
        
	}
    else
    {
		this->draw ( );
    }

    this->afterDraw ( );
	if ( m_pGrid && m_pGrid->isActive ( ) )
    {
		m_pGrid->afterDraw ( this );
    }

	kmGLPopMatrix ( );
}

KDbool CCScrollView::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !this->isVisible ( ) )
    {
        return KD_FALSE;
    }

	CCRect  tFrame = getViewRect ( );

    // dispatcher does not know about clipping. reject touches outside visible bounds.
    if ( m_pTouches->count ( ) > 2 || m_bTouchMoved ||
		 !tFrame.containsPoint ( m_pContainer->convertToWorldSpace ( m_pContainer->convertTouchToNodeSpace ( pTouch ) ) ) )
    {
        return KD_FALSE;
    }

    if ( !m_pTouches->containsObject ( pTouch ) )
    {
        m_pTouches->addObject ( pTouch );
    }

    if ( m_pTouches->count ( ) == 1 )
    {
		// scrolling
        m_tTouchPoint		= this->convertTouchToNodeSpace ( pTouch );
        m_bTouchMoved		= KD_FALSE;   	
        m_tScrollDistance	= ccp ( 0.0f, 0.0f );
        m_fTouchLength		= 0.0f;

		if ( m_bIgnoreScroll )
		{
			const CCSize&  tContentSize = m_pContainer->getContentSize ( );

			if ( tContentSize.cx > m_tViewSize.cx && ( m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionHorizontal ) )
			{
				m_bDragging = KD_TRUE;
			}

			if ( tContentSize.cy > m_tViewSize.cy && ( m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionVertical ) )
			{
				m_bDragging = KD_TRUE;
			}
		}
		else
		{
			m_bDragging = KD_TRUE;
		}
    }
    else if ( m_pTouches->count ( ) == 2 )
    {
        m_tTouchPoint  = ccpMidpoint ( this->convertTouchToNodeSpace ( (CCTouch*) m_pTouches->objectAtIndex ( 0 ) ),
                                       this->convertTouchToNodeSpace ( (CCTouch*) m_pTouches->objectAtIndex ( 1 ) ) );
        m_fTouchLength = ccpDistance ( m_pContainer->convertTouchToNodeSpace ( (CCTouch*) m_pTouches->objectAtIndex ( 0 ) ),
                                       m_pContainer->convertTouchToNodeSpace ( (CCTouch*) m_pTouches->objectAtIndex ( 1 ) ) );
        m_bDragging    = KD_FALSE;
    } 

    return KD_TRUE;
}

KDvoid CCScrollView::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !this->isVisible ( ) )
    {
        return;
    }

    if ( m_pTouches->containsObject ( pTouch ) )
    {
        if ( m_pTouches->count ( ) == 1 && m_bDragging )
        { 
			// scrolling
            CCPoint  tMoveDist, tNewPoint, tMaxInset, tMinInset;
            CCRect   tFrame;
            KDfloat  fNewX, fNewY;

            tFrame = getViewRect ( );

            tNewPoint = this->convertTouchToNodeSpace ( (CCTouch*) m_pTouches->objectAtIndex ( 0 ) );
            tMoveDist = ccpSub ( tNewPoint, m_tTouchPoint );

            KDfloat  fDis = 0.0f;
            if ( m_eDirection == kCCScrollViewDirectionVertical )
            {
                fDis = tMoveDist.y;
            }
            else if (m_eDirection == kCCScrollViewDirectionHorizontal )
            {
                fDis = tMoveDist.x;
            }
            else
            {
                fDis = kdSqrtf ( tMoveDist.x * tMoveDist.x + tMoveDist.y * tMoveDist.y );
            }
/*
            if ( !m_bTouchMoved && kdFabs ( convertDistanceFromPointToInch ( fDis ) ) < MOVE_INCH )
            {
                //CCLOG("Invalid movement, distance = [%f, %f], disInch = %f", moveDistance.x, moveDistance.y);
                return;
            }
*/           
			if ( !m_bTouchMoved )
			{
				if ( ( kdFabsf ( tMoveDist.y ) > m_fDetectTouchMoved && ( m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionVertical ) )
				  || ( kdFabsf ( tMoveDist.x ) > m_fDetectTouchMoved && ( m_eDirection == kCCScrollViewDirectionBoth || m_eDirection == kCCScrollViewDirectionHorizontal ) ) )
				{
					m_bTouchMoved = KD_TRUE;
				}

				if ( !m_bTouchMoved )
				{
					return;
				}
			}

            if ( !m_bTouchMoved )
            {
                tMoveDist = CCPointZero;
            }          

            m_tTouchPoint = tNewPoint;
            m_bTouchMoved = KD_TRUE;
            
			if ( tFrame.containsPoint ( this->convertToWorldSpace ( tNewPoint ) ) )
            {
                switch ( m_eDirection )
                {
                    case kCCScrollViewDirectionVertical :
                        tMoveDist = ccp ( 0.0f, tMoveDist.y );
                        break;

                    case kCCScrollViewDirectionHorizontal :
                        tMoveDist = ccp ( tMoveDist.x, 0.0f );
                        break;

                    default :
                        break;
                }
   
                tMaxInset = m_fMaxInset;
                tMinInset = m_fMinInset;
                                
                fNewX     = m_pContainer->getPosition ( ).x + tMoveDist.x;
                fNewY     = m_pContainer->getPosition ( ).y + tMoveDist.y;
                
                m_tScrollDistance = tMoveDist;
				this->setContentOffset ( ccp ( fNewX, fNewY ) );
            }
        }
        else if ( m_pTouches->count ( ) == 2 && !m_bDragging )
        {
            const KDfloat  fLen = ccpDistance
			(
				m_pContainer->convertTouchToNodeSpace ( (CCTouch*) m_pTouches->objectAtIndex ( 0 ) ),
				m_pContainer->convertTouchToNodeSpace ( (CCTouch*) m_pTouches->objectAtIndex ( 1 ) )
			);
            this->setZoomScale ( this->getZoomScale ( ) * fLen / m_fTouchLength );
        }
    }
}

KDvoid CCScrollView::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( !this->isVisible ( ) )
    {
        return;
    }

    if ( m_pTouches->containsObject ( pTouch ) )
    {
        if ( m_pTouches->count ( ) == 1 && m_bTouchMoved )
        {
            this->schedule ( schedule_selector ( CCScrollView::deaccelerateScrolling ) );
        }

        m_pTouches->removeObject ( pTouch );
    } 

    if ( m_pTouches->count ( ) == 0 )
    {
        m_bDragging   = KD_FALSE;    
        m_bTouchMoved = KD_FALSE;
    }
}

KDbool CCScrollView::isIgnoreScroll ( KDvoid )
{
	return m_bIgnoreScroll;
}

KDvoid CCScrollView::ignoreScroll ( KDbool bIsIgnoreScroll )
{
	m_bIgnoreScroll = bIsIgnoreScroll;
}

CCRect CCScrollView::getViewRect ( KDvoid )
{
    CCPoint  tScreenPos = this->convertToWorldSpace ( CCPointZero );
    
    KDfloat  fScaleX = this->getScaleX ( );
    KDfloat  fScaleY = this->getScaleY ( );
    
    for ( CCNode*  p = m_pParent; p != KD_NULL; p = p->getParent ( ) )
	{
        fScaleX *= p->getScaleX ( );
        fScaleY *= p->getScaleY ( );
    }
    
    return CCRectMake ( tScreenPos.x, tScreenPos.y, m_tViewSize.cx * fScaleX, m_tViewSize.cy * fScaleY );
}

NS_CC_END
