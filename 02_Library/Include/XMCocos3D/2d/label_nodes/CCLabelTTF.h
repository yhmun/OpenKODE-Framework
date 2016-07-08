/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelTTF.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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

#ifndef __CCLabelTTF_h__
#define __CCLabelTTF_h__

#include "../sprite_nodes/CCSprite.h"
#include "../textures/CCTexture2D.h"

NS_CC_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup label
 *	@{
 */

/**
 *	@brief LabelTTF is a subclass of TextureNode that knows how to render text labels
 *
 *	All features from TextureNode are valid in LabelTTF
 *
 *	LabelTTF objects are slow. Consider using LabelAtlas or LabelBMFont instead.
 *
 *	Custom ttf file can be put in assets/ or external storage that the Application can access.
 *	@code
 *	LabelTTF *label1 = LabelTTF::create("alignment left", "A Damn Mess", fontSize, blockSize, 
 *                                          TextHAlignment::LEFT, TextVAlignment::CENTER);
 *	LabelTTF *label2 = LabelTTF::create("alignment right", "/mnt/sdcard/Scissor Cuts.ttf", fontSize, blockSize,
 *                                          TextHAlignment::LEFT, TextVAlignment::CENTER);
 *	@endcode
 *
 */
class CC_DLL LabelTTF : public Sprite, public LabelProtocol
{
	public :

		/**
		 *	@js ctor
		 */
		LabelTTF ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~LabelTTF ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*				description ( KDvoid ) const;
    
		/**
		 *	creates a LabelTTF with a font name and font size in points
		 *	@since v2.0.1
		 */
		static LabelTTF*			create ( const std::string& sString, const std::string& sFontName, KDfloat fFontSize );
    
		/**
		 *	creates a LabelTTF from a fontname, horizontal alignment, dimension in points,  and font size in points.
		 *	@since v2.0.1
		 */
		static LabelTTF*			create ( const std::string& sString, const std::string& sFontName, KDfloat fFontSize, const Size& tDimensions, TextHAlignment eAlignmentH );
  
		/**
		 *	creates a Label from a fontname, alignment, dimension in points and font size in points
		 *	@since v2.0.1
		 */
		static LabelTTF*			create ( const std::string& sString, const std::string& sFontName, KDfloat fFontSize, const Size& tDimensions, TextHAlignment eAlignmentH, TextVAlignment eAlignmentV );
		
		/** Create a lable with string and a font definition*/
		static LabelTTF*			createWithFontDefinition ( const std::string& sString, FontDefinition& tTextDefinition );
    
		/** initializes the LabelTTF with a font name and font size */
		KDbool						initWithString ( const std::string& sString, const std::string& sFontName, KDfloat fFontSize );
    
		/** initializes the LabelTTF with a font name, alignment, dimension and font size */
		KDbool						initWithString ( const std::string& sString, const std::string& sFontName, KDfloat fFontSize, const Size& tDimensions, TextHAlignment eAlignmentH );

		/** initializes the LabelTTF with a font name, alignment, dimension and font size */
		KDbool						initWithString ( const std::string& sString, const std::string& sFontName, KDfloat fontSize, const Size& dimensions, TextHAlignment eAlignmentH, TextVAlignment eAlignmentV );
    
		/** initializes the LabelTTF with a font name, alignment, dimension and font size */
		KDbool						initWithStringAndTextDefinition ( const std::string& sString, FontDefinition& tTextDefinition );
    
		/** set the text definition used by this label */
		KDvoid						setTextDefinition ( const FontDefinition& tTheDefinition );
    
		/** get the text definition used by this label */
		FontDefinition				getTextDefinition ( KDvoid );
           
		/** enable or disable shadow for the label */
		KDvoid						enableShadow ( const Size& tShadowOffset, KDfloat fShadowOpacity, KDfloat fShadowBlur, KDbool bMustUpdateTexture = true );
    
		/** disable shadow rendering */
		KDvoid						disableShadow ( bool bMustUpdateTexture = true );
    
		/** enable or disable stroke */
		KDvoid						enableStroke ( const Color3B& tStrokeColor, KDfloat fStrokeSize, KDbool bMustUpdateTexture = true );
    
		/** disable stroke */
		KDvoid						disableStroke ( KDbool bMustUpdateTexture = true );
    
		/** set text tinting */
		KDvoid						setFontFillColor ( const Color3B& tTintColor, KDbool bMustUpdateTexture = true );
        
		/** initializes the LabelTTF */
		KDbool						init ( KDvoid );

		/**
		 *	Creates an label.
		 */
		static LabelTTF*			create ( KDvoid );

		/** 
		 *	changes the string to render
		 *	@warning Changing the string is as expensive as creating a new LabelTTF. To obtain better performance use LabelAtlas
		 */
		virtual KDvoid				setString ( const std::string& tLabel ) override;
		virtual const std::string&	getString ( KDvoid ) const override;
    
		TextHAlignment				getHorizontalAlignment ( KDvoid ) const;
		KDvoid						setHorizontalAlignment ( TextHAlignment eAlignment );
    
		TextVAlignment				getVerticalAlignment ( KDvoid ) const;
		KDvoid						setVerticalAlignment ( TextVAlignment eVerticalAlignment );
    
		const Size&					getDimensions ( KDvoid ) const;
		KDvoid						setDimensions ( const Size& tDim );
    
		KDfloat						getFontSize ( KDvoid ) const;
		KDvoid						setFontSize ( KDfloat fFontSize );
    
		const std::string&			getFontName ( KDvoid ) const;
		KDvoid						setFontName ( const std::string& fontName );
    
	private :
		
		KDbool						updateTexture ( KDvoid );

	protected :
    
		/** set the text definition for this label */
		KDvoid						_updateWithTextDefinition ( const FontDefinition& tTextDefinition, KDbool bMustUpdateTexture = true );
		FontDefinition				_prepareTextDefinition ( KDbool bAdjustForResolution = false );
    
		/** Dimensions of the label in Points */
		Size						m_tDimensions;
		/** The alignment of the label */
		TextHAlignment				m_eAlignmentH;
		/** The vertical alignment of the label */
		TextVAlignment				m_eAlignmentV;
		/** Font name used in the label */
		std::string					m_sFontName;
		/** Font size of the label */
		KDfloat						m_fFontSize;
		/** label's string */
		std::string					m_sString;
    
		/** font shadow */
		KDbool						m_bShadowEnabled;
		Size						m_tShadowOffset;
		KDfloat						m_fShadowOpacity;
		KDfloat						m_fShadowBlur;
        
		/** font stroke */
		KDbool						m_bStrokeEnabled;
		Color3B						m_tStrokeColor;
		KDfloat						m_fStrokeSize;
        
		/** font tint */
		Color3B						m_tTextFillColor;    
};

//	end of GUI group
/// @}
/// @}

NS_CC_END

#endif	// __CCLabelTTF_h__

