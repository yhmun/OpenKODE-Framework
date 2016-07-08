/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionProgressTimer.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Lam Pham
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
#include "actions/CCActionProgressTimer.h"
#include "misc_nodes/CCProgressTimer.h"
#include "cocoa/CCZone.h"

NS_CC_BEGIN
    
CCProgressTo* CCProgressTo::create ( KDfloat fDuration, KDfloat fPercent )
{
	CCProgressTo*  pRet = new CCProgressTo ( );

    if ( pRet && pRet->initWithDuration ( fDuration, fPercent ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCProgressTo::initWithDuration ( KDfloat fDuration, KDfloat fPercent )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_fTo = fPercent;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCProgressTo::copyWithZone ( CCZone* pZone )
{
	CCZone*     pNewZone = KD_NULL;
	CCProgressTo*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCProgressTo *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCProgressTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_fTo );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCProgressTo::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );
	m_fFrom = ( (CCProgressTimer *) pTarget )->getPercentage ( );

	// XXX: Is this correct ?
	// Adding it to support CCRepeat
	if ( m_fFrom == 100 )  
	{
		m_fFrom = 0;
	}
}

KDvoid CCProgressTo::update ( KDfloat fTime )
{
	( (CCProgressTimer *) m_pTarget )->setPercentage ( m_fFrom + ( m_fTo - m_fFrom ) * fTime );
}

//
// implementation of CCProgressFromTo
//

CCProgressFromTo* CCProgressFromTo::create ( KDfloat fDuration, KDfloat fFromPercentage, KDfloat fToPercentage )
{
	CCProgressFromTo*  pRet = new CCProgressFromTo ( );

    if ( pRet && pRet->initWithDuration ( fDuration, fFromPercentage, fToPercentage ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCProgressFromTo::initWithDuration ( KDfloat fDuration, KDfloat fFromPercentage, KDfloat fToPercentage )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_fTo = fToPercentage;
		m_fFrom = fFromPercentage;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCProgressFromTo::copyWithZone ( CCZone* pZone )
{
	CCZone*         pNewZone = KD_NULL;
	CCProgressFromTo*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCProgressFromTo *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCProgressFromTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_fFrom, m_fTo );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCActionInterval* CCProgressFromTo::reverse ( KDvoid )
{
	return CCProgressFromTo::create ( m_fDuration, m_fTo, m_fFrom );
}

KDvoid CCProgressFromTo::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );
}

KDvoid CCProgressFromTo::update ( KDfloat fTime )
{
	( (CCProgressTimer *) m_pTarget )->setPercentage ( m_fFrom + ( m_fTo - m_fFrom ) * fTime );
}

NS_CC_END