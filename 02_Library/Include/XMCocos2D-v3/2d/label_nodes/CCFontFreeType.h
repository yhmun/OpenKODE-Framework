/* -----------------------------------------------------------------------------------
 *
 *      File            CCFontFreeType.h
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

#ifndef __CCFontFreeType_h__
#define __CCFontFreeType_h__

#include <string>
#include <XMFreeType/ft2build.h>

#include "CCFont.h"

#include FT_FREETYPE_H

NS_CC_BEGIN

class CC_DLL FontFreeType : public Font
{
	public :
    
		static FontFreeType*	create ( const std::string& sFontName, KDint nFontSize, GlyphCollection tGlyphs, const KDchar* pCustomGlyphs );

		static KDvoid			shutdownFreeType ( KDvoid );

		virtual FontAtlas*		createFontAtlas ( KDvoid ) override;
		virtual Size*			getAdvancesForTextUTF16 ( KDushort* pText, KDint& nOutNumLetters ) const override;
		virtual GlyphDef*		getGlyphDefintionsForText ( const KDchar* pText, KDint& nOutNumGlyphs, KDbool bUTF16text = false ) const override;
		KDubyte*				getGlyphBitmap ( KDushort uTheChar, KDint& nOutWidth, KDint& nOutHeight ) const override;
		virtual KDint			getFontMaxHeight ( KDvoid ) const override;
		virtual KDint			getLetterPadding ( KDvoid ) const override;
    
		KDbool					getBBOXFotChar ( KDushort uTheChar, Rect& tOutRect ) const;

		inline KDbool			isDynamicGlyphCollection ( KDvoid ) { return m_bDynamicGlyphCollection;}  

	protected:
    
		FontFreeType ( KDbool bDynamicGlyphCollection = false );
		virtual ~FontFreeType ( KDvoid );

		KDbool					createFontObject ( const std::string& sFontName, KDint nFontSize );
    
	private :

		KDbool					initFreeType ( KDvoid );
		FT_Library				getFTLibrary ( KDvoid );
    
		KDint					getAdvanceForChar ( KDushort uTheChar ) const;
		KDint					getBearingXForChar ( KDushort uTheChar ) const;
		KDint					getHorizontalKerningForChars ( KDushort uFirstChar, KDushort uSecondChar ) const;
    
		static FT_Library		m_pFTlibrary;
		static KDbool			m_bFTInitialized;
		FT_Face					m_tFontRef;
		const KDint				m_nLetterPadding;
		std::string				m_sFontName;
		KDubyte*				m_pTTFData;
		KDbool					m_bDynamicGlyphCollection;
};

NS_CC_END

#endif	// __CCFontFreeType_h__
