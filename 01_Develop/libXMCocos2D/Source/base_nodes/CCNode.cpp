/* -----------------------------------------------------------------------------------
 *
 *      File            CCNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2009      Valentin Milea
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
#include "base_nodes/CCNode.h"
#include "actions/CCActionManager.h"
#include "cocoa/CCString.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "effects/CCGrid.h"
#include "support/TransformUtils.h"
#include "support/CCCamera.h"
#include "support/CCDirector.h"
#include "support/CCEGLView.h"
#include "support/CCScheduler.h"
#include "support/CCPointExtension.h"
#include "support/component/CCComponent.h"
#include "support/component/CCComponentContainer.h"
#include "script_support/CCScriptSupport.h"
#include "kazmath/GL/matrix.h"

#if CC_NODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__)	( kdCeilf ( __ARGS__ ) )
#endif

NS_CC_BEGIN
    
// XXX: Yes, nodes might have a sort problem once every 15 days if the game runs at 60 FPS and each frame sprites are reordered.
static KDint  l_nGlobalOrderOfArrival = 1;

CCNode::CCNode ( KDvoid )
{
    m_tRotation						= ccp ( 0, 0 );
    m_tScale						= ccp ( 1, 1 );
    m_fVertexZ						= 0.0f;
    m_tPosition						= ccp ( 0, 0 );
    m_tSkew							= ccp ( 0, 0 );
	m_tAnchorPointInPoints			= ccp ( 0, 0 );
    m_tAnchorPoint					= ccp ( 0, 0 );
    m_tContentSize					= ccs ( 0, 0 );
	m_tAdditionalTransform			= CCAffineTransformMakeIdentity ( );
    m_pCamera						= KD_NULL;
    m_pGrid							= KD_NULL;
    m_nZOrder						= 0;
    m_pChildren						= KD_NULL;
    m_pParent						= KD_NULL;
    m_nTag							= kCCNodeTagInvalid;
    m_pUserData						= KD_NULL;
	m_pUserObject					= KD_NULL;
	m_pShaderProgram				= KD_NULL;
	m_eGLServerState				= ccGLServerState ( 0 );
	m_uOrderOfArrival				= 0;
    m_bRunning						= KD_FALSE;
    m_bTransformDirty				= KD_TRUE;
    m_bInverseDirty					= KD_TRUE;
	m_bAdditionalTransformDirty		= KD_FALSE;
    m_bVisible						= KD_TRUE;
	m_bIgnoreAnchorPointForPosition = KD_FALSE;
	m_bReorderChildDirty			= KD_FALSE;
	m_nScriptHandler				= 0;
	m_nUpdateScriptHandler			= 0;
	m_pComponentContainer			= KD_NULL;
	m_bDidEnter						= KD_FALSE;
	m_bDidEnterTransition			= KD_FALSE;
	m_bIsClipping					= KD_FALSE;
	m_bIsDebugDraw					= KD_FALSE;

	// set default scheduler and actionManager
    CCDirector*  pDirector = CCDirector::sharedDirector ( );
    
	m_pActionManager = pDirector->getActionManager ( );
    m_pActionManager->retain ( );

    m_pScheduler = pDirector->getScheduler ( );
    m_pScheduler->retain ( );

    CCScriptEngineProtocol*  pEngine = CCScriptEngineManager::sharedManager ( )->getScriptEngine ( );
    m_eScriptType = pEngine != KD_NULL ? pEngine->getScriptType ( ) : kScriptTypeNone;

	m_pComponentContainer = new CCComponentContainer ( this );
}

CCNode::~CCNode ( KDvoid )
{
	CCLOGINFO ( "XMCocos2d : deallocing CCNode = %p", this );

	unregisterScriptHandler ( );

    if ( m_nUpdateScriptHandler )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->removeScriptHandler ( m_nUpdateScriptHandler );
    }

	CC_SAFE_RELEASE ( m_pActionManager );
    CC_SAFE_RELEASE ( m_pScheduler );

	CC_SAFE_RELEASE ( m_pCamera );
	CC_SAFE_RELEASE ( m_pGrid );

	CC_SAFE_RELEASE ( m_pShaderProgram );
    CC_SAFE_RELEASE ( m_pUserObject );

	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
        CCObject*  pObject;
        CCARRAY_FOREACH ( m_pChildren, pObject )
        {
            CCNode*  pNode = (CCNode*) pObject;
            if ( pNode )
            {
                pNode->m_pParent = KD_NULL;
            }
        }
	}

	// children
	CC_SAFE_RELEASE ( m_pChildren );

	// m_pComsContainer
    m_pComponentContainer->removeAll ( );
    CC_SAFE_DELETE ( m_pComponentContainer );
}

KDbool CCNode::init ( KDvoid )
{
    return KD_TRUE;
}

KDfloat CCNode::getSkewX ( KDvoid )
{
	return m_tSkew.x;
}

KDvoid CCNode::setSkewX ( KDfloat fSkewX )
{
	m_tSkew.x = fSkewX;
	m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

KDfloat CCNode::getSkewY ( KDvoid )
{
	return m_tSkew.y;
}

KDvoid CCNode::setSkewY ( KDfloat fSkewY )
{
	m_tSkew.y = fSkewY;
	m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

/// zOrder getter
KDint CCNode::getZOrder ( KDvoid )
{
	return m_nZOrder;
}

KDvoid CCNode::_setZOrder ( KDint z )
{
    m_nZOrder = z;
}

KDvoid CCNode::setZOrder ( KDint nZOrder )
{
	_setZOrder ( nZOrder);
    if ( m_pParent )
    {
        m_pParent->reorderChild ( this, nZOrder );
    }
}

/// vertexZ getter
KDfloat CCNode::getVertexZ ( KDvoid )
{
	return m_fVertexZ;
}

/// vertexZ setter
KDvoid CCNode::setVertexZ ( KDfloat fVertexZ )
{
	m_fVertexZ = fVertexZ;
}

/// rotation getter
KDfloat CCNode::getRotation ( KDvoid )
{
	CCAssert ( m_tRotation.x == m_tRotation.y, "CCNode#rotation. RotationX != RotationY. Don't know which one to return" );

	return m_tRotation.x;
}

/// rotation setter
KDvoid CCNode::setRotation ( KDfloat fRotation )
{
	m_tRotation.x = m_tRotation.y = fRotation;
	m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

KDfloat CCNode::getRotationX ( KDvoid )
{
    return m_tRotation.x;
}

KDvoid CCNode::setRotationX ( KDfloat fRotationX )
{
    m_tRotation.x = fRotationX;
    m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

KDfloat CCNode::getRotationY ( KDvoid )
{
    return m_tRotation.y;
}

KDvoid CCNode::setRotationY ( KDfloat fRotationY )
{
    m_tRotation.y = fRotationY;
    m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

/// scale getter
KDfloat CCNode::getScale ( KDvoid )
{
	CCAssert ( m_tScale.x == m_tScale.y, "CCNode#scale. ScaleX != ScaleY. Don't know which one to return" );

	return m_tScale.x;
}

/// scale setter
KDvoid CCNode::setScale ( KDfloat fScale )
{
	m_tScale.x = m_tScale.y = fScale;
	m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

/// scale setter
KDvoid CCNode::setScale ( KDfloat fScaleX, KDfloat fScaleY )
{
    m_tScale.x = fScaleX;
    m_tScale.y = fScaleY;
    m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

/// scaleX getter
KDfloat CCNode::getScaleX ( KDvoid )
{
	return m_tScale.x;
}

/// scaleX setter
KDvoid CCNode::setScaleX ( KDfloat fScaleX )
{
	m_tScale.x = fScaleX;
	m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

/// scaleY getter
KDfloat CCNode::getScaleY ( KDvoid )
{
	return m_tScale.y;
}

/// scaleY setter
KDvoid CCNode::setScaleY ( KDfloat fScaleY )
{
	m_tScale.y = fScaleY;
	m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

/// position getter
const CCPoint& CCNode::getPosition ( KDvoid )
{
	return m_tPosition;
}

/// position setter
KDvoid CCNode::setPosition ( const CCPoint& tPosition )
{
	m_tPosition = tPosition;
	m_bTransformDirty = m_bInverseDirty = KD_TRUE;
}

KDvoid CCNode::setPosition ( CCNode* pParent, CCAlignment eAlign, const CCPoint& tMargin )
{
	CCAssert ( pParent != KD_NULL, "" );

	CCSize   tPSize  = pParent->getContentSize ( );
	CCSize   tNSize  =    this->getContentSize ( );
	CCPoint  tAnchor = m_bIgnoreAnchorPointForPosition ? CCPointZero : m_tAnchorPoint;
	CCPoint  tPosition;
	
	tNSize.cx *= this->getScaleX ( );
	tNSize.cy *= this->getScaleY ( );	
	
	tPosition.x = tAnchor.x * tNSize.cx;
	if ( eAlign & kCCAlignmentRight )
	{
		tPosition.x += tPSize.cx - tMargin.x - tNSize.cx;
	}
	else if ( eAlign & kCCAlignmentLeft )
	{
		tPosition.x += tMargin.x; 
	}
	else
	{	
		tPosition.x += tPSize.cx * 0.5f - tNSize.cx * 0.5f + tMargin.x;
	}	

	tPosition.y = tAnchor.y * tNSize.cy;
	if ( eAlign & kCCAlignmentTop )
	{
		tPosition.y += tPSize.cy - tMargin.y - tNSize.cy;
	}
	else if ( eAlign & kCCAlignmentBottom )
	{
		tPosition.y += tMargin.y; 
	}
	else
	{
		tPosition.y += tPSize.cy * 0.5f - tNSize.cy * 0.5f + tMargin.y;
	}
	
	this->setPosition ( tPosition );
}

KDvoid CCNode::setPositionWithParent ( CCAlignment eAlign, const CCPoint& tMargin )
{
	setPosition ( m_pParent, eAlign, tMargin );	
}

KDvoid CCNode::getPosition ( KDfloat* x, KDfloat* y )
{
    *x = m_tPosition.x;
    *y = m_tPosition.y;
}

KDfloat CCNode::getPositionX ( KDvoid )
{
    return m_tPosition.x;
}

KDfloat CCNode::getPositionY ( KDvoid )
{
    return m_tPosition.y;
}

KDvoid CCNode::setPositionX ( KDfloat x )
{
    setPosition ( ccp ( x, m_tPosition.y ) );
}

KDvoid CCNode::setPositionY ( KDfloat y )
{
    setPosition ( ccp ( m_tPosition.x, y ) );
}

KDvoid CCNode::setPosition ( KDfloat x, KDfloat y )
{
    setPosition ( ccp ( x, y ) );
}

/// children getter
CCArray* CCNode::getChildren ( KDvoid )
{
	return m_pChildren;
}

KDuint CCNode::getChildrenCount ( KDvoid ) const 
{
    return m_pChildren ? m_pChildren->count ( ) : 0;
}

/// camera getter: lazy alloc
CCCamera* CCNode::getCamera ( KDvoid )
{
	if ( !m_pCamera )
	{
		m_pCamera = new CCCamera ( );
	}
	
	return m_pCamera;
}

/// grid getter
CCGridBase* CCNode::getGrid ( KDvoid )
{
	return m_pGrid;
}

/// grid setter
KDvoid CCNode::setGrid ( CCGridBase* pGrid )
{
	CC_SAFE_RETAIN ( pGrid );
	CC_SAFE_RELEASE ( m_pGrid );
	m_pGrid = pGrid;
}

/// isVisible getter
KDbool CCNode::isVisible ( KDvoid )
{
	return m_bVisible;
}

/// isVisible setter
KDvoid CCNode::setVisible ( KDbool bIsVisible )
{
	m_bVisible = bIsVisible;
}

/// anchorPoint getter
const CCPoint& CCNode::getAnchorPointInPoints ( KDvoid )
{
	return m_tAnchorPointInPoints;
}

const CCPoint& CCNode::getAnchorPoint ( KDvoid )
{
	return m_tAnchorPoint;
}

KDvoid CCNode::setAnchorPoint ( const CCPoint& tAnchorPoint )
{
	if ( !tAnchorPoint.equals ( m_tAnchorPoint ) ) 
	{
		m_tAnchorPoint = tAnchorPoint;
		m_tAnchorPointInPoints.x = m_tAnchorPoint.x * m_tContentSize.cx;
		m_tAnchorPointInPoints.y = m_tAnchorPoint.y * m_tContentSize.cy;
		m_bTransformDirty = m_bInverseDirty = KD_TRUE;
	}
}

/// contentSize getter
const CCSize& CCNode::getContentSize ( KDvoid ) const 
{
	return m_tContentSize;
}

KDvoid CCNode::setContentSize ( const CCSize& tContentSize )
{
	if ( !tContentSize.equals ( m_tContentSize ) ) 
	{
		m_tContentSize = tContentSize;
		m_tAnchorPointInPoints.x = m_tAnchorPoint.x * m_tContentSize.cx;
		m_tAnchorPointInPoints.y = m_tAnchorPoint.y * m_tContentSize.cy;
		m_bTransformDirty = m_bInverseDirty = KD_TRUE;
	}
}

/// isRunning getter
KDbool CCNode::isRunning ( KDvoid )
{
	return m_bRunning;
}

/// parent getter
CCNode* CCNode::getParent ( KDvoid )
{
	return m_pParent;
}

/// parent setter
KDvoid CCNode::setParent ( CCNode* pParent )
{
	m_pParent = pParent;
}

/// isRelativeAnchorPoint getter
KDbool CCNode::isIgnoreAnchorPointForPosition ( KDvoid )
{
	return m_bIgnoreAnchorPointForPosition;
}

/// isRelativeAnchorPoint setter
KDvoid CCNode::ignoreAnchorPointForPosition ( KDbool bIsIgnoreAnchorPointForPosition )
{
	if ( bIsIgnoreAnchorPointForPosition != m_bIgnoreAnchorPointForPosition ) 
	{
		m_bIgnoreAnchorPointForPosition = bIsIgnoreAnchorPointForPosition;
		m_bTransformDirty = m_bInverseDirty = KD_TRUE;
	}
}

/// tag getter
KDint CCNode::getTag ( KDvoid ) const
{
	return m_nTag;
}

/// tag setter
KDvoid CCNode::setTag ( KDint nTag )
{
	m_nTag = nTag;
}

/// userData getter
KDvoid* CCNode::getUserData ( KDvoid )
{
	return m_pUserData;
}

/// userData setter
KDvoid CCNode::setUserData ( KDvoid* pUserData )
{
	m_pUserData = pUserData;
}

KDuint CCNode::getOrderOfArrival ( KDvoid )
{
    return m_uOrderOfArrival;
}

KDvoid CCNode::setOrderOfArrival ( KDuint uOrderOfArrival )
{
    m_uOrderOfArrival = uOrderOfArrival;
}

CCGLProgram* CCNode::getShaderProgram ( KDvoid )
{
    return m_pShaderProgram;
}

CCObject* CCNode::getUserObject ( KDvoid )
{
    return m_pUserObject;
}

ccGLServerState CCNode::getGLServerState ( KDvoid )
{
    return m_eGLServerState;
}

KDvoid CCNode::setGLServerState ( ccGLServerState eGLServerState )
{
    m_eGLServerState = eGLServerState;
}

KDvoid CCNode::setUserObject ( CCObject* pUserObject )
{
	CC_SAFE_RETAIN ( pUserObject );
    CC_SAFE_RELEASE ( m_pUserObject );    
    m_pUserObject = pUserObject;
}

KDvoid CCNode::setShaderProgram ( CCGLProgram* pShaderProgram )
{
	CC_SAFE_RETAIN ( pShaderProgram );
    CC_SAFE_RELEASE ( m_pShaderProgram );
    m_pShaderProgram = pShaderProgram;
}

CCRect CCNode::boundingBox ( KDvoid )
{
	CCRect  tRect = CCRect ( CCPointZero, m_tContentSize );
	return CCRectApplyAffineTransform ( tRect, nodeToParentTransform ( ) );
}

CCRect CCNode::boundingBoxToWorld ( KDvoid )
{
	CCRect  tRect = CCRect ( 0, 0, m_tContentSize.cx, m_tContentSize.cy );

	tRect = CCRectApplyAffineTransform ( tRect, nodeToWorldTransform ( ) );
	tRect = CCRect ( tRect.origin.x - 4, tRect.origin.y - 4, tRect.size.cx + 8, tRect.size.cy + 8 );

	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
        CCObject*  pObject;
        CCARRAY_FOREACH ( m_pChildren, pObject )
        {
            CCNode*  pChild = (CCNode *) pObject;
			if ( pChild && pChild->isVisible ( ) )
            {
				CCRect tChildRect = pChild->boundingBoxToWorld ( );
				tRect.unions ( tChildRect );
            }
        }
	}

	return tRect;
}

KDvoid CCNode::setClipping ( KDbool bIsClipping )
{
	m_bIsClipping = bIsClipping;
}

KDbool CCNode::isClipping ( KDvoid )
{
	return m_bIsClipping;
}

KDvoid CCNode::setDebugDraw ( KDbool bIsDebugDraw )
{
	m_bIsDebugDraw = bIsDebugDraw;
}

KDbool CCNode::isDubugDraw ( KDvoid )
{
	return m_bIsDebugDraw;
}

CCNode* CCNode::create ( KDvoid )
{
	CCNode*  pRet = new CCNode ( );
    
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

KDvoid CCNode::cleanup ( KDvoid )
{
	// actions
	this->stopAllActions ( );
	this->unscheduleAllSelectors ( );	

	if ( m_eScriptType != kScriptTypeNone )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeNodeEvent ( this, kCCNodeOnCleanup );
    }

	// timers
	arrayMakeObjectsPerformSelector ( m_pChildren, cleanup, CCNode* );
}

const KDchar* CCNode::description ( KDvoid )
{
	return CCString::createWithFormat ( "<CCNode | Tag = %d>", m_nTag )->getCString ( );
}

/// lazy allocs
KDvoid CCNode::childrenAlloc ( KDvoid )
{
    m_pChildren = CCArray::createWithCapacity ( 4 );
    m_pChildren->retain ( );
}

CCNode* CCNode::getChildByTag ( KDint nTag )
{
	CCAssert ( nTag != kCCNodeTagInvalid, "Invalid tag" );

	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
        CCObject*  pObject;
        CCARRAY_FOREACH ( m_pChildren, pObject )
        {
            CCNode*  pNode = (CCNode *) pObject;
			if ( pNode && pNode->m_nTag == nTag )
			{
				return pNode;
			}
		}
	}
	return KD_NULL;
}

//
// "add" logic MUST only be on this method
// If a class want's to extend the 'addChild' behavior it only needs
// to override this method
//
KDvoid CCNode::addChild ( CCNode* pChild, KDint nZOrder, KDint nTag )
{	
	CCAssert ( pChild != KD_NULL, "Argument must be non-nil" );
	CCAssert ( pChild->m_pParent == KD_NULL, "child already added. It can't be added again" );

	if ( !m_pChildren )
	{
		this->childrenAlloc ( );
	}

	this->insertChild ( pChild, nZOrder );

	pChild->m_nTag = nTag;

	pChild->setParent ( this );
	pChild->setOrderOfArrival ( l_nGlobalOrderOfArrival++ );

	if ( m_bRunning )
	{
		pChild->onEnter ( );
		pChild->onEnterTransitionDidFinish ( );
	}
}

KDvoid CCNode::addChild ( CCNode* pChild, KDint nZOrder )
{
	CCAssert ( pChild != KD_NULL, "Argument must be non-nil" );
	this->addChild ( pChild, nZOrder, pChild->m_nTag );
}

KDvoid CCNode::addChild ( CCNode* pChild )
{
	CCAssert ( pChild != KD_NULL, "Argument must be non-nil" );
	this->addChild ( pChild, pChild->m_nZOrder, pChild->m_nTag );
}

KDvoid CCNode::removeFromParent ( KDvoid )
{
    this->removeFromParentAndCleanup ( KD_TRUE );
}

KDvoid CCNode::removeFromParentAndCleanup ( KDbool bCleanup )
{
	if ( m_pParent != KD_NULL )
	{
		m_pParent->removeChild ( this, bCleanup );
	}
}

KDvoid CCNode::removeChild ( CCNode* pChild )
{
	this->removeChild ( pChild, KD_TRUE );
}

//
// "remove" logic MUST only be on this method
// If a class want's to extend the 'removeChild' behavior it only needs
// to override this method
//
KDvoid CCNode::removeChild ( CCNode* pChild, KDbool bCleanup )
{
	// explicit nil handling
	if ( m_pChildren == KD_NULL )
	{
		return;
	}

	if ( m_pChildren->containsObject ( pChild ) )
	{
		this->detachChild ( pChild, bCleanup );
	}
}

KDvoid CCNode::removeChildByTag ( KDint nTag )
{
	this->removeChildByTag ( nTag, KD_TRUE );
}

KDvoid CCNode::removeChildByTag ( KDint nTag, KDbool bCleanup )
{
	CCAssert ( nTag != kCCNodeTagInvalid, "Invalid tag" );

	CCNode*  pChild = this->getChildByTag ( nTag );

	if ( pChild == KD_NULL )
	{
		CCLOGINFO ( "XMCocos2D : removeChildByTag(tag = %d): child not found!", nTag );
	}
	else
	{
		this->removeChild ( pChild, bCleanup );
	}
}

KDvoid CCNode::removeAllChildren ( KDvoid )
{
    this->removeAllChildrenWithCleanup ( KD_TRUE );
}

KDvoid CCNode::removeAllChildrenWithCleanup ( KDbool bCleanup )
{
	// not using detachChild improves speed here
	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
        CCObject*  pObject;
        CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCNode*  pNode = (CCNode*) pObject;
			if ( pNode )
			{
				// IMPORTANT:
				//  -1st do onExit
				//  -2nd cleanup
				if ( m_bRunning )
				{
					pNode->onExitTransitionDidStart ( );
					pNode->onExit ( );
				}

				if ( bCleanup )
				{
					pNode->cleanup ( );
				}
				// set parent nil at the end
				pNode->setParent ( KD_NULL );
			}
		}

		m_pChildren->removeAllObjects ( );
	}	
}

KDvoid CCNode::detachChild ( CCNode* pChild, KDbool bCleanup )
{
	// IMPORTANT:
	//  -1st do onExit
	//  -2nd cleanup
	if ( m_bRunning )
	{
		pChild->onExitTransitionDidStart ( );
		pChild->onExit ( );
	}

	// If you don't do cleanup, the child's actions will not get removed and the
	// its scheduledSelectors_ dict will not get released!
	if ( bCleanup )
	{
		pChild->cleanup ( );
	}

	// set parent nil at the end
	pChild->setParent ( KD_NULL );

	m_pChildren->removeObject ( pChild );
}

/// helper used by reorderChild & add
KDvoid CCNode::insertChild ( CCNode* pChild, KDint nZOrder )
{
	m_bReorderChildDirty = KD_TRUE;
    ccArrayAppendObjectWithResize ( m_pChildren->m_pData, pChild );
    pChild->_setZOrder ( nZOrder );
}

KDvoid CCNode::reorderChild ( CCNode* pChild, KDint nZOrder )
{
	CCAssert ( pChild != KD_NULL, "Child must be non-nil" );

    m_bReorderChildDirty = KD_TRUE;
    pChild->setOrderOfArrival ( l_nGlobalOrderOfArrival++ );
    pChild->_setZOrder ( nZOrder );
}

KDvoid CCNode::sortAllChildren ( KDvoid )
{
    if ( m_bReorderChildDirty )
    {
        KDint     i, j, nLength = m_pChildren->m_pData->num;
        CCNode**  pNodes = (CCNode**) m_pChildren->m_pData->arr;
        CCNode*   pTempItem;

        // insertion sort
        for ( i = 1; i< nLength; i++)
        {
            pTempItem = pNodes [ i ];
            j = i - 1;

            //continue moving element downwards while zOrder is smaller or when zOrder is the same but mutatedIndex is smaller
            while ( j >= 0 && ( pTempItem->m_nZOrder < pNodes [ j ]->m_nZOrder ||
							  ( pTempItem->m_nZOrder== pNodes [ j ]->m_nZOrder && 
							    pTempItem->m_uOrderOfArrival < pNodes [ j ]->m_uOrderOfArrival ) ) )
            {
                pNodes [ j + 1 ] = pNodes [ j ];
                j = j - 1;
            }

            pNodes [ j + 1 ] = pTempItem;
        }

        //don't need to check children recursively, that's done in visit of each child

        m_bReorderChildDirty = KD_FALSE;
    }
}

KDvoid CCNode::draw ( KDvoid )
{
	// CCAssert ( 0, "" );
	// override me
	// Only use- this function to draw your stuff.
	// DON'T draw your stuff outside this method
}

KDvoid CCNode::visit ( KDvoid )
{
    // quick return if not visible. children won't be drawn.
    if ( !m_bVisible )
    {
        return;
    }

    kmGLPushMatrix ( );

	if ( m_pGrid && m_pGrid->isActive ( ) )
	{
		m_pGrid->beforeDraw ( this );
	}

    this->transform ( );

	if ( m_bIsClipping == KD_TRUE )
	{
		CCPoint  tStart = this->convertToWorldSpace ( CCPointZero );
		CCPoint  tEnd   = this->convertToWorldSpace ( this->getContentSize ( ) );

		glEnable ( GL_SCISSOR_TEST );
		CCEGLView::sharedOpenGLView ( )->setScissorInPoints
		(
			KD_MIN ( tStart.x, tEnd.x ) - 1.f,
			KD_MIN ( tStart.y, tEnd.y ) - 1.f,
			kdFabsf ( tStart.x - tEnd.x ) + 2.f,
			kdFabsf ( tStart.y - tEnd.y ) + 2.f
		);
	}

    CCNode*  pNode = KD_NULL;
    KDuint   i = 0;

    if ( m_pChildren && m_pChildren->count ( ) > 0 )
    {
        sortAllChildren ( );
        // draw children zOrder < 0
        ccArray*  pArrayData = m_pChildren->m_pData;
        for ( ; i < pArrayData->num; i++ )
        {
            pNode = (CCNode*) pArrayData->arr [ i ];

            if ( pNode && pNode->m_nZOrder < 0 ) 
            {
                pNode->visit ( );
            }
            else
            {
                break;
            }
        }
        // self draw
        this->draw ( );

        for ( ; i < pArrayData->num; i++ )
        {
            pNode = (CCNode*) pArrayData->arr[i];
            if ( pNode )
            {
                pNode->visit ( );
            }
        }        
    }
    else
    {
        this->draw ( );
    }

    // reset for next frame
    m_uOrderOfArrival = 0;

	if ( m_bIsClipping )
	{
		CCNode*   pParent = m_pParent;
		KDbool     bFound = KD_FALSE;
		while ( pParent )
		{
			if ( pParent->m_bIsClipping == KD_TRUE )
			{
				CCPoint  tStart = pParent->convertToWorldSpace ( CCPointZero );
				CCPoint  tEnd   = pParent->convertToWorldSpace ( this->getContentSize ( ) );

				CCEGLView::sharedOpenGLView ( )->setScissorInPoints
				(
					KD_MIN ( tStart.x, tEnd.x ) - 1.f,
					KD_MIN ( tStart.y, tEnd.y ) - 1.f,
					kdFabsf ( tStart.x - tEnd.x ) + 2.f,
					kdFabsf ( tStart.y - tEnd.y ) + 2.f
				);

				bFound = KD_TRUE;
				break;
			}

			pParent = pParent->m_pParent;
		}

		if ( bFound == KD_FALSE )
		{
			glDisable ( GL_SCISSOR_TEST );			
		}
	}

	if ( m_pGrid && m_pGrid->isActive ( ) )
	{
		m_pGrid->afterDraw ( this );
	}

    kmGLPopMatrix ( );
}

KDvoid CCNode::transformAncestors ( KDvoid )
{
	if ( m_pParent != KD_NULL )
	{
		m_pParent->transformAncestors ( );
		m_pParent->transform ( );
	}
}

KDvoid CCNode::transform ( KDvoid )
{
    kmMat4   tTransfrom4x4;

    // Convert 3x3 into 4x4 matrix
    CCAffineTransform  tTempAffine = this->nodeToParentTransform ( );
    CCAffineToGL ( &tTempAffine, tTransfrom4x4.mat );

    // Update Z vertex manually
    tTransfrom4x4.mat[14] = m_fVertexZ;

    kmGLMultMatrix ( &tTransfrom4x4 );

    // XXX: Expensive calls. Camera should be integrated into the cached affine matrix
    if ( m_pCamera && !( m_pGrid && m_pGrid->isActive ( ) ) )
    {
        KDbool  bTranslate = ( m_tAnchorPointInPoints.x != 0.0f || m_tAnchorPointInPoints.y != 0.0f );

        if ( bTranslate )
		{
            kmGLTranslatef ( RENDER_IN_SUBPIXEL ( m_tAnchorPointInPoints.x ), RENDER_IN_SUBPIXEL ( m_tAnchorPointInPoints.y ), 0 );
		}

        m_pCamera->locate ( );

        if ( bTranslate )
		{
            kmGLTranslatef ( RENDER_IN_SUBPIXEL ( -m_tAnchorPointInPoints.x ), RENDER_IN_SUBPIXEL ( -m_tAnchorPointInPoints.y ), 0 );
		}
    }
}

KDvoid CCNode::onEnter ( KDvoid )
{
	arrayMakeObjectsPerformSelector ( m_pChildren, onEnter, CCNode* );

	this->resumeSchedulerAndActions ( );

	m_bRunning  = KD_TRUE;
	m_bDidEnter = KD_TRUE;

	if ( m_eScriptType != kScriptTypeNone )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeNodeEvent ( this, kCCNodeOnEnter );
    }
}

KDvoid CCNode::onEnterTransitionDidFinish ( KDvoid )
{
	arrayMakeObjectsPerformSelector ( m_pChildren, onEnterTransitionDidFinish, CCNode* );

	m_bDidEnterTransition = KD_TRUE;

	if ( m_eScriptType == kScriptTypeJavascript )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeNodeEvent ( this, kCCNodeOnEnterTransitionDidFinish );
    }
}

KDvoid CCNode::onExitTransitionDidStart ( KDvoid )
{
    arrayMakeObjectsPerformSelector ( m_pChildren, onExitTransitionDidStart, CCNode* );

	if ( m_eScriptType == kScriptTypeJavascript )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeNodeEvent ( this, kCCNodeOnExitTransitionDidStart );
    }
}

KDvoid CCNode::onExit ( KDvoid )
{
	this->pauseSchedulerAndActions ( );

	m_bRunning = KD_FALSE;

    if ( m_eScriptType != kScriptTypeNone )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeNodeEvent ( this, kCCNodeOnExit );
    }

	arrayMakeObjectsPerformSelector ( m_pChildren, onExit, CCNode* );
}

KDvoid CCNode::registerScriptHandler ( KDint nHandler )
{
    unregisterScriptHandler ( );

    m_nScriptHandler = nHandler;

    LUALOG ( "[LUA] Add CCNode event handler: %d", m_nScriptHandler );
}

KDvoid CCNode::unregisterScriptHandler ( KDvoid )
{
    if ( m_nScriptHandler )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->removeScriptHandler ( m_nScriptHandler );        
        LUALOG ( "[LUA] Remove CCNode event handler: %d", m_nScriptHandler );
        m_nScriptHandler = 0;
    }
}

KDvoid CCNode::setActionManager ( CCActionManager* pActionManager )
{
    if ( pActionManager != m_pActionManager ) 
	{
        this->stopAllActions ( );
        CC_SAFE_RETAIN ( pActionManager );
        CC_SAFE_RELEASE ( m_pActionManager );
        m_pActionManager = pActionManager;
    }
}

CCActionManager* CCNode::getActionManager ( KDvoid )
{
    return m_pActionManager;
}

CCAction* CCNode::runAction ( CCAction* pAction )
{
	CCAssert ( pAction != KD_NULL, "Argument must be non-nil" );    
	m_pActionManager->addAction ( pAction, this, !m_bRunning );
	return pAction;
}

KDvoid CCNode::stopAllActions ( KDvoid )
{
	m_pActionManager->removeAllActionsFromTarget ( this );
}

KDvoid CCNode::stopAction ( CCAction* pAction )
{
	m_pActionManager->removeAction ( pAction );
}

KDvoid CCNode::stopActionByTag ( KDint nTag )
{
	CCAssert ( nTag != kCCActionTagInvalid, "Invalid tag" );
	m_pActionManager->removeActionByTag ( nTag, this );
}

CCAction* CCNode::getActionByTag ( KDint nTag )
{
	CCAssert ( nTag != kCCActionTagInvalid, "Invalid tag" );    
	return m_pActionManager->getActionByTag ( nTag, this );
}

KDuint CCNode::numberOfRunningActions ( KDvoid )
{
	return m_pActionManager->numberOfRunningActionsInTarget ( this );
}

/// CCNode - Callbacks

KDbool CCNode::isScheduled ( SEL_SCHEDULE pSelector )
{
	return m_pScheduler->isScheduled ( pSelector, this );
}

KDvoid CCNode::setScheduler ( CCScheduler* pScheduler )
{
    if ( pScheduler != m_pScheduler ) 
	{
        this->unscheduleAllSelectors ( );
        CC_SAFE_RETAIN ( pScheduler );
        CC_SAFE_RELEASE ( m_pScheduler );
        m_pScheduler = pScheduler;
    }
}

CCScheduler* CCNode::getScheduler ( KDvoid )
{
    return m_pScheduler;
}

KDvoid CCNode::scheduleUpdate ( KDvoid )
{
	scheduleUpdateWithPriority ( 0 );
}

KDvoid CCNode::scheduleUpdateWithPriority ( KDint nPriority )
{
	m_pScheduler->scheduleUpdateForTarget ( this, nPriority, !m_bRunning );
}

KDvoid CCNode::scheduleUpdateWithPriorityLua ( KDint nHandler, KDint nPriority )
{
    unscheduleUpdate  ();
    m_nUpdateScriptHandler = nHandler;
    m_pScheduler->scheduleUpdateForTarget ( this, nPriority, !m_bRunning );
}

KDvoid CCNode::unscheduleUpdate ( KDvoid )
{
	m_pScheduler->unscheduleUpdateForTarget ( this );
    if ( m_nUpdateScriptHandler )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->removeScriptHandler ( m_nUpdateScriptHandler );
        m_nUpdateScriptHandler = 0;
    }
}

KDvoid CCNode::schedule ( SEL_SCHEDULE pSelector )
{
	this->schedule ( pSelector, 0.0f, kCCRepeatForever, 0.0f );
}

KDvoid CCNode::schedule ( SEL_SCHEDULE pSelector, KDfloat fInterval )
{
	this->schedule ( pSelector, fInterval, kCCRepeatForever, 0.0f );
}

KDvoid CCNode::schedule ( SEL_SCHEDULE pSelector, KDfloat fInterval, KDuint uRepeat, KDfloat fDelay )
{
    CCAssert ( pSelector, "Argument must be non-nil" );
    CCAssert ( fInterval >=0, "Argument must be positive" );

    m_pScheduler->scheduleSelector ( pSelector, this, fInterval, uRepeat, fDelay, !m_bRunning );
}

KDvoid CCNode::scheduleOnce ( SEL_SCHEDULE pSelector, KDfloat fDelay )
{
    this->schedule ( pSelector, 0.0f, 0, fDelay );
}

KDvoid CCNode::unschedule ( SEL_SCHEDULE pSelector )
{
	// explicit nil handling
	if ( pSelector == 0 )
	{
		return;
	}

	m_pScheduler->unscheduleSelector ( pSelector, this );
}

KDvoid CCNode::unscheduleAllSelectors ( KDvoid )
{
	m_pScheduler->unscheduleAllForTarget ( this );
}

KDvoid CCNode::resumeSchedulerAndActions ( KDvoid )
{
	m_pScheduler->resumeTarget ( this );
	m_pActionManager->resumeTarget ( this );
}

KDvoid CCNode::pauseSchedulerAndActions ( KDvoid )
{
	m_pScheduler->pauseTarget ( this );
	m_pActionManager->pauseTarget ( this );
}

// override me
KDvoid CCNode::update ( KDfloat fDelta )
{
    if ( m_nUpdateScriptHandler )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeSchedule ( m_nUpdateScriptHandler, fDelta, this );
    }    

    if ( m_pComponentContainer && !m_pComponentContainer->isEmpty ( ) )
    {
        m_pComponentContainer->visit ( fDelta );
    }
}

CCAffineTransform CCNode::nodeToParentTransform ( KDvoid )
{
    if ( m_bTransformDirty ) 
	{
        // Translate values
        KDfloat  x = m_tPosition.x;
        KDfloat  y = m_tPosition.y;

        if ( m_bIgnoreAnchorPointForPosition )
		{
            x += m_tAnchorPointInPoints.x;
            y += m_tAnchorPointInPoints.y;
        }

        // Rotation values
        // Change rotation code to handle X and Y
		// If we skew with the exact same value for both x and y then we're simply just rotating
        KDfloat  cx = 1, sx = 0, cy = 1, sy = 0;
        if ( m_tRotation.x || m_tRotation.y )
		{
			KDfloat  fRadX = -CC_DEGREES_TO_RADIANS ( m_tRotation.x );
            KDfloat  fRadY = -CC_DEGREES_TO_RADIANS ( m_tRotation.y );

            cx = kdCosf ( fRadX );
            sx = kdSinf ( fRadX );
            cy = kdCosf ( fRadY );
            sy = kdSinf ( fRadY );
        }

        KDbool  bNeedsSkewMatrix = ( m_tSkew.x || m_tSkew.y );

        // optimization:
        // inline anchor point calculation if skew is not needed
		// Adjusted transform calculation for rotational skew
		if ( !bNeedsSkewMatrix && !m_tAnchorPointInPoints.equals ( CCPointZero ) )
		{
            x += cy * -m_tAnchorPointInPoints.x * m_tScale.x + -sx * -m_tAnchorPointInPoints.y * m_tScale.y;
            y += sy * -m_tAnchorPointInPoints.x * m_tScale.x +  cx * -m_tAnchorPointInPoints.y * m_tScale.y;
        }

        // Build Transform Matrix
		// Adjusted transform calculation for rotational skew
        m_tTransform = CCAffineTransformMake ( cy * m_tScale.x, sy * m_tScale.x,
											  -sx * m_tScale.y, cx * m_tScale.y,
											   x, y );

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if ( bNeedsSkewMatrix )
		{
            CCAffineTransform  tSkewMatrix = CCAffineTransformMake ( 1.0f, kdTanf ( CC_DEGREES_TO_RADIANS ( m_tSkew.y ) ),
																	 kdTanf ( CC_DEGREES_TO_RADIANS ( m_tSkew.x ) ), 1.0f,
																	 0.0f, 0.0f );
            m_tTransform = CCAffineTransformConcat ( tSkewMatrix, m_tTransform );

            // adjust anchor point
			if ( !m_tAnchorPointInPoints.equals ( CCPointZero ) )
			{
                m_tTransform = CCAffineTransformTranslate ( m_tTransform, -m_tAnchorPointInPoints.x, -m_tAnchorPointInPoints.y );
			}
        }

        if ( m_bAdditionalTransformDirty )
        {
            m_tTransform = CCAffineTransformConcat ( m_tTransform, m_tAdditionalTransform );
            m_bAdditionalTransformDirty = KD_FALSE;
        }

        m_bTransformDirty = KD_FALSE;
    }

    return m_tTransform;
}

KDvoid CCNode::setAdditionalTransform ( const CCAffineTransform& tAdditionalTransform )
{
    m_tAdditionalTransform = tAdditionalTransform;
    m_bTransformDirty = KD_TRUE;
    m_bAdditionalTransformDirty = KD_TRUE;
}

CCAffineTransform CCNode::parentToNodeTransform ( KDvoid )
{
	if ( m_bInverseDirty ) 
	{
		m_tInverse = CCAffineTransformInvert ( this->nodeToParentTransform ( ) );
		m_bInverseDirty = KD_FALSE;
	}

	return m_tInverse;
}

CCAffineTransform CCNode::nodeToWorldTransform ( KDvoid )
{
	CCAffineTransform  tTransform = this->nodeToParentTransform ( );
    CCNode*  pParent = m_pParent;
    
	for ( ; pParent != KD_NULL; pParent = pParent->getParent ( ) )
	{
		tTransform = CCAffineTransformConcat ( tTransform, pParent->nodeToParentTransform ( ) );
	}

	return tTransform;
}

CCAffineTransform CCNode::worldToNodeTransform ( KDvoid )
{
	return CCAffineTransformInvert ( this->nodeToWorldTransform ( ) );
}

CCPoint CCNode::convertToNodeSpace ( const CCPoint& tWorldPoint )
{
	return CCPointApplyAffineTransform ( tWorldPoint, worldToNodeTransform ( ) );
}

CCPoint CCNode::convertToWorldSpace ( const CCPoint& tNodePoint )
{
	return CCPointApplyAffineTransform ( tNodePoint, nodeToWorldTransform ( ) );
}

CCPoint CCNode::convertToEGLSpace ( const CCPoint& tNodePoint )
{
	CCEGLView*  pEGLView = CCEGLView::sharedOpenGLView ( );
	CCPoint  tWorldPoint = CCPointApplyAffineTransform ( tNodePoint, nodeToWorldTransform ( ) );
	CCRect   tViewPortRect = pEGLView->getViewPortRect ( );

	return ccp 
	(
		tWorldPoint.x * pEGLView->getScaleX ( ) + tViewPortRect.origin.x,
		tWorldPoint.y * pEGLView->getScaleY ( ) + tViewPortRect.origin.y
	);
}

CCPoint CCNode::convertToNodeSpaceAR ( const CCPoint& tWorldPoint )
{
	return ccpSub ( convertToNodeSpace ( tWorldPoint ), m_tAnchorPointInPoints );
}

CCPoint CCNode::convertToWorldSpaceAR ( const CCPoint& tNodePoint )
{
	return convertToWorldSpace ( ccpAdd ( tNodePoint, m_tAnchorPointInPoints ) );
}

CCPoint CCNode::convertToWindowSpace ( const CCPoint& tNodePoint )
{
    return CCDirector::sharedDirector ( )->convertToUI ( convertToWorldSpace ( tNodePoint ) );
}

CCPoint CCNode::convertToEGLSpaceAR ( const CCPoint& tNodePoint )
{
	return convertToEGLSpace ( ccpAdd ( tNodePoint, m_tAnchorPointInPoints ) );
}

CCPoint CCNode::convertTouchToNodeSpace ( CCTouch* pTouch )
{
	return this->convertToNodeSpace ( pTouch->getLocation ( ) );
}

CCPoint CCNode::convertTouchToNodeSpaceAR ( CCTouch* pTouch )
{
	return this->convertToNodeSpaceAR ( pTouch->getLocation ( ) );
}

KDvoid CCNode::updateTransform ( KDvoid )
{
    // Recursively iterate over children
    arrayMakeObjectsPerformSelector ( m_pChildren, updateTransform, CCNode* );
}

CCComponent* CCNode::getComponent ( const KDchar* szName ) const
{
    return m_pComponentContainer->get ( szName );
}

KDbool CCNode::addComponent ( CCComponent* pComponent )
{
    return m_pComponentContainer->add ( pComponent );
}

KDbool CCNode::removeComponent ( const KDchar* szName )
{
    return m_pComponentContainer->remove ( szName );
}

KDvoid CCNode::removeAllComponents ( KDvoid )
{
    m_pComponentContainer->removeAll ( );
}

//
// CCNodeRGBA
//

CCNodeRGBA::CCNodeRGBA ( KDvoid )
{
	m_cDisplayedOpacity		 = 255;
	m_cRealOpacity			 = 255;
	m_tDisplayedColor		 = ccWHITE;
	m_tRealColor			 = ccWHITE;
	m_bCascadeColorEnabled	 = KD_FALSE;
	m_bCascadeOpacityEnabled = KD_FALSE;
}

CCNodeRGBA::~CCNodeRGBA ( KDvoid ) 
{

}

KDbool CCNodeRGBA::init ( KDvoid )
{
    if ( CCNode::init ( ) )
    {
        m_cDisplayedOpacity = m_cRealOpacity = 255;
        m_tDisplayedColor   = m_tRealColor   = ccWHITE;
        m_bCascadeOpacityEnabled = m_bCascadeColorEnabled = KD_FALSE;
        return KD_TRUE;
    }
    return KD_FALSE;
}

CCNodeRGBA* CCNodeRGBA::create ( KDvoid )
{
	CCNodeRGBA*		pRet = new CCNodeRGBA ( );
    
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

GLubyte CCNodeRGBA::getOpacity ( KDvoid )
{
	return m_cRealOpacity;
}

GLubyte CCNodeRGBA::getDisplayedOpacity ( KDvoid )
{
	return m_cDisplayedOpacity;
}

KDvoid CCNodeRGBA::setOpacity ( GLubyte cOpacity )
{
    m_cDisplayedOpacity = m_cRealOpacity = cOpacity;
    
	if ( m_bCascadeOpacityEnabled )
    {
		GLubyte  cParentOpacity = 255;
        CCRGBAProtocol* pParent = dynamic_cast<CCRGBAProtocol*> ( m_pParent );
        if ( pParent && pParent->isCascadeOpacityEnabled ( ) )
        {
            cParentOpacity = pParent->getDisplayedOpacity ( );
        }
        this->updateDisplayedOpacity ( cParentOpacity );
	}
}

KDvoid CCNodeRGBA::updateDisplayedOpacity ( GLubyte cParentOpacity )
{
	m_cDisplayedOpacity = (GLubyte) ( m_cRealOpacity * cParentOpacity / 255.0 );
	
    if ( m_bCascadeOpacityEnabled )
    {
        CCObject*  pObj;
        CCARRAY_FOREACH ( m_pChildren, pObj )
        {
            CCRGBAProtocol*  pItem = dynamic_cast<CCRGBAProtocol*> ( pObj );
            if ( pItem )
            {
                pItem->updateDisplayedOpacity ( m_cDisplayedOpacity );
            }
        }
    }
}

KDbool CCNodeRGBA::isCascadeOpacityEnabled ( KDvoid )
{
    return m_bCascadeOpacityEnabled;
}

KDvoid CCNodeRGBA::setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled )
{
    m_bCascadeOpacityEnabled = bCascadeOpacityEnabled;
}

const ccColor3B& CCNodeRGBA::getColor ( KDvoid )
{
	return m_tRealColor;
}

const ccColor3B& CCNodeRGBA::getDisplayedColor ( KDvoid )
{
	return m_tDisplayedColor;
}

KDvoid CCNodeRGBA::setColor ( const ccColor3B& tColor )
{
	m_tDisplayedColor = m_tRealColor = tColor;
	
	if ( m_bCascadeColorEnabled )
    {
		ccColor3B   tParentColor = ccWHITE;
        CCRGBAProtocol*  pParent = dynamic_cast<CCRGBAProtocol*> ( m_pParent );
		if ( pParent && pParent->isCascadeColorEnabled ( ) )
        {
            tParentColor = pParent->getDisplayedColor ( ); 
        }
        
        updateDisplayedColor ( tParentColor );
	}
}

KDvoid CCNodeRGBA::updateDisplayedColor ( const ccColor3B& tParentColor )
{
	m_tDisplayedColor.r = (GLubyte) ( m_tRealColor.r * tParentColor.r / 255.0 );
	m_tDisplayedColor.g = (GLubyte) ( m_tRealColor.g * tParentColor.g / 255.0 );
	m_tDisplayedColor.b = (GLubyte) ( m_tRealColor.b * tParentColor.b / 255.0 );
    
    if ( m_bCascadeColorEnabled )
    {
        CCObject*  pObj = KD_NULL;
        CCARRAY_FOREACH ( m_pChildren, pObj )
        {
            CCRGBAProtocol*  pItem = dynamic_cast<CCRGBAProtocol*> ( pObj );
            if ( pItem )
            {
                pItem->updateDisplayedColor ( m_tDisplayedColor );
            }
        }
    }
}

KDbool CCNodeRGBA::isCascadeColorEnabled ( KDvoid )
{
    return m_bCascadeColorEnabled;
}

KDvoid CCNodeRGBA::setCascadeColorEnabled ( KDbool bCascadeColorEnabled )
{
    m_bCascadeColorEnabled = bCascadeColorEnabled;
}

NS_CC_END