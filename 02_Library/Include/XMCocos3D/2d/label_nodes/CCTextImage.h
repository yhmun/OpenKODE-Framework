/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextImage.h
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

#ifndef __CCTextImage_h__
#define __CCTextImage_h__

#include "CCFont.h"
#include <vector>

NS_CC_BEGIN

class Font;

/** 
 *	@brief GlyphDef defines one single glyph (character) in a text image
 *
 *	it defines the bounding box for the glyph in the texture page, the character the padding (spacing) between characters
 *
 */

class CC_DLL GlyphDef
{
	public :
    
		GlyphDef ( KDvoid ) : m_bValidGlyph ( false )								{   /*do nothing*/										}
		GlyphDef ( KDushort uLetterUTF8, const Rect& tRect )						{	m_tGliphRect = tRect; m_uUTF16Letter = uLetterUTF8;	}
    
		KDvoid						setUTF16Letter ( KDushort uLetterUTF8 )			{	m_uUTF16Letter  = uLetterUTF8;		}
		KDvoid						setRect ( const Rect& tTheRect )				{	m_tGliphRect = tTheRect;			}
		KDushort					getUTF8Letter ( KDvoid )						{	return m_uUTF16Letter;				}
		const Rect&					getRect ( KDvoid ) const						{	return m_tGliphRect;				}
		KDvoid						setPadding ( KDfloat fPadding )					{	m_uPadding = fPadding;				}
		KDfloat						getPadding ( KDvoid )							{	return m_uPadding;					}
		KDvoid						setCommonHeight ( KDfloat fCommonHeight )		{	m_fCommonHeight = fCommonHeight;	}
		KDfloat						getCommonHeight ( KDvoid )						{	return m_fCommonHeight;				}
		KDvoid						setValid ( KDbool bIsValid )					{	m_bValidGlyph = bIsValid;			}
		KDbool						isValid ( KDvoid )								{	return m_bValidGlyph;				}
    
	private :
    
		Rect						m_tGliphRect;
		KDushort					m_uUTF16Letter;
		KDfloat						m_uPadding;
		KDfloat						m_fCommonHeight;
		KDbool						m_bValidGlyph;    
};

/**
 *	@brief TextLineDef define a line of text in a text image texture page
 *
 *	conllects all the GlyphDef for a text line plus line size and line position in text image space
 *
 */
class CC_DLL TextLineDef
{
	public :
    
		TextLineDef ( KDfloat x, KDfloat y, KDfloat width, KDfloat height );
    
		KDfloat						getX ( KDvoid ) const				{ return m_fX;        }
		KDfloat						getY ( KDvoid ) const				{ return m_fY;        }
		KDfloat						getWidth  ( KDvoid ) const			{ return m_fWidth;    }
		KDfloat						getHeight ( KDvoid ) const			{ return m_fHeight;   }
    
		KDvoid						addGlyph ( GlyphDef tTheGlyph )		{ m_aGlyphs.push_back ( tTheGlyph );	}
		KDint						getNumGlyph ( KDvoid )  const		{ return m_aGlyphs.size ( );			}
		const GlyphDef&				getGlyphAt ( KDint nIndex ) const	{ return m_aGlyphs [ nIndex ];			}
    
	private :
    
		KDfloat						m_fX;
		KDfloat						m_fY;
		KDfloat						m_fWidth;
		KDfloat						m_fHeight;
		std::vector<GlyphDef>		m_aGlyphs;    
};

/**
 *	@brief TextPageDef defines one text image page (a TextImage can have/use more than one page)
 *
 *	collects all the TextLineDef for one page, the witdh and height of the page and the  graphics (texture) for the page
 *
 */
class CC_DLL TextPageDef
{
	public :

		/**
		 *	@js NA
		 */
		TextPageDef ( KDint nPageNum, KDint nWidth, KDint nHeight );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		~TextPageDef ( KDvoid );
    
