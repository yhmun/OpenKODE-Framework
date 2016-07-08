/*
 *
 *      File            XMGView.cpp
 *      Description     View implemation 
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

XMGViewImpl::XMGViewImpl (void ) 
{
	m_type_proj = XMG_TYPE_FIXED;
	m_type_look = XMG_TYPE_FIXED;
}

XMGViewImpl::~XMGViewImpl ( void ) 
{

}

void XMGViewImpl::UpdateLookF ( void )
{
	XMGVector3F*	eye  = (XMGVector3F *) &m_eye;
	XMGVector3F*	at   = (XMGVector3F *) &m_at;
	XMGVector3F*	up   = (XMGVector3F *) &m_up;
	XMGVector3F*	side = (XMGVector3F *) &m_side;
	XMGMatrix4F*	look = (XMGMatrix4F *) &m_look;
		
	*at   = at->Normalize ( );

	*side = at->Cross ( *up );
	*side = side->Normalize ( );

	*up   = side->Cross ( *at );
	*up   = up->Normalize ( );

	look->m_m[ 0] = side->m_x;	look->m_m[ 4] = side->m_y;	look->m_m[ 8] = side->m_z;	look->m_m[12] = -side->Dot ( *eye );
	look->m_m[ 1] =   up->m_x;	look->m_m[ 5] =   up->m_y;	look->m_m[ 9] =   up->m_z;	look->m_m[13] =   -up->Dot ( *eye );
	look->m_m[ 2] =  -at->m_x;	look->m_m[ 6] =  -at->m_y;	look->m_m[10] =  -at->m_z;	look->m_m[14] =    at->Dot ( *eye );
	look->m_m[ 3] =         0;	look->m_m[ 7] =          0;	look->m_m[11] =         0;	look->m_m[15] =                1.0f;	
}

void XMGViewImpl::UpdateLookX ( void )
{
	XMGVector3X*	eye  = (XMGVector3X *) &m_eye;
	XMGVector3X*	at   = (XMGVector3X *) &m_at;
	XMGVector3X*	up   = (XMGVector3X *) &m_up;
	XMGVector3X*	side = (XMGVector3X *) &m_side;
	XMGMatrix4X*	look = (XMGMatrix4X *) &m_look;
		
	*at   = at->Normalize ( );

	*side = at->Cross ( *up );
	*side = side->Normalize ( );

	*up   = side->Cross ( *at );
	*up   = up->Normalize ( );

	look->m_m[ 0] = side->m_x;	look->m_m[ 4] = side->m_y;	look->m_m[ 8] = side->m_z;	look->m_m[12] = -side->Dot ( *eye );
	look->m_m[ 1] =   up->m_x;	look->m_m[ 5] =   up->m_y;	look->m_m[ 9] =   up->m_z;	look->m_m[13] =   -up->Dot ( *eye );
	look->m_m[ 2] =  -at->m_x;	look->m_m[ 6] =  -at->m_y;	look->m_m[10] =  -at->m_z;	look->m_m[14] =    at->Dot ( *eye );
	look->m_m[ 3] =         0;	look->m_m[ 7] =          0;	look->m_m[11] =         0;	look->m_m[15] =               65536;
}

XMGView::XMGView ( void )
{
	XMGAssert ( m_impl = new XMGViewImpl ( ) );

	SetLookAt ( XMGVector3X ( 0, 0, 0 ), XMGVector3X ( 0, 0, -65536 ), XMGVector3X ( 0, 65536, 0 ) );
}

XMGView::~XMGView ( void )
{
	delete (XMGViewImpl *) m_impl;	
}

void XMGView::Update ( void )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;

	glViewport ( impl->m_viewport.m_x, impl->m_viewport.m_y, impl->m_viewport.m_w, impl->m_viewport.m_h );
	glScissor  ( impl->m_viewport.m_x, impl->m_viewport.m_y, impl->m_viewport.m_w, impl->m_viewport.m_h );

	glMatrixMode  ( GL_PROJECTION );

	switch ( impl->m_type_proj )
	{
		case XMG_TYPE_FLOAT :	glLoadMatrixf ( (GLfloat *) &impl->m_proj ); break;
		case XMG_TYPE_FIXED :	glLoadMatrixx ( (GLfixed *) &impl->m_proj ); break;
	}

	glMatrixMode  ( GL_MODELVIEW );

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FLOAT :	glLoadMatrixf ( (GLfloat *) &impl->m_look ); break;
		case XMG_TYPE_FIXED :	glLoadMatrixx ( (GLfixed *) &impl->m_look ); break;
	}
}

void XMGView::SetViewport ( const XMGRectF& rect )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;

	impl->m_viewport.m_x = XMG_F2I ( rect.m_x );
	impl->m_viewport.m_y = XMG_F2I ( rect.m_y );
	impl->m_viewport.m_w = XMG_F2I ( rect.m_w );
	impl->m_viewport.m_h = XMG_F2I ( rect.m_h );
}

void XMGView::SetViewport ( const XMGRectX& rect )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;

	impl->m_viewport.m_x = XMG_X2I ( rect.m_x );
	impl->m_viewport.m_y = XMG_X2I ( rect.m_y );
	impl->m_viewport.m_w = XMG_X2I ( rect.m_w );
	impl->m_viewport.m_h = XMG_X2I ( rect.m_h );
}

void XMGView::SetProject ( const XMGMatrix4F& mat )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;

	impl->m_type_proj = XMG_TYPE_FLOAT;
	kdMemcpy ( &impl->m_proj, &mat, sizeof ( XMGMatrix4F ) );	
}

void XMGView::SetProject ( const XMGMatrix4X& mat )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;

	impl->m_type_proj = XMG_TYPE_FIXED;
	kdMemcpy ( &impl->m_proj, &mat, sizeof ( XMGMatrix4X ) );	
}

void XMGView::SetLookAt ( const XMGVector3F& eye, const XMGVector3F& at, const XMGVector3F& up )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;

	impl->m_type_look = XMG_TYPE_FLOAT;

	kdMemcpy ( &impl->m_eye, &eye, sizeof ( XMGVector3F ) );
	kdMemcpy ( &impl->m_at , &at , sizeof ( XMGVector3F ) );
	kdMemcpy ( &impl->m_up , &up , sizeof ( XMGVector3F ) );

	impl->UpdateLookF ( );
}

void XMGView::SetLookAt ( const XMGVector3X& eye, const XMGVector3X& at, const XMGVector3X& up )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;

	impl->m_type_look = XMG_TYPE_FIXED;

	kdMemcpy ( &impl->m_eye, &eye, sizeof ( XMGVector3X ) );
	kdMemcpy ( &impl->m_at , &at , sizeof ( XMGVector3X ) );
	kdMemcpy ( &impl->m_up , &up , sizeof ( XMGVector3X ) );

	impl->UpdateLookX ( );
}

void XMGView::Walk ( GLfloat val )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3F*	eye  = 0;
	XMGVector3F*	at   = 0;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FLOAT :
			
			 eye = (XMGVector3F *) &impl->m_eye;
			 at  = (XMGVector3F *) &impl->m_at;
			*eye = *eye + ( *at * val );

			impl->UpdateLookF ( );

			break;

		case XMG_TYPE_FIXED : 
			
			Walk ( XMG_F2X ( val ) );
			
			break;
	}
}

void XMGView::Walk ( GLfixed val )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3X*	eye  = 0;
	XMGVector3X*	at   = 0;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FIXED :
			
			 eye  = (XMGVector3X *) &impl->m_eye;
			 at   = (XMGVector3X *) &impl->m_at;
			*eye  = *eye + ( *at * val );

			impl->UpdateLookX ( );

			break;

		case XMG_TYPE_FLOAT : 
			
			Walk ( XMG_X2F ( val ) );
			
			break;
	}
}

void XMGView::Fly ( GLfloat val )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3F*	eye  = 0;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FLOAT :
			
			eye = (XMGVector3F *) &impl->m_eye;
			eye->m_y += val;

			impl->UpdateLookF ( );

			break;

		case XMG_TYPE_FIXED : 
			
			Fly ( XMG_F2X ( val ) );
			
			break;
	}
}

void XMGView::Fly ( GLfixed val )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3X*	eye  = 0;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FIXED :
			
			eye = (XMGVector3X *) &impl->m_eye;
			eye->m_y += val;

			impl->UpdateLookX ( );

			break;

		case XMG_TYPE_FLOAT : 
			
			Fly ( XMG_X2F ( val ) );
			
			break;
	}
}

void XMGView::Strafe ( GLfloat val )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3F*	eye  = 0;
	XMGVector3F*	at   = 0;
	XMGVector3F*	up   = 0;
	XMGVector3F*	side = 0;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FLOAT :
			
			 eye  = (XMGVector3F *) &impl->m_eye;
			 at   = (XMGVector3F *) &impl->m_at;
			 up   = (XMGVector3F *) &impl->m_up;
			 side = (XMGVector3F *) &impl->m_side;
			*eye  = *eye + ( *side * val );

			impl->UpdateLookF ( );

			break;

		case XMG_TYPE_FIXED : 
			
			Strafe ( XMG_F2X ( val ) );
			
			break;
	}
}

void XMGView::Strafe ( GLfixed val )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3X*	eye  = 0;
	XMGVector3X*	at   = 0;
	XMGVector3X*	up   = 0;
	XMGVector3X*	side = 0;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FIXED :
			
			 eye  = (XMGVector3X *) &impl->m_eye;
			 at   = (XMGVector3X *) &impl->m_at;
			 up   = (XMGVector3X *) &impl->m_up;
			 side = (XMGVector3X *) &impl->m_side;
			*eye  = *eye + ( *side * val );

			impl->UpdateLookX ( );

			break;

		case XMG_TYPE_FLOAT : 
			
			Strafe ( XMG_X2F ( val ) );
			
			break;
	}
}

void XMGView::Tilt ( GLfloat val )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3F*	at   = 0;
	XMGVector3F*	up   = 0;
	XMGVector3F*	side = 0;
	XMGMatrix4F		mat;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FLOAT :
			
			at   = (XMGVector3F *) &impl->m_at;
			up   = (XMGVector3F *) &impl->m_up;
			side = (XMGVector3F *) &impl->m_side;

			mat.Rotate ( val, side->m_x, side->m_y, side->m_z );

			*at = mat.Transform ( *at );
			*up = mat.Transform ( *up );
	
			impl->UpdateLookF ( );

			break;

		case XMG_TYPE_FIXED : 
			
			Tilt ( XMG_F2X ( val ) );
			
			break;
	}
}

void XMGView::Tilt ( GLfixed val )
{
	XMGViewImpl*	impl  = (XMGViewImpl *) m_impl;
	XMGVector3X*	xat   = 0;
	XMGVector3X*	xup   = 0;
	XMGVector3X*	xside = 0;
	XMGMatrix4X		mat;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FIXED :
			
			xat   = (XMGVector3X *) &impl->m_at;
			xup   = (XMGVector3X *) &impl->m_up;
			xside = (XMGVector3X *) &impl->m_side;

			mat.Rotate ( val, xside->m_x, xside->m_y, xside->m_z );

			*xat = mat.Transform ( *xat );
			*xup = mat.Transform ( *xup );

			impl->UpdateLookX ( );

			break;

		case XMG_TYPE_FLOAT : 
			
			Tilt ( XMG_X2F ( val ) );
			
			break;
	}
}

void XMGView::RotateX ( GLfloat val )
{
	Rotate ( val, XMGVector3F ( 1.0f, 0.0f, 0.0f ) );
}

void XMGView::RotateX ( GLfixed val )
{
	Rotate ( val, XMGVector3X ( 65536, 0, 0 ) );
}

void XMGView::RotateY ( GLfloat val )
{
	Rotate ( val, XMGVector3F ( 0.0f, 1.0f, 0.0f ) );
}

void XMGView::RotateY ( GLfixed val )
{
	Rotate ( val, XMGVector3X ( 0, 65536, 0 ) );
}

void XMGView::RotateZ ( GLfloat val )
{
	Rotate ( val, XMGVector3F ( 0.0f, 0.0f, 1.0f ) );
}

void XMGView::RotateZ ( GLfixed val )
{
	Rotate ( val, XMGVector3X ( 0, 0, 65536 ) );
}

void XMGView::Rotate ( GLfloat val, const XMGVector3F& vec )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3F*	fat  = 0;
	XMGVector3F*	fup  = 0;
	XMGMatrix4F		mat;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FLOAT :
			
			fat = (XMGVector3F *) &impl->m_at;
			fup = (XMGVector3F *) &impl->m_up;

			mat.Rotate ( val, vec.m_x, vec.m_y, vec.m_z );

			*fat = mat.Transform ( *fat );
			*fup = mat.Transform ( *fup );
	
			impl->UpdateLookF ( );

			break;

		case XMG_TYPE_FIXED : 
			
			Rotate ( XMG_F2X ( val ), XMGVector3X ( vec ) );
			
			break;
	}
}

void XMGView::Rotate ( GLfixed val, const XMGVector3X& vec )
{
	XMGViewImpl*	impl = (XMGViewImpl *) m_impl;
	XMGVector3X*	xat  = 0;
	XMGVector3X*	xup  = 0;
	XMGMatrix4X		mat;

	switch ( impl->m_type_look )
	{
		case XMG_TYPE_FIXED :
			
			xat = (XMGVector3X *) &impl->m_at;
			xup = (XMGVector3X *) &impl->m_up;

			mat.Rotate ( val, vec.m_x, vec.m_y, vec.m_z );

			*xat = mat.Transform ( *xat );
			*xup = mat.Transform ( *xup );

			impl->UpdateLookX ( );

			break;

		case XMG_TYPE_FLOAT : 
			
			Rotate ( XMG_X2F ( val ),  XMGVector3F ( vec )  );
			
			break;
	}
}