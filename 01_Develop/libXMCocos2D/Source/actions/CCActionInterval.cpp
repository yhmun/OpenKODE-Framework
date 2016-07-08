/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionInterval.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (C) 2008-2010 Ricardo Quesada
 *      Copyright (C) 2011      Zynga Inc.
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
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "cocoa/CCZone.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN
    
// Extra action for making a CCSequence or CCSpawn when only adding one action to it.
class ExtraAction : public CCFiniteTimeAction
{
	public :

		static ExtraAction*		create ( KDvoid );

	public :

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual ExtraAction*	reverse ( KDvoid );

		virtual KDvoid			update ( KDfloat fTime );

		virtual KDvoid			step ( KDfloat fDelta );
};

ExtraAction* ExtraAction::create ( KDvoid )
{
    ExtraAction*  pRet = new ExtraAction ( );
    if ( pRet )
    {
        pRet->autorelease ( );
    }
    return pRet;
}

CCObject* ExtraAction::copyWithZone ( CCZone* pZone )
{
    CC_UNUSED_PARAM ( pZone );
    ExtraAction*  pRet = new ExtraAction ( );
    return pRet;
}

ExtraAction* ExtraAction::reverse ( KDvoid )
{
    return ExtraAction::create ( );
}

KDvoid ExtraAction::update ( KDfloat fTime )
{
    CC_UNUSED_PARAM ( fTime );
}

KDvoid ExtraAction::step ( KDfloat fDelta )
{
    CC_UNUSED_PARAM ( fDelta );
}


//
// IntervalAction
//

