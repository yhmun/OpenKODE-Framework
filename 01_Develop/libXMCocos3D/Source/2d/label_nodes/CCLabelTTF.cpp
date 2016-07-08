/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelTTF.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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

#include "2d/label_nodes/CCLabelTTF.h"
#include "2d/CCDirector.h"
#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "platform/CCApplication.h"

NS_CC_BEGIN

#if CC_USE_LA88_LABELS
#define SHADER_PROGRAM GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR
#else
#define SHADER_PROGRAM GLProgram::SHADER_NAME_POSITION_TEXTUREA8Color
#endif

//
//CCLabelTTF
//
LabelTTF::LabelTTF()
: m_eAlignmentH(TextHAlignment::CENTER)
, m_eAlignmentV(TextVAlignment::TOP)
, m_sFontName("")
, m_fFontSize(0.0)
, m_sString("")
, m_bShadowEnabled(false)
, m_bStrokeEnabled(false)
, m_tTextFillColor(Color3B::WHITE)
{
}

LabelTTF::~LabelTTF()
{
}

LabelTTF * LabelTTF::create()
{
    LabelTTF * ret = new LabelTTF();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

LabelTTF * LabelTTF::create(const std::string& string, const std::string& fontName, float fontSize)
{
    return LabelTTF::create(string, fontName, fontSize,
                              Size::ZERO, TextHAlignment::CENTER, TextVAlignment::TOP);
}

LabelTTF * LabelTTF::create(const std::string& string, const std::string& fontName, float fontSize,
                                const Size& dimensions, TextHAlignment hAlignment)
{
    return LabelTTF::create(string, fontName, fontSize, dimensions, hAlignment, TextVAlignment::TOP);
}

LabelTTF* LabelTTF::create(const std::string& string, const std::string& fontName, float fontSize,
                               const Size &dimensions, TextHAlignment hAlignment, 
                               TextVAlignment vAlignment)
{
    LabelTTF *ret = new LabelTTF();
    if(ret && ret->initWithString(string, fontName, fontSize, dimensions, hAlignment, vAlignment))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

LabelTTF * LabelTTF::createWithFontDefinition(const std::string& string, FontDefinition &textDefinition)
{
    LabelTTF *ret = new LabelTTF();
    if(ret && ret->initWithStringAndTextDefinition(string, textDefinition))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool LabelTTF::init()
{
    return this->initWithString("", "Helvetica", 12);
}

bool LabelTTF::initWithString(const std::string& label, const std::string& fontName, float fontSize,
                                const Size& dimensions, TextHAlignment alignment)
{
    return this->initWithString(label, fontName, fontSize, dimensions, alignment, TextVAlignment::TOP);
}

bool LabelTTF::initWithString(const std::string& label, const std::string& fontName, float fontSize)
{
    return this->initWithString(label, fontName, fontSize, 
                                Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
}

bool LabelTTF::initWithString(const std::string& string, const std::string& fontName, float fontSize,
                                const cocos2d::Size &dimensions, TextHAlignment hAlignment,
                                TextVAlignment vAlignment)
{
    if (Sprite::init())
    {
        // shader program
        this->setShaderProgram(ShaderCache::getInstance()->getProgram(SHADER_PROGRAM));
        
        m_tDimensions = Size(dimensions.width, dimensions.height);
        m_eAlignmentH = hAlignment;
        m_eAlignmentV = vAlignment;
        m_sFontName = fontName;
        m_fFontSize = fontSize;
        
        this->setString(string);
        
        return true;
    }
    
    return false;
}

bool LabelTTF::initWithStringAndTextDefinition(const std::string& string, FontDefinition &textDefinition)
{
    if (Sprite::init())
    {
        // shader program
        this->setShaderProgram(ShaderCache::getInstance()->getProgram(SHADER_PROGRAM));
        
        // prepare everythin needed to render the label
        _updateWithTextDefinition(textDefinition, false);
        
        // set the string
        this->setString(string);
        
        //
        return true;
    }
    else
    {
        return false;
    }
}


void LabelTTF::setString(const std::string &string)
{
    if (m_sString.compare(string))
    {
        m_sString = string;
        
        this->updateTexture();
    }
}

const std::string& LabelTTF::getString() const
{
    return m_sString;
}

const char* LabelTTF::description() const
{
    return String::createWithFormat("<LabelTTF | FontName = %s, FontSize = %.1f>", m_sFontName.c_str(), m_fFontSize)->getCString();
}

TextHAlignment LabelTTF::getHorizontalAlignment() const
{
    return m_eAlignmentH;
}

void LabelTTF::setHorizontalAlignment(TextHAlignment alignment)
{
    if (alignment != m_eAlignmentH)
    {
        m_eAlignmentH = alignment;
        
        // Force update
        if (m_sString.size() > 0)
        {
            this->updateTexture();
        }
    }
}

TextVAlignment LabelTTF::getVerticalAlignment() const
{
    return m_eAlignmentV;
}

void LabelTTF::setVerticalAlignment(TextVAlignment verticalAlignment)
{
    if (verticalAlignment != m_eAlignmentV)
    {
        m_eAlignmentV = verticalAlignment;
        
        // Force update
        if (m_sString.size() > 0)
        {
            this->updateTexture();
        }
    }
}

const Size& LabelTTF::getDimensions() const
{
    return m_tDimensions;
}

void LabelTTF::setDimensions(const Size &dim)
{
    // XXX: float comparison... very unreliable
    if (dim.width != m_tDimensions.width || dim.height != m_tDimensions.height)
    {
        m_tDimensions = dim;
        
        // Force update
        if (m_sString.size() > 0)
        {
            this->updateTexture();
        }
    }
}

float LabelTTF::getFontSize() const
{
    return m_fFontSize;
}

void LabelTTF::setFontSize(float fontSize)
{
    // XXX: float comparison... very unreliable
    if (m_fFontSize != fontSize)
    {
        m_fFontSize = fontSize;
        
        // Force update
        if (m_sString.size() > 0)
        {
            this->updateTexture();
        }
    }
}

const std::string& LabelTTF::getFontName() const
{
    return m_sFontName;
}

void LabelTTF::setFontName(const std::string& fontName)
{
    if (m_sFontName.compare(fontName))
    {
        m_sFontName = fontName;
        
        // Force update
        if (m_sString.size() > 0)
        {
            this->updateTexture();
        }
    }
}

// Helper
bool LabelTTF::updateTexture()
{
    Texture2D *tex;
    tex = new Texture2D();
    
    if (!tex)
        return false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
        FontDefinition texDef = _prepareTextDefinition(true);
        tex->initWithString( m_sString.c_str(), texDef );
    
#else
    
        tex->initWithString( m_sString.c_str(),
                            m_sFontName.c_str(),
                            m_fFontSize * CC_CONTENT_SCALE_FACTOR(),
                            CC_SIZE_POINTS_TO_PIXELS(m_tDimensions),
                            m_eAlignmentH,
                            m_eAlignmentV);
    
#endif
    
    // set the texture
    this->setTexture(tex);
    // release it
    tex->release();
    
    // set the size in the sprite
    Rect rect =Rect::ZERO;
    rect.size   = m_pTexture->getContentSize();
    this->setTextureRect(rect);
    
    //ok
    return true;
}

void LabelTTF::enableShadow(const Size &shadowOffset, float shadowOpacity, float shadowBlur, bool updateTexture)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
        bool valueChanged = false;
        
        if (false == m_bShadowEnabled)
        {
            m_bShadowEnabled = true;
            valueChanged    = true;
        }
        
        if ( (m_tShadowOffset.width != shadowOffset.width) || (m_tShadowOffset.height!=shadowOffset.height) )
        {
            m_tShadowOffset.width = shadowOffset.width;
            m_tShadowOffset.height = shadowOffset.height;
            
            valueChanged = true;
        }
        
        if (m_fShadowOpacity != shadowOpacity )
        {
            m_fShadowOpacity = shadowOpacity;
            valueChanged = true;
        }

        if (m_fShadowBlur != shadowBlur)
        {
            m_fShadowBlur = shadowBlur;
            valueChanged = true;
        }
        
        if ( valueChanged && updateTexture )
        {
            this->updateTexture();
        }
    
#else
        CCLOGERROR("Currently only supported on iOS and Android!");
#endif
    
}

void LabelTTF::disableShadow(bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
        if (m_bShadowEnabled)
        {
            m_bShadowEnabled = false;
    
            if (updateTexture)
                this->updateTexture();
        }
    
#else
        CCLOGERROR("Currently only supported on iOS and Android!");
#endif
}

void LabelTTF::enableStroke(const Color3B &strokeColor, float strokeSize, bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
        bool valueChanged = false;
        
        if(m_bStrokeEnabled == false)
        {
            m_bStrokeEnabled = true;
            valueChanged = true;
        }
        
        if ( (m_tStrokeColor.r != strokeColor.r) || (m_tStrokeColor.g != strokeColor.g) || (m_tStrokeColor.b != strokeColor.b) )
        {
            m_tStrokeColor = strokeColor;
            valueChanged = true;
        }
        
        if (m_fStrokeSize!=strokeSize)
        {
            m_fStrokeSize = strokeSize;
            valueChanged = true;
        }
        
        if ( valueChanged && updateTexture )
        {
            this->updateTexture();
        }
    
#else
        CCLOGERROR("Currently only supported on iOS and Android!");
#endif
    
}

void LabelTTF::disableStroke(bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
        if (m_bStrokeEnabled)
        {
            m_bStrokeEnabled = false;
            
            if (updateTexture)
                this->updateTexture();
        }
    
#else
        CCLOGERROR("Currently only supported on iOS and Android!");
#endif
    
}

void LabelTTF::setFontFillColor(const Color3B &tintColor, bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (m_tTextFillColor.r != tintColor.r || m_tTextFillColor.g != tintColor.g || m_tTextFillColor.b != tintColor.b)
        {
            m_tTextFillColor = tintColor;
            
            if (updateTexture)
                this->updateTexture();
        }
#else
        CCLOGERROR("Currently only supported on iOS and Android!");
#endif
}

void LabelTTF::setTextDefinition(const FontDefinition& theDefinition)
{
    _updateWithTextDefinition(theDefinition, true);
}

FontDefinition LabelTTF::getTextDefinition()
{
    return _prepareTextDefinition(false);
}

void LabelTTF::_updateWithTextDefinition(const FontDefinition& textDefinition, bool mustUpdateTexture)
{
    m_tDimensions = Size(textDefinition.m_tDimensions.width, textDefinition.m_tDimensions.height);
    m_eAlignmentH = textDefinition.m_eAlignment;
    m_eAlignmentV = textDefinition.m_eVertAlignment;
    
    m_sFontName = textDefinition.m_sFontName;
    m_fFontSize = textDefinition.m_nFontSize;
    
    
    // shadow
    if ( textDefinition.m_tShadow.m_bShadowEnabled )
    {
        enableShadow(textDefinition.m_tShadow.m_tShadowOffset, textDefinition.m_tShadow.m_fShadowOpacity, textDefinition.m_tShadow.m_fShadowBlur, false);
    }
    
    // stroke
    if ( textDefinition.m_tStroke.m_bStrokeEnabled )
    {
        enableStroke(textDefinition.m_tStroke.m_tStrokeColor, textDefinition.m_tStroke.m_fStrokeSize, false);
    }
    
    // fill color
    setFontFillColor(textDefinition.m_tFontFillColor, false);
    
    if (mustUpdateTexture)
        updateTexture();
}

FontDefinition LabelTTF::_prepareTextDefinition(bool adjustForResolution)
{
    FontDefinition texDef;
    
    if (adjustForResolution)
        texDef.m_nFontSize = m_fFontSize * CC_CONTENT_SCALE_FACTOR();
    else
        texDef.m_nFontSize = m_fFontSize;
    
    texDef.m_sFontName = m_sFontName;
    texDef.m_eAlignment = m_eAlignmentH;
    texDef.m_eVertAlignment = m_eAlignmentV;
    
    
    if (adjustForResolution)
        texDef.m_tDimensions = CC_SIZE_POINTS_TO_PIXELS(m_tDimensions);
    else
        texDef.m_tDimensions = m_tDimensions;
    
    
    // stroke
    if ( m_bStrokeEnabled )
    {
        texDef.m_tStroke.m_bStrokeEnabled = true;
        texDef.m_tStroke.m_tStrokeColor   = m_tStrokeColor;
        
        if (adjustForResolution)
            texDef.m_tStroke.m_fStrokeSize = m_fStrokeSize * CC_CONTENT_SCALE_FACTOR();
        else
            texDef.m_tStroke.m_fStrokeSize = m_fStrokeSize;
    }
    else
    {
        texDef.m_tStroke.m_bStrokeEnabled = false;
    }
    
    // shadow
    if ( m_bShadowEnabled )
    {
        texDef.m_tShadow.m_bShadowEnabled = true;
        texDef.m_tShadow.m_fShadowBlur = m_fShadowBlur;
        texDef.m_tShadow.m_fShadowOpacity = m_fShadowOpacity;
        
        if (adjustForResolution)
            texDef.m_tShadow.m_tShadowOffset = CC_SIZE_POINTS_TO_PIXELS(m_tShadowOffset);
        else
            texDef.m_tShadow.m_tShadowOffset = m_tShadowOffset;
    }
    else
    {
        texDef.m_tShadow.m_bShadowEnabled = false;
    }
    
    // text tint
    texDef.m_tFontFillColor = m_tTextFillColor;
    
    return texDef;
}

NS_CC_END
