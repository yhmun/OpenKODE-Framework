/* -----------------------------------------------------------------------------------
 *
 *      File            CCFont.h
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

#ifndef __CCFont_h__
#define __CCFont_h__

#include <string>

#include "CCLabel.h"

NS_CC_BEGIN

// fwd
class GlyphDef;
class FontAtlas;


class CC_DLL Font : public Object
{
	public :

		// create the font
		static  Font*			createWithTTF ( const std::string& sFntName, KDint nFontSize, GlyphCollection tGlyphs, const KDchar* pCustomGlyphs );
		static  Font*			createWithFNT ( const std::string& sFntFilePath );
    
		virtual FontAtlas*		createFontAtlas ( KDvoid ) = 0;

		virtual Size*			getAdvancesForTextUTF16 ( KDushort* pText, KDint& nOutNumLetters ) const = 0;
		virtual const KDchar*	getCurrentGlyphCollection ( KDvoid ) const;
    
		virtual KDint			getLetterPadding ( KDvoid ) const { return 0; }
		virtual KDubyte*		getGlyphBitmap ( KDushort uTheChar, KDint& nOutWidth, KDint& nOutHeight ) const { return 0; }
		virtual GlyphDef*		getGlyphDefintionsForText ( const KDchar* pText, KDint& nOutNumGlyphs, KDbool bUTF16text = false ) const { return 0; }
		virtual KDint			getFontMaxHeight ( KDvoid ) const { return 0; }
		virtual Rect			getRectForChar ( KDushort uTheChar ) const;
    
		virtual KDint			getUTF16TextLenght ( KDushort* pText ) const;
		virtual KDushort*		getUTF16Text ( const KDchar* pText, KDint& tOutNumLetters ) const;
		virtual KDushort*		trimUTF16Text ( KDushort* pText, KDint nNewBegin, KDint nNewEnd ) const;
    
	protected :
    
		Font ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Font ( KDvoid ) { }

		KDvoid					setCurrentGlyphCollection ( GlyphCollection tGlyphs, const KDchar* pCustomGlyphs = 0 );
		const KDchar*			getGlyphCollection ( GlyphCollection tGlyphs ) const;
    

		GlyphCollection			m_tUsedGlyphs;
		KDchar*					m_pCustomGlyphs;
		static const KDchar*	m_pGlyphASCII;
		static const KDchar*	m_pGlyphNEHE;
    
};

NS_CC_END

#endif	// __CCFont_h__
