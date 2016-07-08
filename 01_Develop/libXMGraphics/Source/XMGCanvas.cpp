/*
 *
 *      File            XMGCanvas.cpp
 *      Description     Canvas implemation
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

XMGCanvasImpl::XMGCanvasImpl ( void ) 
{
	m_ratio  = 0;
	m_width  = 0;
	m_height = 0;
}

XMGCanvasImpl::~XMGCanvasImpl ( void ) 
{

}

XMGCanvas::XMGCanvas ( void ) 
{
	XMGAssert ( m_impl = new XMGCanvasImpl ( ) );
	
	glClearColorx ( 0, 0, XMG_F2X ( 0.25f ) , 65536 );

	glPixelStorei ( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );	
    
	glDepthFunc ( GL_LEQUAL );		
	glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );	
	
	glTexEnvx ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );	
}

XMGCanvas::~XMGCanvas ( void )
{	
	delete (XMGCanvasImpl *) m_impl;
}

void XMGCanvas::Clear ( void )
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	
}

void XMGCanvas::Flush ( void )
{
	glFlush ( );
}

void XMGCanvas::ClearColor ( const XMGColorF& color )
{
	glClearColor ( color.m_r , color.m_g, color.m_b, color.m_a );
}

void XMGCanvas::ClearColor ( const XMGColorX& color )
{
	glClearColorx ( color.m_r , color.m_g, color.m_b, color.m_a );
}

void XMGCanvas::LoadIdentity ( void )
{
	XMGCanvasImpl*	impl = (XMGCanvasImpl *) m_impl;

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );

	if ( impl->m_ratio )
	{
		glTranslatex ( 0, 0, -655360 );
		glScalex ( impl->m_ratio, impl->m_ratio, impl->m_ratio );
		glTranslatex ( -impl->m_width / 2, -impl->m_height / 2, 0 );
	}
}

void XMGCanvas::PushMatrix ( void )
{
	glPushMatrix ( );
}

void XMGCanvas::PopMatrix  ( void )
{
	glPopMatrix ( );
}

void XMGCanvas::MulMatrix ( const XMGMatrix4F& mat )
{
	glMultMatrixf ( (GLfloat *) &mat );	
}

void XMGCanvas::MulMatrix ( const XMGMatrix4X& mat )
{
	glMultMatrixx ( (GLfixed *) &mat);	
}

void XMGCanvas::Translate ( const XMGVector3F& vec )
{
	glTranslatef ( vec.m_x, vec.m_y, vec.m_z );
}

void XMGCanvas::Translate ( const XMGVector3X& vec )
{
	glTranslatex ( vec.m_x, vec.m_y, vec.m_z );
}

void XMGCanvas::Rotate ( GLfloat angle, const XMGVector3F& vec )
{
	glRotatef ( angle, vec.m_x, vec.m_y, vec.m_z );
}

void XMGCanvas::Rotate ( GLfixed angle, const XMGVector3X& vec )
{
	glRotatex ( angle, vec.m_x, vec.m_y, vec.m_z );
}

void XMGCanvas::Scale ( const XMGVector3F& vec )
{
	glScalef ( vec.m_x, vec.m_y, vec.m_z );
}

void XMGCanvas::Scale ( const XMGVector3X& vec )
{
	glScalex ( vec.m_x, vec.m_y, vec.m_z );
}

void  XMGCanvas::Viewport ( const XMGRectF& rect )
{
	GLint		x = XMG_F2I ( rect.m_x );
	GLint		y = XMG_F2I ( rect.m_y );
	GLsizei		w = XMG_F2I ( rect.m_w );
	GLsizei		h = XMG_F2I ( rect.m_h );

	glEnable ( GL_SCISSOR_TEST );
	
	glViewport ( x, y, w, h );
	glScissor  ( x, y, w, h );
}

void  XMGCanvas::Viewport ( const XMGRectX& rect )
{
	GLint		x = XMG_X2I ( rect.m_x );
	GLint		y = XMG_X2I ( rect.m_y );
	GLsizei		w = XMG_X2I ( rect.m_w );
	GLsizei		h = XMG_X2I ( rect.m_h );

	glEnable ( GL_SCISSOR_TEST );
	
	glViewport ( x, y, w, h );
	glScissor  ( x, y, w, h );
}

void XMGCanvas::Ortho ( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar )
{
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );

	glOrthof ( left, right, bottom, top, znear, zfar );

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ( );
}

void XMGCanvas::Ortho ( GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed znear, GLfixed zfar )
{
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );

	glOrthox ( left, right, bottom, top, znear, zfar );

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ( );
}

void XMGCanvas::Perspective ( GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar )
{
	GLfloat		h  = 2.0f * znear * kdTanf ( XMG_DEG2RAD ( fovy ) * 0.5f );
	GLfloat		w  = aspect * h;
	GLfloat		h2 = w * 0.5f;
	GLfloat		w2 = h * 0.5f;

	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );

	glFrustumf ( -w2, w2, -h2, h2, znear, zfar );

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
}

void XMGCanvas::Perspective ( GLfixed fovy, GLfixed aspect, GLfixed znear, GLfixed zfar )
{
	GLfloat		fh  = 2.0f * XMG_X2F ( znear ) * kdTanf ( XMG_DEG2RAD ( XMG_X2F ( fovy ) ) * 0.5f );	
	GLfloat		fw  = XMG_X2F ( aspect ) * fh;
	GLfixed		xh2 = XMG_F2X ( fh * 0.5f );
	GLfixed		xw2 = XMG_F2X ( fw * 0.5f );

	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );

	glFrustumx ( -xw2, xw2, -xh2, xh2, znear, zfar );

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
}

void XMGCanvas::Perspective ( GLuint width, GLuint height )
{
	XMGCanvasImpl*	impl = (XMGCanvasImpl *) m_impl;

	XMGMatrix4X		m;

	GLfloat			w = XMG_I2F ( width );
	GLfloat			h = XMG_I2F ( height );

	m.Perspective ( XMG_I2X ( 45 ), XMG_F2X ( w / h ), XMG_I2X ( 1 ), XMG_I2X ( 100 ) );

	glMatrixMode ( GL_PROJECTION );
	glLoadMatrixx ( (GLfixed *) &m );

	m.Translate ( 0, 0, -655360 );

	impl->m_ratio  = XMG_F2X ( ( 2 * XMG_X2F ( m.m_m[15] ) )  / ( XMG_X2F ( m.m_m[0] ) * width ) );
	impl->m_width  = XMG_I2X ( width );
	impl->m_height = XMG_I2X ( height ); 

	LoadIdentity ( );
}