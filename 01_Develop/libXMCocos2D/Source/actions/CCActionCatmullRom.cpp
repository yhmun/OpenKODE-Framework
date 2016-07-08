/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionCatmullRom.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008      Radu Gruian  : Original code 
 *      Copyright (c) 2011      Vit Valentin : Adapted to cocos2d-x 
 *      Copyright (c) 2012      Ricardo Quesada
 *
 *         http://www.cocos2d-x.org      
 *         http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio.So
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "actions/CCActionCatmullRom.h"
#include "cocoa/CCZone.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

//
//  Implementation of CCPointArray
//

CCPointArray* CCPointArray::create ( KDuint uCapacity )
{
    CCPointArray*  pRet = new CCPointArray ( );

    if ( pRet && pRet->initWithCapacity ( uCapacity ) )
    {
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

KDbool CCPointArray::initWithCapacity ( KDuint uCapacity )
{
	m_pControlPoints = new std::vector<CCPoint*> ( );
    
    return KD_TRUE;
}

CCObject* CCPointArray::copyWithZone ( CCZone* pZone )
{
    std::vector<CCPoint*>*   pNewArray = new std::vector<CCPoint*> ( );
	std::vector<CCPoint*>::iterator  iter;

	for ( iter = m_pControlPoints->begin ( ); iter != m_pControlPoints->end ( ); ++iter )
    {
        pNewArray->push_back ( new CCPoint ( ( *iter )->x, ( *iter )->y ) );
    }

    CCPointArray*  pPoints = new CCPointArray ( );
    pPoints->initWithCapacity ( 10 );
    pPoints->setControlPoints ( pNewArray );
    
    return pPoints;
}

CCPointArray::~CCPointArray ( KDvoid )
{
	std::vector<CCPoint*>::iterator  iter;
	for ( iter = m_pControlPoints->begin ( ); iter != m_pControlPoints->end ( ); ++iter )
	{
		delete *iter;
	}

    delete m_pControlPoints;
}

CCPointArray::CCPointArray ( KDvoid )
{
	m_pControlPoints = KD_NULL;
}

const std::vector<CCPoint*>* CCPointArray::getControlPoints ( KDvoid )
{
    return m_pControlPoints;
}

KDvoid CCPointArray::setControlPoints ( std::vector<CCPoint*>* pControlPoints )
{
    CCAssert ( pControlPoints != KD_NULL, "control points should not be NULL" );
    
    // delete old points
	std::vector<CCPoint*>::iterator iter;
    for ( iter = m_pControlPoints->begin ( ); iter != m_pControlPoints->end ( ); ++iter )
    {
        delete *iter;
    }
    delete m_pControlPoints;
    
    m_pControlPoints = pControlPoints;
}

KDvoid CCPointArray::addControlPoint ( CCPoint tControlPoint )
{
    m_pControlPoints->push_back ( new CCPoint ( tControlPoint.x, tControlPoint.y ) );
}

KDvoid CCPointArray::insertControlPoint ( const CCPoint& tControlPoint, KDuint uIndex )
{
    CCPoint*  pTemp = new CCPoint ( tControlPoint.x, tControlPoint.y );
    m_pControlPoints->insert ( m_pControlPoints->begin ( ) + uIndex, pTemp );
}

CCPoint CCPointArray::getControlPointAtIndex ( KDuint uIndex )
{
    uIndex = KD_MIN ( m_pControlPoints->size ( ) - 1, KD_MAX ( uIndex, 0 ) );
	return *( m_pControlPoints->at ( uIndex ) );
}

KDvoid CCPointArray::replaceControlPoint ( const CCPoint& tControlPoint, KDuint uIndex )
{
    CCPoint*  pTemp = m_pControlPoints->at ( uIndex );
    pTemp->x = tControlPoint.x;
    pTemp->y = tControlPoint.y;
}

KDvoid CCPointArray::removeControlPointAtIndex ( KDuint uIndex )
{
	std::vector<CCPoint*>::iterator  iter = m_pControlPoints->begin ( ) + uIndex;
    CCPoint*  pRemovedPoint = *iter;
    m_pControlPoints->erase ( iter );
    delete pRemovedPoint;
}

KDuint CCPointArray::count ( KDvoid )
{
    return m_pControlPoints->size ( );
}

CCPointArray* CCPointArray::reverse ( KDvoid )
{
	std::vector<CCPoint*>*  pNewArray = new std::vector<CCPoint*> ( );
	std::vector<CCPoint*>::reverse_iterator  iter;
    CCPoint*  pPoint = KD_NULL;
    for ( iter = m_pControlPoints->rbegin ( ); iter != m_pControlPoints->rend ( ); ++iter )
    {
        pPoint = *iter;
        pNewArray->push_back ( new CCPoint ( pPoint->x, pPoint->y ) );
    }

    CCPointArray*  pConfig = CCPointArray::create ( 0 );
    pConfig->setControlPoints ( pNewArray );
    
    return pConfig;
}

KDvoid CCPointArray::reverseInline ( KDvoid )
{
    KDuint    l  = m_pControlPoints->size ( );
    CCPoint*  p1 = KD_NULL;
    CCPoint*  p2 = KD_NULL;
	KDfloat   x, y;

    for ( KDuint i = 0; i < l / 2; ++i )
    {
        p1 = m_pControlPoints->at ( i );
        p2 = m_pControlPoints->at ( l - i - 1 );
        
        x = p1->x;
        y = p1->y;
        
        p1->x = p2->x;
        p1->y = p2->y;
        
        p2->x = x;
        p2->y = y;
    }
}

// CatmullRom Spline formula:
CCPoint ccCardinalSplineAt ( CCPoint& tP0, CCPoint& tP1, CCPoint& tP2, CCPoint& tP3, KDfloat fTension, KDfloat t )
{
    KDfloat  t2 = t * t;
    KDfloat  t3 = t2 * t;
    
	//
	// Formula: s(-ttt + 2tt - t)P1 + s(-ttt + tt)P2 + (2ttt - 3tt + 1)P2 + s(ttt - 2tt + t)P3 + (-2ttt + 3tt)P3 + s(ttt - tt)P4
	//
    KDfloat  s = ( 1 - fTension ) / 2;
	
    KDfloat b1 = s * ( ( -t3 + ( 2 * t2 ) ) - t );                      // s(-t3 + 2 t2 - t)P1
    KDfloat b2 = s * ( -t3 + t2 ) + ( 2 * t3 - 3 * t2 + 1 );			// s(-t3 + t2)P2 + (2 t3 - 3 t2 + 1)P2
    KDfloat b3 = s * ( t3 - 2 * t2 + t ) + ( -2 * t3 + 3 * t2 );		// s(t3 - 2 t2 + t)P3 + (-2 t3 + 3 t2)P3
    KDfloat b4 = s * ( t3 - t2 );										// s(t3 - t2)P4
    
    KDfloat  x = ( tP0.x * b1 + tP1.x * b2 + tP2.x * b3 + tP3.x * b4 );
    KDfloat  y = ( tP0.y * b1 + tP1.y * b2 + tP2.y * b3 + tP3.y * b4 );
	
	return ccp ( x, y );
}

//
// Implementation of CCCardinalSplineTo
//

CCCardinalSplineTo* CCCardinalSplineTo::create ( KDfloat fDuration, CCPointArray* pPoints, KDfloat fTension )
{
    CCCardinalSplineTo*  pRet = new CCCardinalSplineTo ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pPoints, fTension ) )
    {
		pRet->autorelease ( );
	}
	else 
	{
		CC_SAFE_DELETE ( pRet );
	}
    
    return pRet;
}

