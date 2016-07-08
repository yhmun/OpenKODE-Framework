/* -----------------------------------------------------------------------------------
 *
 *      File            CCFontFreeType.cpp
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

#include <algorithm>

#include "support/ccUTF8.h"
#include "2d/label_nodes/CCFontFreeType.h"
#include "2d/label_nodes/CCTextImage.h"
#include "2d/label_nodes/CCFont.h"
#include "2d/label_nodes/CCFontDefinition.h"
#include "platform/CCFileUtils.h"

NS_CC_BEGIN


FT_Library FontFreeType::m_pFTlibrary;
bool       FontFreeType::m_bFTInitialized = false;

FontFreeType * FontFreeType::create(const std::string &fontName, int fontSize, GlyphCollection glyphs, const char *customGlyphs)
{
    bool  dynamicGlyphCollection = false;
    if(glyphs == GlyphCollection::DYNAMIC)
        dynamicGlyphCollection = true;
    
    FontFreeType *tempFont =  new FontFreeType(dynamicGlyphCollection);

    if (!tempFont)
        return nullptr;
    
    tempFont->setCurrentGlyphCollection(glyphs, customGlyphs);
    
    if (!tempFont->createFontObject(fontName, fontSize))
    {
        delete tempFont;
        return nullptr;
    }
    return tempFont;
}

bool FontFreeType::initFreeType()
{
    if (m_bFTInitialized == false)
    {
        // begin freetype
        if (FT_Init_FreeType( &m_pFTlibrary ))
            return false;
        
        m_bFTInitialized = true;
    }
    
    return  m_bFTInitialized;
}

void FontFreeType::shutdownFreeType()
{
    if (m_bFTInitialized == true)
    {
        FT_Done_FreeType(m_pFTlibrary);
        m_bFTInitialized = false;
    }
}

FT_Library FontFreeType::getFTLibrary()
{
    initFreeType();
    return m_pFTlibrary;
}

FontFreeType::FontFreeType(bool dynamicGlyphCollection)
: m_tFontRef(nullptr),
m_nLetterPadding(5),
m_pTTFData(nullptr),
m_bDynamicGlyphCollection(dynamicGlyphCollection)
{
}

bool FontFreeType::createFontObject(const std::string &fontName, int fontSize)
{
    FT_Face face;

    KDint32 len = 0;
    m_pTTFData = FileUtils::getInstance()->getFileData(fontName.c_str(), "rb", &len);
    if (!m_pTTFData)
        return false;

    // create the face from the data
    if (FT_New_Memory_Face(getFTLibrary(), m_pTTFData, len, 0, &face ))          
        return false;

    //we want to use unicode
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
        return false;

    // set the requested font size
    int dpi = 72;
    int fontSizePoints = (int)(64.f * fontSize);
    if (FT_Set_Char_Size(face, fontSizePoints, fontSizePoints, dpi, dpi))
        return false;
    
    // store the face globally
    m_tFontRef = face;
    
    // save font name locally
    m_sFontName = fontName;
    
    // done and good
    return true;
}

FontFreeType::~FontFreeType()
{
    if (m_tFontRef)
    {
        FT_Done_Face(m_tFontRef);
    }
    if (m_pTTFData)
    {
        free(m_pTTFData);
        m_pTTFData = nullptr;
    }
}

FontAtlas * FontFreeType::createFontAtlas()
{
    if (m_bDynamicGlyphCollection)
    {
        FontAtlas *atlas = new FontAtlas(*this);
        this->release();
        return atlas;
    } 
    else
    {
        FontDefinitionTTF *def = FontDefinitionTTF::create(this);

        if (!def)
            return nullptr;

        FontAtlas *atlas = def->createFontAtlas();

        return atlas;
    }   
}

bool FontFreeType::getBBOXFotChar(unsigned short theChar, Rect &outRect) const
{
    if (!m_tFontRef)
        return false;
    
    // get the ID to the char we need
    int glyph_index = FT_Get_Char_Index(m_tFontRef, theChar);
    
    if (!glyph_index)
        return false;
    
    // load glyph infos
    if (FT_Load_Glyph(m_tFontRef, glyph_index, FT_LOAD_DEFAULT))
        return false;
    
    // store result in the passed rectangle
    outRect.origin.x    = 0;
    outRect.origin.y    = - (m_tFontRef->glyph->metrics.horiBearingY >> 6);
    outRect.size.width  =   (m_tFontRef->glyph->metrics.width  >> 6);
    outRect.size.height =   (m_tFontRef->glyph->metrics.height >> 6);
    
    return true;
}

GlyphDef * FontFreeType::getGlyphDefintionsForText(const char *text, int &outNumGlyphs, bool UTF16text) const
{
    unsigned short* utf16String = 0;
    
    if (UTF16text)
    {
        utf16String = (unsigned short*) text;
    }
    else
    {
        utf16String = cc_utf8_to_utf16(text);
    }
    
    //
    if  (!utf16String)
        return 0;
    
    int numChar = cc_wcslen(utf16String);
    if (!numChar)
        return 0;

    // allocate the needed Glyphs
    GlyphDef *glyphs = new GlyphDef[numChar];
    kdAssert(glyphs != nullptr);
    if (!glyphs)
        return 0;
    
    // sore result as CCRect
    for (int c = 0; c < numChar; ++c)
    {
        Rect tempRect;
        
        if (!getBBOXFotChar(utf16String[c], tempRect))
        {
            log("Warning: Cannot find definition for glyph: %c in font:%s", utf16String[c], m_sFontName.c_str());
            
            tempRect.origin.x       = 0;
            tempRect.origin.y       = 0;
            tempRect.size.width     = 0;
            tempRect.size.height    = 0;
            
            glyphs[c].setRect(tempRect);
            glyphs[c].setUTF16Letter(utf16String[c]);
            glyphs[c].setValid(false);
            glyphs[c].setPadding(m_nLetterPadding);
        }
        else
        {
            glyphs[c].setRect(tempRect);
            glyphs[c].setUTF16Letter(utf16String[c]);
            glyphs[c].setPadding(m_nLetterPadding);
            glyphs[c].setValid(true);
        }
    }
    
    outNumGlyphs = numChar;
    
    // free memory
    if (!UTF16text)
        delete [] utf16String;
    
    // done
    return glyphs;
}

Size * FontFreeType::getAdvancesForTextUTF16(unsigned short *text, int &outNumLetters) const
{
    if (!text)
        return 0;
    
    outNumLetters = cc_wcslen(text);

    if (!outNumLetters)
        return 0;
    
    Size *sizes = new Size[outNumLetters];
    if (!sizes)
        return 0;
    
    for (int c = 0; c < outNumLetters; ++c)
    {
        int advance = 0;
        int kerning = 0;
        
        advance = getAdvanceForChar(text[c]) - getBearingXForChar(text[c]);
        
        if (c < (outNumLetters-1))
            kerning = getHorizontalKerningForChars(text[c], text[c+1]);
        
        sizes[c].width = (advance + kerning);
    }
    
    return sizes;
}

int FontFreeType::getAdvanceForChar(unsigned short theChar) const
{
    if (!m_tFontRef)
        return 0;
    
    // get the ID to the char we need
    int glyph_index = FT_Get_Char_Index(m_tFontRef, theChar);
    
    if (!glyph_index)
        return 0;
    
    // load glyph infos
    if (FT_Load_Glyph(m_tFontRef, glyph_index, FT_LOAD_DEFAULT))
        return 0;
    
    // get to the advance for this glyph
    return (m_tFontRef->glyph->advance.x >> 6);
}

int FontFreeType::getBearingXForChar(unsigned short theChar) const
{
    
    if (!m_tFontRef)
        return 0;
    
    // get the ID to the char we need
    int glyphIndex = FT_Get_Char_Index(m_tFontRef, theChar);
    
    if (!glyphIndex)
        return 0;
    
    // load glyph infos
    if (FT_Load_Glyph(m_tFontRef, glyphIndex, FT_LOAD_DEFAULT))
        return 0;
    
    return (m_tFontRef->glyph->metrics.horiBearingX >>6);
}

int  FontFreeType::getHorizontalKerningForChars(unsigned short firstChar, unsigned short secondChar) const
{
    if (!m_tFontRef)
        return 0;

    bool hasKerning = FT_HAS_KERNING( m_tFontRef ) != 0;
    
    if (!hasKerning)
        return 0;
    
    // get the ID to the char we need
    int glyphIndex1 = FT_Get_Char_Index(m_tFontRef, firstChar);
    
    if (!glyphIndex1)
        return 0;
    
    // get the ID to the char we need
    int glyphIndex2 = FT_Get_Char_Index(m_tFontRef, secondChar);
    
    if (!glyphIndex2)
        return 0;
    
    FT_Vector kerning;
    
    if (FT_Get_Kerning( m_tFontRef, glyphIndex1, glyphIndex2,  FT_KERNING_DEFAULT,  &kerning))
        return 0;
    
    return (kerning.x >> 6);
}

int FontFreeType::getFontMaxHeight() const
{
    return (m_tFontRef->size->metrics.height >> 6);
}

unsigned char *   FontFreeType::getGlyphBitmap(unsigned short theChar, int &outWidth, int &outHeight) const
{
    if (!m_tFontRef)
        return 0;
    
    // get the ID to the char we need
    int glyphIndex = FT_Get_Char_Index(m_tFontRef, theChar);
    
    if (!glyphIndex)
        return 0;
    
    // load glyph infos
    if (FT_Load_Glyph(m_tFontRef, glyphIndex, FT_LOAD_DEFAULT))
        return 0;
    
    if (FT_Render_Glyph( m_tFontRef->glyph, FT_RENDER_MODE_NORMAL ))
        return 0;
    
    outWidth  = m_tFontRef->glyph->bitmap.width;
    outHeight = m_tFontRef->glyph->bitmap.rows;
    
    // return the pointer to the bitmap
    return m_tFontRef->glyph->bitmap.buffer;
}

int FontFreeType::getLetterPadding() const
{
    return m_nLetterPadding;
}

NS_CC_END