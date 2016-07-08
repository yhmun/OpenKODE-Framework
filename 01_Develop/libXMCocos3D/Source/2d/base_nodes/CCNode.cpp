/* -----------------------------------------------------------------------------------
 *
 *      File            CCNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#include "2d/base_nodes/CCNode.h"
#include "base/CCString.h"
#include "support/data_support/ccCArray.h"
#include "support/TransformUtils.h"
#include "2d/CCCamera.h"
#include "2d/effects/CCGrid.h"
#include "2d/CCDirector.h"
#include "2d/CCScheduler.h"
#include "2d/event_dispatcher/CCTouch.h"
#include "2d/actions/CCActionManager.h"
#include "2d/script_support/CCScriptSupport.h"
#include "shaders//CCGLProgram.h"
#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "2d/event_dispatcher/CCEvent.h"
#include "2d/event_dispatcher/CCEventTouch.h"
#include "2d/layers_scenes_transitions_nodes/CCScene.h"
#include "platform/CCEGLView.h"

#ifdef CC_USE_PHYSICS
#include "physics/CCPhysicsBody.h"
#endif

// externals
#include "XMKazmath/GL/matrix.h"
#include "support/component/CCComponent.h"
#include "support/component/CCComponentContainer.h"

#include <algorithm>

#if CC_NODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__)	( kdCeilf ( __ARGS__ ) )
#endif

NS_CC_BEGIN

#if CC_USE_ARRAY_VECTOR
bool nodeComparisonLess(const RCPtr<Object>& pp1, const RCPtr<Object>& pp2)
{
    Object *p1 = static_cast<Object*>(pp1);
    Object *p2 = static_cast<Object*>(pp2);
    Node *n1 = static_cast<Node*>(p1);
    Node *n2 = static_cast<Node*>(p2);

    return( n1->getZOrder() < n2->getZOrder() ||
           ( n1->getZOrder() == n2->getZOrder() && n1->getOrderOfArrival() < n2->getOrderOfArrival() )
           );
}
#else
bool nodeComparisonLess(Object* p1, Object* p2)
{
    Node *n1 = static_cast<Node*>(p1);
    Node *n2 = static_cast<Node*>(p2);

    return( n1->getZOrder() < n2->getZOrder() ||
           ( n1->getZOrder() == n2->getZOrder() && n1->getOrderOfArrival() < n2->getOrderOfArrival() )
           );
}
#endif

// XXX: Yes, nodes might have a sort problem once every 15 days if the game runs at 60 FPS and each frame sprites are reordered.
static int s_globalOrderOfArrival = 1;

Node::Node(void)
: m_fRotationX(0.0f)
, m_fRotationY(0.0f)
, m_fScaleX(1.0f)
, m_fScaleY(1.0f)
, m_fVertexZ(0.0f)
, m_tPosition(Point::ZERO)
, m_fSkewX(0.0f)
, m_fSkewY(0.0f)
, m_tAnchorPointInPoints(Point::ZERO)
, m_tAnchorPoint(Point::ZERO)
, m_tContentSize(Size::ZERO)
, m_tAdditionalTransform(AffineTransform::IDENTITY)
, m_tTransform(AffineTransform::IDENTITY)
, m_tInverse(AffineTransform::IDENTITY)
, m_bAdditionalTransformDirty(false)
, m_bTransformDirty(true)
, m_bInverseDirty(true)
, m_pCamera(NULL)
// children (lazy allocs)
// lazy alloc
, m_pGrid(NULL)
, m_nZOrder(0)
, m_pChildren(NULL)
, m_pParent(NULL)
// "whole screen" objects. like Scenes and Layers, should set _ignoreAnchorPointForPosition to true
, m_nTag(Node::INVALID_TAG)
// userData is always inited as nil
, m_pUserData(NULL)
, m_pUserObject(NULL)
, m_pShaderProgram(NULL)
, m_nOrderOfArrival(0)
, m_bRunning(false)
, m_bVisible(true)
, m_bIgnoreAnchorPointForPosition(false)
, m_bReorderChildDirty(false)
, m_bIsTransitionFinished(false)
, m_nUpdateScriptHandler(0)
, m_pComponentContainer(NULL)
#ifdef CC_USE_PHYSICS
, m_pPhysicsBody(nullptr)
#endif
{
    // set default scheduler and actionManager
    Director *director = Director::getInstance();
    m_pActionManager = director->getActionManager();
    m_pActionManager->retain();
    m_pScheduler = director->getScheduler();
    m_pScheduler->retain();
    m_pEventDispatcher = director->getEventDispatcher();
    m_pEventDispatcher->retain();
    
    ScriptEngineProtocol* pEngine = ScriptEngineManager::getInstance()->getScriptEngine();
    m_eScriptType = pEngine != NULL ? pEngine->getScriptType() : kScriptTypeNone;

	m_bIsClipping = KD_FALSE;
}

Node::~Node()
{
    CCLOGINFO( "deallocing Node: %p - tag: %i", this, m_nTag );
    
    if (m_nUpdateScriptHandler)
    {
        ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(m_nUpdateScriptHandler);
    }
    
    m_pEventDispatcher->cleanTarget(this);
        
    // attributes
    CC_SAFE_RELEASE(m_pCamera);

    CC_SAFE_RELEASE(m_pGrid);
    CC_SAFE_RELEASE(m_pShaderProgram);
    CC_SAFE_RELEASE(m_pUserObject);

    if(m_pChildren && m_pChildren->count() > 0)
    {
        Object* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            Node* node = static_cast<Node*>(child);
            if (node)
            {
                node->m_pParent = NULL;
            }
        }
    }

    // children
    CC_SAFE_RELEASE(m_pChildren);
    
    removeAllComponents();
    
    CC_SAFE_DELETE(m_pComponentContainer);
    


#ifdef CC_USE_PHYSICS
    CC_SAFE_RELEASE(m_pPhysicsBody);
#endif

    CC_SAFE_RELEASE(m_pActionManager);
    CC_SAFE_RELEASE(m_pScheduler);
	CC_SAFE_RELEASE(m_pEventDispatcher);
}

bool Node::init()
{
    return true;
}

float Node::getSkewX() const
{
    return m_fSkewX;
}

void Node::setSkewX(float newSkewX)
{
    m_fSkewX = newSkewX;
    m_bTransformDirty = m_bInverseDirty = true;
}

float Node::getSkewY() const
{
    return m_fSkewY;
}

void Node::setSkewY(float newSkewY)
{
    m_fSkewY = newSkewY;

    m_bTransformDirty = m_bInverseDirty = true;
}

/// zOrder getter
int Node::getZOrder() const
{
    return m_nZOrder;
}

/// zOrder setter : private method
/// used internally to alter the zOrder variable. DON'T call this method manually 
void Node::_setZOrder(int z)
{
    m_nZOrder = z;
}

void Node::setZOrder(int z)
{
    if (m_pParent)
    {
        m_pParent->reorderChild(this, z);
    }
    // should set "_ZOrder" after reorderChild, because the implementation of reorderChild subclass of Node, such as Sprite,
    // will return when _ZOrder value is not changed
    _setZOrder(z);
    
    m_pEventDispatcher->setDirtyForNode(this);
}

/// vertexZ getter
float Node::getVertexZ() const
{
    return m_fVertexZ;
}


/// vertexZ setter
void Node::setVertexZ(float var)
{
    m_fVertexZ = var;
}


/// rotation getter
float Node::getRotation() const
{
    CCASSERT(m_fRotationX == m_fRotationY, "CCNode#rotation. RotationX != RotationY. Don't know which one to return");
    return m_fRotationX;
}

/// rotation setter
void Node::setRotation(float newRotation)
{
    m_fRotationX = m_fRotationY = newRotation;
    m_bTransformDirty = m_bInverseDirty = true;
    
#ifdef CC_USE_PHYSICS
    if (m_pPhysicsBody)
    {
        m_pPhysicsBody->setRotation(newRotation);
    }
#endif
}

float Node::getRotationX() const
{
    return m_fRotationX;
}

void Node::setRotationX(float fRotationX)
{
    m_fRotationX = fRotationX;
    m_bTransformDirty = m_bInverseDirty = true;
}

float Node::getRotationY() const
{
    return m_fRotationY;
}

void Node::setRotationY(float fRotationY)
{
    m_fRotationY = fRotationY;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scale getter
float Node::getScale(void) const
{
    CCASSERT( m_fScaleX == m_fScaleY, "CCNode#scale. ScaleX != ScaleY. Don't know which one to return");
    return m_fScaleX;
}

/// scale setter
void Node::setScale(float scale)
{
    m_fScaleX = m_fScaleY = scale;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleX getter
float Node::getScaleX() const
{
    return m_fScaleX;
}

/// scale setter
void Node::setScale(float scaleX,float scaleY)
{
    m_fScaleX = scaleX;
    m_fScaleY = scaleY;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleX setter
void Node::setScaleX(float newScaleX)
{
    m_fScaleX = newScaleX;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleY getter
float Node::getScaleY() const
{
    return m_fScaleY;
}

/// scaleY setter
void Node::setScaleY(float newScaleY)
{
    m_fScaleY = newScaleY;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// position getter
const Point& Node::getPosition() const
{
    return m_tPosition;
}

/// position setter
void Node::setPosition(const Point& newPosition)
{
    m_tPosition = newPosition;
    m_bTransformDirty = m_bInverseDirty = true;
    
#ifdef CC_USE_PHYSICS
    if (m_pPhysicsBody)
    {
        m_pPhysicsBody->setPosition(newPosition);
    }
#endif
}

void Node::getPosition(float* x, float* y) const
{
    *x = m_tPosition.x;
    *y = m_tPosition.y;
}

void Node::setPosition(float x, float y)
{
    setPosition(Point(x, y));
}

float Node::getPositionX() const
{
    return m_tPosition.x;
}

float Node::getPositionY() const
{
    return  m_tPosition.y;
}

void Node::setPositionX(float x)
{
    setPosition(Point(x, m_tPosition.y));
}

void Node::setPositionY(float y)
{
    setPosition(Point(m_tPosition.x, y));
}

KDint32 Node::getChildrenCount ( KDvoid ) const
{
    return m_pChildren ? m_pChildren->count ( ) : 0;
}

/// camera getter: lazy alloc
Camera* Node::getCamera()
{
    if (!m_pCamera)
    {
        m_pCamera = new Camera();
    }
    
    return m_pCamera;
}

/// grid setter
void Node::setGrid(GridBase* pGrid)
{
    CC_SAFE_RETAIN(pGrid);
    CC_SAFE_RELEASE(m_pGrid);
    m_pGrid = pGrid;
}


/// isVisible getter
bool Node::isVisible() const
{
    return m_bVisible;
}

/// isVisible setter
void Node::setVisible(bool var)
{
    m_bVisible = var;
}

const Point& Node::getAnchorPointInPoints() const
{
    return m_tAnchorPointInPoints;
}

/// anchorPoint getter
const Point& Node::getAnchorPoint() const
{
    return m_tAnchorPoint;
}

void Node::setAnchorPoint(const Point& point)
{
    if( ! point.equals(m_tAnchorPoint))
    {
        m_tAnchorPoint = point;
        m_tAnchorPointInPoints = Point(m_tContentSize.width * m_tAnchorPoint.x, m_tContentSize.height * m_tAnchorPoint.y );
        m_bTransformDirty = m_bInverseDirty = true;
    }
}

/// contentSize getter
const Size& Node::getContentSize() const
{
    return m_tContentSize;
}

void Node::setContentSize(const Size & size)
{
    if ( ! size.equals(m_tContentSize))
    {
        m_tContentSize = size;

        m_tAnchorPointInPoints = Point(m_tContentSize.width * m_tAnchorPoint.x, m_tContentSize.height * m_tAnchorPoint.y );
        m_bTransformDirty = m_bInverseDirty = true;
    }
}

// isRunning getter
bool Node::isRunning() const
{
    return m_bRunning;
}

/// parent setter
void Node::setParent(Node * var)
{
    m_pParent = var;
}

/// isRelativeAnchorPoint getter
bool Node::isIgnoreAnchorPointForPosition() const
{
    return m_bIgnoreAnchorPointForPosition;
}
/// isRelativeAnchorPoint setter
void Node::ignoreAnchorPointForPosition(bool newValue)
{
    if (newValue != m_bIgnoreAnchorPointForPosition) 
    {
		m_bIgnoreAnchorPointForPosition = newValue;
		m_bTransformDirty = m_bInverseDirty = true;
	}
}

/// tag getter
int Node::getTag() const
{
    return m_nTag;
}

/// tag setter
void Node::setTag(int var)
{
    m_nTag = var;
}

/// userData setter
void Node::setUserData(void *var)
{
    m_pUserData = var;
}

int Node::getOrderOfArrival() const
{
    return m_nOrderOfArrival;
}

void Node::setOrderOfArrival(int orderOfArrival)
{
    CCASSERT(orderOfArrival >=0, "Invalid orderOfArrival");
    m_nOrderOfArrival = orderOfArrival;
}

void Node::setUserObject(Object *pUserObject)
{
    CC_SAFE_RETAIN(pUserObject);
    CC_SAFE_RELEASE(m_pUserObject);
    m_pUserObject = pUserObject;
}

void Node::setShaderProgram(GLProgram *pShaderProgram)
{
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
}

Rect Node::getBoundingBox() const
{
    Rect rect = Rect(0, 0, m_tContentSize.width, m_tContentSize.height);
    return RectApplyAffineTransform(rect, getNodeToParentTransform());
}

Node * Node::create(void)
{
	Node * pRet = new Node();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

void Node::cleanup()
{
    // actions
    this->stopAllActions();
    this->unscheduleAllSelectors();
    
    if ( m_eScriptType != kScriptTypeNone)
    {
        int action = kNodeOnCleanup;
        BasicScriptData data(this,(void*)&action);
        ScriptEvent scriptEvent(kNodeEvent,(void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
    }
    
    // timers
    arrayMakeObjectsPerformSelector(m_pChildren, cleanup, Node*);
}


const char* Node::description() const
{
    return String::createWithFormat("<Node | Tag = %d>", m_nTag)->getCString();
}

// lazy allocs
void Node::childrenAlloc(void)
{
    m_pChildren = Array::createWithCapacity(4);
    m_pChildren->retain();
}

Node* Node::getChildByTag(int aTag)
{
    CCASSERT( aTag != Node::INVALID_TAG, "Invalid tag");

    if(m_pChildren && m_pChildren->count() > 0)
    {
        Object* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            Node* pNode = static_cast<Node*>(child);
            if(pNode && pNode->m_nTag == aTag)
                return pNode;
        }
    }
    return NULL;
}

/* "add" logic MUST only be on this method
* If a class want's to extend the 'addChild' behavior it only needs
* to override this method
*/
void Node::addChild(Node *child, int zOrder, int tag)
{    
    CCASSERT( child != NULL, "Argument must be non-nil");
    CCASSERT( child->m_pParent == NULL, "child already added. It can't be added again");

    if( ! m_pChildren )
    {
        this->childrenAlloc();
    }

    this->insertChild(child, zOrder);
    
#ifdef CC_USE_PHYSICS
    for (Node* node = this->getParent(); node != nullptr; node = node->getParent())
    {
        if (dynamic_cast<Scene*>(node) != nullptr)
        {
            (dynamic_cast<Scene*>(node))->addChildToPhysicsWorld(child);
            break;
        }
    }
#endif

    child->m_nTag = tag;

    child->setParent(this);
    child->setOrderOfArrival(s_globalOrderOfArrival++);

    if( m_bRunning )
    {
        child->onEnter();
        // prevent onEnterTransitionDidFinish to be called twice when a node is added in onEnter
        if (m_bIsTransitionFinished) {
            child->onEnterTransitionDidFinish();
        }
    }
}

