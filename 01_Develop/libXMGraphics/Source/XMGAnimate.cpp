/*
 *
 *      File            XMGAnimateImpl.cpp
 *      Description     Animation implemation
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

XMGAnimateImpl::XMGAnimateImpl (void ) 
{
	m_frame  = 0;
	m_size   = 0;
	m_count  = 0;
	m_mask   = 0;
}

XMGAnimateImpl::~XMGAnimateImpl ( void ) 
{
	if ( m_frame )
	{
		kdFree ( m_frame );
	}
}

void XMGAnimateImpl::Update ( KDust msec, GLuint* out_frame, XMGType out_type )
{
	GLfloat			interpolate = 0;

	GLfloat*		f_out;
	GLfixed*		x_out;

	GLuint			off;
	GLuint			off_pre;
	GLuint			off_post;

	GLuint			idx;	

	for ( idx = 0; idx < m_count; idx++ )
	{
		if ( msec <= m_frame[ idx * m_size ] )
		{
			break;
		}
	}
	
	if ( idx == 0 )
	{
		off_pre = off_post = 0;
	}
	else if ( idx == m_count )
	{
		off_pre = off_post = ( idx - 1 ) * m_size;
	}
	else
	{
		off_pre  = ( idx - 1 ) * m_size;
		off_post = idx * m_size;
		interpolate = ( msec - m_frame[ off_pre ] ) / ( m_frame[ off_post ] - m_frame[ off_pre ] );
	}

	switch ( out_type )
	{
		case XMG_TYPE_FLOAT :

			f_out = (GLfloat *) out_frame;

			for ( off = 1; off < m_size; off++ )
			{
				f_out[ off - 1 ] = m_frame[ off_pre + off ] + ( m_frame[ off_post + off ] - m_frame[ off_pre + off ] ) * interpolate;
			}

			break;

		case XMG_TYPE_FIXED :

			x_out = (GLfixed *) out_frame;

			for ( off = 1; off < m_size; off++ )
			{
				x_out[ off - 1 ] = XMG_F2X ( m_frame[ off_pre + off ] + ( m_frame[ off_post + off ] - m_frame[ off_pre + off ] ) * interpolate );
			}

			break;
	}
}

void XMGAnimateImpl::Update ( KDust msec, XMGMatrix4* out_matrix, XMGColor* out_color, XMGType out_type )
{
	GLfloat			out_frame[20];
	
	XMGColorF*		f_col = (XMGColorF* ) out_color;
	XMGMatrix4F*	f_mat = (XMGMatrix4F *) out_matrix;
	XMGVector3F		f_axis;

	GLuint			off = 0;

	if ( !out_matrix && !out_color )
	{
		return;
	}

	Update ( msec, (GLuint *) out_frame, XMG_TYPE_FLOAT );

	if ( f_mat )
	{
		f_mat->Identity ( );
	}

	if ( f_col )
	{
		*f_col = XMGColorF ( );
	}

	if ( m_mask & XMG_ANI_POSITION )
	{
		if ( f_mat )
		{
			f_mat->Translate ( out_frame[ off + 0 ], out_frame[ off + 1 ], out_frame[ off + 2 ] );
		}

		off += 3;
	}

	if ( m_mask & XMG_ANI_AXIS )
	{
		if ( f_mat )
		{
			f_axis.m_x = out_frame[ off + 0 ];
			f_axis.m_y = out_frame[ off + 1 ];
			f_axis.m_z = out_frame[ off + 2 ];
		}

		off += 3;
	}

	if ( m_mask & XMG_ANI_ROTATE )
	{
		if ( f_mat )
		{
			f_mat->Translate ( f_axis.m_x, f_axis.m_y, f_axis.m_z );
			f_mat->Rotate ( out_frame[ off + 0 ], out_frame[ off + 1 ], out_frame[ off + 2 ], out_frame[ off + 3 ] );
			f_mat->Translate ( -f_axis.m_x, -f_axis.m_y, -f_axis.m_z );
		}

		off += 4;
	}

	if ( m_mask & XMG_ANI_SCALE )
	{
		if ( f_mat )
		{
			f_mat->Scale ( out_frame[ off + 0 ], out_frame[ off + 1 ], out_frame[ off + 2 ] );
		}

		off += 3;
	}

	if ( m_mask & XMG_ANI_ALPHA )
	{
		if ( f_col )
		{
			f_col->m_a = out_frame [ off + 0 ];
		}

		off += 1;
	}

	if ( m_mask & XMG_ANI_RGB )
	{
		if ( f_col )
		{
			f_col->m_r = out_frame [ off + 0 ];
			f_col->m_g = out_frame [ off + 1 ];
			f_col->m_b = out_frame [ off + 2 ];
		}

		off += 3;
	}

	if ( out_type == XMG_TYPE_FIXED )
	{
		if ( f_mat )
		{
			*out_matrix = *f_mat;
		}

		if ( f_col )
		{
			*out_color = *f_col;
		}
	}
}

void XMGAnimateImpl::SetKeyFrame ( GLuint size, GLuint count, const GLuint* arr_frame, XMGType arr_type )
{
	GLuint		idx;
	GLuint		num;

	m_mask  = 0;
	m_size  = size + 1;
	m_count = count;	

	num = m_size * m_count;

	XMGAssert ( m_frame = (GLfloat *) kdMalloc ( sizeof ( GLfloat ) * num ) );

	if ( arr_type == XMG_TYPE_FIXED )
	{
		for ( idx = 0; idx < num; idx++ )
		{
			m_frame[ idx ] = XMG_X2F ( arr_frame[ idx ] );
		}
	}
	else
	{
		kdMemcpy  ( m_frame, arr_frame, sizeof ( GLfloat ) * num );
	}	
}

void XMGAnimateImpl::SetKeyFrameByMask ( GLbitfield mask, GLuint count, const GLuint* arr_frame, XMGType arr_type )
{
	GLuint		size = 0;
	
	if ( mask & XMG_ANI_POSITION )		size += 3;
	if ( mask & XMG_ANI_AXIS )			size += 3;
	if ( mask & XMG_ANI_ROTATE )		size += 4;
	if ( mask & XMG_ANI_SCALE )			size += 3;	
	if ( mask & XMG_ANI_ALPHA )			size += 1;
	if ( mask & XMG_ANI_RGB	)			size += 3;

	SetKeyFrame ( size, count, arr_frame, arr_type );

	m_mask = mask;
}

XMGAnimate::XMGAnimate ( void )
{
	XMGAssert ( m_impl = new XMGAnimateImpl ( ) );
}

XMGAnimate::~XMGAnimate ( void )
{
	delete (XMGAnimateImpl *) m_impl;	
}

void XMGAnimate::Update ( KDust msec, GLfloat* out_frame )
{
	XMGAnimateImpl*		impl = (XMGAnimateImpl *) m_impl;

	impl->Update ( msec, (GLuint *) out_frame, XMG_TYPE_FLOAT );
}

void XMGAnimate::Update ( KDust msec, GLfixed* out_frame )
{
	XMGAnimateImpl*		impl = (XMGAnimateImpl *) m_impl;

	impl->Update ( msec, (GLuint *) out_frame, XMG_TYPE_FIXED );
}

void XMGAnimate::Update ( KDust msec, XMGMatrix4F* out_matrix, XMGColorF* out_color )
{
	XMGAnimateImpl*		impl = (XMGAnimateImpl *) m_impl;

	impl->Update ( msec, (XMGMatrix4 *) out_matrix, (XMGColor *) out_color, XMG_TYPE_FLOAT );
}

void XMGAnimate::Update ( KDust msec, XMGMatrix4X* out_matrix, XMGColorX* out_color )
{
	XMGAnimateImpl*		impl = (XMGAnimateImpl *) m_impl;

	impl->Update ( msec, (XMGMatrix4 *) out_matrix, (XMGColor *) out_color, XMG_TYPE_FIXED );
}

void XMGAnimate::SetKeyFrame ( GLuint size, GLuint count, const GLfloat* arr_frame )
{
	XMGAnimateImpl*		impl = (XMGAnimateImpl *) m_impl;

	impl->SetKeyFrame ( size, count, (const GLuint *) arr_frame, XMG_TYPE_FLOAT );
}

void XMGAnimate::SetKeyFrame ( GLuint size, GLuint count, const GLfixed* arr_frame )
{
	XMGAnimateImpl*		impl = (XMGAnimateImpl *) m_impl;

	impl->SetKeyFrame ( size, count, (const GLuint *) arr_frame, XMG_TYPE_FIXED );
}

void XMGAnimate::SetKeyFrameByMask ( GLbitfield mask, GLuint count, const GLfloat* arr_frame )
{
	XMGAnimateImpl*		impl = (XMGAnimateImpl *) m_impl;

	impl->SetKeyFrameByMask ( mask, count, (const GLuint *) arr_frame, XMG_TYPE_FLOAT );
}

void XMGAnimate::SetKeyFrameByMask ( GLbitfield mask, GLuint count, const GLfixed* arr_frame )
{
	XMGAnimateImpl*		impl = (XMGAnimateImpl *) m_impl;

	impl->SetKeyFrameByMask ( mask, count, (const GLuint *) arr_frame, XMG_TYPE_FIXED );
}