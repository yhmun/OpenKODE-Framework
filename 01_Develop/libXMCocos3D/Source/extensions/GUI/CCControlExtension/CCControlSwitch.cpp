/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlSwitch.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2012      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://yannickloriot.com
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

#include "extensions/GUI/CCControlExtension/CCControlSwitch.h"
#include "2d/actions/CCActionTween.h"
#include "2d/label_nodes/CCLabelTTF.h"
#include "2d/misc_nodes/CCRenderTexture.h"
#include "shaders/ccShaders.h"

NS_CC_EXT_BEGIN

// ControlSwitchSprite
class ControlSwitchSprite : public Sprite, public ActionTweenDelegate
{
public:
    /**
     * @js NA
     * @lua NA
     */
    ControlSwitchSprite();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~ControlSwitchSprite();
    /**
     * @js NA
     * @lua NA
     */
    bool initWithMaskSprite(
        Sprite *maskSprite, 
        Sprite *onSprite, 
        Sprite *offSprite,
        Sprite *thumbSprite,
        LabelTTF* onLabel, 
        LabelTTF* offLabel);
    /**
     * @js NA
     * @lua NA
     */
    void draw();
    /**
     * @js NA
     * @lua NA
     */
    void needsLayout();
    /**
     * @js NA
     * @lua NA
     */
    void setSliderXPosition(float sliderXPosition);
    /**
     * @js NA
     * @lua NA
     */
    float getSliderXPosition() {return _sliderXPosition;}
    /**
     * @js NA
     * @lua NA
     */
    float onSideWidth();
    /**
     * @js NA
     * @lua NA
     */
    float offSideWidth();
    /**
     * @js NA
     * @lua NA
     */
    virtual void updateTweenAction(float value, const char* key);
/** Contains the position (in x-axis) of the slider inside the receiver. */
    float _sliderXPosition;
    CC_SYNTHESIZE(float, _onPosition, OnPosition)
    CC_SYNTHESIZE(float, _offPosition, OffPosition)
    
    CC_SYNTHESIZE_RETAIN(Texture2D*, _maskTexture, MaskTexture)
    CC_SYNTHESIZE(GLuint, _textureLocation, TextureLocation)
    CC_SYNTHESIZE(GLuint, _maskLocation, MaskLocation)
    
    CC_SYNTHESIZE_RETAIN(Sprite*, _onSprite, OnSprite)
    CC_SYNTHESIZE_RETAIN(Sprite*, _offSprite, OffSprite)
    CC_SYNTHESIZE_RETAIN(Sprite*, _thumbSprite, ThumbSprite)
    CC_SYNTHESIZE_RETAIN(LabelTTF*, _onLabel, OnLabel)
    CC_SYNTHESIZE_RETAIN(LabelTTF*, _offLabel, OffLabel)
};

ControlSwitchSprite::ControlSwitchSprite()
: _sliderXPosition(0.0f)
, _onPosition(0.0f)
, _offPosition(0.0f)
, _maskTexture(NULL)
, _textureLocation(0)
, _maskLocation(0)
, _onSprite(NULL)
, _offSprite(NULL)
, _thumbSprite(NULL)
, _onLabel(NULL)
, _offLabel(NULL)
{

}

ControlSwitchSprite::~ControlSwitchSprite()
{
    CC_SAFE_RELEASE(_onSprite);
    CC_SAFE_RELEASE(_offSprite);
    CC_SAFE_RELEASE(_thumbSprite);
    CC_SAFE_RELEASE(_onLabel);
    CC_SAFE_RELEASE(_offLabel);
    CC_SAFE_RELEASE(_maskTexture);
}

bool ControlSwitchSprite::initWithMaskSprite(
    Sprite *maskSprite, 
    Sprite *onSprite, 
    Sprite *offSprite,
    Sprite *thumbSprite,
    LabelTTF* onLabel, 
    LabelTTF* offLabel)
{
    if (Sprite::initWithTexture(maskSprite->getTexture()))
    {
        // Sets the default values
        _onPosition             = 0;
        _offPosition            = -onSprite->getContentSize().width + thumbSprite->getContentSize().width / 2;
        _sliderXPosition        = _onPosition; 

        setOnSprite(onSprite);
        setOffSprite(offSprite);
        setThumbSprite(thumbSprite);
        setOnLabel(onLabel);
        setOffLabel(offLabel);

        addChild(_thumbSprite);

        // Set up the mask with the Mask shader
        setMaskTexture(maskSprite->getTexture());
        GLProgram* pProgram = new GLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccExSwitchMask_frag);
        setShaderProgram(pProgram);
        pProgram->release();

        CHECK_GL_ERROR_DEBUG();

        getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        getShaderProgram()->addAttribute(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        CHECK_GL_ERROR_DEBUG();

        getShaderProgram()->link();
        CHECK_GL_ERROR_DEBUG();

        getShaderProgram()->updateUniforms();
        CHECK_GL_ERROR_DEBUG();                

        _textureLocation    = glGetUniformLocation( getShaderProgram()->getProgram(), "u_texture");
        _maskLocation       = glGetUniformLocation( getShaderProgram()->getProgram(), "u_mask");
        CHECK_GL_ERROR_DEBUG();

        setContentSize(_maskTexture->getContentSize());

        needsLayout();
        return true;
    }
    return false;
}

