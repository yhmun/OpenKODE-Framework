/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabel.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2013      Zynga Inc.
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

#include "2d/label_nodes/CCLabel.h"
#include "2d/label_nodes/CCFontDefinition.h"
#include "2d/label_nodes/CCFontAtlasCache.h"
#include "2d/label_nodes/CCLabelTextFormatter.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/sprite_nodes/CCSpriteFrame.h"
#include "support/ccUTF8.h"

NS_CC_BEGIN

Label* Label::createWithTTF(const std::string& label, const std::string& fontFilePath, int fontSize, int lineSize, TextHAlignment alignment, GlyphCollection glyphs, const char *customGlyphs )
{
    FontAtlas *tmpAtlas = FontAtlasCache::getFontAtlasTTF(fontFilePath.c_str(), fontSize, glyphs, customGlyphs);

    if (!tmpAtlas)
        return nullptr;
    
    // create the actual label
    Label* templabel = Label::createWithAtlas(tmpAtlas, alignment, lineSize);
    
    if (templabel)
    {
        templabel->setText(label, lineSize, alignment, false);
        return templabel;
    }
    
    return nullptr;
}

Label* Label::createWithBMFont(const std::string& label, const std::string& bmfontFilePath, TextHAlignment alignment, int lineSize)
{
    
    FontAtlas *tmpAtlas = FontAtlasCache::getFontAtlasFNT(bmfontFilePath.c_str());
    
    if (!tmpAtlas)
        return 0;
    
    Label* templabel = Label::createWithAtlas(tmpAtlas, alignment, lineSize);
    
    if (templabel)
    {
        templabel->setText(label, lineSize, alignment, false);
        return templabel;
    }
    else
    {
        return 0;
    }
     
    return 0;
}

Label* Label::createWithAtlas(FontAtlas *atlas, TextHAlignment alignment, int lineSize)
{
    Label *ret = new Label(atlas, alignment);
    
    if (!ret)
        return 0;
    
    if( ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return 0;
    }
    
    return ret;
}

Label::Label(FontAtlas *atlas, TextHAlignment alignment)
: m_pReusedLetter(nullptr)
, m_bLineBreakWithoutSpaces(false)
, m_eAlignment(alignment)
, m_pCurrentUTF16String(0)
, m_pOriginalUTF16String(0)
, m_pAdvances(0)
, m_pFontAtlas(atlas)
, m_tDisplayedColor(Color3B::WHITE)
, m_tRealColor(Color3B::WHITE)
, m_bCascadeColorEnabled(true)
, m_bCascadeOpacityEnabled(true)
, m_cDisplayedOpacity(255)
, m_cRealOpacity(255)
, m_bIsOpacityModifyRGB(true)
{
}

Label::~Label()
{   
    delete [] m_pCurrentUTF16String;
    delete [] m_pOriginalUTF16String;
    delete [] m_pAdvances;
    
    if (m_pFontAtlas)
        FontAtlasCache::releaseFontAtlas(m_pFontAtlas);

    delete m_pReusedLetter;
}

bool Label::init()
{ 
    if(m_pFontAtlas)
    {
        m_pReusedLetter = new Sprite;
        m_pReusedLetter->initWithTexture(&m_pFontAtlas->getTexture(0));
        m_pReusedLetter->setOpacityModifyRGB(m_bIsOpacityModifyRGB);
        return SpriteBatchNode::initWithTexture(&m_pFontAtlas->getTexture(0), 30);
    }

    return true;
}

void Label::setString(const std::string &stringToRender)
{
    setText(stringToRender, m_fWidth, TextHAlignment::CENTER, false);
}

bool Label::setText(const std::string& stringToRender, float lineWidth, TextHAlignment alignment, bool lineBreakWithoutSpaces)
{
    if (!m_pFontAtlas)
        return false;
    
    // carloX
    // reset the string
    resetCurrentString();
    
    m_fWidth                  = lineWidth;
    m_eAlignment              = alignment;
    m_bLineBreakWithoutSpaces = lineBreakWithoutSpaces;
    
    // store locally common line height
    m_fCommonLineHeight = m_pFontAtlas->getCommonLineHeight();
    if (m_fCommonLineHeight <= 0)
        return false;
    
//    int numLetter = 0;
    unsigned short* utf16String = cc_utf8_to_utf16(stringToRender.c_str());
    if(!utf16String)
        return false;
    
    m_bCascadeColorEnabled = true;
    
    setCurrentString(utf16String);
    setOriginalString(utf16String);
    
    // align text
    alignText();
    
    // done here
    return true;
}

