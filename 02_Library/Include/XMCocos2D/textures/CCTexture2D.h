/* -----------------------------------------------------------------------------------
 *
 *      File            CCTexture2D.h
 *      Author          Young-Hwan Mun
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008      Apple Inc. All Rights Reserved.
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

#ifndef __CCTexture2D_h__
#define __CCTexture2D_h__

#include "../cocoa/CCObject.h"
#include "../cocoa/CCGeometry.h"
#include "../ccTypes.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN
    
/**
 * @addtogroup textures
 * @{
 */

class CCImage;

/**
 *	@typedef CCTexture2DPixelFormat
 *	Possible texture pixel formats
 */
typedef enum 
{
	kCCTexture2DPixelFormat_Automatic = 0,	

	//! 32-bit texture: RGBA8888
	kCCTexture2DPixelFormat_RGBA8888  = KD_IMAGE_FORMAT_RGBA8888_ATX,	

	//! 24-bit texture: RGBA888
	kCCTexture2DPixelFormat_RGB888    = KD_IMAGE_FORMAT_RGB888_ATX,

	//! 16-bit texture without Alpha channel
	kCCTexture2DPixelFormat_RGB565    = KD_IMAGE_FORMAT_RGB565_ATX,	

	//! 16-bit textures: RGBA4444
	kCCTexture2DPixelFormat_RGBA4444  = KD_IMAGE_FORMAT_RGBA4444_ATX,	

	//! 16-bit textures: RGB5A1
	kCCTexture2DPixelFormat_RGB5A1    = KD_IMAGE_FORMAT_RGBA5551_ATX,

	//! 8-bit textures used as masks
	kCCTexture2DPixelFormat_A8        = KD_IMAGE_FORMAT_ALPHA8_ATX,	

	//! 8-bit intensity texture
	kCCTexture2DPixelFormat_I8        = KD_IMAGE_FORMAT_LUM8_ATX,

	//! 16-bit textures used as masks
	kCCTexture2DPixelFormat_AI88      = KD_IMAGE_FORMAT_LUMALPHA88_ATX,

	//! 4-bit PVRTC-compressed texture: PVRTC
	kCCTexture2DPixelFormat_PVRTC     = KD_IMAGE_FORMAT_COMPRESSED_ATX,

	kCCTexture2DPixelFormat_Default   = kCCTexture2DPixelFormat_Automatic,
} CCTexture2DPixelFormat;

class CCGLProgram;

/**
 *	Extension to set the Min / Mag filter
 */
typedef struct _ccTexParams 
{
	GLuint    minFilter;
	GLuint    magFilter;
	GLuint    wrapS;
	GLuint    wrapT;
} ccTexParams;

/**
 *	@brief	CCTexture2D class.
 *	This class allows to easily create OpenGL 2D textures from images, text or raw data.
 *	The created CCTexture2D object will always have power-of-two dimensions. 
 *	Depending on how you create the CCTexture2D object, the actual image area of the texture might be smaller than the texture dimensions i.e. "contentSize" != (pixelsWide, pixelsHigh) and (maxS, maxT) != (1.0, 1.0).
 *	Be aware that the content of the generated textures will be upside-down!
 */
