/*
 *
 *      File            XMGTexture.cpp
 *      Description     Texture implemation 
 *      Version         0.20.0801, 2011-08-01
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2011 XMSoft. All rights reserved.
 * 
 *      Contact Email: chris@xmsoft.co.kr 
 *                     xmsoft77@gmail.com	
 * 
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 */

#include "XMGLibrary.h"

XMGTextureImpl::XMGTextureImpl ( void )
{
	m_id		 = 0;
	m_alpha_bit  = 0;
	m_wrap		 = XMG_WRAP_REPEAT;

	glGenTextures ( 1, &m_id );
	glBindTexture ( GL_TEXTURE_2D, m_id );

	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap );
	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap );	
}

XMGTextureImpl::~XMGTextureImpl ( void )
{
	glDeleteTextures ( 1, &m_id );
}

KDboolean XMGTextureImpl::CheckExtension( const KDchar* ext )
{
	KDchar*		gl_ext  = (KDchar *) glGetString ( GL_EXTENSIONS );	
	KDsize		len_str = kdStrlen ( ext );
	KDsize		len_ext = kdStrlen ( gl_ext );
	KDsize		i;

	for ( i = 0; i < len_ext; i++ )
	{
		if ( !kdStrncmp ( gl_ext + i , ext, len_str ) )
		{
			return KD_TRUE;
		}
		else
		{
			for ( i++; i < len_ext; i++ )
			{
				if ( gl_ext[i] == ' ' )
				{					
					break;
				}
			}
		}
	}

	return KD_FALSE;
}

void XMGTextureImpl::ATX2GLFormat( KDint atx_format, GLenum & gl_format, GLenum & gl_type, GLsizei & alpha_bit )
{
	switch ( atx_format )
	{
		case KD_IMAGE_FORMAT_RGBA8888_ATX	:	gl_format = GL_RGBA;			gl_type = GL_UNSIGNED_BYTE;				alpha_bit = 8;	break;
		case KD_IMAGE_FORMAT_RGB888_ATX		:	gl_format = GL_RGB;				gl_type = GL_UNSIGNED_BYTE;				alpha_bit = 0;	break;
		case KD_IMAGE_FORMAT_RGB565_ATX		:	gl_format = GL_RGB;				gl_type = GL_UNSIGNED_SHORT_5_6_5;		alpha_bit = 0;	break;
		case KD_IMAGE_FORMAT_RGBA5551_ATX	:	gl_format = GL_RGBA;			gl_type = GL_UNSIGNED_SHORT_5_5_5_1;	alpha_bit = 1;	break;
		case KD_IMAGE_FORMAT_RGBA4444_ATX	:	gl_format = GL_RGBA;			gl_type = GL_UNSIGNED_SHORT_4_4_4_4;	alpha_bit = 4;	break;
		case KD_IMAGE_FORMAT_ALPHA8_ATX		:	gl_format = GL_ALPHA;			gl_type = GL_UNSIGNED_BYTE;				alpha_bit = 8;	break;
		case KD_IMAGE_FORMAT_LUM8_ATX		:	gl_format = GL_LUMINANCE;		gl_type = GL_UNSIGNED_BYTE;				alpha_bit = 0;	break;
		case KD_IMAGE_FORMAT_LUMALPHA88_ATX :	gl_format = GL_LUMINANCE_ALPHA;	gl_type = GL_UNSIGNED_BYTE;				alpha_bit = 8;	break; 
	}
}

