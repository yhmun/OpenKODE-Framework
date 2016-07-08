/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextImage.cpp
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

#include <vector>
#include <string>

#include "2d/label_nodes/CCTextImage.h"
#include "2d/label_nodes/CCFontFreeType.h"
#include "2d/label_nodes/CCFont.h"
#include "support/ccUTF8.h"

NS_CC_BEGIN


TextLineDef::TextLineDef(float x, float y, float width, float height) :m_fX(x), m_fY(y), m_fWidth(width), m_fHeight(height)
{
}

TextPageDef::TextPageDef(int pageNum, int width, int height):   m_nPageNum(pageNum),
                                                                m_nWidth(width),
                                                                m_nHeight(height),
                                                                m_pPageData(0),
                                                                m_pPageTexture(0)
{
}

TextPageDef::~TextPageDef()
{
    int numLines = m_aLines.size();
    
    for( int c = 0; c<numLines; ++c )
    {
        delete m_aLines[c];
    }
    
    m_aLines.clear();
    
    if (m_pPageData)
    {
        delete [] m_pPageData;
    }
    
    if (m_pPageTexture)
    {
        m_pPageTexture->release();
    }
}

bool TextPageDef::generatePageTexture(bool releasePageData)
{
    if (!m_pPageData)
        return false;
    
    if (m_pPageTexture)
    {
        m_pPageTexture->release();
        m_pPageTexture = 0;
    }
    
    Size imageSize = Size((float)(m_nWidth), (float)(m_nHeight));
    if((imageSize.width <= 0) || (imageSize.height <= 0))
        return false;
    
    m_pPageTexture = new Texture2D();
    if (!m_pPageTexture)
        return false;
    
    int  dataLenght     = (m_nWidth * m_nHeight * 4);
    bool textureCreated = m_pPageTexture->initWithData(m_pPageData, dataLenght, Texture2D::PixelFormat::RGBA8888, m_nWidth, m_nHeight, imageSize);
    
    // release the page data if requested
    if (releasePageData && textureCreated)
    {
        delete [] m_pPageData;
        m_pPageData = 0;
    }
    
    return textureCreated;
}

void TextPageDef::preparePageTexture(bool releaseRAWData)
{
    generatePageTexture(releaseRAWData);
}

Texture2D *TextPageDef::getPageTexture()
{
    if (!m_pPageTexture)
    {
        generatePageTexture();
    }
    
    return m_pPageTexture;
}

TextFontPagesDef::TextFontPagesDef()
{
}

TextFontPagesDef::~TextFontPagesDef()
{
    int numPages = m_aPages.size();
    for( int c = 0; c < numPages; ++c )
    {
        if (m_aPages[c])
            delete m_aPages[c];
    }
}

TextImage::TextImage(): m_pFontPages(0), m_pFont(0)
{
}

TextImage::~TextImage()
{
    if (m_pFontPages)
        delete m_pFontPages;
    
    if (m_pFont)
        m_pFont->release();
}

bool TextImage::initWithString(const char *text, int width, int height, cocos2d::Font* font, bool releaseRAWData)
{
    bool textIsUTF16 = false;
    
    if (m_pFont)
    {
        m_pFont->release();
        m_pFont = 0;
    }
    
    // carloX
    m_pFont = font;
    
    // generate the glyphs for the requested text (glyphs are latter's bounding boxes)
    if (!generateTextGlyphs(text))
        return false;
    
    Size constrainSize;
    unsigned short int *strUTF16 = 0;
    
    int stringNumChars;
    if (textIsUTF16)
    {
        strUTF16       = (unsigned short int *)text;
        stringNumChars = cc_wcslen(strUTF16);
    }
    else
    {
        // string needs to go to unicode
        strUTF16 = m_pFont->getUTF16Text(text, stringNumChars);
    }
    
    if (!strUTF16 || !stringNumChars)
        return false;
    
    // create all the needed pages
    if (!createPageDefinitions(strUTF16, width, height, m_pFont->getFontMaxHeight()))
        return false;
    
    // release the original string if needed
    if (!textIsUTF16)
        delete [] strUTF16;
    
    // actually create the needed images
    return createImageDataFromPages(m_pFontPages, releaseRAWData);
    
    return true;
}