KDbool CCCardinalSplineTo::initWithDuration ( KDfloat fDuration, CCPointArray* pPoints, KDfloat fTension )
{
    CCAssert ( pPoints->count ( ) > 0, "Invalid configuration. It must at least have one control point" );

    if ( CCActionInterval::initWithDuration ( fDuration ) )
    {
        this->setPoints ( pPoints );
        this->m_fTension = fTension;
        
        return KD_TRUE;
    }
    
    return KD_FALSE;
}

CCCardinalSplineTo::~CCCardinalSplineTo ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pPoints );
}

CCCardinalSplineTo::CCCardinalSplineTo ( KDvoid )
{
	m_pPoints	= KD_NULL;
	m_fTension	= 0.f;
	m_fDeltaT	= 0.f;
}

KDvoid CCCardinalSplineTo::startWithTarget ( CCNode* pTarget )
{
    CCActionInterval::startWithTarget ( pTarget );
		
	// Issue #1441
	m_fDeltaT = m_pPoints->count ( ) > 0 ? 1.f / ( m_pPoints->count ( ) - 1 ) : 0;

	m_tPreviousPosition = pTarget->getPosition ( );
    m_tAccumulatedDiff  = ccpz;
}

CCCardinalSplineTo* CCCardinalSplineTo::copyWithZone ( CCZone* pZone )
{
    CCZone*           pNewZone = KD_NULL;
    CCCardinalSplineTo*   pRet = KD_NULL;
    if ( pZone && pZone->m_pCopyObject )	//in case of being called at sub class
    {
        pRet = (CCCardinalSplineTo*) ( pZone->m_pCopyObject );
    }
    else
    {
        pRet  = new CCCardinalSplineTo ( );
        pZone = pNewZone = new CCZone ( pRet );
    }

    CCActionInterval::copyWithZone ( pZone );

    pRet->initWithDuration ( this->getDuration ( ), this->m_pPoints, this->m_fTension );

    CC_SAFE_DELETE ( pNewZone );
    return pRet;
}