XMGTexture::XMGTexture ( const KDchar * path, XMGFormat format, XMGFlip flip ) 
{
	XMGTextureImpl*		impl = 0;

	KDImageATX	image	   = 0;

	KDint		img_width  = 0;
	KDint		img_height = 0;
	KDint		img_alpha  = 0;
	KDint		img_bpp	   = 0;
	KDint		img_format = 0;
	KDint		img_level  = 0;
	KDint		img_offset = 0;
	KDint		img_size   = 0;

	KDuint8*	img_pixels = 0;

	GLenum		tex_format = 0;
	GLenum		tex_type   = 0;
	GLsizei		tex_width  = 1;
	GLsizei		tex_height = 1;

	KDint		idx_level  = 0;

	XMGAssert ( m_impl = new XMGTextureImpl ( ) );
	impl = (XMGTextureImpl *) m_impl;

	XMGAssert ( image = kdGetImageInfoATX ( path ) );

	img_width	= kdGetImageIntATX ( image, KD_IMAGE_WIDTH_ATX		  );	
	img_height	= kdGetImageIntATX ( image, KD_IMAGE_HEIGHT_ATX		  );
	img_format	= kdGetImageIntATX ( image, KD_IMAGE_FORMAT_ATX		  );	
	img_bpp		= kdGetImageIntATX ( image, KD_IMAGE_BITSPERPIXEL_ATX );	
	img_alpha	= kdGetImageIntATX ( image, KD_IMAGE_ALPHA_ATX		  );	
	img_level   = kdGetImageIntATX ( image, KD_IMAGE_LEVELS_ATX		  );

	kdFreeImageATX ( image );

	if ( format == XMG_FORMAT_UNKNOWN )
	{		
		switch ( img_format )
		{
			case KD_IMAGE_FORMAT_COMPRESSED_ATX :

				if ( !impl->CheckExtension ( "GL_IMG_texture_compression_pvrtc" ) )
				{
					img_format	= KD_IMAGE_FORMAT_RGBA8888_ATX;
					tex_format	= GL_RGBA;
					tex_type	= GL_UNSIGNED_BYTE;
				}

				break;

			case KD_IMAGE_FORMAT_LUMINANCE_ATX :

				img_format	= KD_IMAGE_FORMAT_LUM8_ATX;
				tex_format	= GL_LUMINANCE;
				tex_type	= GL_UNSIGNED_BYTE;

				break;

			case KD_IMAGE_FORMAT_LUMALPHA_ATX :

				img_format	= KD_IMAGE_FORMAT_LUMALPHA88_ATX;
				tex_format	= GL_LUMINANCE_ALPHA;
				tex_type	= GL_UNSIGNED_BYTE;

				break;

			case KD_IMAGE_FORMAT_PALETTED_ATX :

				img_format  = KD_IMAGE_FORMAT_RGB565_ATX;
				tex_format	= GL_RGB;
				tex_type	= GL_UNSIGNED_SHORT_5_6_5;	

				break;

			case KD_IMAGE_FORMAT_RGB_ATX  : 

				switch ( img_bpp )
				{
					case 24 :

						img_format	= KD_IMAGE_FORMAT_RGB888_ATX;
						tex_format	= GL_RGB;
						tex_type	= GL_UNSIGNED_BYTE;

						break;

					case 16 :

						img_format	= KD_IMAGE_FORMAT_RGB565_ATX;
						tex_format	= GL_RGB;
						tex_type	= GL_UNSIGNED_SHORT_5_6_5;

						break;
				}

				break;

			case KD_IMAGE_FORMAT_RGBA_ATX :

				switch ( img_bpp )
				{
					case 64 :
					case 32 :

						img_format	= KD_IMAGE_FORMAT_RGBA8888_ATX;
						tex_format	= GL_RGBA;
						tex_type	= GL_UNSIGNED_BYTE;

						break;

					case 16 :		

						switch ( img_alpha )
						{
							case 1 :

								img_format	= KD_IMAGE_FORMAT_RGBA5551_ATX;
								tex_format	= GL_RGBA;
								tex_type	= GL_UNSIGNED_SHORT_5_5_5_1;

								break;

							case 4 :

								img_format	= KD_IMAGE_FORMAT_RGBA4444_ATX;
								tex_format	= GL_RGBA;
								tex_type	= GL_UNSIGNED_SHORT_4_4_4_4;

								break;
						}

						break;		
				}

				break;

			default :

				XMGAssert ( XMG_FALSE );
		}
	}
	else
	{
		img_format = format;
		impl->ATX2GLFormat ( img_format, tex_format, tex_type, img_alpha );
	}
	
	XMGAssert ( image = kdGetImageATX ( path, img_format, 0 ) );

	if ( img_format == KD_IMAGE_FORMAT_COMPRESSED_ATX )
	{
		switch ( kdGetImageIntATX ( image, KD_IMAGE_FORMAT_ATX ) )
		{
			case KD_IMAGE_FORMAT_PVRTC2_ATX : tex_format = img_alpha ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG; break;
			case KD_IMAGE_FORMAT_PVRTC4_ATX : tex_format = img_alpha ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG; break;
		}
	}

	img_pixels = (KDuint8 *) kdGetImagePointerATX ( image, KD_IMAGE_POINTER_BUFFER_ATX );
	img_bpp = kdGetImageIntATX ( image, KD_IMAGE_BITSPERPIXEL_ATX );

	while ( tex_width  < img_width  ) tex_width  = tex_width  << 1;
	while ( tex_height < img_height ) tex_height = tex_height << 1;

	impl->m_size_tex.m_x = tex_width;
	impl->m_size_tex.m_y = tex_height;
	impl->m_size_img.m_x = img_width;
	impl->m_size_img.m_y = img_height;
	impl->m_alpha_bit    = kdGetImageIntATX ( image, KD_IMAGE_ALPHA_ATX );

	for ( idx_level = 0; idx_level <= img_level; idx_level++ )
	{
		img_width  = kdGetImageLevelIntATX ( image, KD_IMAGE_WIDTH_ATX		 , idx_level );
		img_height = kdGetImageLevelIntATX ( image, KD_IMAGE_HEIGHT_ATX		 , idx_level );
		img_offset = kdGetImageLevelIntATX ( image, KD_IMAGE_BUFFEROFFSET_ATX, idx_level );
		img_size   = kdGetImageLevelIntATX ( image, KD_IMAGE_DATASIZE_ATX	 , idx_level );

		if ( img_format == KD_IMAGE_FORMAT_COMPRESSED_ATX )
		{
			glCompressedTexImage2D ( GL_TEXTURE_2D, idx_level, tex_format, img_width, img_height, 0, img_size, (GLvoid *) ( img_pixels + img_offset ) );
		}
		else
		{
			tex_width  = 1;
			tex_height = 1;

			while ( tex_width  < img_width  ) tex_width  = tex_width  << 1;
			while ( tex_height < img_height ) tex_height = tex_height << 1;

			KDubyte*  clear = ( tex_width == img_width && tex_height == img_height ) ? KD_NULL : (KDubyte *) kdCalloc ( tex_width * tex_height * ( img_bpp >> 3 ), 1 );

			glTexImage2D ( GL_TEXTURE_2D, idx_level, tex_format, tex_width, tex_height, 0, tex_format, tex_type, (GLvoid*) clear );
			glTexSubImage2D ( GL_TEXTURE_2D, idx_level, 0, 0, img_width, img_height, tex_format, tex_type, (GLvoid *) ( img_pixels + img_offset ) );	 							

			if ( clear )
			{
				kdFree ( clear );
			}
		}
	}

	if ( img_level > 0 )
	{
		glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST );
		glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, impl->m_wrap );
		glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, impl->m_wrap );
	}
	
	kdFreeImageATX ( image );
	glBindTexture  ( GL_TEXTURE_2D, 0 );	
}