CCActionInterval* CCActionInterval::create ( KDfloat fDuration )
{
	CCActionInterval*  pRet = new CCActionInterval ( );

	if ( pRet && pRet->initWithDuration ( fDuration ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCActionInterval::initWithDuration ( KDfloat fDuration )
{
	m_fDuration = fDuration;

	// prevent division by 0
	// This comparison could be in step:, but it might decrease the performance
	// by 3% in heavy based action games.
	if ( m_fDuration == 0 )
	{
		m_fDuration = KD_FLT_EPSILON;
	}

	m_fElapsed = 0;
	m_bFirstTick = KD_TRUE;

	return KD_TRUE;
}

CCObject* CCActionInterval::copyWithZone ( CCZone* pZone )
{
	CCZone*         pNewZone = KD_NULL;
	CCActionInterval*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCActionInterval*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCActionInterval ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCFiniteTimeAction::copyWithZone ( pZone );

	CC_SAFE_DELETE ( pNewZone );

	pCopy->initWithDuration ( m_fDuration );

	return pCopy;
}

KDbool CCActionInterval::isDone ( KDvoid )
{
	return m_fElapsed >= m_fDuration;
}

KDvoid CCActionInterval::step ( KDfloat fElapsed )
{
	if ( m_bFirstTick )
	{
		m_bFirstTick = KD_FALSE;
		m_fElapsed = 0;
	}
	else
	{
		m_fElapsed += fElapsed;
	}

	this->update ( KD_MAX ( 0, KD_MIN ( 1, m_fElapsed / KD_MAX ( m_fDuration, KD_FLT_EPSILON ) ) ) );
}

KDvoid CCActionInterval::setAmplitudeRate ( KDfloat fAmplitudeRate )
{
	CC_UNUSED_PARAM ( fAmplitudeRate );

	// Abstract class needs implementation
	CCAssert ( 0, "" );  
}

KDfloat CCActionInterval::getAmplitudeRate ( KDvoid )
{
	// Abstract class needs implementation
	CCAssert ( 0, "" );  

	return 0;
}

KDvoid CCActionInterval::startWithTarget ( CCNode* pTarget )
{
	CCFiniteTimeAction::startWithTarget ( pTarget );

	m_fElapsed = 0.0f;
	m_bFirstTick = KD_TRUE;
}

CCActionInterval* CCActionInterval::reverse ( KDvoid )
{
	CCAssert ( KD_FALSE, "CCIntervalAction: reverse not implemented." );
	return KD_NULL;
}


//
// Sequence
//

CCSequence* CCSequence::createWithTwoActions ( CCFiniteTimeAction* pAction1, CCFiniteTimeAction* pAction2 )
{
	CCSequence*  pRet = new CCSequence ( );

	if ( pRet && pRet->initWithTwoActions ( pAction1, pAction2 ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSequence* CCSequence::create ( CCFiniteTimeAction* pAction1, ... )
{
	KDVaListKHR       pArgs;
	KD_VA_START_KHR ( pArgs, pAction1 );

	CCSequence*  pRet = CCSequence::createWithVariableList ( pAction1, pArgs );

	KD_VA_END_KHR ( pArgs );

	return pRet;
}

CCSequence* CCSequence::createWithVariableList ( CCFiniteTimeAction* pAction1, KDVaListKHR pArgs )
{
	CCFiniteTimeAction*  pNow;
	CCFiniteTimeAction*  pPrev = pAction1;
	KDbool  bOneAction = KD_TRUE;

	while ( pAction1 )
	{
		pNow = KD_VA_ARG ( pArgs, CCFiniteTimeAction* );

		if ( pNow )
		{
			pPrev = CCSequence::createWithTwoActions ( pPrev, pNow );
			bOneAction = KD_FALSE;
		}
		else
		{
			// If only one action is added to CCSequence, make up a CCSequence by adding a simplest finite time action.
            if ( bOneAction )
			{
				pPrev = createWithTwoActions ( pPrev, ExtraAction::create ( ) );
			}
			break;
		}
	}

	return ( (CCSequence*) pPrev );
}

CCSequence* CCSequence::create ( CCArray* pArrayOfActions )
{
	CCSequence*  pRet = KD_NULL;
	
	do
	{
		KDuint  uCount = pArrayOfActions->count ( );
        CC_BREAK_IF ( uCount == 0 );

        CCFiniteTimeAction*  pPrev = (CCFiniteTimeAction*) pArrayOfActions->objectAtIndex ( 0 );

        if ( uCount > 1 )
        {
            for ( KDuint i = 1; i < uCount; ++i )
            {
                pPrev = createWithTwoActions ( pPrev, (CCFiniteTimeAction*) pArrayOfActions->objectAtIndex ( i ) );
            }
        }
        else
        {
            // If only one action is added to CCSequence, make up a CCSequence by adding a simplest finite time action.
            pPrev = createWithTwoActions ( pPrev, ExtraAction::create ( ) );
        }

        pRet = (CCSequence*) pPrev;

	} while ( 0 );

	return pRet;
}

KDbool CCSequence::initWithTwoActions ( CCFiniteTimeAction* pAction1, CCFiniteTimeAction* pAction2 )
{
	CCAssert ( pAction1 != KD_NULL, "" );
	CCAssert ( pAction2 != KD_NULL, "" );

	KDfloat  fDuration = pAction1->getDuration ( ) + pAction2->getDuration ( );
	CCActionInterval::initWithDuration ( fDuration );

	m_pActions [ 0 ] = pAction1;
    pAction1->retain ( );

	m_pActions [ 1 ] = pAction2;
    pAction2->retain ( );
    
	return KD_TRUE;
}

CCObject* CCSequence::copyWithZone ( CCZone* pZone )
{
	CCZone*   pNewZone = KD_NULL;
	CCSequence*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCSequence *) ( pZone->m_pCopyObject );
	}
	else
	{
		pCopy = new CCSequence ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithTwoActions ( (CCFiniteTimeAction *) ( m_pActions [ 0 ]->copy ( )->autorelease ( ) ), 
				                (CCFiniteTimeAction *) ( m_pActions [ 1 ]->copy ( )->autorelease ( ) ) );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCSequence::~CCSequence ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pActions [ 0 ] );
	CC_SAFE_RELEASE ( m_pActions [ 1 ] );
}

KDvoid CCSequence::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_fSplit = m_pActions [ 0 ]->getDuration ( ) / m_fDuration;
	m_nLast  = -1;
}

KDvoid CCSequence::stop ( KDvoid )
{
    // Issue #1305
    if ( m_nLast != - 1 )
    {
        m_pActions [ m_nLast ]->stop ( );
    }
    
	CCActionInterval::stop ( );
}

KDvoid CCSequence::update ( KDfloat fTime )
{
	KDint   nFound = 0;
	KDfloat  fNewTime = 0.0f;

	if ( fTime < m_fSplit )
	{
		// action[0]
		nFound = 0;
		if ( m_fSplit != 0 )
		{
			fNewTime = fTime / m_fSplit;
		}
		else
		{
			fNewTime = 1;
		}
	}
	else
	{
		// action[1]
		nFound = 1;
		if ( m_fSplit == 1 )
		{
			fNewTime = 1;
		}
		else
		{
			fNewTime = ( fTime - m_fSplit ) / ( 1 - m_fSplit );
		}
	}

	if ( nFound == 1 )
	{
		if ( m_nLast == -1 )
		{			
			// action[0] was skipped, execute it.
			m_pActions [ 0 ]->startWithTarget ( m_pTarget );
			m_pActions [ 0 ]->update ( 1.0f );
			m_pActions [ 0 ]->stop ( );
		}
		else if ( m_nLast == 0 )
		{
			// switching to action 1. stop action 0.
			m_pActions [ 0 ]->update ( 1.0f );
			m_pActions [ 0 ]->stop ( );
		}
	}
	else if ( nFound == 0 && m_nLast == 1 )
	{
		// Reverse mode ?
		// XXX: Bug. this case doesn't contemplate when _last==-1, found=0 and in "reverse mode"
		// since it will require a hack to know if an action is on reverse mode or not.
		// "step" should be overriden, and the "reverseMode" value propagated to inner Sequences.
		m_pActions [ 1 ]->update ( 0 );
		m_pActions [ 1 ]->stop ( );
	}

	// Last action found and it is done.
    if( nFound == m_nLast && m_pActions [ nFound ]->isDone ( ) )
    {
        return;
    }

	// Last action found and it is done
    if ( nFound != m_nLast )
    {
        m_pActions [ nFound ]->startWithTarget ( m_pTarget );
    }

	m_pActions [ nFound ]->update ( fNewTime );
	m_nLast = nFound;
}

CCActionInterval* CCSequence::reverse ( KDvoid )
{
	return CCSequence::createWithTwoActions ( m_pActions [ 1 ]->reverse ( ), m_pActions [ 0 ]->reverse ( ) );
}


//
// Repeat
//

CCRepeat* CCRepeat::create ( CCFiniteTimeAction* pAction, KDuint uTimes )
{
	CCRepeat*  pRet = new CCRepeat ( );

	if ( pRet && pRet->initWithAction ( pAction, uTimes ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCRepeat::initWithAction ( CCFiniteTimeAction* pAction, KDuint uTimes )
{
	KDfloat  fDuration = pAction->getDuration ( ) * uTimes;

	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_uTimes = uTimes;
		m_pInnerAction = pAction;
		pAction->retain ( );

		m_bActionInstant = dynamic_cast<CCActionInstant*> ( pAction ) ? KD_TRUE : KD_FALSE;
        // an instant action needs to be executed one time less in the update method since it uses startWithTarget to execute the action
        if ( m_bActionInstant ) 
        {
            m_uTimes -=1;
        }
		m_uTotal = 0;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCRepeat::copyWithZone ( CCZone* pZone )
{	
	CCZone*  pNewZone = KD_NULL;
	CCRepeat*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCRepeat*) ( pZone->m_pCopyObject );
	}
	else
	{
		pCopy = new CCRepeat ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithAction ( (CCFiniteTimeAction*) ( m_pInnerAction->copy ( )->autorelease ( ) ), m_uTimes );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCRepeat::~CCRepeat ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pInnerAction );
}

KDvoid CCRepeat::startWithTarget ( CCNode* pTarget )
{
	m_uTotal  = 0;
	m_fNextDt = m_pInnerAction->getDuration ( ) / m_fDuration;

	CCActionInterval::startWithTarget ( pTarget );
	m_pInnerAction->startWithTarget ( pTarget );
}

KDvoid CCRepeat::stop ( KDvoid )
{
	m_pInnerAction->stop ( );

	CCActionInterval::stop ( );
}

// issue #80. Instead of hooking step:, hook update: since it can be called by any 
// container action like CCRepeat, CCSequence, CCEase, etc..
KDvoid CCRepeat::update ( KDfloat fTime )
{
    if ( fTime >= m_fNextDt)
    {
        while ( fTime > m_fNextDt && m_uTotal < m_uTimes )
        {

            m_pInnerAction->update ( 1.0f );
            m_uTotal++;

            m_pInnerAction->stop ( );
            m_pInnerAction->startWithTarget ( m_pTarget );
            m_fNextDt += m_pInnerAction->getDuration ( ) / m_fDuration;
        }

        // fix for issue #1288, incorrect end value of repeat
        if ( fTime >= 1.0f && m_uTotal < m_uTimes ) 
        {
            m_uTotal++;
        }

        // don't set an instant action back or update it, it has no use because it has no duration
        if ( !m_bActionInstant )
        {
            if ( m_uTotal == m_uTimes )
            {
                m_pInnerAction->update ( 1 );
                m_pInnerAction->stop ( );
            }
            else
            {
                // issue #390 prevent jerk, use right update
                m_pInnerAction->update ( fTime - ( m_fNextDt - m_pInnerAction->getDuration ( ) / m_fDuration ) );
            }
        }
    }
    else
    {
        m_pInnerAction->update ( kdFmodf ( fTime * m_uTimes, 1.0f ) );
    }
}

KDbool CCRepeat::isDone ( KDvoid )
{
	return m_uTotal == m_uTimes;
}

CCActionInterval* CCRepeat::reverse ( KDvoid )
{
	return CCRepeat::create ( m_pInnerAction->reverse ( ), m_uTimes );
}


//
// RepeatForever
//
CCRepeatForever::CCRepeatForever ( KDvoid )
{
    m_pInnerAction = KD_NULL;
}

CCRepeatForever::~CCRepeatForever ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pInnerAction );
}

CCRepeatForever* CCRepeatForever::create ( CCActionInterval* pAction )
{
	CCRepeatForever*  pRet = new CCRepeatForever ( );

	if ( pRet && pRet->initWithAction ( pAction ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCRepeatForever::initWithAction ( CCActionInterval* pAction )
{
	CCAssert ( pAction != KD_NULL, "" );

	pAction->retain ( );

	m_pInnerAction = pAction;

	return KD_TRUE;
}

CCObject* CCRepeatForever::copyWithZone ( CCZone* pZone )
{
	CCZone*       pNewZone = KD_NULL;
	CCRepeatForever*  pRet = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) // in case of being called at sub class
	{
		pRet = (CCRepeatForever*) ( pZone->m_pCopyObject );
	}
	else
	{
		pRet  = new CCRepeatForever ( );
		pZone = pNewZone = new CCZone ( pRet );
	}
    
	CCActionInterval::copyWithZone ( pZone );

	// win32 : use the m_pInnerAction's copy object.
	pRet->initWithAction ( (CCActionInterval*) ( m_pInnerAction->copy ( )->autorelease ( ) ) ); 	
	CC_SAFE_DELETE ( pNewZone );

	return pRet;
}

KDvoid CCRepeatForever::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_pInnerAction->startWithTarget ( pTarget );
}

KDvoid CCRepeatForever::step ( KDfloat fElapsed )
{
	m_pInnerAction->step ( fElapsed );

	if ( m_pInnerAction->isDone ( ) )
	{
		KDfloat  fDiff = m_pInnerAction->getElapsed ( ) - m_pInnerAction->getDuration ( );

		m_pInnerAction->startWithTarget ( m_pTarget );

		// to prevent jerk. issue #390, 1247
		m_pInnerAction->step ( 0.0f );
		m_pInnerAction->step ( fDiff );
	}
}

KDbool CCRepeatForever::isDone ( KDvoid )
{
	return KD_FALSE;
}

CCActionInterval* CCRepeatForever::reverse ( KDvoid )
{
	return (CCActionInterval*) ( CCRepeatForever::create ( m_pInnerAction->reverse ( ) ) );
}


//
// Spawn
//

CCSpawn* CCSpawn::create ( CCFiniteTimeAction* pAction1, ... )
{
	KDVaListKHR       pArgs;
	KD_VA_START_KHR ( pArgs, pAction1 );
	
	CCSpawn*  pRet = CCSpawn::createWithVariableList ( pAction1, pArgs );

	KD_VA_END_KHR ( pArgs );

	return pRet;
}

CCSpawn* CCSpawn::createWithVariableList ( CCFiniteTimeAction* pAction1, KDVaListKHR pArgs )
{
	CCFiniteTimeAction*  pNow;
	CCFiniteTimeAction*  pPrev = pAction1;
    KDbool  bOneAction = KD_TRUE;

    while ( pAction1 )
    {
        pNow = KD_VA_ARG ( pArgs, CCFiniteTimeAction* );
        if ( pNow )
        {
            pPrev = createWithTwoActions ( pPrev, pNow );
            bOneAction = KD_FALSE;
        }
        else
        {
            // If only one action is added to CCSpawn, make up a CCSpawn by adding a simplest finite time action.
            if ( bOneAction )
            {
                pPrev = createWithTwoActions ( pPrev, ExtraAction::create ( ) );
            }
            break;
        }
    }

    return ( (CCSpawn*) pPrev );
}

CCSpawn* CCSpawn::create ( CCArray* pArrayOfActions )
{
    CCSpawn*  pRet = KD_NULL;
    do 
    {
        KDuint  uCount = pArrayOfActions->count ( );
        CC_BREAK_IF ( uCount == 0 );
        CCFiniteTimeAction*  pPrev = (CCFiniteTimeAction*) pArrayOfActions->objectAtIndex ( 0 );
        if ( uCount > 1 )
        {
            for ( KDuint i = 1; i < pArrayOfActions->count ( ); ++i )
            {
                pPrev = createWithTwoActions ( pPrev, (CCFiniteTimeAction*) pArrayOfActions->objectAtIndex ( i ) );
            }
        }
        else
        {
            // If only one action is added to CCSpawn, make up a CCSpawn by adding a simplest finite time action.
            pPrev = createWithTwoActions ( pPrev, ExtraAction::create ( ) );
        }

        pRet = (CCSpawn*) pPrev;

    } while ( 0 );

    return pRet;
}

CCSpawn* CCSpawn::createWithTwoActions ( CCFiniteTimeAction* pAction1, CCFiniteTimeAction* pAction2 )
{
	CCSpawn*  pRet = new CCSpawn ( );

	if ( pRet && pRet->initWithTwoActions ( pAction1, pAction2 ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCSpawn::initWithTwoActions ( CCFiniteTimeAction* pAction1, CCFiniteTimeAction* pAction2 )
{
	CCAssert ( pAction1 != KD_NULL, "" );
	CCAssert ( pAction2 != KD_NULL, "" );

	KDfloat  fDuration1 = pAction1->getDuration ( );
	KDfloat  fDuration2 = pAction2->getDuration ( );

	if ( CCActionInterval::initWithDuration ( KD_MAX ( fDuration1, fDuration2 ) ) )
	{
		m_pAction1 = pAction1;
		m_pAction2 = pAction2;

		if ( fDuration1 > fDuration2 )
		{
			m_pAction2 = CCSequence::createWithTwoActions ( pAction2, CCDelayTime::create ( fDuration1 - fDuration2 ) );
		} 
		else if ( fDuration1 < fDuration2 )
		{
			m_pAction1 = CCSequence::createWithTwoActions ( pAction1, CCDelayTime::create ( fDuration2 - fDuration1 ) );
		}

		m_pAction1->retain ( );
		m_pAction2->retain ( );

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCSpawn::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCSpawn*    pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCSpawn*) ( pZone->m_pCopyObject );
	}
	else
	{
		pCopy = new CCSpawn ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithTwoActions ( (CCFiniteTimeAction *) ( m_pAction1->copy ( )->autorelease ( ) ), 
					            (CCFiniteTimeAction *) ( m_pAction2->copy ( )->autorelease ( ) ) );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCSpawn::~CCSpawn ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pAction1 );
	CC_SAFE_RELEASE ( m_pAction2 );
}

KDvoid CCSpawn::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_pAction1->startWithTarget ( pTarget );
	m_pAction2->startWithTarget ( pTarget );
}

KDvoid CCSpawn::stop ( KDvoid )
{
	m_pAction1->stop ( );
	m_pAction2->stop ( );

	CCActionInterval::stop ( );
}

KDvoid CCSpawn::update ( KDfloat fTime )
{
	if ( m_pAction1 )
	{
		m_pAction1->update ( fTime );
	}

	if ( m_pAction2 )
	{
		m_pAction2->update ( fTime );
	}
}

CCActionInterval* CCSpawn::reverse ( KDvoid )
{
	return CCSpawn::createWithTwoActions ( m_pAction1->reverse ( ), m_pAction2->reverse ( ) );
}

//
// RotateTo
//

CCRotateTo* CCRotateTo::create ( KDfloat fDuration, KDfloat fDeltaAngle )
{
	CCRotateTo*  pRet = new CCRotateTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fDeltaAngle ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCRotateTo* CCRotateTo::create ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY )
{
	CCRotateTo*  pRet = new CCRotateTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fDeltaAngleX, fDeltaAngleY ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCRotateTo::initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngle )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tEndAngle.x = m_tEndAngle.y = fDeltaAngle;
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDbool CCRotateTo::initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tEndAngle.x = fDeltaAngleX;
		m_tEndAngle.y = fDeltaAngleY;
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCRotateTo::copyWithZone ( CCZone* pZone )
{
	CCZone*   pNewZone = KD_NULL;
	CCRotateTo*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		// in case of being called at sub class
		pCopy = (CCRotateTo*) ( pZone->m_pCopyObject );
	}
	else
	{
		pCopy = new CCRotateTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tEndAngle.x, m_tEndAngle.y );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCRotateTo::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	// Calculate X
	m_tStartAngle.x = pTarget->getRotationX ( );
    if ( m_tStartAngle.x > 0 )
    {
        m_tStartAngle.x = kdFmodf ( m_tStartAngle.x, 360.0f );
    }
	else
	{
		m_tStartAngle.x = kdFmodf ( m_tStartAngle.x, -360.0f );
	}

    m_tDiffAngle.x = m_tEndAngle.x - m_tStartAngle.x;
    if ( m_tStartAngle.x > 180 )
    {
        m_tStartAngle.x -= 360;
    }
    if ( m_tStartAngle.x < -180 )
    {
        m_tStartAngle.x += 360;
    }

	//Calculate Y: It's duplicated from calculating X since the rotation wrap should be the same
	m_tStartAngle.y = m_pTarget->getRotationY ( );
	if ( m_tStartAngle.y > 0 )
    {
        m_tStartAngle.y = kdFmodf ( m_tStartAngle.y, 360.0f );
    }
    else
    {
        m_tStartAngle.y = kdFmodf ( m_tStartAngle.y, -360.0f );
    }

	m_tDiffAngle.y = m_tEndAngle.y - m_tStartAngle.y;
	if ( m_tDiffAngle.y > 180 )
	{
		m_tDiffAngle.y -= 360;
	}
	if ( m_tDiffAngle.y < -180 )
	{
		m_tDiffAngle.y += 360;
	}
}

KDvoid CCRotateTo::update ( KDfloat fTime )
{
	if ( m_pTarget )
	{
		m_pTarget->setRotationX ( m_tStartAngle.x + m_tDiffAngle.x * fTime );
		m_pTarget->setRotationY ( m_tStartAngle.y + m_tDiffAngle.y * fTime );
	}
}


//
// RotateBy
//

CCRotateBy* CCRotateBy::create ( KDfloat fDuration, KDfloat fDeltaAngle )
{
	CCRotateBy*  pRet = new CCRotateBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fDeltaAngle ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCRotateBy* CCRotateBy::create ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY )
{
	CCRotateBy*  pRet = new CCRotateBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fDeltaAngleX, fDeltaAngleY ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCRotateBy::initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngle )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tAngle.x = m_tAngle.y = fDeltaAngle;
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDbool CCRotateBy::initWithDuration ( KDfloat fDuration, KDfloat fDeltaAngleX, KDfloat fDeltaAngleY )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tAngle.x = fDeltaAngleX;
		m_tAngle.y = fDeltaAngleY;
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCRotateBy::copyWithZone ( CCZone* pZone )
{
	CCZone*   pNewZone = KD_NULL;
	CCRotateBy*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCRotateBy*) ( pZone->m_pCopyObject );
	}
	else
	{
		pCopy = new CCRotateBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tAngle.x, m_tAngle.y );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCRotateBy::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_tStartAngle.x = pTarget->getRotationX ( );
    m_tStartAngle.y = pTarget->getRotationY ( );
}

KDvoid CCRotateBy::update ( KDfloat fTime )
{
	if ( m_pTarget )
	{
		m_pTarget->setRotationX ( m_tStartAngle.x + m_tAngle.x * fTime );
        m_pTarget->setRotationY ( m_tStartAngle.y + m_tAngle.y * fTime );
	}
}

CCActionInterval* CCRotateBy::reverse ( KDvoid )
{
	return CCRotateBy::create ( m_fDuration, -m_tAngle.x, -m_tAngle.y );
}


//
// MoveBy
//

CCMoveBy* CCMoveBy::create ( KDfloat fDuration, const CCPoint& tDeltaPosition )
{
	CCMoveBy*  pRet = new CCMoveBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tDeltaPosition ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCMoveBy::initWithDuration ( KDfloat fDuration, const CCPoint& tDeltaPosition )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tPositionDelta = tDeltaPosition;
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCMoveBy::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCMoveBy*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCMoveBy*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCMoveBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tPositionDelta );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCMoveBy::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_tPreviousPosition = m_tStartPosition = pTarget->getPosition ( );
}

CCActionInterval* CCMoveBy::reverse ( KDvoid )
{
    return CCMoveBy::create ( m_fDuration, ccp ( -m_tPositionDelta.x, -m_tPositionDelta.y ) );
}

KDvoid CCMoveBy::update ( KDfloat fTime )
{
	if ( m_pTarget )
	{
	#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint  tCurrentPos = m_pTarget->getPosition ( );
        CCPoint  tDiff = ccpSub ( tCurrentPos, m_tPreviousPosition );
        m_tStartPosition = ccpAdd ( m_tStartPosition, tDiff );
        CCPoint  tNewPos =  ccpAdd ( m_tStartPosition, ccpMult ( m_tPositionDelta, fTime ) );
        m_pTarget->setPosition ( tNewPos );
        m_tPreviousPosition = tNewPos;
	#else
        m_pTarget->setPosition ( ccpAdd ( m_tStartPosition, ccpMult ( m_tPositionDelta, fTime ) ) );
	#endif	// CC_ENABLE_STACKABLE_ACTIONS
	}
}


//
// MoveTo
//

CCMoveTo* CCMoveTo::create ( KDfloat fDuration, const CCPoint& tPosition )
{
	CCMoveTo*  pRet = new CCMoveTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tPosition ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCMoveTo::initWithDuration ( KDfloat fDuration, const CCPoint& tPosition )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tEndPosition = tPosition;
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCMoveTo::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCMoveTo*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCMoveTo*) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCMoveTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCMoveBy::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tEndPosition );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCMoveTo::startWithTarget ( CCNode* pTarget )
{
	CCMoveBy::startWithTarget ( pTarget );	
	m_tPositionDelta = ccpSub ( m_tEndPosition, pTarget->getPosition ( ) );
}


