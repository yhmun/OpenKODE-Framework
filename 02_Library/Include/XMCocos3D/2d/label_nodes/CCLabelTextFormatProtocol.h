/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelTextFormatProtocol.h
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

#ifndef __CCLabelTextFormatProtocol_h__
#define __CCLabelTextFormatProtocol_h__

#include "CCFontAtlas.h"
#include "../sprite_nodes/CCSprite.h"
#include <vector>

NS_CC_BEGIN

struct LetterInfo
{
    FontLetterDefinition		def;

    Point						position;
    Size						contentSize;
    KDbool						visible;
};

class CC_DLL LabelTextFormatProtocol
{
	public :

		virtual ~LabelTextFormatProtocol ( KDvoid ) { }

		virtual KDbool						recordLetterInfo ( const Point& tPoint, KDushort uTheChar, KDint nSpriteIndex ) = 0;
		virtual KDbool						recordPlaceholderInfo ( KDint nSpriteIndex ) = 0;
		virtual std::vector<LetterInfo>*	getLettersInfo ( KDvoid ) = 0;
		virtual KDfloat						getLetterPosXLeft ( KDint nIndex )  const = 0;
		virtual KDfloat						getLetterPosXRight ( KDint nIndex ) const = 0;
		// sprite related stuff
		virtual Sprite*						getLetter ( KDint nID ) = 0;
    
		// font related stuff
		virtual KDint						getCommonLineHeight ( KDvoid ) const = 0;
		virtual KDint						getKerningForCharsPair ( KDushort uFirst, KDushort uSecond ) const = 0;
		virtual KDint						getXOffsetForChar ( KDushort c ) const = 0;
		virtual KDint						getYOffsetForChar ( KDushort c ) const = 0;
		virtual KDint						getAdvanceForChar ( KDushort c, KDint nHintPositionInString ) const = 0;
		virtual Rect						getRectForChar ( KDushort c ) const = 0;
    
		// string related stuff
		virtual KDint						getStringNumLines ( KDvoid ) const = 0;
		virtual KDint						getStringLenght ( KDvoid ) const = 0;
		virtual KDushort					getCharAtStringPosition ( KDint nPosition ) const = 0;
		virtual KDushort*					getUTF8String ( KDvoid ) const = 0;
		virtual KDvoid						assignNewUTF8String ( KDushort* pNewString) = 0;
		virtual TextHAlignment				getTextAlignment ( KDvoid ) const = 0;
    
		// label related stuff
		virtual KDfloat						getMaxLineWidth ( KDvoid) const = 0;
		virtual KDbool						breakLineWithoutSpace ( KDvoid ) const = 0;
		virtual Size						getLabelContentSize ( KDvoid ) const = 0;
		virtual KDvoid						setLabelContentSize ( const Size& tNewSize ) = 0;
};

NS_CC_END

#endif	// __CCLabelTextFormatProtocol_h__
