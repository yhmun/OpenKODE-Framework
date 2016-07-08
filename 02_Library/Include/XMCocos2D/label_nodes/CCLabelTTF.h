/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelTTF.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCLabelTTF_h__
#define __CCLabelTTF_h__

#include "../sprite_nodes/CCSprite.h"
#include "../textures/CCTexture2D.h"

#include <string>

NS_CC_BEGIN
    
/**
 * @addtogroup GUI
 * @{
 * @addtogroup label
 * @{
 */

/** 
 *	@brief	CCLabelTTF is a subclass of CCTextureNode that knows how to render text labels
 *
 *	All features from CCTextureNode are valid in CCLabelTTF
 *
 *	CCLabelTTF objects are slow. Consider using CCLabelAtlas or CCLabelBMFont instead.
 *
 *	Custom ttf file can be put in assets/ or external storage that the Application can access.
 *	@code
 *	CCLabelTTF *label1 = CCLabelTTF::create("alignment left", "A Damn Mess", fontSize, blockSize, 
 *                                          kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
 *	CCLabelTTF *label2 = CCLabelTTF::create("alignment right", "/mnt/sdcard/Scissor Cuts.ttf", fontSize, blockSize,
 *                                          kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
 *	@endcode
 *
 */
class CCLabelTTF : public CCSprite, public CCLabelProtocol
{
	public :
		         CCLabelTTF ( KDvoid );
		virtual ~CCLabelTTF ( KDvoid );

	public :

		/**
		 *	creates a CCLabelTTF with a font name and font size in points
		 *	@since v2.0.1
		 */
		static   CCLabelTTF*	create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize );
	    
		/** 
		 *	creates a CCLabelTTF from a fontname, horizontal alignment, dimension in points,  and font size in points.
		 *	@since v2.0.1
		 */
		static   CCLabelTTF*	create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCTextAlignment eAlignH );
	    
		/** 
		 *	creates a CCLabel from a fontname, alignment, dimension in points and font size in points
		 *	@since v2.0.1
		 */
		static   CCLabelTTF*	create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCTextAlignment eAlignH, CCVerticalTextAlignment eAlignV );

		static   CCLabelTTF*	create ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCAlignment eAlign = kCCAlignmentCenter );

		/** Create a label with string and a font definition*/
		static   CCLabelTTF*	createWithFontDefinition ( const KDchar* szString, ccFontDefinition &textDefinition );
		/**
		 *	Creates an label.
		 */
		static   CCLabelTTF*	create ( KDvoid );

	public :

		const KDchar*			description ( KDvoid );

		/** initializes the CCLabelTTF with a font name and font size */
		virtual KDbool			initWithString ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize );

		/** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
		virtual KDbool			initWithString ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCTextAlignment eAlignH );
	    
		/** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
		virtual KDbool			initWithString ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCTextAlignment eAlignH, CCVerticalTextAlignment eAlignV );	  

		virtual KDbool			initWithString ( const KDchar* szString, const KDchar* szFontName, KDfloat fFontSize, const CCSize& tDimensions, CCAlignment eAlign );	   
 

		/** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
		virtual KDbool			initWithStringAndTextDefinition ( const KDchar* szString, ccFontDefinition& tTextDefinition );

		/** initializes the CCLabelTTF */
		virtual KDbool			init ( KDvoid );

		/** set the text definition used by this label */
		KDvoid					setTextDefinition ( ccFontDefinition* pTheDefinition );

		/** get the text definition used by this label */
		ccFontDefinition*		getTextDefinition ( KDvoid );

		/** enable or disable shadow for the label */
		KDvoid					enableShadow ( const CCSize& tShadowOffset, KDfloat fShadowOpacity, KDfloat fShadowBlur, KDbool bMustUpdateTexture = KD_TRUE );

		/** disable shadow rendering */
		KDvoid					disableShadow ( KDbool bMustUpdateTexture = KD_TRUE );

		/** enable or disable stroke */
		KDvoid					enableStroke ( const ccColor3B& tStrokeColor, KDfloat fStrokeSize, KDbool bMustUpdateTexture = KD_TRUE );

		/** disable stroke */
		KDvoid					disableStroke ( KDbool bMustUpdateTexture = KD_TRUE );

		/** set text tinting */
		KDvoid					setFontFillColor ( const ccColor3B& tTintColor, KDbool bMustUpdateTexture = KD_TRUE );

		/**
		 *	changes the string to render
		 *	@warning Changing the string is as expensive as creating a new CCLabelTTF. To obtain better performance use CCLabelAtlas
		 */
		virtual KDvoid			setString ( const KDchar* szString );
		virtual const KDchar*	getString ( KDvoid );

		CCTextAlignment			getHorizontalAlignment ( KDvoid );
		KDvoid					setHorizontalAlignment ( CCTextAlignment eAlignment );
	    
		CCVerticalTextAlignment	getVerticalAlignment ( KDvoid );
		KDvoid					setVerticalAlignment ( CCVerticalTextAlignment eAlignment );
	    
		CCSize					getDimensions ( KDvoid );
		KDvoid					setDimensions ( const CCSize& tDimensions );
	    
		KDfloat					getFontSize ( KDvoid );
		KDvoid					setFontSize ( KDfloat fFontSize );
	    
		const KDchar*			getFontName ( KDvoid );
		KDvoid					setFontName ( const KDchar* szFontName );

	private :

		KDbool					updateTexture ( KDvoid );
	
	protected :

		/** set the text definition for this label */
		KDvoid					_updateWithTextDefinition ( ccFontDefinition& tTextDefinition, KDbool bMustUpdateTexture = KD_TRUE );

		ccFontDefinition		_prepareTextDefinition ( KDbool bAdjustForResolution = KD_FALSE );

	protected :

		/** Dimensions of the label in Points */
		CCSize							m_tDimensions;

		/** The alignment of the label */
		CCTextAlignment					m_eAlignH;

		/** The vertical alignment of the label */
		CCVerticalTextAlignment			m_eAlignV;

		/** Font name used in the label */
		std::string						m_sFontName;

		/** Font size of the label */
		KDfloat							m_fFontSize;

		/** label's string */
		std::string						m_sString;

		/** font shadow */
		KDbool							m_bShadowEnabled;
		CCSize							m_tShadowOffset;
		KDfloat							m_fShadowOpacity;
		KDfloat							m_fShadowBlur;


		/** font stroke */
		KDbool							m_bStrokeEnabled;
		ccColor3B						m_tStrokeColor;
		KDfloat							m_fStrokeSize;

		/** font tint */
		ccColor3B						m_tTextFillColor;
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCLabelTTF_h__

