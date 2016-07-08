/*
 *
 *      File            XMGQuad.h
 *      Description     Quadrilateral class
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

#ifndef __XMGQuad_h__
#define __XMGQuad_h__

typedef enum 
{
	XMG_ROTATE_0			= 0,
	XMG_ROTATE_90			= 90,
	XMG_ROTATE_180			= 180,
	XMG_ROTATE_270			= 270,
} XMGRotate;

class XMGQuad : public XMGRender
{
	public :

		 XMGQuad ( GLuint num_shape = 1, XMGGeoType type = XMG_GEO_REGULAR );
		~XMGQuad ( void );

		void	SetVertexArray ( const XMGRectF* arr_rect, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetVertexArray ( const XMGRectX* arr_rect, GLuint idx_shape = XMG_SHAPE_ALL );
		void	SetVertexArray ( const XMGTexture* texture, GLuint idx_shape = XMG_SHAPE_ALL );

		void	SetTexture ( const XMGTexture* texture, XMGFlip flip = XMG_FLIP_NULL, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetTexture ( const XMGTexture* texture, const XMGRectF& offset, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetTexture ( const XMGTexture* texture, const XMGRectX& offset, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );

		void	TexIndices ( XMGRotate rot );

	protected :

		void*	m_impl;
};

#endif 