bool TextImage::createPageDefinitions(unsigned short int *inText, int imageWidth, int imageHeight, int lineHeight)
{
    bool  needToReleaseText = false;
    int   delta             = 0;
    int   currentPage       = 0;
    float currentY          = 0.0;
    
    //
    unsigned short int *strUTF16 = inText;
    
    if (m_pFontPages)
        delete m_pFontPages;
    
    // create pages for the font
    m_pFontPages = new TextFontPagesDef();
    if (!m_pFontPages)
        return false;
    
    // create the first page (ther is going to be at least one page)
    TextPageDef *currentPageDef = new TextPageDef(currentPage, imageWidth, imageHeight);
    if (!currentPageDef)
        return false;
    
    // add the current page
    m_pFontPages->addPage(currentPageDef);
    
    // work out creating pages
    
    do {
        
        // choose texture page
        if ((currentY + lineHeight) > imageHeight)
        {
            currentY     = 0;
            currentPage += 1;
            
            // create a new page and add
            currentPageDef = new TextPageDef(currentPage, imageWidth, imageHeight);
            if (!currentPageDef)
                return false;
            
            m_pFontPages->addPage(currentPageDef);
        }
        
        // get the new fitting string
        Size tempSize;
        tempSize.width  = imageWidth;
        tempSize.height = imageHeight;
        
        // figure out how many glyphs fit in this line
        int newLineSize    = 0;
        int numFittingChar = getNumGlyphsFittingInSize(m_aTextGlyphs, strUTF16, m_pFont, &tempSize, newLineSize);
        
        // crete the temporary new string
        unsigned short int *pTempString = 0;
        pTempString = m_pFont->trimUTF16Text(strUTF16, 0, (numFittingChar - 1));
        
        // create the new line and add to the current page
        TextLineDef *newLine  = new TextLineDef(0.0, currentY, newLineSize, lineHeight);
        if (!newLine)
            return false;
        
        // add all the glyphs to this line
        addGlyphsToLine(newLine, (const char *)pTempString, true);
        
        // add the line the to current page
        currentPageDef->addLine(newLine);
        
        // can now release the string
        delete [] pTempString;
        
        // create the new string
        int stringLenght = m_pFont->getUTF16TextLenght(strUTF16);
        delta = (stringLenght - numFittingChar);
        
        // there is still some leftover, need to work on it
        if (delta)
        {
            // create the new string
            unsigned short int *tempS = m_pFont->trimUTF16Text(strUTF16, numFittingChar, (stringLenght - 1));
            
            if (needToReleaseText)
                delete [] strUTF16;
            
            // a copy of the string has been created, so next time I'll need to release it
            needToReleaseText = true;
            
            // assign pointer
            strUTF16 = tempS;
        }
        
        // go to next line
        currentY += lineHeight;
        
    } while(delta);
    
    if (needToReleaseText)
        delete [] strUTF16;
    
    return true;
}

int TextImage::getNumGlyphsFittingInSize(std::map<unsigned short int, GlyphDef> &glyphDefs, unsigned short int *strUTF8, Font *pFont, Size *constrainSize, int &outNewSize)
{
    if (!strUTF8)
        return 0;
    
    float widthWithBBX  =  0.0f;
    float lastWidth     =  0.0f;
    
    // get the string to UTF8
    int numChar = cc_wcslen(strUTF8);
    
    for (int c = 0; c < numChar; ++c)
    {
        widthWithBBX += (glyphDefs[strUTF8[c]].getRect().size.width + glyphDefs[strUTF8[c]].getPadding());
        
        if (widthWithBBX >= constrainSize->width)
        {
            outNewSize = lastWidth;
            return c;
        }
        
        lastWidth = widthWithBBX;
    }
    
    outNewSize = constrainSize->width;
    return numChar;
}

