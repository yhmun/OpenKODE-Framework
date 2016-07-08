/*
 *
 *      File            TextureImpl.h
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

#ifndef __TextureImpl_h__
#define __TextureImpl_h__

class XMGTextureImpl
{	
	friend class XMGTexture;

	private :

		 XMGTextureImpl ( void );
		~XMGTextureImpl ( void );	

		KDboolean	CheckExtension( const KDchar* ext);
		void		ATX2GLFormat  ( KDint atx_format, GLenum& gl_format, GLenum& gl_type, GLsizei& alpha_bit );

		GLuint		m_id;	
		KDint		m_format;
		XMGVector2	m_size_tex;
		XMGVector2	m_size_img;
		GLsizei		m_alpha_bit;
		GLfloat		m_wrap;
};

#endif 
