/* -----------------------------------------------------------------------------------
 *
 *      File            ccTypes.h
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

#ifndef __ccTypes_h__
#define __ccTypes_h__

#include "cocoa/CCGeometry.h"

NS_CC_BEGIN
     
#pragma pack ( push, 4 )

/**
 *	RGB color composed of bytes 3 bytes
 *	@since	v0.8
 */
typedef struct _ccColor3B
{
	GLubyte	     r;
	GLubyte	     g;
	GLubyte      b;
} ccColor3B;

//! helper macro that creates an ccColor3B type
static inline ccColor3B ccc3 ( const GLubyte r, const GLubyte g, const GLubyte b )
{
	ccColor3B    c = { r, g, b };

	return c;
}

/** returns true if both ccColor3B are equal. Otherwise it returns false.
 */
static inline KDbool ccc3BEqual ( const ccColor3B& col1, const ccColor3B& col2 )
{
    return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b;
}

//ccColor3B predefined colors
//! White color (255,255,255)
static const ccColor3B  ccWHITE   = { 255, 255, 255 };

//! Yellow color (255,255,0)
static const ccColor3B  ccYELLOW  = { 255, 255,   0 };

//! Blue color (0,0,255)
static const ccColor3B  ccBLUE    = {   0,   0, 255 };

//! Green Color (0,255,0)
static const ccColor3B  ccGREEN   = {   0, 255,   0 };

//! Red Color (255,0,0)
static const ccColor3B  ccRED     = { 255,   0,   0 };

//! Magenta Color (255,0,255)
static const ccColor3B  ccMAGENTA = { 255,   0, 255 };

//! Black Color (0,0,0)
static const ccColor3B  ccBLACK   = {   0,   0,   0 };

//! Orange Color (255,127,0)
static const ccColor3B  ccORANGE  = { 255, 127,   0 };

//! Gray Color (166,166,166)
static const ccColor3B  ccGRAY    = { 166, 166, 166 };

/**
 *	RGBA color composed of 4 bytes
 *	@since	v0.8
 */
typedef struct _ccColor4B
{
	GLubyte      r;
	GLubyte      g;
	GLubyte      b;
	GLubyte      a;
} ccColor4B;

//! helper macro that creates an ccColor4B type
static inline ccColor4B  ccc4 ( const GLubyte r, const GLubyte g, const GLubyte b, const GLubyte a = 255 )
{
	ccColor4B    c = { r, g, b, a };

	return c;
}

static inline ccColor4B  ccc4 ( ccColor3B c )
{
	ccColor4B    c4 = { c.r, c.g, c.b, 255 };

	return c4;
}

/** 
 *	RGBA color composed of 4 floats
 *	@since	v0.8
 */
typedef struct _ccColor4F 
{
	GLfloat      r;
	GLfloat      g;
	GLfloat      b;
	GLfloat      a;
} ccColor4F;

//! helper that creates a ccColor4F type
static inline  ccColor4F ccc4F ( const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a = 1.0f )
{
	ccColor4F    c4 = { r, g, b, a };

	return c4;
}

#define ccc4f	ccc4F

/**
 *	Returns a ccColor4F from a ccColor3B. Alpha will be 1.
 *	@since	v0.99.1
 */
static inline  ccColor4F ccc4FFromccc3B ( ccColor3B c )
{
	ccColor4F    c4 = { c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0f };

	return c4;
}

/** 
 *	Returns a ccColor4F from a ccColor4B.
 *	@since	v0.99.1
 */
static inline  ccColor4F ccc4FFromccc4B ( ccColor4B c )
{
	ccColor4F    c4 = { c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f };

	return c4;
}

static inline ccColor4B ccc4BFromccc4F ( ccColor4F c )
{
    ccColor4B   c4b = { (GLubyte) ( c.r * 255 ), (GLubyte) ( c.g * 255 ), (GLubyte) ( c.b * 255 ), (GLubyte) ( c.a * 255 ) };
	return c4b;
}

/**
 *	returns YES if both ccColor4F are equal. Otherwise it returns NO.
 *	@since	v0.99.1
 */
static inline KDbool  ccc4FEqual ( ccColor4F a, ccColor4F b )
{
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a ? KD_TRUE : KD_FALSE;
}


/**
 *	A vertex composed of 2 floats: x, y
 *	@since	v0.8
 */
typedef struct _ccVertex2F
{
	GLfloat      x;
	GLfloat      y;
} ccVertex2F;

static inline ccVertex2F  vertex2 ( const GLfloat x, const GLfloat y )
{
	ccVertex2F   c = { x, y };

	return c;
}

