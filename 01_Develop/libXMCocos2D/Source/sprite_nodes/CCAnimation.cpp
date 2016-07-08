/* -----------------------------------------------------------------------------------
 *
 *      File            CCAnimation.cpp
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
#include "sprite_nodes/CCAnimation.h"
#include "textures/CCTextureCache.h"
#include "cocoa/CCZone.h"

NS_CC_BEGIN
  
CCAnimationFrame::CCAnimationFrame ( KDvoid )
{
	m_pSpriteFrame  = KD_NULL;
	m_fDelayUnits	= 0.0f;
	m_pUserInfo		= KD_NULL;
}

CCAnimationFrame::~CCAnimationFrame ( KDvoid )
{    
    CCLOGINFO ( "XMCocos2D : deallocing CCAnimationFrame = %s", this );

    CC_SAFE_RELEASE ( m_pSpriteFrame );
    CC_SAFE_RELEASE ( m_pUserInfo );
}

KDbool CCAnimationFrame::initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame, KDfloat fDelayUnits, CCDictionary* pUserInfo )
{
    setSpriteFrame ( pSpriteFrame );
    setDelayUnits ( fDelayUnits );
    setUserInfo ( pUserInfo );

    return KD_TRUE;
}

CCObject* CCAnimationFrame::copyWithZone ( CCZone* pZone )
{
    CCZone*         pNewZone = KD_NULL;
    CCAnimationFrame*  pCopy = KD_NULL;
    if ( pZone && pZone->m_pCopyObject ) 
    {
        // in case of being called at sub class
        pCopy = (CCAnimationFrame*) ( pZone->m_pCopyObject );
    }
    else
    {
        pCopy = new CCAnimationFrame ( );
        pNewZone = new CCZone ( pCopy );
    }

    pCopy->initWithSpriteFrame ( (CCSpriteFrame*) m_pSpriteFrame->copy ( )->autorelease ( ),
        m_fDelayUnits, m_pUserInfo != KD_NULL ? (CCDictionary*) m_pUserInfo->copy ( )->autorelease ( ) : KD_NULL );

    CC_SAFE_DELETE ( pNewZone );
    return pCopy;
}

//
// implementation of CCAnimation
//

CCAnimation* CCAnimation::create ( KDvoid )
{
	CCAnimation*  pRet = new CCAnimation ( );

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

CCAnimation* CCAnimation::create ( KDfloat fDelay )
{
	CCAnimation*  pRet = new CCAnimation ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
		pRet->setDelayPerUnit ( fDelay );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCAnimation* CCAnimation::createWithSpriteFrames ( CCArray* pArrayOfSpriteFrameNames, KDfloat fDelay )
{
	CCAnimation*  pRet = new CCAnimation ( );

	if ( pRet && pRet->initWithSpriteFrames ( pArrayOfSpriteFrameNames, fDelay ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCAnimation* CCAnimation::create ( CCArray* pArrayOfAnimationFrameNames, KDfloat fDelayPerUnit, KDuint uLoops )
{
	CCAnimation*  pRet = new CCAnimation ( );

	if ( pRet && pRet->initWithAnimationFrames ( pArrayOfAnimationFrameNames, fDelayPerUnit, uLoops ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCAnimation::init ( KDvoid )
{
	return initWithSpriteFrames ( KD_NULL, 0.0f );
}

KDbool CCAnimation::initWithSpriteFrames ( CCArray* pFrames, KDfloat fDelay )
{
    CCARRAY_VERIFY_TYPE ( pFrames, CCSpriteFrame* );

    m_uLoops = 1;
    m_fDelayPerUnit = fDelay;

    CCArray*    pTempFrames = CCArray::create ( );
    setFrames ( pTempFrames );

    if ( pFrames != KD_NULL )
    {
        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( pFrames, pObject )
        {
            CCSpriteFrame*     pFrame = (CCSpriteFrame*) pObject;
            CCAnimationFrame*  pAnimFrame = new CCAnimationFrame ( );
            pAnimFrame->initWithSpriteFrame ( pFrame, 1, KD_NULL );
            m_pFrames->addObject ( pAnimFrame );
            pAnimFrame->release ( );

            m_fTotalDelayUnits++;
        }
    }

    return KD_TRUE;
}

KDbool CCAnimation::initWithAnimationFrames ( CCArray* pArrayOfSpriteFrameNames, KDfloat fDelayPerUnit, KDuint uLoops )
{
    CCARRAY_VERIFY_TYPE ( pArrayOfSpriteFrameNames, CCAnimationFrame* );

    m_fDelayPerUnit = fDelayPerUnit;
    m_uLoops = uLoops;

    setFrames ( CCArray::createWithArray ( pArrayOfSpriteFrameNames ) );

    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pFrames, pObject )
    {
        CCAnimationFrame*  pAnimFrame = (CCAnimationFrame*) pObject;
        m_fTotalDelayUnits += pAnimFrame->getDelayUnits ( );
    }

    return KD_TRUE;
}

CCAnimation::CCAnimation ( KDvoid )
{
	m_fTotalDelayUnits		= 0.0f;
	m_fDelayPerUnit			= 0.0f;
	m_fDuration				= 0.0f;
	m_pFrames				= KD_NULL;
	m_bRestoreOriginalFrame = KD_FALSE;
	m_uLoops				= 0;
}

CCAnimation::~CCAnimation ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCAnimation = %p", this );

	CC_SAFE_RELEASE ( m_pFrames );
}

KDvoid CCAnimation::addSpriteFrame ( CCSpriteFrame* pFrame )
{
    CCAnimationFrame*  pAnimFrame = new CCAnimationFrame ( );
    pAnimFrame->initWithSpriteFrame ( pFrame, 1.0f, KD_NULL );
    m_pFrames->addObject ( pAnimFrame );
    pAnimFrame->release ( );

    // update duration
    m_fTotalDelayUnits++;
}

KDvoid CCAnimation::addSpriteFrameWithFileName ( const KDchar* pszFileName )
{
    CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( pszFileName );
	CCSpriteFrame*  pFrame = CCSpriteFrame::createWithTexture ( pTexture, CCRect ( CCPointZero, pTexture->getContentSize ( ) ) );

    addSpriteFrame ( pFrame );
}

KDvoid CCAnimation::addSpriteFrameWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
	CCSpriteFrame*  pFrame = CCSpriteFrame::createWithTexture ( pTexture, tRect );

	addSpriteFrame ( pFrame );
}

KDfloat CCAnimation::getDuration ( KDvoid )
{
    return m_fTotalDelayUnits * m_fDelayPerUnit;
}

CCObject* CCAnimation::copyWithZone ( CCZone* pZone )
{
    CCZone*    pNewZone = KD_NULL;
    CCAnimation*  pCopy = KD_NULL;
    if ( pZone && pZone->m_pCopyObject ) 
    {
        // in case of being called at sub class
        pCopy = (CCAnimation*) ( pZone->m_pCopyObject );
    }
    else
    {
        pCopy = new CCAnimation ( );
        pNewZone = new CCZone ( pCopy );
    }

    pCopy->initWithAnimationFrames ( m_pFrames, m_fDelayPerUnit, m_uLoops );
    pCopy->setRestoreOriginalFrame ( m_bRestoreOriginalFrame );

    CC_SAFE_DELETE ( pNewZone );
    return pCopy;
}

NS_CC_END