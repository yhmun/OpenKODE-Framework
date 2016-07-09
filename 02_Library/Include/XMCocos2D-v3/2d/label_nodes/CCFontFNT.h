/* -----------------------------------------------------------------------------------
 *
 *      File            CCFontFNT.h
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

#ifndef __CCFontFNT_h__
#define __CCFontFNT_h__

#include "CCFont.h"
#include "CCLabelBMFont.h"

NS_CC_BEGIN

class FontFNT : public Font
{    
	public :
    
		static FontFNT*				create ( const std::string& sFntFilePath );
    
		virtual Size*				getAdvancesForTextUTF16 ( KDushort* pText, KDint& nOutNumLetters ) const override;
		virtual Rect				getRectForChar ( KDushort uTheChar ) const override;
		virtual FontAtlas*			createFontAtlas ( KDvoid ) override;
    
	protected :
    
		FontFNT ( CCBMFontConfiguration* pTheContfig ) : m_pConfiguration( pTheContfig ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~FontFNT ( KDvoid );
    
	private :
    
		KDint						getAdvanceForChar ( KDushort uTheChar ) const;
		KDint						getHorizontalKerningForChars ( KDushort uFirstChar, KDushort uSecondChar ) const;
		Rect						getRectForCharInternal ( KDushort uTheChar ) const;
    
		CCBMFontConfiguration*		m_pConfiguration;   
};

NS_CC_END

#endif	// __CCFontFNT_h__
