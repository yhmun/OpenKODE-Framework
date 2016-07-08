/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxScrollNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Created by Thomas Broquist on 3/26/12. 
 *      Copyright (c) 2012      Little Wins LLC. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCParallaxScroll/CCParallaxScrollNode.h"

NS_CC_BEGIN


#ifndef PTM_RATIO
#define PTM_RATIO		32
#endif

#ifndef SIGN
#define SIGN( x )		( ( x < 0 ) ? -1 : ( x > 0 ) )
#endif

CCParallaxScrollNode::CCParallaxScrollNode ( KDvoid )
{
	m_pScrollOffsets = KD_NULL;
}

CCParallaxScrollNode::~CCParallaxScrollNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pScrollOffsets );
}

CCParallaxScrollNode* CCParallaxScrollNode::create ( KDvoid )
{
	CCParallaxScrollNode*	 pRet = new CCParallaxScrollNode ( );
	
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

KDbool CCParallaxScrollNode::init ( KDvoid )
{
	if ( !CCLayer::init ( ) ) 
	{
		return KD_FALSE;
	}

	CC_SAFE_RELEASE ( m_pScrollOffsets );
    
    m_pScrollOffsets = new CCArray ( );

	this->setClipping ( KD_TRUE );
    this->setAnchorPoint ( ccpz );
    
    return KD_TRUE;
}

KDvoid CCParallaxScrollNode::addChild ( CCSprite* pNode, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tOffset )
{
	this->addChild ( pNode, nZOrder, tRatio, tPosition, tOffset, ccpz );
}

KDvoid CCParallaxScrollNode::addChild ( CCSprite* pNode, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tOffset, const CCPoint& tVelocity )
{
	pNode->setAnchorPoint ( ccpz );
    
	m_pScrollOffsets->addObject ( CCParallaxScrollOffset::create ( pNode, tRatio, tPosition, tOffset, tVelocity ) );
    
	CCNode::addChild ( pNode, nZOrder );
}

KDvoid CCParallaxScrollNode::removeChild ( CCSprite* pNode, KDbool bCleanup )
{
	CCArray*   pToRemove = CCArray::create ( );
    CCObject*  pObject   = KD_NULL;
    CCARRAY_FOREACH ( m_pScrollOffsets, pObject )
    {
        CCParallaxScrollOffset*  pScrollOffset = (CCParallaxScrollOffset*) pObject;
		if ( pScrollOffset->getTheChild ( ) == pNode )
		{
			pToRemove->addObject ( pScrollOffset );
			break;
		}
	}

	m_pScrollOffsets->removeObjectsInArray ( pToRemove );
}

// Used with box2d style velocity (m/s where m = 32 pixels), but box2d is not required
KDvoid CCParallaxScrollNode::updateWithVelocity ( const CCPoint& rVelocity, KDfloat fDelta )
{
	CCPoint		tVelocity = ccpMult ( rVelocity, PTM_RATIO );
	CCSize		tRange	  = this->getContentSize ( );
    CCObject*	pObject   = KD_NULL;
    CCARRAY_FOREACH ( m_pScrollOffsets, pObject )
	{
		CCParallaxScrollOffset*  pScrollOffset = (CCParallaxScrollOffset*) pObject;

		CCPoint		tRelVelocity   = ccpMult ( pScrollOffset->getRelVelocity ( ), PTM_RATIO );
		CCPoint		tTotalVelocity = ccpAdd  ( tVelocity, tRelVelocity);
		CCPoint		tOffset		   = ccpCompMult ( ccpMult ( tTotalVelocity, fDelta ), pScrollOffset->getRatio ( ) );
        
        CCNode*		pChild = pScrollOffset->getTheChild ( );
		pChild->setPosition ( ccpAdd ( pChild->getPosition ( ), tOffset ) );
		
		if ( ( tOffset.x < 0 && pChild->getPosition ( ).x + pChild->getContentSize ( ).cx < 0 ) ||
             ( tOffset.x > 0 && pChild->getPosition ( ).x > tRange.cx ) )
		{
			pChild->setPosition ( ccpAdd ( pChild->getPosition ( ), ccp ( -SIGN ( tOffset.x ) * kdFabsf ( pScrollOffset->getScrollOffset ( ).x ), 0 ) ) );
		}
		
		// Positive y indicates upward movement in cocos2d
		if ( ( tOffset.y < 0 && pChild->getPosition ( ).y + pChild->getContentSize ( ).cy < 0 ) ||
			 ( tOffset.y > 0 && pChild->getPosition ( ).y > tRange.cy ) ) 
		{
			pChild->setPosition ( ccpAdd ( pChild->getPosition ( ), ccp ( 0, -SIGN ( tOffset.y ) * kdFabsf ( pScrollOffset->getScrollOffset ( ).y ) ) ) );
		}
	}
}

KDvoid CCParallaxScrollNode::updateWithYPosition ( KDfloat fPosY, KDfloat fDelta )
{
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pScrollOffsets, pObject )
	{
		CCParallaxScrollOffset*		pScrollOffset = (CCParallaxScrollOffset*) pObject;
		CCNode*						pChild  = pScrollOffset->getTheChild ( );
		KDfloat						fOffset = fPosY * pScrollOffset->getRatio ( ).y;

		pChild->setPosition ( ccp ( pChild->getPosition ( ).x, pScrollOffset->getOrigPosition ( ).y + fOffset ) );
	}
}