XMGTexture::XMGTexture ( GLsizei width, GLsizei height, XMGFormat format, GLint level ) 
{
	XMGTextureImpl*		impl = 0;

	GLenum		tex_format = 0;
	GLenum		tex_type   = 0;
	GLsizei		tex_width  = 1;
	GLsizei		tex_height = 1;
	GLsizei		alpha_bit  = 0;
	GLint		idx_level  = 0;

	XMGAssert ( m_impl = new XMGTextureImpl ( ) );
	impl = (XMGTextureImpl *) m_impl;

	impl->ATX2GLFormat ( format, tex_format, tex_type, alpha_bit );

	while ( tex_width  < width  ) tex_width  = tex_width  << 1;
	while ( tex_height < height ) tex_height = tex_height << 1;

	impl->m_format       = format;
	impl->m_size_tex.m_x = tex_width;
	impl->m_size_tex.m_y = tex_height;
	impl->m_size_img.m_x = width;
	impl->m_size_img.m_y = height;
	impl->m_alpha_bit    = alpha_bit;

	for ( idx_level = 0; idx_level < level; idx_level++ )
	{
		glTexImage2D ( GL_TEXTURE_2D, idx_level, tex_format, tex_width, tex_height, 0, tex_format, tex_type, 0 );	
	}
	
	glBindTexture ( GL_TEXTURE_2D, 0 );	
}

XMGTexture::~XMGTexture ( void )
{
	delete (XMGTextureImpl *) m_impl;	
}

void XMGTexture::SetPixels ( const GLvoid* pixels, GLint level )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	SetPixels ( pixels, XMGRect ( 0, 0, impl->m_size_img.m_x, impl->m_size_img.m_y ), level );
}

void XMGTexture::SetPixels ( const GLvoid* pixels, const XMGRect& offset, GLint level )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	GLenum		format	  = 0;
	GLenum		type	  = 0;
	GLsizei		alpha_bit = 0;
	
	impl->ATX2GLFormat ( impl->m_format, format, type, alpha_bit );

	glBindTexture   ( GL_TEXTURE_2D, impl->m_id );
	glTexSubImage2D ( GL_TEXTURE_2D, level, offset.m_x, offset.m_y, offset.m_w, offset.m_h, format, type, pixels );
	glBindTexture   ( GL_TEXTURE_2D, 0 );	
}

void XMGTexture::SetWrap ( XMGWrap mode )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	impl->m_wrap = (GLfloat) mode;

	glBindTexture   ( GL_TEXTURE_2D, impl->m_id );

	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, impl->m_wrap );
	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, impl->m_wrap );	

	glBindTexture   ( GL_TEXTURE_2D, 0 );	
}

void XMGTexture::GetID ( GLuint& id )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	id = impl->m_id;
}

void XMGTexture::GetFormat ( KDint& format )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	format = impl->m_format;
}

void XMGTexture::GetAlpha ( GLsizei& bits )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	bits = impl->m_alpha_bit;
}

void XMGTexture::GetSize ( XMGVector2F& size )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	size.m_x = XMG_I2F ( impl->m_size_tex.m_x );
	size.m_y = XMG_I2F ( impl->m_size_tex.m_y );
}

void XMGTexture::GetSize ( XMGVector2X& size )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	size.m_x = XMG_I2X ( impl->m_size_tex.m_x );
	size.m_y = XMG_I2X ( impl->m_size_tex.m_y );
}

void XMGTexture::GetImageSize ( XMGVector2F& size )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	size.m_x = XMG_I2F ( impl->m_size_img.m_x );
	size.m_y = XMG_I2F ( impl->m_size_img.m_y );
}

void XMGTexture::GetImageSize ( XMGVector2X& size )
{
	XMGTextureImpl*		impl = (XMGTextureImpl *) m_impl;

	size.m_x = XMG_I2X ( impl->m_size_img.m_x );
	size.m_y = XMG_I2X ( impl->m_size_img.m_y );
}