void Node::addChild(Node *child, int zOrder)
{
    CCASSERT( child != NULL, "Argument must be non-nil");
    this->addChild(child, zOrder, child->m_nTag);
}

void Node::addChild(Node *child)
{
    CCASSERT( child != NULL, "Argument must be non-nil");
    this->addChild(child, child->m_nZOrder, child->m_nTag);
}

void Node::removeFromParent()
{
    this->removeFromParentAndCleanup(true);
}

void Node::removeFromParentAndCleanup(bool cleanup)
{
    if (m_pParent != NULL)
    {
        m_pParent->removeChild(this,cleanup);
    } 
}

/* "remove" logic MUST only be on this method
* If a class want's to extend the 'removeChild' behavior it only needs
* to override this method
*/
void Node::removeChild(Node* child, bool cleanup /* = true */)
{
    // explicit nil handling
    if (m_pChildren == NULL)
    {
        return;
    }

    long index = m_pChildren->getIndexOfObject(child);
    if( index != CC_INVALID_INDEX )
        this->detachChild( child, index, cleanup );
}

void Node::removeChildByTag(int tag, bool cleanup/* = true */)
{
    CCASSERT( tag != Node::INVALID_TAG, "Invalid tag");

    Node *child = this->getChildByTag(tag);

    if (child == NULL)
    {
        CCLOG("cocos2d: removeChildByTag(tag = %d): child not found!", tag);
    }
    else
    {
        this->removeChild(child, cleanup);
    }
}

