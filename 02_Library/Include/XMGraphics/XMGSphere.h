/*
 *
 *      File            XMGSphere.h
 *      Description     Sphere class
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

#ifndef __XMGSphere_h__
#define __XMGSphere_h__

class XMGSphere : public XMGRender
{
	public :

		 XMGSphere ( GLuint num_shape = 1, GLsizei slices = 48, GLsizei stacks = 48 );
		~XMGSphere ( void );

		void	SetVertexArray ( const GLfloat* radius, const XMGVector3F* pos = 0, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetVertexArray ( const GLfixed* radius, const XMGVector3X* pos = 0, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetTexture ( const XMGTexture* texture, XMGFlip flip = XMG_FLIP_NULL, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetTexture ( const XMGTexture* texture, const XMGRectF& offset, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetTexture ( const XMGTexture* texture, const XMGRectX& offset, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL );

	protected :

		void*	m_impl;
};

#endif 



