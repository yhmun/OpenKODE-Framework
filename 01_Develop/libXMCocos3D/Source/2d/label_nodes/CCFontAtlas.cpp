/* -----------------------------------------------------------------------------------
 *
 *      File            CCFontAtlas.cpp
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

#include "2d/label_nodes/CCFontAtlas.h"
#include "2d/label_nodes/CCFont.h"
#include "2d/label_nodes/CCFontFreeType.h"
#include "support/ccUTF8.h"
#include "2d/CCDirector.h"

NS_CC_BEGIN

FontAtlas::FontAtlas(Font &theFont) : 
m_pFont(&theFont),
m_pCurrentPageData(nullptr)
{
    m_pFont->retain();
   
    FontFreeType* fontTTf = dynamic_cast<FontFreeType*>(m_pFont);
    if (fontTTf && fontTTf->isDynamicGlyphCollection())
    {
        m_fCurrentPageLineHeight = m_pFont->getFontMaxHeight();
        m_fCommonLineHeight = m_fCurrentPageLineHeight * 0.8f;
        Texture2D * tex = new Texture2D;
        m_nCurrentPage = 0;
        m_fCurrentPageOrigX = 0;
        m_fCurrentPageOrigY = 0;
        m_fLetterPadding = 5;
        m_nCurrentPageDataSize = (1024 * 1024 * 4);

        m_pCurrentPageData = new unsigned char[m_nCurrentPageDataSize];       
        kdMemset(m_pCurrentPageData, 0, m_nCurrentPageDataSize);  
        addTexture(*tex,0);
        tex->release();
    }
}

FontAtlas::~FontAtlas()
{
    m_pFont->release();
    relaseTextures();

    delete []m_pCurrentPageData;
}

void FontAtlas::relaseTextures()
{
    for( auto &item: m_aAtlasTextures)
    {
        item.second->release();
    }
}

void FontAtlas::addLetterDefinition(const FontLetterDefinition &letterDefinition)
{
    m_aFontLetterDefinitions[letterDefinition.letteCharUTF16] = letterDefinition;
}

bool FontAtlas::getLetterDefinitionForChar(unsigned short  letteCharUTF16, FontLetterDefinition &outDefinition)
{
    auto outIterator = m_aFontLetterDefinitions.find(letteCharUTF16);
    
    if (outIterator != m_aFontLetterDefinitions.end())
    {
        outDefinition = (*outIterator).second;
        return true;
    }
    else
    {
        return false;
    }
}

bool FontAtlas::prepareLetterDefinitions(unsigned short *utf16String)
{
    if(m_pCurrentPageData == nullptr)
        return false;

    FontFreeType* fontTTf = (FontFreeType*)m_pFont;

    std::vector<FontLetterDefinition> fontDefs;
    int length = cc_wcslen(utf16String);

    //find out new letter
    for (int i = 0; i < length; ++i)
    {
        auto outIterator = m_aFontLetterDefinitions.find(utf16String[i]);

        if (outIterator == m_aFontLetterDefinitions.end())
        {         
            Rect tempRect;           

            FontLetterDefinition tempDef;
            tempDef.offsetX = 0;
            tempDef.anchorX = 0.0f;
            tempDef.anchorY = 1.0f;

            if (!fontTTf->getBBOXFotChar(utf16String[i], tempRect))
            {
                tempDef.validDefinition = false;
                tempDef.letteCharUTF16   = utf16String[i];
                tempDef.commonLineHeight = 0;
                tempDef.width            = 0;
                tempDef.height           = 0;
                tempDef.U                = 0;
                tempDef.V                = 0;            
                tempDef.offsetY          = 0;
                tempDef.textureID        = 0;
            }
            else
            {
                tempDef.validDefinition = true;
                tempDef.letteCharUTF16   = utf16String[i];
                tempDef.width            = tempRect.size.width  + m_fLetterPadding;
                tempDef.height           = m_fCurrentPageLineHeight - 1;             
                tempDef.offsetY          = tempRect.origin.y;
                tempDef.commonLineHeight = m_fCurrentPageLineHeight;
                        
            }            
            fontDefs.push_back(tempDef);
        }       
    }

    float scaleFactor = CC_CONTENT_SCALE_FACTOR();
    int newLetterCount = fontDefs.size();
    float glyphWidth;
    for (int i = 0; i < newLetterCount; ++i)
    {
        if (fontDefs[i].validDefinition)
        {
            m_fCurrentPageOrigX += m_fLetterPadding;
            glyphWidth = fontDefs[i].width - m_fLetterPadding;

            if (m_fCurrentPageOrigX + glyphWidth > 1024)
            {
                m_fCurrentPageOrigY += m_fCurrentPageLineHeight;
                if(m_fCurrentPageOrigY >= 1024)
                {             
                    m_aAtlasTextures[m_nCurrentPage]->initWithData(m_pCurrentPageData, m_nCurrentPageDataSize, Texture2D::PixelFormat::RGBA8888, 1024, 1024, Size(1024, 1024) );
                    m_fCurrentPageOrigX = 0;
                    m_fCurrentPageOrigY = 0;

                    delete []m_pCurrentPageData;
                    m_pCurrentPageData = new unsigned char[m_nCurrentPageDataSize];
                    if(m_pCurrentPageData == nullptr)
                        return false;
                    kdMemset(m_pCurrentPageData, 0, m_nCurrentPageDataSize);
                    m_nCurrentPage++;
                    Texture2D* tex = new Texture2D;
                    addTexture(*tex,m_nCurrentPage);
                    tex->release();
                }
            }
            renderCharAt(fontDefs[i].letteCharUTF16,m_fCurrentPageOrigX,m_fCurrentPageOrigY,m_pCurrentPageData,1024);

            fontDefs[i].U                = m_fCurrentPageOrigX - 1;
            fontDefs[i].V                = m_fCurrentPageOrigY;
            fontDefs[i].textureID        = m_nCurrentPage;
            // take from pixels to points
            fontDefs[i].width  =    fontDefs[i].width  / scaleFactor;
            fontDefs[i].height =    fontDefs[i].height / scaleFactor;      
            fontDefs[i].U      =    fontDefs[i].U      / scaleFactor;
            fontDefs[i].V      =    fontDefs[i].V      / scaleFactor;
        }
        else
            glyphWidth = 0;       
       
        this->addLetterDefinition(fontDefs[i]);
        m_fCurrentPageOrigX += glyphWidth;
    }
    if(newLetterCount > 0)
        m_aAtlasTextures[m_nCurrentPage]->initWithData(m_pCurrentPageData, m_nCurrentPageDataSize, Texture2D::PixelFormat::RGBA8888, 1024, 1024, Size(1024, 1024) );
    return true;
}

bool FontAtlas::renderCharAt(unsigned short int charToRender, int posX, int posY, unsigned char *destMemory, int destSize)
{
    unsigned char *sourceBitmap = 0;
    int sourceWidth  = 0;
    int sourceHeight = 0;

    // get the glyph's bitmap
    sourceBitmap = m_pFont->getGlyphBitmap(charToRender, sourceWidth, sourceHeight);

    if (!sourceBitmap)
        return false;

    int iX = posX;
    int iY = posY;

    for (int y = 0; y < sourceHeight; ++y)
    {
        int bitmap_y = y * sourceWidth;

        for (int x = 0; x < sourceWidth; ++x)
        {
            unsigned char cTemp = sourceBitmap[bitmap_y + x];

            // the final pixel
            int iTemp = cTemp << 24 | cTemp << 16 | cTemp << 8 | cTemp;
            *(int*) &destMemory[(iX + ( iY * destSize ) ) * 4] = iTemp;

            iX += 1;
        }

        iX  = posX;
        iY += 1;
    }

    //everything good
    return true;
}

void FontAtlas::addTexture(Texture2D &texture, int slot)
{
    texture.retain();
    m_aAtlasTextures[slot] = &texture;
}

Texture2D & FontAtlas::getTexture(int slot)
{
    return *(m_aAtlasTextures[slot]);
}

float FontAtlas::getCommonLineHeight() const
{
    return m_fCommonLineHeight;
}

void  FontAtlas::setCommonLineHeight(float newHeight)
{
    m_fCommonLineHeight = newHeight;
}

const Font * FontAtlas::getFont() const
{
    return m_pFont;
}

NS_CC_END