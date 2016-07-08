/*
 *
 *      File            XMGRipple.cpp
 *      Description     Ripple effect implemation 
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
#include "XMGRippleTable.h"

XMGRippleImpl::XMGRippleImpl ( void )
{
	XMGAssert ( m_arr_coord[ 0 ] = (XMGVector2 *) kdMalloc ( sizeof ( XMGVector2 ) * GRID_SIZE_X * GRID_SIZE_Y ) );
	XMGAssert ( m_arr_coord[ 1 ] = (XMGVector2 *) kdMalloc ( sizeof ( XMGVector2 ) * GRID_SIZE_X * GRID_SIZE_Y ) );
}

XMGRippleImpl::~XMGRippleImpl ( void )
{
	kdFree ( m_arr_coord[ 0 ] );
	kdFree ( m_arr_coord[ 1 ] );
}

GLfloat	XMGRippleImpl::GetDistance ( GLint gx, GLint gy, GLint cx, GLint cy )
{
	return kdSqrtf ( 1.f * ( gx - cx ) * ( gx - cx ) + 1.f * ( gy - cy ) * ( gy - cy ) );
}

GLint XMGRippleImpl::GetMaxDistance ( GLint gx, GLint gy, GLint cx, GLint cy )
{
	GLfloat		dist;
	KDfloat32	temp;
	
	dist = GetDistance ( gx, gy, 0, 0 );
	temp = GetDistance ( gx, gy, GRID_SIZE_X, 0 );
	
	if ( temp > dist )
	{
		dist = temp;
	}

	temp = GetDistance ( gx, gy, GRID_SIZE_X, GRID_SIZE_Y );
	
	if ( temp > dist )
	{
		dist = temp;
	}

	temp = GetDistance ( gx, gy, 0, GRID_SIZE_Y );
	
	if ( temp > dist )
	{
		dist = temp;
	}
	
	return (GLint) ( dist / GRID_SIZE_X ) * cx + RIPPLE_LENGTH / 6;
}

XMGRipple::XMGRipple ( void ) : XMGRender ( 1, (GLuint) GRID_SIZE_X * GRID_SIZE_Y )
{	
	GLushort*		arr_index;
	GLuint			arr_size;

	GLuint			row;
	GLuint			col;
	GLuint			idx;
	
	arr_size = GRID_SIZE_X * GRID_SIZE_Y * 6;

	XMGAssert ( m_impl = new XMGRippleImpl ( ) );	
	XMGAssert ( arr_index = (GLushort *) kdMalloc ( sizeof ( GLushort ) * arr_size ) );
	kdMemset ( arr_index, 0, sizeof ( GLushort ) * arr_size );
	
	for ( col = 0; col < GRID_SIZE_X - 1; col++ )
	{
		for ( row = 0; row < GRID_SIZE_Y - 1; row++ )
		{
			idx = ( col * GRID_SIZE_X + row ) * 6;

			arr_index[ idx + 0 ] = ( col     ) * GRID_SIZE_X + row;
			arr_index[ idx + 1 ] = ( col + 1 ) * GRID_SIZE_X + row;
			arr_index[ idx + 2 ] = ( col + 1 ) * GRID_SIZE_X + row + 1;
			arr_index[ idx + 3 ] = ( col     ) * GRID_SIZE_X + row;
			arr_index[ idx + 4 ] = ( col + 1 ) * GRID_SIZE_X + row + 1;
			arr_index[ idx + 5 ] = ( col     ) * GRID_SIZE_X + row + 1;
		}
	}

	XMGRender::SetIndexArray ( arr_index, &arr_size );

	kdFree ( arr_index );
}

XMGRipple::~XMGRipple ( void )
{
	delete (XMGRippleImpl *) m_impl;
}

void XMGRipple::Render ( void )
{
	XMGRippleImpl*	impl = (XMGRippleImpl *) m_impl;
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGVector2F*	fvertex;
	XMGVector2X*	xvertex;

	GLint			x, y;
	GLint			mi, mj;
	GLint			r;
	GLfloat			sx, sy;
	GLfloat			amp;
	
	GLuint			row;
	GLuint			col;	
	GLuint			idx;
	GLuint			off;

	for ( idx = 0; idx < XMG_RIPPLE_COUNT; idx++ )
	{
		impl->m_data[ idx ].m_t += impl->m_data[ idx ].m_step;
	}

	for ( col = 0; col < GRID_SIZE_X; col++ )
	{
		for ( row = 0; row < GRID_SIZE_Y; row++ )
		{
			off = col * GRID_SIZE_Y + row;

			impl->m_arr_coord[ 1 ][ off ] = impl->m_arr_coord[ 0 ][ off ];
			
			for ( idx = 0; idx < XMG_RIPPLE_COUNT; idx++ )
			{
				x = col - impl->m_data[ idx ].m_cx;
				y = row - impl->m_data[ idx ].m_cy;

				if ( x < 0 )
				{
					x *= -1;
					sx = -1;
				}
				else
				{
					sx = 1;
				}
				
				if ( y < 0 )
				{
					y *= -1;
					sy = -1;
				}
				else
				{
					sy = 1;
				}
				
				mi = x;
				mj = y;
				
				r = impl->m_data[ idx ].m_t - g_ripple_vector[ mi ][ mj ].r;
				
				if ( r < 0 )
				{
					r = 0;
				}
				
				if ( r > RIPPLE_LENGTH - 1 )
				{
					r = RIPPLE_LENGTH - 1;
				}
				
				amp  = 1.f - 1.f * impl->m_data[ idx ].m_t / RIPPLE_LENGTH;
				amp *= amp;
				
				if ( amp < 0 )
				{
					amp = 0;
				}
				
				switch ( impl_parent->m_type_coords[ 0 ] )
				{
					case XMG_TYPE_FLOAT :
					
						fvertex = (XMGVector2F *) &impl->m_arr_coord[ 1 ][ off ];
						fvertex->m_x += g_ripple_vector[ mi ][ mj ].dx[0] * sx * g_ripple_amp[ r ].amplitude * amp;
						fvertex->m_y += g_ripple_vector[ mi ][ mj ].dx[1] * sy * g_ripple_amp[ r ].amplitude * amp;

						break;

					case XMG_TYPE_FIXED :

						xvertex = (XMGVector2X *) &impl->m_arr_coord[ 1 ][ off ];

						xvertex->m_x += XMG_F2X ( g_ripple_vector[ mi ][ mj ].dx[0] * sx * g_ripple_amp[ r ].amplitude * amp );
						xvertex->m_y += XMG_F2X ( g_ripple_vector[ mi ][ mj ].dx[1] * sy * g_ripple_amp[ r ].amplitude * amp );

						break;
				}
			}			
		}
	}

	XMGRender::SetCoordArray ( (XMGVector2F *) impl->m_arr_coord[ 1 ], XMG_TEX_UNIT_0, 0 );
	XMGRender::Render ( );
}

void XMGRipple::SetRipple ( GLint x, GLint y, GLuint step )
{
	XMGRippleImpl*	impl = (XMGRippleImpl *) m_impl;

	GLuint			idx = 0;

	while ( impl->m_data[ idx ].m_t < impl->m_data[ idx ].m_max && idx < XMG_RIPPLE_COUNT )
	{
		idx++;
	}
	
	if ( idx < XMG_RIPPLE_COUNT )
	{
		impl->m_data[ idx ].m_step = step;
		impl->m_data[ idx ].m_t    = 4 * step;
		impl->m_data[ idx ].m_cx   = (GLuint ) ( x / impl->m_size.m_x * GRID_SIZE_X );
		impl->m_data[ idx ].m_cy   = (GLuint ) ( y / impl->m_size.m_y * GRID_SIZE_Y );
		impl->m_data[ idx ].m_max  = impl->GetMaxDistance ( impl->m_data[ idx ].m_cx, impl->m_data[ idx ].m_cy, (GLint) impl->m_size.m_x, (GLint) impl->m_size.m_y );
	}
}

void XMGRipple::SetVertexArray ( GLfloat width, GLfloat height )
{
	XMGRippleImpl*	impl = (XMGRippleImpl *) m_impl;

	XMGVector3F*	arr_vertex;

	GLuint			row;
	GLuint			col;
	GLuint			idx;

	impl->m_size.m_x = width;
	impl->m_size.m_y = height;

	XMGAssert ( arr_vertex = (XMGVector3F *) kdMalloc ( sizeof ( XMGVector3F ) * GRID_SIZE_X * GRID_SIZE_Y ) );

	for ( col = 0; col < GRID_SIZE_X; col++ )
	{
		for ( row = 0; row < GRID_SIZE_Y; row++ )
		{
			idx = col * GRID_SIZE_Y + row;

			arr_vertex[ idx ].m_x = col / ( GRID_SIZE_X - 1.f ) * impl->m_size.m_x;
			arr_vertex[ idx ].m_y = row / ( GRID_SIZE_Y - 1.f ) * impl->m_size.m_y;
			arr_vertex[ idx ].m_z = 0;
		}
	}

	XMGRender::SetVertexArray ( arr_vertex );
	kdFree ( arr_vertex );

	for ( idx = 0; idx < XMG_RIPPLE_COUNT; idx++ )
	{
		impl->m_data[ idx ].m_t   = (KDint) kdSqrtf ( impl->m_size.m_y * impl->m_size.m_y + impl->m_size.m_x * impl->m_size.m_x ) + RIPPLE_LENGTH;
		impl->m_data[ idx ].m_cx  = 0;
		impl->m_data[ idx ].m_cy  = 0;
		impl->m_data[ idx ].m_max = 0;
	}
}

void XMGRipple::SetVertexArray ( GLfixed width, GLfixed height )
{

}

void XMGRipple::SetTexture ( const XMGTexture* texture, XMGFlip flip )
{
	XMGTexture*		tex = (XMGTexture *) texture;
	XMGVector2F		size;

	tex->GetImageSize ( size );

	if ( flip & XMG_FLIP_X ) { size.m_x = -size.m_x; }
	if ( flip & XMG_FLIP_Y ) { size.m_y = -size.m_y; }

	SetTexture ( texture, XMGRectF ( 0, 0, size.m_x, size.m_y ) );	
}

void XMGRipple::SetTexture ( const XMGTexture* texture, const XMGRectF& offset )
{
	XMGRippleImpl*	impl = (XMGRippleImpl *) m_impl;
	XMGRenderImpl*  impl_parent = (XMGRenderImpl *) XMGRender::m_impl;
	XMGTexture*		tex = (XMGTexture *) texture;

	XMGVector2F*	vertex;

	GLuint			dst_row;
	GLuint			dst_col;

	GLuint			row;
	GLuint			col;

	XMGVector2F		size;
	XMGVector2F		rate;

	tex->GetSize ( size );

	rate.m_x = kdFabsf ( offset.m_w ) / size.m_x;
	rate.m_y = kdFabsf ( offset.m_h ) / size.m_y;

	for ( col = 0; col < GRID_SIZE_X; col++ )
	{
		dst_col = offset.m_w < 0 ? GRID_SIZE_X - 1 - col : col;

		for ( row = 0; row < GRID_SIZE_Y; row++ )
		{
			dst_row = offset.m_h < 0 ? GRID_SIZE_X - 1 - row : row;			

			vertex = (XMGVector2F *) &impl->m_arr_coord[ 0 ][ dst_col * GRID_SIZE_Y + dst_row ];

			vertex->m_x = offset.m_x + col / ( GRID_SIZE_X - 1.f ) * rate.m_x;
			vertex->m_y = offset.m_y + row / ( GRID_SIZE_Y - 1.f ) * rate.m_y;
		}
	}

	impl_parent->m_type_coords[ 0 ] = XMG_TYPE_FLOAT;

	XMGRender::SetTexture ( texture );
	XMGRender::SetDispMode ( XMG_DISP_TEXTURE );
}

void XMGRipple::SetTexture ( const XMGTexture* texture, const XMGRectX& offset )
{

}