bool TextImage::addGlyphsToLine(TextLineDef *line, const char *lineText, bool textIsUTF16)
{
    if (!m_pFont)
        return false;
    
    int numLetters                   = 0;
    unsigned short int *UTF16string  = 0;
    
    if (textIsUTF16)
    {
        UTF16string = (unsigned short int *)lineText;
        numLetters = cc_wcslen(UTF16string);
    }
    else
    {
        UTF16string  = m_pFont->getUTF16Text(lineText, numLetters);
    }
    
    for (int c = 0; c < numLetters; ++c)
    {
        m_aTextGlyphs[UTF16string[c]].setCommonHeight(line->getHeight());
        line->addGlyph(m_aTextGlyphs[UTF16string[c]] );
    }
    
    if(!textIsUTF16)
        delete [] UTF16string;
    
    return true;
}

bool TextImage::generateTextGlyphs(const char * text)
{
    if (!m_pFont)
        return false;
    
    int numGlyphs = 0;
    GlyphDef *newGlyphs  = m_pFont->getGlyphDefintionsForText(text, numGlyphs);
    
    if (!newGlyphs)
        return false;
    
    if (!m_aTextGlyphs.empty())
        m_aTextGlyphs.clear();
    
    for (int c = 0; c < numGlyphs; ++c)
        m_aTextGlyphs[newGlyphs[c].getUTF8Letter()] = newGlyphs[c];
    
    delete [] newGlyphs;
    return true;
}

bool TextImage::createImageDataFromPages(TextFontPagesDef *thePages, bool releaseRAWData)
{
    int numPages = thePages->getNumPages();
    if (!numPages)
        return false;
    
    for (int c = 0; c < numPages; ++c)
    {
        unsigned char *pageData = 0;
        pageData = preparePageGlyphData(thePages->getPageAt(c));
        
        if (pageData)
        {
            // set the page data
            thePages->getPageAt(c)->setPageData(pageData);
            
            // crete page texture and relase RAW data
            thePages->getPageAt(c)->preparePageTexture(releaseRAWData);
        }
        else
        {
            return false;
        }
    }
    
    return true;
}

unsigned char * TextImage::preparePageGlyphData(TextPageDef *thePage)
{
    return renderGlyphData(thePage);
}

unsigned char * TextImage::renderGlyphData(TextPageDef *thePage)
{
    if (!thePage)
        return 0;
    
    if (!m_pFont)
        return 0;
    
    if (thePage->getNumLines() == 0)
        return NULL;
    
    int pageWidth  = thePage->getWidth();
    int pageHeight = thePage->getHeight();
    
    // prepare memory and clean to 0
    int sizeInBytes     = (pageWidth * pageHeight * 4);
    unsigned char* data = new unsigned char[sizeInBytes];
    
    if (!data)
        return 0;
    
    kdMemset(data, 0, sizeInBytes);
    
    int numLines = thePage->getNumLines();
    
    for (int c = 0; c < numLines; ++c)
    {
        TextLineDef *currentLine   = thePage->getLineAt(c);
        
        float origX         = m_pFont->getLetterPadding();
        float origY         = currentLine->getY();
        
        int numGlyphToRender = currentLine->getNumGlyph();
        
        for (int cglyph = 0; cglyph < numGlyphToRender; ++cglyph)
        {
            GlyphDef currGlyph      = currentLine->getGlyphAt(cglyph);
            renderCharAt(currGlyph.getUTF8Letter(), origX, origY, data, pageWidth);
            origX += (currGlyph.getRect().size.width + m_pFont->getLetterPadding());
        }
    }
    
#ifdef _DEBUG_FONTS_
    static int counter = 0;
    char outFilename[512];
    sprintf(outFilename,"testIMG%d", counter);
    ++counter;
    Image *image = new Image;
    image->initWithRawData(data, (pageWidth * pageWidth * 4), 1024, 1024, 8, false);
    image->saveToFile(outFilename);
#endif
    
    // we are done here
    return data;
}

bool TextImage::renderCharAt(unsigned short int charToRender, int posX, int posY, unsigned char *destMemory, int destSize)
{
    if (!m_pFont)
        return false;
    
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

NS_CC_END












