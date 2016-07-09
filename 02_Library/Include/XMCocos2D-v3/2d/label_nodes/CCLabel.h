/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabel.h
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

#ifndef __CCLabel_h__
#define __CCLabel_h__

#include "../sprite_nodes/CCSpriteBatchNode.h"
#include "CCLabelTextFormatProtocol.h"
#include "../../ccTypes.h"

NS_CC_BEGIN

enum class GlyphCollection 
{    
    DYNAMIC,
    NEHE,
    ASCII,
    CUSTOM
};

// fwd
struct FontLetterDefinition;
class FontAtlas;

class CC_DLL Label : public SpriteBatchNode, public LabelProtocol, public RGBAProtocol, public LabelTextFormatProtocol
{
	public:
    
		// static create
		static Label*						createWithTTF ( const std::string& sLabel, const std::string& sFontFilePath, KDint nFontSize, KDint nLineSize = 0, TextHAlignment eAlignment = TextHAlignment::CENTER, GlyphCollection eGlyphs = GlyphCollection::NEHE, const KDchar* pCustomGlyphs = 0 );
    
		static Label*						createWithBMFont ( const std::string& sLabel, const std::string& sBmfontFilePath, TextHAlignment eAlignment = TextHAlignment::CENTER, KDint nlineSize = 0 );
    
		KDbool								setText ( const std::string& sStringToRender, KDfloat fLineWidth, TextHAlignment eAlignment = TextHAlignment::LEFT, KDbool bLineBreakWithoutSpaces = false );
    
		virtual KDvoid						setString ( const std::string& sStringToRender ) override;
		virtual KDvoid						setAlignment ( TextHAlignment eAlignment );
		virtual KDvoid						setWidth ( KDfloat fWidth );
		virtual KDvoid						setLineBreakWithoutSpace ( KDbool bBreakWithoutSpace );
		virtual KDvoid						setScale  ( KDfloat scale) override;
		virtual KDvoid						setScaleX ( KDfloat scaleX) override;
		virtual KDvoid						setScaleY ( KDfloat scaleY) override;

		// RGBAProtocol
		virtual KDbool						 isOpacityModifyRGB ( KDvoid ) const override;
		virtual KDvoid						setOpacityModifyRGB ( KDbool bIsOpacityModifyRGB ) override;
		virtual KDvoid						setOpacity ( GLubyte cOpacity ) override;
		virtual KDvoid						updateDisplayedOpacity ( GLubyte cParentOpacity ) override;
		
		virtual KDbool						 isCascadeOpacityEnabled ( KDvoid ) const override;
		virtual KDvoid						setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled ) override;

		virtual KDvoid						setColor ( const Color3B& tColor ) override;
		virtual KDvoid						updateDisplayedColor(const Color3B& tParentColor ) override;
		
		virtual KDbool						 isCascadeColorEnabled ( KDvoid ) const override;
		virtual KDvoid						setCascadeColorEnabled ( KDbool bCascadeColorEnabled ) override;

		virtual const Color3B&				getColor ( KDvoid ) const override;
		virtual const Color3B&				getDisplayedColor ( KDvoid ) const override;
		virtual KDubyte						getOpacity ( KDvoid ) const override;
		virtual KDubyte						getDisplayedOpacity ( KDvoid ) const override;
    
		 // CCLabelTextFormat protocol implementation
		virtual std::vector<LetterInfo>*	getLettersInfo ( KDvoid ) override { return &m_aLettersInfo; }
		virtual KDbool						recordLetterInfo ( const Point& tPoint, KDushort uTheChar, KDint nSpriteIndex ) override;
		virtual KDbool						recordPlaceholderInfo ( KDint nSpriteIndex ) override;
		virtual KDfloat						getLetterPosXLeft ( KDint nIndex ) const override;
		virtual KDfloat						getLetterPosXRight ( KDint nIndex ) const override;

		virtual Sprite*						getLetter ( KDint nID ) override;
    
		// font related stuff
		virtual KDint						getCommonLineHeight ( KDvoid ) const override;
		virtual KDint						getKerningForCharsPair ( KDushort uFirst, KDushort uSecond ) const override;
		virtual KDint						getXOffsetForChar ( KDushort c ) const override;
		virtual KDint						getYOffsetForChar ( KDushort c ) const override;
		virtual KDint						getAdvanceForChar ( KDushort c, int hintPositionInString ) const override;
		virtual Rect						getRectForChar ( KDushort c ) const override;
    
		// string related stuff
		virtual KDint						getStringNumLines ( KDvoid ) const override;
		virtual KDint						getStringLenght ( KDvoid ) const override;
		virtual KDushort					getCharAtStringPosition ( KDint nPosition ) const override;
		virtual KDushort*					getUTF8String ( KDvoid ) const override;
		virtual KDvoid						assignNewUTF8String ( KDushort* pNewString ) override;
		virtual TextHAlignment				getTextAlignment ( KDvoid ) const override;
    
		// label related stuff
		virtual KDfloat						getMaxLineWidth ( KDvoid ) const override;
		virtual KDbool						breakLineWithoutSpace ( KDvoid ) const override;
		virtual Size						getLabelContentSize ( KDvoid ) const override;
		virtual KDvoid						setLabelContentSize ( const Size& tNewSize ) override;
    
		// carloX
		virtual const std::string&			getString ( KDvoid ) const override { static std::string _ret ( "not implemented" ); return _ret; }

		KDvoid								addChild ( Node* pChild, KDint zOrder = 0, KDint nTag = 0 ) override;

	private :

		/**
		 *	@js NA
		 */
		Label ( FontAtlas* pAtlas, TextHAlignment eAlignment );

		/**
		 *  @js NA
		 *  @lua NA
		 */
	   ~Label ( KDvoid );
    
		static Label*						createWithAtlas ( FontAtlas* pAtlas, TextHAlignment eAlignment = TextHAlignment::LEFT, KDint nLineSize = 0 );
    
		KDbool								init ( KDvoid );
    
		KDvoid								alignText ( KDvoid );   
    
		KDbool								computeAdvancesForString ( KDushort* pStringToRender );
		KDbool								setCurrentString ( KDushort* pStringToSet );
		KDbool								setOriginalString ( KDushort* pStringToSet );
		KDvoid								resetCurrentString ( KDvoid );
         
		Sprite*								updateSpriteWithLetterDefinition ( Sprite* pSpriteToUpdate, const FontLetterDefinition& tTheDefinition, Texture2D* pTheTexture );        
        
		//! used for optimization
		Sprite*								m_pReusedLetter;
		std::vector<LetterInfo>				m_aLettersInfo;       
   
		KDfloat								m_fCommonLineHeight;
		KDbool								m_bLineBreakWithoutSpaces;
		KDfloat								m_fWidth;
		TextHAlignment						m_eAlignment;
		KDushort*							m_pCurrentUTF16String;
		KDushort*							m_pOriginalUTF16String;
		Size*								m_pAdvances;
		FontAtlas*							m_pFontAtlas;
		Color3B								m_tDisplayedColor;
		Color3B								m_tRealColor;
		KDbool								m_bCascadeColorEnabled;
		KDbool								m_bCascadeOpacityEnabled;
		KDubyte								m_cDisplayedOpacity;
		KDubyte								m_cRealOpacity;
		KDbool								m_bIsOpacityModifyRGB;        
};

NS_CC_END

#endif	// __CCLabel_h__
