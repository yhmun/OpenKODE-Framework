/*
 *
 *      File            XMGPolygon.h
 *      Description     Polygon class
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

#ifndef __XMGPolygon_h__
#define __XMGPolygon_h__

class XMGPolygon : public XMGRender
{
	public :

		 XMGPolygon ( GLuint num_shape, const GLuint* arr_num_vertex, XMGGeoType mode = XMG_GEO_REGULAR );
		~XMGPolygon ( void );

		void	SetVertexArray ( const XMGVector3F* arr_vertex, const GLuint* arr_idx_hole = 0 );
		void	SetVertexArray ( const XMGVector3X* arr_vertex, const GLuint* arr_idx_hole = 0 );

		void	SetColorArray  ( const XMGColorF* arr_color, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetColorArray  ( const XMGColorX* arr_color, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );

		void	SetCoordArray  ( const XMGVector2F* arr_coord, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );
		void	SetCoordArray  ( const XMGVector2X* arr_coord, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );

		void	SetTexture	   ( const XMGVector2* arr_coord, const XMGTexture* texture, XMGTexUnit unit = XMG_TEX_UNIT_0, GLuint idx_shape = XMG_SHAPE_ALL, GLuint idx_facet = XMG_FACET_ALL );

	private :

		void*	m_impl;
};

#endif 