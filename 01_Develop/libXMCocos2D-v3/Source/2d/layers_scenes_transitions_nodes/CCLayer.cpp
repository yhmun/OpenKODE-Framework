/* -----------------------------------------------------------------------------------
 *
 *      File            CCLayer.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#include "2d/layers_scenes_transitions_nodes/CCLayer.h"
#include "2d/CCDirector.h"
#include "2d/script_support/CCScriptSupport.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "support/TransformUtils.h"
// extern
#include "XMKazmath/GL/matrix.h"
#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "2d/event_dispatcher/CCEventListenerTouch.h"
#include "2d/event_dispatcher/CCEventTouch.h"
#include "2d/event_dispatcher/CCEventKeyboard.h"
#include "2d/event_dispatcher/CCEventListenerKeyboard.h"
#include "2d/event_dispatcher/CCEventAcceleration.h"
#include "2d/event_dispatcher/CCEventListenerAcceleration.h"
#include "2d/layers_scenes_transitions_nodes/CCScene.h"
#include "platform/CCDevice.h"
#include "platform/CCEGLView.h"

NS_CC_BEGIN

// Layer
Layer::Layer()
: m_bTouchEnabled(false)
, m_bAccelerometerEnabled(false)
, m_bKeyboardEnabled(false)
, m_pTouchListener(nullptr)
, m_pKeyboardListener(nullptr)
, m_pAccelerationListener(nullptr)
{
    m_bIgnoreAnchorPointForPosition = true;
    setAnchorPoint(Point(0.5f, 0.5f));
}

Layer::~Layer()
{

}

bool Layer::init()
{
    bool bRet = false;
    do 
    {        
        Director * pDirector;
        CC_BREAK_IF(!(pDirector = Director::getInstance()));
        this->setContentSize(pDirector->getWinSize());
        // success
        bRet = true;
    } while(0);
    return bRet;
}

Layer *Layer::create()
{
    Layer *pRet = new Layer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void Layer::onAcceleration(Acceleration* acc, Event* unused_event)
{
    CC_UNUSED_PARAM(acc);

    if(kScriptTypeNone != m_eScriptType)
    {
        BasicScriptData data(this,(void*)acc);
        ScriptEvent event(kAccelerometerEvent,&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
    CC_UNUSED_PARAM(unused_event);
}

void Layer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    CC_UNUSED_PARAM(keyCode);
    CC_UNUSED_PARAM(unused_event);
}

void Layer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    CC_UNUSED_PARAM(unused_event);
    if(kScriptTypeNone != m_eScriptType)
    {
        KeypadScriptData data(keyCode, this);
        ScriptEvent event(kKeypadEvent,&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
}

/// Callbacks

bool Layer::onTouchBegan(Touch *touch, Event *unused_event)
{
    CC_UNUSED_PARAM(unused_event);
    CCASSERT(false, "Layer#ccTouchBegan override me");
    return true;
}

void Layer::onTouchMoved(Touch *touch, Event *unused_event)
{
    CC_UNUSED_PARAM(unused_event);
}

void Layer::onTouchEnded(Touch *touch, Event *unused_event)
{
    CC_UNUSED_PARAM(unused_event);
}

void Layer::onTouchCancelled(Touch *touch, Event *unused_event)
{
    CC_UNUSED_PARAM(unused_event);
}    

void Layer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    CC_UNUSED_PARAM(unused_event);
}

void Layer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    CC_UNUSED_PARAM(unused_event);
}

void Layer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    CC_UNUSED_PARAM(unused_event);
}

void Layer::onTouchesCancelled(const std::vector<Touch*>& touches, Event *unused_event)
{
    CC_UNUSED_PARAM(unused_event);
}

// Added By XMSoft : Move Visible Position & Set Visible Size
KDvoid Layer::setVisibleViewport ( KDvoid )
{
	this->setPosition    ( EGLView::getInstance ( )->getVisibleOrigin ( ) );
	this->setContentSize ( EGLView::getInstance ( )->getVisibleSize ( ) );
}

// LayerRGBA
LayerRGBA::LayerRGBA()
: m_cDisplayedOpacity(255)
, m_cRealOpacity (255)
, m_tDisplayedColor(Color3B::WHITE)
, m_tRealColor(Color3B::WHITE)
, m_bCascadeOpacityEnabled(false)
, m_bCascadeColorEnabled(false)
{}

LayerRGBA::~LayerRGBA() {}

bool LayerRGBA::init()
{
	if (Layer::init())
    {
        m_cDisplayedOpacity = m_cRealOpacity = 255;
        m_tDisplayedColor = m_tRealColor = Color3B::WHITE;
        setCascadeOpacityEnabled(false);
        setCascadeColorEnabled(false);
        
        return true;
    }
    else
    {
        return false;
    }
}

GLubyte LayerRGBA::getOpacity() const
{
	return m_cRealOpacity;
}

GLubyte LayerRGBA::getDisplayedOpacity() const
{
	return m_cDisplayedOpacity;
}

/** Override synthesized setOpacity to recurse items */
void LayerRGBA::setOpacity(GLubyte opacity)
{
	m_cDisplayedOpacity = m_cRealOpacity = opacity;
    
	if( m_bCascadeOpacityEnabled )
    {
		GLubyte parentOpacity = 255;
        RGBAProtocol *parent = dynamic_cast<RGBAProtocol*>(m_pParent);
        if (parent && parent->isCascadeOpacityEnabled())
        {
            parentOpacity = parent->getDisplayedOpacity();
        }
        updateDisplayedOpacity(parentOpacity);
	}
}

