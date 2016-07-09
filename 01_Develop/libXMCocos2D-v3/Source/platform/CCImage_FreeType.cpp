/* -----------------------------------------------------------------------------------
 *
 *      File            CCImage_FreeType.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 * -----------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#include "platform/CCImage.h"
#include "platform/CCFileUtils.h"
#include "2d/CCConfiguration.h"

#include <XMFreeType/ft2build.h>
#include <XMFreeType/freetype.h>
#include <vector>
#include <algorithm>
#include <ctype.h>

NS_CC_BEGIN

// as FcFontMatch is quite an expensive call, cache the results of getFontFile
static std::map<std::string, std::string>	l_aFontCache;

struct LineBreakGlyph 
{
	FT_UInt							uGlyphIndex;
	KDint							nPaintPosition;
	KDint							nGlyphWidth;

	KDint							nBearingX;
	KDint							nKerning;
	KDint							nHorizAdvance;
};

struct LineBreakLine 
{
	std::vector<LineBreakGlyph>		aGlyphs;
	KDint							nLineWidth;

	LineBreakLine ( KDvoid ) 
	{
		nLineWidth = 0;
	}

	KDvoid		reset ( KDvoid )
	{
		aGlyphs.clear ( );
		nLineWidth = 0;
	}

	KDvoid		calculateWidth ( KDvoid )
	{
		nLineWidth = 0;
		if ( aGlyphs.empty() == KD_FALSE )
		{
			nLineWidth = aGlyphs.at ( aGlyphs.size ( ) - 1 ).nPaintPosition + aGlyphs.at ( aGlyphs.size ( ) - 1 ).nGlyphWidth;
		}
	}
};

class FreeTypeDC
{
	public :

		FreeTypeDC ( KDvoid )
		{
			m_pLibrary  = KD_NULL;
			m_nLibError = FT_Init_FreeType ( &m_pLibrary );
			m_pData     = KD_NULL;

//			FcInit ( );			
			reset  ( );
		}

		virtual ~FreeTypeDC ( KDvoid )
		{
			if ( m_pLibrary )
			{
				FT_Done_FreeType ( m_pLibrary );
			}
			
//			FcFini ( );
		}

		KDvoid			reset ( KDvoid )
		{
			m_nMaxLineWidth  = 0;
			m_nMaxLineHeight = 0;
			m_aTextLines.clear ( );
		}

		KDint			utf8 ( KDchar **p )
		{
			if ( ( **p & 0x80 ) == 0x00 )
			{
				KDint  a = *( (*p)++ );
				return a;
			}
			
			if ( ( **p & 0xE0 ) == 0xC0 )
			{
				KDint  a = *( (*p)++ ) & 0x1F;
				KDint  b = *( (*p)++ ) & 0x3F;

				return ( a << 6 ) | b;
			}
			
			if ( ( **p & 0xF0 ) == 0xE0 )
			{
				KDint  a = *( (*p)++ ) & 0x0F;
				KDint  b = *( (*p)++ ) & 0x3F;
				KDint  c = *( (*p)++ ) & 0x3F;

				return ( a << 12 ) | ( b << 6 ) | c;
			}
			
			if ( ( **p & 0xF8 ) == 0xF0 )
			{
				KDint  a = *( (*p)++ ) & 0x07;
				KDint  b = *( (*p)++ ) & 0x3F;
				KDint  c = *( (*p)++ ) & 0x3F;
				KDint  d = *( (*p)++ ) & 0x3F;

				return ( a << 18 ) | ( b << 12 ) | ( c << 8 ) | d;
			}

			return 0;
		}

		KDbool			isBreakPoint ( FT_UInt uCurrentCharacter, FT_UInt uPreviousCharacter )
		{
    		if ( uPreviousCharacter == '-' || uPreviousCharacter == '/' || uPreviousCharacter == '\\' ) 
			{
    	   		// we can insert a line break after one of these characters
    			return KD_TRUE;
    		}
    		return KD_FALSE;
		}

		KDbool			divideString ( FT_Face pFace, const KDchar* szText, KDint nMaxWidth, KDint nMaxHeight )
		{			
			const KDchar*  pText = szText;

			m_aTextLines.clear ( );
			m_nMaxLineWidth = 0;

			FT_UInt			uUnicode		= 0;
			FT_UInt			uPrevCharacter	= 0;
			FT_UInt			uGlyphIndex		= 0;
			FT_UInt			uPrevGlyphIndex	= 0;
			FT_Vector		tDelta;
			LineBreakLine	tCurrentLine;

			KDint			nCurrentPaintPosition	= 0;
			KDint			nLastBreakIndex			= -1;
			KDboolean		bHasKerning				= FT_HAS_KERNING ( pFace );
			while ( ( uUnicode = utf8 ( (KDchar**) &pText ) ) )
			{
				if ( uUnicode == '\n' ) 
				{
					tCurrentLine.calculateWidth ( );
					m_nMaxLineWidth = KD_MAX ( m_nMaxLineWidth, tCurrentLine.nLineWidth );
					m_aTextLines.push_back ( tCurrentLine );
					tCurrentLine.reset ( );
					uPrevGlyphIndex  = 0;
					uPrevCharacter   = 0;
					nLastBreakIndex  = -1;
					nCurrentPaintPosition = 0;
					continue;
				}

				if ( isBreakPoint ( uUnicode, uPrevCharacter ) )
				{
            		nLastBreakIndex = tCurrentLine.aGlyphs.size ( ) - 1;
				}

				uGlyphIndex = FT_Get_Char_Index ( pFace, uUnicode );
				if ( FT_Load_Glyph ( pFace, uGlyphIndex, FT_LOAD_DEFAULT ) )
				{
					return KD_FALSE;
				}

				if ( uUnicode == (FT_UInt) ' ' ) 
				{
					nCurrentPaintPosition += pFace->glyph->metrics.horiAdvance >> 6;
					uPrevGlyphIndex = uGlyphIndex;
					uPrevCharacter  = uUnicode;
					nLastBreakIndex = tCurrentLine.aGlyphs.size ( );
					continue;
				}

				LineBreakGlyph  tGlyph;
				tGlyph.uGlyphIndex	 = uGlyphIndex;
				tGlyph.nGlyphWidth	 = pFace->glyph->metrics.width >> 6;
				tGlyph.nBearingX	 = pFace->glyph->metrics.horiBearingX >> 6;
				tGlyph.nHorizAdvance = pFace->glyph->metrics.horiAdvance >> 6;
				tGlyph.nKerning		 = 0;

				if ( uPrevGlyphIndex != 0 && bHasKerning )
				{
					FT_Get_Kerning ( pFace, uPrevGlyphIndex, uGlyphIndex, FT_KERNING_DEFAULT, &tDelta );
					tGlyph.nKerning = tDelta.x >> 6;
				}

				if ( nMaxWidth > 0 && nCurrentPaintPosition + tGlyph.nBearingX + tGlyph.nKerning + tGlyph.nGlyphWidth > nMaxWidth )
				{
					KDint  nGlyphCount = tCurrentLine.aGlyphs.size ( );
					if ( nLastBreakIndex >= 0 && nLastBreakIndex < nGlyphCount && nCurrentPaintPosition + tGlyph.nBearingX + tGlyph.nKerning + tGlyph.nGlyphWidth - tCurrentLine.aGlyphs.at ( nLastBreakIndex ).nPaintPosition < nMaxWidth )
					{
						// we insert a line break at our last break opportunity
						std::vector<LineBreakGlyph> aTempGlyphs;
						std::vector<LineBreakGlyph>::iterator  it = tCurrentLine.aGlyphs.begin ( );
						std::advance ( it, nLastBreakIndex );
						aTempGlyphs.insert ( aTempGlyphs.begin ( ), it, tCurrentLine.aGlyphs.end ( ) );
						tCurrentLine.aGlyphs.erase ( it, tCurrentLine.aGlyphs.end ( ) );
						tCurrentLine.calculateWidth ( );
						m_nMaxLineWidth = KD_MAX ( m_nMaxLineWidth, tCurrentLine.nLineWidth );
						m_aTextLines.push_back ( tCurrentLine );
						tCurrentLine.reset();
						nCurrentPaintPosition = 0;
						for ( it = aTempGlyphs.begin ( ); it != aTempGlyphs.end ( ); it++ ) 
						{
							if ( tCurrentLine.aGlyphs.empty ( ) ) 
							{
								nCurrentPaintPosition = -(*it).nBearingX;
								( *it ).nKerning = 0;
							}
							( *it ).nPaintPosition = nCurrentPaintPosition + ( *it ).nBearingX + ( *it ).nKerning;
							tCurrentLine.aGlyphs.push_back ( ( *it ) );
							nCurrentPaintPosition += ( *it ).nKerning + ( *it ).nHorizAdvance;
						}
					} 
					else
					{
						// the current word is too big to fit into one line, insert line break right here
						nCurrentPaintPosition = 0;
						tGlyph.nKerning = 0;
						tCurrentLine.calculateWidth ( );
						m_nMaxLineWidth = KD_MAX ( m_nMaxLineWidth, tCurrentLine.nLineWidth );
						m_aTextLines.push_back ( tCurrentLine );
						tCurrentLine.reset ( );
					}

					uPrevGlyphIndex = 0;
					uPrevCharacter  = 0;
					nLastBreakIndex = -1;
				}
				else
				{
					uPrevGlyphIndex = uGlyphIndex;
					uPrevCharacter  = uUnicode;
				}

				if ( tCurrentLine.aGlyphs.empty ( ) )
				{
					nCurrentPaintPosition = -tGlyph.nBearingX;
				}
				tGlyph.nPaintPosition = nCurrentPaintPosition + tGlyph.nBearingX + tGlyph.nKerning;
				tCurrentLine.aGlyphs.push_back ( tGlyph );
				nCurrentPaintPosition += tGlyph.nKerning + tGlyph.nHorizAdvance;
			}

			if ( tCurrentLine.aGlyphs.empty ( ) == KD_FALSE )
			{
				tCurrentLine.calculateWidth ( );
				m_nMaxLineWidth = KD_MAX ( m_nMaxLineWidth, tCurrentLine.nLineWidth );
				m_aTextLines.push_back ( tCurrentLine );
			}
			return KD_TRUE;
		}

		//
		// compute the start pos of every line
		//
		// return >0 represent the start x pos of the line
		// while -1 means fail
		//
		//
		KDint			computeLineStart ( FT_Face pFace, Image::TextAlign eAlignMask, KDint nLine )
		{
			KDint	nLineWidth = m_aTextLines.at(nLine).nLineWidth;

			if ( eAlignMask == Image::TextAlign::CENTER || eAlignMask == Image::TextAlign::TOP || eAlignMask == Image::TextAlign::BOTTOM )
			{
				return ( m_nMaxLineWidth - nLineWidth ) / 2;
			}
			else if ( eAlignMask == Image::TextAlign::RIGHT || eAlignMask == Image::TextAlign::TOP_RIGHT || eAlignMask == Image::TextAlign::BOTTOM_RIGHT ) 
			{
				return ( m_nMaxLineWidth - nLineWidth );
			}

			// left or other situation
			return 0;
		}

		KDint			computeLineStartY ( FT_Face pFace, Image::TextAlign eAlignMask, KDint nTxtHeight, KDint nBorderHeight )
		{
			KDint	nBaseLinePos = kdCeilf ( FT_MulFix ( pFace->bbox.yMax, pFace->size->metrics.y_scale ) / 64.0f );
			if ( eAlignMask == Image::TextAlign::CENTER || eAlignMask == Image::TextAlign::LEFT || eAlignMask == Image::TextAlign::RIGHT ) 
			{
				//vertical center
				return ( nBorderHeight - nTxtHeight ) / 2 + nBaseLinePos;
			}
			else if ( eAlignMask == Image::TextAlign::BOTTOM_RIGHT || eAlignMask == Image::TextAlign::BOTTOM || eAlignMask == Image::TextAlign::BOTTOM_LEFT )
			{
				//vertical bottom
				return nBorderHeight - nTxtHeight + nBaseLinePos;
			}

			// top alignment
			return nBaseLinePos;
		}

		std::string		getFontFile ( const KDchar* szFamilyName )
		{
    		std::string  sFontPath = szFamilyName;

    		std::map<std::string, std::string>::iterator it = l_aFontCache.find ( szFamilyName );
    		if ( it != l_aFontCache.end ( ) )
			{
    			return it->second;
    		}

    		// check if the parameter is a font file shipped with the application
    		std::string  sLowerCasePath = sFontPath;
    		std::transform ( sLowerCasePath.begin ( ), sLowerCasePath.end ( ), sLowerCasePath.begin ( ), ::tolower );
    		if ( sLowerCasePath.find ( ".ttf" ) != std::string::npos ) 
			{
				sFontPath = FileUtils::getInstance ( )->fullPathForFilename(sFontPath.c_str ( ) );

    			KDFile*  pFile = kdFopen ( sFontPath.c_str ( ), "r" );
    			if ( pFile ) 
				{
    				kdFclose ( pFile );
    				l_aFontCache.insert ( std::pair<std::string, std::string> ( szFamilyName, sFontPath ) );
    				return sFontPath;
    			}
    		}
/*
    		// use fontconfig to match the parameter against the fonts installed on the system
    		FcPattern*  pPattern = FcPatternBuild ( 0, FC_FAMILY, FcTypeString, szFamilyName, (KDchar*) 0 );
    		FcConfigSubstitute ( 0, pPattern, FcMatchPattern );
    		FcDefaultSubstitute ( pPattern );

    		FcResult    tResult;
    		FcPattern*  pFont = FcFontMatch ( 0, pPattern, &tResult );
    		if ( pFont ) 
			{
    			FcChar8*  s = NULL;
    			if ( FcPatternGetString ( font, FC_FILE, 0, &s) == FcResultMatch ) 
				{
    				sFontPath = (const KDchar*) s;

    				FcPatternDestroy ( pFont );
    				FcPatternDestroy ( pattern );

    				l_aFontCache.insert ( std::pair<std::string, std::string> ( szFamilyName, sFontPath ) );
    				return sFontPath;
    			}
    			FcPatternDestroy ( pFont );
    		}
    		FcPatternDestroy ( pPattern );
*/
    		return szFamilyName;
		}

		KDbool			getBitmap ( const KDchar* szText, KDint nWidth, KDint nHeight, Image::TextAlign eAlignMask, const KDchar* szFontName, KDuint uFontSize )
		{
			if ( m_nLibError )
			{
				return KD_FALSE;
			}

			FT_Face      pFace;
			std::string  sFontfile = getFontFile ( szFontName );
			if ( FT_New_Face ( m_pLibrary, sFontfile.c_str ( ), 0, &pFace ) ) 
			{
				// no valid font found use default
				if ( FT_New_Face ( m_pLibrary, "/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 0, &pFace ) )
				{
					return KD_FALSE;
				}
			}

			// select utf8 charmap
			if ( FT_Select_Charmap ( pFace, FT_ENCODING_UNICODE ) )
			{
				FT_Done_Face ( pFace );
				return KD_FALSE;
			}

			if ( FT_Set_Pixel_Sizes ( pFace, uFontSize, uFontSize ) ) 
			{
				FT_Done_Face ( pFace );
				return KD_FALSE;
			}

			if ( divideString ( pFace, szText, nWidth, nHeight ) == KD_FALSE )
			{
				FT_Done_Face ( pFace );
				return KD_FALSE;
			}

			// compute the final line width
			m_nMaxLineWidth = KD_MAX ( m_nMaxLineWidth, nWidth );

			// compute the final line height
			m_nMaxLineHeight = (KDint) kdCeilf ( FT_MulFix ( pFace->bbox.yMax - pFace->bbox.yMin, pFace->size->metrics.y_scale ) / 64.0f );
			KDint  nLineHeight = pFace->size->metrics.height >> 6;
			if ( m_aTextLines.size ( ) > 0 ) 
			{
				m_nMaxLineHeight += ( nLineHeight * ( m_aTextLines.size ( ) - 1 ) );
			}

			KDint  nTxtHeight = m_nMaxLineHeight;
			m_nMaxLineHeight  = KD_MAX ( m_nMaxLineHeight, nHeight );

			m_pData = new KDubyte [ m_nMaxLineWidth * m_nMaxLineHeight * 4 ];
			kdMemset ( m_pData, 0, m_nMaxLineWidth * m_nMaxLineHeight * 4 );

			KDint  nCurYCursor = computeLineStartY ( pFace, eAlignMask, nTxtHeight, m_nMaxLineHeight );
			KDint  nLineCount  = m_aTextLines.size ( );
			for ( KDint nLine = 0; nLine < nLineCount; nLine++ )
			{
				KDint  nCurXCursor = computeLineStart ( pFace, eAlignMask, nLine );
				KDint  nGlyphCount = m_aTextLines.at ( nLine ).aGlyphs.size ( );
				for ( KDint i = 0; i < nGlyphCount; i++ )
				{
					LineBreakGlyph  tGlyph = m_aTextLines.at ( nLine ).aGlyphs.at ( i );

					if ( FT_Load_Glyph ( pFace, tGlyph.uGlyphIndex, FT_LOAD_RENDER ) )
					{
						continue;
					}

					FT_Bitmap&   tBitmap = pFace->glyph->bitmap;
										
					KDint  nOffsetY = nCurYCursor - ( pFace->glyph->metrics.horiBearingY >> 6 ); 
					KDint  nOffsetX = nCurXCursor + tGlyph.nPaintPosition;

					if ( tBitmap.pixel_mode == FT_PIXEL_MODE_MONO )		// 폰트 사이즈가 작은 경우 발생 한다. By 유재현님
					{
						for ( KDint y = 0; y < tBitmap.rows; ++y )
						{
							KDint  nY = nOffsetY + y;
							if ( nY >= m_nMaxLineHeight )
							{
								// exceed the height truncate
								break;
							}
							nY *= m_nMaxLineWidth;

							KDint  nBitmapY = y * tBitmap.pitch;

							for ( KDint x = 0; x < tBitmap.width; ++x )
							{
								KDint  nX = nOffsetX + x;
								if ( tBitmap.buffer [ nBitmapY + ( x >> 3 ) ] & ( 0x80 >> ( x & 7 ) ) )
								{
									*(KDint*) &m_pData [ ( nY + nX ) * 4 + 0 ] = 0xffffffff;
								}
							}
						}
					}
					else
					{
						for ( KDint y = 0; y < tBitmap.rows; ++y )
						{
							KDint  nY = nOffsetY + y;
							if ( nY >= m_nMaxLineHeight )
							{
								// exceed the height truncate
								break;
							}
							nY *= m_nMaxLineWidth;

							KDint  nBitmapY = y * tBitmap.width;
							for ( KDint x = 0; x < tBitmap.width; ++x )
							{
								KDubyte  cTemp = tBitmap.buffer [ nBitmapY + x ];
								if ( cTemp == 0 )
								{
									continue;
								}

								KDint	nX = nOffsetX + x;
								KDint	nTemp = cTemp << 24 | cTemp << 16 | cTemp << 8 | cTemp;

								*(KDint*) &m_pData [ ( nY + nX ) * 4 + 0 ] = nTemp;
							}
						}
					}
				}
				// step to next line
				nCurYCursor += nLineHeight;				
			}

			//  free face
			FT_Done_Face ( pFace );
			return KD_TRUE;
		}

	public :
		
		FT_Library						m_pLibrary;
		KDubyte*						m_pData;
		KDint							m_nLibError;
		std::vector<LineBreakLine>		m_aTextLines;
		KDint							m_nMaxLineWidth;
		KDint							m_nMaxLineHeight;
};

static FreeTypeDC& sharedBitmapDC ( KDvoid )
{
	static FreeTypeDC  s_tBitmapDC;
	return s_tBitmapDC;
}

//////////////////////////////////////////////////////////////////////////
// Impliment CCImage
//////////////////////////////////////////////////////////////////////////

KDbool Image::initWithFreeTypeFont ( const KDchar* szText, KDint nWidth, KDint nHeight, TextAlign eAlignMask, const KDchar* szFontName, KDint nSize )
{
	KDbool  bRet = KD_FALSE;
	do
	{
		CC_BREAK_IF ( !szText );

		FreeTypeDC&  tDC = sharedBitmapDC ( );
		std::string  sFullFontName = FileUtils::getInstance ( )->fullPathForFilename ( szFontName ).c_str ( );

		CC_BREAK_IF ( !tDC.getBitmap ( szText, nWidth, nHeight, eAlignMask, sFullFontName.c_str ( ), nSize ) );

		// assign the dc.m_pData to m_pData in order to save time
		m_pData = tDC.m_pData;
		CC_BREAK_IF ( !m_pData );

		m_nWidth				 = tDC.m_nMaxLineWidth;
		m_nHeight				 = tDC.m_nMaxLineHeight;
		m_bHasPremultipliedAlpha = KD_TRUE;
		m_eRenderFormat			 = Texture2D::PixelFormat::RGBA8888;
		m_nDataLen				 = m_nWidth * m_nHeight * 4;

		bRet = KD_TRUE;

		tDC.reset ( );
	} while ( 0 );

	// do nothing
	return bRet;
}

KDchar*  trim_string_with_dot2 ( KDchar* szDst, const KDchar* szSrc, KDsize uSize )
{
	return szDst;
}

NS_CC_END

