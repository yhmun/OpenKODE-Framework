/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelBMFont.cpp
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
 *      Use any of these editors to generate BMFonts:
 *
 *         http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
 *         http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
 *         http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
 *         http://www.angelcode.com/products/bmfont/ (Free, Windows only)
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

#include "2d/label_nodes/CCLabelBMFont.h"
#include "base/CCString.h"
#include "base/CCDictionary.h"
#include "2d/CCConfiguration.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "platform/CCFileUtils.h"
#include "2d/CCDirector.h"
#include "2d/textures/CCTextureCache.h"
#include "support/ccUTF8.h"

using namespace std;

NS_CC_BEGIN

// The return value needs to be deleted by CC_SAFE_DELETE_ARRAY.
static unsigned short* copyUTF16StringN(unsigned short* str)
{
    int length = str ? cc_wcslen(str) : 0;
    unsigned short* ret = new unsigned short[length+1];
    for (int i = 0; i < length; ++i) {
        ret[i] = str[i];
    }
    ret[length] = 0;
    return ret;
}

//
//FNTConfig Cache - free functions
//
static Dictionary* s_pConfigurations = NULL;

CCBMFontConfiguration* FNTConfigLoadFile(const std::string& fntFile)
{
    CCBMFontConfiguration* ret = NULL;

    if( s_pConfigurations == NULL )
    {
        s_pConfigurations = new Dictionary();
        s_pConfigurations->init();
    }

    ret = static_cast<CCBMFontConfiguration*>( s_pConfigurations->objectForKey(fntFile) );
    if( ret == NULL )
    {
        ret = CCBMFontConfiguration::create(fntFile.c_str());
        if (ret)
        {
            s_pConfigurations->setObject(ret, fntFile);
        }        
    }

    return ret;
}

void FNTConfigRemoveCache( void )
{
    if (s_pConfigurations)
    {
        s_pConfigurations->removeAllObjects();
        CC_SAFE_RELEASE(s_pConfigurations);
    }
}

//
//BitmapFontConfiguration
//