void Label::setAlignment(TextHAlignment alignment)
{
    // store the new alignment
    if (alignment != m_eAlignment)
    {
        // store
        m_eAlignment = alignment;
        
        // reset the string
        resetCurrentString();
        
        // need to align text again
        alignText();
    }
}

void Label::setWidth(float width)
{
    if (width != m_fWidth)
    {
        // store
        m_fWidth = width;
        
        
        // reset the string
        resetCurrentString();
        
        // need to align text again
        alignText();
    }
}

void Label::setLineBreakWithoutSpace(bool breakWithoutSpace)
{
    if (breakWithoutSpace != m_bLineBreakWithoutSpaces)
    {
        // store
        m_bLineBreakWithoutSpaces = breakWithoutSpace;
        
        // need to align text again
        alignText();
    }
}

void Label::setScale(float scale)
{
    Node::setScale(scale);
    alignText();
}

void Label::setScaleX(float scaleX)
{
    Node::setScaleX(scaleX);
    alignText();
}

void Label::setScaleY(float scaleY)
{
    Node::setScaleY(scaleY);
    alignText();
}

void Label::alignText()
{      
    if(m_pTextureAtlas)
        m_pTextureAtlas->removeAllQuads();  
    m_pFontAtlas->prepareLetterDefinitions(m_pCurrentUTF16String);
    LabelTextFormatter::createStringSprites(this);    
    if( LabelTextFormatter::multilineText(this) )      
        LabelTextFormatter::createStringSprites(this);
    
    LabelTextFormatter::alignText(this);
  
    int strLen = cc_wcslen(m_pCurrentUTF16String);  
    if (m_pChildren && m_pChildren->count() != 0)
    {
        for (auto child: *m_pChildren)
        {
            Node* pNode = static_cast<Node*>( child );
            if (pNode)
            {
                int tag = pNode->getTag();
                if(tag < 0 || tag >= strLen)
                    SpriteBatchNode::removeChild(pNode, true);
            }
        }
    }
    m_pReusedLetter->setBatchNode(nullptr);
   
    int vaildIndex = 0;
    Sprite* child = nullptr;
    Rect uvRect;
    for (int ctr = 0; ctr < strLen; ++ctr)
    {        
        if (m_aLettersInfo[ctr].def.validDefinition)
        {
            child = static_cast<Sprite*>( this->getChildByTag(ctr) );
            if (child)
            {
                uvRect.size.height = m_aLettersInfo[ctr].def.height;
                uvRect.size.width  = m_aLettersInfo[ctr].def.width;
                uvRect.origin.x    = m_aLettersInfo[ctr].def.U;
                uvRect.origin.y    = m_aLettersInfo[ctr].def.V;

                child->setTexture(&m_pFontAtlas->getTexture(m_aLettersInfo[ctr].def.textureID));
                child->setTextureRect(uvRect);              
            }
           
            updateSpriteWithLetterDefinition(m_pReusedLetter,m_aLettersInfo[ctr].def,&m_pFontAtlas->getTexture(m_aLettersInfo[ctr].def.textureID));
            m_pReusedLetter->setPosition(m_aLettersInfo[ctr].position);
            insertQuadFromSprite(m_pReusedLetter,vaildIndex++);
        }     
    }
}

bool Label::computeAdvancesForString(unsigned short int *stringToRender)
{
    if (m_pAdvances)
    {
        delete [] m_pAdvances;
        m_pAdvances = 0;
    }
    
    int letterCount = 0;
    m_pAdvances = m_pFontAtlas->getFont()->getAdvancesForTextUTF16(stringToRender, letterCount);
    
    if(!m_pAdvances)
        return false;
    else
        return true;
}

bool Label::setOriginalString(unsigned short *stringToSet)
{
    if (m_pOriginalUTF16String)
    {
        delete [] m_pOriginalUTF16String;
        m_pOriginalUTF16String = 0;
    }
    
    int newStringLenght = cc_wcslen(stringToSet);
    m_pOriginalUTF16String = new unsigned short int [newStringLenght + 1];
    kdMemset(m_pOriginalUTF16String, 0, (newStringLenght + 1) * 2);
    memcpy(m_pOriginalUTF16String, stringToSet, (newStringLenght * 2));
    m_pOriginalUTF16String[newStringLenght] = 0;
    
    return true;
}