//
// CCSkewTo
//

CCSkewTo* CCSkewTo::create ( KDfloat fDuration, KDfloat fSkewX, KDfloat fSkewY )
{
	CCSkewTo*  pRet = new CCSkewTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fSkewX, fSkewY ) )
	{		
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCSkewTo::initWithDuration ( KDfloat fDuration, KDfloat fSkewX, KDfloat fSkewY )
{
	KDbool  bRet = KD_FALSE;

	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tEndSkew.x = fSkewX;
		m_tEndSkew.y = fSkewY;

		bRet = KD_TRUE;
	}

	return bRet;
}

CCObject* CCSkewTo::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCSkewTo*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCSkewTo *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCSkewTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tEndSkew.x, m_tEndSkew.y );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCSkewTo::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_tStartSkew.x = pTarget->getSkewX ( );

	if ( m_tStartSkew.x > 0 )
	{
		m_tStartSkew.x = kdFmodf ( m_tStartSkew.x, 180.f );
	}
	else
	{
		m_tStartSkew.x = kdFmodf ( m_tStartSkew.x, -180.f );
	}

	m_tDelta.x = m_tEndSkew.x - m_tStartSkew.x;

	if ( m_tDelta.x > 180 )
	{
		m_tDelta.x -= 360;
	}

	if ( m_tDelta.x < -180 )
	{
		m_tDelta.x += 360;
	}

	m_tStartSkew.y = pTarget->getSkewY ( );

	if ( m_tStartSkew.y > 0 )
	{
		m_tStartSkew.y = kdFmodf ( m_tStartSkew.y, 360.f );
	}
	else
	{
		m_tStartSkew.y = kdFmodf ( m_tStartSkew.y, -360.f );
	}

	m_tDelta.y = m_tEndSkew.y - m_tStartSkew.y;

	if ( m_tDelta.y > 180 )
	{
		m_tDelta.y -= 360;
	}

	if ( m_tDelta.y < -180 )
	{
		m_tDelta.y += 360;
	}
}

