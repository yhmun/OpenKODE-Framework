/* -----------------------------------------------------------------------------------
 *
 *      File            CCAnimationCache.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Ricardo Quesada
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
#include "sprite_nodes/CCAnimationCache.h"
#include "sprite_nodes/CCAnimation.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "support/CCFileUtils.h"

NS_CC_BEGIN
    
static CCAnimationCache*   l_pSharedAnimationCache = KD_NULL;

CCAnimationCache* CCAnimationCache::sharedAnimationCache ( KDvoid )
{
    if ( !l_pSharedAnimationCache )
	{
		l_pSharedAnimationCache = new CCAnimationCache ( );
		l_pSharedAnimationCache->init ( );
	}

	return l_pSharedAnimationCache;
}

KDvoid CCAnimationCache::purgeSharedAnimationCache ( KDvoid )
{
    CC_SAFE_RELEASE ( l_pSharedAnimationCache );
}

KDbool CCAnimationCache::init ( KDvoid )
{
	m_pAnimations = new CCDictionary ( );
    
	return KD_TRUE;
}

CCAnimationCache::CCAnimationCache ( KDvoid )
{
    m_pAnimations = KD_NULL;
}

CCAnimationCache::~CCAnimationCache ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCAnimationCache = %p", this );

	CC_SAFE_RELEASE ( m_pAnimations );
}

KDvoid CCAnimationCache::addAnimation ( CCAnimation* pAnimation, const KDchar* szName )
{
	m_pAnimations->setObject ( pAnimation, szName );
}

KDvoid CCAnimationCache::removeAnimationByName ( const KDchar* szName )
{
    if ( !szName )
	{
		return;
	}

	m_pAnimations->removeObjectForKey ( szName );
}

CCAnimation* CCAnimationCache::animationByName ( const KDchar* szName )
{
	return (CCAnimation*) m_pAnimations->objectForKey ( szName );
}

KDvoid CCAnimationCache::parseVersion1 ( CCDictionary* pAnimations )
{
    CCSpriteFrameCache*  pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

    CCDictElement*   pElement = KD_NULL;
    CCDICT_FOREACH ( pAnimations, pElement )
    {
        CCDictionary*	pAnimationDict = (CCDictionary*) pElement->getObject ( );
        CCArray*		pFrameNames = (CCArray*) pAnimationDict->objectForKey ( "frames" );
        KDfloat			fDelay		= pAnimationDict->valueForKey ( "delay" )->floatValue ( );
        CCAnimation*	pAnimation	= KD_NULL;

        if ( pFrameNames == KD_NULL ) 
        {
            CCLOG ( "XMCocos2D : CCAnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.", pElement->getStrKey ( ) );
            continue;
        }

        CCArray*  pFrames = CCArray::createWithCapacity ( pFrameNames->count ( ) );
        pFrames->retain ( );

        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( pFrameNames, pObject )
        {
            const KDchar*   szFrameName = ( (CCString*) pObject )->getCString ( );
            CCSpriteFrame*  pSpriteFrame = pFrameCache->spriteFrameByName ( szFrameName );

            if (  !pSpriteFrame )
			{
                CCLOG ( "XMCocos2D : CCAnimationCache: Animation '%s' refers to frame '%s' which is not currently in the CCSpriteFrameCache. This frame will not be added to the animation.", pElement->getStrKey ( ), szFrameName );
                continue;
            }

            CCAnimationFrame*  pAnimFrame = new CCAnimationFrame ( );
            pAnimFrame->initWithSpriteFrame ( pSpriteFrame, 1, KD_NULL );
            pFrames->addObject ( pAnimFrame );
            pAnimFrame->release ( );
        }

        if ( pFrames->count ( ) == 0 )
		{
            CCLOG ( "XMCocos2D : CCAnimationCache: None of the pFrames for animation '%s' were found in the CCSpriteFrameCache. Animation is not being added to the Animation Cache.", pElement->getStrKey ( ) );
            continue;
        } 
		else if ( pFrames->count ( ) != pFrameNames->count ( ) )
		{
            CCLOG ( "XMCocos2D : CCAnimationCache: An animation in your dictionary refers to a frame which is not in the CCSpriteFrameCache. Some or all of the pFrames for the animation '%s' may be missing.", pElement->getStrKey ( ) );
        }

        pAnimation = CCAnimation::create ( pFrames, fDelay, 1 );

        CCAnimationCache::sharedAnimationCache ( )->addAnimation ( pAnimation, pElement->getStrKey ( ) );
        pFrames->release ( );
    }    
}

KDvoid CCAnimationCache::parseVersion2 ( CCDictionary* pAnimations )
{
    CCSpriteFrameCache*  pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

    CCDictElement*  pElement = NULL;
    CCDICT_FOREACH ( pAnimations, pElement )
    {
        const KDchar*  szName = pElement->getStrKey ( );
        CCDictionary*  pAnimationDict = (CCDictionary*) pElement->getObject ( );

        const CCString*   pLoops = pAnimationDict->valueForKey ( "loops" );
        KDbool  bRestoreOriginalFrame = pAnimationDict->valueForKey ( "restoreOriginalFrame" )->boolValue ( );

        CCArray*  pFrameArray = (CCArray*) pAnimationDict->objectForKey ( "frames" );

        if ( pFrameArray == KD_NULL )
		{
            CCLOG ( "XMCocos2D : CCAnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.", szName );
            continue;
        }

        // Array of AnimationFrames
        CCArray*  pArray = CCArray::createWithCapacity ( pFrameArray->count ( ) );
        pArray->retain ( );

        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( pFrameArray, pObject )
        {
            CCDictionary*   pEntry = (CCDictionary*) pObject;
            const KDchar*   pSpriteFrameName = pEntry->valueForKey ( "spriteframe" )->getCString ( );
            CCSpriteFrame*  pSpriteFrame = pFrameCache->spriteFrameByName ( pSpriteFrameName );

            if ( !pSpriteFrame ) 
			{
                CCLOG ( "XMCocos2D : CCAnimationCache: Animation '%s' refers to frame '%s' which is not currently in the CCSpriteFrameCache. This frame will not be added to the animation.", szName, pSpriteFrameName );
                continue;
            }

            KDfloat  fDelayUnits = pEntry->valueForKey ( "delayUnits" )->floatValue ( );
            CCDictionary*  pUserInfo = (CCDictionary*) pEntry->objectForKey ( "notification" );

            CCAnimationFrame*  pAnimFrame = new CCAnimationFrame ( );
            pAnimFrame->initWithSpriteFrame ( pSpriteFrame, fDelayUnits, pUserInfo );

            pArray->addObject ( pAnimFrame );
            pAnimFrame->release ( );
        }

        KDfloat    fDelayPerUnit = pAnimationDict->valueForKey ( "delayPerUnit" )->floatValue ( );
        CCAnimation*  pAnimation = new CCAnimation ( );
		pAnimation->initWithAnimationFrames ( pArray, fDelayPerUnit, 0 != pLoops->length ( ) ? pLoops->intValue ( ) : 1 );
        pArray->release ( );

        pAnimation->setRestoreOriginalFrame ( bRestoreOriginalFrame );

        CCAnimationCache::sharedAnimationCache ( )->addAnimation ( pAnimation, szName );
        pAnimation->release ( );
    }
}

KDvoid CCAnimationCache::addAnimationsWithDictionary ( CCDictionary* pDictionary )
{
    CCDictionary*  pAnimations = (CCDictionary*) pDictionary->objectForKey ( "animations" );

    if ( pAnimations == KD_NULL )
	{
        CCLOG ( "XMCocos2D : CCAnimationCache: No animations were found in provided dictionary." );
        return;
    }

    KDuint  nVersion = 1;
    CCDictionary*  pProperties = (CCDictionary*) pDictionary->objectForKey ( "properties" );
    if ( pProperties )
    {
        nVersion = pProperties->valueForKey ( "format" )->intValue ( );
        CCArray*  pSpritesheets = (CCArray*) pProperties->objectForKey ( "spritesheets" );

        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( pSpritesheets, pObject )
        {
            CCString*  pName = (CCString*) pObject;
            CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( pName->getCString ( ) );
        }
    }

    switch ( nVersion ) 
	{
        case 1 :
            parseVersion1 ( pAnimations );
            break;

        case 2 :
            parseVersion2 ( pAnimations );
            break;

        default :
            CCAssert ( KD_FALSE, "Invalid animation format" );
    }
}

/** Read an NSDictionary from a plist file and parse it automatically for pAnimations */
KDvoid CCAnimationCache::addAnimationsWithFile ( const KDchar* szPlist )
{
    CCAssert ( szPlist, "Invalid texture file name" );

	std::string    sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szPlist );
	CCDictionary*  pDict = CCDictionary::createWithContentsOfFile ( sPath.c_str ( ) );

    CCAssert ( pDict, "CCAnimationCache: File could not be found" );

    addAnimationsWithDictionary ( pDict );
}

NS_CC_END