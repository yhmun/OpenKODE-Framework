/*
 *
 *      File            XMGShape.cpp
 *      Description     Shape implemation 
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
/*
XMGSubShape::XMGSubShape ( void )
{	
	m_disp_mode  = 0;
	m_num_vertex = 0;
	m_off_vertex = 0;
}

XMGSubShape::~XMGSubShape ( void )
{

}

XMGShape::XMGShape ( void )
{
	m_type_color	 = XMG_TYPE_FIXED;
	m_visible		 = XMG_TRUE;
	m_tex_combine    = XMG_COMBINE_MODULATE;

	m_id_textures[0] = 0;
	m_id_textures[1] = 0;

	m_num_sub_shape  = 0;
	m_arr_sub_shape  = 0;

	m_num_vertex	 = 0;
	m_off_vertex	 = 0;
}

XMGShape::~XMGShape ( void )
{
	if ( m_arr_sub_shape )
	{
		delete[] m_arr_sub_shape;
	}
}
*/
/*
XMGContour::XMGContour ( void ) 
{
	m_prim_line		   = XMG_PRIM_LINES;
	m_prim_tri		   = XMG_PRIM_TRIANGLES;
	m_mode			   = XMG_MODE_FILL;
	m_cull			   = XMG_CULL_BACK;
	m_wind			   = XMG_WIND_ANTI_CLOCK;

	m_blend			   = XMG_FALSE;
	m_depth			   = XMG_FALSE;
	m_visible		   = XMG_TRUE;

	m_sfactor		   = XMG_BLEND_ONE_MINUS_SRC_ALPHA;
	m_dfactor		   = XMG_BLEND_ONE_MINUS_SRC_ALPHA;		
	m_tex_combine	   = XMG_COMBINE_MODULATE;
	
	m_type_position	   = XMG_TYPE_FIXED;
	m_type_color	   = XMG_TYPE_FIXED;
	m_type_matrix	   = XMG_TYPE_FIXED;
	m_type_index	   = XMG_TYPE_FIXED;
	m_type_vertex	   = XMG_TYPE_FIXED;
	m_type_colors	   = XMG_TYPE_FIXED;
	m_type_coords[0]   = XMG_TYPE_FIXED;
	m_type_coords[1]   = XMG_TYPE_FIXED;

	m_id_index		   = 0;
	m_id_vertex		   = 0;
	m_id_colors	  	   = 0;	
	m_id_coords[ 0 ]   = 0;
	m_id_coords[ 1 ]   = 0;
	m_id_textures[ 0 ] = 0;
	m_id_textures[ 1 ] = 0;
}

XMGContour::~XMGContour ( void )
{
	if ( m_id_index )
	{
		glDeleteBuffers( 1, &m_id_index );
	}

	if ( m_id_vertex )
	{
		glDeleteBuffers( 1, &m_id_vertex );
	}

	if ( m_id_colors )
	{
		glDeleteBuffers( 1, &m_id_colors );
	}

	if ( m_id_coords[ 0 ] )
	{
		glDeleteBuffers( 1, &m_id_coords[ 0 ] );
	}

	if ( m_id_coords[ 1 ] )
	{
		glDeleteBuffers( 1, &m_id_coords[ 1 ] );
	}
}

void XMGContour::SetBuffer ( GLuint& id, GLvoid* data, GLsizeiptr size, GLenum target )
{	
	if ( id )
	{
		glDeleteBuffers ( 1, &id );
	}

	glGenBuffers ( 1, &id );

	glBindBuffer ( target, id );
	glBufferData ( target, size, data, GL_STATIC_DRAW ); 
	glBindBuffer ( target, 0 );
}
*/