/**
 *	A vertex composed of 2 floats: x, y
 *	@since v0.8
 */
typedef struct _ccVertex3F
{
	GLfloat      x;
	GLfloat      y;
	GLfloat      z;
} ccVertex3F;

static inline ccVertex3F  vertex3 ( const GLfloat x, const GLfloat y, const GLfloat z )
{
	ccVertex3F   c = { x, y, z };

	return c;
}
		
/**
 *	A texcoord composed of 2 floats: u, y
 *	@since v0.8
 */
typedef struct _ccTex2F 
{
	 GLfloat     u;
	 GLfloat     v;
} ccTex2F;

static inline ccTex2F  tex2 ( const GLfloat u, const GLfloat v )
{
	ccTex2F      t = { u , v };

	return t;
}

//! A 2D grid size
typedef struct _ccGridSize
{
	GLint        x;
	GLint        y;
} ccGridSize;

//! helper function to create a ccGridSize
static inline ccGridSize ccg ( const GLint x, const GLint y )
{
	ccGridSize   v = { x, y };

	return v;
}

//! Point Sprite component
typedef struct _ccPointSprite
{
	ccVertex2F   pos;       // 8 bytes
	ccColor4B	 color;		// 4 bytes
	GLfloat      size;      // 4 bytes
} ccPointSprite;

//!	A 2D Quad. 4 * 2 floats
typedef struct _ccQuad2 
{
	ccVertex2F   tl;
	ccVertex2F   tr;
	ccVertex2F   bl;
	ccVertex2F   br;
} ccQuad2;

//!	A 3D Quad. 4 * 3 floats
typedef struct _ccQuad3
{
	ccVertex3F   bl;
	ccVertex3F   br;
	ccVertex3F   tl;
	ccVertex3F   tr;
} ccQuad3;

//! a Point with a vertex point, a tex coord point and a color 4B
typedef struct _ccV2F_C4B_T2F
{
	//! vertices (2F)
	ccVertex2F		vertices;

	//! colors (4B)
	ccColor4B		colors;

	//! tex coords (2F)
	ccTex2F			texCoords;
} ccV2F_C4B_T2F;

//! a Point with a vertex point, a tex coord point and a color 4F
typedef struct _ccV2F_C4F_T2F
{	
	//! vertices (2F)
	ccVertex2F      vertices;	

	//! colors (4F)
	ccColor4F       colors;	

	//! tex coords (2F)
	ccTex2F         texCoords;
} ccV2F_C4F_T2F;

//! a Point with a vertex point, a tex coord point and a color 4B
typedef struct _ccV3F_C4B_T2F
{
	//! vertices (3F)
	ccVertex3F      vertices;		// 12 bytes
	
	//! colors (4B)
	ccColor4B       colors;			// 4 bytes

	// tex coords (2F)
	ccTex2F         texCoords;		// 8 bytes
} ccV3F_C4B_T2F;

//! A Triangle of ccV2F_C4B_T2F
typedef struct _ccV2F_C4B_T2F_Triangle
{
	//! Point A
	ccV2F_C4B_T2F	a;
	//! Point B
	ccV2F_C4B_T2F	b;
	//! Point B
	ccV2F_C4B_T2F	c;
} ccV2F_C4B_T2F_Triangle;

//! 4 ccVertex2FTex2FColor4B Quad
typedef struct _ccV2F_C4B_T2F_Quad
{
	//! bottom left
	ccV2F_C4B_T2F	bl;

	//! bottom right
	ccV2F_C4B_T2F	br;

	//! top left
	ccV2F_C4B_T2F	tl;

	//! top right
	ccV2F_C4B_T2F	tr;
} ccV2F_C4B_T2F_Quad;

//! 4 ccVertex3FTex2FColor4B
typedef struct _ccV3F_C4B_T2F_Quad
{
	//! top left
	ccV3F_C4B_T2F	tl;

	//! bottom left
	ccV3F_C4B_T2F	bl;

	//! top right
	ccV3F_C4B_T2F	tr;

	//! bottom right
	ccV3F_C4B_T2F	br;
} ccV3F_C4B_T2F_Quad;

//! 4 ccVertex2FTex2FColor4F Quad
typedef struct _ccV2F_C4F_T2F_Quad
{
	//! bottom left
	ccV2F_C4F_T2F	bl;

	//! bottom right
	ccV2F_C4F_T2F	br;

	//! top left
	ccV2F_C4F_T2F	tl;

	//! top right
	ccV2F_C4F_T2F	tr;
} ccV2F_C4F_T2F_Quad;

//! Blend Function used for textures
typedef struct _ccBlendFunc
{
	//! source blend function
	GLenum     src;

	//! destination blend function
	GLenum     dst;
} ccBlendFunc;