KDvoid CCParallaxScrollNode::addInfiniteScrollWithZ ( KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tDir, CCSprite* pObject, ... )
{
	KDVaListKHR		pArgs;
	CCArray*		pArray;

    KD_VA_START_KHR ( pArgs, pObject );		

	pArray = CCArray::create ( );

	while ( pObject != KD_NULL )
	{
		pArray->addObject ( pObject );

		pObject = KD_VA_ARG ( pArgs, CCSprite* );
	}

	KD_VA_END_KHR ( pArgs );
	
	this->addInfiniteScrollWithObjects ( pArray, nZOrder, tRatio, tPosition, tDir );
}

KDvoid CCParallaxScrollNode::addInfiniteScrollXWithZ ( KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, CCSprite* pObject, ... )
{
	KDVaListKHR		pArgs;
	CCArray*		pArray;

    KD_VA_START_KHR ( pArgs, pObject );		

	pArray = CCArray::create ( );

	while ( pObject != KD_NULL )
	{
		pArray->addObject ( pObject );

		pObject = KD_VA_ARG ( pArgs, CCSprite* );
	}

	KD_VA_END_KHR ( pArgs );
	
	this->addInfiniteScrollWithObjects ( pArray, nZOrder, tRatio, tPosition, ccp ( 1, 0 ) );
}

KDvoid CCParallaxScrollNode::addInfiniteScrollYWithZ ( KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPosition, CCSprite* pObject, ... )
{
	KDVaListKHR		pArgs;
	CCArray*		pArray;

    KD_VA_START_KHR ( pArgs, pObject );		

	pArray = CCArray::create ( );

	while ( pObject != KD_NULL )
	{
		pArray->addObject ( pObject );

		pObject = KD_VA_ARG ( pArgs, CCSprite* );
	}

	KD_VA_END_KHR ( pArgs );
	
	this->addInfiniteScrollWithObjects ( pArray, nZOrder, tRatio, tPosition, ccp ( 0, 1 ) );
}

KDvoid CCParallaxScrollNode::addInfiniteScrollWithObjects ( CCArray* pObjects, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPos, const CCPoint& tDir )
{
	this->addInfiniteScrollWithObjects ( pObjects, nZOrder, tRatio, tPos, tDir, ccpz );
}

KDvoid CCParallaxScrollNode::addInfiniteScrollWithObjects ( CCArray* pObjects, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPos, const CCPoint& tDir, const CCPoint& tVelocity )
{
	this->addInfiniteScrollWithObjects ( pObjects, nZOrder, tRatio, tPos, tDir, tVelocity, ccp ( -1, -1 ) );
}

KDvoid CCParallaxScrollNode::addInfiniteScrollWithObjects ( CCArray* pObjects, KDint nZOrder, const CCPoint& tRatio, const CCPoint& tPos, const CCPoint& tDir, const CCPoint& tVelocity, const CCPoint& tPadding )
{
	// NOTE: corrects for 1 pixel at end of each sprite to avoid thin lines appearing

	// Calculate total width and height
	CCSize		tTotal = ccsz;
    
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( pObjects, pObject )
    {
        CCSprite*	 pSprite = (CCSprite*) pObject;

		tTotal.cx += pSprite->getContentSize ( ).cx + tDir.x * tPadding.x;
		tTotal.cy += pSprite->getContentSize ( ).cy + tDir.y * tPadding.y;
	}
    
	// Position objects, add to parallax
	CCPoint		tCurPos = tPos;
    CCARRAY_FOREACH ( pObjects, pObject )
    {
        CCSprite*	 pSprite = (CCSprite*) pObject;
		CCPoint		 tNextPosOffset = ccp ( tDir.x * ( pSprite->getContentSize ( ).cx + tPadding.x ), tDir.y * ( pSprite->getContentSize ( ).cy + tPadding.y ) );

		this->addChild ( pSprite, nZOrder, tRatio, tCurPos, ccp ( tTotal.cx, tTotal.cy ), tVelocity );

		tCurPos = ccpAdd ( tCurPos, tNextPosOffset );
	}
}


NS_CC_END