class CCTexture2D : public CCObject
#ifdef EMSCRIPTEN
, public CCGLBufferedNode
#endif // EMSCRIPTEN
{
	public :
		         CCTexture2D ( KDvoid );
		virtual ~CCTexture2D ( KDvoid );

		static CCTexture2D*		createWithImage ( CCImage* pImage );

	public :

		const KDchar*			description ( KDvoid );

		/** Initializes with a texture2d with data */
		virtual KDbool			initWithData ( const KDvoid* pData, CCTexture2DPixelFormat ePixelFormat, const CCSize& tContentSize, KDuint uMipmaps = 1 );

		/**
		 *	Extensions to make it easy to create a CCTexture2D object from an image file.
		 *	Note that RGBA type textures will have their alpha premultiplied - use the blending mode (GL_ONE, GL_ONE_MINUS_SRC_ALPHA).
		 */

		/** Initializes a texture from a CCImage object */
		virtual KDbool			initWithImage ( CCImage* pImage );

		/**
		 *  Extensions to make it easy to create a CCTexture2D object from a PVRTC file
		 *	Note that the generated textures don't have their alpha premultiplied - use the blending mode (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA).
		 */
		/** Initializes a texture from a PVRTC buffer */
		virtual KDbool			initWithPVRTCData ( const KDvoid* pData, KDint nLevel, KDint nBpp, KDbool bHasAlpha, KDuint uPOTWide, KDuint uPOTHigh );

		/** Initializes a texture from a string with font name and font size */
		virtual KDbool			initWithString ( const KDchar* szText, const KDchar* szFontPath, KDfloat fFontSize, const CCSize& tDimension, CCTextAlignment eAlignH, CCVerticalTextAlignment eAlignV );
		virtual KDbool			initWithString ( const KDchar* szText, const KDchar* szFontPath, KDfloat fFontSize, const CCSize& tDimension, CCAlignment eAlign );
			
		/** Initializes a texture from a string with dimensions, alignment, font name and font size */
        virtual KDbool			initWithString ( const KDchar* szText, const KDchar* szFontPath, KDfloat fFontSize );

		/** Initializes a texture from a string using a text definition*/
		virtual bool			initWithString ( const KDchar* szText, ccFontDefinition* pTextDefinition );

		/** These functions are needed to create mutable textures */
		KDvoid					releaseData ( KDvoid* data );

		KDvoid*					keepData ( KDvoid* data, KDuint length );

		/**
		 *	Drawing extensions to make it easy to draw basic quads using a CCTexture2D object.
		 *	These functions require GL_TEXTURE_2D and both GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY client states to be enabled.
		 */

		/** draws a texture at a given point */
		KDvoid					drawAtPoint ( const CCPoint& tPoint );		

		/** draws a texture inside a rect */
		KDvoid					drawInRect  ( const CCRect& tRect );

		/**
		 *	sets the min filter, mag filter, wrap s and wrap t texture parameters.
		 *	If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.
		 *
		 *	@warning Calling this method could allocate additional texture memory.
		 *	@since v0.8
		 */
		KDvoid					setTexParameters ( const ccTexParams& tTexParams );

		KDvoid					setTexParameters ( const ccTexParams* pTexParams );

		/** 
		 *	sets antialias texture parameters:
		 *		- GL_TEXTURE_MIN_FILTER = GL_LINEAR
		 *		- GL_TEXTURE_MAG_FILTER = GL_LINEAR
		 *
		 *	@warning Calling this method could allocate additional texture memory.
		 *	@since v0.8
		 */
		KDvoid					setAntiAliasTexParameters ( KDvoid );

		/** 
		 *	sets alias texture parameters:
		 *		- GL_TEXTURE_MIN_FILTER = GL_NEAREST
		 *		- GL_TEXTURE_MAG_FILTER = GL_NEAREST
		 *
		 *	@warning Calling this method could allocate additional texture memory.
		 *	@since v0.8
		 */
		KDvoid					setAliasTexParameters ( KDvoid );

		/** 
		 *	Generates mipmap images for the texture.
		 *	It only works if the texture size is POT (power of 2).
		 *	@since v0.99.0
		 */
		KDvoid					generateMipmap ( KDvoid );

		/** 
		 *	returns the pixel format.
		 *	@since v2.0
		 */
		const KDchar*			stringForFormat ( KDvoid );

		/** 
		 *	returns the bits-per-pixel of the in-memory OpenGL texture
		 *	@since v1.0
		 */
		KDuint					bitsPerPixelForFormat ( KDvoid );

		/** 
		 *	Helper functions that returns bits per pixels for a given format.
		 *	@since v2.0
		 */
		KDuint					bitsPerPixelForFormat ( CCTexture2DPixelFormat eFormat );

		KDbool					hasPremultipliedAlpha ( KDvoid );
		KDbool					hasMipmaps ( KDvoid );
		KDbool					hasFlipY ( KDvoid );

		/** content size */
		virtual	CCSize			getContentSize ( KDvoid );
		const   CCSize&			getContentSizeInPixels ( KDvoid );
				
	public :

		/** 
		 *	sets the default pixel format for UIImagescontains alpha channel.
		 *	If the UIImage contains alpha channel, then the options are:
		 *		- generate 32-bit textures: kCCTexture2DPixelFormat_RGBA8888 (default one)
		 *		- generate 24-bit textures: kCCTexture2DPixelFormat_RGB888
		 *		- generate 16-bit textures: kCCTexture2DPixelFormat_RGBA4444
		 *		- generate 16-bit textures: kCCTexture2DPixelFormat_RGB5A1
		 *		- generate 16-bit textures: kCCTexture2DPixelFormat_RGB565
		 *		- generate 8-bit textures: kCCTexture2DPixelFormat_A8 (only use it if you use just 1 color)
		 *
		 *	How does it work ?
		 *		- If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)
		 *		- If the image is an RGB (without Alpha) then: If the default pixel format is RGBA8888 then a RGBA8888 (32-bit) will be used. Otherwise a RGB565 (16-bit texture) will be used.
		 *
		 *	@since	v0.8
		 */
		static KDvoid			setDefaultAlphaPixelFormat ( CCTexture2DPixelFormat ePixelFormat );

		/**
		 *	returns the alpha pixel format
		 *	@since	v0.8
		 */
		static CCTexture2DPixelFormat	defaultAlphaPixelFormat ( KDvoid );

	public :

		/** pixel format of the texture */
        CC_PROPERTY_READONLY ( CCTexture2DPixelFormat, m_ePixelFormat, PixelFormat );

		/** width in pixels */
		CC_PROPERTY_READONLY ( KDuint, m_uPixelsWide, PixelsWide );

		/** height in pixels */
		CC_PROPERTY_READONLY ( KDuint, m_uPixelsHigh, PixelsHigh );

		/** texture name */
		CC_PROPERTY_READONLY ( GLuint, m_uName, Name );

		/** texture max S */
		CC_PROPERTY ( GLfloat, m_fMaxS, MaxS );

		/** texture max T */
		CC_PROPERTY ( GLfloat, m_fMaxT, MaxT );
    
		/** shader program used by drawAtPoint and drawInRect */
		CC_PROPERTY ( CCGLProgram*, m_pShaderProgram, ShaderProgram );

	protected :

		/** whether or not the texture has their Alpha premultiplied */
		KDbool					m_bHasPremultipliedAlpha;
		KDbool					m_bHasMipmaps;
		KDbool					m_bHasFlipY;
		CCSize					m_tContentSize;
};

// end of textures group
/// @}

NS_CC_END

#endif	// __CCTexture2D_h__