KDvoid CCSkewTo::update ( KDfloat fTime )
{
	m_pTarget->setSkewX ( m_tStartSkew.x + m_tDelta.x * fTime );
	m_pTarget->setSkewY ( m_tStartSkew.y + m_tDelta.y * fTime );
}

CCSkewTo::CCSkewTo ( KDvoid )
{
    m_tSkew       = CCPointZero;
    m_tStartSkew  = CCPointZero;
    m_tEndSkew    = CCPointZero;
    m_tDelta      = CCPointZero;
}


//
// CCSkewBy
//

CCSkewBy* CCSkewBy::create ( KDfloat fDuration, KDfloat fDeltaSkewX, KDfloat fDeltaSkewY )
{
	CCSkewBy*  pRet = new CCSkewBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fDeltaSkewX, fDeltaSkewY ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}	

	return pRet;
}

KDbool CCSkewBy::initWithDuration ( KDfloat t, KDfloat fDeltaSkewX, KDfloat fDeltaSkewY )
{
	KDbool  bRet = KD_FALSE;

	if ( CCSkewTo::initWithDuration ( t, fDeltaSkewX, fDeltaSkewY ) )
	{
		m_tSkew.x = fDeltaSkewX;
		m_tSkew.y = fDeltaSkewY;

		bRet = KD_TRUE;
	}

	return bRet;
}

