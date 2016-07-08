/* -----------------------------------------------------------------------------------
 *
 *      File            CCTexture2D.h
 *      Author          Young-Hwan Mun
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCTexture2D_h__
#define __CCTexture2D_h__

#include <string>
#include <map>

#include "../../base/CCObject.h"
#include "../../base/CCGeometry.h"
#include "../../ccTypes.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

class Image;
typedef struct _MipmapInfo MipmapInfo;

/**
 *	@addtogroup textures
 *	@{
 */

//CONSTANTS:

class GLProgram;

//CLASS INTERFACES:

/** 
 *	@brief Texture2D class.
 *	This class allows to easily create OpenGL 2D textures from images, text or raw data.
 *	The created Texture2D object will always have power-of-two dimensions.
 *	Depending on how you create the Texture2D object, the actual image area of the texture might be smaller than the texture dimensions i.e. "contentSize" != (pixelsWide, pixelsHigh) and (maxS, maxT) != (1.0, 1.0).
 *	Be aware that the content of the generated textures will be upside-down!
 */
class CC_DLL Texture2D : public Object
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
	public :

		/**
		 *	@typedef Texture2D::PixelFormat
		 *	Possible texture pixel formats
		 */
		enum class PixelFormat
		{
			//! auto detect the type
			AUTO										= 0,
			//! 32-bit texture: BGRA8888
			BGRA8888									= KD_IMAGE_FORMAT_BGRA8888_ATX,
			//! 32-bit texture: RGBA8888
			RGBA8888									= KD_IMAGE_FORMAT_RGBA8888_ATX,
			//! 24-bit texture: RGBA888
			RGB888										= KD_IMAGE_FORMAT_RGB888_ATX,
			//! 16-bit texture without Alpha channel
			RGB565										= KD_IMAGE_FORMAT_RGB565_ATX,
			//! 8-bit textures used as masks
			A8											= KD_IMAGE_FORMAT_ALPHA8_ATX,
			//! 8-bit intensity texture
			I8											= KD_IMAGE_FORMAT_LUM8_ATX,
			//! 16-bit textures used as masks
			AI88										= KD_IMAGE_FORMAT_LUMALPHA88_ATX,
			//! 16-bit textures: RGBA4444
			RGBA4444									= KD_IMAGE_FORMAT_RGBA4444_ATX,
			//! 16-bit textures: RGB5A1
			RGB5A1										= KD_IMAGE_FORMAT_RGBA5551_ATX,
			//! 4-bit PVRTC-compressed texture: PVRTC4
			PVRTC4										= KD_IMAGE_FORMAT_PVRTC4_ATX,
			//! 4-bit PVRTC-compressed texture: PVRTC4 (has alpha channel)
			PVRTC4A										= KD_IMAGE_FORMAT_PVRTC4_ATX,
			//! 2-bit PVRTC-compressed texture: PVRTC2
			PVRTC2										= KD_IMAGE_FORMAT_PVRTC2_ATX,
			//! 2-bit PVRTC-compressed texture: PVRTC2 (has alpha channel)
			PVRTC2A										= KD_IMAGE_FORMAT_PVRTC2_ATX,		
			//! ETC-compressed texture: ETC
			ETC											= KD_IMAGE_FORMAT_ETC1_ATX,
			//! S3TC-compressed texture: S3TC_Dxt1
			S3TC_DXT1									= KD_IMAGE_FORMAT_DXT1_ATX,
			//! S3TC-compressed texture: S3TC_Dxt3
			S3TC_DXT3									= KD_IMAGE_FORMAT_DXT3_ATX,
			//! S3TC-compressed texture: S3TC_Dxt5
			S3TC_DXT5									= KD_IMAGE_FORMAT_DXT5_ATX,
			//! ATITC-compressed texture: ATC_RGB
			ATC_RGB										= KD_IMAGE_FORMAT_ATC_RGB,
			//! ATITC-compressed texture: ATC_EXPLICIT_ALPHA
			ATC_EXPLICIT_ALPHA							= KD_IMAGE_FORMAT_ATC_RGBA_EXPLICIT_ALPHA_AMD,
			//! ATITC-compresed texture: ATC_INTERPOLATED_ALPHA
			ATC_INTERPOLATED_ALPHA						= KD_IMAGE_FORMAT_ATC_RGBA_INTERPOLATED_ALPHA_AMD,
			COMPRESSED									= KD_IMAGE_FORMAT_COMPRESSED_ATX,
			//! Default texture format: AUTO
			DEFAULT										= AUTO,        
			NONE										= -1
		};
        
		struct PixelFormatInfo 
		{
			PixelFormatInfo ( GLenum eInternalFormat, GLenum eFormat, GLenum eType, KDint nBpp, KDbool bCompressed, KDbool bAlpha )
			: internalFormat ( eInternalFormat )
			, format ( eFormat )
			, type ( eType )
			, bpp ( nBpp )
			, compressed ( bCompressed )
			, alpha ( bAlpha )
			{ }

			GLenum			internalFormat;
			GLenum			format;
			GLenum			type;
			KDint			bpp;
			KDbool			compressed;
			KDbool			alpha;
		};
    
		typedef std::map<Texture2D::PixelFormat, const PixelFormatInfo>		PixelFormatInfoMap;
    
		/**
		 *	Extension to set the Min / Mag filter
		 */
		typedef struct _TexParams 
		{
			GLuint		minFilter;
			GLuint		magFilter;
			GLuint		wrapS;
			GLuint		wrapT;
		} TexParams;
    
	public :

		/**
		 *	sets the default pixel format for UIImagescontains alpha channel.
		 *	If the UIImage contains alpha channel, then the options are:
		 *		- generate 32-bit textures: Texture2D::PixelFormat::RGBA8888 (default one)
		 *		- generate 24-bit textures: Texture2D::PixelFormat::RGB888
		 *		- generate 16-bit textures: Texture2D::PixelFormat::RGBA4444
		 *		- generate 16-bit textures: Texture2D::PixelFormat::RGB5A1
		 *		- generate 16-bit textures: Texture2D::PixelFormat::RGB565
		 *		- generate  8-bit textures: Texture2D::PixelFormat::A8 (only use it if you use just 1 color)
		 *
		 *	How does it work ?
		 *		- If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)
		 *		- If the image is an RGB (without Alpha) then: If the default pixel format is RGBA8888 then a RGBA8888 (32-bit) will be used. Otherwise a RGB565 (16-bit texture) will be used.
		 *
		 *	This parameter is not valid for PVR / PVR.CCZ images.
		 *
		 *	@since v0.8
		 */
		static KDvoid						setDefaultAlphaPixelFormat ( Texture2D::PixelFormat eFormat );

		/**
		 *	returns the alpha pixel format
		 *	@since v0.8
		 */
		static Texture2D::PixelFormat		getDefaultAlphaPixelFormat ( KDvoid );
	
		/** 
		 *	treats (or not) PVR files as if they have alpha premultiplied.
		 *	Since it is impossible to know at runtime if the PVR images have the alpha channel premultiplied, it is
		 *	possible load them as if they have (or not) the alpha channel premultiplied.
		 *
		 *	By default it is disabled.
		 *
		 *	@since v0.99.5
		 */
		static KDvoid						PVRImagesHavePremultipliedAlpha ( KDbool bHaveAlphaPremultiplied );
    		 
	public :

		static Texture2D*					createWithImage ( Image* pImage );

		/**
		 *	@js ctor
		 */
		Texture2D ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Texture2D ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*						description ( KDvoid ) const;

		/**
		 *	These functions are needed to create mutable textures
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid								releaseData ( KDvoid* pData );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid*								keepData ( KDvoid* pData, KDuint uLength );

		/** 
		 *	Initializes with a texture2d with data 
		 *	@js NA
		 *	@lua NA
		 */
		KDbool								initWithData ( const KDvoid* pData, KDint32 nDataLen, Texture2D::PixelFormat ePixelFormat, KDint32 nPixelsWide, KDint32 nPixelsHigh, const Size& tContentSize );

		/** Initializes with mipmaps */
		KDbool								initWithMipmaps ( MipmapInfo* pMipmaps, KDint nMipmapsNum, Texture2D::PixelFormat ePixelFormat, KDint32 nPixelsWide, KDint32 nPixelsHigh );

		/**
		 *	Drawing extensions to make it easy to draw basic quads using a Texture2D object.
		 *	These functions require GL_TEXTURE_2D and both GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY client states to be enabled.
		 */
		
		/** draws a texture at a given point */
		KDvoid								drawAtPoint ( const Point& tPoint );
		
		/** draws a texture inside a rect */
		KDvoid								drawInRect ( const Rect& tRect );

		/**
		 *	Extensions to make it easy to create a Texture2D object from an image file.
		 */
		
		/** 
		 *	Initializes a texture from a UIImage object.
		 *	We will use the format you specified with setDefaultAlphaPixelFormat to convert the image for texture.
		 *	NOTE: It will not convert the pvr image file.
		 */
		KDbool								initWithImage ( Image* pImage );
    
		/** 
		 *	Initializes a texture from a UIImage object.
		 *	we will use the format you passed to the function to convert the image format to the texture format.
		 *	If you pass PixelFormat::Automatic, we will auto detect the image render type and use that type for texture to render.
		 **/
		KDbool								initWithImage ( Image* pImage, PixelFormat eFormat );

		/** Initializes a texture from a string with dimensions, alignment, font name and font size */
		KDbool								initWithString ( const KDchar* pText, const KDchar* pFontName, KDfloat fFontSize, const Size& tDimensions = Size ( 0, 0 ), TextHAlignment hAlignment = TextHAlignment::CENTER, TextVAlignment vAlignment = TextVAlignment::TOP );

		/** Initializes a texture from a string using a text definition*/
		KDbool								initWithString ( const KDchar* pText, const FontDefinition& textDefinition);

		/**
		 *	sets the min filter, mag filter, wrap s and wrap t texture parameters.
		 *	If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.
		 *
		 *	@warning Calling this method could allocate additional texture memory.
		 *
		 *	@since v0.8
		 *	@code 
		 *	When this function bound into js or lua,the input parameter will be changed
		 *	In js: var setBlendFunc(var arg1, var arg2, var arg3, var arg4)
		 *	In lua: local setBlendFunc(local arg1, local arg2, local arg3, local arg4)
		 *	@endcode
		 */
		KDvoid								setTexParameters ( const TexParams& texParams );

		/** 
		 *	sets antialias texture parameters:
		 *		- GL_TEXTURE_MIN_FILTER = GL_LINEAR
		 *		- GL_TEXTURE_MAG_FILTER = GL_LINEAR
		 *
		 *	@warning Calling this method could allocate additional texture memory.
		 *
		 *	@since v0.8
		 */
		KDvoid								setAntiAliasTexParameters ( KDvoid );

		/** 
		 *	sets alias texture parameters:
		 *		- GL_TEXTURE_MIN_FILTER = GL_NEAREST
		 *		- GL_TEXTURE_MAG_FILTER = GL_NEAREST
		 *
		 *	@warning Calling this method could allocate additional texture memory.
		 *
		 *	@since v0.8
		 */
		KDvoid								setAliasTexParameters ( KDvoid );

		/**
		 *	Generates mipmap images for the texture.
		 *	It only works if the texture size is POT (power of 2).
		 *	@since v0.99.0
		 */
		KDvoid								generateMipmap ( KDvoid );

		/**
		 *	returns the pixel format.
		 *	@since v2.0
		 */
		const KDchar*						getStringForFormat ( KDvoid ) const;

		/**
		 *	returns the bits-per-pixel of the in-memory OpenGL texture
		 *	@since v1.0
		 */
		KDuint								getBitsPerPixelForFormat ( KDvoid ) const;

		/**
		 *	Helper functions that returns bits per pixels for a given format.
		 *	@since v2.0
		 */
		KDuint								getBitsPerPixelForFormat ( Texture2D::PixelFormat eFormat ) const;

		/** content size */
		const Size&							getContentSizeInPixels ( KDvoid );

		KDbool								hasPremultipliedAlpha ( KDvoid ) const;
		KDbool								hasMipmaps ( KDvoid ) const;

		/** Gets the pixel format of the texture */
		Texture2D::PixelFormat				getPixelFormat ( KDvoid ) const;
    
		/** Gets the width of the texture in pixels */
		KDint32								getPixelsWide ( KDvoid ) const;
    
		/** Gets the height of the texture in pixels */
		KDint32								getPixelsHigh ( KDvoid ) const;
    
		/** Gets the texture name */
		GLuint								getName ( KDvoid ) const;
    
		/** Gets max S */
		GLfloat								getMaxS ( KDvoid ) const;
		/** Sets max S */
		KDvoid								setMaxS ( GLfloat fMaxS );
    
		/** Gets max T */
		GLfloat								getMaxT ( KDvoid ) const;
		/** Sets max T */
		KDvoid								setMaxT ( GLfloat fMaxT );
    
		Size								getContentSize ( KDvoid ) const;
    
		KDvoid								setShaderProgram ( GLProgram* pProgram );
		GLProgram*							getShaderProgram ( KDvoid ) const;
    
	public :

		static const PixelFormatInfoMap&	getPixelFormatInfoMap ( KDvoid );
    
	private :

		/** convert functions */

		/**
		 *	Convert the format to the format param you specified, if the format is PixelFormat::Automatic, it will detect it automatically and convert to the closest format for you.
		 *	It will return the converted format to you. if the outData != data, you must delete it manually.
		 */
		static PixelFormat					convertDataToFormat			( const KDubyte* pData, KDint32 nDataLen, PixelFormat originFormat, PixelFormat format, KDubyte** pOutData, KDint* pOutDataLen );

		static PixelFormat					convertI8ToFormat			( const KDubyte* pData, KDint32 nDataLen, PixelFormat format, KDubyte** pOutData, KDint* pOutDataLen );
		static PixelFormat					convertAI88ToFormat			( const KDubyte* pData, KDint32 nDataLen, PixelFormat format, KDubyte** pOutData, KDint* pOutDataLen );
		static PixelFormat					convertRGB888ToFormat		( const KDubyte* pData, KDint32 nDataLen, PixelFormat format, KDubyte** pOutData, KDint* pOutDataLen );
		static PixelFormat					convertRGBA8888ToFormat		( const KDubyte* pData, KDint32 nDataLen, PixelFormat format, KDubyte** pOutData, KDint* pOutDataLen );

		// I8 to XXX
		static KDvoid						convertI8ToRGB888			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertI8ToRGBA8888			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertI8ToRGB565			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertI8ToRGBA4444			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertI8ToRGB5A1			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertI8ToAI88				( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );

		// AI88 to XXX
		static KDvoid						convertAI88ToRGB888			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertAI88ToRGBA8888		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertAI88ToRGB565			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertAI88ToRGBA4444		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertAI88ToRGB5A1			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertAI88ToA8				( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertAI88ToI8				( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );

		// RGB888 to XXX
		static KDvoid						convertRGB888ToRGBA8888		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGB888ToRGB565		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGB888ToI8			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGB888ToAI88			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGB888ToRGBA4444		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGB888ToRGB5A1		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );

		// RGBA8888 to XXX
		static KDvoid						convertRGBA8888ToRGB888		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGBA8888ToRGB565		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGBA8888ToI8			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGBA8888ToA8			( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGBA8888ToAI88		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGBA8888ToRGBA4444	( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );
		static KDvoid						convertRGBA8888ToRGB5A1		( const KDubyte* pData, KDint32 nDataLen, KDubyte* pOutData );

	protected :

		/** pixel format of the texture */
		Texture2D::PixelFormat				m_ePixelFormat;

		/** width in pixels */
		KDint32								m_nPixelsWide;

		/** height in pixels */
		KDint32								m_nPixelsHigh;

		/** texture name */
		GLuint								m_uName;

		/** texture max S */
		GLfloat								m_fMaxS;
    
		/** texture max T */
		GLfloat								m_fMaxT;

		/** content size */
		Size								m_tContentSize;

		/** whether or not the texture has their Alpha premultiplied */
		KDbool								m_bHasPremultipliedAlpha;

		KDbool								m_bHasMipmaps;

		/** shader program used by drawAtPoint and drawInRect */
		GLProgram*							m_pShaderProgram;

		static const PixelFormatInfoMap		m_aPixelFormatInfoTables;
};

//	end of textures group
/// @}

NS_CC_END

#endif	// __CCTexture2D_h__

