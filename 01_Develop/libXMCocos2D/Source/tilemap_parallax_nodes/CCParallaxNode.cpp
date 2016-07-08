/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009-2010 Ricardo Quesada
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
#include "tilemap_parallax_nodes/CCParallaxNode.h"
#include "support/data_support/ccCArray.h"
#include "support/CCPointExtension.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN
    
class CCPointObject : CCObject
{
	public :

		static CCPointObject*  create ( const CCPoint& tRatio, const CCPoint& tOffset )
		{
			CCPointObject*  pRet = new CCPointObject ( );

			if ( pRet && pRet->initWithCCPoint ( tRatio, tOffset ) )
			{
				pRet->autorelease ( );
			}
			else
			{
				CC_SAFE_DELETE ( pRet );
			}

			return pRet;
		}

	public :

		virtual KDbool  initWithCCPoint ( const CCPoint& tRatio, const CCPoint& tOffset )
		{
			m_tRatio  = tRatio;
			m_tOffset = tOffset;
			m_pChild  = KD_NULL;

			return KD_TRUE;
		}

		CC_SYNTHESIZE_PASS_BY_REF ( CCPoint, m_tRatio , Ratio );
		CC_SYNTHESIZE_PASS_BY_REF ( CCPoint, m_tOffset, Offset );
		CC_SYNTHESIZE ( CCNode*, m_pChild , Child );	
};

CCParallaxNode::CCParallaxNode ( KDvoid )
{
	m_pParallaxArray = ccArrayNew ( 5 );		
	m_tLastPosition  = ccp ( -100, -100 );
}

CCParallaxNode::~CCParallaxNode ( KDvoid )
{
	if ( m_pParallaxArray )
	{
		ccArrayFree ( m_pParallaxArray );
		m_pParallaxArray = KD_NULL;
	}
}

CCParallaxNode* CCParallaxNode::create ( KDvoid )
{
	CCParallaxNode*  pRet = new CCParallaxNode ( );

	if ( pRet )
	{
		pRet->autorelease ( );
	}

	return pRet;
}

KDvoid CCParallaxNode::addChild ( CCNode* pChild, KDuint nZOrder, KDint nTag )
{
	CC_UNUSED_PARAM ( nZOrder );
	CC_UNUSED_PARAM ( pChild );
	CC_UNUSED_PARAM ( nTag );

	CCAssert ( 0,"ParallaxNode: use addChild:z:parallaxRatio:positionOffset instead" );
}

KDvoid CCParallaxNode::addChild ( CCNode* pChild, KDuint nZOrder, const CCPoint& tRatio, const CCPoint& tOffset )
{
	CCAssert ( pChild != KD_NULL, "Argument must be non-nil" );

	CCPointObject*  pObject = CCPointObject::create ( tRatio, tOffset );
	pObject->setChild ( pChild );

	ccArrayAppendObjectWithResize ( m_pParallaxArray, (CCObject *) pObject );

	pChild->setPosition ( ccp ( m_tPosition.x * tRatio.x + tOffset.x, m_tPosition.y * tRatio.y + tOffset.y ) );

	CCNode::addChild ( pChild, nZOrder, pChild->getTag ( ) );
}

KDvoid CCParallaxNode::removeChild ( CCNode* pChild, KDbool bCleanup )
{
	for ( KDuint  i = 0;i < m_pParallaxArray->num; i++ )
	{
		CCPointObject*  pObject = (CCPointObject *) m_pParallaxArray->arr[ i ];
		if ( pObject->getChild ( )->isEqual ( pChild ) ) 
		{
			ccArrayRemoveObjectAtIndex ( m_pParallaxArray, i, KD_TRUE );
			break;
		}
	}
    
	CCNode::removeChild ( pChild, bCleanup );
}

KDvoid CCParallaxNode::removeAllChildrenWithCleanup ( KDbool bCleanup )
{
	ccArrayRemoveAllObjects ( m_pParallaxArray );
	CCNode::removeAllChildrenWithCleanup ( bCleanup );
}

CCPoint CCParallaxNode::absolutePosition ( KDvoid )
{
	CCPoint  tPosition = m_tPosition;
	CCNode*      pNode = this;

	while ( pNode->getParent ( ) != KD_NULL )
	{
        pNode = pNode->getParent ( );
		tPosition = ccpAdd ( tPosition, pNode->getPosition ( ) );
	}

	return tPosition;
}

//
// The positions are updated at visit because:
//   - using a timer is not guaranteed that it will called after all the positions were updated
//   - overriding "draw" will only precise if the children have a z > 0
//
KDvoid CCParallaxNode::visit ( KDvoid )
{
	CCPoint  tPosition = this->absolutePosition ( );

	if ( !tPosition.equals ( m_tLastPosition ) )
	{
		for ( KDuint  i = 0; i < m_pParallaxArray->num; i++ ) 
		{
			CCPointObject*  pObject = (CCPointObject*) m_pParallaxArray->arr [ i ];

			KDfloat x = -tPosition.x + tPosition.x * pObject->getRatio ( ).x + pObject->getOffset ( ).x;
			KDfloat y = -tPosition.y + tPosition.y * pObject->getRatio ( ).y + pObject->getOffset ( ).y;			

			pObject->getChild ( )->setPosition ( ccp ( x, y ) );
		}

		m_tLastPosition = tPosition;
	}

	CCNode::visit ( );
}

KDvoid CCParallaxNode::incrementOffset ( CCPoint tOffset, CCNode* pNode )
{
	for ( KDuint i = 0; i < m_pParallaxArray->num; i++ )
	{
		CCPointObject*  pPoint   = (CCPointObject*) m_pParallaxArray->arr [ i ];
		CCNode*			pCurNode = pPoint->getChild ( );

		if ( pCurNode->isEqual ( pNode ) )
		{
			pPoint->setOffset ( ccpAdd ( pPoint->getOffset ( ), tOffset ) );
			break;
		}
	}
}

NS_CC_END