KDvoid CCSkewBy::startWithTarget ( CCNode* pTarget )
{
	CCSkewTo::startWithTarget ( pTarget );

	m_tDelta.x = m_tSkew.x;
	m_tDelta.y = m_tSkew.y;

	m_tEndSkew.x = m_tStartSkew.x + m_tDelta.x;
	m_tEndSkew.y = m_tStartSkew.y + m_tDelta.y;
}

CCActionInterval* CCSkewBy::reverse ( KDvoid )
{
	return CCSkewBy::create ( m_fDuration, -m_tSkew.x, -m_tSkew.y );
}


//
// JumpBy
//

CCJumpBy* CCJumpBy::create ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDuint uJumps )
{
	CCJumpBy*  pRet = new CCJumpBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tPosition, fHeight, uJumps ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCJumpBy::initWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDuint uJumps )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_tDelta  = tPosition;
		m_fHeight = fHeight;
		m_uJumps  = uJumps;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCJumpBy::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCJumpBy*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCJumpBy *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCJumpBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tDelta, m_fHeight, m_uJumps );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCJumpBy::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );
	
	m_tPreviousPos = m_tStartPosition = pTarget->getPosition ( );
}

KDvoid CCJumpBy::update ( KDfloat fTime )
{
	// parabolic jump (since v0.8.2)
	if ( m_pTarget )
	{
		KDfloat  fFrac = kdFmodf ( fTime * m_uJumps, 1.0f );
		KDfloat     fY = m_fHeight * 4 * fFrac * ( 1 - fFrac );

		fY += m_tDelta.y * fTime;
		KDfloat     fX = m_tDelta.x * fTime;
	#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint  tCurrentPos = m_pTarget->getPosition();
        CCPoint  tDiff   = ccpSub ( tCurrentPos, m_tPreviousPos );
        m_tStartPosition = ccpAdd ( tDiff, m_tStartPosition );

        CCPoint  tNewPos = ccpAdd ( m_tStartPosition, ccp ( fX, fY ) );
        m_pTarget->setPosition ( tNewPos );

        m_tPreviousPos = tNewPos;
	#else
        m_pTarget->setPosition ( ccpAdd ( m_tStartPosition, ccp ( fX, fY ) ) );
	#endif	// !CC_ENABLE_STACKABLE_ACTIONS
	}
}

CCActionInterval* CCJumpBy::reverse ( KDvoid )
{
	return CCJumpBy::create ( m_fDuration, ccp ( -m_tDelta.x, -m_tDelta.y ), m_fHeight, m_uJumps );
}

//
// JumpTo
//

CCJumpTo* CCJumpTo::create ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDint uJumps )
{
	CCJumpTo*  pRet = new CCJumpTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tPosition, fHeight, uJumps ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCObject* CCJumpTo::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCJumpTo*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		// in case of being called at sub class
		pCopy = (CCJumpTo *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCJumpTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCJumpBy::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tDelta, m_fHeight, m_uJumps );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCJumpTo::startWithTarget ( CCNode* pTarget )
{
	CCJumpBy::startWithTarget ( pTarget );
	m_tDelta = ccp ( m_tDelta.x - m_tStartPosition.x, m_tDelta.y - m_tStartPosition.y );
}

// Bezier cubic formula:
//	((1 - t) + t)3 = 1 
// Expands to¡­ 
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1 
static inline KDfloat bezierat ( KDfloat a, KDfloat b, KDfloat c, KDfloat d, KDfloat t )
{
	return ( kdPowf ( 1 - t, 3 ) * a + 
			 3 * t * ( kdPowf ( 1 - t, 2 ) ) * b + 
			 3 * kdPowf ( t, 2 ) * ( 1 - t ) * c +
			 kdPowf ( t, 3 ) * d );
}


//
// CCJumpY
//

CCJumpY* CCJumpY::create ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDint uJumps, KDfloat* pGroundPlane )
{
	CCJumpY*  pRet = new CCJumpY ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tPosition, fHeight, uJumps, pGroundPlane ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCJumpY::initWithDuration ( KDfloat fDuration, const CCPoint& tPosition, KDfloat fHeight, KDuint uJumps, KDfloat* pGroundPlane )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_tDelta  = tPosition;
		m_fHeight = fHeight;
		m_uJumps  = uJumps;
		m_pGroundPlane = pGroundPlane;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCJumpY::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCJumpY*    pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCJumpY *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCJumpY ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tDelta, m_fHeight, m_uJumps, m_pGroundPlane );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCJumpY::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );
	
	m_tStartPosition = pTarget->getPosition ( );
	m_pParent = pTarget->getParent ( );
}

KDvoid CCJumpY::update ( KDfloat fTime )
{
	// Sin jump. Less realistic
    //	ccTime y = height * fabsf( sinf(t * (CGFloat)M_PI * jumps ) );
    //	y += delta.y * t;
    //	ccTime x = delta.x * t;
    //	[target setPosition: ccp( startPosition.x + x, startPosition.y + y )];
    
	// parabolic jump (since v0.8.2)
	KDfloat		fFrac = kdFmodf ( fTime * m_uJumps, 1.0f );
	KDfloat		fY = m_fHeight * 4 * fFrac * ( 1 - fFrac );
    KDfloat		fPx = ( (CCNode*) m_pTarget )->getPosition ( ).x;
    KDfloat		fPy = m_tStartPosition.y + fY;
    if ( fTime > 0.5f ) 
	{
        KDfloat		fDipingmian = *( m_pGroundPlane + (KDint) fPx );
        if ( fPy < fDipingmian )
		{
			fPy = fDipingmian;
		}
    }
	m_pTarget->setPosition ( ccp ( fPx, fPy ) );
}

CCActionInterval* CCJumpY::reverse ( KDvoid )
{
	return CCJumpY::create ( m_fDuration, ccp ( -m_tDelta.x, -m_tDelta.y ), m_fHeight, m_uJumps, m_pGroundPlane );
}

//
// BezierBy
//

CCBezierBy* CCBezierBy::create ( KDfloat fDuration, const ccBezierConfig& tBezierConfig )
{
	CCBezierBy*  pRet = new CCBezierBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tBezierConfig ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCBezierBy::initWithDuration ( KDfloat fDuration, const ccBezierConfig& tBezierConfig )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_tBezierConfig = tBezierConfig;
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CCBezierBy::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_tPreviousPosition = m_tStartPosition = pTarget->getPosition ( );
}

CCObject* CCBezierBy::copyWithZone ( CCZone* pZone )
{
	CCZone*   pNewZone = KD_NULL;
	CCBezierBy*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCBezierBy *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCBezierBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tBezierConfig );
    
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCBezierBy::update ( KDfloat fTime )
{
	if ( m_pTarget )
	{
		KDfloat xa = 0;
		KDfloat xb = m_tBezierConfig.tControlPoint1.x;
		KDfloat xc = m_tBezierConfig.tControlPoint2.x;
		KDfloat xd = m_tBezierConfig.tEndPosition.x;

		KDfloat ya = 0;
		KDfloat yb = m_tBezierConfig.tControlPoint1.y;
		KDfloat yc = m_tBezierConfig.tControlPoint2.y;
		KDfloat yd = m_tBezierConfig.tEndPosition.y;

		KDfloat x = bezierat ( xa, xb, xc, xd, fTime );
		KDfloat y = bezierat ( ya, yb, yc, yd, fTime );

	#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint  tCurrentPos = m_pTarget->getPosition ( );
        CCPoint  tDiff = ccpSub ( tCurrentPos, m_tPreviousPosition );
        m_tStartPosition = ccpAdd ( m_tStartPosition, tDiff );

        CCPoint  tNewPos = ccpAdd ( m_tStartPosition, ccp ( x, y ) );
        m_pTarget->setPosition ( tNewPos );

        m_tPreviousPosition = tNewPos;
	#else
        m_pTarget->setPosition ( ccpAdd ( m_tStartPosition, ccp ( x, y ) ) );
	#endif	// !CC_ENABLE_STACKABLE_ACTIONS
	}
}