CCBMFontConfiguration * CCBMFontConfiguration::create(const std::string& FNTfile)
{
    CCBMFontConfiguration * ret = new CCBMFontConfiguration();
    if (ret->initWithFNTfile(FNTfile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool CCBMFontConfiguration::initWithFNTfile(const std::string& FNTfile)
{
    m_pKerningDictionary = NULL;
    m_pFontDefDictionary = NULL;
    
    m_pCharacterSet = this->parseConfigFile(FNTfile);
    
    if (! m_pCharacterSet)
    {
        return false;
    }

    return true;
}

std::set<unsigned int>* CCBMFontConfiguration::getCharacterSet() const
{
    return m_pCharacterSet;
}

CCBMFontConfiguration::CCBMFontConfiguration()
: m_pFontDefDictionary(NULL)
, m_nCommonHeight(0)
, m_pKerningDictionary(NULL)
, m_pCharacterSet(NULL)
{

}

CCBMFontConfiguration::~CCBMFontConfiguration()
{
    CCLOGINFO( "deallocing CCBMFontConfiguration: %p", this );
    this->purgeFontDefDictionary();
    this->purgeKerningDictionary();
    m_sAtlasName.clear();
    CC_SAFE_DELETE(m_pCharacterSet);
}

const char* CCBMFontConfiguration::description(void) const
{
    return String::createWithFormat(
        "<CCBMFontConfiguration = " CC_FORMAT_PRINTF_SIZE_T " | Glphys:%d Kernings:%d | Image = %s>",
        (size_t)this,
        HASH_COUNT(m_pFontDefDictionary),
        HASH_COUNT(m_pKerningDictionary),
        m_sAtlasName.c_str()
    )->getCString();
}

void CCBMFontConfiguration::purgeKerningDictionary()
{
    tKerningHashElement *current;
    while(m_pKerningDictionary) 
    {
        current = m_pKerningDictionary; 
        HASH_DEL(m_pKerningDictionary,current);
        free(current);
    }
}

void CCBMFontConfiguration::purgeFontDefDictionary()
{    
    tFontDefHashElement *current, *tmp;

    HASH_ITER(hh, m_pFontDefDictionary, current, tmp) {
        HASH_DEL(m_pFontDefDictionary, current);
        free(current);
    }
}

std::set<unsigned int>* CCBMFontConfiguration::parseConfigFile(const std::string& controlFile)
{    
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(controlFile);
    String *contents = String::createWithContentsOfFile(fullpath.c_str());

    CCASSERT(contents, "CCBMFontConfiguration::parseConfigFile | Open file error.");
    
    set<unsigned int> *validCharsString = new set<unsigned int>();

    if (!contents)
    {
        CCLOG("cocos2d: Error parsing FNTfile %s", controlFile.c_str());
        return NULL;
    }

    // parse spacing / padding
    std::string line;
    std::string strLeft = contents->getCString();
    while (strLeft.length() > 0)
    {
        size_t pos = strLeft.find('\n');

        if (pos != std::string::npos)
        {
            // the data is more than a line.get one line
            line = strLeft.substr(0, pos);
            strLeft = strLeft.substr(pos + 1);
        }
        else
        {
            // get the left data
            line = strLeft;
            strLeft.erase();
        }

        if(line.substr(0,strlen("info face")) == "info face") 
        {
            // XXX: info parsing is incomplete
            // Not needed for the Hiero editors, but needed for the AngelCode editor
            //            [self parseInfoArguments:line];
            this->parseInfoArguments(line);
        }
        // Check to see if the start of the line is something we are interested in
        else if(line.substr(0,strlen("common lineHeight")) == "common lineHeight")
        {
            this->parseCommonArguments(line);
        }
        else if(line.substr(0,strlen("page id")) == "page id")
        {
            this->parseImageFileName(line, controlFile);
        }
        else if(line.substr(0,strlen("chars c")) == "chars c")
        {
            // Ignore this line
        }
        else if(line.substr(0,strlen("char")) == "char")
        {
            // Parse the current line and create a new CharDef
            tFontDefHashElement* element = (tFontDefHashElement*)malloc( sizeof(*element) );
            this->parseCharacterDefinition(line, &element->fontDef);

            element->key = element->fontDef.charID;
            HASH_ADD_INT(m_pFontDefDictionary, key, element);
            
            validCharsString->insert(element->fontDef.charID);
        }
//        else if(line.substr(0,strlen("kernings count")) == "kernings count")
//        {
//            this->parseKerningCapacity(line);
//        }
        else if(line.substr(0,strlen("kerning first")) == "kerning first")
        {
            this->parseKerningEntry(line);
        }
    }
    
    return validCharsString;
}

void CCBMFontConfiguration::parseImageFileName(std::string line, const std::string& fntFile)
{
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // page id=0 file="bitmapFontTest.png"
    //////////////////////////////////////////////////////////////////////////

    // page ID. Sanity check
    long index = line.find('=')+1;
    long index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    CCASSERT(kdAtoi(value.c_str()) == 0, "LabelBMFont file could not be found");
    // file 
    index = line.find('"')+1;
    index2 = line.find('"', index);
    value = line.substr(index, index2-index);

    m_sAtlasName = FileUtils::getInstance()->fullPathFromRelativeFile(value.c_str(), fntFile);
}

void CCBMFontConfiguration::parseInfoArguments(std::string line)
{
    //////////////////////////////////////////////////////////////////////////
    // possible lines to parse:
    // info face="Script" size=32 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=1 padding=1,4,3,2 spacing=0,0 outline=0
    // info face="Cracked" size=36 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1
    //////////////////////////////////////////////////////////////////////////

    // padding
    long index = line.find("padding=");
    long index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    sscanf(value.c_str(), "padding=%d,%d,%d,%d", &m_tPadding.top, &m_tPadding.right, &m_tPadding.bottom, &m_tPadding.left);
    CCLOG("cocos2d: padding: %d,%d,%d,%d", m_tPadding.left, m_tPadding.top, m_tPadding.right, m_tPadding.bottom);
}

void CCBMFontConfiguration::parseCommonArguments(std::string line)
{
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // common lineHeight=104 base=26 scaleW=1024 scaleH=512 pages=1 packed=0
    //////////////////////////////////////////////////////////////////////////

    // Height
    long index = line.find("lineHeight=");
    long index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    sscanf(value.c_str(), "lineHeight=%d", &m_nCommonHeight);
    // scaleW. sanity check
    index = line.find("scaleW=") + strlen("scaleW=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    CCASSERT(kdAtoi(value.c_str()) <= Configuration::getInstance()->getMaxTextureSize(), "CCLabelBMFont: page can't be larger than supported");
    // scaleH. sanity check
    index = line.find("scaleH=") + strlen("scaleH=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    CCASSERT(kdAtoi(value.c_str()) <= Configuration::getInstance()->getMaxTextureSize(), "CCLabelBMFont: page can't be larger than supported");
    // pages. sanity check
    index = line.find("pages=") + strlen("pages=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    CCASSERT(kdAtoi(value.c_str()) == 1, "CCBitfontAtlas: only supports 1 page");

    // packed (ignore) What does this mean ??
}

void CCBMFontConfiguration::parseCharacterDefinition(std::string line, ccBMFontDef *characterDefinition)
{    
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // char id=32   x=0     y=0     width=0     height=0     xoffset=0     yoffset=44    xadvance=14     page=0  chnl=0 
    //////////////////////////////////////////////////////////////////////////

    // Character ID
    long index = line.find("id=");
    long index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    sscanf(value.c_str(), "id=%u", &characterDefinition->charID);

    // Character x
    index = line.find("x=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "x=%f", &characterDefinition->rect.origin.x);
    // Character y
    index = line.find("y=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "y=%f", &characterDefinition->rect.origin.y);
    // Character width
    index = line.find("width=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "width=%f", &characterDefinition->rect.size.width);
    // Character height
    index = line.find("height=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "height=%f", &characterDefinition->rect.size.height);
    // Character xoffset
    index = line.find("xoffset=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "xoffset=%hd", &characterDefinition->xOffset);
    // Character yoffset
    index = line.find("yoffset=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "yoffset=%hd", &characterDefinition->yOffset);
    // Character xadvance
    index = line.find("xadvance=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "xadvance=%hd", &characterDefinition->xAdvance);
}

void CCBMFontConfiguration::parseKerningEntry(std::string line)
{        
    //////////////////////////////////////////////////////////////////////////
    // line to parse:
    // kerning first=121  second=44  amount=-7
    //////////////////////////////////////////////////////////////////////////

    // first
    int first;
    long index = line.find("first=");
    long index2 = line.find(' ', index);
    std::string value = line.substr(index, index2-index);
    sscanf(value.c_str(), "first=%d", &first);

    // second
    int second;
    index = line.find("second=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "second=%d", &second);

    // amount
    int amount;
    index = line.find("amount=");
    index2 = line.find(' ', index);
    value = line.substr(index, index2-index);
    sscanf(value.c_str(), "amount=%d", &amount);

    tKerningHashElement *element = (tKerningHashElement *)calloc( sizeof( *element ), 1 );
    element->amount = amount;
    element->key = (first<<16) | (second&0xffff);
    HASH_ADD_INT(m_pKerningDictionary,key, element);
}
//
//CCLabelBMFont
//

//LabelBMFont - Purge Cache
void LabelBMFont::purgeCachedData()
{
    FNTConfigRemoveCache();
}

LabelBMFont * LabelBMFont::create()
{
    LabelBMFont * pRet = new LabelBMFont();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

LabelBMFont * LabelBMFont::create(const std::string& str, const std::string& fntFile, float width, TextHAlignment alignment)
{
    return LabelBMFont::create(str, fntFile, width, alignment, Point::ZERO);
}

LabelBMFont * LabelBMFont::create(const std::string& str, const std::string& fntFile, float width)
{
    return LabelBMFont::create(str, fntFile, width, TextHAlignment::LEFT, Point::ZERO);
}

LabelBMFont * LabelBMFont::create(const std::string& str, const std::string& fntFile)
{
    return LabelBMFont::create(str, fntFile, kLabelAutomaticWidth, TextHAlignment::LEFT, Point::ZERO);
}

//LabelBMFont - Creation & Init
LabelBMFont *LabelBMFont::create(const std::string& str, const std::string& fntFile, float width/* = kLabelAutomaticWidth*/, TextHAlignment alignment/* = TextHAlignment::LEFT*/, Point imageOffset/* = Point::ZERO*/)
{
    LabelBMFont *pRet = new LabelBMFont();
    if(pRet && pRet->initWithString(str, fntFile, width, alignment, imageOffset))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool LabelBMFont::init()
{
    return initWithString("", "", kLabelAutomaticWidth, TextHAlignment::LEFT, Point::ZERO);
}

bool LabelBMFont::initWithString(const std::string& theString, const std::string& fntFile, float width/* = kLabelAutomaticWidth*/, TextHAlignment alignment/* = TextHAlignment::LEFT*/, Point imageOffset/* = Point::ZERO*/)
{
    CCASSERT(!m_pConfiguration, "re-init is no longer supported");

    Texture2D *texture = NULL;
    
    if (fntFile.size() > 0 )
    {
        CCBMFontConfiguration *newConf = FNTConfigLoadFile(fntFile);
        if (!newConf)
        {
            CCLOG("cocos2d: WARNING. LabelBMFont: Impossible to create font. Please check file: '%s'", fntFile.c_str());
            release();
            return false;
        }
        
        newConf->retain();
        CC_SAFE_RELEASE(m_pConfiguration);
        m_pConfiguration = newConf;
        
        m_sFntFile = fntFile;
        
        texture = Director::getInstance()->getTextureCache()->addImage(m_pConfiguration->getAtlasName());
    }
    else 
    {
        texture = new Texture2D();
        texture->autorelease();
    }

    if (SpriteBatchNode::initWithTexture(texture, theString.size()))
    {
        m_fWidth = width;
        m_eAlignment = alignment;
        
        m_cDisplayedOpacity = m_cRealOpacity = 255;
		m_tDisplayedColor = m_tRealColor = Color3B::WHITE;
        m_bCascadeOpacityEnabled = true;
        m_bCascadeColorEnabled = true;
        
        m_tContentSize = Size::ZERO;
        
        m_bIsOpacityModifyRGB = m_pTextureAtlas->getTexture()->hasPremultipliedAlpha();
        m_tAnchorPoint = Point(0.5f, 0.5f);
        
        m_tImageOffset = imageOffset;
        
        m_pReusedChar = new Sprite();
        m_pReusedChar->initWithTexture(m_pTextureAtlas->getTexture(), Rect(0, 0, 0, 0), false);
        m_pReusedChar->setBatchNode(this);
        
        this->setString(theString, true);
        
        return true;
    }
    return false;
}

LabelBMFont::LabelBMFont()
: m_pString(NULL)
, m_pInitialString(NULL)
, m_eAlignment(TextHAlignment::CENTER)
, m_fWidth(-1.0f)
, m_pConfiguration(NULL)
, m_bLineBreakWithoutSpaces(false)
, m_tImageOffset(Point::ZERO)
, m_pReusedChar(NULL)
, m_cDisplayedOpacity(255)
, m_cRealOpacity(255)
, m_tDisplayedColor(Color3B::WHITE)
, m_tRealColor(Color3B::WHITE)
, m_bCascadeColorEnabled(true)
, m_bCascadeOpacityEnabled(true)
, m_bIsOpacityModifyRGB(false)
{

}

LabelBMFont::~LabelBMFont()
{
    CC_SAFE_RELEASE(m_pReusedChar);
    CC_SAFE_DELETE_ARRAY(m_pString);
    CC_SAFE_DELETE_ARRAY(m_pInitialString);
    CC_SAFE_RELEASE(m_pConfiguration);
}

// LabelBMFont - Atlas generation
int LabelBMFont::kerningAmountForFirst(unsigned short first, unsigned short second)
{
    int ret = 0;
    unsigned int key = (first<<16) | (second & 0xffff);

    if( m_pConfiguration->m_pKerningDictionary ) {
        tKerningHashElement *element = NULL;
        HASH_FIND_INT(m_pConfiguration->m_pKerningDictionary, &key, element);        
        if(element)
            ret = element->amount;
    }
    return ret;
}

void LabelBMFont::createFontChars()
{
    int nextFontPositionX = 0;
    int nextFontPositionY = 0;
    unsigned short prev = -1;
    int kerningAmount = 0;

    Size tmpSize = Size::ZERO;

    int longestLine = 0;
    unsigned int totalHeight = 0;

    unsigned int quantityOfLines = 1;
    unsigned int stringLen = m_pString ? cc_wcslen(m_pString) : 0;
    if (stringLen == 0)
    {
        return;
    }

    set<unsigned int> *charSet = m_pConfiguration->getCharacterSet();

    for (unsigned int i = 0; i < stringLen - 1; ++i)
    {
        unsigned short c = m_pString[i];
        if (c == '\n')
        {
            quantityOfLines++;
        }
    }

    totalHeight = m_pConfiguration->m_nCommonHeight * quantityOfLines;
    nextFontPositionY = 0-(m_pConfiguration->m_nCommonHeight - m_pConfiguration->m_nCommonHeight * quantityOfLines);
    
    Rect rect;
    ccBMFontDef fontDef;

    for (unsigned int i= 0; i < stringLen; i++)
    {
        unsigned short c = m_pString[i];

        if (c == '\n')
        {
            nextFontPositionX = 0;
            nextFontPositionY -= m_pConfiguration->m_nCommonHeight;
            continue;
        }
        
        if (charSet->find(c) == charSet->end())
        {
            CCLOGWARN("cocos2d::LabelBMFont: Attempted to use character not defined in this bitmap: %d", c);
            continue;      
        }

        kerningAmount = this->kerningAmountForFirst(prev, c);
        
        tFontDefHashElement *element = NULL;

        // unichar is a short, and an int is needed on HASH_FIND_INT
        unsigned int key = c;
        HASH_FIND_INT(m_pConfiguration->m_pFontDefDictionary, &key, element);
        if (! element)
        {
            CCLOGWARN("cocos2d::LabelBMFont: characer not found %d", c);
            continue;
        }

        fontDef = element->fontDef;

        rect = fontDef.rect;
        rect = CC_RECT_PIXELS_TO_POINTS(rect);

        rect.origin.x += m_tImageOffset.x;
        rect.origin.y += m_tImageOffset.y;

        Sprite *fontChar;

        bool hasSprite = true;
        fontChar = static_cast<Sprite*>( this->getChildByTag(i) );
        if(fontChar )
        {
            // Reusing previous Sprite
			fontChar->setVisible(true);
        }
        else
        {
            // New Sprite ? Set correct color, opacity, etc...
            if( 0 )
            {
				/* WIP: Doesn't support many features yet.
				 But this code is super fast. It doesn't create any sprite.
				 Ideal for big labels.
				 */
				fontChar = m_pReusedChar;
				fontChar->setBatchNode(NULL);
				hasSprite = false;
			}
            else
            {
                fontChar = new Sprite();
                fontChar->initWithTexture(m_pTextureAtlas->getTexture(), rect);
                addChild(fontChar, i, i);
                fontChar->release();
			}
            
            // Apply label properties
			fontChar->setOpacityModifyRGB(m_bIsOpacityModifyRGB);
            
			// Color MUST be set before opacity, since opacity might change color if OpacityModifyRGB is on
			fontChar->updateDisplayedColor(m_tDisplayedColor);
			fontChar->updateDisplayedOpacity(m_cDisplayedOpacity);
        }

        // updating previous sprite
        fontChar->setTextureRect(rect, false, rect.size);

        // See issue 1343. cast( signed short + unsigned integer ) == unsigned integer (sign is lost!)
        int yOffset = m_pConfiguration->m_nCommonHeight - fontDef.yOffset;
        Point fontPos = Point( (float)nextFontPositionX + fontDef.xOffset + fontDef.rect.size.width*0.5f + kerningAmount,
            (float)nextFontPositionY + yOffset - rect.size.height*0.5f * CC_CONTENT_SCALE_FACTOR() );
        fontChar->setPosition(CC_POINT_PIXELS_TO_POINTS(fontPos));

        // update kerning
        nextFontPositionX += fontDef.xAdvance + kerningAmount;
        prev = c;

        if (longestLine < nextFontPositionX)
        {
            longestLine = nextFontPositionX;
        }
        
        if (! hasSprite)
        {
            updateQuadFromSprite(fontChar, i);
        }
    }

    // If the last character processed has an xAdvance which is less that the width of the characters image, then we need
    // to adjust the width of the string to take this into account, or the character will overlap the end of the bounding
    // box
    if (fontDef.xAdvance < fontDef.rect.size.width)
    {
        tmpSize.width = longestLine + fontDef.rect.size.width - fontDef.xAdvance;
    }
    else
    {
        tmpSize.width = longestLine;
    }
    tmpSize.height = totalHeight;

    this->setContentSize(CC_SIZE_PIXELS_TO_POINTS(tmpSize));
}

//LabelBMFont - LabelProtocol protocol
void LabelBMFont::setString(const std::string &newString)
{
    this->setString(newString, true);
}

void LabelBMFont::setString(const std::string &newString, bool needUpdateLabel)
{
    if (needUpdateLabel) {
        m_sInitialStringUTF8 = newString;
    }
    unsigned short* utf16String = cc_utf8_to_utf16(newString.c_str());
    setString(utf16String, needUpdateLabel);
    CC_SAFE_DELETE_ARRAY(utf16String);
 }

void LabelBMFont::setString(unsigned short *newString, bool needUpdateLabel)
{
    if (!needUpdateLabel)
    {
        unsigned short* tmp = m_pString;
        m_pString = copyUTF16StringN(newString);
        CC_SAFE_DELETE_ARRAY(tmp);
    }
    else
    {
        unsigned short* tmp = m_pInitialString;
        m_pInitialString = copyUTF16StringN(newString);
        CC_SAFE_DELETE_ARRAY(tmp);
    }
    
    if (m_pChildren && m_pChildren->count() != 0)
    {
        Object* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            Node* pNode = static_cast<Node*>( child );
            if (pNode)
            {
                pNode->setVisible(false);
            }
        }
    }
    this->createFontChars();
    
    if (needUpdateLabel) {
        updateLabel();
    }
}

const std::string& LabelBMFont::getString() const
{
    return m_sInitialStringUTF8;
}

void LabelBMFont::setCString(const char *label)
{
    setString(label);
}

//LabelBMFont - RGBAProtocol protocol
const Color3B& LabelBMFont::getColor() const
{
    return m_tRealColor;
}

const Color3B& LabelBMFont::getDisplayedColor() const
{
    return m_tDisplayedColor;
}

void LabelBMFont::setColor(const Color3B& color)
{
	m_tDisplayedColor = m_tRealColor = color;
	
	if( m_bCascadeColorEnabled ) {
		Color3B parentColor = Color3B::WHITE;
        RGBAProtocol* pParent = dynamic_cast<RGBAProtocol*>(m_pParent);
        if (pParent && pParent->isCascadeColorEnabled())
        {
            parentColor = pParent->getDisplayedColor();
        }
        this->updateDisplayedColor(parentColor);
	}
}

GLubyte LabelBMFont::getOpacity(void) const
{
    return m_cRealOpacity;
}

GLubyte LabelBMFont::getDisplayedOpacity(void) const
{
    return m_cDisplayedOpacity;
}

/** Override synthesized setOpacity to recurse items */
void LabelBMFont::setOpacity(GLubyte opacity)
{
	m_cDisplayedOpacity = m_cRealOpacity = opacity;
    
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

void LabelBMFont::setOpacityModifyRGB(bool var)
{
    m_bIsOpacityModifyRGB = var;
    if (m_pChildren && m_pChildren->count() != 0)
    {
        Object* child;
        CCARRAY_FOREACH(m_pChildren, child)
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
}
bool LabelBMFont::isOpacityModifyRGB() const
{
    return m_bIsOpacityModifyRGB;
}

void LabelBMFont::updateDisplayedOpacity(GLubyte parentOpacity)
{
	m_cDisplayedOpacity = m_cRealOpacity * parentOpacity/255.0;
    
	Object* pObj;
	CCARRAY_FOREACH(m_pChildren, pObj)
    {
        Sprite *item = static_cast<Sprite*>( pObj );
		item->updateDisplayedOpacity(m_cDisplayedOpacity);
	}
}

void LabelBMFont::updateDisplayedColor(const Color3B& parentColor)
{
	m_tDisplayedColor.r = m_tRealColor.r * parentColor.r/255.0;
	m_tDisplayedColor.g = m_tRealColor.g * parentColor.g/255.0;
	m_tDisplayedColor.b = m_tRealColor.b * parentColor.b/255.0;
    
    Object* pObj;
	CCARRAY_FOREACH(m_pChildren, pObj)
    {
        Sprite *item = static_cast<Sprite*>( pObj );
		item->updateDisplayedColor(m_tDisplayedColor);
	}
}

bool LabelBMFont::isCascadeColorEnabled() const
{
    return false;
}

void LabelBMFont::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    m_bCascadeColorEnabled = cascadeColorEnabled;
}

bool LabelBMFont::isCascadeOpacityEnabled() const
{
    return false;
}

void LabelBMFont::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    m_bCascadeOpacityEnabled = cascadeOpacityEnabled;
}

// LabelBMFont - AnchorPoint
void LabelBMFont::setAnchorPoint(const Point& point)
{
    if( ! point.equals(m_tAnchorPoint))
    {
        SpriteBatchNode::setAnchorPoint(point);
        updateLabel();
    }
}

// LabelBMFont - Alignment
void LabelBMFont::updateLabel()
{
    this->setString(m_pInitialString, false);

    if (m_fWidth > 0)
    {
        // Step 1: Make multiline
        vector<unsigned short> str_whole = cc_utf16_vec_from_utf16_str(m_pString);
        unsigned int stringLength = str_whole.size();
        vector<unsigned short> multiline_string;
        multiline_string.reserve( stringLength );
        vector<unsigned short> last_word;
        last_word.reserve( stringLength );

        unsigned int line = 1, i = 0;
        bool start_line = false, start_word = false;
        float startOfLine = -1, startOfWord = -1;
        int skip = 0;

        Array* children = getChildren();
        for (int j = 0; j < children->count(); j++)
        {
            Sprite* characterSprite;
            unsigned int justSkipped = 0;
            
            while (!(characterSprite = static_cast<Sprite*>( this->getChildByTag(j + skip + justSkipped))) )
            {
                justSkipped++;
            }
            
            skip += justSkipped;
            
            if (!characterSprite->isVisible())
                continue;

            if (i >= stringLength)
                break;

            unsigned short character = str_whole[i];

            if (!start_word)
            {
                startOfWord = getLetterPosXLeft( characterSprite );
                start_word = true;
            }
            if (!start_line)
            {
                startOfLine = startOfWord;
                start_line = true;
            }

            // Newline.
            if (character == '\n')
            {
                cc_utf8_trim_ws(&last_word);

                last_word.push_back('\n');
                multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
                last_word.clear();
                start_word = false;
                start_line = false;
                startOfWord = -1;
                startOfLine = -1;
                i+=justSkipped;
                line++;

                if (i >= stringLength)
                    break;

                character = str_whole[i];

                if (!startOfWord)
                {
                    startOfWord = getLetterPosXLeft( characterSprite );
                    start_word = true;
                }
                if (!startOfLine)
                {
                    startOfLine  = startOfWord;
                    start_line = true;
                }
            }

            // Whitespace.
            if (isspace_unicode(character))
            {
                last_word.push_back(character);
                multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
                last_word.clear();
                start_word = false;
                startOfWord = -1;
                i++;
                continue;
            }

            // Out of bounds.
            if ( getLetterPosXRight( characterSprite ) - startOfLine > m_fWidth )
            {
                if (!m_bLineBreakWithoutSpaces)
                {
                    last_word.push_back(character);

                    int found = cc_utf8_find_last_not_char(multiline_string, ' ');
                    if (found != -1)
                        cc_utf8_trim_ws(&multiline_string);
                    else
                        multiline_string.clear();

                    if (multiline_string.size() > 0)
                        multiline_string.push_back('\n');

                    line++;
                    start_line = false;
                    startOfLine = -1;
                    i++;
                }
                else
                {
                    cc_utf8_trim_ws(&last_word);

                    last_word.push_back('\n');
                    multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
                    last_word.clear();
                    start_word = false;
                    start_line = false;
                    startOfWord = -1;
                    startOfLine = -1;
                    line++;

                    if (i >= stringLength)
                        break;

                    if (!startOfWord)
                    {
                        startOfWord = getLetterPosXLeft( characterSprite );
                        start_word = true;
                    }
                    if (!startOfLine)
                    {
                        startOfLine  = startOfWord;
                        start_line = true;
                    }

                    j--;
                }

                continue;
            }
            else
            {
                // Character is normal.
                last_word.push_back(character);
                i++;
                continue;
            }
        }

        multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());

        int size = multiline_string.size();
        unsigned short* str_new = new unsigned short[size + 1];

        for (int j = 0; j < size; ++j)
        {
            str_new[j] = multiline_string[j];
        }

        str_new[size] = '\0';

        this->setString(str_new, false);
        
        CC_SAFE_DELETE_ARRAY(str_new);
    }

    // Step 2: Make alignment
    if (m_eAlignment != TextHAlignment::LEFT)
    {
        int i = 0;

        int lineNumber = 0;
        int str_len = cc_wcslen(m_pString);
        vector<unsigned short> last_line;
        for (int ctr = 0; ctr <= str_len; ++ctr)
        {
            if (m_pString[ctr] == '\n' || m_pString[ctr] == 0)
            {
                float lineWidth = 0.0f;
                unsigned int line_length = last_line.size();
				// if last line is empty we must just increase lineNumber and work with next line
                if (line_length == 0)
                {
                    lineNumber++;
                    continue;
                }
                int index = i + line_length - 1 + lineNumber;
                if (index < 0) continue;

                Sprite* lastChar = static_cast<Sprite*>( getChildByTag(index) );
                if ( lastChar == NULL )
                    continue;

                lineWidth = lastChar->getPosition().x + lastChar->getContentSize().width/2.0f;

                float shift = 0;
                switch (m_eAlignment)
                {
                case TextHAlignment::CENTER:
                    shift = getContentSize().width/2.0f - lineWidth/2.0f;
                    break;
                case TextHAlignment::RIGHT:
                    shift = getContentSize().width - lineWidth;
                    break;
                default:
                    break;
                }

                if (shift != 0)
                {
                    for (unsigned j = 0; j < line_length; j++)
                    {
                        index = i + j + lineNumber;
                        if (index < 0) continue;

                        Sprite* characterSprite = static_cast<Sprite*>( getChildByTag(index) );
                        characterSprite->setPosition(characterSprite->getPosition() + Point(shift, 0.0f));
                    }
                }

                i += line_length;
                lineNumber++;

                last_line.clear();
                continue;
            }

            last_line.push_back(m_pString[ctr]);
        }
    }
}

// LabelBMFont - Alignment
void LabelBMFont::setAlignment(TextHAlignment alignment)
{
    this->m_eAlignment = alignment;
    updateLabel();
}

void LabelBMFont::setWidth(float width)
{
    this->m_fWidth = width;
    updateLabel();
}

void LabelBMFont::setLineBreakWithoutSpace( bool breakWithoutSpace )
{
    m_bLineBreakWithoutSpaces = breakWithoutSpace;
    updateLabel();
}

void LabelBMFont::setScale(float scale)
{
    SpriteBatchNode::setScale(scale);
    updateLabel();
}

void LabelBMFont::setScaleX(float scaleX)
{
    SpriteBatchNode::setScaleX(scaleX);
    updateLabel();
}

void LabelBMFont::setScaleY(float scaleY)
{
    SpriteBatchNode::setScaleY(scaleY);
    updateLabel();
}

float LabelBMFont::getLetterPosXLeft( Sprite* sp )
{
    return sp->getPosition().x * m_fScaleX - (sp->getContentSize().width * m_fScaleX * sp->getAnchorPoint().x);
}

float LabelBMFont::getLetterPosXRight( Sprite* sp )
{
    return sp->getPosition().x * m_fScaleX + (sp->getContentSize().width * m_fScaleX * sp->getAnchorPoint().x);
}

// LabelBMFont - FntFile
void LabelBMFont::setFntFile(const char* fntFile)
{
    if (fntFile != NULL && strcmp(fntFile, m_sFntFile.c_str()) != 0 )
    {
        CCBMFontConfiguration *newConf = FNTConfigLoadFile(fntFile);

        CCASSERT( newConf, "CCLabelBMFont: Impossible to create font. Please check file");

        m_sFntFile = fntFile;

        CC_SAFE_RETAIN(newConf);
        CC_SAFE_RELEASE(m_pConfiguration);
        m_pConfiguration = newConf;

        this->setTexture(Director::getInstance()->getTextureCache()->addImage(m_pConfiguration->getAtlasName()));
        this->createFontChars();
    }
}

const char* LabelBMFont::getFntFile()
{
    return m_sFntFile.c_str();
}


//LabelBMFont - Debug draw
#if CC_LABELBMFONT_DEBUG_DRAW
void LabelBMFont::draw()
{
    SpriteBatchNode::draw();
    const Size& s = this->getContentSize();
    Point vertices[4]={
        Point(0,0),Point(s.width,0),
        Point(s.width,s.height),Point(0,s.height),
    };
    ccDrawPoly(vertices, 4, true);
}

#endif // CC_LABELBMFONT_DEBUG_DRAW

NS_CC_END