bool Label::setCurrentString(unsigned short *stringToSet)
{
    // set the new string
    if (m_pCurrentUTF16String)
    {
        delete [] m_pCurrentUTF16String;
        m_pCurrentUTF16String = 0;
    }
    //
    m_pCurrentUTF16String  = stringToSet;
    // compute the advances
    return computeAdvancesForString(stringToSet);
}

void Label::resetCurrentString()
{
    if ((!m_pCurrentUTF16String) && (!m_pOriginalUTF16String))
        return;
    
    // set the new string
    if (m_pCurrentUTF16String)
    {
        delete [] m_pCurrentUTF16String;
        m_pCurrentUTF16String = 0;
    }
    
    int stringLenght = cc_wcslen(m_pOriginalUTF16String);
    m_pCurrentUTF16String = new unsigned short int [stringLenght + 1];
    memcpy(m_pCurrentUTF16String, m_pOriginalUTF16String, stringLenght * 2);
    m_pCurrentUTF16String[stringLenght] = 0;
    
}

Sprite * Label::updateSpriteWithLetterDefinition(Sprite *spriteToUpdate, const FontLetterDefinition &theDefinition, Texture2D *theTexture)
{
    if (!spriteToUpdate)
    {
        return 0;
    }
    else
    {
        Rect uvRect;
        uvRect.size.height = theDefinition.height;
        uvRect.size.width  = theDefinition.width;
        uvRect.origin.x    = theDefinition.U;
        uvRect.origin.y    = theDefinition.V;
        
        SpriteFrame *frame = SpriteFrame::createWithTexture(theTexture, uvRect);
        if (frame)
        {
            spriteToUpdate->setBatchNode(this); 
            spriteToUpdate->setTexture(theTexture);
            spriteToUpdate->setDisplayFrame(frame);
            spriteToUpdate->setAnchorPoint(Point(theDefinition.anchorX, theDefinition.anchorY));                                        
        }     
        
        return spriteToUpdate;
    }
}

bool Label::recordLetterInfo(const cocos2d::Point& point,unsigned short int theChar, int spriteIndex)
{
    if (static_cast<std::size_t>(spriteIndex) >= m_aLettersInfo.size())
    {
        LetterInfo tmpInfo;
        m_aLettersInfo.push_back(tmpInfo);
    }    
       
    m_pFontAtlas->getLetterDefinitionForChar(theChar, m_aLettersInfo[spriteIndex].def);
    m_aLettersInfo[spriteIndex].position = point;
    m_aLettersInfo[spriteIndex].contentSize.width = m_aLettersInfo[spriteIndex].def.width;
    m_aLettersInfo[spriteIndex].contentSize.height = m_aLettersInfo[spriteIndex].def.height;

    return m_aLettersInfo[spriteIndex].def.validDefinition;
}

bool Label::recordPlaceholderInfo(int spriteIndex)
{
    if (static_cast<std::size_t>(spriteIndex) >= m_aLettersInfo.size())
    {
        LetterInfo tmpInfo;
        m_aLettersInfo.push_back(tmpInfo);
    }

    m_aLettersInfo[spriteIndex].def.validDefinition = false;
    
    return false;
}

void Label::addChild(Node * child, int zOrder/* =0 */, int tag/* =0 */)
{
    CCASSERT(0, "addChild: is not supported on Label.");
}

///// PROTOCOL STUFF

Sprite * Label::getLetter(int ID)
{
    if (ID < getStringLenght())
    {       
        if(m_aLettersInfo[ID].def.validDefinition == false)
            return nullptr;
       
        Sprite* sp = static_cast<Sprite*>(this->getChildByTag(ID));

        if (!sp)
        {
            Rect uvRect;
            uvRect.size.height = m_aLettersInfo[ID].def.height;
            uvRect.size.width  = m_aLettersInfo[ID].def.width;
            uvRect.origin.x    = m_aLettersInfo[ID].def.U;
            uvRect.origin.y    = m_aLettersInfo[ID].def.V;

            sp = new Sprite();
            sp->initWithTexture(&m_pFontAtlas->getTexture(m_aLettersInfo[ID].def.textureID),uvRect);
            sp->setBatchNode(this);
            sp->setAnchorPoint(Point(m_aLettersInfo[ID].def.anchorX, m_aLettersInfo[ID].def.anchorY));                    
            sp->setPosition(m_aLettersInfo[ID].position);
            sp->setOpacity(m_cRealOpacity);
         
            this->addSpriteWithoutQuad(sp, ID, ID);
            sp->release();
        }
        return sp;
    }
    
    return nullptr;
}