void Node::removeAllChildren()
{
    this->removeAllChildrenWithCleanup(true);
}

void Node::removeAllChildrenWithCleanup(bool cleanup)
{
    // not using detachChild improves speed here
    if ( m_pChildren && m_pChildren->count() > 0 )
    {
        Object* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            Node* pNode = static_cast<Node*>(child);
            if (pNode)
            {
                // IMPORTANT:
                //  -1st do onExit
                //  -2nd cleanup
                if(m_bRunning)
                {
                    pNode->onExitTransitionDidStart();
                    pNode->onExit();
                }

                if (cleanup)
                {
                    pNode->cleanup();
                }
                // set parent nil at the end
                pNode->setParent(NULL);
            }
        }
        
        m_pChildren->removeAllObjects();
    }
    
}

KDvoid Node::detachChild ( Node* pChild, KDint32 nChildIndex, KDbool bDoCleanup )
{
    // IMPORTANT:
    //  -1st do onExit
    //  -2nd cleanup
    if (m_bRunning)
    {
        pChild->onExitTransitionDidStart();
        pChild->onExit();
    }
    
#ifdef CC_USE_PHYSICS
    if (pChild->m_pPhysicsBody != nullptr)
    {
        pChild->m_pPhysicsBody->removeFromWorld();
    }
    
#endif

    // If you don't do cleanup, the child's actions will not get removed and the
    // its scheduledSelectors_ dict will not get released!
    if (bDoCleanup)
    {
        pChild->cleanup();
    }

    // set parent nil at the end
    pChild->setParent(NULL);

    m_pChildren->removeObjectAtIndex(nChildIndex);
}


