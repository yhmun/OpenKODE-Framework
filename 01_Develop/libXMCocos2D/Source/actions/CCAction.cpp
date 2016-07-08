/* -----------------------------------------------------------------------------------
 *
 *      File            CCAction.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
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
#include "actions/CCAction.h"
#include "actions/CCActionInterval.h"
#include "cocoa/CCZone.h"
#include "cocoa/CCString.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"

NS_CC_BEGIN
    
//
// Action Base Class
//
CCAction::CCAction ( KDvoid )
{
    m_pOriginalTarget   = KD_NULL;
    m_pTarget           = KD_NULL;
    m_nTag              = kCCActionTagInvalid;
}

CCAction::~CCAction ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCAction = %p", this );
}

CCAction* CCAction::create ( KDvoid )
{
	CCAction*  pRet = new CCAction ( );

    if ( pRet )
    {
        pRet->autorelease ( );
    }
    
	return pRet;
}

const KDchar* CCAction::description ( KDvoid )
{
    return CCString::createWithFormat ( "<CCAction | Tag = %d>", m_nTag )->getCString ( );
}

CCObject* CCAction::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCAction*    pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		pRet = (CCAction *) pZone->m_pCopyObject;
	}
	else
	{
		pRet = new CCAction ( );
		pNewZone = new CCZone ( pRet );
	}

	// copy member data
	pRet->m_nTag = m_nTag;
	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

KDvoid CCAction::startWithTarget ( CCNode* aTarget )
{
	m_pOriginalTarget = m_pTarget = aTarget;
}

KDvoid CCAction::stop ( KDvoid )
{
	m_pTarget = KD_NULL;
}

KDbool CCAction::isDone ( KDvoid )
{
	return KD_TRUE;
}

KDvoid CCAction::step ( KDfloat fDelta )
{
	CC_UNUSED_PARAM ( fDelta );
	CCLOG ( "[Action step]. override me" );
}

KDvoid CCAction::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );
	CCLOG ( "[Action update]. override me" );
}

//
// FiniteTimeAction
//
CCFiniteTimeAction::CCFiniteTimeAction ( KDvoid )
{
    m_fDuration = 0.0f;
}

CCFiniteTimeAction* CCFiniteTimeAction::reverse ( KDvoid )
{
	CCLOG ( "XMCocos2D : FiniteTimeAction#reverse - Implement me");

	return KD_NULL;
}

//
// Speed
//
CCSpeed::CCSpeed ( KDvoid )
{
    m_fSpeed        = 0.0f;
    m_pInnerAction  = KD_NULL;
}

CCSpeed::~CCSpeed ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pInnerAction );
}

CCSpeed* CCSpeed::create ( CCActionInterval* pAction, KDfloat fSpeed )
{
	CCSpeed*  pRet = new CCSpeed ( );

	if ( pRet && pRet->initWithAction ( pAction, fSpeed ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCSpeed::initWithAction ( CCActionInterval* pAction, KDfloat fSpeed )
{
	CCAssert ( pAction != KD_NULL, "" );

	pAction->retain ( );

	m_pInnerAction = pAction;
	m_fSpeed = fSpeed;	

	return KD_TRUE;
}

CCObject* CCSpeed::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCSpeed*     pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )	// in case of being called at sub class
	{
		pRet = (CCSpeed *) pZone->m_pCopyObject;
	}
	else
	{
		pRet = new CCSpeed ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCAction::copyWithZone ( pZone );

	pRet->initWithAction ( (CCActionInterval *) m_pInnerAction->copy ( )->autorelease ( ), m_fSpeed );
	
	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

KDvoid CCSpeed::startWithTarget ( CCNode* pTarget )
{
	CCAction::startWithTarget ( pTarget );
    
	m_pInnerAction->startWithTarget ( pTarget );
}

KDvoid CCSpeed::stop ( KDvoid )
{
	m_pInnerAction->stop ( );
    
	CCAction::stop ( );
}

KDvoid CCSpeed::step ( KDfloat fDelta )
{
    m_pInnerAction->step ( fDelta * m_fSpeed );
}

KDbool CCSpeed::isDone ( KDvoid )
{
	return m_pInnerAction->isDone ( );
}

CCActionInterval* CCSpeed::reverse ( )
{
	 return (CCActionInterval*) CCSpeed::create ( m_pInnerAction->reverse ( ), m_fSpeed );
}

KDvoid CCSpeed::setInnerAction ( CCActionInterval* pAction )
{
	if ( m_pInnerAction != pAction )
	{
		CC_SAFE_RELEASE ( m_pInnerAction );
        
		m_pInnerAction = pAction;
		
        CC_SAFE_RETAIN ( m_pInnerAction );
	}
}

//
// Follow
//
CCFollow::CCFollow ( KDvoid )
{
    m_pFollowedNode         = KD_NULL;
    m_bBoundarySet          = KD_FALSE;
    m_bBoundaryFullyCovered = KD_FALSE;		
    m_fLeftBoundary         = 0.0f;
    m_fRightBoundary        = 0.0f;
    m_fTopBoundary          = 0.0f;
    m_fBottomBoundary       = 0.0f;
}

CCFollow::~CCFollow ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pFollowedNode );
}

CCFollow* CCFollow::create ( CCNode* pFollowedNode, const CCRect& tRect )
{
	CCFollow*  pRet = new CCFollow ( );

	if ( pRet && pRet->initWithTarget ( pFollowedNode, tRect ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCFollow::initWithTarget ( CCNode* pFollowedNode, const CCRect& tRect )
{
	CCAssert ( pFollowedNode != KD_NULL, "" );

	pFollowedNode->retain ( );

	m_pFollowedNode = pFollowedNode;
	m_bBoundarySet  = tRect.equals ( CCRectZero ) ? KD_FALSE : KD_TRUE;
	m_bBoundaryFullyCovered = KD_FALSE;

	const CCSize&  winSize = CCDirector::sharedDirector ( )->getWinSize ( );
	m_tFullScreenSize = CCPointMake ( winSize.width, winSize.height );
	m_tHalfScreenSize = ccpMult ( m_tFullScreenSize, 0.5f );

	if ( m_bBoundarySet )
	{
		m_fLeftBoundary   = -( ( tRect.origin.x + tRect.size.cx ) - m_tFullScreenSize.x );
		m_fRightBoundary  = -tRect.origin.x ;
		m_fTopBoundary    = -tRect.origin.y;
		m_fBottomBoundary = -( ( tRect.origin.y + tRect.size.height ) - m_tFullScreenSize.y );

		if ( m_fRightBoundary < m_fLeftBoundary )
		{
			// screen width is larger than world's boundary width
			// set both in the middle of the world
			m_fRightBoundary = m_fLeftBoundary = ( m_fLeftBoundary + m_fRightBoundary ) / 2;
		}

		if ( m_fTopBoundary < m_fBottomBoundary )
		{
			// screen width is larger than world's boundary width
			// set both in the middle of the world
			m_fTopBoundary = m_fBottomBoundary = ( m_fTopBoundary + m_fBottomBoundary ) / 2;
		}

		if( ( m_fTopBoundary == m_fBottomBoundary ) && ( m_fLeftBoundary == m_fRightBoundary ) )
		{
			m_bBoundaryFullyCovered = KD_TRUE;
		}
	}

	return KD_TRUE;
}

CCObject* CCFollow::copyWithZone ( CCZone* pZone )
{
	CCZone*    pNewZone = KD_NULL;
	CCFollow*  pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) //in case of being called at sub class
	{
		pRet = (CCFollow*) ( pZone->m_pCopyObject );
	}
	else
	{
		pRet = new CCFollow ( );
		pZone = pNewZone = new CCZone ( pRet );
	}

	CCAction::copyWithZone ( pZone );

	// copy member data
	pRet->m_nTag = m_nTag;
	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

KDvoid CCFollow::step ( KDfloat fDelta )
{
	CC_UNUSED_PARAM ( fDelta );

	if ( m_bBoundarySet )
	{
		// whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
		if ( m_bBoundaryFullyCovered )
		{
			return;
		}

		CCPoint tempPos = ccpSub ( m_tHalfScreenSize, m_pFollowedNode->getPosition ( ) );

		m_pTarget->setPosition ( ccp ( clampf ( tempPos.x, m_fLeftBoundary, m_fRightBoundary ), 
								       clampf ( tempPos.y, m_fBottomBoundary, m_fTopBoundary ) ) );
	}
	else
	{
		m_pTarget->setPosition ( ccpSub ( m_tHalfScreenSize, m_pFollowedNode->getPosition ( ) ) );
	}
}

KDbool CCFollow::isDone ( KDvoid )
{
	return ( !m_pFollowedNode->isRunning ( ) );
}

KDvoid CCFollow::stop ( KDvoid )
{
	m_pTarget = KD_NULL;
	CCAction::stop ( );
}
    

NS_CC_END