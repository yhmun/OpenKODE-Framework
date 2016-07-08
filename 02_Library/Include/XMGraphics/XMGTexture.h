/*
 *
 *      File            XMGTexture.h
 *      Description     Texture class
 *      Version         0.20.0801, 2011-08-01
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2011 XMSoft. All rights reserved.
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

#ifndef __XMGTexture_h__
#define __XMGTexture_h__

typedef enum 
{
	XMG_FLIP_NULL           = 0x0000,
	XMG_FLIP_X              = 0x0001,
	XMG_FLIP_Y              = 0x0002,
	XMG_FLIP_ALL            = 0x0003,
} XMGFlip;

typedef enum
{
	XMG_FORMAT_UNKNOWN      = 0,
	XMG_FORMAT_RGBA8888     = KD_IMAGE_FORMAT_RGBA8888_ATX,
	XMG_FORMAT_RGBA888      = KD_IMAGE_FORMAT_RGB888_ATX,
	XMG_FORMAT_RGB565       = KD_IMAGE_FORMAT_RGB565_ATX,
	XMG_FORMAT_RGBA5551     = KD_IMAGE_FORMAT_RGBA5551_ATX,
	XMG_FORMAT_RGBA4444     = KD_IMAGE_FORMAT_RGBA4444_ATX,
	XMG_FORMAT_ALPHA8       = KD_IMAGE_FORMAT_ALPHA8_ATX,
	XMG_FORMAT_LUM8         = KD_IMAGE_FORMAT_LUM8_ATX,
	XMG_FORMAT_LUMALPHA88   = KD_IMAGE_FORMAT_LUMALPHA88_ATX,
} XMGFormat;

typedef enum 
{
	XMG_WRAP_REPEAT         = 0x2901,
	XMG_WRAP_CLAMP_TO_EDGE  = 0x812F,
} XMGWrap;

class XMGTexture
{
	public :

	 	 XMGTexture ( const KDchar* path, XMGFormat format = XMG_FORMAT_UNKNOWN, XMGFlip flip = XMG_FLIP_NULL );
		 XMGTexture ( GLsizei width, GLsizei height, XMGFormat format, GLint level = 1 );
		~XMGTexture ( void );
		
		void	SetPixels ( const GLvoid* pixels, GLint level = 0 );
		void	SetPixels ( const GLvoid* pixels, const XMGRect& offset, GLint level = 0 );

		void	SetWrap ( XMGWrap mode );

		void	GetID	  ( GLuint& id );
		void	GetFormat ( KDint& format );
		void	GetAlpha  ( GLsizei& bits ); 

		void	GetSize ( XMGVector2F& size );
		void	GetSize ( XMGVector2X& size );

		void	GetImageSize ( XMGVector2F& size  );
		void	GetImageSize ( XMGVector2X& size );

	protected :

		void*	m_impl;
};

#endif 



