/*
 *
 *      File            XMGSphere.cpp
 *      Description     Sphere implemation 
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

XMGSphereImpl::XMGSphereImpl ( GLsizei slices, GLsizei stacks )
{
	m_slices = slices;
	m_stacks = stacks;
}

XMGSphereImpl::~XMGSphereImpl ( void )
{

}

void XMGSphereImpl::MakeVertexArray ( XMGVector3F* arr, const XMGVector3F& pos, GLfloat rad, GLsizei stack, GLsizei slice )
{		
	GLfloat		lat		= -90.0f;
	GLfloat		lon		= 0;
	GLfloat		lat_inc = 180.0f / stack;
	GLfloat		lon_inc = 360.0f / slice;

	GLsizei		col     = 0;
	GLsizei		row     = 0;
	GLsizei		idx		= 0;
	GLfloat		phi1	= 0;
	GLfloat		phi2	= 0;
	GLfloat		the1	= 0;
	GLfloat		the2	= 0;	

	for ( col = 0; col < stack; col++ )
	{
		phi1 = XMG_DEG2RAD ( lon + col * lon_inc );
		phi2 = XMG_DEG2RAD ( lon + ( col + 1 ) * lon_inc );

		for ( row = 0; row < slice; row++ )
		{
			the1 = XMG_DEG2RAD ( lat + row * lat_inc );
			the2 = XMG_DEG2RAD ( lat + ( row + 1 ) * lat_inc );

			arr[ idx ].m_x = pos.m_x + rad * kdCosf ( phi1 ) * kdCosf ( the1 );					
			arr[ idx ].m_y = pos.m_y + rad * kdSinf ( the1 );
			arr[ idx ].m_z = pos.m_z + rad * kdSinf ( phi1 ) * kdCosf ( the1 );

			idx++;
 
			arr[ idx ].m_x = pos.m_x + rad * kdCosf ( phi1 ) * kdCosf ( the2 );
			arr[ idx ].m_y = pos.m_y + rad * kdSinf ( the2 );
			arr[ idx ].m_z = pos.m_z + rad * kdSinf ( phi1 ) * kdCosf ( the2 );

			idx++;

			arr[ idx ].m_x = pos.m_x + rad * kdCosf ( phi2 ) * kdCosf ( the2 );
			arr[ idx ].m_y = pos.m_y + rad * kdSinf ( the2 );
			arr[ idx ].m_z = pos.m_z + rad * kdSinf ( phi2 ) * kdCosf ( the2 );

			idx++;

			arr[ idx ].m_x = pos.m_x + rad * kdCosf ( phi1 ) * kdCosf ( the1 );
			arr[ idx ].m_y = pos.m_y + rad * kdSinf ( the1 );
			arr[ idx ].m_z = pos.m_z + rad * kdSinf ( phi1 ) * kdCosf ( the1 );

			idx++;

			arr[ idx ].m_x = pos.m_x + rad * kdCosf ( phi2 ) * kdCosf ( the2 );
			arr[ idx ].m_y = pos.m_y + rad * kdSinf ( the2 );
			arr[ idx ].m_z = pos.m_z + rad * kdSinf ( phi2 ) * kdCosf ( the2 );

			idx++;

			arr[ idx ].m_x = pos.m_x + rad * kdCosf ( phi2 ) * kdCosf ( the1 );
			arr[ idx ].m_y = pos.m_y + rad * kdSinf ( the1 );
			arr[ idx ].m_z = pos.m_z + rad * kdSinf ( phi2 ) * kdCosf ( the1 );

			idx++;
		}
	}	
}

void XMGSphereImpl::MakeVertexArray ( XMGVector3X* arr, const XMGVector3X& pos, GLfixed rad, GLsizei stack, GLsizei slice )
{
	GLfloat		frad    = XMG_X2F ( rad );
	GLfloat		lat		= -90.0f;
	GLfloat		lon		= 0;
	GLfloat		lat_inc = 180.0f / stack;
	GLfloat		lon_inc = 360.0f / slice;

	GLsizei		col     = 0;
	GLsizei		row     = 0;
	GLsizei		idx		= 0;
	GLfloat		phi1	= 0;
	GLfloat		phi2	= 0;
	GLfloat		the1	= 0;
	GLfloat		the2	= 0;	

	for ( col = 0; col < stack; col++ )
	{
		phi1 = XMG_DEG2RAD ( lon + col * lon_inc );
		phi2 = XMG_DEG2RAD ( lon + ( col + 1 ) * lon_inc );

		for ( row = 0; row < slice; row++ )
		{
			the1 = XMG_DEG2RAD ( lat + row * lat_inc );
			the2 = XMG_DEG2RAD ( lat + ( row + 1 ) * lat_inc );

			arr[ idx ].m_x = pos.m_x + XMG_F2X ( frad * kdCosf ( phi1 ) * kdCosf ( the1 ) );
			arr[ idx ].m_y = pos.m_y + XMG_F2X ( frad * kdSinf ( the1 ) );
			arr[ idx ].m_z = pos.m_z + XMG_F2X ( frad * kdSinf ( phi1 ) * kdCosf ( the1 ) );
 
			idx++;

			arr[ idx ].m_x = pos.m_x + XMG_F2X ( frad * kdCosf ( phi1 ) * kdCosf ( the2 ) );
			arr[ idx ].m_y = pos.m_y + XMG_F2X ( frad * kdSinf ( the2 ) );
			arr[ idx ].m_z = pos.m_z + XMG_F2X ( frad * kdSinf ( phi1 ) * kdCosf ( the2 ) );

			idx++;

			arr[ idx ].m_x = pos.m_x + XMG_F2X ( frad * kdCosf ( phi2 ) * kdCosf ( the2 ) );
			arr[ idx ].m_y = pos.m_y + XMG_F2X ( frad * kdSinf ( the2 ) );
			arr[ idx ].m_z = pos.m_z + XMG_F2X ( frad * kdSinf ( phi2 ) * kdCosf ( the2 ) );

			idx++;

			arr[ idx ].m_x = pos.m_x + XMG_F2X ( frad * kdCosf ( phi1 ) * kdCosf ( the1 ) );
			arr[ idx ].m_y = pos.m_y + XMG_F2X ( frad * kdSinf ( the1 ) );
			arr[ idx ].m_z = pos.m_z + XMG_F2X ( frad * kdSinf ( phi1 ) * kdCosf ( the1 ) );

			idx++;

			arr[ idx ].m_x = pos.m_x + XMG_F2X ( frad * kdCosf ( phi2 ) * kdCosf ( the2 ) );
			arr[ idx ].m_y = pos.m_y + XMG_F2X ( frad * kdSinf ( the2 ) );
			arr[ idx ].m_z = pos.m_z + XMG_F2X ( frad * kdSinf ( phi2 ) * kdCosf ( the2 ) );

			idx++;

			arr[ idx ].m_x = pos.m_x + XMG_F2X ( frad * kdCosf ( phi2 ) * kdCosf ( the1 ) );
			arr[ idx ].m_y = pos.m_y + XMG_F2X ( frad * kdSinf ( the1 ) );
			arr[ idx ].m_z = pos.m_z + XMG_F2X ( frad * kdSinf ( phi2 ) * kdCosf ( the1 ) );		

			idx++;
		}
	}
}

void XMGSphereImpl::MakeCoordArray ( XMGVector2F* arr, const XMGRectF& off, const XMGTexture* texture, GLsizei stack, GLsizei slice )
{
	XMGTexture*	tex		= (XMGTexture *) texture;

	XMGVector2F	size;
	XMGVector2F	rate;

	GLfloat		lat		= -90.0f;
	GLfloat		lon		= 0;
	GLfloat		lat_inc = 180.0f / stack;
	GLfloat		lon_inc = 360.0f / slice;

	GLsizei		col     = 0;
	GLsizei		row     = 0;
	GLsizei		idx		= 0;
	GLfloat		phi1	= 0;
	GLfloat		phi2	= 0;
	GLfloat		the1	= 0;
	GLfloat		the2	= 0;

	tex->GetSize ( size );
	
	rate.m_x = kdFabsf ( off.m_w ) / size.m_x;
	rate.m_y = kdFabsf ( off.m_h ) / size.m_y;
	
	for ( col = 0; col < stack; col++ )
	{
		phi1 = XMG_DEG2RAD ( lon + col * lon_inc );
		phi2 = XMG_DEG2RAD ( lon + ( col + 1 ) * lon_inc );

		if ( off.m_w < 0 )
		{
			phi1 = -phi1;
			phi2 = -phi2;
		}

		for ( row = 0; row < slice; row++ )
		{
			the1 = XMG_DEG2RAD ( lat + row * lat_inc );
			the2 = XMG_DEG2RAD ( lat + ( row + 1 ) * lat_inc );

			if ( off.m_h < 0 )
			{
				the1 = -the1;
				the2 = -the2;
			}

			arr[ idx ].m_x = off.m_x + ( 180.0f - XMG_RAD2DEG ( phi1 )  ) / 360.0f * rate.m_x;				
			arr[ idx ].m_y = off.m_y + XMG_RAD2DEG ( the1 + KD_PI_2_F ) / 180.0f * rate.m_y;	

			idx++;

			arr[ idx ].m_x = off.m_x + ( 180.0f - XMG_RAD2DEG ( phi1 )  ) / 360.0f * rate.m_x;	
			arr[ idx ].m_y = off.m_y + XMG_RAD2DEG ( the2 + KD_PI_2_F ) / 180.0f * rate.m_y;	

			idx++;

			arr[ idx ].m_x = off.m_x + ( 180.0f - XMG_RAD2DEG ( phi2 )  ) / 360.0f * rate.m_x;	
			arr[ idx ].m_y = off.m_y + XMG_RAD2DEG ( the2 + KD_PI_2_F ) / 180.0f * rate.m_y;	

			idx++;

			arr[ idx ].m_x = off.m_x + ( 180.0f - XMG_RAD2DEG ( phi1 )  ) / 360.0f * rate.m_x;	
			arr[ idx ].m_y = off.m_y + XMG_RAD2DEG ( the1 + KD_PI_2_F ) / 180.0f * rate.m_y;	
		
			idx++;

			arr[ idx ].m_x = off.m_x + ( 180.0f - XMG_RAD2DEG ( phi2 )  ) / 360.0f * rate.m_x;
			arr[ idx ].m_y = off.m_y + XMG_RAD2DEG ( the2 + KD_PI_2_F ) / 180.0f * rate.m_y;

			idx++;

			arr[ idx ].m_x = off.m_x + ( 180.0f - XMG_RAD2DEG ( phi2 )  ) / 360.0f * rate.m_x;
			arr[ idx ].m_y = off.m_y + XMG_RAD2DEG ( the1 + KD_PI_2_F ) / 180.0f * rate.m_y;

			idx++;
		}
	}	
}

void XMGSphereImpl::MakeCoordArray ( XMGVector2X* arr, const XMGRectX& off, const XMGTexture* texture, GLsizei stack, GLsizei slice )
{
	XMGTexture*	tex		= (XMGTexture *) texture;

	XMGVector2F	size;
	XMGVector2F	rate;

	GLfloat		lat		= -90.0f;
	GLfloat		lon		= 0;
	GLfloat		lat_inc = 180.0f / stack;
	GLfloat		lon_inc = 360.0f / slice;

	GLsizei		col     = 0;
	GLsizei		row     = 0;
	GLsizei		idx		= 0;
	GLfloat		phi1	= 0;
	GLfloat		phi2	= 0;
	GLfloat		the1	= 0;
	GLfloat		the2	= 0;

	tex->GetSize ( size );

	rate.m_x = kdFabsf ( XMG_X2F ( off.m_w ) ) / size.m_x;
	rate.m_y = kdFabsf ( XMG_X2F ( off.m_h ) ) / size.m_y;
	
	for ( col = 0; col < stack; col++ )
	{
		phi1 = XMG_DEG2RAD ( lon + col * lon_inc );
		phi2 = XMG_DEG2RAD ( lon + ( col + 1 ) * lon_inc );

		if ( off.m_w < 0 )
		{
			phi1 = -phi1;
			phi2 = -phi2;
		}

		for ( row = 0; row < slice; row++ )
		{
			the1 = XMG_DEG2RAD ( lat + row * lat_inc );
			the2 = XMG_DEG2RAD ( lat + ( row + 1 ) * lat_inc );

			if ( off.m_h < 0 )
			{
				the1 = -the1;
				the2 = -the2;
			}

			arr[ idx ].m_x = off.m_x + XMG_F2X ( ( 180.0f - XMG_RAD2DEG ( phi1 )  ) / 360.0f * rate.m_x );				
			arr[ idx ].m_y = off.m_y + XMG_F2X ( XMG_RAD2DEG ( the1 + KD_PI_2_F ) / 180.0f * rate.m_y );	

			idx++;

			arr[ idx ].m_x = off.m_x + XMG_F2X ( ( 180.0f - XMG_RAD2DEG ( phi1 )  ) / 360.0f * rate.m_x );	
			arr[ idx ].m_y = off.m_y + XMG_F2X ( XMG_RAD2DEG ( the2 + KD_PI_2_F ) / 180.0f * rate.m_y );	

			idx++;

			arr[ idx ].m_x = off.m_x + XMG_F2X ( ( 180.0f - XMG_RAD2DEG ( phi2 )  ) / 360.0f * rate.m_x );	
			arr[ idx ].m_y = off.m_y + XMG_F2X ( XMG_RAD2DEG ( the2 + KD_PI_2_F ) / 180.0f * rate.m_y );	

			idx++;

			arr[ idx ].m_x = off.m_x + XMG_F2X ( ( 180.0f - XMG_RAD2DEG ( phi1 )  ) / 360.0f * rate.m_x );	
			arr[ idx ].m_y = off.m_y + XMG_F2X ( XMG_RAD2DEG ( the1 + KD_PI_2_F ) / 180.0f * rate.m_y );	
		
			idx++;

			arr[ idx ].m_x = off.m_x + XMG_F2X ( ( 180.0f - XMG_RAD2DEG ( phi2 )  ) / 360.0f * rate.m_x );
			arr[ idx ].m_y = off.m_y + XMG_F2X ( XMG_RAD2DEG ( the2 + KD_PI_2_F ) / 180.0f * rate.m_y );

			idx++;

			arr[ idx ].m_x = off.m_x + XMG_F2X ( ( 180.0f - XMG_RAD2DEG ( phi2 )  ) / 360.0f * rate.m_x );
			arr[ idx ].m_y = off.m_y + XMG_F2X ( XMG_RAD2DEG ( the1 + KD_PI_2_F ) / 180.0f * rate.m_y );

			idx++;
		}
	}	
}

XMGSphere::XMGSphere ( GLuint num_shape, GLsizei slices, GLsizei stacks ) : XMGRender ( num_shape, slices * stacks * 3 * 2 )
{	
	XMGAssert ( m_impl = new XMGSphereImpl ( slices, stacks ) );	
}

XMGSphere::~XMGSphere ( void )
{
	delete (XMGSphereImpl *) m_impl;
}

void XMGSphere::SetVertexArray ( const GLfloat* radius, const XMGVector3F* pos, GLuint src_idx_shape )
{
	XMGSphereImpl*	impl = (XMGSphereImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;
	GLuint			offset;

	XMGVector3F*	arr_vertex;
	
	XMGAssert ( arr_vertex = (XMGVector3F *) kdMalloc ( sizeof ( XMGVector3F ) * impl->m_slices * impl->m_stacks * 3 * 2 ) );

	offset = 0;
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		XMGSphereImpl::MakeVertexArray ( arr_vertex, pos == 0 ? XMGVector3F ( ) : pos[ offset ], radius[ offset ], impl->m_slices, impl->m_stacks );
		XMGRender::SetVertexArray ( arr_vertex, idx_shape );
		offset++;
	}

	kdFree ( arr_vertex );
}

void XMGSphere::SetVertexArray ( const GLfixed* radius, const XMGVector3X* pos, GLuint src_idx_shape )
{
	XMGSphereImpl*	impl = (XMGSphereImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;
	GLuint			offset;

	XMGVector3X*	arr_vertex;
	
	XMGAssert ( arr_vertex = (XMGVector3X *) kdMalloc ( sizeof ( XMGVector3X ) * impl->m_slices * impl->m_stacks * 3 * 2 ) );

	offset = 0;
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		XMGSphereImpl::MakeVertexArray ( arr_vertex, pos == 0 ? XMGVector3X ( ) : pos[ offset ], radius[ offset ], impl->m_slices, impl->m_stacks );
		XMGRender::SetVertexArray ( arr_vertex, idx_shape );
		offset++;
	}

	kdFree ( arr_vertex );
}

void XMGSphere::SetTexture ( const XMGTexture* texture, XMGFlip flip, XMGTexUnit unit, GLuint src_idx_shape )
{
	XMGTexture*		tex = (XMGTexture *) texture;
	XMGVector2X		size;

	tex->GetImageSize ( size );

	if ( flip & XMG_FLIP_X ) { size.m_x = -size.m_x; }
	if ( flip & XMG_FLIP_Y ) { size.m_y = -size.m_y; }

	SetTexture ( texture, XMGRectX ( 0, 0, size.m_x, size.m_y ), unit, src_idx_shape );	
}

void XMGSphere::SetTexture ( const XMGTexture* texture, const XMGRectF& offset, XMGTexUnit unit, GLuint src_idx_shape )
{
	XMGSphereImpl*	impl = (XMGSphereImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	XMGVector2F*	arr_coord;
	
	XMGAssert ( arr_coord = (XMGVector2F *) kdMalloc ( sizeof ( XMGVector2F ) * impl->m_slices * impl->m_stacks * 3 * 2 ) );

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		XMGSphereImpl::MakeCoordArray ( arr_coord, offset, texture, impl->m_slices, impl->m_stacks );
		XMGRender::SetCoordArray ( arr_coord, unit, idx_shape );
		XMGRender::SetTexture ( texture, unit, idx_shape );
		XMGRender::SetDispMode ( XMG_DISP_TEXTURE, idx_shape );
	}

	kdFree ( arr_coord );
}

void XMGSphere::SetTexture ( const XMGTexture* texture, const XMGRectX& offset, XMGTexUnit unit, GLuint src_idx_shape )
{
	XMGSphereImpl*	impl = (XMGSphereImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	XMGVector2X*	arr_coord;
	
	XMGAssert ( arr_coord = (XMGVector2X *) kdMalloc ( sizeof ( XMGVector2X ) * impl->m_slices * impl->m_stacks * 3 * 2 ) );

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		XMGSphereImpl::MakeCoordArray ( arr_coord, offset, texture, impl->m_slices, impl->m_stacks );
		XMGRender::SetCoordArray ( arr_coord, unit, idx_shape );
		XMGRender::SetCoordArray ( arr_coord, unit, idx_shape );
		XMGRender::SetTexture ( texture, unit, idx_shape );
		XMGRender::SetDispMode ( XMG_DISP_TEXTURE, idx_shape );
	}

	kdFree ( arr_coord );
}
