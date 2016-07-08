/*
 *
 *      File            XMGPolygon.cpp
 *      Description     Polygon implemation
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

static void callback_tess_error ( GLenum error )
{	
	kdLogMessagefKHR ( "Tessellation Error : %s", (const KDchar *) gluErrorString ( error ) );
}

static void callback_tess_begin ( GLenum mode, GLvoid* data )
{
	XMGTess*		tess = (XMGTess *) data;

	tess->m_vec_mode.push_back ( mode );
	tess->m_vec_num_vertex.push_back ( 0 );	
}

static void callback_tess_vertex ( GLvoid* vtx, GLvoid* data )
{
	XMGTess*		tess = (XMGTess *) data;
	GLfloat*		tess_vertex = (GLfloat *) vtx;

	XMGVector3F		vertex;
	XMGVector2F		coord;
	XMGColorF		color;

	switch ( tess->m_tess_mode )
	{
		case XMG_TESS_VERTEX :

			vertex.m_x = tess_vertex[ 0 ];
			vertex.m_y = tess_vertex[ 1 ];
			vertex.m_z = tess_vertex[ 2 ];

			tess->m_vec_vertex.push_back ( vertex );

			break;

		case XMG_TESS_COLOR :

			color.m_r = tess_vertex[ 3 ];
			color.m_g = tess_vertex[ 4 ];
			color.m_b = tess_vertex[ 5 ];
			color.m_a = 1.0f;

			tess->m_vec_color.push_back ( color );

			break;

		case XMG_TESS_COORD :

			coord.m_x = tess_vertex[ 6 ];
			coord.m_y = tess_vertex[ 7 ];

			tess->m_vec_coord.push_back ( coord );

			break;
	}

	tess->m_vec_num_vertex[ tess->m_vec_num_vertex.size ( ) - 1 ] += 1;
}

static void callback_tess_combine ( GLfloat coords[3], GLfloat* vertex_data[4], GLfloat weight[4], GLfloat** data_out, GLvoid* data )
{
	XMGTess*		tess = (XMGTess *) data;
	
	GLfloat*		vertex;
	GLuint			idx;
	
	vertex = (GLfloat *) kdMalloc ( sizeof ( GLfloat ) * 8 );
	
	for ( idx = 0; idx < 3; idx++ )
	{
		vertex[ idx ] = coords[ idx ];
	}
	
	switch ( tess->m_tess_mode )
	{
		case XMG_TESS_COLOR :
			
			for ( idx = 3; idx < 6; idx++ )
			{
				vertex[ idx ] = weight[ 0 ] * vertex_data[ 0 ][ idx ] +
		 						weight[ 1 ] * vertex_data[ 1 ][ idx ] +
								weight[ 2 ] * vertex_data[ 2 ][ idx ] +
								weight[ 3 ] * vertex_data[ 3 ][ idx	];
			}

			break;

		case XMG_TESS_COORD :

			for ( idx = 6; idx < 8; idx++ )
			{
				vertex[ idx ] = weight[ 0 ] * vertex_data[ 0 ][ idx ] +
		 						weight[ 1 ] * vertex_data[ 1 ][ idx ] +
								weight[ 2 ] * vertex_data[ 2 ][ idx ] +
								weight[ 3 ] * vertex_data[ 3 ][ idx	];
			}

			break;
	}

   *data_out = vertex;
}

XMGPolygonImpl::XMGPolygonImpl ( GLuint num_shape, const GLuint* arr_num_vertex, XMGBool* has_face )
{	
	GLuint		idx_tess;
	GLuint		idx_shape;

	m_num_shape = num_shape;
	
	for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
	{
		m_tess[ idx_tess ] = 0;
		m_data[ idx_tess ] = 0;

		if ( has_face[ idx_tess ] == XMG_TRUE )
		{
			XMGAssert ( m_data[ idx_tess ] = (XMGTess **) kdMalloc ( sizeof ( XMGTess * ) * num_shape ) );
			for ( idx_shape = 0; idx_shape < num_shape; idx_shape++ )
			{
				XMGAssert ( m_data[ idx_tess ][ idx_shape ] = new XMGTess ( arr_num_vertex[ idx_shape ] ) );
			}

			XMGAssert ( m_tess[ idx_tess ] = gluNewTess ( ) );
			gluTessCallback ( m_tess[ idx_tess ], GLU_TESS_ERROR	   , (_GLUfuncptr) &callback_tess_error );
			gluTessCallback ( m_tess[ idx_tess ], GLU_TESS_BEGIN_DATA  , (_GLUfuncptr) &callback_tess_begin );	
			gluTessCallback ( m_tess[ idx_tess ], GLU_TESS_VERTEX_DATA , (_GLUfuncptr) &callback_tess_vertex );
			gluTessCallback ( m_tess[ idx_tess ], GLU_TESS_COMBINE_DATA, (_GLUfuncptr) &callback_tess_combine );
			gluTessProperty ( m_tess[ idx_tess ], GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE );
		}
	}
}

XMGPolygonImpl::~XMGPolygonImpl ( void )
{
	GLuint		idx_tess;
	GLuint		idx_shape;

	for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
	{
		if ( m_tess[ idx_tess ] )
		{
			for ( idx_shape = 0; idx_shape < m_num_shape; idx_shape++ )
			{
				delete m_data[ idx_tess ][ idx_shape ];
			}

			kdFree ( m_data[ idx_tess ] );
			gluDeleteTess ( m_tess[ idx_tess ] );
		}
	}
}

XMGPolygon::XMGPolygon ( GLuint num_shape, const GLuint* arr_num_vertex, XMGGeoType mode ) : XMGRender ( num_shape, arr_num_vertex, mode )
{	
	XMGRenderImpl*		impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGAssert ( m_impl = new XMGPolygonImpl ( num_shape, arr_num_vertex, impl_parent->m_has_face ) );

	XMGRender::SetDispLine ( XMG_DISP_LINE_LOOP );
}

XMGPolygon::~XMGPolygon ( void )
{
	delete (XMGPolygonImpl *) m_impl;
}

void XMGPolygon::SetVertexArray ( const XMGVector3F* src_arr_vertex, const GLuint* arr_idx_hole )
{
	XMGPolygonImpl*	impl = (XMGPolygonImpl *) m_impl;
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTess*		data[2];
	XMGShape*		shape;
	XMGFacet*		facet;
	XMGContour*		contour;

	GLuint			idx_tess;
	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_vertex;
	GLuint			idx_vec;
	
	GLuint			num_shape;
	GLuint			num_vertex;
	GLuint			num_vec;

	GLuint			off_vertex;
	GLuint			off_ext;

	XMGVector3F		src_vertex;
	XMGVector3F*	arr_vertex;

	off_vertex = 0;
	off_ext = impl_parent->m_num_vertex;
	impl_parent->m_ext_vertex = 0;
	for ( idx_shape = 0, num_shape = impl_parent->m_vec_shape.size ( ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];
		shape->m_idx_hole = arr_idx_hole ? arr_idx_hole[ idx_shape ] : XMG_HOLE_NULL;

		// Set tess vertices
		if ( impl->m_tess[ 0 ] || impl->m_tess[ 1 ] )
		{
			for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
			{
				if ( impl->m_tess[ idx_tess ] )
				{
					data[ idx_tess ] = impl->m_data[ idx_tess ][ idx_shape ];
					data[ idx_tess ]->Init ( XMG_TESS_VERTEX );

					gluTessBeginPolygon ( impl->m_tess[ idx_tess ], (GLvoid *) data[ idx_tess ] );
					gluTessBeginContour ( impl->m_tess[ idx_tess ] );	
				}
			}

			for ( idx_vertex = 0, num_vertex = shape->m_num_basic; idx_vertex < num_vertex; idx_vertex++ )
			{
				for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
				{
					if ( idx_tess == 0 )
					{
						if ( impl->m_tess[ idx_tess ] )
						{
							src_vertex = src_arr_vertex[ off_vertex + idx_vertex ];
						}
						else
						{
							continue;
						}
					}
					else if ( idx_tess == 1 )
					{
						if ( impl->m_tess[ idx_tess ] )
						{
							switch ( impl_parent->m_geo_type )
							{
								case XMG_GEO_SOLID :

									src_vertex = src_arr_vertex[ off_vertex + num_vertex + idx_vertex ];

									break;

								case XMG_GEO_BSOLID :

									if ( shape->m_idx_hole )
									{
										src_vertex = src_arr_vertex[ idx_vertex < shape->m_idx_hole ? shape->m_idx_hole - idx_vertex - 1 : num_vertex + shape->m_idx_hole - idx_vertex - 1 ];
									}
									else
									{				
										src_vertex = src_arr_vertex[ num_vertex - idx_vertex - 1 ];
									}

									src_vertex.m_z = 0;

									break;
							}							
						}
						else
						{
							continue;
						}
					}

					data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 + 0 ] = src_vertex.m_x;
					data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 + 1 ] = src_vertex.m_y;
					data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 + 2 ] = src_vertex.m_z;	

					if ( shape->m_idx_hole != 0 && shape->m_idx_hole == idx_vertex )
					{
						gluNextContour ( impl->m_tess[ idx_tess ], GLU_INTERIOR );
					}

					gluTessVertex ( impl->m_tess[ idx_tess ], &data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 ], &data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 ] );
				}				
			}

			idx_facet = 0;
			for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
			{
				if ( impl->m_tess[ idx_tess ] )
				{
					gluTessEndContour ( impl->m_tess[ idx_tess ] );	
					gluTessEndPolygon ( impl->m_tess[ idx_tess ] );	

					// Set contours with original
					facet = shape->m_vec_facet[ idx_facet ];
					facet->ClearContour ( );

					if ( shape->m_idx_hole == XMG_HOLE_NULL )
					{
						XMGAssert ( contour = new XMGContour ( ) );

						contour->m_draw_limit = XMG_LIMIT_LINE;
						contour->m_off_vertex = facet->m_off_vertex;
						contour->m_num_vertex = facet->m_num_vertex;

						facet->m_vec_contour.push_back ( contour );
					}
					else
					{
						XMGAssert ( contour = new XMGContour ( ) );

						contour->m_draw_limit = XMG_LIMIT_LINE;
						contour->m_off_vertex = facet->m_off_vertex;
						contour->m_num_vertex = shape->m_idx_hole;						
						
						facet->m_vec_contour.push_back ( contour );

						XMGAssert ( contour = new XMGContour ( ) );

						contour->m_draw_limit = XMG_LIMIT_LINE;
						contour->m_off_vertex = facet->m_off_vertex + shape->m_idx_hole;
						contour->m_num_vertex = facet->m_num_vertex - shape->m_idx_hole;
						
						facet->m_vec_contour.push_back ( contour );
					}
					
					// Set contours with tessed
					facet->m_off_ext = off_ext;
					for ( idx_vec = 0, num_vec = data[ idx_tess ]->m_vec_num_vertex.size ( ); idx_vec < num_vec; idx_vec++ )
					{
						XMGAssert ( contour = new XMGContour ( ) );
						
						contour->m_draw_limit = XMG_LIMIT_TRI;
						contour->m_disp_mode  = data[ idx_tess ]->m_vec_mode[ idx_vec ];
						contour->m_off_vertex = off_ext;
						contour->m_num_vertex = data[ idx_tess ]->m_vec_num_vertex[ idx_vec ];	

						facet->m_vec_contour.push_back ( contour );
						off_ext += contour->m_num_vertex;
					}

					idx_facet++;
					facet->m_num_ext = data[ idx_tess ]->m_vec_vertex.size ( );
					impl_parent->m_ext_vertex += facet->m_num_ext;
				}
			}
		}

		off_vertex += shape->m_num_input;
	}

	XMGRender::SetVertexArray ( src_arr_vertex, XMG_SHAPE_ALL );

	if ( impl->m_tess[ 0 ] || impl->m_tess[ 1 ] )
	{
		off_vertex = 0;
		num_vertex = impl_parent->m_ext_vertex;
		XMGAssert ( arr_vertex = (XMGVector3F *) kdMalloc ( sizeof ( XMGVector3F ) * num_vertex ) );

		for ( idx_shape = 0, num_shape = impl_parent->m_vec_shape.size ( ); idx_shape < num_shape; idx_shape++ )
		{
			for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
			{
				if ( impl->m_tess[ idx_tess ] )
				{
					data[ idx_tess ] = impl->m_data[ idx_tess ][ idx_shape ];
					for ( idx_vertex = 0, num_vertex = data[ idx_tess ]->m_vec_vertex.size ( ); idx_vertex < num_vertex; idx_vertex++ )
					{
						arr_vertex[ off_vertex ] = data[ idx_tess ]->m_vec_vertex[ idx_vertex ];
						off_vertex++;
					}
					data[ idx_tess ]->ClearVector ( );
				}
			}
		}

		impl_parent->SetBuffer ( impl_parent->m_id_vertex, arr_vertex, sizeof ( XMGVector3F ) * impl_parent->m_num_vertex, sizeof ( XMGVector3F ) * impl_parent->m_ext_vertex );
		kdFree ( arr_vertex );
	}
}

void XMGPolygon::SetVertexArray ( const XMGVector3X* src_arr_vertex, const GLuint* arr_idx_hole )
{
	XMGPolygonImpl*	impl = (XMGPolygonImpl *) m_impl;
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTess*		data[2];
	XMGShape*		shape;
	XMGFacet*		facet;
	XMGContour*		contour;

	GLuint			idx_tess;
	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_vertex;
	GLuint			idx_vec;
	
	GLuint			num_shape;
	GLuint			num_vertex;
	GLuint			num_vec;

	GLuint			off_vertex;
	GLuint			off_ext;

	XMGVector3X		src_vertex;
	XMGVector3X*	arr_vertex;

	off_vertex = 0;
	off_ext = impl_parent->m_num_vertex;
	impl_parent->m_ext_vertex = 0;
	for ( idx_shape = 0, num_shape = impl_parent->m_vec_shape.size ( ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];
		shape->m_idx_hole = arr_idx_hole ? arr_idx_hole[ idx_shape ] : XMG_HOLE_NULL;

		// Set tess vertices
		if ( impl->m_tess[ 0 ] || impl->m_tess[ 1 ] )
		{
			for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
			{
				if ( impl->m_tess[ idx_tess ] )
				{
					data[ idx_tess ] = impl->m_data[ idx_tess ][ idx_shape ];
					data[ idx_tess ]->Init ( XMG_TESS_VERTEX );

					gluTessBeginPolygon ( impl->m_tess[ idx_tess ], (GLvoid *) data[ idx_tess ] );
					gluTessBeginContour ( impl->m_tess[ idx_tess ] );	
				}
			}

			for ( idx_vertex = 0, num_vertex = shape->m_num_basic; idx_vertex < num_vertex; idx_vertex++ )
			{
				for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
				{
					if ( idx_tess == 0 )
					{
						if ( impl->m_tess[ idx_tess ] )
						{
							src_vertex = src_arr_vertex[ off_vertex + idx_vertex ];
						}
						else
						{
							continue;
						}
					}
					else if ( idx_tess == 1 )
					{
						if ( impl->m_tess[ idx_tess ] )
						{
							switch ( impl_parent->m_geo_type )
							{
								case XMG_GEO_SOLID :

									src_vertex = src_arr_vertex[ off_vertex + num_vertex + idx_vertex ];

									break;

								case XMG_GEO_BSOLID :

									if ( shape->m_idx_hole )
									{
										src_vertex = src_arr_vertex[ idx_vertex < shape->m_idx_hole ? shape->m_idx_hole - idx_vertex - 1 : num_vertex + shape->m_idx_hole - idx_vertex - 1 ];
									}
									else
									{				
										src_vertex = src_arr_vertex[ num_vertex - idx_vertex - 1 ];
									}

									src_vertex.m_z = 0;

									break;
							}							
						}
						else
						{
							continue;
						}
					}

					data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 + 0 ] = XMG_X2F ( src_vertex.m_x );
					data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 + 1 ] = XMG_X2F ( src_vertex.m_y );
					data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 + 2 ] = XMG_X2F ( src_vertex.m_z );	

					if ( shape->m_idx_hole != 0 && shape->m_idx_hole == idx_vertex )
					{
						gluNextContour ( impl->m_tess[ idx_tess ], GLU_INTERIOR );
					}

					gluTessVertex ( impl->m_tess[ idx_tess ], &data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 ], &data[ idx_tess ]->m_arr_vertex[ idx_vertex * 8 ] );
				}				
			}

			idx_facet = 0;
			for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
			{
				if ( impl->m_tess[ idx_tess ] )
				{
					gluTessEndContour ( impl->m_tess[ idx_tess ] );	
					gluTessEndPolygon ( impl->m_tess[ idx_tess ] );	

					// Set contours with original
					facet = shape->m_vec_facet[ idx_facet ];
					facet->ClearContour ( );

					if ( shape->m_idx_hole == XMG_HOLE_NULL )
					{
						XMGAssert ( contour = new XMGContour ( ) );

						contour->m_draw_limit = XMG_LIMIT_LINE;
						contour->m_off_vertex = facet->m_off_vertex;
						contour->m_num_vertex = facet->m_num_vertex;

						facet->m_vec_contour.push_back ( contour );
					}
					else
					{
						XMGAssert ( contour = new XMGContour ( ) );

						contour->m_draw_limit = XMG_LIMIT_LINE;
						contour->m_off_vertex = facet->m_off_vertex;
						contour->m_num_vertex = shape->m_idx_hole;						
						
						facet->m_vec_contour.push_back ( contour );

						XMGAssert ( contour = new XMGContour ( ) );

						contour->m_draw_limit = XMG_LIMIT_LINE;
						contour->m_off_vertex = facet->m_off_vertex + shape->m_idx_hole;
						contour->m_num_vertex = facet->m_num_vertex - shape->m_idx_hole;
						
						facet->m_vec_contour.push_back ( contour );
					}
					
					// Set contours with tessed
					facet->m_off_ext = off_ext;
					for ( idx_vec = 0, num_vec = data[ idx_tess ]->m_vec_num_vertex.size ( ); idx_vec < num_vec; idx_vec++ )
					{
						XMGAssert ( contour = new XMGContour ( ) );
						
						contour->m_draw_limit = XMG_LIMIT_TRI;
						contour->m_disp_mode  = data[ idx_tess ]->m_vec_mode[ idx_vec ];
						contour->m_off_vertex = off_ext;
						contour->m_num_vertex = data[ idx_tess ]->m_vec_num_vertex[ idx_vec ];	

						facet->m_vec_contour.push_back ( contour );
						off_ext += contour->m_num_vertex;
					}
					
					idx_facet++;
					facet->m_num_ext = data[ idx_tess ]->m_vec_vertex.size ( );
					impl_parent->m_ext_vertex += facet->m_num_ext;
				}
			}
		}

		off_vertex += shape->m_num_input;
	}

	XMGRender::SetVertexArray ( src_arr_vertex, XMG_SHAPE_ALL );

	if ( impl->m_tess[ 0 ] || impl->m_tess[ 1 ] )
	{
		off_vertex = 0;
		num_vertex = impl_parent->m_ext_vertex;
		XMGAssert ( arr_vertex = (XMGVector3X *) kdMalloc ( sizeof ( XMGVector3X ) * num_vertex ) );

		for ( idx_shape = 0, num_shape = impl_parent->m_vec_shape.size ( ); idx_shape < num_shape; idx_shape++ )
		{
			for ( idx_tess = 0; idx_tess < 2; idx_tess++ )
			{
				if ( impl->m_tess[ idx_tess ] )
				{
					data[ idx_tess ] = impl->m_data[ idx_tess ][ idx_shape ];
					for ( idx_vertex = 0, num_vertex = data[ idx_tess ]->m_vec_vertex.size ( ); idx_vertex < num_vertex; idx_vertex++ )
					{
						arr_vertex[ off_vertex ] = data[ idx_tess ]->m_vec_vertex[ idx_vertex ];
						off_vertex++;
					}
					data[ idx_tess ]->ClearVector ( );
				}
			}
		}

		impl_parent->SetBuffer ( impl_parent->m_id_vertex, arr_vertex, sizeof ( XMGVector3X ) * impl_parent->m_num_vertex, sizeof ( XMGVector3X ) * impl_parent->m_ext_vertex );
		kdFree ( arr_vertex );
	}
}

void XMGPolygon::SetColorArray ( const XMGColorF* src_arr_color, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGPolygonImpl*	impl = (XMGPolygonImpl *) m_impl;
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTess*		data;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_tess;
	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_vertex;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_vertex;

	GLuint			off_src;

	XMGColorF*		farr_color;
	XMGColorX*		xarr_color;

	XMGRender::SetColorArray ( src_arr_color, src_idx_shape, src_idx_facet );

	if ( impl_parent->m_has_face[ 0 ] == XMG_FALSE && impl_parent->m_has_face[ 1 ] )
	{
		return;
	}

	if ( src_idx_facet != XMG_FACET_ALL )
	{
		shape = impl_parent->m_vec_shape[ src_idx_facet ];
		facet = shape->m_vec_facet[ src_idx_facet ];
		if ( facet->m_vec_contour.size ( ) == 0 )
		{
			return;
		}
	}

	off_src = 0;
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];
		
		for ( impl_parent->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			if ( idx_facet < 2 )
			{
				switch ( impl_parent->m_geo_type )
				{
					case XMG_GEO_CONE	:
					case XMG_GEO_CWALL	:	idx_tess = 1;			break;
					default				:	idx_tess = idx_facet;	break;
				}

				if ( impl->m_tess[ idx_tess ] )
				{
					data = impl->m_data[ idx_tess ][ idx_shape ];
					data->Init ( XMG_TESS_COLOR );

					gluTessBeginPolygon ( impl->m_tess[ idx_tess ], (GLvoid *) data );
					gluTessBeginContour ( impl->m_tess[ idx_tess ] );	

					for ( idx_vertex = 0, num_vertex = shape->m_num_basic; idx_vertex < num_vertex; idx_vertex++ )
					{
						data->m_arr_vertex[ idx_vertex * 8 + 3 ] = src_arr_color[ off_src + idx_vertex ].m_r;
						data->m_arr_vertex[ idx_vertex * 8 + 4 ] = src_arr_color[ off_src + idx_vertex ].m_g;
						data->m_arr_vertex[ idx_vertex * 8 + 5 ] = src_arr_color[ off_src + idx_vertex ].m_b;

						if ( shape->m_idx_hole != 0 && shape->m_idx_hole == idx_vertex )
						{
							gluNextContour ( impl->m_tess[ idx_tess ], GLU_INTERIOR );
						}

						gluTessVertex ( impl->m_tess[ idx_tess ], &data->m_arr_vertex[ idx_vertex * 8 ], &data->m_arr_vertex[ idx_vertex * 8 ] );
					}

					gluTessEndContour ( impl->m_tess[ idx_tess ] );
					gluTessEndPolygon ( impl->m_tess[ idx_tess ] );

					if ( impl_parent->m_type_colors == XMG_TYPE_FLOAT )
					{
						XMGAssert ( farr_color = (XMGColorF *) kdMalloc ( sizeof ( XMGColorF ) * facet->m_num_ext ) );

						for ( idx_vertex = 0, num_vertex = facet->m_num_ext; idx_vertex < num_vertex; idx_vertex++ )
						{
							farr_color[ idx_vertex ] = data->m_vec_color[ idx_vertex ];
						}

						impl_parent->SetBuffer ( impl_parent->m_id_colors, farr_color, sizeof ( XMGColorF ) * facet->m_off_ext, sizeof ( XMGColorF ) * facet->m_num_ext );
						kdFree ( farr_color );
					}
					else
					{
						XMGAssert ( xarr_color = (XMGColorX *) kdMalloc ( sizeof ( XMGColorX ) * facet->m_num_ext ) );

						for ( idx_vertex = 0, num_vertex = facet->m_num_ext; idx_vertex < num_vertex; idx_vertex++ )
						{
							xarr_color[ idx_vertex ] = data->m_vec_color[ idx_vertex ];
						}

						impl_parent->SetBuffer ( impl_parent->m_id_colors, xarr_color, sizeof ( XMGColorX ) * facet->m_off_ext, sizeof ( XMGColorX ) * facet->m_num_ext );
						kdFree ( xarr_color );
					}
				}			
			}

			off_src += facet->m_num_vertex;
		}
	}
}

void XMGPolygon::SetColorArray ( const XMGColorX* src_arr_color, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGPolygonImpl*	impl = (XMGPolygonImpl *) m_impl;
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTess*		data;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_tess;
	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_vertex;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_vertex;

	GLuint			off_src;

	XMGColorF*		farr_color;
	XMGColorX*		xarr_color;

	XMGRender::SetColorArray ( src_arr_color, src_idx_shape, src_idx_facet );

	if ( impl_parent->m_has_face[ 0 ] == XMG_FALSE && impl_parent->m_has_face[ 1 ] )
	{
		return;
	}

	if ( src_idx_facet != XMG_FACET_ALL )
	{
		shape = impl_parent->m_vec_shape[ src_idx_facet ];
		facet = shape->m_vec_facet[ src_idx_facet ];
		if ( facet->m_vec_contour.size ( ) == 0 )
		{
			return;
		}
	}

	off_src = 0;
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];
		
		for ( impl_parent->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			if ( idx_facet < 2 )
			{
				switch ( impl_parent->m_geo_type )
				{
					case XMG_GEO_CONE	:
					case XMG_GEO_CWALL	:	idx_tess = 1;			break;
					default				:	idx_tess = idx_facet;	break;
				}

				if ( impl->m_tess[ idx_tess ] )
				{
					data = impl->m_data[ idx_tess ][ idx_shape ];
					data->Init ( XMG_TESS_COLOR );

					gluTessBeginPolygon ( impl->m_tess[ idx_tess ], (GLvoid *) data );
					gluTessBeginContour ( impl->m_tess[ idx_tess ] );	

					for ( idx_vertex = 0, num_vertex = shape->m_num_basic; idx_vertex < num_vertex; idx_vertex++ )
					{
						data->m_arr_vertex[ idx_vertex * 8 + 3 ] = XMG_X2F ( src_arr_color[ off_src + idx_vertex ].m_r );
						data->m_arr_vertex[ idx_vertex * 8 + 4 ] = XMG_X2F ( src_arr_color[ off_src + idx_vertex ].m_g );
						data->m_arr_vertex[ idx_vertex * 8 + 5 ] = XMG_X2F ( src_arr_color[ off_src + idx_vertex ].m_b );

						if ( shape->m_idx_hole != 0 && shape->m_idx_hole == idx_vertex )
						{
							gluNextContour ( impl->m_tess[ idx_tess ], GLU_INTERIOR );
						}

						gluTessVertex ( impl->m_tess[ idx_tess ], &data->m_arr_vertex[ idx_vertex * 8 ], &data->m_arr_vertex[ idx_vertex * 8 ] );
					}

					gluTessEndContour ( impl->m_tess[ idx_tess ] );
					gluTessEndPolygon ( impl->m_tess[ idx_tess ] );

					if ( impl_parent->m_type_colors == XMG_TYPE_FLOAT )
					{
						XMGAssert ( farr_color = (XMGColorF *) kdMalloc ( sizeof ( XMGColorF ) * facet->m_num_ext ) );

						for ( idx_vertex = 0, num_vertex = facet->m_num_ext; idx_vertex < num_vertex; idx_vertex++ )
						{
							farr_color[ idx_vertex ] = data->m_vec_color[ idx_vertex ];
						}

						impl_parent->SetBuffer ( impl_parent->m_id_colors, farr_color, sizeof ( XMGColorF ) * facet->m_off_ext, sizeof ( XMGColorF ) * facet->m_num_ext );
						kdFree ( farr_color );
					}
					else
					{
						XMGAssert ( xarr_color = (XMGColorX *) kdMalloc ( sizeof ( XMGColorX ) * facet->m_num_ext ) );

						for ( idx_vertex = 0, num_vertex = facet->m_num_ext; idx_vertex < num_vertex; idx_vertex++ )
						{
							xarr_color[ idx_vertex ] = data->m_vec_color[ idx_vertex ];
						}

						impl_parent->SetBuffer ( impl_parent->m_id_colors, xarr_color, sizeof ( XMGColorX ) * facet->m_off_ext, sizeof ( XMGColorX ) * facet->m_num_ext );
						kdFree ( xarr_color );
					}
				}			
			}

			off_src += facet->m_num_vertex;
		}
	}
}

void XMGPolygon::SetCoordArray ( const XMGVector2F* src_arr_coord, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGPolygonImpl*	impl = (XMGPolygonImpl *) m_impl;
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTess*		data;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_tess;
	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_vertex;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_vertex;

	GLuint			off_src;

	XMGVector2F*	farr_coord;
	XMGVector2X*	xarr_coord;

	XMGRender::SetCoordArray ( src_arr_coord, unit, src_idx_shape, src_idx_facet );

	if ( impl_parent->m_has_face[ 0 ] == XMG_FALSE && impl_parent->m_has_face[ 1 ] )
	{
		return;
	}

	if ( src_idx_facet != XMG_FACET_ALL )
	{
		shape = impl_parent->m_vec_shape[ src_idx_facet ];
		facet = shape->m_vec_facet[ src_idx_facet ];
		if ( facet->m_vec_contour.size ( ) == 0 )
		{
			return;
		}
	}

	off_src = 0;
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];
		
		for ( impl_parent->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			if ( idx_facet < 2 )
			{
				switch ( impl_parent->m_geo_type )
				{
					case XMG_GEO_CONE	:
					case XMG_GEO_CWALL	:	idx_tess = 1;			break;
					default				:	idx_tess = idx_facet;	break;
				}

				if ( impl->m_tess[ idx_tess ] )
				{
					data = impl->m_data[ idx_tess ][ idx_shape ];
					data->Init ( XMG_TESS_COORD );

					gluTessBeginPolygon ( impl->m_tess[ idx_tess ], (GLvoid *) data );
					gluTessBeginContour ( impl->m_tess[ idx_tess ] );	

					for ( idx_vertex = 0, num_vertex = shape->m_num_basic; idx_vertex < num_vertex; idx_vertex++ )
					{
						data->m_arr_vertex[ idx_vertex * 8 + 6 ] = src_arr_coord[ off_src + idx_vertex ].m_x;
						data->m_arr_vertex[ idx_vertex * 8 + 7 ] = src_arr_coord[ off_src + idx_vertex ].m_y;

						if ( shape->m_idx_hole != 0 && shape->m_idx_hole == idx_vertex )
						{
							gluNextContour ( impl->m_tess[ idx_tess ], GLU_INTERIOR );
						}

						gluTessVertex ( impl->m_tess[ idx_tess ], &data->m_arr_vertex[ idx_vertex * 8 ], &data->m_arr_vertex[ idx_vertex * 8 ] );
					}

					gluTessEndContour ( impl->m_tess[ idx_tess ] );
					gluTessEndPolygon ( impl->m_tess[ idx_tess ] );

					if ( impl_parent->m_type_coords[ unit ] == XMG_TYPE_FLOAT )
					{
						XMGAssert ( farr_coord = (XMGVector2F *) kdMalloc ( sizeof ( XMGVector2F ) * facet->m_num_ext ) );

						for ( idx_vertex = 0, num_vertex = facet->m_num_ext; idx_vertex < num_vertex; idx_vertex++ )
						{
							farr_coord[ idx_vertex ] = data->m_vec_coord[ idx_vertex ];
						}

						impl_parent->SetBuffer ( impl_parent->m_id_coords[ unit ], farr_coord, sizeof ( XMGVector2F ) * facet->m_off_ext, sizeof ( XMGVector2F ) * facet->m_num_ext );
						kdFree ( farr_coord );
					}
					else
					{
						XMGAssert ( xarr_coord = (XMGVector2X *) kdMalloc ( sizeof ( XMGVector2X ) * facet->m_num_ext ) );

						for ( idx_vertex = 0, num_vertex = facet->m_num_ext; idx_vertex < num_vertex; idx_vertex++ )
						{
							xarr_coord[ idx_vertex ] = data->m_vec_coord[ idx_vertex ];
						}

						impl_parent->SetBuffer ( impl_parent->m_id_coords[ unit ], xarr_coord, sizeof ( XMGVector2X ) * facet->m_off_ext, sizeof ( XMGVector2X ) * facet->m_num_ext );
						kdFree ( xarr_coord );
					}
				}			
			}

			off_src += facet->m_num_vertex;
		}
	}
}

void XMGPolygon::SetCoordArray ( const XMGVector2X* src_arr_coord, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGPolygonImpl*	impl = (XMGPolygonImpl *) m_impl;
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTess*		data;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_tess;
	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_vertex;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_vertex;

	GLuint			off_src;

	XMGVector2F*	farr_coord;
	XMGVector2X*	xarr_coord;

	XMGRender::SetCoordArray ( src_arr_coord, unit, src_idx_shape, src_idx_facet );	

	if ( impl_parent->m_has_face[ 0 ] == XMG_FALSE && impl_parent->m_has_face[ 1 ] )
	{
		return;
	}

	if ( src_idx_facet != XMG_FACET_ALL )
	{
		shape = impl_parent->m_vec_shape[ src_idx_facet ];
		facet = shape->m_vec_facet[ src_idx_facet ];
		if ( facet->m_vec_contour.size ( ) == 0 )
		{
			return;
		}
	}

	off_src = 0;
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];
		
		for ( impl_parent->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			if ( idx_facet < 2 )
			{
				switch ( impl_parent->m_geo_type )
				{
					case XMG_GEO_CONE	:
					case XMG_GEO_CWALL	:	idx_tess = 1;			break;
					default				:	idx_tess = idx_facet;	break;
				}

				if ( impl->m_tess[ idx_tess ] )
				{
					data = impl->m_data[ idx_tess ][ idx_shape ];
					data->Init ( XMG_TESS_COORD );

					gluTessBeginPolygon ( impl->m_tess[ idx_tess ], (GLvoid *) data );
					gluTessBeginContour ( impl->m_tess[ idx_tess ] );	

					for ( idx_vertex = 0, num_vertex = shape->m_num_basic; idx_vertex < num_vertex; idx_vertex++ )
					{
						data->m_arr_vertex[ idx_vertex * 8 + 6 ] = XMG_X2F ( src_arr_coord[ off_src + idx_vertex ].m_x );
						data->m_arr_vertex[ idx_vertex * 8 + 7 ] = XMG_X2F ( src_arr_coord[ off_src + idx_vertex ].m_y );

						if ( shape->m_idx_hole != 0 && shape->m_idx_hole == idx_vertex )
						{
							gluNextContour ( impl->m_tess[ idx_tess ], GLU_INTERIOR );
						}

						gluTessVertex ( impl->m_tess[ idx_tess ], &data->m_arr_vertex[ idx_vertex * 8 ], &data->m_arr_vertex[ idx_vertex * 8 ] );
					}

					gluTessEndContour ( impl->m_tess[ idx_tess ] );
					gluTessEndPolygon ( impl->m_tess[ idx_tess ] );

					if ( impl_parent->m_type_coords[ unit ] == XMG_TYPE_FLOAT )
					{
						XMGAssert ( farr_coord = (XMGVector2F *) kdMalloc ( sizeof ( XMGVector2F ) * facet->m_num_ext ) );

						for ( idx_vertex = 0, num_vertex = facet->m_num_ext; idx_vertex < num_vertex; idx_vertex++ )
						{
							farr_coord[ idx_vertex ] = data->m_vec_coord[ idx_vertex ];
						}

						impl_parent->SetBuffer ( impl_parent->m_id_coords[ unit ], farr_coord, sizeof ( XMGVector2F ) * facet->m_off_ext, sizeof ( XMGVector2F ) * facet->m_num_ext );
						kdFree ( farr_coord );
					}
					else
					{
						XMGAssert ( xarr_coord = (XMGVector2X *) kdMalloc ( sizeof ( XMGVector2X ) * facet->m_num_ext ) );

						for ( idx_vertex = 0, num_vertex = facet->m_num_ext; idx_vertex < num_vertex; idx_vertex++ )
						{
							xarr_coord[ idx_vertex ] = data->m_vec_coord[ idx_vertex ];
						}

						impl_parent->SetBuffer ( impl_parent->m_id_coords[ unit ], xarr_coord, sizeof ( XMGVector2X ) * facet->m_off_ext, sizeof ( XMGVector2X ) * facet->m_num_ext );
						kdFree ( xarr_coord );
					}
				}			
			}

			off_src += facet->m_num_vertex;
		}
	}
}

void XMGPolygon::SetTexture	( const XMGVector2* src_arr_coord, const XMGTexture* texture, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;
	XMGTexture*		tex = (XMGTexture *) texture;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			idx_shape;
	GLuint			idx_facet;
	GLuint			idx_vertex;

	GLuint			num_shape;
	GLuint			num_facet;
	GLuint			num_vertex;

	GLuint			off_src;

	XMGVector2X*	arr_coord;

	XMGVector2F		size;

	tex->GetSize ( size );

	off_src = 0;
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];
		
		for ( impl_parent->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			facet = shape->m_vec_facet[ idx_facet ];

			XMGAssert ( arr_coord = (XMGVector2X *) kdMalloc ( sizeof ( XMGVector2X ) * facet->m_num_vertex ) );

			for ( idx_vertex = 0, num_vertex = facet->m_num_vertex; idx_vertex < num_vertex; idx_vertex++ )
			{
				arr_coord[ idx_vertex ].m_x = XMG_F2X ( src_arr_coord[ off_src + idx_vertex ].m_x / size.m_x );
				arr_coord[ idx_vertex ].m_y = XMG_F2X ( src_arr_coord[ off_src + idx_vertex ].m_y / size.m_y );
			}

			SetCoordArray ( arr_coord, unit, idx_shape, idx_facet );
			kdFree ( arr_coord );

			off_src += facet->m_num_vertex;
		}
	}

	XMGRender::SetTexture ( texture, unit, src_idx_shape, src_idx_facet );
	XMGRender::SetDispMode ( XMG_DISP_TEXTURE, src_idx_shape );
}