void ControlSwitchSprite::updateTweenAction(float value, const char* key)
{
    CCLOG("key = %s, value = %f", key, value);
    setSliderXPosition(value);
}

void ControlSwitchSprite::draw()
{
    CC_NODE_DRAW_SETUP();

    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    GL::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    getShaderProgram()->setUniformsForBuiltins();

    GL::bindTexture2DN(0, getTexture()->getName());
    glUniform1i(_textureLocation, 0);

    GL::bindTexture2DN(1, _maskTexture->getName());
    glUniform1i(_maskLocation, 1);

#define kQuadSize sizeof(m_tQuad.bl)
#ifdef EMSCRIPTEN
    long offset = 0;
    setGLBufferData(&m_tQuad, 4 * kQuadSize, 0);
#else
    long offset = (long)&m_tQuad;
#endif // EMSCRIPTEN

    // vertex
    int diff = offsetof( V3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

    // texCoods
    diff = offsetof( V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

    // color
    diff = offsetof( V3F_C4B_T2F, colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    GL::bindTexture2DN(0, 0);
}

void ControlSwitchSprite::needsLayout()
{
    _onSprite->setPosition(Point(_onSprite->getContentSize().width / 2 + _sliderXPosition,
        _onSprite->getContentSize().height / 2));
    _offSprite->setPosition(Point(_onSprite->getContentSize().width + _offSprite->getContentSize().width / 2 + _sliderXPosition, 
        _offSprite->getContentSize().height / 2));
    _thumbSprite->setPosition(Point(_onSprite->getContentSize().width + _sliderXPosition,
        _maskTexture->getContentSize().height / 2));

    if (_onLabel)
    {
        _onLabel->setPosition(Point(_onSprite->getPosition().x - _thumbSprite->getContentSize().width / 6,
            _onSprite->getContentSize().height / 2));
    }
    if (_offLabel)
    {
        _offLabel->setPosition(Point(_offSprite->getPosition().x + _thumbSprite->getContentSize().width / 6,
            _offSprite->getContentSize().height / 2));
    }

    RenderTexture *rt = RenderTexture::create((int)_maskTexture->getContentSize().width, (int)_maskTexture->getContentSize().height);

    rt->begin();
    _onSprite->visit();
    _offSprite->visit();

    if (_onLabel)
    {
        _onLabel->visit();
    }
    if (_offLabel)
    {
        _offLabel->visit();
    }

    rt->end();

    setTexture(rt->getSprite()->getTexture());
    setFlippedY(true);
}

void ControlSwitchSprite::setSliderXPosition(float sliderXPosition)
{
    if (sliderXPosition <= _offPosition)
    {
        // Off
        sliderXPosition = _offPosition;
    } else if (sliderXPosition >= _onPosition)
    {
        // On
        sliderXPosition = _onPosition;
    }

    _sliderXPosition    = sliderXPosition;

    needsLayout();
}


float ControlSwitchSprite::onSideWidth()
{
    return _onSprite->getContentSize().width;
}

float ControlSwitchSprite::offSideWidth()
{
    return _offSprite->getContentSize().height;
}


// ControlSwitch

ControlSwitch::ControlSwitch()
: m_pSwitchSprite(NULL)
, m_fInitialTouchXPosition(0.0f)
, m_bMoved(false)
, m_bOn(false)
{

}

ControlSwitch::~ControlSwitch()
{
    CC_SAFE_RELEASE(m_pSwitchSprite);
}

bool ControlSwitch::initWithMaskSprite(Sprite *maskSprite, Sprite * onSprite, Sprite * offSprite, Sprite * thumbSprite)
{
    return initWithMaskSprite(maskSprite, onSprite, offSprite, thumbSprite, NULL, NULL);
}

ControlSwitch* ControlSwitch::create(Sprite *maskSprite, Sprite * onSprite, Sprite * offSprite, Sprite * thumbSprite)
{
    ControlSwitch* pRet = new ControlSwitch();
    if (pRet && pRet->initWithMaskSprite(maskSprite, onSprite, offSprite, thumbSprite, NULL, NULL))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool ControlSwitch::initWithMaskSprite(Sprite *maskSprite, Sprite * onSprite, Sprite * offSprite, Sprite * thumbSprite, LabelTTF* onLabel, LabelTTF* offLabel)
{
    if (Control::init())
    {
        CCASSERT(maskSprite,    "Mask must not be nil.");
        CCASSERT(onSprite,      "onSprite must not be nil.");
        CCASSERT(offSprite,     "offSprite must not be nil.");
        CCASSERT(thumbSprite,   "thumbSprite must not be nil.");
        
        m_bOn = true;

        m_pSwitchSprite = new ControlSwitchSprite();
        m_pSwitchSprite->initWithMaskSprite(maskSprite,
                                            onSprite,
                                           offSprite,
                                           thumbSprite,
                                           onLabel,
                                           offLabel);
        m_pSwitchSprite->setPosition(Point(m_pSwitchSprite->getContentSize().width / 2, m_pSwitchSprite->getContentSize().height / 2));
        addChild(m_pSwitchSprite);
        
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Point(0.5f, 0.5f));
        setContentSize(m_pSwitchSprite->getContentSize());
        return true;
    }
    return false;
}

ControlSwitch* ControlSwitch::create(Sprite *maskSprite, Sprite * onSprite, Sprite * offSprite, Sprite * thumbSprite, LabelTTF* onLabel, LabelTTF* offLabel)
{
    ControlSwitch* pRet = new ControlSwitch();
    if (pRet && pRet->initWithMaskSprite(maskSprite, onSprite, offSprite, thumbSprite, onLabel, offLabel))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void ControlSwitch::setOn(bool isOn)
{
    setOn(isOn, false);
}

void ControlSwitch::setOn(bool isOn, bool animated)
{
    m_bOn     = isOn;
    
    if (animated) {
        m_pSwitchSprite->runAction
        (
            ActionTween::create
                (
                    0.2f,
                    "sliderXPosition",
                    m_pSwitchSprite->getSliderXPosition(),
                    (m_bOn) ? m_pSwitchSprite->getOnPosition() : m_pSwitchSprite->getOffPosition()
                )
         );
    }
    else {
        m_pSwitchSprite->setSliderXPosition((m_bOn) ? m_pSwitchSprite->getOnPosition() : m_pSwitchSprite->getOffPosition());
    }
    
    sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
}

void ControlSwitch::setEnabled(bool enabled)
{
    m_bEnabled = enabled;
    if (m_pSwitchSprite != NULL)
    {
        m_pSwitchSprite->setOpacity((enabled) ? 255 : 128);
    } 
}

Point ControlSwitch::locationFromTouch(Touch* pTouch)
{
    Point touchLocation   = pTouch->getLocation();                      // Get the touch position
    touchLocation           = this->convertToNodeSpace(touchLocation);                  // Convert to the node space of this class
    
    return touchLocation;
}

bool ControlSwitch::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!isTouchInside(pTouch) || !isEnabled() || !isVisible())
    {
        return false;
    }
    
    m_bMoved = false;
    
    Point location = this->locationFromTouch(pTouch);
    
    m_fInitialTouchXPosition = location.x - m_pSwitchSprite->getSliderXPosition();
    
    m_pSwitchSprite->getThumbSprite()->setColor(Color3B::GRAY);
    m_pSwitchSprite->needsLayout();
    
    return true;
}

void ControlSwitch::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point location    = this->locationFromTouch(pTouch);
    location            = Point(location.x - m_fInitialTouchXPosition, 0);
    
    m_bMoved              = true;
    
    m_pSwitchSprite->setSliderXPosition(location.x);
}

void ControlSwitch::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    Point location   = this->locationFromTouch(pTouch);
    
    m_pSwitchSprite->getThumbSprite()->setColor(Color3B::WHITE);
    
    if (hasMoved())
    {
        setOn(!(location.x < m_pSwitchSprite->getContentSize().width / 2), true);
    } 
    else
    {
        setOn(!m_bOn, true);
    }
}

void ControlSwitch::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    Point location   = this->locationFromTouch(pTouch);
    
    m_pSwitchSprite->getThumbSprite()->setColor(Color3B::WHITE);
    
    if (hasMoved())
    {
        setOn(!(location.x < m_pSwitchSprite->getContentSize().width / 2), true);
    } else
    {
        setOn(!m_bOn, true);
    }
}

NS_CC_EXT_END