static const ccBlendFunc  kCCBlendFuncDisable = { GL_ONE, GL_ZERO };

// XXX: If any of these enums are edited and/or reordered, udpate CCTexture2D.m
//! Vertical text alignment type
typedef enum
{
    kCCVerticalTextAlignmentTop		= 0x10,
    kCCVerticalTextAlignmentCenter	= 0x00,
    kCCVerticalTextAlignmentBottom	= 0x20,
} CCVerticalTextAlignment;

// XXX: If any of these enums are edited and/or reordered, update CCTexture2D.m
//! Horizontal text alignment type
typedef enum
{
    kCCTextAlignmentLeft			= 0x01,
    kCCTextAlignmentCenter			= 0x00,
    kCCTextAlignmentRight			= 0x02,
} CCTextAlignment;

typedef enum
{
	kCCAlignmentCenter				= 0x00,											///< Horizontal center and vertical center.
	kCCAlignmentLeft				= 0x01,											///< Horizontal left and vertical center.			
	kCCAlignmentRight				= 0x02,											///< Horizontal right and vertical center.
	kCCAlignmentTop					= 0x10,											///< Horizontal center and vertical top.
	kCCAlignmentBottom				= 0x20,											///< Horizontal center and vertical bottom.

	kCCAlignmentTopLeft				= kCCAlignmentTop | kCCAlignmentLeft,			///< Horizontal left and vertical top.
	kCCAlignmentTopRight			= kCCAlignmentTop | kCCAlignmentRight,			///< Horizontal right and vertical top.
	kCCAlignmentBottomLeft			= kCCAlignmentBottom | kCCAlignmentLeft,		///< Horizontal left and vertical bottom.
	kCCAlignmentBottomRight			= kCCAlignmentBottom | kCCAlignmentRight,		///< Horizontal right and vertical bottom.
} CCAlignment;

// types for animation in particle systems

// texture coordinates for a quad
typedef struct _ccT2F_Quad
{
	//! bottom left
	ccTex2F			bl;
	//! bottom right
	ccTex2F			br;
	//! top left
	ccTex2F			tl;
	//! top right
	ccTex2F			tr;
} ccT2F_Quad;

// struct that holds the size in pixels, texture coordinates and delays for animated CCParticleSystemQuad
typedef struct
{
	ccT2F_Quad		texCoords;
	KDfloat			delay;
	CCSize			size; 
} ccAnimationFrameData;

/**
 types used for defining fonts properties (i.e. font name, size, stroke or shadow)
 */

// shadow attributes
typedef struct _ccFontShadow
{
	public :
    
		// shadow is not enabled by default
		_ccFontShadow ( KDvoid ) : m_bShadowEnabled ( KD_FALSE ) { }
	    
		// true if shadow enabled
		KDbool		m_bShadowEnabled;
		// shadow x and y offset
		CCSize		m_tShadowOffset;
		// shadow blurrines
		KDfloat		m_fShadowBlur;
		// shadow opacity
		KDfloat		m_fShadowOpacity;
    
} ccFontShadow;

// stroke attributes
typedef struct _ccFontStroke
{
	public :
	    
		// stroke is disabled by default
		_ccFontStroke ( KDvoid ) : m_bStrokeEnabled ( KD_FALSE ) { }
	    
		// true if stroke enabled
		KDbool		m_bStrokeEnabled;
		// stroke color
		ccColor3B   m_tStrokeColor;
		// stroke size
		KDfloat		m_fStrokeSize;
    
} ccFontStroke;

// font attributes
/**
 *  @js NA
 *  @lua NA
 */
typedef struct _ccFontDefinition
{
	public :
	    
		_ccFontDefinition ( KDvoid ) :
			m_eAlignment ( kCCTextAlignmentCenter ),
			m_eVertAlignment ( kCCVerticalTextAlignmentTop ),
			m_tFontFillColor ( ccWHITE )
		{
			m_tDimensions = CCSizeMake ( 0, 0 ); 
		}
	    
		// font name
		std::string             m_sFontName;
		// font size
		KDint                   m_nFontSize;
		// horizontal alignment
		CCTextAlignment         m_eAlignment;
		// vertical alignment
		CCVerticalTextAlignment m_eVertAlignment;
		// rendering box
		CCSize                  m_tDimensions;
		// font color
		ccColor3B               m_tFontFillColor;
		// font shadow
		ccFontShadow            m_tShadow;
		// font stroke
		ccFontStroke            m_tStroke;
	    
} ccFontDefinition;

#pragma pack(pop)

NS_CC_END

#endif // __ccTypes_h__