float Label::getLetterPosXLeft( int index ) const
{
    return m_aLettersInfo[index].position.x * m_fScaleX - (m_aLettersInfo[index].contentSize.width * m_fScaleX * m_aLettersInfo[index].def.anchorX);
}

float Label::getLetterPosXRight( int index ) const
{
    return m_aLettersInfo[index].position.x * m_fScaleX + (m_aLettersInfo[index].contentSize.width * m_fScaleX * m_aLettersInfo[index].def.anchorX);
}

int Label::getCommonLineHeight() const
{
    return m_fCommonLineHeight;
}

int Label::getKerningForCharsPair(unsigned short first, unsigned short second) const
{
    return 0;
}

int Label::getXOffsetForChar(unsigned short c) const
{
    FontLetterDefinition tempDefinition;
    bool validDefinition = m_pFontAtlas->getLetterDefinitionForChar(c, tempDefinition);
    if (!validDefinition)
        return -1;
    
    return (tempDefinition.offsetX);
}

int Label::getYOffsetForChar(unsigned short c) const
{
    FontLetterDefinition tempDefinition;
    bool validDefinition = m_pFontAtlas->getLetterDefinitionForChar(c, tempDefinition);
    if (!validDefinition)
        return -1;
    
    return (tempDefinition.offsetY);
}

int Label::getAdvanceForChar(unsigned short c, int hintPositionInString) const
{
    if (m_pAdvances)
    {
        // not that advance contains the X offset already
        FontLetterDefinition tempDefinition;
        bool validDefinition = m_pFontAtlas->getLetterDefinitionForChar(c, tempDefinition);
        if (!validDefinition)
            return -1;
        
        return (m_pAdvances[hintPositionInString].width);
    }
    else
    {
        return -1;
    }
}

Rect Label::getRectForChar(unsigned short c) const
{
    return m_pFontAtlas->getFont()->getRectForChar(c);
}

// string related stuff
int Label::getStringNumLines() const
{
    int quantityOfLines = 1;
    
    unsigned int stringLen = m_pCurrentUTF16String ? cc_wcslen(m_pCurrentUTF16String) : 0;
    if (stringLen == 0)
        return (-1);
    
    // count number of lines
    for (unsigned int i = 0; i < stringLen - 1; ++i)
    {
        unsigned short c = m_pCurrentUTF16String[i];
        if (c == '\n')
        {
            quantityOfLines++;
        }
    }
    
    return quantityOfLines;
}

int Label::getStringLenght() const
{
    return m_pCurrentUTF16String ? cc_wcslen(m_pCurrentUTF16String) : 0;
}

unsigned short Label::getCharAtStringPosition(int position) const
{
    return m_pCurrentUTF16String[position];
}

unsigned short * Label::getUTF8String() const
{
    return m_pCurrentUTF16String;
}

void Label::assignNewUTF8String(unsigned short *newString)
{
    setCurrentString(newString);
}

TextHAlignment Label::getTextAlignment() const
{
    return m_eAlignment;
}

// label related stuff
float Label::getMaxLineWidth() const
{
    return m_fWidth;
}

bool Label::breakLineWithoutSpace() const
{
    return m_bLineBreakWithoutSpaces;
}

Size Label::getLabelContentSize() const
{
    return getContentSize();
}

void Label::setLabelContentSize(const Size &newSize)
{
    setContentSize(newSize);
}


// RGBA protocol


bool Label::isOpacityModifyRGB() const
{
    return m_bIsOpacityModifyRGB;
}

void Label::setOpacityModifyRGB(bool isOpacityModifyRGB)
{
    m_bIsOpacityModifyRGB = isOpacityModifyRGB;
    if (m_pChildren && m_pChildren->count() != 0)
    {
        for (auto child: *m_pChildren)
        {
            Node* pNode = static_cast<Node*>( child );
            if (pNode)
            {
                RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(pNode);
                if (pRGBAProtocol)
                {
                    pRGBAProtocol->setOpacityModifyRGB(m_bIsOpacityModifyRGB);
                }
            }
        }
    }
    m_pReusedLetter->setOpacityModifyRGB(true);
}

