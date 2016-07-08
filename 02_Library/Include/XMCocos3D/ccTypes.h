/* -----------------------------------------------------------------------------------
 *
 *      File            ccTypes.h
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

#ifndef __ccTypes_h__
#define __ccTypes_h__

#include <string>
#include "base/CCGeometry.h"
#include "platform/CCGL.h"

NS_CC_BEGIN

struct Color4B;
struct Color4F;

/** 
 *	RGB color composed of bytes 3 bytes
 *	@since v3.0
 */
struct Color3B
{
    Color3B ( KDvoid );
    Color3B ( GLubyte _r, GLubyte _g, GLubyte _b );

    explicit Color3B ( const Color4B& tColor );
    explicit Color3B ( const Color4F& tColor );

    KDbool	operator== ( const Color3B& tRight ) const;
    KDbool	operator== ( const Color4B& tRight ) const;
    KDbool	operator== ( const Color4F& tRight ) const;
    KDbool	operator!= ( const Color3B& tRight ) const;
    KDbool	operator!= ( const Color4B& tRight ) const;
    KDbool	operator!= ( const Color4F& tRight ) const;

    KDbool equals ( const Color3B& tOther )
    {
        return ( *this == tOther );
    }

    GLubyte			r;
    GLubyte			g;
    GLubyte			b;
    
    const static Color3B	WHITE;
    const static Color3B	YELLOW;
    const static Color3B	BLUE;
    const static Color3B	GREEN;
    const static Color3B	RED;
    const static Color3B	MAGENTA;
    const static Color3B	BLACK;
    const static Color3B	ORANGE;
    const static Color3B	GRAY;
};

/** 
 *	RGBA color composed of 4 bytes
 *	@since v3.0
 */
struct Color4B
{
    Color4B ( KDvoid );
    Color4B ( GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a );

    explicit Color4B ( const Color3B& tColor );
    explicit Color4B ( const Color4F& tColor );

    KDbool	operator== ( const Color4B& tRight ) const;
    KDbool	operator== ( const Color3B& tRight ) const;
    KDbool	operator== ( const Color4F& tRight ) const;
    KDbool	operator!= ( const Color4B& tRight ) const;
    KDbool	operator!= ( const Color3B& tRight ) const;
    KDbool	operator!= ( const Color4F& tRight ) const;

    GLubyte			r;
    GLubyte			g;
    GLubyte			b;
    GLubyte			a;

    const static Color4B	WHITE;
    const static Color4B	YELLOW;
    const static Color4B	BLUE;
    const static Color4B	GREEN;
    const static Color4B	RED;
    const static Color4B	MAGENTA;
    const static Color4B	BLACK;
    const static Color4B	ORANGE;
    const static Color4B	GRAY;
};


/**
 *	RGBA color composed of 4 floats
 *	@since v3.0
 */
struct Color4F
{
    Color4F ( KDvoid );
    Color4F ( KDfloat _r, KDfloat _g, KDfloat _b, KDfloat _a );

    explicit Color4F ( const Color3B& tColor );
    explicit Color4F ( const Color4B& tColor );

    KDbool	operator== ( const Color4F& tRight ) const;
    KDbool	operator== ( const Color3B& tRight ) const;
    KDbool	operator== ( const Color4B& tRight ) const;
    KDbool	operator!= ( const Color4F& tRight ) const;
    KDbool	operator!= ( const Color3B& tRight ) const;
    KDbool	operator!= ( const Color4B& tRight ) const;

    KDbool	equals(const Color4F &other)
    {
        return ( *this == other );
    }
    
    GLfloat			r;
    GLfloat			g;
    GLfloat			b;
    GLfloat			a;

    const static Color4F	WHITE;
    const static Color4F	YELLOW;
    const static Color4F	BLUE;
    const static Color4F	GREEN;
    const static Color4F	RED;
    const static Color4F	MAGENTA;
    const static Color4F	BLACK;
    const static Color4F	ORANGE;
    const static Color4F	GRAY;
};

/** 
 *	A vertex composed of 2 floats: x, y
 *	@since v3.0
 */