const Color3B& LayerRGBA::getColor() const
{
	return m_tRealColor;
}

const Color3B& LayerRGBA::getDisplayedColor() const
{
	return m_tDisplayedColor;
}

void LayerRGBA::setColor(const Color3B& color)
{
	m_tDisplayedColor = m_tRealColor = color;
	
	if (m_bCascadeColorEnabled)
    {
		Color3B parentColor = Color3B::WHITE;
        RGBAProtocol* parent = dynamic_cast<RGBAProtocol*>(m_pParent);
		if (parent && parent->isCascadeColorEnabled())
        {
            parentColor = parent->getDisplayedColor();
        }

        updateDisplayedColor(parentColor);
	}
}

void LayerRGBA::updateDisplayedOpacity(GLubyte parentOpacity)
{
	m_cDisplayedOpacity = m_cRealOpacity * parentOpacity/255.0;
    
    if (m_bCascadeOpacityEnabled)
    {
        Object *obj = NULL;
        CCARRAY_FOREACH(m_pChildren, obj)
        {
            RGBAProtocol *item = dynamic_cast<RGBAProtocol*>(obj);
            if (item)
            {
                item->updateDisplayedOpacity(m_cDisplayedOpacity);
            }
        }
    }
}

void LayerRGBA::updateDisplayedColor(const Color3B& parentColor)
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

bool LayerRGBA::isCascadeOpacityEnabled() const
{
    return m_bCascadeOpacityEnabled;
}

void LayerRGBA::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    m_bCascadeOpacityEnabled = cascadeOpacityEnabled;
}

bool LayerRGBA::isCascadeColorEnabled() const
{
    return m_bCascadeColorEnabled;
}

void LayerRGBA::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    m_bCascadeColorEnabled = cascadeColorEnabled;
}

/// LayerColor

LayerColor::LayerColor()
{
    // default blend function
    m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
}
    
LayerColor::~LayerColor()
{
}

/// blendFunc getter
const BlendFunc &LayerColor::getBlendFunc() const
{
    return m_tBlendFunc;
}
/// blendFunc setter
void LayerColor::setBlendFunc(const BlendFunc &var)
{
    m_tBlendFunc = var;
}

