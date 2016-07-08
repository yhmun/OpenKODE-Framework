/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionEase.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2009 Jason Booth
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
#include "actions/CCActionEase.h"
#include "cocoa/CCZone.h"

NS_CC_BEGIN
    
//
// EaseAction
//

CCActionEase* CCActionEase::create ( CCActionInterval* pAction )
{
	CCActionEase*  pRet = new CCActionEase ( );

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

KDbool CCActionEase::initWithAction ( CCActionInterval* pAction )
{
	CCAssert ( pAction != KD_NULL, "" );

	if ( CCActionInterval::initWithDuration ( pAction->getDuration ( ) ) )
	{
		m_pInner = pAction;
		pAction->retain ( );

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCActionEase::copyWithZone ( CCZone* pZone )
{
	CCZone*     pNewZone = KD_NULL;
	CCActionEase*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		pCopy = (CCActionEase *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCActionEase ( );
		pZone = pNewZone = new CCZone ( pCopy );
	}

	CCActionInterval::copyWithZone ( pZone );

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCActionEase::~CCActionEase ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pInner );
}

KDvoid CCActionEase::startWithTarget ( CCNode* pTarget )
{
	CCActionInterval::startWithTarget ( pTarget );
    
	m_pInner->startWithTarget ( m_pTarget );
}

KDvoid CCActionEase::stop ( KDvoid )
{
	m_pInner->stop ( );
    
	CCActionInterval::stop ( );
}

KDvoid CCActionEase::update ( KDfloat fTime )
{
	m_pInner->update ( fTime );
}

CCActionInterval* CCActionEase::reverse ( KDvoid )
{
	return CCActionEase::create ( m_pInner->reverse ( ) );
}

CCActionInterval* CCActionEase::getInnerAction ( KDvoid )
{
    return m_pInner;
}

//
// EaseRateAction
//

CCEaseRateAction* CCEaseRateAction::create ( CCActionInterval* pAction, KDfloat fRate )
{
	CCEaseRateAction*  pRet = new CCEaseRateAction ( );

    if ( pRet && pRet->initWithAction ( pAction, fRate ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }	

	return pRet;
}

KDbool CCEaseRateAction::initWithAction ( CCActionInterval* pAction, KDfloat fRate )
{
	if ( CCActionEase::initWithAction ( pAction ) )
	{
		m_fRate = fRate;
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCEaseRateAction::copyWithZone ( CCZone* pZone )
{
	CCZone*         pNewZone = KD_NULL;
	CCEaseRateAction*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseRateAction *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseRateAction ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ), m_fRate );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCActionInterval* CCEaseRateAction::reverse ( KDvoid )
{
	return CCEaseRateAction::create ( m_pInner->reverse ( ), 1 / m_fRate );
}

//
// EeseIn
//

CCEaseIn* CCEaseIn::create ( CCActionInterval* pAction, KDfloat fRate )
{
	CCEaseIn*  pRet = new CCEaseIn ( );

    if ( pRet && pRet->initWithAction ( pAction, fRate ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

CCObject* CCEaseIn::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCEaseIn*   pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseIn *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseIn ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ), m_fRate );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseIn::update ( KDfloat fTime )
{
	m_pInner->update ( kdPowf ( fTime, m_fRate ) );
}

CCActionInterval* CCEaseIn::reverse ( KDvoid )
{
	return CCEaseIn::create ( m_pInner->reverse ( ), 1 / m_fRate );
}

//
// EaseOut
//

CCEaseOut* CCEaseOut::create ( CCActionInterval* pAction, KDfloat fRate )
{
	CCEaseOut*  pRet = new CCEaseOut ( );

    if ( pRet && pRet->initWithAction ( pAction, fRate ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;      
}

CCObject* CCEaseOut::copyWithZone ( CCZone* pZone )
{
	CCZone*  pNewZone = KD_NULL;
	CCEaseOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ), m_fRate );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseOut::update ( KDfloat fTime )
{
	m_pInner->update ( kdPowf ( fTime, 1 / m_fRate ) );
}

CCActionInterval* CCEaseOut::reverse ( KDvoid )
{
    return CCEaseOut::create ( m_pInner->reverse ( ), 1 / m_fRate );
}

//
// EaseInOut
//

CCEaseInOut* CCEaseInOut::create ( CCActionInterval* pAction, KDfloat fRate )
{
	CCEaseInOut*  pRet = new CCEaseInOut ( );

    if ( pRet && pRet->initWithAction ( pAction, fRate ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;        
}

CCObject* CCEaseInOut::copyWithZone ( CCZone* pZone )
{
	CCZone*    pNewZone = KD_NULL;
	CCEaseInOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseInOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseInOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ), m_fRate );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseInOut::update ( KDfloat fTime )
{
	fTime *= 2;
    if ( fTime < 1 )
    {
        m_pInner->update ( 0.5f * kdPowf ( fTime, m_fRate ) );
    }
    else
    {
        m_pInner->update ( 1.0f - 0.5f * kdPowf ( 2 - fTime, m_fRate ) );
    }
}

CCActionInterval* CCEaseInOut::reverse ( KDvoid )
{
	return CCEaseInOut::create ( m_pInner->reverse ( ), m_fRate );
}

//
// EaseExponentialIn
//

CCEaseExponentialIn* CCEaseExponentialIn::create ( CCActionInterval* pAction )
{
	CCEaseExponentialIn*  pRet = new CCEaseExponentialIn ( );

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

CCObject* CCEaseExponentialIn::copyWithZone ( CCZone* pZone )
{
	CCZone*            pNewZone = KD_NULL;
	CCEaseExponentialIn*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseExponentialIn *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseExponentialIn ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseExponentialIn::update ( KDfloat fTime )
{
	m_pInner->update ( fTime == 0 ? 0 : kdPowf ( 2, 10 * ( fTime / 1 - 1 ) ) - 1 * 0.001f );
}

CCActionInterval* CCEaseExponentialIn::reverse ( KDvoid )
{
	return CCEaseExponentialOut::create ( m_pInner->reverse ( ) );
}

//
// EaseExponentialOut
//

CCEaseExponentialOut* CCEaseExponentialOut::create ( CCActionInterval* pAction )
{
	CCEaseExponentialOut*  pRet = new CCEaseExponentialOut ( );

    if ( pRet && pRet->initWithAction ( pAction ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE( pRet );
    }

	return pRet; 
}

CCObject* CCEaseExponentialOut::copyWithZone ( CCZone* pZone )
{
	CCZone*             pNewZone = KD_NULL;
	CCEaseExponentialOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseExponentialOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseExponentialOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseExponentialOut::update ( KDfloat fTime )
{
	m_pInner->update ( fTime == 1 ? 1 : ( -kdPowf ( 2, -10 * fTime / 1 ) + 1 ) );
}

CCActionInterval* CCEaseExponentialOut::reverse ( KDvoid )
{
	return CCEaseExponentialIn::create ( m_pInner->reverse ( ) );
}

//
// EaseExponentialInOut
//

CCEaseExponentialInOut* CCEaseExponentialInOut::create ( CCActionInterval* pAction )
{
	CCEaseExponentialInOut*  pRet = new CCEaseExponentialInOut ( );

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

CCObject* CCEaseExponentialInOut::copyWithZone ( CCZone* pZone )
{
	CCZone*               pNewZone = KD_NULL;
	CCEaseExponentialInOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseExponentialInOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseExponentialInOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseExponentialInOut::update ( KDfloat fTime )
{
	fTime /= 0.5f;

	if ( fTime < 1 )
	{
		fTime = 0.5f * kdPowf ( 2, 10 * ( fTime - 1 ) );
	}
	else
	{
		fTime = 0.5f * ( -kdPowf ( 2, -10 * ( fTime - 1 ) ) + 2 );
	}

	m_pInner->update ( fTime );
}

CCActionInterval* CCEaseExponentialInOut::reverse ( KDvoid )
{
    return CCEaseExponentialInOut::create ( m_pInner->reverse ( ) );
}

//
// EaseSineIn
//

CCEaseSineIn* CCEaseSineIn::create ( CCActionInterval* pAction )
{
	CCEaseSineIn*  pRet = new CCEaseSineIn ( );

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

CCObject* CCEaseSineIn::copyWithZone ( CCZone* pZone )
{
	CCZone*     pNewZone = KD_NULL;
	CCEaseSineIn*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		//in case of being called at sub class
		pCopy = (CCEaseSineIn *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseSineIn ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseSineIn::update ( KDfloat fTime )
{
	m_pInner->update ( -1 * kdCosf ( fTime * KD_PI_2_F ) + 1 );
}

CCActionInterval* CCEaseSineIn::reverse ( KDvoid )
{
	return CCEaseSineOut::create ( m_pInner->reverse ( ) );
}

//
// EaseSineOut
//

CCEaseSineOut* CCEaseSineOut::create ( CCActionInterval* pAction )
{
	CCEaseSineOut*  pRet = new CCEaseSineOut ( );

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

CCObject* CCEaseSineOut::copyWithZone ( CCZone* pZone )
{
	CCZone*      pNewZone = KD_NULL;
	CCEaseSineOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseSineOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseSineOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseSineOut::update ( KDfloat fTime )
{
	m_pInner->update ( kdSinf ( fTime * KD_PI_2_F ) );
}

CCActionInterval* CCEaseSineOut::reverse ( KDvoid )
{
	return CCEaseSineIn::create ( m_pInner->reverse ( ) );
}

//
// EaseSineInOut
//

CCEaseSineInOut* CCEaseSineInOut::create ( CCActionInterval* pAction )
{
	CCEaseSineInOut*  pRet = new CCEaseSineInOut ( );

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

CCObject* CCEaseSineInOut::copyWithZone ( CCZone* pZone )
{
	CCZone*        pNewZone = KD_NULL;
	CCEaseSineInOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseSineInOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseSineInOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseSineInOut::update ( KDfloat fTime )
{
	m_pInner->update ( -0.5f * ( kdCosf ( KD_PI_F * fTime ) - 1 ) );
}

CCActionInterval* CCEaseSineInOut::reverse ( KDvoid )
{
    return CCEaseSineInOut::create ( m_pInner->reverse ( ) );
}

//
// EaseElastic
//

CCEaseElastic* CCEaseElastic::create ( CCActionInterval* pAction )
{
    return CCEaseElastic::create ( pAction, 0.3f );
}

CCEaseElastic* CCEaseElastic::create ( CCActionInterval* pAction, KDfloat fPeriod )
{
	CCEaseElastic*  pRet = new CCEaseElastic ( );

    if ( pRet && pRet->initWithAction ( pAction, fPeriod ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet; 
}

KDbool CCEaseElastic::initWithAction ( CCActionInterval* pAction, KDfloat fPeriod )
{
	if ( CCActionEase::initWithAction ( pAction ) )
	{
		m_fPeriod = fPeriod;
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCObject* CCEaseElastic::copyWithZone ( CCZone* pZone )
{
	CCZone*      pNewZone = KD_NULL;
	CCEaseElastic*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseElastic *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseElastic ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ), m_fPeriod );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

CCActionInterval* CCEaseElastic::reverse ( KDvoid )
{
	CCAssert ( 0, "Override me" );

	return KD_NULL;
}

//
// EaseElasticIn
//

CCEaseElasticIn* CCEaseElasticIn::create ( CCActionInterval* pAction )
{
    return CCEaseElasticIn::create ( pAction, 0.3f );
}

CCEaseElasticIn* CCEaseElasticIn::create ( CCActionInterval* pAction, KDfloat fPeriod )
{
	CCEaseElasticIn*  pRet = new CCEaseElasticIn ( );

    if ( pRet && pRet->initWithAction ( pAction, fPeriod ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet; 
}

CCObject* CCEaseElasticIn::copyWithZone ( CCZone* pZone )
{
	CCZone*        pNewZone = KD_NULL;
	CCEaseElasticIn*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseElasticIn *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseElasticIn ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ), m_fPeriod );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseElasticIn::update ( KDfloat fTime )
{
	KDfloat fNewTime = 0;

	if ( fTime == 0 || fTime == 1 )
	{
		fNewTime = fTime;
	}
	else
	{
		KDfloat fS = m_fPeriod / 4;

		fTime = fTime - 1;
		fNewTime = -kdPowf ( 2, 10 * fTime ) * kdSinf ( ( fTime - fS ) * KD_2PI_F / m_fPeriod );
	} 

	m_pInner->update ( fNewTime );
}

CCActionInterval* CCEaseElasticIn::reverse ( KDvoid )
{
	return CCEaseElasticOut::create ( m_pInner->reverse ( ), m_fPeriod );
}

//
// EaseElasticOut
//

CCEaseElasticOut* CCEaseElasticOut::create ( CCActionInterval* pAction )
{
    return CCEaseElasticOut::create ( pAction, 0.3f );
}

CCEaseElasticOut* CCEaseElasticOut::create ( CCActionInterval* pAction, KDfloat fPeriod )
{
	CCEaseElasticOut*  pRet = new CCEaseElasticOut ( );

    if ( pRet && pRet->initWithAction ( pAction, fPeriod ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet; 
}

CCObject* CCEaseElasticOut::copyWithZone ( CCZone* pZone )
{
	CCZone*         pNewZone = KD_NULL;
	CCEaseElasticOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseElasticOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseElasticOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ), m_fPeriod );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseElasticOut::update ( KDfloat fTime )
{
	KDfloat  fNewTime = 0;

	if ( fTime == 0 || fTime == 1 )
	{
		fNewTime = fTime;
	}
	else
	{
		KDfloat  fS = m_fPeriod / 4;

		fNewTime = kdPowf ( 2, -10 * fTime ) * kdSinf ( ( fTime - fS ) * KD_2PI_F / m_fPeriod ) + 1;
	}

	m_pInner->update ( fNewTime );
}

CCActionInterval* CCEaseElasticOut::reverse ( KDvoid )
{
	return CCEaseElasticIn::create ( m_pInner->reverse ( ), m_fPeriod );
}

//
// EaseElasticInOut
//

CCEaseElasticInOut* CCEaseElasticInOut::create ( CCActionInterval* pAction )
{
    return CCEaseElasticInOut::create ( pAction, 0.3f );
}

CCEaseElasticInOut* CCEaseElasticInOut::create ( CCActionInterval* pAction, KDfloat fPeriod )
{
	CCEaseElasticInOut*  pRet = new CCEaseElasticInOut ( );

    if ( pRet->initWithAction ( pAction, fPeriod ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet; 
}

CCObject* CCEaseElasticInOut::copyWithZone ( CCZone* pZone )
{
	CCZone*           pNewZone = KD_NULL;
	CCEaseElasticInOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseElasticInOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseElasticInOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ), m_fPeriod );

	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseElasticInOut::update ( KDfloat fTime )
{
	KDfloat  fNewTime = 0;

	if ( fTime == 0 || fTime == 1 )
	{
		fNewTime = fTime;
	}
	else
	{
		fTime = fTime * 2;

		if ( !m_fPeriod )
		{
			m_fPeriod = 0.3f * 1.5f;
		}

		KDfloat  fS = m_fPeriod / 4;

		fTime = fTime - 1;

		if ( fTime < 0 )
		{
			fNewTime = -0.5f * kdPowf ( 2, 10 * fTime ) * kdSinf ( ( fTime - fS ) * KD_2PI_F / m_fPeriod );
		}
		else
		{
			fNewTime = kdPowf ( 2, -10 * fTime ) * kdSinf ( ( fTime - fS ) * KD_2PI_F / m_fPeriod ) * 0.5f + 1;
		}
	}

	m_pInner->update ( fNewTime );
}

CCActionInterval* CCEaseElasticInOut::reverse ( KDvoid )
{
	return CCEaseElasticInOut::create ( m_pInner->reverse ( ), m_fPeriod );
}

//
// EaseBounce
//

CCEaseBounce* CCEaseBounce::create ( CCActionInterval* pAction )
{
	CCEaseBounce*  pRet = new CCEaseBounce ( );

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

CCObject* CCEaseBounce::copyWithZone ( CCZone* pZone )
{
	CCZone*     pNewZone = KD_NULL;
	CCEaseBounce*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseBounce *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseBounce ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDfloat CCEaseBounce::bounceTime ( KDfloat fTime )
{
	if ( fTime < 1 / 2.75 )
	{
		return 7.5625f * fTime * fTime;
	} 
	else if ( fTime < 2 / 2.75 )
	{
		fTime -= 1.5f / 2.75f;
		return 7.5625f * fTime * fTime + 0.75f;
	} 
	else if ( fTime < 2.5 / 2.75 )
	{
		fTime -= 2.25f / 2.75f;
		return 7.5625f * fTime * fTime + 0.9375f;
	}
    else
    {
        fTime -= 2.625f / 2.75f;
        return 7.5625f * fTime * fTime + 0.984375f;
    }
}

CCActionInterval* CCEaseBounce::reverse ( KDvoid )
{
    return CCEaseBounce::create ( m_pInner->reverse ( ) );
}

//
// EaseBounceIn
//

CCEaseBounceIn* CCEaseBounceIn::create ( CCActionInterval* pAction )
{
	CCEaseBounceIn*  pRet = new CCEaseBounceIn ( );

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

CCObject* CCEaseBounceIn::copyWithZone ( CCZone* pZone )
{
	CCZone*       pNewZone = KD_NULL;
	CCEaseBounceIn*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseBounceIn *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseBounceIn ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseBounceIn::update ( KDfloat fTime )
{
	KDfloat  fNewTime = 1 - bounceTime ( 1 - fTime );

	m_pInner->update ( fNewTime );
}

CCActionInterval* CCEaseBounceIn::reverse ( KDvoid )
{
	return CCEaseBounceOut::create ( m_pInner->reverse ( ) );
}

//
// EaseBounceOut
//

CCEaseBounceOut* CCEaseBounceOut::create ( CCActionInterval* pAction )
{
	CCEaseBounceOut*  pRet = new CCEaseBounceOut ( );

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

CCObject* CCEaseBounceOut::copyWithZone ( CCZone* pZone )
{
	CCZone*        pNewZone = KD_NULL;
	CCEaseBounceOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject )
	{
		//in case of being called at sub class
		pCopy = (CCEaseBounceOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseBounceOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseBounceOut::update ( KDfloat fTime )
{
	KDfloat  fNewTime = bounceTime ( fTime );
	m_pInner->update ( fNewTime );
}

CCActionInterval* CCEaseBounceOut::reverse ( KDvoid )
{
	return CCEaseBounceIn::create ( m_pInner->reverse ( ) );
}

//
// EaseBounceInOut
//

CCEaseBounceInOut* CCEaseBounceInOut::create ( CCActionInterval* pAction )
{
	CCEaseBounceInOut*  pRet = new CCEaseBounceInOut ( );

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

CCObject* CCEaseBounceInOut::copyWithZone ( CCZone* pZone )
{
	CCZone*          pNewZone = KD_NULL;
	CCEaseBounceInOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseBounceInOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseBounceInOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseBounceInOut::update ( KDfloat fTime )
{
	KDfloat  fNewTime = 0;

	if ( fTime < 0.5f )
	{
		fTime = fTime * 2;
		fNewTime = (1 - bounceTime ( 1 - fTime ) ) * 0.5f;
	}
	else
	{
		fNewTime = bounceTime ( fTime * 2 - 1 ) * 0.5f + 0.5f;
	}

	m_pInner->update ( fNewTime );
}

CCActionInterval* CCEaseBounceInOut::reverse ( KDvoid )
{
    return CCEaseBounceInOut::create ( m_pInner->reverse ( ) );
}

//
// EaseBackIn
//

CCEaseBackIn* CCEaseBackIn::create ( CCActionInterval* pAction )
{
	CCEaseBackIn*  pRet = new CCEaseBackIn ( );

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

CCObject* CCEaseBackIn::copyWithZone ( CCZone* pZone )
{
	CCZone*     pNewZone = KD_NULL;
	CCEaseBackIn*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseBackIn *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseBackIn ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseBackIn::update ( KDfloat fTime )
{
	KDfloat  fOverShoot = 1.70158f;
	m_pInner->update ( fTime * fTime * ( ( fOverShoot + 1 ) * fTime - fOverShoot ) );
}

CCActionInterval* CCEaseBackIn::reverse ( KDvoid )
{
	return CCEaseBackOut::create ( m_pInner->reverse ( ) );
}

//
// EaseBackOut
//

CCEaseBackOut* CCEaseBackOut::create ( CCActionInterval* pAction )
{
	CCEaseBackOut*  pRet = new CCEaseBackOut ( );

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

CCObject* CCEaseBackOut::copyWithZone ( CCZone* pZone )
{
	CCZone*      pNewZone = KD_NULL;
	CCEaseBackOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseBackOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseBackOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseBackOut::update ( KDfloat fTime )
{
	KDfloat  fOverShoot = 1.70158f;

	fTime = fTime - 1;
	m_pInner->update ( fTime * fTime * ( ( fOverShoot + 1 ) * fTime + fOverShoot ) + 1 );
}

CCActionInterval* CCEaseBackOut::reverse ( KDvoid )
{
	return CCEaseBackIn::create ( m_pInner->reverse ( ) );
}

//
// EaseBackInOut
//

CCEaseBackInOut* CCEaseBackInOut::create ( CCActionInterval* pAction )
{
	CCEaseBackInOut*  pRet = new CCEaseBackInOut ( );

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

CCObject* CCEaseBackInOut::copyWithZone ( CCZone* pZone )
{
	CCZone*        pNewZone = KD_NULL;
	CCEaseBackInOut*  pCopy = KD_NULL;

	if ( pZone && pZone->m_pCopyObject ) 
	{
		//in case of being called at sub class
		pCopy = (CCEaseBackInOut *) pZone->m_pCopyObject;
	}
	else
	{
		pCopy = new CCEaseBackInOut ( );
		pNewZone = new CCZone ( pCopy );
	}

	pCopy->initWithAction ( (CCActionInterval*) m_pInner->copy ( )->autorelease ( ) );
	
	CC_SAFE_DELETE ( pNewZone );

	return pCopy;
}

KDvoid CCEaseBackInOut::update ( KDfloat fTime )
{
	KDfloat  fOverShoot = 1.70158f * 1.525f;

	fTime = fTime * 2;
	if ( fTime < 1)
	{
		m_pInner->update ( ( fTime * fTime * ( ( fOverShoot + 1 ) * fTime - fOverShoot ) ) / 2);
	}
	else
	{
		fTime = fTime - 2;
		m_pInner->update ( ( fTime * fTime * ( ( fOverShoot + 1 ) * fTime + fOverShoot ) ) / 2 + 1);
	}
}

CCActionInterval* CCEaseBackInOut::reverse ( KDvoid )
{
    return CCEaseBackInOut::create ( m_pInner->reverse ( ) );
}

NS_CC_END