/* --------------------------------------------------------------------------
 *
 *      File            CCActionGrid.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
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
#include "actions/CCActionGrid.h"
#include "cocoa/CCZone.h"
#include "effects/CCGrid.h"
#include "support/CCDirector.h"

NS_CC_BEGIN
    
//
// implementation of CCGridAction
//

CCGridAction* CCGridAction::create ( KDfloat fDuration, const CCSize& tGridSize )
{
	CCGridAction*  pRet = new CCGridAction ( );

    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCGridAction::initWithDuration ( KDfloat fDuration, const CCSize& tGridSize )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tGridSize = tGridSize;

		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CCGridAction::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	CCGridBase*  pNewGrid = this->getGrid ( );
	CCGridBase*  pTargetGrid = m_pTarget->getGrid ( );

	if ( pTargetGrid && pTargetGrid->getReuseGrid ( ) > 0 )
	{
		if ( pTargetGrid->isActive ( ) && pTargetGrid->getGridSize ( ).cx == m_tGridSize.cx
			                           && pTargetGrid->getGridSize ( ).cy == m_tGridSize.cy )
		{
			pTargetGrid->reuse ( );
		}
		else
		{
			CCAssert ( 0, "" );
		}
	}
	else
	{
		if ( pTargetGrid && pTargetGrid->isActive ( ) )
		{
			pTargetGrid->setActive ( KD_FALSE );
		}

		pTarget->setGrid ( pNewGrid );
		pTarget->getGrid ( )->setActive ( KD_TRUE );
	}
}

CCGridBase* CCGridAction::getGrid ( KDvoid )
{
	// Abstract class needs implementation
	CCAssert ( 0, "" );

	return KD_NULL;
}

CCActionInterval* CCGridAction::reverse ( KDvoid )
{
	return CCReverseTime::create ( this );
}

CCObject* CCGridAction::copyWithZone ( CCZone* pZone )
{
	CCZone*     pNewZone = KD_NULL;
	CCGridAction*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCGridAction *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCGridAction ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tGridSize );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

//
// implementation of Grid3DAction
//
CCGridBase* CCGrid3DAction::getGrid ( KDvoid )
{
	return CCGrid3D::create ( m_tGridSize );
}

ccVertex3F CCGrid3DAction::vertex ( const CCPoint& tPos )
{
	CCGrid3D*  pGrid = (CCGrid3D*) m_pTarget->getGrid ( );

	return pGrid->vertex ( tPos );
}

ccVertex3F CCGrid3DAction::originalVertex ( const CCPoint& tPos )
{
	CCGrid3D*  pGrid = (CCGrid3D*) m_pTarget->getGrid ( );

	return pGrid->originalVertex ( tPos );
}

KDvoid CCGrid3DAction::setVertex ( const CCPoint& tPos, const ccVertex3F& tVertex )
{
	CCGrid3D*  pGrid = (CCGrid3D*) m_pTarget->getGrid ( );
	pGrid->setVertex ( tPos, tVertex );
}

CCGrid3DAction* CCGrid3DAction::create ( KDfloat fDuration, const CCSize& tGridSize )
{
    CCGrid3DAction*  pRet = new CCGrid3DAction ( );
    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    return pRet;
}

//
// implementation of TiledGrid3DAction
//
CCGridBase* CCTiledGrid3DAction::getGrid ( KDvoid )
{
	return CCTiledGrid3D::create ( m_tGridSize );
}

ccQuad3 CCTiledGrid3DAction::tile ( const CCPoint& tPos )
{
	CCTiledGrid3D*  pGrid = (CCTiledGrid3D *) m_pTarget->getGrid ( );

	return pGrid->tile ( tPos );
}

ccQuad3 CCTiledGrid3DAction::originalTile ( const CCPoint& tPos )
{
	CCTiledGrid3D*  pGrid = (CCTiledGrid3D *) m_pTarget->getGrid ( );

	return pGrid->originalTile ( tPos );
}

KDvoid CCTiledGrid3DAction::setTile ( const CCPoint& tPos, const ccQuad3& tCoords )
{
	CCTiledGrid3D*  pGrid = (CCTiledGrid3D *) m_pTarget->getGrid ( );

	pGrid->setTile ( tPos, tCoords );
}

CCTiledGrid3DAction* CCTiledGrid3DAction::create ( KDfloat fDuration, const CCSize& tGridSize )
{
    CCTiledGrid3DAction*  pRet = new CCTiledGrid3DAction ( );
    if ( pRet && pRet->initWithDuration ( fDuration, tGridSize ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    return pRet;
}

//
// implementation CCAccelDeccelAmplitude
//

CCAccelDeccelAmplitude* CCAccelDeccelAmplitude::create ( CCAction* pAction, KDfloat fDuration )
{
	CCAccelDeccelAmplitude*  pRet = new CCAccelDeccelAmplitude ( );

    if ( pRet && pRet->initWithAction ( pAction, fDuration ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCAccelDeccelAmplitude::initWithAction ( CCAction* pAction, KDfloat fDuration )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_fRate = 1.0f;
		m_pOther = (CCActionInterval *) pAction;
		pAction->retain ( );

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCAccelDeccelAmplitude::~CCAccelDeccelAmplitude ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pOther );
}

KDvoid CCAccelDeccelAmplitude::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_pOther->startWithTarget ( pTarget );
}

KDvoid CCAccelDeccelAmplitude::update ( KDfloat fTime )
{
	KDfloat  f = fTime * 2;

	if ( f > 1 )
	{
		f -= 1;
		f  = 1 - f;
	}

	 ( (CCAccelDeccelAmplitude *) m_pOther )->setAmplitudeRate ( kdPowf ( f, m_fRate ) );
}

CCActionInterval* CCAccelDeccelAmplitude::reverse ( KDvoid )
{
	return CCAccelDeccelAmplitude::create ( m_pOther->reverse ( ), m_fDuration );
}

//
// implementation of AccelAmplitude
//

CCAccelAmplitude* CCAccelAmplitude::create ( CCAction* pAction, KDfloat fDuration )
{
	CCAccelAmplitude*  pRet = new CCAccelAmplitude ( );

    if ( pRet && pRet->initWithAction ( pAction, fDuration ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCAccelAmplitude::initWithAction ( CCAction* pAction, KDfloat fDuration )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_fRate = 1.0f;
		m_pOther = (CCActionInterval *) pAction;
		pAction->retain ( );

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCAccelAmplitude::~CCAccelAmplitude ( KDvoid )
{
	CC_SAFE_DELETE ( m_pOther );
}

KDvoid CCAccelAmplitude::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_pOther->startWithTarget ( pTarget );
}

KDvoid CCAccelAmplitude::update ( KDfloat fTime )
{
    ( (CCAccelAmplitude *) m_pOther )->setAmplitudeRate ( kdPowf ( fTime, m_fRate ) );

	m_pOther->update ( fTime );
}

CCActionInterval* CCAccelAmplitude::reverse ( KDvoid )
{
	return CCAccelAmplitude::create ( m_pOther->reverse ( ), m_fDuration );
}

//
// DeccelAmplitude
//

CCDeccelAmplitude* CCDeccelAmplitude::create ( CCAction* pAction, KDfloat fDuration )
{
	CCDeccelAmplitude*  pRet = new CCDeccelAmplitude ( );

    if ( pRet && pRet->initWithAction ( pAction, fDuration ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCDeccelAmplitude::initWithAction ( CCAction* pAction, KDfloat fDuration )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_fRate = 1.0f;
		m_pOther = (CCActionInterval *) pAction;
		pAction->retain ( );

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCDeccelAmplitude::~CCDeccelAmplitude ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pOther );
}

KDvoid CCDeccelAmplitude::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_pOther->startWithTarget ( pTarget );
}

KDvoid CCDeccelAmplitude::update ( KDfloat fTime )
{
    ( (CCDeccelAmplitude *) m_pOther )->setAmplitudeRate ( kdPowf ( ( 1 - fTime ), m_fRate ) );

	m_pOther->update ( fTime );
}

CCActionInterval* CCDeccelAmplitude::reverse ( KDvoid )
{
	return CCDeccelAmplitude::create ( m_pOther->reverse ( ), m_fDuration );
}

//
// implementation of StopGrid
//
KDvoid CCStopGrid::startWithTarget ( CCNode* pTarget )
{
	CCActionInstant::startWithTarget ( pTarget );

	CCGridBase*  pGrid = m_pTarget->getGrid ( );

	if ( pGrid && pGrid->isActive ( ) )
	{
		pGrid->setActive ( KD_FALSE );
	}
}

CCStopGrid* CCStopGrid::create ( KDvoid )
{
	CCStopGrid*  pRet = new CCStopGrid ( );

    if ( pRet )
    {
        pRet->autorelease ( );
    }

	return pRet;
}

//
// implementation of CCReuseGrid
//

CCReuseGrid* CCReuseGrid::create ( KDint nTimes )
{
	CCReuseGrid*  pRet = new CCReuseGrid ( );

    if ( pRet && pRet->initWithTimes ( nTimes ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCReuseGrid::initWithTimes ( KDint nTimes )
{
	m_nTimes = nTimes;

	return KD_TRUE;
}

KDvoid CCReuseGrid::startWithTarget ( CCNode* pTarget )
{
	CCActionInstant::startWithTarget ( pTarget );

	if ( m_pTarget->getGrid ( ) && m_pTarget->getGrid ( )->isActive ( ) )
	{
		m_pTarget->getGrid ( )->setReuseGrid ( m_pTarget->getGrid ( )->getReuseGrid ( ) + m_nTimes );
	}
}

NS_CC_END