LayerColor* LayerColor::create()
{
    LayerColor* pRet = new LayerColor();
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

LayerColor* LayerColor::create(const Color4B& color, GLfloat width, GLfloat height)
{
    LayerColor * pLayer = new LayerColor();
    if( pLayer && pLayer->initWithColor(color,width,height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

// Added By XMSoft
LayerColor* LayerColor::create ( const Color4B& tColor, const Size& tSize )
{
    LayerColor*		pLayer = new LayerColor ( );

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

LayerColor * LayerColor::create(const Color4B& color)
{
    LayerColor * pLayer = new LayerColor();
    if(pLayer && pLayer->initWithColor(color))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

bool LayerColor::init()
{
    Size s = Director::getInstance()->getWinSize();
    return initWithColor(Color4B(0,0,0,0), s.width, s.height);
}

// Added By XMSoft
KDbool LayerColor::initWithColor ( const Color4B& tColor, const Size& tSize )
{
	return this->initWithColor ( tColor, tSize.cx, tSize.cy );
}

bool LayerColor::initWithColor(const Color4B& color, GLfloat w, GLfloat h)
{
    if (Layer::init())
    {

        // default blend function
        m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;

        m_tDisplayedColor.r = m_tRealColor.r = color.r;
        m_tDisplayedColor.g = m_tRealColor.g = color.g;
        m_tDisplayedColor.b = m_tRealColor.b = color.b;
        m_cDisplayedOpacity = m_cRealOpacity = color.a;

        for (size_t i = 0; i<sizeof(m_aSquareVertices) / sizeof( m_aSquareVertices[0]); i++ )
        {
            m_aSquareVertices[i].x = 0.0f;
            m_aSquareVertices[i].y = 0.0f;
        }

        updateColor();
        setContentSize(Size(w, h));

        setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
        return true;
    }
    return false;
}

bool LayerColor::initWithColor(const Color4B& color)
{
    Size s = Director::getInstance()->getWinSize();
    this->initWithColor(color, s.width, s.height);
    return true;
}

/// override contentSize
void LayerColor::setContentSize(const Size & size)
{
    m_aSquareVertices[1].x = size.width;
    m_aSquareVertices[2].y = size.height;
    m_aSquareVertices[3].x = size.width;
    m_aSquareVertices[3].y = size.height;

    Layer::setContentSize(size);
}

void LayerColor::changeWidthAndHeight(GLfloat w ,GLfloat h)
{
    this->setContentSize(Size(w, h));
}

void LayerColor::changeWidth(GLfloat w)
{
    this->setContentSize(Size(w, m_tContentSize.height));
}

void LayerColor::changeHeight(GLfloat h)
{
    this->setContentSize(Size(m_tContentSize.width, h));
}

void LayerColor::updateColor()
{
    for( unsigned int i=0; i < 4; i++ )
    {
        m_aSquareColors[i].r = m_tDisplayedColor.r / 255.0f;
        m_aSquareColors[i].g = m_tDisplayedColor.g / 255.0f;
        m_aSquareColors[i].b = m_tDisplayedColor.b / 255.0f;
        m_aSquareColors[i].a = m_cDisplayedOpacity / 255.0f;
    }
}

void LayerColor::draw()
{
    CC_NODE_DRAW_SETUP();

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR );

    //
    // Attributes
    //
#ifdef EMSCRIPTEN
    setGLBufferData(m_aSquareVertices, 4 * sizeof(Vertex2F), 0);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setGLBufferData(m_aSquareColors, 4 * sizeof(Color4F), 1);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);
#else
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, m_aSquareVertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, m_aSquareColors);
#endif // EMSCRIPTEN

    GL::blendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    CC_INCREMENT_GL_DRAWS(1);
}

void LayerColor::setColor(const Color3B &color)
{
    LayerRGBA::setColor(color);
    updateColor();
}

void LayerColor::setOpacity(GLubyte opacity)
{
    LayerRGBA::setOpacity(opacity);
    updateColor();
}

//
// LayerGradient
// 
LayerGradient* LayerGradient::create(const Color4B& start, const Color4B& end)
{
    LayerGradient * pLayer = new LayerGradient();
    if( pLayer && pLayer->initWithColor(start, end))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

LayerGradient* LayerGradient::create(const Color4B& start, const Color4B& end, const Point& v)
{
    LayerGradient * pLayer = new LayerGradient();
    if( pLayer && pLayer->initWithColor(start, end, v))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

LayerGradient* LayerGradient::create()
{
    LayerGradient* pRet = new LayerGradient();
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

bool LayerGradient::init()
{
	return initWithColor(Color4B(0, 0, 0, 255), Color4B(0, 0, 0, 255));
}

bool LayerGradient::initWithColor(const Color4B& start, const Color4B& end)
{
    return initWithColor(start, end, Point(0, -1));
}

bool LayerGradient::initWithColor(const Color4B& start, const Color4B& end, const Point& v)
{
    m_tEndColor.r  = end.r;
    m_tEndColor.g  = end.g;
    m_tEndColor.b  = end.b;

    m_cEndOpacity   = end.a;
    m_cStartOpacity    = start.a;
    m_tAlongVector   = v;

    m_bCompressedInterpolation = true;

    return LayerColor::initWithColor(Color4B(start.r, start.g, start.b, 255));
}

void LayerGradient::updateColor()
{
    LayerColor::updateColor();

    float h = m_tAlongVector.getLength();
    if (h == 0)
        return;

    float c = kdSqrtf(2.0f);
    Point u = Point(m_tAlongVector.x / h, m_tAlongVector.y / h);

    // Compressed Interpolation mode
    if (m_bCompressedInterpolation)
    {
        float h2 = 1 / ( kdFabsf(u.x) + kdFabsf(u.y) );
        u = u * (h2 * (float)c);
    }

    float opacityf = (float)m_cDisplayedOpacity / 255.0f;

    Color4F S(
        m_tDisplayedColor.r / 255.0f,
        m_tDisplayedColor.g / 255.0f,
        m_tDisplayedColor.b / 255.0f,
        m_cStartOpacity * opacityf / 255.0f
    );

    Color4F E(
        m_tEndColor.r / 255.0f,
        m_tEndColor.g / 255.0f,
        m_tEndColor.b / 255.0f,
        m_cEndOpacity * opacityf / 255.0f
    );

    // (-1, -1)
    m_aSquareColors[0].r = E.r + (S.r - E.r) * ((c + u.x + u.y) / (2.0f * c));
    m_aSquareColors[0].g = E.g + (S.g - E.g) * ((c + u.x + u.y) / (2.0f * c));
    m_aSquareColors[0].b = E.b + (S.b - E.b) * ((c + u.x + u.y) / (2.0f * c));
    m_aSquareColors[0].a = E.a + (S.a - E.a) * ((c + u.x + u.y) / (2.0f * c));
    // (1, -1)
    m_aSquareColors[1].r = E.r + (S.r - E.r) * ((c - u.x + u.y) / (2.0f * c));
    m_aSquareColors[1].g = E.g + (S.g - E.g) * ((c - u.x + u.y) / (2.0f * c));
    m_aSquareColors[1].b = E.b + (S.b - E.b) * ((c - u.x + u.y) / (2.0f * c));
    m_aSquareColors[1].a = E.a + (S.a - E.a) * ((c - u.x + u.y) / (2.0f * c));
    // (-1, 1)
    m_aSquareColors[2].r = E.r + (S.r - E.r) * ((c + u.x - u.y) / (2.0f * c));
    m_aSquareColors[2].g = E.g + (S.g - E.g) * ((c + u.x - u.y) / (2.0f * c));
    m_aSquareColors[2].b = E.b + (S.b - E.b) * ((c + u.x - u.y) / (2.0f * c));
    m_aSquareColors[2].a = E.a + (S.a - E.a) * ((c + u.x - u.y) / (2.0f * c));
    // (1, 1)
    m_aSquareColors[3].r = E.r + (S.r - E.r) * ((c - u.x - u.y) / (2.0f * c));
    m_aSquareColors[3].g = E.g + (S.g - E.g) * ((c - u.x - u.y) / (2.0f * c));
    m_aSquareColors[3].b = E.b + (S.b - E.b) * ((c - u.x - u.y) / (2.0f * c));
    m_aSquareColors[3].a = E.a + (S.a - E.a) * ((c - u.x - u.y) / (2.0f * c));
}

const Color3B& LayerGradient::getStartColor() const
{
    return m_tRealColor;
}

void LayerGradient::setStartColor(const Color3B& color)
{
    setColor(color);
}

void LayerGradient::setEndColor(const Color3B& color)
{
    m_tEndColor = color;
    updateColor();
}

const Color3B& LayerGradient::getEndColor() const
{
    return m_tEndColor;
}

void LayerGradient::setStartOpacity(GLubyte o)
{
    m_cStartOpacity = o;
    updateColor();
}

GLubyte LayerGradient::getStartOpacity() const
{
    return m_cStartOpacity;
}

void LayerGradient::setEndOpacity(GLubyte o)
{
    m_cEndOpacity = o;
    updateColor();
}

GLubyte LayerGradient::getEndOpacity() const
{
    return m_cEndOpacity;
}

void LayerGradient::setVector(const Point& var)
{
    m_tAlongVector = var;
    updateColor();
}

const Point& LayerGradient::getVector() const
{
    return m_tAlongVector;
}

bool LayerGradient::isCompressedInterpolation() const
{
    return m_bCompressedInterpolation;
}

void LayerGradient::setCompressedInterpolation(bool compress)
{
    m_bCompressedInterpolation = compress;
    updateColor();
}

/// MultiplexLayer

LayerMultiplex::LayerMultiplex()
: m_uEnabledLayer(0)
, m_pLayers(NULL)
{
}
LayerMultiplex::~LayerMultiplex()
{
    if (m_pLayers)
    {
        for (auto& item : *m_pLayers)
        {
            static_cast<Layer*>(item)->cleanup();
        }
        m_pLayers->release();
    }
}

LayerMultiplex * LayerMultiplex::create(Layer * layer, ...)
{
    va_list args;
    va_start(args,layer);

    LayerMultiplex * pMultiplexLayer = new LayerMultiplex();
    if(pMultiplexLayer && pMultiplexLayer->initWithLayers(layer, args))
    {
        pMultiplexLayer->autorelease();
        va_end(args);
        return pMultiplexLayer;
    }
    va_end(args);
    CC_SAFE_DELETE(pMultiplexLayer);
    return NULL;
}

LayerMultiplex * LayerMultiplex::createWithLayer(Layer* layer)
{
    return LayerMultiplex::create(layer, NULL);
}

LayerMultiplex* LayerMultiplex::create()
{
    LayerMultiplex* pRet = new LayerMultiplex();
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

LayerMultiplex* LayerMultiplex::createWithArray(Array* arrayOfLayers)
{
    LayerMultiplex* pRet = new LayerMultiplex();
    if (pRet && pRet->initWithArray(arrayOfLayers))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void LayerMultiplex::addLayer(Layer* layer)
{
    CCASSERT(m_pLayers, "");
    m_pLayers->addObject(layer);
}

bool LayerMultiplex::init()
{
    if (Layer::init())
    {
        m_pLayers = Array::create();
        m_pLayers->retain();

        m_uEnabledLayer = 0;
        return true;
    }
    return false;
}

bool LayerMultiplex::initWithLayers(Layer *layer, va_list params)
{
    if (Layer::init())
    {
        m_pLayers = Array::createWithCapacity(5);
        m_pLayers->retain();
        m_pLayers->addObject(layer);

        Layer *l = va_arg(params,Layer*);
        while( l ) {
            m_pLayers->addObject(l);
            l = va_arg(params,Layer*);
        }

        m_uEnabledLayer = 0;
        this->addChild((Node*)m_pLayers->getObjectAtIndex(m_uEnabledLayer));
        return true;
    }

    return false;
}

bool LayerMultiplex::initWithArray(Array* arrayOfLayers)
{
    if (Layer::init())
    {
        m_pLayers = Array::createWithCapacity(arrayOfLayers->count());
        m_pLayers->addObjectsFromArray(arrayOfLayers);
        m_pLayers->retain();

        m_uEnabledLayer = 0;
        this->addChild((Node*)m_pLayers->getObjectAtIndex(m_uEnabledLayer));
        return true;
    }
    return false;
}

void LayerMultiplex::switchTo(int n)
{
    CCASSERT( n < m_pLayers->count(), "Invalid index in MultiplexLayer switchTo message" );

    this->removeChild((Node*)m_pLayers->getObjectAtIndex(m_uEnabledLayer), true);

    m_uEnabledLayer = n;

    this->addChild((Node*)m_pLayers->getObjectAtIndex(n));
}

void LayerMultiplex::switchToAndReleaseMe(int n)
{
    CCASSERT( n < m_pLayers->count(), "Invalid index in MultiplexLayer switchTo message" );

    this->removeChild((Node*)m_pLayers->getObjectAtIndex(m_uEnabledLayer), true);

    //[layers replaceObjectAtIndex:enabledLayer withObject:[NSNull null]];
    m_pLayers->replaceObjectAtIndex(m_uEnabledLayer, NULL);

    m_uEnabledLayer = n;

    this->addChild((Node*)m_pLayers->getObjectAtIndex(n));
}

NS_CC_END
