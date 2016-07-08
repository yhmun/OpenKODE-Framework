/*
 *
 *      File            XMGRenderImpl.h
 *      Description     Render implemation 
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

#ifndef __XMGRenderImpl_h__
#define __XMGRenderImpl_h__
 
class XMGRenderImpl
{
	friend class XMGRender;

	private :
			
		 XMGRenderImpl ( GLuint num_shape, GLuint num_vertex, XMGGeoType type );
		 XMGRenderImpl ( GLuint num_shape, const GLuint* arr_num_vertex, XMGGeoType type );
		~XMGRenderImpl ( void );

		void	Init ( GLuint num_shape, const GLuint* arr_num_vertex, XMGGeoType type );

	public :
		
		void	Begin   ( void );
		void	Display ( GLuint idx_shape );
		void	End	    ( void );

		void	DisplayBegin ( GLuint idx_shape );
		void	DisplayEnd   ( GLuint idx_shape );

		void	Color ( GLuint idx_shape, GLuint idx_facet );

		void	RangeArray ( GLuint idx_array, GLuint num_array, GLuint& begin, GLuint& end );

		void	GenBuffer ( GLuint& id, GLsizeiptr size, GLenum target = GL_ARRAY_BUFFER );	
		void	SetBuffer ( GLuint& id, const GLvoid* data, GLintptr offset, GLsizeiptr size, GLenum target = GL_ARRAY_BUFFER );

		void	SetRegular  ( const XMGVector3* arr_vertex, XMGShape* shape );
		void	SetSolid	( const XMGVector3* arr_vertex, XMGShape* shape );
		void	SetPillar   ( const XMGVector3* arr_vertex, XMGShape* shape );
		void	SetWall		( const XMGVector3* arr_vertex, XMGShape* shape );
		void	SetBSolid	( const XMGVector3* arr_vertex, XMGShape* shape );
		void	SetBPillar	( const XMGVector3* arr_vertex, XMGShape* shape );
		void	SetBWall	( const XMGVector3* arr_vertex, XMGShape* shape );

		XMGGeoType		m_geo_type;

		XMGVector3		m_position;
		XMGMatrix4		m_matrix;

		VECShape		m_vec_shape;
		GLuint			m_num_vertex;
		GLuint			m_ext_vertex;

		XMGType			m_type_position;
		XMGType			m_type_matrix;
		XMGType			m_type_vertex;
		XMGType			m_type_colors;
		XMGType			m_type_coords[2];		

		GLuint			m_id_index;	
		GLuint			m_id_vertex;			
		GLuint			m_id_colors;
		GLuint			m_id_coords[2];

		XMGBool			m_has_face[3];
};

#endif 