		KDvoid						addLine ( TextLineDef* pTheLine )		{ m_aLines.push_back ( pTheLine );	}
		KDint						getNumLines ( KDvoid ) const			{ return m_aLines.size();			}
		TextLineDef*				getLineAt ( KDint nIndex ) const        { return m_aLines [ nIndex ];		}
		KDint						getWidth ( KDvoid ) const				{ return m_nWidth;					}
		KDint						getHeight ( KDvoid ) const				{ return m_nHeight;					}
		KDint						getPageNumber ( KDvoid ) const			{ return m_nPageNum;				}
		KDvoid						setPageData ( KDubyte* pData )			{ m_pPageData = pData;				}
		const KDubyte*				getPageData ( KDvoid ) const			{ return m_pPageData;				}
		Texture2D*					getPageTexture ( KDvoid );
		KDvoid						preparePageTexture ( KDbool bReleaseRAWData = true );
    
	private :
    
		KDbool						generatePageTexture ( KDbool bReleasePageData = false );
    
		KDint						m_nPageNum;
		KDint						m_nWidth;
		KDint						m_nHeight;
		KDubyte*					m_pPageData;
		Texture2D*					m_pPageTexture;
		std::vector<TextLineDef*>	m_aLines;
    
};

/** 
 *	@brief CCTextFontPages collection of pages (TextPageDef)
 *
 *	A TextImage is composed by one or more text pages. This calss collects all of those pages
 */
class CC_DLL TextFontPagesDef
{
	public :

		/**
		 *	@js ctor
		 */
		TextFontPagesDef ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
	   ~TextFontPagesDef ( KDvoid );
    
		KDvoid						addPage ( TextPageDef* pNewPage )	{ m_aPages.push_back ( pNewPage );	}
		KDint						getNumPages ( KDvoid )  const		{ return m_aPages.size ( );			}
		TextPageDef*				getPageAt ( KDint nIndex ) const	{ return m_aPages [ nIndex ];		}
    
	private :
    
		std::vector<TextPageDef*>	m_aPages;
    
};

/** 
 *	@brief TextImage 
 */
class CC_DLL TextImage
{
	public :

		/**
		 *	@js ctor
		 */
		TextImage ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
	   ~TextImage ( KDvoid );
    
		KDbool							initWithString ( const KDchar* pText, KDint nWidth, KDint nHeight, Font* pFont, KDbool bReleaseRAWData = true );
    
		TextFontPagesDef*				getPages ( KDvoid )  const  { return m_pFontPages; }
		Font*							getFont ( KDvoid ) const    { return m_pFont;      }
    
	private :
    
		KDbool							createImageDataFromPages ( TextFontPagesDef* pThePages, KDbool bReleaseRAWData = true );
		KDbool							addGlyphsToLine ( TextLineDef* pLine, const KDchar* pLineText, KDbool bTextIsUTF16 = false );
		KDbool							generateTextGlyphs ( const KDchar* pText );
		KDint							getNumGlyphsFittingInSize ( std::map<KDushort, GlyphDef>& aGlyphDefs, KDushort* pStrUTF8, Font* pFont, Size* pConstrainSize, KDint& nOutNewSize );
		KDbool							createPageDefinitions ( KDushort* pInText, KDint nImageWidth, KDint nImageHeight, KDint nLineHeight );
		KDubyte*						preparePageGlyphData ( TextPageDef* pThePage );
    
		// glyph rendering
		KDubyte*						renderGlyphData ( TextPageDef* pThePage );
		KDbool							renderCharAt ( KDushort charToRender, KDint nPosX, KDint nPosY, KDubyte* pDestMemory, KDint nDestSize );
    
		std::map<KDushort, GlyphDef>	m_aTextGlyphs;
		TextFontPagesDef*				m_pFontPages;
		Font*							m_pFont;
};

NS_CC_END

#endif	// __CCTextImage_h__
