/*
 *
 *      File            XMGQuad.cpp
 *      Description     Quadrilateral implemation 
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

XMGQuadImpl::XMGQuadImpl ( XMGGeoType type )
{
	m_geo_type = type;

	m_tex_indices[ 0 ] = 0;
	m_tex_indices[ 1 ] = 1;
	m_tex_indices[ 2 ] = 2;
	m_tex_indices[ 3 ] = 3;
}

XMGQuadImpl::~XMGQuadImpl ( void )
{

}

void XMGQuadImpl::Rect2Vector ( XMGVector3F* vec, XMGRectF& rect, XMGWind wind )
{
	GLuint		idx;

	idx = wind == XMG_WIND_CLOCK ? 3 : 0;

	vec[ idx ].m_x = rect.GetLeft ( );
	vec[ idx ].m_y = rect.GetTop ( );
	vec[ idx ].m_z = rect.m_z;

	idx = wind == XMG_WIND_CLOCK ? 2 : 1;

	vec[ idx ].m_x = rect.GetLeft ( );
	vec[ idx ].m_y = rect.GetBottom ( );
	vec[ idx ].m_z = rect.m_z;

	idx = wind == XMG_WIND_CLOCK ? 1 : 2;

	vec[ idx ].m_x = rect.GetRight ( );
	vec[ idx ].m_y = rect.GetBottom ( );
	vec[ idx ].m_z = rect.m_z;

	idx = wind == XMG_WIND_CLOCK ? 0 : 3;

	vec[ idx ].m_x = rect.GetRight ( );
	vec[ idx ].m_y = rect.GetTop ( );
	vec[ idx ].m_z = rect.m_z;
}

void XMGQuadImpl::Rect2Vector ( XMGVector3X* vec, XMGRectX& rect, XMGWind wind )
{
	GLuint		idx;

	idx = wind == XMG_WIND_CLOCK ? 3 : 0;

	vec[ idx ].m_x = rect.GetLeft ( );
	vec[ idx ].m_y = rect.GetTop ( );
	vec[ idx ].m_z = rect.m_z;

	idx = wind == XMG_WIND_CLOCK ? 2 : 1;

	vec[ idx ].m_x = rect.GetLeft ( );
	vec[ idx ].m_y = rect.GetBottom ( );
	vec[ idx ].m_z = rect.m_z;

	idx = wind == XMG_WIND_CLOCK ? 1 : 2;

	vec[ idx ].m_x = rect.GetRight ( );
	vec[ idx ].m_y = rect.GetBottom ( );
	vec[ idx ].m_z = rect.m_z;

	idx = wind == XMG_WIND_CLOCK ? 0 : 3;

	vec[ idx ].m_x = rect.GetRight ( );
	vec[ idx ].m_y = rect.GetTop ( );
	vec[ idx ].m_z = rect.m_z;
}

XMGQuad::XMGQuad ( GLuint num_shape, XMGGeoType type ) : XMGRender ( num_shape, 4, type )
{
	XMGAssert ( m_impl = new XMGQuadImpl ( type ) );

	XMGRender::SetDispLine ( XMG_DISP_LINE_LOOP );	
	XMGRender::SetDispTri  ( XMG_DISP_TRIANGLE_FAN );	
}

XMGQuad::~XMGQuad ( void )
{
	delete (XMGQuadImpl *) m_impl;
}

void XMGQuad::SetVertexArray ( const XMGRectF* arr_rect, GLuint src_idx_shape )
{
	XMGQuadImpl*	impl = (XMGQuadImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGVector3F*	arr_vertex;

	GLuint			idx_shape;
	GLuint			idx_rect;

	GLuint			num_vertex;
	GLuint			num_shape;

	GLuint			off_vertex;	

	switch ( impl_parent->m_geo_type )
	{
		case XMG_GEO_WALL   :
		case XMG_GEO_PILLAR :
		case XMG_GEO_SOLID	:	num_vertex = 8;		break;
		case XMG_GEO_CONE	: 
		case XMG_GEO_CWALL	:	num_vertex = 5;		break;
		default				:	num_vertex = 4;		break;
	}

	XMGAssert ( arr_vertex = (XMGVector3F *) kdMalloc ( sizeof ( XMGVector3F ) * num_vertex * ( src_idx_shape == XMG_SHAPE_ALL ? impl_parent->m_vec_shape.size ( ) : 1 ) ) );
	
	idx_rect = 0;
	off_vertex = 0;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		impl->Rect2Vector ( &arr_vertex[ off_vertex ], (XMGRectF &) arr_rect[ idx_rect ], XMG_WIND_ANTI_CLOCK );
		
		if ( num_vertex == 8 )
		{
			idx_rect += 1;
			impl->Rect2Vector ( &arr_vertex[ off_vertex + 4 ], (XMGRectF &) arr_rect[ idx_rect ], XMG_WIND_CLOCK );
		}

		off_vertex += num_vertex;

		idx_rect += 1;
	}

	XMGRender::SetVertexArray ( arr_vertex, src_idx_shape );

	kdFree ( arr_vertex );
}

void XMGQuad::SetVertexArray ( const XMGRectX* arr_rect, GLuint src_idx_shape )
{
	XMGQuadImpl*	impl = (XMGQuadImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;
	
	XMGVector3X*	arr_vertex;

	GLuint			idx_shape;
	GLuint			idx_rect;

	GLuint			num_vertex;
	GLuint			num_shape;

	GLuint			off_vertex;	

	switch ( impl_parent->m_geo_type )
	{
		case XMG_GEO_WALL   :
		case XMG_GEO_PILLAR :
		case XMG_GEO_SOLID	:	num_vertex = 8;		break;
		case XMG_GEO_CONE	: 
		case XMG_GEO_CWALL	:	num_vertex = 5;		break;
		default				:	num_vertex = 4;		break;
	}

	XMGAssert ( arr_vertex = (XMGVector3X *) kdMalloc ( sizeof ( XMGVector3X ) * num_vertex * ( src_idx_shape == XMG_SHAPE_ALL ? impl_parent->m_vec_shape.size ( ) : 1 ) ) );
	
	idx_rect = 0;
	off_vertex = 0;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		impl->Rect2Vector ( &arr_vertex[ off_vertex ], (XMGRectX &) arr_rect[ idx_rect ], XMG_WIND_ANTI_CLOCK );
		
		if ( num_vertex == 8 )
		{
			idx_rect += 1;
			impl->Rect2Vector ( &arr_vertex[ off_vertex + 4 ], (XMGRectX &) arr_rect[ idx_rect ], XMG_WIND_CLOCK );
		}

		idx_rect += 1;
		off_vertex += num_vertex;		
	}

	XMGRender::SetVertexArray ( arr_vertex, src_idx_shape );

	kdFree ( arr_vertex );
}

void XMGQuad::SetVertexArray ( const XMGTexture* texture, GLuint src_idx_shape )
{
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTexture*		tex = (XMGTexture *) texture;

	XMGRectX*		arr_rect;

	GLuint			idx_shape;

	GLuint			num_shape;
	GLuint			num_rect;

	GLuint			off_rect;

	XMGVector2X		size;

	tex->GetImageSize( size );

	num_rect = impl_parent->m_geo_type == XMG_GEO_SOLID ? 2 : 1;
	if ( src_idx_shape == XMG_SHAPE_ALL )
	{
		num_rect *= impl_parent->m_vec_shape.size ( );
	}

	XMGAssert ( arr_rect = (XMGRectX *) kdMalloc ( sizeof ( XMGRectX ) * num_rect ) );

	off_rect = 0;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		switch ( impl_parent->m_geo_type )
		{
			case XMG_GEO_REGULAR :

				arr_rect[ off_rect ] = XMGRectX ( 0, 0, 0, size.m_x, size.m_y );

				break;

			case XMG_GEO_SOLID :

				arr_rect[ off_rect ] = XMGRectX ( 0, 0, size.m_y, size.m_x, size.m_y );
				off_rect++;
				arr_rect[ off_rect ] = XMGRectX ( 0, 0, size.m_y, size.m_x, size.m_y );

				break;

			default :

				arr_rect[ off_rect ] = XMGRectX ( 0, 0, size.m_y, size.m_x, size.m_y );

		}

		off_rect++;
		
	}

	SetVertexArray ( arr_rect, src_idx_shape );

	kdFree ( arr_rect );
}

void XMGQuad::SetTexture ( const XMGTexture* texture, XMGFlip flip, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGTexture*		tex = (XMGTexture *) texture;
	XMGVector2X		size;

	tex->GetImageSize ( size );

	if ( flip & XMG_FLIP_X ) { size.m_x = -size.m_x; }
	if ( flip & XMG_FLIP_Y ) { size.m_y = -size.m_y; }

	SetTexture ( texture, XMGRectX ( 0, 0, size.m_x, size.m_y ), unit, src_idx_shape, src_idx_facet );	
}

void XMGQuad::SetTexture ( const XMGTexture* texture, const XMGRectF& offset, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGQuadImpl*	impl = (XMGQuadImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTexture*		tex = (XMGTexture *) texture;
	XMGVector2F		size;
	
	GLuint			idx_shape;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;

	XMGVector2F		arr_coord[4];
	XMGRectF		rect;
	XMGShape*		shape;
	
	tex->GetSize ( size );
		
	rect.m_x = offset.m_x / size.m_x;
	rect.m_y = offset.m_y / size.m_y;
	rect.m_w = kdFabsf ( offset.m_w ) / size.m_x;
	rect.m_h = kdFabsf ( offset.m_h ) / size.m_y;

	arr_coord[ impl->m_tex_indices[ 0 ] ].m_x = offset.m_w < 0 ? rect.GetRight  ( ) : rect.GetLeft   ( );	
	arr_coord[ impl->m_tex_indices[ 0 ] ].m_y = offset.m_h < 0 ? rect.GetBottom ( ) : rect.GetTop    ( );			

	arr_coord[ impl->m_tex_indices[ 1 ] ].m_x = offset.m_w < 0 ? rect.GetRight  ( ) : rect.GetLeft   ( );	
	arr_coord[ impl->m_tex_indices[ 1 ] ].m_y = offset.m_h < 0 ? rect.GetTop    ( ) : rect.GetBottom ( );

	arr_coord[ impl->m_tex_indices[ 2 ] ].m_x = offset.m_w < 0 ? rect.GetLeft   ( ) : rect.GetRight  ( );	
	arr_coord[ impl->m_tex_indices[ 2 ] ].m_y = offset.m_h < 0 ? rect.GetTop    ( ) : rect.GetBottom ( );

	arr_coord[ impl->m_tex_indices[ 3 ] ].m_x = offset.m_w < 0 ? rect.GetLeft   ( ) : rect.GetRight  ( );	
	arr_coord[ impl->m_tex_indices[ 3 ] ].m_y = offset.m_h < 0 ? rect.GetBottom ( ) : rect.GetTop    ( );

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];

		for ( impl_parent->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			XMGRender::SetCoordArray ( arr_coord, unit, idx_shape, idx_facet );
		}
	}

	XMGRender::SetTexture ( texture, unit, src_idx_shape, src_idx_facet );
	XMGRender::SetDispMode ( XMG_DISP_TEXTURE, src_idx_shape );
}

void XMGQuad::SetTexture ( const XMGTexture* texture, const XMGRectX& offset, XMGTexUnit unit, GLuint src_idx_shape, GLuint src_idx_facet )
{
	XMGQuadImpl*	impl = (XMGQuadImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGTexture*		tex = (XMGTexture *) texture;
	XMGVector2F		size;
	
	GLuint			idx_shape;
	GLuint			idx_facet;

	GLuint			num_shape;
	GLuint			num_facet;

	XMGVector2X		arr_coord[4];
	XMGRectX		rect;
	XMGShape*		shape;
	
	tex->GetSize ( size );
		
	rect.m_x = XMG_F2X ( XMG_X2F ( offset.m_x ) / size.m_x );
	rect.m_y = XMG_F2X ( XMG_X2F ( offset.m_y ) / size.m_y );
	rect.m_w = XMG_F2X ( kdFabsf ( XMG_X2F ( offset.m_w ) ) / size.m_x );
	rect.m_h = XMG_F2X ( kdFabsf ( XMG_X2F ( offset.m_h ) ) / size.m_y );

	arr_coord[ impl->m_tex_indices[ 0 ] ].m_x = offset.m_w < 0 ? rect.GetRight  ( ) : rect.GetLeft   ( );	
	arr_coord[ impl->m_tex_indices[ 0 ] ].m_y = offset.m_h < 0 ? rect.GetBottom ( ) : rect.GetTop    ( );			

	arr_coord[ impl->m_tex_indices[ 1 ] ].m_x = offset.m_w < 0 ? rect.GetRight  ( ) : rect.GetLeft   ( );	
	arr_coord[ impl->m_tex_indices[ 1 ] ].m_y = offset.m_h < 0 ? rect.GetTop    ( ) : rect.GetBottom ( );

	arr_coord[ impl->m_tex_indices[ 2 ] ].m_x = offset.m_w < 0 ? rect.GetLeft   ( ) : rect.GetRight  ( );	
	arr_coord[ impl->m_tex_indices[ 2 ] ].m_y = offset.m_h < 0 ? rect.GetTop    ( ) : rect.GetBottom ( );

	arr_coord[ impl->m_tex_indices[ 3 ] ].m_x = offset.m_w < 0 ? rect.GetLeft   ( ) : rect.GetRight  ( );	
	arr_coord[ impl->m_tex_indices[ 3 ] ].m_y = offset.m_h < 0 ? rect.GetBottom ( ) : rect.GetTop    ( );

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];

		for ( impl_parent->RangeArray ( src_idx_facet, shape->m_vec_facet.size ( ), idx_facet, num_facet ); idx_facet < num_facet; idx_facet++ )
		{
			XMGRender::SetCoordArray ( arr_coord, unit, idx_shape, idx_facet );
		}
	}

	XMGRender::SetTexture ( texture, unit, src_idx_shape, src_idx_facet );
	XMGRender::SetDispMode ( XMG_DISP_TEXTURE, src_idx_shape );
}

void XMGQuad::TexIndices ( XMGRotate rot )
{
	XMGQuadImpl*	impl = (XMGQuadImpl *) m_impl;

	GLuint*			indices = impl->m_tex_indices;

	switch ( rot )
	{
		case XMG_ROTATE_0	:	indices[ 0 ] = 0;	indices[ 1 ] = 1;	indices[ 2 ] = 2;	indices[ 3 ] = 3;	break;
		case XMG_ROTATE_90	:	indices[ 0 ] = 3;	indices[ 1 ] = 0;	indices[ 2 ] = 1;	indices[ 3 ] = 2;	break;
		case XMG_ROTATE_180	:	indices[ 0 ] = 2;	indices[ 1 ] = 3;	indices[ 2 ] = 0;	indices[ 3 ] = 1;	break;
		case XMG_ROTATE_270	:	indices[ 0 ] = 1;	indices[ 1 ] = 2;	indices[ 2 ] = 3;	indices[ 3 ] = 0;	break;
	}
}