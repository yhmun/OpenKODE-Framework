/* -----------------------------------------------------------------------------------
 *
 *      File            CCFontAtlas.h
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

#ifndef __CCFontAtlas_h__
#define __CCFontAtlas_h__

#include <unordered_map>
#include "../../base/CCObject.h"
#include "../textures/CCTexture2D.h"

NS_CC_BEGIN

// fwd
class Font;

struct FontLetterDefinition
{
    KDushort			letteCharUTF16;
    KDfloat				U;
    KDfloat				V;
    KDfloat				width;
    KDfloat				height;
    KDfloat				offsetX;
    KDfloat				offsetY;
    KDint				textureID;
    KDfloat				commonLineHeight;
    KDfloat				anchorX;
    KDfloat				anchorY;
    KDbool				validDefinition;
};

class CC_DLL FontAtlas : public Object
{
	public :

		/**
		 *	@js ctor
		 */
		FontAtlas ( Font& tTheFont );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~FontAtlas ( KDvoid );
    
		KDvoid			addLetterDefinition ( const FontLetterDefinition& tLetterDefinition );
		KDbool			getLetterDefinitionForChar ( KDushort uLetteCharUTF16, FontLetterDefinition& tOutDefinition );
    
		KDbool			prepareLetterDefinitions ( KDushort* pUtf16String );

		KDvoid			addTexture ( Texture2D& tTexture, KDint nSlot );
		KDfloat			getCommonLineHeight ( KDvoid ) const;
		KDvoid			setCommonLineHeight ( KDfloat fNewHeight );
    
		Texture2D&		getTexture ( KDint nSlot );
		const Font*		getFont ( KDvoid ) const;
    
	private :

		KDbool			renderCharAt ( KDushort uCharToRender, KDint nPosX, KDint nPosY, KDubyte* pDestMemory, KDint nDestSize );

		KDvoid			relaseTextures ( KDvoid );

		std::unordered_map<KDint, Texture2D*>				m_aAtlasTextures;
		std::unordered_map<KDushort, FontLetterDefinition>	m_aFontLetterDefinitions;

		KDfloat			m_fCommonLineHeight;
		Font*			m_pFont;

		// Dynamic GlyphCollection related stuff
		KDint			m_nCurrentPage;
		KDubyte*		m_pCurrentPageData;
		KDint			m_nCurrentPageDataSize;
		KDfloat			m_fCurrentPageOrigX;
		KDfloat			m_fCurrentPageOrigY;
		KDfloat			m_fCurrentPageLineHeight;
		KDfloat			m_fLetterPadding;
};

NS_CC_END

#endif	// __CCFontAtlas_h__
