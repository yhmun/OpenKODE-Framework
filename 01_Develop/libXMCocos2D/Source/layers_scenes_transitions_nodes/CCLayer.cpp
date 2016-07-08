/* -----------------------------------------------------------------------------------
 *
 *      File            CCLayer.cpp
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
#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "actions/CCActionInterval.h"
#include "accelerometer_dispatcher/CCAccelerometer.h"
#include "keypad_dispatcher/CCKeypadDispatcher.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouch.h"
#include "event_dispatcher/CCEventDispatcher.h"
#include "support/CCDirector.h"
#include "support/CCPointExtension.h"
#include "support/CCEGLView.h"
#include "script_support/CCScriptSupport.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"

NS_CC_BEGIN
    
//
// CCLayer
//

CCLayer::CCLayer ( KDvoid )
{
    m_bTouchEnabled					= KD_FALSE;
    m_bKeypadEnabled				= KD_FALSE;
    m_bAccelerometerEnabled			= KD_FALSE;
	m_bEventEnabled					= KD_FALSE;
	m_pScriptTouchHandlerEntry		= KD_NULL;
	m_pScriptKeypadHandlerEntry		= KD_NULL;
	m_pScriptAccelerateHandlerEntry	= KD_NULL;
	m_eTouchMode					= kCCTouchesAllAtOnce;
    m_nTouchPriority				= 0;
    m_bIgnoreAnchorPointForPosition = KD_TRUE;

	setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
}

CCLayer::~CCLayer ( KDvoid )
{
	unregisterScriptTouchHandler ( );
	unregisterScriptKeypadHandler ( );
    unregisterScriptAccelerateHandler ( );
}

KDbool CCLayer::init ( KDvoid )
{
	this->setContentSize ( CCDirector::sharedDirector ( )->getWinSize ( ) );

	m_bTouchEnabled			= KD_FALSE;
    m_bAccelerometerEnabled = KD_FALSE;
	m_bEventEnabled			= KD_FALSE;

	return KD_TRUE;
}

KDbool CCLayer::initWithVisibleViewport ( KDvoid )
{
	m_bTouchEnabled			= KD_FALSE;
    m_bAccelerometerEnabled = KD_FALSE;
	m_bEventEnabled			= KD_FALSE;

	this->setVisibleViewport ( );

	return KD_TRUE;
}

CCLayer* CCLayer::create ( KDvoid )
{
	CCLayer*  pRet = new CCLayer ( );

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

CCLayer* CCLayer::createWithVisibleViewport ( KDvoid )
{
	CCLayer*  pRet = new CCLayer ( );

	if ( pRet && pRet->initWithVisibleViewport ( ) )
	{
		pRet->autorelease ( );
	}
    else
    {
	    CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

/// Touch and Accelerometer related
KDvoid CCLayer::registerWithTouchDispatcher ( KDvoid )
{
	CCTouchDispatcher*  pDispatcher = CCDirector::sharedDirector ( )->getTouchDispatcher ( );

	// Using LuaBindings
	if ( m_pScriptTouchHandlerEntry )
    {
        if ( m_pScriptTouchHandlerEntry->isMultiTouches ( ) )
        {
            pDispatcher->addStandardDelegate ( this, 0 );
            LUALOG ( "[LUA] Add multi-touches event handler: %d", m_pScriptTouchHandlerEntry->getHandler ( ) );
        }
        else
        {
            pDispatcher->addTargetedDelegate ( this, m_pScriptTouchHandlerEntry->getPriority ( ), m_pScriptTouchHandlerEntry->getSwallowsTouches ( ) );
            LUALOG ( "[LUA] Add touch event handler: %d", m_pScriptTouchHandlerEntry->getHandler ( ) );
        }       
    }
	else
	{
        if ( m_eTouchMode == kCCTouchesAllAtOnce )
		{
            pDispatcher->addStandardDelegate ( this, 0 );
        } 
		else
		{
            pDispatcher->addTargetedDelegate ( this, m_nTouchPriority, KD_TRUE );
        }
	}
}

KDvoid CCLayer::registerScriptTouchHandler ( KDint nHandler, KDbool bIsMultiTouches, KDint nPriority, KDbool bSwallowsTouches )
{
    unregisterScriptTouchHandler ( );
    m_pScriptTouchHandlerEntry = CCTouchScriptHandlerEntry::create ( nHandler, bIsMultiTouches, nPriority, bSwallowsTouches );
    m_pScriptTouchHandlerEntry->retain ( );
}

KDvoid CCLayer::unregisterScriptTouchHandler ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pScriptTouchHandlerEntry );
}

KDint CCLayer::excuteScriptTouchHandler ( KDint nEventType, CCTouch* pTouch )
{
    return CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeLayerTouchEvent ( this, nEventType, pTouch );
}

KDint CCLayer::excuteScriptTouchHandler ( KDint nEventType, CCSet* pTouches )
{
    return CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeLayerTouchesEvent ( this, nEventType, pTouches );
}

/// isTouchEnabled getter
KDbool CCLayer::isTouchEnabled ( KDvoid )
{
	return m_bTouchEnabled;
}

/// isTouchEnabled setter
KDvoid CCLayer::setTouchEnabled ( KDbool bEnabled )
{
	if ( m_bTouchEnabled != bEnabled )
	{
		m_bTouchEnabled = bEnabled;
        
		if ( m_bRunning )
		{
			if ( bEnabled )
			{
				this->registerWithTouchDispatcher ( );
			}
			else
			{
				CCDirector::sharedDirector ( )->getTouchDispatcher ( )->removeDelegate ( this );
			}
		}
	}
}

KDvoid CCLayer::setTouchMode ( ccTouchesMode eMode ) 
{
    if ( m_eTouchMode != eMode )
	{
        m_eTouchMode = eMode;
        
		if ( m_bTouchEnabled )
		{
			setTouchEnabled ( KD_FALSE );
			setTouchEnabled ( KD_TRUE  );
		}
    }
}

KDvoid CCLayer::setTouchPriority ( KDint nPriority ) 
{
    if ( m_nTouchPriority != nPriority )
	{
        m_nTouchPriority = nPriority;
        
		if ( m_bTouchEnabled )
		{
			setTouchEnabled ( KD_FALSE );
			setTouchEnabled ( KD_TRUE  );
		}
    }
}

KDint CCLayer::getTouchPriority ( KDvoid )
{
    return m_nTouchPriority;
}

ccTouchesMode CCLayer::getTouchMode ( KDvoid )
{
    return m_eTouchMode;
}


/// isAccelerometerEnabled getter
KDbool CCLayer::isAccelerometerEnabled ( KDvoid )
{
	return m_bAccelerometerEnabled;
}

/// isAccelerometerEnabled setter
KDvoid CCLayer::setAccelerometerEnabled ( KDbool bEnabled )
{
    if ( m_bAccelerometerEnabled != bEnabled )
    {
        m_bAccelerometerEnabled = bEnabled;
        
        if ( m_bRunning )
        {
			CCAccelerometer*  pAccelerometer = CCDirector::sharedDirector ( )->getAccelerometer ( );

            if ( bEnabled )
            {
                pAccelerometer->setDelegate ( this );
            }
            else
            {
                pAccelerometer->setDelegate ( KD_NULL );
            }
        }
    }
}

KDvoid CCLayer::setAccelerometerInterval ( KDdouble dInterval )
{
    if ( m_bAccelerometerEnabled )
    {
        if ( m_bRunning )
        {
			CCDirector::sharedDirector ( )->getAccelerometer ( )->setAccelerometerInterval ( dInterval );
        }
    }
}

KDvoid CCLayer::didAccelerate ( CCAcceleration* pAccelerationValue )
{
    CC_UNUSED_PARAM ( pAccelerationValue );
    
	if ( m_eScriptType != kScriptTypeNone )
	{
		CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeAccelerometerEvent ( this, pAccelerationValue );
	}
}

KDvoid CCLayer::onEvent ( const KDEvent* pEvent )
{
	
}

KDvoid CCLayer::registerScriptAccelerateHandler ( KDint nHandler )
{
    unregisterScriptAccelerateHandler ( );
    m_pScriptAccelerateHandlerEntry = CCScriptHandlerEntry::create ( nHandler );
    m_pScriptAccelerateHandlerEntry->retain ( );
}

KDvoid CCLayer::unregisterScriptAccelerateHandler ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pScriptAccelerateHandlerEntry );
}

/// isKeypadEnabled getter
KDbool CCLayer::isKeypadEnabled ( KDvoid )
{
    return m_bKeypadEnabled;
}

/// isKeypadEnabled setter
KDvoid CCLayer::setKeypadEnabled ( KDbool bEnabled )
{
    if (  m_bKeypadEnabled != bEnabled )
    {
        m_bKeypadEnabled = bEnabled;

        if ( m_bRunning )
        {
			CCKeypadDispatcher*  pKeypadDispatcher = CCDirector::sharedDirector ( )->getKeypadDispatcher ( );

            if ( bEnabled )
            {
                pKeypadDispatcher->addDelegate ( this );
            }
            else
            {
                pKeypadDispatcher->removeDelegate ( this );
            }
        }
    }
}

KDbool CCLayer::isEventEnabled ( KDvoid )
{
	return m_bEventEnabled;
}

KDvoid CCLayer::setEventEnabled ( KDbool bEnabled )
{
    if (  m_bEventEnabled != bEnabled )
    {
        m_bEventEnabled = bEnabled;

        if ( m_bRunning )
        {
			CCEventDispatcher*  pEventDispatcher = CCDirector::sharedDirector ( )->getEventDispatcher ( );

            if ( bEnabled )
            {
                pEventDispatcher->addDelegate ( this );
            }
            else
            {
                pEventDispatcher->removeDelegate ( this );
            }
        }
	}
}

KDvoid CCLayer::registerScriptKeypadHandler ( KDint nHandler )
{
    unregisterScriptKeypadHandler ( );
    m_pScriptKeypadHandlerEntry = CCScriptHandlerEntry::create ( nHandler );
    m_pScriptKeypadHandlerEntry->retain ( );
}

KDvoid CCLayer::unregisterScriptKeypadHandler ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pScriptKeypadHandlerEntry );
}

KDvoid CCLayer::keyBackClicked ( KDvoid )
{
    if ( m_pScriptKeypadHandlerEntry || m_eScriptType == kScriptTypeJavascript )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeLayerKeypadEvent ( this, KD_INPUT_XMSOFT_ESCAPE );
    }
}

KDvoid CCLayer::keyMenuClicked ( KDvoid )
{
    if ( m_pScriptKeypadHandlerEntry )
    {
        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeLayerKeypadEvent ( this, KD_INPUT_XMSOFT_MENU );
    }
}

/// Callbacks
KDvoid CCLayer::onEnter ( KDvoid )
{
	// register 'parent' nodes first
	// since events are propagated in reverse order
	if ( m_bTouchEnabled )
	{
		this->registerWithTouchDispatcher ( );
	}

	// then iterate over all the children
	CCNode::onEnter ( );

	CCDirector*  pDirector = CCDirector::sharedDirector ( );

    // add this layer to concern the Accelerometer Sensor
    if ( m_bAccelerometerEnabled )
    {
        pDirector->getAccelerometer ( )->setDelegate ( this );
    }

    // add this layer to concern the keypad msg
    if ( m_bKeypadEnabled )
    {
        pDirector->getKeypadDispatcher ( )->addDelegate ( this );
    }

	if ( m_bEventEnabled )
	{
		pDirector->getEventDispatcher ( )->addDelegate ( this );
	}
}

KDvoid CCLayer::onExit ( KDvoid )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );

	if ( m_bTouchEnabled )
	{
		pDirector->getTouchDispatcher ( )->removeDelegate ( this );
	}

    // remove this layer from the delegates who concern Accelerometer Sensor
    if ( m_bAccelerometerEnabled )
    {
        pDirector->getAccelerometer ( )->setDelegate ( KD_NULL );
    }

    // remove this layer from the delegates who concern the keypad msg
    if ( m_bKeypadEnabled )
    {
        pDirector->getKeypadDispatcher ( )->removeDelegate ( this );
    }

    if ( m_bEventEnabled )
    {
        pDirector->getEventDispatcher ( )->removeDelegate ( this );
    }

	CCNode::onExit ( );
}

KDvoid CCLayer::onEnterTransitionDidFinish ( KDvoid )
{  
	CCDirector*  pDirector = CCDirector::sharedDirector ( );

    if ( m_bAccelerometerEnabled )
    {
        pDirector->getAccelerometer ( )->setDelegate ( this );
    }

    CCNode::onEnterTransitionDidFinish ( );
}

KDbool CCLayer::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( kScriptTypeNone != m_eScriptType )
    {
		return excuteScriptTouchHandler ( CCTOUCHBEGAN, pTouch ) == 0 ? KD_FALSE : KD_TRUE;
    }

	CC_UNUSED_PARAM ( pTouch );
    CC_UNUSED_PARAM ( pEvent );
    
	CCAssert ( KD_FALSE, "Layer#ccTouchBegan override me" );
    
	return KD_TRUE;
}

KDvoid CCLayer::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( kScriptTypeNone != m_eScriptType )
    {
        excuteScriptTouchHandler ( CCTOUCHMOVED, pTouch );
    }

	CC_UNUSED_PARAM ( pTouch );
    CC_UNUSED_PARAM ( pEvent );
}

KDvoid CCLayer::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( kScriptTypeNone != m_eScriptType )
    {
        excuteScriptTouchHandler ( CCTOUCHENDED, pTouch );
    }

	CC_UNUSED_PARAM ( pTouch );
    CC_UNUSED_PARAM ( pEvent );
}

KDvoid CCLayer::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( kScriptTypeNone != m_eScriptType )
	{
		excuteScriptTouchHandler ( CCTOUCHBEGAN, pTouches );
	}

	CC_UNUSED_PARAM ( pTouches );
    CC_UNUSED_PARAM ( pEvent );
}

KDvoid CCLayer::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( kScriptTypeNone != m_eScriptType )
	{
		excuteScriptTouchHandler ( CCTOUCHMOVED, pTouches );
	}

	CC_UNUSED_PARAM ( pTouches );
    CC_UNUSED_PARAM ( pEvent );
}

KDvoid CCLayer::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( kScriptTypeNone != m_eScriptType )
	{
		excuteScriptTouchHandler ( CCTOUCHENDED, pTouches );
	}

	CC_UNUSED_PARAM ( pTouches );
    CC_UNUSED_PARAM ( pEvent );
}

KDvoid CCLayer::setVisibleViewport ( KDvoid )
{
	CCRect  tRect = CCEGLView::sharedOpenGLView ( )->getVisibleRect ( );

	this->setPosition    ( tRect.origin );
	this->setContentSize ( tRect.size   );
}

// LayerRGBA
CCLayerRGBA::CCLayerRGBA ( KDvoid )
{
	m_cDisplayedOpacity		 = 255;
	m_cRealOpacity			 = 255;
	m_tDisplayedColor		 = ccWHITE;
	m_tRealColor			 = ccWHITE;
	m_bCascadeOpacityEnabled = KD_FALSE;
	m_bCascadeColorEnabled	 = KD_FALSE;
}

CCLayerRGBA::~CCLayerRGBA ( KDvoid )
{

}

KDbool CCLayerRGBA::init ( KDvoid )
{
	if ( CCLayer::init ( ) )
    {
        m_cDisplayedOpacity = m_cRealOpacity = 255;
        m_tDisplayedColor = m_tRealColor = ccWHITE;
        setCascadeOpacityEnabled ( KD_FALSE );
        setCascadeColorEnabled ( KD_FALSE );
        
        return KD_TRUE;
    }
    else
    {
        return KD_FALSE;
    }
}

GLubyte CCLayerRGBA::getOpacity ( KDvoid )
{
	return m_cRealOpacity;
}

GLubyte CCLayerRGBA::getDisplayedOpacity ( KDvoid )
{
	return m_cDisplayedOpacity;
}

// Override synthesized setOpacity to recurse items 
KDvoid CCLayerRGBA::setOpacity ( GLubyte cOpacity )
{
	m_cDisplayedOpacity = m_cRealOpacity = cOpacity;
    
	if ( m_bCascadeOpacityEnabled )
    {
		GLubyte  cParentOpacity = 255;
        CCRGBAProtocol*  pParent = dynamic_cast<CCRGBAProtocol*> ( m_pParent );
        if ( pParent && pParent->isCascadeOpacityEnabled ( ) )
        {
            cParentOpacity = pParent->getDisplayedOpacity ( );
        }
        updateDisplayedOpacity ( cParentOpacity );
	}
}

const ccColor3B& CCLayerRGBA::getColor ( KDvoid )
{
	return m_tRealColor;
}

const ccColor3B& CCLayerRGBA::getDisplayedColor ( KDvoid )
{
	return m_tDisplayedColor;
}

KDvoid CCLayerRGBA::setColor ( const ccColor3B& tColor )
{
	m_tDisplayedColor = m_tRealColor = tColor;
	
	if ( m_bCascadeColorEnabled )
    {
		ccColor3B  tParentColor = ccWHITE;
        CCRGBAProtocol*  pParent = dynamic_cast<CCRGBAProtocol*> ( m_pParent );
		if ( pParent && pParent->isCascadeColorEnabled ( ) )
        {
            tParentColor = pParent->getDisplayedColor ( );
        }

        updateDisplayedColor ( tParentColor );
	}
}

KDvoid CCLayerRGBA::updateDisplayedOpacity ( GLubyte cParentOpacity )
{
	m_cDisplayedOpacity = (GLubyte) ( m_cRealOpacity * cParentOpacity / 255.0 );
    
    if ( m_bCascadeOpacityEnabled )
    {
        CCObject*  pObj = NULL;
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

KDvoid CCLayerRGBA::updateDisplayedColor ( const ccColor3B& tParentColor )
{
	m_tDisplayedColor.r = (GLubyte) ( m_tRealColor.r * tParentColor.r / 255.0 );
	m_tDisplayedColor.g = (GLubyte) ( m_tRealColor.g * tParentColor.g / 255.0 );
	m_tDisplayedColor.b = (GLubyte) ( m_tRealColor.b * tParentColor.b / 255.0 );
    
    if ( m_bCascadeColorEnabled )
    {
        CCObject*  pObj = NULL;
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

KDbool CCLayerRGBA::isCascadeOpacityEnabled ( KDvoid )
{
    return m_bCascadeOpacityEnabled;
}

KDvoid CCLayerRGBA::setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled )
{
    m_bCascadeOpacityEnabled = bCascadeOpacityEnabled;
}

KDbool CCLayerRGBA::isCascadeColorEnabled ( KDvoid )
{
    return m_bCascadeColorEnabled;
}

KDvoid CCLayerRGBA::setCascadeColorEnabled ( KDbool bCascadeColorEnabled )
{
    m_bCascadeColorEnabled = bCascadeColorEnabled;
}


//
// ColorLayer
//
CCLayerColor::CCLayerColor ( KDvoid )
{
	// default blend function
	m_tBlendFunc.src = CC_BLEND_SRC;
	m_tBlendFunc.dst = CC_BLEND_DST;
}

CCLayerColor::~CCLayerColor ( KDvoid )
{

}

/// blendFunc getter
ccBlendFunc CCLayerColor::getBlendFunc ( KDvoid )
{
	return m_tBlendFunc;
}

/// blendFunc setter
KDvoid CCLayerColor::setBlendFunc ( ccBlendFunc tBlendFunc )
{
	m_tBlendFunc = tBlendFunc;
}

CCLayerColor* CCLayerColor::create ( KDvoid )
{
    CCLayerColor*  pRet = new CCLayerColor ( );

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

CCLayerColor* CCLayerColor::create ( const ccColor4B& tColor, GLfloat fWidth, GLfloat fHeight )
{
	CCLayerColor*  pLayer = new CCLayerColor ( );

	if ( pLayer && pLayer->initWithColor ( tColor, fWidth, fHeight ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

CCLayerColor* CCLayerColor::create ( const ccColor4B& tColor, const CCSize& tSize )
{
	CCLayerColor*  pLayer = new CCLayerColor ( );

	if ( pLayer && pLayer->initWithColor ( tColor, tSize ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

CCLayerColor* CCLayerColor::create ( const ccColor4B& tColor )
{
	CCLayerColor*  pLayer = new CCLayerColor ( );

	if ( pLayer && pLayer->initWithColor ( tColor ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDbool CCLayerColor::init ( KDvoid )
{
    return this->initWithColor ( ccc4 ( 0, 0, 0, 0), CCDirector::sharedDirector ( )->getWinSize ( ) );
}

KDbool CCLayerColor::initWithColor ( const ccColor4B& tColor, GLfloat fWidth, GLfloat height )
{
	return this->initWithColor ( tColor, CCSize ( fWidth, height ) );
}

KDbool CCLayerColor::initWithColor ( const ccColor4B& tColor, const CCSize& tSize )
{
	if ( CCLayer::init ( ) ) 
	{
		// default blend function
		m_tBlendFunc.src = GL_SRC_ALPHA;
		m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;

		m_tDisplayedColor.r = m_tRealColor.r = tColor.r;
		m_tDisplayedColor.g = m_tRealColor.g = tColor.g;
		m_tDisplayedColor.b = m_tRealColor.b = tColor.b;
		m_cDisplayedOpacity = m_cRealOpacity = tColor.a;

		for ( KDuint i = 0; i < sizeof ( m_aSquareVertices ) / sizeof ( m_aSquareVertices[ 0 ] ); i++ )
		{
			m_aSquareVertices[ i ].x = 0.0f;
			m_aSquareVertices[ i ].y = 0.0f;
		}

		updateColor ( );
		setContentSize ( tSize );

		setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionColor ) );
	}

	return KD_TRUE;
}

KDbool CCLayerColor::initWithColor ( const ccColor4B& tColor )
{
	return this->initWithColor ( tColor, CCDirector::sharedDirector ( )->getWinSize ( ) );
}

KDvoid CCLayerColor::setContentSize ( const CCSize& tSize )
{
	m_aSquareVertices [ 1 ].x = tSize.cx;
	m_aSquareVertices [ 2 ].y = tSize.cy;
	m_aSquareVertices [ 3 ].x = tSize.cx;
	m_aSquareVertices [ 3 ].y = tSize.cy;

	CCLayer::setContentSize ( tSize );
}

KDvoid CCLayerColor::changeWidthAndHeight ( GLfloat fWidth ,GLfloat fHeight )
{
	this->setContentSize ( CCSize ( fWidth, fHeight ) );
}

KDvoid CCLayerColor::changeWidth ( GLfloat fWidth )
{
	this->setContentSize ( CCSize ( fWidth, m_tContentSize.cy ) );
}

KDvoid CCLayerColor::changeHeight ( GLfloat fHeight )
{
	this->setContentSize ( CCSize ( m_tContentSize.cx, fHeight ) );
}

KDvoid CCLayerColor::updateColor ( KDvoid )
{
	for ( KDuint i = 0; i < 4; i++ )
	{
		m_aSquareColors [ i ].r = m_tDisplayedColor.r / 255.0f;
		m_aSquareColors [ i ].g = m_tDisplayedColor.g / 255.0f;
		m_aSquareColors [ i ].b = m_tDisplayedColor.b / 255.0f;
		m_aSquareColors [ i ].a = m_cDisplayedOpacity / 255.0f;
	}
}

KDvoid CCLayerColor::draw ( KDvoid )
{
    CC_NODE_DRAW_SETUP ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );

    //
    // Attributes
    //
#ifdef EMSCRIPTEN
	setGLBufferData ( m_aSquareVertices, 4 * sizeof ( ccVertex2F ), 0 );
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, 0);

	setGLBufferData ( m_aSquareColors, 4 * sizeof ( ccColor4F ), 1 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, 0 );
#else
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_aSquareVertices );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Color   , 4, GL_FLOAT, GL_FALSE, 0, m_aSquareColors );
#endif // EMSCRIPTEN

    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid CCLayerColor::setColor ( const ccColor3B& tColor )
{
    CCLayerRGBA::setColor ( tColor );
    updateColor ( );
}

KDvoid CCLayerColor::setOpacity ( GLubyte cOpacity )
{
    CCLayerRGBA::setOpacity ( cOpacity );
    updateColor ( );
}

//
// CCLayerGradient
// 

CCLayerGradient* CCLayerGradient::create ( KDvoid )
{
    CCLayerGradient*  pRet = new CCLayerGradient ( );
    
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

KDbool CCLayerGradient::init ( KDvoid )
{
	return initWithColor ( ccc4 ( 0, 0, 0, 255 ), ccc4 ( 0, 0, 0, 255 ) );
}

CCLayerGradient* CCLayerGradient::create ( const ccColor4B& tStart, const ccColor4B& tEnd )
{
    CCLayerGradient*  pLayer = new CCLayerGradient ( );

    if ( pLayer && pLayer->initWithColor ( tStart, tEnd ) )
    {
        pLayer->autorelease ( );        
    }
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

    return pLayer;
}

CCLayerGradient* CCLayerGradient::create ( const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& tVector )
{
    CCLayerGradient*  pLayer = new CCLayerGradient ( );

    if ( pLayer && pLayer->initWithColor ( tStart, tEnd, tVector ) )
    {
        pLayer->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

    return pLayer;
}

KDbool CCLayerGradient::initWithColor ( const ccColor4B& tStart, const ccColor4B& tEnd )
{
    return initWithColor ( tStart, tEnd, ccp ( 0, -1 ) );
}

KDbool CCLayerGradient::initWithColor ( const ccColor4B& tStart, const ccColor4B& tEnd, const CCPoint& tVector )
{
    m_tEndColor.r   = tEnd.r;
    m_tEndColor.g   = tEnd.g;
    m_tEndColor.b   = tEnd.b;
    m_cEndOpacity   = tEnd.a;
    m_cStartOpacity = tStart.a;
    m_tVector		= tVector;

	m_bCompressedInterpolation = KD_TRUE;

    return CCLayerColor::initWithColor ( ccc4 ( tStart.r, tStart.g, tStart.b, 255 ) );
}

KDvoid CCLayerGradient::updateColor ( KDvoid )
{
    CCLayerColor::updateColor ( );

    KDfloat  h = ccpLength ( m_tVector );

    if ( h == 0 )
	{
        return;
	}

    KDfloat  c = kdSqrtf ( 2.0f );
    CCPoint  u = ccp ( m_tVector.x / h, m_tVector.y / h );

	// Compressed Interpolation mode
    if ( m_bCompressedInterpolation )
    {
        KDfloat h2 = 1 / ( kdFabsf ( u.x ) + kdFabsf ( u.y ) );
        u = ccpMult ( u, h2 * c );
    }

    KDfloat  fOpacity = m_cDisplayedOpacity / 255.0f;

    ccColor4F S =
	{
        m_tDisplayedColor.r / 255.0f,
        m_tDisplayedColor.g / 255.0f,
        m_tDisplayedColor.b / 255.0f,
        m_cStartOpacity * fOpacity / 255.0f
    };

    ccColor4F E =
	{
        m_tEndColor.r / 255.0f,
        m_tEndColor.g / 255.0f,
        m_tEndColor.b / 255.0f,
        m_cEndOpacity * fOpacity / 255.0f,
    };

    // (-1, -1)
    m_aSquareColors[0].r = E.r + ( S.r - E.r ) * ( ( c + u.x + u.y ) / ( 2.0f * c ) );
    m_aSquareColors[0].g = E.g + ( S.g - E.g ) * ( ( c + u.x + u.y ) / ( 2.0f * c ) );
    m_aSquareColors[0].b = E.b + ( S.b - E.b ) * ( ( c + u.x + u.y ) / ( 2.0f * c ) );
    m_aSquareColors[0].a = E.a + ( S.a - E.a ) * ( ( c + u.x + u.y ) / ( 2.0f * c ) );
    
    // (1, -1)
    m_aSquareColors[1].r = E.r + ( S.r - E.r ) * ( ( c - u.x + u.y ) / ( 2.0f * c ) );
    m_aSquareColors[1].g = E.g + ( S.g - E.g ) * ( ( c - u.x + u.y ) / ( 2.0f * c ) );
    m_aSquareColors[1].b = E.b + ( S.b - E.b ) * ( ( c - u.x + u.y ) / ( 2.0f * c ) );
    m_aSquareColors[1].a = E.a + ( S.a - E.a ) * ( ( c - u.x + u.y ) / ( 2.0f * c ) );
    
    // (-1, 1)
    m_aSquareColors[2].r = E.r + ( S.r - E.r ) * ( ( c + u.x - u.y ) / ( 2.0f * c ) );
    m_aSquareColors[2].g = E.g + ( S.g - E.g ) * ( ( c + u.x - u.y ) / ( 2.0f * c ) );
    m_aSquareColors[2].b = E.b + ( S.b - E.b ) * ( ( c + u.x - u.y ) / ( 2.0f * c ) );
    m_aSquareColors[2].a = E.a + ( S.a - E.a ) * ( ( c + u.x - u.y ) / ( 2.0f * c ) );
    
    // (1, 1)
    m_aSquareColors[3].r = E.r + ( S.r - E.r ) * ( ( c - u.x - u.y ) / ( 2.0f * c ) );
    m_aSquareColors[3].g = E.g + ( S.g - E.g ) * ( ( c - u.x - u.y ) / ( 2.0f * c ) );
    m_aSquareColors[3].b = E.b + ( S.b - E.b ) * ( ( c - u.x - u.y ) / ( 2.0f * c ) );
    m_aSquareColors[3].a = E.a + ( S.a - E.a ) * ( ( c - u.x - u.y ) / ( 2.0f * c ) );
}

const ccColor3B& CCLayerGradient::getStartColor ( KDvoid )
{
    return m_tRealColor;
}

KDvoid CCLayerGradient::setStartColor ( const ccColor3B& tColor )
{
    setColor ( tColor );
}

KDvoid CCLayerGradient::setEndColor ( const ccColor3B& tColor )
{
    m_tEndColor = tColor;
    updateColor ( );
}

const ccColor3B& CCLayerGradient::getEndColor ( KDvoid )
{
    return m_tEndColor;
}

KDvoid CCLayerGradient::setStartOpacity ( GLubyte cStartOpacity )
{
    m_cStartOpacity = cStartOpacity;
    updateColor ( );
}

GLubyte CCLayerGradient::getStartOpacity ( KDvoid )
{
    return m_cStartOpacity;
}

KDvoid CCLayerGradient::setEndOpacity ( GLubyte cEndOpacity )
{
    m_cEndOpacity = cEndOpacity;
    updateColor ( );
}

GLubyte CCLayerGradient::getEndOpacity ( KDvoid )
{
    return m_cEndOpacity;
}

KDvoid CCLayerGradient::setVector ( const CCPoint& tVector )
{
    m_tVector = tVector;
    updateColor ( );
}

const CCPoint& CCLayerGradient::getVector ( KDvoid )
{
    return m_tVector;
}

KDbool CCLayerGradient::isCompressedInterpolation ( KDvoid )
{
    return m_bCompressedInterpolation;
}

KDvoid CCLayerGradient::setCompressedInterpolation ( KDbool bCompressedInterpolation )
{
    m_bCompressedInterpolation = bCompressedInterpolation;
    updateColor ( );
}


//
// LayerMultiplex
//
CCLayerMultiplex::CCLayerMultiplex ( KDvoid )
{
    m_nEnabled = 0;
    m_pLayers  = KD_NULL;
}

CCLayerMultiplex::~CCLayerMultiplex ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pLayers );
}

CCLayerMultiplex* CCLayerMultiplex::create ( KDvoid )
{
    CCLayerMultiplex*  pRet = new CCLayerMultiplex ( );

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

CCLayerMultiplex* CCLayerMultiplex::create ( CCLayer* pLayer, ... )
{
	KDVaListKHR       pArgs;
	KD_VA_START_KHR ( pArgs, pLayer );

	CCLayerMultiplex*  pRet = new CCLayerMultiplex ( );

	if ( pRet && pRet->initWithLayers ( pLayer, pArgs ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	KD_VA_END_KHR ( pArgs );
	
	return pRet;
}

CCLayerMultiplex* CCLayerMultiplex::createWithLayer ( CCLayer* pLayer )
{
	return CCLayerMultiplex::create ( pLayer, KD_NULL );
}

CCLayerMultiplex* CCLayerMultiplex::createWithArray ( CCArray* pArrayOfLayers )
{
    CCLayerMultiplex*  pRet = new CCLayerMultiplex ( );
    
	if ( pRet && pRet->initWithArray ( pArrayOfLayers ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

KDvoid CCLayerMultiplex::addLayer ( CCLayer* pLayer )
{
	CCAssert ( m_pLayers, "" );
	m_pLayers->addObject ( pLayer );
}

KDbool CCLayerMultiplex::initWithLayers ( CCLayer* pLayer, KDVaListKHR pArgs )
{
	if ( CCLayer::init ( ) )
    {
        m_pLayers = CCArray::createWithCapacity ( 5 );
        m_pLayers->retain ( );
        m_pLayers->addObject ( pLayer );

        CCLayer*  l = KD_VA_ARG ( pArgs, CCLayer* );
        while ( l ) 
		{
            m_pLayers->addObject ( l );
            l = KD_VA_ARG ( pArgs, CCLayer* );
        }

        m_nEnabled = 0;
        this->addChild ( (CCNode*) m_pLayers->objectAtIndex ( m_nEnabled ) );
        return KD_TRUE;
    }

    return KD_FALSE;
}

KDbool CCLayerMultiplex::initWithArray ( CCArray* pArrayOfLayers )
{
    if ( CCLayer::init ( ) )
    {
        m_pLayers = CCArray::createWithCapacity ( pArrayOfLayers->count ( ) );
        m_pLayers->addObjectsFromArray ( pArrayOfLayers );
        m_pLayers->retain ( );

        m_nEnabled = 0;
        this->addChild ( (CCNode*) m_pLayers->objectAtIndex ( m_nEnabled ) );
        return KD_TRUE;
    }

    return KD_FALSE;
}

KDvoid CCLayerMultiplex::switchTo ( KDuint nEnabled )
{
	CCAssert ( nEnabled < m_pLayers->count ( ), "Invalid index in MultiplexLayer switchTo message" );

	this->removeChild ( (CCNode*) m_pLayers->objectAtIndex ( m_nEnabled ), KD_TRUE );

	m_nEnabled = nEnabled;

	this->addChild ( (CCNode*) m_pLayers->objectAtIndex ( nEnabled ) );
}

KDvoid CCLayerMultiplex::switchToAndReleaseMe ( KDuint nEnabled )
{
	CCAssert ( nEnabled < m_pLayers->count ( ), "Invalid index in MultiplexLayer switchTo message" );

	this->removeChild ( (CCNode*) m_pLayers->objectAtIndex ( m_nEnabled ), KD_TRUE );

	m_pLayers->replaceObjectAtIndex ( m_nEnabled, KD_NULL );
 
	m_nEnabled = nEnabled;

	this->addChild ( (CCNode*) m_pLayers->objectAtIndex ( nEnabled ) );
}

NS_CC_END