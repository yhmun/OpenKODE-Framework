/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxScrollNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
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

#include "extensions/parallax-scroll/CCParallaxScrollNode.h"

NS_CC_EXT_BEGIN

#ifndef PTM_RATIO
#define PTM_RATIO		32
#endif

#ifndef SIGN
#define SIGN( x )		( ( x < 0 ) ? -1 : ( x > 0 ) )
#endif

ParallaxScrollNode::ParallaxScrollNode ( KDvoid )
{
	m_pScrollOffsets = KD_NULL;
}

ParallaxScrollNode::~ParallaxScrollNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pScrollOffsets );
}

ParallaxScrollNode* ParallaxScrollNode::create ( KDvoid )
{
	ParallaxScrollNode*	 pRet = new ParallaxScrollNode ( );
	
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

KDbool ParallaxScrollNode::init ( KDvoid )
{
	if ( !Layer::init ( ) ) 
	{
		return KD_FALSE;
	}

	CC_SAFE_RELEASE ( m_pScrollOffsets );
    
    m_pScrollOffsets = Array::create ( );
	m_pScrollOffsets->retain ( );

	this->setClipping ( KD_TRUE );
    this->setAnchorPoint ( Point::ZERO );
    
    return KD_TRUE;
}

KDvoid ParallaxScrollNode::addChild ( Sprite* pNode, KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tOffset )
{
	this->addChild ( pNode, nZOrder, tRatio, tPosition, tOffset, Point::ZERO );
}

KDvoid ParallaxScrollNode::addChild ( Sprite* pNode, KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tOffset, const Point& tVelocity )
{
	pNode->setAnchorPoint ( Point::ZERO );
    
	m_pScrollOffsets->addObject ( ParallaxScrollOffset::create ( pNode, tRatio, tPosition, tOffset, tVelocity ) );
    
	Node::addChild ( pNode, nZOrder );
}

KDvoid ParallaxScrollNode::removeChild ( Sprite* pNode, KDbool bCleanup )
{
	Array*   pToRemove = Array::create ( );
    Object*  pObject   = KD_NULL;
    CCARRAY_FOREACH ( m_pScrollOffsets, pObject )
    {
        ParallaxScrollOffset*  pScrollOffset = (ParallaxScrollOffset*) pObject;
		if ( pScrollOffset->getTheChild ( ) == pNode )
		{
			pToRemove->addObject ( pScrollOffset );
			break;
		}
	}

	m_pScrollOffsets->removeObjectsInArray ( pToRemove );
}

// Used with box2d style velocity (m/s where m = 32 pixels), but box2d is not required
KDvoid ParallaxScrollNode::updateWithVelocity ( const Point& rVelocity, KDfloat fDelta )
{
	Point		tVelocity = rVelocity * PTM_RATIO; 
	Size		tRange	  = this->getContentSize ( );
    Object*		pObject   = KD_NULL;
    CCARRAY_FOREACH ( m_pScrollOffsets, pObject )
	{
		ParallaxScrollOffset*  pScrollOffset = (ParallaxScrollOffset*) pObject;

		Point		tRelVelocity   = pScrollOffset->getRelVelocity ( ) * PTM_RATIO ;
		Point		tTotalVelocity = tVelocity + tRelVelocity;
		Point		tOffset		   = Point ( tTotalVelocity * fDelta ) * pScrollOffset->getRatio ( );
        
        Node*		pChild = pScrollOffset->getTheChild ( );
		pChild->setPosition ( pChild->getPosition ( ) + tOffset );
		
		if ( ( tOffset.x < 0 && pChild->getPosition ( ).x + pChild->getContentSize ( ).cx < 0 ) ||
             ( tOffset.x > 0 && pChild->getPosition ( ).x > tRange.cx ) )
		{
			pChild->setPosition ( pChild->getPosition ( ) + Point ( -SIGN ( tOffset.x ) * kdFabsf ( pScrollOffset->getScrollOffset ( ).x ), 0 ) );
		}
		
		// Positive y indicates upward movement in cocos2d
		if ( ( tOffset.y < 0 && pChild->getPosition ( ).y + pChild->getContentSize ( ).cy < 0 ) ||
			 ( tOffset.y > 0 && pChild->getPosition ( ).y > tRange.cy ) ) 
		{
			pChild->setPosition ( pChild->getPosition ( ) + Point ( 0, -SIGN ( tOffset.y ) * kdFabsf ( pScrollOffset->getScrollOffset ( ).y ) ) );
		}
	}
}

KDvoid ParallaxScrollNode::updateWithYPosition ( KDfloat fPosY, KDfloat fDelta )
{
    Object*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pScrollOffsets, pObject )
	{
		ParallaxScrollOffset*		pScrollOffset = (ParallaxScrollOffset*) pObject;
		Node*						pChild  = pScrollOffset->getTheChild ( );
		KDfloat						fOffset = fPosY * pScrollOffset->getRatio ( ).y;

		pChild->setPosition ( pChild->getPosition ( ).x, pScrollOffset->getOrigPosition ( ).y + fOffset );
	}
}