CCActionInterval* CCBezierBy::reverse ( KDvoid )
{
	ccBezierConfig  tReverse;

	tReverse.tEndPosition   = ccpNeg ( m_tBezierConfig.tEndPosition );
	tReverse.tControlPoint1 = ccpAdd ( m_tBezierConfig.tControlPoint2, ccpNeg ( m_tBezierConfig.tEndPosition ) );
	tReverse.tControlPoint2 = ccpAdd ( m_tBezierConfig.tControlPoint1, ccpNeg ( m_tBezierConfig.tEndPosition ) );

	CCBezierBy*  pAction = CCBezierBy::create ( m_fDuration, tReverse );

	return pAction;
}


//
// BezierTo
//

CCBezierTo* CCBezierTo::create ( KDfloat fDuration, const ccBezierConfig& tBezierConfig )
{
	CCBezierTo*  pRet = new CCBezierTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tBezierConfig ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet ); 
	}

	return pRet;
}

KDbool CCBezierTo::initWithDuration ( KDfloat fDuration, const ccBezierConfig& tBezierConfig )
{    
    if ( CCActionInterval::initWithDuration ( fDuration ) )
    {
        m_tBezierToConfig = tBezierConfig;
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCBezierTo::copyWithZone ( CCZone* pZone )
{
	CCZone*   pNewZone = KD_NULL;
	CCBezierBy*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCBezierTo *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCBezierTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCBezierBy::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tBezierConfig );
    
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCBezierTo::startWithTarget ( CCNode* pTarget )
{
	CCBezierBy::startWithTarget ( pTarget );

	m_tBezierConfig.tControlPoint1 = ccpSub ( m_tBezierToConfig.tControlPoint1, m_tStartPosition );
	m_tBezierConfig.tControlPoint2 = ccpSub ( m_tBezierToConfig.tControlPoint2, m_tStartPosition );
	m_tBezierConfig.tEndPosition   = ccpSub ( m_tBezierToConfig.tEndPosition  , m_tStartPosition );
}


//
// ScaleTo
//

CCScaleTo* CCScaleTo::create ( KDfloat fDuration, KDfloat fScale )
{
	CCScaleTo*  pRet = new CCScaleTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fScale ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCScaleTo::initWithDuration ( KDfloat fDuration, KDfloat fScale )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_tEndScale.x = fScale;
		m_tEndScale.y = fScale;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCScaleTo* CCScaleTo::create ( KDfloat fDuration, KDfloat fScaleX, KDfloat fScaleY )
{
	CCScaleTo*  pRet = new CCScaleTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fScaleX, fScaleY ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCScaleTo::initWithDuration ( KDfloat fDuration, KDfloat fScaleX, KDfloat fScaleY )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
		m_tEndScale.x = fScaleX;
		m_tEndScale.y = fScaleY;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCScaleTo::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCScaleTo*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCScaleTo *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCScaleTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tEndScale.x, m_tEndScale.y );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCScaleTo::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_tStartScale.x = pTarget->getScaleX ( );
	m_tStartScale.y = pTarget->getScaleY ( );

    m_tDelta.x = m_tEndScale.x - m_tStartScale.x;
	m_tDelta.y = m_tEndScale.y - m_tStartScale.y;
}

KDvoid CCScaleTo::update ( KDfloat fTime )
{
	if ( m_pTarget )
	{
		m_pTarget->setScaleX ( m_tStartScale.x + m_tDelta.x * fTime );
		m_pTarget->setScaleY ( m_tStartScale.y + m_tDelta.y * fTime );
	}
}


//
// ScaleBy
//

CCScaleBy* CCScaleBy::create ( KDfloat fDuration, KDfloat fScale )
{
	CCScaleBy*  pRet = new CCScaleBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fScale ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCScaleBy* CCScaleBy::create ( KDfloat fDuration, KDfloat fScaleX, KDfloat fScaleY )
{
	CCScaleBy*  pRet = new CCScaleBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, fScaleX, fScaleY ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCObject* CCScaleBy::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCScaleTo*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		// in case of being called at sub class
		pCopy = (CCScaleBy *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCScaleBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCScaleTo::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tEndScale.x, m_tEndScale.y );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCScaleBy::startWithTarget ( CCNode* pTarget )
{
	CCScaleTo::startWithTarget ( pTarget );

	m_tDelta.x = m_tStartScale.x * m_tEndScale.x - m_tStartScale.x;
	m_tDelta.y = m_tStartScale.y * m_tEndScale.y - m_tStartScale.y;
}

CCActionInterval* CCScaleBy::reverse ( KDvoid )
{
	return CCScaleBy::create ( m_fDuration, 1 / m_tEndScale.x, 1 / m_tEndScale.y );
}


//
// Blink
//

CCBlink* CCBlink::create ( KDfloat fDuration, KDuint uBlinks )
{
	CCBlink*  pRet = new CCBlink ( );

	if ( pRet && pRet->initWithDuration ( fDuration, uBlinks ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCBlink::initWithDuration ( KDfloat fDuration, KDuint uBlinks )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_uBlinks = uBlinks;
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid CCBlink::stop ( KDvoid )
{
    m_pTarget->setVisible ( m_bOriginalState );
    CCActionInterval::stop ( );
}

KDvoid CCBlink::startWithTarget ( CCNode* pTarget )
{
    CCActionInterval::startWithTarget ( pTarget );
    m_bOriginalState = pTarget->isVisible ( );
}

CCObject* CCBlink::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCBlink*    pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCBlink *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCBlink ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_uBlinks );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCBlink::update ( KDfloat fTime )
{
	if ( m_pTarget && ! isDone ( ) )
	{
		KDfloat fSlice = 1.0f / m_uBlinks;
		KDfloat     fM = kdFmodf ( fTime, fSlice );

		m_pTarget->setVisible ( fM > fSlice / 2 ? KD_TRUE : KD_FALSE );
	}
}

CCActionInterval* CCBlink::reverse ( KDvoid )
{
	return CCBlink::create ( m_fDuration, m_uBlinks );
}


//
// FadeIn
//

CCFadeIn* CCFadeIn::create ( KDfloat fDuration )
{
	CCFadeIn*  pRet = new CCFadeIn ( );

	if ( pRet && pRet->initWithDuration ( fDuration ) )
	{
 		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCObject* CCFadeIn::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCFadeIn*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		// in case of being called at sub class
		pCopy = (CCFadeIn *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCFadeIn ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
	
	CCActionInterval::copyWithZone ( pZone );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCFadeIn::update ( KDfloat fTime )
{
	CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( m_pTarget );

	if ( pRGBAProtocol )
	{
        pRGBAProtocol->setOpacity ( (GLubyte) ( 255 * fTime ) );
	}
}

CCActionInterval* CCFadeIn::reverse ( KDvoid )
{
	return CCFadeOut::create ( m_fDuration );
}


//
// FadeOut
//

CCFadeOut* CCFadeOut::create ( KDfloat fDuration )
{
	CCFadeOut*  pRet = new CCFadeOut ( );

	if ( pRet && pRet->initWithDuration ( fDuration ) )
	{
 		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCObject* CCFadeOut::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCFadeOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCFadeOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCFadeOut ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCFadeOut::update ( KDfloat fTime )
{
	CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( m_pTarget );

	if ( pRGBAProtocol )
	{
		pRGBAProtocol->setOpacity ( ( GLubyte ) ( 255 * ( 1 - fTime ) ) );
	}
}

CCActionInterval* CCFadeOut::reverse ( KDvoid )
{
	return CCFadeIn::create ( m_fDuration );
}


//
// FadeTo
//

CCFadeTo* CCFadeTo::create ( KDfloat fDuration, GLubyte cOpacity )
{
	CCFadeTo*  pRet = new CCFadeTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, cOpacity ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	 return pRet;
}

KDbool CCFadeTo::initWithDuration ( KDfloat fDuration, GLubyte cOpacity )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_cToOpacity = cOpacity;
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCFadeTo::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCFadeTo*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCFadeTo *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCFadeTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_cToOpacity );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCFadeTo::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( pTarget );

	if ( pRGBAProtocol )
	{
		m_cFromOpacity = pRGBAProtocol->getOpacity ( );
	}	
}

KDvoid CCFadeTo::update ( KDfloat fTime )
{
	CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( m_pTarget );

	if ( pRGBAProtocol )
	{
		pRGBAProtocol->setOpacity ( (GLubyte) ( m_cFromOpacity + ( m_cToOpacity - m_cFromOpacity ) * fTime ) );
	}
}


//
// TintTo
//

CCTintTo* CCTintTo::create ( KDfloat fDuration, GLubyte cRed, GLubyte cGreen, GLubyte cBlue )
{
	CCTintTo*  pRet = new CCTintTo ( );

	if ( pRet && pRet->initWithDuration ( fDuration, cRed, cGreen, cBlue ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCTintTo::initWithDuration ( KDfloat fDuration, GLubyte cRed, GLubyte cGreen, GLubyte cBlue )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_tTo = ccc3 ( cRed, cGreen, cBlue );
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCTintTo::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCTintTo*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCTintTo *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCTintTo ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_tTo.r, m_tTo.g, m_tTo.b );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCTintTo::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

    CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( pTarget );
	
	if ( pRGBAProtocol )
	{
		m_tFrom = pRGBAProtocol->getColor ( );
	}
}

KDvoid CCTintTo::update ( KDfloat fTime )
{
	CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( m_pTarget );

	if ( pRGBAProtocol )
	{
		pRGBAProtocol->setColor ( ccc3 ( (GLubyte) ( m_tFrom.r + ( m_tTo.r - m_tFrom.r ) * fTime ), 
			                             (GLubyte) ( m_tFrom.g + ( m_tTo.g - m_tFrom.g ) * fTime ),
			                             (GLubyte) ( m_tFrom.b + ( m_tTo.b - m_tFrom.b ) * fTime ) ) );
	}	
}


//
// TintBy
//

CCTintBy* CCTintBy::create ( KDfloat fDuration, GLshort nDeltaRed, GLshort nDeltaGreen, GLshort nDeltaBlue )
{
	CCTintBy*  pRet = new CCTintBy ( );

	if ( pRet && pRet->initWithDuration ( fDuration, nDeltaRed, nDeltaGreen, nDeltaBlue ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCTintBy::initWithDuration ( KDfloat fDuration, GLshort nDeltaRed, GLshort nDeltaGreen, GLshort nDeltaBlue )
{
	if ( CCActionInterval::initWithDuration ( fDuration ) )
	{
        m_nDeltaRed   = nDeltaRed;
		m_nDeltaGreen = nDeltaGreen;
		m_nDeltaBlue  = nDeltaBlue;

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCTintBy::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCTintBy*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCTintBy *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCTintBy ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithDuration ( m_fDuration, m_nDeltaRed, m_nDeltaGreen, m_nDeltaBlue );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCTintBy::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( pTarget );

	if ( pRGBAProtocol )
	{
		ccColor3B  tColor = pRGBAProtocol->getColor ( );

		m_nFromRed   = tColor.r;
		m_nFromGreen = tColor.g;
		m_nFromBlue  = tColor.b;
	}	
}

KDvoid CCTintBy::update ( KDfloat fTime )
{
	CCRGBAProtocol*  pRGBAProtocol = dynamic_cast<CCRGBAProtocol*> ( m_pTarget );

	if ( pRGBAProtocol )
	{
		pRGBAProtocol->setColor ( ccc3 ( (GLubyte) ( m_nFromRed   + m_nDeltaRed   * fTime ),
			                             (GLubyte) ( m_nFromGreen + m_nDeltaGreen * fTime ),
		                                 (GLubyte) ( m_nFromBlue  + m_nDeltaBlue  * fTime ) ) );
	}	
}

CCActionInterval* CCTintBy::reverse ( KDvoid )
{
	return CCTintBy::create ( m_fDuration, -m_nDeltaRed, -m_nDeltaGreen, -m_nDeltaBlue );
}


//
// DelayTime
//

CCDelayTime* CCDelayTime::create ( KDfloat fDuration )
{
	CCDelayTime*  pRet = new CCDelayTime ( );

	if ( pRet && pRet->initWithDuration ( fDuration ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCObject* CCDelayTime::copyWithZone ( CCZone* pZone )
{
	CCZone*    pNewZone = KD_NULL;
	CCDelayTime*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCDelayTime *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCDelayTime ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}
	
	CCActionInterval::copyWithZone ( pZone );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCDelayTime::update ( KDfloat fTime )
{
	CC_UNUSED_PARAM ( fTime );

	return;
}

CCActionInterval* CCDelayTime::reverse ( KDvoid )
{
	return CCDelayTime::create ( m_fDuration );
}


//
// ReverseTime
//

CCReverseTime* CCReverseTime::create ( CCFiniteTimeAction* pAction )
{
	// casting to prevent warnings
	CCReverseTime*  pRet = new CCReverseTime ( );

	if ( pRet && pRet->initWithAction ( pAction ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCReverseTime::initWithAction ( CCFiniteTimeAction* pAction )
{
	CCAssert ( pAction != KD_NULL , "" );
	CCAssert ( pAction != m_pOther, "" );

	if ( CCActionInterval::initWithDuration ( pAction->getDuration ( ) ) )
	{
		// Don't leak if action is reused
		CC_SAFE_RELEASE ( m_pOther );

		m_pOther = pAction;
		pAction->retain ( );

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCReverseTime::copyWithZone ( CCZone* pZone )
{
	CCZone*      pNewZone = KD_NULL;
	CCReverseTime*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		// in case of being called at sub class
		pCopy = (CCReverseTime *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCReverseTime ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithAction ( (CCFiniteTimeAction *) m_pOther->copy ( )->autorelease ( ) );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCReverseTime::CCReverseTime ( KDvoid ) 
{
    m_pOther = KD_NULL;
}

CCReverseTime::~CCReverseTime ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pOther );
}

KDvoid CCReverseTime::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );

	m_pOther->startWithTarget ( pTarget );
}

KDvoid CCReverseTime::stop ( KDvoid )
{
	m_pOther->stop ( );

	CCActionInterval::stop ( );
}

KDvoid CCReverseTime::update ( KDfloat fTime )
{
	if ( m_pOther )
	{
		m_pOther->update ( 1 - fTime );
	}
}

CCActionInterval* CCReverseTime::reverse ( KDvoid )
{
	return (CCActionInterval *) m_pOther->copy ( )->autorelease ( );
}


//
// Animate
//

CCAnimate* CCAnimate::create ( CCAnimation* pAnimation )
{
	CCAnimate*  pRet = new CCAnimate ( );

	if ( pRet && pRet->initWithAnimation ( pAnimation ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCAnimate::initWithAnimation ( CCAnimation* pAnimation )
{
    CCAssert ( pAnimation != KD_NULL, "Animate: argument Animation must be non-NULL" );

    KDfloat  fSingleDuration = pAnimation->getDuration ( );

    if ( CCActionInterval::initWithDuration ( fSingleDuration * pAnimation->getLoops ( ) ) ) 
    {
        m_nNextFrame = 0;
        setAnimation ( pAnimation );
        m_pOrigFrame = KD_NULL;
        m_uExecutedLoops = 0;

        m_pSplitTimes->reserve ( pAnimation->getFrames ( )->count ( ) );

        KDfloat  fAccumUnitsOfTime   = 0;
        KDfloat  fNewUnitOfTimeValue = fSingleDuration / pAnimation->getTotalDelayUnits ( );

        CCArray*  pFrames = pAnimation->getFrames ( );
        CCARRAY_VERIFY_TYPE ( pFrames, CCAnimationFrame* );

        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( pFrames, pObject )
        {
            CCAnimationFrame*  pFrame = (CCAnimationFrame*) pObject;
            KDfloat  fValue = ( fAccumUnitsOfTime * fNewUnitOfTimeValue ) / fSingleDuration;
            fAccumUnitsOfTime += pFrame->getDelayUnits ( );
            m_pSplitTimes->push_back ( fValue );
        }    
        return KD_TRUE;
    }

    return KD_FALSE;
}

CCObject* CCAnimate::copyWithZone ( CCZone* pZone )
{
    CCZone*  pNewZone = KD_NULL;
    CCAnimate*  pCopy = KD_NULL;
    
	if ( pZone && pZone->m_pCopyObject ) 
    {
        // in case of being called at sub class
        pCopy = (CCAnimate*) ( pZone->m_pCopyObject );
    }
    else
    {
        pCopy = new CCAnimate ( );
        pZone = pNewZone = new CCZone ( pCopy );
    }

    CCActionInterval::copyWithZone ( pZone );

    pCopy->initWithAnimation ( (CCAnimation*) m_pAnimation->copy ( )->autorelease ( ) );

    CC_SAFE_DELETE ( pNewZone );
    return pCopy;
}

CCAnimate::CCAnimate ( KDvoid )
{
	m_pAnimation	 = KD_NULL;
	m_pSplitTimes	 = new std::vector<KDfloat>;
	m_nNextFrame	 = 0;
	m_pOrigFrame	 = KD_NULL;
	m_uExecutedLoops = 0;
}

CCAnimate::~CCAnimate ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pAnimation );
    CC_SAFE_RELEASE ( m_pOrigFrame );
    CC_SAFE_DELETE ( m_pSplitTimes );
}

KDvoid CCAnimate::startWithTarget ( CCNode* pTarget )
{
    CCActionInterval::startWithTarget ( pTarget );
    CCSprite*  pSprite = (CCSprite*) pTarget;

    CC_SAFE_RELEASE ( m_pOrigFrame );

    if ( m_pAnimation->getRestoreOriginalFrame ( ) )
    {
        m_pOrigFrame = pSprite->displayFrame ( );
        m_pOrigFrame->retain ( );
    }

    m_nNextFrame     = 0;
    m_uExecutedLoops = 0;
}

KDvoid CCAnimate::stop ( KDvoid )
{
    if ( m_pAnimation->getRestoreOriginalFrame ( ) && m_pTarget )
    {
        ( (CCSprite*) m_pTarget )->setDisplayFrame ( m_pOrigFrame );
    }

    CCActionInterval::stop ( );
}

KDvoid CCAnimate::update ( KDfloat fTime )
{
    // if fTime == 1, ignore. Animation should finish with fTime == 1
    if ( fTime < 1.0f )
	{
        fTime *= m_pAnimation->getLoops ( );

        // new loop?  If so, reset frame counter
        KDuint uLoopNumber = (KDuint) fTime;
        if ( uLoopNumber > m_uExecutedLoops ) 
		{
            m_nNextFrame = 0;
            m_uExecutedLoops++;
        }

        // new fTime for animations
        fTime = kdFmodf ( fTime, 1.0f );
    }

    CCArray*  pFrames = m_pAnimation->getFrames ( );
    KDuint    uNumberOfFrames = pFrames->count ( );
    CCSpriteFrame*  pFrameToDisplay = KD_NULL;

    for ( KDuint i = m_nNextFrame; i < uNumberOfFrames; i++ ) 
	{
        KDfloat  fSplitTime = m_pSplitTimes->at ( i );

        if ( fSplitTime <= fTime ) 
		{
            CCAnimationFrame*  pFrame = (CCAnimationFrame*) pFrames->objectAtIndex ( i );
            pFrameToDisplay = pFrame->getSpriteFrame ( );
            ( (CCSprite*) m_pTarget )->setDisplayFrame ( pFrameToDisplay );

            CCDictionary*  pDict = pFrame->getUserInfo ( );
            if ( pDict )
            {
                //TODO: [[NSNotificationCenter defaultCenter] postNotificationName:CCAnimationFrameDisplayedNotification object:target_ userInfo:dict];
            }

            m_nNextFrame = i + 1;
		}
		else	// Issue 1438. Could be more than one frame per tick, due to low frame rate or frame delta < 1/FPS
		{
            break;
        }
    }
}

CCActionInterval* CCAnimate::reverse ( KDvoid )
{
    CCArray*  pOldArray = m_pAnimation->getFrames ( );
    CCArray*  pNewArray = CCArray::createWithCapacity ( pOldArray->count ( ) );
   
    CCARRAY_VERIFY_TYPE ( pOldArray, CCAnimationFrame* );

    if ( pOldArray->count ( ) > 0 )
    {
        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH_REVERSE ( pOldArray, pObject )
        {
            CCAnimationFrame*  pElement = (CCAnimationFrame*) pObject;
            if ( !pElement)
            {
                break;
            }

            pNewArray->addObject ( (CCAnimationFrame*) ( pElement->copy ( )->autorelease ( ) ) );
        }
    }

    CCAnimation*  pNewAnim = CCAnimation::create ( pNewArray, m_pAnimation->getDelayPerUnit ( ), m_pAnimation->getLoops ( ) );
    pNewAnim->setRestoreOriginalFrame ( m_pAnimation->getRestoreOriginalFrame ( ) );
	return CCAnimate::create ( pNewAnim );
}


//
// CCTargetedAction
//

CCTargetedAction::CCTargetedAction ( KDvoid )
{
	m_pForcedTarget	= KD_NULL;
	m_pAction		= KD_NULL;
}

CCTargetedAction::~CCTargetedAction ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pForcedTarget );
    CC_SAFE_RELEASE ( m_pAction );
}

CCTargetedAction* CCTargetedAction::create ( CCNode* pTarget, CCFiniteTimeAction* pAction )
{
    CCTargetedAction*  pRet = new CCTargetedAction ( );

	if ( pRet && pRet->initWithTarget ( pTarget, pAction ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDbool CCTargetedAction::initWithTarget ( CCNode* pTarget, CCFiniteTimeAction* pAction )
{
    if ( CCActionInterval::initWithDuration ( pAction->getDuration ( ) ) )
    {
        CC_SAFE_RETAIN ( pTarget );
        m_pForcedTarget = pTarget;
        CC_SAFE_RETAIN ( pAction );
        m_pAction = pAction;
        return KD_TRUE;
    }

    return KD_FALSE;
}

CCObject* CCTargetedAction::copyWithZone ( CCZone* pZone )
{
    CCZone*			pNewZone = KD_NULL;
    CCTargetedAction*   pRet = KD_NULL;

    if ( pZone && pZone->m_pCopyObject ) // in case of being called at sub class
    {
        pRet = (CCTargetedAction*) ( pZone->m_pCopyObject );
    }
    else
    {
        pRet = new CCTargetedAction ( );
        pZone = pNewZone = new CCZone ( pRet );
    }
    CCActionInterval::copyWithZone ( pZone );
    // win32 : use the m_pOther's copy object.
    pRet->initWithTarget ( m_pForcedTarget, (CCFiniteTimeAction*) m_pAction->copy ( )->autorelease ( ) ); 
    CC_SAFE_DELETE ( pNewZone );
    return pRet;
}

KDvoid CCTargetedAction::startWithTarget ( CCNode* pTarget )
{
    CCActionInterval::startWithTarget ( pTarget );
    m_pAction->startWithTarget ( m_pForcedTarget );
}

KDvoid CCTargetedAction::stop ( KDvoid )
{
    m_pAction->stop();
}

KDvoid CCTargetedAction::update ( KDfloat fTime )
{
    m_pAction->update ( fTime );
}

NS_CC_END