unsigned char Label::getOpacity() const
{
     return m_cRealOpacity;
}

unsigned char Label::getDisplayedOpacity() const
{
    return m_cDisplayedOpacity;
}

void Label::setOpacity(GLubyte opacity)
{
    m_cDisplayedOpacity = m_cRealOpacity = opacity;
    m_pReusedLetter->setOpacity(opacity);
	if( m_bCascadeOpacityEnabled ) {
		GLubyte parentOpacity = 255;
        RGBAProtocol* pParent = dynamic_cast<RGBAProtocol*>(m_pParent);
        if (pParent && pParent->isCascadeOpacityEnabled())
        {
            parentOpacity = pParent->getDisplayedOpacity();
        }
        this->updateDisplayedOpacity(parentOpacity);
	}
}
void Label::updateDisplayedOpacity(GLubyte parentOpacity)
{
    m_cDisplayedOpacity = m_cRealOpacity * parentOpacity/255.0;
    
    for (auto child: *m_pChildren)
    {
        Sprite *item = static_cast<Sprite*>( child );
		item->updateDisplayedOpacity(m_cDisplayedOpacity);
	}
    V3F_C4B_T2F_Quad *quads = m_pTextureAtlas->getQuads();
    int count = m_pTextureAtlas->getTotalQuads();
    Color4B color4( m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity );
    if (m_bIsOpacityModifyRGB)
    {
        color4.r *= m_cDisplayedOpacity/255.0f;
        color4.g *= m_cDisplayedOpacity/255.0f;
        color4.b *= m_cDisplayedOpacity/255.0f;
    }
    for (int index = 0; index < count; ++index)
    {    
        quads[index].bl.colors = color4;
        quads[index].br.colors = color4;
        quads[index].tl.colors = color4;
        quads[index].tr.colors = color4;
        m_pTextureAtlas->updateQuad(&quads[index], index);           
    }
}

bool Label::isCascadeOpacityEnabled() const
{
    return false;
}

void Label::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    m_bCascadeOpacityEnabled = cascadeOpacityEnabled;
}

const Color3B& Label::getColor(void) const
{
    return m_tRealColor;
}

const Color3B& Label::getDisplayedColor() const
{
    return m_tDisplayedColor;
}

void Label::setColor(const Color3B& color)
{
    m_tDisplayedColor = m_tRealColor = color;
	m_pReusedLetter->setColor(color);
	if( m_bCascadeColorEnabled )
    {
		Color3B parentColor = Color3B::WHITE;
        RGBAProtocol* pParent = dynamic_cast<RGBAProtocol*>(m_pParent);
        
        if (pParent && pParent->isCascadeColorEnabled())
            parentColor = pParent->getDisplayedColor();
        
        updateDisplayedColor(parentColor);
	}
}

void Label::updateDisplayedColor(const Color3B& parentColor)
{
    m_tDisplayedColor.r = m_tRealColor.r * parentColor.r/255.0;
	m_tDisplayedColor.g = m_tRealColor.g * parentColor.g/255.0;
	m_tDisplayedColor.b = m_tRealColor.b * parentColor.b/255.0;
    
	for (auto child: *m_pChildren)
    {
        Sprite *item = static_cast<Sprite*>( child );
		item->updateDisplayedColor(m_tDisplayedColor);
	}

    V3F_C4B_T2F_Quad *quads = m_pTextureAtlas->getQuads();
    int count = m_pTextureAtlas->getTotalQuads();
    Color4B color4( m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity );

    // special opacity for premultiplied textures
    if (m_bIsOpacityModifyRGB)
    {
        color4.r *= m_cDisplayedOpacity/255.0f;
        color4.g *= m_cDisplayedOpacity/255.0f;
        color4.b *= m_cDisplayedOpacity/255.0f;
    }
    for (int index=0; index<count; ++index)
    {    
        quads[index].bl.colors = color4;
        quads[index].br.colors = color4;
        quads[index].tl.colors = color4;
        quads[index].tr.colors = color4;
        m_pTextureAtlas->updateQuad(&quads[index], index);
    }  
}

bool Label::isCascadeColorEnabled() const
{
    return false;
}

void Label::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    m_bCascadeColorEnabled = cascadeColorEnabled;
}


NS_CC_END