KDvoid CCCardinalSplineTo::update ( KDfloat fTime )
{
    KDuint  p;
    KDfloat lt;
	
	// eg.
	// p..p..p..p..p..p..p
	// 1..2..3..4..5..6..7
	// want p to be 1, 2, 3, 4, 5, 6
    if ( fTime == 1 )
    {
        p  = m_pPoints->count ( ) - 1;
        lt = 1;
    }
    else 
    {
        p  = (KDuint) ( fTime / m_fDeltaT );
        lt = ( fTime - m_fDeltaT * p ) / m_fDeltaT;
    }
    
	// Interpolate    
    CCPoint  tP0 = m_pPoints->getControlPointAtIndex ( p - 1 );
    CCPoint  tP1 = m_pPoints->getControlPointAtIndex ( p + 0 );
    CCPoint  tP2 = m_pPoints->getControlPointAtIndex ( p + 1 );
    CCPoint  tP3 = m_pPoints->getControlPointAtIndex ( p + 2 );
	
    CCPoint  tNewPos = ccCardinalSplineAt ( tP0, tP1, tP2, tP3, m_fTension, lt );
	
#if CC_ENABLE_STACKABLE_ACTIONS
    // Support for stacked actions
    CCNode*  pNode = m_pTarget;
    CCPoint  tDiff = ccpSub ( pNode->getPosition ( ), m_tPreviousPosition );
    if ( tDiff.x !=0 || tDiff.y != 0 )
	{
        m_tAccumulatedDiff = ccpAdd ( m_tAccumulatedDiff, tDiff );
        tNewPos = ccpAdd ( tNewPos, m_tAccumulatedDiff);
    }
#endif

    this->updatePosition ( tNewPos );
}

KDvoid CCCardinalSplineTo::updatePosition ( CCPoint& tNewPos )
{
    m_pTarget->setPosition ( tNewPos );
	m_tPreviousPosition = tNewPos;
}

CCActionInterval* CCCardinalSplineTo::reverse ( KDvoid )
{
    CCPointArray*  pReverse = m_pPoints->reverse ( );
    
    return CCCardinalSplineTo::create ( m_fDuration, pReverse, m_fTension );
}

//
// CCCardinalSplineBy
//