// helper used by reorderChild & add
void Node::insertChild(Node* child, int z)
{
    m_bReorderChildDirty = true;
    m_pChildren->addObject(child);
    child->_setZOrder(z);
}

void Node::reorderChild(Node *child, int zOrder)
{
    CCASSERT( child != NULL, "Child must be non-nil");
    m_bReorderChildDirty = true;
    child->setOrderOfArrival(s_globalOrderOfArrival++);
    child->_setZOrder(zOrder);
}

void Node::sortAllChildren()
{
#if 0
    if (_reorderChildDirty)
    {
        int i,j,length = _children->count();

        // insertion sort
        for(i=1; i<length; i++)
        {
            j = i-1;
            auto tempI = static_cast<Node*>( _children->getObjectAtIndex(i) );
            auto tempJ = static_cast<Node*>( _children->getObjectAtIndex(j) );

            //continue moving element downwards while zOrder is smaller or when zOrder is the same but mutatedIndex is smaller
            while(j>=0 && ( tempI->_ZOrder < tempJ->_ZOrder || ( tempI->_ZOrder == tempJ->_ZOrder && tempI->_orderOfArrival < tempJ->_orderOfArrival ) ) )
            {
                _children->fastSetObject( tempJ, j+1 );
                j = j-1;
                if(j>=0)
                    tempJ = static_cast<Node*>( _children->getObjectAtIndex(j) );
            }
            _children->fastSetObject(tempI, j+1);
        }

        //don't need to check children recursively, that's done in visit of each child

        _reorderChildDirty = false;
    }
#else
    if( m_bReorderChildDirty ) {
        std::sort( std::begin(*m_pChildren), std::end(*m_pChildren), nodeComparisonLess );
        m_bReorderChildDirty = false;
    }
#endif
}


 void Node::draw()
 {
     //CCASSERT(0);
     // override me
     // Only use- this function to draw your stuff.
     // DON'T draw your stuff outside this method
 }

