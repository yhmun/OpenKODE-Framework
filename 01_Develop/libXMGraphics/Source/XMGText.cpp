/*
 *
 *      File            XMGText.cpp
 *      Description     Text implemation 
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

#if defined( _WIN32 ) && !defined( SHP )
#include "platform.h"
#endif

XMGTextImpl::XMGTextImpl ( GLuint num_shape )
{			
	XMGAssert ( m_data = new XMGTextData[ num_shape ] );
}

XMGTextImpl::~XMGTextImpl ( void )
{
	delete[] m_data;
}

XMGText::XMGText ( GLuint num_shape ) : XMGRender ( num_shape, (GLuint) 0 )
{
	XMGAssert ( m_impl = new XMGTextImpl ( num_shape ) );

	XMGRender::SetColor ( XMGColorX ( XMG_F2X ( 0.4f ), XMG_F2X ( 0.8f ), XMG_F2X ( 0.0f ), XMG_F2X ( 1.0f ) ) );
}

XMGText::~XMGText ( void )
{	
	delete (XMGTextImpl *) m_impl;	
}

void XMGText::Render ( GLuint idx_shape )
{
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	XMGShape*		shape;
	XMGFacet*		facet;

	GLuint			num_shape;

	impl_parent->Begin ( );

	for ( impl_parent->RangeArray ( idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		shape = impl_parent->m_vec_shape[ idx_shape ];
		facet = shape->m_vec_facet[ 0 ];

		if ( facet->m_visible && ( impl->m_data[ idx_shape ].m_wstr || impl->m_data[ idx_shape ].m_mstr ) )
		{
			impl_parent->DisplayBegin ( idx_shape );
			impl_parent->Color ( idx_shape, 0 );

			ftglInitImmediateModeGL ( );

			if ( impl->m_data[ idx_shape ].m_wstr )
			{
				impl->m_data[ idx_shape ].m_layout->Render ( impl->m_data[ idx_shape ].m_wstr, -1, FTPoint(), impl->m_data[ idx_shape ].m_face );
			}
			else if ( impl->m_data[ idx_shape ].m_mstr )
			{
				impl->m_data[ idx_shape ].m_layout->Render ( impl->m_data[ idx_shape ].m_mstr, -1, FTPoint(), impl->m_data[ idx_shape ].m_face );
			}

			impl_parent->DisplayEnd ( idx_shape );
		}
	}

	impl_parent->End ( );
}

void XMGText::SetCull ( XMGCull cull, GLuint src_idx_shape )
{	
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		switch ( cull )
		{
			case XMG_CULL_DISABLE	:	impl->m_data[ idx_shape ].m_face = FTGL::RENDER_ALL;						break;
			case XMG_CULL_BACK		:	impl->m_data[ idx_shape ].m_face = FTGL::RENDER_FRONT | FTGL::RENDER_SIDE;	break;
			case XMG_CULL_FRONT		:	impl->m_data[ idx_shape ].m_face = FTGL::RENDER_BACK  | FTGL::RENDER_SIDE;	break;
		}
	}

	XMGRender::SetCull ( cull, src_idx_shape );	
}

void XMGText::SetFont ( XMGFont* font, GLuint src_idx_shape )
{
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;
	XMGFontImpl*	impl_font = (XMGFontImpl *) font->m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		impl->m_data[ idx_shape ].m_layout->SetFont ( impl_font->m_font );
	}
}

void XMGText::SetText ( const KDchar* text, GLuint src_idx_shape )
{
	#if defined( _WIN32 ) && !defined( SHP )

	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;
	KDsize			length;

	length = kdStrlen ( text );
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		if ( impl->m_data[ idx_shape ].m_wstr )
		{
			kdFree ( impl->m_data[ idx_shape ].m_wstr );
		}		

		impl->m_data[ idx_shape ].m_wstr = (wchar_t *) kdMalloc ( sizeof( wchar_t ) * ( length + 1 ) );
		MultiByteToWideChar ( CP_ACP, 0, text, -1, impl->m_data[ idx_shape ].m_wstr, length );
		kdMemset ( &impl->m_data[ idx_shape ].m_wstr[ length ], 0, sizeof( wchar_t ) );			
	}

	#else
	
	SetUTF8 ( text, src_idx_shape );
	
	#endif
}

void XMGText::SetUTF8 ( const KDchar* text, GLuint src_idx_shape )
{
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;
	KDsize			length;

	length = kdStrlen ( text );
	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		if ( impl->m_data[ idx_shape ].m_mstr )
		{
			kdFree ( impl->m_data[ idx_shape ].m_mstr );
		}		

		impl->m_data[ idx_shape ].m_mstr = (char *) kdMalloc ( length + 1 );
		kdMemcpy ( impl->m_data[ idx_shape ].m_mstr, text, length );
		impl->m_data[ idx_shape ].m_mstr[ length ] = 0;
	}
}

void XMGText::SetLineLength ( GLfloat length, GLuint src_idx_shape )
{
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;	
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		impl->m_data[ idx_shape ].m_layout->SetLineLength ( length );
	}
}

void XMGText::SetLineLength ( GLfixed length, GLuint src_idx_shape )
{
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;	
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		impl->m_data[ idx_shape ].m_layout->SetLineLength ( XMG_X2F ( length ) );
	}
}

void XMGText::SetLineSpace ( GLfloat space, GLuint src_idx_shape )
{
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;	
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		impl->m_data[ idx_shape ].m_layout->SetLineSpacing ( space );
	}
}

void XMGText::SetLineSpace ( GLfixed space, GLuint src_idx_shape )
{
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;	
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		impl->m_data[ idx_shape ].m_layout->SetLineSpacing ( XMG_X2F ( space ) );
	}
}

void XMGText::SetLineAlign ( XMGAlign align, GLuint src_idx_shape )
{	
	XMGTextImpl*	impl = (XMGTextImpl *) m_impl;	
	XMGRenderImpl*	impl_parent = (XMGRenderImpl *) XMGRender::m_impl;

	GLuint			idx_shape;
	GLuint			num_shape;

	for ( impl_parent->RangeArray ( src_idx_shape, impl_parent->m_vec_shape.size ( ), idx_shape, num_shape ); idx_shape < num_shape; idx_shape++ )
	{
		impl->m_data[ idx_shape ].m_layout->SetAlignment ( (FTGL::TextAlignment) align );
	}
}
