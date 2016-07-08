/* -----------------------------------------------------------------------------------
 *
 *      File            CCLabelBMFont.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCLabelBMFont_h__
#define __CCLabelBMFont_h__

#include "../sprite_nodes/CCSpriteBatchNode.h"
#include "../support/data_support/uthash.h"

#include <string>

NS_CC_BEGIN
    
/**
 * @addtogroup GUI
 * @{
 * @addtogroup label
 * @{
 */

enum 
{
    kCCLabelAutomaticWidth = -1,
};

struct _FontDefHashElement;

/**
 *	@struct ccBMFontDef
 *	BMFont definition
 */
typedef struct _BMFontDef 
{
	//! ID of the character
	KDuint     charID;

	//! origin and size of the font
	CCRect     rect;

	//! The X amount the image should be offset when drawing the image (in pixels)
	KDshort    xOffset;

	//! The Y amount the image should be offset when drawing the image (in pixels)
	KDshort    yOffset;

	//! The amount to move the current position after drawing the character (in pixels)
	KDshort    xAdvance;
} ccBMFontDef;

/** 
 *	@struct ccBMFontPadding
 *	BMFont padding
 *	@since	v0.8.2
 */
typedef struct _BMFontPadding 
{
	/// padding left
	KDint	   left;

	/// padding top
	KDint      top;

	/// padding right
	KDint      right;

	/// padding bottom
	KDint      bottom;
} ccBMFontPadding;

typedef struct _FontDefHashElement
{
	KDuint			key;		// key. Font Unicode value
	ccBMFontDef		fontDef;	// font definition
	UT_hash_handle	hh;
} tCCFontDefHashElement;

// Equal function for targetSet.
typedef struct _KerningHashElement
{
	KDint			key;		// key for the hash. 16-bit for 1st element, 16-bit for 2nd element
	KDint			amount;
	UT_hash_handle	hh;
} tCCKerningHashElement;

/** 
 *	@brief	CCBMFontConfiguration has parsed configuration of the the .fnt file
 *	@since	v0.8
 */
class CCBMFontConfiguration : public CCObject
{	
	// XXX: Creating a public interface so that the bitmapFontArray[] is accessible
	public :
		         CCBMFontConfiguration ( KDvoid );
		virtual ~CCBMFontConfiguration ( KDvoid );		

	public :

		/** allocates a CCBMFontConfiguration with a FNT file */
		static   CCBMFontConfiguration*		create ( const KDchar* szFntFile );

	public :
		
		const  KDchar*				description ( KDvoid );

		/** initializes a BitmapFontConfiguration with a FNT file */
		virtual KDbool				initWithFNTfile ( const KDchar* szFntFile );

		inline  const KDchar*		getAtlasName ( KDvoid ) { return m_sAtlasName.c_str ( ); }
		inline  KDvoid				setAtlasName ( const KDchar* szAtlasName ) { m_sAtlasName = szAtlasName; }

		std::set<KDuint>*			getCharacterSet ( KDvoid ) const;

	private :

		std::set<KDuint>*			parseConfigFile ( const KDchar* szControlFile );
		KDvoid						parseCharacterDefinition ( std::string sLine, ccBMFontDef* tBMFontDefinition );
		KDvoid						parseInfoArguments ( std::string sLine );
		KDvoid						parseCommonArguments ( std::string sLine );
		KDvoid						parseImageFileName ( std::string sLine, const KDchar* szFntFile );
		KDvoid						parseKerningEntry ( std::string sLine );
		KDvoid						purgeKerningDictionary ( KDvoid );
		KDvoid						purgeFontDefDictionary ( KDvoid );

	public :

		// BMFont definitions
		tCCFontDefHashElement*		m_pFontDefDictionary;

		//! FNTConfig: Common Height Should be signed (issue #1343)
		KDint						m_nCommonHeight;

		//! Padding
		ccBMFontPadding				m_tPadding;

		//! atlas name
		std::string					m_sAtlasName;

		//! values for kerning
		tCCKerningHashElement*		m_pKerningDictionary;

		// Character Set defines the letters that actually exist in the font
		std::set<KDuint>*			m_pCharacterSet;
};

/** 
 *	@brief CCLabelBMFont is a subclass of CCSpriteBatchNode.
 *
 *	Features:
 *	- Treats each character like a CCSprite. This means that each individual character can be:
 *	- rotated
 *	- scaled
 *	- translated
 *	- tinted
 *	- change the opacity
 *	- It can be used as part of a menu item.
 *	- anchorPoint can be used to align the "label"
 *	- Supports AngelCode text format
 *
 *	Limitations:
 *	- All inner characters are using an anchorPoint of (0.5f, 0.5f) and it is not recommend to change it
 *	because it might affect the rendering
 *
 *	CCLabelBMFont implements the protocol CCLabelProtocol, like CCLabel and CCLabelAtlas.
 *	CCLabelBMFont has the flexibility of CCLabel, the speed of CCLabelAtlas and all the features of CCSprite.
 *	If in doubt, use CCLabelBMFont instead of CCLabelAtlas / CCLabel.
 *
 *	Supported editors:
 *  http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
 *  http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
 *  http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
 *  http://www.angelcode.com/products/bmfont/ (Free, Windows only)
 *
 *	@since v0.8
 */
class CCLabelBMFont : public CCSpriteBatchNode, public CCLabelProtocol, public CCRGBAProtocol
{
	public :
		         CCLabelBMFont ( KDvoid );
		virtual ~CCLabelBMFont ( KDvoid );