void Node::visit()
{
    // quick return if not visible. children won't be drawn.
    if (!m_bVisible)
    {
        return;
    }
    
    kmGLPushMatrix();

     if (m_pGrid && m_pGrid->isActive())
     {
         m_pGrid->beforeDraw();
     }

    this->transform();

	if ( m_bIsClipping == KD_TRUE )
	{
		Point  tStart = this->convertToWorldSpace ( Point::ZERO );
		Point  tEnd   = this->convertToWorldSpace ( Point( m_tContentSize ) );

		glEnable ( GL_SCISSOR_TEST );
		EGLView::getInstance ( )->setScissorInPoints
		(
			KD_MIN ( tStart.x, tEnd.x ) - 1.f,
			KD_MIN ( tStart.y, tEnd.y ) - 1.f,
			kdFabsf ( tStart.x - tEnd.x ) + 2.f,
			kdFabsf ( tStart.y - tEnd.y ) + 2.f
		);
	}

    int i = 0;

    if(m_pChildren && m_pChildren->count() > 0)
    {
        sortAllChildren();
        // draw children zOrder < 0
        for( ; i < m_pChildren->count(); i++ )
        {
            auto node = static_cast<Node*>( m_pChildren->getObjectAtIndex(i) );

            if ( node && node->m_nZOrder < 0 )
                node->visit();
            else
                break;
        }
        // self draw
        this->draw();

        for( ; i < m_pChildren->count(); i++ )
        {
            auto node = static_cast<Node*>( m_pChildren->getObjectAtIndex(i) );
            if (node)
                node->visit();
        }
    }
    else
    {
        this->draw();
    }

    // reset for next frame
    m_nOrderOfArrival = 0;

	if ( m_bIsClipping )
	{
		Node*	pParent = m_pParent;
		KDbool	bFound  = KD_FALSE;
		while ( pParent )
		{
			if ( pParent->m_bIsClipping == KD_TRUE )
			{
				Point  tStart = pParent->convertToWorldSpace ( Point::ZERO );
				Point  tEnd   = pParent->convertToWorldSpace ( Point ( m_tContentSize ) );

				EGLView::getInstance ( )->setScissorInPoints
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

     if (m_pGrid && m_pGrid->isActive())
     {
         m_pGrid->afterDraw(this);
    }
 
    kmGLPopMatrix();
}

void Node::transformAncestors()
{
    if( m_pParent != NULL  )
    {
        m_pParent->transformAncestors();
        m_pParent->transform();
    }
}

void Node::transform()
{
#ifdef CC_USE_PHYSICS
    updatePhysicsTransform();
#endif

    kmMat4 transfrom4x4;

    // Convert 3x3 into 4x4 matrix
    CGAffineToGL(this->getNodeToParentTransform(), transfrom4x4.mat);

    // Update Z vertex manually
    transfrom4x4.mat[14] = m_fVertexZ;

    kmGLMultMatrix( &transfrom4x4 );


    // XXX: Expensive calls. Camera should be integrated into the cached affine matrix
    if ( m_pCamera != NULL && !(m_pGrid != NULL && m_pGrid->isActive()) )
    {
        bool translate = (m_tAnchorPointInPoints.x != 0.0f || m_tAnchorPointInPoints.y != 0.0f);

        if( translate )
            kmGLTranslatef(RENDER_IN_SUBPIXEL(m_tAnchorPointInPoints.x), RENDER_IN_SUBPIXEL(m_tAnchorPointInPoints.y), 0 );

        m_pCamera->locate();

        if( translate )
            kmGLTranslatef(RENDER_IN_SUBPIXEL(-m_tAnchorPointInPoints.x), RENDER_IN_SUBPIXEL(-m_tAnchorPointInPoints.y), 0 );
    }

}


void Node::onEnter()
{
    m_bIsTransitionFinished = false;

    arrayMakeObjectsPerformSelector(m_pChildren, onEnter, Node*);

    this->resume();
    
    m_bRunning = true;

    if (m_eScriptType != kScriptTypeNone)
    {
        int action = kNodeOnEnter;
        BasicScriptData data(this,(void*)&action);
        ScriptEvent scriptEvent(kNodeEvent,(void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
    }
}

void Node::onEnterTransitionDidFinish()
{
    m_bIsTransitionFinished = true;

    arrayMakeObjectsPerformSelector(m_pChildren, onEnterTransitionDidFinish, Node*);

    if (m_eScriptType != kScriptTypeNone)
    {
        int action = kNodeOnEnterTransitionDidFinish;
        BasicScriptData data(this,(void*)&action);
        ScriptEvent scriptEvent(kNodeEvent,(void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
    }
}

void Node::onExitTransitionDidStart()
{
    arrayMakeObjectsPerformSelector(m_pChildren, onExitTransitionDidStart, Node*);
    if (m_eScriptType != kScriptTypeNone)
    {
        int action = kNodeOnExitTransitionDidStart;
        BasicScriptData data(this,(void*)&action);
        ScriptEvent scriptEvent(kNodeEvent,(void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
    }
}

void Node::onExit()
{
    this->pause();

    m_bRunning = false;
    if (m_eScriptType != kScriptTypeNone)
    {
        int action = kNodeOnExit;
        BasicScriptData data(this,(void*)&action);
        ScriptEvent scriptEvent(kNodeEvent,(void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
    }

    arrayMakeObjectsPerformSelector(m_pChildren, onExit, Node*);
}

void Node::setEventDispatcher(EventDispatcher* dispatcher)
{
    if (dispatcher != m_pEventDispatcher)
    {
        m_pEventDispatcher->cleanTarget(this);
        CC_SAFE_RETAIN(dispatcher);
        CC_SAFE_RELEASE(m_pEventDispatcher);
        m_pEventDispatcher = dispatcher;
    }
}

void Node::setActionManager(ActionManager* actionManager)
{
    if( actionManager != m_pActionManager ) {
        this->stopAllActions();
        CC_SAFE_RETAIN(actionManager);
        CC_SAFE_RELEASE(m_pActionManager);
        m_pActionManager = actionManager;
    }
}

Action * Node::runAction(Action* action)
{
    CCASSERT( action != NULL, "Argument must be non-nil");
    m_pActionManager->addAction(action, this, !m_bRunning);
    return action;
}

void Node::stopAllActions()
{
    m_pActionManager->removeAllActionsFromTarget(this);
}

void Node::stopAction(Action* action)
{
    m_pActionManager->removeAction(action);
}

void Node::stopActionByTag(int tag)
{
    CCASSERT( tag != Action::INVALID_TAG, "Invalid tag");
    m_pActionManager->removeActionByTag(tag, this);
}

Action * Node::getActionByTag(int tag)
{
    CCASSERT( tag != Action::INVALID_TAG, "Invalid tag");
    return m_pActionManager->getActionByTag(tag, this);
}

unsigned int Node::getNumberOfRunningActions() const
{
    return m_pActionManager->getNumberOfRunningActionsInTarget(this);
}

// Node - Callbacks

void Node::setScheduler(Scheduler* scheduler)
{
    if( scheduler != m_pScheduler ) {
        this->unscheduleAllSelectors();
        CC_SAFE_RETAIN(scheduler);
        CC_SAFE_RELEASE(m_pScheduler);
        m_pScheduler = scheduler;
    }
}

bool Node::isScheduled(SEL_SCHEDULE selector)
{
    return m_pScheduler->isScheduledForTarget(selector, this);
}

void Node::scheduleUpdate()
{
    scheduleUpdateWithPriority(0);
}

void Node::scheduleUpdateWithPriority(int priority)
{
    m_pScheduler->scheduleUpdateForTarget(this, priority, !m_bRunning);
}

void Node::scheduleUpdateWithPriorityLua(int nHandler, int priority)
{
    unscheduleUpdate();
    m_nUpdateScriptHandler = nHandler;
    m_pScheduler->scheduleUpdateForTarget(this, priority, !m_bRunning);
}

void Node::unscheduleUpdate()
{
    m_pScheduler->unscheduleUpdateForTarget(this);
    if (m_nUpdateScriptHandler)
    {
        ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(m_nUpdateScriptHandler);
        m_nUpdateScriptHandler = 0;
    }
}

void Node::schedule(SEL_SCHEDULE selector)
{
    this->schedule(selector, 0.0f, kRepeatForever, 0.0f);
}

void Node::schedule(SEL_SCHEDULE selector, float interval)
{
    this->schedule(selector, interval, kRepeatForever, 0.0f);
}

void Node::schedule(SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay)
{
    CCASSERT( selector, "Argument must be non-nil");
    CCASSERT( interval >=0, "Argument must be positive");

    m_pScheduler->scheduleSelector(selector, this, interval , repeat, delay, !m_bRunning);
}

void Node::scheduleOnce(SEL_SCHEDULE selector, float delay)
{
    this->schedule(selector, 0.0f, 0, delay);
}

void Node::unschedule(SEL_SCHEDULE selector)
{
    // explicit nil handling
    if (selector == 0)
        return;

    m_pScheduler->unscheduleSelector(selector, this);
}

void Node::unscheduleAllSelectors()
{
    m_pScheduler->unscheduleAllForTarget(this);
}

void Node::resume()
{
    m_pScheduler->resumeTarget(this);
    m_pActionManager->resumeTarget(this);
    m_pEventDispatcher->resumeTarget(this);
}

void Node::pause()
{
    m_pScheduler->pauseTarget(this);
    m_pActionManager->pauseTarget(this);
    m_pEventDispatcher->pauseTarget(this);
}

// override me
void Node::update(float fDelta)
{
    if (0 != m_nUpdateScriptHandler)
    {
        //only lua use
        SchedulerScriptData data(m_nUpdateScriptHandler,fDelta);
        ScriptEvent event(kScheduleEvent,&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
    
    if (m_pComponentContainer && !m_pComponentContainer->isEmpty())
    {
        m_pComponentContainer->visit(fDelta);
    }
}

const AffineTransform& Node::getNodeToParentTransform() const
{
    if (m_bTransformDirty) 
    {

        // Translate values
        float x = m_tPosition.x;
        float y = m_tPosition.y;

        if (m_bIgnoreAnchorPointForPosition) 
        {
            x += m_tAnchorPointInPoints.x;
            y += m_tAnchorPointInPoints.y;
        }

        // Rotation values
		// Change rotation code to handle X and Y
		// If we skew with the exact same value for both x and y then we're simply just rotating
        float cx = 1, sx = 0, cy = 1, sy = 0;
        if (m_fRotationX || m_fRotationY)
        {
            float radiansX = -CC_DEGREES_TO_RADIANS(m_fRotationX);
            float radiansY = -CC_DEGREES_TO_RADIANS(m_fRotationY);
            cx = kdCosf(radiansX);
            sx = kdSinf(radiansX);
            cy = kdCosf(radiansY);
            sy = kdSinf(radiansY);
        }

        bool needsSkewMatrix = ( m_fSkewX || m_fSkewY );


        // optimization:
        // inline anchor point calculation if skew is not needed
        // Adjusted transform calculation for rotational skew
        if (! needsSkewMatrix && !m_tAnchorPointInPoints.equals(Point::ZERO))
        {
            x += cy * -m_tAnchorPointInPoints.x * m_fScaleX + -sx * -m_tAnchorPointInPoints.y * m_fScaleY;
            y += sy * -m_tAnchorPointInPoints.x * m_fScaleX +  cx * -m_tAnchorPointInPoints.y * m_fScaleY;
        }


        // Build Transform Matrix
        // Adjusted transform calculation for rotational skew
        m_tTransform = AffineTransformMake( cy * m_fScaleX,  sy * m_fScaleX,
            -sx * m_fScaleY, cx * m_fScaleY,
            x, y );

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needsSkewMatrix) 
        {
            AffineTransform skewMatrix = AffineTransformMake(1.0f, kdTanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
                kdTanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f,
                0.0f, 0.0f );
            m_tTransform = AffineTransformConcat(skewMatrix, m_tTransform);

            // adjust anchor point
            if (!m_tAnchorPointInPoints.equals(Point::ZERO))
            {
                m_tTransform = AffineTransformTranslate(m_tTransform, -m_tAnchorPointInPoints.x, -m_tAnchorPointInPoints.y);
            }
        }
        
        if (m_bAdditionalTransformDirty)
        {
            m_tTransform = AffineTransformConcat(m_tTransform, m_tAdditionalTransform);
            m_bAdditionalTransformDirty = false;
        }

        m_bTransformDirty = false;
    }

    return m_tTransform;
}

void Node::setAdditionalTransform(const AffineTransform& additionalTransform)
{
    m_tAdditionalTransform = additionalTransform;
    m_bTransformDirty = true;
    m_bAdditionalTransformDirty = true;
}

const AffineTransform& Node::getParentToNodeTransform() const
{
    if ( m_bInverseDirty ) {
        m_tInverse = AffineTransformInvert(this->getNodeToParentTransform());
        m_bInverseDirty = false;
    }

    return m_tInverse;
}

AffineTransform Node::getNodeToWorldTransform() const
{
    AffineTransform t = this->getNodeToParentTransform();

    for (Node *p = m_pParent; p != NULL; p = p->getParent())
        t = AffineTransformConcat(t, p->getNodeToParentTransform());

    return t;
}

AffineTransform Node::getWorldToNodeTransform() const
{
    return AffineTransformInvert(this->getNodeToWorldTransform());
}

Point Node::convertToNodeSpace(const Point& worldPoint) const
{
    Point ret = PointApplyAffineTransform(worldPoint, getWorldToNodeTransform());
    return ret;
}

Point Node::convertToWorldSpace(const Point& nodePoint) const
{
    Point ret = PointApplyAffineTransform(nodePoint, getNodeToWorldTransform());
    return ret;
}

Point Node::convertToNodeSpaceAR(const Point& worldPoint) const
{
    Point nodePoint = convertToNodeSpace(worldPoint);
    return nodePoint - m_tAnchorPointInPoints;
}

Point Node::convertToWorldSpaceAR(const Point& nodePoint) const
{
    Point pt = nodePoint + m_tAnchorPointInPoints;
    return convertToWorldSpace(pt);
}

Point Node::convertToWindowSpace(const Point& nodePoint) const
{
    Point worldPoint = this->convertToWorldSpace(nodePoint);
    return Director::getInstance()->convertToUI(worldPoint);
}

// convenience methods which take a Touch instead of Point
Point Node::convertTouchToNodeSpace(Touch *touch) const
{
    Point point = touch->getLocation();
    return this->convertToNodeSpace(point);
}
Point Node::convertTouchToNodeSpaceAR(Touch *touch) const
{
    Point point = touch->getLocation();
    return this->convertToNodeSpaceAR(point);
}

#ifdef CC_USE_PHYSICS
void Node::updatePhysicsTransform()
{
    if (m_pPhysicsBody)
    {
        m_tPosition = m_pPhysicsBody->getPosition();
        m_fRotationX = m_fRotationY = m_pPhysicsBody->getRotation();
        m_bTransformDirty = m_bInverseDirty = true;
    }
}
#endif

void Node::updateTransform()
{
    // Recursively iterate over children
    arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, Node*);
}

Component* Node::getComponent(const char *pName)
{
    if( m_pComponentContainer )
        return m_pComponentContainer->get(pName);
    return nullptr;
}

bool Node::addComponent(Component *pComponent)
{
    // lazy alloc
    if( !m_pComponentContainer )
        m_pComponentContainer = new ComponentContainer(this);
    return m_pComponentContainer->add(pComponent);
}

bool Node::removeComponent(const char *pName)
{
    if( m_pComponentContainer )
        return m_pComponentContainer->remove(pName);
    return false;
}

void Node::removeAllComponents()
{
    if( m_pComponentContainer )
        m_pComponentContainer->removeAll();
}

#ifdef CC_USE_PHYSICS
void Node::setPhysicsBody(PhysicsBody* body)
{
    if (m_pPhysicsBody != nullptr)
    {
        m_pPhysicsBody->m_pNode = nullptr;
        m_pPhysicsBody->release();
    }
    
    m_pPhysicsBody = body;
    m_pPhysicsBody->m_pNode = this;
    m_pPhysicsBody->retain();
    m_pPhysicsBody->setPosition(getPosition());
    m_pPhysicsBody->setRotation(getRotation());
}

PhysicsBody* Node::getPhysicsBody() const
{
    return m_pPhysicsBody;
}
#endif //CC_USE_PHYSICS

KDvoid Node::setClipping ( KDbool bClipping )
{
	m_bIsClipping = bClipping;
}

KDbool Node::isClipping ( KDvoid )
{
	return m_bIsClipping;
}

// NodeRGBA
NodeRGBA::NodeRGBA()
: m_cDisplayedOpacity(255)
, m_cRealOpacity(255)
, m_tDisplayedColor(Color3B::WHITE)
, m_tRealColor(Color3B::WHITE)
, m_bCascadeColorEnabled(false)
, m_bCascadeOpacityEnabled(false)
{}

NodeRGBA::~NodeRGBA() {}

bool NodeRGBA::init()
{
    if (Node::init())
    {
        m_cDisplayedOpacity = m_cRealOpacity = 255;
        m_tDisplayedColor = m_tRealColor = Color3B::WHITE;
        m_bCascadeOpacityEnabled = m_bCascadeColorEnabled = false;
        return true;
    }
    return false;
}

GLubyte NodeRGBA::getOpacity(void) const
{
	return m_cRealOpacity;
}

GLubyte NodeRGBA::getDisplayedOpacity(void) const
{
	return m_cDisplayedOpacity;
}

void NodeRGBA::setOpacity(GLubyte opacity)
{
    m_cDisplayedOpacity = m_cRealOpacity = opacity;
    
	if (m_bCascadeOpacityEnabled)
    {
		GLubyte parentOpacity = 255;
        RGBAProtocol* pParent = dynamic_cast<RGBAProtocol*>(m_pParent);
        if (pParent && pParent->isCascadeOpacityEnabled())
        {
            parentOpacity = pParent->getDisplayedOpacity();
        }
        this->updateDisplayedOpacity(parentOpacity);
	}
}

void NodeRGBA::updateDisplayedOpacity(GLubyte parentOpacity)
{
	m_cDisplayedOpacity = m_cRealOpacity * parentOpacity/255.0;
	
    if (m_bCascadeOpacityEnabled)
    {
        Object* pObj;
        CCARRAY_FOREACH(m_pChildren, pObj)
        {
            RGBAProtocol* item = dynamic_cast<RGBAProtocol*>(pObj);
            if (item)
            {
                item->updateDisplayedOpacity(m_cDisplayedOpacity);
            }
        }
    }
}

bool NodeRGBA::isCascadeOpacityEnabled(void) const
{
    return m_bCascadeOpacityEnabled;
}

void NodeRGBA::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    m_bCascadeOpacityEnabled = cascadeOpacityEnabled;
}

const Color3B& NodeRGBA::getColor(void) const
{
	return m_tRealColor;
}

const Color3B& NodeRGBA::getDisplayedColor() const
{
	return m_tDisplayedColor;
}

void NodeRGBA::setColor(const Color3B& color)
{
	m_tDisplayedColor = m_tRealColor = color;
	
	if (m_bCascadeColorEnabled)
    {
		Color3B parentColor = Color3B::WHITE;
        RGBAProtocol *parent = dynamic_cast<RGBAProtocol*>(m_pParent);
		if (parent && parent->isCascadeColorEnabled())
        {
            parentColor = parent->getDisplayedColor(); 
        }
        
        updateDisplayedColor(parentColor);
	}
}

void NodeRGBA::updateDisplayedColor(const Color3B& parentColor)
{
	m_tDisplayedColor.r = m_tRealColor.r * parentColor.r/255.0;
	m_tDisplayedColor.g = m_tRealColor.g * parentColor.g/255.0;
	m_tDisplayedColor.b = m_tRealColor.b * parentColor.b/255.0;
    
    if (m_bCascadeColorEnabled)
    {
        Object *obj = NULL;
        CCARRAY_FOREACH(m_pChildren, obj)
        {
            RGBAProtocol *item = dynamic_cast<RGBAProtocol*>(obj);
            if (item)
            {
                item->updateDisplayedColor(m_tDisplayedColor);
            }
        }
    }
}

bool NodeRGBA::isCascadeColorEnabled(void) const
{
    return m_bCascadeColorEnabled;
}

void NodeRGBA::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    m_bCascadeColorEnabled = cascadeColorEnabled;
}

NS_CC_END