struct Vertex2F
{
    Vertex2F ( KDfloat _x, KDfloat _y ) : x ( _x ), y ( _y ) { }
    
    Vertex2F ( KDvoid ) : x ( 0.f ), y ( 0.f ) { }
    
    GLfloat			x;
    GLfloat			y;
};


/** 
 *	A vertex composed of 2 floats: x, y
 *	@since v3.0
 */
struct Vertex3F
{
    Vertex3F ( KDfloat _x, KDfloat _y, KDfloat _z )
    : x ( _x )
    , y ( _y )
    , z ( _z )
    {

	}
    
    Vertex3F ( KDvoid ) : x ( 0.f ), y ( 0.f ), z ( 0.f ) { }
    
    GLfloat			x;
    GLfloat			y;
    GLfloat			z;
};
        
/**
 *	A texcoord composed of 2 floats: u, y
 *	@since v3.0
 */
struct Tex2F 
{
    Tex2F ( KDfloat _u, KDfloat _v ) : u ( _u ), v ( _v ) { }
    
    Tex2F ( KDvoid ) : u ( 0.f ), v ( 0.f ) { }
    
    GLfloat			u;
    GLfloat			v;
};

 
//! Point Sprite component
struct PointSprite
{
    Vertex2F		pos;        // 8 bytes
    Color4B			color;      // 4 bytes
    GLfloat			size;       // 4 bytes
};

//!    A 2D Quad. 4 * 2 floats
struct Quad2
{
    Vertex2F        tl;
    Vertex2F        tr;
    Vertex2F        bl;
    Vertex2F        br;
};


//!    A 3D Quad. 4 * 3 floats
struct Quad3 
{
    Vertex3F        bl;
    Vertex3F        br;
    Vertex3F        tl;
    Vertex3F        tr;
};

//! a Point with a vertex point, a tex coord point and a color 4B
struct V2F_C4B_T2F
{
    //! vertices (2F)
    Vertex2F		vertices;
    //! colors (4B)
    Color4B			colors;
    //! tex coords (2F)
    Tex2F			texCoords;
};

//! a Point with a vertex point, a tex coord point and a color 4F
struct V2F_C4F_T2F
{
    //! vertices (2F)
    Vertex2F		vertices;
    //! colors (4F)
    Color4F			colors;
    //! tex coords (2F)
    Tex2F			texCoords;
};

//! a Point with a vertex point, a tex coord point and a color 4B
struct V3F_C4B_T2F
{
    //! vertices (3F)
    Vertex3F		vertices;            // 12 bytes

    //! colors (4B)
    Color4B			colors;              // 4 bytes

    // tex coords (2F)
    Tex2F			texCoords;           // 8 bytes
};

//! A Triangle of V2F_C4B_T2F
struct V2F_C4B_T2F_Triangle
{
	//! Point A
	V2F_C4B_T2F		a;
	//! Point B
	V2F_C4B_T2F		b;
	//! Point B
	V2F_C4B_T2F		c;
};

//! A Quad of V2F_C4B_T2F
struct V2F_C4B_T2F_Quad
{
    //! bottom left
    V2F_C4B_T2F		bl;
    //! bottom right
    V2F_C4B_T2F		br;
    //! top left
    V2F_C4B_T2F		tl;
    //! top right
    V2F_C4B_T2F		tr;
};

//! 4 Vertex3FTex2FColor4B
struct V3F_C4B_T2F_Quad
{
    //! top left
    V3F_C4B_T2F		tl;
    //! bottom left
    V3F_C4B_T2F		bl;
    //! top right
    V3F_C4B_T2F		tr;
    //! bottom right
    V3F_C4B_T2F		br;
};

//! 4 Vertex2FTex2FColor4F Quad
struct V2F_C4F_T2F_Quad
{
    //! bottom left
    V2F_C4F_T2F		bl;
    //! bottom right
    V2F_C4F_T2F		br;
    //! top left
    V2F_C4F_T2F		tl;
    //! top right
    V2F_C4F_T2F		tr;
};

//! Blend Function used for textures
struct BlendFunc
{
    //! source blend function
    GLenum			src;
    //! destination blend function
    GLenum			dst;

