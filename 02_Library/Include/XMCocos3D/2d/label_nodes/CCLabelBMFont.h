/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelBMFont.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
 *
 *      Use any of these editors to generate BMFonts:
 *
 *         http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
 *         http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
 *         http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
 *         http://www.angelcode.com/products/bmfont/ (Free, Windows only)
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

#ifndef __CCLabelBMFont_h__
#define __CCLabelBMFont_h__

#include "../sprite_nodes/CCSpriteBatchNode.h"
#include "../../support/data_support/uthash.h"
#include <map>
#include <sstream>
#include <iostream>
#include <vector>

NS_CC_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup label
 *	@{
 */

enum 
{
    kLabelAutomaticWidth = -1,
};

struct _FontDefHashElement;

/**
 *	@struct ccBMFontDef
 *	BMFont definition
 */
typedef struct _BMFontDef 
{
    //! ID of the character
    KDuint			charID;
    //! origin and size of the font
    Rect			rect;
    //! The X amount the image should be offset when drawing the image (in pixels)
    KDshort			xOffset;
    //! The Y amount the image should be offset when drawing the image (in pixels)
    KDshort			yOffset;
    //! The amount to move the current position after drawing the character (in pixels)
    KDshort			xAdvance;
} ccBMFontDef;

/**
 *	@struct ccBMFontPadding
 *	BMFont padding
 *	@since v0.8.2
 */
typedef struct _BMFontPadding
{
    /// padding left
    KDint			left;
    /// padding top
    KDint			top;
    /// padding right
    KDint			right;
    /// padding bottom
    KDint			bottom;
} ccBMFontPadding;

typedef struct _FontDefHashElement
{
	KDuint			key;		// key. Font Unicode value
	ccBMFontDef		fontDef;	// font definition
	UT_hash_handle	hh;
} tFontDefHashElement;

// Equal function for targetSet.
typedef struct _KerningHashElement
{
	KDint			key;		// key for the hash. 16-bit for 1st element, 16-bit for 2nd element
	KDint			amount;
	UT_hash_handle	hh;
} tKerningHashElement;

/**
 *	@brief CCBMFontConfiguration has parsed configuration of the the .fnt file
 *	@since v0.8
 */
class CC_DLL CCBMFontConfiguration : public Object
{
	// XXX: Creating a public interface so that the bitmapFontArray[] is accessible
	public :	//@public
		
		// BMFont definitions
		tFontDefHashElement*			m_pFontDefDictionary;

		//! FNTConfig: Common Height Should be signed (issue #1343)
		KDint							m_nCommonHeight;
		//! Padding
		ccBMFontPadding					m_tPadding;
		//! atlas name
		std::string						m_sAtlasName;
		//! values for kerning
		tKerningHashElement*			m_pKerningDictionary;
    
		// Character Set defines the letters that actually exist in the font
		std::set<KDuint>*				m_pCharacterSet;

	public :

		/**
		 *	@js ctor
		 */
		CCBMFontConfiguration ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~CCBMFontConfiguration ( KDvoid );
		/**
		 * @js NA
		 * @lua NA
		 */
		const KDchar*					description ( KDvoid ) const;

		/** allocates a CCBMFontConfiguration with a FNT file */
		static CCBMFontConfiguration*	create ( const std::string& sFNTfile );

		/** initializes a BitmapFontConfiguration with a FNT file */
		KDbool							initWithFNTfile ( const std::string& sFNTfile );
    
		inline const std::string&		getAtlasName ( KDvoid ) { return m_sAtlasName; }
		inline KDvoid					setAtlasName ( const std::string& sAtlasName ) { m_sAtlasName = sAtlasName; }
    
		std::set<KDuint>*				getCharacterSet ( KDvoid ) const;

	private :

		std::set<KDuint>*				parseConfigFile ( const std::string& sControlFile );
		KDvoid							parseCharacterDefinition ( std::string sLine, ccBMFontDef* pCharacterDefinition );
		KDvoid							parseInfoArguments ( std::string sLine );
		KDvoid							parseCommonArguments ( std::string sLine );
		KDvoid							parseImageFileName ( std::string sLine, const std::string& sFntFile );
		KDvoid							parseKerningEntry ( std::string sLine );
		KDvoid							purgeKerningDictionary ( KDvoid );
		KDvoid							purgeFontDefDictionary ( KDvoid );
};

/** 
 *	@brief LabelBMFont is a subclass of SpriteBatchNode.
 *
 *	Features:
 *		- Treats each character like a Sprite. This means that each individual character can be:
 *		- rotated
 *		- scaled
 *		- translated
 *		- tinted
 *		- change the opacity
 *		- It can be used as part of a menu item.
 *		- anchorPoint can be used to align the "label"
 *		- Supports AngelCode text format
 *
 *	Limitations:
 *		- All inner characters are using an anchorPoint of (0.5f, 0.5f) and it is not recommend to change it
 *		  because it might affect the rendering
 *
 *	LabelBMFont implements the protocol LabelProtocol, like Label and LabelAtlas.
 *	LabelBMFont has the flexibility of Label, the speed of LabelAtlas and all the features of Sprite.
 *	If in doubt, use LabelBMFont instead of LabelAtlas / Label.
 *
 *	Supported editors:
 *		http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
 *		http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
 *		http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
 *		http://www.angelcode.com/products/bmfont/ (Free, Windows only)
 *
 *	@since v0.8
 */

class CC_DLL LabelBMFont : public SpriteBatchNode, public LabelProtocol, public RGBAProtocol
{
	public :

		/**
		 *	@js ctor
		 */
		LabelBMFont ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~LabelBMFont ( KDvoid );

		/**
		 *	Purges the cached data.
		 *	Removes from memory the cached configurations and the atlas name dictionary.
		 *	@since v0.99.3
		 */
		static KDvoid				purgeCachedData ( KDvoid );

		/** creates a bitmap font atlas with an initial string and the FNT file */
		static LabelBMFont*			create ( const std::string& sStr, const std::string& sFntFile, KDfloat fWidth, TextHAlignment eAlignment, Point tImageOffset );
    
		static LabelBMFont*			create ( const std::string& sStr, const std::string& sFntFile, KDfloat fWidth, TextHAlignment eAlignment );

		static LabelBMFont*			create ( const std::string& sStr, const std::string& sFntFile, KDfloat fWidth);

		static LabelBMFont*			create ( const std::string& sStr, const std::string& sFntFile);

		/** 
		 *	Creates an label.
		 */
		static LabelBMFont*			create ( KDvoid );

		KDbool						init ( KDvoid );

		/** init a bitmap font atlas with an initial string and the FNT file */
		KDbool						initWithString ( const std::string& sStr, const std::string& sFntFile, KDfloat fWidth = kLabelAutomaticWidth, TextHAlignment eAlignment = TextHAlignment::LEFT, Point tImageOffset = Point::ZERO );

		/** updates the font chars based on the string to render */
		KDvoid						createFontChars();
		// super method
		virtual KDvoid				setString ( const std::string& sNewString ) override;
		virtual KDvoid				setString ( const std::string& sNewString, KDbool bNeedUpdateLabel );

		virtual const std::string&	getString ( KDvoid ) const override;
		virtual KDvoid				setCString ( const KDchar* pLabel );
		virtual KDvoid				setAnchorPoint ( const Point& tVar );
		virtual KDvoid				updateLabel ( KDvoid );
		virtual KDvoid				setAlignment ( TextHAlignment eAlignment );
		virtual KDvoid				setWidth ( KDfloat fWidth );
		virtual KDvoid				setLineBreakWithoutSpace ( KDbool bBreakWithoutSpace );
		virtual KDvoid				setScale  ( KDfloat fScale );
		virtual KDvoid				setScaleX ( KDfloat fScaleX );
		virtual KDvoid				setScaleY ( KDfloat fScaleY );
    
		// RGBAProtocol 
		virtual KDbool				 isOpacityModifyRGB ( KDvoid ) const;
		virtual KDvoid				setOpacityModifyRGB ( KDbool bIsOpacityModifyRGB );
		virtual GLubyte				getOpacity ( KDvoid ) const;
		virtual GLubyte				getDisplayedOpacity ( KDvoid ) const;
		virtual KDvoid				setOpacity ( GLubyte cOpacity );
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cParentOpacity );

		virtual KDbool				 isCascadeOpacityEnabled ( ) const;
		virtual KDvoid				setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled );
		virtual const Color3B&		getColor ( KDvoid ) const;
		virtual const Color3B&		getDisplayedColor ( KDvoid ) const;
		virtual KDvoid				setColor ( const Color3B& tColor );
		virtual KDvoid				updateDisplayedColor ( const Color3B& tParentColor );

		virtual KDbool				 isCascadeColorEnabled ( KDvoid ) const;
		virtual KDvoid				setCascadeColorEnabled ( KDbool bCascadeColorEnabled );

		KDvoid						setFntFile ( const KDchar* pFntFile );
		const KDchar*				getFntFile ( KDvoid );
	#if CC_LABELBMFONT_DEBUG_DRAW
		virtual KDvoid				draw ( KDvoid );
	#endif // CC_LABELBMFONT_DEBUG_DRAW

	private :

		KDchar*						atlasNameFromFntFile ( const std::string& sFntFile );
		KDint						kerningAmountForFirst ( KDushort uFirst, KDushort uSecond );
		KDfloat						getLetterPosXLeft( Sprite* pCharacterSprite );
		KDfloat						getLetterPosXRight( Sprite* pCharacterSprite );
    
	protected :

		virtual KDvoid				setString ( KDushort* pNewString, KDbool bNeedUpdateLabel );
		
		// string to render
		KDushort*					m_pString;
    
		// name of fntFile
		std::string					m_sFntFile;
    
		// initial string without line breaks
		KDushort*					m_pInitialString;
		std::string					m_sInitialStringUTF8;
    
		// alignment of all lines
		TextHAlignment				m_eAlignment;
		// max width until a line break is added
		KDfloat						m_fWidth;
    
		CCBMFontConfiguration*		m_pConfiguration;
    
		KDbool						m_bLineBreakWithoutSpaces;
		// offset of the texture atlas
		Point						m_tImageOffset;
    
		// reused char
		Sprite*						m_pReusedChar;
    
		// texture RGBA
		GLubyte						m_cDisplayedOpacity;
		GLubyte						m_cRealOpacity;
		Color3B						m_tDisplayedColor;
		Color3B						m_tRealColor;
		KDbool						m_bCascadeColorEnabled;
		KDbool						m_bCascadeOpacityEnabled;
		/** conforms to RGBAProtocol protocol */
		KDbool						m_bIsOpacityModifyRGB;
};

/**
 *	Free function that parses a FNT file a place it on the cache
 */
CC_DLL CCBMFontConfiguration*		FNTConfigLoadFile ( const std::string& sFile );

/** 
 *	Purges the FNT config cache
 */
CC_DLL KDvoid						FNTConfigRemoveCache( KDvoid );

//  end of GUI group
/// @}
/// @}

NS_CC_END

#endif	// __CCLabelBMFont_h__