CCCardinalSplineBy* CCCardinalSplineBy::create ( KDfloat fDuration, CCPointArray* pPoints, KDfloat fTension )
{
    CCCardinalSplineBy*  pRet = new CCCardinalSplineBy ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pPoints, fTension ) )
    {
		pRet->autorelease ( );
	}
	else 
	{
		CC_SAFE_DELETE ( pRet );
	}
    
    return pRet;
}

CCCardinalSplineBy::CCCardinalSplineBy ( KDvoid )
{
	m_tStartPosition = ccp ( 0, 0 );
}

KDvoid CCCardinalSplineBy::updatePosition ( CCPoint& tNewPos )
{
	CCPoint  p = ccpAdd ( tNewPos, m_tStartPosition );
    m_pTarget->setPosition ( p );
	m_tPreviousPosition = p;
}

CCActionInterval* CCCardinalSplineBy::reverse ( KDvoid )
{
    CCPointArray*  pCopyConfig = (CCPointArray*) m_pPoints->copy ( );
	
	//
	// convert "absolutes" to "diffs"
	//
    CCPoint  tPos = pCopyConfig->getControlPointAtIndex ( 0 );
    for ( KDuint i = 1; i < pCopyConfig->count ( ); ++i )
    {
        CCPoint  tCurrent = pCopyConfig->getControlPointAtIndex ( i );
        CCPoint  tDiff = ccpSub ( tCurrent, tPos );
        pCopyConfig->replaceControlPoint ( tDiff, i );
		tPos = tCurrent;
    }
	
	// convert to "diffs" to "reverse absolute"
	
    CCPointArray*  pReverse = pCopyConfig->reverse ( );
    pCopyConfig->release ( );
	
	// 1st element (which should be 0,0) should be here too
    
    tPos = pReverse->getControlPointAtIndex ( pReverse->count ( ) - 1 );
    pReverse->removeControlPointAtIndex ( pReverse->count ( ) - 1 );
    
    tPos = ccpNeg ( tPos );
    pReverse->insertControlPoint ( tPos, 0 );
    
    for ( KDuint i = 1; i < pReverse->count ( ); ++i )
    {
        CCPoint  tCurrent = pReverse->getControlPointAtIndex ( i );
        tCurrent = ccpNeg ( tCurrent );
        CCPoint  tAbs = ccpAdd ( tCurrent, tPos );
        pReverse->replaceControlPoint ( tAbs, i );
        tPos = tAbs;
    }
	
    return CCCardinalSplineBy::create ( m_fDuration, pReverse, m_fTension );
}

KDvoid CCCardinalSplineBy::startWithTarget ( CCNode* pTarget )
{    
    CCCardinalSplineTo::startWithTarget ( pTarget );
    m_tStartPosition = pTarget->getPosition ( );
}

//
// CCCatmullRomTo
//

CCCatmullRomTo* CCCatmullRomTo::create ( KDfloat fDuration, CCPointArray* pPoints )
{
    CCCatmullRomTo*  pRet = new CCCatmullRomTo ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pPoints ) )
    {
        pRet->autorelease ( );
    }
    else 
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

KDbool CCCatmullRomTo::initWithDuration ( KDfloat fDuration, CCPointArray* pPoints )
{
    if ( CCCardinalSplineTo::initWithDuration ( fDuration, pPoints, 0.5f ) )
    {
        return KD_TRUE;
    }
    
    return KD_FALSE;
}

//
// CCCatmullRomBy
//

CCCatmullRomBy* CCCatmullRomBy::create ( KDfloat fDuration, CCPointArray* pPoints )
{
    CCCatmullRomBy*  pRet = new CCCatmullRomBy ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pPoints ) )
    {
        pRet->autorelease ( );
    }
    else 
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

KDbool CCCatmullRomBy::initWithDuration ( KDfloat fDuration, CCPointArray* pPoints )
{
    if ( CCCardinalSplineTo::initWithDuration ( fDuration, pPoints, 0.5f ) )
    {
        return KD_TRUE;
    }
    
    return KD_FALSE;
}

NS_CC_END