		/** 
		 *	creates a bitmap font altas with an initial string and the FNT file 
		 */
		static   CCLabelBMFont*		create ( const KDchar* szText, const KDchar* szFntFile, KDfloat fWidth, CCTextAlignment eAlignment, CCPoint tImageOffset );
		static   CCLabelBMFont*		create ( const KDchar* szText, const KDchar* szFntFile, KDfloat fWidth, CCTextAlignment eAlignment );
		static   CCLabelBMFont*		create ( const KDchar* szText, const KDchar* szFntFile, KDfloat fWidth );
		static   CCLabelBMFont*		create ( const KDchar* szText, const KDchar* szFntFile );

		/** 
		 *	Creates an label.
		 */
		static   CCLabelBMFont*		create ( KDvoid );

		/** 
		 *	Purges the cached data.
		 *	Removes from memory the cached configurations and the atlas name dictionary.
		 *	@since	v0.99.3
		 */
		static KDvoid				purgeCachedData ( KDvoid );

	public :

		virtual KDbool				init ( KDvoid );

		/** init a bitmap font atlas with an initial string and the FNT file */
		virtual KDbool				initWithString ( const KDchar* szText, const KDchar* szFntFile, KDfloat fWidth = kCCLabelAutomaticWidth, CCTextAlignment eAlignment = kCCTextAlignmentLeft, CCPoint tImageOffset = CCPointZero );

		/** updates the font chars based on the string to render */
		KDvoid						createFontChars ( KDvoid );

		// super method

		virtual KDvoid				setString ( const KDchar* szNewString );
		virtual KDvoid				setString ( const KDchar* szNewString, KDbool bNeedUpdateLabel );

		virtual const KDchar*		getString ( KDvoid );

		virtual KDvoid				setCString ( const KDchar* szText );

		virtual KDvoid				setAnchorPoint ( const CCPoint& tAnchorPoint );

		virtual KDvoid				updateLabel ( KDvoid );

		virtual KDvoid				setAlignment ( CCTextAlignment eAlignment );

		virtual KDvoid				setWidth ( KDfloat fWidth );

		virtual KDvoid				setLineBreakWithoutSpace ( KDbool breakWithoutSpace );

		virtual KDvoid				setScale  ( KDfloat fScale );
		virtual KDvoid				setScaleX ( KDfloat fScaleX );
		virtual KDvoid				setScaleY ( KDfloat fScaleY );

		// CCRGBAProtocol

		virtual KDbool				 isOpacityModifyRGB ( KDvoid );
		virtual KDvoid				setOpacityModifyRGB ( KDbool bIsOpacityModifyRGB ); 

		virtual GLubyte				getOpacity ( KDvoid );
		virtual KDvoid				setOpacity ( GLubyte cOpacity );
		
		virtual GLubyte				getDisplayedOpacity ( KDvoid );
		virtual KDvoid				updateDisplayedOpacity ( GLubyte cParentOpacity );
		
		virtual KDbool				 isCascadeOpacityEnabled ( KDvoid );
		virtual KDvoid				setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled );

		virtual const ccColor3B&	getColor ( KDvoid );
		virtual const ccColor3B&	getDisplayedColor ( KDvoid );
		
		virtual KDvoid				setColor ( const ccColor3B& tColor );
		virtual KDvoid				updateDisplayedColor ( const ccColor3B& tParentColor );
		
		virtual KDbool				 isCascadeColorEnabled ( KDvoid );
		virtual KDvoid				setCascadeColorEnabled ( KDbool bCascadeColorEnabled );

		KDvoid						setFntFile ( const KDchar* szFntFile );
		const KDchar*				getFntFile ( KDvoid );

		CCBMFontConfiguration*		getConfiguration ( KDvoid ) const;

		virtual KDvoid				draw ( KDvoid );

	private :

		KDchar*						atlasNameFromFntFile ( const KDchar* szFntFile );
		
		KDint						kerningAmountForFirst ( KDuint16 uFirst, KDuint16 uSecond );

		KDfloat						getLetterPosXLeft  ( CCSprite* pCharacterSprite );
		KDfloat						getLetterPosXRight ( CCSprite* pCharacterSprite );

	protected :

		virtual KDvoid				setString ( KDushort* pNewString, KDbool bNeedUpdateLabel );

	protected :

		// string to render
		KDushort*					m_pString;

		// name of fntFile
		std::string					m_sFntFile;					
		
		// initial string without line breaks
		KDushort*					m_pInitialString;
		std::string					m_sInitialStringUTF8;		

		// alignment of all lines
		CCTextAlignment				m_eAlignment;				

		// max width until a line break is added
		KDfloat						m_fWidth;					

		CCBMFontConfiguration*		m_pConfiguration;
		
		KDbool						m_bLineBreakWithoutSpaces;

		// offset of the texture atlas
		CCPoint						m_tImageOffset;				

		// reused char
		CCSprite*					m_pReusedChar;

		// texture RGBA
		GLubyte						m_cDisplayedOpacity;
		GLubyte						m_cRealOpacity;
		ccColor3B					m_tDisplayedColor;
		ccColor3B					m_tRealColor;
		KDbool						m_bCascadeColorEnabled;
		KDbool						m_bCascadeOpacityEnabled;
		/** conforms to CCRGBAProtocol protocol */
		KDbool						m_bIsOpacityModifyRGB;
};

/** Free function that parses a FNT file a place it on the cache */
CCBMFontConfiguration*	FNTConfigLoadFile ( const KDchar* szFNTPath );

/** Purges the FNT config cache */
KDvoid	FNTConfigRemoveCache ( KDvoid );

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCLabelBMFont_h__