KDvoid ParallaxScrollNode::addInfiniteScrollWithZ ( KDint nZOrder, const Point& tRatio, const Point& tPosition, const Point& tDir, Sprite* pObject, ... )
{
	KDVaListKHR		pArgs;
	Array*		pArray;

    KD_VA_START_KHR ( pArgs, pObject );		

	pArray = Array::create ( );

	while ( pObject != KD_NULL )
	{
		pArray->addObject ( pObject );

		pObject = KD_VA_ARG ( pArgs, Sprite* );
	}

	KD_VA_END_KHR ( pArgs );
	
	this->addInfiniteScrollWithObjects ( pArray, nZOrder, tRatio, tPosition, tDir );
}

KDvoid ParallaxScrollNode::addInfiniteScrollXWithZ ( KDint nZOrder, const Point& tRatio, const Point& tPosition, Sprite* pObject, ... )
{
	KDVaListKHR		pArgs;
	Array*		pArray;

    KD_VA_START_KHR ( pArgs, pObject );		

	pArray = Array::create ( );

	while ( pObject != KD_NULL )
	{
		pArray->addObject ( pObject );

		pObject = KD_VA_ARG ( pArgs, Sprite* );
	}

	KD_VA_END_KHR ( pArgs );
	
	this->addInfiniteScrollWithObjects ( pArray, nZOrder, tRatio, tPosition, Point ( 1, 0 ) );
}

KDvoid ParallaxScrollNode::addInfiniteScrollYWithZ ( KDint nZOrder, const Point& tRatio, const Point& tPosition, Sprite* pObject, ... )
{
	KDVaListKHR		pArgs;
	Array*		pArray;

    KD_VA_START_KHR ( pArgs, pObject );		

	pArray = Array::create ( );

	while ( pObject != KD_NULL )
	{
		pArray->addObject ( pObject );

		pObject = KD_VA_ARG ( pArgs, Sprite* );
	}

	KD_VA_END_KHR ( pArgs );
	
	this->addInfiniteScrollWithObjects ( pArray, nZOrder, tRatio, tPosition, Point ( 0, 1 ) );
}

KDvoid ParallaxScrollNode::addInfiniteScrollWithObjects ( Array* pObjects, KDint nZOrder, const Point& tRatio, const Point& tPos, const Point& tDir )
{
	this->addInfiniteScrollWithObjects ( pObjects, nZOrder, tRatio, tPos, tDir, Point::ZERO );
}

KDvoid ParallaxScrollNode::addInfiniteScrollWithObjects ( Array* pObjects, KDint nZOrder, const Point& tRatio, const Point& tPos, const Point& tDir, const Point& tVelocity )
{
	this->addInfiniteScrollWithObjects ( pObjects, nZOrder, tRatio, tPos, tDir, tVelocity, Point ( -1, -1 ) );
}

KDvoid ParallaxScrollNode::addInfiniteScrollWithObjects ( Array* pObjects, KDint nZOrder, const Point& tRatio, const Point& tPos, const Point& tDir, const Point& tVelocity, const Point& tPadding )
{
	// NOTE: corrects for 1 pixel at end of each sprite to avoid thin lines appearing

	// Calculate total width and height
	Size		tTotal = Size::ZERO;
    
    Object*		pObject = KD_NULL;
    CCARRAY_FOREACH ( pObjects, pObject )
    {
        Sprite*	 pSprite = (Sprite*) pObject;

		tTotal.cx += pSprite->getContentSize ( ).cx + tDir.x * tPadding.x;
		tTotal.cy += pSprite->getContentSize ( ).cy + tDir.y * tPadding.y;
	}
    
	// Position objects, add to parallax
	Point		tCurPos = tPos;
    CCARRAY_FOREACH ( pObjects, pObject )
    {
        Sprite*		pSprite = (Sprite*) pObject;
		Point		tNextPosOffset = Point ( tDir.x * ( pSprite->getContentSize ( ).cx + tPadding.x ), tDir.y * ( pSprite->getContentSize ( ).cy + tPadding.y ) );

		this->addChild ( pSprite, nZOrder, tRatio, tCurPos, Point ( tTotal.cx, tTotal.cy ), tVelocity );

		tCurPos = tCurPos + tNextPosOffset;
	}
}

NS_CC_EXT_END