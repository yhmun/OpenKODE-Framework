/*
 *
 *      File            XMGRender.cpp
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

#include "XMGLibrary.h"

static	XMGBool			l_depth		  = XMG_TRUE;
static	XMGBool			l_blend		  = XMG_FALSE;
static	XMGBlend		l_sfactor	  = XMG_BLEND_SRC_ALPHA;
static	XMGBlend		l_dfactor	  = XMG_BLEND_ONE_MINUS_SRC_ALPHA;
static	XMGCull			l_cull		  = XMG_CULL_BACK;
static	XMGWind			l_wind		  = XMG_WIND_ANTI_CLOCK;
static	XMGColor		l_color;
static  XMGTexEnv		l_tex_env;
static	XMGCombine		l_combine;
static  GLuint			l_id_tex[2];

XMGRenderImpl::XMGRenderImpl ( GLuint num_shape, GLuint num_vertex, XMGGeoType type )
{
	GLuint* 	arr_num_vertex;
	GLuint		idx_shape;

	XMGAssert ( num_shape > 0 );
	XMGAssert ( arr_num_vertex = (GLuint *) kdMalloc ( sizeof ( GLuint ) * num_shape ) );

	for ( idx_shape = 0; idx_shape < num_shape; idx_shape++ )
	{
		arr_num_vertex[ idx_shape ] = num_vertex;
	}

	Init ( num_shape, arr_num_vertex, type );

	kdFree ( arr_num_vertex );
}

XMGRenderImpl::XMGRenderImpl ( GLuint num_shape, const GLuint* arr_num_vertex, XMGGeoType type )
{	
	XMGAssert ( num_shape > 0 );

	Init ( num_shape, arr_num_vertex, type );
}

XMGRenderImpl::~XMGRenderImpl ( void )
{	
	GLuint		idx_shape;
	GLuint		num_shape;
	XMGShape*	shape;
	
	for ( idx_shape = 0, num_shape = m_vec_shape.size ( ); idx_shape < num_shape; idx_shape++ )
	{
		shape = m_vec_shape[ idx_shape ];
		delete shape;
	}

	m_vec_shape.clear ( );

	if ( m_id_index )
	{
		glDeleteBuffers ( 1, &m_id_index );
	}

	if ( m_id_vertex )
	{
		glDeleteBuffers ( 1, &m_id_vertex );
	}

	if ( m_id_colors )
	{
		glDeleteBuffers ( 1, &m_id_colors );
	}

	if ( m_id_coords[0] )
	{
		glDeleteBuffers ( 1, &m_id_coords[0] );
	}

	if ( m_id_coords[1] )
	{
		glDeleteBuffers ( 1, & m_id_coords[1] );
	}
}

void XMGRenderImpl::Init ( GLuint num_shape, const GLuint* arr_num_vertex, XMGGeoType type )
{
	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;	
	GLuint			idx_face;
	GLuint			idx_side;
	GLuint			num_side;
	GLuint			num_side_vertex;

	m_geo_type       = type;

	m_type_position  = (XMGType) 0;
	m_type_matrix	 = (XMGType) 0;
	m_type_vertex	 = (XMGType) 0;
	m_type_colors	 = (XMGType) 0;
	m_type_coords[0] = (XMGType) 0;
	m_type_coords[1] = (XMGType) 0;

	m_num_vertex     = 0;
	m_ext_vertex	 = 0;
	m_id_index		 = 0;
	m_id_vertex		 = 0;
	m_id_colors		 = 0;
	m_id_coords[0]	 = 0;
	m_id_coords[1]	 = 0;

	switch ( m_geo_type )
	{
		case XMG_GEO_REGULAR	:	m_has_face[0] = XMG_TRUE;	m_has_face[1] = XMG_FALSE;	m_has_face[2] = XMG_FALSE;	num_side_vertex = 0;	break;
		case XMG_GEO_SOLID		:	m_has_face[0] = XMG_TRUE;	m_has_face[1] = XMG_TRUE;	m_has_face[2] = XMG_TRUE;	num_side_vertex = 4;	break;
		case XMG_GEO_PILLAR		:	m_has_face[0] = XMG_TRUE;	m_has_face[1] = XMG_FALSE;	m_has_face[2] = XMG_TRUE;	num_side_vertex = 4;	break;
		case XMG_GEO_WALL		:	m_has_face[0] = XMG_FALSE;	m_has_face[1] = XMG_FALSE;	m_has_face[2] = XMG_TRUE;	num_side_vertex = 4;	break;
		case XMG_GEO_BSOLID		:	m_has_face[0] = XMG_TRUE;	m_has_face[1] = XMG_TRUE;	m_has_face[2] = XMG_TRUE;	num_side_vertex = 4;	break;
		case XMG_GEO_BPILLAR	:	m_has_face[0] = XMG_TRUE;	m_has_face[1] = XMG_FALSE;	m_has_face[2] = XMG_TRUE;	num_side_vertex = 4;	break;
		case XMG_GEO_BWALL		:	m_has_face[0] = XMG_FALSE;	m_has_face[1] = XMG_FALSE;	m_has_face[2] = XMG_TRUE;	num_side_vertex = 4;	break;
		case XMG_GEO_CONE		:	m_has_face[0] = XMG_FALSE;	m_has_face[1] = XMG_TRUE;	m_has_face[2] = XMG_TRUE;	num_side_vertex = 3;	break;
		case XMG_GEO_CWALL		:	m_has_face[0] = XMG_FALSE;	m_has_face[1] = XMG_FALSE;	m_has_face[2] = XMG_TRUE;	num_side_vertex = 3;	break;
	}

	for ( idx_shape = 0; idx_shape < num_shape; idx_shape++ )
	{
		XMGAssert ( shape = new XMGShape ( ) );

		shape->m_num_basic = arr_num_vertex[ idx_shape ];

		switch ( m_geo_type )
		{			
			case XMG_GEO_SOLID	 :
			case XMG_GEO_PILLAR	 :
			case XMG_GEO_WALL	 :	shape->m_num_input = shape->m_num_basic * 2;	break;
			case XMG_GEO_CONE	 :
			case XMG_GEO_CWALL	 :	shape->m_num_input = shape->m_num_basic + 1;	break;
			default				 :  shape->m_num_input = shape->m_num_basic;		break;
		}

		shape->m_off_vertex = m_num_vertex;		

		for ( idx_face = 0; idx_face < 2; idx_face++ )
		{
			if ( m_has_face[ idx_face ] )
			{
				XMGAssert ( facet = new XMGFacet ( ) );			

				facet->m_off_vertex = m_num_vertex;
				facet->m_num_vertex = shape->m_num_basic;

				shape->m_vec_facet.push_back ( facet );
				m_num_vertex += facet->m_num_vertex;
			}
		}

		if ( m_has_face[ 2 ] )
		{
			num_side = shape->m_num_basic;		
			for ( idx_side = 0; idx_side < num_side; idx_side++ )
			{
				XMGAssert ( facet = new XMGFacet ( ) );

				facet->m_is_side    = XMG_TRUE;
				facet->m_off_vertex = m_num_vertex;
				facet->m_num_vertex = num_side_vertex;

				shape->m_vec_facet.push_back ( facet );
				m_num_vertex += facet->m_num_vertex;
			}
		}

		shape->m_num_vertex = m_num_vertex - shape->m_off_vertex;
		m_vec_shape.push_back ( shape );
	}
}

void XMGRenderImpl::Begin ( void )
{
	XMGVector3F*	fposition = 0;
	XMGVector3X*	xposition = 0;	

	l_depth		  = XMG_TRUE;
	l_blend		  = XMG_FALSE;
	l_sfactor	  = XMG_BLEND_SRC_ALPHA;
	l_dfactor	  = XMG_BLEND_ONE_MINUS_SRC_ALPHA;
	l_cull		  = XMG_CULL_BACK;
	l_wind		  = XMG_WIND_ANTI_CLOCK;

	glPushMatrix ( );
	
	glEnable ( GL_DEPTH_TEST );		
	glDepthFunc ( GL_LEQUAL );	

	glDisable ( GL_BLEND );
	glBlendFunc ( l_sfactor, l_dfactor );

	glEnable ( GL_CULL_FACE );
	glCullFace ( GL_BACK );

	glFrontFace ( l_wind );	

	switch ( m_type_position )
	{
		case XMG_TYPE_FLOAT :				

			fposition = (XMGVector3F *) &m_position;									
			glTranslatef ( fposition->m_x, fposition->m_y, fposition->m_z );

			break;

		case XMG_TYPE_FIXED :				

			xposition = (XMGVector3X *) &m_position;
			glTranslatex ( xposition->m_x, xposition->m_y, xposition->m_z );

			break;
	}

	switch ( m_type_matrix )
	{
		case XMG_TYPE_FLOAT :	glMultMatrixf ( (GLfloat *) &m_matrix );	break;
		case XMG_TYPE_FIXED :	glMultMatrixx ( (GLfixed *) &m_matrix );	break;
	}

	kdMemset ( &l_color, 0, sizeof ( XMGColor ) );

	if ( m_id_vertex )
	{
		glEnableClientState ( GL_VERTEX_ARRAY );
		glBindBuffer ( GL_ARRAY_BUFFER, m_id_vertex );
		glVertexPointer ( 3, m_type_vertex, 0, 0 ); 
	}
	else
	{
		glDisableClientState ( GL_VERTEX_ARRAY );
	}

	if ( m_id_colors )
	{
		glEnableClientState ( GL_COLOR_ARRAY );
		glBindBuffer ( GL_ARRAY_BUFFER, m_id_colors );
		glColorPointer ( 4, m_type_colors, 0, 0 ); 
	}
	else
	{
		glDisableClientState ( GL_COLOR_ARRAY );
	}

	if ( m_id_coords[ XMG_TEX_UNIT_0 ] )
	{
		glClientActiveTexture ( GL_TEXTURE0 );
		glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
		glBindBuffer ( GL_ARRAY_BUFFER, m_id_coords[ XMG_TEX_UNIT_0 ] );
		glTexCoordPointer ( 2, m_type_coords[ XMG_TEX_UNIT_0 ], 0, 0 );

		if ( m_id_coords[ XMG_TEX_UNIT_1 ] )
		{			
			glClientActiveTexture ( GL_TEXTURE1 );
			glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
			glBindBuffer ( GL_ARRAY_BUFFER, m_id_coords[ XMG_TEX_UNIT_1 ] );
			glTexCoordPointer ( 2, m_type_coords[ XMG_TEX_UNIT_1 ], 0, 0 ); 
		}
		else
		{
			glClientActiveTexture ( GL_TEXTURE1 );
			glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
			glClientActiveTexture ( GL_TEXTURE0 );			
		}
	}
	else
	{
		glClientActiveTexture ( GL_TEXTURE1 );
		glDisableClientState ( GL_TEXTURE_COORD_ARRAY );

		glClientActiveTexture ( GL_TEXTURE0 );
		glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
	}

	if ( m_id_index )
	{
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_id_index );
	}

	kdMemset ( &l_tex_env, 0, sizeof ( XMGTexEnv ) );	 
	kdMemset ( &l_combine, 0, sizeof ( XMGCombine ) );	 
	l_id_tex[ XMG_TEX_UNIT_0 ] = 0;
	l_id_tex[ XMG_TEX_UNIT_1 ] = 0;
}

void XMGRenderImpl::Display ( GLuint src_idx_shape )
{
	XMGVector3F*	fposition = 0;
	XMGVector3X*	xposition = 0;	

	XMGShape*		shape;
	XMGFacet*		facet;
	XMGContour*		contour;

	GLuint			idx_shape;
	GLuint			idx_contour;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_contour;
	
	GLenum			disp_shape   = 0;
	GLenum			disp_facet   = 0;
	GLenum			disp_contour = 0;

	for ( RangeArray ( src_idx_shape, m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = m_vec_shape[ idx_shape ];

		if ( l_depth != shape->m_depth )
		{
			l_depth = shape->m_depth;
		
			if ( l_depth == KD_TRUE )
			{
				glEnable ( GL_DEPTH_TEST );
				glDepthFunc ( GL_LEQUAL );	
			}
			else
			{
				glDisable ( GL_DEPTH_TEST );
			}
		}

		if ( l_blend != shape->m_blend )
		{
			l_blend = shape->m_blend;

			if ( l_blend == KD_TRUE )
			{
				glEnable ( GL_BLEND );		
			}	
			else
			{
				glDisable ( GL_BLEND );
			}
		}

		if ( l_blend == KD_TRUE && l_sfactor != shape->m_sfactor || l_dfactor != shape->m_dfactor )
		{
			l_sfactor = shape->m_sfactor;
			l_dfactor = shape->m_dfactor;

			glBlendFunc ( l_sfactor, l_dfactor );
		}

		if ( l_cull != shape->m_cull )
		{
			l_cull = shape->m_cull;

			switch ( l_cull )
			{
				case XMG_CULL_DISABLE :	glDisable ( GL_CULL_FACE );								break;
				case XMG_CULL_FRONT   :	glEnable  ( GL_CULL_FACE );	glCullFace ( GL_FRONT );	break;
				case XMG_CULL_BACK    :	glEnable  ( GL_CULL_FACE );	glCullFace ( GL_BACK );		break;
			}
		}

		if ( l_wind != shape->m_wind )
		{
			l_wind = shape->m_wind;

			glFrontFace ( l_wind );	
		}

		if ( shape->m_type_position || shape->m_type_matrix )
		{
			glPushMatrix ( );

			switch ( shape->m_type_position )
			{
				case XMG_TYPE_FLOAT :				

					fposition = (XMGVector3F *) &shape->m_position;									
					glTranslatef ( fposition->m_x, fposition->m_y, fposition->m_z );

					break;

				case XMG_TYPE_FIXED :				

					xposition = (XMGVector3X *) &shape->m_position;
					glTranslatex ( xposition->m_x, xposition->m_y, xposition->m_z );

					break;
			}

			switch ( shape->m_type_matrix )
			{
				case XMG_TYPE_FLOAT :	glMultMatrixf ( (GLfloat *) &shape->m_matrix );	break;
				case XMG_TYPE_FIXED :	glMultMatrixx ( (GLfixed *) &shape->m_matrix );	break;
			}
		}
	
		switch ( shape->m_disp_mode )
		{
			case XMG_DISP_FILL    :
			case XMG_DISP_TEXTURE :	disp_shape = shape->m_disp_tri;		break;
			case XMG_DISP_LINE	  : disp_shape = shape->m_disp_line;	break;
			case XMG_DISP_POINT	  : disp_shape = GL_POINTS;				break;
		}

		num_facet = shape->m_vec_facet.size ( );
		for ( idx_facet = 0; idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			if ( facet->m_visible == XMG_FALSE )
			{
				continue;
			}

			Color ( idx_shape, idx_facet );

			if ( shape->m_disp_mode == XMG_DISP_TEXTURE )
			{
				if ( m_id_coords[ XMG_TEX_UNIT_0 ] && facet->m_id_textures[ XMG_TEX_UNIT_0 ] )
				{
					if ( l_id_tex[ XMG_TEX_UNIT_0 ] != facet->m_id_textures[ XMG_TEX_UNIT_0 ] || l_tex_env != facet->m_tex_env )
					{
						l_id_tex[ XMG_TEX_UNIT_0 ] = facet->m_id_textures[ XMG_TEX_UNIT_0 ];
						l_tex_env = facet->m_tex_env;

						glActiveTexture ( GL_TEXTURE0 );
						glEnable ( GL_TEXTURE_2D ); 
						glBindTexture ( GL_TEXTURE_2D, facet->m_id_textures [ XMG_TEX_UNIT_0 ] );
						glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLfloat) facet->m_tex_env );
					}

					if ( m_id_coords[ XMG_TEX_UNIT_1 ] && facet->m_id_textures[ XMG_TEX_UNIT_1 ] )
					{
						if ( l_id_tex[ XMG_TEX_UNIT_1 ] != facet->m_id_textures[ XMG_TEX_UNIT_1 ] || l_combine != facet->m_tex_combine )
						{
							l_id_tex[ XMG_TEX_UNIT_1 ] = facet->m_id_textures[ XMG_TEX_UNIT_1 ];
							l_combine = facet->m_tex_combine;

							glActiveTexture ( GL_TEXTURE1 );
							glEnable ( GL_TEXTURE_2D ); 
							glBindTexture ( GL_TEXTURE_2D, facet->m_id_textures [ XMG_TEX_UNIT_1 ] );

							glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
							glTexEnvf ( GL_TEXTURE_ENV, GL_COMBINE_RGB, (GLfloat) facet->m_tex_combine );
								
							glTexEnvf ( GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE );
							glTexEnvf ( GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS );

							glTexEnvf ( GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR );
							glTexEnvf ( GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR );
						}
					}
				}
				else
				{	
					l_id_tex[ XMG_TEX_UNIT_0 ] = 0;
					l_id_tex[ XMG_TEX_UNIT_1 ] = 0;

					glActiveTexture ( GL_TEXTURE0 );
					glDisable ( GL_TEXTURE_2D ); 	
				}
			}

			num_contour = facet->m_vec_contour.size ( );
			if (  num_contour > 0 )
			{
				for ( idx_contour = 0; idx_contour < num_contour; idx_contour++ )
				{
					contour = facet->m_vec_contour[ idx_contour ];

					switch (  contour->m_draw_limit )
					{
						case XMG_LIMIT_TRI :

							if (  shape->m_disp_mode == XMG_DISP_POINT || shape->m_disp_mode == XMG_DISP_LINE )
							{
								continue;
							}

							disp_contour = contour->m_disp_mode ? contour->m_disp_mode : disp_shape;

							break;

						case XMG_LIMIT_LINE :

							if (  shape->m_disp_mode == XMG_DISP_FILL || shape->m_disp_mode == XMG_DISP_TEXTURE )
							{
								continue;
							}							

						case XMG_LIMIT_NONE :

							disp_contour = disp_shape;

					}

					glDrawArrays ( disp_contour, contour->m_off_vertex, contour->m_num_vertex );									
				}
			}
			else
			{
				if ( m_id_index )
				{
					glDrawElements ( disp_shape, facet->m_num_index, GL_UNSIGNED_SHORT, &((GLshort*) 0)[ sizeof (GLushort) * facet->m_off_index ] );
				}
				else
				{
					if ( facet->m_is_side )
					{
						switch ( shape->m_disp_mode )
						{
							case XMG_DISP_FILL    :
							case XMG_DISP_TEXTURE :	disp_facet = GL_TRIANGLE_FAN;	break;
							case XMG_DISP_LINE	  : disp_facet = GL_LINE_LOOP;		break;
							case XMG_DISP_POINT	  : disp_facet = GL_POINTS;			break;
						}

						glDrawArrays ( disp_facet, facet->m_off_vertex, facet->m_num_vertex );							
					}
					else
					{
						glDrawArrays ( disp_shape, facet->m_off_vertex, facet->m_num_vertex );						
					}		
				}		
			}
		}				

		if ( shape->m_type_position || shape->m_type_matrix )
		{
			glPopMatrix ( );
		}
	}
}

void XMGRenderImpl::End ( void )
{
	glActiveTexture ( GL_TEXTURE1 );
	glDisable ( GL_TEXTURE_2D );

	glActiveTexture ( GL_TEXTURE0 );
	glDisable ( GL_TEXTURE_2D );	

	glClientActiveTexture ( GL_TEXTURE1 );
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );	
	
	glClientActiveTexture ( GL_TEXTURE0 );
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );

	glDisableClientState ( GL_COLOR_ARRAY );
	glDisableClientState ( GL_VERTEX_ARRAY );

	glBindTexture ( GL_TEXTURE_2D, 0 );		
	glBindBuffer  ( GL_ARRAY_BUFFER, 0 );
	glBindBuffer  ( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glPopMatrix ( );
}

void XMGRenderImpl::DisplayBegin ( GLuint idx_shape )
{
	XMGVector3F*	fposition = 0;
	XMGVector3X*	xposition = 0;	
	XMGShape*		shape;

	shape = m_vec_shape[ idx_shape ];

	if ( l_depth != shape->m_depth )
	{
		l_depth = shape->m_depth;
	
		if ( l_depth == KD_TRUE )
		{
			glEnable ( GL_DEPTH_TEST );
			glDepthFunc ( GL_LEQUAL );	
		}
		else
		{
			glDisable ( GL_DEPTH_TEST );
		}
	}

	if ( l_blend != shape->m_blend )
	{
		l_blend = shape->m_blend;

		if ( l_blend == KD_TRUE )
		{
			glEnable ( GL_BLEND );		
		}	
		else
		{
			glDisable ( GL_BLEND );
		}
	}

	if ( l_blend == KD_TRUE && l_sfactor != shape->m_sfactor || l_dfactor != shape->m_dfactor )
	{
		l_sfactor = shape->m_sfactor;
		l_dfactor = shape->m_dfactor;

		glBlendFunc ( l_sfactor, l_dfactor );
	}

	if ( l_cull != shape->m_cull )
	{
		l_cull = shape->m_cull;

		switch ( l_cull )
		{
			case XMG_CULL_DISABLE :	glDisable ( GL_CULL_FACE );								break;
			case XMG_CULL_FRONT   :	glEnable  ( GL_CULL_FACE );	glCullFace ( GL_FRONT );	break;
			case XMG_CULL_BACK    :	glEnable  ( GL_CULL_FACE );	glCullFace ( GL_BACK );		break;
		}
	}

	if ( l_wind != shape->m_wind )
	{
		l_wind = shape->m_wind;

		glFrontFace ( l_wind );	
	}

	if ( shape->m_type_position || shape->m_type_matrix )
	{
		glPushMatrix ( );

		switch ( shape->m_type_position )
		{
			case XMG_TYPE_FLOAT :				

				fposition = (XMGVector3F *) &shape->m_position;									
				glTranslatef ( fposition->m_x, fposition->m_y, fposition->m_z );

				break;

			case XMG_TYPE_FIXED :				

				xposition = (XMGVector3X *) &shape->m_position;
				glTranslatex ( xposition->m_x, xposition->m_y, xposition->m_z );

				break;
		}

		switch ( shape->m_type_matrix )
		{
			case XMG_TYPE_FLOAT :	glMultMatrixf ( (GLfloat *) &shape->m_matrix );	break;
			case XMG_TYPE_FIXED :	glMultMatrixx ( (GLfixed *) &shape->m_matrix );	break;
		}
	}
}

void XMGRenderImpl::DisplayEnd ( GLuint idx_shape )
{
	XMGShape*	shape;

	shape = m_vec_shape[ idx_shape ];

	if ( shape->m_type_position || shape->m_type_matrix )
	{
		glPopMatrix ( );
	}
}

void XMGRenderImpl::Color ( GLuint idx_shape, GLuint idx_facet )
{
	XMGColorF*		fcolor;
	XMGColorX*		xcolor;

	XMGShape*		shape;
	XMGFacet*		facet;

	shape = m_vec_shape[ idx_shape ];
	facet = shape->m_vec_facet[ idx_facet ];

	if ( l_color != facet->m_color )
	{
		kdMemcpy ( &l_color, &facet->m_color, sizeof ( XMGColor ) );

		switch ( facet->m_type_color )
		{
			case XMG_TYPE_FLOAT :
				fcolor = (XMGColorF *) &l_color;
				glColor4f ( fcolor->m_r, fcolor->m_g, fcolor->m_b, fcolor->m_a ); 
				break;

			case XMG_TYPE_FIXED :
				xcolor = (XMGColorX *) &l_color;
				glColor4x ( xcolor->m_r, xcolor->m_g, xcolor->m_b, xcolor->m_a ); 
				break;
		}
	}
}

void XMGRenderImpl::RangeArray ( GLuint idx_array, GLuint num_array, GLuint& begin, GLuint& end )
{
	if ( idx_array == 0xffffffff )
	{
		begin = 0;
		end   = num_array; 
	}
	else
	{
		begin = idx_array;
		end   = begin + 1;
	}
}

void XMGRenderImpl::SetRegular ( const XMGVector3* arr_vertex, XMGShape* shape )
{	
	SetBuffer ( m_id_vertex, arr_vertex, sizeof ( XMGVector3 ) * shape->m_off_vertex, sizeof ( XMGVector3 ) * shape->m_num_vertex );
}

void XMGRenderImpl::SetSolid ( const XMGVector3* arr_vertex, XMGShape* shape )
{
	GLuint			idx_vertex1;
	GLuint			idx_vertex2;

	XMGVector3*		arr_vertex_side;

	// Front & Rear
	SetBuffer ( m_id_vertex, arr_vertex, sizeof ( XMGVector3 ) * shape->m_off_vertex, sizeof ( XMGVector3 ) * shape->m_num_basic * 2 );

	// Side 
	XMGAssert ( arr_vertex_side = (XMGVector3 *) kdMalloc ( sizeof ( XMGVector3 ) * shape->m_num_basic * 4 ) );

	for ( idx_vertex1 = 0; idx_vertex1 < shape->m_num_basic; idx_vertex1++ )
	{
		if ( shape->m_idx_hole == XMG_HOLE_NULL )
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) < shape->m_num_basic ? idx_vertex1 + 1 : 0;
		}
		else
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) == shape->m_idx_hole ? 0 : ( idx_vertex1 + 1 ) == shape->m_num_basic ? shape->m_idx_hole : idx_vertex1 + 1;			
		}

		arr_vertex_side[ idx_vertex1 * 4 + 0 ] = arr_vertex[ idx_vertex1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 1 ] = arr_vertex[ shape->m_num_input - idx_vertex1 - 1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 2 ] = arr_vertex[ shape->m_num_input - idx_vertex2 - 1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 3 ] = arr_vertex[ idx_vertex2 ];
	}

	SetBuffer ( m_id_vertex, arr_vertex_side, sizeof ( XMGVector3 ) * ( shape->m_off_vertex + shape->m_num_basic * 2 ), sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	kdFree ( arr_vertex_side );
}

void XMGRenderImpl::SetPillar ( const XMGVector3* arr_vertex, XMGShape* shape )
{
	GLuint			idx_vertex1;
	GLuint			idx_vertex2;

	XMGVector3*		arr_vertex_side;

	// Front 
	SetBuffer ( m_id_vertex, arr_vertex, sizeof ( XMGVector3 ) * shape->m_off_vertex, sizeof ( XMGVector3 ) * shape->m_num_basic );

	// Side 
	XMGAssert ( arr_vertex_side = (XMGVector3 *) kdMalloc ( sizeof ( XMGVector3 ) * shape->m_num_basic * 4 ) );

	for ( idx_vertex1 = 0; idx_vertex1 < shape->m_num_basic; idx_vertex1++ )
	{
		if ( shape->m_idx_hole == XMG_HOLE_NULL )
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) < shape->m_num_basic ? idx_vertex1 + 1 : 0;
		}
		else
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) == shape->m_idx_hole ? 0 : ( idx_vertex1 + 1 ) == shape->m_num_basic ? shape->m_idx_hole : idx_vertex1 + 1;			
		}

		arr_vertex_side[ idx_vertex1 * 4 + 0 ] = arr_vertex[ idx_vertex1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 1 ] = arr_vertex[ shape->m_num_input - idx_vertex1 - 1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 2 ] = arr_vertex[ shape->m_num_input - idx_vertex2 - 1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 3 ] = arr_vertex[ idx_vertex2 ];
	}

	SetBuffer ( m_id_vertex, arr_vertex_side, sizeof ( XMGVector3 ) * ( shape->m_off_vertex + shape->m_num_basic ), sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	kdFree ( arr_vertex_side );
}

void XMGRenderImpl::SetWall ( const XMGVector3* arr_vertex, XMGShape* shape )
{
	GLuint			idx_vertex1;
	GLuint			idx_vertex2;

	XMGVector3*		arr_vertex_side;

	// Side 
	XMGAssert ( arr_vertex_side = (XMGVector3 *) kdMalloc ( sizeof ( XMGVector3 ) * shape->m_num_basic * 4 ) );

	for ( idx_vertex1 = 0; idx_vertex1 < shape->m_num_basic; idx_vertex1++ )
	{
		if ( shape->m_idx_hole == XMG_HOLE_NULL )
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) < shape->m_num_basic ? idx_vertex1 + 1 : 0;
		}
		else
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) == shape->m_idx_hole ? 0 : ( idx_vertex1 + 1 ) == shape->m_num_basic ? shape->m_idx_hole : idx_vertex1 + 1;			
		}

		arr_vertex_side[ idx_vertex1 * 4 + 0 ] = arr_vertex[ idx_vertex1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 1 ] = arr_vertex[ shape->m_num_input - idx_vertex1 - 1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 2 ] = arr_vertex[ shape->m_num_input - idx_vertex2 - 1 ];
		arr_vertex_side[ idx_vertex1 * 4 + 3 ] = arr_vertex[ idx_vertex2 ];
	}

	SetBuffer ( m_id_vertex, arr_vertex_side, sizeof ( XMGVector3 ) * ( shape->m_off_vertex ), sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	kdFree ( arr_vertex_side );
}

void XMGRenderImpl::SetBSolid ( const XMGVector3* arr_vertex, XMGShape* shape )
{
	GLuint			idx_vertex1;
	GLuint			idx_vertex2;

	XMGVector3*		arr_vertex_rear;
	XMGVector3*		arr_vertex_side;

	GLfixed			zero  = 0;
	GLfloat			fzero = 0;

	if ( m_type_vertex == XMG_TYPE_FLOAT )
	{
		kdMemcpy ( &zero, &fzero, sizeof ( GLfixed ) );
	}

	// Front
	SetBuffer ( m_id_vertex, arr_vertex, sizeof ( XMGVector3 ) * shape->m_off_vertex, sizeof ( XMGVector3 ) * shape->m_num_basic );

	// Rear
	XMGAssert ( arr_vertex_rear = (XMGVector3 *) kdMalloc ( sizeof ( XMGVector3 ) * shape->m_num_basic ) );

	for ( idx_vertex1 = 0; idx_vertex1 < shape->m_num_basic; idx_vertex1++ )
	{
		if ( shape->m_idx_hole == XMG_HOLE_NULL )
		{
			idx_vertex2 = shape->m_num_basic - idx_vertex1 - 1;
		}
		else
		{
			idx_vertex2 = idx_vertex1 < shape->m_idx_hole ? shape->m_idx_hole - idx_vertex1 - 1 : shape->m_num_basic + shape->m_idx_hole - idx_vertex1 - 1;			
		}

		arr_vertex_rear[ idx_vertex1 ].m_x = arr_vertex[ idx_vertex2 ].m_x;
		arr_vertex_rear[ idx_vertex1 ].m_y = arr_vertex[ idx_vertex2 ].m_y;
		arr_vertex_rear[ idx_vertex1 ].m_z = zero;
	}

	SetBuffer ( m_id_vertex, arr_vertex_rear, sizeof ( XMGVector3 ) * ( shape->m_off_vertex + shape->m_num_basic ), sizeof ( XMGVector3 ) * shape->m_num_basic );
	kdFree ( arr_vertex_rear );

	// Side
	XMGAssert ( arr_vertex_side = (XMGVector3 *) kdMalloc ( sizeof ( XMGVector3 ) * shape->m_num_basic * 4 ) );
	kdMemset ( arr_vertex_side, 0, sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	
	for ( idx_vertex1 = 0; idx_vertex1 < shape->m_num_basic; idx_vertex1++ )
	{
		if ( shape->m_idx_hole == XMG_HOLE_NULL )
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) < shape->m_num_basic ? idx_vertex1 + 1 : 0;
		}
		else
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) == shape->m_idx_hole ? 0 : ( idx_vertex1 + 1 ) == shape->m_num_basic ? shape->m_idx_hole : idx_vertex1 + 1;			
		}

		arr_vertex_side[ idx_vertex1 * 4 + 0 ] = arr_vertex[ idx_vertex1 ];

		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_x = arr_vertex[ idx_vertex1 ].m_x;
		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_y = arr_vertex[ idx_vertex1 ].m_y;
		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_z = zero;

		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_x = arr_vertex[ idx_vertex2 ].m_x;
		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_y = arr_vertex[ idx_vertex2 ].m_y;
		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_z = zero;

		arr_vertex_side[ idx_vertex1 * 4 + 3 ] = arr_vertex[ idx_vertex2 ];
	}

	SetBuffer ( m_id_vertex, arr_vertex_side, sizeof ( XMGVector3 ) * ( shape->m_off_vertex + shape->m_num_basic * 2 ), sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	kdFree ( arr_vertex_side );
}

void XMGRenderImpl::SetBPillar ( const XMGVector3* arr_vertex, XMGShape* shape )
{
	GLuint			idx_vertex1;
	GLuint			idx_vertex2;

	XMGVector3*		arr_vertex_side;

	GLfixed			zero  = 0;
	GLfloat			fzero = 0;

	if ( m_type_vertex == XMG_TYPE_FLOAT )
	{
		kdMemcpy ( &zero, &fzero, sizeof ( GLfixed ) );
	}

	// Front
	SetBuffer ( m_id_vertex, arr_vertex, sizeof ( XMGVector3 ) * shape->m_off_vertex, sizeof ( XMGVector3 ) * shape->m_num_basic );

	// Side
	XMGAssert ( arr_vertex_side = (XMGVector3 *) kdMalloc ( sizeof ( XMGVector3 ) * shape->m_num_basic * 4 ) );
	kdMemset ( arr_vertex_side, 0, sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	
	for ( idx_vertex1 = 0; idx_vertex1 < shape->m_num_basic; idx_vertex1++ )
	{
		if ( shape->m_idx_hole == XMG_HOLE_NULL )
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) < shape->m_num_basic ? idx_vertex1 + 1 : 0;
		}
		else
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) == shape->m_idx_hole ? 0 : ( idx_vertex1 + 1 ) == shape->m_num_basic ? shape->m_idx_hole : idx_vertex1 + 1;			
		}

		arr_vertex_side[ idx_vertex1 * 4 + 0 ] = arr_vertex[ idx_vertex1 ];

		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_x = arr_vertex[ idx_vertex1 ].m_x;
		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_y = arr_vertex[ idx_vertex1 ].m_y;
		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_z = zero;

		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_x = arr_vertex[ idx_vertex2 ].m_x;
		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_y = arr_vertex[ idx_vertex2 ].m_y;
		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_z = zero;

		arr_vertex_side[ idx_vertex1 * 4 + 3 ] = arr_vertex[ idx_vertex2 ];
	}

	SetBuffer ( m_id_vertex, arr_vertex_side, sizeof ( XMGVector3 ) * ( shape->m_off_vertex + shape->m_num_basic ), sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	kdFree ( arr_vertex_side );
}

void XMGRenderImpl::SetBWall ( const XMGVector3* arr_vertex, XMGShape* shape )
{
	GLuint			idx_vertex1;
	GLuint			idx_vertex2;

	XMGVector3*		arr_vertex_side;

	GLfixed			zero  = 0;
	GLfloat			fzero = 0;

	if ( m_type_vertex == XMG_TYPE_FLOAT )
	{
		kdMemcpy ( &zero, &fzero, sizeof ( GLfixed ) );
	}

	// Side
	XMGAssert ( arr_vertex_side = (XMGVector3 *) kdMalloc ( sizeof ( XMGVector3 ) * shape->m_num_basic * 4 ) );
	kdMemset ( arr_vertex_side, 0, sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	
	for ( idx_vertex1 = 0; idx_vertex1 < shape->m_num_basic; idx_vertex1++ )
	{
		if ( shape->m_idx_hole == XMG_HOLE_NULL )
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) < shape->m_num_basic ? idx_vertex1 + 1 : 0;
		}
		else
		{
			idx_vertex2 = ( idx_vertex1 + 1 ) == shape->m_idx_hole ? 0 : ( idx_vertex1 + 1 ) == shape->m_num_basic ? shape->m_idx_hole : idx_vertex1 + 1;			
		}

		arr_vertex_side[ idx_vertex1 * 4 + 0 ] = arr_vertex[ idx_vertex1 ];

		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_x = arr_vertex[ idx_vertex1 ].m_x;
		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_y = arr_vertex[ idx_vertex1 ].m_y;
		arr_vertex_side[ idx_vertex1 * 4 + 1 ].m_z = zero;

		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_x = arr_vertex[ idx_vertex2 ].m_x;
		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_y = arr_vertex[ idx_vertex2 ].m_y;
		arr_vertex_side[ idx_vertex1 * 4 + 2 ].m_z = zero;

		arr_vertex_side[ idx_vertex1 * 4 + 3 ] = arr_vertex[ idx_vertex2 ];
	}

	SetBuffer ( m_id_vertex, arr_vertex_side, sizeof ( XMGVector3 ) * ( shape->m_off_vertex ), sizeof ( XMGVector3 ) * shape->m_num_basic * 4 );
	kdFree ( arr_vertex_side );
}

void XMGRenderImpl::GenBuffer ( GLuint& id, GLsizeiptr size, GLenum target )
{
	if ( id )
	{
		glDeleteBuffers ( 1, &id );
	}

	glGenBuffers ( 1, &id );

	glBindBuffer ( target, id );
	glBufferData ( target, size, 0, GL_STATIC_DRAW ); 
}

void XMGRenderImpl::SetBuffer ( GLuint& id, const GLvoid* data, GLintptr offset, GLsizeiptr size, GLenum target )
{
	glBindBuffer ( target, id );
	glBufferSubData ( target, offset, size, data );
}

XMGRender::XMGRender ( GLuint num_shape, GLuint num_vertex, XMGGeoType type )
{
	XMGAssert ( m_impl = new XMGRenderImpl ( num_shape, num_vertex, type ) );
}

XMGRender::XMGRender ( GLuint num_shape, const GLuint* arr_num_vertex, XMGGeoType type )
{
	XMGAssert ( m_impl = new XMGRenderImpl ( num_shape, arr_num_vertex, type ) );
}

XMGRender::~XMGRender ( void )
{
	delete (XMGRenderImpl *) m_impl;
}

void XMGRender::Render ( GLuint idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	impl->Begin ( );

	impl->Display ( idx_shape );

	impl->End ( );
}

void XMGRender::SetDispMode	( XMGDispMode mode, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			idx_shape;
	GLuint			num_shape;	

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];
		shape->m_disp_mode = mode;
	}
}

void XMGRender::SetDispLine ( XMGDispLine mode, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			idx_shape;
	GLuint			num_shape;	

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];
		shape->m_disp_line = mode;
	}
}

void XMGRender::SetDispTri ( XMGDispTri mode, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			idx_shape;
	GLuint			num_shape;	

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];
		shape->m_disp_tri = mode;
	}
}

void XMGRender::SetCull ( XMGCull mode, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			idx_shape;
	GLuint			num_shape;	

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];
		shape->m_cull = mode;
	}
}

void XMGRender::SetWind ( XMGWind mode, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			idx_shape;
	GLuint			num_shape;	

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];
		shape->m_wind = mode;
	}
}

void XMGRender::SetBlend ( XMGBool enable, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			idx_shape;
	GLuint			num_shape;	

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];
		shape->m_blend = enable;
	}	
}

void XMGRender::SetDepth ( XMGBool enable, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			idx_shape;
	GLuint			num_shape;	

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];
		shape->m_depth = enable;
	}	
}

void XMGRender::SetBlendOP ( XMGBlend sfactor, XMGBlend dfactor, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			idx_shape;
	GLuint			num_shape;	

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];
		shape->m_sfactor = sfactor;
		shape->m_dfactor = dfactor;
	}	
}

void XMGRender::SetPosition ( const XMGVector3F& position, GLuint idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;
	
	XMGShape*		shape;

	if ( idx_shape == XMG_SHAPE_ALL )
	{
		impl->m_type_position = XMG_TYPE_FLOAT;
		kdMemcpy ( &impl->m_position, &position, sizeof ( XMGVector3F ) );	
	}
	else
	{
		shape = impl->m_vec_shape[ idx_shape ];

		shape->m_type_position = XMG_TYPE_FLOAT;
		kdMemcpy ( &shape->m_position, &position, sizeof ( XMGVector3F ) );	
	}
}

void XMGRender::SetPosition ( const XMGVector3X& position, GLuint idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;
	
	XMGShape*		shape;

	if ( idx_shape == XMG_SHAPE_ALL )
	{
		impl->m_type_position = XMG_TYPE_FIXED;
		kdMemcpy ( &impl->m_position, &position, sizeof ( XMGVector3X ) );	
	}
	else
	{
		shape = impl->m_vec_shape[ idx_shape ];

		shape->m_type_position = XMG_TYPE_FIXED;
		kdMemcpy ( &shape->m_position, &position, sizeof ( XMGVector3F ) );	
	}
}

void XMGRender::SetMatrix ( const XMGMatrix4F& matrix, GLuint idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;
	
	XMGShape*		shape;

	if ( idx_shape == XMG_SHAPE_ALL )
	{
		impl->m_type_matrix = XMG_TYPE_FLOAT;
		kdMemcpy ( &impl->m_matrix, &matrix, sizeof ( XMGMatrix4F ) );	
	}
	else
	{
		shape = impl->m_vec_shape[ idx_shape ];

		shape->m_type_matrix = XMG_TYPE_FLOAT;
		kdMemcpy ( &shape->m_matrix, &matrix, sizeof ( XMGMatrix4F ) );	
	}
}

void XMGRender::SetMatrix ( const XMGMatrix4X& matrix, GLuint idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	if ( idx_shape == XMG_SHAPE_ALL )
	{
		impl->m_type_matrix = XMG_TYPE_FIXED;
		kdMemcpy ( &impl->m_matrix, &matrix, sizeof ( XMGMatrix4X ) );	
	}
	else
	{
		shape = impl->m_vec_shape[ idx_shape ];

		shape->m_type_matrix = XMG_TYPE_FIXED;
		kdMemcpy ( &shape->m_matrix, &matrix, sizeof ( XMGMatrix4X ) );	
	}
}

void XMGRender::SetColor ( const XMGColorF& color, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			facet->m_type_color = XMG_TYPE_FLOAT;
			kdMemcpy ( &facet->m_color, &color, sizeof ( XMGColorF ) );
		}
	}
}

void XMGRender::SetColor ( const XMGColorX& color, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			facet->m_type_color = XMG_TYPE_FIXED;
			kdMemcpy ( &facet->m_color, &color, sizeof ( XMGColorX ) );
		}
	}
}

void XMGRender::SetColors ( const XMGColorF* colors, XMGBool is_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_color;

	GLuint			num_shape;
	GLuint			num_facet;

	XMGShape*		shape;
	XMGFacet*		facet;

	idx_color = 0;

	for ( idx_shape = 0, num_shape = impl->m_vec_shape.size ( ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( idx_facet = 0, num_facet = shape->m_vec_facet.size ( ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			facet->m_type_color = XMG_TYPE_FLOAT;
			kdMemcpy ( &facet->m_color, &colors[ idx_color ], sizeof ( XMGColorF ) );

			if ( is_facet == XMG_TRUE )
			{
				idx_color += 1;
			}
		}

		if ( is_facet == XMG_FALSE )
		{
			idx_color += 1;
		}
	}
}

void XMGRender::SetColors ( const XMGColorX* colors, XMGBool is_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_color;

	GLuint			num_shape;
	GLuint			num_facet;

	XMGShape*		shape;
	XMGFacet*		facet;

	idx_color = 0;

	for ( idx_shape = 0, num_shape = impl->m_vec_shape.size ( ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( idx_facet = 0, num_facet = shape->m_vec_facet.size ( ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			facet->m_type_color = XMG_TYPE_FIXED;
			kdMemcpy ( &facet->m_color, &colors[ idx_color ], sizeof ( XMGColorX ) );

			if ( is_facet == XMG_TRUE )
			{
				idx_color += 1;
			}
		}

		if ( is_facet == XMG_FALSE )
		{
			idx_color += 1;
		}
	}
}

void XMGRender::SetVisible ( XMGBool visible, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			facet->m_visible = visible;
		}
	}
}

void XMGRender::SetTexEnv ( XMGTexEnv tex_env, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			facet->m_tex_env = tex_env;
		}
	}
}

void XMGRender::SetTexCombine ( XMGCombine combine, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			facet->m_tex_combine = combine;
		}
	}
}

void XMGRender::SetTexture ( GLuint id, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;

	for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			facet->m_id_textures[ unit ] = id;
		}
	}
}

void XMGRender::SetTexture ( const XMGTexture* texture, XMGTexUnit unit, GLuint idx_shape, GLuint idx_facet )
{
	XMGTexture*		tex = (XMGTexture *) texture;
	GLuint			id;

	tex->GetID ( id );
	SetTexture ( id, unit, idx_shape, idx_facet );
}

void XMGRender::SetVertexArray ( const XMGVector3F* arr_vertex, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			num_shape;
	GLuint			num_vertex;

	GLuint			idx_shape;
	GLuint			idx_vertex;

	GLuint			off_vertex;

	XMGVector3X*	dst_arr_vertex;

	if ( src_idx_shape == XMG_SHAPE_ALL )
	{
		impl->m_type_vertex = XMG_TYPE_FLOAT;

		if ( impl->m_geo_type == XMG_GEO_REGULAR )
		{
			impl->GenBuffer ( impl->m_id_vertex, sizeof ( XMGVector3F ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
			impl->SetBuffer ( impl->m_id_vertex, arr_vertex, 0, sizeof ( XMGVector3F ) * impl->m_num_vertex );
			return;
		}
		else
		{
			impl->GenBuffer ( impl->m_id_vertex, sizeof ( XMGVector3F ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		}		
	}
	else if ( !impl->m_id_vertex )
	{
		impl->m_type_vertex = XMG_TYPE_FLOAT;
		impl->GenBuffer ( impl->m_id_vertex, sizeof ( XMGVector3F ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
	}

	off_vertex = 0;

	if ( impl->m_type_vertex == XMG_TYPE_FLOAT )
	{		
		for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
		{
			shape = impl->m_vec_shape[ idx_shape ];

			switch ( impl->m_geo_type )
			{
				case XMG_GEO_REGULAR	: impl->SetRegular ( (const XMGVector3 *) &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_SOLID		: impl->SetSolid   ( (const XMGVector3 *) &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_PILLAR		: impl->SetPillar  ( (const XMGVector3 *) &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_WALL		: impl->SetWall    ( (const XMGVector3 *) &arr_vertex[ off_vertex ], shape );	break;				
				case XMG_GEO_BSOLID		: impl->SetBSolid  ( (const XMGVector3 *) &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_BPILLAR	: impl->SetBPillar ( (const XMGVector3 *) &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_BWALL		: impl->SetBWall   ( (const XMGVector3 *) &arr_vertex[ off_vertex ], shape );	break;
			}			

			off_vertex += shape->m_num_input;
		}		
	}
	else
	{
		shape = impl->m_vec_shape[ src_idx_shape ];
		num_vertex = shape->m_num_input;

		XMGAssert ( dst_arr_vertex = (XMGVector3X *) kdMalloc ( sizeof ( XMGVector3X ) * num_vertex ) );
		for ( idx_vertex = 0; idx_vertex < num_vertex; idx_vertex++ )
		{
			dst_arr_vertex[ idx_vertex ] = arr_vertex[ idx_vertex ];
		}

		SetVertexArray ( dst_arr_vertex, src_idx_shape );
		kdFree ( dst_arr_vertex );
	}
}

void XMGRender::SetVertexArray ( const XMGVector3X* arr_vertex, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;

	GLuint			num_shape;
	GLuint			num_vertex;

	GLuint			idx_shape;
	GLuint			idx_vertex;

	GLuint			off_vertex;

	XMGVector3F*	dst_arr_vertex;

	if ( src_idx_shape == XMG_SHAPE_ALL )
	{
		impl->m_type_vertex = XMG_TYPE_FIXED;

		if ( impl->m_geo_type == XMG_GEO_REGULAR )
		{
			impl->GenBuffer ( impl->m_id_vertex, sizeof ( XMGVector3X ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
			impl->SetBuffer ( impl->m_id_vertex, arr_vertex, 0, sizeof ( XMGVector3X ) * impl->m_num_vertex );
			return;
		}
		else
		{
			impl->GenBuffer ( impl->m_id_vertex, sizeof ( XMGVector3X ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		}		
	}
	else if ( !impl->m_id_vertex )
	{
		impl->m_type_vertex = XMG_TYPE_FIXED;
		impl->GenBuffer ( impl->m_id_vertex, sizeof ( XMGVector3X ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
	}

	off_vertex = 0;

	if ( impl->m_type_vertex == XMG_TYPE_FIXED )
	{		
		for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
		{
			shape = impl->m_vec_shape[ idx_shape ];

			switch ( impl->m_geo_type )
			{
				case XMG_GEO_REGULAR	: impl->SetRegular ( &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_SOLID		: impl->SetSolid   ( &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_PILLAR		: impl->SetPillar  ( &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_WALL		: impl->SetWall    ( &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_BSOLID		: impl->SetBSolid  ( &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_BPILLAR	: impl->SetBPillar ( &arr_vertex[ off_vertex ], shape );	break;
				case XMG_GEO_BWALL		: impl->SetBWall   ( &arr_vertex[ off_vertex ], shape );	break;
			}	

			off_vertex += shape->m_num_input;
		}		
	}
	else
	{	
		shape = impl->m_vec_shape[ src_idx_shape ];
		num_vertex = shape->m_num_input;

		XMGAssert ( dst_arr_vertex = (XMGVector3F *) kdMalloc ( sizeof ( XMGVector3F ) * num_vertex ) );

		for ( idx_vertex = 0; idx_vertex < num_vertex; idx_vertex++ )
		{
			dst_arr_vertex[ idx_vertex ] = arr_vertex[ idx_vertex ];
		}

		SetVertexArray ( dst_arr_vertex, src_idx_shape );
		kdFree ( dst_arr_vertex );
	}
}

void XMGRender::SetCoordArray ( const XMGVector2F* src_arr_coord, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_coord;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_coord;

	XMGVector2X*	arr_coord;

	GLuint			off_src;

	if ( src_idx_shape == XMG_SHAPE_ALL && src_idx_facet == XMG_FACET_ALL )
	{
		impl->m_type_coords[ unit ] = XMG_TYPE_FLOAT;
		impl->GenBuffer ( impl->m_id_coords[ unit ], sizeof ( XMGVector2F ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		impl->SetBuffer ( impl->m_id_coords[ unit ], src_arr_coord, 0, sizeof ( XMGVector2F ) * impl->m_num_vertex );
	}
	else
	{
		if ( impl->m_id_coords[ unit ] == 0 )
		{
			impl->m_type_coords[ unit ] = XMG_TYPE_FLOAT;
			impl->GenBuffer ( impl->m_id_coords[ unit ], sizeof ( XMGVector2F ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		}

		off_src = 0;
		for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
		{
			shape = impl->m_vec_shape[ idx_shape ];
			
			for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
			{
				facet = shape->m_vec_facet[ idx_facet ];

				if ( impl->m_type_coords[ unit ] == XMG_TYPE_FLOAT )
				{
					impl->SetBuffer ( impl->m_id_coords[ unit ], &src_arr_coord[ off_src ], sizeof ( XMGVector2F ) * facet->m_off_vertex, sizeof ( XMGVector2F ) * facet->m_num_vertex );
				}
				else
				{
					XMGAssert ( arr_coord = (XMGVector2X *) kdMalloc ( sizeof ( XMGVector2X ) * facet->m_num_vertex ) );

					for ( idx_coord = 0, num_coord = facet->m_num_vertex; idx_coord < num_coord; idx_coord++ )
					{
						arr_coord[ idx_coord ] = src_arr_coord[ off_src + idx_coord ];
					}

					impl->SetBuffer ( impl->m_id_coords[ unit ], arr_coord, sizeof ( XMGVector2X ) * facet->m_off_vertex, sizeof ( XMGVector2X ) * facet->m_num_vertex );
					kdFree ( arr_coord );
				}

				off_src += facet->m_num_vertex;
			}
		}
	}	
}

void XMGRender::SetCoordArray ( const XMGVector2X* src_arr_coord, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_coord;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_coord;

	XMGVector2F*	arr_coord;

	GLuint			off_src;

	if ( src_idx_shape == XMG_SHAPE_ALL && src_idx_facet == XMG_FACET_ALL )
	{
		impl->m_type_coords[ unit ] = XMG_TYPE_FIXED;
		impl->GenBuffer ( impl->m_id_coords[ unit ], sizeof ( XMGVector2X ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		impl->SetBuffer ( impl->m_id_coords[ unit ], src_arr_coord, 0, sizeof ( XMGVector2X ) * impl->m_num_vertex );
	}
	else
	{
		if ( impl->m_type_coords[ unit ] == 0 )
		{
			impl->m_type_coords[ unit ] = XMG_TYPE_FIXED;
			impl->GenBuffer ( impl->m_id_coords[ unit ], sizeof ( XMGVector2X ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		}

		off_src = 0;
		for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
		{
			shape = impl->m_vec_shape[ idx_shape ];
			
			for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
			{
				facet = shape->m_vec_facet[ idx_facet ];

				if ( impl->m_type_coords[ unit ] == XMG_TYPE_FIXED )
				{
					impl->SetBuffer ( impl->m_id_coords[ unit ], &src_arr_coord[ off_src ], sizeof ( XMGVector2X ) * facet->m_off_vertex, sizeof ( XMGVector2X ) * facet->m_num_vertex );
				}
				else
				{
					XMGAssert ( arr_coord = (XMGVector2F *) kdMalloc ( sizeof ( XMGVector2F ) * facet->m_num_vertex ) );

					for ( idx_coord = 0, num_coord = facet->m_num_vertex; idx_coord < num_coord; idx_coord++ )
					{
						arr_coord[ idx_coord ] = src_arr_coord[ off_src + idx_coord ];
					}

					impl->SetBuffer ( impl->m_id_coords[ unit ], arr_coord, sizeof ( XMGVector2F ) * facet->m_off_vertex, sizeof ( XMGVector2F ) * facet->m_num_vertex );
					kdFree ( arr_coord );
				}

				off_src += facet->m_num_vertex;
			}
		}
	}	
}

void XMGRender::SetColorArray ( const XMGColorF* src_arr_color, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_color;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_color;

	XMGColorX*		arr_color;

	GLuint			off_src;

	if ( src_idx_shape == XMG_SHAPE_ALL && src_idx_facet == XMG_FACET_ALL )
	{
		impl->m_type_colors = XMG_TYPE_FLOAT;
		impl->GenBuffer ( impl->m_id_colors, sizeof ( XMGColorF ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		impl->SetBuffer ( impl->m_id_colors, src_arr_color, 0, sizeof ( XMGColorF ) * impl->m_num_vertex );
	}
	else
	{
		if ( impl->m_id_colors == 0 )
		{
			impl->m_type_colors = XMG_TYPE_FLOAT;
			impl->GenBuffer ( impl->m_id_colors, sizeof ( XMGColorF ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		}

		off_src = 0;
		for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
		{
			shape = impl->m_vec_shape[ idx_shape ];
			
			for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
			{
				facet = shape->m_vec_facet[ idx_facet ];

				if ( impl->m_type_colors == XMG_TYPE_FLOAT )
				{
					impl->SetBuffer ( impl->m_id_colors, &src_arr_color[ off_src ], sizeof ( XMGColorF ) * facet->m_off_vertex, sizeof ( XMGColorF ) * facet->m_num_vertex );
				}
				else
				{
					XMGAssert ( arr_color = (XMGColorX *) kdMalloc ( sizeof ( XMGColorX ) * facet->m_num_vertex ) );

					for ( idx_color = 0, num_color = facet->m_num_vertex; idx_color < num_color; idx_color++ )
					{
						arr_color[ idx_color ] = src_arr_color[ off_src + idx_color ];
					}

					impl->SetBuffer ( impl->m_id_colors, arr_color, sizeof ( XMGColorX ) * facet->m_off_vertex, sizeof ( XMGColorX ) * facet->m_num_vertex );
					kdFree ( arr_color );
				}

				off_src += facet->m_num_vertex;
			}
		}
	}	
}

void XMGRender::SetColorArray ( const XMGColorX* src_arr_color, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_color;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_color;

	XMGColorF*		arr_color;

	GLuint			off_src;

	if ( src_idx_shape == XMG_SHAPE_ALL && src_idx_facet == XMG_FACET_ALL )
	{
		impl->m_type_colors = XMG_TYPE_FIXED;
		impl->GenBuffer ( impl->m_id_colors, sizeof ( XMGColorF ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		impl->SetBuffer ( impl->m_id_colors, src_arr_color, 0, sizeof ( XMGColorF ) * impl->m_num_vertex );
	}
	else
	{
		if ( impl->m_id_colors == 0 )
		{
			impl->m_type_colors = XMG_TYPE_FIXED;
			impl->GenBuffer ( impl->m_id_colors, sizeof ( XMGColorF ) * ( impl->m_num_vertex + impl->m_ext_vertex ) );
		}

		off_src = 0;
		for ( impl->RangeArray ( src_idx_shape, impl->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
		{
			shape = impl->m_vec_shape[ idx_shape ];
			
			for ( impl->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
			{
				facet = shape->m_vec_facet[ idx_facet ];

				if ( impl->m_type_colors == XMG_TYPE_FIXED )
				{
					impl->SetBuffer ( impl->m_id_colors, &src_arr_color[ off_src ], sizeof ( XMGColorX ) * facet->m_off_vertex, sizeof ( XMGColorX ) * facet->m_num_vertex );
				}
				else
				{
					XMGAssert ( arr_color = (XMGColorF *) kdMalloc ( sizeof ( XMGColorF ) * facet->m_num_vertex ) );

					for ( idx_color = 0, num_color = facet->m_num_vertex; idx_color < num_color; idx_color++ )
					{
						arr_color[ idx_color ] = src_arr_color[ off_src + idx_color ];
					}

					impl->SetBuffer ( impl->m_id_colors, arr_color, sizeof ( XMGColorF ) * facet->m_off_vertex, sizeof ( XMGColorF ) * facet->m_num_vertex );
					kdFree ( arr_color );
				}

				off_src += facet->m_num_vertex;
			}
		}
	}
	
}

void XMGRender::SetIndexArray ( const GLushort* arr_index, const GLuint* arr_num_index )
{
	XMGRenderImpl*	impl = (XMGRenderImpl *) m_impl;

	GLuint			idx_shape = 0;
	GLuint			idx_facet = 0;

	GLuint			num_shape = 0;	
	GLuint			num_facet = 0;
	GLuint			num_index = 0;

	XMGShape*		shape;
	XMGFacet*		facet;

	for ( idx_shape = 0, num_shape = impl->m_vec_shape.size ( ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl->m_vec_shape[ idx_shape ];

		for ( idx_facet = 0, num_facet = shape->m_vec_facet.size ( ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_shape ];

			facet->m_off_index = num_index;
			facet->m_num_index = arr_num_index[ idx_shape ];

			num_index += arr_num_index[ idx_shape ];
		}		
	}

	impl->GenBuffer ( impl->m_id_index, sizeof ( GLushort ) * num_index, GL_ELEMENT_ARRAY_BUFFER );
	impl->SetBuffer ( impl->m_id_index, arr_index, 0, sizeof ( GLushort ) * num_index );
}