    //! Blending disabled. Uses {GL_ONE, GL_ZERO}
    const static BlendFunc	DISABLE;
    //! Blending enabled for textures with Alpha premultiplied. Uses {GL_ONE, GL_ONE_MINUS_SRC_ALPHA}
    const static BlendFunc	ALPHA_PREMULTIPLIED;
    //! Blending enabled for textures with Alpha NON premultiplied. Uses {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
    const static BlendFunc	ALPHA_NON_PREMULTIPLIED;
    //! Enables Additive blending. Uses {GL_SRC_ALPHA, GL_ONE}
    const static BlendFunc	ADDITIVE;
};

// Label::VAlignment
// Label::HAlignment

// XXX: If any of these enums are edited and/or reordered, update Texture2D.m
//! Vertical text alignment type
enum class TextVAlignment
{
    TOP,
    CENTER,
    BOTTOM,
};

// XXX: If any of these enums are edited and/or reordered, update Texture2D.m
//! Horizontal text alignment type
enum class TextHAlignment
{
    LEFT,
    CENTER,
    RIGHT,
};

// types for animation in particle systems

// texture coordinates for a quad
struct T2F_Quad
{
    //! bottom left
    Tex2F			bl;
    //! bottom right
    Tex2F			br;
    //! top left
    Tex2F			tl;
    //! top right
    Tex2F			tr;
};

// struct that holds the size in pixels, texture coordinates and delays for animated ParticleSystemQuad
struct AnimationFrameData
{
	T2F_Quad		texCoords;
	KDfloat			delay;
	Size			size; 
};

/**
 types used for defining fonts properties (i.e. font name, size, stroke or shadow)
 */


// shadow attributes
struct FontShadow
{
	public :
    
		// shadow is not enabled by default
		FontShadow ( KDvoid )
		: m_bShadowEnabled ( false )
		, m_fShadowBlur ( 0 )
		, m_fShadowOpacity ( 0 )
		{

		}

		// true if shadow enabled
		KDbool					m_bShadowEnabled;
		// shadow x and y offset
		Size					m_tShadowOffset;
		// shadow blurrines
		KDfloat					m_fShadowBlur;
		// shadow opacity
		KDfloat					m_fShadowOpacity;
};

// stroke attributes
struct FontStroke
{
	public :
    
		// stroke is disabled by default
		FontStroke ( KDvoid )
		: m_bStrokeEnabled ( false )
		, m_tStrokeColor ( Color3B::BLACK )
		, m_fStrokeSize ( 0 )
		{

		}
    
		// true if stroke enabled
		KDbool					m_bStrokeEnabled;
		// stroke color
		Color3B					m_tStrokeColor;
		// stroke size
		KDfloat					m_fStrokeSize;
    
};

// font attributes
struct FontDefinition
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		FontDefinition ( KDvoid )
			: m_nFontSize ( 0 )
			, m_eAlignment ( TextHAlignment::CENTER )
			, m_eVertAlignment ( TextVAlignment::TOP )
    		, m_tDimensions ( Size::ZERO )
			, m_tFontFillColor ( Color3B::WHITE )
		{

		}
    
		// font name
		std::string				m_sFontName;
		// font size
		KDint					m_nFontSize;
		// horizontal alignment
		TextHAlignment			m_eAlignment;
		// vertical alignment
		TextVAlignment			m_eVertAlignment;
		// renering box
		Size					m_tDimensions;
		// font color
		Color3B					m_tFontFillColor;
		// font shadow
		FontShadow				m_tShadow;
		// font stroke
		FontStroke				m_tStroke;
    
};

/**
 *	@brief The device accelerometer reports values for each axis in units of g-force
 */
class Acceleration
{
	public :

		KDdouble				x;
		KDdouble				y;
		KDdouble				z;
    
		KDdouble				timestamp;
    
		Acceleration ( KDvoid ) : x ( 0 ), y ( 0 ), z ( 0 ), timestamp ( 0 ) { }
};

NS_CC_END

#endif // __ccTypes_h__
